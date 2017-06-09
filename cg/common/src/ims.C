// This file automatically generated from ims.bC with bpp.
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
#include "CyclicIndex.h"

//==================================================================
//            Implicit Multi-step Method
//            --------------------------
//
//==================================================================

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

// This next declaration also appears in advance.bC
#define updateOpt EXTERN_C_NAME(updateopt)
#define updateOptNew EXTERN_C_NAME(updateoptnew)
extern "C"
{
      void updateOpt(const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,
                                    const int &nd3a,const int &nd3b,const int &nd4a,const int &nd4b, 		  const int &mask,real &u1, const real&u2,  
                                    const real&ut1, const real&ut2, const real&ut3, const real&ut4, 
                                    const int &ipar, const real& rpar, int & ierr );

      void updateOptNew(const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,
                                          const int &nd3a,const int &nd3b,const int &nd4a,const int &nd4b, 		     const int &mask,real & uNew,
                         	             const real&u1, const real&u2, const real&u3, const real&u4, const real&u5,
                         	             const real&u6, const real&u7, const real&u8, const real&u9, const real&u10,
                                          const int &ipar, const real& rpar, int & ierr );
}


#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) for( side=0; side<=1; side++ )

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{buildImplicitSolvers}} 
void DomainSolver::
buildImplicitSolvers(CompositeGrid & cg)
// ==========================================================================================
// /Description:
//     Determine the number and type of implicit solvers needed. Depending on the boundary
//
//  1) If the equations are decoupled and the boundary conditions for all components are 
//     the same then we can form one scalar implicit system.
//  2) If the equations are decoupled and the boundary conditions are not the same but 
//     decoupled then we can solve separate scalar implicit systems.
//  3) If the boundary conditions or equations are coupled then we solve a implicit system.  
//
//\end{CompositeGridSolverInclude.tex}  
// ==========================================================================================
{
    printF("DomainSolver::buildImplicitSolvers:ERROR: base class function called! This function should\n"
                  "              be re-written by a derived class!\n");
    Overture::abort("error");

}


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{implicitSolve}} 
void DomainSolver::
implicitSolve(const real & dt0,
            	      GridFunction & cgf1,
                            GridFunction & cgf0)
// ==========================================================================================
// /Description:
//    The implicit method can be optionally used on only some grids. To implement this
//   approach we simply create a sparse matrix that is just the identity matrix on grids that
// are advanced explicitly but equal to the standard implicit matrix on grids that are advance
// implicitly:
// \begin{verbatim}
//         I - \nu \alpha \dt \Delta      on implicit grids
//         I                              on explicit grids
// \end{verbatim}
// If the form of the boundary conditions for the different components of $\uv$ are the same
// then we can build a single scalar matrix that can be used to advance each component, one after
// the other. If the boundary conditions are not of the same form then we build a matrix for
// a system of equations for the velocity components $(u,v,w)$.
//
// /dt0 (input) : time step used to build the implicit matrix.
// /cgf1 (input/output) : On input holds the right-hand-side for the implicit equations; on output
//    holds the solution.
// /cgf0 (input) : current best approximation to the solution. Used as initial guess for iterative
//   solvers and used for linearization.
//\end{CompositeGridSolverInclude.tex}  
// ==========================================================================================
{
    
    printF("DomainSolver::implicitSolve:ERROR: base class function called! This function should\n"
                  "              be re-written by a derived class!\n");
    OV_ABORT("error");

}

// ===========================================================================================
//      *** Time-step update macro ***
// 
//  IMEX-CN
//    u1 <- u0 + AB1*UA0 + AB2*UB0 [ + DTI*UTIMPLICIT ]  (add last terms for implicit grids)
//
//  IMEX-BDF2
//    u1 = (4/3)*uCur + (-1/3)*uOld + (4/3)*dt*utCur + (-2/3)*dt*utOld + (2/3)*dt*utImplicit
//
// ===========================================================================================


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{advanceImplicitMultiStep}} 
void DomainSolver::
advanceImplicitMultiStep( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  )
// ======================================================================================
//  /Description:
//     Time step using an implicit method on the viscous terms and and a multi-step
//    method on the rest.
//
//  This approach should be efficient for moving grids since a larger time step is taken
// and the grid need to be regenerated fewer times. The grid needs only be moved on the
// predictor step. Then one or more corrections can be applied.
//
//
//  Method:
//   Suppose we are solving a PDE:
//           u_t = f(u,x,t)  + F(x,t)
//   That we have split into an explicit part, fe(u),  and implicit part, A*u:
//           u_t = fe(u) + A u  + F(x,t)
//
//   Predictor (2nd-order): (implicit part involves u(p) )
//
//      (u(p)-u(n))/dt = 1.5*( fe(u(n)) + g(n) ) - .5*( fe(u(n-1))+g(n-1) )
//                       + A(  u(p)+u(n) )/2   + G(n)
//   where for TZ flow with exact soluton, ue, the forcing functions g and G are 
//             g(n) = ue_t - fe(ue)
//             G(n) = -[ A( ue(n+1)+ue(n) )/2 ]
// 
//   Corrector:
//      (u(c)-u(n))/dt = 5*( fe(u(p)) + g(p) ) + .5*( fe(u(n))+g(n) )  
//                            + A( u(c)+u(n) )/2   + G(n)
//   
//  Note: 
//           utImplicit = A*u(n)/2  + G(n) 
//  which is computed (once per step) by getUt when implicitOption=computeImplicitTermsSeparately
// 
//\end{CompositeGridSolverInclude.tex}  
// ==========================================================================================
{
    FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

    if( debug() & 4 )
        printF(" $$$$$$ DomainSolver::advanceImplicitMultiStep $$$$$$$$\n");

    const Parameters::TimeSteppingMethod & timeSteppingMethod =
        parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::implicit;
    
    assert( timeSteppingMethod==Parameters::implicit );

    const Parameters::ImplicitMethod & implicitMethod = 
                                parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");
    assert( implicitMethod==Parameters::crankNicolson ||
                    implicitMethod==Parameters::implicitExplicitMultistep );

    const int & numberOfSolutionLevels = parameters.dbase.get<int>("numberOfSolutionLevels");
    const int & numberOfTimeDerivativeLevels = parameters.dbase.get<int>("numberOfTimeDerivativeLevels");
    const int orderOfAccuracy = parameters.dbase.get<int >("orderOfAccuracy");
    const int orderOfTimeAccuracy = parameters.dbase.get<int >("orderOfTimeAccuracy");

    if( !parameters.dbase.get<DataBase >("modelData").has_key("AdamsPCData") )
    {
    // this must be the initial call to this routine
        parameters.dbase.get<DataBase >("modelData").put<AdamsPCData>("AdamsPCData");
    }

    AdamsPCData & adamsData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData");
    
    real & dtb=adamsData.dtb;
  // int &mab0 =adamsData.mab0, &mab1=adamsData.mab1, &mab2=adamsData.mab2;
    int &mab0 =adamsData.mab0;
    int &nab0 =adamsData.nab0, &nab1=adamsData.nab1, &nab2=adamsData.nab2, &nab3=adamsData.nab3;
    int &ndt0=adamsData.ndt0;
    real *dtp = adamsData.dtp;

    int & predictorOrder = parameters.dbase.get<int>("predictorOrder");
    int & orderOfPredictorCorrector= parameters.dbase.get<int >("orderOfPredictorCorrector");
    if( implicitMethod==Parameters::implicitExplicitMultistep )
    {
        orderOfPredictorCorrector=orderOfTimeAccuracy;  // *FIX ME**
        predictorOrder=orderOfTimeAccuracy;  // *FIX ME**
    }

    assert( orderOfPredictorCorrector==2 || 
                    orderOfPredictorCorrector==4    );

    if( predictorOrder==0 )
        predictorOrder=2; // default
    if( predictorOrder<0 || predictorOrder>orderOfTimeAccuracy )
    {
        if( init )
            printF("advanceImplicitMultiStep: ERROR: predictorOrder=%i!",predictorOrder);
        predictorOrder=2; // default
        if( init )
            printF("Will use default=%i\n",predictorOrder);
    }
    if( init && debug() & 1 )
    {
        printF("advanceImplicitMultiStep:INFO: predictorOrder=%i ( =0 -> use default, order=%i)\n",
                      predictorOrder,orderOfTimeAccuracy);
        fPrintF(debugFile,"advanceImplicitMultiStep:INFO: predictorOrder=%i ( =0 -> use default, order=%i)\n",
                        predictorOrder,orderOfTimeAccuracy);
    }
    
    int numberOfCorrections=parameters.dbase.get<int>("numberOfPCcorrections"); 

  // If we check a convergence tolerance when correcting (e.g. for moving grids) then this is
  // the minimum number of corrector steps we must take:
    const int minimumNumberOfPCcorrections = parameters.dbase.get<int>("minimumNumberOfPCcorrections");
    
      
    
    
  // ***** NOTE *********
  // **** For moving grids we need to keep gf[OLD] in addition to fn[OLD] since we need the mask
  // **** for computing exposed points
  // ********************

  // For moving grids we keep gf[mab0], gf[mab1] and gf[mab2]
  // For non-moving grids we keep gf[mab0], gf[mab1] and we set mab2==mab1

  // *wdh* 2017/01/29 const int numberOfGridFunctions =  movingGridProblem() ? 3 : 2; 
    const int numberOfGridFunctions =  numberOfGridFunctionsToUse; // assigned in setupGridFunctions

  // Make a reverse cyclic index to match mab0: 
    CyclicIndex mgfi(numberOfSolutionLevels,ovmod(-mab0,numberOfSolutionLevels),false);
    CyclicIndex nfni(numberOfTimeDerivativeLevels,ovmod(-nab0,numberOfTimeDerivativeLevels),false);
    if( (true || implicitMethod==Parameters::implicitExplicitMultistep) && t0<5.*dt0 )
    {
        printF(" mgfi[1]=%i mgfi[0]=%i mgfi[-1]=%i mgfi[-2]=%i mgfi[-3]=%i\n", 
                          mgfi[1],mgfi[0],mgfi[-1],mgfi[-2],mgfi[-3]);
        printF(" nfni[1]=%i nfni[0]=%i nfni[-1]=%i nfni[-2]=%i nfni[-3]=%i\n", 
                          nfni[1],nfni[0],nfni[-1],nfni[-2],nfni[-3]);

    // mgfi.shift();
    // printF(" mgfi[2]=%i mgfi[1]=%i mgfi[0]=%i mgfi[-1]=%i mgfi[-2]=%i (after shift)\n", mgfi[2],mgfi[1],mgfi[0],mgfi[-1]
    //	   ,mgfi[-2]);
    }
    
  // Cyclic indexes for gf[.]
    int mNew = mgfi[ 1];    // new     : gf[mNew] : will hold u(t+dt)
    int mCur = mgfi[ 0];    // current : gf[mCur] : holds u(t) 
    int mOld = mgfi[-1];    // old     : gf[mOld] : holds u(t-dt) if numberOfGridFunctions==3 otherwise mOld=mNew


  // Index's into fn[.]
    int nNew=nfni[1];
    int nCur=nfni[0];
    int nOld=nfni[-1];


  // // *** WHAT IS THIS ??? *WDH* 2017
  // mab2 = (mab0 -1 + numberOfGridFunctions) % numberOfGridFunctions;


  // int mNew = mab2;    // new     : gf[mNew] : will hold u(t+dt)
  // int mCur = mab0;    // current : gf[mCur] : holds u(t) 
  // int mOld = mab1;    // old     : gf[mOld] : holds u(t-dt) if numberOfGridFunctions==3 otherwise mOld=mNew

    
    if( debug() & 2 )
    {
        fPrintF(debugFile," *** Entering advanceImplicitMultiStep: t0=%e, dt0=%e *** \n",t0,dt0);
  
        if( implicitMethod==Parameters::implicitExplicitMultistep && t0<5.*dt0 )
        {
            printF("____-> IMEX AB-BDF scheme: numberOfCorrections=%i numberOfSolutionLevels=%i "
                          "numberOfTimeDerivativeLevels=%i numberOfGridFunctions=%i numberOfExtraFunctionsToUse=%i\n",
                          numberOfCorrections,numberOfSolutionLevels,numberOfTimeDerivativeLevels,numberOfGridFunctions,
                          numberOfExtraFunctionsToUse);

        }
        else
        {
            printF("____-> IM scheme: numberOfCorrections=%i numberOfSolutionLevels=%i "
                          "numberOfTimeDerivativeLevels=%i numberOfGridFunctions=%i numberOfExtraFunctionsToUse=%i\n",
                          numberOfCorrections,numberOfSolutionLevels,numberOfTimeDerivativeLevels,numberOfGridFunctions,
                          numberOfExtraFunctionsToUse);
        }
    // printF("____-> [mab0,mab1,mab2]=[%i,%i,%i] [nab0,nab1,nab2]=[%i,%i,%i] [mNew,mCur,mOld]=[%i,%i,%i]\n",
    //       mab0,mab1,mab2, nab0,nab1,nab2, mNew,mCur,mOld);

        printF("____-> [mab0]=[%i,%i,%i] [nab0,nab1,nab2]=[%i,%i,%i] [mNew,mCur,mOld]=[%i,%i,%i]\n",
                    mab0, nab0,nab1,nab2, mNew,mCur,mOld);
        


    }



    Parameters::ImplicitOption & implicitOption = parameters.dbase.get<Parameters::ImplicitOption >("implicitOption");
    
    implicitOption=Parameters::doNotComputeImplicitTerms; // no need to compute during initialization

  // parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::crankNicolson;

    int iparam[10];
    real rparam[10];

  // uti : holds the implicit part of the operator at the current time "n" used in C.N. for example.
    RealCompositeGridFunction & uti = fn[numberOfExtraFunctionsToUse-1];  // *wdh* 2017/02/03 Was [2]
  // RealCompositeGridFunction & uti = fn[2];   // **** Holds du/dt(t) for implicit part of operator I think..
    
    if( debug() & 2 )
    {
        printF(" *** Entering advanceImplicitMultiStep: t0=%e, dt0=%e dtb=%e*** \n",t0,dt0,dtb);
        fPrintF(debugFile," *** Entering advanceImplicitMultiStep: t0=%e, dt0=%e dtb=%e *** \n",t0,dt0,dtb);
    }
  
    
    int grid;
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2]; 
    Range N = parameters.dbase.get<Range >("Rt");   // time dependent variables
    RealArray error(numberOfComponents()+3); 
    
  // real time0=getCPU();

    int numberOfExtraPressureTimeLevels=0;
    
    if( init )
    {
    // Form the matrix for implicit time stepping (optionally save the solution used for linearization)
    // NOTE: the matrix will only be generated the first time through or if parameters.dbase.get<int >("globalStepNumber") % refactorFrequency == 0 
    // We want to factor the matrix here since it may be needed for computing the RHS
        formMatrixForImplicitSolve(dt,gf[mNew], gf[mCur] );

    // **** To initialize the method we need to compute du/dt at times t and t-dt *****

    // this is a macro (pcMacros.h):
    // PC22 needs 
    //   numberOfPastTimes=1 : u(t-dt)
    //   numberOfPastTimeDerivatives=1 : u_t(t-dt)
    // BDF4 needs
    //  numberOfPastTimes=3 : u(n-1) u(n-2) u(n-3)
    // numberOfPastTimeDerivatives=3 : u_t(n-1) u_t(n-2) u_t(n-3)
        const int numberOfPastTimes=orderOfTimeAccuracy-1;
        const int numberOfPastTimeDerivatives=orderOfTimeAccuracy-1;
    // *old* -- *wdh* Feb 3, 2017
    // *old* int numberOfPastTimes=1;                            // PC needs u(t-dt)
    // *old* int numberOfPastTimeDerivatives=orderOfAccuracy-1;  // PC needs u_t(t-dt), u_t(t-2*dt), ...

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
          // *** FIX ME: 4 - numberOfExtraFunctionsToUse
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

    }
    else
    {
        if( dtb!=dt0 )
        {
            if( debug() & 2 ) printF("advanceImplicitMultiStep dtb!=dt0 : re-initialize\n");
            parameters.dbase.get<int >("initializeImplicitTimeStepping")=true;
        }
        else
        {
            if( debug() & 2 )
                printF("advanceImplicitMultiStep dtb==dt0 : do not re-initialize\n");
        }
    }

    const bool TESTING=false;
    

    for( int mst=1; mst<=numberOfSubSteps; mst++ )
    {
        parameters.dbase.get<int >("globalStepNumber")++;
        
        realCompositeGridFunction & ua = fn[nab0];   // pointer to du/dt
        realCompositeGridFunction & ub = fn[nab1];   // pointer to du(t-dt)/dt or du/dt(t+dt)
        
    // -- new way : *wdh* 2017/02/05
    // Index's into fn[.]
        nNew=nfni[1];
        nCur=nfni[0];
        nOld=nfni[-1];

        realCompositeGridFunction & fNew = fn[nNew];   // pointer to du/dt(t+dt)
        realCompositeGridFunction & fCur = fn[nCur];   // pointer to du/dt
        realCompositeGridFunction & fOld = fn[nOld];   // pointer to du/dt(t-dt)

    // *new* June 7, 2017 *wdh* 
        int nOld2=nOld, nOld3=nOld;  // not used for orderOfTimeAccuracy==2 
        if( orderOfTimeAccuracy>2 )
        {
      // Fouth-order in time requires two more past levels for the boundaryConditionPredictor
            nOld2 =nfni[-2];
            nOld3 =nfni[-3];
        }
        realCompositeGridFunction & fOld2 = fn[nOld2];   // pointer to du/dt(t-2*dt)
        realCompositeGridFunction & fOld3 = fn[nOld3];   // pointer to du/dt(t-3*dt)


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


        dtp[ndt0]=dt0;
        real dt1=dtp[(ndt0+1)%5];
        real dt2=dtp[(ndt0+2)%5];
        real dt3=dtp[(ndt0+3)%5];
        real dt4=dtp[(ndt0+4)%5];

        const real am1=.5*dt0;                  // for AM corrector
        const real am2=.5*dt0;

    // coefficients for 2nd order extrap:
        const real cex2a=1.+dt0/dtb;       // -> 2.
        const real cex2b=-dt0/dtb;         // -> -1.

    // coefficients for third order extrapolation (from ab.maple)
    //   These reduce to 3, -3, 1 for dt=constant
        const real cex30= (dt0+dt1+dt2)*(dt0+dt1)/(dt1+dt2)/dt1;
        const real cex31= -(dt0+dt1+dt2)/dt2*dt0/dt1;
        const real cex32= (dt0+dt1)*dt0/dt2/(dt1+dt2);

        for( int correction=0; correction<=numberOfCorrections; correction++ )
        {
            if( correction>1  && debug() & 4 )
                printF("ims: correction=%i\n",correction);

            parameters.dbase.get<int>("totalNumberOfPCcorrections")++;  // count the total number of corrections.
            
      // Predictor-Corrector. First time predict, subsequent times correct.
      //
      //  correction==0 :
      //       ---Adams-Bashforth Predictor
      //           u(*) <- u(t) + ab1*du/dt +ab2*du(t-dtb)/dt
      //  i.e.     gf[1]<- gf[mCur]+ ab1*fCur    +ab2*fOld
      //      
      //            The constants ab1 and ab2 are
      //                 ab1 = dt*( 1.+dt/(2*dtb) )   = (3/2)*dt if dtb=dt
      //                 ab2 = -dt*(  dt/(2*dtb) )    =-(1/2)*dt if dtb=dt
      //            Determined by extrapolation to time t+dt/2 from the
      //            times of fCur and fOld
      //
      // correction>0 :
      //       ---Adams Moulton Corrector
      //          u(t+dt) <- u(t) + dt* ( (1/2) du(*)/dt + (1/2) du(t)/dt )
      //          gf[mNew]  gf[mCur]              fNew             fCur

      // We only need to compute the "explicit" part of the implicit terms once for correction==0: 
      // These values are stored in utImplicit 
            implicitOption =correction==0 ? Parameters::computeImplicitTermsSeparately : 
                                                                            Parameters::doNotComputeImplicitTerms;

            if( correction==0 )
            {
	// ------------------------------------------------------
	// ----------------- Moving Grids -----------------------
	// ------------------------------------------------------

      	bool useNew=false;
      	bool gridWasAdapted=false;

                real tb=gf[mCur].t-dtb, tc=tb-dtb, td=tc-dtb; // tc,td not used
                if( movingGridProblem() )
      	{
                    assert( predictorOrder<=2 );
      	}
                const int numberOfPastTimes=0;
                const int numberOfPastTimeDerivatives=predictorOrder-1; 
        // Fill in exposed points on (tb,ub), ...
        // **FIX ME FOR HIGHER ORDER -- fill in exposed for more past time u's and f's
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

            const int maba = correction==0 ? mCur : mNew;
            const int naba = correction==0 ? nCur : nNew;


      // --- Compute: fn[nab0] <- du/dt(t0)  or fn[nab1] <- du/dt(t+dt0) ---

      // -- evaluate any body forcing (this is saved in realCompositeGridFunction bodyForce found in the data-base) ---
            const real tForce = gf[maba].t; // evaluate the body force at this time  ***CHECK ME**
            computeBodyForcing( gf[maba], tForce );

            for( grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
            {
        // if( false && TESTING )
        // {
        //   gf[maba].u[grid].updateGhostBoundaries(); // TRY THIS June 5, 2017 +TEMP+
        // }
                

      	rparam[0]=gf[maba].t;
      	rparam[1]=gf[maba].t;     // tforce
      	rparam[2]=gf[maba].t+dt0; // tImplicit
      	iparam[0]=grid;
      	iparam[1]=gf[maba].cg.refinementLevelNumber(grid);
      	iparam[2]=numberOfStepsTaken;

      	getUt(gf[maba].u[grid],gf[maba].getGridVelocity(grid),
            	      fn[naba][grid],iparam,rparam,uti[grid],&gf[mNew].cg[grid]);
            }

            addArtificialDissipation(gf[maba].u,dt0);	// add "implicit" dissipation to u 

            if( (TESTING && debug() & 4) || debug() & 64 ) // turned 16 -> 4  June 5, 2017 +TEMP+
            {
      	for( grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
      	{
        	  display(fCur[grid],"\n ****fCur: du/dt(t)",debugFile);
        	  if( correction==0 )
                	    display(fOld[grid],"\n ****fOld: du/dt(t-dt)",debugFile);
                    else
                	    display(fNew[grid],"\n ****fNew: du/dt(t+dt)",debugFile);
      	}
            }
            if( FALSE )
            {
                ::display(fn[naba][0],sPrintF(" du/dt t=%9.2e fn[naba=%i]",gf[maba].t,naba),"%5.2f ");
            }
            
            if( debug() & 16 )
            {
      	aString label;
                label = sPrintF(" ImplicitMS: errors in u at t=%e, correction=%i maba=%i gf[maba].t=%9.2e\n",
                  			t0,correction,maba,gf[maba].t);
      	determineErrors( gf[maba].u,gf[mCur].gridVelocity, t0, 0, error,label );

      	label = sPrintF(" ImplicitMS: errors in ut fCur at t=%e, correction=%i \n",t0,correction);
      	determineErrors( fCur,gf[mCur].gridVelocity, t0, 1, error,label );
      	if( correction==0 )
      	{
        	  real tub = t0-dtb;
        	  label = sPrintF(" ImplicitMS: errors in ut (fOld) at t=%e, correction=%i \n",tub,correction);
        	  determineErrors( fOld,gf[mNew].gridVelocity, tub, 1, error,label );
      	}
      	else
      	{
        	  real tub = t0+dt0;
        	  label = sPrintF(" ImplicitMS: errors in ut (fNew) at t=%e, correction=%i \n",tub,correction);
        	  determineErrors( fNew,gf[mNew].gridVelocity, tub, 1, error,label );
      	}
      	
            }

      //  --------------------------------------------------------
      //  --- Assign the explicit or implicit time-step update ---
      //  --------------------------------------------------------
            for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
            {

                const real dti = (1.-parameters.dbase.get<real >("implicitFactor"))*dt0;

      	if( correction==0 )
      	{ // u1 <- u0 + ab1*ua0 + ab2*ub0 [ + dti*utImplicit ]  (add last terms for implicit grids)
                  OV_GET_SERIAL_ARRAY(real,gf[mNew].u[grid],u1);
                  OV_GET_SERIAL_ARRAY(real,gf[mCur].u[grid],u0);
                  OV_GET_SERIAL_ARRAY(real,ua[grid],ua0);
                  OV_GET_SERIAL_ARRAY(real,ub[grid],ub0);
                  OV_GET_SERIAL_ARRAY(real,uti[grid],utImplicit);
                  OV_GET_SERIAL_ARRAY(int,gf[mNew].cg[grid].mask(),mask1);
        // #ifdef USE_PPP
        //   RealArray u1;  getLocalArrayWithGhostBoundaries(gf[mNew].u[grid],u1);
        //   RealArray u0;  getLocalArrayWithGhostBoundaries(gf[mCur].u[grid],u0);
        //   RealArray ua0; getLocalArrayWithGhostBoundaries(ua[grid],ua0);
        //   RealArray ub0; getLocalArrayWithGhostBoundaries(ub[grid],ub0);
        //   RealArray utImplicit; getLocalArrayWithGhostBoundaries(uti[grid],utImplicit);
        //   const intSerialArray & mask1 = gf[mNew].cg[grid].mask().getLocalArray();
        // #else
        //   RealDistributedArray & u1 = gf[mNew].u[grid];
        //   RealDistributedArray & u0 = gf[mCur].u[grid];
        //   RealDistributedArray & ua0 = ua[grid];
        //   RealDistributedArray & ub0 = ub[grid];
        //   RealDistributedArray & utImplicit = uti[grid];
        //   const intSerialArray & mask1 = gf[mNew].cg[grid].mask(); 
        // #endif
                    getIndex(gf[mNew].cg[grid].extendedIndexRange(),I1,I2,I3);
                    int n1a,n1b,n2a,n2b,n3a,n3b;
                    bool ok = ParallelUtility::getLocalArrayBounds(gf[mCur].u[grid],u0,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b);
                    if( !ok ) continue;
          // const intArray & mask1 = gf[mNew].cg[grid].mask();
                    int ierr=0;
                    const int maskOption=0; // assign pts where mask>0
                    int ipar[]={0,maskOption,n1a,n1b,n2a,n2b,n3a,n3b,N.getBase(),N.getBound()}; //
                    real rpar[15]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
                    real *ut1p, *ut2p, *ut3p, *ut4p;
                    if( implicitMethod==Parameters::crankNicolson )
                    {
            // --- IMEX: ab2+CN ---
                        if( parameters.getGridIsImplicit(grid) )
                            ipar[0]=3;  // add three extra "ut" terms if grid is advanced implicitly
                        else    
                            ipar[0]=2;  // add two extra "ut" terms
                        rpar[0]=ab1; rpar[1]=ab2; rpar[2]=dti;
                        ut1p=fCur[grid].getDataPointer();
                        ut2p=fOld[grid].getDataPointer();
                        ut3p=utImplicit.getDataPointer();
                        ut4p=ut3p;
                        updateOpt(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
                            	      u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
                            	      *mask1.getDataPointer(),  
                            	      *u0.getDataPointer(),*u1.getDataPointer(), 
                            	      *ut1p, *ut2p, *ut3p, *ut4p, 
                            	      ipar[0], rpar[0], ierr );
                    }
                    else if( implicitMethod==Parameters::implicitExplicitMultistep )
                    {
            // --------------
            // ---- IMEX ----
            // --------------
                        assert( parameters.getGridIsImplicit(grid) );  // fix me when some grids are explicit
                        if( orderOfTimeAccuracy==2 )
                        {
              // --- 2nd-order IMEX-BDF ----
                            OV_GET_SERIAL_ARRAY(real,gf[mOld].u[grid],uOld);
              // **fix me for variable dt**
                            if( fabs(dt0/dtb-1.) > 1.e-12 )
                            {
                      	printF("IMEX-BDF WARNING, dt has changed but formula assume constant dt, dt/dtOld=%8.2e\n",dt0/dtb);
                            }
                            if( correction==0 )
                            {
                      	if( debug() & 4 )
                        	  printF("IMEX BDF2 updateOptNew predictor mNew=%i mCur=%i mOld=%i...\n",mNew,mCur,mOld);
        	// --- PREDICTOR   (last term is already in the matrix)
        	//    u1 = (4/3)*uCur + (-1/3)*uOld + (4/3)*dt*utCur + (-2/3)*dt*utOld  [ + (2/3)*dt*utImplicit ]
        	// BDF weights for variable time-step
        	// **FINISH ME***
        	// -- ISSUES:
        	// -->   If dt changes then matrix coefficient will also change (ok for moving grids, sibnce matrix changes evry step)
        	// --> for non-moving grids we could alter BDF a bit to use (2/3)*dt*f_I^{n+1} + gamma*dt f_I^n 
                /* ---
                      const real dtRatio = dtb/dt0;
                      real alpha = -1./(dtRatio*(2.+dtRatio));  // -1/3 if dtRatio=1
                      real beta = (1.+dtRatio)/(2.+dtRatio);     // 2/3 if dtRatio=1
           //const real c1=4./3., c2=-1./3, c3=(4./3.)*dt0, c4=-(2./3.)*dt0;
                      real c1 = 1.-alpha, c2=alpha, c3=2.*beta*dt0, c4=-beta*dt0;
                      --- */      
                // This assumes a constant dt: 
                                const real c1=4./3., c2=-1./3, c3=(4./3.)*dt0, c4=-(2./3.)*dt0;
                      	rpar[0]=c1;
                      	rpar[1]=c2;
                      	rpar[2]=c3;
                      	rpar[3]=c4;
                      	int option=4;  // 4 terms on the RHS
                      	ipar[0]=option;
                              	real *puNew, *pu1, *pu2, *pu3, *pu4;
                                puNew=u1.getDataPointer();    // uNew
                      	pu1  =u0.getDataPointer();    // uCur
                      	pu2  =uOld.getDataPointer();  // uOld
                      	pu3  =fCur[grid].getDataPointer();   // fCur
                      	pu4  =fOld[grid].getDataPointer();   // fOld
                      	updateOptNew(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
                                 		     u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
                                 		     *mask1.getDataPointer(),  
                                 		     *puNew,
                                 		     *pu1,*pu2,*pu3,*pu4, *pu2,*pu2,*pu2,*pu2,*pu2,*pu2, // only first 4 arguments are used
                                 		     ipar[0], rpar[0], ierr );
                            }
                            else
                            {
                      	if( debug() & 4 )
                                    printF("IMEX BDF2 updateOptNew corrector mNew=%i mCur=%i mOld=%i...\n",mNew,mCur,mOld);
        	// ---- CORRECTOR   (last term is already in the matrix)
                // u1 = (4/3)*uCur + (-1/3)*uOld + (2/3)*dt*utNew   [ + (2/3)*dt*utImplicit ] 
                // This assumes a constant dt: 
                                const real c1=4./3., c2=-1./3, c3=(2./3.)*dt0;  
                      	rpar[0]=c1;
                      	rpar[1]=c2;
                      	rpar[2]=c3;
                      	int option=3;  // 3 terms on the RHS
                      	ipar[0]=option;
                      	real *puNew, *pu1, *pu2, *pu3, *pu4;
                                puNew=u1.getDataPointer();    // uNew
                      	pu1  =u0.getDataPointer();    // uCur
                      	pu2  =uOld.getDataPointer();  // uOld
                      	pu3  =fCur[grid].getDataPointer();   // fNew
        	// pu3  = fn[naba][grid].getDataPointer(); // fCur[grid].getDataPointer();   // fNew
                      	updateOptNew(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
                                 		     u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
                                 		     *mask1.getDataPointer(),  
                                 		     *puNew,
                                 		     *pu1,*pu2,*pu3,  *pu1, *pu1,*pu1,*pu1,*pu1,*pu1,*pu1, // only first 3 arguments are used
                                 		     ipar[0], rpar[0], ierr );
                            }
                        }
                        else if( orderOfTimeAccuracy==4 )
                        {
              // -----------------------------
              // ---- 4th-order IMEX-BDF -----
              // -----------------------------
                            OV_GET_SERIAL_ARRAY(real,gf[mOld].u[grid],uOld); // u(t-dt)
                            int mu;
                            mu = (mCur + 2 + numberOfGridFunctions) % numberOfGridFunctions;
                            OV_GET_SERIAL_ARRAY(real,gf[mu].u[grid],uOld2);  // u(t-2*dt)
                            mu = (mCur + 3 + numberOfGridFunctions) % numberOfGridFunctions;
                            OV_GET_SERIAL_ARRAY(real,gf[mu].u[grid],uOld3); // u(t-3*dt)
              // **fix me for variable dt**
                            if( fabs(dt0/dtb-1.) > 1.e-12 )
                            {
                      	printF("IMEX-BDF4 WARNING, dt has changed but formula assume constant dt, dt/dtOld=%8.2e\n",dt0/dtb);
                            }
                            if( correction==0 )
                            {
                      	if( debug() & 4 )
                        	  printF("IMEX BDF4 updateOptNew predictor mNew=%i mCur=%i mOld=%i, "
                             		 "numberOfGridFunctions=%i numberOfTimeDerivativeLevels=%i\n",
                             		 mNew,mCur,mOld,numberOfGridFunctions,numberOfTimeDerivativeLevels);
                                int nfeCur, nfeOld, nfeOld2, nfeOld3;
                                int mf;
                      	mf = (nab0 + 0 + numberOfTimeDerivativeLevels ) % numberOfTimeDerivativeLevels; 
                                nfeCur=mf;
                      	OV_GET_SERIAL_ARRAY(real,fn[mf][grid],feCur);  // F_E(t)
                      	mf = (nab0 + 1 + numberOfTimeDerivativeLevels ) % numberOfTimeDerivativeLevels;
                                nfeOld=mf;
                      	OV_GET_SERIAL_ARRAY(real,fn[mf][grid],feOld);  // F_E(t-dt)
                      	mf = (nab0 + 2 + numberOfTimeDerivativeLevels ) % numberOfTimeDerivativeLevels;
                                nfeOld2=mf;
                      	OV_GET_SERIAL_ARRAY(real,fn[mf][grid],feOld2);  // F_E(t-2*dt)
                      	mf = (nab0 + 3 + numberOfTimeDerivativeLevels ) % numberOfTimeDerivativeLevels;
                                nfeOld3=mf;
                      	OV_GET_SERIAL_ARRAY(real,fn[mf][grid],feOld3);  // F_E(t-3*dt)
                      	if( debug() & 4 )
                      	{
                        	  fPrintF(debugFile,"\n *********************** IMEX BDF PREDICTOR t=%9.3e *******************\n",gf[mCur].t);
                        	  ::display(u0,"u0=uCur",debugFile,"%6.3f ");
                        	  ::display(uOld,"uOld",debugFile,"%6.3f ");
                        	  ::display(uOld2,"uOld2",debugFile,"%6.3f ");
                        	  ::display(uOld3,"uOld3",debugFile,"%6.3f ");
                        	  ::display(feCur,sPrintF("feCur fn[nf=%i]",nfeCur),debugFile,"%6.3f ");
                        	  ::display(feOld,sPrintF("feOld fn[nf=%i]",nfeOld),debugFile,"%6.3f ");
                        	  ::display(feOld2,sPrintF("feOld2 fn[nf=%i]",nfeOld2),debugFile,"%6.3f ");
                        	  ::display(feOld3,sPrintF("feOld3 fn[nf=%i]",nfeOld3),debugFile,"%6.3f ");
                      	}
        	// --- PREDICTOR   (last term is already in the matrix)
                // (25/12)*u(n+1) = 4*U(n) -3*u(n-1) + (4/3)*u(n-2) - (1/4)*u(n-3) +
                //                  dt*( 4*fe(n) - 6*fe(n-1) + 4*fe(n-2) - fe(n-3) [+ fI(n+1)] )
                // This assumes a constant dt: 
                                const real c0=25./12., c1=4./c0, c2=-3./c0, c3=(4./3.)/c0, c4=-(1./4.)/c0;
                      	const real c5=(4./c0)*dt0, c6=(-6./c0)*dt0, c7 = (4./c0)*dt0, c8=(-1./c0)*dt0;
                      	rpar[0]=c1;
                      	rpar[1]=c2;
                      	rpar[2]=c3;
                      	rpar[3]=c4;
                      	rpar[4]=c5;
                      	rpar[5]=c6;
                      	rpar[6]=c7;
                      	rpar[7]=c8;
                      	int option=8;  // option = number of terms on the RHS
                      	ipar[0]=option;
                              	real *puNew, *pu1, *pu2, *pu3, *pu4, *pu5, *pu6, *pu7, *pu8;
                                puNew=u1.getDataPointer();    // uNew
                      	pu1  =u0.getDataPointer();    // uCur
                      	pu2  =uOld.getDataPointer();  // u(t-dt)
                      	pu3  =uOld2.getDataPointer(); // u(t-2*dt)
                      	pu4  =uOld3.getDataPointer(); // u(t-3*dt)
                      	pu5  =feCur.getDataPointer();  // fe(t)
                      	pu6  =feOld.getDataPointer();  // fe(t-dt)
                      	pu7  =feOld2.getDataPointer(); // fe(t-2*dt)
                      	pu8  =feOld3.getDataPointer(); // fe(t-3*dt)
                      	updateOptNew(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
                                 		     u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
                                 		     *mask1.getDataPointer(),  
                                 		     *puNew,
                                 		     *pu1,*pu2,*pu3,*pu4, *pu5,*pu6,*pu7,*pu8, *pu8,*pu8, // only first 8 arguments are used
                                 		     ipar[0], rpar[0], ierr );
                      	if( false )
                      	{
        	  // -- initial testing:
                        	  printF("\n___ IMEX-BDF4 : predictor after updateOptNew:\n");
                                    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                        	  const int & uc = parameters.dbase.get<int >("uc");
                        	  int i1=1,i2=1,i3=0;
                                    real x=0., y=0., z=0.;
                                    real ue[5], uet[5];
                        	  for( int k=0; k<5; k++ ){ ue[k]=e(x,y,x,uc,t0-k*dt0); uet[k]=e.t(x,y,x,uc,t0-k*dt0); }  // 
                        	  real ueNew = e(x,y,x,uc,t0+dt0);
                                    real uBDF = (12./25)*( 4.*ue[0]-3.*ue[1]+(4./3.)*ue[2]-(1./4.)*ue[3] )
                          	    +(12./25.)*dt*( 4.*uet[0]-6.*uet[1]+4.*uet[2]-1.*uet[3]);
                                    real *puv[4] ={pu1,pu2,pu3,pu4};
                        	  for( int k=0; k<4; k++ )
                        	  {
                                        realArray &ugf = gf[mgfi[-k]].u[grid];
                                        real uk = ugf(i1,i2,i3,uc);
                          	    printF(" t=%9.2e u =%12.5e ue =%12.5e err=%9.2e\n",t0-k*dt0,uk,ue[k],uk-ue[k]);
                                        real utk = fn[nfni[-k]][grid](i1,i2,i3,uc);
                          	    printF(" t=%9.2e ut=%12.5e uet=%12.5e err=%9.2e\n",t0-k*dt0,utk,uet[k],utk-uet[k]);
                        	  }
                                    printF(" uNew=%12.5e ue=%12.5e err=%9.2e\n",u1(i1,i2,i3,uc),ueNew,u1(i1,i2,i3,uc)-ueNew);
                                    printF(" uBDF=%12.5e ue=%12.5e err=%9.2e\n",uBDF,ueNew,uBDF-ueNew);
                      	}
                            }
                            else
                            {
                      	if( debug() & 4 )
                                    printF("IMEX BDF4 updateOptNew corrector mNew=%i mCur=%i mOld=%i...\n",mNew,mCur,mOld);
                                int mf;
                      	mf = (nab3 + numberOfTimeDerivativeLevels ) % numberOfTimeDerivativeLevels;       // **CHECK nab3 ***
                      	OV_GET_SERIAL_ARRAY(real,fn[mf][grid],feNew);  // F_E(t+dt) (from predictor)
        	// ---- CORRECTOR   (last term is already in the matrix)
                // (25/12)*u(n+1) = 4*U(n) -3*u(n-1) + (4/3)*u(n-2) - (1/4)*u(n-3) +
                //                  dt*( fe(predictor) [+ fI(n+1)] )
                // This assumes a constant dt: 
                                const real c0=25./12., c1=4./c0, c2=-3./c0, c3=(4./3.)/c0, c4=-(1./4.)/c0;
                      	const real c5=(1./c0)*dt0;
                      	rpar[0]=c1;
                      	rpar[1]=c2;
                      	rpar[2]=c3;
                      	rpar[3]=c4;
                      	rpar[4]=c5;
                      	int option=5;  // option = number of terms on the RHS
                      	ipar[0]=option;
                              	real *puNew, *pu1, *pu2, *pu3, *pu4, *pu5, *pu6, *pu7, *pu8;
                                puNew=u1.getDataPointer();    // uNew
                      	pu1  =u0.getDataPointer();    // uCur
                      	pu2  =uOld.getDataPointer();  // u(t-dt)
                      	pu3  =uOld2.getDataPointer(); // u(t-2*dt)
                      	pu4  =uOld3.getDataPointer(); // u(t-3*dt)
                      	pu5  =feNew.getDataPointer();  // fe(t+dt) from predictor 
                      	updateOptNew(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
                                 		     u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
                                 		     *mask1.getDataPointer(),  
                                 		     *puNew,
                                 		     *pu1,*pu2,*pu3,*pu4,*pu5, *pu5,*pu5,*pu5,*pu5,*pu5, // only first 5 arguments are used
                                 		     ipar[0], rpar[0], ierr );
                      	if( false )
                      	{
        	  // -- initial testing:
                        	  printF("\n___ IMEX-BDF4 corrector: after updateOptNew:\n");
                                    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                        	  const int & uc = parameters.dbase.get<int >("uc");
                        	  int i1=1,i2=1,i3=0;
                                    real x=0., y=0., z=0.;
                                    real ue[5], uet;
                        	  for( int k=0; k<5; k++ ){ ue[k]=e(x,y,x,uc,t0-k*dt0);  }  // 
                                    uet = e.t(x,y,x,uc,t0+dt0);
                        	  real ueNew = e(x,y,x,uc,t0+dt0);
                                    real uBDF = (12./25)*( 4.*ue[0]-3.*ue[1]+(4./3.)*ue[2]-(1./4.)*ue[3] )
                          	    +(12./25.)*dt*( uet );
                                    real *puv[4] ={pu1,pu2,pu3,pu4};
                        	  for( int k=0; k<4; k++ )
                        	  {
                                        realArray &ugf = gf[mgfi[-k]].u[grid];
                                        real uk = ugf(i1,i2,i3,uc);
                          	    printF(" t=%9.2e u=%12.5e ue=%12.5e err=%9.2e\n",t0-k*dt0,uk,ue[k],uk-ue[k]);
                        	  }
                        	  real utk = fn[nfni[1]][grid](i1,i2,i3,uc);
                        	  printF(" t=%9.2e ut=%12.5e uet=%12.5e err=%9.2e mf=%i nfni[1]=%i\n",t0+dt0,utk,uet,utk-uet,mf,nfni[1]);
                                    printF(" uNew=%12.5e ue=%12.5e err=%9.2e\n",u1(i1,i2,i3,uc),ueNew,u1(i1,i2,i3,uc)-ueNew);
                                    printF(" uBDF=%12.5e ue=%12.5e err=%9.2e\n",uBDF,ueNew,uBDF-ueNew);
                      	}
                            }
                        }
                        else
                        {
                            printF("IMEX-BDF:ERROR: unexpected orderOfTimeAccuracy=%i\n",orderOfTimeAccuracy);
                            OV_ABORT("ERROR unexpected orderOfTimeAccuracy");
                        }
                    }
                    else
                    {
                        OV_ABORT("unexpected implicitMethod");
                    }
      	}
      	else
      	{ // u1 <- u0 + am1*ub0 + am2*ua0 [ + dti*utImplicit ] (add last terms for implicit grids)
                  OV_GET_SERIAL_ARRAY(real,gf[mNew].u[grid],u1);
                  OV_GET_SERIAL_ARRAY(real,gf[mCur].u[grid],u0);
                  OV_GET_SERIAL_ARRAY(real,ua[grid],ua0);
                  OV_GET_SERIAL_ARRAY(real,ub[grid],ub0);
                  OV_GET_SERIAL_ARRAY(real,uti[grid],utImplicit);
                  OV_GET_SERIAL_ARRAY(int,gf[mNew].cg[grid].mask(),mask1);
        // #ifdef USE_PPP
        //   RealArray u1;  getLocalArrayWithGhostBoundaries(gf[mNew].u[grid],u1);
        //   RealArray u0;  getLocalArrayWithGhostBoundaries(gf[mCur].u[grid],u0);
        //   RealArray ua0; getLocalArrayWithGhostBoundaries(ua[grid],ua0);
        //   RealArray ub0; getLocalArrayWithGhostBoundaries(ub[grid],ub0);
        //   RealArray utImplicit; getLocalArrayWithGhostBoundaries(uti[grid],utImplicit);
        //   const intSerialArray & mask1 = gf[mNew].cg[grid].mask().getLocalArray();
        // #else
        //   RealDistributedArray & u1 = gf[mNew].u[grid];
        //   RealDistributedArray & u0 = gf[mCur].u[grid];
        //   RealDistributedArray & ua0 = ua[grid];
        //   RealDistributedArray & ub0 = ub[grid];
        //   RealDistributedArray & utImplicit = uti[grid];
        //   const intSerialArray & mask1 = gf[mNew].cg[grid].mask(); 
        // #endif
                    getIndex(gf[mNew].cg[grid].extendedIndexRange(),I1,I2,I3);
                    int n1a,n1b,n2a,n2b,n3a,n3b;
                    bool ok = ParallelUtility::getLocalArrayBounds(gf[mCur].u[grid],u0,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b);
                    if( !ok ) continue;
          // const intArray & mask1 = gf[mNew].cg[grid].mask();
                    int ierr=0;
                    const int maskOption=0; // assign pts where mask>0
                    int ipar[]={0,maskOption,n1a,n1b,n2a,n2b,n3a,n3b,N.getBase(),N.getBound()}; //
                    real rpar[15]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
                    real *ut1p, *ut2p, *ut3p, *ut4p;
                    if( implicitMethod==Parameters::crankNicolson )
                    {
            // --- IMEX: am2+CN ---
                        if( parameters.getGridIsImplicit(grid) )
                            ipar[0]=3;  // add three extra "ut" terms if grid is advanced implicitly
                        else    
                            ipar[0]=2;  // add two extra "ut" terms
                        rpar[0]=am1; rpar[1]=am2; rpar[2]=dti;
                        ut1p=fNew[grid].getDataPointer();
                        ut2p=fCur[grid].getDataPointer();
                        ut3p=utImplicit.getDataPointer();
                        ut4p=ut3p;
                        updateOpt(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
                            	      u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
                            	      *mask1.getDataPointer(),  
                            	      *u0.getDataPointer(),*u1.getDataPointer(), 
                            	      *ut1p, *ut2p, *ut3p, *ut4p, 
                            	      ipar[0], rpar[0], ierr );
                    }
                    else if( implicitMethod==Parameters::implicitExplicitMultistep )
                    {
            // --------------
            // ---- IMEX ----
            // --------------
                        assert( parameters.getGridIsImplicit(grid) );  // fix me when some grids are explicit
                        if( orderOfTimeAccuracy==2 )
                        {
              // --- 2nd-order IMEX-BDF ----
                            OV_GET_SERIAL_ARRAY(real,gf[mOld].u[grid],uOld);
              // **fix me for variable dt**
                            if( fabs(dt0/dtb-1.) > 1.e-12 )
                            {
                      	printF("IMEX-BDF WARNING, dt has changed but formula assume constant dt, dt/dtOld=%8.2e\n",dt0/dtb);
                            }
                            if( correction==0 )
                            {
                      	if( debug() & 4 )
                        	  printF("IMEX BDF2 updateOptNew predictor mNew=%i mCur=%i mOld=%i...\n",mNew,mCur,mOld);
        	// --- PREDICTOR   (last term is already in the matrix)
        	//    u1 = (4/3)*uCur + (-1/3)*uOld + (4/3)*dt*utCur + (-2/3)*dt*utOld  [ + (2/3)*dt*utImplicit ]
        	// BDF weights for variable time-step
        	// **FINISH ME***
        	// -- ISSUES:
        	// -->   If dt changes then matrix coefficient will also change (ok for moving grids, sibnce matrix changes evry step)
        	// --> for non-moving grids we could alter BDF a bit to use (2/3)*dt*f_I^{n+1} + gamma*dt f_I^n 
                /* ---
                      const real dtRatio = dtb/dt0;
                      real alpha = -1./(dtRatio*(2.+dtRatio));  // -1/3 if dtRatio=1
                      real beta = (1.+dtRatio)/(2.+dtRatio);     // 2/3 if dtRatio=1
           //const real c1=4./3., c2=-1./3, c3=(4./3.)*dt0, c4=-(2./3.)*dt0;
                      real c1 = 1.-alpha, c2=alpha, c3=2.*beta*dt0, c4=-beta*dt0;
                      --- */      
                // This assumes a constant dt: 
                                const real c1=4./3., c2=-1./3, c3=(4./3.)*dt0, c4=-(2./3.)*dt0;
                      	rpar[0]=c1;
                      	rpar[1]=c2;
                      	rpar[2]=c3;
                      	rpar[3]=c4;
                      	int option=4;  // 4 terms on the RHS
                      	ipar[0]=option;
                              	real *puNew, *pu1, *pu2, *pu3, *pu4;
                                puNew=u1.getDataPointer();    // uNew
                      	pu1  =u0.getDataPointer();    // uCur
                      	pu2  =uOld.getDataPointer();  // uOld
                      	pu3  =fNew[grid].getDataPointer();   // fCur
                      	pu4  =fCur[grid].getDataPointer();   // fOld
                      	updateOptNew(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
                                 		     u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
                                 		     *mask1.getDataPointer(),  
                                 		     *puNew,
                                 		     *pu1,*pu2,*pu3,*pu4, *pu2,*pu2,*pu2,*pu2,*pu2,*pu2, // only first 4 arguments are used
                                 		     ipar[0], rpar[0], ierr );
                            }
                            else
                            {
                      	if( debug() & 4 )
                                    printF("IMEX BDF2 updateOptNew corrector mNew=%i mCur=%i mOld=%i...\n",mNew,mCur,mOld);
        	// ---- CORRECTOR   (last term is already in the matrix)
                // u1 = (4/3)*uCur + (-1/3)*uOld + (2/3)*dt*utNew   [ + (2/3)*dt*utImplicit ] 
                // This assumes a constant dt: 
                                const real c1=4./3., c2=-1./3, c3=(2./3.)*dt0;  
                      	rpar[0]=c1;
                      	rpar[1]=c2;
                      	rpar[2]=c3;
                      	int option=3;  // 3 terms on the RHS
                      	ipar[0]=option;
                      	real *puNew, *pu1, *pu2, *pu3, *pu4;
                                puNew=u1.getDataPointer();    // uNew
                      	pu1  =u0.getDataPointer();    // uCur
                      	pu2  =uOld.getDataPointer();  // uOld
                      	pu3  =fNew[grid].getDataPointer();   // fNew
        	// pu3  = fn[naba][grid].getDataPointer(); // fNew[grid].getDataPointer();   // fNew
                      	updateOptNew(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
                                 		     u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
                                 		     *mask1.getDataPointer(),  
                                 		     *puNew,
                                 		     *pu1,*pu2,*pu3,  *pu1, *pu1,*pu1,*pu1,*pu1,*pu1,*pu1, // only first 3 arguments are used
                                 		     ipar[0], rpar[0], ierr );
                            }
                        }
                        else if( orderOfTimeAccuracy==4 )
                        {
              // -----------------------------
              // ---- 4th-order IMEX-BDF -----
              // -----------------------------
                            OV_GET_SERIAL_ARRAY(real,gf[mOld].u[grid],uOld); // u(t-dt)
                            int mu;
                            mu = (mCur + 2 + numberOfGridFunctions) % numberOfGridFunctions;
                            OV_GET_SERIAL_ARRAY(real,gf[mu].u[grid],uOld2);  // u(t-2*dt)
                            mu = (mCur + 3 + numberOfGridFunctions) % numberOfGridFunctions;
                            OV_GET_SERIAL_ARRAY(real,gf[mu].u[grid],uOld3); // u(t-3*dt)
              // **fix me for variable dt**
                            if( fabs(dt0/dtb-1.) > 1.e-12 )
                            {
                      	printF("IMEX-BDF4 WARNING, dt has changed but formula assume constant dt, dt/dtOld=%8.2e\n",dt0/dtb);
                            }
                            if( correction==0 )
                            {
                      	if( debug() & 4 )
                        	  printF("IMEX BDF4 updateOptNew predictor mNew=%i mCur=%i mOld=%i, "
                             		 "numberOfGridFunctions=%i numberOfTimeDerivativeLevels=%i\n",
                             		 mNew,mCur,mOld,numberOfGridFunctions,numberOfTimeDerivativeLevels);
                                int nfeCur, nfeOld, nfeOld2, nfeOld3;
                                int mf;
                      	mf = (nab0 + 0 + numberOfTimeDerivativeLevels ) % numberOfTimeDerivativeLevels; 
                                nfeCur=mf;
                      	OV_GET_SERIAL_ARRAY(real,fn[mf][grid],feCur);  // F_E(t)
                      	mf = (nab0 + 1 + numberOfTimeDerivativeLevels ) % numberOfTimeDerivativeLevels;
                                nfeOld=mf;
                      	OV_GET_SERIAL_ARRAY(real,fn[mf][grid],feOld);  // F_E(t-dt)
                      	mf = (nab0 + 2 + numberOfTimeDerivativeLevels ) % numberOfTimeDerivativeLevels;
                                nfeOld2=mf;
                      	OV_GET_SERIAL_ARRAY(real,fn[mf][grid],feOld2);  // F_E(t-2*dt)
                      	mf = (nab0 + 3 + numberOfTimeDerivativeLevels ) % numberOfTimeDerivativeLevels;
                                nfeOld3=mf;
                      	OV_GET_SERIAL_ARRAY(real,fn[mf][grid],feOld3);  // F_E(t-3*dt)
                      	if( debug() & 4 )
                      	{
                        	  fPrintF(debugFile,"\n *********************** IMEX BDF PREDICTOR t=%9.3e *******************\n",gf[mCur].t);
                        	  ::display(u0,"u0=uCur",debugFile,"%6.3f ");
                        	  ::display(uOld,"uOld",debugFile,"%6.3f ");
                        	  ::display(uOld2,"uOld2",debugFile,"%6.3f ");
                        	  ::display(uOld3,"uOld3",debugFile,"%6.3f ");
                        	  ::display(feCur,sPrintF("feCur fn[nf=%i]",nfeCur),debugFile,"%6.3f ");
                        	  ::display(feOld,sPrintF("feOld fn[nf=%i]",nfeOld),debugFile,"%6.3f ");
                        	  ::display(feOld2,sPrintF("feOld2 fn[nf=%i]",nfeOld2),debugFile,"%6.3f ");
                        	  ::display(feOld3,sPrintF("feOld3 fn[nf=%i]",nfeOld3),debugFile,"%6.3f ");
                      	}
        	// --- PREDICTOR   (last term is already in the matrix)
                // (25/12)*u(n+1) = 4*U(n) -3*u(n-1) + (4/3)*u(n-2) - (1/4)*u(n-3) +
                //                  dt*( 4*fe(n) - 6*fe(n-1) + 4*fe(n-2) - fe(n-3) [+ fI(n+1)] )
                // This assumes a constant dt: 
                                const real c0=25./12., c1=4./c0, c2=-3./c0, c3=(4./3.)/c0, c4=-(1./4.)/c0;
                      	const real c5=(4./c0)*dt0, c6=(-6./c0)*dt0, c7 = (4./c0)*dt0, c8=(-1./c0)*dt0;
                      	rpar[0]=c1;
                      	rpar[1]=c2;
                      	rpar[2]=c3;
                      	rpar[3]=c4;
                      	rpar[4]=c5;
                      	rpar[5]=c6;
                      	rpar[6]=c7;
                      	rpar[7]=c8;
                      	int option=8;  // option = number of terms on the RHS
                      	ipar[0]=option;
                              	real *puNew, *pu1, *pu2, *pu3, *pu4, *pu5, *pu6, *pu7, *pu8;
                                puNew=u1.getDataPointer();    // uNew
                      	pu1  =u0.getDataPointer();    // uCur
                      	pu2  =uOld.getDataPointer();  // u(t-dt)
                      	pu3  =uOld2.getDataPointer(); // u(t-2*dt)
                      	pu4  =uOld3.getDataPointer(); // u(t-3*dt)
                      	pu5  =feCur.getDataPointer();  // fe(t)
                      	pu6  =feOld.getDataPointer();  // fe(t-dt)
                      	pu7  =feOld2.getDataPointer(); // fe(t-2*dt)
                      	pu8  =feOld3.getDataPointer(); // fe(t-3*dt)
                      	updateOptNew(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
                                 		     u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
                                 		     *mask1.getDataPointer(),  
                                 		     *puNew,
                                 		     *pu1,*pu2,*pu3,*pu4, *pu5,*pu6,*pu7,*pu8, *pu8,*pu8, // only first 8 arguments are used
                                 		     ipar[0], rpar[0], ierr );
                      	if( false )
                      	{
        	  // -- initial testing:
                        	  printF("\n___ IMEX-BDF4 : predictor after updateOptNew:\n");
                                    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                        	  const int & uc = parameters.dbase.get<int >("uc");
                        	  int i1=1,i2=1,i3=0;
                                    real x=0., y=0., z=0.;
                                    real ue[5], uet[5];
                        	  for( int k=0; k<5; k++ ){ ue[k]=e(x,y,x,uc,t0-k*dt0); uet[k]=e.t(x,y,x,uc,t0-k*dt0); }  // 
                        	  real ueNew = e(x,y,x,uc,t0+dt0);
                                    real uBDF = (12./25)*( 4.*ue[0]-3.*ue[1]+(4./3.)*ue[2]-(1./4.)*ue[3] )
                          	    +(12./25.)*dt*( 4.*uet[0]-6.*uet[1]+4.*uet[2]-1.*uet[3]);
                                    real *puv[4] ={pu1,pu2,pu3,pu4};
                        	  for( int k=0; k<4; k++ )
                        	  {
                                        realArray &ugf = gf[mgfi[-k]].u[grid];
                                        real uk = ugf(i1,i2,i3,uc);
                          	    printF(" t=%9.2e u =%12.5e ue =%12.5e err=%9.2e\n",t0-k*dt0,uk,ue[k],uk-ue[k]);
                                        real utk = fn[nfni[-k]][grid](i1,i2,i3,uc);
                          	    printF(" t=%9.2e ut=%12.5e uet=%12.5e err=%9.2e\n",t0-k*dt0,utk,uet[k],utk-uet[k]);
                        	  }
                                    printF(" uNew=%12.5e ue=%12.5e err=%9.2e\n",u1(i1,i2,i3,uc),ueNew,u1(i1,i2,i3,uc)-ueNew);
                                    printF(" uBDF=%12.5e ue=%12.5e err=%9.2e\n",uBDF,ueNew,uBDF-ueNew);
                      	}
                            }
                            else
                            {
                      	if( debug() & 4 )
                                    printF("IMEX BDF4 updateOptNew corrector mNew=%i mCur=%i mOld=%i...\n",mNew,mCur,mOld);
                                int mf;
                      	mf = (nab3 + numberOfTimeDerivativeLevels ) % numberOfTimeDerivativeLevels;       // **CHECK nab3 ***
                      	OV_GET_SERIAL_ARRAY(real,fn[mf][grid],feNew);  // F_E(t+dt) (from predictor)
        	// ---- CORRECTOR   (last term is already in the matrix)
                // (25/12)*u(n+1) = 4*U(n) -3*u(n-1) + (4/3)*u(n-2) - (1/4)*u(n-3) +
                //                  dt*( fe(predictor) [+ fI(n+1)] )
                // This assumes a constant dt: 
                                const real c0=25./12., c1=4./c0, c2=-3./c0, c3=(4./3.)/c0, c4=-(1./4.)/c0;
                      	const real c5=(1./c0)*dt0;
                      	rpar[0]=c1;
                      	rpar[1]=c2;
                      	rpar[2]=c3;
                      	rpar[3]=c4;
                      	rpar[4]=c5;
                      	int option=5;  // option = number of terms on the RHS
                      	ipar[0]=option;
                              	real *puNew, *pu1, *pu2, *pu3, *pu4, *pu5, *pu6, *pu7, *pu8;
                                puNew=u1.getDataPointer();    // uNew
                      	pu1  =u0.getDataPointer();    // uCur
                      	pu2  =uOld.getDataPointer();  // u(t-dt)
                      	pu3  =uOld2.getDataPointer(); // u(t-2*dt)
                      	pu4  =uOld3.getDataPointer(); // u(t-3*dt)
                      	pu5  =feNew.getDataPointer();  // fe(t+dt) from predictor 
                      	updateOptNew(u0.getBase(0),u0.getBound(0),u0.getBase(1),u0.getBound(1),
                                 		     u0.getBase(2),u0.getBound(2),u0.getBase(3),u0.getBound(3),
                                 		     *mask1.getDataPointer(),  
                                 		     *puNew,
                                 		     *pu1,*pu2,*pu3,*pu4,*pu5, *pu5,*pu5,*pu5,*pu5,*pu5, // only first 5 arguments are used
                                 		     ipar[0], rpar[0], ierr );
                      	if( false )
                      	{
        	  // -- initial testing:
                        	  printF("\n___ IMEX-BDF4 corrector: after updateOptNew:\n");
                                    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                        	  const int & uc = parameters.dbase.get<int >("uc");
                        	  int i1=1,i2=1,i3=0;
                                    real x=0., y=0., z=0.;
                                    real ue[5], uet;
                        	  for( int k=0; k<5; k++ ){ ue[k]=e(x,y,x,uc,t0-k*dt0);  }  // 
                                    uet = e.t(x,y,x,uc,t0+dt0);
                        	  real ueNew = e(x,y,x,uc,t0+dt0);
                                    real uBDF = (12./25)*( 4.*ue[0]-3.*ue[1]+(4./3.)*ue[2]-(1./4.)*ue[3] )
                          	    +(12./25.)*dt*( uet );
                                    real *puv[4] ={pu1,pu2,pu3,pu4};
                        	  for( int k=0; k<4; k++ )
                        	  {
                                        realArray &ugf = gf[mgfi[-k]].u[grid];
                                        real uk = ugf(i1,i2,i3,uc);
                          	    printF(" t=%9.2e u=%12.5e ue=%12.5e err=%9.2e\n",t0-k*dt0,uk,ue[k],uk-ue[k]);
                        	  }
                        	  real utk = fn[nfni[1]][grid](i1,i2,i3,uc);
                        	  printF(" t=%9.2e ut=%12.5e uet=%12.5e err=%9.2e mf=%i nfni[1]=%i\n",t0+dt0,utk,uet,utk-uet,mf,nfni[1]);
                                    printF(" uNew=%12.5e ue=%12.5e err=%9.2e\n",u1(i1,i2,i3,uc),ueNew,u1(i1,i2,i3,uc)-ueNew);
                                    printF(" uBDF=%12.5e ue=%12.5e err=%9.2e\n",uBDF,ueNew,uBDF-ueNew);
                      	}
                            }
                        }
                        else
                        {
                            printF("IMEX-BDF:ERROR: unexpected orderOfTimeAccuracy=%i\n",orderOfTimeAccuracy);
                            OV_ABORT("ERROR unexpected orderOfTimeAccuracy");
                        }
                    }
                    else
                    {
                        OV_ABORT("unexpected implicitMethod");
                    }
      	}
            }

            if( correction==0 )
            {
        // printF(" +++ ims: gf[mNew].t=%9.3e --> change to t0+dt0=%9.3e +++\n",gf[mNew].t,t0+dt0);
                gf[mNew].t=t0+dt0;  // gf[mNew] now lives at this time
            }
            

      // *** assign boundary conditions for the implicit method 
            applyBoundaryConditionsForImplicitTimeStepping( gf[mNew] ); // ***** gf[mNew].gridVelocity must be correct here
        
            if( debug() & 4 )
            {
      	aString label = sPrintF(" ***ImplicitMS: RHS Before implicitSolve t=%e, correction=%i\n",gf[mNew].t,correction);
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
//         // Optionally refactor the matrix : if parameters.dbase.get<int >("globalStepNumber") % refactorFrequency == 0 
// 	formMatrixForImplicitSolve(dt0,gf[mNew], gf[mCur] );
//       }
            

      // ------------------------------------
      // --- Solve the implicit equations ---
      // ------------------------------------
  
            implicitSolve( dt0,gf[mNew], gf[mCur] );  // gf[mNew]=RHS  gf[mCur]=used for initial guess and linearization

      // * ---
//       implicitSolve( dt0,gf[mNew], gfl );  // gf[mNew]=RHS  gf[mCur]=used for initial guess and linearization
//       gfl.u=gf[mNew].u;


            if( false )
            {
                realCompositeGridFunction diff;
      	diff=gf[mNew].u-gf[mCur].u;
                Range & Rt = parameters.dbase.get<Range >("Rt");
      	printF(" After implicit solve: max-diff(u-uGuess) =");
      	for( int n=Rt.getBase(); n<=Rt.getBound(); n++ )
      	{
                    int maskOption=0, extra=1;
        	  real maxDiff = maxNorm(diff, n, maskOption, extra );
                    real mxd = 0;
        	  for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
          	    mxd = max( mxd, max(fabs(gf[mNew].u[grid]-gf[mCur].u[grid])));
        	  printF(" n=%i : %8.2e (all=%8.2e), ",n,maxDiff,mxd);
      	}
      	printF("\n");
            }

            if( debug() & 4 )
            {
      	if( twilightZoneFlow() )
      	{
        	  gf[mNew].u.display(sPrintF("ImplicitMS: gf[mNew].u after implicitSolve but BEFORE BC's (t=%8.2e)",
                             				     gf[mNew].t),debugFile,"%8.5f ");
      	}
      	aString label = sPrintF(" ***ImplicitMS: after implicitSolve but BEFORE BC's, t=%e, correction=%i\n",gf[mNew].t,correction);
      	determineErrors( gf[mNew],label );
            }

            if( correction==0 )
            {
        // --- For fourth-order in space we need to extrapolate p in time at ghost points --
                if( true )
                {
          // *new* way June 7, 2017 -- extrapolate in time to higher order ---
          // int orderOfExtrapolation = orderOfTimeAccuracy==2 ? 3 : 4;
                    int orderOfExtrapolation = orderOfTimeAccuracy==2 ? 3 : 5;
                    boundaryConditionPredictor( predictPressure,adamsData,orderOfExtrapolation, 
                                                                            mNew,mCur,mOld,&fCur,&fOld,&fOld2,&fOld3 );
                }
                else
                {
                    const int orderOfExtrapolation = 3;
                    boundaryConditionPredictor( predictPressure,adamsData,orderOfExtrapolation, mNew,mCur,mOld,&fCur,&fOld );
                }
                
            }
            
            if( debug() & 64 ) 
            {
      	if( twilightZoneFlow() )
      	{
        	  gf[mNew].u.display(sPrintF("ImplicitMS: gf[mNew].u after implicitSolve and boundaryConditionPredictor but BEFORE BC's (t=%8.2e)",
                             				     gf[mNew].t),debugFile,"%8.5f ");
      	}
            }

      // apply explicit BC's  --- > really only have to apply to implicit grids I think?
            applyBoundaryConditions(gf[mNew]);   // ***** gf[mNew].gridVelocity must be correct here!


            updateStateVariables( gf[mNew],1 );  

            if( debug() & 4 )
            {
      	if( twilightZoneFlow() )
      	{
        	  gf[mNew].u.display(sPrintF("ImplicitMS: gf[mNew].u after implicitSolve and BC's (t=%8.2e)",
                             				     gf[mNew].t),debugFile,"%8.5f ");
      	}
      	aString label = sPrintF(" ***ImplicitMS: after implicitSolve and BC's, t=%e, correction=%i\n",gf[mNew].t,correction);
      	determineErrors( gf[mNew],label );
            }

      // extrapolate p in time as an initial guess for iterative solvers
            if( correction==0 )  // *new way* 2015/01/22
            { 
	// --- for some reason the implicit scheme always extrapolates p in time ---

      	assert( parameters.dbase.get<int>("movingBodyPressureBC")==0 ); // this case was treated below for testing -- maybe not used
      	
      	if( parameters.dbase.has_key("extrapolatePoissonSolveInTime") )
        	  parameters.dbase.get<bool>("predictedPressureNeeded")= parameters.dbase.get<bool>("extrapolatePoissonSolveInTime");
      	const int numberOfTimeLevels=3;
      	const int gfIndex[numberOfTimeLevels]={mNew,mCur,mOld}; // 
      	predictTimeIndependentVariables( numberOfTimeLevels,gfIndex );
            }

            const bool addedDampingSkip = (parameters.dbase.get<bool>("useAddedDampingAlgorithm") && 
                                                                          parameters.dbase.get<bool>("addedDampingProjectVelocity") );

      // --- For added-damping scheme we skip the last correction of the rigid body ---
      //   *wdh* June 9, 2016      
            bool includeExtraPressureSolve=false;
            if( !includeExtraPressureSolve && correction>1 && correction==numberOfCorrections && addedDampingSkip )
            {
                  	printF("--IMS: skip pressure solve and moving grid correction step for AMP: correction=%i, t=%9.3e\n",
                              correction,t0);
                  	break;  // break from corrections 
            }


      // e.g. for variable density, update p eqn here     
            bool updateSolutionDependentEquations = correction==0;  
            solveForTimeIndependentVariables( gf[mNew],updateSolutionDependentEquations ); 

            if( debug() & 8 )
            {
      	aString label =sPrintF(" ImplicitMS: Errors after pressure solve, t0+dt0: t0=%e, dt0=%e  \n",t0,dt0);
      	determineErrors( gf[mNew],label );
            }

      // -- Correct for forces on moving bodies if we have more corrections --

      // We could skip here -- but this is worse for sher block case at least
            if( includeExtraPressureSolve && correction>1 && correction==numberOfCorrections && addedDampingSkip )
            {
                  	printF("--IMS: skip moving grid correction step for AMP: correction=%i, t=%9.3e\n",
                              correction,t0);
      	break;  // break from corrections 
            }


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
            if( movingGridCorrectionsHaveConverged )
                break;

            
        } // end corrections
        
        
    // -----------------------------------------------
    // --- Shift cyclic indices for next sub-step ----
    // -----------------------------------------------
    // Shift cyclic indices for next sub-step
        nfni.shift();
        mgfi.shift();
        
        if( true )
        {
      // *new way Feb 9, 2017 
            mNew=mgfi[1];
            mCur=mgfi[0];
            mOld=mgfi[-1];
            
      // mab2=mNew;
            mab0=mCur;
      // mab1=mOld;
            
            nab0=nfni[ 0];
            nab1=nfni[-1];
            nab2=nfni[-2];
            nab3=nfni[-3];
        }
        else
        {
/* ----
      // *old way*

      // permute (mab0,mab1,mab2) 
            mab0 = (mab0-1 + numberOfGridFunctions) % numberOfGridFunctions;
            mab1 = (mab1-1 + numberOfGridFunctions) % numberOfGridFunctions;

      // mab2 is always 1 "ahead" of mab0 
            mab2 = (mab0-1 + numberOfGridFunctions) % numberOfGridFunctions;

      // mab2=mgfi[1];

      // XXX     ^ should that be a 2 ?? kkc 060301  -- this is ok *wdh* 071121
            mNew=mab2;
            mCur=mab0;
            mOld=mab1;

      // *** FIX ME ***
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
        --- */
        }
        
        ndt0=(ndt0-1 +5)%5;  // for dtp[]
//      // switch mab0 <-> mab1
//      mab0 = (mab0+1) % 2;
//      mab1 = (mab1+1) % 2;
            
        dtb=dt0;
        t0+=dt0;

        if( parameters.dbase.get<int >("globalStepNumber") % 10 == 0 )
        {
      // residual = u.t = fn[nab0]+uti
      // saveSequenceInfo(t0,evaluate(fn[nab0]+uti));   // 070704 *wdh* turn this off for now -- this is not correct in general 

      // new way:
            if( parameters.dbase.get<int>("useNewImplicitMethod")==1 )
            {
      	realCompositeGridFunction & residual = uti;  // save residual here -- check this 
                getResidual( t0,dt0,gf[mab0],residual );
      	saveSequenceInfo(t0,residual);
            }
            
        }
        
        output( gf[mab0],parameters.dbase.get<int >("globalStepNumber")+1 ); // output to files, user defined output

        const int zeroUnusedPointsAfterThisManySteps=20;
        if( (mst==numberOfSubSteps || (mst % zeroUnusedPointsAfterThisManySteps)==0) &&  // mst starts at 1
      	parameters.dbase.get<int >("extrapolateInterpolationNeighbours")==0 )
        {
      // *note* we cannot fixup unused if we extrapolate interp. neighbours since these values will be zeroed out!
      // (esp. important for viscoPlastic model -- linearized solution becomes corrupted)

            if( debug() & 2 ) printF(" ************** ims.bC fixupUnusedPoints ************\n");
            
      // zero out unused points to keep them from getting too big ** is this needed?? ****
            for( int m=0; m<=1; m++ )
            {
	// ** gf[m].u.zeroUnusedPoints(coeff);
      	fixupUnusedPoints(gf[m].u);
            }
        }
        
    } // end  substeps

  // update the current solution:  
    current = mab0;
    
  // tm(2)+=getCPU()-time0;
}


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{applyBoundaryConditionsForImplicitTimeStepping}} 
int DomainSolver::
applyBoundaryConditionsForImplicitTimeStepping(GridFunction & cgf )
// ======================================================================================
//  /Description:
//     On implicit grids, apply boundary conditions to the rhs side grid function used in the implicit solve;
//  on explicit grids apply the normal explicit boundary conditions. 
// /cgf (input) : use this grid function as the right-hand-side. cgf.t should be the time corresponding to the
//      next time step.
//\end{CompositeGridSolverInclude.tex}  
// ==========================================================================================
{
    for( int grid=0; grid<cgf.cg.numberOfComponentGrids(); grid++ )
    {
        if( true )
        {
      // ---- Revaluate any time-dependent boundary conditions ----
      // *wdh* 2014/06/26 

      // determine time dependent conditions:
            getTimeDependentBoundaryConditions( cgf.cg[grid],cgf.t,grid ); 

      // Variable boundary values:
            setVariableBoundaryValues( cgf.t,cgf,grid );

            if( parameters.thereAreTimeDependentUserBoundaryConditions(nullIndex,nullIndex,grid)>0 )
            {
	// there are user defined boundary conditions
      	userDefinedBoundaryValues( cgf.t,cgf,grid);
            }
        }
        


        if( parameters.getGridIsImplicit(grid) )
        {
            applyBoundaryConditionsForImplicitTimeStepping(cgf.u[grid],
                                         						     gf[current].u[grid],  // -- fix this -- should be uL
                                         						     cgf.getGridVelocity(grid),
                                         						     cgf.t,
                                         						     parameters.dbase.get<int >("scalarSystemForImplicitTimeStepping"),grid );
        }
        else
        { // apply explicit BC's **** could be trouble if these require interpolation points ??   **********
            applyBoundaryConditions(cgf.t,cgf.u[grid],cgf.getGridVelocity(grid),grid);
        }


    }
    
    return 0;
}


#include "Integrate.h"


bool DomainSolver::
isImplicitMatrixSingular( realCompositeGridFunction &uL )
{
    return false;
}

int  DomainSolver::
addConstraintEquation( Parameters &parameters, Oges& solver, 
                   		       realCompositeGridFunction &coeff, 
                   		       realCompositeGridFunction &ucur, 
                   		       realCompositeGridFunction &rhs, const int &numberOfComponents) 
{
    printF("DomainSolver::addConstraintEquation should never be called!");
    Overture::abort("error");
    return 0;
}

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{implicitSolve}} 
void DomainSolver::
formMatrixForImplicitSolve(const real & dt0,
                     			   GridFunction & cgf1,
                     			   GridFunction & cgf0 )
// ==========================================================================================
// /Description: This function was once part of implicitSolve.  It was
// broken out to allow the construction of the matrix independently of
// the actual solve.  Basically all the work is done to initialize the
// implicit time stepping.  The implicit method can be optionally used
// on only some grids. To implement this approach we simply create a
// sparse matrix that is just the identity matrix on grids that are
// advanced explicitly but equal to the standard implicit matrix on
// grids that are advance implicitly: 
//  \begin{verbatim} 
//  I - \nu \alpha \dt \Delta on implicit grids 
//  I on explicit grids 
//  \end{verbatim} 
// If the form of the boundary conditions for the different components of
// $\uv$ are the same then we can build a single scalar matrix that
// can be used to advance each component, one after the other. If the
// boundary conditions are not of the same form then we build a matrix
// for a system of equations for the velocity components $(u,v,w)$.
//
// Note that originally cgf1 from implicitSolve was used to get the time,
// grid, and operators.  We are now using whatever is passed in as "u" to
// this function.  The operators should be the same (?) and the time is
// used in the debug output.  What about the grid though? It can change 
// due to AMR (used with implicit?) as well as from the grid velocity.
// /dt0 (input) : time step used to build the implicit matrix.
// /cgf1 (input) : holds the RHS 
// /cgf0 (input) : holds the current state of the solution (used for linearization)
//\end{CompositeGridSolverInclude.tex}  
// ==========================================================================================
{
    printf("DomainSolver::formMatrixForImplicitSolve:ERROR: base class function called. This function should be over-ridden\n");
    Overture::abort("error");
}

int
DomainSolver::
setOgesBoundaryConditions( GridFunction &cgf, IntegerArray & boundaryConditions, RealArray &boundaryConditionData,
                                                      const int imp )
// ===================================================================================================================
// /Description:
//   Assign the boundaryCondition data for passing to Oges (predfined equations) when it builds the implicit system.
//
// This function is called by DomainSolver::formMatrixForImplicitSolve
// 
//  /cgf (input) : A grid function holding the current grid.
//  /boundaryConditions (output) : boundary conditions for Oges
//  /boundaryConditionData (output) : boundary condition data for Oges 
//  /imp (input) : the number of the implicit system being solved
// 
// ====================================================================================================================
{
    CompositeGrid & cg = cgf.cg;

    printf("DomainSolver::setOgesBoundaryConditions:ERROR: base class function called. This function should be over-ridden\n");
    Overture::abort("error");

    return 0;
}
