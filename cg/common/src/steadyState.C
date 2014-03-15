#include "DomainSolver.h"
#include "CompositeGridOperators.h"
#include "GridCollectionOperators.h"
#include "PlotStuff.h"
#include "LineSolve.h"
#include "AdamsPCData.h"

//==================================================================
// 
//            "Steady state" solver 
//            ---------------------
//              1. Line solver
//              2. Runge Kutta Routine
//
//==================================================================


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{advanceSteadyStateRungeKutta}} 
void DomainSolver::
advanceSteadyStateRungeKutta( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  )
//=====================================================================================================
// /Description:
//   Advance some time steps. Use a line-solver or a steady state Runge Kutta routine
//
// /t0,dt0 (input) : current time and time step.
// /numberOfSubSteps (input) : take this many steps
// /init (input) : if TRUE this is the first time step in which case this routine will initialize itself.
//
// /Notes:
//   
//\end{CompositeGridSolverInclude.tex}  
//================================================================================================
{
  FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");


  if( debug() & 2 )
    printF(" *** Entering advanceSteadyStateRungeKutta: t0=%e, dt0=%e *** \n",t0,dt0);

  if( !parameters.dbase.get<DataBase >("modelData").has_key("steadyStateRungeKuttaData") )
    parameters.dbase.get<DataBase >("modelData").put<AdamsPCData>("steadyStateRungeKuttaData");
  AdamsPCData & adamsData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("steadyStateRungeKuttaData");
  
  int &mab0 =adamsData.mab0, &mab1=adamsData.mab1;

  if(  debug() & 2 )
    fprintf(pDebugFile," *** Entering advanceSteadyStateRungeKutta: t0=%e, dt0=%e *** \n",t0,dt0);
 
  const int numberOfDimensions=gf[current].cg.numberOfDimensions();

  int mSolution=mab0;  // save initial value
  
  const int pc=parameters.dbase.get<int >("pc");
  const int uc=parameters.dbase.get<int >("uc");
  const int vc=parameters.dbase.get<int >("vc");
  const int wc=parameters.dbase.get<int >("wc");

  int grid;
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];  
  Range N = parameters.dbase.get<Range >("Rt");   // time dependent variables
  realArray error(numberOfComponents()+3); 
  Range C=parameters.dbase.get<int >("numberOfComponents");
  int iparam[10];
  real rparam[10];

  if( parameters.useConservativeVariables() )
    {
      gf[0].primitiveToConservative();  // *wdh* 010318  -- do amr interpolation on conservative variables.
      gf[1].primitiveToConservative();
    }
  
  if( init )
  {
    init=FALSE;
  }
  else
  {
  }
  
  if( debug() & 4 )
  {
    if( false && twilightZoneFlow() )
    {
      fPrintF(debugFile," SSRK: Errors at start t=%e  \n",gf[mab0].t);
      determineErrors( gf[mab0] );
    }
    else
    {
      fPrintF(debugFile," SSRK: Solution at start t=%e  \n",gf[mab0].t);
      outputSolution( gf[mab0].u,gf[mab0].t );
    }
  }

  bool useLineSolver=parameters.dbase.get<bool>("useLineSolver");
  if( useLineSolver )
  {
    if( pLineSolve==NULL )
    {
      pLineSolve=new LineSolve;
    }

    mab0=0;
    mab1=1;
    
    if( debug() & 2 )
      printF(" >>>> SteadyState line solve: numberOfSubSteps=%i globalStepNumber=%i\n",numberOfSubSteps,parameters.dbase.get<int >("globalStepNumber"));
    
    for( int mst=1; mst<=numberOfSubSteps; mst++ )
    {
      parameters.dbase.get<int >("globalStepNumber")++;
    
      realCompositeGridFunction & f = fn[0];   
      realCompositeGridFunction & u0 = gf[mab0].u; 
      realCompositeGridFunction & residual = gf[mab1].u; 
      CompositeGrid & cg = gf[current].cg;
      
      real time0=getCPU();

      if( debug() & 64 )
      {
        Index I1,I2,I3;
	for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  getIndex(cg[grid].gridIndexRange(),I1,I2,I3);
	  I1=Range(-1,1);
	  display(u0[grid](I1,I2,I3,Range(uc,vc)),"\n SSRK: u0 BEFORE advanceLineSolve",debugFile,"%7.4f ");
	}
      }


      bool refactor = true;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	assert( pdtVar !=NULL );
	realArray & dtVar = (*pdtVar)[grid];
	
	// display(dtVar,"SteadyState line solve: dtVar","%4.2f ");
	
        // for( int direction=numberOfDimensions-1; direction>=0; direction-- )
        // for( int direction=2; direction<=2; direction++ )
        // for( int direction=1; direction<=1; direction++ )
        // for( int direction=0; direction<=0; direction++ )
        for( int direction=0; direction<numberOfDimensions; direction++ )
	{
          advanceLineSolve(*pLineSolve,grid,direction,u0, f[grid], residual[grid], refactor );
	}
	
	
      }
      gf[mab0].t=t0+dt0;  // gf[mab0] now lives at this time

      parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForLineImplicit"))+=getCPU()-time0;

      if( debug() & 64 )
      {
        Index I1,I2,I3;
	for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  getIndex(cg[grid].gridIndexRange(),I1,I2,I3);
	  I1=Range(-1,1);
	  display(u0[grid](I1,I2,I3,Range(uc,vc)),"\n SSRK: u0 AFTER advanceLineSolve",debugFile,"%7.4f ");
	}
      }
      

      if( true )
      {
	interpolateAndApplyBoundaryConditions( gf[mab0] );
      }
      else
      {
 	Range V(uc,uc+numberOfDimensions-1);
	gf[mab0].u.applyBoundaryCondition(V,BCTypes::generalizedDivergence,Parameters::slipWall,0.,gf[mab0].t); 	
	gf[mab0].u.finishBoundaryConditions();
      }
      
      if( debug() & 64 )
      {
        Index I1,I2,I3;
	for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  getIndex(cg[grid].gridIndexRange(),I1,I2,I3);
	  I1=Range(-1,1);
	  display(u0[grid](I1,I2,I3,Range(uc,vc)),"\n SSRK: u0 after BC",debugFile,"%7.4f ");
	}
      }


      if( debug() & 16  )
      {
	fPrintF(debugFile," ***SSRK: after apply boundary Conditions %i t=%e\n",gf[mab0].t);
	outputSolution( gf[mab0].u,gf[mab0].t );
      }

      solveForTimeIndependentVariables( gf[mab0] ); 

      
      if( debug() & 8 )
      {
	fPrintF(debugFile," SSRK: After pressure solve, t0+dt0: t0=%e, dt0=%e  \n",t0,dt0);
	outputSolution( gf[mab0].u,gf[mab0].t );
      }

      // compute the residual every few steps
      int frequencyToOutputResidual=parameters.dbase.get<int >("frequencyToSaveSequenceInfo"); // 10;
      
      if( ((mst-1) % frequencyToOutputResidual == 0) || 
          mst==numberOfSubSteps ||  // compute residual for plotting
          debug() & 2 ) 
      {
	
	if( true )
	{
	  // here is the efficient version of computing the residual
	  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    assert( pdtVar !=NULL );
	    realArray & dtVar = (*pdtVar)[grid];
	
            bool computeTheResidual=true;
	    int direction=0; 
            bool refactor=false;
            // **** note: residual is returned in fn[0]
	    advanceLineSolve(*pLineSolve,grid,direction,u0, residual[grid], fn[0][grid], 
                             refactor, computeTheResidual );

            if( debug() & 4 )
	      display(fn[0][grid],"\n SSRK: residual ",debugFile,"%8.1e ");

	  }
	}
	
        if( false && debug() & 4  )
	{
	  time0=getCPU();
	  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    rparam[0]=gf[mab0].t;
	    rparam[1]=gf[mab0].t;
	    rparam[2]=gf[mab0].t; // tImplicit
	    iparam[0]=grid;
	    iparam[1]=gf[mab0].cg.refinementLevelNumber(grid);
            iparam[2]=numberOfStepsTaken;
           
	    // mappedGridSolver[grid]->getUt(gf[mab0].u[grid],gf[mab0].getGridVelocity(grid),f[grid],iparam,rparam);
	    getUt(gf[mab0].u[grid],gf[mab0].getGridVelocity(grid),f[grid],iparam,rparam);
	  }
	  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForGetUt"))+=getCPU()-time0;
      

	  // give zero BC's for residual
	  // fn[0].getOperators()->setTwilightZoneFlow(false);
	  // fn[0].applyBoundaryCondition(N,BCTypes::dirichlet,BCTypes::allBoundaries,0.,gf[mab0].t);
	  // fn[0].interpolate();  // interpolate the residual for plotting 
	  // fn[0].getOperators()->setTwilightZoneFlow(parameters.dbase.get<bool >("twilightZoneFlow"));
	  
	}

        // *wdh* 081116 - use the iteration count instead of the pseduo time for residual plots
        // real timeForSequence = t0;
        real timeForSequence=parameters.dbase.get<int >("globalStepNumber")+1;
	saveSequenceInfo(timeForSequence,fn[0]);

      }
      
      t0+=dt0;
      output( gf[mab0],initialStep+mst-1 ); // output to files, user defined output

    }
    
  }
  else
  {
    // ***************************************
    // ***********  Runge Kutta **************
    // ***************************************

    const int numberOfStages=5;
    const real crk[numberOfStages]={1./4.,1./6.,3./8.,1./2.,1.}; //
    for( int mst=1; mst<=numberOfSubSteps; mst++ )
    {
      //       ---5-stage Runge Kutta
      //           u(0) <- u(t)
      //           u(k) <- u(0) + c_k du/dt  k=1,2,...,p
      //           u(t+dt)=u(p)
      //      
      parameters.dbase.get<int >("globalStepNumber")++;
    
      realCompositeGridFunction & ua = fn[0];   // pointer to du/dt

      for( int stage=0; stage<numberOfStages; stage++ )
      {
	const int mk = stage==0 ? mab0 : mab1;

        if( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::lineImplicit )
	{
	}
	

	//  Compute ua = d(u(stage)/dt 
	for( grid=0; grid<gf[mk].cg.numberOfComponentGrids(); grid++ )
	{
	  rparam[0]=gf[mk].t;
	  rparam[1]=gf[mk].t;
	  rparam[2]=gf[mk].t; // tImplicit
	  iparam[0]=grid;
	  iparam[1]=gf[mk].cg.refinementLevelNumber(grid);
          iparam[2]=numberOfStepsTaken;
//	  mappedGridSolver[grid]->getUt(gf[mk].u[grid],gf[mk].getGridVelocity(grid),ua[grid],iparam,rparam,Overture::nullRealMappedGridFunction(),&gf[mk].cg[grid]);
	  getUt(gf[mk].u[grid],gf[mk].getGridVelocity(grid),ua[grid],iparam,rparam,
                Overture::nullRealMappedGridFunction(),&gf[mk].cg[grid]);
	}
    
	if( debug() & 8 || debug() & 64 )
	{
	  fPrintF(debugFile," SSRK: Stage %i: mab0=%i mab1=%i mk=%i \n",stage,mab0,mab1,mk);
	  for( grid=0; grid<gf[mab0].cg.numberOfComponentGrids(); grid++ )
	    display(ua[grid],"\n SSRK: ****ua : du/dt(t) ",debugFile,"%8.1e ");
	}
//        if( debug() & 64 )
//        {
//  	fPrintF(debugFile," SSRK: errors in ut (ua) at t=%e \n",t0);
//  	determineErrors( ua,gf[mab0].gridVelocity, t0, 1, error );
//        }

	real cpu0=getCPU();
	real ckdt = crk[stage]*dt0;
	for( grid=0; grid<gf[mab1].cg.numberOfComponentGrids(); grid++ )
	{
	  assert( pdtVar !=NULL );
	  realArray & dtVar = (*pdtVar)[grid];

	  getIndex(gf[mab1].cg[grid].extendedIndexRange(),I1,I2,I3);
	  if( parameters.dbase.get<int >("useLocalTimeStepping") )
	  {
	    for( int n=N.getBase(); n<=N.getBound(); n++ )
	    {
	      gf[mab1].u[grid](I1,I2,I3,n)=gf[mab0].u[grid](I1,I2,I3,n) + 
		(crk[stage]*parameters.dbase.get<real >("cfl"))*dtVar(I1,I2,I3)*ua[grid](I1,I2,I3,n);
	    }
	  }
	  else
	  {
	    gf[mab1].u[grid](I1,I2,I3,N)=gf[mab0].u[grid](I1,I2,I3,N) + ckdt*ua[grid](I1,I2,I3,N);
	  }
	
	
	}
	gf[mab1].t=t0+crk[stage]*dt0;  // gf[mab1] now lives at this time
	gf[mab1].form=gf[mab0].form;

	parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAddUt"))+=getCPU()-cpu0;

	interpolateAndApplyBoundaryConditions( gf[mab1] );

	if( debug() & 16 || debug() & 2 )
	{
	  fPrintF(debugFile," ***SSRK: after apply boundary Conditions at stage %i t=%e\n",stage,gf[mab1].t);
	  if( false && twilightZoneFlow() )
	    determineErrors( gf[mab1] );
	  else
	    outputSolution( gf[mab1].u,gf[mab1].t );
	}

	solveForTimeIndependentVariables( gf[mab1] ); 

	if( debug() & 4 )
	  printF("  SSRK: Stage=%i After pressure solve: max(fabs(gf[mab1]))=%e \n",stage,max(fabs(gf[mab1].u)));
	if( debug() & 8 || debug() & 2 )
	{
	  fPrintF(debugFile," SSRK: Stage %i: After pressure solve, t0+dt0: t0=%e, dt0=%e  \n",stage,t0,dt0);
	  if(false &&  twilightZoneFlow() )
	    determineErrors( gf[mab1] );
	  else
	    outputSolution( gf[mab1].u,gf[mab1].t );
	}
      
      } // end for stage
    
      // switch mab0 <-> mab1
      mab0 = (mab0+1) % 2;
      mab1 = (mab1+1) % 2;
      t0+=dt0;

      // give zero BC's for residual
      fn[0].getOperators()->setTwilightZoneFlow(false);
      fn[0].applyBoundaryCondition(N,BCTypes::dirichlet,BCTypes::allBoundaries,0.,gf[mab0].t);
      fn[0].interpolate();  // interpolate the residual for plotting 
      fn[0].getOperators()->setTwilightZoneFlow(parameters.dbase.get<bool >("twilightZoneFlow"));
      if( (mst-1) % 10 == 0 )
	saveSequenceInfo(t0,fn[0]);

      output( gf[mab0],initialStep+mst-1 ); // output to files, user defined output

    }
  
  }
  

  if( parameters.useConservativeVariables() )
    gf[mab0].conservativeToPrimitive();
  
  // update the current solution:  
  current = mab0;
  
//   if( mab0!=mSolution || parameters.isAdaptiveGridProblem() )
//   {
//     solution.u.reference(gf[mab0].u);
//     if( movingGridProblem()  || parameters.isAdaptiveGridProblem() )  // 990826
//     {
//       solution.cg.reference(gf[mab0].cg);
//       // solution.gridVelocity.reference(gf[mab0].gridVelocity);
//       solution.referenceGridVelocity(gf[mab0]);
//     }
//   }
//   solution.t=gf[mab0].t;

}

