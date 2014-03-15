// -----------------------------------------------------------------------------------------------------------
// This file contains the functions:
// 
// solve() : main routine to advance and plot the solution
// printStatistics : output final stats
// -----------------------------------------------------------------------------------------------------------

#include "Cgmp.h"
#include "MpParameters.h"
#include "Ogshow.h"
#include "Regrid.h"
#include "ParallelUtility.h"



// ===================================================================================================================
/// \brief The main routine to advance and plot the solution 
/// \details 
// ===================================================================================================================
int 
Cgmp::
solve()
{
  real cpu0 = getCPU();
  cycleZero();
  buildRunTimeDialog();
  const int numberOfDomains = domainSolver.size();
  real & tFinal=parameters.dbase.get<real >("tFinal");
  real & tPrint = parameters.dbase.get<real >("tPrint");
  bool & timeStepHasChanged = parameters.dbase.get<bool>("timeStepHasChanged");
  int & plotOption = parameters.dbase.get<int >("plotOption");
  
    
  RealArray & printArray = parameters.dbase.get<RealArray >("printArray");
  printArray.redim(10); printArray=Parameters::defaultValue;
  
  int nextPrintValue=0;
    
  int & frequencyToSaveInShowFile = parameters.dbase.get<int >("frequencyToSaveInShowFile");
    
  // printF("*********** solve: tInitial=%8.2e gf[0].t=%8.2e\n",parameters.dbase.get<real>("tInitial"),gf[current].t);

  real & nextTimeToPrint = parameters.dbase.get<real >("nextTimeToPrint");
  real t = gf[current].t;
  nextTimeToPrint= !parameters.isSteadyStateSolver() ? t : 0.;  //  ...new time to print solution (print initial time)
  int iPrint=0;
    
  const int maximumNumberOfSteps=max(int( 1e8 ),parameters.dbase.get<int >("maxIterations"));
  int & numberOfSubSteps= parameters.dbase.has_key("numberOfSubSteps") ? parameters.dbase.get<int>("numberOfSubSteps") : parameters.dbase.put<int>("numberOfSubSteps");
  numberOfSubSteps = 1;

  numberOfStepsTaken=max(0,numberOfStepsTaken);  // numberOfStepsTaken==-1 : for initialization steps

  bool finish = plot(t, (plotOption ? 1 : plotOption), tFinal);  // wait when we first start the solve
  finish=0;

  for( int step=0; step<maximumNumberOfSteps && !finish;  )
  {
    real cpuTime=getCPU()-cpu0;
    if( (!parameters.isSteadyStateSolver() && t >= nextTimeToPrint-dt*.25) ||
	( parameters.isSteadyStateSolver() && (parameters.dbase.get<int >("globalStepNumber")+1) > nextTimeToPrint-.1) )
    {
      
      fPrintF(parameters.dbase.get<FILE* >("debugFile")," advance::printTimeStepInfo at t=%20.12e, dt=%20.12e \n",t,dt);
      printTimeStepInfo(step,t,cpuTime);
      
      
      if( frequencyToSaveInShowFile>0 && (iPrint % frequencyToSaveInShowFile == 0) )
	saveShow( gf[current] );  // save the current solution in the show file
      
      // *wdh* 080829 finish = plot(t, plotOption, tFinal);
      int optionIn = step==0 && plotOption ? 1 : plotOption; // wait on first step
      finish=plot(t, optionIn, tFinal);  // optionIn: 0=wait, 1=plot-and-wait, 2=plot-but-don't-wait
      if ( finish ) break;

      if( (!parameters.isSteadyStateSolver() && t >tFinal-.5*dt) ||
	  ( parameters.isSteadyStateSolver() && parameters.dbase.get<int >("globalStepNumber")+1>=parameters.dbase.get<int >("maxIterations")) )
      {
	// we are done (unless tFinal is increased in plot). plot solution at final time
	if( true || plotOption & 1 )
	  plot(t,1, tFinal);

	// tFinal may have been increased, so check again
	if( (!parameters.isSteadyStateSolver() && t >tFinal-.5*dt) ||
	    ( parameters.isSteadyStateSolver() && parameters.dbase.get<int >("globalStepNumber")+1>=parameters.dbase.get<int >("maxIterations")) )
	{ 
	  finish=true;
	  break;
	}
      }

      ForDomain(d) // *wdh* check this 
      {
	// this will close any open sub-file if it contains the max number of solutions allowed.
	// do this here since we save sequences if we finish, so we cannot do this in saveShow.
	if( domainSolver[d]->parameters.dbase.get<Ogshow* >("show")!=NULL )
	{
	  parameters.dbase.get<Ogshow* >("show")->setCurrentFrameSeries(cg.getDomainName(d));
	  domainSolver[d]->parameters.dbase.get<Ogshow* >("show")->endFrame();
	}
      }
      
      if( printArray(nextPrintValue) != (int)Parameters::defaultValue )
	nextTimeToPrint=min(printArray(nextPrintValue++),tFinal);   //  ...new time to print:
      else
      {
	// **** this next line is possibly wrong if tPrint has changed!!  *****
	// ***** or if t/tPrint > MAX_INT
	if( !parameters.isSteadyStateSolver() )
	{
	  nextTimeToPrint=min(ceil(t/tPrint-.5)*tPrint+tPrint,tFinal);   //  ...new time to print:
	}
	else
	  nextTimeToPrint=min(int(nextTimeToPrint+parameters.dbase.get<int >("plotIterations")+.5),parameters.dbase.get<int >("maxIterations"));
      }
      if( debug() & 4 )
      {
	printF("advance: nextTimeToPrint=%18.10e, t=%18.10e \n",nextTimeToPrint,t);
      }
  
	
      iPrint++;
    }

    // actually do the advance now
    // finish = setupAdvance();              *wdh* 080723 : move this down to after dt is computed.

    real dtOld=dt;
    real dtNew = getTimeStep( gf[current] ); //       ===Choose time step====;
    computeNumberOfStepsAndAdjustTheTimeStep(t,tFinal,nextTimeToPrint,numberOfSubSteps,dtNew);
    dt = dtNew;
    ForDomain(d){ domainSolver[d]->dt=dt;} // ** Do this for now **

    timeStepHasChanged = fabs( dt -dtOld ) > dtOld*REAL_EPSILON*100.; // is this ok ? 

    finish = setupAdvance();
    if ( finish ) break;

    // now advance to t=nextTimeToPrint
    advance(tFinal);

    t=gf[current].t;
    finish = finishAdvance();
    
    step++;
  }
  // cycleInfinity()
  
  // do final plotting and file io
  if( plotOption & 1 && !finish )
    plot(t,1, tFinal);
  
  //  tFinal=t;
  
  // Here we save time sequences to the show file
  // (if this was the last frame in a subFile then the sequences were already saved.)
  ForDomain(d)
  {
    Parameters & parameters = domainSolver[d]->parameters;
    if( parameters.dbase.get<Ogshow* >("show")!=NULL &&
	(!parameters.dbase.get<Ogshow* >("show")->isLastFrameInSubFile() || 
	 !parameters.dbase.get<bool >("saveSequencesEveryTime")) )
    {
      // printF("\n *********** saveSequencesToShowFile() *********\n");
      domainSolver[d]->saveSequencesToShowFile();
    
      // time sequence info for moving grids is saved here
      if( parameters.isMovingGridProblem() )
	parameters.dbase.get<MovingGrids >("movingGrids").saveToShowFile();
    }
    if( parameters.dbase.get<Ogshow* >("show")!=NULL )
    {
      // printF("\n *********** AT END parameters.dbase.get<Ogshow* >("show")->endFrame(); *********\n");
      if( domainSolver[d]->parameters.dbase.get<Ogshow* >("show")!=NULL )
      {
	parameters.dbase.get<Ogshow* >("show")->setCurrentFrameSeries(cg.getDomainName(d));
	domainSolver[d]->parameters.dbase.get<Ogshow* >("show")->endFrame();
      }
    }
  } // for each domain
  
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("totalTime"))+=getCPU()-cpu0;
  return 0;
}

// #define OLD_VERSION
// #ifdef OLD_VERSION
// int Cgmp::
// solve2()
// // ============================================================================
// //   Multiphysics solve method
// // ============================================================================
// {

//   real cpu0=getCPU();

//   FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");
//   FILE *pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");
  
//   int & plotOption = parameters.dbase.get<int >("plotOption");

//   Parameters::TimeSteppingMethod & timeSteppingMethod = parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
  
//   timeSteppingMethod=Parameters::forwardEuler;  // ***** do this for now ***
  

//   const int numberOfDomains = domainSolver.size();
  
//   real & tFinal=parameters.dbase.get<real >("tFinal");


//   // --- this is taken from advance.bC --------------

  
//   real t=gf[current].t;

//   if( !parameters.dbase.get<DataBase >("modelData").has_key("initializeAdvance") )
//     parameters.dbase.get<DataBase >("modelData").put<int>("initializeAdvance",true);
//   int & init=parameters.dbase.get<DataBase >("modelData").get<int>("initializeAdvance");


//   real & tPrint = parameters.dbase.get<real >("tPrint");

//   RealArray & printArray = parameters.dbase.get<RealArray >("printArray");
//   printArray.redim(10); printArray=Parameters::defaultValue;
  
//   int nextPrintValue=0;
  
//   int & frequencyToSaveInShowFile = parameters.dbase.get<int >("frequencyToSaveInShowFile");
  
//   real & nextTimeToPrint = parameters.dbase.get<real >("nextTimeToPrint");

//   nextTimeToPrint= !parameters.isSteadyStateSolver() ? t : 0.;  //  ...new time to print solution (print initial time)
//   int iPrint=0;

//   const int maximumNumberOfSteps=max(int( 1e8 ),parameters.dbase.get<int >("maxIterations"));
//   int numberOfSubSteps=1;
//   int finish=0;
  
//   buildRunTimeDialog();

  
//   numberOfStepsTaken=max(0,numberOfStepsTaken);  // numberOfStepsTaken==-1 : for initialization steps
//   for( int d=0; d<domainSolver.size(); d++ )
//     domainSolver[d]->numberOfStepsTaken=max(0,domainSolver[d]->numberOfStepsTaken);

//   //  for( int step=0; step<maximumNumberOfSteps && t<tFinal+dt; step++ )
//   for( int step=0; step<maximumNumberOfSteps;  )
//   {
//     real cpuTime=getCPU()-cpu0;
// //      printF(" numberOfStepsTaken=%i, parameters.dbase.get<int >("globalStepNumber")=%i nextTimeToPrint=%8.2e\n",numberOfStepsTaken,
// //                 parameters.dbase.get<int >("globalStepNumber"),nextTimeToPrint);


//     if( (!parameters.isSteadyStateSolver() && t >= nextTimeToPrint-dt*.25) ||
//         ( parameters.isSteadyStateSolver() && (parameters.dbase.get<int >("globalStepNumber")+1) > nextTimeToPrint-.1) )
//     {

//       fPrintF(debugFile," advance::printTimeStepInfo at t=%20.12e, dt=%20.12e \n",t,dt);
//       printTimeStepInfo(step,t,cpuTime);
      

//       if( frequencyToSaveInShowFile>0 && (iPrint % frequencyToSaveInShowFile == 0) )
//         saveShow( gf[current] );  // save the current solution in the show file

//       if( false )
//       {
// 	FILE *file = stdout;
// 	fprintf(file,"\n ++++++++++++advance before plot+++++++++++++++++++++++++++++++++++++++++++++++++++\n");
// 	for( int grid=0; grid<gf[current].cg.numberOfComponentGrids(); grid++ )
// 	  gf[current].cg[grid].displayComputedGeometry(file);
// 	fprintf(file," +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//       }


//       finish=plot(t, ( (step==0 && plotOption) ? 1 : plotOption), tFinal);  // wait on first step


//       if( finish )
//         break;
//       if( (!parameters.isSteadyStateSolver() && t >tFinal-.5*dt) ||
//           ( parameters.isSteadyStateSolver() && parameters.dbase.get<int >("globalStepNumber")+1>=parameters.dbase.get<int >("maxIterations")) )
//       {
// 	// we are done (unless tFinal is increased in plot). plot solution at final time
// 	if( true || plotOption & 1 )
// 	  plot(t,1, tFinal);

//          // tFinal may have been increased, so check again
//         if( (!parameters.isSteadyStateSolver() && t >tFinal-.5*dt) ||
//             ( parameters.isSteadyStateSolver() && parameters.dbase.get<int >("globalStepNumber")+1>=parameters.dbase.get<int >("maxIterations")) )
// 	{ 
// 	  finish=true;
// 	  break;
// 	}
//       }

//       for( int d=0; d<domainSolver.size(); d++ )
//       {
//         // this will close any open sub-file if it contains the max number of solutions allowed.
//         // do this here since we save sequences if we finish, so we cannot do this in saveShow.
// 	if( domainSolver[d]->parameters.dbase.get<Ogshow* >("show")!=NULL )
// 	{
// 	  for( int d=0; d<domainSolver.size(); d++ )
// 	  {
//             parameters.dbase.get<Ogshow* >("show")->setCurrentFrameSeries(cg.getDomainName(d));
// 	    domainSolver[d]->parameters.dbase.get<Ogshow* >("show")->endFrame();
// 	  }
// 	}
//       }
      
//       if( printArray(nextPrintValue) != (int)Parameters::defaultValue )
//         nextTimeToPrint=min(printArray(nextPrintValue++),tFinal);   //  ...new time to print:
//       else
//       {
//         // **** this next line is possibly wrong if tPrint has changed!!  *****
//         // ***** or if t/tPrint > MAX_INT
// 	if( !parameters.isSteadyStateSolver() )
// 	{
//           nextTimeToPrint=min(ceil(t/tPrint-.5)*tPrint+tPrint,tFinal);   //  ...new time to print:
// 	}
//         else
// 	  nextTimeToPrint=min(int(nextTimeToPrint+parameters.dbase.get<int >("plotIterations")+.5),parameters.dbase.get<int >("maxIterations"));
//       }
//       if( true )
//       {
// 	printF("advance: nextTimeToPrint=%18.10e, t=%18.10e \n",nextTimeToPrint,t);
//       }
      

//       iPrint++;
//     }



//     if(  timeSteppingMethod!=Parameters::implicit 
// 	 && timeSteppingMethod!=Parameters::implicitAllSpeed 
// 	 && timeSteppingMethod!=Parameters::rKutta ) 
//     {  //   ===Choose a new time step====

//       real dtNew= getTimeStep( gf[current] ); //       ===Choose time step====
//       computeNumberOfStepsAndAdjustTheTimeStep(t,tFinal,nextTimeToPrint,numberOfSubSteps,dtNew);
	
//       if( debug() & 1 )
//       {
// 	printF("Cgmp::solve:recompute dt: dt(old)=%8.3e, dtNew = %8.3e numberOfSubSteps=%i\n",
// 	       dt,dtNew,numberOfSubSteps);
//       }
//       dt=dtNew;
//     }
//     else if( timeSteppingMethod==Parameters::implicit || 
//              timeSteppingMethod==Parameters::steadyStateNewton) // not really needed for newton, just did this to make code happy right now kkc 060724
//     {
//       // first compute what the explicit time step would be:

//       const Parameters::TimeSteppingMethod timeSteppingMethodSaved=timeSteppingMethod;
//       timeSteppingMethod=Parameters::adamsPredictorCorrector2;

//       real dtExplicit= getTimeStep( gf[current] ); //       ===Choose time step====

//       timeSteppingMethod=timeSteppingMethodSaved;  // reset

//       // now compute the implicit time step
//       real dtNew= getTimeStep( gf[current] ); //       ===Choose time step====
//       computeNumberOfStepsAndAdjustTheTimeStep(t,tFinal,nextTimeToPrint,numberOfSubSteps,dtNew);
//       printF("recompute dt: dt(old)=%8.3e, dtNew = %8.3e, (explicit dt=%8.3e, ratio=%8.3e) \n",
//            dt,dtNew,dtExplicit,dtNew/dtExplicit);
      
//       // only change the time step if we exceed the cfl limit or we could increase the time step substantially
//       real ratio=dt/dtNew;
//       if( step==0 || ratio < parameters.dbase.get<real >("cflMin")/parameters.dbase.get<real >("cfl") || 
//           ratio > parameters.dbase.get<real >("cflMax")/parameters.dbase.get<real >("cfl") )
//       {
//         dt=dtNew;
//         printF(" ****** time step is being changed for the implicit method *****\n");
//       }
//       else
//       {
//         printF(" ****** time step is NOT being changed for the implicit method *****\n");
//         computeNumberOfStepsAndAdjustTheTimeStep(t,tFinal,nextTimeToPrint,numberOfSubSteps,dt,FALSE);
//       }
//     }
    
//     for( int d=0; d<domainSolver.size(); d++ )
//       domainSolver[d]->updateForNewTimeStep(domainSolver[d]->gf[0],dt);

//     parameters.dbase.get<real >("dt")=dt;
//     if( true || timeSteppingMethod==Parameters::forwardEuler )
//     {
//       // ************* forward Euler ******************

//       for( int i=0; i<numberOfSubSteps; i++ )
//       {
//         const int next = (current+1) %2;
//         for( int d=0; d<domainSolver.size(); d++ )
// 	{
// 	  if( false )
// 	  {
// 	    // old way: 
// 	    GridFunction & gf0 = domainSolver[d]->gf[current];
// 	    GridFunction & gf1 = domainSolver[d]->gf[next];
// 	    realCompositeGridFunction & fn0 = domainSolver[d]->fn[0];
	  
// 	    domainSolver[d]->eulerStep(t,t,t+dt,dt,gf0,gf0,gf1,fn0,fn0,i  ,numberOfSubSteps);
// 	  }
// 	  else
// 	  {
//           // new way:
//             domainSolver[d]->takeOneStep( t,dt,i,numberOfSubSteps );
// 	  }
	  
//           domainSolver[d]->numberOfStepsTaken++; 
// 	}

//         // now apply interface boundary conditions 

// 	// gfIndex[domain] : indicates which solution to use in each domain 
//         std::vector<int> gfIndex(numberOfDomains,next); 
// 	assignInterfaceBoundaryConditions(gfIndex, dt );

// 	t+=dt; 	step++; numberOfStepsTaken++; 
//         current=next;
// 	for( int d=0; d<domainSolver.size(); d++ )
//           domainSolver[d]->current=current;
	
// 	for( int d=0; d<domainSolver.size(); d++ )
// 	  domainSolver[d]->output( domainSolver[d]->gf[current],step );

//  	if( false && (numberOfStepsTaken-1) % parameters.dbase.get<int >("frequencyToSaveSequenceInfo") == 0 )
//  	{  // fix this -- trouble if residual is not there...
// 	  for( int d=0; d<domainSolver.size(); d++ )
// 	  {
// 	    if( !domainSolver[d]->parameters.isAdaptiveGridProblem() )  // fn[0] is not valid for AMR (?) -- fix this
// 	    {
// 	      domainSolver[d]->saveSequenceInfo(t,fn[0]);
// 	    }
// 	  }
//  	}
	

//       }
      
//     }
//     else
//     {
//       cout <<"Cgmg::solve: unknown timeSteppingMethod\n";
//       Overture::abort("error");
//     }


//   }
  
//   if( plotOption & 1 && !finish )
//     plot(t,1, tFinal);
  
//   tFinal=t;
  
//   // Here we save time sequences to the show file
//   // (if this was the last frame in a subFile then the seqeuences were already saved.)
//   for( int d=0; d<domainSolver.size(); d++ )
//   {
//     Parameters & parameters = domainSolver[d]->parameters;
//     if( parameters.dbase.get<Ogshow* >("show")!=NULL &&
// 	(!parameters.dbase.get<Ogshow* >("show")->isLastFrameInSubFile() || 
// 	 !parameters.dbase.get<bool >("saveSequencesEveryTime")) )
//     {
//       // printF("\n *********** saveSequencesToShowFile() *********\n");
//       domainSolver[d]->saveSequencesToShowFile();
    
//       // time sequence info for moving grids is saved here
//       if( parameters.isMovingGridProblem() )
// 	parameters.dbase.get<MovingGrids >("movingGrids").saveToShowFile();
//     }
//     if( parameters.dbase.get<Ogshow* >("show")!=NULL )
//     {
//       // printF("\n *********** AT END parameters.dbase.get<Ogshow* >("show")->endFrame(); *********\n");
//       if( domainSolver[d]->parameters.dbase.get<Ogshow* >("show")!=NULL )
//       {
// 	for( int d=0; d<domainSolver.size(); d++ )
// 	{
// 	  parameters.dbase.get<Ogshow* >("show")->setCurrentFrameSeries(cg.getDomainName(d));
// 	  domainSolver[d]->parameters.dbase.get<Ogshow* >("show")->endFrame();
// 	}
//       }
//     }
//   }
  
//   parameters.dbase.get<RealArray>("timing")(Parameters::totalTime)+=getCPU()-cpu0;
//   return 0;
// }
// #endif

//\begin{>>DomainSolverInclude.tex}{\subsection{printStatistics}} 
int Cgmp::
printStatistics(FILE *file /* = stdout */)
//===================================================================================
// /Description:
// Output timing statistics
//
//\end{DomainSolverInclude.tex}  
//===================================================================================
{
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int myid=max(0,Communication_Manager::My_Process_Number);
  fflush(0);
  Communication_Manager::Sync();

  int numberOfDomains=0;
  ForDomain(d)
  {
    numberOfDomains++;
    domainSolver[d]->printStatistics(file);
  }
  
  // Get the current date
  time_t *tp= new time_t;
  time(tp);
  // tm *ptm=localtime(tp);
  const char *dateString = ctime(tp);




  // const int numSteps=max(1,numberOfStepsTaken);
  const int numSteps=max(1,numberOfStepsTaken);

  RealArray & timing = parameters.dbase.get<RealArray >("timing");
  const std::vector<aString> & timingName = parameters.dbase.get<std::vector<aString> >("timingName");
  const int maximumNumberOfTimings = parameters.dbase.get<int>("maximumNumberOfTimings");

  // count the total number of grid points
  real totalNumberOfGridPoints=0.;
  ForDomain(d)
  {
    Parameters & parameters = domainSolver[d]->parameters;
    CompositeGrid & cg = domainSolver[d]->gf[0].cg;
    if( parameters.dbase.get<real >("numberOfRegrids")==0. )
    {
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	totalNumberOfGridPoints+=domainSolver[d]->numberOfGridPoints[grid];
    }
    else
    {
      totalNumberOfGridPoints=parameters.dbase.get<real >("sumTotalNumberOfGridPoints")/parameters.dbase.get<real >("numberOfRegrids");  // use average
    }
  }
  
  const int totalTimeIndex = parameters.dbase.get<int>("totalTime");
  
  
  // adjust times for waiting
  real timeForWaiting=timing(parameters.dbase.get<int>("timeForWaiting"));
  timing(parameters.dbase.get<int>("timeForPlotting"))-=timeForWaiting;
  timing(parameters.dbase.get<int>("totalTime"))-=timeForWaiting;
  // timing(parameters.dbase.get<int>("timeForAdvance"))-=timeForWaiting;

  timing(totalTimeIndex)=max(timing(totalTimeIndex),REAL_MIN*10.);   // total time 

  // Get max/ave times 
  RealArray maxTiming(timing.dimension(0)),minTiming(timing.dimension(0)),aveTiming(timing.dimension(0));

  ParallelUtility::getMaxValues(&timing(0),&maxTiming(0),maximumNumberOfTimings);
  ParallelUtility::getMinValues(&timing(0),&minTiming(0),maximumNumberOfTimings);
  ParallelUtility::getSums(&timing(0),&aveTiming(0),maximumNumberOfTimings);
  aveTiming/=np;

//   printF(" ***** totalTime = %8.2e  max=%8.2e  ave=%8.2e \n",
//          timing(totalTimeIndex),maxTiming(totalTimeIndex),
// 	 aveTiming(totalTimeIndex));

  real mem=Overture::getCurrentMemoryUsage();
  real maxMem=ParallelUtility::getMaxValue(mem);  // max over all processors
  real minMem=ParallelUtility::getMinValue(mem);  // min over all processors
  real totalMem=ParallelUtility::getSum(mem);  // min over all processors
  real aveMem=totalMem/np;
  real maxMemRecorded=ParallelUtility::getMaxValue(Overture::getMaximumMemoryUsage());
  

  // save a summary to the interface file
  FILE *& interfaceFile =parameters.dbase.get<FILE* >("interfaceFile");
  fPrintF(interfaceFile,"\n"
	  " ***************************************************************************\n"
	  "         **************** %s Interface Summary ***************\n"
	  "               number of steps = %i\n",
	  (const char*)getClassName(),numberOfStepsTaken);
  ((MpParameters&)parameters).displayInterfaceInfo(interfaceFile);
  fPrintF(interfaceFile," ***************************************************************************\n");

  // **********************
  if( myid==0 )
  {
    for( int fileio=0; fileio<2; fileio++ )
    {
      FILE *output = fileio==0 ? parameters.dbase.get<FILE* >("logFile") : file;

      // print statistics for cgmp 
      fPrintF(output,"\n"
	      "*****************************************************************************************\n");
      fPrintF(output,
	      "             %s Version 0.1                                 \n"
	      "             -----------------                              \n"
	      "             %s                                             \n",
	      (const char*)getClassName(),(const char*)dateString   );
    

      fPrintF(output," numberOfDomains=%i, processors=%i\n",numberOfDomains,np);

      fPrintF(output," numberOfStepsTaken=%i, total time = %e \n",numberOfStepsTaken,
	      parameters.dbase.get<RealArray>("timing")(totalTimeIndex));
    
      Parameters::TimeSteppingMethod & timeSteppingMethod= 
              parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
      fprintf(file,"\n"
	      " cfl = %f, tFinal=%e, tPrint = %e                                 \n"
	      " Time stepping method: %s\n"
	      " solveCoupledInterfaceEquations = %i, useMixedInterfaceConditions=%i\n"
              " interface-tolerance=%8.2e, interface-omega=%8.2e\n"
	      ,
	      parameters.dbase.get<real >("cfl"),
	      parameters.dbase.get<real >("tFinal"),
	      parameters.dbase.get<real >("tPrint"),
	      (const char*)Parameters::timeSteppingName[timeSteppingMethod],
	      (int)parameters.dbase.get<bool>("solveCoupledInterfaceEquations"),
              (int)parameters.dbase.get<bool>("useMixedInterfaceConditions"),
              parameters.dbase.get<real>("interfaceTolerance"),parameters.dbase.get<real>("interfaceOmega"));

      fPrintF(output," Interface info written to file interfaceFile.log (Use debug=3 to see convergence rates)\n");


      if( parameters.dbase.get<real >("numberOfRegrids")==0 )
      {
	fprintf(output,
                "\n"
                "         ---%s Summary : %s --- \n"
                "            %s"          
		"  ==== numberOfStepsTaken =%i, number of grids=%i, number of gridpts =%g, processors=%i ==== \n"
		"  ==== memory per-proc: [min=%g,ave=%g,max=%g](Mb), max-recorded=%g (Mb), total=%g (Mb)\n"
		"   Timings:           (ave-sec/proc:) seconds    sec/step   sec/step/pt     %%     [max-s/proc] [min-s/proc]\n",
		(const char*)getClassName(),(const char*)getName(),
                dateString,
		numSteps,cg.numberOfComponentGrids(),totalNumberOfGridPoints,np,
                minMem,aveMem,maxMem,maxMemRecorded,totalMem);
      }
      else
      {
	int levels = 1, ratio=1;
	aString loadBalanceName="off";
	if( parameters.dbase.get<Regrid* >("regrid")!=NULL )
	{
	  levels=parameters.dbase.get<Regrid* >("regrid")->getDefaultNumberOfRefinementLevels();
	  ratio=parameters.dbase.get<Regrid* >("regrid")->getRefinementRatio();
	  if( parameters.dbase.get<Regrid* >("regrid")->loadBalancingIsOn() )
	    loadBalanceName=parameters.dbase.get<Regrid* >("regrid")->getLoadBalancer().getLoadBalancerTypeName();
	}
      
	fprintf(output,
                "\n"
                "         ---%s Summary : %s --- \n"
                "           %s"          
		"  ==== numberOfStepsTaken =%i, processors=%i\n"
		"  ==== AMR levels=%i, AMR ratio=%i, load-balance=%s\n"
		"  ==== number of grids:   [min=%g,max=%g,ave=%g] (number of regrid steps=%g)\n"
		"  ==== number of gridpts: [min=%g,max=%g,ave=%g], \n"
		"  ==== memory per-proc: [min=%g,ave=%g,max=%g](Mb), max-recorded=%g (Mb), total=%g (Mb)\n"
		"   Timings:           (ave-sec/proc:) seconds    sec/step   sec/step/pt     %%     [max-s/proc] [min-s/proc]\n",
		(const char*)getClassName(),(const char*)getName(),
                dateString,
		numSteps,np,
		levels,ratio,(const char*)loadBalanceName,
		parameters.dbase.get<real >("minimumNumberOfGrids"),
		parameters.dbase.get<real >("maximumNumberOfGrids"),
		parameters.dbase.get<real >("totalNumberOfGrids")/parameters.dbase.get<real >("numberOfRegrids"),
		parameters.dbase.get<real >("numberOfRegrids"),
		parameters.dbase.get<real >("minimumNumberOfGridPoints"),parameters.dbase.get<real >("maximumNumberOfGridPoints"),
		parameters.dbase.get<real >("sumTotalNumberOfGridPoints")/parameters.dbase.get<real >("numberOfRegrids"),
		minMem,aveMem,maxMem,maxMemRecorded,totalMem);
      }
    
  
      int nSpace=35;
      aString dots="........................................................................";
      if( maxTiming(0)==0. )
	maxTiming(0)=REAL_MIN;
      for( int i=0; i<maximumNumberOfTimings; i++ )
	if( timingName[i]!="" && aveTiming(i)!=0. )    
	  fprintf(output,"%s%s%10.3e  %10.3e  %10.3e   %7.3f  %10.3e  %10.3e\n",(const char*)timingName[i],
		  (const char*)dots(0,max(0,nSpace-timingName[i].length())),
		  aveTiming(i),aveTiming(i)/numSteps,aveTiming(i)/numSteps/totalNumberOfGridPoints,
		  100.*aveTiming(i)/aveTiming(totalTimeIndex),maxTiming(i),minTiming(i));

      if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit )
	fprintf(output,"implicit time stepping: average number of iterations to solve implicit system =%5.1f/step\n",
		real(parameters.dbase.get<int >("numberOfIterationsForImplicitTimeStepping"))/max(1,numSteps));

      if( poisson )//parameters.dbase.get<Parameters::PDE >("pde")==Parameters::incompressibleNavierStokes ||
	//	  parameters.dbase.get<Parameters::PDE >("pde")==Parameters::allSpeedNavierStokes )
	fprintf(output,"pressure equation: average number of iterations to solve =%f/solve (%3.2f solves/step)\n",
		real(parameters.dbase.get<int >("numberOfIterationsForConstraints"))/max(1,parameters.dbase.get<int >("numberOfSolvesForConstraints")),
		real(parameters.dbase.get<int >("numberOfSolvesForConstraints"))/max(1,numSteps));
    
#ifdef USE_PPP
      fprintf(output," Total: messages sent=%i messages received=%i\n",
              Diagnostic_Manager::getNumberOfMessagesSent(),Diagnostic_Manager::getNumberOfMessagesReceived());
      fprintf(output," Per step: messages sent=%6.1f messages received=%6.1f\n",
              Diagnostic_Manager::getNumberOfMessagesSent()/max(1.,numSteps),
              Diagnostic_Manager::getNumberOfMessagesReceived()/max(1.,numSteps));
#endif

      fPrintF(output,
	      "*****************************************************************************************\n\n");
      
    }
  }
  
  
  if( np>1 )
  {
    // In parallel we print some timings for each processor
    for( int fileio=0; fileio<2; fileio++ )
    {
      FILE *output = fileio==0 ? parameters.dbase.get<FILE* >("logFile") : file;
      fflush(output);
      fPrintF(output,"\n"
	      " ------- Summary: Timings per processor -----------\n"
	      "   p   ");
      for( int i=0; i<maximumNumberOfTimings; i++ )
      { // output a short-form name (7 chars)
	if( timingName[i]!="" && maxTiming(i)!=0. )  
	{
          aString shortName="       ";
          int m=0;
	  for( int s=0; m<7 && s<timingName[i].length(); s++ ) 
	  { // strip off blanks
	    if( timingName[i][s]!=' ' ) {shortName[m]=timingName[i][s]; m++;} //
	  }
          fPrintF(output,"%7.7s ",(const char*)shortName);
	}
      }
      fPrintF(output,"\n");
      fflush(output);
      RealArray timingLocal(timing.dimension(0));
      for( int p=0; p<np; p++ )
      {
	// Note -- it did not work very well to have processor p try to write results, so instead
        // we copy results to processor 0 to print 
        timingLocal=timing;
        broadCast(timingLocal,p);  // send timing info from processor p   -- don't need a broad cast here **fix**
	fPrintF(output,"%4i : ",p);
	for( int i=0; i<maximumNumberOfTimings; i++ )
	{
	  if( timingName[i]!="" && maxTiming(i)!=0. )    
	    fPrintF(output,"%7.1e ",timingLocal(i));
	}
	fflush(output);
	fPrintF(output,"\n");
      }
      fPrintF(output,"\n");
      fflush(output);

    }
  }
  
  printF("\n >>>> See the log file for further timings, memory usage and other statistics <<<< \n\n");

  delete tp;

  // reset times
  timing(parameters.dbase.get<int>("timeForPlotting"))+=timeForWaiting;
  timing(parameters.dbase.get<int>("totalTime"))+=timeForWaiting;
  // timing(Parameters::timeForAdvance)+=timeForWaiting;
}
