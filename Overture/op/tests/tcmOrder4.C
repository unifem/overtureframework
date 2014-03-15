//===============================================================================
//  Coefficient Matrix Example
//    Using Oges to solve Poisson's equation on a CompositeGrid
//    *** using fourth-0rder accuracy ****
//==============================================================================
#include "Overture.h"  
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )
int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture
  printf(" ===========================================================================\n"
         "   Solve an Elliptic Equation to Fourth order on an overlapping Grid        \n"
         "     The overlapping grid must be generated for fourth order accuracy       \n"
         " ===========================================================================\n");

//   cout << "Enter Oges::debug\n";
//   cin >> Oges::debug;
  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  aString nameOfOGFile="square16.order4.hdf";


//  cout << "Enter the name of the composite grid file (in the ogen directory)" << endl;
//  cin >> nameOfOGFile;
  int twilightZoneOption=0;
  int solverType=OgesParameters::yale; 
  aString solverName ="yale";
  bool check=false;
  real tol=1.e-8;
  aString iterativeSolverType="bi-cg";
  int iluLevels=-1; // -1 : use default
  int problemsToSolve=1+2;  // solve dirichlet=1 and neumann=2

  int len=0;
  if( argc > 1 )
  { 
    for( int i=1; i<argc; i++ )
    {
      aString arg = argv[i];
      if( len=arg.matches("-debug=") )
      {
        sScanF(arg(len,arg.length()-1),"%i",&Oges::debug);
	printf("Setting Oges::debug=%i\n",Oges::debug);
      }
      else if( len=arg.matches("-tol=") )
      {
        sScanF(arg(len,arg.length()-1),"%e",&tol);
	printf("Setting tol=%e\n",tol);
      }
       else if( len=arg.matches("-ilu=") )
      {
        sScanF(arg(len,arg.length()-1),"%i",&iluLevels);
	printf("Setting ilu levels =%i\n",iluLevels);
      }
      else if( len=arg.matches("-gmres") )
      {
	iterativeSolverType="gmres";
      }
      else if( len=arg.matches("-dirichlet") )
      {
	problemsToSolve=1; // just solve dirichlet problem
      }
      else if( arg(0,7)=="-solver=" )
      {
        solverName=arg(8,arg.length()-1);
        if( solverName=="yale" )
          solverType=OgesParameters::yale;
	else if( solverName=="harwell" )
          solverType=OgesParameters::harwell;
	else if( solverName=="petsc" || solverName=="PETSc" )
          solverType=OgesParameters::PETSc;
	else if( solverName=="slap" || solverName=="SLAP" )
          solverType=OgesParameters::SLAP;
	else if( solverName=="mg" || solverName=="multigrid" )
          solverType=OgesParameters::multigrid;
	else
	{
	  printf("Unknown solver=%s \n",(const char*)solverName);
	  throw "error";
	}
	
	printf("Setting solverType=%i\n",solverType);
      }
      else if( arg=="-check" )
      {
	check=true;
      }
      else if( arg=="-trig" )
      {
	twilightZoneOption=1;
      }
      else
      {
	nameOfOGFile=argv[1];
      }
    }
  }
  else
    cout << "Usage: tcmOrder4 [<gridName>] [-solver=[yale][harwell][slap][petsc][mg]] [-debug=<value>] " 
            "-noTiming -check -trig -tol=<value> -ilu=<value> -gmres -dirichlet \n";



  cout << "\n Create an Overlapping Grid, mount file " << nameOfOGFile<< endl;
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEvertexBoundaryNormal);

  const int inflow=1, outflow=2, wall=3;
  
  // create a twilight-zone function for checking the errors
  OGFunction *exactPointer;
  if( min(abs(cg[0].isPeriodic()(Range(0,cg.numberOfDimensions()-1))-Mapping::derivativePeriodic))==0 )
  {
    // this grid is probably periodic in space, use a trig function
    real fx=1, fy=1.;
    printf("TwilightZone: trigonometric polynomial, fx=%6.2f, fy=%6.2f\n",fx,fy);
    exactPointer = new OGTrigFunction(fx,fy);  // 2*Pi periodic
  }
  else
  {
    printf("TwilightZone: algebraic polynomial\n");
    int degreeOfSpacePolynomial = 4;
    int degreeOfTimePolynomial = 1;
    int numberOfComponents = cg.numberOfDimensions();
    exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
			              degreeOfTimePolynomial);
    
  }
  OGFunction & exact = *exactPointer;

  // make a grid function to hold the coefficients
  Range all;
  int stencilSize=int(pow(5,cg.numberOfDimensions())+1.5);  // add 1 for interpolation equations
  realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
  const int numberOfGhostLines=2;
  coeff.setIsACoefficientMatrix(TRUE,stencilSize,numberOfGhostLines);  
    
  // create grid functions: 
  realCompositeGridFunction u(cg),f(cg);

  Interpolant interpolant(cg);
  u=1.;
  u.interpolate();

  CompositeGridOperators op(cg);                            // create some differential operators
  op.setStencilSize(stencilSize);
  op.setOrderOfAccuracy(4);

  coeff.setOperators(op);
  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  // fill in the coefficients for the boundary conditions
  coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
  coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);
  // we also extrapolate the 2nd ghostline:
  BoundaryConditionParameters bcParams;
  bcParams.ghostLineToAssign=2;
  coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries,bcParams); // extrap 2nd ghost line
  coeff.finishBoundaryConditions();

  Oges solver( cg );                     // create a solver
  solver.setCoefficientArray( coeff );   // supply coefficients
  solver.set(OgesParameters::THEsolverType,solverType); 
  if( solver.isSolverIterative() ) 
  {
    solver.setCommandLineArguments( argc,argv );
    if( iterativeSolverType=="gmres" )
      solver.set(OgesParameters::THEsolverMethod,OgesParameters::generalizedMinimalResidual);
    else
      solver.set(OgesParameters::THEsolverMethod,OgesParameters::biConjugateGradientStabilized);
    solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
    solver.set(OgesParameters::THErelativeTolerance,max(tol,REAL_EPSILON*10.));
    solver.set(OgesParameters::THEmaximumNumberOfIterations,10000);
    if( iluLevels>=0 )
      solver.set(OgesParameters::THEnumberOfIncompleteLULevels,iluLevels);
  }    

  printf("\n === Solver:\n %s\n =====\n",(const char*)solver.parameters.getSolverName());

  // assign the rhs: Laplacian(u)=f, u=exact on the boundary
  Index I1,I2,I3, Ia1,Ia2,Ia3;
  int side,axis;
  Index Ib1,Ib2,Ib3;
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
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
	f[grid](Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
      }
    }
  }
  real time0=getCPU();
  solver.solve( u,f );   // solve the equations
  real time=getCPU()-time0;
  printf("\n*** max residual=%8.2e, time for 1st solve of the Dirichlet problem = %8.2e (iterations=%i) ***\n\n",
	     solver.getMaximumResidual(),time,solver.getNumberOfIterations());

  real error=0.;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    getIndex(cg[grid].indexRange(),I1,I2,I3,1);  
    where( cg[grid].mask()(I1,I2,I3)!=0 )
      error=max(error,max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0))));    
    if( Oges::debug & 8 )
    {
      RealArray err(I1,I2,I3);
      err(I1,I2,I3)=abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0));
      where( cg[grid].mask()(I1,I2,I3)==0 )
        err(I1,I2,I3)=0.;
      err.display("abs(error on indexRange +1)");
      // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
    }
  }
  printf("Maximum error with dirichlet bc's= %e\n\n",error);  
  
  if( problemsToSolve>1 )
  {
    // ----- Neumann BC's ----
    coeff=0.;
    coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
    // fill in the coefficients for the boundary conditions
    coeff.applyBoundaryConditionCoefficients(0,0,neumann,allBoundaries);
    coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries,bcParams);
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
      solver.set(OgesParameters::THEcompatibilityConstraint,true);
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
      f[gride](i1e,i2e,i3e)=0.;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].dimension(),I1,I2,I3);
	f[gride](i1e,i2e,i3e)+=sum(solver.rightNullVector[grid](I1,I2,I3)*exact(cg[grid],I1,I2,I3,0,0.));
      }
      printf("Extra equation: (i1,i2,i3,grid)=(%i,%i,%i,%i), rhs=%e \n",i1e,i2e,i3e,gride,f[gride](i1e,i2e,i3e));
    
    }
    time0=getCPU();
    solver.solve( u,f );   // solve the equations
    time=getCPU()-time0;
    cout << "time for solve of the Neumann problem = " << time << endl;

    error=0.;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].indexRange(),I1,I2,I3);  
      where( cg[grid].mask()(I1,I2,I3)!=0 )
	error=max(error,max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0))));    
      if( Oges::debug & 32 ) 
      {
	getIndex(cg[grid].dimension(),I1,I2,I3);  
	u.display("Computed solution");
	exact(cg[grid],I1,I2,I3,0).display("exact solution");
	abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
      }
    }
    Overture::finish();          
    printf("Maximum error with neumann bc's= %e\n",error);  
  }
  
  return(0);
}

