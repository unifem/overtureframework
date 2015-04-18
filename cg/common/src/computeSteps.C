#include "DomainSolver.h"

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{computeNumberOfStepsAndAdjustTheTimeStep}} 
void DomainSolver::
computeNumberOfStepsAndAdjustTheTimeStep(const real & t,
                                         const real & tFinal,
					 const real & nextTimeToPrint, 
					 int & numberOfSubSteps, 
					 real & dtNew,
                                         const bool & adjustTimeStep /* = true */ )
// =====================================================================================
// /Description:
// determine how many steps we should take to reach the next output time, but do
// not take more than `maximumStepsBetweenComputingDt'
//
// /nextTimeToPrint (input):  
// /numberOfSteps (output): Take this many sub-steps
// /dtNew (input/output) : On input this is the time step. On output this value may be changed
//     if adjustTimeStep=TRUE.
// /adjustTimeStep (input) : if TRUE alter the time step to exactly reach the nexTimeToPrint.
//\end{CompositeGridSolverInclude.tex}  
// =====================================================================================
{
  const int & debug = parameters.dbase.get<int >("debug");
  

  if( dtNew<=0. )
  {
    printF("\n\ncomputeNumberOfStepsAndAdjustTheTimeStep:ERROR: dtNew<=0., dtNew=%e\n",dtNew);
    printF(" t=%e, tFinal=%e, nextTimeToPrint=%e, tPrint=%e \n",t,tFinal,nextTimeToPrint,parameters.dbase.get<real >("tPrint"));
    Overture::abort("error");
  }
  
  // We recompute dt at least this often:
  int maximumStepsBetweenComputingDt = parameters.dbase.get<int >("maximumStepsBetweenComputingDt");
  if( parameters.dbase.get<real >("slowStartTime")>0. || parameters.dbase.get<int  >("slowStartSteps")>0 )
  {
    // for a slow start we may recompute more often:
    maximumStepsBetweenComputingDt=min(maximumStepsBetweenComputingDt,parameters.dbase.get<int>("slowStartRecomputeDtSteps"));
  }
  
  const int & globalStepNumber = parameters.dbase.get<int >("globalStepNumber");

  if( parameters.isSteadyStateSolver() )
  {
    // for steady state problems we take the following number of steps:
    numberOfSubSteps=min(int(nextTimeToPrint-(globalStepNumber+1)+.5),parameters.dbase.get<int >("maxIterations")-(globalStepNumber+1));

    // check time step more often at the start of a steady state computation
    numberOfSubSteps=min(10+ (globalStepNumber+1),numberOfSubSteps);

    numberOfSubSteps=min(numberOfSubSteps,maximumStepsBetweenComputingDt);
    
    return;
  }

  real timeInterval=min(tFinal,nextTimeToPrint)-t;
  if( timeInterval<=0. )
  {
    printF("adjustTheTimeStep: WARNING: timeInterval=%18.10e, nextTimeToPrint=%18.10e, t=%18.10e tPrint=%10.3e\n",
           timeInterval,nextTimeToPrint,t,parameters.dbase.get<real >("tPrint"));


    timeInterval+=parameters.dbase.get<real >("tPrint");
    if( timeInterval<=0. )
    {
      printF("There is something wrong here\n");
      OV_ABORT("error");
    }
    
  }
  

  if( timeInterval/dtNew > INT_MAX ) 
  {
    printF("computeNumberOfStepsAndAdjustTheTimeStep:ERROR: time step too small? dtNew=%e, timeInterval=%e \n",dtNew,
           timeInterval);
    printF(" t=%e, tFinal=%e, nextTimeToPrint=%e, tPrint=%e \n",t,tFinal,nextTimeToPrint,parameters.dbase.get<real >("tPrint"));
    Overture::abort("error");
  }
  
  real dtNewSave=dtNew;
  
  const real tPlus=.9999;
  numberOfSubSteps=max(1,int(timeInterval/dtNew+tPlus));   // used to be +.5 or +1.

  if( numberOfSubSteps > maximumStepsBetweenComputingDt )
  {
    if( numberOfSubSteps > maximumStepsBetweenComputingDt+1 )
    {
      // no need to adjust dt in this case since we will recompute dt anyway
      numberOfSubSteps=maximumStepsBetweenComputingDt;
    }
    else
    { 

      // *wdh* 070529 -- this was wrong since sometimes the time step would increase
      // numberOfSubSteps=maximumStepsBetweenComputingDt;
      // dtNew=timeInterval/numberOfSubSteps;

      // adjust the time step in this case since we are very close to taking the correct number of steps
      // reduce the time step to force another full step 
      dtNew=(timeInterval/numberOfSubSteps)*(real(maximumStepsBetweenComputingDt)/real(numberOfSubSteps));
	
      
      if( debug & 2 )
	printF(" ---- adjust time step: t=%12.6e, limit numberOfSubSteps=%i by max allowed=%i, timeInterval=%e, "
	       "dtNew=%e, (input: dtNew=%e)\n",
	       t, numberOfSubSteps,maximumStepsBetweenComputingDt,timeInterval,dtNew,dtNewSave);


      numberOfSubSteps=maximumStepsBetweenComputingDt;

    }
  }
  else if( adjustTimeStep )
  {
    dtNew=timeInterval/numberOfSubSteps;

    if( true || debug & 2 )
      printF(" ---- adjust time step to reach tprint: timeInterval=%e, numberOfSubSteps=%i, dtNew=%10.4e\n",
	     timeInterval,numberOfSubSteps,dtNew);
  }

  if( dtNew > dtNewSave*1.01 )
  {
    printF(" ---- adjust time step:ERROR:  dtNew=%8.2e (input) is less than adjusted value dtNew=%8.2e !\n"
           "  numberOfSubSteps=%i, maximumStepsBetweenComputingDt=%i, timeInterval/dtNewSave=%8.2e\n"
           "  timeInterval/numberOfSubSteps=%8.2e\n",
           dtNewSave,dtNew,numberOfSubSteps,maximumStepsBetweenComputingDt,timeInterval/dtNewSave,timeInterval/numberOfSubSteps);
    Overture::abort("error");
  }
  
  if( dtNew<0. )
  {
    printF("computeNumberOfStepsAndAdjustTheTimeStep:ERROR: dtNew<=0., dtNew=%e, numberOfSubSteps=%i\n",dtNew,
           numberOfSubSteps);
    printF(" t=%e, tFinal=%e, nextTimeToPrint=%e, tPrint=%e \n",t,tFinal,nextTimeToPrint,parameters.dbase.get<real >("tPrint"));
    Overture::abort("error");
  }

}

