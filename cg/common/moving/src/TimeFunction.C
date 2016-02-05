// ==========================================================================
//   TimeFunction: This class defines a function of time that can be used to rotate 
//    or translate a body
// ==========================================================================

#include "TimeFunction.h"
#include "PlotStuff.h"
#include "SplineMapping.h"
#include "display.h"
#include "MappingInformation.h"

// ============================================================================
/// \brief constructor. This class defines a function of time that can be used to rotate 
///    or translate a body
// ===========================================================================
TimeFunction::
TimeFunction()
{
  functionType=linearFunction;
  composeType=composeWillMultiply;
  a0=0.;
  a1=1.;

  b0=1.;
  f0=1.;
  t0=0.;

  rampStart=0.;     // value of ramp for t< rampStartTime
  rampEnd=1.;       // value of ramp for t > rampEndTime
  rampStartTime=0.; // ramp turns on at this time
  rampEndTime=1.;   
  rampOrder=3;      // how many derivatives are zero at the ends of the ramp
  
  // put user defined parameters here 
  ipar.redim(10);
  rpar.redim(10);

  preFunction=NULL;

  timeParameterizationScaleFactor=1.; // factor to scale time to the unit interval, used for the mappingFunction
  
}

// =============================================================================
/// \brief Copy constructor.
// =============================================================================
TimeFunction::
TimeFunction( const TimeFunction & tf, const CopyType ct /* = DEEP */ )
{
  *this=tf;
}

// ============================================================================
/// \brief destructor.
// ===========================================================================
TimeFunction::
~TimeFunction()
{
  if( preFunction!=NULL && preFunction->decrementReferenceCount()==0 ) 
  {
    delete preFunction; preFunction=NULL;
  }
}


// =============================================================================
/// \brief Equals operator. Set this object to be equal to another.
// =============================================================================
TimeFunction & TimeFunction::
operator =( const TimeFunction & tf)
{
  functionType=tf.functionType;
  composeType =tf.composeType;
  a0=tf.a0;
  a1=tf.a1;
  b0=tf.b0;
  f0=tf.f0;
  t0=tf.t0;

  rampStart    =tf.rampStart;
  rampEnd      =tf.rampEnd;
  rampStartTime=tf.rampStartTime;
  rampEndTime  =tf.rampEndTime;
  rampOrder    =tf.rampOrder;

  ipar=tf.ipar;
  rpar=tf.rpar;
  
  preFunction=NULL;
  if( tf.preFunction!=NULL )
  {
    if( preFunction==NULL )
    {
      preFunction =new TimeFunction;  preFunction->incrementReferenceCount();
    }
    // deep copy: 
    *preFunction=*tf.preFunction;
  }
  else if( preFunction!=NULL && preFunction->decrementReferenceCount()==0 )
  {
    delete preFunction; preFunction=NULL;
  }

  mapFunction=tf.mapFunction;
  timeParameterizationScaleFactor=tf.timeParameterizationScaleFactor;

  return *this;
}


// ==========================================================================
/// \brief Compose this TimeFunction with another which is applied first.
/// \param motion (input) : apply this TimeFunction before the current. (set to NULL for none)
/// \param ct (input) : specify whether to multiply or add when composing.
// ==========================================================================
int TimeFunction::
compose( TimeFunction *preFunc, const ComposeTypeEnum ct /* =composeWillMultiply */ )
{
  composeType=ct;
  preFunction=preFunc;
  preFunction->incrementReferenceCount();
  return 0;
}

// ============================================================================
/// \brief Set the coefficients of the linear function, f(t)=a0 + a1*t.
/// \param a0,a1 (input) parameters in the linear function
// ===========================================================================
int TimeFunction::
setLinearFunction( const real a0_, const real a1_ )
{
  functionType=linearFunction; 
  a0=a0_;
  a1=a1_;

  return 0;
}

// ============================================================================
/// \brief Set the coefficients of the ramp function.
/// \param rampStart (input) : value of ramp function for t<rampStartTime
/// \param rampEnd (input) : value o ramp function for t>rampEndTime
/// \param rampStartTime, rampEndTime (input): ramp time interval
/// \ramp rampOrder (input) : indicates how smoothly the ramp turns on and off and is equal to the number of
///        derivatives that are zero at the start and end of the ramp.
///
/// \details:
/// The ramp function transitions from one one value (rampStart) to a second value (rampEnd)
///       as t varies between rampTimeStart and rampTimeEnd: 
///           ramp(t)  =  rampStart for t<rampTimeStart
///           ramp(t)  =  smoothly varies between rampStart and rampEnd, for rampStartTime < t < rampEndTime,
///           ramp(t)  =  rampStart for t>rampTimeEnd.
///        The rampOrder indicates how smoothly the ramp turns on and off and is equal to the number of
///        derivatives that are zero at the start and end of the ramp.
/// 
// ===========================================================================
int TimeFunction::
setRampFunction( const real rampStart_, const real rampEnd_,
		 const real rampStartTime_, const real rampEndTime_,
		 const int rampOrder_ )
{
  functionType =rampFunction; 
  rampStart    =rampStart_;
  rampEnd      =rampEnd_;
  rampStartTime=rampStartTime_;
  rampEndTime  =rampEndTime_;
  rampOrder    =rampOrder_;

  return 0;
}

// ============================================================================
/// \brief Set the coefficients of the sinusoid function,
///        f(t)=b0*sin(2.*Pi*f0*(t-t0));
/// \param b0,f0,t0 (input) parameters in the sinsoidal function
// ===========================================================================
int TimeFunction::
setSinusoidFunction( const real b0_, const real f0_, const real t0_ )
{
  functionType=sinusoidalFunction;
  b0=b0_;
  f0=f0_;
  t0=t0_;

  return 0;
}

// ===========================================================================
/// \brief Get from a data base file.
// ===========================================================================
int TimeFunction::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"TimeFunction");

  aString className;
  subDir.get( className,"className" ); 

  int temp;
  subDir.get(temp,"functionType");  functionType=(FunctionTypeEnum)temp;
  subDir.get(temp,"composeType");  composeType=(ComposeTypeEnum)temp;

  subDir.get(a0,"a0");
  subDir.get(a1,"a1");
  subDir.get(b0,"b0");
  subDir.get(f0,"f0");
  subDir.get(t0,"t0");

  subDir.get(rampStart,"rampStart");
  subDir.get(rampEnd,"rampEnd");
  subDir.get(rampStartTime,"rampStartTime");
  subDir.get(rampEndTime,"rampEndTime");
  subDir.get(rampOrder,"rampOrder");

  subDir.get(ipar,"ipar");
  subDir.get(rpar,"rpar");

  int preFunctionExists;
  subDir.get(preFunctionExists,"preFunctionExists");
  if( preFunctionExists )
  {
    if( preFunction==NULL )
    {
      preFunction = new TimeFunction; preFunction->incrementReferenceCount();
    }
    preFunction->get(subDir,"preFunction");
  }
  else
  {
    if( preFunction!=NULL && preFunction->decrementReferenceCount()==0 )
    {
      delete preFunction; preFunction=NULL;
    }
  }

  mapFunction.get( subDir, "mapFunction" );
  subDir.get(timeParameterizationScaleFactor,"timeParameterizationScaleFactor");

  delete &subDir;
  return 0;
}


// ===========================================================================
/// \brief Put to a data base file.
// ===========================================================================
int TimeFunction::
put( GenericDataBase & dir, const aString & name) const
{
  GenericDataBase & subDir = *dir.virtualConstructor();   // create a derived data-base object
  dir.create(subDir,name,"TimeFunction");                 // create a sub-directory 

  aString className="TimeFunction";
  subDir.put( className,"className" );

  subDir.put((int)functionType,"functionType"); 
  subDir.put((int)composeType,"composeType"); 

  subDir.put(a0,"a0");
  subDir.put(a1,"a1");
  subDir.put(b0,"b0");
  subDir.put(f0,"f0");
  subDir.put(t0,"t0");

  subDir.put(rampStart,"rampStart");
  subDir.put(rampEnd,"rampEnd");
  subDir.put(rampStartTime,"rampStartTime");
  subDir.put(rampEndTime,"rampEndTime");
  subDir.put(rampOrder,"rampOrder");

  subDir.put(ipar,"ipar");
  subDir.put(rpar,"rpar");

  int preFunctionExists=preFunction!=NULL;
  subDir.put(preFunctionExists,"preFunctionExists");
  if( preFunctionExists )
    preFunction->put(subDir,"preFunction");

  mapFunction.put( subDir, "mapFunction" );
  subDir.put(timeParameterizationScaleFactor,"timeParameterizationScaleFactor");

  delete &subDir;
  return 0;  
}



// ============================================================================
/// \brief Evaluate the time function.
/// \param t (input) : evaluate at this time.
/// \param f (output) : value at time t.
// ===========================================================================
int TimeFunction::
eval( const real t, real & f )
{
  return evalDerivative( t,f,0 );
}

// ============================================================================
/// \brief Evaluate the time function and its first derivative.
/// \param t (input) : evaluate at this time.
/// \param f,ft (output) : function and derivative at time t.
// ===========================================================================
int TimeFunction::
eval( const real t, real & f, real & ft )
{
  evalDerivative( t,f,0 );
  evalDerivative( t,ft,1 );

  return 0;
}



// ============================================================================
/// \brief Evaluate the time function and an arbitraty derivative.
/// \param t (input) : evaluate at this time.
/// \param f,ft (output) : function and derivative at time t.
/// \param derivative (input) : evaluate this derivative.
/// \param computeComposed (input) : if true (default) then evaluate the composed function, otherwise
///            just evaluate the un-composed mapping.
// ===========================================================================
int TimeFunction::
evalDerivative( const real t, real & fp, int derivative, bool computeComposed /* = true */ )
{
  if( functionType==linearFunction )
  {
    if( derivative==0 )
      fp=a0+a1*t;
    else if( derivative==1 )
      fp=a1;
    else
      fp=0.;
 
  }
  else if( functionType==sinusoidalFunction )
  {
    real arg=2.*Pi*f0*(t-t0);
    if( derivative==0 )
      fp=b0*sin(arg);
    else if( derivative==1 ) 
      fp=b0*2.*Pi*f0*cos(arg);
    else if( derivative==2 ) 
      fp=-b0*SQR(2.*Pi*f0)*sin(arg);
    else
    {
      OV_ABORT("TimeFunction::evalDerivative:finish me");
    }
  }
  else if( functionType==userDefinedFunction )
  {
    // here is a user-defined function 
    if( derivative==0 )
      fp= t + sin(t);
    else if( derivative==1 ) 
      fp=1. + cos(t);
    else if( derivative==2 ) 
      fp=-sin(t);
    else
    {
      OV_ABORT("TimeFunction::evalDerivative:finish me");
    }
  }
  else if( functionType==rampFunction )
  {
    // ---- Ramp function ----
    //
    //                ----------------
    //               /                
    //              /                 
    //  ------------                  
    // 
    const real rampInterval=rampEndTime-rampStartTime;
    if( t<rampStartTime )
    { // ramp is off
      if( derivative==0 )
	fp=rampStart;
      else
	fp=0;
    }
    else if( t<rampEndTime && rampInterval>0. )
    {
      // *** see gf/ramp.maple

      const real ts=(t-rampStartTime)/rampInterval;

      // scale basic ramp function by this factor: 
      const real scale=derivative==0 ? (rampEnd-rampStart) : (rampEnd-rampStart)/pow(rampInterval,derivative);

      if( rampOrder==1 )
      {
#define ramp1(t)    ((3-2.*t)*(t)*(t))
#define ramp1t(t)   ((6-6.*t)*t)
#define ramp1tt(t)  (-12.*t+6)
#define ramp1ttt(t) (-12)
//         // here is a cubic ramp (first derivative is zero at start and end)
// 	if( derivative==0 )
// 	  fp = rampStart + (rampEnd-rampStart)*ts*ts*(3.-2.*ts);
// 	else if( derivative==1 )
// 	  fp  = (rampEnd-rampStart)*6.*ts*(1.-ts)/rampInterval;
// 	else if( derivative==2 )
// 	  fp = (rampEnd-rampStart)*(6.-12.*ts)/(rampInterval*rampInterval);
// 	else
// 	  fp = 0.;

	if( derivative==0 )
	  fp = rampStart + scale*ramp1(ts);
	else if( derivative==1 )
	  fp  = scale*ramp1t(ts);
	else if( derivative==2 )
	  fp = scale*ramp1tt(ts);
 	else if( derivative==3 )
 	  fp = scale*ramp1ttt(ts);
	else
	{
	  printF("TimeFunction::ERROR: ramp derivative=%i is not implemented\n",derivative);
	}
      }
      else if( rampOrder==2 )
      {
        // This ramp has 2 derivatives zero at the ends 
#define ramp2(t)    ((10+(-15+6.*t)*t)*(t)*(t)*(t))
#define ramp2t(t)   ((30+(-60+30.*t)*t)*(t)*(t))
#define ramp2tt(t)  ((60+(-180+120.*t)*t)*t)
#define ramp2ttt(t) (60+(-360+360.*t)*t)

	if( derivative==0 )
	  fp = rampStart + scale*ramp2(ts);
	else if( derivative==1 )
	  fp  = scale*ramp2t(ts);
	else if( derivative==2 )
	  fp = scale*ramp2tt(ts);
 	else if( derivative==3 )
 	  fp = scale*ramp2ttt(ts);
	else
	{
	  printF("TimeFunction::ERROR: ramp derivative=%i is not implemented\n",derivative);
	}
      }
      else if( rampOrder==3 )
      {

        // This ramp has 3 derivatives zero at the ends 
#define ramp3(t)    ((35+(-84+(70-20.*t)*t)*t)*(t)*(t)*(t)*(t))
#define ramp3t(t)   ((140+(-420+(420-140.*t)*t)*t)*(t)*(t)*(t))
#define ramp3tt(t)  ((420+(-1680+(2100-840.*t)*t)*t)*(t)*(t))
#define ramp3ttt(t) ((840+(-5040+(8400-4200.*t)*t)*t)*t)
        
	if( derivative==0 )
	  fp = rampStart + scale*ramp3(ts);
	else if( derivative==1 )
	  fp  = scale*ramp3t(ts);
	else if( derivative==2 )
	  fp = scale*ramp3tt(ts);
 	else if( derivative==3 )
 	  fp = scale*ramp3ttt(ts);
	else
	{
	  printF("TimeFunction::ERROR: ramp derivative=%i is not implemented\n",derivative);
	}
      }
      else  if( rampOrder==4 )
      {
        // This ramp has 4 derivatives zero at the ends 
#define ramp4(t)    ((126+(-420+(540+(-315+70.*t)*t)*t)*t)*(t)*(t)*(t)*(t)*(t))
#define ramp4t(t)   ((630+(-2520+(3780+(-2520+630.*t)*t)*t)*t)*(t)*(t)*(t)*(t))
#define ramp4tt(t)  ((2520+(-12600+(22680+(-17640+5040.*t)*t)*t)*t)*(t)*(t)*(t))
#define ramp4ttt(t) ((7560+(-50400+(113400+(-105840+35280.*t)*t)*t)*t)*(t)*(t))
	if( derivative==0 )
	  fp = rampStart + scale*ramp4(ts);
	else if( derivative==1 )
	  fp  = scale*ramp4t(ts);
	else if( derivative==2 )
	  fp = scale*ramp4tt(ts);
 	else if( derivative==3 )
 	  fp = scale*ramp4ttt(ts);
	else
	{
	  printF("TimeFunction::ERROR: ramp derivative=%i is not implemented\n",derivative);
	}	
      }
      else
      {
	printF("TimeFunction::ERROR: rampOrder=%i is not implemented. Only orders 1,2,3 and 4 are available\n");
	OV_ABORT("error");
      }

    }
    else
    {
      if( derivative==0 )
        fp = rampEnd;
      else
        fp = 0.;
    }

  }
  else if( functionType==mappingFunction )
  {
    // The time function is defined by a mapping 
    assert( mapFunction.mapPointer!=NULL );
    
    RealArray r(1,1),x(1,1),xr(1,1,1);
    r=t*timeParameterizationScaleFactor;   // scale t to [0,1]
    if( derivative==0 )
    {
      mapFunction.mapS(r,x);
      fp= x(0,0);
    }
    else if( derivative==1 )
    {
      mapFunction.mapS(r,x,xr);
      fp=xr(0,0,0)*timeParameterizationScaleFactor;
    }
    else if( derivative==2 ) 
    {
      Index I=Range(0,0);
      mapFunction.mapPointer->secondOrderDerivative(I,r,xr,0,0);
      fp=xr(0,0,0)*SQR(timeParameterizationScaleFactor);  // second derivative 
    }
    else
    {
      OV_ABORT("TimeFunction::mappingFunction:evalDerivative:finish me");
    }


  }
  else
  {
    printF("TimeFunction::evalDerivative:ERROR: unknown functionType=%i\n",(int)functionType);
    OV_ABORT("error");
  }

  if( computeComposed && preFunction!=NULL )
  {
    // --- we "compose" the current function with preFunction ---


    if( composeType==composeWillAdd )
    {
      // composition simply adds the functions or derivatives: 
      real f2p;
      preFunction->evalDerivative( t, f2p, derivative );
      fp+= f2p;
    }
    else if( composeType==composeWillMultiply )
    {
      // Composition is defined as multiplication 
      //  f(t) = f1(t)*f2(t) 
      if( derivative==0 )  
      {
	real f1,f2;
	f1=fp;
	preFunction->eval( t, f2 );
	fp = f1*f2;
      }
      else if( derivative==1 )
      {
	real f1,f1t,f2,f2t;
	const bool compose=false;
	evalDerivative( t,f1,0,compose );  // eval un-composed function
	f1t=fp;
	preFunction->evalDerivative( t, f2,  0 );
	preFunction->evalDerivative( t, f2t, 1 );
       
	fp = f1t*f2 + f1*f2t;
      }
      else if( derivative==2 )
      {
	real f1,f1t,f1tt, f2,f2t,f2tt;

	const bool compose=false;
	evalDerivative( t,f1 ,0,compose ); // eval un-composed function
	evalDerivative( t,f1t,1,compose );
	f1tt=fp;
	preFunction->evalDerivative( t, f2,  0 );
	preFunction->evalDerivative( t, f2t, 1 );
	preFunction->evalDerivative( t, f2tt,2 );
       
	fp = f1tt*f2 + 2.*f1t*f2t + f1*f2tt;
      }
      else
      {
	printF("TimeFunction::compose functions: derivative=%i not implemented.\n",derivative);
	OV_ABORT("finish me");
      }
    }
    else
    {
	printF("TimeFunction::compose functions: unknown composeType=%i.\n",(int)composeType);
	OV_ABORT("finish me");
    }
    
  }
  


  return 0;

}

// ===============================================================================
/// \brief display the parameters.
// ===============================================================================
int TimeFunction::
display( FILE *file /* = stdout */ )
{
  fPrintF(file," ----------------- TimeFunction Parameters -----------------\n");
  fPrintF(file," Linear function    : f(t) = a0 + a1*t\n"
	 " Sinusoid function: f(t) = b0*sin(2*Pi*f0*(t-t0))\n");
  fPrintF(file," Function: %s\n",(functionType==linearFunction ? "linear function" :
			   functionType==sinusoidalFunction ? "sinusoidal function" : 
			   functionType==rampFunction ? "ramp function" : 
			   functionType==mappingFunction ? "mapping function" : 
			   functionType==userDefinedFunction ? "user defined function" : "unknown"));
  fPrintF(file," Linear parameters: a0=%g, a1=%g\n",a0,a1);
  fPrintF(file," Sinusoid parameters: b0=%g, f0=%g, t0=%g\n",b0,f0,t0);
  fPrintF(file," ramp: varies from %g to %g over the time-interval=[%g,%g]. rampOrder=%i.\n",
	 rampStart,rampEnd,rampStartTime,rampEndTime,rampOrder);
  if( preFunction!=NULL )
  {
    fPrintF(file," This TimeFunction is composed (%s) with another one.\n",
	   (composeType==composeWillMultiply ? "multiplies" : composeType==composeWillAdd ? "adds" : "unknown composition"));
  }
  else
    fPrintF(file," This TimeFuncion is not currently composed with any other.\n");
  fPrintF(file," -----------------------------------------------------------\n");

  return 0;
}


// ===============================================================================
/// \brief Interactively update the TimeFunction parameters:
// ===============================================================================
int TimeFunction::
update(GenericGraphicsInterface & gi )
{

  real plotStartTime=0.,plotEndTime=1.; 

  GUIState dialog;
  bool buildDialog=true;
  if( buildDialog )
  {
    dialog.setWindowTitle("TimeFunction");
    dialog.setExitCommand("exit", "exit");

    // option menus
    dialog.setOptionMenuColumns(1);


    aString opCommand1[] = {"linear function",
			    "sinusoidal function",
			    "ramp function",
			    "mapping function",
			    "user defined function",
			    ""};
    
    dialog.addOptionMenu( "type:", opCommand1, opCommand1, functionType); 

    aString opCommand2[] = {"compose will multiply",
			    "compose will add",
			    ""};
    
    dialog.addOptionMenu( "compose type:", opCommand2, opCommand2, composeType); 


    aString cmds[] = {"add composed function",
                      "edit composed function",
                      "edit mapping function",
                      "show parameters",
                      "check derivatives",
                      "plot",
		      ""};

    int numberOfPushButtons=5;  // number of entries in cmds
    int numRows=(numberOfPushButtons+1)/2;
    dialog.setPushButtons( cmds, cmds, numRows ); 

    const int numberOfTextStrings=7;  // max number allowed
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];


    int nt=0;
    textLabels[nt] = "linear parameters:";  sPrintF(textStrings[nt],"%g,%g (a0,a1)",a0,a1);  nt++; 
    textLabels[nt] = "sinusoid parameters:";  sPrintF(textStrings[nt],"%g,%g,%g (b0,f0,t0)",b0,f0,t0);  nt++; 
    textLabels[nt] = "ramp end values:";  sPrintF(textStrings[nt],"%g,%g (start,end)",rampStart,rampEnd);  nt++; 
    textLabels[nt] = "ramp times:";  sPrintF(textStrings[nt],"%g,%g (start,end)",rampStartTime,rampEndTime);  nt++; 
    textLabels[nt] = "ramp order:";  sPrintF(textStrings[nt],"%i",rampOrder);  nt++; 
    textLabels[nt] = "plot bounds:";  sPrintF(textStrings[nt],"%g,%g (start,end)",plotStartTime,plotEndTime);  nt++; 

    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    dialog.setTextBoxes(textLabels, textLabels, textStrings);

    // dialog.buildPopup(menu);
    gi.pushGUI(dialog);
  }
  

  aString answer;

  gi.appendToTheDefaultPrompt("TimeFunction>"); // set the default prompt
  int len=0;
  for( int it=0;; it++ )
  {
    gi.getAnswer(answer,"");
 

    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="linear function" )
    {
      functionType=linearFunction;
      dialog.getOptionMenu("type:").setCurrentChoice(functionType);
    }
    else if( answer=="sinusoidal function" )
    {
      functionType=sinusoidalFunction;
      dialog.getOptionMenu("type:").setCurrentChoice(functionType);
    }
    else if( answer=="ramp function" )
    {
      printF("INFO: The ramp function transitions from one one value (rampStart) to a second value (rampEnd)\n"
             "  as t varies between rampTimeStart and rampTimeEnd: \n"
             "    ramp(t)  =  rampStart for t<rampTimeStart,\n"
             "    ramp(t)  =  smoothly varies between rampStart and rampEnd, for rampStartTime < t < rampEndTime,\n"
             "    ramp(t)  =  rampStart for t>rampTimeEnd.\n"
             " The rampOrder indicates how smoothly the ramp turns on and off and is equal to the number of\n"
             " derivatives that are zero at the start and end of the ramp.\n");
      
      functionType=rampFunction;
      dialog.getOptionMenu("type:").setCurrentChoice(functionType);
    }
    else if( answer=="mapping function" )
    {
      functionType=mappingFunction;
      dialog.getOptionMenu("type:").setCurrentChoice(functionType);

      printF("TimeFunction::INFO: The mapping function f(t) is defined by a set of points (t_i,f_i) i=0,..,n-1.\n");
      printF("     The values for t_i must be increasing. \n");

      int n=0;
      aString line;
      gi.inputString(line,"Enter the number of points, n");
      sScanF(line,"%i",&n);
      if( n>0 )
      {
	RealArray t(n),f(n);
	
	for( int i=0; i<n; i++ )
	{
	  gi.inputString(line,sPrintF("Enter t,f for point %i",i));
	  sScanF(line,"%e %e",&t(i),&f(i));
	}
      
	if( mapFunction.getClassName()!="SplineMapping" )
	{
	  Mapping & map = *new SplineMapping;  // 1D spline function 
	  map.incrementReferenceCount();
	  mapFunction.reference(map);
	  map.decrementReferenceCount();
	}

        SplineMapping & spline = *((SplineMapping*)(mapFunction.mapPointer));
	
	plotStartTime=t(0);
	plotEndTime=t(n-1);
        dialog.setTextLabel("plot bounds:",sPrintF(answer,"%g,%g (start,end)",plotStartTime,plotEndTime));

	assert( t(n-1)>0. );
        // t is scaled to be on the interval [0,1]
        timeParameterizationScaleFactor=1./t(n-1);

	t *= timeParameterizationScaleFactor;   // scale t to be on [0,1]

	
	// ::display(t,"Here is t");

	spline.setParameterization(t); // parameterize by the scaled t 
	spline.setPoints(f); 
      }
      else
      {
	printF("TimeFunction::ERROR: n=%i but n must be positive.\n",n);
	gi.stopReadingCommandFile();
	continue;
      }
      
      // printF("  I will now call the SplineMapping update function so you can edit the SplineMapping.\n");
      

    }
    else if( answer=="edit mapping function" )
    {
      if( mapFunction.getClassName()!="SplineMapping" )
      {
	printF("The mapping function has not been defined yet.\n");
	continue;
      }
      
      printF("The mapping function is a SplineMapping.\n");
      MappingInformation mapInfo;
      mapInfo.graphXInterface=&gi;
      mapFunction.update(mapInfo);
     
    }
    else if( answer=="user defined function" )
    {
      functionType=userDefinedFunction;
      dialog.getOptionMenu("type:").setCurrentChoice(functionType);
    }
    else if( answer=="compose will multiply" )
    {
      composeType=composeWillMultiply;
      dialog.getOptionMenu("compose type:").setCurrentChoice(composeType);
    }
    else if( answer=="compose will add" )
    {
      composeType=composeWillAdd;
      dialog.getOptionMenu("compose type:").setCurrentChoice(composeType);
    }
    else if( len=answer.matches("linear parameters:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&a0,&a1);
      if( !gi.isGraphicsWindowOpen() )
        dialog.setTextLabel("linear parameters:",sPrintF(answer,"%g,%g (a0,a1)",a0,a1));
    }
    else if( len=answer.matches("sinusoid parameters:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&b0,&f0,&t0);
      if( !gi.isGraphicsWindowOpen() )
        dialog.setTextLabel("sinusoid parameters:",sPrintF(answer,"%g,%g (b0,f0,t0)",b0,f0,t0));
    }
    else if( len=answer.matches("ramp times:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&rampStartTime,&rampEndTime);
      if( !gi.isGraphicsWindowOpen() )
        dialog.setTextLabel("ramp times:",sPrintF(answer,"%g,%g (start,end)",rampStartTime,rampEndTime));
    }
    else if( len=answer.matches("ramp end values:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&rampStart,&rampEnd);
      if( !gi.isGraphicsWindowOpen() )
        dialog.setTextLabel("ramp end values:",sPrintF(answer,"%g,%g (start,end)",rampStart,rampEnd));
    }
    else if( len=answer.matches("plot bounds:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&plotStartTime,&plotEndTime);
      if( !gi.isGraphicsWindowOpen() )
        dialog.setTextLabel("plot bounds:",sPrintF(answer,"%g,%g (start,end)",plotStartTime,plotEndTime));
    }
    else if( dialog.getTextValue(answer,"ramp order:","%i",rampOrder) ){} // 
    else if( answer=="check derivatives" )
    {
      const int maxDerivativesToCheck=2;
      for( int d=1; d<=maxDerivativesToCheck; d++ )
      {
        real maxErr=0.;
        real norm=REAL_MIN*100.;
        int numberOfTimesToCheck=11;
        real dt=1./(numberOfTimesToCheck-1.);
	for( int i=0; i<numberOfTimesToCheck; i++ )
	{
	  real t=plotStartTime + (plotEndTime-plotStartTime)*i*dt;
	  
	  real ft=0.;
          evalDerivative( t, ft, d );

	  // Approx. deriv's with finite differences :
          // compute the first derivative by differences
	  real f0,f1,f2,fd;

          // optimal h from trucation and round off:   eps/h = h^2 (first derivative) or e/h^2 = h^2 for second
          const real h = pow(REAL_EPSILON,1./(2.+d));  
	  real t1=t-h, t2=t+h;
          eval( t1,f1 );
          eval( t ,f0 );
          eval( t2,f2 );

	  if( d==1 )
	  {
	    fd=(f2-f1)/(2.*h);
	  }
	  else if( d==2 )
	  {
	    fd=(f2-2.*f0+f1)/(h*h);
	  }
	  else
	  {
	    printF("TimeFunction::Finish me for derivative=%i\n",d);
	  }
	  
          norm = max( norm,fabs(fd) );
	  maxErr = max( maxErr, fabs(ft-fd) );
	  
	}

	printF("TimeFunction:: max rel. err in derivative %i is %8.2e (For 0 <= t <= 1)\n",d,maxErr/norm,
	       plotStartTime,plotEndTime);
      }
      

    }
    else if( answer=="edit composed function" )
    {
      printF("TimeFunction::INFO: The current TimeFunction can be composed with another TimeFunction\n"
             "   The current TimeFunction is applied AFTER the `composed TimeFunction'\n");
      if( preFunction!=NULL )
      {
	preFunction->update(gi);
      }
      else
      {
	printF("TimeFunction::WARNING: there is no composed motion defined. "
               "You should choose `add composed motion'\n");
      }
    }
    else if( answer=="add composed function" )
    {
      printF("TimeFunction::INFO: The current TimeFunction can be composed with another TimeFunction\n"
             "   The current TimeFunction is applied AFTER the `composed TimeFunction'\n");
      if( preFunction==NULL )
      {
        preFunction = new TimeFunction; preFunction->incrementReferenceCount();
	
	preFunction->update(gi);
      }
      else
      {
	printF("TimeFunction::WARNING: there is already a composed motion defined. "
               "You should choose `edit composed function' to make changes.\n");
      }
    }
    else if( answer=="show parameters" )
    {
      display();
    }
    else if( answer=="plot" )
    {

      const int nt=201;
      realArray tv(nt);
      // plot on a slightly larger interval tha
      real dt=(plotEndTime-plotStartTime)/(nt-1.);
      const int nd=3;  // plot f, f', f''
      realArray xv(nt,nd);
      for( int i=0; i<nt; i++ )
      {
	real t= plotStartTime + i*dt;
        tv(i)=t;
	eval( t,xv(i,0) );
	evalDerivative( t,xv(i,1),1 );
	evalDerivative( t,xv(i,2),2 );
      }
      aString title="TimeFunction";
      aString tName = "t";
      aString xName[nd]={ "f", "f'", "f''"};
      
      GraphicsParameters gip;
      gi.erase();
      PlotIt::plot(gi,tv,xv,title,tName,xName,gip);
      
    }
    else 
    {
      printF("TimeFunction::update: unknown response=%s",(const char*)answer);
      gi.stopReadingCommandFile();
    }

  }
  gi.unAppendTheDefaultPrompt();  // reset prompt
  if( buildDialog )
  {
    gi.popGUI(); // restore the previous GUI
  }
  return 0;
}


