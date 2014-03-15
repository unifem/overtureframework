//===============================================================================
//  Coefficient Matrix Example
//    Using Oges to solve Poisson's equation on a CompositeGrid
//
// Usage: `tcm3 [<gridName>] [-solver=[yale][harwell][gmres]] [-debug=<value>] -noTiming' 
//==============================================================================
#include "Oges.h"

#include "CompositeGridOperators.h"
#include "Square.h"
#include "Annulus.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "SparseRep.h"

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
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

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
	else
	{
	  printf("Unknown solver=%s \n",(const char*)solver);
	  Overture::abort("error");
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
    cout << "Usage: `tcm3 [<gridName>] [-solver=[yale][harwell][gmres]] [-debug=<value>] -noTiming' \n";


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
    // cg.update();

    if( TRUE )
    {
      printf("**** start ***\n");
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	cg[grid].displayComputedGeometry();
    }


    const int inflow=1, outflow=2, wall=3;
  
    // create a twilight-zone function for checking the errors
    OGFunction *exactPointer;
    if( min(abs(cg[0].isPeriodic()(Range(0,cg.numberOfDimensions()-1))-Mapping::derivativePeriodic))==0 )
    {
      // this grid is probably periodic in space, use a trig function
      printf("TwilightZone: trigonometric polynomial\n");
      exactPointer = new OGTrigFunction(2.,2.);  // 2*Pi periodic
    }
    else
    {
      printf("TwilightZone: algebraic polynomial\n");
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

    if( TRUE )
    {
      printf("**** before operators ***\n");
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	cg[grid].displayComputedGeometry();
    }

    CompositeGridOperators op(cg);                            // create some differential operators
    op.setStencilSize(stencilSize);

    if( TRUE )
    {
      printf("**** after operators ***\n");
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	cg[grid].displayComputedGeometry();
    }

    coeff.setOperators(op);
  
    coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
    // fill in the coefficients for the boundary conditions
    coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
    coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);

    coeff.finishBoundaryConditions();
    // coeff.display("Here is coeff after finishBoundaryConditions");


    Oges solver( cg );                     // create a solver
    solver.setCoefficientArray( coeff );   // supply coefficients
    solver.set(OgesParameters::THEsolverType,solverType); 
    if( solverType==OgesParameters::SLAP ||  solverType==OgesParameters::PETSc )
    {
      solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
      solver.set(OgesParameters::THErelativeTolerance,max(1.e-8,REAL_EPSILON*10.));
    }    

    if( TRUE )
    {
      printf("**** after oges ***\n");
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	cg[grid].displayComputedGeometry();
    }


    u=0.;  // initial guess for iterative solvers
    real time0=CPU();
    solver.solve( u,f );   // solve the equations
    real time=CPU()-time0;
    printf("time for 1st solve of the Dirichlet problem = %8.2e, (iterations=%i)\n",time,
       solver.getNumberOfIterations());
    
  }
  
  return(0);
}
