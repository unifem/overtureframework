// This file automatically generated from advanceSteps.bC with bpp.
// ==========================================================================================
//   This file contains functions that implement separate steps in an advance routine
//   These separate steps can be combined to form a time stepping algorithm such as 
//   a predictor corrector method.
//
// These functions should probably be virtual members of an Advance class so they can be 
// over-loaded? 
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


#include "Cgsm.h"
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
#include "SmParameters.h"
#include "AdvanceOptions.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) for( side=0; side<=1; side++ )



// ===================================================================================================================
/// \brief Initialize the time stepping (a time sub-step function). 
/// \details 
/// \param t0 (input) : current time
/// \param dt0 (input) : current time step
///
// ===================================================================================================================
int Cgsm::
initializeTimeStepping( real & t0, real & dt0 )
{
    if( debug() & 2 ) printF(" Cgsm::initializeTimeStepping, t=%9.3e, dt=%9.3e\n",t0,dt0);
        
    assert( current==0 );  // do this for now as a sanity check

  // For linear-elasticity this next method will compute the solution at t-dt
    updateForNewTimeStep( gf[current],dt0 );

  // apply BC's at t=0 (to get interface conditions correct, for example) *wdh* 081105 
    int option=0; // not used.
    int next=current;
    applyBoundaryConditions( option, dt0, next,current ); // apply BC to "next" (current=previous time step)

    outputResultsAfterEachTimeStep( current,t0,dt0,0 );

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
int Cgsm::
startTimeStep( real & t0, real & dt0, int & currentGF, int & nextGF, AdvanceOptions & advanceOptions )
{
    if( debug() & 2 ) printF("Cgsm:startTimeStep: t=%9.3e, dt=%9.3e\n",t0,dt0);

  // globalStepNumber++;

    currentGF=current;
    nextGF= (current+1) % numberOfTimeLevels;

    advanceOptions.numberOfCorrectorSteps=0;  // fix me 
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
///
// ===================================================================================================================
int Cgsm::
takeTimeStep( real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions )
{
    real time0=getCPU();

    if( debug() & 2 ) 
        printP("Cgsm:takeTimeStep: t=%9.3e, dt=%9.3e, correction=%i, current=%i\n",t0,dt0,correction,current);

    const SmParameters::TimeSteppingMethodSm & timeSteppingMethod = 
        parameters.dbase.get< SmParameters::TimeSteppingMethodSm>("timeSteppingMethodSm");
    RealArray & timing = parameters.dbase.get<RealArray >("timing");

    deltaT=dt0;
    
    const bool takeTimeStep =(advanceOptions.takeTimeStepOption==AdvanceOptions::takeStepAndApplyBoundaryConditions ||
                      			    advanceOptions.takeTimeStepOption==AdvanceOptions::takeStepButDoNotApplyBoundaryConditions);
    const bool applyBC = ( advanceOptions.takeTimeStepOption==AdvanceOptions::takeStepAndApplyBoundaryConditions ||
                   			 advanceOptions.takeTimeStepOption==AdvanceOptions::applyBoundaryConditionsOnly );

    if( timeSteppingMethod==SmParameters::defaultTimeStepping ||
            timeSteppingMethod==SmParameters::modifiedEquationTimeStepping)
    {
    // --- Space-time schemes ---

        if( correction==0 && takeTimeStep )
        {
            advance(  current,t0,dt0,&advanceOptions );
        }
        if( !takeTimeStep && applyBC )
        {
      // -- re-apply the boundary conditions --
            if( debug() & 2 ) 
                printP("Cgsm:takeTimeStep:apply BC's ONLY: t=%9.3e, dt=%9.3e, correction=%i, current=%i\n",
                              t0,dt0,correction,current);

            const real dt=dt0;

      // check these: 
            const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
            const int next = (current+1) % numberOfTimeLevels;

            if( cg.numberOfComponentGrids()>1 )
            {
                real timei=getCPU();
        // Note: interpolate performs a periodicUpdate and updateGhostBoundaries even if there is only one grid
                gf[next].u.interpolate();

                timing(parameters.dbase.get<int>("timeForInterpolate"))+=getCPU()-timei;
            }

            int option=0; // not used.
            applyBoundaryConditions( option, dt, next,current ); // apply BC to "next" (current=previous time step)
        }
    }
    else
    {
    // -- method of lines schemes --
        advanceMethodOfLines(  current,t0,dt0,correction,&advanceOptions );
    }

    
    timing(parameters.dbase.get<int>("totalTime"))+=getCPU()-time0;

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
int Cgsm::
endTimeStep( real & t0, real & dt0, AdvanceOptions & advanceOptions )
{
    if( debug() & 2 ) printF("Cgsm:endTimeStep: t0+dt0=%9.3e, dt=%9.3e, current=%i\n",t0+dt0,dt0,current);

    numberOfStepsTaken++;
    current= (current+1) % numberOfTimeLevels;

    outputResultsAfterEachTimeStep( current,t0,dt0,numberOfStepsTaken );
    
    return 0;
}




