#include "DomainSolver.h"
#include "GenericGraphicsInterface.h"
#include "GridFunctionFilter.h"

// ===================================================================================================================
/// \brief Build the dialog that shows the various options for time stepping.
/// \param dialog (input) : graphics dialog to use.
///
/// <b> Dialog Menu Options: </b>\n
///  - method
///    - the list time stepping methods depends on the PDE being solved. 
///  - accuracy
///    - second order accurate : solve the problem to 2nd order accuracy in space.
///    - fourth order accurate : solve the problem to 4th order accuracy in space.
///  - time accuracy 
///    - solve for steady state :
///    - second order accurate in time : 
///    - fourth order accurate in time :
///  - choose grids for implicit : choose a list of grids that should be treated implicitly.
///  - use local time stepping (toggle) : 
// ==================================================================================================================
int DomainSolver::
buildTimeSteppingDialog(DialogData & dialog )
{
  const int numColumns=2;
  
  dialog.setWindowTitle("Time Stepping Parameters");
  dialog.setExitCommand("close time stepping", "close");
  dialog.setOptionMenuColumns(numColumns);

  const int maxNumberOfTimeSteppingMethods=Parameters::numberOfTimeSteppingMethods+10;
  aString *cmd = new aString [maxNumberOfTimeSteppingMethods];

  int nt=0;

  assert( nt<maxNumberOfTimeSteppingMethods );
  cmd[nt]="";

  if( nt>0 )
    dialog.addOptionMenu("method", cmd, cmd, (int)parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod"));

  delete [] cmd;
    
  aString accuracyCommands[] =  { "second order accurate",
				  "fourth order accurate",
				  ""     };
      
  dialog.addOptionMenu("accuracy", accuracyCommands, accuracyCommands,
				      parameters.dbase.get<int >("orderOfAccuracy")==2 ? 0 : 1);

  aString timeAccuracyCommands[] =  { "solve for steady state",
				      "second order accurate in time",
				      "fourth order accurate in time",
				      "sixth order accurate in time",
				      "eighth order accurate in time",
				      ""     };
      
  dialog.addOptionMenu("time accuracy", timeAccuracyCommands, timeAccuracyCommands,
				      parameters.dbase.get<int >("orderOfTimeAccuracy")==2 ? 1 : 
				      parameters.dbase.get<int >("orderOfTimeAccuracy")==4 ? 2 : 0);

  aString predictorOrderCommands[] =  { "default order predictor",
                                        "first order predictor",
					"second order predictor",
					"third order predictor",
					"fourth order predictor",
					 ""     };
      
  dialog.addOptionMenu("predictor order", predictorOrderCommands, predictorOrderCommands,
		       parameters.dbase.get<int>("predictorOrder"));

  // push buttons
  aString pbCommands[] = {"choose grids for implicit",
			  ""};

  const int numRows=1;
  dialog.setPushButtons( pbCommands, pbCommands, numRows ); 


// old way:
//   if( parameters.dbase.get<Parameters::PDE >("pde")==Parameters::incompressibleNavierStokes )
//   {
//     // build a menu to indicate which grids are solved implicitly

//     aString *cmd = new aString[cg.numberOfComponentGrids()+3];
//     aString *label = new aString[cg.numberOfComponentGrids()+3];
//     int *initialState = new int [cg.numberOfComponentGrids()+3];
//     int i;
//     for( i=0; i<cg.numberOfComponentGrids(); i++ )
//     {
//       cmd[i]=cg[i].getName();
//       label[i]="implicit grid: "+cmd[i];
//       initialState[i]=0;       // **** fix ******
//     }
//     i=cg.numberOfComponentGrids();
//     cmd[i]="all"; label[i]="implicit grid: "+cmd[i]; initialState[i]=0; i++;
//     cmd[i]="none"; label[i]="implicit grid: "+cmd[i]; initialState[i]=0; i++;
//     cmd[i]=""; label[i]="";
//     dialog.addPulldownMenu("implicit grids", label, cmd, GI_TOGGLEBUTTON,initialState);
//     delete [] cmd;
//     delete [] label;
//     delete [] initialState;
//   }

  const int maxNumberOfToggleButtons=30;
  aString tbCommands[maxNumberOfToggleButtons]; 
  int tbState[maxNumberOfToggleButtons];
  int ntb=0;
  if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
  {
    tbCommands[ntb]="use local time stepping";
    tbState[ntb] = parameters.dbase.get<int >("useLocalTimeStepping");
    ntb++;
  }
    
  tbCommands[ntb]="adjust dt for moving bodies";
  tbState[ntb]=parameters.dbase.get<bool >("adjustTimeStepForMovingBodies");
  ntb++;

  tbCommands[ntb]="project interface";
  tbState[ntb]=parameters.dbase.get<bool>("projectInterface");
  ntb++;

  tbCommands[ntb]="project rigid body interface";
  tbState[ntb]=parameters.dbase.get<bool>("projectRigidBodyInterface");
  ntb++;

  tbCommands[ntb]="use full implicit system";
  tbState[ntb]=parameters.dbase.get<bool >("useFullSystemForImplicitTimeStepping");
  ntb++;

  tbCommands[ntb]="apply explicit BCs to implicit grids";
  tbState[ntb]=parameters.dbase.get<bool >("applyExplicitBCsToImplicitGrids");
  ntb++;

  tbCommands[ntb]="apply filter";
  tbState[ntb]=parameters.dbase.get<bool>("applyFilter");
  ntb++;

  tbCommands[ntb]="use AF BC Limiter";
  tbState[ntb]=parameters.dbase.get<bool>("applyAFBCLimiter");
  ntb++;

  tbCommands[ntb]="use added mass algorithm";
  tbState[ntb]=parameters.dbase.get<bool>("useAddedMassAlgorithm");
  ntb++;

  tbCommands[ntb]="use added damping algorithm";
  tbState[ntb]=parameters.dbase.get<bool>("useAddedDampingAlgorithm");
  ntb++;

  tbCommands[ntb]="scale added damping with dt";
  tbState[ntb]=parameters.dbase.get<bool>("scaleAddedDampingWithDt");
  ntb++;

  tbCommands[ntb]= "added damping project velocity";
  tbState[ntb]=parameters.dbase.get<bool>("addedDampingProjectVelocity");
  ntb++;

  tbCommands[ntb]="use approximate AMP condition";
  tbState[ntb]=parameters.dbase.get<bool>("useApproximateAMPcondition");
  ntb++;

  tbCommands[ntb]="project added mass velocity";
  tbState[ntb]=parameters.dbase.get<bool>("projectAddedMassVelocity");
  ntb++;

  tbCommands[ntb]="project normal component";
  tbState[ntb]=parameters.dbase.get<bool>("projectNormalComponentOfAddedMassVelocity");
  ntb++;

  tbCommands[ntb]="project beam velocity";
  tbState[ntb]=parameters.dbase.get<bool>("projectBeamVelocity");
  ntb++;

  tbCommands[ntb]="smooth interface velocity";
  tbState[ntb]=parameters.dbase.get<bool>("smoothInterfaceVelocity");
  ntb++;

  tbCommands[ntb]="project velocity on beam ends";
  tbState[ntb]=parameters.dbase.get<bool>("projectVelocityOnBeamEnds");
  ntb++;

  // Put this here for now: We should be a able to compute this based on other parameters
  tbCommands[ntb]="predicted pressure needed";
  tbState[ntb]=parameters.dbase.get<bool>("predictedPressureNeeded");
  ntb++;

  tbCommands[ntb]="use moving grid sub-iterations";
  tbState[ntb]=parameters.dbase.get<bool>("useMovingGridSubIterations");
  ntb++;

  tbCommands[ntb]="use vector implicit system";
  tbState[ntb]=parameters.dbase.get<bool>("useFullSystemForImplicitTimeStepping");
  ntb++;

  tbCommands[ntb]="exit on instability";
  tbState[ntb]=parameters.dbase.get<bool>("exitOnInstablity");
  ntb++;

  assert( ntb<maxNumberOfToggleButtons );
  tbCommands[ntb]="";  // null termination string


  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  // ----- Text strings ------
  const int numberOfTextStrings=31;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  nt=0;
    
  // we may not know whether this is a steady state solver at this point
//   if( !parameters.isSteadyStateSolver() )
//   {
    textCommands[nt] = "final time";  textLabels[nt]=textCommands[nt];
    sPrintF(textStrings[nt], "%g", parameters.dbase.get<real >("tFinal"));  nt++; 
//   }
//   else
//   {
    textCommands[nt] = "max iterations";  textLabels[nt]=textCommands[nt];
    sPrintF(textStrings[nt], "%i", parameters.dbase.get<int >("maxIterations"));  nt++; 
//  }
    
  textCommands[nt] = "cfl"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%5.3f", parameters.dbase.get<real >("cfl")); nt++; 

  textCommands[nt] = "dtMax"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%9.3e", parameters.dbase.get<real >("dtMax")); nt++; 

  textCommands[nt] = "implicit factor"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%9.3e (1=BE,0=FE)", parameters.dbase.get<real >("implicitFactor")); nt++; 

  textCommands[nt] = "recompute dt every"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i steps", parameters.dbase.get<int >("maximumStepsBetweenComputingDt")); nt++; 
    
  textCommands[nt] = "refactor frequency"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i steps", parameters.dbase.get<int >("refactorFrequency")); nt++; 
    
  textCommands[nt] = "slow start cfl"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g", parameters.dbase.get<real >("slowStartCFL")); nt++; 

  textCommands[nt] = "slow start steps"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i", parameters.dbase.get<int>("slowStartSteps")); nt++; 

  textCommands[nt] = "slow start recompute dt"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i (steps)", parameters.dbase.get<int>("slowStartRecomputeDtSteps")); nt++; 

  textCommands[nt] = "slow start"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g (seconds)", parameters.dbase.get<real >("slowStartTime")); nt++; 

  textCommands[nt] = "fixup unused frequency"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i", parameters.dbase.get<int >("fixupFrequency")); nt++; 

  textCommands[nt] = "cflMin, cflMax"; textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g, %g", parameters.dbase.get<real >("cflMin"),parameters.dbase.get<real >("cflMax")); nt++; 

  textCommands[nt] = "preconditioner frequency"; textLabels[nt] =textCommands[nt];
  sPrintF(textStrings[nt],"%i",parameters.dbase.get<int>("preconditionerFrequency")); nt++;

  textCommands[nt] = "number of PC corrections"; textLabels[nt] =textCommands[nt];
  sPrintF(textStrings[nt],"%i",parameters.dbase.get<int>("numberOfPCcorrections")); nt++;

  textCommands[nt] = "max number of AF corrections"; textLabels[nt] =textCommands[nt];
  sPrintF(textStrings[nt],"%i",parameters.dbase.get<int>("numberOfAFcorrections")); nt++;

  textCommands[nt] = "AF correction relative tol"; textLabels[nt] =textCommands[nt];
  sPrintF(textStrings[nt],"%e",parameters.dbase.get<real>("AFcorrectionRelTol")); nt++;

  textCommands[nt] = "number of interface velocity smooths"; textLabels[nt] =textCommands[nt];
  sPrintF(textStrings[nt],"%i",parameters.dbase.get<int>("numberOfInterfaceVelocitySmooths")); nt++;

  textCommands[nt] = "BDF order"; textLabels[nt] =textCommands[nt];
  sPrintF(textStrings[nt],"%i",parameters.dbase.get<int>("orderOfBDF")); nt++;

  textCommands[nt] = "added damping coefficient:"; textLabels[nt] =textCommands[nt];
  sPrintF(textStrings[nt],"%e",parameters.dbase.get<real>("addedDampingCoefficient")); nt++;

  if( parameters.dbase.has_key("upwindOrder") )  // order of upwinding (INS)
  {
    textCommands[nt] = "upwind order:"; textLabels[nt] =textCommands[nt];
    sPrintF(textStrings[nt],"%i (-1=default)",parameters.dbase.get<int>("upwindOrder")); nt++;
  }
 


 // null strings terminal list
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;

}


//\begin{>>DomainSolverInclude.tex}{\subsection{getTimeSteppingOption}}   
int DomainSolver::
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

  bool & useAddedMassAlgorithm = parameters.dbase.get<bool>("useAddedMassAlgorithm");

  bool & useAddedDampingAlgorithm = parameters.dbase.get<bool>("useAddedDampingAlgorithm");
  bool & scaleAddedDampingWithDt = parameters.dbase.get<bool>("scaleAddedDampingWithDt");
  bool & addedDampingProjectVelocity = parameters.dbase.get<bool>("addedDampingProjectVelocity");

  bool & useApproximateAMPcondition = parameters.dbase.get<bool>("useApproximateAMPcondition");
  bool & projectAddedMassVelocity = parameters.dbase.get<bool>("projectAddedMassVelocity");
  bool & projectNormalComponentOfAddedMassVelocity = parameters.dbase.get<bool>("projectNormalComponentOfAddedMassVelocity");
  bool & projectBeamVelocity = parameters.dbase.get<bool>("projectBeamVelocity");
  bool & smoothInterfaceVelocity = parameters.dbase.get<bool>("smoothInterfaceVelocity");
  bool & projectVelocityOnBeamEnds = parameters.dbase.get<bool>("projectVelocityOnBeamEnds");
  bool & useMovingGridSubIterations = parameters.dbase.get<bool>("useMovingGridSubIterations");
  bool & useFullSystemForImplicitTimeStepping = parameters.dbase.get<bool>("useFullSystemForImplicitTimeStepping");
  bool & exitOnInstablity = parameters.dbase.get<bool>("exitOnInstablity");
  int upwindOrder=-1;
  
  int found=true; 
  char buff[180];
  aString answer2;
  int len=0;
  bool applyFilter = false;

  if( answer=="cfl" || answer=="cfl (cfl=)" ) // old way 
  {
    gi.inputString(answer2,sPrintF(buff,"Enter cfl (default value=%e)",parameters.dbase.get<real >("cfl")));
    if( answer2!="" )
      sScanF(answer2,"%e",&parameters.dbase.get<real >("cfl"));
    printF(" cfl=%9.3e\n",parameters.dbase.get<real >("cfl"));
    parameters.dbase.get<real >("cflMin")=parameters.dbase.get<real >("cfl")*.9;
    parameters.dbase.get<real >("cflMax")=min(max(parameters.dbase.get<real >("cfl"),.95),parameters.dbase.get<real >("cfl")*1.1);
  }
  else if( len=answer.matches("cflMin, cflMax") )
  {
    printF("INFO: cflMin and cflMax are used to determine when the time step for implicit\n"
	   "      time stepping should be changed. Decrease cflMin and increase cflMax to\n"
	   "      reduce the frequency of refactors -- but don't make cflMax too large.\n");
      
    sScanF(answer(len,answer.length()),"%e %e",&parameters.dbase.get<real >("cflMin"),&parameters.dbase.get<real >("cflMax"));
    printF(" cflMin=%g cflMax=%g \n",parameters.dbase.get<real >("cflMin"),parameters.dbase.get<real >("cflMax"));
    dialog.setTextLabel("cflMin, cflMax",
				       sPrintF(answer2,"%g, %g",parameters.dbase.get<real >("cflMin"),parameters.dbase.get<real >("cflMax")));
  }
  else if( answer.matches("cfl") )
  {
    sScanF(answer,"cfl %e",&parameters.dbase.get<real >("cfl"));
    printF(" cfl=%9.3e\n",parameters.dbase.get<real >("cfl"));
    parameters.dbase.get<real >("cflMin")=parameters.dbase.get<real >("cfl")*.9;
    parameters.dbase.get<real >("cflMax")=min(max(parameters.dbase.get<real >("cfl"),.95),parameters.dbase.get<real >("cfl")*1.1);
    dialog.setTextLabel("cfl",sPrintF(answer2,"%g", parameters.dbase.get<real >("cfl")));
    dialog.setTextLabel("cflMin, cflMax",
				       sPrintF(answer2,"%g, %g",parameters.dbase.get<real >("cflMin"),parameters.dbase.get<real >("cflMax")));
  }
  else if( answer=="forwardEuler" || answer=="forward Euler" )
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
  else if( dialog.getToggleValue(answer,"use full implicit system",
                                 parameters.dbase.get<bool >("useFullSystemForImplicitTimeStepping")) ){ }
  else if( dialog.getToggleValue(answer,"apply explicit BCs to implicit grids",
                                 parameters.dbase.get<bool >("applyExplicitBCsToImplicitGrids")) ){ }
  // *** scLC
  else if( answer=="Runge-Kutta" )
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::rKutta;
    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::notImplicit;
    parameters.dbase.get<IntegerArray >("timeStepType") = 0; // Explicit by default.
    dialog.getOptionMenu("method").setCurrentChoice(answer);
  }
  // *** ecLC
  else if( answer=="second order accurate" )
  {
    parameters.dbase.get<int >("orderOfAccuracy")=2;
    dialog.getOptionMenu("accuracy").setCurrentChoice(parameters.dbase.get<int >("orderOfAccuracy")==2 ? 0 : 1);
  }
  else if( answer=="fourth order accurate" )
  {
    MappedGrid & mg=cg[0];
    if( mg.discretizationWidth(0)<5 )
    {
      printf("ERROR: This grid does not have a discretization width of at least 5. Current width=%i\n",
	     "   The order of accuracy will not be changed\n", mg.discretizationWidth(0));
    }
    else
    {
      parameters.dbase.get<int >("orderOfAccuracy")=4;
      dialog.getOptionMenu("accuracy").setCurrentChoice(parameters.dbase.get<int >("orderOfAccuracy")==2 ? 0 : 1);
    }
  }
  else if( answer=="sixth order accurate" )
  {
    MappedGrid & mg=cg[0];
    if( mg.discretizationWidth(0)<5 )
    {
      printf("ERROR: This grid does not have a discretization width of at least 5. Current width=%i\n",
	     "   The order of accuracy will not be changed\n", mg.discretizationWidth(0));
    }
    else
    {
      parameters.dbase.get<int >("orderOfAccuracy")=6;
      dialog.getOptionMenu("accuracy").setCurrentChoice(2);
    }
  }
  else if( answer=="eighth order accurate" )
  {
    MappedGrid & mg=cg[0];
    if( mg.discretizationWidth(0)<5 )
    {
      printf("ERROR: This grid does not have a discretization width of at least 5. Current width=%i\n",
	     "   The order of accuracy will not be changed\n", mg.discretizationWidth(0));
    }
    else
    {
      parameters.dbase.get<int >("orderOfAccuracy")=8;
      dialog.getOptionMenu("accuracy").setCurrentChoice(3);
    }
  }
  else if( answer=="second order accurate in time" || 
	   answer=="fourth order accurate in time" ||
	   answer=="solve for steady state" )
  {
    parameters.dbase.get<int >("orderOfTimeAccuracy")= answer=="second order accurate in time" ? 2 : 
      answer== "fourth order accurate in time" ? 4 : 0;
    dialog.getOptionMenu("time accuracy").setCurrentChoice(parameters.dbase.get<int >("orderOfAccuracy")==0 ? 0 :
									  parameters.dbase.get<int >("orderOfAccuracy")==2 ? 1 : 2);
  }
  else if( answer=="implicit" )
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::implicit;
    parameters.dbase.get<int >("orderOfPredictorCorrector")=2;

    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::crankNicolson;
    parameters.setGridIsImplicit();  // by default all grids are implicit for the implicit time stepping method
  }
  else if( answer=="all speed implicit" )
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::implicitAllSpeed;
  else if( answer=="linearized all speed implicit" )
  {
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::implicitAllSpeed;
    parameters.dbase.get<int >("linearizeImplicitMethod")=TRUE;
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
  else if( answer=="slow start time interval" )
  {
    gi.inputString(answer2,sPrintF(buff,"Enter the slow start time interval (default value=%e)",
				   parameters.dbase.get<real >("slowStartTime")));
    if( answer2!="" )
      sScanF(answer2,"%e",&parameters.dbase.get<real >("slowStartTime"));
    printF(" slowStartTime=%9.3e\n",parameters.dbase.get<real >("slowStartTime"));
  }
  else if( answer=="slow start cfl" )
  {
    gi.inputString(answer2,sPrintF(buff,"Enter the slow start initial cfl value (default value=%e)",
				   parameters.dbase.get<real >("slowStartCFL")));
    if( answer2!="" )
      sScanF(answer2,"%e",&parameters.dbase.get<real >("slowStartCFL"));
    printF(" slowStartCFL=%9.3e\n",parameters.dbase.get<real >("slowStartCFL"));
  }
  else if( answer.matches("slow start cfl") )
  {
    sScanF(answer,"slow start cfl %e",&parameters.dbase.get<real >("slowStartCFL"));
    printF("INFO: The slow start initial cfl value=%9.3e\n"
           " You should also set `slow start steps'  or 'slow start' (time) to indicate the slow start interval.\n",
              parameters.dbase.get<real >("slowStartCFL"));
    dialog.setTextLabel("slow start cfl",sPrintF(answer2,"%g", parameters.dbase.get<real >("slowStartCFL")));
  }
  else if ( dialog.getTextValue(answer,"slow start steps","%i",parameters.dbase.get<int>("slowStartSteps")) )
  {
    printF("INFO: Setting slowStartSteps=%i. \n"
           " if slowStartSteps > 0 then the time step will be ramped up from `slow start cfl' to `cfl' based on the"
           " number of steps.\n"
           " You might also want to recompute dt more often by setting the option `recompute dt every <num>'.\n",
           parameters.dbase.get<int>("slowStartSteps"));
  }
  else if ( dialog.getTextValue(answer,"slow start recompute dt","%i",parameters.dbase.get<int>("slowStartRecomputeDtSteps")) )
  {
     printF("INFO: Setting slowStartRecomputeDtSteps=%i. The time-step will be computed every this many steps during the slow start\n"
       " time interval\n");
  }
  else if( answer.matches("slow start") ) // put this after other slow start
  {
    sScanF(answer,"slow start %e",&parameters.dbase.get<real >("slowStartTime"));
    printF(" The slow start time interval=%9.3e\n",parameters.dbase.get<real >("slowStartTime"));
    dialog.setTextLabel("slow start",sPrintF(answer2,"%g (seconds)", parameters.dbase.get<real >("slowStartTime")));
    printF("INFO: if slowStartTime > 0 then the time step will be ramped up from `slow start cfl' to `cfl' based on the current time.\n"
           " You might also want to recompute dt more often by setting the option `recompute dt every <num>'.\n");

  }
  


  else if( dialog.getToggleValue(answer,"use local time stepping",parameters.dbase.get<int >("useLocalTimeStepping")) ){ }
  else if( dialog.getToggleValue(answer,"project interface",parameters.dbase.get<bool>("projectInterface")) ){ }
  else if( dialog.getToggleValue(answer,"project rigid body interface",parameters.dbase.get<bool>("projectRigidBodyInterface")) ){ }
  else if( dialog.getToggleValue(answer,"adjust dt for moving bodies",
						parameters.dbase.get<bool >("adjustTimeStepForMovingBodies")) ){}//
  else if( answer.matches("recompute dt every") )
  {
    printF("The time step, dt,  is recomputed every time the solution is plotted/saved \n");
    printF("In addition you may specify the maximum number of steps that will be taken \n");
    printF("before dt is recomputed. Use this if the solution is not plotted very often. \n");
    sScanF(answer,"recompute dt every %i",&parameters.dbase.get<int >("maximumStepsBetweenComputingDt"));
    printF(" recompute dt=%i\n",parameters.dbase.get<int >("maximumStepsBetweenComputingDt"));
    dialog.setTextLabel("recompute dt every",
				       sPrintF(answer2,"%i steps", parameters.dbase.get<int >("maximumStepsBetweenComputingDt"))); 
  }
  else if( answer=="recompute dt interval" ) // old way
  {
    printF("The time step, dt,  is recomputed every time the solution is plotted/saved \n");
    printF("In addition you may specify the maximum number of steps that will be taken \n");
    printF("before dt is recomputed. Use this if the solution is not plotted very often. \n");
    gi.inputString(answer2,sPrintF(buff,"Enter the max number of steps between recomputing dt"
				   "(current=%i)",parameters.dbase.get<int >("maximumStepsBetweenComputingDt")));
    if( answer2!="" )
      sScanF(answer2,"%i",&parameters.dbase.get<int >("maximumStepsBetweenComputingDt"));
    if( parameters.dbase.get<int >("maximumStepsBetweenComputingDt")>0 )
      printF(" maximumStepsBetweenComputingDt=%i\n",parameters.dbase.get<int >("maximumStepsBetweenComputingDt"));
    else
    {
      printF(" the answer must be a positive integer\n");
      parameters.dbase.get<int >("maximumStepsBetweenComputingDt")=100;
    }
  }
  else if( answer.matches("recompute dt every") )
  {
    printF("The time step, dt,  is recomputed every time the solution is plotted/saved \n");
    printF("In addition you may specify the maximum number of steps that will be taken \n");
    printF("before dt is recomputed. Use this if the solution is not plotted very often. \n");
    sScanF(answer,"recompute dt every %i",&parameters.dbase.get<int >("maximumStepsBetweenComputingDt"));
    printF(" recompute dt=%i\n",parameters.dbase.get<int >("maximumStepsBetweenComputingDt"));
    dialog.setTextLabel("recompute dt every",
				       sPrintF(answer2,"%i steps", parameters.dbase.get<int >("maximumStepsBetweenComputingDt"))); 
  }
  else if( len=answer.matches("refactor frequency") )
  {
    sScanF(answer(len,answer.length()-1),"%i",&parameters.dbase.get<int >("refactorFrequency"));
    printF("Implicit systems will be refactored at least every %i steps.\n",parameters.dbase.get<int >("refactorFrequency"));
    dialog.setTextLabel("refactor frequency",sPrintF("%i steps",parameters.dbase.get<int >("refactorFrequency")));
  }
  else if( len=answer.matches("max iterations") )
  {
    sScanF(answer(len,answer.length()-1),"%i",&parameters.dbase.get<int >("maxIterations"));
    dialog.setTextLabel("max iterations",sPrintF("%i",parameters.dbase.get<int >("maxIterations")));      
  }
  else if( len=answer.matches("plot iterations") )
  {
    sScanF(answer(len,answer.length()-1),"%i",&parameters.dbase.get<int >("plotIterations"));
    dialog.setToggleState("plot iterations",parameters.dbase.get<int >("plotIterations"));      
  }
  else if( answer=="dtMax" )
  {
    gi.inputString(answer2,sPrintF(buff,"Enter dtMax (default value=%e)",parameters.dbase.get<real >("dtMax")));
    if( answer2!="" )
      sScanF(answer2,"%e",&parameters.dbase.get<real >("dtMax"));
    printF(" dtMax=%9.3e\n",parameters.dbase.get<real >("dtMax"));
  }
  else if( answer(0,4)=="dtMax" )
  {
    sScanF(answer,"dtMax %e",&parameters.dbase.get<real >("dtMax")); 
    printF(" dtMax=%9.3e\n",parameters.dbase.get<real >("dtMax"));
    dialog.setTextLabel("dtMax",sPrintF(answer2,"%9.3e", parameters.dbase.get<real >("dtMax")));
  }
  else if( answer=="final time (tf=)" ) // for backward compatibility
  {
    gi.inputString(answer2,sPrintF(buff,"Enter the final time (default value=%e)",parameters.dbase.get<real >("tFinal")));
    if( answer2!="" )
      sScanF(answer2,"%e",&parameters.dbase.get<real >("tFinal"));
    printF(" parameters.tFinal=%9.3e\n",parameters.dbase.get<real >("tFinal"));
  }
  else if( answer.matches("final time") )
  {
    sScanF(answer,"final time %e",&parameters.dbase.get<real >("tFinal"));
    printF(" tFinal=%9.3e\n",parameters.dbase.get<real >("tFinal"));
    dialog.setTextLabel("final time",sPrintF(answer2,"%9.3e", parameters.dbase.get<real >("tFinal")));   // ***** 0 may not be the correct position
  }
  else if( dialog.getToggleValue(answer,"use AF BC Limiter",parameters.dbase.get<bool >("applyAFBCLimiter") ) )
  {
  }
  else if( dialog.getToggleValue(answer,"apply filter",applyFilter) )
  {
    if( applyFilter )
    {
      GridFunctionFilter *& gridFunctionFilter =parameters.dbase.get<GridFunctionFilter*>("gridFunctionFilter");
      if( gridFunctionFilter==NULL )
      {
        gridFunctionFilter = new GridFunctionFilter();
      }
      GridFunctionFilter & filter = *gridFunctionFilter;
      filter.update( gi ); // make changes to any filter parameters

      const int orderOfFilter = filter.orderOfFilter;
      const int filterFrequency = filter.filterFrequency;
      const int numberOfFilterIterations = filter.numberOfFilterIterations;
      const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");

      const real filterCoefficient = filter.filterCoefficient;
      if( filter.filterType==GridFunctionFilter::explicitFilter &&
          orderOfFilter> orderOfAccuracyInSpace )
      {
	printF("INFO: I will extrapolate interpolation neighbours for the explicit filter.\n");
	parameters.dbase.get<int >("extrapolateInterpolationNeighbours")=true;
      }
    }
  }
  else if( dialog.getToggleValue(answer,"use moving grid sub-iterations",useMovingGridSubIterations) )
  {
    if( useMovingGridSubIterations )
      printF("Use multiple sub-iterations per time-step for moving grid problems with light bodies.\n");
    else
      printF("Do NOT use multiple sub-iterations per time-step for moving grid problems with light bodies.\n");
  }
  else if( dialog.getToggleValue(answer,"exit on instability",exitOnInstablity) )
  {
    if( exitOnInstablity )
      printF("exitOnInstablity=true: Exit code if an instability is detected.\n");
    else
      printF("exitOnInstablity=false: do not monitor solution for an instability\n");
  }

  else if( dialog.getToggleValue(answer,"use vector implicit system",useFullSystemForImplicitTimeStepping) )
  {
    if( useFullSystemForImplicitTimeStepping )
      printF("Use a single vector implicit system for the velocity components even if multiple scalar systems may be used.\n");
    else
      printF("Use multiple scalar implicit systems for the velocity components if possible.\n");
  }

  else if( dialog.getToggleValue(answer,"use added mass algorithm",useAddedMassAlgorithm) )
  {
    if( useAddedMassAlgorithm )
      printF("Use the added mass algorithm.\n");
    else
      printF("Do NOT use the added mass algorithm.\n");
  }
  else if( dialog.getToggleValue(answer,"use added damping algorithm",useAddedDampingAlgorithm) )
  {
    if( useAddedDampingAlgorithm )
      printF("Use the added damping algorithm.\n");
    else
      printF("Do NOT use the added damping algorithm.\n");
  }
  else if( dialog.getToggleValue(answer,"scale added damping with dt",scaleAddedDampingWithDt) )
  {
    if( scaleAddedDampingWithDt )
      printF("Scale dn for added damping as sqrt(nu*dt).\n");
    else
      printF("Scale dn for added damping with grid spacing in the normal direction.\n");
  }
  else if( dialog.getToggleValue(answer,"added damping project velocity",addedDampingProjectVelocity) )
  {
    if( addedDampingProjectVelocity )
      printF("Include a final solve for the fluid velocity with the added damping algorithm.\n");
    else
      printF("Do not iInclude a final solve for the fluid velocity with the added damping algorithm.\n");
  }
  else if( dialog.getToggleValue(answer,"use approximate AMP condition",useApproximateAMPcondition) )
  {
    if( useApproximateAMPcondition )
      printF("Use the approximate AMP condition.\n");
    else
      printF("Do NOT use the approximate AMP condition.\n");
  }
  else if( dialog.getToggleValue(answer,"project added mass velocity",projectAddedMassVelocity) )
  {
    if( projectAddedMassVelocity )
      printF("PROJECT the added mass velocity.\n");
    else
      printF("Do NOT project the added mass velocity.\n");
  }
  else if( dialog.getToggleValue(answer,"project normal component",projectNormalComponentOfAddedMassVelocity) )
  {
    if( projectNormalComponentOfAddedMassVelocity )
      printF("project the NORMAL component only of the added mass velocity.\n");
    else
      printF("project all components of the added mass velocity.\n");
  }

  else if( dialog.getToggleValue(answer,"project beam velocity",projectBeamVelocity) )
  {
    if( projectBeamVelocity )
      printF("PROJECT the beam velocity (if we are projected the added mass velocity too).\n");
    else
      printF("Do NOT project the beam velocity.\n");
  }
  else if( dialog.getToggleValue(answer,"project velocity on beam ends",projectVelocityOnBeamEnds) )
  {
    if( projectVelocityOnBeamEnds )
      printF("project the velocity on the ends of beam for the AMP algorithm.\n");
    else
      printF("do NOT project the velocity on the ends of beam for the AMP algorithm.\n");
  }

  else if( dialog.getToggleValue(answer,"smooth interface velocity",smoothInterfaceVelocity) )
  {
    if( smoothInterfaceVelocity )
      printF("SMOOTH the interface velocity.\n");
    else
      printF("Do NOT smooth the interface velocity.\n");
  }
  else if ( dialog.getTextValue(answer,"number of interface velocity smooths", "%i", 
                                parameters.dbase.get<int>("numberOfInterfaceVelocitySmooths"))){ }

  else if ( dialog.getTextValue(answer,"BDF order", "%i", 
                                parameters.dbase.get<int>("orderOfBDF"))){ }

  else if ( dialog.getTextValue(answer,"added damping coefficient:", "%e", 
                                parameters.dbase.get<real>("addedDampingCoefficient")) ) 
  {
    printF("Setting addedDampingCoefficient=%9.3e. This value scales the added damping term in the\n"
           " rigid body equations when coupling to an incompressible flow.\n",
           parameters.dbase.get<real>("addedDampingCoefficient"));
  }
  
  else if( dialog.getTextValue(answer,"upwind order:", "%i",upwindOrder) )
  {
    if( parameters.dbase.has_key("upwindOrder") )  // order of upwinding (INS)
    {
      parameters.dbase.get<int>("upwindOrder")=upwindOrder;
      printF("Setting upwinding order to [%i] (for Cgins)\n",upwindOrder);
    }
    else
    {
      printf("WARNING: Not setting upwindOrder as this option is not valid for this solver\n");
    }
    
  }
    

  else if( dialog.getToggleValue(answer,"predicted pressure needed",parameters.dbase.get<bool>("predictedPressureNeeded")) ){}  // 
  
  else if ( dialog.getTextValue(answer,"preconditioner frequency", "%i", 
                                        parameters.dbase.get<int>("preconditionerFrequency"))){}
  else if ( dialog.getTextValue(answer,"number of PC corrections", "%i", 
                                parameters.dbase.get<int>("numberOfPCcorrections"))){ }
  else if ( dialog.getTextValue(answer,"max number of AF corrections", "%i", 
                                parameters.dbase.get<int>("numberOfAFcorrections"))){ }
  else if ( dialog.getTextValue(answer,"AF correction relative tol", "%e", 
                                parameters.dbase.get<real>("AFcorrectionRelTol"))){ }


  else if( answer=="default order predictor" ||
           answer=="first order predictor" || 
           answer=="second order predictor" ||
           answer=="third order predictor" ||
           answer=="fourth order predictor" )
  {
    parameters.dbase.get<int>("predictorOrder") = 
      answer=="default order predictor" ? 0 : 
      answer=="first order predictor" ? 1 : 
      answer=="second order predictor" ? 2 :
      answer=="third order predictor" ? 3 : 4;
  }
  else if( answer=="choose grids for implicit" )
  {
//\begin{>>setParametersInclude.tex}{\subsubsection{Choosing grids for implicit time stepping.}\label{sec:implicitMenu}}
//\no function header:
//
//  When the option {\tt `choose grids for implicit'} is chosen from the main parameter menu one can
//  specify which grids should be treated implicitly or explicitly with the {\bf implicit} time stepping
//  option. Type a line of the form
//  \begin{verbatim}
//           <grid name>=[explicit][implicit] 
//  \end{verbatim}
//  where {\tt <grid name>} is the name of a grid or {\tt `all'}. Type {\tt `help'} to see the names.
//  Examples:
//  \begin{verbatim}
//       square=explicit
//       all=implicit
//       cylinder=implicit
//  \end{verbatim}
//  Type {\tt `done'} when finished.
//\end{setParametersInclude.tex}
    // *** scLC
    if( (parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")!=Parameters::implicit) &&  
	(parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")!=Parameters::rKutta) )
    {
      printF("WARNING: This option only currently only works for timeSteppingMethod==implicit or timeSteppingMethod==rKutta! \n");
      parameters.setGridIsImplicit(-1,0);
      parameters.dbase.get<IntegerArray >("timeStepType")=0;
    }
    printF("Set grids to be implicit, semi-implicit or explicit. Examples: \n"
	   "  square=explicit \n"
	   "  annulus=semi \n"
	   "  all=implicit \n" );

    gi.appendToTheDefaultPrompt("implicit>");
    aString gridName;
    int implicit;// implicit used to be a bool.
    for( ;; )
    {
      gi.inputString(answer2,"Specify grids to be implicit, semi-implicit or explicit. (or type `help' or `done')");
      if( answer2=="done" || answer2=="exit" )
	break;
      else if( answer2=="help" )
      {
	printF("Specify grids to be implicit, semi-implicit or explicit. Type a string of the form     \n"
	       "                                                                             \n"
	       "       <grid name>=[explicit][semi][implicit]\n"
	       "                                                                             \n"
	       " By default all grids are implicit.                                          \n"
	       " Examples: \n"
	       "     square=explicit                            \n"
	       "     annulus=semi                            \n"
	       "     all=implicit                               \n"
	  );
	// *** ecLC
	printF("Here are the names of the grids: \n");
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  printF(" grid %i : name=%s \n",grid,(const char*)cg[grid].mapping().getName(Mapping::mappingName));
      }
      else
      {
	int length=answer2.length();
	int i,mark=-1;
	for( i=0; i<length; i++ )
	{
	  if( answer2[i]=='=' )
	  {
	    mark=i-1;
	    break;
	  }
	}
	if( mark<0 )
	{
	  printF("unknown form of answer=[%s]. Try again or type `help' for examples.\n",(const char *)answer2);
	  gi.stopReadingCommandFile();
	  continue;
	}
	else
	{
	  gridName=answer2(0,mark);  // this is the name of the grid or `all'
	  Range G(-1,-1);
	  if( gridName=="all" )
	    G=Range(0,cg.numberOfComponentGrids()-1);
	  else
	  {
	    // search for the name of the grid
	    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	    {
	      if( gridName==cg[grid].mapping().getName(Mapping::mappingName) )
	      {
		G=Range(grid,grid);
		break;
	      }
	    }
	  }
	  if( G.getBase()==-1  )
	  {
	    printF("Unknown grid name = <%s> \n",(const char *)gridName);
	    gi.stopReadingCommandFile();
	    continue;
	  }

// *** scLC
	  //This could probably be done in a better way! 
	  if( answer2(mark+2,mark+3)=="im" ){
	    implicit=1;
	  }
	  else if( answer2(mark+2,mark+3)=="se" ){
	    implicit=2;
	  }
	  else {
	    implicit=0;
	  }
		   
	  for( int grid=G.getBase(); grid<=G.getBound(); grid++ )
	  {
	    parameters.setGridIsImplicit(grid,implicit);
	    printF("Setting time stepping to be %s for grid %s\n",
		   (parameters.getGridIsImplicit(grid)==1 ? "implicit" : 
		    (parameters.getGridIsImplicit(grid)==2 ? "semi-implicit" : 
		     "explicit")),
		   (const char*)cg[grid].mapping().getName(Mapping::mappingName));
	  }
	  // *** ecLC
	}
      }
    }
    gi.unAppendTheDefaultPrompt();
  }
//   else if( answer(0,13)=="implicit grid:" )
//   {
//     // answer is of the form "implicit grid: gridName [0,1]"
//     aString number=answer(answer.length()-2,answer.length()-1);
//     int toggleState=0;
//     sScanF(number,"%i",&toggleState);
      
//     answer2=answer(0,answer.length()-3);  // strip off last number
//     aString gridName=answer2(15,answer2.length()-1);
//     printF("implicit grid: gridName=[%s]\n",(const char*)gridName);
//     PullDownMenu& pulldown = dialog.getPulldownMenu(0);


//     if( gridName=="all" || gridName=="none" )
//     {
//       pulldown.setToggleState(cg.numberOfComponentGrids(),0); // turn off "all" state
//       pulldown.setToggleState(cg.numberOfComponentGrids()+1,0); // turn off "none" state

//       toggleState = gridName=="all" ? 1 : 0;
//       for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//       {
// 	parameters.setGridIsImplicit(grid,toggleState);
// 	pulldown.setToggleState(grid,toggleState);
//       }
//     }
//     else
//     {

//       for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//       {
// 	if( gridName==cg[grid].getName() )
// 	{
// 	  parameters.setGridIsImplicit(grid,toggleState);
// 	  printF("Setting time stepping to be %s for grid %s\n",
// 		 (parameters.getGridIsImplicit(grid)==1 ? "implicit" : 
// 		  (parameters.getGridIsImplicit(grid)==2 ? "semi-implicit" : 
// 		   "explicit")),(const char*)cg[grid].getName());

// 	  pulldown.setToggleState(grid,toggleState);
// 	}
//       }
//     }
//   }
  else 
  {
    found =false;
  }


  return found;
}

