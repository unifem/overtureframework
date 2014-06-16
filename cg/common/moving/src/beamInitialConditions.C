#include "BeamModel.h"
#include "display.h"

// standing wave: 
#define W(x,t) (a*sin(k*(x))*cos(w*(t)))
#define Wt(x,t) (-w*a*sin(k*(x))*sin(w*(t)))
#define Wx(x,t) (w*a*cos(k*(x))*cos(w*(t)))
#define Wtx(x,t) (-w*a*cos(k*(x))*sin(w*(t)))

// =====================================================================================
/// \brief Evaluate the standing wave solution
/// \param t (input) : assign values at this time.
/// \param u,v (output) : displacement and velocity
// =====================================================================================
int BeamModel::
getStandingWave( real t, RealArray & u, RealArray & v ) const
{

  const real & amplitude=dbase.get<real>("amplitude");
  const real & waveNumber=dbase.get<real>("waveNumber");

  real beamLength=L;
  real a=amplitude;
  real k=2.*Pi*waveNumber/L;
  real breadth=1.;
  real w = sqrt( elasticModulus*areaMomentOfInertia*pow(k,4)/( density*thickness*breadth ) );

  for (int i = 0; i <= numElem; ++i)
  {
    real xl = ( (real)i /numElem) *  beamLength;
    u(i*2)   = W(xl,t);     // w 
    u(i*2+1) = Wx(xl,t);    // w_x
    
    v(i*2)   = Wt(xl,t);    // w_t 
    v(i*2+1) = Wtx(xl,t);   // w_xt 

  }
  return 0;
}


// =====================================================================================
/// \brief Assign initial conditions
/// \param t (input) : assign values at this time.
/// \param u,v (output) : displacement and velocity
// =====================================================================================
int  BeamModel::
assignInitialConditions( real t, RealArray & u, RealArray & v )
{

  if( initialConditionOption=="zeroInitialConditions" )
  {
    u=0.;
    v=0.;
  }
  else if( initialConditionOption=="standingWave" )
  {
    getStandingWave( t, u, v );
  }
  else
  {
    printF("BeamModel::assignInitialConditions:ERROR:unknown initialConditionOption=[%s]\n",
	   (const char*)initialConditionOption);
    OV_ABORT("error");
  }
  

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
                        "standing wave",
                        "traveling wave FSI-INS",
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
    }
    else if( answer=="standing wave" )
    {
      initialConditionOption="standingWave"; 
    }
    else if( answer=="traveling wave FSI-INS" )
    {
      initialConditionOption="travelingWaveFSI"; 
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
