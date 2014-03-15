// This file automatically generated from advanceEuler.bC with bpp.
#include "DomainSolver.h"
#include "CompositeGridOperators.h"
#include "GridCollectionOperators.h"
#include "interpPoints.h"
#include "ExposedPoints.h"
#include "PlotStuff.h"
#include "InterpolateRefinements.h"
#include "Regrid.h"
#include "Ogen.h"
#include "updateOpt.h"
#include "ParallelUtility.h"

static bool useNewExposedPoints=true;

int
checkForSymmetry(realCompositeGridFunction & u, Parameters & parameters, const aString & label,
                                  int numberOfGhostLinesToCheck);

void DomainSolver::
eulerStep(const real & t1, const real & t2, const real & t3In, const real & dtIn,
        	  GridFunction & cgf1,  
        	  GridFunction & cgf2,  
        	  GridFunction & cgf3,  
        	  realCompositeGridFunction & ut, 
        	  realCompositeGridFunction & uti,
                    int stepNumber,
                    int & numberOfSubSteps )
//===========================================================================================
// /Description:
//   Take a Generalized Euler Step
//   \begin{vebatim}
//           u3(t3) <- u1 + dt*du2(t2)/dt
//   \end{verbatim}
//   /t1,t2,t3,dt : times 
//   /cgf1,cgf2 (input): grid functions
//   /cgf3 (output): grid functions
//   /ut,uti (work space)   : grid functions for holding time derivatives
//
// /Note: cgf3 must be different than cgf1 and cgf2. cgf1 and cg2 may be the same.
//===========================================================================================
{

    checkArrays(" eulerStep, start");  
    parameters.dbase.get<int >("globalStepNumber")++;

    if( debug() & 4 )
    {
    // fPrintF(parameters.dbase.get<FILE* >("debugFile"),"\n ----------- DomainSolver::eulerStep: START t=%e ------------- \n",cgf1.t);
        fPrintF(stdout,"\n ----------- DomainSolver::eulerStep: START t=%e ------------- \n",cgf1.t);
    }
    

    real time0=getCPU();

    real dt0 = dtIn;
    real t3=t3In;
    

    int grid;
    Index I1,I2,I3;  
    Index N = parameters.dbase.get<Range >("Rt");  // time dependent components
    RealArray error(numberOfComponents()+3); 
    int iparam[10];
    real rparam[10];

    FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");
    FILE *pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");
    
    const int regridFrequency = parameters.dbase.get<int >("amrRegridFrequency")>0 ? parameters.dbase.get<int >("amrRegridFrequency") :
                                                            parameters.dbase.get<Regrid* >("regrid")==NULL ? 2 : parameters.dbase.get<Regrid* >("regrid")->getRefinementRatio();

    if( parameters.isAdaptiveGridProblem() && ((parameters.dbase.get<int >("globalStepNumber") % regridFrequency) == 0) )
    {
    // ****************************************************************************
    // ****************** Adaptive Grid Step  *************************************
    // ****************************************************************************

        if( debug() & 2 )
        {
            printP("***** EulerStep: AMR regrid at step %i t=%e dt=%8.2e***** \n",parameters.dbase.get<int >("globalStepNumber"),cgf1.t,dt0);
            fPrintF(debugFile,"***** EulerStep: AMR regrid at step %i t=%e dt=%8.2e***** \n",parameters.dbase.get<int >("globalStepNumber"),cgf1.t,dt0);
        }
        
        real timea=getCPU();
      
        if( debug() & 4 )
            fPrintF(debugFile,"\n ***** EulerStep: AMR regrid at step %i ***** \n\n",parameters.dbase.get<int >("globalStepNumber"));

        if( false ) // mask-problems
        {
            printF(">>>>eulerStep: before AMR regrid :\n");
            
            for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
            {
                cgf1.cg[grid].update(MappedGrid::THEmask | MappedGrid::THEvertex );
                cgf3.cg[grid].update(MappedGrid::THEmask | MappedGrid::THEvertex );
      	
      	printf(" grid=%i &cgf1.cg[grid].mask()=%i &cgf3.cg[grid].mask()=%i \n",grid,
             	       &cgf1.cg[grid].mask()(0,0,0),&cgf3.cg[grid].mask()(0,0,0));
      	printf(" grid=%i &cgf1.cg[grid].vertex()=%i &cgf3.cg[grid].vertex()=%i \n",grid,
             	       &cgf1.cg[grid].vertex()(0,0,0),&cgf3.cg[grid].vertex()(0,0,0));
            }
        }

        if( debug() & 8 )
        {
            if( parameters.dbase.get<bool >("twilightZoneFlow") )
            {
      	fPrintF(debugFile," eulerStep: errors before regrid, t=%e \n",cgf1.t);
      	determineErrors( cgf1 ) ;
            }
            else
            {
        // displayMask(cgf1.cg[0].mask(),"***aafter regrid:cgf1.cg[0].mask() ***a",debugFile);

      	if( parameters.dbase.get<int >("myid")==0 ) 
                    fprintf(debugFile," ***before regrid: solution ***\n");
      	outputSolution( cgf1.u,cgf1.t );
            }
        }

    //     if( parameters.useConservativeVariables() )
    //       cgf1.primitiveToConservative(parameters);  // *wdh* 010318  -- do amr interp on conservative variables.
        
    //     if( debug() & 8 && cgf1.cg.numberOfComponentGrids()>1 )
    //     {
    //        char buff[80];
    //        GenericGraphicsInterface & ps = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
    //        GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");      
    //        psp.set(GI_TOP_LABEL,sPrintF(buff,"BEFORE AMR regrid, solution at time %f",cgf1.t));
    //        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
    //        ps.erase();
        
    //        realCompositeGridFunction v;
    //        realCompositeGridFunction & uu = getAugmentedSolution(cgf1,v); 
    //        PlotIt::contour(ps,uu,psp);
    //     }

//      if( true ) // move bug
//      {
//        printF("eulerStep: before adaptGrids: cgf1.cg[1].getName()=%s cgf3.cg[1].getName=%s\n",
//  	     (const char*)cgf1.cg[1].getName(),(const char*)cgf3.cg[1].getName());
//      }

        int numberToUpdate=0; // we need to update ub to live on the new grid, and interpolate values.

        adaptGrids( cgf1, numberToUpdate,0, &ut );  // use ut as work space, we could use cgf3 as temp space??

    // the next has been moved into adaptGrids 070706
    //     real time1=getCPU();
    //     cgf1.cg.rcData->interpolant->updateToMatchGrid( cgf1.cg ); 
    //     parameters.dbase.get<RealArray>("timing")(Parameters::timeForUpdateInterpolant)+=getCPU()-time1;
        real time1=getCPU();
        
        if( debug() & 8 ) printF("eulerStep:adapt step: update cgf1 for moving grids...\n");
        updateForMovingGrids(cgf1);  // ****

    // do here for now -- we shouldn't do this in updateForMovingGrids since this is not correct below
    // when the grids are moved
        cgf1.gridVelocityTime=cgf1.t -1.e10; 

      // we need to recompute the grid velocity on AMR grids -- really only need to do refinements***
        if( parameters.isMovingGridProblem() )
        {
      // recompute the grid velocity
            getGridVelocity( cgf1, cgf1.t );
        }

        if( debug() & 8 )
        {
            outputSolution( cgf1.u,cgf1.t,sPrintF(" eulerStep:after adaptGrids, before interpAndApplyBC at t=%11.4e \n",cgf1.t) );
        }

        interpolateAndApplyBoundaryConditions( cgf1 );
        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAmrBoundaryConditions"))+=getCPU()-time1;    

    // checkFor symmetry here ---------------------------------------------
        if( false )
        {
            aString buff;
            int nGhost=2;
            int rt=checkForSymmetry(cgf1.u,parameters,sPrintF(buff,"after adaptGrids, t=%8.2e: u",cgf1.t),nGhost);
            if( rt!=0 )
            {
      	fprintf(debugFile," ***after adaptGrids: Symmetry broken! solution ***\n");
      	outputSolution( cgf1.u,cgf1.t );
            }
        }

//     if( debug() & 8 && cgf1.cg.numberOfComponentGrids()>1 )
//     {
//       char buff[80];
//        GenericGraphicsInterface & ps = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
//        GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");      
//        psp.set(GI_TOP_LABEL,sPrintF(buff,"After AMR regrid, solution at time %f",cgf1.t));
//        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
//        ps.erase();
//        realCompositeGridFunction v;
//        realCompositeGridFunction & uu = getAugmentedSolution(cgf1,v); 
//        PlotIt::contour(ps,uu,psp);
//     }
        

        if( debug() & 8 )
        {
            if( parameters.dbase.get<bool >("twilightZoneFlow") )
            {
      	fPrintF(debugFile," eulerStep: errors after regrid, t=%e \n",cgf1.t);
      	determineErrors( cgf1 ) ;
            }
            else
            {
        // displayMask(cgf1.cg[0].mask(),"***aafter regrid:cgf1.cg[0].mask() ***a",debugFile);

      	if( parameters.dbase.get<int >("myid")==0 ) 
                    fprintf(debugFile," ***after regrid: solution ***\n");
      	outputSolution( cgf1.u,cgf1.t );
            }
        }
        
        assert( &cgf1==&cgf2 );

        if( parameters.isMovingGridProblem() )
        {
//        if( true ) // move bug
//        {
//  	printf("eulerStep: before copy: cgf1.cg[1].getName()=%s cgf3.cg[1].getName=%s\n",
//  	       (const char*)cgf1.cg[1].getName(),(const char*)cgf3.cg[1].getName());
//        }


// This works: 
//        cgf3.cg.destroy(CompositeGrid::EVERYTHING);
//        cgf3.cg.setNumberOfGrids(0);
            

            if( false ) // ********* TEMP FIX FOR bug with parallel AMR and moving ******************
      	cgf3.cg->gridDistributionList.clear();
            
            cgf3.cg=cgf1.cg;  // we make a copy in this case

            if( false ) // problem with AMR and moving in parallel -- parallel partitions don't match on cgf2 and cgf1 *****************
            {
      	const int & myid = parameters.dbase.get<int >("myid");
                assert( cgf1.cg.numberOfComponentGrids() == cgf3.cg.numberOfComponentGrids() );
      	for( int grid=0; grid<cgf1.cg.numberOfComponentGrids(); grid++ )
      	{
        	  intSerialArray  mask1Local; getLocalArrayWithGhostBoundaries(cgf1.cg[grid].mask(),mask1Local);
        	  intSerialArray  mask3Local; getLocalArrayWithGhostBoundaries(cgf3.cg[grid].mask(),mask3Local);
                    assert( &cgf1.cg[grid].mask() == &cgf2.cg[grid].mask() );
        	  for( int axis=0; axis<3; axis++ )
        	  {
          	    if( mask1Local.getBase(axis) != mask3Local.getBase(axis) ||
            		mask1Local.getBound(axis) != mask3Local.getBound(axis) )
          	    {
                            printf("EULER:after cgf3.cg=cgf1.cg; ERROR: grid=%i myid=%i \n"
                                          "                              cgf1 : maskLocal=[%i,%i][%i,%i]\n"
                                          "                              cgf3 : maskLocal=[%i,%i][%i,%i]\n",
                                          grid,myid,
                 		     mask1Local.getBase(0),mask1Local.getBound(0),
                 		     mask1Local.getBase(1),mask1Local.getBound(1),
                 		     mask3Local.getBase(0),mask3Local.getBound(0),
                 		     mask3Local.getBase(1),mask3Local.getBound(1));
            	      OV_ABORT("error");
          	    }
        	  }
      	}
            }
            
        }
        else
        {
            cgf3.updateToMatchGrid(cgf1.cg);
        }
        
        cgf3.u.updateToMatchGrid(cgf3.cg);
        cgf3.u=0.;
        cgf3.u.setOperators(*cgf1.u.getOperators());
            
    // ** ut.updateToMatchGrid(cgf1.cg); // this is done in adaptGrids

    // *this is done above* updateForMovingGrids(cgf1);  // ****

        updateForMovingGrids(cgf3);  // **** 040320 *** need to update gridVelocityArrays

//      if( true ) // move bug
//      {
//        printF("eulerStep: after adaptGrids: cgf1.cg[1].getName()=%s cgf3.cg[1].getName=%s\n",
//  	     (const char*)cgf1.cg[1].getName(),(const char*)cgf3.cg[1].getName());
//      }

        if( false )
        {
            int grid=2, i1=31, i2=44, i3=0;
            const IntegerArray & d = cgf1.cg[grid].dimension();
            if( i1>=d(0,0) && i1<=d(1,0) && i2>=d(0,1) && i2<=d(1,1) )
            {
      	printf("After AMR : (grid,i1,i2,i3)=(%i,%i,%i,%i) mask=%i mask2=%i u1=%8.5f,%8.5f,%8.5f,%8.5f\n"
             	       ,grid,i1,i2,i3,cgf1.cg[grid].mask()(i1,i2,i3),cgf3.cg[grid].mask()(i1,i2,i3),
             	       cgf1.u[grid](i1,i2,i3,0),cgf1.u[grid](i1,i2,i3,1),cgf1.u[grid](i1,i2,i3,2),
             	       cgf1.u[grid](i1,i2,i3,3));

            }
        }

    // solveForTimeIndependentVariables( cgf1 ); 
        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAmrRegrid"))+=getCPU()-timea;
    }

    if( ((stepNumber % 2)==0) && ( 
            parameters.isAdaptiveGridProblem() && ((parameters.dbase.get<int >("globalStepNumber") % regridFrequency) == 0)  ||
            parameters.dbase.get<bool >("recomputeDTEveryStep") ) )
    {
    // *********************************************************************************
    // **** recompute the time step -- recompute every time step for some codes *****
    // *********************************************************************************


        if( parameters.useConservativeVariables() && !parameters.dbase.get<bool>("timeStepDataIsPrecomputed") )
            {
	// Note: no need to convert to primitive variables for some codes
      	cgf1.conservativeToPrimitive(); 
            }


        real dtNew= getTimeStep( cgf1 ); //       ===Choose time step====
        int numberOfSteps;
    //     real nextTimeToPrint=cgf1.t+(numberOfSubSteps-stepNumber+1)*dt;
    //    real nextTimeToPrint=cgf1.t+(numberOfSubSteps-stepNumber)*dt;
        real & nextTimeToPrint = parameters.dbase.get<real >("nextTimeToPrint");
        real tFinal=nextTimeToPrint;
        computeNumberOfStepsAndAdjustTheTimeStep(cgf1.t,tFinal,nextTimeToPrint,numberOfSteps,dtNew);
      	
    //    numberOfSubSteps=stepNumber+numberOfSteps-1;
        numberOfSubSteps=stepNumber+numberOfSteps;

        if( debug() & 4 )
        {
            printF("Euler: stepNumber=%i numberOfSubSteps=%i, numberOfSteps=%i\n",stepNumber,numberOfSubSteps,
                            numberOfSteps);
            printF("nextTimeToPrint=%12.6e, t+ numberOfSteps*dt=%12.6e\n",nextTimeToPrint,
           	     cgf1.t+numberOfSteps*dtNew);
        }
        
        if( debug() & 4 )
            printF("\n $$$$$$$$$$$ Euler:recompute dt: dt(old)=%8.3e, dtNew = %8.3e (t=%8.3e)\n",dt,dtNew,cgf1.t);
        dt=dtNew;
        parameters.dbase.get<real >("dt")=dt;
    // These next adjustments needed for moving grids
        if( fabs(t1+dt0 - t3) > REAL_EPSILON*(1.+fabs(t1)) )
        {
            printF("\n XXXXXXXXXXX Euler:recompute dt:ERROR t1+dt0!=t3, diff=%8.2e XXXXXXXXXXXX\n",t1+dt0 - t3);
        }
        dt0=dt;  // *wdh* 030918
        t3=t1+dt0;
    }


  //bool isMovingGridProblem = movingGridProblem();
  //printF(" advanceEuler: isMovingGridProblem=%i\n",isMovingGridProblem);
    
    if( movingGridProblem() )
    {
    // *********************************************************************************
    // *********************** Moving Grid Step ****************************************
    // *********************************************************************************

        checkArrays(" eulerStep, before move grids"); 

        if( debug() & 1 )
        {
            printF("===== eulerStep: Move the grids at stepNumber=%i globalStepNumber=%i)\n",stepNumber,parameters.dbase.get<int >("globalStepNumber"));
        }
        if( debug() & 2 )
            fPrintF(parameters.dbase.get<FILE* >("moveFile"),
                        "\n===== eulerStep: Move the grids at stepNumber=%i (globalStepNumber=%i)\n"
                                                                      "                  t1=%6.3f(cgf.t=%6.3f) t2=%6.3f(%6.3f) t3=%6.3f(%6.3f) \n",
                              stepNumber,parameters.dbase.get<int >("globalStepNumber"),t1,cgf1.t,t2,cgf2.t,t3,cgf3.t);
        
    // move cgf3, compute grid velocity for cgf2 and cgf3
        moveGrids( t1,t2,t3,dt0,cgf1,cgf2,cgf3 ); 

        if( false ) // mask-problems  
        {
            const int & myid = parameters.dbase.get<int >("myid");
            int grid=2;
            if( grid< cgf3.cg.numberOfComponentGrids() )
            {
                intArray & mask = cgf3.cg[grid].mask();
      	printf("AFTER move grids: grid=%i myid=%i cgf3.cg[grid].mask() = [%i,%i][%i,%i]\n",
             	       grid,myid,mask.getBase(0),mask.getBound(0),mask.getBase(1),mask.getBound(1));
            }
        }

        if( parameters.isAdaptiveGridProblem() )
        {
      // both moving and AMR 
            parameters.dbase.get<Ogen* >("gridGenerator")->updateRefinement(cgf3.cg);

            if( true ) // problem with AMR and moving in parallel -- parallel partitions don't match on cgf2 and cgf1 *****************
            {
      	const int & myid = parameters.dbase.get<int >("myid");
                assert( cgf2.cg.numberOfComponentGrids() == cgf3.cg.numberOfComponentGrids() );
      	for( int grid=0; grid<cgf2.cg.numberOfComponentGrids(); grid++ )
      	{
        	  realSerialArray u2Local; getLocalArrayWithGhostBoundaries(cgf2.u[grid],u2Local);
        	  intSerialArray  mask2Local; getLocalArrayWithGhostBoundaries(cgf2.cg[grid].mask(),mask2Local);
        	  realSerialArray u3Local; getLocalArrayWithGhostBoundaries(cgf3.u[grid],u3Local);
        	  intSerialArray  mask3Local; getLocalArrayWithGhostBoundaries(cgf3.cg[grid].mask(),mask3Local);
                    assert( &cgf1.cg[grid].mask() == &cgf2.cg[grid].mask() );
                    assert( &cgf1.u[grid] == &cgf2.u[grid] );
        	  for( int axis=0; axis<3; axis++ )
        	  {
          	    if( u2Local.getBase(axis) != mask3Local.getBase(axis) ||
            		u2Local.getBound(axis) != mask3Local.getBound(axis) )
          	    {
                            printf("EULER: ERROR: grid=%i myid=%i cgf2 :    uLocal=[%i,%i][%i,%i], \n"
                                          "                              cgf2 : maskLocal=[%i,%i][%i,%i]\n"
                                          "                              cgf3 :    uLocal=[%i,%i][%i,%i]\n" 
                                          "                              cgf3 : maskLocal=[%i,%i][%i,%i]\n",
                                          grid,myid,
                 		     u2Local.getBase(0),u2Local.getBound(0),
                 		     u2Local.getBase(1),u2Local.getBound(1),
                 		     mask2Local.getBase(0),mask2Local.getBound(0),
                 		     mask2Local.getBase(1),mask2Local.getBound(1),
                 		     u3Local.getBase(0),u3Local.getBound(0),
                 		     u3Local.getBase(1),u3Local.getBound(1),
                 		     mask3Local.getBase(0),mask3Local.getBound(0),
                 		     mask3Local.getBase(1),mask3Local.getBound(1));
            	      OV_ABORT("error");
          	    }
        	  }
      	}
// 	int grid=2;
// 	if( grid< cgf3.cg.numberOfComponentGrids() )
// 	{
// 	  intArray & mask = cgf3.cg[grid].mask();
// 	  printf("AFTER updateRefinement: grid=%i myid=%i cgf3.cg[grid].mask() = [%i,%i][%i,%i]\n",
// 		 grid,myid,mask.getBase(0),mask.getBound(0),mask.getBase(1),mask.getBound(1));
// 	}
            }
        }

    // note: the interpolant does not need operators
        real cpu0=getCPU();
        cgf3.cg.rcData->interpolant->updateToMatchGrid( cgf3.cg );  
        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateInterpolant"))+=getCPU()-cpu0;
        
    // ****** fix this -- need different operators
    // ****    cgf3.u.getOperators()->updateToMatchGrid(cgf3.cg);
    // **** operators are needed by cfg2 !  *** fix this ****
        cpu0=getCPU();
        cgf2.u.getOperators()->updateToMatchGrid(cgf2.cg); 
        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateOperators"))+=getCPU()-cpu0;
    // cgf3.cg.rcData->interpolant->updateToMatchGrid( cgf3.cg );

        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForMovingUpdate"))+=getCPU()-cpu0;

        updateForMovingGrids(cgf3);
    //    cgf3.u.updateToMatchGrid( cgf3.cg );    // won't work if number of grid points changes and u1==u3

        if( debug() & 32 )
        {
            fPrintF(debugFile," eulerStep: u1 before interpExposed t=%e \n",cgf1.t);
            outputSolution( cgf1.u,cgf1.t ) ;
        }


        cpu0=getCPU();
    // get values for exposed points on cgf1.cg
    // *** first two args here cannot be the same
        if( useNewExposedPoints )
        {
            ExposedPoints exposedPoints;
            exposedPoints.setAssumeInterpolationNeighboursAreAssigned(parameters.dbase.get<int >("extrapolateInterpolationNeighbours"));
            exposedPoints.initialize(cgf1.cg,cgf3.cg,parameters.dbase.get<int >("stencilWidthForExposedPoints"));
            
            exposedPoints.interpolate(cgf1.u,(twilightZoneFlow() ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),t1);
            if( &cgf1 != &cgf2 )  // needed for du/dt:
            {
      	exposedPoints.initialize(cgf2.cg,cgf3.cg,parameters.dbase.get<int >("stencilWidthForExposedPoints"));
      	exposedPoints.interpolate(cgf2.u,(twilightZoneFlow() ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),t2);
            }

            if( false ) // *wdh* 110718
            {
                MappedGrid & mg67 = cgf3.cg[67];
      	::display(mg67.gridIndexRange(),"Grid 67 gridIndexRange");
      	
                ::displayMask(cgf3.cg[67].mask(),"Grid 67 mask (new) -  After ExposedPoints");
                ::display(cgf1.u[67]," After ExposedPoints, solution at old time grid=67","%6.3f ");
      	
      	GenericGraphicsInterface & ps = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
      	GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");    
      	ps.erase();
                psp.set(GI_TOP_LABEL,sPrintF("After ExposedPoints, solution at old time %f",cgf1.t));
      	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
                PlotIt::contour(ps,cgf1.u,psp);
            }

        }
        else
        {
            interpolateExposedPoints(cgf1.cg,cgf3.cg,cgf1.u, (twilightZoneFlow() ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),t1,
                         			       false,Overture::nullIntArray(),Overture::nullIntegerDistributedArray(),
                                                              parameters.dbase.get<int >("stencilWidthForExposedPoints") );  
            if( &cgf1 != &cgf2 )  // needed for du/dt:
      	interpolateExposedPoints(cgf2.cg,cgf3.cg,cgf2.u,(twilightZoneFlow() ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),t2,
                         				 false,Overture::nullIntArray(),Overture::nullIntegerDistributedArray(),
                                                                  parameters.dbase.get<int >("stencilWidthForExposedPoints") ); 
        }
        
        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInterpolateExposedPoints"))+=getCPU()-cpu0;

    //  printF(" *** exposedPoints: extrapolateInterpolationNeighbours=%i stencilWidthForExposedPoints=%i\n",
    //      parameters.dbase.get<int >("extrapolateInterpolationNeighbours"), parameters.dbase.get<int >("stencilWidthForExposedPoints"));
            
        if( false ) // mask-problems
        {
            int grid=2, j1=31, j2=44, i3=0;
            const IntegerArray & d = cgf1.cg[grid].dimension();
            for( int i2=j2-2; i2<=j2+2; i2++ )
      	for( int i1=j1-2; i1<=j1+2; i1++ )
      	{
        	  if( i1>=d(0,0) && i1<=d(1,0) && i2>=d(0,1) && i2<=d(1,1) )
        	  {
          	    printF("After interpExposed: (grid,i1,i2,i3)=(%i,%i,%i,%i) mask=%i mask2=%i u1=%8.5f,%8.5f,%8.5f,%8.5f\n"
               		   ,grid,i1,i2,i3,cgf1.cg[grid].mask()(i1,i2,i3),cgf3.cg[grid].mask()(i1,i2,i3),
                                        cgf1.u[grid](i1,i2,i3,0),cgf1.u[grid](i1,i2,i3,1),cgf1.u[grid](i1,i2,i3,2),
                                        cgf1.u[grid](i1,i2,i3,3));
        	  }
      	}
        }
        

        if( debug() & 32 )
        {
      // cgf1.cg[0].mask()=cgf3.cg[0].mask();
            fPrintF(debugFile," eulerStep: u1 after interpExposed t=%e \n",cgf1.t);
            outputSolution( cgf1.u,cgf1.t ) ;
        }

        checkArrays(" eulerStep, after move grids"); 

        
    }  // end moving grid problem
    


    if( debug() & 16 )
    {
        fprintf(pDebugFile," eulerStep: errors in u1 on input t=%e \n",cgf1.t);
        determineErrors( cgf1 ) ;
        fprintf(pDebugFile," eulerStep: errors in u2 on input t=%e \n",cgf2.t);
        determineErrors( cgf2 ) ;
    }

    if( parameters.useConservativeVariables() )
    {
        cgf1.primitiveToConservative();
        cgf2.primitiveToConservative();
        cgf3.form=cgf2.form;
    }

    real tForce = parameters.dbase.get<int >("explicitMethod") ? cgf2.t : cgf2.t+dt0*.5;
  // printF("################# forwardEuler : cgf2.t =%9.3e t2=%9.3e diff=%9.3e\n",cgf2.t,t2,fabs(cgf2.t-t2));

  // -- evaluate any body forcing (this is saved in realCompositeGridFunction bodyForce found in the data-base) ---
    computeBodyForcing( cgf2, tForce );

    for( grid=0; grid<cgf2.cg.numberOfComponentGrids(); grid++ )
    {
        rparam[0]=t2;
        rparam[1]=tForce;
        rparam[2]=t2; // tImplicit
        iparam[0]=grid;
        iparam[1]=cgf2.cg.refinementLevelNumber(grid);
        iparam[2]=numberOfStepsTaken;

    // **********************************************
    // *********** get du/dt ************************
    // **********************************************

        int returnValue=getUt(cgf2.u[grid],cgf2.getGridVelocity(grid),ut[grid],iparam,rparam,
                                                    Overture::nullRealMappedGridFunction(),&cgf3.cg[grid],&cgf3.getGridVelocity(grid));

    // ************wdh 060302
    // ut[grid].updateGhostBoundaries();


        if( returnValue!=0 ) 
        {
            printF("eulerStep:ERROR:return from mappedGridSolver for grid=%i, error code=%i\n",grid,returnValue);
            printF("I am going to plot the solution that caused the problem\n");
            char buff[80];
            GenericGraphicsInterface & ps = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
            GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");      
            psp.set(GI_TOP_LABEL,sPrintF(buff,"Solution at time %f",cgf2.t));
            psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
            ps.erase();

            realCompositeGridFunction v;
            realCompositeGridFunction & uu = getAugmentedSolution(cgf2,v); 
            PlotIt::contour(ps,uu,psp);
        }

        if( debug() & 64 )
        {
              

            if( parameters.gridIsMoving(grid) )
                display(cgf2.getGridVelocity(grid),"cgf2[grid].gridVelocity",debugFile);

      // fflush(debugFile);
      // Communication_Manager::Sync();
            ::display(ut[grid],sPrintF(" eulerStep:du/dt at t=%9.4e \n",cgf2.t),debugFile,"%12.5e ");
      // fflush(debugFile);
      // Communication_Manager::Sync();
        }

        if( debug() & 16 )
        {
            outputSolution( ut,cgf2.t,sPrintF(" eulerStep:du/dt at t=%11.4e \n",cgf2.t) );
        }


//    dudt( t2,grid,cgf2.cg[grid],cgf2.cg[grid].mask(),cgf2.u[grid],cgf2.getGridVelocity(grid),
//          cgft.u[grid],cgfti.u[grid] );  // get du/dt  *** cgft.cg must match cgf2.cg ***

        real timeForAddUt=getCPU();
    // *wdh* 030623     getIndex(cgf3.cg[grid].extendedIndexRange(),I1,I2,I3);
    // copy all points since consPrim wants to set values all points
    // ut will be zero at unused points

        const bool useOptUpdate=true;
        if( useOptUpdate )
        {
          #ifdef USE_PPP
            RealArray u1; getLocalArrayWithGhostBoundaries(cgf1.u[grid],u1);
            const RealArray & u3 = cgf3.u[grid].getLocalArray();
            const RealArray & uta= ut[grid].getLocalArray();
            const intSerialArray & mask1 = cgf3.cg[grid].mask().getLocalArray();
          #else
            RealArray & u1 = cgf1.u[grid];
            RealArray & u3 = cgf3.u[grid];
            RealArray & uta= ut[grid];
            const intArray & mask1 = cgf3.cg[grid].mask();
          #endif

            getIndex(cgf3.cg[grid].dimension(),I1,I2,I3);
            const int n1a=max(u1.getBase(0),I1.getBase()), n1b=min(u1.getBound(0),I1.getBound());  
            const int n2a=max(u1.getBase(1),I2.getBase()), n2b=min(u1.getBound(1),I2.getBound());
            const int n3a=max(u1.getBase(2),I3.getBase()), n3b=min(u1.getBound(2),I3.getBound());

      	
            int ierr=0;
            const int option=1;     // add on one "ut"
            const int maskOption=1; // copy all points, do not use the mask
            int ipar[]={option,maskOption,n1a,n1b,n2a,n2b,n3a,n3b,N.getBase(),N.getBound()}; //
            real rpar[5]={dt0,0.,0.,0.,0.};
            real *utap= uta.getDataPointer();

            updateOpt(u1.getBase(0),u1.getBound(0),u1.getBase(1),u1.getBound(1),
            		u1.getBase(2),u1.getBound(2),u1.getBase(3),u1.getBound(3),
            		*mask1.getDataPointer(),  
            		*u1.getDataPointer(),*u3.getDataPointer(), 
            		*utap, *utap, *utap, *utap,
            		ipar[0], rpar[0], ierr );

        }
        else
        {
            Index all;
            I1=all; I2=all; I3=all;
            if( &(cgf1.u) != &(cgf3.u) )
      	cgf3.u[grid](I1,I2,I3,N)=cgf1.u[grid](I1,I2,I3,N)+dt0*ut[grid](I1,I2,I3,N); 
            else
      	cgf3.u[grid](I1,I2,I3,N)+=dt0*ut[grid](I1,I2,I3,N); 
            
        }
        
        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAddUt"))+=getCPU()-timeForAddUt;
    }
    cgf3.t=cgf1.t+dt0;

    checkArrays(" eulerStep, after dudt");  

    if( false ) // mask-problems
    {
        int grid=2, i1=31, i2=44, i3=0;
        const IntegerArray & d = cgf1.cg[grid].dimension();
        if( i1>=d(0,0) && i1<=d(1,0) && i2>=d(0,1) && i2<=d(1,1) )
        {
            printF(">>>After euler step: (grid,i1,i2,i3)=(%i,%i,%i,%i) mask=%i u=%12.6e, u3=%12.6e,mask2=%i\n"
           	     ,grid,i1,i2,i3,cgf1.cg[grid].mask()(i1,i2,i3),cgf1.u[grid](i1,i2,i3,0),
           	     cgf3.u[grid](i1,i2,i3,0),cgf3.cg[grid].mask()(i1,i2,i3));
        }
    }

    if( debug() & 16 ) 
    {
    // when checking errors in ut, use the grid from cgf2 
        if( twilightZoneFlow() )
        {
            fPrintF(debugFile," eulerStep: errors in ut at t=%e \n",cgf2.t);
            fprintf(pDebugFile," eulerStep: errors in ut at t=%e \n",cgf2.t);
            determineErrors( ut,cgf2.gridVelocity, cgf2.t, 1, error );
        }
        else
        {
            aString label=sPrintF(" eulerStep: cgf2.u at t=%e \n",cgf2.t);
            outputSolution( cgf2.u,cgf2.t,label );
      // ** the next is trouble with amr on:
      // label=sPrintF(" eulerStep: ut at t=%e \n",cgf2.t);
      // outputSolution( ut,cgf2.t,label );
        }
    }

    if( debug() & 32 )
    {
        fPrintF(debugFile," eulerStep: Before interpolate: errors at t=%e \n",cgf3.t);
        fprintf(pDebugFile," eulerStep: Before interpolate: errors at t=%e \n",cgf3.t);
        determineErrors( cgf3 );
    }

    if(  movingGridProblem() )
    { // update operators on cgf3 before interpolating
        real cpu0=getCPU();
        cgf3.u.getOperators()->updateToMatchGrid(cgf3.cg);
        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateOperators"))+=getCPU()-cpu0;

    }

    if( debug() & 4 ) 
    {
        fPrintF(debugFile,"After eulerStep: interpolateAndApplyBoundaryConditions...\n");
    }
    
    interpolateAndApplyBoundaryConditions(cgf3, &cgf1, dt0);
    
    bool updateSolutionDependentEquations=true;  // e.g. for variable density, do update p eqn here 
    solveForTimeIndependentVariables( cgf3,updateSolutionDependentEquations ); 

    if( debug() & 16 )
    {
        aString label=sPrintF(" eulerStep:DONE errors at t=%e \n",cgf3.t);
        if( twilightZoneFlow() )
            determineErrors( cgf3,label );
        else
            outputSolution( cgf3.u,cgf3.t,label );
    }
    
  // correct for forces on moving bodies
    if( movingGridProblem() )
    {
        if( debug() & 8  ) fprintf(parameters.dbase.get<FILE* >("moveFile"),"<<< eulerStep: correct for moving grids cgf3.t=%6.3f\n",cgf3.t);
        
        correctMovingGrids( cgf1.t,cgf3.t, cgf1,cgf3 );   // *wdh* 040318
    }

    if( debug() & 64 )
    {
        for( grid=0; grid<cgf3.cg.numberOfComponentGrids(); grid++ )
            ::display(cgf3.u[grid],sPrintF(" eulerStep:done t=%9.4e grid=%i\n",cgf3.t,grid),debugFile,"%12.5e ");
    }


//    if( true )
//    {
//      FILE *file = stdout;
//      fprintf(file,"\n ++++++++++++eulerStep:cgf3.END+++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//      for( grid=0; grid<cgf3.cg.numberOfComponentGrids(); grid++ )
//        cgf3.cg[grid].displayComputedGeometry(file);
//      fprintf(file," +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    }


//  tm(3)+=getCPU()-time0;    // ***** ??
    checkArrays(" eulerStep, end");  


}

