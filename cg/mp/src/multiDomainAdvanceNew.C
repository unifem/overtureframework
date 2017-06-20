// This file automatically generated from multiDomainAdvanceNew.bC with bpp.
// -----------------------------------------------------------------------------------------------------------
// This file contains the **NEW VERSION** of the multi-domain advance routine that supports AMR.
// 
// multiDomainAdvanceNew( real &t, real & tFinal )
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
/// \brief Multi-domain explicit/implicit time stepping, **NEW VERSION** that supports AMR
/// \details 
/// \param t (input/output) : current time
/// \param tFinal (input) : integrate to this final time.
// ==================================================================================================================
int Cgmp::
multiDomainAdvanceNew( real &t, real & tFinal )
{  
    real cpu0=getCPU();

    const MpParameters::MultiDomainAlgorithmEnum multiDomainAlgorithm = 
                                                            parameters.dbase.get<MpParameters::MultiDomainAlgorithmEnum>("multiDomainAlgorithm");
    
    assert( multiDomainAlgorithm==MpParameters::stepAllThenMatchMultiDomainAlgorithm );

    FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");
    FILE *& interfaceFile =parameters.dbase.get<FILE* >("interfaceFile");
    
  // Is this next right or should we expect each DomainSolver to know whether it needs to initialize?
    if( !parameters.dbase.get<DataBase >("modelData").has_key("initializeAdvance") )
        parameters.dbase.get<DataBase >("modelData").put<int>("initializeAdvance",true);
    int & init=parameters.dbase.get<DataBase >("modelData").get<int>("initializeAdvance");

    const int numberOfDomains = domainSolver.size(); 
    int numberOfSubSteps=parameters.dbase.get<int>("numberOfSubSteps");

    InterfaceList & interfaceList = parameters.dbase.get<InterfaceList>("interfaceList");
    const bool solveCoupledInterfaceEquations = parameters.dbase.get<bool>("solveCoupledInterfaceEquations");
    const int & interfaceProjectionGhostOption = parameters.dbase.get<int>("interfaceProjectionGhostOption");
    
    bool & timeStepHasChanged = parameters.dbase.get<bool>("timeStepHasChanged");

    if( true || debug() & 4 )
        printF("\n"
                      " --------------------------------------------------------------------------------------------\n"
                      " ---- Cgmp::multiDomainAdvanceNew ---- t=%e, dt=%e, tFinal=%e, timeStepHasChanged=%i\n"
                      " --------------------------------------------------------------------------------------------\n\n",
                          t,dt,tFinal,(int)timeStepHasChanged);
    if( debug() & 2 )
        fprintf(debugFile," *** Cgmp::multiDomainAdvanceNew: t=%e, dt=%e, tFinal=%e, timeStepHasChanged=%i *** \n",t,dt,tFinal,(int)timeStepHasChanged);


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
      	printF("Cgmp::::multiDomainAdvanceNew: AMR is being used in at least one domain\n");
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
        const bool & projectMultiDomainInitialConditions = parameters.dbase.get<bool>("projectMultiDomainInitialConditions");
        
        if( projectMultiDomainInitialConditions )
        {
      // --- project initial conditions --- 
      // Sometimes we need to iterate on the initial conditions to be self-consistent such as
      // with the pressure for INS + SM). 
            std::vector<int> gfIndex(numberOfDomains,current); // ** fix this ** get gfIndex from each domain solver
            projectInitialConditions(t,dt, gfIndex);
        }
        else
        {
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
    std::vector<real> & maxResidual = parameters.dbase.get<std::vector<real> >("maxResidual");

  // std::vector<real> maxResidual, oldResidual, initialResidual, firstResidual;
    std::vector<real> oldResidual, initialResidual, firstResidual;
    bool interfaceIterationsHaveConverged=false;
    std::vector<AdvanceOptions> advanceOptions(numberOfDomains);  

    int & globalStepNumber = parameters.dbase.get<int >("globalStepNumber");

    for( int i=0; i<numberOfSubSteps; i++ )
    {
        globalStepNumber++;

        const int next = (current+1) %2;

        if( debug() & 2 )
            printF("\n"
                          "#######################################################################################################\n"
                          "############ multiDomainAdvanceNew - START STEP %i, t=%8.2e -> t+dt=%8.2e ##########################\n"
                          "#######################################################################################################\n\n",
                          globalStepNumber,t,t+dt);

        std::vector<int> gfIndex(numberOfDomains,-1);  // keep track of which GridFunction to use for each domain
        int numberOfRequiredCorrectorSteps=0;          // The minimum number of corrector steps that we must take
        ForDomainOrdered(d)
        { 
      // The next call will return the number of corrector steps needed by this domain solver.
      // gfIndexCurrent[d],gfIndexNext[d] are also returned here.

            domainSolver[d]->startTimeStep( t,dt,gfIndexCurrent[d],gfIndexNext[d],advanceOptions[d] );

            gfIndex[d]=gfIndexCurrent[d];
            numberOfRequiredCorrectorSteps=max(numberOfRequiredCorrectorSteps,advanceOptions[d].numberOfCorrectorSteps);
        }
        

    // -- corrector steps in a PC method OR stages in a R-K method ---
    //  
    //  numberOfPCcorrections : this is actually the maximum number of correction steps that we
    //                          can take in order to solve the interface equations by iteration.
        int numberOfCorrectorSteps=max(numberOfRequiredCorrectorSteps,parameters.dbase.get<int>("numberOfPCcorrections")); 

        if( debug() & 2 )
            printF(" @@@@ Cgmp::multiDomainAdvanceNew: maximum numberOfCorrectorSteps=%i (required=%i)\n",
           	     numberOfCorrectorSteps,numberOfRequiredCorrectorSteps);
        if( debug() & 2 )
        {
            fPrintF(interfaceFile,
                            "\n --- Start of step: t=%9.3e globalStep=%i numberOfCorrectorSteps=%i required=%i coupled=%i ---\n",
            	      t,globalStepNumber,numberOfCorrectorSteps,numberOfRequiredCorrectorSteps,
                            int(solveCoupledInterfaceEquations));
        }

        
        if( !parameters.isAdaptiveGridProblem()
                && ( alwaysSetBoundaryData || !solveCoupledInterfaceEquations) ) // -- this is not right with AMR
        {
      // Check how well the interface equations are satisfied at the start of the step

      // **** NOTE: interface history values are saved here ****
            if( debug() & 2 )
                printF("\n"
                              "+++++++++++++++++++++++++++ STEP=%i t=+dt=%8.2e GET INTERFACE RESIDUALS ++++++++++++++++++++++++++++++++++++++++\n\n",
                              globalStepNumber,t+dt);

            getInterfaceResiduals( t, dt, gfIndex, maxResidual, saveInterfaceTimeHistoryValues );
            if( debug() & 2 )
            {
      	for( int inter=0; inter<maxResidual.size(); inter++ )
      	{
        	  printF("--MP-- Before time step %i (t=%9.3e) : interface %i : max-interface-residual=%8.2e\n",
             		 globalStepNumber,t,inter,maxResidual[inter]);
      	}
            }
        }

#define ForDomainReverse(d) for( int d=domainSolver.size()-1; d>=0; d-- )if( domainSolver[d]!=NULL )

        for( int correct=0; correct<=numberOfCorrectorSteps; correct++ )
        {
        if( debug() & 2 )
            printF("\n"
                          "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                          "+++++++++++++++++++++++ STEP=%i t=%8.2e -> t+dt=%8.2e START CORRECTION STAGE %i +++++++++++++++++++++++++++++++\n"
                          "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n",
                          globalStepNumber,t,t+dt,correct);

      // Stage I: advance the solution but do not apply (interface) BC's
            bool gridHasChanged=false;
            ForDomainOrdered(d)
            {
      	if( debug() & 2 )
                    printF("\n"
                                  "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                                  "++++++ MDA: ASSIGN INTERFACE RHS for domain %s (d=%i,dd=%i) correct=%i t+dt=%8.2e ++++++\n"
                                  "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n",
                                (const char*)domainSolver[d]->getName(),d,dd,correct,t+dt);
        // For FSI we need a guess for the new solid location *wdh* 101108 
        // -- could do better here: these values are not always needed  ---
      	if( alwaysSetBoundaryData || !solveCoupledInterfaceEquations )
      	{
        	  assignInterfaceRightHandSide( d, t+dt, dt, correct, gfIndex );
      	}

      	if( debug() & 2 )
                    printF("\n"
                                  "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                                  "++++++ MDA: takeTimeStep (no BCs) for domain %s (d=%i,dd=%i) correct=%i t+dt=%8.2e ++++++\n"
                                  "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n",
                                (const char*)domainSolver[d]->getName(),d,dd,correct,t+dt);

                advanceOptions[d].takeTimeStepOption=AdvanceOptions::takeStepButDoNotApplyBoundaryConditions;

                domainSolver[d]->takeTimeStep( t,dt,correct,advanceOptions[d] );

      	gfIndex[d]=gfIndexNext[d]; // Domain d now has a solution at the next time level we can use

                gridHasChanged = gridHasChanged || advanceOptions[d].gridChanges != AdvanceOptions::noChangeToGrid;
            }
            
      // Update the interfaces if the grids have changed in stage I.
            if( gridHasChanged )
            { 
	// we need to redefine the interfaces if the grid has changed
      	printF("\n *-*-* Cgmp::multiDomainAdvanceNew: The grids has CHANGED : re-init the interfaces *-*-*\n\n");
      	if( alwaysSetBoundaryData || !solveCoupledInterfaceEquations )
      	{
        	  initializeInterfaces(gfIndex); // this will re-define the interfaces
        	  initializeInterfaceBoundaryConditions( t,dt,gfIndex );  // assign boundary conditions at the interface
      	}      
            }

      // Stage II: optionally project the interface values: option=0 : 0=set values on the interface
            bool turnOnProjection=true;
            if( turnOnProjection )
            {
      	interfaceProjection( t+dt, dt, correct, gfIndex, 0 );
        // interfaceProjection( t+dt, dt, correct, gfIndex, 1 );
            }
            

      // Stage III: evaluate the interface conditions and apply the boundary conditions
            ForDomainOrdered(d)
            {
      	if( debug() & 2 )
                    printF("\n"
                                  "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                                  "++++++ MDA: ASSIGN INTERFACE RHS for domain %s (d=%i,dd=%i) correct=%i t+dt=%8.2e ++++++\n"
                                  "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n",
                                (const char*)domainSolver[d]->getName(),d,dd,correct,t+dt);

	// Assign the RHS for the interface equations on domain d 
        // We could extrapolate the values of the RHS from previous times as an inital guess (correct=0)
        // or use the current guess (correct >0)
      	if( alwaysSetBoundaryData || !solveCoupledInterfaceEquations )
      	{
        	  assignInterfaceRightHandSide( d, t+dt, dt, correct, gfIndex );
      	}

      	if( debug() & 2 )
                    printF("\n"
                                  "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                                  "++++++ MDA: APPLY BCS only for domain %s (d=%i,dd=%i) correct=%i t+dt=%8.2e ++++++\n"
                                  "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n",
                                (const char*)domainSolver[d]->getName(),d,dd,correct,t+dt);
      	
                advanceOptions[d].takeTimeStepOption=AdvanceOptions::applyBoundaryConditionsOnly;

        // domainSolver[d]->parameters.dbase.get<int>("applyInterfaceBoundaryConditions")=0;  // *** TESTING

      	domainSolver[d]->takeTimeStep( t,dt,correct,advanceOptions[d] );

        // domainSolver[d]->parameters.dbase.get<int>("applyInterfaceBoundaryConditions")=1;  // *** TESTING

      	if( debug() & 4 )
      	{
	  // Now check how well the interface equations are satisfied
                    fPrintF(interfaceFile,"\n --- After takeTimeStep for domain d=%i (correction=%i t=%9.3e)\n",d,correct,t+dt);
        	  getInterfaceResiduals( t+dt, dt, gfIndex, maxResidual );
      	}
      	
            } // for domain 
            

            if( turnOnProjection && interfaceProjectionGhostOption!=3 ) 
            {
        // Stage IV: assign ghost values at interfaces 
	// printF(">>>multiDomainAdvanceNew: assign ghost values at interfaces\n");

      	interfaceProjection( t+dt, dt, correct, gfIndex, 1 );

            }
            else if( interfaceProjectionGhostOption==3 )
            {
      	if( debug() & 4 )
        	  printF(">>>multiDomainAdvanceNew: Do not assign ghost with interfaceProjection. "
             		 "Use domain solver BC routines\n");
            }
            
        if( debug() & 2 )
            printF("\n"
                          "++++++++++++++++++++ STEP=%i t+dt=%8.2e CHECK FOR CONVERGENCE, CORRECTION %i ++++++++++++++++++++++++++++++\n\n",
                          globalStepNumber,t+dt,correct);
            

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

            
        if( debug() & 2 )
            printF("\n"
                          "+++++++++++++++++++++ STEP=%i t+dt=%8.2e END CORRECTION STAGE %i +++++++++++++++++++++++++++++++++++++\n\n",
                          globalStepNumber,t+dt,correct);

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

