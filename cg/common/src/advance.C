// This file automatically generated from advance.bC with bpp.
#include "DomainSolver.h"
#include "CompositeGridOperators.h"
#include "GridCollectionOperators.h"
#include "ExposedPoints.h"
#include "PlotStuff.h"
#include "InterpolateRefinements.h"
#include "Regrid.h"
#include "Ogen.h"
#include "MatrixTransform.h"
#include "App.h"
#include "updateOpt.h"
#include "ParallelUtility.h"
#include "Ogshow.h"
#include "FileOutput.h"
#include "AdamsPCData.h"
#include "Controller.h"

int
checkForSymmetry(realCompositeGridFunction & u, Parameters & parameters, const aString & label,
                                  int numberOfGhostLinesToCheck)
// ==================================================================================================
// check for symmetry -- this is for testing 3D AMR grids where the solution should be constant along i2
// ==================================================================================================
{
    if( true ) return 0;
    
    
    const real tol=1.e-8;

    CompositeGrid & cg = *u.getCompositeGrid();
    Index I1,I2,I3;
    real maxDiff=0.;
    const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    const int nc=min(numberOfComponents,u[0].getLength(3));
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        MappedGrid & mg = cg[grid];
        realArray & uu = u[grid];
        getIndex(mg.gridIndexRange(),I1,I2,I3,numberOfGhostLinesToCheck);
        
        for( int n=0; n<nc; n++ )
        {
            real diff=0.;
            const int i20=mg.gridIndexRange(0,axis2);
            for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
            {
      	diff=max(diff,max(fabs(uu(I1,i2,I3,n)-uu(I1,i20,I3,n))));
      	if( diff>tol )
      	{
        	  bool found=false;
        	  for( int i1=I1.getBase(); i1<=I1.getBound() && !found ; i1++ )
          	    for( int i3=I3.getBase(); i3<=I3.getBound() && !found ; i3++ )
          	    {
            	      if( fabs(uu(i1,i2,i3,n)-uu(i1,i20,i3,n)) > tol )
            	      {
            		printf("symmetry broken: grid=%i n=%i (i1,i2,i3)=(%i,%i,%i)\n",grid,n,i1,i2,i3);

                                if( false || nc==1 )
            		{
              		  uu(i1,i2,i3,n)=uu(i1,i20,i3,n);  // ************** do this for a test **************
            		}
            		else
            		{
              		  found=true;
              		  break;
            		}
            		
            	      }
          	    }
      	}
          	    
            }
            maxDiff=max(maxDiff,diff);
            printf("%s grid=%i check for 2D  n=%i diff=%9.3e\n",(const char*)label,grid,n,diff);
        }
    }
    if( nc>1 && maxDiff > tol )
    {
        char buff[80];
        GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
        GenericGraphicsInterface & ps = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

        psp.set(GI_TOP_LABEL,label+" symmetry broken!");
        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
        ps.erase();
        PlotIt::contour(ps,u,psp);

        return 1;
    }

    return 0;
}

//\begin{>>DomainSolverInclude.tex}{\subsection{advance}} 
int DomainSolver::
advance(real & tFinal )
//===============================================================================================
//  /Description:
//    Advance to time tFinal
// 
//\end{DomainSolverInclude.tex}  
//===============================================================================================
{
    if( Parameters::checkForFloatingPointErrors )
    {
        checkSolution(gf[current].u,"advance:start (gf[current])");
    }
    
    real t=gf[current].t;
    real cpu0=getCPU();

    if( !parameters.dbase.get<DataBase >("modelData").has_key("initializeAdvance") )
        parameters.dbase.get<DataBase >("modelData").put<int>("initializeAdvance",true);
    int & init=parameters.dbase.get<DataBase >("modelData").get<int>("initializeAdvance");

    Parameters::TimeSteppingMethod & timeSteppingMethod = 
                                              parameters.dbase.get<Parameters::TimeSteppingMethod>("timeSteppingMethod");
    const Parameters::ImplicitMethod & implicitMethod = 
        parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");

    RealArray & timing = parameters.dbase.get<RealArray>("timing");

    const real & cfl    = parameters.dbase.get<real>("cfl");
    const real & cflMin = parameters.dbase.get<real>("cflMin");
    const real & cflMax = parameters.dbase.get<real>("cflMax");
    
    real & tPrint = parameters.dbase.get<real >("tPrint");
    const int & myid = parameters.dbase.get<int >("myid");
    FILE * debugFile = parameters.dbase.get<FILE* >("debugFile");
    FILE * pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");

    RealArray & printArray = parameters.dbase.get<RealArray >("printArray");
    int nextPrintValue=0;
    
    int & frequencyToSaveInShowFile = parameters.dbase.get<int >("frequencyToSaveInShowFile");
    
    int & plotOption = parameters.dbase.get<int >("plotOption");
    int & maxIterations = parameters.dbase.get<int >("maxIterations");

    real & nextTimeToPrint = parameters.dbase.get<real >("nextTimeToPrint");

    nextTimeToPrint= !parameters.isSteadyStateSolver() ? t : 0.;  //  ...new time to print solution (print initial time)
    int iPrint=0;

    const int maximumNumberOfSteps=max(int( 1e8 ),maxIterations);
    int numberOfSubSteps=1;
    int finish=0;
    
    buildRunTimeDialog();


    numberOfStepsTaken=max(0,numberOfStepsTaken);  // numberOfStepsTaken==-1 : for initialization steps

  //  for( int step=0; step<maximumNumberOfSteps && t<tFinal+dt; step++ )
    for( int step=0; step<maximumNumberOfSteps;  )
    {
        real cpuTime=getCPU()-cpu0;
        checkArrayIDs(sPrintF("advance: step=%i",step) ); 

//      printF(" numberOfStepsTaken=%i, parameters.dbase.get<int >("globalStepNumber")=%i nextTimeToPrint=%8.2e\n",numberOfStepsTaken,
//                 parameters.dbase.get<int >("globalStepNumber"),nextTimeToPrint);


        if( (!parameters.isSteadyStateSolver() && t >= nextTimeToPrint-dt*.25) ||
                ( parameters.isSteadyStateSolver() && (parameters.dbase.get<int >("globalStepNumber")+1) > nextTimeToPrint-.1) )
        {

            fPrintF(parameters.dbase.get<FILE* >("debugFile")," advance::printTimeStepInfo at t=%20.12e, dt=%20.12e \n",t,dt);
            printTimeStepInfo(step,t,cpuTime);
            

            if( frequencyToSaveInShowFile>0 && (iPrint % frequencyToSaveInShowFile == 0) )
                saveShow( gf[current] );  // save the current solution in the show file

            if( false )
            {
      	FILE *file = stdout;
      	fprintf(file,"\n ++++++++++++advance before plot+++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      	for( int grid=0; grid<gf[current].cg.numberOfComponentGrids(); grid++ )
        	  gf[current].cg[grid].displayComputedGeometry(file);
      	fprintf(file," +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
            }

            output( gf[current],step );  // *wdh* 081104
            
            checkArrayIDs(sPrintF("advance: step=%i, before plot",step) ); 

            int optionIn = step==0 && plotOption ? 1 : plotOption; // wait on first step
            finish=plot(t, optionIn, tFinal);  // optionIn: 0=wait, 1=plot-and-wait, 2=plot-but-don't-wait

            checkArrayIDs(sPrintF("advance: step=%i, after plot",step) ); 

            if( finish )
                break;
            if( (!parameters.isSteadyStateSolver() && t >tFinal-.5*dt) ||
                    ( parameters.isSteadyStateSolver() && parameters.dbase.get<int >("globalStepNumber")+1>=maxIterations) )
            {
	// we are done (unless tFinal is increased in plot). plot solution at final time
      	if( true || parameters.dbase.get<int >("plotOption") & 1 )
        	  plot(t,1, tFinal);

         // tFinal may have been increased, so check again
                if( (!parameters.isSteadyStateSolver() && t >tFinal-.5*dt) ||
                        ( parameters.isSteadyStateSolver() && parameters.dbase.get<int >("globalStepNumber")+1>=maxIterations) )
      	{ 
        	  finish=true;
        	  break;
      	}
            }
            if( parameters.dbase.get<Ogshow* >("show")!=NULL )
            {
	// this will close any open sub-file if it contains the max number of solutions allowed.
        // do this here since we save sequences if we finish, so we cannot do this in saveShow.
        // printF("\n *********** parameters.dbase.get<Ogshow* >("show")->endFrame(); *********\n");
                real timea=getCPU();
                parameters.dbase.get<Ogshow* >("show")->endFrame();  
                timea=getCPU()-timea;
                timing(parameters.dbase.get<int>("timeForShowFile"))+=timea;
      	if( debug() & 1 )
      	{
        	  printF("advance: time to endFrame and save show=%8.2e.\n",timea);
        	  fflush(0);
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
          // nextTimeToPrint=min(int(t/tPrint+.5)*tPrint+tPrint,tFinal);   //  ...new time to print:
          // *wdh* avoid integer overflows: 
          // nextTimeToPrint=min(ceil(t/tPrint)*tPrint+tPrint,tFinal);   //  ...new time to print:
                    nextTimeToPrint=min(ceil(t/tPrint-.5)*tPrint+tPrint,tFinal);   //  ...new time to print:
      	}
                else
        	  nextTimeToPrint=min(int(nextTimeToPrint+parameters.dbase.get<int >("plotIterations")+.5),maxIterations);
            }
            if( debug() & 8 )
            {
      	printF("advance: nextTimeToPrint=%18.10e, t=%18.10e \n",nextTimeToPrint,t);
            }
            

            iPrint++;
        }



        if(  timeSteppingMethod!=Parameters::steadyStateNewton
       	 && timeSteppingMethod!=Parameters::implicit 
       	 && timeSteppingMethod!=Parameters::implicitAllSpeed 
       	 && timeSteppingMethod!=Parameters::rKutta ) 
        {  //   ===Choose a new time step====
            if( TRUE || step==0 || timeSteppingMethod!=Parameters::implicitAllSpeed2 )
            {
                checkArrayIDs(sPrintF("advance: step=%i, before get dt",step) ); 

      	real dtNew= getTimeStep( gf[current] ); //       ===Choose time step====
                computeNumberOfStepsAndAdjustTheTimeStep(t,tFinal,nextTimeToPrint,numberOfSubSteps,dtNew);
      	
                checkArrayIDs(sPrintF("advance: step=%i, after get dt",step) ); 

                if( debug() & 4 )
      	{
              	  printF("advance:recompute dt: dt(old)=%8.3e, dtNew = %8.3e numberOfSubSteps=%i\n",dt,dtNew,numberOfSubSteps);
      	}
      	dt=dtNew;
            }
        }
    // *** scLC
        else if( timeSteppingMethod==Parameters::rKutta )
        {
            parameters.setGridIsImplicit(-1,false);
            real dtNew= getTimeStep( gf[current] );
            dt = dtNew;
        }
    // *** ecLC
        else if( timeSteppingMethod==Parameters::implicit )//|| timeSteppingMethod==Parameters::steadyStateNewton) // not really needed for newton, just did this to make code happy right now kkc 060724
        {
      // first compute what the explicit time step would be:

            const Parameters::TimeSteppingMethod timeSteppingMethodSaved=timeSteppingMethod;
            timeSteppingMethod=Parameters::adamsPredictorCorrector2;

            real dtExplicit= getTimeStep( gf[current] ); //       ===Choose time step====

            timeSteppingMethod=timeSteppingMethodSaved;  // reset

      // now compute the implicit time step
            real dtNew= getTimeStep( gf[current] ); //       ===Choose time step====
            computeNumberOfStepsAndAdjustTheTimeStep(t,tFinal,nextTimeToPrint,numberOfSubSteps,dtNew);

            if( debug() & 1 )
      	printF("recompute dt: dt(old)=%8.3e, dtNew = %8.3e, (explicit dt=%8.3e, ratio=%8.3e) \n",
             	       dt,dtNew,dtExplicit,dtNew/dtExplicit);
            
      // --- only change the time step if we exceed the cfl limit or we could increase the time step substantially --

            real ratio=dt/dtNew;
            if( step==0 || 
                    implicitMethod==Parameters::approximateFactorization || // *wdh* always change time step for AF scheme 2011/09/06
                    ratio < cflMin/cfl || 
                    ratio > cflMax/cfl )
            {
                dt=dtNew;
                if( debug() & 1 ) 
                    printF(" ****** time step is being changed for the implicit method, dt/dtNew=%8.2e "
                                  "(cflMin=%5.2f, cfl=%5.2f, cflMax=%5.2f)*****\n",ratio,cflMin,cfl,cflMax);
            }
            else
            {
                if( debug() & 2 ) printF(" ****** time step is NOT being changed for the implicit method *****\n");
                computeNumberOfStepsAndAdjustTheTimeStep(t,tFinal,nextTimeToPrint,numberOfSubSteps,dt,FALSE);
            }
        }
//     else if ( timeSteppingMethod==Parameters::steadyStateNewton )
//       {
// 	dt = 1;
// 	numberOfSubSteps = 1;
//       }

        
//     if( parameters.dbase.get<Parameters::PDE >("pde")==Parameters::incompressibleNavierStokes ) // *wdh* put here instead of ims
//     {
//       // recompute the damping term (if the time step has changed)
//       const int geometryHasChanged=FALSE;
//       updateDivergenceDamping( gf[current].cg,geometryHasChanged ); 
//     }

        updateForNewTimeStep(gf[current],dt);  // new way

        parameters.dbase.get<real >("dt")=dt;

        if( timeSteppingMethod==Parameters::forwardEuler )
        {
            if( parameters.dbase.get<int >("useNewAdvanceStepsVersions") )
            { 
      	advanceForwardEulerNew( t,dt, numberOfSubSteps,init,step );

                step+=numberOfSubSteps; numberOfStepsTaken+=numberOfSubSteps; // *wdh* 101106
      	
            }
            else if( true )
            {
	// new way -- no need to take an even number of steps
      	for( int i=0; i<numberOfSubSteps; i++ )
      	{
        	  const int next = (current + 1) % 2;

        	  eulerStep(t,t,t+dt,dt,gf[current],gf[current],gf[next],fn[0],fn[0],i,numberOfSubSteps);
        	  t+=dt;
        	  step++; numberOfStepsTaken++; // parameters.dbase.get<int >("globalStepNumber")++;
        	  current=next;

        	  output( gf[current],step );
      	
        	  if( (numberOfStepsTaken-1) % parameters.dbase.get<int >("frequencyToSaveSequenceInfo") == 0 )
        	  {
          	    if( !parameters.isAdaptiveGridProblem() )  // fn[0] is not valid for AMR (?) -- fix this
          	    {
            	      saveSequenceInfo(t,fn[0]);
          	    }
        	  }
      	}
            }
            else
            {
	// old way
	// take an even number of sub-steps so the solution remains in gf[0]
      	assert( (numberOfSubSteps % 2) == 0 );
      	for( int i=0; i<numberOfSubSteps; i+=2 )
      	{
        	  eulerStep(t,t,t+dt,dt,gf[0],gf[0],gf[1],fn[0],fn[0],i  ,numberOfSubSteps);
        	  t+=dt;
        	  step++; numberOfStepsTaken++; // parameters.dbase.get<int >("globalStepNumber")++;
        	  output( gf[1],step );
        	  eulerStep(t,t,t+dt,dt,gf[1],gf[1],gf[0],fn[0],fn[0],i+1,numberOfSubSteps);
        	  t+=dt;
        	  step++; numberOfStepsTaken++; // parameters.dbase.get<int >("globalStepNumber")++;
        	  output( gf[0],step );

        	  if( (numberOfStepsTaken-1) % parameters.dbase.get<int >("frequencyToSaveSequenceInfo") == 0 ||
            	      (numberOfStepsTaken-1) % parameters.dbase.get<int >("frequencyToSaveSequenceInfo") == 1 )
        	  {
          	    if( !parameters.isAdaptiveGridProblem() )  // fn[0] is not valid for AMR (?) -- fix this
          	    {
            	      saveSequenceInfo(t,fn[0]);
          	    }
        	  }
      	}
            }
            
        }
        else if( timeSteppingMethod==Parameters::midPoint )
        {
            advanceMidPoint(t,dt,numberOfSubSteps,step );
            step+=numberOfSubSteps;  numberOfStepsTaken+=numberOfSubSteps; parameters.dbase.get<int >("globalStepNumber")+=numberOfSubSteps;
        }
        else if( timeSteppingMethod==Parameters::adi )
        {
            advanceADI(t,dt,numberOfSubSteps,init,step );
            step+=numberOfSubSteps;  numberOfStepsTaken+=numberOfSubSteps; parameters.dbase.get<int >("globalStepNumber")+=numberOfSubSteps;
        }
        else if( timeSteppingMethod==Parameters::adamsBashforth2 ||
                          timeSteppingMethod==Parameters::adamsPredictorCorrector2 ||
                          timeSteppingMethod==Parameters::adamsPredictorCorrector4 )
        {
            if( parameters.dbase.get<int >("useNewAdvanceStepsVersions") )
            { // here is the new way: 
      	advanceAdamsPredictorCorrectorNew( t,dt, numberOfSubSteps,init,step );
            }
            else
            {
      	advanceAdamsPredictorCorrector( t,dt, numberOfSubSteps,init,step ); 
            }
            step+=numberOfSubSteps; numberOfStepsTaken+=numberOfSubSteps; 
        }
        else if( timeSteppingMethod==Parameters::variableTimeStepAdamsPredictorCorrector )
        {
            advanceVariableTimeStepAdamsPredictorCorrector( t,dt, numberOfSubSteps,init,step ); // *** need init ***
            step+=numberOfSubSteps; numberOfStepsTaken+=numberOfSubSteps; parameters.dbase.get<int >("globalStepNumber")+=numberOfSubSteps;
        }
        else if( timeSteppingMethod==Parameters::laxFriedrichs )
        {
      // non-method of lines solver

            Overture::abort("ERROR -- fix this Bill!");
            
            gf[current].t+=dt;  // ???
            gf[current].u.interpolate();
            applyBoundaryConditions(gf[current]);
            step++; numberOfStepsTaken++; parameters.dbase.get<int >("globalStepNumber")++;
        }
        else if( timeSteppingMethod==Parameters::steadyStateRungeKutta )
        {
            advanceSteadyStateRungeKutta( t,dt, numberOfSubSteps,init,step ); 
            step+=numberOfSubSteps; numberOfStepsTaken+=numberOfSubSteps; 
      // parameters.dbase.get<int >("globalStepNumber")+=numberOfSubSteps; // this is done in the above routine
        }

    // *** scLC
//     else if( timeSteppingMethod==Parameters::rKutta )
//     {
//       advanceSemiImplicit( t,dt, numberOfSubSteps,init );
//       step+=numberOfSubSteps; numberOfStepsTaken+=numberOfSubSteps; parameters.dbase.get<int >("globalStepNumber")+=numberOfSubSteps;
//     }
    // *** ecLC

        else if( timeSteppingMethod==Parameters::implicit )
        {
            if ( implicitMethod==Parameters::trapezoidal )
      	advanceTrapezoidal(t,dt,numberOfSubSteps,init,step);
            else
            {
      	if( parameters.dbase.get<int >("useNewAdvanceStepsVersions") ||
                          implicitMethod==Parameters::approximateFactorization ||
                          implicitMethod==Parameters::backwardDifferentiationFormula )
      	{ // here is the new way: 
        	  advanceImplicitMultiStepNew( t,dt, numberOfSubSteps,init,step );
      	}
      	else
      	{
        	  advanceImplicitMultiStep( t,dt, numberOfSubSteps,init,step );
      	}
            }
            
            step+=numberOfSubSteps; numberOfStepsTaken+=numberOfSubSteps; 
      // parameters.dbase.get<int >("globalStepNumber")+=numberOfSubSteps;
        }

        else if( timeSteppingMethod==Parameters::steadyStateNewton )
        {
            advanceNewton(t,dt,numberOfSubSteps,init,step);

            step+=numberOfSubSteps; numberOfStepsTaken+=numberOfSubSteps; 
      // parameters.dbase.get<int >("globalStepNumber")+=numberOfSubSteps;
        }
        else if( timeSteppingMethod==Parameters::nonMethodOfLines )
        {
      // non-method of lines solver
            Overture::abort("ERROR -- fix this Bill!");

//       for( int grid=0; grid<solution.u.numberOfComponentGrids(); grid++)
// 	mappedGridSolver[grid]->advance(t,dt,solution.u[grid],grid);

            gf[current].t+=dt;  // ???
            gf[current].u.interpolate();
            applyBoundaryConditions(gf[current]);
            step++; numberOfStepsTaken++; parameters.dbase.get<int >("globalStepNumber")++;
        }
        else if( timeSteppingMethod==Parameters::implicitAllSpeed )
        {
      // this routine will determine its own time step

            allSpeedImplicitTimeStep(gf[current],t,dt,numberOfSubSteps,min(tFinal,nextTimeToPrint) );

            step+=numberOfSubSteps; numberOfStepsTaken+=numberOfSubSteps; parameters.dbase.get<int >("globalStepNumber")+=numberOfSubSteps;
        }  
        else if( timeSteppingMethod==Parameters::secondOrderSystemTimeStepping )
        {
            advanceSecondOrderSystem( t,dt, numberOfSubSteps,init,step ); // *** need init ***
            step+=numberOfSubSteps; numberOfStepsTaken+=numberOfSubSteps;    
        }
        else
        {
            printF("DomainSolver::advance: unknown timeSteppingMethod\n");
            Overture::abort("error");
        }


    }
    
    timing(parameters.dbase.get<int>("timeForAdvance"))+=getCPU()-cpu0;
    if( parameters.dbase.get<int >("plotOption") & 1 && !finish )
        plot(t,1, tFinal);
    
    tFinal=t;
    
  // Here we save time sequences to the show file
  // (if this was the last frame in a subFile then the seqeuences were already saved.)
    real timea=getCPU();
    if( parameters.dbase.get<Ogshow* >("show")!=NULL &&
            (!parameters.dbase.get<Ogshow* >("show")->isLastFrameInSubFile() || 
       !parameters.dbase.get<bool >("saveSequencesEveryTime")) )
    {
    // *** NOTE: this code also appears in saveShow.bC **** FIX ME 
    // printF("\n *********** saveSequencesToShowFile() *********\n");
        saveSequencesToShowFile();
    // time sequence info for moving grids is saved here
        if( parameters.isMovingGridProblem() )
            parameters.dbase.get<MovingGrids >("movingGrids").saveToShowFile();
    // Save control sequences to the show file
        printF(" ++++++++++ save control sequences ? \n");
        if( parameters.dbase.has_key("Controller") )
        {
            Controller & controller = parameters.dbase.get<Controller>("Controller");
      // Controller & controller = *(parameters.dbase.get<Controller*>("Controller"));
            controller.saveToShowFile();
        }
    }
    if( parameters.dbase.get<Ogshow* >("show")!=NULL )
    {
    // printF("\n *********** AT END parameters.dbase.get<Ogshow* >("show")->endFrame(); *********\n");
        real times=getCPU();
        parameters.dbase.get<Ogshow* >("show")->endFrame();  
        times=getCPU()-times;
        if( debug() & 1 )
        {
            printF("advance: time to endFrame and save show=%8.2e.\n",times);
            fflush(0);
        }
    }
    timing(parameters.dbase.get<int>("timeForShowFile"))+=getCPU()-timea;
    

    return 0;

}


//==================================================================
//        Method of Lines Time Stepping Auxillary Routines 
//        ------------------------------------------------
//
//==================================================================

//\begin{>>DomainSolverInclude.tex}{\subsection{getUt}} 
void DomainSolver::
getUt( GridFunction & cgf, 
              const real & t, 
              RealCompositeGridFunction & ut, 
              real tForce )
// ======================================================================================
// /Description:
//     
//\end{DomainSolverInclude.tex}  
// ======================================================================================
{
    if( Parameters::checkForFloatingPointErrors!=0 )
        checkSolution(cgf.u,"getUt:start");

    const int & myid = parameters.dbase.get<int >("myid");
    FILE * debugFile = parameters.dbase.get<FILE* >("debugFile");
    FILE * pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");

//    if( debug() & 8  )
//    {
//      fprintf(debugFile," ***DomainSolver::getUt:before primitiveToConservative: cgf.u *** t=%e dt=%e\n",cgf.t,parameters.dbase.get<real >("dt"));
//      outputSolution( cgf.u, cgf.t );
//    }
  // If necessary, convert to conservative variables
    if( parameters.useConservativeVariables() &&
            cgf.form==GridFunction::primitiveVariables )
    {
        cgf.primitiveToConservative();
        if( Parameters::checkForFloatingPointErrors!=0) 
            checkSolution(cgf.u,"getUt:after primToCons");
    }
//    if( debug() & 8  )
//    {
//      fprintf(debugFile," ***DomainSolver::getUt:after primitiveToConservative: cgf.u *** t=%e dt=%e\n",cgf.t,parameters.dbase.get<real >("dt"));
//      outputSolution( cgf.u, cgf.t );
//    }

    int iparam[10];
    real rparam[10];
    rparam[0]=t;
    rparam[1]=tForce;
    rparam[2]=t; // tImplicit
    iparam[2]=numberOfStepsTaken;

    for( int grid=0; grid<cgf.cg.numberOfComponentGrids(); grid++ )
    {
        iparam[0]=grid;
        iparam[1]=cgf.cg.refinementLevelNumber(grid);

    // checkSolution(cgf.u,sPrintF("getUt:before getUt for grid=%i",grid));

//     mappedGridSolver[grid]->getUt(cgf.u[grid],cgf.getGridVelocity(grid),ut[grid],iparam,rparam,
//                Overture::nullRealMappedGridFunction(),&cgf.cg[grid]);
        getUt(cgf.u[grid],cgf.getGridVelocity(grid),ut[grid],iparam,rparam,
        	  Overture::nullRealMappedGridFunction(),&cgf.cg[grid]);
    }
    
}




//\begin{>>DomainSolverInclude.tex}{\subsection{interpolateAndApplyBoundaryConditions}} 
int DomainSolver::
interpolateAndApplyBoundaryConditions( GridFunction & cgf,
                                                                              GridFunction *uOld /* =NULL */, 
                                                                              const real & dt /* =-1. */ )
// =================================================================================================
// /Description:
//      Apply boundary conditions for an overlapping grids with refinements.
// 
//\end{DomainSolverInclude.tex}  
// =================================================================================================
{
    if( parameters.dbase.get<int>("simulateGridMotion")>0 ) return 0;

    const int & myid = parameters.dbase.get<int >("myid");
    FILE * debugFile = parameters.dbase.get<FILE* >("debugFile");
    FILE * pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");

    checkArrayIDs(sPrintF(" interpolateAndApplyBoundaryConditions (start) steps=%i t=%g",
                                                    parameters.dbase.get<int >("globalStepNumber"),cgf.t)); 
    if( debug() & 4 )
        Overture::printMemoryUsage(sPrintF("interpAndApplyBC (start) step=%i",parameters.dbase.get<int >("globalStepNumber")),stdout);

    if( Parameters::checkForFloatingPointErrors!=0 )
        checkSolution(cgf.u,"interpAndApplyBC:start",true);

    if( debug() & 4 ) 
        printMessageInfo(sPrintF("\n****interpolateAndApplyBoundaryConditions::START:(step=%i)",parameters.dbase.get<int >("globalStepNumber")));

    char buff[80];
    if( debug() & 16 )
    {
        cgf.u.display(sPrintF(buff,"interpolateAndApplyBCs at START: u (t=%8.2e)",cgf.t),debugFile,"%5.3f ");

      determineErrors( cgf,sPrintF("interpolateAndApplyBCs: START: errors at t=%e \n",cgf.t) );

    }

    if( debug() & 32 )  // *wdh* 060713
    {
        fPrintF(debugFile,"interpolateAndApplyBCs at START: u (t=%8.2e)\n",cgf.t);
        outputSolution( cgf.u,cgf.t );
    }

    if( debug() & 32 )
        cgf.u.display("interpolateAndApplyBCs START (again): u",debugFile,"%9.6f ");
    
    #ifdef USE_PPP
        real timea=getCPU();
        for( int grid=0; grid<cgf.cg.numberOfComponentGrids(); grid++ )
        {
            realArray & un = cgf.u[grid];
            if( debug() & 16 )
            {
      	realSerialArray unLocal; getLocalArrayWithGhostBoundaries(un,unLocal);
      	display(unLocal,sPrintF("interpAndApplyBC: before updateGhostBoundaries: processor=%i t=%e",
                        				myid,cgf.t),pDebugFile,"%8.2e ");
            }
        
      // **** at this point we really only need to update interior-ghost points needed for
      //      interpolation or boundary conditions

            if( debug() & 16 ) 
                printf("interpAndBC: Before un.updateGhostBoundaries() for grid=%i t=%8.2e p=%i...\n",grid,cgf.t,myid);      

            un.updateGhostBoundaries();

            if( debug() & 16 ) 
                printf("interpAndBC: After un.updateGhostBoundaries() for grid=%i t=%8.2e p=%i...\n",grid,cgf.t,myid);      

            if( debug() & 16 )
            {
      	realSerialArray unLocal; getLocalArrayWithGhostBoundaries(un,unLocal);
      	display(unLocal,sPrintF("interpAndApplyBC: after updateGhostBoundaries: processor=%i t=%e",
                        				myid,cgf.t),pDebugFile,"%8.2e ");
            }
        }

        if( debug() & 32 )
            cgf.u.display("interpolateAndApplyBCs After update ghost: u",debugFile,"%9.6f ");

        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateGhostBoundaries"))+=getCPU()-timea;
    #endif



  // fix up points before interpolating *wdh* 020521
    if( true && parameters.dbase.get<int >("orderOfAccuracy")!=4 ) // *** this needs to be fixed for 4th order INS
    {
        const int fixupUnusedPointsFrequency = parameters.dbase.get<DataBase >("modelParameters").get<int>("fixupUnusedPointsFrequency");
        assert( fixupUnusedPointsFrequency>=0 );
        if( (parameters.dbase.get<int >("globalStepNumber") % fixupUnusedPointsFrequency) == 0)
        {
//      printf(">>interpolateAndApplyBoundaryConditions Fixup unused points: parameters.dbase.get<int >("globalStepNumber")=%i, t=%e\n",
//                 parameters.dbase.get<int >("globalStepNumber"),cgf.t);
        
            fixupUnusedPoints(cgf.u);
            if( Parameters::checkForFloatingPointErrors!=0 ) 
                checkSolution(cgf.u,"after:fixupUnusedPoints");
            
            if( debug() & 16 ) 
                cgf.u.display("interpolateAndApplyBCs after fixup unused: u",debugFile,"%9.6f ");
        }
        
    }

  // kkc 070125 XXXXX BILL : are the following two lines a problem in the other solvers...
    cgf.u.getInterpolant()->bcParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfExtrapolationForSecondGhostLine"); // 1;
    cgf.u.getInterpolant()->bcParams.extrapolationOption=parameters.dbase.get<BoundaryConditionParameters::ExtrapolationOptionEnum >("extrapolationOption");
        

    if( debug() & 32 )
        cgf.u.display("interpolateAndApplyBCs Before interpolate: u",debugFile,"%9.6f ");

    checkArrayIDs(" interpolateAndApplyBoundaryConditions (before interpolate)"); 

    interpolate(cgf);  // call this one so it is timed.

    checkArrayIDs(" interpolateAndApplyBoundaryConditions (after interpolate)"); 

    if( Parameters::checkForFloatingPointErrors!=0 )
        checkSolution(cgf.u,"interpAndApplyBC:after interp");

    if( debug() & 16 )  // *wdh* 060713
    {
        fPrintF(debugFile,"interpolateAndApplyBCs After interpolate: u (t=%8.2e)\n",cgf.t);
        outputSolution( cgf.u,cgf.t );
    }

    if( debug() & 32 )
    {
      // *wdh* 060511
        #ifdef USE_PPP
          timea=getCPU();
          for( int grid=0; grid<cgf.cg.numberOfComponentGrids(); grid++ )
              cgf.u[grid].updateGhostBoundaries();
          parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateGhostBoundaries"))+=getCPU()-timea;
        #endif
        cgf.u.display("interpolateAndApplyBCs After interpolate: u",debugFile,"%9.6f ");
    }
    
  // update any time independent state variables (such as an equation of state)
  // Here, cgf is u+dt*ut in conserved variables.  Update only for j1=n1a:n1b, etc
    updateStateVariables(cgf,0);

    if( debug() & 32 )  // *** 060713
    {
        fPrintF(debugFile,"interpolateAndApplyBCs after updateState(I): u (t=%8.2e)\n",cgf.t);
        outputSolution( cgf.u,cgf.t );
    }


    if( debug() & 64 ) cgf.u.display("interpolateAndApplyBCs After updateStateVariables: u",debugFile,"%11.8f ");

    if( debug() & 4 ) printMessageInfo( "interpAndApplyBCs::After Intep BEFORE applyBC:");

    checkArrayIDs(" interpolateAndApplyBoundaryConditions (before applyBC's)"); 
    if( debug() & 32 )
        cgf.u.display(sPrintF(buff,"interpolateAndApplyBCs BEFORE applyBoundaryConditions: u (t=%8.2e)",cgf.t),
                  debugFile,"%9.6f ");

  // 5. Now apply true boundary conditions.
    applyBoundaryConditions(cgf,-1,-1,uOld,dt);
  // cgf.u.display("interpolateAndApplyBCs After applyBoundaryConditions(1): u",debugFile,"%8.5f ");

    if( debug() & 4 ) printMessageInfo( "interpAndApplyBCs::After Intep AFTER applyBC:");
    checkArrayIDs(" interpolateAndApplyBoundaryConditions (after applyBC's)"); 

    if( debug() & 32 )
        cgf.u.display(sPrintF(buff,"interpolateAndApplyBCs AFTER applyBoundaryConditions: u (t=%8.2e)",cgf.t),
                  debugFile,"%9.6f ");


    if( debug() & 32 )  // *** 060713
    {
        fPrintF(debugFile,"interpolateAndApplyBCs after applyBoundaryConditions: u (t=%8.2e)\n",cgf.t);
        outputSolution( cgf.u,cgf.t );
    }

  // update any time independent state variables (such as an equation of state)
  // Here, cgf is in primitive variables.  Update only for j1=n1a-1:n1a, j1=n1b:n1b+1, etc and
  // extrapolate the other ghost cells.
    updateStateVariables(cgf, 1);

    if( debug() & 32 )  // *** 060713
    {
        fPrintF(debugFile,"interpolateAndApplyBCs after updateState(II): u (t=%8.2e)\n",cgf.t);
        outputSolution( cgf.u,cgf.t );
    }

  // *wdh* 050322
    #ifdef USE_PPP
        timea=getCPU();
        for( int grid=0; grid<cgf.cg.numberOfComponentGrids(); grid++ )
        {
            realArray & un = cgf.u[grid];
            un.updateGhostBoundaries();
        }
        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateGhostBoundaries"))+=getCPU()-timea;
    #endif


    if( debug() & 16 )
    {
        char buff[80];
        cgf.u.display(sPrintF(buff,"interpolateAndApplyBCs After applyBoundaryConditions(2): u (t=%8.2e)",cgf.t),
                        debugFile,"%8.5f ");

        determineErrors( cgf,sPrintF("interpolateAndApplyBCs: After applyBoundaryConditions: errors at t=%e \n",cgf.t) );
    }

    if( Parameters::checkForFloatingPointErrors!=0 )
        checkSolution(cgf.u,"interpAndApplyBC:END");

    if( debug() & 32 ) // *wdh* 060713
    {
        fPrintF(debugFile,"interpolateAndApplyBCs at END: u (t=%8.2e)\n",cgf.t);
        outputSolution( cgf.u,cgf.t );
    }

    if( debug() & 4 ) printMessageInfo( "interpolateAndApplyBoundaryConditions::END:");


    checkArrayIDs(" interpolateAndApplyBoundaryConditions (end)"); 
    if( debug() & 4 )
        Overture::printMemoryUsage(sPrintF("interpAndApplyBC (end) step=%i",parameters.dbase.get<int >("globalStepNumber")),stdout);

    return 0;
}

//\begin{>>DomainSolverInclude.tex}{\subsection{advanceMidPoint}} 
void DomainSolver::
advanceMidPoint( real & t0, real & dt0, int & numberOfSubSteps, int initialStep  )
// =======================================================================================
// /Description:
//   Advance the solution using the mid-point rule.
//
//\end{DomainSolverInclude.tex}  
// =======================================================================================
{
  // checkArrays("advanceMidPoint, start");  

    cout << "midPoint: numberOfSubsteps=" << numberOfSubSteps << endl;

    const int & myid = parameters.dbase.get<int >("myid");
    FILE * debugFile = parameters.dbase.get<FILE* >("debugFile");
    FILE * pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");

  // these point into the gf[m] array
  // m0 = current solution, m1 holds du/dt and m2,m3 hold substep info
  // (use AdamsPCData for mid-point rule as well)
    if( !parameters.dbase.get<DataBase >("modelData").has_key("midPointData") )
        parameters.dbase.get<DataBase >("modelData").put<AdamsPCData>("midPointData");
    AdamsPCData & midPointData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("midPointData");
    
    int &m0 =midPointData.mab0, &m1=midPointData.mab1, &m2=midPointData.mab2;

    for( int mst=1; mst<=numberOfSubSteps; mst++ )
    {
    // u[m1] <- u[m0] + .5*dt0 d(u[m0])/dt  
        eulerStep(  t0,t0,t0+.5*dt0,.5*dt0, gf[m0],gf[m0],gf[m1], fn[0],fn[0],mst,numberOfSubSteps );
        
        output( gf[m1],initialStep+mst-1 );

    //    u[m2] <- u[m0] + dt0* du[m1]/dt
        eulerStep(  t0,t0+.5*dt0,t0+dt0,dt0, gf[m0],gf[m1],gf[m2], fn[0],fn[0],mst,numberOfSubSteps );
  
        output( gf[m2],initialStep+mst );
        
/* ---- old:
    // u[m2] <- u[m0] + .5*dt0 d(u[m0])/dt  
        eulerStep( t0,t0,t0+.5*dt0,.5*dt0, gf[m0],gf[m0],gf[m2], gf[m1],gf[m1] );
    //    u[m3] <- u[m0] + dt0* du[m2]/dt
        eulerStep( t0,t0+.5*dt0,t0+dt0,dt0, gf[m0],gf[m2],gf[m3], gf[m1],gf[m1] );
---- */

        if( debug() & 8 )
        {
            fprintf(debugFile,"======midpoint Errors After step 2, t0+dt0=%e \n",t0+dt0);
            determineErrors( gf[m2] );
        }
        
        t0+=dt0;
    // switch m2<->m0
        int mt=m0; m0=m2; m2=mt;
    }

  // update the current solution:  
    current = m0;
    
  // checkArrays("advanceMidPoint, end");  
}


