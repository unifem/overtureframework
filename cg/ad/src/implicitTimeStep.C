// This file automatically generated from implicitTimeStep.bC with bpp.
#include "DomainSolver.h"
#include "Cgad.h"
#include "CompositeGridOperators.h"
// #include "GridCollectionOperators.h"
// #include "interpPoints.h"
// #include "SparseRep.h"
// #include "ExposedPoints.h"
// #include "Ogen.h"
// #include "App.h"
// #include "ParallelUtility.h"
// #include "Oges.h"
// #include "OgesParameters.h"
// #include "AdamsPCData.h"
// #include "gridFunctionNorms.h"
// #include "updateOpt.h"
#include "AdvanceOptions.h"

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
///  \METHOD (input) : name of the method: adamsPC or implicitPC
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



// ===================================================================================================================
/// \brief Advance the time dependent variables one time step for the BDF scheme.
/// \details This routine is called by the takeTimeStep routine which handles details of moving
///     and adaptive grids.
/// 
/// \param t0 (input) : current time
/// \param dt0 (input) : current time step
///
// ===================================================================================================================
int Cgad::
implicitTimeStep(  real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions )
{
      
    if( pdeName=="advection diffusion" )
    {
    // call base class version 
        DomainSolver::implicitTimeStep(  t0,dt0, correction, advanceOptions );
    }
    else if( pdeName=="thinFilmEquations" )
    {
    // --- solve the thin film equations ---
        thinFilmSolver(  t0,dt0, correction, advanceOptions ); 

    }
    else
    {
        OV_ABORT("--AD-- implicitTimeStep: ERROR: unknown pdeName!");
    }
    
    

    return 0;
}

