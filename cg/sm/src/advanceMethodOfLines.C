// This file automatically generated from advanceMethodOfLines.bC with bpp.
#include "Cgsm.h"
#include "display.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "ParallelOverlappingGridInterpolator.h"
#include "SmParameters.h"
#include "Regrid.h"

// ======================================================================================================
//  This macro will update the grids and grid functions when using AMR
// ======================================================================================================

#define FN(m) fn[m+numberOfFunctions*(grid)]

// ===================================================================================================================
/// \brief Advance one time-step using the method of lines.
/// \param current (input) : index of the current solution 
/// \param t (input) : current time.
/// \param dt (input) : time step.
// ===================================================================================================================
void Cgsm::
advanceMethodOfLines( int current, real t, real dt )
{
    checkArrays("advanceMethodOfLines:start");
    int & globalStepNumber = parameters.dbase.get<int >("globalStepNumber");
    globalStepNumber++;

    if( debug() & 2 )
        printF("advanceMethodOfLines: t=%e current=%i, numberOfFunctions=%i, numberOfTimeLevels=%i\n",t,
         	   current,numberOfFunctions,numberOfTimeLevels);

    FILE *& debugFile  =parameters.dbase.get<FILE* >("debugFile");
    FILE *& logFile    =parameters.dbase.get<FILE* >("logFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");
    
    const int numberOfDimensions = cg.numberOfDimensions();
    const int numberOfComponentGrids = cg.numberOfComponentGrids();
    const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    const int & uc =  parameters.dbase.get<int >("uc");
    const int & vc =  parameters.dbase.get<int >("vc");
    const int & wc =  parameters.dbase.get<int >("wc");
    const int & rc =  parameters.dbase.get<int >("rc");
    const int & tc =  parameters.dbase.get<int >("tc");
    const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
    const int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");

    SmParameters::TimeSteppingMethodSm & timeSteppingMethodSm = 
                                                                      parameters.dbase.get<SmParameters::TimeSteppingMethodSm>("timeSteppingMethodSm");

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Range C=numberOfComponents;
    const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
    const int next = (current+1) % numberOfTimeLevels;

    int & debug = parameters.dbase.get<int >("debug");

    const bool isSecondOrderSystem = ((SmParameters&)parameters).isSecondOrderSystem();

  // ------ AMR -----
  // --- Start AMR ---
    const int regridFrequency = parameters.dbase.get<int >("amrRegridFrequency")>0 ? 
                                                            parameters.dbase.get<int >("amrRegridFrequency") :
                                                            parameters.dbase.get<Regrid* >("regrid")==NULL ? 2 : 
                                                            parameters.dbase.get<Regrid* >("regrid")->getRefinementRatio();
    if( parameters.isAdaptiveGridProblem() && ((globalStepNumber % regridFrequency) == 0) )
    {
    // ****************************************************************************
    // ****************** Adaptive Grid Step  *************************************
    // ****************************************************************************
        if( debug & 2 )
        {
            printP("***** advance: AMR regrid at step %i t=%e dt=%8.2e***** \n",globalStepNumber,t,dt);
            fPrintF(debugFile,"***** advance: AMR regrid at step %i t=%e dt=%8.2e***** \n",globalStepNumber,t,dt);
        }
        real timea=getCPU();
        if( debug & 4 )
            fPrintF(debugFile,"\n ***** advance: AMR regrid at step %i ***** \n\n",globalStepNumber);
        if( debug & 8 )
        {
            if( parameters.dbase.get<bool >("twilightZoneFlow") )
            {
                getErrors( current,t,dt,sPrintF(" advance: errors before regrid, t=%e \n",t) );
            }
            else
            {
                fPrintF(debugFile," ***advance: before regrid: solution ***\n");
                outputSolution( gf[current].u,t );
            }
        }
        int numberToUpdate=0; // we need to update ub to live on the new grid, and interpolate values.
        if( ((SmParameters&)parameters).isSecondOrderSystem() )
        {
            numberToUpdate=1;  // also update and interpolate prev solution to the new grid 
        }
        adaptGrids( gf[current], numberToUpdate,&(gf[prev].u), NULL );  // last arg is for work-space **fix me **
    // *wdh* do this: 090315
        cg.reference(gf[current].cg);
              gf[prev].cg.reference(gf[current].cg);
              gf[next].cg.reference(gf[current].cg);
              gf[next].u.updateToMatchGrid(cg);
    // printF(" After adaptGrids: prev=%i, current=%i, next=%i\n",prev,current,next);
        for( int n=0; n<numberOfTimeLevels; n++ )
        {
            if( n!=current )
            {
          	gf[n].cg.reference(gf[current].cg);  //
          	if( n!=prev ) // this was already done for prev in adaptGrids
            	  gf[n].u.updateToMatchGrid(gf[current].cg);
            }
            gf[n].u.setOperators(*cgop);
      // printF(" After adaptGrids: gf[%i].cg.numberOfComponentGrids = %i\n",n,gf[n].cg.numberOfComponentGrids());
      // printF(" After adaptGrids: gf[%i].u.getCompositeGrid()->numberOfComponentGrids = %i\n",n,gf[n].u.getCompositeGrid()->numberOfComponentGrids());
        }
    // ** do this for now ** fix me **
        if( checkErrors )
        {
            assert( cgerrp!=NULL );
            (*cgerrp).updateToMatchGrid(cg);
        }
    // the next has been moved into adaptGrids 070706
    //     real time1=getCPU();
    //     cgf1.cg.rcData->interpolant->updateToMatchGrid( cgf1.cg ); 
    //     parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateInterpolant"))+=getCPU()-time1;
        real time1=getCPU();
        if( debug & 8 )
        {
            outputSolution( gf[current].u,t,
                                          sPrintF(" advance:after adaptGrids, before interpAndApplyBC at t=%11.4e \n",t) );
        }
        interpolateAndApplyBoundaryConditions( gf[current] );
    // *wdh* 090829
        if( numberToUpdate==1 )
        {
            interpolateAndApplyBoundaryConditions( gf[prev] );
        }
        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAmrBoundaryConditions"))+=getCPU()-time1;    
        if( debug & 4 )
        {
            if( parameters.dbase.get<bool >("twilightZoneFlow") )
            {
                getErrors( prev   ,t-dt,dt,sPrintF(" advance: errors in prev    after regrid, t=%e \n",t-dt) );
                getErrors( current,t   ,dt,sPrintF(" advance: errors in current after regrid, t=%e \n",t) );
            }
            else
            {
                fPrintF(debugFile," ***after regrid: solution ***\n");
                outputSolution( gf[current].u,t );
            }
        }
        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAmrRegrid"))+=getCPU()-timea;
    }
  // --- End AMR ---

  // ========== Forward-Euler or Improved Euler: =============

    if( timeSteppingMethodSm==SmParameters::forwardEuler ||
            timeSteppingMethodSm==SmParameters::improvedEuler )
    {
        
    // ---- predictor:  ----
        getUt( gf[current], t, fn[0], t );

        if( debug & 8 )
        {
            fn[0].display(sPrintF("MOL: predictor: u.t or u.tt at t=%9.3e, dt=%8.2e\n",t,dt),debugFile);
        }
    
        if( isSecondOrderSystem )
        { // fn[0] holds u.tt in this case 
            gf[next].u = 2.*gf[current].u -gf[prev].u + (dt*dt)*fn[0];
        }
        else
        {  // fn[0] holds u.t in this case 
            gf[next].u = gf[current].u + (dt)*fn[0];
        }
        gf[next].t=t+dt;
    

    // ............. Boundary Conditions ..............
        int option=0; // not used.
        applyBoundaryConditions( option, dt, next,current ); // apply BC to "next" (current=previous time step)

    // ---- corrector:  ----
        if( timeSteppingMethodSm==SmParameters::improvedEuler && !isSecondOrderSystem )
        {
            getUt( gf[next], t+dt, fn[1],t+dt );

            if( debug & 8 )
            {
      	fn[1].display(sPrintF("MOL: corrector: u.t or u.tt at t=%9.3e\n",t),debugFile);
            }

            gf[next].u = gf[current].u + (.5*dt)*(fn[0]+fn[1]);

            applyBoundaryConditions( option, dt, next,current );
        }
    }
    else
    {
        printF("Cgsm::advanceMethodOfLines:ERROR: un-implemented time-stepping method : timeSteppingMethodSm=%i\n",(int)timeSteppingMethodSm);
        Overture::abort("error");
    }
    
    checkArrays("advanceMethodOfLines:end");
    
}

// =================================================================================================
/// \brief: compute u.t or u.tt for a method of lines time-stepper
// =================================================================================================
void Cgsm::
getUt( GridFunction & cgf, 
              const real & t, 
              RealCompositeGridFunction & ut, 
              real tForce )
{
    real & dt= deltaT;
    
  // --- finish me ---
    if( ((SmParameters&)parameters).isSecondOrderSystem() )
    {
    // advance the solution as a second-order system (do not apply BCs)
        getUtSOS( cgf,t,ut,tForce );
    }
    else
    {
    // advance the solution as a first order system
        getUtFOS( cgf,t,ut,tForce );

    }

}
