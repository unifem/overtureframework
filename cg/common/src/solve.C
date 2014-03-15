#include "DomainSolver.h"
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "LineSolve.h"
#include "Regrid.h"
#include "ParallelUtility.h"
#include <time.h>

int DomainSolver::
solve()    
// ===================================================================================================================
/// \brief Solve equations to time tFinal.
// ==================================================================================================================
{
  real cpu0 = getCPU();
  
  advance(parameters.dbase.get<real >("tFinal"));

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("totalTime"))+=getCPU()-cpu0;
  return 0;
}

// ===================================================================================================================
/// \brief Advance one time-step. This function is used by the multi-physics solver Cgmp.
/// \param t (input) : current time.
/// \param dt (input) : time step.
/// \param stepNumber (input) : current counter for the step number.
/// \param numberOfSubSteps (input) : number of sub-steps to take.
// ===================================================================================================================
void DomainSolver::
takeOneStep( real & t, real & dt, int stepNumber, int & numberOfSubSteps )
{
  Parameters::TimeSteppingMethod & timeSteppingMethod = 
                       parameters.dbase.get<Parameters::TimeSteppingMethod>("timeSteppingMethod");

  if( !parameters.dbase.get<DataBase >("modelData").has_key("initializeAdvance") )
    parameters.dbase.get<DataBase >("modelData").put<int>("initializeAdvance",true);
  int & init=parameters.dbase.get<DataBase >("modelData").get<int>("initializeAdvance");


  if( timeSteppingMethod==Parameters::forwardEuler )
  {
    const int next = (current+1) %2;

    GridFunction & gf0 = gf[current];
    GridFunction & gf1 = gf[next];
    realCompositeGridFunction & fn0 = fn[0];
	  
    eulerStep(t,t,t+dt,dt,gf0,gf0,gf1,fn0,fn0,stepNumber  ,numberOfSubSteps);
  }
  else if( timeSteppingMethod==Parameters::adamsBashforth2 ||
	   timeSteppingMethod==Parameters::adamsPredictorCorrector2 ||
	   timeSteppingMethod==Parameters::adamsPredictorCorrector4 )
  {
    advanceAdamsPredictorCorrector( t,dt, numberOfSubSteps,init,stepNumber ); 
  }
  else if( timeSteppingMethod==Parameters::implicit )
  {
    if ( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::trapezoidal )
      advanceTrapezoidal(t,dt,numberOfSubSteps,init,stepNumber);
    else
      advanceImplicitMultiStep( t,dt, numberOfSubSteps,init,stepNumber );

  }
  else
  {
    printF("DomainSolver::takeOneStep:ERROR: timeSteppingMethod=%i is not yet implemented\n",(int)timeSteppingMethod);
    Overture::abort("DomainSolver::takeOneStep:ERROR");
  }

}




// ===================================================================================================================
/// \brief Set the time to integrate to.
/// \param tFinal_ (input) : integrate to this time this final time.
// ===================================================================================================================
int DomainSolver::
setFinalTime(const real & tFinal_ )
{
  parameters.dbase.get<real >("tFinal")=tFinal_;
  return 0;
}



// ===================================================================================================================
/// \brief Output timing statistics. 
/// \details This information is normally printed at the end of the run.
/// \param file (input) : output to this file.
// ===================================================================================================================
int DomainSolver::
printStatistics(FILE *file /* = stdout */)
{
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int myid=max(0,Communication_Manager::My_Process_Number);
  fflush(0);
  Communication_Manager::Sync();
  
  FILE *& logFile =parameters.dbase.get<FILE* >("logFile");

  fPrintF(logFile,"\n"
	  " -------------- Final Statistics for %s  ---------------\n",(const char*)getClassName());

  if( np>1 )
  { // output parallel distribution: 
    aString buff;
    cg.displayDistribution(sPrintF(buff,"%s summary",(const char*)getClassName()),logFile);
  }
  
  if( poisson!=0 )
  {
    fPrintF(logFile,"\n"
	    " ----------------- Poisson Solver -------------------------------------- \n");
    poisson->printStatistics(logFile);
  }
  for( int imp=0; imp<numberOfImplicitSolvers; imp++ )
  {
    fPrintF(logFile,"\n"
	    "&&&&&&&&&&&&&&&&&&&&&&&&&&& Implicit Solver %i &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& \n",imp);
    implicitSolver[imp].printStatistics(logFile);
    fPrintF(logFile,"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
  }

  
  // output timings from Interpolant.
  Interpolant::printStatistics(logFile);

  printMemoryUsage(logFile); // note: communication required here

  GenericMappedGridOperators::printBoundaryConditionStatistics(logFile);

  if( np>1 && parameters.isAdaptiveGridProblem() && parameters.dbase.get<Regrid* >("regrid")!=NULL )
  { // print load balancing statistics
    parameters.dbase.get<Regrid* >("regrid")->getLoadBalancer().printStatistics(logFile);
  }
  

  // const int numSteps=max(1,numberOfStepsTaken);
  const int numSteps=max(1,parameters.dbase.get<int >("globalStepNumber"));

  RealArray & timing = parameters.dbase.get<RealArray >("timing");
  //  const aString *timingName = parameters.dbase.get<ArraySimpleFixed<aString,Parameters::maximumNumberOfTimings,1,1,1> >("timingName");
//  const ArraySimpleFixed<aString,Parameters::maximumNumberOfTimings,1,1,1> &timingName = parameters.dbase.get<ArraySimpleFixed<aString,Parameters::maximumNumberOfTimings,1,1,1> >("timingName");

  const std::vector<aString> & timingName = parameters.dbase.get<std::vector<aString> >("timingName");
  const int maximumNumberOfTimings = parameters.dbase.get<int>("maximumNumberOfTimings");

  const Parameters::TimeSteppingMethod & timeSteppingMethod = 
           parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
  
  // count the total number of grid points
  real totalNumberOfGridPoints=0.;
  int grid;
  if( parameters.dbase.get<real >("numberOfRegrids")==0. )
  {
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      totalNumberOfGridPoints+=numberOfGridPoints[grid];
  }
  else
  {
    totalNumberOfGridPoints=parameters.dbase.get<real >("sumTotalNumberOfGridPoints")/parameters.dbase.get<real >("numberOfRegrids");  // use average
  }
  
  
  // *****************************
  // printP("+++printStat: timeForMovingGrids=%8.2e (int=%i) name=%s\n",
  //    parameters.dbase.get<RealArray>("timing")(Parameters::timeForMovingGrids),
  //     (int)Parameters::timeForMovingGrids,(const char*)timingName[Parameters::timeForMovingGrids] );

  // adjust times for waiting
  const int totalTimeIndex = parameters.dbase.get<int>("totalTime");

  real timeForWaiting=timing(parameters.dbase.get<int>("timeForWaiting"));
  timing(parameters.dbase.get<int>("timeForPlotting"))-=timeForWaiting;
  timing(totalTimeIndex)-=timeForWaiting;
  timing(parameters.dbase.get<int>("timeForAdvance"))-=timeForWaiting;

  // Compute timeForOther -- times we have not counted
  real totalMeasured=0;  // sum of times we have measured
  for( int i=0; i<maximumNumberOfTimings; i++ )
  {
    // Add up the cpu times we have measured -- exclude total time and any names that beginning with a blank
    // since these are assumed to be sub-times of a larger category
    if( i!=totalTimeIndex && timingName[i]!="" && timingName[i][0]!=' ' )
    {
      // printF("time: i=%i, %s cpu=%8.2e\n",i,(const char*)timingName[i],timing(i));
      totalMeasured+=timing(i);
    }
    
  }
  timing(parameters.dbase.get<int>("timeForOther"))=timing(totalTimeIndex)-totalMeasured;
  printP(" ****  CPU: totalTime=%9.3e totalMeasured=%9.3e, timeForOther=%8.2e \n",
	 timing(totalTimeIndex),totalMeasured,timing(parameters.dbase.get<int>("timeForOther")));
  

  timing(totalTimeIndex)=max(timing(totalTimeIndex),REAL_MIN*10.);   // total time 

  // Get max/ave times 
  RealArray maxTiming(timing.dimension(0)),minTiming(timing.dimension(0)),aveTiming(timing.dimension(0));

//   for( int i=0; i<Parameters::maximumNumberOfTimings; i++ )
//     maxTiming(i)=ParallelUtility::getMaxValue(timing(i));   // max over all processors  -- is this the right thing to do?
  
  ParallelUtility::getMaxValues(&timing(0),&maxTiming(0),maximumNumberOfTimings);
  ParallelUtility::getMinValues(&timing(0),&minTiming(0),maximumNumberOfTimings);
  ParallelUtility::getSums(&timing(0),&aveTiming(0),maximumNumberOfTimings);
  aveTiming/=np;

  real mem=Overture::getCurrentMemoryUsage();
  real maxMem=ParallelUtility::getMaxValue(mem);  // max over all processors
  real minMem=ParallelUtility::getMinValue(mem);  // min over all processors
  real totalMem=ParallelUtility::getSum(mem);  // min over all processors
  real aveMem=totalMem/np;
  real maxMemRecorded=ParallelUtility::getMaxValue(Overture::getMaximumMemoryUsage());

  const real realsPerGridPoint = (totalMem*1024.*1024.)/totalNumberOfGridPoints/sizeof(real);

  // Here we determine some values that are used to compute the TTS -- normalized time-to-solution
  const real tf = gf[current].t;
  const real ti = parameters.dbase.get<real>("tInitial");
  const real dtAve = (tf-ti)/max(1.,numSteps);
  // here is a normalized CFL number 
  const real targetGridSpacing = parameters.dbase.get<real>("targetGridSpacing"); // background or "target" grid spacing
  const real velocityScale = parameters.dbase.get<real>("velocityScale");  // fix me 
  const real dtScale = targetGridSpacing/max(REAL_MIN*10.,velocityScale);  // "CFL=1" time step for normalizing actual dt

  const int numberOfDimensions = gf[current].cg.numberOfDimensions();
  const real dtNormalized = dtAve/dtScale; // = pow(totalNumberOfGridPoints,1./numberOfDimensions)*max(REAL_MIN*10.,dtAve);


  // **********************
  if( myid==0 )
  {

    // Get the current date
    time_t *tp= new time_t;
    time(tp);
    // tm *ptm=localtime(tp);
    const char *dateString = ctime(tp);

    for( int fileio=0; fileio<2; fileio++ )
    {
      FILE *output = fileio==0 ? logFile : file;

      if( parameters.dbase.get<real >("numberOfRegrids")==0 )
      {
        // === No AMR ===
	fprintf(output,
                "\n"
                "         ---%s Summary : %s --- \n"
                "            %s"          
                "            Grid file=[%s]\n"
		"  ==== numberOfStepsTaken =%i, number of grids=%i, number of gridpts =%g, Np=%i (procs) ==== \n",
		(const char*)getClassName(),(const char*)getName(),
                dateString,
                (const char*)parameters.dbase.get<aString>("nameOfGridFile"),
		numSteps,cg.numberOfComponentGrids(),totalNumberOfGridPoints,np);
      }
      else
      {
        // === AMR ===
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
		"  ==== number of gridpts: [min=%g,max=%g,ave=%g], \n",
		(const char*)getClassName(),(const char*)getName(),
                dateString,
		numSteps,np,
		levels,ratio,(const char*)loadBalanceName,
		parameters.dbase.get<real >("minimumNumberOfGrids"),
		parameters.dbase.get<real >("maximumNumberOfGrids"),
		parameters.dbase.get<real >("totalNumberOfGrids")/parameters.dbase.get<real >("numberOfRegrids"),
		parameters.dbase.get<real >("numberOfRegrids"),
		parameters.dbase.get<real >("minimumNumberOfGridPoints"),parameters.dbase.get<real >("maximumNumberOfGridPoints"),
		parameters.dbase.get<real >("sumTotalNumberOfGridPoints")/parameters.dbase.get<real >("numberOfRegrids"));
      }

      fprintf(output,"  ==== tFinal=%8.3e, tInitial=%8.3e, dt(ave)=%7.2e ====\n",tf,ti,dtAve);

      fprintf(output,
	      "  ==== memory/proc: [min=%g,ave=%g,max=%g]Mb, max-recorded=%g Mb, total=%g Mb, %5.1f reals/(grid-pt)\n"
	      "   Timings:           (ave-sec/proc:) seconds   sec/step    TPSM     TTS       %%    [max-s/proc] [min-s/proc]\n",
	      minMem,aveMem,maxMem,maxMemRecorded,totalMem,realsPerGridPoint);
  
      int nSpace=35;
      aString dots="........................................................................";
      if( maxTiming(0)==0. )
	maxTiming(0)=REAL_MIN;
      for( int i=0; i<maximumNumberOfTimings; i++ )
	if( timingName[i]!="" && aveTiming(i)!=0. )    
	  fprintf(output,"%s%s%9.2e  %9.2e %8.1e %8.1e  %7.3f  %9.2e    %9.2e\n",(const char*)timingName[i],
		  (const char*)dots(0,max(0,nSpace-timingName[i].length())),
		  aveTiming(i),
                  aveTiming(i)/numSteps,
                  np*aveTiming(i)/numSteps/(totalNumberOfGridPoints/1.e6),
                  np*aveTiming(i)/numSteps/(totalNumberOfGridPoints/1.e6)/max(REAL_MIN*10.,dtNormalized),
		  100.*aveTiming(i)/aveTiming(totalTimeIndex),maxTiming(i),minTiming(i));

      fprintf(output,
	      "\n"
	      "TPSM = Np * (seconds/step)/( number-of-grid-points / 10^6 ), (time-per-step-per-million-grid-points),\n"
	      "TTS = TPSM/dtNormalized  (normalized measure of total time to solve),\n"
	      "dtNormalized = %8.1e = dtAve/( targetGridSpacing/velocityScale ), targetGridSpacing=%8.1e, velocityScale=%8.1e,\n",
	      dtNormalized,targetGridSpacing,velocityScale );

      if( timeSteppingMethod==Parameters::implicit )
	fprintf(output,"implicit time stepping: average number of iterations to solve implicit system =%5.1f/step\n",
		real(parameters.dbase.get<int >("numberOfIterationsForImplicitTimeStepping"))/max(1.,numSteps));

      if( timeSteppingMethod==Parameters::implicit || 
          timeSteppingMethod==Parameters::adamsPredictorCorrector2 || 
          timeSteppingMethod==Parameters::adamsPredictorCorrector4)
      {
	fprintf(output,"Predictor-corrector time stepping: average number of corrector steps =%5.2f.\n",
		real(parameters.dbase.get<int>("totalNumberOfPCcorrections"))/max(1.,numSteps));
      }
      

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
    }

    delete tp;

  }
  
  
  if( np>1 )
  {
    // In parallel we print some timings for each processor
    for( int fileio=0; fileio<2; fileio++ )
    {
      FILE *output = fileio==0 ? logFile : file;
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

  // reset times
  timing(parameters.dbase.get<int>("timeForPlotting"))+=timeForWaiting;
  timing(parameters.dbase.get<int>("totalTime"))+=timeForWaiting;
  timing(parameters.dbase.get<int>("timeForAdvance"))+=timeForWaiting;

  return 0;
}

// ===================================================================================================================
/// \brief Output information about the memory usage. 
/// \details This information is normally printed at the end of the run.
/// \param file (input) : output to this file.
// ===================================================================================================================
int DomainSolver::
printMemoryUsage(FILE *file /* = stdout */)
{
  const int np= max(1,Communication_Manager::numberOfProcessors());

  // These next functions may require communication
  //  const real totalArrayMemoryInUse= Diagnostic_Manager::getTotalArrayMemoryInUse();
  // const real totalMemoryInUse     = Diagnostic_Manager::getTotalMemoryInUse();

  real mem=Overture::getCurrentMemoryUsage();
  real maxMem=ParallelUtility::getMaxValue(mem);  // max over all processors
  real minMem=ParallelUtility::getMinValue(mem);  // min over all processors
  real totalMem=ParallelUtility::getSum(mem);     // sum of all processors
  real aveMem=totalMem/np;
  real maxMemRecorded=ParallelUtility::getMaxValue(Overture::getMaximumMemoryUsage());

  //  if( parameters.dbase.get<int >("myid")!=0 ) return 0;  // ** *wdh* 050409 not here -- display below

  const real megaByte=1024.*1024;

  // count the total number of grid points
  real totalNumberOfGridPoints=0.;
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    totalNumberOfGridPoints+=numberOfGridPoints[grid];
  
  int nSpace=30;
  aString dots="...................................................................";

  fPrintF(file,"\n\n"
	  "  --------------------------------------------------------- \n"
	  "   Memory usage                   Mbytes    real/point   percent   \n");
  enum
  {
    memoryForCompositeGrid,
    memoryForOperators,
    memoryForInterpolant,
    memoryForCompositeGridSolver,
    memoryForGridFunctions,
    memoryForPressureSolver,
    memoryForImplicitSolver,
    memoryForWorkSpace,
    memoryForLineSolver,
    memoryTotal,
    numberOfMemoryItems
  };

  aString memoryName[numberOfMemoryItems]=
  {
    "CompositeGrid and u",
    "finite difference operators",
    "Interpolant",
    "CompositeGridSolver",
    "  grid functions",
    "  pressure solver",
    "  implicit solver",
    "  work space",
    "  line solver",
    "total (of above items)"
  };
  real memory[numberOfMemoryItems];
  memory[memoryTotal]=0.;
  memory[memoryForCompositeGrid]=gf[current].cg.sizeOf(); // +gf[current].u.sizeOf();
  memory[memoryTotal]+=memory[memoryForCompositeGrid];
  
  memory[memoryForOperators]=finiteDifferenceOperators.sizeOf();
  memory[memoryTotal]+=memory[memoryForOperators];

  if( cg.rcData->interpolant!=NULL )
  {
    memory[memoryForInterpolant]=cg.rcData->interpolant->sizeOf();
    memory[memoryTotal]+=memory[memoryForInterpolant];
  }

  memory[memoryForCompositeGridSolver]=sizeOf(file);
  memory[memoryTotal]+=memory[memoryForCompositeGridSolver];

  memory[memoryForGridFunctions]=0.;
  int i;
  for( i=0; i<DomainSolver::maximumNumberOfGridFunctionsToUse; i++ )
    memory[memoryForGridFunctions]+=gf[i].sizeOf()-
                                    gf[i].cg.sizeOf();   // do not count the cg;

  for( i=0; i<4; i++ )
    memory[memoryForGridFunctions]+=fn[i].sizeOf();

  memory[memoryForGridFunctions]+=pressureRightHandSide.sizeOf();
  if( pp!=NULL )
     memory[memoryForGridFunctions]+=p().sizeOf();  

  memory[memoryForPressureSolver]=poissonCoefficients.sizeOf();
  if( poisson!=NULL )
    memory[memoryForPressureSolver]+=poisson->sizeOf();
  
  memory[memoryForImplicitSolver]=0.;
  for( i=0; i<numberOfImplicitSolvers; i++ )
    memory[memoryForImplicitSolver]+=implicitSolver[i].sizeOf();
  memory[memoryForImplicitSolver]+=coeff.sizeOf();


  memory[memoryForWorkSpace]=0.;
//   if( pWorkSpace!=NULL )
//   {
//     real wSize=0.;
//     // only count unique work spaces
//     for( grid=0; grid<cg.numberOfComponentGrids(); grid++)
//     {
//       if( grid==0 || min(abs( workSpaceIndex(grid)-workSpaceIndex(Range(0,grid-1)))) !=0 )
// 	wSize+=workSpace(grid).sizeOf();
//     }
//     memory[memoryForWorkSpace]=wSize;
//   }
 

  memory[memoryForLineSolver]=0.;
  if( pLineSolve!=NULL ) 
     memory[memoryForLineSolver]=pLineSolve->sizeOf();
  
  for( i=0; i<numberOfMemoryItems; i++ )
  {
    fPrintF(file," %s%s%9.2f  %9.2f     %5.1f  \n",
	    (const char*)memoryName[i],(const char*)dots(0,max(0,nSpace-memoryName[i].length())),
	    memory[i]/megaByte,memory[i]/sizeof(real)/totalNumberOfGridPoints, 100.*memory[i]/memory[memoryTotal]);
  }
  fPrintF(file,"\n **Bytes per grid point = %9.3e/%i = %9.3e\n",
	  memory[memoryTotal],totalNumberOfGridPoints,memory[memoryTotal]/totalNumberOfGridPoints);
  fPrintF(file," **number of reals per grid point = %9.3e\n\n",
          memory[memoryTotal]/sizeof(real)/totalNumberOfGridPoints);
  
  if( parameters.dbase.get<int >("debug") & 2 )
    finiteDifferenceOperators.sizeOf();

  if( true || parameters.dbase.get<int >("debug") & 2 )
  {
    fPrintF(file,"\n +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry(file);
    fPrintF(file," +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  }
  fPrintF(file,"===== memory per-proc: [min=%g,ave=%g,max=%g](Mb), max-recorded=%g (Mb), total=%g (Mb)\n",
	  minMem,aveMem,maxMem,maxMemRecorded,totalMem);

//     fPrintF(file,"************************************************************************* \n"
// 	    " Here is what memory is used by A++ arrays \n"
// 	    " total array memory in use = %10.3fM, (%10.3fM including overhead)\n"
// 	    " (This info is obtained by running cg with the `memory' option)\n"
// 	    "*************************************************************************\n\n",
// 	    totalArrayMemoryInUse/megaByte,totalMemoryInUse/megaByte);
    
  return 0;
}

