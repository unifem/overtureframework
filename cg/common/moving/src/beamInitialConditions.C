#include "BeamModel.h"
#include "display.h"
#include "TravelingWaveFsi.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"

// standing wave: 
#define W(x,t) (amplitude*sin(k*(x))*cos(w*(t+tsw)))
#define Wt(x,t) (-w*amplitude*sin(k*(x))*sin(w*(t+tsw)))
#define Wtt(x,t) (-w*w*amplitude*sin(k*(x))*cos(w*(t+tsw)))

#define Wx(x,t) (k*amplitude*cos(k*(x))*cos(w*(t+tsw)))
#define Wtx(x,t) (-w*k*amplitude*cos(k*(x))*sin(w*(t+tsw)))
#define Wttx(x,t) (-w*w*k*amplitude*cos(k*(x))*cos(w*(t+tsw)))

// =====================================================================================
/// \brief Evaluate the exact solution (if any)
/// \param t (input) : assign values at this time.
/// \param u,v (output) : displacement and velocity
// =====================================================================================
int BeamModel::
getExactSolution( real t, RealArray & u, RealArray & v, RealArray & a ) const
{
  if( u.getLength(0)==0 )
    u.redim(2*numElem+2);
  if( v.getLength(0)==0 )
    v.redim(2*numElem+2);
  if( a.getLength(0)==0 )
    a.redim(2*numElem+2);

  real beamLength=L;
  real breadth=1.;

  if( exactSolutionOption=="standingWave" )
  {
    return getStandingWave( t,u,v,a );
  }
  else if( exactSolutionOption=="beamPiston" )
  {
    getBeamPiston( t,u,v,a  );
  }
  else if( exactSolutionOption=="beamUnderPressure" )
  {
    getBeamUnderPressure( t,u,v,a  );
  }
  else if( exactSolutionOption=="travelingWaveFSI" )
  {
    getTravelingWaveFSI( t,u,v,a  );
  }
  else if( exactSolutionOption=="oldTravelingWaveFsi" )
  { // old way
    setExactSolution( t,u,v,a);
  }  
  else if( exactSolutionOption=="twilightZone" ) 
  {
    bool & twilightZone = dbase.get<bool>("twilightZone");
    assert( twilightZone );
    assert( dbase.get<OGFunction*>("exactPointer")!=NULL );
    OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");

    Index I1,I2,I3;
    I1=Range(0,numElem); I2=0; I3=0;

    RealArray x(I1,I2,I3,2);  // beam axis (undeformed)
    const real dx=beamLength/numElem;
    for( int i1 = I1.getBase(); i1<=I1.getBound(); i1++ )
    {
      x(i1,0,0,0) = i1*dx; 
      x(i1,0,0,1) = 0.;    // should this be y0 ?
    }

    RealArray ue(I1,I2,I3,1), ve(I1,I2,I3,1), ae(I1,I2,I3,1), uxe(I1,I2,I3,1), vxe(I1,I2,I3,1), axe(I1,I2,I3,1);
    int isRectangular=0;
    const int wc=0;
    exact.gd( ue ,x,domainDimension,isRectangular,0,0,0,0,I1,I2,I3,wc,t );
    exact.gd( ve ,x,domainDimension,isRectangular,1,0,0,0,I1,I2,I3,wc,t );
    exact.gd( ae ,x,domainDimension,isRectangular,2,0,0,0,I1,I2,I3,wc,t );

    exact.gd( uxe,x,domainDimension,isRectangular,0,1,0,0,I1,I2,I3,wc,t );
    exact.gd( vxe,x,domainDimension,isRectangular,1,1,0,0,I1,I2,I3,wc,t );
    exact.gd( axe,x,domainDimension,isRectangular,2,1,0,0,I1,I2,I3,wc,t );
    for (int i = 0; i <= numElem; ++i)
    {
      u(i*2)   = ue(i,0,0,0);     // w 
      u(i*2+1) = uxe(i,0,0,0);    // w_x
    
      v(i*2)   = ve(i,0,0,0);     // w_t 
      v(i*2+1) = vxe(i,0,0,0);    // w_xt

      // real uxt = exact.gd(1,1,0,0, x(i,0,0,0),0.,0.,wc,t);
      // printF("Exact: u=%9.3e, ux=%9.3e, v=%9.3e, vx=%9.3e uxt=%9.3e\n",u(i*2),u(i*2+1),v(i*2),v(i*2+1),uxt);
      

      a(i*2)   = ae(i,0,0,0);     // w_tt 
      a(i*2+1) = axe(i,0,0,0);    // w_xtt

    }

    if( false )
    {
      exact.gd( uxe,x,domainDimension,isRectangular,0,2,0,0,I1,I2,I3,wc,t );
      ::display(uxe,"uxx exact","%6.2f ");

      exact.gd( uxe,x,domainDimension,isRectangular,0,3,0,0,I1,I2,I3,wc,t );
      ::display(uxe,"uxxx exact","%6.2f ");
    
    }
    
  }
  
  else
  {
    u=0.; v=0; a=0; 
  }

  return 0;
}


// =====================================================================================
/// \brief Evaluate the standing wave solution
/// \param t (input) : assign values at this time.
/// \param u,v (output) : displacement and velocity
// =====================================================================================
int BeamModel::
getStandingWave( real t, RealArray & u, RealArray & v, RealArray & a ) const
{
  if( u.getLength(0)==0 )
    u.redim(2*numElem+2);
  if( v.getLength(0)==0 )
    v.redim(2*numElem+2);
  if( a.getLength(0)==0 )
    a.redim(2*numElem+2);

  const real & amplitude=dbase.get<real>("amplitude");
  const real & waveNumber=dbase.get<real>("waveNumber");

  const real & tension=dbase.get<real>("tension");

  real beamLength=L;
  real k=2.*Pi*waveNumber/L;
  real breadth=1.;
  real w = sqrt( (elasticModulus*areaMomentOfInertia*pow(k,4) +tension*k*k)/( density*thickness*breadth ) );

  // time offset for the standing wave: 
  real tsw = (2.*Pi/w)*dbase.get<real>("standingWaveTimeOffset");
  
  for (int i = 0; i <= numElem; ++i)
  {
    real xl = ( (real)i /numElem) *  beamLength;
    u(i*2)   = W(xl,t);     // w 
    u(i*2+1) = Wx(xl,t);    // w_x
    
    v(i*2)   = Wt(xl,t);    // w_t 
    v(i*2+1) = Wtx(xl,t);   // w_tx 

    a(i*2)   = Wtt(xl,t);    // w_tt 
    a(i*2+1) = Wttx(xl,t);   // w_ttx

  }
  return 0;
}

// =====================================================================================
/// \brief Evaluate the FSi traveling wave solution
/// \param t (input) : assign values at this time.
/// \param u,v (output) : displacement and velocity
// =====================================================================================
int BeamModel::
getTravelingWaveFSI( real t, RealArray & u, RealArray & v, RealArray & a ) const
{
  if( u.getLength(0)==0 )
    u.redim(2*numElem+2);
  if( v.getLength(0)==0 )
    v.redim(2*numElem+2);
  if( a.getLength(0)==0 )
    a.redim(2*numElem+2);

  assert( dbase.get<TravelingWaveFsi*>("travelingWaveFsi")!=NULL );
  TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

  real beamLength=L;

  int numGhost=1;
  Index I1,I2,I3;
  I1=Range(-numGhost,numElem+numGhost); I2=0; I3=0;

  RealArray x(I1,I2,I3,2), ue(I1,I2,I3,2), ve(I1,I2,I3,2), ae(I1,I2,I3,2);
  const real dx=beamLength/numElem;
  for( int i1 = I1.getBase(); i1<=I1.getBound(); i1++ )
  {
    x(i1,0,0,0) = i1*dx; 
    x(i1,0,0,1) = 0.;    // should this be y0 ?
  }
  travelingWaveFsi.getExactShellSolution( x,ue,ve,ae, t, I1,I2,I3 );

  for (int i = 0; i <= numElem; ++i)
  {
    u(i*2)   = ue(i,0,0,1);     // w 
    u(i*2+1) = (ue(i+1,0,0,1)-ue(i-1,0,0,1))/(2.*dx);  // w_x   *** DO THIS FOR NOW **
    
    v(i*2)   = ve(i,0,0,1);     // w_t 
    v(i*2+1) = (ve(i+1,0,0,1)-ve(i-1,0,0,1))/(2.*dx);      // w_xt *** DO THIS FOR NOW **

    a(i*2)   = ae(i,0,0,1);     // w_tt 
    a(i*2+1) = (ae(i+1,0,0,1)-ae(i-1,0,0,1))/(2.*dx);      // w_xtt *** DO THIS FOR NOW **

  }

  return 0;
}


// =====================================================================================
/// \brief Evaluate the "beam piston" exact solution (beam adjacent to one or two fluid domains)
/// \param t (input) : assign values at this time.
/// \param u,v (output) : displacement and velocity
// =====================================================================================
int BeamModel::
getBeamPiston( real t, RealArray & u, RealArray & v, RealArray & a ) const
{
  if( u.getLength(0)==0 )
    u.redim(2*numElem+2);
  if( v.getLength(0)==0 )
    v.redim(2*numElem+2);
  if( a.getLength(0)==0 )
    a.redim(2*numElem+2);

  // ** NOTE this solution appears in userDefinedKnownSolution.C and beamInitialConditions.C ***
  const real *rpar = dbase.get<real[10]>("beamPistonPar");
  const real & ya          = rpar[0];    
  const real & yb          = rpar[1];    
  const real & pa          = rpar[2];   
  const real & pb          = rpar[3];   
  const real & rhos        = rpar[4];    
  const real & hs          = rpar[5];    
  const real & fluidHeight = rpar[6];    
  const real & K0          = rpar[7];    
  const real & Kt          = rpar[8];    

  assert( K0!=0. && Kt==0. );
  
  const real dp = pa-pb;
  // -- hard code these for now: **FIX ME**
  const real rho=1;

  const real omega=sqrt(K0/(rhos*hs+rho*fluidHeight));
  const real w    = (dp/K0)*(1. - cos(omega*t) );   // beam position y=w(t) 
  const real wt   = (dp/K0)*omega*sin(omega*t);
  const real wtt  = (dp/K0)*omega*omega*cos(omega*t);
  const real dpdy =-rho*wtt;  // dp/dy = -rho*w_tt

  const real & dt = dbase.get<real>("dt");
  if( t<=0 || t<=3.*dt )
    printF("--BM-- getBeamPiston solution at t=%9.3e, rhos*hs=%8.2e, rho*H=%8.2e, K0=%8.2e, pa=%g pb=%g dpdy=%g"
           " w=%g, wt=%g, wtt=%g\n",
	   t,rhos*hs,rho*fluidHeight,K0,pa,pb,dpdy,w,wt,wtt);


  for (int i = 0; i <= numElem; ++i)
  {
    u(i*2)   = w;      // w 
    u(i*2+1) = 0.;     // w_x
    
    v(i*2)   = wt;     // w_t 
    v(i*2+1) = 0.;     // w_tx 

    a(i*2)   = wtt;    // w_tt 
    a(i*2+1) = 0.;     // w_ttx

  }


  return 0;
}

// =====================================================================================
/// \brief Evaluate the "beam under pressure" exact solution 
/// \param t (input) : assign values at this time.
/// \param u,v (output) : displacement and velocity
// =====================================================================================
int BeamModel::
getBeamUnderPressure( real t, RealArray & u, RealArray & v, RealArray & a ) const
{
  if( u.getLength(0)==0 )
    u.redim(2*numElem+2);
  if( v.getLength(0)==0 )
    v.redim(2*numElem+2);
  if( a.getLength(0)==0 )
    a.redim(2*numElem+2);

  const real *rpar = dbase.get<real[10]>("beamUnderPressurePar");
  const real & dp          = rpar[0];    

  const real & tension = dbase.get<real>("tension");
  
  const real & dt = dbase.get<real>("dt");
  if( t<=0 || t<=3.*dt )
    printF("--BM-- getBeamUnderPressure solution at t=%9.3e, dp=%8.2e\n",t,dp);

  real beamLength=L;
  real factor;
  if( tension!=0. && elasticModulus==0. )
    factor = dp/(2.*tension);
  else if( tension==0. && elasticModulus!=0. )
    factor = dp/(24.*elasticModulus*areaMomentOfInertia);
  else
  {
    OV_ABORT("BeamModel::getBeamUnderPressure:ERROR -- finish me--");
  }

  real xa=0, xb=beamLength;

  // TODO: Workout the solution for E!=0 AND T!=0 ******

  for (int i = 0; i <= numElem; ++i)
  {
    real x = ( (real)i /numElem) *  beamLength;
    if( tension!=0. && elasticModulus==0. )
    { // solution is a quadratic
      u(i*2)   = factor*(x-xa)*(xb-x);           // w 
      u(i*2+1) = factor*( (xb-x) - (x-xa) );     // w_x
    }
    else if( tension==0. && elasticModulus!=0. )
    { // solution is a quartic
      u(i*2)   = factor*SQR(x-xa)*SQR(x-xb);     // w 
      u(i*2+1) = factor*( 2.*(x-xa)*SQR(x-xb) + 2.*(x-xb)*SQR(x-xa) );     // w_x
    
    }
    
    v(i*2)   = 0.;     // w_t 
    v(i*2+1) = 0.;     // w_tx 

    a(i*2)   = 0.;    // w_tt 
    a(i*2+1) = 0.;     // w_ttx

  }


  return 0;
}


// =====================================================================================
/// \brief Assign initial conditions
/// \param t (input) : assign values at this time.
/// \param u,v,a (output) : displacement, velocity and acceleration
// =====================================================================================
int BeamModel::
assignInitialConditions( real t, RealArray & u, RealArray & v, RealArray & a )
{

  if( debug & 1 )
    printF("BeamModel::assignInitialConditions for %s, t=%9.3e\n",(const char*)initialConditionOption,t);

  real beamLength=L;
  
  if( u.getLength(0)==0 )
    u.redim(2*numElem+2);
  if( v.getLength(0)==0 )
    v.redim(2*numElem+2);
  if( a.getLength(0)==0 )
    a.redim(2*numElem+2);
  

  if( initialConditionOption=="zero" || initialConditionOption=="none" )
  {
    u=0.;
    v=0.;
    a=0.;
  }
  else if( initialConditionOption=="exact" )
  {
    getExactSolution( t, u, v, a  );
  }
  // else if( initialConditionOption=="oldTravelingWaveFsi" )
  // {
  //   setExactSolution( t,u,v,a);
  // }
  // else if( initialConditionOption=="twilightZone" )
  // {
  //   bool & twilightZone = dbase.get<bool>("twilightZone");
  //   assert( twilightZone );
  //   assert( dbase.get<OGFunction*>("exactPointer")!=NULL );
  //   OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");

  //   Index I1,I2,I3;
  //   I1=Range(0,numElem); I2=0; I3=0;

  //   RealArray x(I1,I2,I3,2);  // beam axis (undeformed)
  //   const real dx=beamLength/numElem;
  //   for( int i1 = I1.getBase(); i1<=I1.getBound(); i1++ )
  //   {
  //     x(i1,0,0,0) = i1*dx; 
  //     x(i1,0,0,1) = 0.;    // should this be y0 ?
  //   }

  //   RealArray ue(I1,I2,I3,1), ve(I1,I2,I3,1), ae(I1,I2,I3,1), uxe(I1,I2,I3,1), vxe(I1,I2,I3,1), axe(I1,I2,I3,1);
  //   int isRectangular=0;
  //   const int wc=0;
  //   exact.gd( ue ,x,domainDimension,isRectangular,0,0,0,0,I1,I2,I3,wc,t );
  //   exact.gd( ve ,x,domainDimension,isRectangular,1,0,0,0,I1,I2,I3,wc,t );
  //   exact.gd( ae ,x,domainDimension,isRectangular,2,0,0,0,I1,I2,I3,wc,t );
  //   exact.gd( uxe,x,domainDimension,isRectangular,0,1,0,0,I1,I2,I3,wc,t );
  //   exact.gd( vxe,x,domainDimension,isRectangular,1,1,0,0,I1,I2,I3,wc,t );
  //   exact.gd( axe,x,domainDimension,isRectangular,2,1,0,0,I1,I2,I3,wc,t );
  //   for (int i = 0; i <= numElem; ++i)
  //   {
  //     u(i*2)   = ue(i,0,0,0);     // w 
  //     u(i*2+1) = uxe(i,0,0,0);    // w_x
    
  //     v(i*2)   = ve(i,0,0,0);     // w_t 
  //     v(i*2+1) = vxe(i,0,0,0);    // w_xt

  //     a(i*2)   = ae(i,0,0,0);     // w_tt 
  //     a(i*2+1) = axe(i,0,0,0);    // w_xtt

  //   }
  // }
  else
  {
    printF("BeamModel::assignInitialConditions:ERROR:unknown initialConditionOption=[%s]\n",
	   (const char*)initialConditionOption);
    OV_ABORT("error");
  }
  
  assignBoundaryConditions(t,u,v,a );

  // if( t==0. )
  // {
  //   myPosition=u;
  //   myVelocity=v;
  //   myAcceleration=a;
  // }
  
  return 0;
}


// =====================================================================================
/// \brief Choose the exact solution.
// =====================================================================================
int BeamModel::
chooseExactSolution(CompositeGrid & cg, GenericGraphicsInterface & gi )
{

  real & amplitude=dbase.get<real>("amplitude");
  real & waveNumber=dbase.get<real>("waveNumber");

  GUIState gui;
  gui.setWindowTitle("Beam exact solution");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;


  const int maxCommands=40;
  aString cmd[maxCommands];

  aString exactSolutionOptions[] = {"no exact solution",
				    "twilight zone",
				    "standing wave",
				    "traveling wave FSI-INS",
				    "old traveling wave FSI-INS",
				    "beam piston",
                                    "beam under pressure",
				    ""};
  GUIState::addPrefix(exactSolutionOptions,"Exact solution:",cmd,maxCommands);
  int option  =(exactSolutionOption=="none"                ? 0 : 
		exactSolutionOption=="twilightZone"        ? 1 : 
                exactSolutionOption=="standingWave"        ? 2 :
                exactSolutionOption=="travelingWaveFSI"    ? 3 :
                exactSolutionOption=="oldTravelingWaveFsi" ? 4 :
                exactSolutionOption=="beamPiston"          ? 5 : 
                exactSolutionOption=="beamUnderPressure"   ? 6 : 
                0 );
  dialog.addOptionMenu("Exact solution:",cmd,exactSolutionOptions,option );

  const int numberOfTextStrings=10;
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
    
  textLabels[nt] = "amplitude:"; sPrintF(textStrings[nt], "%g",amplitude);  nt++; 
  textLabels[nt] = "wave number:"; sPrintF(textStrings[nt], "%g",waveNumber);  nt++; 
  textLabels[nt] = "standing wave t0:"; sPrintF(textStrings[nt], "%g",dbase.get<real>("standingWaveTimeOffset"));  nt++; 


  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  // addPrefix(textLabels,prefix,cmd,maxCommands);
  // dialog.setTextBoxes(cmd, textLabels, textStrings);
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("beamExact>");

  aString prefix = ""; // prefix for commands to make them unique.

  aString answer,buff;

  int len=0;
  for( ;; ) 
  {
	    
    gi.getAnswer(answer,"");
  
    // printF(answer,"answer=[answer]\n",(const char *)answer);

    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);


    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( (len=answer.matches("Exact solution:")) )
    {
      aString option = answer(len,answer.length()-1);
      if( option=="no exact solution" )
      {
	exactSolutionOption="none";
	printF("Setting exactSolutionOption=[%s]\n",(const char*)exactSolutionOption);
      }
      else if( option=="twilight zone" )
      {
	exactSolutionOption="twilightZone";
	dbase.get<bool>("twilightZone")=true;
	printF("Setting exactSolutionOption=[%s]\n",(const char*)exactSolutionOption);
      }
      else if( option=="standing wave" )
      {
	exactSolutionOption="standingWave"; 
	printF("Setting exactSolutionOption=[%s]\n",(const char*)exactSolutionOption);
      }
      else if( option=="old traveling wave FSI-INS" )
      {
	// *old way*
	exactSolutionOption="oldTravelingWaveFsi";
      }
      else if( option=="traveling wave FSI-INS" )
      {
	exactSolutionOption="travelingWaveFSI"; 
	printF("Setting exactSolutionOption=[%s]\n",(const char*)exactSolutionOption);

	printF("INFO:The FSI traveling wave solution is an exact solution for a solid (shell or bulk)\n"
	       "coupled to a linearized incompressible fluid\n"
	       "See: `An analysis of a new stable partitioned algorithm for FSI problems.\n"
	       "      Part I: Incompressible flow and elastic solids'. \n"
	       "      J.W. Banks, W.D. Henshaw and D.W. Schwendeman, JCP 2014.\n");
    
      
	// *************** WE should share this object with the fluid domain ********************   **FIX ME*

	if( !dbase.has_key("travelingWaveFsi") )
	  dbase.put<TravelingWaveFsi*>("travelingWaveFsi")=NULL;

	if( dbase.get<TravelingWaveFsi*>("travelingWaveFsi")==NULL )
	  dbase.get<TravelingWaveFsi*>("travelingWaveFsi")= new TravelingWaveFsi; // who will delete ???
	TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

	travelingWaveFsi.update(gi );

	// we also pass the grid for the solid:
	CompositeGrid & cgSolid = cg; // do this for now  -- only used for number of grid points

	int numberOfFluidGridPoints=21, numberOfSolidGridPoints=21;  // I don't think these matter
	travelingWaveFsi.setup( numberOfFluidGridPoints, numberOfSolidGridPoints );


      }
      else if( option=="beam piston" )
      {
	exactSolutionOption="beamPiston"; 
	printF("--BM-- The beam piston solution is an FSI solution for a horizontal beam adjacent to one or two fluid domains.\n"
	       " The vertical motion of the beam is of the form:\n"
	       "      y(t) = (dp/K0)*( 1 - cos(omega*t) )   (for an undamped beam, Kt=0)\n");

	if( !dbase.has_key("beamPistonPar") )
	  dbase.put<real[10]>("beamPistonPar");
	real *rpar = dbase.get<real[10]>("beamPistonPar");

	real & ya          = rpar[0];    
	real & yb          = rpar[1];    
	real & pa          = rpar[2];   
	real & pb          = rpar[3];   
	real & rhos        = rpar[4];    
	real & hs          = rpar[5];    
	real & fluidHeight = rpar[6];    
	real & K0          = rpar[7];    
	real & Kt          = rpar[8];    

	gi.inputString(answer,sPrintF("Enter ya,yb,pa,pb,rhos,hs,fluidHeight,K0,Kt"));
	sScanF(answer,"%e %e %e %e %e  %e %e %e %e %e %e %e %e",&ya,&yb,&pa,&pb,&rhos,&hs,&fluidHeight,&K0,&Kt);
	printF("Setting ya=%g, yb=%g, pa=%g, pb=%g, rhos=%g, hs=%g, fluidHeight=%g, K0=%g, Kt=%g \n",
               ya,yb,pa,pb,rhos,hs,fluidHeight,K0,Kt);   


      }
      else if( option=="beam under pressure" )
      {
	exactSolutionOption="beamUnderPressure"; 
	printF("--BM-- The `beam under pressure' solution is a steady FSI solution for a horizontal beam \n"
               " pinned (or clamped) on both ends and with constant pressure force.\n"
	       " The vertical displacement of the beam is of the form :\n"
	       "      y(t) = ( dp/(2T) )*(x-a)(b-x) )          (for T!=0, E=0)\n"
	       "      y(t) = ( dp/(24 EI) )*(x-a)^2*(b-x)^2 )  (for T=0, E!=0)\n"
               "      dp = pressure force\n"
               );

	if( !dbase.has_key("beamUnderPressurePar") )
	  dbase.put<real[10]>("beamUnderPressurePar");
	real *rpar = dbase.get<real[10]>("beamUnderPressurePar");
	real & dp          = rpar[0];    
	gi.inputString(answer,sPrintF("Enter dp"));
	sScanF(answer,"%e",&dp);
	printF("Setting dp=%g\n",dp);

      }
      else
      {
	printF("BeamModel::chooseExactSolution:ERROR:unknown response=[%s]\n",(const char*)answer);
	gi.stopReadingCommandFile();
      }
      
    }
    
    else if( dialog.getTextValue(answer,"amplitude:","%g",amplitude) ){} //
    else if( dialog.getTextValue(answer,"wave number:","%g",waveNumber) ){} //
    else if( dialog.getTextValue(answer,"standing wave t0:","%g",dbase.get<real>("standingWaveTimeOffset")) ){} //
    else
    {
      printF("BeamModel::chooseExactSolution:ERROR:unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }



  }
    
  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

  return 0;
}




// =====================================================================================
/// \brief Choose initial conditions for beam models
// =====================================================================================
int BeamModel::
chooseInitialConditions(CompositeGrid & cg, GenericGraphicsInterface & gi )
{

  // real & amplitude=dbase.get<real>("amplitude");
  // real & waveNumber=dbase.get<real>("waveNumber");

  GUIState gui;
  gui.setWindowTitle("Beam inital conditions");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;

  const int maxCommands=40;
  aString cmd[maxCommands];

  aString initialConditionOptions[] = { "zero",
					"exact solution",
                                        "none", 
					"" };

  GUIState::addPrefix(initialConditionOptions,"Initial conditions:",cmd,maxCommands);
  dialog.addOptionMenu("Initial conditions:",cmd,initialConditionOptions,(initialConditionOption=="zero" ? 0 : 
						      initialConditionOption=="exact" ? 1 : 2) );



  // aString pbLabels[] = {"zero initial conditions",
  //                       "exact solution initial conditions",
  // 			""};

  // int numRows=4;
  // dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

  // const int numberOfTextStrings=10;
  // aString textLabels[numberOfTextStrings];
  // aString textStrings[numberOfTextStrings];

  // int nt=0;
    
  // textLabels[nt] = "amplitude:"; sPrintF(textStrings[nt], "%g",amplitude);  nt++; 
  // textLabels[nt] = "wave number:"; sPrintF(textStrings[nt], "%g",waveNumber);  nt++; 
  // textLabels[nt] = "standing wave t0:"; sPrintF(textStrings[nt], "%g",dbase.get<real>("standingWaveTimeOffset"));  nt++; 


  // // null strings terminal list
  // textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  // // addPrefix(textLabels,prefix,cmd,maxCommands);
  // // dialog.setTextBoxes(cmd, textLabels, textStrings);
  // dialog.setTextBoxes(textLabels, textLabels, textStrings);


  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("beamIC>");

  aString prefix = ""; // prefix for commands to make them unique.

  aString answer,buff;

  int len=0;
  for( ;; ) 
  {
	    
    gi.getAnswer(answer,"");
  
    // printF(answer,"answer=[answer]\n",(const char *)answer);

    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);


    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( (len=answer.matches("Initial conditions:")) )
    {
      aString option = answer(len,answer.length()-1);
      if( option=="zero" )
      {
	initialConditionOption="zero";
	printF("Setting initialConditionOption=[%s]\n",(const char*)initialConditionOption);
      }
      else if( option=="exact solution" )
      {
	initialConditionOption="exact";
	printF("Setting initialConditionOption=[%s]\n",(const char*)initialConditionOption);
      }
      else if( option=="none" )
      {
	initialConditionOption="none";
	printF("Setting initialConditionOption=[%s]\n",(const char*)initialConditionOption);
      }
      else 
      {
	printF("BeamModel::chooseInitialConditions:ERROR:unknown response=[%s]\n",(const char*)answer);
	gi.stopReadingCommandFile();
      }
    }
    
    else
    {
      printF("BeamModel::chooseInitialConditions:ERROR:unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

  }
    
  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

  return 0;
}
