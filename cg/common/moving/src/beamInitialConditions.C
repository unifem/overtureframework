#include "BeamModel.h"
#include "display.h"
#include "TravelingWaveFsi.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"

// standing wave: 
#define W(x,t) (amplitude*sin(k*(x))*cos(w*(t)))
#define Wt(x,t) (-w*amplitude*sin(k*(x))*sin(w*(t)))
#define Wtt(x,t) (-w*w*amplitude*sin(k*(x))*cos(w*(t)))

#define Wx(x,t) (k*amplitude*cos(k*(x))*cos(w*(t)))
#define Wtx(x,t) (-w*k*amplitude*cos(k*(x))*sin(w*(t)))
#define Wttx(x,t) (-w*w*k*amplitude*cos(k*(x))*cos(w*(t)))

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

  const bool & twilightZone = dbase.get<bool>("twilightZone");
  real beamLength=L;
  real breadth=1.;

  if( initialConditionOption=="standingWave" )
  {
    return getStandingWave( t,u,v,a );
  }
  else if( initialConditionOption=="travelingWaveFSI" )
  {
    getTravelingWaveFSI( t,u,v,a  );
  }
  else if( twilightZone ) // ****************************************** this is copied from below -- FIX ME --
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
/// \brief Assign initial conditions
/// \param t (input) : assign values at this time.
/// \param u,v,a (output) : displacement, velocity and acceleration
// =====================================================================================
int BeamModel::
assignInitialConditions( real t, RealArray & u, RealArray & v, RealArray & a )
{

  printF("BeamModel::assignInitialConditions for %s, t=%9.3e\n",(const char*)initialConditionOption,t);
  real beamLength=L;
  
  if( u.getLength(0)==0 )
    u.redim(2*numElem+2);
  if( v.getLength(0)==0 )
    v.redim(2*numElem+2);
  if( a.getLength(0)==0 )
    a.redim(2*numElem+2);
  

  if( initialConditionOption=="zeroInitialConditions" )
  {
    u=0.;
    v=0.;
    a=0.;
  }
  else if( initialConditionOption=="standingWave" )
  {
    getStandingWave( t, u, v, a  );
  }
  else if( initialConditionOption=="travelingWaveFSI" )
  {
    getTravelingWaveFSI( t, u, v, a  );
  }
  else if( initialConditionOption=="oldTravelingWaveFsi" )
  {
    setExactSolution( t,u,v,a);
  }
  else if( initialConditionOption=="twilightZone" )
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

      a(i*2)   = ae(i,0,0,0);     // w_tt 
      a(i*2+1) = axe(i,0,0,0);    // w_xtt

    }
  }
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
/// \brief Choose initial conditions for beam models
// =====================================================================================
int BeamModel::
chooseInitialConditions(CompositeGrid & cg, GenericGraphicsInterface & gi )
{

  real & amplitude=dbase.get<real>("amplitude");
  real & waveNumber=dbase.get<real>("waveNumber");

  GUIState gui;
  gui.setWindowTitle("Beam inital conditions");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;


  aString pbLabels[] = {"zero initial conditions",
                        "twilight zone initial conditions",
                        "standing wave",
                        "traveling wave FSI-INS",
                        "old traveling wave FSI-INS",
			""};

  int numRows=4;
  dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

  const int numberOfTextStrings=10;
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
    
  textLabels[nt] = "amplitude:"; sPrintF(textStrings[nt], "%g",amplitude);  nt++; 
  textLabels[nt] = "wave number:"; sPrintF(textStrings[nt], "%g",waveNumber);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  // addPrefix(textLabels,prefix,cmd,maxCommands);
  // dialog.setTextBoxes(cmd, textLabels, textStrings);
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


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
    else if( answer=="zero initial conditions" )
    {
      initialConditionOption="zero";
      printF("Setting initialConditionOption=[%s]\n",(const char*)initialConditionOption);
    }
    else if( answer=="twilight zone initial conditions" )
    {
      initialConditionOption="twilightZone";
      printF("Setting initialConditionOption=[%s]\n",(const char*)initialConditionOption);
    }
    else if( answer=="standing wave" )
    {
      initialConditionOption="standingWave"; 
      printF("Setting initialConditionOption=[%s]\n",(const char*)initialConditionOption);
    }
    else if( answer=="old traveling wave FSI-INS" )
    {
      // *old way*
      initialConditionOption="oldTravelingWaveFsi";
    }
    else if( answer=="traveling wave FSI-INS" )
    {
      initialConditionOption="travelingWaveFSI"; 
      printF("Setting initialConditionOption=[%s]\n",(const char*)initialConditionOption);

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
    else if( dialog.getTextValue(answer,"amplitude:","%g",amplitude) ){} //
    else if( dialog.getTextValue(answer,"wave number:","%g",waveNumber) ){} //
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
