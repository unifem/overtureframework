//===============================================================================
//  Coefficient Matrix Example
//    Using Oges to solve Poisson's equation on a CompositeGrid
//
// Usage: `tcm3 [<gridName>] [-solver=[yale][harwell][slap][petsc][mg]] [-debug=<value>] -noTiming' 
//==============================================================================
#include "Overture.h"  
#include "MappedGridOperators.h"
#include "Oges.h"
#include "CompositeGridOperators.h"
#include "Square.h"
#include "Annulus.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "SparseRep.h"
#include "display.h"
#include "Ogmg.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

bool measureCPU=TRUE;
real
CPU()
// In this version of getCPU we can turn off the timing
{
  if( measureCPU )
    return getCPU();
  else
    return 0;
}

int 
main(int argc, char **argv)
{
  Diagnostic_Manager::setTrackArrayData(TRUE);

  Overture::start(argc,argv);  // initialize Overture

  const int maxNumberOfGridsToTest=3;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "square5", "cic", "sib" };
    
  int solverType=OgesParameters::yale; 
  if( argc > 1 )
  { 
    for( int i=1; i<argc; i++ )
    {
      aString arg = argv[i];
      if( arg=="-noTiming" )
        measureCPU=FALSE;
      else if( arg(0,6)=="-debug=" )
      {
        sScanF(arg(7,arg.length()-1),"%i",&Oges::debug);
	printf("Setting Oges::debug=%i\n",Oges::debug);
      }
      else if( arg(0,7)=="-solver=" )
      {
        aString solver=arg(8,arg.length()-1);
        if( solver=="yale" )
          solverType=OgesParameters::yale;
	else if( solver=="harwell" )
          solverType=OgesParameters::harwell;
	else if( solver=="petsc" || solver=="PETSc" )
          solverType=OgesParameters::PETSc;
	else if( solver=="slap" || solver=="SLAP" )
          solverType=OgesParameters::SLAP;
	else if( solver=="mg" || solver=="multigrid" )
          solverType=OgesParameters::multigrid;
	else
	{
	  printf("Unknown solver=%s \n",(const char*)solver);
	  throw "error";
	}
	
	printf("Setting solverType=%i\n",solverType);
      }
      else
      {
	numberOfGridsToTest=1;
	gridName[0]=argv[1];
      }
    }
  }
  else
    cout << "Usage: `tcm3 [<gridName>] [-solver=[yale][harwell][slap][petsc][mg]] [-debug=<value>] -noTiming' \n";


  if( Oges::debug > 3 )
    SparseRepForMGF::debug=3;  

  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  real worstError=0.;
  for( int it=0; it<numberOfGridsToTest; it++ )
  {
    aString nameOfOGFile=gridName[it];

    cout << "\n *****************************************************************\n";
    cout << " ******** Checking grid: " << nameOfOGFile << " ************ \n";
    cout << " *****************************************************************\n\n";

    CompositeGrid cg;
    getFromADataBase(cg,nameOfOGFile);
    cg.update();

    if( Oges::debug >3 )
    {
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	displayMask(cg[grid].mask(),"mask");
    }

    const int inflow=1, outflow=2, wall=3;
  
    // create a twilight-zone function for checking the errors
    OGFunction *exactPointer;
    if( true || min(abs(cg[0].isPeriodic()(Range(0,cg.numberOfDimensions()-1))-Mapping::derivativePeriodic))==0 )
    {
      // this grid is probably periodic in space, use a trig function
      printf("TwilightZone: trigonometric polynomial\n");
      real fx=2., fy=2., fz=2.;
      // real fx=.5, fy=.5, fz=.5;
      exactPointer = new OGTrigFunction(fx,fy,fz); 
    }
    else
    {
      printf("TwilightZone: algebraic polynomial\n");
      // cg.changeInterpolationWidth(2);

      int degreeOfSpacePolynomial = 2; 
      int degreeOfTimePolynomial = 1;
      int numberOfComponents = cg.numberOfDimensions();
      exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
					degreeOfTimePolynomial);
    
      
    }
    OGFunction & exact = *exactPointer;

  // make a grid function to hold the coefficients
    Range all;
    int stencilSize=int(pow(3,cg.numberOfDimensions())+1);  // add 1 for interpolation equations
    realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
    coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
    coeff=0.;
    
  // create grid functions: 
    realCompositeGridFunction u(cg),f(cg);
    f=0.; // for iterative solvers

    CompositeGridOperators op(cg);                            // create some differential operators
    op.setStencilSize(stencilSize);

    //   op.setTwilightZoneFlow(TRUE);
    // op.setTwilightZoneFlowFunction(exact);

    f.setOperators(op); // for apply the BC
    coeff.setOperators(op);
  
    // cout << "op.laplacianCoefficients().className: " << (op.laplacianCoefficients()).getClassName() << endl;
    // cout << "-op.laplacianCoefficients().className: " << (-op.laplacianCoefficients()).getClassName() << endl;
    
    {coeff=op.laplacianCoefficients();}       // get the coefficients for the Laplace operator
    // fill in the coefficients for the boundary conditions
    coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
    coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);

    coeff.finishBoundaryConditions();
    // coeff.display("Here is coeff after finishBoundaryConditions");

    if( false )
    {
      int grid;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	coeff[grid].sparse->classify.display("the classify matrix after applying finishBoundaryConditions()");
	//	  coeff[grid].display("this is the coefficient matrix");
      }
    }
    

    real total=0.;
    total+=cg.sizeOf()+u.sizeOf()+f.sizeOf()+op.sizeOf()+coeff.sizeOf();
    printf(" Before Oges==> cg.sizeOf()= %12.0f, (u,f).sizeOf()=%12.0f, \n" 
           "                op.sizeOf()=%12.0f coeff.sizeOf()=%12.0f, \n",
	   cg.sizeOf(),u.sizeOf()+f.sizeOf(),op.sizeOf(),coeff.sizeOf());

    printf(" Before Oges===> total = %12.0f, getTotalArrayMemoryInUse()=%i getTotalMemoryInUse()=%i \n\n",total,
	   Diagnostic_Manager::getTotalArrayMemoryInUse(),Diagnostic_Manager::getTotalMemoryInUse());

    Oges solver( cg );                     // create a solver
    solver.setCoefficientArray( coeff );   // supply coefficients
    solver.set(OgesParameters::THEsolverType,solverType); 
    if( solver.isSolverIterative() ) 
    {
      solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
      solver.set(OgesParameters::THErelativeTolerance,max(1.e-8,REAL_EPSILON*10.));
    }    

    // assign the rhs: Laplacian(u)=f, u=exact on the boundary
    Index I1,I2,I3, Ia1,Ia2,Ia3;
    int side,axis;
    Index Ib1,Ib2,Ib3;
    int grid;

    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      // mg.mapping().getMapping().getGrid();
      // printf(" signForJacobian=%e\n",mg.mapping().getMapping().getSignForJacobian());
      

      getIndex(mg.indexRange(),I1,I2,I3);  

      if( cg.numberOfDimensions()==1 )
	f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0);
      else if( cg.numberOfDimensions()==2 )
	f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0);
      else
	f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0)+exact.zz(mg,I1,I2,I3,0);

      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  // f[grid](Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
          f[grid].applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::boundary(side,axis),exact(mg,Ib1,Ib2,Ib3,0));
	}
      }
    }
    // f.applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::allBoundaries,0.);   
    // f.display("Here is f");
  
    // Ogmg::debug=7;
  
    u=0.;  // initial guess for iterative solvers
    real time0=CPU();
    solver.solve( u,f );   // solve the equations
    real time=CPU()-time0;
    printf("residual=%8.2e, time for 1st solve of the Dirichlet problem = %8.2e (iterations=%i)\n",
         solver.getMaximumResidual(),time,solver.getNumberOfIterations());

    // solve again
    // u=0.;
    time0=CPU();
    solver.solve( u,f );   // solve the equations
    time=CPU()-time0;
    printf("residual=%8.2e, time for 2nd solve of the Dirichlet problem = %8.2e (iterations=%i)\n",
         solver.getMaximumResidual(),time,solver.getNumberOfIterations());

    total=0.;
    total+=cg.sizeOf()+u.sizeOf()+f.sizeOf()+op.sizeOf()+solver.sizeOf();
    printf(" After Oges==> cg.sizeOf()= %12.0f, (u,f).sizeOf()=%12.0f, \n" 
           "                op.sizeOf()=%12.0f solver.sizeOf()=%12.0f (includes coeff)\n",
	   cg.sizeOf(),u.sizeOf()+f.sizeOf(),op.sizeOf(),coeff.sizeOf(),solver.sizeOf());

    printf("           ===> total = %12.0f, getTotalArrayMemoryInUse()=%i getTotalMemoryInUse()=%i \n\n",total,
	   Diagnostic_Manager::getTotalArrayMemoryInUse(),Diagnostic_Manager::getTotalMemoryInUse());


    // u.display("Here is the solution to Laplacian(u)=f");
    real error=0.;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].indexRange(),I1,I2,I3,1);  
      where( cg[grid].mask()(I1,I2,I3)!=0 )
	error=max(error, max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)))/
		  max(abs(exact(cg[grid],I1,I2,I3,0))) );
      if( Oges::debug & 8 )
      {
	RealArray err(I1,I2,I3);
	err(I1,I2,I3)=abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0))/max(abs(exact(cg[grid],I1,I2,I3,0)));
	where( cg[grid].mask()(I1,I2,I3)==0 )
	  err(I1,I2,I3)=0.;
	display(err,"abs(error on indexRange +1)");
	// abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
      }
    }
    printf("Maximum relative error with dirichlet bc's= %e\n",error);  
    worstError=max(worstError,error);

  
    // ----- Neumann BC's ----

    coeff=0.;
    coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
    // fill in the coefficients for the boundary conditions
    coeff.applyBoundaryConditionCoefficients(0,0,neumann,allBoundaries);
    coeff.finishBoundaryConditions();

    Index Ig1,Ig2,Ig3;
    bool singularProblem=TRUE;  

    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      getIndex(mg.indexRange(),I1,I2,I3);  
      if( mg.numberOfDimensions()==1 )
	f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0);
      else if(  mg.numberOfDimensions()==2 )
	f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0);
      else 
	f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0)+exact.zz(mg,I1,I2,I3,0);
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0  )
	{ // for Neumann BC's -- fill in f on first ghostline
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	  RealArray & normal = mg.vertexBoundaryNormal(side,axis);
	  if( mg.numberOfDimensions()==1 )
	    f[grid](Ig1,Ig2,Ig3)=(2*side-1)*exact.x(mg,Ib1,Ib2,Ib3,0);  
	  else if( mg.numberOfDimensions()==2 )
	    f[grid](Ig1,Ig2,Ig3)=
	      normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
	      +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0);
	  else
	    f[grid](Ig1,Ig2,Ig3)=
	      normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,0)
	      +normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,0)
	      +normal(Ib1,Ib2,Ib3,2)*exact.z(mg,Ib1,Ib2,Ib3,0);
	}
	else if( mg.boundaryCondition()(side,axis) ==inflow ||  mg.boundaryCondition()(side,axis) ==outflow )
	{
	  singularProblem=FALSE;
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  f[grid](Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
	}
      }
    }
    // if the problem is singular Oges will add an extra constraint equation to make the system nonsingular
    if( singularProblem )
      solver.set(OgesParameters::THEcompatibilityConstraint,TRUE);
    // Tell the solver to refactor the matrix since the coefficients have changed
    solver.setRefactor(TRUE);
    // we need to reorder too because the matrix changes a lot for the singular case
    solver.setReorder(TRUE);

    if( singularProblem )
    {
      // we need to first initialize the solver before we can fill in the rhs for the compatibility equation
      solver.initialize();
      int ne,i1e,i2e,i3e,gride;
      
      solver.equationToIndex( solver.extraEquationNumber(0),ne,i1e,i2e,i3e,gride);
      // printf("extra equation at (i1,i2,i3,grid)=(%i,%i,%i,%i)\n",i1e,i2e,i3e,gride);
      // display(solver.rightNullVector[gride],"solver.rightNullVector[grid]");

      f[gride](i1e,i2e,i3e)=0.;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].dimension(),I1,I2,I3);
	f[gride](i1e,i2e,i3e)+=sum(solver.rightNullVector[grid](I1,I2,I3)*exact(cg[grid],I1,I2,I3,0,0.));
      }
    }

    u=0.;  // initial guess for iterative solvers
    time0=CPU();
    solver.solve( u,f );   // solve the equations
    time=CPU()-time0;
    printf("residual=%8.2e, time for 1st solve of the Neumann problem = %8.2e (iterations=%i)\n",
         solver.getMaximumResidual(),time,solver.getNumberOfIterations());

  // turn off refactor for the 2nd solve
    solver.setRefactor(FALSE);
    solver.setReorder(FALSE);
    // u=0.;  // initial guess for iterative solvers
    time0=CPU();
    solver.solve( u,f );   // solve the equations
    time=CPU()-time0;
    printf("residual=%8.2e, time for 2nd solve of the Neumann problem = %8.2e (iterations=%i)\n",
         solver.getMaximumResidual(),time,solver.getNumberOfIterations());

    error=0.;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].indexRange(),I1,I2,I3);  
      where( cg[grid].mask()(I1,I2,I3)!=0 )
	error=max(error,  max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)))/
		  max(abs(exact(cg[grid],I1,I2,I3,0))) );
      if( Oges::debug & 32 ) 
      {
	getIndex(cg[grid].dimension(),I1,I2,I3);  
	u[grid].display("Computed solution");
	exact(cg[grid],I1,I2,I3,0).display("exact solution");
	abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
      }
    }
    printf("Maximum relative error with neumann bc's= %e\n",error);  
    worstError=max(worstError,error);
    
  }
  printf("\n\n ************************************************************************************************\n");
  if( worstError > .025 )
    printf(" ************** Warning, there is a large error somewhere, worst error =%e ******************\n",
	   worstError);
  else
    printf(" ************** Test apparently successful, worst error =%e ******************\n",worstError);
  printf(" **************************************************************************************************\n\n");

  Overture::finish();          
  return(0);
}


