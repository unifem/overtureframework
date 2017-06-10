// This file automatically generated from advancePC.bC with bpp.
#include "DomainSolver.h"
#include "CompositeGridOperators.h"
#include "GridCollectionOperators.h"
#include "interpPoints.h"
#include "ExposedPoints.h"
#include "PlotStuff.h"
#include "InterpolateRefinements.h"
#include "Regrid.h"
#include "Ogen.h"
#include "MatrixTransform.h"
#include "updateOpt.h"
#include "App.h"
#include "ParallelUtility.h"
#include "Oges.h"
#include "AdamsPCData.h"

static bool useNewExposedPoints=true;

int
checkForSymmetry(realCompositeGridFunction & u, Parameters & parameters, const aString & label,
                                  int numberOfGhostLinesToCheck);

// // --- Put these somewhere more general, App.h ? Overture.h? -> wdhdefs.h
// // Macros that hide the use of PPP when getting local data (modified versions from insFactors.C)
// #ifdef USE_PPP

// #define OV_GET_SERIAL_ARRAY(TYPE,U,ULOCAL) TYPE ## SerialArray ULOCAL; getLocalArrayWithGhostBoundaries(U,ULOCAL);
// #define OV_GET_SERIAL_ARRAY_CONST(TYPE,U,ULOCAL) TYPE ## SerialArray ULOCAL; getLocalArrayWithGhostBoundaries(U,ULOCAL);
// #define OV_GET_SERIAL_ARRAY_CONDITIONAL(TYPE,U,ULOCAL,CONDITION) // TYPE ## SerialArray ULOCAL; if( CONDITION ) getLocalArrayWithGhostBoundaries(U,ULOCAL);

// #else

// #define OV_GET_SERIAL_ARRAY(TYPE,U,ULOCAL) TYPE ## SerialArray & ULOCAL = U;
// #define OV_GET_SERIAL_ARRAY_CONST(TYPE,U,ULOCAL) const TYPE ## SerialArray & ULOCAL = U;

// #define OV_GET_SERIAL_ARRAY_CONDITIONAL(TYPE,U,ULOCAL,CONDITION) TYPE ## SerialArray & ULOCAL = U;

// #endif

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


// ==============================================================================================
//  Macro: outputDebugInfoAdamsAfterDuDt
// ==============================================================================================



//\begin{>>CompositeGridSolverInclude.tex}{\subsection{advanceAdamsPredictorCorrector}} 
void DomainSolver::
advanceAdamsPredictorCorrector( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  )
//=====================================================================================================
// /Description:
//   Advance some time steps - Adams Predictor Corrector (or Adams Bashforth)
//
// /t0,dt0 (input) : current time and time step.
// /numberOfSubSteps (input) : take this many steps
// /init (input) : if true this is the first time step in whcih case this routine will initialize itself.
//
// /Notes:
//
//  At the start of each subStep in this function: 
//             gf[mab0]  ==  u(t)
//             gf[mab1]  ==  u(t-dt)   if numberOfGridFunctions==2, mab2==mab1
//             gf[mab2]  ==  u(t-2*dt) if numberOfGridFunctions==3 
//
//
// By the end of each subStep
//             gf[mab2]  ==  u(t+dt) : if numberOfGridFunctions==2 then mab2==mab1
//             gf[mab0]  ==  u(t)   
//             gf[mab1]  ==  u(t-dt) if numberOfGridFunctions==3 
//
//   
//\end{CompositeGridSolverInclude.tex}  
//================================================================================================
{
      FILE *& debugFile = parameters.dbase.get<FILE* >("debugFile");
      FILE *& pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");
    
    const int & numberOfSolutionLevels = parameters.dbase.get<int>("numberOfSolutionLevels");
    const int & numberOfTimeDerivativeLevels = parameters.dbase.get<int>("numberOfTimeDerivativeLevels");
    const Parameters::ImplicitMethod & implicitMethod = 
                                parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");

      if( !parameters.dbase.get<DataBase >("modelData").has_key("AdamsPCData") )
          parameters.dbase.get<DataBase >("modelData").put<AdamsPCData>("AdamsPCData");
      AdamsPCData & adamsData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData");
    
      real & dtb=adamsData.dtb;
      int &mab0 =adamsData.mab0, &mab1=adamsData.mab1, &mab2=adamsData.mab2;
      int &nab0 =adamsData.nab0, &nab1=adamsData.nab1, &nab2=adamsData.nab2, &nab3=adamsData.nab3;
      int &ndt0=adamsData.ndt0;
      real *dtp = adamsData.dtp;

//   printF("\n &&&&&&&&&&& advanceAdamsPredictorCorrector t0=%8.2e &&&&&&&&&&&&&&&&&\n"
//          "  mab0,mab1,mab2 = %i, %i, %i \n"
//          "  nab0,nab1,nab2,nab3= %i, %i, %i, %i \n"
//         ,t0,mab0,mab1,mab2,nab0,nab1,nab2,nab3);

    const int numberOfDimensions = parameters.dbase.get<int >("numberOfDimensions");;
    const int orderOfAccuracy = parameters.dbase.get<int >("orderOfAccuracy");
    const int orderOfTimeAccuracy = parameters.dbase.get<int >("orderOfTimeAccuracy");
    const int orderOfPredictorCorrector = parameters.dbase.get<int >("orderOfPredictorCorrector");
    const Parameters::TimeSteppingMethod timeSteppingMethod=
        parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
    RealArray & timing = parameters.dbase.get<RealArray>("timing");
    

    aString method,label;
    int numberOfCorrections;
    assert( timeSteppingMethod==Parameters::adamsBashforth2 ||
                    timeSteppingMethod==Parameters::adamsPredictorCorrector2 ||
                    timeSteppingMethod==Parameters::adamsPredictorCorrector4 );

    assert( orderOfPredictorCorrector==2 ||
                    orderOfPredictorCorrector==4 );

    int & predictorOrder = parameters.dbase.get<int>("predictorOrder");
    if( predictorOrder==0 )
    {
    // predictorOrder=2; // default
        predictorOrder=orderOfPredictorCorrector; // *wdh* fixed for PC44  Dec 22, 2016
    }
    
    if( predictorOrder<0 || predictorOrder>orderOfPredictorCorrector )
    {
        if( init )
            printF("advancePC: ERROR: predictorOrder=%i!",predictorOrder);
        predictorOrder=2; // default
        if( init )
            printF("Will use default=%i\n",predictorOrder);
        
    }

    if( timeSteppingMethod==Parameters::adamsBashforth2 )
        numberOfCorrections=0;  // adams predictor only
    else
        numberOfCorrections=parameters.dbase.get<int>("numberOfPCcorrections"); // predictor corrector
  
  // If we check a convergence tolerance when correcting (e.g. for moving grids) then this is
  // the minimum number of corrector steps we must take:
    const int minimumNumberOfPCcorrections = parameters.dbase.get<int>("minimumNumberOfPCcorrections");
    
    if( debug() & 2 )
    {
        if( init )
        {
            printF(" advanceAdamsPredictorCorrector: orderOfPredictorCorrector=%i predictorOrder=%i numberOfCorrections=%i\n",
                          orderOfPredictorCorrector,predictorOrder, numberOfCorrections);
        }
        fPrintF(debugFile," *** Entering advanceAdamsPredictorCorrector: t0=%e, dt0=%e *** \n",t0,dt0);
    }
    
    int mInitial=mab0;  // save initial value
    
  // For moving grids we keep gf[mab0], gf[mab1] and gf[mab2]
  // For non-moving grids we keep gf[mab0], gf[mab1] and we set mab2==mab1

    const int numberOfGridFunctions =  movingGridProblem() ? 3 : 2; 

    mab2 = (mab0 -1 + numberOfGridFunctions) % numberOfGridFunctions;

  // 
    int mNew = mab2;    // new     : gf[mNew] : will hold u(t+dt)
    int mCur = mab0;    // current : gf[mCur] : holds u(t) 
    int mOld = mab1;    // old     : gf[mOld] : holds u(t-dt) if numberOfGridFunctions==3 otherwise mOld=mNew
    
    int nNew = nab1;    // new :    ut(t+dt)
    int nCur = nab0;    // current: ut(t)
    int nOld = nab1;    // old :    ut(t-dt)


    int grid;
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];  
    Range N = parameters.dbase.get<Range >("Rt");   // time dependent variables
    RealArray error(numberOfComponents()+3); 
    Range C=parameters.dbase.get<int >("numberOfComponents");
    int iparam[10];
    real rparam[10];
    
    int numberOfExtraPressureTimeLevels=0;

  // real time0=getCPU();
    checkArrays(" adamsPC, start"); 
    
    if( debug() & 4 )
    {
        determineErrors( gf[mCur].u,gf[mCur].gridVelocity, gf[mCur].t, 0, error,
                      sPrintF("\n ---> adams:START errors in u at t=%e \n",gf[mCur].t) );
    }
    
    if( init )
    {
    // **** To initialize the method we need to compute du/dt at times t and t-dt *****

    // this is a macro (pcMacros.h):
        const int numberOfPastTimes=1;                            // PC needs u(t-dt)
        const int numberOfPastTimeDerivatives=orderOfAccuracy-1;  // PC needs u_t(t-dt), u_t(t-2*dt), ...
        const int orderOfPredictorCorrector = parameters.dbase.get<int >("orderOfPredictorCorrector");
        const int orderOfTimeExtrapolationForPressure = parameters.dbase.get<int >("orderOfTimeExtrapolationForPressure");
        printF("--adamsPC-- initializePredictorCorrector: mCur=%i, mOld=%i gf[mCur].t=%9.2e\n",mCur,mOld,gf[mCur].t);
        fPrintF(debugFile,"--adamsPC-- initializePredictorCorrector: mCur=%i, mOld=%i gf[mCur].t=%9.2e\n",mCur,mOld,gf[mCur].t);
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
            printF("--adamsPC-- orderOfPredictorCorrector=%i, orderOfTimeExtrapolationForPressure=%i, predictPressure=%i\n",
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
                    		gf[mab0].u[grid],&gf[mOld].cg[grid]);
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
                    fPrintF(debugFile,"--adamsPC-- take an initial step backwards\n");
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
                	  display(gf[mOld].cg[grid].vertex()(I1,I2,I3,0),sPrintF("\n --adamsPC-- AFTER moveGrids:  gf[mOld] grid=%i vertex after move back t=%e",grid,gf[mOld].t),
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
                      printF("\n --adamsPC-- init past time solution gf[mgf=%i] at t=%9.3e numberOfGridFunctions=%i " 
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
                      ::display(gf[mgf].u[0],sPrintF("--adamsPC-- past time solution gf[mgf=%i].u t=%9.3e",mgf,tgf),"%6.3f ");
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
              	printF("--adamsPC-- init past time du/dt at t=%9.3e (gf[mgf=%i].t=%9.3e) fn[ngf=%i]\n",
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
                    	      gf[mab0].u[grid],&gf[mgf].cg[grid]);
              	if( false )
              	{
                	  ::display(fn[ngf][grid],sPrintF("--adamsPC-- past time du/dt fn[ngf=%i] t=%9.3e",ngf,tgf),"%6.3f ");
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
                    	      gf[mab0].u[grid],&gf[mOld].cg[grid]);
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
                        display(fn[nab1][grid],sPrintF("adamsPC:init: ut(t-dt) grid=%i from TZ at t=%e\n",grid,gf[mOld].t),debugFile,"%5.2f ");
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
                    display(gf[mOld].u[grid],sPrintF(buff,"\n--adamsPC-- Init:gf[mOld].u grid=%i : du/dt(t) t=%9.3e",grid,gf[mOld].t),
                                    debugFile,"%9.3e ");
                    if( numberOfPastTimeDerivatives>0 )
                        display(fn[nab1][grid],sPrintF(buff,"\n--adamsPC-- Init:fn[nab1] grid=%i : du/dt(t) t=%9.3e",grid,gf[mOld].t),
                                      debugFile,"%9.3e ");
                    if( parameters.isMovingGridProblem() )
                    {
                        display(gf[mOld].getGridVelocity(grid),sPrintF("--adamsPC-- t=-dt: gridVelocity[%i] at t=%9.3e\n",grid,gf[mOld].t),debugFile,"%5.2f ");
                        display(gf[mCur].getGridVelocity(grid),sPrintF("--adamsPC-- t=0 : gridVelocity[%i] at t=%9.3e\n",grid,gf[mCur].t),debugFile,"%5.2f ");
                    }
                    if( debug() & 64 && parameters.isMovingGridProblem() )
                    {
              	display(gf[mOld].cg[grid].vertex(),sPrintF("--adamsPC-- gf[mOld].cg[%i].vertex at t=%9.3e\n",grid,gf[mOld].t),debugFile,"%7.4f ");
              	display(gf[mCur].cg[grid].vertex(),sPrintF("--adamsPC-- gf[mCur].cg[%i].vertex at t=%9.3e\n",grid,gf[mCur].t),debugFile,"%7.4f ");
                    }
                }
            }
            if( debug() & 4 )
            {
                if( parameters.isMovingGridProblem() )
                {
                    determineErrors( gf[mOld].u,gf[mOld].gridVelocity, gf[mOld].t, 0, error,
                           		       sPrintF("--adamsPC-- errors in u at t=%9.3e (t0-dt0=%9.3e)\n",gf[mOld].t,t0-dt0) );
                    if( numberOfPastTimeDerivatives>0 )
                    {
                        fn[nab1].updateToMatchGrid(gf[mOld].cg);  // for moving grid TZ to get errors correct
                        determineErrors( fn[nab1],gf[mOld].gridVelocity, gf[mOld].t, 1, error,
                               		       sPrintF("--adamsPC-- errors in ut (fn[nab1]) at t=%9.3e (t0-dt0=%9.3e)\n",gf[mOld].t,t0-dt0) );
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
      // printF(" **************** adamsPC: still need correct initial values for du/dt(t-dt)  ****** \n");
      // printF(" **************** use values from du/dt(t)                                  ****** \n");
            printF("\n--adamsPC-- Initialize past time values for scheme, numberOfPastTimes=%i"
                          " numberOfPastTimeDerivatives=%i ---\n",numberOfPastTimes,numberOfPastTimeDerivatives);
            if( debug() & 2 )
                fPrintF(debugFile,"--adamsPC-- Initialize past time values for scheme, numberOfPastTimes=%i"
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
                printF(" -- adamsPC-- USE OLD STARTUP, Set past time solutions to t=0 solution \n");
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
                printF(" -- adamsPC-- USE NEW STARTUP numberOfPastTimes=%i mCur=%i mOld=%i\n",numberOfPastTimes,mCur,mOld);
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
              	printF("--adamsPC-- init past time du/dt at t=%9.3e (gf[mgf=%i].t=%9.3e) fn[ngf=%i]\n",
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
                    	      gf[mab0].u[grid],&gf[mgf].cg[grid]);
              	if( false )
              	{
                	  ::display(fn[ngf][grid],sPrintF("--adamsPC-- past time du/dt fn[ngf=%i] t=%9.3e",ngf,tgf),"%6.3f ");
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
                        fPrintF(debugFile,"--adamsPC-- get past time du/dt at t=%9.3e...\n",gf[mOld].t);
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
                                    gf[mab0].u[grid],&gf[mOld].cg[grid]);
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
        
    }
    else
    {
    // this is not an initialization step

    }
    

    if( debug() & 16 )
    {
        if( twilightZoneFlow() )
        {
            determineErrors( gf[mCur],sPrintF("\n ---> AdamsPC: Errors at start t=%e  \n",gf[mCur].t) );
        }
        else
        {
            outputSolution( gf[mCur].u,gf[mCur].t,sPrintF(" AdamsPC: Solution at start t=%e  \n",gf[mCur].t) );
        }
    }

    for( int mst=1; mst<=numberOfSubSteps; mst++ )
    {
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
        parameters.dbase.get<int >("globalStepNumber")++;
        
        realCompositeGridFunction & ua = fn[nab0];   // pointer to du/dt
        realCompositeGridFunction & ub = fn[nab1];   // pointer to du(t-dt)/dt

        if( orderOfPredictorCorrector==2 ) 
        {
            nab2 =  nab0;
            nab3 =  nab1;
        }
        
        realCompositeGridFunction & uc = fn[nab2];
        realCompositeGridFunction & ud = fn[nab3];

        realCompositeGridFunction & uNew = fn[nab3]; // Here is where we put u.t(t+dt)


    // -------------------------------------------------
    // --------------- adaptive grids ------------------
    // -------------------------------------------------

        bool useNew=false;

        const int regridFrequency = parameters.dbase.get<int >("amrRegridFrequency")>0 ? parameters.dbase.get<int >("amrRegridFrequency") :
                                                                parameters.dbase.get<Regrid* >("regrid")==NULL ? 2 : parameters.dbase.get<Regrid* >("regrid")->getRefinementRatio();
        bool gridWasAdapted=false; // set to true if we have performed an AMR regrid on this step
        if( parameters.isAdaptiveGridProblem() && ((parameters.dbase.get<int >("globalStepNumber") % regridFrequency) == 0) )
        {
            gridWasAdapted=true;

            printF("\n ***** AdamsPC: AMR regrid at step %i ***** \n\n",parameters.dbase.get<int >("globalStepNumber"));
            if( debug() & 2 )
      	fPrintF(debugFile,"\n ***** AdamsPC: AMR regrid at step %i ***** \n\n",parameters.dbase.get<int >("globalStepNumber"));
            
            real timea=getCPU();

            GridFunction & gf0= gf[mCur];
            GridFunction & gf1= gf[mOld];
            
            if( parameters.useConservativeVariables() )
      	gf0.primitiveToConservative();  // *wdh* 010318  -- do amr interpolation on conservative variables.

            const int numberToUpdate=1; // we update one extra grid-function to live on the new AMR grid

            if( !useNew || !parameters.isMovingGridProblem() )  // 
            { // for non-moving grids we interpolate uNew=du/dt(t-dt)
      	
	// we need to extrapolate values on uNew since the interpolator may use these values
      	uNew.setOperators(*gf[mCur].u.getOperators());
      	uNew.interpolate(); // we need to interpolate uNew since we may use these values.
      	uNew.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.);
      	uNew.finishBoundaryConditions();

      	adaptGrids( gf0, numberToUpdate,&uNew ); 
                gf1.updateToMatchGrid(gf0.cg);  // *wdh* 040928
            }
            else
            { // for moving grids we interpolate gf1 = u(t-dt)
        // du/dt(t-dt) will be recomputed below in the moving grid section

      	adaptGrids( gf0, numberToUpdate,&gf1.u ); 
                gf1.updateToMatchGrid(gf0.cg);
            }

      // the next has been moved into adaptGrids 070706
      // real time1=getCPU();
      // gf0.cg.rcData->interpolant->updateToMatchGrid( gf0.cg ); 
      // timing(Parameters::timeForUpdateInterpolant)+=getCPU()-time1;
            real time1=getCPU();

            if( debug() & 2 ) printf("adams:adapt step: update gf0 for moving grids...\n");
            updateForMovingGrids(gf0);  // ****

      // do here for now -- we shouldn't do this in updateForMovingGrids since this is not correct below
      // when the grids are moved
            gf0.gridVelocityTime=gf0.t -1.e10; 

      // we need to recompute the grid velocity on AMR grids -- really only need to do refinements***
            if( parameters.isMovingGridProblem() )
            {
        // recompute the grid velocity
                getGridVelocity( gf0, gf0.t );
            }

            interpolateAndApplyBoundaryConditions( gf0 );
            timing(parameters.dbase.get<int>("timeForAmrBoundaryConditions"))+=getCPU()-time1;
            
            if( debug() & 16 )
            {
      	if( parameters.dbase.get<bool >("twilightZoneFlow") )
      	{
        	  determineErrors( gf0,sPrintF(" AdamsPC: errors after regrid, t=%e \n",gf0.t) ) ;
      	}
      	else
      	{
        	  outputSolution( gf0.u,gf0.t,sPrintF(" ***after regrid: solution ***\n") );
      	}
            }

            ua.updateToMatchGrid(gf0.cg);

            gf1.updateToMatchGrid(gf0.cg);
            if( !useNew || !parameters.isMovingGridProblem() )
                gf1.u.updateToMatchGrid(gf1.cg);
      // gf1.u=0.;
            gf1.u.setOperators(*gf0.u.getOperators());
            
            if( debug() & 2 ) printf("adams:adapt step: update gf1 for moving grids...\n");
            updateForMovingGrids(gf1);
      // do here for now -- we shouldn't do this in updateForMovingGrids since this is not correct below
      // when the grids are moved
            gf1.gridVelocityTime=gf1.t -1.e10;

            
            if( numberOfGridFunctions==3 )
            { 
	// update gf2
        // ***note: if we want to retain u(t-dt) then we must interpolate here instead, as was done
        //          for uNew in the call to adaptGrids

                GridFunction & gf2= gf[mNew];

      	if( parameters.isMovingGridProblem() )
      	{
        	  gf2.cg=gf0.cg;  // we make a copy in this case
      	}
      	else
      	{
        	  gf2.updateToMatchGrid(gf0.cg);
      	}
      	gf2.u.updateToMatchGrid(gf2.cg);
      	gf2.u=0.;
      	gf2.u.setOperators(*gf0.u.getOperators());
            
      	updateForMovingGrids(gf2);
      	gf2.gridVelocityTime=gf2.t -1.e10;

            }
            

      // p has actually been interpolated ok ?? maybe ghost points are wrong?
      // **** solveForTimeIndependentVariables( gf0 ); 

            if( true )
            {

      	if( parameters.useConservativeVariables() )
        	  gf0.conservativeToPrimitive();  // *wdh* 010318

      	real dtNew= getTimeStep( gf0 ); //       ===Choose time step====


                int numberOfSteps;
                real nextTimeToPrint=gf0.t+(numberOfSubSteps-mst+1)*dt;
      	real tFinal=nextTimeToPrint;
                computeNumberOfStepsAndAdjustTheTimeStep(gf0.t,tFinal,nextTimeToPrint,numberOfSteps,dtNew);
      	
                numberOfSubSteps=mst+numberOfSteps-1;

                if( true || debug() & 1 )
              	  printf("AdamsPC:recompute dt: dt(old)=%8.3e, dtNew = %8.3e, t=%9.3e (step=%i)\n",dt,dtNew,gf0.t,
                                  parameters.dbase.get<int >("globalStepNumber"));
      	dt=dtNew;  // *********************** should this be dt0 ???????????????????????????????????
            }
            timing(parameters.dbase.get<int>("timeForAmrRegrid"))+=getCPU()-timea;

        }

    // moveTheGridsMacro(adamsPC,gf[mCur].u); // *wdh* 090804 

        real ab1,ab2;
        if( predictorOrder==1 )
        { // first order predictor
            ab1=dt0;
            ab2=0.;
        }
        else 
        { // 2nd -order predictor
            ab1= dt0*(1.+dt0/(2.*dtb));  // becomes 1.5*dt0  if dt0==dtb
            ab2= -dt0*dt0/(2.*dtb);      //         -.5*dt0
        }
    
        dtp[ndt0]=dt0;
        real dt1=dtp[(ndt0+1)%5];
        real dt2=dtp[(ndt0+2)%5];
        real dt3=dtp[(ndt0+3)%5];
        real dt4=dtp[(ndt0+4)%5];


    // ------------------------------------------------------
    // ----------------- Moving Grids -----------------------
    // ------------------------------------------------------
        real tb=gf[mCur].t-dt1, tc=tb-dt2, td=tc-dt3; // check me 
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
                    		gf[mCur].u[grid],&gf[mNew].cg[grid]);
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
        


    // coefficients for the 4th-order AdamsBashforth predictor for variable dt (from ab.maple)
    // For dt=fixed these would be
    // ab41 = ( 55./24.)*dt0;    
    // ab42 = (-59./24.)*dt0;
    // ab43 = ( 37./24.)*dt0;
    // ab44 = ( -9./24.)*dt0;
        const real ab41 = (6.0*dt0*dt2*dt2+12.0*dt2*dt2*dt1+8.0*dt0*dt0*dt2+24.0*dt2*dt0*dt1+
                                              12.0*dt2*dt1*dt3+6.0*dt3*dt2*dt0+24.0*dt1*dt1*dt2+12.0*dt0*dt3*dt1+18.0*dt0*dt1
                   		       *dt1+4.0*dt0*dt0*dt3+12.0*dt1*dt1*dt3+3.0*dt0*dt0*dt0+12.0*dt0*dt0*dt1+12.0*dt1
                   		       *dt1*dt1)*dt0/(dt1+dt2+dt3)/dt1/(dt1+dt2)/12.0;
        const real ab42 = -dt0*dt0*(6.0*dt1*dt1+6.0*dt3*dt1+12.0*dt2*dt1+8.0*dt0*dt1+3.0*dt0*
                        				dt0+6.0*dt2*dt3+4.0*dt0*dt3+8.0*dt2*dt0+6.0*dt2*dt2)/dt1/(dt2+dt3)/dt2/12.0;
        const real ab43 = dt0*dt0*(6.0*dt1*dt1+6.0*dt2*dt1+6.0*dt3*dt1+8.0*dt0*dt1+3.0*dt0*dt0
                         			       +4.0*dt2*dt0+4.0*dt0*dt3)/dt3/dt2/(dt1+dt2)/12.0;
        const real ab44 = -(6.0*dt1*dt1+6.0*dt2*dt1+8.0*dt0*dt1+4.0*dt2*dt0+3.0*dt0*dt0)*dt0*
                                                dt0/(dt1+dt2+dt3)/(dt2+dt3)/dt3/12.0;

    // coefficients for the 3rd-order AdamsBashforth predictor for variable dt (from ab.maple)
    // For dt=fixed these would be 
    // ab31 = (23/12.)*dt0; 
    // ab32 = ( -4/3.)*dt0; 
    // ab33 = ( 5/12.)*dt0; 
        const real ab31 = dt0*(2.0*dt0*dt0+6.0*dt0*dt1+3.0*dt2*dt0+6.0*dt1*dt1+6.0*dt2*dt1)/(dt1+dt2)/dt1/6.0;
        const real ab32 = -(3.0*dt1+3.0*dt2+2.0*dt0)*dt0*dt0/dt2/dt1/6.0;
        const real ab33 = dt0*dt0*(2.0*dt0+3.0*dt1)/(dt1+dt2)/dt2/6.0;


    // coefficients for 2nd order extrap:
        const real cex2a=1.+dt0/dtb;       // -> 2.
        const real cex2b=-dt0/dtb;         // -> -1.

    // coefficients for third order extrapolation (from ab.maple)
    //   These reduce to 3, -3, 1 for dt=constant
        const real cex30= (dt0+dt1+dt2)*(dt0+dt1)/(dt1+dt2)/dt1;
        const real cex31= -(dt0+dt1+dt2)/dt2*dt0/dt1;
        const real cex32= (dt0+dt1)*dt0/dt2/(dt1+dt2);

    // coefficients for 4th order extrapolation 
    //   (corresponds to   4 -6 4 1 for dt=fixed)
        const real cex40= (dt0+dt1+dt2+dt3)*(dt0+dt1+dt2)*(dt0+dt1)/(dt1+dt2+dt3)/(dt1+dt2)/dt1;
        const real cex41= -(dt0+dt1+dt2+dt3)/(dt2+dt3)*(dt0+dt1+dt2)/dt2*dt0/dt1;
        const real cex42= (dt0+dt1+dt2+dt3)*(dt0+dt1)*dt0/dt3/dt2/(dt1+dt2);
        const real cex43= -(dt0+dt1+dt2)*(dt0+dt1)*dt0/dt3/(dt2+dt3)/(dt1+dt2+dt3);

    // coefficients for fixth order extrapolation (exact for 4th order poly's)
    // These reduce to 5, -10, 10, -5, 1 for dt=constant
//    const real cex50=5., cex51=-10., cex52=10., cex53=-5., cex54=1.;
//  const real cex50=4., cex51=-6., cex52=4., cex53=-1., cex54=0.;

        const real cex50=(dt0+dt1+dt2+dt3+dt4)*(dt0+dt1+dt2+dt3)*(dt0+dt1+dt2)*(dt0+dt1)/
            (dt1+dt2+dt3+dt4)/(dt1+dt2+dt3)/(dt1+dt2)/dt1;
        const real cex51= -(dt0+dt1+dt2+dt3+dt4)/(dt2+dt3+dt4)*(dt0+dt1+dt2+dt3)/(dt2+dt3)*(
            dt0+dt1+dt2)/dt2*dt0/dt1;
        const real cex52= (dt0+dt1+dt2+dt3+dt4)*(dt0+dt1+dt2+dt3)*(dt0+dt1)*dt0/(dt3+dt4)/dt3/dt2/(dt1+dt2);
        const real cex53= -(dt0+dt1+dt2+dt3+dt4)*(dt0+dt1+dt2)*(dt0+dt1)*dt0/dt4/dt3/(dt2+dt3)/(dt1+dt2+dt3);
        const real cex54= (dt0+dt1+dt2+dt3)*(dt0+dt1+dt2)*(dt0+dt1)*dt0/dt4/(dt3+dt4)/
              (dt2+dt3+dt4)/(dt1+dt2+dt3+dt4);
        
    // ********************************************************************
    // ************  Compute ua = d(u(t)/dt  ******************************
    // ********************************************************************

        if( parameters.useConservativeVariables() )    // *wdh* 010318 convert here. Should be done before interpExposed?
            gf[mCur].primitiveToConservative();

    // -- evaluate any body forcing (this is saved in realCompositeGridFunction bodyForce found in the data-base) ---
    // -- provide solution at past time levels if available: 
        const int numberOfBodyForceTimeLevels= numberOfGridFunctions>2 ? 2 : 1;
        int gfIndexBodyForce[] = {mCur,mOld}; // 
        real timesBodyForce[] =  {gf[mCur].t,gf[mOld].t};
        const real tForce = gf[mCur].t; // evaluate the body force at this time
        computeBodyForcing( gf, gfIndexBodyForce, timesBodyForce, numberOfBodyForceTimeLevels, tForce );

    // *old way*
    // const real tForce = gf[mCur].t; // evaluate the body force at this time
    // computeBodyForcing( gf[mCur], tForce );
        if( debug() & 4 )
        {
            fPrintF(debugFile,"--PC-- before getUt at t=%9.3e --- \n",gf[mCur].t);
        }
        
        for( grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
        {
      	rparam[0]=gf[mCur].t;
      	rparam[1]=gf[mCur].t; // tForce
      	rparam[2]=gf[mCur].t; // tImplicit
      	iparam[0]=grid;
                iparam[1]=gf[mCur].cg.refinementLevelNumber(grid);
                iparam[2]=numberOfStepsTaken;

      	getUt(gf[mCur].u[grid],gf[mCur].getGridVelocity(grid),ua[grid],iparam,rparam,
                            Overture::nullRealMappedGridFunction(),&gf[mNew].cg[grid]);

        }
        
//     if( predictorOrder>1 && parameters.isMovingGridProblem() )
//     {
//       // ****** THIS DOES NOT WORK: du/dt on moving grids includes the -gDot.grad(u) term!

//       OV_ABORT("finish me");
            
//       // We interpolate ua=du/dt(t) so we can compute exposed pts if necessary
//       CompositeGrid & cga = *ua.getCompositeGrid();
//       Interpolant & interpa = *new Interpolant(cga);  // ************** TEMP : LEAK here ***********
//       interpa.interpolate(ua);
//     }
        


        if( debug() & 4 || debug() & 64 )
        {
            if( parameters.dbase.get<int >("myid")==0 )
            {
                fprintf(debugFile,"\n ----------------------------------------------------------------\n"); 
                fprintf(debugFile,"After_compute_du/dt_in_predictor");
            }
        }
        if( debug() & 64 )
        {
            for( grid=0; grid<gf[mab0].cg.numberOfComponentGrids(); grid++ )
            {
                aString buff;
                display(ua[grid],sPrintF(buff,"\n ****ua grid=%i : du/dt(t)",grid),debugFile,"%7.1e ");
                display(ub[grid],sPrintF(buff,"\n ****ub grid=%i: du/dt(t-dt)",grid),debugFile,"%7.1e ");
            }
        }
        if( debug() & 4 || debug() & 64 )
        {
            determineErrors( gf[mCur].u,gf[mCur].gridVelocity, gf[mCur].t, 0, error,
                    sPrintF(" adams: errors in u at t=%e \n",gf[mCur].t) );
            determineErrors( gf[mOld].u,gf[mOld].gridVelocity, gf[mOld].t, 0, error,
                    sPrintF(" adams: errors in uOld at t=%e \n",gf[mOld].t) );
            determineErrors( ua,gf[mCur].gridVelocity, t0, 1, error,
                                              sPrintF(" adams: errors in ut (ua) at t=%e \n",t0) );
            determineErrors( ub,gf[mOld].gridVelocity, t0-dtb, 1, error,
                                            sPrintF(" adams: errors in ut (ub) at t=%e \n",t0-dtb) );
            if( parameters.dbase.get<int >("orderOfPredictorCorrector")==4 )
            {
        // --- This gridVelocity is not correct:
                determineErrors( uc,gf[mOld].gridVelocity, t0-2.*dtb, 1, error,
                         		     sPrintF(" adams: errors in ut (uc) at t=%e \n",t0-2*dtb) );
                determineErrors( ud,gf[mOld].gridVelocity, t0-3.*dtb, 1, error,
                                    sPrintF(" adams: errors in ut (ud) at t=%e \n",t0-3*dtb) );
            }
            if( debug() & 16 )
            {
                label=sPrintF(" ***************** ab1=%8.2e ab2=%8.2e  *****************\n"
                                            " ***Adams PC: gf[mOld] before advance interior t=%e\n",ab1,ab2,gf[mab1].t);
                if( twilightZoneFlow() )
                    determineErrors( gf[mOld],label );
                else
                    outputSolution( gf[mOld].u,gf[mOld].t,label );
                label=sPrintF(" ***Adams PC: gf[mCur] before advance interior t=%e\n",gf[mab0].t);
                if( twilightZoneFlow() )
                    determineErrors( gf[mCur],label );
                else
                    outputSolution( gf[mCur].u,gf[mCur].t,label );
            }
        }

        if( debug() & 64 )
        {
            for( grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
      	::display(gf[mCur].u[grid],sPrintF(" ***Adams PC: u for predictor"
                                      " t=%9.4e grid=%i\n",gf[mCur].t,grid),debugFile,"%10.7f ");
            for( grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
      	::display(ua[grid],sPrintF(" ***Adams PC: du/dt (ua) for predictor"
                                      " t=%9.4e grid=%i\n",gf[mCur].t,grid),debugFile,"%10.7f ");
        }


        addArtificialDissipation(gf[mCur].u,dt0);  // add "implicit" dissipation to u 

        if( Parameters::checkForFloatingPointErrors )
            checkSolution(gf[mCur].u,"AdamsPC: u0 before adding ut",true);

        const bool useOptUpdate=true;  // use new optimized updates

        real cpu0=getCPU();
    //  gf[1].u <- gf[mCur].u + dt*( 1.5* du(t)/dt - .5 du(t-dt)/dt
        for( grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
        {
          #ifdef USE_PPP
            RealArray u0;  getLocalArrayWithGhostBoundaries(gf[mCur].u[grid],u0);
            RealArray u1;  getLocalArrayWithGhostBoundaries(gf[mNew].u[grid],u1);
            RealArray uta; getLocalArrayWithGhostBoundaries(ua[grid],uta);
            RealArray utb; getLocalArrayWithGhostBoundaries(ub[grid],utb);
            RealArray utc; getLocalArrayWithGhostBoundaries(uc[grid],utc);
            const intSerialArray & mask1 = gf[mNew].cg[grid].mask().getLocalArray();
          #else
            const RealArray & u0 = gf[mCur].u[grid];
            const RealArray & u1 = gf[mNew].u[grid];
            const RealArray & uta= ua[grid];
            const RealArray & utb= ub[grid];
            const RealArray & utc= uc[grid];
            const intSerialArray & mask1 = gf[mNew].cg[grid].mask(); 
          #endif

            getIndex(gf[mNew].cg[grid].extendedIndexRange(),I1,I2,I3);
      // ******************************** note: may need du/dt(t-dt) at more exposed points than we have ************

            if( useOptUpdate )
            {
                const int n1a=max(u1.getBase(0),I1.getBase()), n1b=min(u1.getBound(0),I1.getBound());  
                const int n2a=max(u1.getBase(1),I2.getBase()), n2b=min(u1.getBound(1),I2.getBound());
                const int n3a=max(u1.getBase(2),I3.getBase()), n3b=min(u1.getBound(2),I3.getBound());
      	
                int ierr=0;
                const int maskOption=0; // assign pts where mask>0
                int ipar[]={0,maskOption,n1a,n1b,n2a,n2b,n3a,n3b,N.getBase(),N.getBound()}; //
                real rpar[5]={0.,0.,0.,0.,0.};
      	if( orderOfPredictorCorrector==2 )
      	{
	  // u1(I1,I2,I3,N)=u0(I1,I2,I3,N) + ab1*uta(I1,I2,I3,N) + ab2*utb(I1,I2,I3,N);
                    ipar[0]=2;
        	  rpar[0]=ab1; rpar[1]=ab2;
      	}
      	else if( orderOfPredictorCorrector==4 )
      	{
	  // here is the 4th-order predictor
	  //  gf[mNew].u[grid](I1,I2,I3,N)=gf[mCur].u[grid](I1,I2,I3,N) + 
	  //  ab41*ua[grid](I1,I2,I3,N) + ab42*ub[grid](I1,I2,I3,N) + ab43*uc[grid](I1,I2,I3,N) + ab44*ud[grid](I1,I2,I3,N);

	  // Here is the 3rd order predictor
	  // gf[mNew].u[grid](I1,I2,I3,N)=gf[mCur].u[grid](I1,I2,I3,N) + 
	  //   ab31*ua[grid](I1,I2,I3,N) + ab32*ub[grid](I1,I2,I3,N) + ab33*uc[grid](I1,I2,I3,N);

	  // u1(I1,I2,I3,N)=u0(I1,I2,I3,N) + ab31*uta(I1,I2,I3,N) + ab32*utb(I1,I2,I3,N) + ab33*utc(I1,I2,I3,N);
                    ipar[0]=3;
        	  rpar[0]=ab31; rpar[1]=ab32; rpar[2]=ab33;
      	}
      	else
      	{
        	  Overture::abort();
      	}
      	updateOpt(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
              		  u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
              		  *mask1.getDataPointer(),  
              		  *u0.getDataPointer(),*u1.getDataPointer(), 
              		  *uta.getDataPointer(),*utb.getDataPointer(),*utc.getDataPointer(),*utc.getDataPointer(),
              		  ipar[0], rpar[0], ierr );

            }
            else
            {
      	if( orderOfPredictorCorrector==2 ) 
      	{
	  // gf[mNew].u[grid](I1,I2,I3,N)=gf[mCur].u[grid](I1,I2,I3,N) + 
	  //   ab1*ua[grid](I1,I2,I3,N) + ab2*ub[grid](I1,I2,I3,N);

        	  u1(I1,I2,I3,N)=u0(I1,I2,I3,N) + ab1*uta(I1,I2,I3,N) + ab2*utb(I1,I2,I3,N);
      	}
      	else if( orderOfPredictorCorrector==4 )
      	{
	  // here is the 4th-order predictor
	  //  gf[mNew].u[grid](I1,I2,I3,N)=gf[mCur].u[grid](I1,I2,I3,N) + 
	  //  ab41*ua[grid](I1,I2,I3,N) + ab42*ub[grid](I1,I2,I3,N) + ab43*uc[grid](I1,I2,I3,N) + ab44*ud[grid](I1,I2,I3,N);

	  // Here is the 3rd order predictor
	  // gf[mNew].u[grid](I1,I2,I3,N)=gf[mCur].u[grid](I1,I2,I3,N) + 
	  //   ab31*ua[grid](I1,I2,I3,N) + ab32*ub[grid](I1,I2,I3,N) + ab33*uc[grid](I1,I2,I3,N);

        	  u1(I1,I2,I3,N)=u0(I1,I2,I3,N) + ab31*uta(I1,I2,I3,N) + ab32*utb(I1,I2,I3,N) + ab33*utc(I1,I2,I3,N);
      	}
      	else
      	{
        	  Overture::abort();
      	}
            }
            
        }
        gf[mNew].t=t0+dt0;  // gf[mNew] now lives at this time
        gf[mNew].form=gf[mCur].form;

        if( Parameters::checkForFloatingPointErrors )
            checkSolution(gf[mNew].u,"AdamsPC: u1 after adding ut",true);

        if( debug() & 16 )
        {
            for( grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
      	::display(gf[mNew].u[grid],sPrintF(" ***Adams PC: uNew after predictor update"
                                      " t=%9.4e grid=%i\n",gf[mNew].t,grid),debugFile,"%10.7f ");
        }


    // --- predict values for boundary conditions -- ( e.g. for fractional-step methods) ---
        const int orderOfExtrapolation = orderOfPredictorCorrector==2 ? 3 : 4;
        boundaryConditionPredictor( predictPressureAndVelocity,adamsData,orderOfExtrapolation,
                                                                mNew,mCur,mOld,&ua,&ub,&uc,&ud );

        if( debug() & 4 )
        {
            label=sPrintF(" ***Adams PC: predictor: before interpolate t=%e\n",gf[mNew].t);
            determineErrors( gf[mNew],label );
        }
        
        if( debug() & 16  )
        {
            label=sPrintF(" ***Adams PC: predictor: before interpolate t=%e\n",gf[mNew].t);
            if( twilightZoneFlow() )
      	determineErrors( gf[mNew],label );
            else
      	outputSolution( gf[mNew].u,gf[mNew].t,label );
        }
        timing(parameters.dbase.get<int>("timeForAddUt"))+=getCPU()-cpu0;


        interpolateAndApplyBoundaryConditions( gf[mNew],&gf[mCur],dt0 );


        if( debug() & 8 )
        {
            label=sPrintF(" ***Adams PC: after boundary conditions in predictor, t=%e\n",gf[mNew].t);
            if( twilightZoneFlow() )
      	determineErrors( gf[mNew],label );
            else
      	outputSolution( gf[mNew].u,gf[mNew].t,label );
        }
        if( debug() & 4 || debug() & 64 )
        {
            for( grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
      	::display(gf[mNew].u[grid],sPrintF(" ***Adams PC: after apply boundary conditions in predictor"
                                      " t=%9.4e grid=%i\n",gf[mNew].t,grid),debugFile,"%10.7f ");
        }
        if( debug() & 4 )
        {
            label=sPrintF(" ***Adams PC: after apply boundary conditions in predictor t=%e\n",gf[mNew].t);
            determineErrors( gf[mNew],label );
        }

        cpu0=getCPU();
    //  ---- extrapolate time-independent variables (e.g. p) in time -----
    //        e.g. needed for an initial guess for iterative solvers (or some added mass algorithms)
    // *new way* *wdh* 2015/01/21 
        const int numberOfTimeLevels=3;
        const int gfIndex[numberOfTimeLevels]={mNew,mCur,mOld}; // 
        predictTimeIndependentVariables( numberOfTimeLevels,gfIndex );

/* ---
        else
        {
      // **OLD WAY**
    
            if( poisson!=NULL && poisson->isSolverIterative() && orderOfAccuracy!=4 )
            {
	// extrapolate p in time as an initial guess for iterative solvers
      	const int & pc = parameters.dbase.get<int >("pc");
      	assert( pc>= 0 );
      	for( grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
      	{
        	  getIndex(gf[mCur].cg[grid].dimension(),I1,I2,I3);
	  // note that initially gf[mNew](.,.,.,pc) = p(t-dt)
	  // **** check this -- it's doesn't seem to make much difference whether we
	  // extrpolate or use the old value ??
#ifdef USE_PPP
        	  realSerialArray uNew; getLocalArrayWithGhostBoundaries(gf[mNew].u[grid],uNew);
        	  realSerialArray uCur; getLocalArrayWithGhostBoundaries(gf[mCur].u[grid],uCur);
        	  realSerialArray uOld; getLocalArrayWithGhostBoundaries(gf[mOld].u[grid],uOld);
                    bool ok = ParallelUtility::getLocalArrayBounds(gf[mNew].u[grid],uNew,I1,I2,I3); 
                    if( !ok ) continue;
#else
                    const realSerialArray & uNew=gf[mNew].u[grid];
                    const realSerialArray & uCur=gf[mCur].u[grid];
                    const realSerialArray & uOld=gf[mOld].u[grid];
#endif

        	  uNew(I1,I2,I3,pc)=cex2a*uCur(I1,I2,I3,pc)+cex2b*uOld(I1,I2,I3,pc);
	  //  gf[mNew].u[grid](I1,I2,I3,pc)=cex2a*gf[mCur].u[grid](I1,I2,I3,pc)+cex2b*gf[mOld].u[grid](I1,I2,I3,pc);

        	  if( debug() & 4 )
        	  {
          	    Range all;
          	    for( grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
            	      ::display(gf[mNew].u[grid](all,all,all,pc),sPrintF(" ***Adams PC: after extrap p in time"
                                                 								 " t=%9.4e grid=%i\n",gf[mNew].t,grid),debugFile,"%10.7f ");
        	  }

// **	gf[mNew].u[grid](I1,I2,I3,pc)=gf[mCur].u[grid](I1,I2,I3,pc);
      	}
            }
        }
    ---- */
      
        timing(parameters.dbase.get<int>("timeForAddUt"))+=getCPU()-cpu0;

        
        bool updateSolutionDependentEquations=true;  // e.g. for variable density, do update p eqn here 
        solveForTimeIndependentVariables( gf[mNew],updateSolutionDependentEquations ); 

    // correct for forces on moving bodies
        if( movingGridProblem() ) 
        {
            correctMovingGrids( t0,t0+dt0,gf[mCur],gf[mNew] ); 
        }


        if( debug() & 4 )
        {
            real max1=max(fabs(gf[mNew].u)), max2=max(fabs(gf[mCur].u));
            if( parameters.dbase.get<int >("myid")==0 )
            {
      	fprintf(debugFile,
            		"AdamsPredictorCorrector After solve for time indep. vars: "
            		"max(fabs(gf[mNew]))=%e, max(fabs(gf[mCur]))=%e \n",
            		max1,max2); 
            }
        }
        if( debug() & 8 )
        {
            label=sPrintF(" AdamsPC: After Predictor, t0+dt0: t0=%e, dt0=%e  \n",t0,dt0);
            if( twilightZoneFlow() )
      	determineErrors( gf[mNew],label );
            else
      	outputSolution( gf[mNew].u,gf[mNew].t,label );
        }

    // For constant dt the coefficients would be
    //    am41=(9./24.)*dt0, am42=(19./24.)*dt0, am43=(-5./24.)*dt0, am44=(1./24.)*dt0;
    // Here are the coeff for variable dt: (from ab.maple)
        const real am41 = (6.0*dt1*dt1+6.0*dt2*dt1+8.0*dt0*dt1+4.0*dt2*dt0+3.0*dt0*dt0)*dt0/(
                                              dt0+dt1+dt2)/(dt0+dt1)/12.0;
        const real am42 = dt0*(dt0*dt0+4.0*dt0*dt1+2.0*dt2*dt0+6.0*dt1*dt1+6.0*dt2*dt1)/(dt1+dt2)/dt1/12.0;
        const real am43 = -dt0*dt0*dt0*(dt0+2.0*dt1+2.0*dt2)/(dt0+dt1)/dt2/dt1/12.0;
        const real am44 = (dt0+2.0*dt1)*dt0*dt0*dt0/(dt0+dt1+dt2)/(dt1+dt2)/dt2/12.0;

        for( int correction=0; correction<numberOfCorrections; correction++ )
        {
            parameters.dbase.get<int>("totalNumberOfPCcorrections")++;  // count the total number of corrections.

      //       ---Adams Moulton Corrector
      //          u(t+dt) <- u(t) + dt* ( (1/2) du(*)/dt + (1/2) du(t)/dt )
      //          gf[mNew]  gf[mCur]              ub               ua
            real am1=.5*dt0;
            real am2=.5*dt0;

      // real am41=(9./24.)*dt0, am42=(19./24.)*dt0, am43=(-5./24.)*dt0, am44=(1./24.)*dt0;

      //       --- f(*) <- du(*)/dt(t)
      //           uNew      d gf[mNew].u/dt
            if( parameters.useConservativeVariables() )    // *wdh* 010318 convert here. 
            {
                gf[mCur].primitiveToConservative();
                gf[mNew].primitiveToConservative();
            }
            
      // -- evaluate any body forcing (this is saved in realCompositeGridFunction bodyForce found in the data-base) ---
      // -- provide solution at past time levels if available: 
            const int numberOfBodyForceTimeLevels= 2;
            int gfIndexBodyForce[] = {mNew,mCur}; // 
            real timesBodyForce[] =  {gf[mNew].t,gf[mCur].t};
            const real tForce =  gf[mNew].t; // evaluate the body force at this time
            computeBodyForcing( gf, gfIndexBodyForce, timesBodyForce, numberOfBodyForceTimeLevels, tForce );

      // const real tForce = gf[mNew].t; // evaluate the body force at this time
      // computeBodyForcing( gf[mCur], tForce );  // *NOTE* *wdh* 2015/08/19 -- this should have used gf[mNew] I believe

            for( grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
            {
      	rparam[0]=gf[mNew].t;
      	rparam[1]=gf[mNew].t; // tForce
      	rparam[2]=gf[mNew].t; // tImplicit
      	iparam[0]=grid;
                iparam[1]=gf[mNew].cg.refinementLevelNumber(grid);
                iparam[2]=numberOfStepsTaken;

	// mappedGridSolver[grid]->getUt(gf[mNew].u[grid],gf[mNew].getGridVelocity(grid),uNew[grid],iparam,rparam,
        //                 Overture::nullRealMappedGridFunction(),&gf[mNew].cg[grid]);
      	getUt(gf[mNew].u[grid],gf[mNew].getGridVelocity(grid),uNew[grid],iparam,rparam,
                                                  Overture::nullRealMappedGridFunction(),&gf[mNew].cg[grid]);
            }

            if( debug() & 8 || debug() & 64 )
            {
      	determineErrors( ua,gf[mCur].gridVelocity, t0, 1, error,
                      sPrintF(" adams:corrector: errors in ut (ua) at t=%e \n",t0) );

      	if( parameters.dbase.get<int >("myid")==0 )
        	  fprintf(debugFile," ****>> &uNew=%i &ub=%i\n",&uNew,&ub);
      	
                display(uNew[0],sPrintF("uNew[0] at t=%e\n",gf[mNew].t),debugFile,"%5.2f ");
                if( parameters.isMovingGridProblem() )
                    display(gf[mNew].getGridVelocity(0),sPrintF("gridVelocity[0] at t=%e\n",gf[mNew].t),
                                                                                                            debugFile,"%5.2f ");
      	
      	determineErrors( uNew,gf[mNew].gridVelocity, gf[mNew].t, 1, error,
                              sPrintF(" adams:corrector: errors in ut (uNew) at t=%e \n",gf[mNew].t) );

      	if( orderOfPredictorCorrector==4 )
      	{
        	  if( parameters.isMovingGridProblem() )
                        determineErrors( uc,gf[mNew].gridVelocity, t0-2.*dtb, 1, error,
                                  sPrintF(" adams:corrector: errors in ut (uc) at t=%e \n",t0-2*dtb) );

        	  determineErrors( ud,gf[mNew].gridVelocity, t0+dtb, 1, error,
                                            sPrintF(" adams:corrector: errors in ut (ud) at t=%e \n",t0+dt) );
      	}
            }
        
            addArtificialDissipation(gf[mNew].u,dt0);  // add "implicit" dissipation to u 
          
            if( Parameters::checkForFloatingPointErrors )
      	checkSolution(gf[mNew].u,"AdamsPC: u1 before corrector",true);

            real cpu0=getCPU();
            for( grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
            {
      	getIndex(gf[mNew].cg[grid].extendedIndexRange(),I1,I2,I3);

      	if( useOptUpdate )
      	{
                  #ifdef USE_PPP
                    RealArray u0;  getLocalArrayWithGhostBoundaries(gf[mCur].u[grid],u0);
                    RealArray u1;  getLocalArrayWithGhostBoundaries(gf[mNew].u[grid],u1);
                    RealArray uta; getLocalArrayWithGhostBoundaries(ua[grid],uta);
                    RealArray utb; getLocalArrayWithGhostBoundaries(ub[grid],utb);
                    RealArray utc; getLocalArrayWithGhostBoundaries(uc[grid],utc);
                    RealArray utd; getLocalArrayWithGhostBoundaries(ud[grid],utd);
                    const intSerialArray & mask1 = gf[mNew].cg[grid].mask().getLocalArray();

                #else
                    const RealArray & u0 = gf[mCur].u[grid];
                    const RealArray & u1 = gf[mNew].u[grid];
                    const RealArray & uta= ua[grid];
                    const RealArray & utb= ub[grid];
                    const RealArray & utc= uc[grid];
                    const RealArray & utd= ud[grid];
                    const intSerialArray & mask1 = gf[mNew].cg[grid].mask(); 
                  #endif

                    const int n1a=max(u1.getBase(0),I1.getBase()), n1b=min(u1.getBound(0),I1.getBound());  
                    const int n2a=max(u1.getBase(1),I2.getBase()), n2b=min(u1.getBound(1),I2.getBound());
                    const int n3a=max(u1.getBase(2),I3.getBase()), n3b=min(u1.getBound(2),I3.getBound());
      	
        	  int ierr=0;
                    const int maskOption=0; // assign pts where mask>0
        	  int ipar[]={0,maskOption,n1a,n1b,n2a,n2b,n3a,n3b,N.getBase(),N.getBound()}; //
        	  real rpar[5]={0.,0.,0.,0.,0.};
                    real *ut1p, *ut2p, *ut3p, *ut4p;
        	  if( orderOfPredictorCorrector==2 )
        	  {
          	    ipar[0]=2;
          	    rpar[0]=am1; rpar[1]=am2;
                        ut1p=utb.getDataPointer();
          	    ut2p=uta.getDataPointer();
                        ut3p=ut2p;  // not used
          	    ut4p=ut2p;  // not used
        	  }
        	  else if( orderOfPredictorCorrector==4 )
        	  {
          	    ipar[0]=4;
          	    rpar[0]=am41; rpar[1]=am42; rpar[2]=am43; rpar[3]=am44;
                        ut1p=utd.getDataPointer();
                        ut2p=uta.getDataPointer();
                        ut3p=utb.getDataPointer();
                        ut4p=utc.getDataPointer();
        	  }
        	  else
        	  {
          	    Overture::abort();
        	  }
        	  updateOpt(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
                		    u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
                		    *mask1.getDataPointer(),  
                		    *u0.getDataPointer(),*u1.getDataPointer(), 
                		    *ut1p, *ut2p, *ut3p, *ut4p,
                		    ipar[0], rpar[0], ierr );

      	}
      	else
      	{
        	  if( orderOfPredictorCorrector==2 )
        	  {
          	    gf[mNew].u[grid](I1,I2,I3,N)=gf[mCur].u[grid](I1,I2,I3,N)+am1*ub[grid](I1,I2,I3,N) 
            	      + am2*ua[grid](I1,I2,I3,N);
        	  }
        	  else if( orderOfPredictorCorrector==4 )
        	  {
          	    gf[mNew].u[grid](I1,I2,I3,N)=gf[mCur].u[grid](I1,I2,I3,N)+am41*ud[grid](I1,I2,I3,N) 
            	      +am42*ua[grid](I1,I2,I3,N) 
            	      +am43*ub[grid](I1,I2,I3,N) 
            	      +am44*uc[grid](I1,I2,I3,N);

        	  }
      	}
      	
            }
      // * gf[mNew].t=t0+dt0;  //  gf[mNew] now lives at this time
            timing(parameters.dbase.get<int>("timeForAddUt"))+=getCPU()-cpu0;

            if( Parameters::checkForFloatingPointErrors )
      	checkSolution(gf[mNew].u,"AdamsPC: u1 after corrector",true);

            if( debug() & 16  )
            {
      	label=sPrintF(" ***Adams PC: corrector: before interpolate t=%e\n",gf[mNew].t);
      	if( twilightZoneFlow() )
        	  determineErrors( gf[mNew],label );
      	else
        	  outputSolution( gf[mNew].u,gf[mNew].t,label );
            }

            interpolateAndApplyBoundaryConditions( gf[mNew],&gf[mCur],dt0 );

            if( debug() & 8 )
            {
      	label=sPrintF("======advance2 After boundary conditions in corrector, t=%e \n",t0+dt0);
      	if( twilightZoneFlow() )
        	  determineErrors( gf[mNew],label );
      	else
                    outputSolution( gf[mNew].u,gf[mNew].t,label );
            }
        
            
            updateSolutionDependentEquations=false;  // e.g. for variable density, do not update p eqn here 
            solveForTimeIndependentVariables( gf[mNew],updateSolutionDependentEquations ); 

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
                        printF("PC: moving grid correction step : delta =%8.2e (correction=%i, isConverged=%i)\n",
                       	     delta,correction+1,(int)isConverged);
                    if( isConverged && (correction+1) >=minimumNumberOfPCcorrections )  // note correction+1 
                    {
                        movingGridCorrectionsHaveConverged=true;  // we have converged -- we can break from correction steps
                        if( delta!=0. && debug() & 1 )
                  	printF("PC: moving grid correction step : sub-iterations converged after %i corrections, rel-err =%8.2e\n",
                         	       correction+1,delta);
            // break;  // we have converged -- break from correction steps
                    }
                    if( (correction+1)>=numberOfCorrections )
                    {
                        printF("PC:ERROR: moving grid corrections have not converged! numberOfCorrections=%i, rel-err =%8.2e\n",
                       	     correction+1,delta);
                    }
                }
                else 
                {
                }
            if( movingGridCorrectionsHaveConverged )
                break;
            
      // if( movingGridProblem() && (correction+1)<numberOfCorrections)
      // {
      // 	correctMovingGrids( t0,t0+dt0,gf[mCur],gf[mNew] ); 

      // 	// Check if the correction step has converged
      // 	movingGridCorrectionsHaveConverged = getMovingGridCorrectionHasConverged();
      // 	real delta = getMovingGridMaximumRelativeCorrection();
      // 	if( debug() & 2 )
      // 	  printF("PC: moving grid correction step : delta =%8.2e (correction=%i, isConverged=%i)\n",
      // 		 delta,correction+1,(int)movingGridCorrectionsHaveConverged);

      // 	if( movingGridCorrectionsHaveConverged && (correction+1) >=minimumNumberOfPCcorrections )  // note correction+1 
      // 	{
      // 	  if( debug() & 1 )
      // 	    printF("PC: moving grid correction step : sub-iterations converged after %i corrections, rel-err =%8.2e\n",
      // 		   correction+1,delta);
      // 	  break;  // we have converged -- break from correction steps
      // 	}
      // }
      // if( (correction+1)==numberOfCorrections && !movingGridCorrectionsHaveConverged )
      // {
      //   real delta = getMovingGridMaximumRelativeCorrection();
      // 	printF("PC:ERROR: moving grid corrections have not converged! numberOfCorrections=%i, rel-err =%8.2e\n",
      // 		   correction+1,delta);
      // }
            

        
            if( debug() & 4 )
      	printf("AdamsPredictorCorrector After correction: max(fabs(gf[mNew]))=%e, max(fabs(gf[mCur]))=%e \n",
             	       max(fabs(gf[mNew].u)),max(fabs(gf[mCur].u)));

            if( debug() & 8 )
            {
      	if( twilightZoneFlow() )
      	{
        	  determineErrors( gf[mNew],sPrintF("======advance2 Errors After PECE, t0+dt0=%e \n",t0+dt0) );
      	}
      	else
      	{
                    outputSolution( gf[mNew].u,gf[mNew].t,sPrintF( " ======advance2 solution after PECE, t0+dt0=%e \n",t0+dt0));
      	}
            }
        }// end for correction.
        
        
        
    // permute (mab0,mab1,mab2) 
        mab0 = (mab0-1 + numberOfGridFunctions) % numberOfGridFunctions;
        mab1 = (mab1-1 + numberOfGridFunctions) % numberOfGridFunctions;
        mab2 = (mab0-1 + numberOfGridFunctions) % numberOfGridFunctions;

        mNew=mab2;
        mCur=mab0;
        mOld=mab1;

        if( orderOfPredictorCorrector==2 ) 
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

    // -- Save a pointer to the residual:  fn[nab1] is assumed to hold the latest "residual" (i.e. du/dt) ---
        realCompositeGridFunction *& pResidual  = parameters.dbase.get<realCompositeGridFunction*>("pResidual"); // current residual
        pResidual = &fn[nab1];

        if( (mst-1) % parameters.dbase.get<int >("frequencyToSaveSequenceInfo") == 0 )
        { // save residual in the sequence info (also output residual info if requested)
            saveSequenceInfo(t0,*pResidual);
      // saveSequenceInfo(t0,fn[nab1]);
        }
        
    // output( gf[mab0],initialStep+mst-1 ); // output to files, user defined output
        output( gf[mab0],parameters.dbase.get<int >("globalStepNumber")+1 ); // output to files, user defined output

    } // end for mst 
    

    if( parameters.useConservativeVariables() )
        gf[mab0].conservativeToPrimitive();
    
  // update the current solution:  
    current = mab0;
    
    checkArrayIDs("advancePC:end");
    
}

