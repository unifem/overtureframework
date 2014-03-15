#include "DomainSolver.h"
#include "ParallelUtility.h"
#include "EquationDomain.h"


//\begin{>>cgInclude.tex}{\subsection{getTimeStep}} 
real DomainSolver::
getTimeStep( GridFunction & gf0 )
//=======================================================================================================
// /Description:
//      Determine the time step dt for the entire grid
//
// /Author: WDH
//
//\end{cgInclude.tex}  
// =======================================================================================================
{

  if( parameters.dbase.get<int>("simulateGridMotion")>0 )
  {
    return parameters.dbase.get<real >("dtMax");
  }
  

  real cpu0=getCPU();

  Parameters::TimeSteppingMethod &timeSteppingMethod=parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
  

  if ( timeSteppingMethod==Parameters::steadyStateNewton )
    { // we don't use a time step for the Newton solver, skip out of here and return something that makes the errors/warnings go away
      parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForComputingDeltaT"))+=getCPU()-cpu0;
      return 1.;
    }

  CompositeGrid & cg = gf0.cg;


  // kkc 070130 : BILL : for a general PDE the following check probably makes little sense, or at least should be done in the derived classes who know about these things; if you agree please delete the following commented block of code.
//   if( parameters.dbase.get<Parameters::PDEVariation >("pdeVariation")==Parameters::conservativeGodunov )
//   {
//     // no need to be in primitive variables for this method
//   }
//   else if( gf0.form!=GridFunction::primitiveVariables )
//   {
//     printf(" getTimeStep: gf0.form=%s\n",gf0.form==0 ? "primitiveVariables" : "conservative" );
//     printf("OB_CompositeGridSolver::getTimeStep: ERROR: gf0 should be in primitive variables!\n");
//     Overture::abort("error");
//   }

  const real cfl = parameters.dbase.get<real >("cfl");

  real dtNew;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
//     real dtGrid = mappedGridSolver[grid]->getTimeStep(gf0.cg[grid],gf0.u[grid],gf0.getGridVelocity(grid),
//                                                       timeSteppingMethod,grid );
    real dtGrid = getTimeStep(gf0.cg[grid],gf0.u[grid],gf0.getGridVelocity(grid),
			      timeSteppingMethod,grid );

    if( timeSteppingMethod==Parameters::variableTimeStepAdamsPredictorCorrector )
      dtGrid = ParallelUtility::getMinValue(dtGrid);  // *wdh* 060714 

    variableDt(grid)=dtGrid;
    
    if( timeSteppingMethod==Parameters::implicit && debug() & 4 )
      printf("getTimeStep: dt=%e for grid=%s if time stepping is %s\n",dtGrid,
             (const char*)cg[grid].mapping().getName(Mapping::mappingName),
             (bool)parameters.getGridIsImplicit(grid) ? "implicit" : "explicit");

    dtNew = grid==0 ? dtGrid : min(dtNew,dtGrid);
  }
  if( timeSteppingMethod!=Parameters::variableTimeStepAdamsPredictorCorrector )
  {
    dtNew = ParallelUtility::getMinValue(dtNew);  // *wdh* 060714 
  }
  
  
  if( parameters.dbase.get<bool >("adjustTimeStepForMovingBodies") && parameters.isMovingGridProblem() )
  {
    real dtRB = parameters.dbase.get<MovingGrids >("movingGrids").getTimeStepForRigidBodies();
    // dtRB*=.1;
     
    printf(" ========getTimeStep:  dt=%8.2e, dtRB=%8.2e =====\n",dtNew,dtRB);
    if( dtRB>0. && dtRB<dtNew )
    {
      // dtRB=max(dtRB,dtNew*1.e-2);  // ****
      

      printf("+++++++ getTimeStep: decreasing dt for rigid bodies, dt=%8.2e -> dtRB=%8.2e +++++++\n",dtNew,dtRB);
      dtNew=min(dtNew,dtRB);
    }
    
  }

  const real t0 = parameters.dbase.get<real>("tInitial");
  const real slowStartTime=parameters.dbase.get<real >("slowStartTime");
  const int slowStartSteps = parameters.dbase.get<int>("slowStartSteps");
  const int globalStepNumber =parameters.dbase.get<int >("globalStepNumber");
  if( globalStepNumber<slowStartSteps )
  {
    // -- slow start based on the number of time STEPS ---
    const real slowStartCFL = parameters.dbase.get<real >("slowStartCFL");
    real alpha = globalStepNumber/real(max(1,slowStartSteps));
    real factor = ( (1.-alpha)*slowStartCFL + alpha*cfl )/cfl;
    dtNew*=factor;
    printF("+++slow start by step: step=%i (t=%8.2e) slowStartCFL=%8.2e, targetCFL=%8.2e, current CFL=%8.2e (dt=%9.3e)\n",
	   globalStepNumber,gf0.t,slowStartCFL,cfl,cfl*factor,dtNew); 
  }
  else if( gf0.t-t0 < slowStartTime )
  {
    // -- slow start based on the current t ---
    const real slowStartCFL = parameters.dbase.get<real >("slowStartCFL");
    real tDiff = gf0.t-t0;
    real alpha = tDiff/slowStartTime;
    real factor = ( (1.-alpha)*slowStartCFL + alpha*cfl )/cfl;
    dtNew*=factor;
    printF("+++slow start by time: t=%8.2e (step=%i) slowStartCFL=%8.2e, targetCFL=%8.2e, current CFL=%8.2e (dt=%9.3e)\n",
	   gf0.t,globalStepNumber,slowStartCFL,cfl,cfl*factor,dtNew);
  }
  // dt should be less than or equal to dtMax
  real & dtMax = parameters.dbase.get<real >("dtMax");
  if( dtNew > dtMax )
  {
    if( debug() & 2 )
      printF("DomainSolver::getTimeStep:INFO: t=%8.2e, reducing the time step to dtMax=%e\n",gf0.t,dtMax);
    dtNew = dtMax;
  }

  // dt should be less than or equal to tPrint *wdh* 100818
  real & tPrint =  parameters.dbase.get<real >("tPrint");
  if( dtNew > tPrint )
  {
    if( debug() & 2 )
      printF("DomainSolver::getTimeStep:INFO: t=%8.2e, reducing the time step to tPrint=%e\n",gf0.t,tPrint);
    dtNew = tPrint;
  }
  
  if( debug() & 4 )
  {
    printF(">> DomainSolver::getTimeStep, t=%8.2e, dtNew = %10.4e, cfl=%5.2f, stepNumber=%i\n",
	   gf0.t,dtNew,cfl,globalStepNumber);
  }
  
  // check that the time step is the same on all processors
  real dtp = ParallelUtility::getMinValue(dtNew);  
  if( dtp!=dtNew )
  {
    printf("******* DomainSolver::getTimeStep:ERROR: dtp=%10.4e, dtNew=%10.4e  *****\n",dtp,dtNew);
  }
  
  if( dtNew!=dtNew ) // check for nan -- does this always work?
  {
    printF("getTimeStep:FATAL ERROR: dt computed to a `nan'! dtNew=%e\n",dtNew);
    OV_ABORT("ERROR");
  }
  


  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForComputingDeltaT"))+=getCPU()-cpu0;
  return dtNew;
}


  // determine the time step based on a given solution
real DomainSolver::
getTimeStep(MappedGrid & mg,
            realMappedGridFunction & u0, 
	    realMappedGridFunction & gridVelocity,
            const Parameters::TimeSteppingMethod & timeSteppingMethod,
            const int & grid )
//=======================================================================================================
// /Description:
//      Determine the time step dt for a grid
//
// /Author: WDH
//
//\end{cgInclude.tex}  
// =======================================================================================================
{
  if( parameters.dbase.get<int>("simulateGridMotion")>0 )
  {
    return parameters.dbase.get<real >("dtMax");
  }

  real dtNew,reLambda,imLambda;
  getTimeSteppingEigenvalue( mg,u0,gridVelocity,reLambda,imLambda,grid );

  const real eps=REAL_MIN*1.e3;
  if( fabs(reLambda)+fabs(imLambda) < eps )
  {
    reLambda=eps;
    imLambda=eps;
  }
  // *** scLC
  if( timeSteppingMethod==Parameters::midPoint ||
      timeSteppingMethod==Parameters::rKutta)
  { // the stability region for the midpoint rule extends to -2 on the Re axis
    // and about sqrt(3) along the imaginary (NOT REALLY -- it does not touch the Im axis!!)
    dtNew= 1./SQRT( SQR(reLambda/2.)+SQR(imLambda/SQRT(3.)) );
  }
  // *** ecLC
  else if( timeSteppingMethod==Parameters::adamsBashforth2 )
  {
    // the stability region for 2nd order AB: -1 on th Real axis, about .8 on the Im. axis
    dtNew= 1./SQRT( SQR(reLambda/1.)+SQR(imLambda/.8) ); 
  }
  else if( timeSteppingMethod==Parameters::steadyStateRungeKutta )
  {
    // ** fix this **
    //  stability region 3 on the Im axis ???
    dtNew= 1./SQRT( SQR(reLambda/1.)+SQR(imLambda/.8) ); 
  }
  else if( timeSteppingMethod==Parameters::adamsPredictorCorrector2 ||
           timeSteppingMethod==Parameters::variableTimeStepAdamsPredictorCorrector )
  {
    // the stability region for 2nd order PECE: -2 on the Real axis, about 1.27 on the Im. axis
    // but there is a bit of a cusp on the Re axis so go for -1.9 instead
    dtNew= 1./SQRT( SQR(reLambda/1.8)+SQR(imLambda/1.27) );
  }
  else if( timeSteppingMethod==Parameters::adamsPredictorCorrector4 )
  {
    // the stability region for 4th order PECE: -1.7 on the Real axis, about 1.15 on the Im. axis
    //  ** AB3-AM4  : 3rd-order predictor, 4th-order corrector (has a better stability region than AB4-AM4)
    dtNew= 1./SQRT( SQR(reLambda/1.7)+SQR(imLambda/1.15) );
  }
  else if( timeSteppingMethod==Parameters::implicit )
  {
    // the stability region for the implicit method:
    // the stability region for 2nd order PECE: -2 on the Real axis, about 1.27 on the Im. axis
    // but there is a bit of a cusp on the Re axis so go for -1.9 instead
    dtNew= 1./SQRT( SQR(reLambda/1.8)+SQR(imLambda/1.27) );
    

    // * // the stability region for 2nd order AB: -1 on th Real axis, about .8 on the Im. axis
    // * dtNew= 1./SQRT( SQR(reLambda/1.)+SQR(imLambda/.8) ); 
  }
  else if( timeSteppingMethod==Parameters::implicitAllSpeed )
  {
    // the stability region for the implicit method:
    dtNew= 1./SQRT( SQR(reLambda/1.)+SQR(imLambda/SQRT(.8)) ); // fix this ****
  }
  else if( timeSteppingMethod==Parameters::forwardEuler )
  { 
    dtNew= 1./SQRT( SQR(reLambda/2.)+SQR(imLambda/1) );  // ***********is this ok ****
  }
  else if( timeSteppingMethod==Parameters::adi )
  {
    dtNew= REAL_MAX/100.;  // no time-stepping limit
  }
  else if( timeSteppingMethod==Parameters::steadyStateNewton )
  {
  }
  else 
  {
    printF("DomainSolver::getTimeStep:ERROR unknown timeSteppingMethod \n");
    Overture::abort("error");
  }


  dtNew*=parameters.dbase.get<real >("cfl");

  if( debug() & 4 )
  {
    printf("++++getTimeStep, grid=%i reLambda=%8.2e imLambda=%8.2e cfl=%f dtNew =%8.2e (p=%i)\n",
	   grid,reLambda,imLambda,parameters.dbase.get<real >("cfl"),dtNew,parameters.dbase.get<int >("myid"));
  }
  if( dtNew < 1.e-10 )
  {
    printF("DomainSolver::getTimeStep:WARNING: small time step? grid=%i, dtNew=%e, reLambda=%e, imLambda=%e\n",
	   grid,dtNew,reLambda,imLambda);
  }

  return dtNew;

}
 


//\begin{>>cgInclude.tex}{\subsection{getTimeSteppingEigenvalue}} 
void DomainSolver::
getTimeSteppingEigenvalue(MappedGrid & mg,
			  realMappedGridFunction & u0, 
			  realMappedGridFunction & gridVelocity,  
			  real & reLambda,
			  real & imLambda,
                          const int & grid)
//=====================================================================================================
// /Description:
//   Determine the real and imaginary parts of the eigenvalue for time stepping.
//
// /Author: WDH
//
//\end{cgInclude.tex}  
// ===================================================================================================
{

  if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
  {
    ListOfEquationDomains & equationDomainList = *(parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"));  
    const int numberOfEquationDomains=equationDomainList.size();
    const int equationDomainNumber=equationDomainList.gridDomainNumberList[grid];
    assert( equationDomainNumber>=0 && equationDomainNumber<numberOfEquationDomains );
    EquationDomain & equationDomain = equationDomainList[equationDomainNumber];

  }
  
  // Get Index's for the interior+boundary points
  Index I1,I2,I3;
  getIndex( mg.extendedIndexRange(),I1,I2,I3);

  getTimeSteppingEigenvalue(mg,u0,gridVelocity,reLambda,imLambda,grid);


  // get max values over all processors:
  reLambda=ParallelUtility::getMaxValue(reLambda);
  imLambda=ParallelUtility::getMaxValue(imLambda);

}

