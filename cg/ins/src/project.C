#include "Cgins.h"
#include "ProjectVelocity.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"

namespace {
  ProjectVelocity projector;  // ******************* fix this *******************
}


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{project}} 
int Cgins::
project(GridFunction & cgf)
// ===================================================================================
// /Description:
//   Project the solution to be divergence free (approximately)
// The projection uses the Oges object "poisson" from this class -- it thus will
// use the same parameter values set in OverBlown. The projection Poisson equation uses
// Neumann BC's all around so that it may have different BC's from the NS equations.
//\end{CompositeGridSolverInclude.tex}  
// ===================================================================================
{
  if( false )
  {
    printF("**** Cgins::project  START\n");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }


  if( parameters.dbase.get<int>("simulateGridMotion")>0 ) return 0;
  
  real time0=getCPU();

  FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");

  if( parameters.dbase.get<bool >("projectInitialConditions") )
  {
    printF(">>>>>Cgins::project: project the initial conditions <<<<\n");
    if( debug() & 2 )
      fPrintF(debugFile,">>>>>Cgins::project: project the initial conditions <<<<\n");

    if( Parameters::checkForFloatingPointErrors )
      checkSolution(cgf.u,"Cgins::project: cgf.u at start");

    // *new* way, avoids generating a separate matrix for the projection operator
    CompositeGrid & cg = cgf.cg;
    realCompositeGridFunction & u = cgf.u;

    const int & numberOfDimensions = parameters.dbase.get<int >("numberOfDimensions");
    const int & orderOfAccuracy = min(4,parameters.dbase.get<int >("orderOfAccuracy")); // kkc 101116 added min
    
    const Parameters::TimeSteppingMethod & timeSteppingMethod = 
      parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
    const Parameters::ImplicitMethod &method = parameters.dbase.get<Parameters::ImplicitMethod>("implicitMethod");
    
    const bool isFactoredScheme = (timeSteppingMethod==Parameters::implicit && method==Parameters::approximateFactorization);


    int numberOfSmoothsPerProjection=5;
    projector.setNumberOfSmoothsPerProjectionIteration(numberOfSmoothsPerProjection);
    projector.setVelocityComponent(parameters.dbase.get<int >("uc"));
    projector.setPoissonSolver(poisson);
    projector.setCompare3Dto2D(parameters.dbase.get<int >("compare3Dto2D"));
    projector.setIsAxisymmetric(parameters.isAxisymmetric());

    const int uc = parameters.dbase.get<int >("uc");
    const int vc = parameters.dbase.get<int >("vc");
    const int wc = parameters.dbase.get<int >("wc");

    Range V(uc,uc+numberOfDimensions-1);

    int numberOfSmoothsPerProjectionIteration=5;
    int minimumNumberOfProjectionIterations=1;
    real convergenceTolerance=.95;
    // int numberOfSmoothingSubIterations=1;  // only do 1 because of BC's and interp pts      

    int maximumNumberOfProjectionIterations=9;
    int nitsm=10;  // number of smoothing steps

    // ********************
#ifdef USE_PPP
    //   maximumNumberOfProjectionIterations=1;
    //   nitsm=1;
#endif      

    real divMax;
    real divMaxOld=1.e6;

    const real cdv=parameters.dbase.get<real >("cdv");
    const real advectionCoefficient=parameters.dbase.get<real >("advectionCoefficient");
    const real nu = parameters.dbase.get<real >("nu"); // save this value

    // ------------------------------------------------------------------------------
    // --- Set parameters so that the RHS to the pressure equation is just div(u) ---
    // ------------------------------------------------------------------------------

    parameters.dbase.get<int>("initialConditionsAreBeingProjected")=1; // new way, this is used in assignPressureRHS

    parameters.dbase.get<real >("cdv")=1.e6;
    parameters.dbase.get<real >("advectionCoefficient")=0.;
    // *wdh* Only set nu to zero just before pressure solve as BC's in smooth etc. may use nu (e.g. order=4)
    // parameters.dbase.get<real >("nu")=0.;
      
    dt=parameters.dbase.get<real >("cDt");  // this will make the divergenceDampingWeight==1.
      

    // For 2nd-order : extrap order = 3 seems to work best
    BoundaryConditionParameters bcParams;
    // bcParams.orderOfExtrapolation=2;  // *wdh* 100816
    bcParams.orderOfExtrapolation=3;  // *wdh* 100816
    
    // bcParams.ghostLineToAssign= orderOfAccuracy/2;  // *note*
      
    // this will create the pressure equation.
    updateToMatchGrid(gf[0].cg); 


    int grid;
    int extra[3] = { 0,0,0 };  
    Range all;
    Index I1,I2,I3;
      
    // for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    // displayMask(cg[grid].mask(),"mask in project",debugFile);

    // realCompositeGridFunction divergence(cg,all,all,all);  // holds divergence  ****** no need ***************
    assert( numberOfExtraFunctionsToUse>0 );
    realCompositeGridFunction divergence;  // use fn[0] as a workspace
#ifdef USE_PPP
    divergence.updateToMatchGrid(cgf.cg); 
#else
    divergence.link(fn[0],Range(0,0));
#endif
    aString buff;
      
    bool overWriteBoundaryConditions=true;
    // overWriteBoundaryConditions=false;  // *wdh* 100816  ************* TEMP*********** try this

    // if( orderOfAccuracy==4 )
    //   overWriteBoundaryConditions=false;  // testing ..

    for( int it=0; it<maximumNumberOfProjectionIterations; it++ )
    {
      // ---smoothing iterations, smooth more the first few times
      int numberOfSmoothingSubIterations = (it<5) ? max(nitsm,numberOfSmoothsPerProjectionIteration) : nitsm; 
      smoothVelocity( cgf,numberOfSmoothingSubIterations ); 

      DomainSolver::applyBoundaryConditions(cgf); //kkc 120323, these get overwritten in smoothVelocity, and then overwritten again below

      if( debug() & 4 || debug() & 32 )
	cgf.u.display(sPrintF(buff,"projectVelocity>>> Solution after smooth velocity: u, it=%i",it),debugFile,"%5.2f ");
	
      if( Parameters::checkForFloatingPointErrors )
        checkSolution(cgf.u,"Cgins::project: cgf.u after smoothVelocity");

      real divMaxAfterSmooth = projector.computeDivergence( u,divergence );   // compute divergence for rhs 
      if( it==0 ) divMaxOld=divMaxAfterSmooth;
      if( debug() & 4 )
	printF(" projectVelocity>>> iteration=%i, divergence after smooth    =%f\n",it,divMaxAfterSmooth);

      if( overWriteBoundaryConditions )
      {
	//kkc 120323 added loop over boundary conditions to skip overwritting of penalty bcs
	for ( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  if( true )
	  {
	    const int nbc=2;  // noSlipWall and outflow
	    for( int ibc=0; ibc<nbc; ibc++ )
	    {
	      int bcToExtrap = ibc==0 ? Parameters::noSlipWall : InsParameters::outflow;
	      bcParams.ghostLineToAssign=1;
	      u[grid].applyBoundaryCondition(V,BCTypes::extrapolate,bcToExtrap,0.,0.,bcParams);
	      if( orderOfAccuracy==4 )
	      {
		bcParams.ghostLineToAssign=2;
		u[grid].applyBoundaryCondition(V,BCTypes::extrapolate,bcToExtrap,0.,0.,bcParams); 
		bcParams.ghostLineToAssign=1;
	      }
	      
	    }
	    
	  }
	  else
	  {
	    // *old* 
	    IntegerArray indexRangeLocal(2,3), dimLocal(2,3), bcLocal(2,3);
	    ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( u[grid],indexRangeLocal,dimLocal,bcLocal ); 

	    for ( int side=0; side<2; side++ )
	      for ( int axis=0; axis<numberOfDimensions; axis++ )
	      {
		// if ( (bcLocal(side,axis)!=Parameters::penaltyBoundaryCondition && !(isFactoredScheme && bcLocal(side,axis)==InsParameters::outflow) ))
		// *wdh* 2012/09/14 -- only over-write noSlipWall (to undo div(u)=0)
		if( bcLocal(side,axis)!=Parameters::penaltyBoundaryCondition && bcLocal(side,axis)==InsParameters::noSlipWall )
		{
		  // *** undo the div(u)=0 BC so the projection will work better on the boundary.
		  if( orderOfAccuracy==2 ) // turn off for 4th order for testing
		  {
		    u[grid].applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::boundary(side,axis),0.,0.,bcParams);
		  }
		  else if( orderOfAccuracy==4 ) // trouble at outflow with standard BC's
		  {
		    u[grid].applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::boundary(side,axis),0.,0.,bcParams);
		    bcParams.ghostLineToAssign=2;
		    u[grid].applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::boundary(side,axis),0.,0.,bcParams); // **** 990106
		    bcParams.ghostLineToAssign=1;
		  }
		}
	      }
	  }
	  
	}
	//	u.finishBoundaryConditions();
      }
      
      parameters.dbase.get<real >("nu")=0.; // so RHS to presure equation is div(u)

      solveForTimeIndependentVariables( cgf ); 

      parameters.dbase.get<real >("nu")=nu; // reset 

      if( debug() & 4 || debug() & 32 )
	p().display(sPrintF(buff,"projectVelocity>>> Pressure after solve: p() it=%i",it),debugFile,"%9.6f ");
        
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	// update interior points (and periodic boundaries)
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	  extra[axis] = cg[grid].isPeriodic()(axis) ? 0 : -1;
	getIndex(extendedGridIndexRange(cg[grid]),I1,I2,I3,extra[0],extra[1],extra[2]); 

	// **** used optimized operators here ****
	MappedGridOperators & op = *(u[grid].getOperators());
	realMappedGridFunction & px = fn[0][grid];   // use fn[0] as a work space
	realMappedGridFunction & pg = p()[grid];
	realArray & uu = u[grid];
	  
	Range V(uc,uc+cg.numberOfDimensions()-1);
	  
#ifdef USE_PPP
	realSerialArray pgLocal;  getLocalArrayWithGhostBoundaries(pg,pgLocal);
	realSerialArray pxLocal;  getLocalArrayWithGhostBoundaries(px,pxLocal);
	realSerialArray uuLocal;  getLocalArrayWithGhostBoundaries(uu,uuLocal);
#else
	realSerialArray & pgLocal = pg;
	realSerialArray & pxLocal = px;
	realSerialArray & uuLocal = uu;
#endif  

	bool ok = ParallelUtility::getLocalArrayBounds(uu,uuLocal,I1,I2,I3);
	if( ok )
	{
	  op.derivative(MappedGridOperators::xDerivative,pgLocal,pxLocal,I1,I2,I3);
	  uuLocal(I1,I2,I3,uc)-=pxLocal(I1,I2,I3,0);
	  op.derivative(MappedGridOperators::yDerivative,pgLocal,pxLocal,I1,I2,I3);
	  uuLocal(I1,I2,I3,vc)-=pxLocal(I1,I2,I3,0);
	  if( cg.numberOfDimensions()==3 )
	  {
	    op.derivative(MappedGridOperators::zDerivative,pgLocal,pxLocal,I1,I2,I3);
	    uuLocal(I1,I2,I3,wc)-=pxLocal(I1,I2,I3,0);
	  }
	}
	if( false ) // old way
	{
	  op.derivative(MappedGridOperators::xDerivative,pg,px,I1,I2,I3);

	  // px=0;
	  
	  uu(I1,I2,I3,uc)-=px(I1,I2,I3,0);

	  if( debug() & 4 )
	  {
	    ::display(pg,sPrintF(buff,"projectVelocity>>> p, grid=%i it=%i",grid,it),debugFile,"%5.2f ");
	  }
	  if( debug() & 8 )
	  {
	    ::display(px,sPrintF(buff,"projectVelocity>>> px, grid=%i it=%i",grid,it),debugFile,"%5.2f ");
	  }
	  op.derivative(MappedGridOperators::yDerivative,pg,px,I1,I2,I3);
	  uu(I1,I2,I3,vc)-=px(I1,I2,I3,0);
	  if( cg.numberOfDimensions()==3 )
	  {
	    op.derivative(MappedGridOperators::zDerivative,pg,px,I1,I2,I3);
	    uu(I1,I2,I3,wc)-=px(I1,I2,I3,0);
	  }
	}
	  
	if( debug() & 8 )
	{
	  ::display(px,sPrintF(buff,"projectVelocity>>> py, grid=%i it=%i",grid,it),debugFile,"%5.2f ");
	}

	// 	  u[grid](I1,I2,I3,uc  )-=p()[grid].x(I1,I2,I3)(I1,I2,I3);
	// 	  u[grid](I1,I2,I3,uc+1)-=p()[grid].y(I1,I2,I3)(I1,I2,I3);
	// 	  if( cg.numberOfDimensions()==3 )
	// 	    u[grid](I1,I2,I3,uc+2)-=p()[grid].z(I1,I2,I3)(I1,I2,I3);


      } // end for grid 

      if( debug() & 4 || debug() & 32 )
	cgf.u.display(sPrintF(buff,"projectVelocity>>> Solution after project: u it=%i",it),debugFile,"%9.6f ");

      if( (it % 2) == 1)
	fixupUnusedPoints(cgf.u);
      // u.display("project velocity u, before interpolate");
    
      // interpolate first, needed for extrapolate-neighbours
      interpolate(cgf,V);

      if( debug() & 4 || debug() & 32 )
	cgf.u.display(sPrintF("projectVelocity>>> Solution after interpolate: u, it=%i",it),debugFile,"%9.6f ");

      // assign bc's to get ghost point values
      DomainSolver::applyBoundaryConditions(cgf); 

      if( overWriteBoundaryConditions )
      {
	//kkc 120323 added loop over boundary conditions to skip overwritting of penalty bcs
	for ( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  if( true )
	  {
	    const int nbc=2;  // noSlipWall and outflow
	    for( int ibc=0; ibc<nbc; ibc++ )
	    {
	      int bcToExtrap = ibc==0 ? Parameters::noSlipWall : InsParameters::outflow;
	      bcParams.ghostLineToAssign=1;
	      u[grid].applyBoundaryCondition(V,BCTypes::extrapolate,bcToExtrap,0.,0.,bcParams);
	      if( orderOfAccuracy==4 )
	      {
		bcParams.ghostLineToAssign=2;
		u[grid].applyBoundaryCondition(V,BCTypes::extrapolate,bcToExtrap,0.,0.,bcParams); 
		bcParams.ghostLineToAssign=1;
	      }
	      
	    }
	    
	  }
	  else
	  {
	    IntegerArray indexRangeLocal(2,3), dimLocal(2,3), bcLocal(2,3);
	    ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( u[grid],indexRangeLocal,dimLocal,bcLocal ); 

	    for ( int side=0; side<2; side++ )
	      for ( int axis=0; axis<numberOfDimensions; axis++ )
	      {
		// if ( (bcLocal(side,axis)!=Parameters::penaltyBoundaryCondition && !(isFactoredScheme && bcLocal(side,axis)==InsParameters::outflow)))
		// *wdh* 2012/09/14 -- only over-write noSlipWall (to undo div(u)=0)
		if( bcLocal(side,axis)!=Parameters::penaltyBoundaryCondition && bcLocal(side,axis)==InsParameters::noSlipWall )
		{
		  // *** undo the div(u)=0 BC so the projection will work better on the boundary.
		  if( orderOfAccuracy==2 ) // turn off for 4th order for testing
		  {
		    u[grid].applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::boundary(side,axis),0.,0.,bcParams);
		  }
		  else if( orderOfAccuracy==4 ) // trouble at outflow with standard BC's
		  {
		    u[grid].applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::boundary(side,axis),0.,0.,bcParams);
		    bcParams.ghostLineToAssign=2;
		    u[grid].applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::boundary(side,axis),0.,0.,bcParams); // **** 990106
		    bcParams.ghostLineToAssign=1;
		  }
		}
	      }
	  }
	  
	}
	//	u.finishBoundaryConditions();
      }
	
      if( debug() & 4 || debug() & 32 )
	cgf.u.display(sPrintF(buff,"projectVelocity>>> Solution after applyBC: u, it=%i",it),debugFile,"%9.6f ");
    
      divMax = projector.computeDivergence( u,divergence );  // compute new divergence to test for convergence
      if( debug() & 4 )
	printF(" projectVelocity>>> iteration=%i, divergence after project =%f\n",it,divMax);

      if( (true || debug() & 2) )
      {
	printF(" projectVelocity>>> iteration=%i, (new div)/(old div)=%6.3f, divergence after projection=%9.2e \n", 
	       it,min(1.e6,divMax/max(REAL_MIN,divMaxOld)),divMax);
	fPrintF(debugFile,"******* projectVelocity>>> iteration=%i, (new div)/(old div)=%6.3f, divergence after projection=%9.2e \n", 
		it,min(1.e6,divMax/max(REAL_MIN,divMaxOld)),divMax);
      }
      //   psp.set(GI_TOP_LABEL,"After project");
      //   parameters.dbase.get<GenericGraphicsInterface* >("ps")->contour(u,psp);
	
      //  --- stop when the divergence is nolonger decreasing
      if(divMax==0. || (it>minimumNumberOfProjectionIterations && divMax/divMaxOld > convergenceTolerance))
	break;
      divMaxOld=divMax;


    }

    // reset: 
    parameters.dbase.get<int>("initialConditionsAreBeingProjected")=0; // new way 

    parameters.dbase.get<real >("cdv")=cdv;  // reset
    parameters.dbase.get<real >("advectionCoefficient")=advectionCoefficient;
    parameters.dbase.get<real >("nu")=nu;

    dt=0.;
  }
  
  else
  {
    updateToMatchGrid(cgf.cg); 
  }
  

  if( debug() & 16 ) 
  {
    fPrintF(debugFile," \n ****Solution after projectVelocity and BC's**** \n");
    outputSolution( cgf.u,0. );
  }
  
  real time=getCPU()-time0;
  printF(">>>>>Time to project = %8.2e s <<<<\n",time);

  return 0;
}
