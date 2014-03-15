#include "DomainSolver.h"
#include "CompositeGridOperators.h"
#include "GridCollectionOperators.h"
#include "interpPoints.h"
#include "PlotStuff.h"

//==================================================================
//           Variable Time stepping
//
//==================================================================


static IntegerArray *m0Pointer=NULL, *m1Pointer=NULL;

// variableDt(grid) : time step for grid
// tv(grid) == variableTime(grid) : current time for grid
// tvb(grid) : previous time of grid
//
// tv0(grid,timeLevel) : time for values in ui[grid](.,.,timeLevel)
// ui[grid](i,n,timeLevel)  i=0:numberOfInterpolationPoints-1 n=0:numberOfComponents-1, timeLevel=0,1,..

// static realArray ui[10];
// static realArray tv, tvb, tv0;

static int numberOfTimeLevels;

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{variableTimeStepBoundaryInterpolation}} 
int DomainSolver::
variableTimeStepBoundaryInterpolation( int grid, GridFunction & cgf )
// ========================================================================================
// /Description:
//    Obtain values on the interpolation points component grid "grid" by interpolation
//  in time. Also swap periodic edges
//
//
//\end{CompositeGridSolverInclude.tex}  
// =======================================================================================
{
  if( cgf.cg.numberOfInterpolationPoints(grid)<=0 )
    return 0;
  
  const int numberOfComponentGrids=cgf.cg.numberOfComponentGrids();
  const int numberOfDimensions=cgf.cg.numberOfDimensions();
  const int numberOfInterpolationPoints=cgf.cg.numberOfInterpolationPoints(grid);
  
  assert( grid>=0 && grid<numberOfComponentGrids );
  
  realArray & u = cgf.u[grid];
  realArray & uig = ui[grid];

  intArray & ip = cgf.cg.interpolationPoint[grid];
  intArray & interpoleeGrid = cgf.cg.interpoleeGrid[grid];
  
  
  RealArray a1(numberOfComponentGrids), a2(numberOfComponentGrids);
  
  Range G=numberOfComponentGrids;
  a1(G)= (tv(grid)-tv0(G,1,grid))/(tv0(G,0,grid)-tv0(G,1,grid));
  a2(G)= (tv(grid)-tv0(G,0,grid))/(tv0(G,1,grid)-tv0(G,0,grid));
  
  // display(a1,"a1");
  // display(a2,"a2");
  

  const int ip3=cgf.cg[grid].dimension(Start,axis3);
  if( numberOfDimensions==2 )
  {
    for( int i=0; i<numberOfInterpolationPoints; i++ )
    {
      const int ki=interpoleeGrid(i);
      for( int n=u.getBase(3);  n<=u.getBound(3); n++ )
	u(ip(i,0),ip(i,1),ip3,n)=a1(ki)*uig(i,n,0)+a2(ki)*uig(i,n,1);
    }
  }
  else
  {
    for( int i=0; i<numberOfInterpolationPoints; i++ )
    {
      const int ki=interpoleeGrid(i);
      for( int n=u.getBase(3);  n<=u.getBound(3); n++ )
	u(ip(i,0),ip(i,1),ip(i,2),n)=a1(ki)*uig(i,n,0)+a2(ki)*uig(i,n,1);
    }
  }
  
  cgf.u[grid].periodicUpdate();
  
  return 0;
}

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{updateVariableTimeInterpolation}} 
int DomainSolver::
updateVariableTimeInterpolation( int newGrid, GridFunction & cgf )
// ========================================================================================
// /Description:
//     Update the variable time values given new values on component grid "newGrid".
// Interpolate values on other grids -- if the time corresponding to the other grid 
// is less than or equal to the time of newGrid.
//
//\end{CompositeGridSolverInclude.tex}  
// =======================================================================================
{
  const int numberOfComponentGrids=cgf.cg.numberOfComponentGrids();
  const int numberOfDimensions=cgf.cg.numberOfDimensions();

  realArray & u = cgf.u[newGrid];
//intArray & ip = cgf.cg.interpolationPoint[newGrid];
//intArray & interpoleeGrid = cgf.cg.interpoleeGrid[newGrid];

  Interpolant & interpolant = *(cgf.u.getInterpolant());
  
  // obtain updated interpolation points for all points that interpolate from "newGrid"

  realArray uInt;  // will be dimensioned when interpolated.
  
  Range C=u.dimension(3);
  
  for( int grid=0; grid<numberOfComponentGrids; grid++ )
  {
    if( grid!=newGrid && tv(grid)<=tv(newGrid) && cgf.cg.numberOfInterpolationPoints(grid)>0 )
    {
      // interpolate "grid" from "newGrid"
      interpolant.interpolate( uInt,grid,newGrid,cgf.u );
      Range R = uInt.dimension(0);
      
      for( int m=numberOfTimeLevels-1; m>0; m-- )
      {
        ui[grid](R,C,m)=ui[grid](R,C,m-1);
	tv0(newGrid,m,grid)=tv0(newGrid,m-1,grid);
      }
      
      ui[grid](R,C,0)=uInt(R,C);   // note that interp pts are sorted by interpolee grid so this is easy!

      tv0(newGrid,0,grid)=tv(newGrid);
    }
  }
  
  return 0;
}


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{advanceVariableTimeStepAdamsPredictorCorrector}} 
void DomainSolver::
advanceVariableTimeStepAdamsPredictorCorrector( real & t0, real & dt0, int & numberOfSubSteps, int & init, 
                                               int initialStep  )
//=====================================================================================================
//
// /Variable Time Step Method: Use a 2nd order Adams Predictor Corrector
//
// /Description:
//    Advance the solution in time by stepping each component grid
//  with its own time-step dtv(k).
//
//  Use a second-order Adams-Bashforth/Adams-Moulton PC method where
//  the time step dtv(k) depends on the grid number k:
// \begin{verbatim}
//  Predictor:
//   u(*) <- u(t) + dtv(k)*[ (3/2) du/dt - (1/2) du(t-dtv(k))/dt ]
//  Corrector
//   u(t+dt) <- u(t) + dtv(k)*[ (1/2) du(*)/dt(k) + (1/2) du(t)/dt ]
// \end{verbatim}
//
// The factors of (3/2) and (1/2) appearing in the formula will change
// when the time step changes. The method is a so-called PECE method
// because we Predict-Evaluate-Correct-Evaluate.
//
// /Variable time stepping:
//   The variable time-stepping algorithm chooses a different
// time step on each component grid. The algorithm proceeds by
// always advancing the grid, k=kmin, with the minimum time. The values
// on the interpolation boundaries of grid kmin are obtained by
// interpolating in time from the other grids.
// \begin{verbatim}
//   tv(k)  : current time for current solution on grid k
//   tvb(k) : time of previous solution on grid k
// \end{verbatim}
// In order to make the interpolation efficient we keep some
// auxillary arrays which hold the interpolation values at
// three different times - this lets us do quadratic interpolation.
//
// \begin{verbatim}
//   ui0(m,n,l,k) : for each interpolation point m=1,2,...,ni(k)
//     on grid k the values n=1,2,...,nv of the solution are saved at
//     three times l=1,2,3. These times are stored in tv0(k1,l,k)
//     where k1(m)=il(m,nd+1) is the grid number form which point
//     m interpolates from.
// \end{verbatim}
//
// /NOTE: For technical reasons we actually perform a ECEP method
// so that the value returned is the result from the predictor (which
// is still second order accurate). The reason for this is because
// of the way the variable step step method works - after we predict
// a component grid k to a new time level it may no longer be the
// grid with the minimum time so we could not correct it until we
// advance the other grids with smaller times.
//
//
// /t0,dt0 (input) : current time and time step.
// /numberOfSubSteps (input) : take this many steps
// /init (input) : if TRUE this is the first time step in whcih case this routine will initialize itself.
//
// /Notes:
//   
//\end{CompositeGridSolverInclude.tex}  
//================================================================================================
{
  assert( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::variableTimeStepAdamsPredictorCorrector );

  int numberOfCorrections=1;
  int mab0=0;
  int mab1=1;

  if( debug() & 2 )
  {
    fprintf(parameters.dbase.get<FILE* >("debugFile")," *** Entering advanceVariableTimeStepAdamsPredictorCorrector: t=%e, dt=%e *** \n",
      t0,dt0);
  }
 
  
  int grid;
  Index I1,I2,I3;  
  Range N = parameters.dbase.get<Range >("Rt");   // time dependent variables
  RealArray error(numberOfComponents()+3); 
  int iparam[10];
  real rparam[10];

  int numberOfComponentGrids = gf[mab0].cg.numberOfComponentGrids();

  real tNext=t0+numberOfSubSteps*dt0;  // integrate to this next major time level

  // adjust the time steps so we exactly reach the next major time level
  for( grid=0; grid<numberOfComponentGrids; grid++ )
  {
    real dtv=variableDt(grid);
    int numberOfSteps=max(1,int((tNext-t0)/dtv+.9999));  
    dtv=(tNext-t0)/numberOfSteps;

    variableDt(grid) = dtv;
  }
  real dtMax=max(variableDt);
  real dtMin=min(variableDt);
  for( grid=0; grid<numberOfComponentGrids; grid++ )
    printf("variableTimeStep: grid=%i dtv=%e, dtv/dtMin=%8.2e (tMin=%8.2e, tMax=%8.2e)\n",grid,
             variableDt(grid),variableDt(grid)/dtMin,dtMin,dtMax);
  


  // real time0=getCPU();
  if( m0Pointer==NULL )
  {
    m0Pointer=new IntegerArray;
    m1Pointer=new IntegerArray;
  }
  IntegerArray & m0=*m0Pointer;
  IntegerArray & m1=*m1Pointer;
  
  if( init )
  {
    // **** To initialize the method we need to compute du/dt at times t and t-dt *****

    if( !gf[current].u.getInterpolant()->interpolationIsExplicit() )
    {
      printf("advanceVariableTimeStepAdamsPredictorCorrector:ERROR: variable time stepping requires the grid\n"
             " to have explicit interpolation\n");
      Overture::abort("error");
    }

    m0.redim(numberOfComponentGrids);
    m0=mab0;
    m1.redim(numberOfComponentGrids);
    m1=mab1;

    if( parameters.dbase.get<bool >("twilightZoneFlow") )
    {
      OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

      
      // get solution and derivative at t-dt
      gf[mab1].t=t0-dt0; 

      for( grid=0; grid<gf[mab1].cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & c = gf[mab1].cg[grid];
	getIndex(c.dimension(),I1,I2,I3);

        real dtv=variableDt(grid); // dt0;

        Range Na(0,parameters.dbase.get<int >("numberOfComponents")-1);
        gf[mab1].u[grid](I1,I2,I3,Na)=e(c,I1,I2,I3,Na,t0-dtv); 

	if( parameters.useConservativeVariables() )
	{
	  printf("*********  advanceAdamsPredictorCorrector: convert primitiveToConservative\n");
	  gf[mab0].primitiveToConservative();
	  gf[mab1].primitiveToConservative();
	}

	// fn[mab1][grid](I1,I2,I3,N)=e.t(c,I1,I2,I3,N,t0-dt0); 
        real t1=t0-dtv;
	rparam[0]=t1;
	rparam[1]=t1; // tforce
	rparam[2]=t1; // tImplicit
	iparam[0]=grid;
        iparam[1]=gf[mab1].cg.refinementLevelNumber(grid);
        iparam[2]=numberOfStepsTaken;
        // mappedGridSolver[grid]->getUt(gf[mab1].u[grid],gf[mab1].getGridVelocity(grid),fn[mab1][grid],iparam,rparam);
        getUt(gf[mab1].u[grid],gf[mab1].getGridVelocity(grid),fn[mab1][grid],iparam,rparam);

      }
    }
    else
    {
      cout << " **************** Adams: still need correct initial values for du/dt(t-dt)  ****** \n";
      cout << " **************** use values from du/dt(t)                                  ****** \n";
      // ****** no need to compute at time t any more *****
      // fn[mab0] <- du/dt(t0)
      //      getUt( gf[mab0].t,gf[mab0],fn[mab0],gf[mab0].t);

      gf[mab1].u=gf[mab0].u;  // 990903 give initial values to avoid NAN's at ghost points for CNS
      if( parameters.useConservativeVariables() )
      {
	printf("*********  advanceAdamsPredictorCorrector: convert primitiveToConservative\n");
	gf[mab0].primitiveToConservative();
	gf[mab1].primitiveToConservative();
      }
      
      for( grid=0; grid<gf[mab0].cg.numberOfComponentGrids(); grid++ )
      {
        // we don't real need du/dt(t0) but we do need du/dt(t0-dt)
	real t1=t0-variableDt(grid);
	rparam[0]=t1;
	rparam[1]=t1; // tforce
	rparam[2]=t1; // tImplicit
	iparam[0]=grid;
        iparam[1]=gf[mab0].cg.refinementLevelNumber(grid);
        iparam[2]=numberOfStepsTaken;
        // mappedGridSolver[grid]->getUt(gf[mab0].u[grid],gf[mab0].getGridVelocity(grid),fn[mab0][grid],iparam,rparam);
        getUt(gf[mab0].u[grid],gf[mab0].getGridVelocity(grid),fn[mab0][grid],iparam,rparam);
      }
      
      for( int grid=0; grid<gf[mab1].cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & c = gf[mab1].cg[grid];
	getIndex(c.dimension(),I1,I2,I3);
	fn[mab1][grid](I1,I2,I3,N)=fn[mab0][grid](I1,I2,I3,N);
      }


    }
    

    CompositeGrid & cg = gf[mab0].cg;

    numberOfTimeLevels=2; // ****************************************
    
    tv.redim(cg.numberOfComponentGrids());
    tv=t0;
    tvb.redim(cg.numberOfComponentGrids());
    tvb=tv-variableDt; // dt0;
    
    Range G=numberOfComponentGrids;
    tv0.redim(cg.numberOfComponentGrids(),numberOfTimeLevels,cg.numberOfComponentGrids());
    tv0(G,0,G)=t0;
    for( int m=1; m<numberOfTimeLevels; m++ )
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
        tv0(G,m,grid)=t0-m*variableDt(G); // dt0;

    if( cg.numberOfComponentGrids()>1 )
    {
      assert( ui==NULL );
      ui = new realArray [cg.numberOfComponentGrids()];
    }
    
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      // variableTime(grid,0)=t0;
      // variableTime(grid,1)=t0-variableDt(grid);

      // ui[grid]( i, 
      int ni=cg.numberOfInterpolationPoints(grid);
      Range R=ni;
      if( ni>0 )
      {
        ui[grid].redim(ni,parameters.dbase.get<int >("numberOfComponents"),numberOfTimeLevels);

	const intArray & ip = cg.interpolationPoint[grid];
	const int ip3=cg[grid].dimension(Start,axis3);

	for( int m=0; m<2; m++ )
	{
	  realArray & u = m==0 ? gf[mab0].u[grid] : gf[mab1].u[grid];
	  for( int n=0; n<parameters.dbase.get<int >("numberOfComponents"); n++ )
	  {
	    if( cg.numberOfDimensions()==2 )
	      ui[grid](R,n,m)=u(ip(R,0),ip(R,1),ip3,n);
	    else
	      ui[grid](R,n,m)=u(ip(R,0),ip(R,1),ip(R,2),n);

	  }
	}
      }
    }

    init=FALSE;
  }
  else
  {
//     if( parameters.useConservativeVariables() )
//     {
//       printf("*********  variablePC: convert primitiveToConservative\n");
//       gf[mab0].primitiveToConservative();
//       gf[mab1].primitiveToConservative();
//     }
  }
  
  if( debug() & 2 )
  {
    if( twilightZoneFlow() )
    {
      fprintf(parameters.dbase.get<FILE* >("debugFile")," variablePC: Errors at start t=%e  \n",gf[mab0].t);
      determineErrors( gf[mab0] );
    }
    else
    {
      fprintf(parameters.dbase.get<FILE* >("debugFile")," variablePC: Solution at start t=%e  \n",gf[mab0].t);
      outputSolution( gf[mab0].u,gf[mab0].t );
    }
    
  }

      
  Range G=gf[mab0].cg.numberOfComponentGrids();
  real tMin= min(variableTime(G,0));

  const int maximumNumberOfSteps=int(numberOfComponentGrids*numberOfSubSteps*dt0/dtMin+10);

  for( int mst=1; mst<maximumNumberOfSteps; mst++ )
  {
    // choose the grid to advance next (the one with minimum time)
    int nextGrid=0;
    real tMin=variableTime(nextGrid,0);
    for( grid=1; grid<numberOfComponentGrids; grid++ )
    {
      if( variableTime(grid,0)<tMin )
      {
	tMin=variableTime(grid,0);
	nextGrid=grid;
      }
    }
    if( tMin >= tNext -variableDt(nextGrid)*.001 )
      break;  // We are done 


    grid=nextGrid;
    const real dtv = variableDt(grid);
    t0=variableTime(grid,0);

    int mab0=m0(grid);
    int mab1=m1(grid);

    realCompositeGridFunction & ua = fn[mab0];   // pointer to du/dt
    realCompositeGridFunction & ub = fn[mab1];   // pointer to du(t-dt)/dt
    
    printf("*** VST: advance grid %i, t=%e, dtv=%e \n",nextGrid,variableTime(grid,0),dtv);
    fprintf(parameters.dbase.get<FILE* >("debugFile"),"*** VST: advance grid %i, t=%e, dtv=%e \n",nextGrid,variableTime(grid,0),dtv);


    // interpolate in time : points on grid 
    variableTimeStepBoundaryInterpolation( nextGrid,gf[mab0] );

    gf[mab0].t=t0;
    applyBoundaryConditions(gf[mab0],-1,nextGrid);

    //  Compute ua = d(u(t)/dt 
    rparam[0]=t0;
    rparam[1]=t0; // tforce
    rparam[2]=t0; // tImplicit
    iparam[0]=grid;
    iparam[1]=gf[mab0].cg.refinementLevelNumber(grid);
    iparam[2]=numberOfStepsTaken;
    // mappedGridSolver[grid]->getUt(gf[mab0].u[grid],gf[mab0].getGridVelocity(grid),ua[grid],iparam,rparam);
    getUt(gf[mab0].u[grid],gf[mab0].getGridVelocity(grid),ua[grid],iparam,rparam);

    gf[mab0].t=t0;
    
    if( debug() & 32 )
    {
      fprintf(parameters.dbase.get<FILE* >("debugFile")," AdamsPC: ua=du/dt t=%e  \n",gf[mab0].t);
      determineErrors( ua,gf[mab0].gridVelocity, gf[mab0].t, 1, error );
    }
    if( debug() & 64 )
    {
      for( grid=0; grid<gf[mab0].cg.numberOfComponentGrids(); grid++ )
      {
        display(ua[grid],"\n ****ua : du/dt(t)",parameters.dbase.get<FILE* >("debugFile"),"%7.1e ");
        display(ub[grid],"\n ****ub : du/dt(t-dt)",parameters.dbase.get<FILE* >("debugFile"),"%7.1e ");
      }
    }
    if( debug() & 16 || debug() & 64 )
    {
      fprintf(parameters.dbase.get<FILE* >("debugFile")," adams: errors in ut (ua) at t=%e \n",t0);
      determineErrors( ua,gf[mab0].gridVelocity, t0, 1, error );
      fprintf(parameters.dbase.get<FILE* >("debugFile")," adams: errors in ut (ub) at t=%e \n",tvb(nextGrid));
      determineErrors( ub,gf[mab1].gridVelocity, tvb(nextGrid), 1, error );
    }

    // **** start with the Adams Moulton corrector****
    real dtOld=tv(nextGrid)-tvb(nextGrid); // old dt
    real am1 = .5*dtOld;
    real am2=  .5*dtOld;

    fprintf(parameters.dbase.get<FILE* >("debugFile"),">>>  corrector: nextGrid=%i, mab0=%i mab1=%i dtOld=%e\n",nextGrid,mab0,mab1,dtOld);
    
    getIndex(gf[mab1].cg[grid].extendedIndexRange(),I1,I2,I3);
    gf[mab0].u[grid](I1,I2,I3,N)=gf[mab1].u[grid](I1,I2,I3,N) + am1*ua[grid](I1,I2,I3,N) + am2*ub[grid](I1,I2,I3,N);

    if( debug() & 16 || debug() & 2 )
    {
      fprintf(parameters.dbase.get<FILE* >("debugFile")," ***Adams PC: before interpolate t=%e\n",gf[mab1].t);
      if( twilightZoneFlow() )
	determineErrors( gf[mab0] );
      else
	outputSolution( gf[mab0].u,gf[mab1].t );
    }

    // interpolate corrected value in time
    variableTimeStepBoundaryInterpolation( nextGrid,gf[mab0] );

    if( debug() & 16 || debug() & 2 )
    {
      fprintf(parameters.dbase.get<FILE* >("debugFile")," ***Adams PC: after interpolate t=%e\n",gf[mab1].t);
      if( twilightZoneFlow() )
	determineErrors( gf[mab0] );
      else
	outputSolution( gf[mab0].u,gf[mab1].t );
    }

    gf[mab0].t=t0;
    applyBoundaryConditions(gf[mab0],-1,nextGrid);


    // *** adams predictor ****

    rparam[0]=t0;
    rparam[1]=t0; // tforce
    rparam[2]=t0; // tImplicit
    iparam[0]=grid;
    iparam[1]=gf[mab0].cg.refinementLevelNumber(grid);
    iparam[2]=numberOfStepsTaken;
    // mappedGridSolver[grid]->getUt(gf[mab0].u[grid],gf[mab0].getGridVelocity(grid),ub[grid],iparam,rparam);
    getUt(gf[mab0].u[grid],gf[mab0].getGridVelocity(grid),ub[grid],iparam,rparam);

    real ab1=dtv*(1.+dtv/(2.*dtOld));
    real ab2=-dtv*dtv/(2.*dtOld);
    

    getIndex(gf[mab1].cg[grid].extendedIndexRange(),I1,I2,I3);
    gf[mab1].u[grid](I1,I2,I3,N)=gf[mab0].u[grid](I1,I2,I3,N)+ab1*ub[grid](I1,I2,I3,N) + ab2*ua[grid](I1,I2,I3,N);

    t0=t0+dtv;      // **** 
    
    gf[mab1].t=t0;  // gf[mab1] now lives at this time

    tvb(nextGrid)=tv(nextGrid);
    tv(nextGrid)+=dtv;
    variableTime(nextGrid,0)=tv(nextGrid);
    
    updateVariableTimeInterpolation( nextGrid,gf[mab1] );

    if( debug() & 16 || debug() & 2 )
    {
      fprintf(parameters.dbase.get<FILE* >("debugFile")," ***Adams PC: after interpolate (predictor), t=%e\n",gf[mab1].t);
      if( twilightZoneFlow() )
	determineErrors( gf[mab1] );
      else
	outputSolution( gf[mab1].u,gf[mab1].t );
    }

    // switch mab0 <-> mab1
    m0(nextGrid) = (m0(nextGrid)+1) % 2;
    m1(nextGrid) = (m1(nextGrid)+1) % 2;


    if( (mst-1) % 10 == 0 )
      saveSequenceInfo(t0,fn[mab1]);

    output( gf[mab0],initialStep+mst-1 ); // output to files, user defined output

    if( true )//|| parameters.dbase.get<Parameters::PDE >("pde")==Parameters::incompressibleNavierStokes )
    {
      const int zeroUnusedPointsAfterThisManySteps=20;
      if( TRUE || mst==numberOfSubSteps || (mst % zeroUnusedPointsAfterThisManySteps)==0 ) // mst starts at 1
      {
	// fix up unused points to keep them from getting too big 
	for( int m=0; m<=1; m++ )
	  fixupUnusedPoints(gf[m].u);
      }
    }
    
  }

  printf("*** VST: end of step: times: ");
  for( grid=0; grid<numberOfComponentGrids; grid++ )
    printf(" t(%i)=%e ",grid,variableTime(grid,0));
  printf("\n");
  
  // update the current solution:  
  // Note that the current solution is scattered amongst gf[0].u and gf[1].u
  current=mab0;

//   for( grid=0; grid<numberOfComponentGrids; grid++ )
//   {
//     // solution.u[grid].reference(gf[m0(grid)].u[grid]);
//     solution.u[grid]=gf[m0(grid)].u[grid];
//     if( movingGridProblem() )  // 990826
//     {
//       if( this!=NULL )
//         Overture::abort("error");
	
//       solution.cg.reference(gf[mab0].cg);
//       // solution.gridVelocity.reference(gf[mab0].gridVelocity);
//       solution.referenceGridVelocity(gf[mab0]);
//     }
//   }

//   // we need to update boundary values of solution
//   solution.form=gf[0].form;   // in case we are using conservative variables
//   solution.t=tNext;

  gf[0].t=tNext;
  gf[1].t=tNext;
  
//   if( debug() & 16 || debug() & 2 )
//   {
//     fprintf(parameters.dbase.get<FILE* >("debugFile")," ***Adams PC: solution before applyBC t=%e\n",solution.t);
//     outputSolution( gf[current].u,gf[current].t );
//   }

//   // interpolate( solution );   // **** some interpolation points are not final values ??
//   // applyBoundaryConditions(solution); 
//   interpolateAndApplyBoundaryConditions( solution );

//   if( debug() & 16 || debug() & 2 )
//   {
//     fprintf(parameters.dbase.get<FILE* >("debugFile")," ***Adams PC: solution after applyBC t=%e\n",solution.t);
//     outputSolution( solution.u,solution.t );
//   }

//   // should just turn off the prim to conserv in applyBoundaryCondition
//   if( parameters.useConservativeVariables() )
//     solution.conservativeToPrimitive();  

  
}

