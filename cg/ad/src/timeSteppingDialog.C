#include "Cgad.h"
#include "GenericGraphicsInterface.h"

int Cgad::
buildTimeSteppingDialog(DialogData & dialog )
// ====================================================================================
// /Description:
//    Build the dialog that shows the various options for time stepping
// ====================================================================================
{
  // Add the "method" dialog so it appears first

  const int maxNumberOfTimeSteppingMethods=Parameters::numberOfTimeSteppingMethods+10;
  aString *cmd = new aString [maxNumberOfTimeSteppingMethods];

  int nt=0;
  cmd[nt]="forward Euler";    nt++;
  cmd[nt]="adams order 2";    nt++;
  cmd[nt]="adams PC";         nt++;
  cmd[nt]="adams PC order 4"; nt++;
  cmd[nt]="midpoint";         nt++;
  cmd[nt]="Runge-Kutta";      nt++;
  cmd[nt]="implicit";         nt++;
  cmd[nt]="variable time step PC";  nt++;
  cmd[nt]="steady state RK";  nt++;
  cmd[nt]="steady state RK-line";  nt++;
  cmd[nt]="adi";  nt++;

  assert( nt<maxNumberOfTimeSteppingMethods );
  cmd[nt]="";

  if( nt>0 )
    dialog.addOptionMenu("method", cmd, cmd, (int)parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod"));

  // build all the generic options now
  DomainSolver::buildTimeSteppingDialog(dialog);

  // Over-ride the defaults
  dialog.setWindowTitle("Cgad Time Stepping Parameters");
  dialog.setExitCommand("close time stepping", "close");
  dialog.setOptionMenuColumns(1);


  return 0;
}

//\begin{>>DomainSolverInclude.tex}{\subsection{getTimeSteppingOption}}   
int Cgad::
getTimeSteppingOption(const aString & answer,
		      DialogData & dialog )  
//================================================================================
// /Description:
//    Look for time stepping options in the string "answer"
//
// /answer (input) : check this command for a change to the time stepping options
//
// /Return value: return 1 if the command was found, 0 otherwise.
//\end{DomainSolverInclude.tex}  
//====================================================================
{
  

  assert( parameters.dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  int found=true; 
  char buff[180];
  aString answer2;
  int len=0;

  if( answer=="forwardEuler" || answer=="forward Euler" )
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::forwardEuler;
    dialog.getOptionMenu("method").setCurrentChoice("forward Euler");
  }
  else if( answer=="adamsBashforth2" || answer=="adams order 2" )
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::adamsBashforth2;
    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::notImplicit;
    dialog.getOptionMenu("method").setCurrentChoice("adams order 2");
  }
  else if( answer=="adamsPredictorCorrector2" || answer=="adams PC")
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::adamsPredictorCorrector2;
    parameters.dbase.get<int >("orderOfPredictorCorrector")=2;
    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::notImplicit;
    dialog.getOptionMenu("method").setCurrentChoice("adams PC");
  }
  else if( answer=="adamsPredictorCorrector4" || answer=="adams PC order 4")
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::adamsPredictorCorrector4;
    parameters.dbase.get<int >("orderOfPredictorCorrector")=4;
    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::notImplicit;
    dialog.getOptionMenu("method").setCurrentChoice("adams PC order 4");
  }
  else if( answer=="variable time step PC" )
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::variableTimeStepAdamsPredictorCorrector;
    parameters.dbase.get<int >("orderOfPredictorCorrector")=2;
    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::notImplicit;
    dialog.getOptionMenu("method").setCurrentChoice(answer);
  }
  else if( answer=="midpoint" )
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::midPoint;
    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::notImplicit;
    dialog.getOptionMenu("method").setCurrentChoice(answer);
  }
  else if( answer.matches("steady state RK") )
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::steadyStateRungeKutta;
    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::notImplicit;
    parameters.dbase.get<int >("useLocalTimeStepping")=true;
    if( answer=="steady state RK-line" )
    {
      parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::lineImplicit;
      parameters.setGridIsImplicit();
    }
      
    dialog.getOptionMenu("method").setCurrentChoice(answer);
    dialog.setToggleState("use local time stepping",parameters.dbase.get<int >("useLocalTimeStepping"));  
  }
  else if( answer.matches("steady state Newton") )
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::steadyStateNewton;
    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::backwardEuler;
    parameters.dbase.get<int >("refactorFrequency")=1;
    parameters.dbase.get<int >("useLocalTimeStepping")=true;
      
    dialog.getOptionMenu("method").setCurrentChoice(answer);
    dialog.setToggleState("use local time stepping",parameters.dbase.get<int >("useLocalTimeStepping"));  
  }
  // *** scLC
  else if( answer=="Runge-Kutta" )
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::rKutta;
    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::notImplicit;
    parameters.dbase.get<IntegerArray >("timeStepType") = 0; // Explicit by default.
    dialog.getOptionMenu("method").setCurrentChoice(answer);
  }
  // *** ecLC
  else if( answer=="implicit" )
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::implicit;
    parameters.dbase.get<int >("orderOfPredictorCorrector")=2;

    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::crankNicolson;
    parameters.setGridIsImplicit();  // by default all grids are implicit for the implicit time stepping method
  }
  else if( answer=="adi" )
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::adi;
    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::crankNicolson;
    parameters.setGridIsImplicit();  // by default all grids are implicit for the adi time stepping method
  }
  else if( answer=="implicit factor" )
  {
    gi.inputString(answer2,sPrintF(buff,"Enter the implicit factor .5=CN, 1=BE (default value=%e)",
				   parameters.dbase.get<real >("implicitFactor")));
    if( answer2!="" )
      sScanF(answer2,"%e",&parameters.dbase.get<real >("implicitFactor"));
    printF(" implicitFactor=%9.3e\n",parameters.dbase.get<real >("implicitFactor"));
  }
  else if( answer.matches("implicit factor") )
  {
    sScanF(answer,"implicit factor %e",&parameters.dbase.get<real >("implicitFactor")); 
    printF(" implicit factor=%9.3e\n",parameters.dbase.get<real >("implicitFactor"));
    dialog.setTextLabel("implicit factor",
				       sPrintF(answer2,"%9.3e (1=BE,0=FE)", parameters.dbase.get<real >("implicitFactor")));
  }
  else 
  {
    found =false;
  }

  // lastly look for base class options
  if( !found )
    return DomainSolver::getTimeSteppingOption(answer,dialog);

  return found;
}

