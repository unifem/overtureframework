// -----------------------------------------------------------------------------------------------------------
// This file contains the functions:
// 
// multiDomainAdvance( real &t, real & tFinal )
// 
//
// Notes on multi-domain (implicit/explicit) time stepping
//
//
// assignInterfaceBoundaryConditions.C
//  - initializeInterfaces(std::vector<int> gfIndex)
//  - assignInterfaceBoundaryConditions(std::vector<int> gfIndex, const real dt )
//  - initializeInterfaceBoundaryConditions( real t, real dt, std::vector<int> gfIndex )
//  - assignInterfaceRightHandSide( int d, real t, real dt, int correct, std::vector<int> gfIndex ) 
//  - getInterfaceResiduals( real t, real dt, std::vector<int> gfIndex, real & maxRes )
// 
// ** For interfaces see also:
// common/src/interfaceBoundaryConditions.C : for iterative implicit interface conditions
//  - setInterfaceBoundaryCondition( GridFaceDescriptor & info )
//  - iterativeInterfaceRightHandSide( InterfaceOptionsEnum option, GridFaceDescriptor & info, 
//                                       int gfIndex, real t )
// common/src/assignInterfaceBoundaryConditions.C
//
//
// -----------------------------------------------------------------------------------------------------------
#include "Cgmp.h"
#include "Ogshow.h"
#include "Interface.h"
#include "AdvanceOptions.h"
#include "MpParameters.h"

// ===================================================================================================================
/// \brief Multi-domain explicit/implicit time stepping
/// \details 
/// \param t (input/output) : current time
/// \param tFinal (input) : integrate to this final time.
// ==================================================================================================================
int Cgmp::
multiDomainAdvance( real &t, real & tFinal )
{  
  const MpParameters::MultiDomainAlgorithmEnum multiDomainAlgorithm = 
                              parameters.dbase.get<MpParameters::MultiDomainAlgorithmEnum>("multiDomainAlgorithm");
  
  if( multiDomainAlgorithm==MpParameters::stepAllThenMatchMultiDomainAlgorithm )
  {
    // This new algorithm supports AMR: 
    return multiDomainAdvanceNew(t,tFinal);
  }
  else if(  multiDomainAlgorithm==MpParameters::multiStageAlgorithm )
  {
    // User-defined multi-stage algorithm 
    return multiStageAdvance(t,tFinal);
  }


  real cpu0=getCPU();

  FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");
  FILE *& interfaceFile =parameters.dbase.get<FILE* >("interfaceFile");
  
  // Is this next right or should we expect each DomainSolver to know whether it needs to initialize?
  if( !parameters.dbase.get<DataBase >("modelData").has_key("initializeAdvance") )
    parameters.dbase.get<DataBase >("modelData").put<int>("initializeAdvance",true);
  int & init=parameters.dbase.get<DataBase >("modelData").get<int>("initializeAdvance");

  const int numberOfDomains = domainSolver.size(); 
  const std::vector<int> & domainOrder =parameters.dbase.get<std::vector<int> >("domainOrder");
  int numberOfSubSteps=parameters.dbase.get<int>("numberOfSubSteps");

  InterfaceList & interfaceList = parameters.dbase.get<InterfaceList>("interfaceList");
  const bool solveCoupledInterfaceEquations = parameters.dbase.get<bool>("solveCoupledInterfaceEquations");

  bool & timeStepHasChanged = parameters.dbase.get<bool>("timeStepHasChanged");

  if( true || debug() & 4 )
    printF(" ---- Cgmp::multiDomainAdvance ---- t=%e, dt=%e, tFinal=%e, timeStepHasChanged=%i\n",t,dt,tFinal,(int)timeStepHasChanged);
  if( debug() & 2 )
    fprintf(debugFile," *** Cgmp::multiDomainAdvance: t=%e, dt=%e, tFinal=%e, timeStepHasChanged=%i *** \n",t,dt,tFinal,(int)timeStepHasChanged);


  bool alwaysSetBoundaryData=true;
  #ifdef USE_PPP
    alwaysSetBoundaryData=false;
  #endif

  if( init )
  {
    // -- check if we are using AMR --
    ForDomainOrdered(d)
    {
      if( domainSolver[d]->parameters.isAdaptiveGridProblem() )
      {
	parameters.dbase.get<bool>("adaptiveGridProblem")=true;
	break;
      }
    }
    if( parameters.isAdaptiveGridProblem() )
    {
      if( true || debug() & 2 )
	printF("Cgmp::::multiDomainAdvance: AMR is being used in at least one domain\n");
    }
  }
  

  if( init || ( timeStepHasChanged && parameters.dbase.get<bool>("useMixedInterfaceConditions")) )
  {
    // When we iterate to solve the decoupled interface conditions we need to specify what sub-set
    // of the interface conditions we solve on each domain.
    if( alwaysSetBoundaryData || !solveCoupledInterfaceEquations )
    {
      std::vector<int> gfIndex(numberOfDomains,current); // ** fix this ** get gfIndex from each domain solver

      // For AMR we need to re-define the interface since the AMR grids for the initial conditions were created
      // after the interfaces were initially constructed
      if( parameters.isAdaptiveGridProblem() )
	initializeInterfaces(gfIndex); // this will re-define the interfaces

      initializeInterfaceBoundaryConditions( t,dt,gfIndex );
    }
  }
  
  if( init )
  {
    //  Assign the RHS for the interface equations on domain at t=0 *wdh* 081105
    //  so that we can apply the boundary conditions at t=0 
    ForDomainOrdered(d)
    {
      // Assign the RHS for the interface equations on domain d 
      if( alwaysSetBoundaryData || !solveCoupledInterfaceEquations )
      {
        std::vector<int> gfIndex(numberOfDomains,current); // ** fix this ** get gfIndex from each domain solver
        const int correct=0;
	assignInterfaceRightHandSide( d, t, dt, correct, gfIndex );
      }
    }
      
    ForDomainOrdered(d)
    {
      domainSolver[d]->initializeTimeStepping( t,dt );
    }

    init=false;
  }


  std::vector<int> gfIndexCurrent(numberOfDomains,-1); // current GridFunction used by domain solver d
  std::vector<int> gfIndexNext(numberOfDomains,-1);    // next time level GridFunction for domain d 

  // initialResidual : holds the initial residual
  // oldResidual     : holds previous max residual on the interface
  std::vector<real> maxResidual, oldResidual, initialResidual, firstResidual;
  bool interfaceIterationsHaveConverged=false;
  std::vector<AdvanceOptions> advanceOptions(numberOfDomains);  

  for( int i=0; i<numberOfSubSteps; i++ )
  {
    parameters.dbase.get<int >("globalStepNumber")++;

    const int next = (current+1) %2;

    std::vector<int> gfIndex(numberOfDomains,-1);  // keep track of which GridFunction to use for each domain
    int numberOfRequiredCorrectorSteps=0;          // The minimum number of corrector steps that we must take
    bool gridHasChanged=false;
    ForDomainOrdered(d)
    { 
      // The next call will return the number of corrector steps needed by this domain solver.
      // gfIndexCurrent[d],gfIndexNext[d] are also returned here.

      domainSolver[d]->startTimeStep( t,dt,gfIndexCurrent[d],gfIndexNext[d],advanceOptions[d] );

      gfIndex[d]=gfIndexCurrent[d];
      numberOfRequiredCorrectorSteps=max(numberOfRequiredCorrectorSteps,advanceOptions[d].numberOfCorrectorSteps);
      gridHasChanged = gridHasChanged || advanceOptions[d].gridChanges != AdvanceOptions::noChangeToGrid;
    }
      
    if( gridHasChanged )
    { 
      // we need to redefine the interfaces if the grid has changed
      // -- do this here for now --
      printF("\n *-*-* Cgmp::multiDomainAdvance: The grids has CHANGED : re-init the interfaces *-*-*\n\n");
      if( alwaysSetBoundaryData || !solveCoupledInterfaceEquations )
      {
        initializeInterfaces(gfIndex); // this will re-define the interfaces
	initializeInterfaceBoundaryConditions( t,dt,gfIndex );  // assign boundary conditions at the interface
      }      
    }
    

    // -- corrector steps in a PC method OR stages in a R-K method ---
    //  
    //  numberOfPCcorrections : this is actually the maximum number of correction steps that we
    //                          can take in order to solve the interface equations by iteration.
    int numberOfCorrectorSteps=max(numberOfRequiredCorrectorSteps,parameters.dbase.get<int>("numberOfPCcorrections")); 

    if( debug() & 2 )
      printF(" @@@@ Cgmp::multiDomainAdvance: maximum numberOfCorrectorSteps=%i (required=%i)\n",
	     numberOfCorrectorSteps,numberOfRequiredCorrectorSteps);
    if( debug() & 2 )
    {
      fPrintF(interfaceFile,
              "\n --- Start of step: t=%9.3e globalStep=%i numberOfCorrectorSteps=%i required=%i coupled=%i ---\n",
	      t,parameters.dbase.get<int >("globalStepNumber"),numberOfCorrectorSteps,numberOfRequiredCorrectorSteps,
              int(solveCoupledInterfaceEquations));
    }

    
    if( true || alwaysSetBoundaryData || !solveCoupledInterfaceEquations )
    {
      // Check how well the interface equations are satisfied at the start of the step
      getInterfaceResiduals( t, dt, gfIndex, maxResidual, saveInterfaceTimeHistoryValues );
      if( debug() & 2 )
      {
	for( int inter=0; inter<maxResidual.size(); inter++ )
	{
	  printF(" --- Before time step %i (t=%9.3e) : interface %i : max-interface-residual=%8.2e\n",
		 parameters.dbase.get<int >("globalStepNumber"),t,inter,maxResidual[inter]);
	}
      }
    }

#define ForDomainReverse(d) for( int d=domainSolver.size()-1; d>=0; d-- )\
                       if( domainSolver[d]!=NULL )

    for( int correct=0; correct<=numberOfCorrectorSteps; correct++ )
    {

      ForDomainOrdered(d)
      {
	// Assign the RHS for the interface equations on domain d 
        // We could extrapolate the values of the RHS from previous times as an inital guess (correct=0)
        // or use the current guess (correct >0)
	if( alwaysSetBoundaryData || !solveCoupledInterfaceEquations )
	{
	  assignInterfaceRightHandSide( d, t+dt, dt, correct, gfIndex );
	}
	
	if( debug() & 2 )
          printF("\n *** multiDomainAdvance: takeTimeStep for domain %s (d=%i,dd=%i) correct=%i t=%8.2e ***\n\n",
                (const char*)domainSolver[d]->getName(),d,dd,correct,t);

	domainSolver[d]->takeTimeStep( t,dt,correct,advanceOptions[d] );


	gfIndex[d]=gfIndexNext[d]; // Domain d now has a solution at the next time level we can use
	
	if( debug() & 4 )
	{
	  // Now check how well the interface equations are satisfied
          fPrintF(interfaceFile,"\n --- After takeTimeStep for domain d=%i (correction=%i t=%9.3e)\n",d,correct,t+dt);
	  getInterfaceResiduals( t+dt, dt, gfIndex, maxResidual );
	}
	
      } // for domain 
      
      
      // Solve the coupled interface equations: 
      if( solveCoupledInterfaceEquations )
      {
	if( debug() & 2 )
          printF("=== multiDomainAdvance: solve the coupled interface equations\n");
	assignInterfaceBoundaryConditions(gfIndex, dt );
      }

      // -- check for convergence --
      bool hasConverged = checkInterfaceForConvergence( correct,
							numberOfCorrectorSteps,
							numberOfRequiredCorrectorSteps,
							t+dt,
							alwaysSetBoundaryData,
							gfIndex,
							oldResidual,initialResidual,firstResidual,
							maxResidual,
							interfaceIterationsHaveConverged );
      if( hasConverged ) break;

      
    } // end correct 
    
    ForDomainOrdered(d)
    {
      real td=t; //  endTimeStep will increment the time. Do not increment t here. 
      domainSolver[d]->endTimeStep( td,dt,advanceOptions[d] );
    }
    
    t+=dt; 	
    numberOfStepsTaken++; 
    current=next;

  }

  gf[current].t = t;
  
  return 0;
}

// ===================================================================================================================
/// \brief Check that the interface equations have converged to the required tolerance
///        or if the maximum number of corrections has been reached.
/// \details 
/// \param correct (input) : current corrector step
/// \param numberOfCorrectorSteps (input) : maximum number of corrector steps allowed.
/// \param numberOfRequiredCorrectorSteps (input) : we must take at least this many corrector steps
/// \param tNew (input) : current time.
/// \param alwaysSetBoundaryData (input) : 
/// \param gfIndex (input) :
/// \param oldResidual, initialResidual, firstResidual (input) : 
/// \param maxResidual (output) :
/// \param interfaceIterationsHaveConverged (output) : true if the interface equations have converged
///        to the requested tolerance
/// \return value: true if the interface equations have converged or if the maximum number of corrections
///   has been reached, false otherwise.
// ==================================================================================================================

bool Cgmp::
checkInterfaceForConvergence( const int correct,
                              const int numberOfCorrectorSteps,
                              const int numberOfRequiredCorrectorSteps,
                              const real tNew,
                              const bool alwaysSetBoundaryData,
                              std::vector<int> & gfIndex,
                              std::vector<real> & oldResidual,
                              std::vector<real> & initialResidual,
                              std::vector<real> & firstResidual,
                              std::vector<real> & maxResidual,
                              bool & interfaceIterationsHaveConverged )
{
  bool hasConverged=false;
  interfaceIterationsHaveConverged=false;
  
  FILE *& interfaceFile =parameters.dbase.get<FILE* >("interfaceFile");
  InterfaceList & interfaceList = parameters.dbase.get<InterfaceList>("interfaceList");
  const bool solveCoupledInterfaceEquations = parameters.dbase.get<bool>("solveCoupledInterfaceEquations");

  // Check how well the interface equations are satisfied
  if( debug() & 4 ) printF("\n=== After takeTimeStep (correction=%i t=%9.3e)\n",correct,tNew);
  if( alwaysSetBoundaryData || !solveCoupledInterfaceEquations )
  {
    if( debug() & 2 )
      fPrintF(interfaceFile,"--checkConvergence-- After takeTimeStep for all domains: (correction=%i t=%9.3e)\n",correct,tNew);

    // NOTE: the history of interface iterates are saved here 
    getInterfaceResiduals( tNew, dt, gfIndex, maxResidual, saveInterfaceIterateValues );

    if( oldResidual.size()<maxResidual.size() )
    {
      oldResidual.resize(maxResidual.size(),1.);
      initialResidual.resize(maxResidual.size(),1.);
      firstResidual.resize(maxResidual.size(),1.);
    }

    // check if the interface iterations have converged:
    interfaceIterationsHaveConverged=true;
    for( int inter=0; inter<interfaceList.size(); inter++ )
    {
      if( correct==0 ) 
	initialResidual[inter]=maxResidual[inter];
      else if( correct==1 )  
	firstResidual[inter]=maxResidual[inter];

      interfaceIterationsHaveConverged = interfaceIterationsHaveConverged && 
	maxResidual[inter] < interfaceList[inter].interfaceTolerance;
      if( !interfaceIterationsHaveConverged ) break;
    }
	
    if( debug() & 2 )
    {
      assert( maxResidual.size()==interfaceList.size() );
	  
      for( int inter=0; inter<interfaceList.size(); inter++ )
      {
	real resRatio=min(1000., maxResidual[inter]/max(REAL_MIN*100.,oldResidual[inter]));
	oldResidual[inter]=maxResidual[inter];
	    
	printF("--MP-- After takeTimeStep: interface %i: (correction=%i t=%9.3e) :  max-interface-residual=%8.2e, "
	       "ratio=%7.4f tol=%8.2e\n",
	       inter,correct,tNew,maxResidual[inter],resRatio,interfaceList[inter].interfaceTolerance);
	fPrintF(interfaceFile,
		" interface %i : t=%9.3e correction=%i max-interface-residual=%8.2e ratio=%7.4f tol=%8.2e [%i]\n",
		inter,tNew,correct,maxResidual[inter],resRatio,interfaceList[inter].interfaceTolerance,inter);
            
      }
    }
	
  }

  bool correctionsAreDone= correct==numberOfCorrectorSteps;
  if( interfaceIterationsHaveConverged && !solveCoupledInterfaceEquations )
  {
	
    if( correct >= numberOfRequiredCorrectorSteps )
    {
      if( debug() & 1 )
	printF("*** Cgmp: t=%9.3e interface iterations have converged (%i iterations)****\n",tNew,correct+1);
      fPrintF(interfaceFile,"****t=%9.3e correction=%i : interface iterations have converged****\n",tNew,correct);
    }
    else
    {
      if( debug() & 1 )
	printF("*** Cgmp: t=%9.3e interface iterations have converged BEFORE PC corrections are done (%i iterations)****\n",
	       tNew,correct+1);
      fPrintF(interfaceFile,"****t=%9.3e correction=%i : interface iterations have converged BEFORE PC corrections are done****\n",
	      tNew,correct);
    }
	
    if( correct >= numberOfRequiredCorrectorSteps ) // we can stop if we have corrected enough times.
    {
      correctionsAreDone=true;
    }
  }
  if( correctionsAreDone )
  {
    if( !solveCoupledInterfaceEquations )
    {
      // save the current convergence rate: 
      for( int inter=0; inter<interfaceList.size(); inter++ )
      {
	real cr;
	if( correct<=1 )
	{
	  cr=pow( maxResidual[inter]/max(REAL_MIN*100.,initialResidual[inter]), 1./max(correct,1));
	}
	else
	{
	  // start measuring the CR at the first residual
	  cr=pow( maxResidual[inter]/max(REAL_MIN*100.,firstResidual[inter]), 1./max(correct-1,1));
	}
	interfaceList[inter].estimatedConvergenceRate +=cr;

	// printF(" ** estimatedConvergenceRate: interface=%i maxRes=%8.2e initialRes=%8.2e rate=%8.2e\n",
	//    inter,maxResidual[inter],initialResidual[inter],cr);
	    
	interfaceList[inter].numberOfInterfaceSolves++;
	interfaceList[inter].totalNumberOfInterfaceIterations+=correct+1;
      }
    }
    // break;
  }

  hasConverged = correctionsAreDone;
  
  return hasConverged;
}
