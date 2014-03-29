// This file automatically generated from advanceFactored.bC with bpp.
#include "DomainSolver.h"
#include "ApproximateFactorization.h"
#include "App.h"
#include "gridFunctionNorms.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "TridiagonalSolver.h"
#include "PlotIt.h"
#include "AdvanceOptions.h"
#include "AdamsPCData.h"
#include "InterpolationData.h"
#include "ExposedPoints.h"
#include "InterpolateRefinements.h"
#include "kkcdefs.h"


#define EXTRAP_2(UP,I1,I2,I3,II,C) 2*A_4D(UP,I1+II[0],I2+II[1],I3+II[2],C)-A_4D(UP,I1+2*II[0],I2+2*II[1],I3+2*II[2],C)
#define EXTRAP_3(UP,I1,I2,I3,II,C) 3*A_4D(UP,I1+II[0],I2+II[1],I3+II[2],C)-3*A_4D(UP,I1+2*II[0],I2+2*II[1],I3+2*II[2],C)+A_4D(UP,I1+3*II[0],I2+3*II[1],I3+3*II[2],C)
#define EXTRAP_4(UP,I1,I2,I3,II,C) 4*A_4D(UP,I1+II[0],I2+II[1],I3+II[2],C)-6*A_4D(UP,I1+2*II[0],I2+2*II[1],I3+2*II[2],C)+4*A_4D(UP,I1+3*II[0],I2+3*II[1],I3+3*II[2],C) - A_4D(UP,I1+4*II[0],I2+4*II[1],I3+4*II[2],C)
#define EXTRAP_5(UP,I1,I2,I3,II,C) 5*A_4D(UP,I1+II[0],I2+II[1],I3+II[2],C)-10*A_4D(UP,I1+2*II[0],I2+2*II[1],I3+2*II[2],C)+10*A_4D(UP,I1+3*II[0],I2+3*II[1],I3+3*II[2],C) - 5*A_4D(UP,I1+4*II[0],I2+4*II[1],I3+4*II[2],C)+A_4D(UP,I1+5*II[0],I2+5*II[1],I3+5*II[2],C)
#define EXTRAP_6(UP,I1,I2,I3,II,C) 6*A_4D(UP,I1+II[0],I2+II[1],I3+II[2],C)-15*A_4D(UP,I1+2*II[0],I2+2*II[1],I3+2*II[2],C)+20*A_4D(UP,I1+3*II[0],I2+3*II[1],I3+3*II[2],C) - 15*A_4D(UP,I1+4*II[0],I2+4*II[1],I3+4*II[2],C)+6*A_4D(UP,I1+5*II[0],I2+5*II[1],I3+5*II[2],C)-A_4D(UP,I1+6*II[0],I2+6*II[1],I3+6*II[2],C)

const int CG_ApproximateFactorization::parallelBC = -31415;

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
//  MACRO:  Perform the initialization step for the PC method
//
//  /METHOD (input) : name of the method: adamsPC or implicitPC
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

#ifdef USE_PPP

#define GET_LOCAL_INTERPOLATION_POINTS(CG,GRID,NPTS,INTERPOLATION_POINTS) \                                                     
int NPTS = 0;intSerialArray INTERPOLATION_POINTS; if( ( grid<CG.numberOfComponentGrids() && CG->localInterpolationDataState==CompositeGridData::localInterpolationDataForAMR ) || CG->localInterpolationDataState==CompositeGridData::noLocalInterpolationData )       { if ( !parameters.dbase.has_key("AF_LocalInterpolationData") )	  {	    parameters.dbase.put<InterpolationData *>("AF_LocalInterpolationData");	    parameters.dbase.get<InterpolationData *>("AF_LocalInterpolationData") = NULL;	    ParallelGridUtility::getLocalInterpolationData( cg, parameters.dbase.get<InterpolationData *>("AF_LocalInterpolationData") );	  }	InterpolationData & ipd = parameters.dbase.get<InterpolationData *>("AF_LocalInterpolationData")[grid];	NPTS = ipd.numberOfInterpolationPoints;	INTERPOLATION_POINTS.reference(ipd.interpolationPoint);} else  					\ 
        { INTERPOLATION_POINTS.reference( CG->interpolationPointLocal[GRID] ); NPTS = CG.numberOfComponentGrids()>1 ? CG->numberOfInterpolationPointsLocal(GRID) : 0; } 
    
#define OV_BARRIER MPI_Barrier(Overture::OV_COMM)

#else
#define GET_LOCAL_INTERPOLATION_POINTS(CG,GRID,NPTS,INTERPOLATION_POINTS) const int NPTS = CG.numberOfInterpolationPoints(GRID);		  IntegerArray & INTERPOLATION_POINTS = CG.interpolationPoint[GRID];

#define OV_BARRIER

#endif

namespace {

    int parallel_array_bounds_option = 1;
}

using namespace CG_ApproximateFactorization;
// ===================================================================================================================
/// \brief Initialize the time stepping (a time sub-step function). 
/// \details 
/// \param t0 (input) : current time
/// \param dt0 (input) : current time step
///
// ===================================================================================================================
int DomainSolver::
initializeTimeSteppingAF( real & t0, real & dt0 )
{
  // where else could this be done, eh?
    assert(parameters.registerBC(CG_ApproximateFactorization::parallelBC,"AFParallelBC"));

    int init=true;
    FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

    assert(parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::approximateFactorization);
    parameters.dbase.get<Parameters::ImplicitOption >("implicitOption")=Parameters::doNotComputeImplicitTerms;

    if( debug() & 4 )
        printF(" ====== DomainSolver::initializeTimeSteppingAF ======\n");
    if( debug() & 2 )
        fprintf(debugFile," *** DomainSolver::initializeTimeSteppingAF: t0=%e, dt0=%e *** \n",t0,dt0);

  // the following stuff was copied from advanceStepsIM.bC , do we really need the initializePredictorCorrector macro??
    RealCompositeGridFunction & uti = fn[2]; 
    int numberOfExtraPressureTimeLevels=0; // is suppose we could use this instead of a whole extra grid function?
  //  initializePredictorCorrector(implicitAF,uti);
    if ( !factors.size() ) 
        initializeFactorization();
    assert(factors.size());

    return 0;
}

// ===================================================================================================================
/// \brief Start an individual time step (a time sub-step function) for an approximate factorization method.
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
startTimeStepAF( real & t0, real & dt0, int & currentGF, int & nextGF, AdvanceOptions & advanceOptions )
{

    if( parameters.dbase.get<int >("globalStepNumber")<0 )
        {
            if( movingGridProblem() )
      	{ 
          // printF(">>>>>>>>> startTimeStepAF t=%9.3e, dt0=%9.3e\n",t0,dt0);
        	  
        	  int iuOld =(current+2)%3;
        	  int iuNew =(current+1)%3;
        	  int mCur = current, mNew=iuNew, mOld=iuOld;
        	  getGridVelocity( gf[current],t0 );
        	  moveGrids( t0,t0,t0-dt0,-dt0,gf[mCur],gf[mCur],gf[mOld] );  

                	  	  getGridVelocity( gf[mOld],t0-dt0); // done in moveGrids
	  //      OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
      	}

            parameters.dbase.get<int >("globalStepNumber")=0;
            int oldGF = (current+2)%3;
            gf[oldGF].t = t0-dt0;
            Parameters::InitialConditionOption & initialConditionOption = parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption");
            if ( false &&  // *wdh* 2013/10/02
                      initialConditionOption!=Parameters::twilightZoneFunctionInitialCondition )
      	{
        	  gf[oldGF].u.dataCopy(gf[current].u);
      	}
            else
      	{
        	  assignInitialConditions(oldGF);
        	  for ( int grid=0; grid<gf[oldGF].cg.numberOfGrids(); grid++ )
          	    gf[oldGF].u[grid].updateGhostBoundaries();
      	}

            if( !parameters.dbase.get<DataBase >("modelData").has_key("AdamsPCData") )
      	{
        	  parameters.dbase.get<DataBase >("modelData").put<AdamsPCData>("AdamsPCData");
        	  parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData").dtb = dt0;
      	}
            AdamsPCData & adamsData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData");  
            adamsData.dtb = dt0;
        }
    parameters.dbase.get<int >("globalStepNumber")++;

    currentGF=current;
    nextGF=(current+1)%3;

    parameters.dbase.get<int>("numberOfPCcorrections") = 0;
    advanceOptions.numberOfCorrectorSteps=0;//parameters.dbase.get<int>("numberOfPCcorrections"); 
    advanceOptions.gridChanges=AdvanceOptions::noChangeToGrid;  // fix me for AMR

    return 0;

}

// ===================================================================================================================
/// \brief Take a time step using the approximate factored scheme.
/// \details kkc 091215 : initial version.
/// \param t0 (input) : current time
/// \param dt0 (input) : current time step
/// \param correction (input) : for predictor corrector methods this indicates the correction step number.
/// \param advanceOptions (input) : additional options that adjust the behaviour of this function.
///       advanceOptions.takeTimeStepOption can be used to not apply or only apply the boundary conditions.
///
// ===================================================================================================================
int DomainSolver::
takeTimeStepAF(real &t0, real &dt0, int correction, AdvanceOptions & advanceOptions )
{
    FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");


    assert(parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::approximateFactorization);
    assert(factors.size());

    ArraySimpleFixed<real,20,1,1,1> rparam;
    ArraySimpleFixed<int,20,1,1,1> iparam;

    ListOfShowFileParameters &pPar = parameters.dbase.get<ListOfShowFileParameters >("pdeParameters");

    int iuOld =(current+2)%3;
    int iuNew =(current+1)%3;
    GridFunction &uOld = gf[iuOld];
    GridFunction &uCur = gf[current];
    GridFunction &uNew = gf[iuNew];

    int afParallelGhostWidth = parameters.dbase.get<int>("AFparallelGhostWidth");

  // the following 2nd order variable dt adams stuff was copied from advancePC.bC
    if( !parameters.dbase.get<DataBase >("modelData").has_key("AdamsPCData") )
        {
            parameters.dbase.get<DataBase >("modelData").put<AdamsPCData>("AdamsPCData");
            parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData").dtb = dt0;
        }

    AdamsPCData & adamsData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData");  
    real & dtb=adamsData.dtb;
    real ab1 = dt0*(1.+dt0/(2.*dtb));
    real ab2 = -dt0*dt0/(2.*dtb);

  // // // MOVING GRID INITIALIZATION
  // moving grid initialization for the time step, move the grid and fill in exposed points
  // most of the code is similar to moveTheGridsMacro in pcMacros.h but adjusted for the afs time step
    if ( movingGridProblem() )
        {
            int mCur = current, mNew=iuNew, mOld=iuOld;
            checkArrays(" AFS : before move grids");
            if (debug() & 8) printf(" AFS : before moveTheGrids : t0=%9.3e, gf[mNew].t=%9.3e, gf[mNew].gridVelocityTime=%9.3e\n",
         	   t0,gf[mNew].t,gf[mNew].gridVelocityTime);

            if( debug() & 16 )
      	{
        	  if( twilightZoneFlow() )
          	    {
            	      fprintf(debugFile,"\n ---> AFS : Errors in u before moveGrids t=%e  \n",gf[mCur].t);
            	      determineErrors( gf[mCur] );
          	    }
      	}

       // generate gf[mNew] from gf[mCur] (compute grid velocity on gf[mCur] and gf[mNew]
            moveGrids(t0,t0,t0+dt0,dt0,gf[mCur],gf[mCur],gf[mNew]);
            
            checkArrays(" AFS : after move grids");

            if( debug() & 16 )
      	{
        	  if( twilightZoneFlow() )
          	    {
            	      fprintf(debugFile,"\n ---> AFS : Errors in u after moveGrids t=%e  \n",gf[mCur].t);
            	      determineErrors( gf[mCur] );
          	    }
      	}
            real cpu0=getCPU();
            gf[mNew].cg.rcData->interpolant->updateToMatchGrid( gf[mNew].cg );  
            parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateInterpolant"))+=getCPU()-cpu0;

            cpu0=getCPU();
            gf[mNew].u.getOperators()->updateToMatchGrid(gf[mNew].cg); 
            parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateOperators"))+=getCPU()-cpu0;

            if( debug() & 4 ) printf("AFS : step: update gf[mNew] for moving grids, gf[mNew].t=%9.3e,...\n",gf[mNew].t);

            if( debug() & 16 )
      	{
        	  if( twilightZoneFlow() )
          	    {
            	      fprintf(debugFile,"\n ---> AFS: Errors in u before updateForMovingGrids t=%e  \n",gf[mCur].t);
            	      fprintf(debugFile,"*** mCur=%i mNew=%i\n",mCur,mNew);
        	  
            	      determineErrors( gf[mCur] );
          	    }
      	}

            updateForMovingGrids(gf[mNew]);
      // ****      gf[mNew].u.updateToMatchGrid( gf[mNew].cg );  

            if( debug() & 16 )
      	{
        	  if( twilightZoneFlow() )
          	    {
            	      fprintf(debugFile,"\n ---> AFS: Errors in u after updateForMovingGrids t=%e  \n",gf[mCur].t);
            	      determineErrors( gf[mCur] );
          	    }
      	}

            ExposedPoints::debug=2;
            ExposedPoints exposedPoints;
            exposedPoints.setFillExposedInterpolationPoints(true);
            exposedPoints.setAssumeInterpolationNeighboursAreAssigned(parameters.dbase.get<int >("extrapolateInterpolationNeighbours"));
      // interpolate exposed points on mCur
            exposedPoints.initialize(gf[mCur].cg,gf[mNew].cg,parameters.dbase.get<int >("stencilWidthForExposedPoints"));
            exposedPoints.interpolate(gf[mCur].u,(twilightZoneFlow() ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),t0);
            if( debug() & 16 )
      	{
        	  if( twilightZoneFlow() )
          	    {
            	      fprintf(debugFile,"\n ---> AFS: Errors in uCur AFTER interp exposed t=%e  \n",gf[mCur].t);
            	      determineErrors( gf[mCur],"AFS Errors: uCur " );
          	    }
      	}
            
      // interpolate exposed points on mOld
            {
      	ExposedPoints exposedPoints;
      	exposedPoints.setFillExposedInterpolationPoints(true);
      	exposedPoints.setAssumeInterpolationNeighboursAreAssigned(parameters.dbase.get<int >("extrapolateInterpolationNeighbours"));
      	exposedPoints.initialize(gf[mOld].cg,gf[mNew].cg,parameters.dbase.get<int >("stencilWidthForExposedPoints"));
      	exposedPoints.interpolate(gf[mOld].u,(twilightZoneFlow() ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),gf[mOld].t);
      	if( debug() & 16 )
        	  {
          	    if( twilightZoneFlow() )
            	      {
            		fprintf(debugFile,"\n ---> AFS: Errors in uOld AFTER interp exposed t=%e  \n",gf[mCur].t);
            		determineErrors( gf[mOld],"AFS Errors: uOld " );
            	      }
        	  }

            }

            
            parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInterpolateExposedPoints"))+=getCPU()-cpu0;
      // compute dudt now -- after exposed points have been computed!

            checkArrays(" AFS :  after moving grids update"); 

            if( debug() & 16 )
      	{
        	  if( twilightZoneFlow() )
          	    {
            	      fprintf(debugFile,"\n ---> AFS: Errors in u after move grids t=%e  \n",gf[mCur].t);
            	      determineErrors( gf[mCur] );
          	    }
      	}

            if( debug() & 16 )
      	printf(" AFS: AFTER moveTheGridsMacro: t0=%9.3e, gf[mNew].t=%9.3e, gf[mNew].gridVelocityTime=%9.3e\n",
             	       t0,gf[mNew].t,gf[mNew].gridVelocityTime);

        }
  //  PlotIt::contour(*Overture::getGraphicsInterface(),uOld.u);
  //  PlotIt::contour(*Overture::getGraphicsInterface(),uCur.u);

  // // // END OF MOVING GRID INITIALIZATION

    for ( int grid=0; grid<uCur.cg.numberOfComponentGrids(); grid++ )
        uNew.u[grid].dataCopy(uCur.u[grid]);

    CompositeGrid &cg = uNew.cg;

    fn[0].updateToMatchGridFunction(uNew.u);
  //    (  (*fn[0].getCompositeGrid())[0].mask()-(*uNew.u.getCompositeGrid())[0].mask()).display("MASK DIFF");

    fn[0] = 0.; // temporarily holds some of the forcing terms
  // first compute the initial rhs:
  //   U^{*} = (I-A_0)(I-A_1) ... (I-A_i) ... (I-A_N) u^{n} + f
  // stick U^{*} in the uNew grid function
  // We do this by first computing
  // U^{*} =  (I-A_0)(I-A_1) ... (I-A_i) ... (I-A_N) u^{n}
  // and then 
  // U^{*} <-- U^{*} + f

  //  for ( FactorList::iterator i_factor=factors.begin(); i_factor!=factors.end(); i_factor++ )
    real cpu1=getCPU();
    
    for ( int ifac=factors.size()-1; ifac>=0; ifac-- )
        {
            Factor_P factor = factors[ifac];
      // *wdh* 2012/04/06 changed to &4 from & 2 
            if ( debug() & 4 ) printF("solving RHS for factor %i, dir = %i, type = %s\n",ifac,factor->getDirection(),factor->getName().c_str());

            factor->solveRightHandSide(dt0,uCur,uNew);

#if 0
            real cdt = dt0;
            factor->addExplicitContribution(cdt,uCur,fn[0]); // some parts cannot be factored and are discretized explicitly in time
#else
      // we do an adams-bashforth style update to the explicit part, the coefficients ab1 and ab2 are computed above
      // *wdh* 2012/04/06 changed to &4 from & 2 
            if ( debug() & 4 ) printF("getting forcing for factor %i, dir = %i, type = %s\n",ifac,factor->getDirection(),factor->getName().c_str());
            real cdt = ab1; //dt0*3./2. ;
            factor->addExplicitContribution(cdt,uCur,fn[0]); // some parts cannot be factored and are discretized explicitly in time
            cdt = ab2; //-dt0/2.;
            factor->addExplicitContribution(cdt,uOld,fn[0]); // some parts cannot be factored and are discretized explicitly in time, recompute or store this level?
#endif
        }

      parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAFSrhs"))+=getCPU()-cpu1;

   //       PlotIt::contour(*Overture::getGraphicsInterface(),fn[0]);
   //         PlotIt::contour(*Overture::getGraphicsInterface(),uNew.u);

  // //
  // Get the forcing that results from twilight zone, gravity or whatever.  
  // Continue to stick these results into uNew (which is U^{*}).
  // basically : U^{*} <-- U^{*} + f
  // However, we will need to integrate these with a multi-level scheme to get second order accuracy so
  // this is just a placeholder for now.  Or could we iterate and get the accuracy back?
  // //
    if( debug() & 8 ) 
        {
            uNew.u.display("advanceFactored : RHS before getUt ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
            fn[0].display("advanceFactored : explicit forcing before getUt ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
        }

  // -- evaluate any body forcing (this is saved in realCompositeGridFunction bodyForce found in the data-base) ---
  // *wdh* 2011/10/09
    cpu1=getCPU();
    const real tForce = uCur.t; // evaluate the body force at this time  
    computeBodyForcing( uCur, tForce );

    for ( int grid=0; grid<uCur.cg.numberOfComponentGrids(); grid++ )
        {
      // we can save some memory per processor by moving this block to the beginning (before the
      // rhs and explicit contribution block) and stick the results directly into fn[0].  But do this after an initial check in of working code.

      // some options in getUt return the rhs in two parts, one from explict discretizations (dvdt->ftmp) 
      //      and one from linearization of a nonlinear implicit operator (dvdtImplicit->fn[0]).  The sum
      //      of these terms forms rhs.  
            realMappedGridFunction ftmp; ftmp.updateToMatchGridFunction(uCur.u[grid]);
            realMappedGridFunction ftmp2; ftmp2.updateToMatchGridFunction(uNew.u[grid]);
            ftmp = 0.;
            ftmp2 = 0.;
            rparam[0]=uCur.t;
            rparam[1]=uCur.t; // tforce, for stuff that gets put into uNew directly
            rparam[2]=uCur.t+dt0; // tImplicit, for stuff that goes into fn[0]
            rparam[3]=dt0;
            iparam[0]=grid;
            iparam[1]=uCur.cg.refinementLevelNumber(grid);
            iparam[2]=numberOfStepsTaken;

      // here is the (overloaded) call that actually computes the forcing
      //      getUt(uCur.u[grid],uCur.getGridVelocity(grid),
      //	   ftmp,iparam.ptr(),rparam.ptr(),fn[0][grid],&uNew.cg[grid]);

      //      addForcing(ftmp, uCur.u[grid],iparam.ptr(),rparam.ptr(),fn[0][grid]);//,&uNew.cg[grid])
      //                  PlotIt::contour(*Overture::getGraphicsInterface(),ftmp);
            # if 1
            rparam[1]=uCur.t; // tforce, for stuff that gets put into uNew directly
            realMappedGridFunction *gridVelocity = parameters.gridIsMoving(grid) ? &(uCur.getGridVelocity(grid)) : 0;
            if ( movingGridProblem() )
      	uCur.cg[grid].mask().dataCopy(uNew.cg[grid].mask());

            addForcing(ftmp, uCur.u[grid],iparam.ptr(),rparam.ptr(),fn[0][grid],gridVelocity);//,&uNew.cg[grid])

            rparam[1]=uCur.t+dt0; // tforce, for stuff that gets put into uNew directly
            gridVelocity = parameters.gridIsMoving(grid) ? &(uNew.getGridVelocity(grid)) : 0;
            addForcing(ftmp2, uNew.u[grid],iparam.ptr(),rparam.ptr(),fn[0][grid],gridVelocity);//,&uNew.cg[grid])
            #else
            rparam[1] = uCur.t + dt0/2.;
            realMappedGridFunction *gridVelocity = parameters.gridIsMoving(grid) ? &(uCur.getGridVelocity(grid)) : 0;
            addForcing(ftmp, uCur.u[grid],iparam.ptr(),rparam.ptr(),fn[0][grid],gridVelocity);//,&uNew.cg[grid])
            #endif
      //                              PlotIt::contour(*Overture::getGraphicsInterface(),ftmp);
      //                              PlotIt::contour(*Overture::getGraphicsInterface(),ftmp2);

            if( debug() & 8 ) 
      	{
        	  display(fn[0][grid],"advanceFactored : fn[0] after addForcing","%4.2f ");
        	  display(ftmp,"advanceFactored : forcing 1 after addForcing","%4.2f ");
        	  display(ftmp2,"advanceFactored : forcing 2 after addForcing","%4.2f ");
      	}

      //      rparam[1]=uCur.t + dt0; // tforce, for stuff that gets put into uNew directly
      //      getUt(uCur.u[grid],uCur.getGridVelocity(grid),
      //	   ftmp,iparam.ptr(),rparam.ptr(),fn[0][grid],&uNew.cg[grid]);

            if( debug() & 8 ) 
                  	fn[0][grid].display("advanceFactored : implicit forcing after getUt");
            
      //      ftmp /= 2.;
      // and here we add in the forcing
            Index I1,I2,I3,all;
            getIndex(uNew.cg[grid].dimension(),I1,I2,I3);
            realMappedGridFunction &un = uNew.u[grid];
            OV_GET_LOCAL_ARRAY(real,un);
            int lb1s,lb1e,lb2s,lb2e,lb3s,lb3e;
            bool have_local_points = ParallelUtility::getLocalArrayBounds(un,unLocal,I1,I2,I3,
                                                    								    lb1s,lb1e,lb2s,lb2e,lb3s,lb3e,parallel_array_bounds_option);

	  //	  uNew.u[grid](I1,I2,I3,all) += (dt0)*ftmp(I1,I2,I3,all) + fn[0][grid](I1,I2,I3,all);
            if ( have_local_points )
      	{
        	  realMappedGridFunction &fn0 = fn[0][grid];
        	  OV_GET_LOCAL_ARRAY(real,ftmp);
        	  OV_GET_LOCAL_ARRAY(real,ftmp2);
        	  OV_GET_LOCAL_ARRAY(real,fn0);

#if 0
        	  unLocal(I1,I2,I3,all) += dt0*ftmpLocal(I1,I2,I3,all) + fn0Local(I1,I2,I3,all);
#else
        	  unLocal(I1,I2,I3,all) += dt0*0.5*(ftmpLocal(I1,I2,I3,all)+ftmp2Local(I1,I2,I3,all)) + fn0Local(I1,I2,I3,all);
#endif
      	}

            uNew.u[grid].periodicUpdate();
            
        }
  //          PlotIt::contour(*Overture::getGraphicsInterface(),fn[0]);
  //  PlotIt::contour(*Overture::getGraphicsInterface(),uNew.u);

    if( debug() & 8 ) 
        uNew.u.display("advanceFactored : RHS after adding f^n ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");

    fn[1].dataCopy(uNew.u); // save this forcing for the interpolation point iteration
    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAFSforcing"))+=getCPU()-cpu1;

  // //
  // Now solve each of the factors.
  //    (I+A_i)U^{**} = U^{*}
  //    U^{*} <-- U^{**}
  // We iterate in order to converge the interpolation point values.
  // At this point we no longer need fn[0], use it to hold an extrapolation of
  // U to U^{n+1} to perform the LHS linearization about.
  // //
  // compute the extrapolated value of U{n+1} and save it in fn[0]
    cpu1=getCPU();
    GridFunction uExtrapolated;
    uExtrapolated.u.reference(fn[0]);
    for ( int grid=0; grid<uCur.cg.numberOfComponentGrids(); grid++ )
        {
            Index I1,I2,I3,all;
            getIndex(uNew.cg[grid].dimension(),I1,I2,I3);
            realMappedGridFunction &un = uNew.u[grid];
            OV_GET_LOCAL_ARRAY(real,un);
            int lb1s,lb1e,lb2s,lb2e,lb3s,lb3e;
            bool have_local_points = ParallelUtility::getLocalArrayBounds(un,unLocal,I1,I2,I3,
                                                    								    lb1s,lb1e,lb2s,lb2e,lb3s,lb3e,parallel_array_bounds_option);

            if ( have_local_points )
      	{
        	  realMappedGridFunction &uc = uCur.u[grid];
        	  realMappedGridFunction &uo = uOld.u[grid];
        	  realMappedGridFunction &fn0 = fn[0][grid];
        	  OV_GET_LOCAL_ARRAY(real,un);
        	  OV_GET_LOCAL_ARRAY(real,uc);
        	  OV_GET_LOCAL_ARRAY(real,uo);
        	  OV_GET_LOCAL_ARRAY(real,fn0);
        	  
	  //	  fn[0][grid](I1,I2,I3,all) = uCur.u[grid](I1,I2,I3,all) + (dt0/dtb) *(uCur.u[grid](I1,I2,I3,all)  - uOld.u[grid](I1,I2,I3,all));
	  // *wdh* 2012/04/30 fn0Local(I1,I2,I3,all) = ucLocal(I1,I2,I3,all) + (dt0/dtb) *(ucLocal(I1,I2,I3,all)  - uoLocal(I1,I2,I3,all));
          // *wdh* 2012/04/30 -- I don't think the above statement sets parallel ghost outside periodic boundaries. This caused
          // trouble on some restarts when these parallel ghost points were not set and maxGhostCorrection was always 2.
        	  fn0Local = ucLocal + (dt0/dtb) *(ucLocal - uoLocal);

	  // the following block of code should be moved somewhere more appropriate, it is only for cgins, but where should it go?!
	  // or else should we have an option to extrapolate some variables in time?
        	  const int & pc = parameters.dbase.get<int >("pc");
        	  if ( pc>=0 ) 
          	    unLocal(I1,I2,I3,pc) = ucLocal(I1,I2,I3,pc) + (dt0/dtb) *(ucLocal(I1,I2,I3,pc)  - uoLocal(I1,I2,I3,pc));
	    //	    uNew.u[grid](I1,I2,I3,pc) = uCur.u[grid](I1,I2,I3,pc) + (dt0/dtb) *(uCur.u[grid](I1,I2,I3,pc)  - uOld.u[grid](I1,I2,I3,pc));
        	  
      	}

        }

//            PlotIt::contour(*Overture::getGraphicsInterface(), uNew.u);
    
    const int nInterpolationPointIterations = parameters.dbase.get<int>("numberOfAFcorrections");
    real maxInterpolationPointCorrection = 0.;
    const int nComponents = uCur.u.getComponentDimension(0);
    ArraySimple<bool> skip(nComponents);
    skip = false;
    for ( std::list<int>::iterator i=parameters.dbase.get<std::list<int> >("AFComponentsToSkip").begin(); 
      	i!=parameters.dbase.get<std::list<int> >("AFComponentsToSkip").end(); 
      	i++ )
        skip[*i] = true;

    real tolerance = parameters.dbase.get<real>("AFcorrectionRelTol"); 
    int ipit = 0;
    bool tolNotMet = true;
    ArraySimple<ArraySimple<real> > maxCorrection(uCur.cg.numberOfComponentGrids());

    real maxGhostCorrection = 0.;
    real maxParallelGhostCorrection = 0.;
    real maxInterpGhostCorrection =0.; // *wdh* 2012/04/05 
    for ( ipit=0; ipit<nInterpolationPointIterations && tolNotMet ; ipit++ )
        {
            maxGhostCorrection = 0.;
            maxInterpolationPointCorrection = 0;
            maxParallelGhostCorrection = 0;
            if (ipit) 
      	{
	  // At the end of each iteration:
	  //   uNew  : current value of U^{n+1}
	  //   fn[1] : the new value of the RHS forcing, computed at the end of the loop block
        	  uNew.u.dataCopy(fn[1]);
      	}

            int ifac=0;
            for ( FactorList::iterator i_factor=factors.begin(); i_factor!=factors.end(); i_factor++ )
      	{
        	  Factor_P factor = *i_factor;
          // *wdh* 2012/04/06 changed to &4 from & 2           
        	  if ( debug() & 4 ) printF("solving LHS for factor %i, dir = %i, type = %s\n",ifac,factor->getDirection(),factor->getName().c_str());
	  //non-extrapolated linearization state	  factor->solveLeftHandSide(dt0,uCur,uNew);
        	  factor->solveLeftHandSide(dt0,uExtrapolated,uNew);
		  //            PlotIt::contour(*Overture::getGraphicsInterface(),uNew.u);
        	  if( debug() & 8 ) 
          	    uNew.u.display("advanceFactored : solution after solving LHS (inside)",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
        	  ifac++;
      	}
      //      PlotIt::contour(*Overture::getGraphicsInterface(),uNew.u);
      // Except for the time independent variables, we should now essentially have:
      // U^{n+1} <-- U^{*}
      // //
            if( debug() & 8 ) 
      	uNew.u.display("advanceFactored : solution after solving LHS ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
            
            gf[iuNew].t = t0 + dt0; // this needs to be here so the correct bc's are applied, especially in the tz case

      // To adjust the interpolation point forcing we first remove the last iteration's value of the interpolation
      //   point forcing, interpolate, and then add back in the new forcing.

      // here we remove the old interpolation point forcing from fn[1]
      // *wdh* 101030 if ( cg.numberOfInterpolationPoints.getLength(0) )
            for ( int grid=0; grid<uCur.cg.numberOfComponentGrids(); grid++ )
            {
      	OV_GET_LOCAL_ARRAY_FROM(int,mask,uNew.cg[grid].mask());  
      	
      	realMappedGridFunction &fn1 = fn[1][grid];
      	OV_GET_LOCAL_ARRAY(real,fn1);
      	realMappedGridFunction &un = uNew.u[grid];
      	OV_GET_LOCAL_ARRAY(real,un);
      	realMappedGridFunction &uc = uCur.u[grid];
      	OV_GET_LOCAL_ARRAY(real,uc);

      	if( cg.numberOfInterpolationPoints(grid)>0 ) // *wdh* 101030
        	  {
          	    GET_LOCAL_INTERPOLATION_POINTS(uNew.cg,grid,nInterpolationPoints,interpolationPoints);
	    //	      intArray &interpolationPoints = cg.interpolationPoint[grid];
	    //	      const int nInterpolationPoints = cg.numberOfInterpolationPoints(grid);
          	    maxCorrection[grid].resize(nInterpolationPoints,nComponents);
          	    
          	    int ii[3],&i1=ii[0],&i2=ii[1],&i3=ii[2];
          	    ii[2] = 0;
          	    for ( int i=0; i<nInterpolationPoints; i++ )
            	      {
            		for ( int axis=0; axis<uCur.cg.numberOfDimensions(); axis++ )
              		  ii[axis] = interpolationPoints(i,axis);
            		for ( int c=0; c<nComponents; c++ )
              		  {
                		    if ( !skip[c] )
                  		      fn1Local(i1,i2,i3,c) -= unLocal(i1,i2,i3,c)-ucLocal(i1,i2,i3,c);
              		  }
            	      }
        	  }

	//	PlotIt::contour(*Overture::getGraphicsInterface(),fn[1]);	
      	Index I1,I2,I3;
      	int lb1s,lb1e,lb2s,lb2e,lb3s,lb3e;
      	bool have_local_points = ParallelUtility::getLocalArrayBounds(un,unLocal,I1,I2,I3,
                                                      								      lb1s,lb1e,lb2s,lb2e,lb3s,lb3e,parallel_array_bounds_option);

      	if ( /*false &&*/ have_local_points )
        	  {
          	    IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
	    // NOTE: bcLocal(side,axis) == CG_ApproximateFactorization::parallelBC for internal boundaries between processors
          	    ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( fn1,gidLocal,dimLocal,bcLocal, CG_ApproximateFactorization::parallelBC ); // call this to get bcLocal
          	    
          	    OV_APP_TO_PTR_4D(real,ucLocal, ucp);
          	    OV_APP_TO_PTR_4D(real,unLocal, unp);
          	    OV_APP_TO_PTR_4D(real,fn1Local, fn1p);
          	    OV_APP_TO_PTR_3D(int,maskLocal,maskp);
          	    
          	    for ( int side=0; side<2; side++ )
            	      for ( int axis=0; axis<cg.numberOfDimensions(); axis++ )
            		if ( /*bcLocal(side,axis)>0 ||*/ bcLocal(side,axis)==CG_ApproximateFactorization::parallelBC || (bcLocal(side,axis)<0 && bcLocal((side+1)%2,axis)==CG_ApproximateFactorization::parallelBC) )
              		  for ( int gli=1; gli<=afParallelGhostWidth/*cg[grid].numberOfGhostPoints(side,axis)*/; gli++ )
                		    {
                  		      int offset = bcLocal(side,axis)==CG_ApproximateFactorization::parallelBC ? cg[grid].getMinimumNumberOfDistributedGhostLines()-afParallelGhostWidth : 0;
                  		      int gl = gli +offset;//bcLocal(side,axis)==CG_ApproximateFactorization::parallelBC ? gli +offset : (side ? gli+1 : gli);
                  		      Index Ig1,Ig2,Ig3;
                  		      getGhostIndex(gidLocal,side,axis,Ig1,Ig2,Ig3,gl);
		      // 		  if ( bcLocal(side,axis)==CG_ApproximateFactorization::parallelBC && Communication_Manager::My_Process_Number==0) 
		      // 		    {
		      // 		      gidLocal.display("GIDLOCAL");
		      // 		      cout<<"DBG: "<<Ig1.getBase()<<", "<<Ig1.getBound()<<", "<<Ig2.getBase()<<", "<<Ig2.getBound()<<endl;
		      // 		    }
                  		      for ( int c=0; c<nComponents; c++ )
                  			if ( !skip[c] )
                    			  for ( int ig3=Ig3.getBase(); ig3<=Ig3.getBound(); ig3++ )
                      			    for ( int ig2=Ig2.getBase(); ig2<=Ig2.getBound(); ig2++ )
                        			      for ( int ig1=Ig1.getBase(); ig1<=Ig1.getBound(); ig1++ )
                        				if ( A_3D(maskp,ig1,ig2,ig3)>0 )
                          				  {
                            				    A_4D(fn1p,ig1,ig2,ig3,c) -= A_4D(unp,ig1,ig2,ig3,c)-A_4D(ucp,ig1,ig2,ig3,c);
				    //			      cout<<"DBG: "<<ig1<<", "<<ig2<<", "<<c<<" ("<<Communication_Manager::My_Process_Number<<") : "<<A_4D(unp,ig1,ig2,ig3,c)<<", "<<A_4D(ucp,ig1,ig2,ig3,c)<<endl;
                          				  }
                  		      
		      //fn1Local(Ig1,Ig2,Ig3,c) -= unLocal(Ig1,Ig2,Ig3,c)-ucLocal(Ig1,Ig2,Ig3,c);
                		    }
        	  }// have local points
            } // for each grid
      // get the new interpolation point data
      //                  PlotIt::contour(*Overture::getGraphicsInterface(), uNew.u);
            interpolateAndApplyBoundaryConditions( uNew, &uCur,dt0 );

      //            PlotIt::contour(*Overture::getGraphicsInterface(), uNew.u);
            if ( false ) { // maybe this should be an option...
      	bool updateSolutionDependentEquations=true;  // e.g. for variable density, update p eqn here 
      	solveForTimeIndependentVariables( uNew,updateSolutionDependentEquations ); 
            }
      // now we add back the new interpolation point forcing using the newly interpolated points
            bool foundLargeDiff = false;
            for ( int grid=0; grid<uCur.cg.numberOfComponentGrids(); grid++ )
            {
	// reset the n+1 estimate for the linearization
      	OV_GET_LOCAL_ARRAY_FROM(int,mask,uNew.cg[grid].mask());  

      	realMappedGridFunction &fn1 = fn[1][grid];
      	OV_GET_LOCAL_ARRAY(real,fn1);
      	realMappedGridFunction &un = uNew.u[grid];
      	OV_GET_LOCAL_ARRAY(real,un);
      	realMappedGridFunction &uc = uCur.u[grid];
      	OV_GET_LOCAL_ARRAY(real,uc);
      	realMappedGridFunction &fn0 = fn[0][grid];
      	OV_GET_LOCAL_ARRAY(real,fn0);

      	Index I1,I2,I3,all;
      	getIndex(uNew.cg[grid].dimension(),I1,I2,I3);
      	int lb1s,lb1e,lb2s,lb2e,lb3s,lb3e;
      	bool have_local_points = ParallelUtility::getLocalArrayBounds(un,unLocal,I1,I2,I3,
                                                      								      lb1s,lb1e,lb2s,lb2e,lb3s,lb3e,parallel_array_bounds_option);

	// *wdh* 101030 if ( cg.numberOfInterpolationPoints.getLength(0) )
      	if( uNew.cg.numberOfInterpolationPoints(grid)>0 ) // *wdh* 101030
      	{
        	  GET_LOCAL_INTERPOLATION_POINTS(uNew.cg,grid,nInterpolationPoints,interpolationPoints);
	  //	      intArray &interpolationPoints = cg.interpolationPoint[grid];
	  //	      const int nInterpolationPoints = cg.numberOfInterpolationPoints(grid);
            	      
        	  int ii[3],&i1=ii[0],&i2=ii[1],&i3=ii[2];
        	  ii[2] = 0;
        	  for ( int i=0; i<nInterpolationPoints; i++ )
        	  {
          	    for ( int axis=0; axis<uCur.cg.numberOfDimensions(); axis++ )
            	      ii[axis] = interpolationPoints(i,axis);
          	    for ( int c=0; c<nComponents; c++ )
            	      if ( !skip[c] )
            		{
              		  maxCorrection[grid](i,c) = unLocal(i1,i2,i3,c)-fn0Local(i1,i2,i3,c);
              		  fn1Local(i1,i2,i3,c) += unLocal(i1,i2,i3,c)-ucLocal(i1,i2,i3,c);
              		  maxCorrection[grid](i,c) = fabs(maxCorrection[grid](i,c));
              		  maxInterpolationPointCorrection = max(maxInterpolationPointCorrection,maxCorrection[grid](i,c));
              		  if( debug() & 4 &&  maxCorrection[grid](i,c)>1.0 )  // *wdh* 2012/08/10 added debug 
                		    {
                  		      cout<<"DBG: INTERP LARGE DIFF: "<<grid<<", "<<", "<<i1<<", "<<i2<<", "<<i3<<", "<<c<<" : "<<unLocal(i1,i2,i3,c)<<", "<<fn0Local(i1,i2,i3,c)<<endl;
                		    }

            		}
        	  }
      	} // if there are interpolation points

      	if ( have_local_points ) 
        	  {
          	    IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
	    // NOTE: bcLocal(side,axis) == CG_ApproximateFactorization::parallelBC for internal boundaries between processors
          	    ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( fn1,gidLocal,dimLocal,bcLocal, CG_ApproximateFactorization::parallelBC ); // call this to get bcLocal
          	    OV_APP_TO_PTR_4D(real,ucLocal, ucp);
          	    OV_APP_TO_PTR_4D(real,unLocal, unp);
          	    OV_APP_TO_PTR_4D(real,fn1Local, fn1p);
          	    OV_APP_TO_PTR_3D(int,maskLocal,maskp);
	    //	    realMappedGridFunction &fn2 = fn[2][grid];
	    //	    OV_GET_LOCAL_ARRAY(real,fn2);
	    //	    OV_APP_TO_PTR_4D(real,fn2Local, fn2p);
          	    
          	    for ( int side=0; side<2; side++ )
            	      for ( int axis=0; axis<cg.numberOfDimensions(); axis++ )
            		{
              		  int ii[] = {0,0,0};
              		  ii[axis] = 1-2*side;
		  // !!! kkc we should have a list of bc's on which to ignore the limiter
              		  bool useLimiter = parameters.dbase.get<bool>("applyAFBCLimiter") && bcLocal(side,axis)!=CG_ApproximateFactorization::parallelBC;// && bcLocal(side,axis)!=Parameters::outflow;
              		  if ( useLimiter )
                		    {
                  		      std::list<int> &afBCToSkip = parameters.dbase.get<std::list<int> >("AFLimiterBoundariesToSkip");
                  		      for ( std::list<int>::iterator i=afBCToSkip.begin(); i!=afBCToSkip.end() && useLimiter; i++ )
                  			useLimiter = useLimiter && bcLocal(side,axis)!=*i;
                		    }

              		  if ( /*bcLocal(side,axis)>0 ||*/ bcLocal(side,axis)==CG_ApproximateFactorization::parallelBC || (bcLocal(side,axis)<0 && bcLocal((side+1)%2,axis)==CG_ApproximateFactorization::parallelBC))
                		    for ( int gli=1; gli<=afParallelGhostWidth/*cg[grid].numberOfGhostPoints(side,axis)*/; gli++ )
                  		      {
			//			int offset = cg[grid].getMinimumNumberOfDistributedGhostLines()-afParallelGhostWidth; //2;
                  		      int offset = bcLocal(side,axis)==CG_ApproximateFactorization::parallelBC ? cg[grid].getMinimumNumberOfDistributedGhostLines()-afParallelGhostWidth : 0;
                  			int gl = gli +offset;//XXXbcLocal(side,axis)==CG_ApproximateFactorization::parallelBC ? gli +offset : (side ? gli+1 : gli);
                  			Index Ig1,Ig2,Ig3;
                  			getGhostIndex(gidLocal,side,axis,Ig1,Ig2,Ig3,gl);
                  			for ( int c=0; c<nComponents; c++ )
                    			  if ( !skip[c] )
                      			    for ( int ig3=Ig3.getBase(); ig3<=Ig3.getBound(); ig3++ )
                        			      for ( int ig2=Ig2.getBase(); ig2<=Ig2.getBound(); ig2++ )
                        				for ( int ig1=Ig1.getBase(); ig1<=Ig1.getBound(); ig1++ )
                          				  if ( A_3D(maskp,ig1,ig2,ig3)>0 )
                            				    {
                              				      real extrap_2 = EXTRAP_2(unp,ig1,ig2,ig3,ii,c);
				      // *wdh* real extrap_3 = EXTRAP_3(unp,ig1,ig2,ig3,ii,c);
				      // *wdh* real extrap_4 = EXTRAP_4(unp,ig1,ig2,ig3,ii,c);
                              				      real extrap_5 = EXTRAP_5(unp,ig1,ig2,ig3,ii,c);
				      // *wdh* real extrap_6 = EXTRAP_6(unp,ig1,ig2,ig3,ii,c);
                              				      
                              				      real ucur = A_4D(ucp,ig1,ig2,ig3,c);
				      //				      real urealold = fn2Local(ig1,ig2,ig3,c);
                              				      real uold = fn0Local(ig1,ig2,ig3,c);//A_4D(ucp,ig1,ig2,ig3,c);
                              				      real unew = A_4D(unp,ig1,ig2,ig3,c);
                              				      real alpha = fabs((extrap_5-unew)/(extrap_2 + 100.*REAL_EPSILON));
                              				      alpha = min(1.,alpha);
				      //				if ( grid==1 && axis==1 && side==0 && gl<3 && ig1==0 && c) cout<<"DBG: "<<ig1<<", "<<ig2<<", "<<c<<" : "<<extrap_2<<", "<<extrap_3<<", "<<extrap_4<<", "<<extrap_5<<", "<<extrap_6<<" : "<<A_4D(unp,ig1,ig2,ig3,c)<<", alpha = "<<alpha<<endl;
                              				      if (useLimiter)
                              					unew = (1.-alpha)*unew + alpha*A_4D(ucp,ig1+gl*ii[0],ig2+gl*ii[1],ig3+gl*ii[2],c);
				      //								cout<<"DBG: "<<ig1<<", "<<ig2<<" ("<<Communication_Manager::My_Process_Number<<") : "<<unew<<", "<<uold<<endl;
				      // *wdh* real f=1;//0.5;//1;
				      // *wdh* if ( /*bcLocal(side,axis)==CG_ApproximateFactorization::parallelBC*/ true  )  unew = f*unew + (1-f)*uold;
                              				      
#if 0
                              				      real dx0 = A_4D(fn1p,ig1,ig2,ig3,c);//uold-urealold;//(A_4D(fn1p,ig1,ig2,ig3,c)-ucur);
                              				      real urealold = uold - dx0;
                              				      bool denomok = fabs(-dx0 - uold + unew)>100*REAL_EPSILON;
                              				      if ( ipit>1 /*&& bcLocal(side,axis)==CG_ApproximateFactorization::parallelBC*/ )  unew = !denomok ? unew : urealold - dx0*dx0/(urealold - 2.*uold + unew);
#endif
                              				      
                              				      A_4D(unp,ig1,ig2,ig3,c) = unew;
                              				      A_4D(fn1p,ig1,ig2,ig3,c) += unew-A_4D(ucp,ig1,ig2,ig3,c);
                              				      if (debug() & 4 && fabs(unew-fn0Local(ig1,ig2,ig3,c)>1.0) )// *wdh* 2012/08/10 added debug 
                              					{
                                					  cout<<"DBG: LARGE DIFF: "<<grid<<", "<<side<<", "<<axis<<", "<<ig1<<", "<<ig2<<", "<<ig3<<", "<<c<<" : "<<unew<<", "<<fn0Local(ig1,ig2,ig3,c)<<endl;
                              					}
                              				      if ( bcLocal(side,axis)==CG_ApproximateFactorization::parallelBC  )
                              					{
                                					  maxParallelGhostCorrection = max(maxParallelGhostCorrection,fabs(unew-fn0Local(ig1,ig2,ig3,c)));
                              					}
                              				      else
                              					{
                                					  maxGhostCorrection = max(maxGhostCorrection,fabs(unew-fn0Local(ig1,ig2,ig3,c)));
                              					}
                            				    }
			//		  for ( int c=0; c<nComponents; c++ )
			//		    fn1Local(Ig1,Ig2,Ig3,c) += unLocal(Ig1,Ig2,Ig3,c)-ucLocal(Ig1,Ig2,Ig3,c);
                  		      } // ghost line	
            		} // axis,side

          	    realMappedGridFunction &un = uNew.u[grid];
          	    realMappedGridFunction &fn0 = fn[0][grid];
          	    OV_GET_LOCAL_ARRAY(real,fn0);
          	    OV_GET_LOCAL_ARRAY(real,un);
          	    
	    //	    fn[0][grid](I1,I2,I3,all) = uNew.u[grid](I1,I2,I3,all);
	    //	    fn2Local(I1,I2,I3,all) = fn0Local(I1,I2,I3,all);
	    // *wdh* 2012/04/30 fn0Local(I1,I2,I3,all) = unLocal(I1,I2,I3,all);
            // *wdh* 2012/04/30 -- assign all points -- see remark above for fn0Local
          	    fn0Local = unLocal; // *wdh* 
        	  } // have local points
            } // for each grid

            if (debug()>3) // *wdh* 2012/08/03 -- changed to >3 
      	{
#ifdef USE_PPP
        	  cout<<"advanceFactored: max parallel ghost point correction ("<<Communication_Manager::My_Process_Number<<") at "<<ipit<<" steps = "<<maxParallelGhostCorrection<<endl;
#endif
	  //	  cout<<"advanceFactored: max ghost point correction ("<<Communication_Manager::My_Process_Number<<") at "<<ipit<<" steps = "<<maxGhostCorrection<<endl;
      	}	
            maxInterpGhostCorrection = max(maxParallelGhostCorrection,max(maxInterpolationPointCorrection,maxGhostCorrection));
            maxInterpGhostCorrection = ParallelUtility::getMaxValue(maxInterpGhostCorrection);
            tolNotMet = maxInterpGhostCorrection>tolerance;// || ipit==0;// do at least one iteration

      // printF("advanceFactored: ipit=%i, maxGhostCorrection=%8.2e, maxParallelGhostCorrection=%8.2e, maxInterpolationPointCorrection=%8.2e maxInterpGhostCorrection=%8.2e, tolerance=%8.2e\n",ipit,maxGhostCorrection,maxParallelGhostCorrection,maxInterpolationPointCorrection,maxInterpGhostCorrection,tolerance);
            

      //            cout<<"maxInterpolationPointCorrection = "<<maxInterpolationPointCorrection<<endl;
        } // end of interpolation point iteration loop

    if (debug()>3) // *wdh* 2012/08/03 -- changed to >3
        {
            cout<<"advanceFactored("<<Communication_Manager::My_Process_Number<<"): max interpolation point correction in "<<ipit<<" steps = "<<maxInterpolationPointCorrection<<endl;
#ifdef USE_PPP
            cout<<"advanceFactored("<<Communication_Manager::My_Process_Number<<"): max parallel ghost point correction in "<<ipit<<" steps = "<<maxParallelGhostCorrection<<endl;
#endif
        }

    if( debug() & 1 ) // *wdh* 2012/04/05
    {
        printF("advanceFactored:INFO: maxInterpGhostCorrection=%8.2e in %i steps (interp. and parallel ghost) t=%9.3e\n", 
         	   maxInterpGhostCorrection,ipit,t0+dt0);
    }
    
  // *wdh* 2012/04/05:
  // When we fail to converge by too large an amount we abort -- otherwise batch jobs can hang doing nothing
  // once nan's appear
    const real divergenceTolerance = 100.*tolerance; // we could make this a user parameter
    if( ipit>=nInterpolationPointIterations && maxInterpGhostCorrection>divergenceTolerance )
    {
        printF("advanceFactored:FATAL ERROR: factored iterations are diverging, steps=%i, "
                      "maxInterpGhostCorrection=%8.2e > divergenceTolerence=%8.2e.\n"
                      " Possible corrective actions: decrease the CFL number or increase the number of parallel ghost.\n",
                      ipit,maxInterpGhostCorrection,divergenceTolerance);
        OV_ABORT("ERROR");
    }
    

    if( debug() & 8 ) 
        uNew.u.display("advanceFactored : solution after boundary conditions ",parameters.dbase.get<FILE* >("debugFile"),"%6.3f ");
    
    parameters.dbase.get<int>("totalNumberOfPCcorrections")+=ipit;  // count the total number of corrections.
    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAFSlhs"))+=getCPU()-cpu1;
  // //
  // Solve any constraint (or extra) equations, note that we will need to iterate to return the scheme to
  // second order accuracy.
  // //
    bool updateSolutionDependentEquations=true;  // e.g. for variable density, update p eqn here 
    solveForTimeIndependentVariables( uNew,updateSolutionDependentEquations ); 


  // correct for forces on moving bodies if we have more corrections. *wdh* 2011/12/07 
    int moveCorrection=0, numberOfCorrections=2, minimumNumberOfPCcorrections=1;
    if( movingGridProblem() && (moveCorrection+1)<numberOfCorrections)
    {
        if( debug() & 2 ) // *wdh* 2013/10/02
            printF("advFactored: add correction for moving grids, t=%9.3e\n",gf[iuNew].t );
        correctMovingGrids( t0,t0+dt0,uCur,uNew ); 

    // NOTE: This next section (currently turned off) is needed for iterations for light moving bodies -- see advanceStepsPC.bC 
    //       Eventually we may need to add this correction iteration to AFS
        if( false )
        {
      // Check if the correction step has converged
            bool isConverged = getMovingGridCorrectionHasConverged();
            real delta = getMovingGridMaximumRelativeCorrection();
            if( debug() & 1 )
            {
      	printF("PC: moving grid correction step : delta =%8.2e (correction=%i, isConverged=%i)\n",
             	       delta,moveCorrection+1,(int)isConverged);
            }
            if( isConverged && (moveCorrection+1) >=minimumNumberOfPCcorrections )  // note correction+1 
      	advanceOptions.correctionIterationsHaveConverged=true;  // we have converged 
        }
        
    }

//   if( uNew.t <= 4.*dt ) // *****************************************************************************
//   {
//     printF("AF: Set solution to TRUE at t=%9.3e\n",uNew.t);
//     assignInitialConditions( iuNew );
//   }
    

    adamsData.dtb = dt0;

    if( debug() & 16 )
        {
            if( twilightZoneFlow() )
      	{
        	  fprintf(debugFile,"\n ---> AFS : Errors in u at end of step t=%e  \n",gf[iuNew].t);
        	  determineErrors( gf[iuNew] );
      	}
        }


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
endTimeStepAF( real & t0, real & dt0, AdvanceOptions & advanceOptions )
{
    int iuNew =(current+1)%3;

    t0+=dt0;

    current = iuNew;
  //  saveSequenceInfo(t0,fn[0]);
  //  fn[0].reference(fn[0]);
    output( gf[current],parameters.dbase.get<int >("globalStepNumber") );
    if( debug() & 4 )
        printP("DomainSolver::endTimeStepAF  t0=%e dt0=%e ----\n",t0,dt0);

    return 0;
}
