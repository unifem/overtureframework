// This file automatically generated from advanceStepsIM.bC with bpp.
// ==========================================================================================
//   This file contains functions that implement separate steps in an advance routine
//   These separate steps can be combined to form a time stepping algorithm such as 
//   a predictor corrector method.
//
// These functions should probably be virtual members of an Advance class so they can be 
// over-loaded? They now implement a PC method. 
// 
//      initializeTimeStepping( t,dt,init );
//      startTimeStep( t0,dt0,advanceOptions );
//      takeTimeStep( t0,dt0,correction,advanceOptions );
//      endTimeStep( t0,dt0,advanceOptions );
// 
// Here is the anticipated usage: 
//
//   initializeTimeStepping( t,dt,init )
//   for( int subStep=0; subStep<numberOfSubSteps; subStep++ )
//   {
//     startTimeStep( t0,dt0,advanceOptions );
//     for( int correction=0; correction<numberOfCorrections; correction++ )  // these could also be stages of a RK 
//     {    
//       takeTimeStep( t0,dt0,correction,advanceOptions );
//     }
//     endTimeStep( t0,dt0,advanceOptions );
// 
//   } // end  substeps
//
//
// ==========================================================================================


#include "DomainSolver.h"
#include "CompositeGridOperators.h"
#include "GridCollectionOperators.h"
#include "interpPoints.h"
#include "SparseRep.h"
#include "ExposedPoints.h"
#include "Ogen.h"
#include "App.h"
#include "ParallelUtility.h"
#include "Oges.h"
#include "OgesParameters.h"
#include "AdamsPCData.h"
#include "gridFunctionNorms.h"
#include "updateOpt.h"
#include "AdvanceOptions.h"

static bool useNewExposedPoints=true;

// here are some bpp macros that are used for the explicit and implicit predictor-corrector methods
// This file contains some macros that are shared amongst the different predictor-corrector methods


// ==================================================================================================
// MACRO: This macro saves past values of the pressure and values of the velocity on the ghost lines
// For use with the fourth-order accurate INS solver. 
//
// tp (input) : past value of time
// nab (input) : save results in fn[nab] (NOTE: use the grid from gf[mOld] not the one with fn[nab] !)
// ==================================================================================================



// ===============================================================================
///  MACRO:  Perform the initialization step for the PC method
///
///  \METHOD (input) : name of the method: e.g. adamsPC or implicitPC
///  Parameters:
///   numberOfPastTimes (input) : method needs u(t-dt), ... u(t-n*dt), n=numberOfPastTimes  
///   numberOfPastTimeDerivatives (input) : method needs u_t(t-dt), ..., u_t(t-m*dt) m=numberOfPastTimeDerivatives
///
// ===============================================================================


// =======================================================================================================
//    Macro to move the grids at the start of a PC time step.
// Arguments:
//    METHOD : name of the calling function (for debug output)
//    utImplicit : name of the grid function that holds the explicit part of the implicit operator.
//    
//    predictorOrder : order of the predictor corrector
//    ub,uc,ud : grid functions that hold du/dt at times tb, tc, td
//               If predictorOrder==2 then explosed points are filled in on ub.
//               If predictorOrder==3 then explosed points are filled in on ub and uc.
//               If predictorOrder==4 then explosed points are filled in on ub, uc and ud.
// =======================================================================================================



// =======================================================================================================
//    Macro to correct for moving grids.
// Arguments:
//    METHOD : name of the calling function (for debug output)
// Retrun:
//   movingGridCorrectionsHaveConverged = true if this is a moving grid problem and the sub-iteration
//             corrections have converged.
// =======================================================================================================

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) for( side=0; side<=1; side++ )


// ===========================================================================================
//      *** Time-step update macro ***
// 
//         u1 <- u0 + AB1*UA0 + AB2*UB0 [ + DTI*UTIMPLICIT ]  (add last terms for implicit grids)
// ===========================================================================================

// ===================================================================================================================
/// \brief Initialize the time stepping (a time sub-step function). 
/// \details 
/// \param t0 (input) : current time
/// \param dt0 (input) : current time step
///
// ===================================================================================================================
int DomainSolver::
initializeTimeSteppingIM( real & t0, real & dt0 )
{
    int init=true;

    FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

    if( debug() & 4 )
        printF(" ====== DomainSolver::initializeTimeSteppingIM ======\n");
    if( debug() & 2 )
        fprintf(debugFile," *** DomainSolver::initializeTimeSteppingIM: t0=%e, dt0=%e *** \n",t0,dt0);
  

    assert( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit );

    assert( parameters.dbase.get<int >("orderOfPredictorCorrector")==2 );  // for now we just have 2nd-order in time

    if( !parameters.dbase.get<DataBase >("modelData").has_key("AdamsPCData") )
    {
    // this must be the initial call to this routine
        parameters.dbase.get<DataBase >("modelData").put<AdamsPCData>("AdamsPCData");
    }

    AdamsPCData & adamsData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData");
    
    real & dtb=adamsData.dtb;
    int &mab0 =adamsData.mab0, &mab1=adamsData.mab1, &mab2=adamsData.mab2;
    int &nab0 =adamsData.nab0, &nab1=adamsData.nab1, &nab2=adamsData.nab2, &nab3=adamsData.nab3;
    int &ndt0=adamsData.ndt0;
    real *dtp = adamsData.dtp;

    const int orderOfAccuracy = parameters.dbase.get<int >("orderOfAccuracy");
    const int orderOfTimeAccuracy = parameters.dbase.get<int >("orderOfTimeAccuracy");
    int & predictorOrder = parameters.dbase.get<int>("predictorOrder");
    if( predictorOrder<0 || predictorOrder>2 )
    {
        printF("DomainSolver::initializeTimeSteppingIM: ERROR: predictorOrder=%i! Will use default.\n",predictorOrder);
    }
    if( predictorOrder==0 ) // *new* wdh July 4, 2017
        predictorOrder=orderOfTimeAccuracy; // default

    if( debug() & 1 )
    {
        printF("DomainSolver::initializeTimeSteppingIM:INFO: predictorOrder=%i (orderOfTimeAccuracy=%i)\n",
                      predictorOrder,orderOfTimeAccuracy);
    }
    
//   int numberOfCorrections;
//   if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::adamsBashforth2 )
//     numberOfCorrections=0; //kkc 060301 given the assertion above, this should never happen, right?
//   else
//     numberOfCorrections=parameters.dbase.get<int>("numberOfPCcorrections"); 
  
    
  // For moving grids we keep gf[mab0], gf[mab1] and gf[mab2]
  // For non-moving grids we keep gf[mab0], gf[mab1] and we set mab2==mab1

    const int numberOfGridFunctions =  movingGridProblem() ? 3 : 2; 

    mab2 = (mab0 -1 + numberOfGridFunctions) % numberOfGridFunctions;

  // 
    int & mNew = mab2;    // new     : gf[mNew] : will hold u(t+dt)
    int & mCur = mab0;    // current : gf[mCur] : holds u(t) 
    int & mOld = mab1;    // old     : gf[mOld] : holds u(t-dt) if numberOfGridFunctions==3 otherwise mOld=mNew
    
    Parameters::ImplicitOption & implicitOption = parameters.dbase.get<Parameters::ImplicitOption >("implicitOption");
    
    implicitOption=Parameters::doNotComputeImplicitTerms; // no need to compute during initialization

    const Parameters::ImplicitMethod & implicitMethod = 
                                parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");
    assert( implicitMethod==Parameters::crankNicolson ||
                    implicitMethod==Parameters::implicitExplicitMultistep );
    
  // parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::crankNicolson;

    int iparam[10];
    real rparam[10];

    RealCompositeGridFunction & uti = fn[2];   // ****
    
      int grid;
      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2]; 
      Range N = parameters.dbase.get<Range >("Rt");   // time dependent variables
      RealArray error(numberOfComponents()+3); 
    

    
  // Form the matrix for implicit time stepping (optionally save the solution used for linearization)
  // NOTE: the matrix will only be generated the first time through or if parameters.dbase.get<int >("globalStepNumber") % refactorFrequency == 0 
  // We want to factor the matrix here since it may be needed for computing the RHS
    formMatrixForImplicitSolve(dt,gf[mNew], gf[mCur] );

  // **** To initialize the method we need to compute du/dt at times t and t-dt *****

  // this is a macro (pcMacros.h):
    const int & numberOfSolutionLevels = parameters.dbase.get<int>("numberOfSolutionLevels");
    const int & numberOfTimeDerivativeLevels = parameters.dbase.get<int>("numberOfTimeDerivativeLevels");
    int numberOfExtraPressureTimeLevels=0;
    const int numberOfPastTimes=1;                            // PC needs u(t-dt)
    const int numberOfPastTimeDerivatives=orderOfTimeAccuracy-1;  // PC needs u_t(t-dt), u_t(t-2*dt), ...
    const int orderOfPredictorCorrector = parameters.dbase.get<int >("orderOfPredictorCorrector");
    const int orderOfTimeExtrapolationForPressure = parameters.dbase.get<int >("orderOfTimeExtrapolationForPressure");
    printF("--implicitPC-- initializePredictorCorrector: mCur=%i, mOld=%i gf[mCur].t=%9.2e\n",mCur,mOld,gf[mCur].t);
    fPrintF(debugFile,"--implicitPC-- initializePredictorCorrector: mCur=%i, mOld=%i gf[mCur].t=%9.2e\n",mCur,mOld,gf[mCur].t);
    if( movingGridProblem() )
    { 
        getGridVelocity( gf[mCur],t0 );
    }
    if( orderOfTimeExtrapolationForPressure!=-1 )
    {
    // if( orderOfPredictorCorrector==2 && orderOfTimeExtrapolationForPressure>1 &&
    //     poisson!=NULL && poisson->isSolverIterative()  )
    // *wdh* 2015/01/26: we may need past time pressure for other reasons:
        const bool & predictedPressureNeeded = parameters.dbase.get<bool>("predictedPressureNeeded");
        const bool predictPressure = predictedPressureNeeded || (poisson!=NULL && poisson->isSolverIterative());
        if( orderOfPredictorCorrector==2 && orderOfTimeExtrapolationForPressure>1 && predictPressure )
        {
      // orderOfTimeExtrapolationForPressure==1 :  p(t+dt) = 2*p(t) - p(t-dt)
      //                                      2 :  p(t+dt) = 3*p(t) - 3*p(t-dt) + p(t-2*dt)
            assert( previousPressure==NULL );
            assert( !parameters.isMovingGridProblem() );  // fix for this case
            numberOfExtraPressureTimeLevels = orderOfTimeExtrapolationForPressure - 1;
            printF("--DS-- ***initPC: allocate %i extra grid functions to store the pressure at previous times ****\n",
             	   numberOfExtraPressureTimeLevels);
            previousPressure = new realCompositeGridFunction [numberOfExtraPressureTimeLevels];
            for( int i=0; i<numberOfExtraPressureTimeLevels; i++ )
            {
                previousPressure[i].updateToMatchGrid(gf[mCur].cg);
            }
        }
        printF("--implicitPC-- orderOfPredictorCorrector=%i, orderOfTimeExtrapolationForPressure=%i, predictPressure=%i\n",
                      "           numberOfExtraPressureTimeLevels=%i\n",
           	 orderOfPredictorCorrector,orderOfTimeExtrapolationForPressure,(int)predictPressure,numberOfExtraPressureTimeLevels);
    }
    fn[nab0]=0.; 
    if( numberOfPastTimeDerivatives>0 )
        fn[nab1]=0.; 
    if( parameters.dbase.get<bool >("twilightZoneFlow") )
    {
        OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
        if( orderOfAccuracy==4 )
        {
      // For the fourth-order PC method, first compute u.t(t-2*dt) and u.t(t-3*dt)
      // Even for 2nd-order in time methods -- save p and u-ghost at t-2*dt
            const int numberOfPreviousValuesOfPressureToSave= orderOfPredictorCorrector==2 ? 1 : 2;
            int grid;
            for( int m=0; m<numberOfPreviousValuesOfPressureToSave; m++ )
            {
        // *** FIX ME: 4 -> numberOfExtraFunctionsToUse
                const int nab=(nab2+m) % 4; // save du/dt in fn[nab] 
                real tp=t0-(m+2)*dt0;       // move grid to this previous time
                if( movingGridProblem() )
                {
  	// move gf[mOld] to t-(m+2)*dt
          	moveGrids( t0,t0,tp,dt0,gf[mCur],gf[mCur],gf[mOld] );   // Is this correct? dt0?   
          	gf[mOld].u.updateToMatchGrid(gf[mOld].cg); // *wdh* 040826
          // *wdh* 111125: the vertex is used below for error checking
                    fn[nab].updateToMatchGrid(gf[mOld].cg);    
          // *wdh* 090806
          	real cpu0=getCPU();
          	gf[mOld].u.getOperators()->updateToMatchGrid(gf[mOld].cg); 
          	parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateOperators"))+=getCPU()-cpu0;
                }
                gf[mOld].t=tp;
                e.assignGridFunction( gf[mOld].u,tp );
                updateStateVariables(gf[mOld]); // *wdh* 080204 
                if( parameters.useConservativeVariables() )
          	gf[mOld].primitiveToConservative();
                if( orderOfPredictorCorrector==4 ) 
                { // we only need du/dt at old times for pc4
          	for( int grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
          	{
            	  rparam[0]=gf[mOld].t;
            	  rparam[1]=gf[mOld].t; // tforce
            	  rparam[2]=gf[mCur].t-gf[mOld].t; // tImplicit  *************** check me 111124 **********************
            	  iparam[0]=grid;
            	  iparam[1]=gf[mOld].cg.refinementLevelNumber(grid);
            	  iparam[2]=numberOfStepsTaken;
            	  getUt(gf[mOld].u[grid],gf[mOld].getGridVelocity(grid),fn[nab][grid],iparam,rparam,
                		uti[grid],&gf[mOld].cg[grid]);
          	}
                }
        // save past time values of p and ghost u for the 4th order method
        // NOTE: PAST time values are saved in a funny place:
        // save p for use when extrapolating in time
        //   ua(.,.,.,pc)= p(t-2*dt) : needed for 3rd order extrapolation: uCur(t), uOld(t-dt), ua(t-2*dt)
        //   ub(.,.,.,pc)= p(t-3*dt) : needed for 4th-order extrapolation: uCur(t), uOld(t-dt), ua(t-2*dt), ub(t-3*dt)
        //   uc(.,.,.,pc)= p(t-4*dt) : needed for 5th-order extrapolation: uCur(t), uOld(t-dt), ua(t-2*dt), ub(t-3*dt), ub(t-4*dt)
                assert( nab0==0 );
                const int nabPastTime=(nab0+m);
                if( orderOfAccuracy==4 )
                {
                    const int uc = parameters.dbase.get<int >("uc");
                    const int pc = parameters.dbase.get<int >("pc");
                    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                    const int numberOfDimensions=cg.numberOfDimensions();
                    const int numberOfGhostLines=2;
                    Range V(uc,uc+numberOfDimensions-1);
                    for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
                    {
                        MappedGrid & c = gf[mOld].cg[grid];
                        realArray & fng = fn[nabPastTime][grid];
                        realArray & uOld = gf[mOld].u[grid];
                #ifdef USE_PPP
                        realSerialArray fnLocal; getLocalArrayWithGhostBoundaries(fng,fnLocal);
                        realSerialArray uOldLocal; getLocalArrayWithGhostBoundaries(uOld,uOldLocal);
                #else
                        realSerialArray & fnLocal = fng;
                        realSerialArray & uOldLocal = uOld;
                #endif
                        OV_GET_SERIAL_ARRAY_CONST(real,c.vertex(),xLocal);
                        const int isRectangular=false; // for e.gd(..)
                        const IntegerArray & gridIndexRange = c.gridIndexRange();
                        getIndex(c.dimension(),I1,I2,I3);
            // save p for use when extrapolating in time
            //    ua(.,.,.,pc)= p(t-2*dt)  (for 2nd/4th order)
            //    ub(.,.,.,pc)= p(t-3*dt)  (for 4th order)
            //    uc(.,.,.,pc)= p(t-4*dt)  (for 4th order)
                        if( parameters.dbase.get<bool >("twilightZoneFlow") )
                        {
              // *wdh* 050416 fn[nabPastTime][grid](I1,I2,I3,pc)=e(c,I1,I2,I3,pc,tp);  
              //  fn[nabPastTime][grid](I1,I2,I3,pc)=e(c,I1,I2,I3,pc,tp);
              // e.gd(fn[nabPastTime][grid],0,0,0,0,I1,I2,I3,pc,tp);
                            e.gd(fnLocal,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,pc,tp);
              //  display(fn[nabPastTime][grid],"fn[nabPastTime][grid] after assigning for fourth order",debugFile,"%5.2f ");
                            fprintf(debugFile,"savePressureAndGhostVelocity: Set p at old time for fourth-order: nabPastTime=%i, t=%9.3e\n",nabPastTime,tp);
                            if( debug() & 4 )
                            {
                      	display(xLocal,"savePressureAndGhostVelocity: xLocal from gf[mOld] ",debugFile,"%6.3f ");
                      	display(fn[nabPastTime][grid],"savePressureAndGhostVelocity: fn[nabPastTime][grid] after assigning p for fourth order",debugFile,"%6.3f ");
                            }
                        }
                        else
                        {
                            bool ok = ParallelUtility::getLocalArrayBounds(fng,fnLocal,I1,I2,I3);
                            if( ok )
                      	fnLocal(I1,I2,I3,pc)=uOldLocal(I1,I2,I3,pc); // *** fix this ****
                        }
            // We also extrapolate, in time, the ghost values of u -- used in the BC's
                        getIndex(gridIndexRange,I1,I2,I3,numberOfGhostLines);
                        for( int axis=0; axis<c.numberOfDimensions(); axis++ )
                        {
                            for( int side=0; side<=1; side++ )
                            {
                      	const int is=1-2*side;
                      	if( c.boundaryCondition(side,axis)>0 )
                      	{
        	  // set values on the two ghost lines
                        	  if( side==0 )
                          	    Iv[axis]=Range(gridIndexRange(side,axis)-2,gridIndexRange(side,axis)-1);
                        	  else
                          	    Iv[axis]=Range(gridIndexRange(side,axis)+1,gridIndexRange(side,axis)+2);
                        	  if( parameters.dbase.get<bool >("twilightZoneFlow") )
                        	  {
        	    // *wdh* 050416 fn[nabPastTime][grid](I1,I2,I3,V)=e(c,I1,I2,I3,V,tp);
        	    // fn[nabPastTime][grid](I1,I2,I3,V)=e(c,I1,I2,I3,V,tp);
        	    // display(fn[nabPastTime][grid],"fn[nabPastTime][grid] before assign V on ghost",debugFile,"%5.2f ");
        	    // e.gd(fn[nabPastTime][grid],0,0,0,0,I1,I2,I3,V,tp);
                          	    e.gd(fnLocal,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,V,tp);
        	    // display(fn[nabPastTime][grid],"fn[nabPastTime][grid] after assign V on ghost",debugFile,"%5.2f ");
                        	  }
                        	  else
                        	  {
                          	    bool ok = ParallelUtility::getLocalArrayBounds(fng,fnLocal,I1,I2,I3);
                          	    if( ok )
                            	      fnLocal(I1,I2,I3,V)=uOldLocal(I1,I2,I3,V); // ***** fix this ****
                        	  }
                      	}
                            }
              // set back to gridIndexRange to avoid re-doing corners: *** is this ok for 3D ???
                            Iv[axis]=Range(gridIndexRange(0,axis),gridIndexRange(1,axis));
                        }
                    }  // end for grid 
                }
                if( debug() & 4 )
                {
  	// determineErrors( gf[mOld].u,gf[mOld].gridVelocity, tp, 0, error,
  	// 		 sPrintF(" adams:startup: errors in u at t=%e \n",nab,tp) );
          	if( movingGridProblem() && debug() & 64 )
          	{
                        CompositeGrid & cg = *fn[nab].getCompositeGrid();
            	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            	  {
              	    if( parameters.gridIsMoving(grid) )
              	    {
                	      display(cg[grid].vertex()(I1,I2,I3,0),sPrintF("\n *** PC: AFTER moveGrids:  fn[nab] "
                                                "grid=%i vertex after move back t=%e",grid,gf[mOld].t),debugFile,"%8.5f ");
              	    }
            	  }
          	}
          	determineErrors( fn[nab],gf[mOld].gridVelocity, tp, 1, error,
                       			 sPrintF(" adams:startup: errors in ut (nab=%i) at t=%e \n",nab,tp) );
                }
            }
        }
    // get solution and derivative at t-dt
        if( movingGridProblem() )
        {
      // move gf[mOld] to t-dt
            if( debug() & 2 )
                fPrintF(debugFile,"--implicitPC-- take an initial step backwards\n");
           // display(gf[mOld].cg[0].vertex()(I1,I2,I3,0),sPrintF(" gf[mOld] vertex before move back at t=%e",gf[mOld].t),
           //                  debugFile,"%5.2f ");
            moveGrids( t0,t0,t0-dt0,dt0,gf[mCur],gf[mCur],gf[mOld] );          // this will set gf[mOld].t=t-dt
      // *wdh* 090806
            if( parameters.isAdaptiveGridProblem() )
            { // both moving and AMR 
                parameters.dbase.get<Ogen* >("gridGenerator")->updateRefinement(gf[mOld].cg);
            }
      // *wdh* 090806
            real cpu0=getCPU();
            gf[mOld].u.getOperators()->updateToMatchGrid(gf[mOld].cg); 
            parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateOperators"))+=getCPU()-cpu0;
            if( debug() & 64 )
            {
                for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
                {
                    if( parameters.gridIsMoving(grid) )
          	{
            	  display(gf[mOld].cg[grid].vertex()(I1,I2,I3,0),sPrintF("\n --implicitPC-- AFTER moveGrids:  gf[mOld] grid=%i vertex after move back t=%e",grid,gf[mOld].t),
                  		  debugFile,"%10.7f ");
          	}
                }
            }
            gf[mOld].u.updateToMatchGrid(gf[mOld].cg); // make sure the grid is correct, vertex used in TZ  *wdh* 040826
      // *wdh* 111125: the vertex is used below for error checking and computing ghost values of u
            if( numberOfPastTimeDerivatives>0 )
                fn[nab1].updateToMatchGrid(gf[mOld].cg);
        }
        else
            gf[mOld].t=t0-dt0; 
    // assign u(t-dt) with the TZ solution: 
        e.assignGridFunction( gf[mOld].u,t0-dt0 );
        updateStateVariables(gf[mOld]); // *wdh* 080204 
        if( parameters.useConservativeVariables() )
            gf[mOld].primitiveToConservative();
    // For BDF or IMEX-BDF schemes we need more past solutions
        for( int kgf=2; kgf<=numberOfPastTimes; kgf++ )
        {
         // PC and IMEX-BDF scheme grid index counts forward for past time 
                  const int mgf = (mCur + kgf + numberOfGridFunctions) % numberOfGridFunctions;
              const real tgf = t0-dt0*kgf;
              if( true )
                  printF("\n --implicitPC-- init past time solution gf[mgf=%i] at t=%9.3e numberOfGridFunctions=%i " 
                                "numberOfPastTimes=%i orderOfTimeAccuracy=%i\n",
                                mgf,tgf,numberOfGridFunctions,numberOfPastTimes,orderOfTimeAccuracy);
              if( movingGridProblem() )
              {
         // **CHECK ME: dt0*kgf ? or -dt0*kgf
         // Note: on input gf[mgf].t=0 indicates the initial grid in gf[mgf] is located at t=0
                  moveGrids( t0,t0,tgf,dt0*kgf,gf[mCur],gf[mCur],gf[mgf] );// this will set gf[mgf].t=tgf
              }
              else
              {
                  gf[mgf].t=tgf;
              }
              gf[mgf].u.updateToMatchGrid(gf[mgf].cg); 
              e.assignGridFunction( gf[mgf].u,tgf );
              updateStateVariables(gf[mgf]); 
              if( parameters.useConservativeVariables() )
                  gf[mgf].primitiveToConservative();
              if( false )
              {
                  ::display(gf[mgf].u[0],sPrintF("--implicitPC-- past time solution gf[mgf=%i].u t=%9.3e",mgf,tgf),"%6.3f ");
              }
        }
    // For IMEX-BDF schemes we need more past time-derivatives
        if( true && implicitMethod==Parameters::implicitExplicitMultistep  ) // *wdh* Feb. 3, 2017
        {
            for( int kgf=1; kgf<=numberOfPastTimeDerivatives; kgf++ )
            {
                const int mgf = (mCur + kgf + numberOfGridFunctions) % numberOfGridFunctions;
                const int ngf = (nab0 + kgf + numberOfTimeDerivativeLevels) % numberOfTimeDerivativeLevels;
                const real tgf = t0-dt0*kgf;
                gf[mgf].t=tgf;
                if( true )
          	printF("--implicitPC-- init past time du/dt at t=%9.3e (gf[mgf=%i].t=%9.3e) fn[ngf=%i]\n",
                          tgf,mgf,gf[mgf].t,ngf);
        // -- evaluate du/dt(t-dt) --
                for( int grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
                {
          	rparam[0]=gf[mgf].t;
          	rparam[1]=gf[mgf].t; // tforce
          	rparam[2]=gf[mCur].t-gf[mgf].t; // tImplicit  *************** check me 090806 **********************
          	iparam[0]=grid;
          	iparam[1]=gf[mgf].cg.refinementLevelNumber(grid);
          	iparam[2]=numberOfStepsTaken;
          	getUt(gf[mgf].u[grid],gf[mgf].getGridVelocity(grid),fn[ngf][grid],iparam,rparam,
                	      uti[grid],&gf[mgf].cg[grid]);
          	if( false )
          	{
            	  ::display(fn[ngf][grid],sPrintF("--implicitPC-- past time du/dt fn[ngf=%i] t=%9.3e",ngf,tgf),"%6.3f ");
          	}
                }
        // *wdh* *new* June 7, 2017 **CHECK ME**
        // save past time values of p and ghost u for the 4th order method
        // NOTE: PAST time values are saved in a funny place:
        // save p for use when extrapolating in time
        //    ua(.,.,.,pc)= p(t-2*dt)  (for 2nd/4th order)
        //    ub(.,.,.,pc)= p(t-3*dt)  (for 4th order)
        //    uc(.,.,.,pc)= p(t-4*dt)  (for 4th order)
        // *** savePressureAndGhostVelocity(tgf,ngf);
            }
        }
        else
        {
      // *old* 
            if( numberOfPastTimeDerivatives>0 )
            {
        // -- evaluate du/dt(t-dt) --
                for( int grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
                {
          	rparam[0]=gf[mOld].t;
          	rparam[1]=gf[mOld].t; // tforce
          	rparam[2]=gf[mCur].t-gf[mOld].t; // tImplicit  *************** check me 090806 **********************
          	iparam[0]=grid;
          	iparam[1]=gf[mOld].cg.refinementLevelNumber(grid);
          	iparam[2]=numberOfStepsTaken;
          	getUt(gf[mOld].u[grid],gf[mOld].getGridVelocity(grid),fn[nab1][grid],iparam,rparam,
                	      uti[grid],&gf[mOld].cg[grid]);
                }
            }
        }
    // display(fn[nab1][0],sPrintF("ut(t-dt) from getUt at t=%e\n",gf[mOld].t),debugFile,"%5.2f ");
        if( false ) // for testing assign du/dt(t-dt) from TZ directly
        {
            for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
            {
                MappedGrid & c = gf[mOld].cg[grid];
                getIndex(c.dimension(),I1,I2,I3);
                Range Na(0,parameters.dbase.get<int >("numberOfComponents")-1);
                fn[nab1][grid](I1,I2,I3,Na)=e.t(c,I1,I2,I3,Na,t0-dt0); 
                if( parameters.gridIsMoving(grid) )
                { // add on gDot.grad(u)
          	const realArray & gridVelocity = gf[mOld].getGridVelocity(grid);
          	const int na=parameters.dbase.get<int >("uc"), nb=na+c.numberOfDimensions()-1;   // ***** watch out ***
          	for( int n=na; n<=nb; n++ )
          	{
            	  fn[nab1][grid](I1,I2,I3,n)+=gridVelocity(I1,I2,I3,0)*e.x(c,I1,I2,I3,n,t0-dt0)+
              	    gridVelocity(I1,I2,I3,1)*e.y(c,I1,I2,I3,n,t0-dt0);
            	  if( c.numberOfDimensions()>2 )
              	    fn[nab1][grid](I1,I2,I3,n)+=gridVelocity(I1,I2,I3,2)*e.z(c,I1,I2,I3,n,t0-dt0);
          	}
                    display(fn[nab1][grid],sPrintF("implicitPC:init: ut(t-dt) grid=%i from TZ at t=%e\n",grid,gf[mOld].t),debugFile,"%5.2f ");
                }
            }
        }
        if( numberOfExtraPressureTimeLevels>0 )
        {
      // get extra time levels for extrapolating the pressure in time (as an initial guess for iterative solvers)
            for( int i=0; i<numberOfExtraPressureTimeLevels; i++ )
            {
        // if orderOfPC==2 : we need p(t-2*dt), p(t-3*dt) ...
        //             ==4 : we need p(t-5*dt), ... **check**
                const real tp = t0-dt0*(i+orderOfPredictorCorrector);
                realCompositeGridFunction & pp = previousPressure[i];
                Range all;
                for( int grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
                {
          	e.gd( pp[grid],0,0,0,0,all,all,all,parameters.dbase.get<int >("pc"),tp);
                }
            }
        }
        if( debug() & 4 || debug() & 64 )
        {
            for( int grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
            {
                aString buff;
                display(gf[mOld].u[grid],sPrintF(buff,"\n--implicitPC-- Init:gf[mOld].u grid=%i : du/dt(t) t=%9.3e",grid,gf[mOld].t),
                                debugFile,"%9.3e ");
                if( numberOfPastTimeDerivatives>0 )
                    display(fn[nab1][grid],sPrintF(buff,"\n--implicitPC-- Init:fn[nab1] grid=%i : du/dt(t) t=%9.3e",grid,gf[mOld].t),
                                  debugFile,"%9.3e ");
                if( parameters.isMovingGridProblem() )
                {
                    display(gf[mOld].getGridVelocity(grid),sPrintF("--implicitPC-- t=-dt: gridVelocity[%i] at t=%9.3e\n",grid,gf[mOld].t),debugFile,"%5.2f ");
                    display(gf[mCur].getGridVelocity(grid),sPrintF("--implicitPC-- t=0 : gridVelocity[%i] at t=%9.3e\n",grid,gf[mCur].t),debugFile,"%5.2f ");
                }
                if( debug() & 64 && parameters.isMovingGridProblem() )
                {
          	display(gf[mOld].cg[grid].vertex(),sPrintF("--implicitPC-- gf[mOld].cg[%i].vertex at t=%9.3e\n",grid,gf[mOld].t),debugFile,"%7.4f ");
          	display(gf[mCur].cg[grid].vertex(),sPrintF("--implicitPC-- gf[mCur].cg[%i].vertex at t=%9.3e\n",grid,gf[mCur].t),debugFile,"%7.4f ");
                }
            }
        }
        if( debug() & 4 )
        {
            if( parameters.isMovingGridProblem() )
            {
                determineErrors( gf[mOld].u,gf[mOld].gridVelocity, gf[mOld].t, 0, error,
                       		       sPrintF("--implicitPC-- errors in u at t=%9.3e (t0-dt0=%9.3e)\n",gf[mOld].t,t0-dt0) );
                if( numberOfPastTimeDerivatives>0 )
                {
                    fn[nab1].updateToMatchGrid(gf[mOld].cg);  // for moving grid TZ to get errors correct
                    determineErrors( fn[nab1],gf[mOld].gridVelocity, gf[mOld].t, 1, error,
                           		       sPrintF("--implicitPC-- errors in ut (fn[nab1]) at t=%9.3e (t0-dt0=%9.3e)\n",gf[mOld].t,t0-dt0) );
                }
            }
        }
    }
    else  
    {
    // **************************************************************************
    // ************************ REAL RUN ****************************************
    // ****************** Initialize for NOT twilightZoneFlow *******************
    // **************************************************************************
    // printF(" **************** implicitPC: still need correct initial values for du/dt(t-dt)  ****** \n");
    // printF(" **************** use values from du/dt(t)                                  ****** \n");
        printF("\n--implicitPC-- Initialize past time values for scheme, numberOfPastTimes=%i"
                      " numberOfPastTimeDerivatives=%i ---\n",numberOfPastTimes,numberOfPastTimeDerivatives);
        if( debug() & 2 )
            fPrintF(debugFile,"--implicitPC-- Initialize past time values for scheme, numberOfPastTimes=%i"
                            " numberOfPastTimeDerivatives=%i ---\n",numberOfPastTimes,numberOfPastTimeDerivatives);
        if( parameters.useConservativeVariables() )
            gf[mCur].primitiveToConservative();
    // if( parameters.isAdaptiveGridProblem() )
    //   gf[mOld].u.updateToMatchGrid(gf[mOld].cg);  // 040928 -- why is this needed here ?
        if( debug() & 8 )
        {
            printF(" PC: init: gf[mOld].u.numberOfGrids=%i \n",gf[mOld].u.numberOfGrids());
            printF(" PC: init: gf[mOld].cg.numberOfComponentGrids=%i \n",gf[mOld].cg.numberOfComponentGrids());
            printF(" PC: init: gf[mCur].u.numberOfGrids=%i \n",gf[mCur].u.numberOfGrids());
            printF(" PC: init: gf[mCur].cg.numberOfComponentGrids=%i \n",gf[mCur].cg.numberOfComponentGrids());
        }
        if( !parameters.dbase.get<bool>("useNewTimeSteppingStartup") )
        {
            printF(" -- implicitPC-- USE OLD STARTUP, Set past time solutions to t=0 solution \n");
            if( numberOfPastTimes==1 )
            {
        // uOld=uCur 
                assign(gf[mOld].u,gf[mCur].u); 
                gf[mOld].form=gf[mCur].form;
            }
            else
            { // June 8, 2017 *wdh*
                for( int kgf=1; kgf<=numberOfPastTimes; kgf++ )
                {
          	const int mgf = (mCur + kgf + numberOfGridFunctions) % numberOfGridFunctions;
                    assign(gf[mgf].u,gf[mCur].u); 
                    gf[mgf].t=t0-dt0*kgf;
                    gf[mgf].form=gf[mCur].form;
                }
            }
        }
        else
        {
      // *new* way to initialize past time solution  // *wdh* 2014/06/28 
            printF(" -- implicitPC-- USE NEW STARTUP numberOfPastTimes=%i mCur=%i mOld=%i\n",numberOfPastTimes,mCur,mOld);
            if( numberOfPastTimes==1 )
            {
                gf[mOld].t=t0-dt0;
                int previous[1]={mOld};  // 
                getPastTimeSolutions( mCur, numberOfPastTimes, previous  ); 
            }
            else
            {
        // For BDF schemes we need more past solutions (NOTE: this does not work for PC since previous[0]!=mOld)
                int *previous = new int[numberOfPastTimes];
                for( int kgf=1; kgf<=numberOfPastTimes; kgf++ )
                {
  	// const int mgf = (mCur - kgf + numberOfGridFunctions) % numberOfGridFunctions; // *wdh* June 7, 2017
          	const int mgf = (mCur + kgf + numberOfGridFunctions) % numberOfGridFunctions;
                    gf[mgf].t=t0-dt0*kgf;
                    gf[mgf].form=gf[mCur].form;
          	previous[kgf-1]=mgf;
                }
                getPastTimeSolutions( mCur, numberOfPastTimes, previous  );
                delete [] previous;
            }
        }
    // gf[mOld].form=gf[mCur].form;
    // For IMEX-BDF schemes we need more past time-derivatives
        if( true && implicitMethod==Parameters::implicitExplicitMultistep )
        {
      // ** THIS SECTION IS REPEATED FROM ABOVE -- *FIX ME*
            for( int kgf=1; kgf<=numberOfPastTimeDerivatives; kgf++ )
            {
                const int mgf = (mCur + kgf + numberOfGridFunctions) % numberOfGridFunctions;
                const int ngf = (nab0 + kgf + numberOfTimeDerivativeLevels) % numberOfTimeDerivativeLevels;
                const real tgf = t0-dt0*kgf;
                gf[mgf].t=tgf;
                if( true )
          	printF("--implicitPC-- init past time du/dt at t=%9.3e (gf[mgf=%i].t=%9.3e) fn[ngf=%i]\n",
                          tgf,mgf,gf[mgf].t,ngf);
        // -- evaluate du/dt(t-dt) --
                for( int grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
                {
          	rparam[0]=gf[mgf].t;
          	rparam[1]=gf[mgf].t; // tforce
          	rparam[2]=gf[mCur].t-gf[mgf].t; // tImplicit  *************** check me 090806 **********************
          	iparam[0]=grid;
          	iparam[1]=gf[mgf].cg.refinementLevelNumber(grid);
          	iparam[2]=numberOfStepsTaken;
          	getUt(gf[mgf].u[grid],gf[mgf].getGridVelocity(grid),fn[ngf][grid],iparam,rparam,
                	      uti[grid],&gf[mgf].cg[grid]);
          	if( false )
          	{
            	  ::display(fn[ngf][grid],sPrintF("--implicitPC-- past time du/dt fn[ngf=%i] t=%9.3e",ngf,tgf),"%6.3f ");
          	}
                }
            }
        }
        else
        {
      // *********** OLD WAY *******
            if( numberOfPastTimeDerivatives>0 )
            {
                if( debug() & 2 )
                    fPrintF(debugFile,"--implicitPC-- get past time du/dt at t=%9.3e...\n",gf[mOld].t);
                for( int grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
                {
                    rparam[0]=gf[mOld].t;
                    rparam[1]=gf[mOld].t; // tforce
          // *wdh* 090806 : what was this? rparam[2]=gf[mCur].t-gf[mOld].t; // tImplicit
                    rparam[2]=gf[mCur].t; // tImplicit = apply forcing for implicit time stepping at this time
                    iparam[0]=grid;
                    iparam[1]=gf[mOld].cg.refinementLevelNumber(grid);
                    iparam[2]=numberOfStepsTaken;
                    getUt(gf[mOld].u[grid],gf[mOld].getGridVelocity(grid),fn[nab1][grid],iparam,rparam,
                                uti[grid],&gf[mOld].cg[grid]);
                }
            }
            if( debug() & 4 )
            {
                determineErrors( fn[nab1],gf[mOld].gridVelocity, gf[mOld].t, 1, error,
                                                  sPrintF(" PC:init: du/dt at past time t=%e \n",gf[mOld].t) );
            }
            for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
            {
                MappedGrid & c = gf[mOld].cg[grid];
                getIndex(c.dimension(),I1,I2,I3);
        // fn[nab1][grid](I1,I2,I3,N)=fn[nab0][grid](I1,I2,I3,N);
                if( orderOfPredictorCorrector==4 )
                {
                    for( int m=0; m<=1; m++ )
                    {
                        const int nab=(mOld+m+1) % 4;
            // *** WE COULD DO BETTER HERE ***
                        assign(fn[nab][grid],fn[nab0][grid],I1,I2,I3,N);
                    }
                }
            }
      // *wdh* *new* June 7, 2017 **CHECK ME**
            if( orderOfPredictorCorrector==4 )
            {
                for( int m=0; m<=1; m++ )
                {
          // save past time values of p and ghost u for the 4th order method
          // NOTE: PAST time values are saved in a funny place:
          // save p for use when extrapolating in time
          //    ua(.,.,.,pc)= p(t-2*dt)  (for 2nd/4th order)
          //    ub(.,.,.,pc)= p(t-3*dt)  (for 4th order)
          //    uc(.,.,.,pc)= p(t-4*dt)  (for 4th order)
                    assert( nab0==0 );
                    const int nabPastTime=(nab0+m);
                    real tp=t0-(m+2)*dt0;     
                    if( orderOfAccuracy==4 )
                    {
                        const int uc = parameters.dbase.get<int >("uc");
                        const int pc = parameters.dbase.get<int >("pc");
                        OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                        const int numberOfDimensions=cg.numberOfDimensions();
                        const int numberOfGhostLines=2;
                        Range V(uc,uc+numberOfDimensions-1);
                        for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
                        {
                            MappedGrid & c = gf[mOld].cg[grid];
                            realArray & fng = fn[nabPastTime][grid];
                            realArray & uOld = gf[mOld].u[grid];
                    #ifdef USE_PPP
                            realSerialArray fnLocal; getLocalArrayWithGhostBoundaries(fng,fnLocal);
                            realSerialArray uOldLocal; getLocalArrayWithGhostBoundaries(uOld,uOldLocal);
                    #else
                            realSerialArray & fnLocal = fng;
                            realSerialArray & uOldLocal = uOld;
                    #endif
                            OV_GET_SERIAL_ARRAY_CONST(real,c.vertex(),xLocal);
                            const int isRectangular=false; // for e.gd(..)
                            const IntegerArray & gridIndexRange = c.gridIndexRange();
                            getIndex(c.dimension(),I1,I2,I3);
              // save p for use when extrapolating in time
              //    ua(.,.,.,pc)= p(t-2*dt)  (for 2nd/4th order)
              //    ub(.,.,.,pc)= p(t-3*dt)  (for 4th order)
              //    uc(.,.,.,pc)= p(t-4*dt)  (for 4th order)
                            if( parameters.dbase.get<bool >("twilightZoneFlow") )
                            {
                // *wdh* 050416 fn[nabPastTime][grid](I1,I2,I3,pc)=e(c,I1,I2,I3,pc,tp);  
                //  fn[nabPastTime][grid](I1,I2,I3,pc)=e(c,I1,I2,I3,pc,tp);
                // e.gd(fn[nabPastTime][grid],0,0,0,0,I1,I2,I3,pc,tp);
                                e.gd(fnLocal,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,pc,tp);
                //  display(fn[nabPastTime][grid],"fn[nabPastTime][grid] after assigning for fourth order",debugFile,"%5.2f ");
                                fprintf(debugFile,"savePressureAndGhostVelocity: Set p at old time for fourth-order: nabPastTime=%i, t=%9.3e\n",nabPastTime,tp);
                                if( debug() & 4 )
                                {
                          	display(xLocal,"savePressureAndGhostVelocity: xLocal from gf[mOld] ",debugFile,"%6.3f ");
                          	display(fn[nabPastTime][grid],"savePressureAndGhostVelocity: fn[nabPastTime][grid] after assigning p for fourth order",debugFile,"%6.3f ");
                                }
                            }
                            else
                            {
                                bool ok = ParallelUtility::getLocalArrayBounds(fng,fnLocal,I1,I2,I3);
                                if( ok )
                          	fnLocal(I1,I2,I3,pc)=uOldLocal(I1,I2,I3,pc); // *** fix this ****
                            }
              // We also extrapolate, in time, the ghost values of u -- used in the BC's
                            getIndex(gridIndexRange,I1,I2,I3,numberOfGhostLines);
                            for( int axis=0; axis<c.numberOfDimensions(); axis++ )
                            {
                                for( int side=0; side<=1; side++ )
                                {
                          	const int is=1-2*side;
                          	if( c.boundaryCondition(side,axis)>0 )
                          	{
          	  // set values on the two ghost lines
                            	  if( side==0 )
                              	    Iv[axis]=Range(gridIndexRange(side,axis)-2,gridIndexRange(side,axis)-1);
                            	  else
                              	    Iv[axis]=Range(gridIndexRange(side,axis)+1,gridIndexRange(side,axis)+2);
                            	  if( parameters.dbase.get<bool >("twilightZoneFlow") )
                            	  {
          	    // *wdh* 050416 fn[nabPastTime][grid](I1,I2,I3,V)=e(c,I1,I2,I3,V,tp);
          	    // fn[nabPastTime][grid](I1,I2,I3,V)=e(c,I1,I2,I3,V,tp);
          	    // display(fn[nabPastTime][grid],"fn[nabPastTime][grid] before assign V on ghost",debugFile,"%5.2f ");
          	    // e.gd(fn[nabPastTime][grid],0,0,0,0,I1,I2,I3,V,tp);
                              	    e.gd(fnLocal,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,V,tp);
          	    // display(fn[nabPastTime][grid],"fn[nabPastTime][grid] after assign V on ghost",debugFile,"%5.2f ");
                            	  }
                            	  else
                            	  {
                              	    bool ok = ParallelUtility::getLocalArrayBounds(fng,fnLocal,I1,I2,I3);
                              	    if( ok )
                                	      fnLocal(I1,I2,I3,V)=uOldLocal(I1,I2,I3,V); // ***** fix this ****
                            	  }
                          	}
                                }
                // set back to gridIndexRange to avoid re-doing corners: *** is this ok for 3D ???
                                Iv[axis]=Range(gridIndexRange(0,axis),gridIndexRange(1,axis));
                            }
                        }  // end for grid 
                    }
                }
            }
        } // end OLD WAY
    }
    dtb=dt0;    // delta t to go from ub to ua
    dtp[0]=dt0;
    dtp[1]=dt0;
    dtp[2]=dt0;
    dtp[3]=dt0;
    dtp[4]=dt0;
  //       if( debug() & 8 )
  //       {
  //         fprintf(debugFile," advance Adams PC: ut at t0=%e\n",t0);
  //         outputSolution( fn[nab0],t0 );
  //         fprintf(debugFile," advance Adams PC: Errors in ut at t0=%e\n",t0);
  //         determineErrors( fn[1],t0-dt0 );
  //       }
    init=false;

    return 0;
}


// ===================================================================================================================
/// \brief Start an individual time step (a time sub-step function).
/// \details 
/// \param t0 (input) : current time
/// \param dt0 (input) : current time step
/// \param correction (input) : for predictor corrector methods this indicates the correction step number.
/// \param currentGF (output) : points to the grid-function holding the current solution (time t0)
/// \param nextGF (output) : points to the grid-function holding the new solution (time t0+dt0)
/// \param advanceOptions.numberOfCorrectorSteps (output) : return the number of corrector steps that will be used.
///
// ===================================================================================================================
int DomainSolver::
startTimeStepIM( real & t0, real & dt0, int & currentGF, int & nextGF, AdvanceOptions & advanceOptions )
{

    if( parameters.dbase.get<int >("globalStepNumber")<0 )
        parameters.dbase.get<int >("globalStepNumber")=0;
    parameters.dbase.get<int >("globalStepNumber")++;

    assert( parameters.dbase.get<DataBase >("modelData").has_key("AdamsPCData") );
    AdamsPCData & adamsData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData");
    int &mab0 =adamsData.mab0, &mab1=adamsData.mab1, &mab2=adamsData.mab2;

    currentGF=mab0;
    nextGF=mab2;

    advanceOptions.numberOfCorrectorSteps=parameters.dbase.get<int>("numberOfPCcorrections"); 
    advanceOptions.gridChanges=AdvanceOptions::noChangeToGrid;  // fix me for AMR

    return 0;

}

// ===================================================================================================================
/// \brief Take a single time step (a time sub-step function).
/// \details 
/// \param t0 (input) : current time
/// \param dt0 (input) : current time step
/// \param correction (input) : for predictor corrector methods this indicates the correction step number.
/// \param advanceOptions (input) : additional options that adjust the behaviour of this function.
///       advanceOptions.takeTimeStepOption can be used to not apply or only apply the boundary conditions.
///   advanceOptions.correctionIterationsHaveConverged (output) is set to true if the correction iterations
///   have converged (e.g. for moving grids)
///
// ===================================================================================================================
int DomainSolver::
takeTimeStepIM( real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions )
{
    FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

    const bool takeTimeStep =(advanceOptions.takeTimeStepOption==AdvanceOptions::takeStepAndApplyBoundaryConditions ||
                      			    advanceOptions.takeTimeStepOption==AdvanceOptions::takeStepButDoNotApplyBoundaryConditions);
    const bool applyBC = ( advanceOptions.takeTimeStepOption==AdvanceOptions::takeStepAndApplyBoundaryConditions ||
                   			 advanceOptions.takeTimeStepOption==AdvanceOptions::applyBoundaryConditionsOnly );

    if( debug() & 2 )
        printP("\n +++++++++++++++ START takeTimeStepIM t0=%8.2e, dt0=%8.2e, correction=%i takeTimeStep=%i applyBC=%i"
                      "  +++++++++++++\n",t0,dt0,correction,(int)takeTimeStep,(int)applyBC );
    if( debug() & 2 )
    {
        fprintf(debugFile," *** DomainSolver::takeTimeStepIM (start): t0=%e, dt0=%e, correction=%i*** \n",t0,dt0,correction);
    }

    advanceOptions.correctionIterationsHaveConverged=false; // this may be set to true below

    assert( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit );
    parameters.dbase.get<real >("dt")=dt0; // *wdh* 101106 this is the dt used in getUt (cns)

    const int & numberOfSolutionLevels = parameters.dbase.get<int>("numberOfSolutionLevels");
    const int & numberOfTimeDerivativeLevels = parameters.dbase.get<int>("numberOfTimeDerivativeLevels");

    assert( parameters.dbase.get<int >("orderOfPredictorCorrector")==2 );  // for now we just have 2nd-order in time

    assert( parameters.dbase.get<DataBase >("modelData").has_key("AdamsPCData") );
    AdamsPCData & adamsData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData");
    
    real & dtb=adamsData.dtb;
    int &mab0 =adamsData.mab0, &mab1=adamsData.mab1, &mab2=adamsData.mab2;
    int &nab0 =adamsData.nab0, &nab1=adamsData.nab1, &nab2=adamsData.nab2, &nab3=adamsData.nab3;
    int &ndt0=adamsData.ndt0;
    real *dtp = adamsData.dtp;

    const int & predictorOrder = parameters.dbase.get<int>("predictorOrder");
    
    int numberOfCorrections=parameters.dbase.get<int>("numberOfPCcorrections"); 

  // If we check a convergence tolerance when correcting (e.g. for moving grids) then this is
  // the minimum number of corrector steps we must take:
    const int minimumNumberOfPCcorrections = parameters.dbase.get<int>("minimumNumberOfPCcorrections"); 
    
  // For moving grids we keep gf[mab0], gf[mab1] and gf[mab2]
  // For non-moving grids we keep gf[mab0], gf[mab1] and we set mab2==mab1

    const int numberOfGridFunctions =  movingGridProblem() ? 3 : 2; 

// **   mab2 = (mab0 -1 + numberOfGridFunctions) % numberOfGridFunctions;

    int & mNew = mab2;    // new     : gf[mNew] : will hold u(t+dt)
    int & mCur = mab0;    // current : gf[mCur] : holds u(t) 
    int & mOld = mab1;    // old     : gf[mOld] : holds u(t-dt) if numberOfGridFunctions==3 otherwise mOld=mNew
    

    Parameters::ImplicitOption & implicitOption = parameters.dbase.get<Parameters::ImplicitOption >("implicitOption");
    
    implicitOption=Parameters::doNotComputeImplicitTerms; // no need to compute during initialization
    parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::crankNicolson;
    int iparam[10];
    real rparam[10];

    RealCompositeGridFunction & uti = fn[2];   // ****
    
    
    int grid;
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2]; 
    Range N = parameters.dbase.get<Range >("Rt");   // time dependent variables
    RealArray error(numberOfComponents()+3); 
    
//  int numberOfExtraPressureTimeLevels=0;
    
    if( correction==0 )
    {
    // if( dtb!=dt0 )
        if( fabs(dtb-dt0) > dt0*REAL_EPSILON*100. )
        {
            if( debug() & 2 ) 
                printP("advanceImplicitMultiStep dtb!=dt0 : re-initialize (dtb=%9.3e, dt0=%9.3e, diff=%8.2e)\n",
                              dtb,dt0,fabs(dtb-dt0));
            parameters.dbase.get<int >("initializeImplicitTimeStepping")=true;
        }
        else
        {
            if( debug() & 2 )
      	printP("advanceImplicitMultiStep dtb==dt0 : do not re-initialize\n");
        }
    }
        
    realCompositeGridFunction & ua = fn[nab0];   // pointer to du/dt
    realCompositeGridFunction & ub = fn[nab1];   // pointer to du(t-dt)/dt or du/dt(t+dt)
        
    real ab1,ab2;
    if( predictorOrder==1 )
    { // first order predictor
        ab1=dt0;
        ab2=0.;
    }
    else 
    { // 2nd -order predictor
        ab1= dt0*(1.+dt0/(2.*dtb));  // for AB predictor
        ab2= -dt0*dt0/(2.*dtb);
    }

    const real am1=.5*dt0;                  // for AM corrector
    const real am2=.5*dt0;


    if( correction>1  && debug() & 2 )
        printP("takeTimeStepIM: correction=%i\n",correction);
            
    if( correction >0 )
        parameters.dbase.get<int>("totalNumberOfPCcorrections")++;  // count the total number of corrections.

  // Predictor-Corrector. First time predict, subsequent times correct.
  //
  //  correction==0 :
  //       ---Adams-Bashforth Predictor
  //           u(*) <- u(t) + ab1*du/dt +ab2*du(t-dtb)/dt
  //  i.e.     gf[1]<- gf[mCur]+ ab1*ua    +ab2*ub
  //      
  //            The constants ab1 and ab2 are
  //                 ab1 = dt*( 1.+dt/(2*dtb) )   = (3/2)*dt if dtb=dt
  //                 ab2 = -dt*(  dt/(2*dtb) )    =-(1/2)*dt if dtb=dt
  //            Determined by extrapolation to time t+dt/2 from the
  //            times of ua and ub
  //
  // correction>0 :
  //       ---Adams Moulton Corrector
  //          u(t+dt) <- u(t) + dt* ( (1/2) du(*)/dt + (1/2) du(t)/dt )
  //          gf[mNew]  gf[mCur]              ub               ua

  // We only need to compute the "explicit" part of the implicit terms once for correction==0: 
  // These values are stored in utImplicit 
    implicitOption =correction==0 ? Parameters::computeImplicitTermsSeparately : Parameters::doNotComputeImplicitTerms;

  // ------------------------------------------------------------------------------------------
  // --------------------------- UPDATE INTERIOR POINTS ---------------------------------------
  // ------------------------------------------------------------------------------------------
    if( takeTimeStep ) 
    {
        if( debug() & 2 )
            printP("++++++++++++ takeTimeStep: update interior points t0+dt0=%9.3e\n\n",t0+dt0);

        const int maba = correction==0 ? mCur : mNew;
        const int naba = correction==0 ? nab0 : nab1;

        if( correction==0 )
        {
      // ------------------------------------------------------
      // ----------------- Moving Grids -----------------------
      // ------------------------------------------------------

            bool gridWasAdapted=false;

      // moveTheGridsMacro(adamsPC,uti); // *wdh* 090804 

            real tb=gf[mCur].t-dtb, tc=tb-dtb, td=tc-dtb; // tc,td not used
            assert( predictorOrder<=2 );
            const int numberOfPastTimes=0;
            const int numberOfPastTimeDerivatives=predictorOrder-1; 
            if( movingGridProblem() )
            {
                checkArrays(" adamsPC : before move grids"); 
                if( debug() & 8 )
                    printF(" adamsPC: before moveTheGridsMacro: t0=%9.3e, gf[mNew].t=%9.3e, gf[mNew].gridVelocityTime=%9.3e\n",
                     	   t0,gf[mNew].t,gf[mNew].gridVelocityTime);
                if( debug() & 4 )
                    fPrintF(debugFile," adamsPC: before moveTheGridsMacro: t0=%9.3e, gf[mNew].t=%9.3e, gf[mNew].gridVelocityTime=%9.3e\n",
                     	   t0,gf[mNew].t,gf[mNew].gridVelocityTime);
        // generate gf[mNew] from gf[mCur] (compute grid velocity on gf[mCur] and gf[mNew]
                moveGrids( t0,t0,t0+dt0,dt0,gf[mCur],gf[mCur],gf[mNew] ); 
                checkArrayIDs(sPrintF(" adamsPC : after move grids t=%9.3e",gf[mCur].t));
                if( parameters.isAdaptiveGridProblem() )
                {
          // both moving and AMR 
                    parameters.dbase.get<Ogen* >("gridGenerator")->updateRefinement(gf[mNew].cg);
                }
                if( debug() & 16 )
                {
                    if( twilightZoneFlow() )
                    {
                        fprintf(debugFile,"\n ---> adamsPC : Errors in u after moveGrids t=%e  \n",gf[mCur].t);
                        determineErrors( gf[mCur] );
                    }
                }
                real cpu0=getCPU();
                gf[mNew].cg.rcData->interpolant->updateToMatchGrid( gf[mNew].cg );  
                parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateInterpolant"))+=getCPU()-cpu0;
                cpu0=getCPU();
                gf[mNew].u.getOperators()->updateToMatchGrid(gf[mNew].cg); 
                parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateOperators"))+=getCPU()-cpu0;
                if( debug() & 4 ) printf("adamsPC : step: update gf[mNew] for moving grids, gf[mNew].t=%9.3e,...\n",gf[mNew].t);
                if( debug() & 16 )
                {
                    if( twilightZoneFlow() )
                    {
                        fprintf(debugFile,"\n ---> adamsPC: Errors in u before updateForMovingGrids t=%e  \n",gf[mCur].t);
                        fprintf(debugFile,"*** mCur=%i mNew=%i numberOfGridFunctions=%i *** \n",
                        	      mCur,mNew,numberOfGridFunctions);
                        determineErrors( gf[mCur] );
                    }
                }
                updateForMovingGrids(gf[mNew]);
        // ****      gf[mNew].u.updateToMatchGrid( gf[mNew].cg );  
                checkArrayIDs(sPrintF(" adamsPC : after updateForMovingGrids t=%9.3e",gf[mCur].t));
                if( debug() & 16 )
                {
                    if( twilightZoneFlow() )
                    {
                        fprintf(debugFile,"\n ---> adamsPC: Errors in u after updateForMovingGrids t=%e  \n",gf[mCur].t);
                        determineErrors( gf[mCur] );
                    }
                }
        // get values for exposed points on gf[mCur]
                if( parameters.useConservativeVariables() )
                    gf[mCur].primitiveToConservative();  // *wdh* 010318
                cpu0=getCPU();
                if( useNewExposedPoints && parameters.dbase.get<int>("simulateGridMotion")==0 )
                {
                    if( Parameters::checkForFloatingPointErrors )
                        checkSolution(gf[mCur].u,"Before interp exposed points",true);
                    if( debug() & 16 )
                    {
                        if( twilightZoneFlow() )
                        {
                  	fprintf(debugFile,"\n ---> adamsPC: Errors in u BEFORE interp exposed t=%e  \n",gf[mCur].t);
                  	determineErrors( gf[mCur] );
                        }
                    }
                    ExposedPoints exposedPoints;
                    exposedPoints.setAssumeInterpolationNeighboursAreAssigned(parameters.dbase.get<int >("extrapolateInterpolationNeighbours"));
                    exposedPoints.initialize(gf[mCur].cg,gf[mNew].cg,parameters.dbase.get<int >("stencilWidthForExposedPoints"));
                    exposedPoints.interpolate(gf[mCur].u,(twilightZoneFlow() ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),t0);
          // Added for BDF: *wdh* 2015/04/05
                    for( int kp=1; kp<=numberOfPastTimes; kp++ )
                    {
                        const int mPast = (mCur -kp + numberOfGridFunctions) % numberOfGridFunctions; 
                        exposedPoints.initialize(gf[mPast].cg,gf[mNew].cg,parameters.dbase.get<int >("stencilWidthForExposedPoints"));
                        exposedPoints.interpolate(gf[mPast].u,(twilightZoneFlow() ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),t0);
                    }
                    if( debug() & 16 )
                    {
                        if( twilightZoneFlow() )
                        {
                  	fprintf(debugFile,"\n ---> adamsPC: Errors in u AFTER interp exposed t=%e  \n",gf[mCur].t);
                  	determineErrors( gf[mCur] );
                        }
                    }
                    if( predictorOrder>=2  )
                    {
            // -------------------------
            // --- fixup du/dt(t-dt) ---
            // -------------------------
            // NOTE: we CANNOT directly interpolate points on du/dt since for moving grids
            // du/dt includes the -gDot.grad(u) term which differs from grid to grid 
            // Current procedure: 
            //   1. Interpolate exposed points on u(t-dt)
            //   2. Recompute du/dt(t-dt) 
            // Optimizations: 
            //   - only recompute du/dt(t-dt) on grids with exposed points
            //   - could only compute du/dt(t-dt) on those points where is not already known.
                        if( gf[mCur].t<=0. || debug() & 4 )
                        {
                            printF(" --- INFO: Fixup exposed points of u(t-dt) and recompute du/dt(t-dt) t=%9.3e, tb=%9.3e ----- \n"
                                          "     The extra work involved in recomputing du/dt(t-dt) can be avoided by using "
                                          "the option 'first order predictor'.\n",gf[mCur].t,tb);
                        }
                        ExposedPoints exposedPoints;
            //            
            // exposedPoints.setExposedPointType(ExposedPoints::exposedDiscretization);
                        const int extrapolateInterpolationNeighbours=parameters.dbase.get<int >("extrapolateInterpolationNeighbours");
                        const int stencilWidthForExposedPoints=parameters.dbase.get<int >("stencilWidthForExposedPoints");
                        if( debug() & 4 )
                        {
                            fPrintF(debugFile," ---- compute exposed for du/dt(t-dt), extrapolateInterpolationNeighbours=%i, "
                                            "stencilWidthForExposedPoints=%i\n",extrapolateInterpolationNeighbours,stencilWidthForExposedPoints);
                        }
                        exposedPoints.setAssumeInterpolationNeighboursAreAssigned(extrapolateInterpolationNeighbours);
                        exposedPoints.initialize(gf[mOld].cg,gf[mNew].cg,stencilWidthForExposedPoints);
                        exposedPoints.interpolate(gf[mOld].u,(twilightZoneFlow() ? 
                                                                            parameters.dbase.get<OGFunction* >("exactSolution") : NULL),gf[mOld].t);
            // For now recompute du/dt(t-dt) using the mask values from cg(t+dt)
                        for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
                        {
                  	if( gridWasAdapted || exposedPoints.getNumberOfExposedPoints(grid)>0 )
                  	{
                                if( debug() & 2 )
                                {
                      	    printF(" ---- adamsPC: recompute du/dt(t-dt) for grid=%i t-dt = %9.3e  (%i exposed)-----\n",grid,gf[mOld].t,
                           		   exposedPoints.getNumberOfExposedPoints(grid));
                                    fPrintF(debugFile," ---- adamsPC: recompute du/dt(t-dt) for grid=%i t-dt = %9.3e  (%i exposed)-----\n",
                                                  grid,gf[mOld].t,exposedPoints.getNumberOfExposedPoints(grid));
                                }
      	  // This is only necesssary if there are exposed points on this grid
                    	  rparam[0]=gf[mOld].t;
                    	  rparam[1]=gf[mOld].t;
                    	  rparam[2]=gf[mCur].t; // tImplicit
                    	  iparam[0]=grid;
                    	  iparam[1]=gf[mOld].cg.refinementLevelNumber(grid);
                    	  iparam[2]=numberOfStepsTaken;
                    	  getUt(gf[mOld].u[grid],gf[mOld].getGridVelocity(grid),ub[grid],iparam,rparam,
                        		uti[grid],&gf[mNew].cg[grid]);
                  	}
                            else
                            {
                                if( debug() & 2 )
                                {
                      	    printF(" ---- adamsPC: fixp du/dt(t-dt) for grid=%i t-dt = %9.3e  (%i exposed) ...ok -----\n",
                                                  grid,gf[mOld].t,exposedPoints.getNumberOfExposedPoints(grid));
                                    fPrintF(debugFile," ---- adamsPC: fixp du/dt(t-dt) for grid=%i t-dt = %9.3e  (%i exposed) ...ok -----\n",
                                                  grid,gf[mOld].t,exposedPoints.getNumberOfExposedPoints(grid));
                                }
                            }
                        }
                        if( debug() & 4 )
                        {	
                  	if( twilightZoneFlow() )
                  	{
                    	  fprintf(debugFile," ***adamsPC: gf[mOld] after interp exposed, gf[mOld].t=%e",gf[mOld].t);
                    	  for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
                    	  {
                      	    display(gf[mOld].u[grid],sPrintF("\n ****gf[mOld].u[grid=%i]",grid),debugFile,"%7.1e ");
                    	  }
                    	  determineErrors( gf[mOld] );
                    	  fprintf(debugFile," ***adamsPC: du/dt(t-dt)  after interp exposed, gf[mOld].t=%e",gf[mOld].t);
                    	  for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
                    	  {
                      	    display(ub[grid],sPrintF("\n ****ub[grid=%i]: du/dt(t-dt)",grid),debugFile,"%7.1e ");
                    	  }
                    	  determineErrors( ub,gf[mOld].gridVelocity, gf[mOld].t, 1, error );
                  	}
                        }
                        if( predictorOrder>=3 )
                        {
                            OV_ABORT("adamsPC: moveTheGridsMacro:Error: finish me for predictorOrder>=3");
                        }
                    } // end if predictorOrder>=2
                    if( Parameters::checkForFloatingPointErrors )
                        checkSolution(gf[mCur].u,"adamsPC: After interp exposed points",true);
                }
                else if( parameters.dbase.get<int>("simulateGridMotion")==0  )
                {
          // *old way* 
                    interpolateExposedPoints(gf[mCur].cg,gf[mNew].cg,gf[mCur].u, 
                                   			     (twilightZoneFlow() ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),t0,
                                   			     false,Overture::nullIntArray(),Overture::nullIntegerDistributedArray(),
                                   			     parameters.dbase.get<int >("stencilWidthForExposedPoints") ); 
                }
                if( twilightZoneFlow() && false ) // **** wdh **** 
                {
          // for testing ***
                    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                    int grid=0; 
                    MappedGrid & c = gf[mNew].cg[grid];
                    getIndex(c.dimension(),I1,I2,I3);
                    ub[grid](I1,I2,I3,N)=e.t(c,I1,I2,I3,N,t0-dt0); 
                }
                parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInterpolateExposedPoints"))+=getCPU()-cpu0;
        // compute dudt now -- after exposed points have been computed!
                checkArrayIDs(sPrintF(" adamsPC : after moving grids update t=%9.3e",gf[mCur].t));
                if( debug() & 16 )
                {
                    if( twilightZoneFlow() )
                    {
                        fprintf(debugFile,"\n ---> adamsPC: Errors in u after move grids t=%e  \n",gf[mCur].t);
                        determineErrors( gf[mCur] );
                    }
                }
                if( debug() & 16 )
                    printf(" adamsPC: AFTER moveTheGridsMacro: t0=%9.3e, gf[mNew].t=%9.3e, gf[mNew].gridVelocityTime=%9.3e\n",
                     	   t0,gf[mNew].t,gf[mNew].gridVelocityTime);
            }

      // we need to rebuild the implicit time stepping matrix.
            if( movingGridProblem() )
                parameters.dbase.get<int >("initializeImplicitTimeStepping")=true;
        }

    // Optionally refactor the matrix : if parameters.dbase.get<int >("globalStepNumber") % refactorFrequency == 0 
    // (We need to do this after the grids have moved but before dudt is evaluated (for nonlinear problems)
        if( correction==0 && (parameters.dbase.get<int >("initializeImplicitTimeStepping") || parameters.dbase.get<int >("globalStepNumber")>0) )
            formMatrixForImplicitSolve(dt0,gf[mNew], gf[mCur] );

    // -- evaluate any body forcing (this is saved in realCompositeGridFunction bodyForce found in the data-base) ---
        const real tForce = gf[maba].t; // evaluate the body force at this time  ***CHECK ME**
        computeBodyForcing( gf[maba], tForce );

    // fn[nab0] <- du/dt(t0)  or fn[nab1] <- du/dt(t+dt0)
        for( grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
        {
            rparam[0]=gf[maba].t;
            rparam[1]=gf[maba].t; // tforce
            rparam[2]=gf[maba].t+dt0; // tImplicit
            iparam[0]=grid;
            iparam[1]=gf[maba].cg.refinementLevelNumber(grid);
            iparam[2]=numberOfStepsTaken;
            getUt(gf[maba].u[grid],gf[maba].getGridVelocity(grid),
                        fn[naba][grid],iparam,rparam,uti[grid],&gf[mNew].cg[grid]);
        }

        addArtificialDissipation(gf[maba].u,dt0);	// add "implicit" dissipation to u 

        if( debug() & 16 || debug() & 64 )
        {
            for( grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
            {
                display(ua[grid],"\n ****ua: du/dt(t)",debugFile);
                display(ub[grid],"\n ****ub: du/dt(t-dt)",debugFile);
            }
        }
        if( debug() & 16 )
        {
            aString label = sPrintF(" ImplicitMS: errors in ut (ua) at t=%e, correction=%i \n",t0,correction);
            determineErrors( ua,gf[mCur].gridVelocity, t0, 1, error,label );
            real tub = correction==0 ? t0-dtb : t0+dt0;
            label = sPrintF(" ImplicitMS: errors in ut (ub) at t=%e, correction=%i \n",tub,correction);
            determineErrors( ub,gf[mNew].gridVelocity, tub, 1, error,label );
        }

    //  --------------------------------------------------------
    //  --- Assign the explicit or implicit time-step update ---
    //  --------------------------------------------------------
        for( grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
        {
      // const real dti = dt0; 
            const real dti = (1.-parameters.dbase.get<real >("implicitFactor"))*dt0;

            if( correction==0 )
            { // u1 <- u0 + ab1*ua0 + ab2*ub0 [ + dti*utImplicit ]  (add last terms for implicit grids)
                #ifdef USE_PPP
                    RealArray u1;  getLocalArrayWithGhostBoundaries(gf[mNew].u[grid],u1);
                    RealArray u0;  getLocalArrayWithGhostBoundaries(gf[mCur].u[grid],u0);
                    RealArray ua0; getLocalArrayWithGhostBoundaries(ua[grid],ua0);
                    RealArray ub0; getLocalArrayWithGhostBoundaries(ub[grid],ub0);
                    RealArray utImplicit; getLocalArrayWithGhostBoundaries(uti[grid],utImplicit);
                    const intSerialArray & mask1 = gf[mNew].cg[grid].mask().getLocalArray();
                #else
                    RealDistributedArray & u1 = gf[mNew].u[grid];
                    RealDistributedArray & u0 = gf[mCur].u[grid];
                    RealDistributedArray & ua0 = ua[grid];
                    RealDistributedArray & ub0 = ub[grid];
                    RealDistributedArray & utImplicit = uti[grid];
                    const intSerialArray & mask1 = gf[mNew].cg[grid].mask(); 
                #endif
                    getIndex(gf[mNew].cg[grid].extendedIndexRange(),I1,I2,I3);
                    int n1a,n1b,n2a,n2b,n3a,n3b;
                    bool ok = ParallelUtility::getLocalArrayBounds(gf[mCur].u[grid],u0,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b);
                    if( !ok ) continue;
          // const intArray & mask1 = gf[mNew].cg[grid].mask();
                    int ierr=0;
                    const int maskOption=0; // assign pts where mask>0
                    int ipar[]={0,maskOption,n1a,n1b,n2a,n2b,n3a,n3b,N.getBase(),N.getBound()}; //
                    real rpar[5]={0.,0.,0.,0.,0.};
                    real *ut1p, *ut2p, *ut3p, *ut4p;
                    if( parameters.getGridIsImplicit(grid) )
                        ipar[0]=3;  // add three extra "ut" terms if grid is advanced implicitly
                    else    
                        ipar[0]=2;  // add two extra "ut" terms
                    rpar[0]=ab1; rpar[1]=ab2; rpar[2]=dti;
                    ut1p=ua0.getDataPointer();
                    ut2p=ub0.getDataPointer();
                    ut3p=utImplicit.getDataPointer();
                    ut4p=ut3p;
                    updateOpt(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
                          	    u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
                          	    *mask1.getDataPointer(),  
                          	    *u0.getDataPointer(),*u1.getDataPointer(), 
                          	    *ut1p, *ut2p, *ut3p, *ut4p, 
                          	    ipar[0], rpar[0], ierr );
            }
            else
            { // u1 <- u0 + am1*ub0 + am2*ua0 [ + dti*utImplicit ] (add last terms for implicit grids)
                #ifdef USE_PPP
                    RealArray u1;  getLocalArrayWithGhostBoundaries(gf[mNew].u[grid],u1);
                    RealArray u0;  getLocalArrayWithGhostBoundaries(gf[mCur].u[grid],u0);
                    RealArray ua0; getLocalArrayWithGhostBoundaries(ua[grid],ua0);
                    RealArray ub0; getLocalArrayWithGhostBoundaries(ub[grid],ub0);
                    RealArray utImplicit; getLocalArrayWithGhostBoundaries(uti[grid],utImplicit);
                    const intSerialArray & mask1 = gf[mNew].cg[grid].mask().getLocalArray();
                #else
                    RealDistributedArray & u1 = gf[mNew].u[grid];
                    RealDistributedArray & u0 = gf[mCur].u[grid];
                    RealDistributedArray & ua0 = ua[grid];
                    RealDistributedArray & ub0 = ub[grid];
                    RealDistributedArray & utImplicit = uti[grid];
                    const intSerialArray & mask1 = gf[mNew].cg[grid].mask(); 
                #endif
                    getIndex(gf[mNew].cg[grid].extendedIndexRange(),I1,I2,I3);
                    int n1a,n1b,n2a,n2b,n3a,n3b;
                    bool ok = ParallelUtility::getLocalArrayBounds(gf[mCur].u[grid],u0,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b);
                    if( !ok ) continue;
          // const intArray & mask1 = gf[mNew].cg[grid].mask();
                    int ierr=0;
                    const int maskOption=0; // assign pts where mask>0
                    int ipar[]={0,maskOption,n1a,n1b,n2a,n2b,n3a,n3b,N.getBase(),N.getBound()}; //
                    real rpar[5]={0.,0.,0.,0.,0.};
                    real *ut1p, *ut2p, *ut3p, *ut4p;
                    if( parameters.getGridIsImplicit(grid) )
                        ipar[0]=3;  // add three extra "ut" terms if grid is advanced implicitly
                    else    
                        ipar[0]=2;  // add two extra "ut" terms
                    rpar[0]=am1; rpar[1]=am2; rpar[2]=dti;
                    ut1p=ub0.getDataPointer();
                    ut2p=ua0.getDataPointer();
                    ut3p=utImplicit.getDataPointer();
                    ut4p=ut3p;
                    updateOpt(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
                          	    u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
                          	    *mask1.getDataPointer(),  
                          	    *u0.getDataPointer(),*u1.getDataPointer(), 
                          	    *ut1p, *ut2p, *ut3p, *ut4p, 
                          	    ipar[0], rpar[0], ierr );
            }
        }

        if( correction==0 )
        {
      // printF(" +++ ims: gf[mNew].t=%9.3e --> change to t0+dt0=%9.3e +++\n",gf[mNew].t,t0+dt0);
            gf[mNew].t=t0+dt0;  // gf[mNew] now lives at this time
        }
            

    // *** assign boundary conditions for the implicit method 
        applyBoundaryConditionsForImplicitTimeStepping( gf[mNew] ); // ***** gf[mNew].gridVelocity must be correct here
        
        if( Parameters::checkForFloatingPointErrors!=0 )
            checkSolution(gf[mNew].u,"advanceStepsIM: after applyBCIMP",true);


        if( debug() & 4 )
        {
            aString label = sPrintF(" ***ImplicitMS: RHS Before implicitSolve t=%e, ,correction=%i\n",gf[mNew].t,correction);
            if( twilightZoneFlow() )
            {
                gf[mNew].u.display(label,debugFile,"%8.5f ");
            }
            label = sPrintF(" ***ImplicitMS: Errors in rhs gf before implicitSolve t=%e, correction=%i\n",gf[mNew].t,correction);
            determineErrors( gf[mNew],label );
        }

    // **** fix this *** we could refactor for each correction here !
    //       if( mst>1 || correction>0 )
    //       {
    //  // Optionally refactor the matrix : if parameters.dbase.get<int >("globalStepNumber") % refactorFrequency == 0 
    // 	formMatrixForImplicitSolve(dt0,gf[mNew], gf[mCur] );
    //       }
            

    // ------------------------------------
    // --- Solve the implicit equations ---
    // ------------------------------------
  
        implicitSolve( dt0,gf[mNew], gf[mCur] );  // gf[mNew]=RHS  gf[mCur]=used for initial guess and linearization

        if( Parameters::checkForFloatingPointErrors!=0 )
            checkSolution(gf[mNew].u,"advanceStepsIM: after implicitSolve",true);

        if( debug() & 4 )
        {
            if( twilightZoneFlow() )
            {
                gf[mNew].u.display(sPrintF("ImplicitMS: gf[mNew].u after implicitSolve but BEFORE BC's (t=%8.2e), correction=%i",
                                                                      gf[mNew].t,correction),debugFile,"%8.5f ");
            }
            aString label = sPrintF(" ***ImplicitMS: after implicitSolve but BEFORE BC's, t=%e, correction=%i\n",
                                                            gf[mNew].t,correction);
            determineErrors( gf[mNew],label );
        }
    } // end if takeTimeStep 
    
  // ------------------------------------------------------------------------------------------------
  // --------------------------- Apply Boundary Conditions ------------------------------------------
  // ------------------------------------------------------------------------------------------------
    if( applyBC )
    {
        if( debug() & 2 )
            printP("++++++++++++ apply BCs t0+dt0=%9.3e\n\n",t0+dt0);

    // *added this for fourth order *FINISH ME*
    // if( correction==0 ) 
    // {
    //   // --- For fourth-order in space we need to extrapolate p in time at ghost points --
    //   //    We extrapolate in time using 
    //   //               uCur : t
    //   //               uOld : t-dt
    //   //               fCur : t-2*dt   (holds boundary p and u in unuesd ghost points)
    //   //               fOld : t-3*dt   (holds boundary p and u in unused ghost points)
    //   if( true )
    //   {
    //     int orderOfExtrapolation = orderOfTimeAccuracy==2 ? 3 : 4;
    //     // int orderOfExtrapolation = orderOfTimeAccuracy==2 ? 3 : 5;
    //     boundaryConditionPredictor( predictPressure,adamsData,orderOfExtrapolation, 
    //                                 mNew,mCur,mOld,&fCur,&fOld,&fOld2,&fOld3 );
    //   }
    //   else
    //   {
    //     const int orderOfExtrapolation = 3;
    //     boundaryConditionPredictor( predictPressure,adamsData,orderOfExtrapolation, mNew,mCur,mOld,&fCur,&fOld );
    //   }
                
    // }

    // apply explicit BC's  --- > really only have to apply to implicit grids I think?
        applyBoundaryConditions(gf[mNew]);   // ***** gf[mNew].gridVelocity must be correct here!


        updateStateVariables( gf[mNew],1 );  

        if( debug() & 4 )
        {
            if( twilightZoneFlow() )
            {
                gf[mNew].u.display(sPrintF("ImplicitMS: gf[mNew].u after implicitSolve and BC's (t=%8.2e), correction=%i",
                                                                      gf[mNew].t,correction),debugFile,"%8.5f ");
            }
            aString label = sPrintF(" ***ImplicitMS: after implicitSolve and BC's, t=%e, correction=%i\n",gf[mNew].t,correction);
            determineErrors( gf[mNew],label );
        }

    // extrapolate p in time as an initial guess for iterative solvers
        if( correction==0 )  // *new way* 2015/01/22
        { 
      // --- for some reason the implicit scheme always extrapolates p in time ---
            if( parameters.dbase.has_key("extrapolatePoissonSolveInTime") )
                parameters.dbase.get<bool>("predictedPressureNeeded")= parameters.dbase.get<bool>("extrapolatePoissonSolveInTime");
            const int numberOfTimeLevels=3;
            const int gfIndex[numberOfTimeLevels]={mNew,mCur,mOld}; // 
            predictTimeIndependentVariables( numberOfTimeLevels,gfIndex );
        }

        
        solveForTimeIndependentVariables( gf[mNew] ); 

        if( debug() & 8 )
        {
            aString label =sPrintF(" ImplicitMS: Errors after pressure solve, t0+dt0: t0=%e, dt0=%e  \n",t0,dt0);
            determineErrors( gf[mNew],label );
        }

    // -- Correct for forces on moving bodies if we have more corrections --
    //  *wdh* use macro: 2015/03/08
      // Correct for forces on moving bodies if we have more corrections.
            bool movingGridCorrectionsHaveConverged = false;
            real delta =0.; // holds relative correction when we are sub-cycling 
            const bool useMovingGridSubIterations= parameters.dbase.get<bool>("useMovingGridSubIterations");
      // *wdh* 2015/12/16 -- explicitly check for useMovingGridSubIterations, otherwise we can do multiple
      //                     corrections always if requested,
            if( movingGridProblem() && (numberOfCorrections==1  // *wdh* 2015/05/24 -- this case was missing in new version
                                			      || !useMovingGridSubIterations)  ) // *wdh* 2015/12/16 
            {
                if( numberOfCorrections>10 )
                {
                    printF("WARNING: movingGrid problem, useMovingGridSubIterations=false but numberOfCorrections>10\n");
                    OV_ABORT("ERROR: this is an error for now");
                }
                correctMovingGrids( t0,t0+dt0,gf[mCur],gf[mNew] ); 
            }
    // else if( movingGridProblem() && (correction+1)<numberOfCorrections)
            else if( movingGridProblem() )
            {
        // --- we may be iterating on the moving body motion (e.g.for light bodies) ---
        //     After correcting for the motion, check for convergence
                correctMovingGrids( t0,t0+dt0,gf[mCur],gf[mNew] ); 
        // Check if the correction step has converged
                bool isConverged = getMovingGridCorrectionHasConverged();
                delta = getMovingGridMaximumRelativeCorrection();
                if( debug() & 2 )
                    printF("IMS: moving grid correction step : delta =%8.2e (correction=%i, isConverged=%i)\n",
                   	     delta,correction+1,(int)isConverged);
                if( isConverged && (correction+1) >=minimumNumberOfPCcorrections )  // note correction+1 
                {
                    movingGridCorrectionsHaveConverged=true;  // we have converged -- we can break from correction steps
                    if( delta!=0. && debug() & 1 )
              	printF("IMS: moving grid correction step : sub-iterations converged after %i corrections, rel-err =%8.2e\n",
                     	       correction+1,delta);
          // break;  // we have converged -- break from correction steps
                }
                if( (correction+1)>=numberOfCorrections )
                {
                    printF("IMS:ERROR: moving grid corrections have not converged! numberOfCorrections=%i, rel-err =%8.2e\n",
                   	     correction+1,delta);
                }
            }
            else 
            {
            }
        advanceOptions.correctionIterationsHaveConverged=movingGridCorrectionsHaveConverged;  // we have converged 

    } // end if applyBC
    


    return 0;
}

// ===================================================================================================================
/// \brief End an individual time step (a time sub-step function).
/// \details 
/// \param t0 (input) : current time
/// \param dt0 (input) : current time step
/// \param correction (input) : for predictor corrector methods this indicates the correction step number.
///
// ===================================================================================================================
int DomainSolver::
endTimeStepIM( real & t0, real & dt0, AdvanceOptions & advanceOptions )
{
  //   FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  //   FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");


    assert( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit );
    assert( parameters.dbase.get<int >("orderOfPredictorCorrector")==2 );  // for now we just have 2nd-order in time

    assert( parameters.dbase.get<DataBase >("modelData").has_key("AdamsPCData") );

    AdamsPCData & adamsData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData");
    
    real & dtb=adamsData.dtb;
    int &mab0 =adamsData.mab0, &mab1=adamsData.mab1, &mab2=adamsData.mab2;
    int &nab0 =adamsData.nab0, &nab1=adamsData.nab1, &nab2=adamsData.nab2, &nab3=adamsData.nab3;
    int &ndt0=adamsData.ndt0;
    real *dtp = adamsData.dtp;

    const int numberOfGridFunctions =  movingGridProblem() ? 3 : 2; 

  // permute (mab0,mab1,mab2) 
    mab0 = (mab0-1 + numberOfGridFunctions) % numberOfGridFunctions;
    mab1 = (mab1-1 + numberOfGridFunctions) % numberOfGridFunctions;
  // mab2 is always 1 "ahead" of mab0 
    mab2 = (mab0-1 + numberOfGridFunctions) % numberOfGridFunctions;

    if( parameters.dbase.get<int >("orderOfPredictorCorrector")==2 )
    {
        nab0 = (nab0+1) % 2;
        nab1 = (nab1+1) % 2;
    }
    else
    {
        nab0 = (nab0-1 +4) % 4;
        nab1 = (nab1-1 +4) % 4;
        nab2 = (nab2-1 +4) % 4;
        nab3 = (nab3-1 +4) % 4;
    }

    ndt0=(ndt0-1 +5)%5;  // for dtp[]
            
    dtb=dt0;
    t0+=dt0;

    if( parameters.dbase.get<int >("globalStepNumber") % 10 == 0 )
    {
    // new way:
        if( parameters.dbase.get<int>("useNewImplicitMethod")==1 )
        {
            RealCompositeGridFunction & uti = fn[2];   // ****
            realCompositeGridFunction & residual = uti;  // save residual here -- check this 
            getResidual( t0,dt0,gf[mab0],residual );
            saveSequenceInfo(t0,residual);
        }
            
    }
        
//  output( gf[mab0],initialStep+subStep ); // output to files, user defined output
    output( gf[mab0],parameters.dbase.get<int >("globalStepNumber") ); // output to files, user defined output

    const int zeroUnusedPointsAfterThisManySteps=20;
    if( ( ((parameters.dbase.get<int >("globalStepNumber")+1) % zeroUnusedPointsAfterThisManySteps)==0 ) &&  
            parameters.dbase.get<int >("extrapolateInterpolationNeighbours")==0 )
    {
    // *note* we cannot fixup unused if we extrapolate interp. neighbours since these values will be zeroed out!
    // (esp. important for viscoPlastic model -- linearized solution becomes corrupted)

        if( debug() & 2 ) printF(" ************** DomainSolver::endTimeStep fixupUnusedPoints ************\n");
            
    // zero out unused points to keep them from getting too big ** is this needed?? ****
        for( int m=0; m<=1; m++ )
        {
      // ** gf[m].u.zeroUnusedPoints(coeff);
            fixupUnusedPoints(gf[m].u);
        }
    }
        

  // update the current solution:  
    current = mab0;

    if( debug() & 4 )
        printP("DomainSolver::endTimeStep  t0=%e dt0=%e ----\n",t0,dt0);

    return 0;
}


// ===================================================================================================================
/// \brief Generic advance routine that uses the separate time sub-step functions. 
/// \details This advance routine uses the function startTimeStep, takeTimeStep and endTimeStep
/// \param t0 (input) : current time
/// \param dt0 (input) : current time step
///
// ===================================================================================================================
void DomainSolver::
advanceImplicitMultiStepNew( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  )
// ======================================================================================
// 
// ==========================================================================================
{

    FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

    if( true || debug() & 4 )
        printF(" ---- DomainSolver::advanceImplicitMultiStepNew t0=%e, dt0=%e ----\n",t0,dt0);
    if( debug() & 2 )
        fprintf(debugFile," *** Entering advanceImplicitMultiStep: t0=%e, dt0=%e *** \n",t0,dt0);


    if( init )
    {
        initializeTimeStepping( t0,dt0 );
        init=false;
    }

    AdvanceOptions advanceOptions;
    for( int mst=1; mst<=numberOfSubSteps; mst++ )
    {
    // parameters.dbase.get<int >("globalStepNumber")++;
        int currentGF, nextGF;
        startTimeStep( t0,dt0,currentGF,nextGF,advanceOptions );
        
        for( int correction=0; correction<=advanceOptions.numberOfCorrectorSteps; correction++ )
        {
            takeTimeStep( t0,dt0,correction,advanceOptions );

            if( advanceOptions.correctionIterationsHaveConverged ) // *wdh* 100917 
                break;
        } // end corrections
        
        endTimeStep( t0,dt0,advanceOptions );

    } // end  substeps


}


