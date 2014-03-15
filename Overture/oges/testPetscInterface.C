//
// Test PetscOverture class
//

#include <iostream.h>
#include "mpi.h"
#include "Overture.h"
extern "C"
{
#include "sles.h"
}

#include "MappedGridOperators.h"
#include "Oges.h"
#include "Square.h"
#include "Annulus.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "SparseRep.h"

#include "petscOverture.h"

// ..MACROS
#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )
int 
zeroUnusedPoints(realGridCollectionFunction & u, realGridCollectionFunction & coeff );

//......PETSC help string
static char help[]="Tests PetscOverture class & it's linear solver.";

int 
main(int argc,char **args)
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  int ierr, numProcs;

  PetscInitialize(&argc,&args,(char *)0,help);
  MPI_Comm_size(PETSC_COMM_WORLD, &numProcs);
  if (numProcs !=1)  SETERRA(1,0,"This is a uniprocessor code only!");

  int solverType=Oges::yale; 
  Oges::debug=0;

  if( Oges::debug > 3 )
    SparseRepForMGF::debug=3;

  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  aString nameOfOGFile;
  cout << "Enter the name of the composite grid file (in the cgsh directory)" << endl;
  cin >> nameOfOGFile;
  if( nameOfOGFile[0]!='.' )
    nameOfOGFile="/users/henshaw/res/ogen/" + nameOfOGFile;
  aString nameOfDirectory = ".";
  cout << "\n Create an Overlapping Grid, mount file " << nameOfOGFile<< endl;
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

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
  int stencilSize=pow(3,cg.numberOfDimensions())+1;  // add 1 for interpolation equations
  realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
    
  // create grid functions: 
  realCompositeGridFunction u(cg),f(cg);
  f=0.; // for iterative solvers

  CompositeGridOperators op(cg);                            // create some differential operators
  op.setStencilSize(stencilSize);

  coeff.setOperators(op);
  
  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  // fill in the coefficients for the boundary conditions
  coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
  coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);

  coeff.finishBoundaryConditions();

  //.....NEW LINEAR SOLVER MODULE
  PetscOverture solver( cg );
  solver.setCoefficientArray( coeff );   // supply coefficients

  //.....Old solver, for checking
  Oges oldSolver( cg );
  oldSolver.setCoefficientArray( coeff );
  solverType=3; // GMRES
  oldSolver.setSolverType((Oges::solvers)solverType); 
  if( solverType>2 )
  {
    oldSolver.setConjugateGradientPreconditioner(Oges::incompleteLU);
    oldSolver.setConjugateGradientTolerance(REAL_EPSILON*10.);
  }    

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
  
  //..........INVOKE the SOLVER
  u=0.;  // ---------------------initial guess 

  //......... OGES first
  //u=f;
  //oldSolver.solve( u,f ); 
  //realCompositeGridFunction uOvert(cg);
  //uOvert=u;

  //.......... Try NEW Petsc interface 
  cout << "..............TRY NEW PETSC INTERFACE............\n";
  u=0.0;
  solver.solve(u,f);

  // ---------- CHECK ERRORS
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
      //      err.display("abs(error on indexRange +1)");
      // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
    }
  }
  printf("Maximum error with dirichlet bc's= %e\n",error);  



  printf("SOLVER.neq=%i\n",solver.numberOfEquations);

//   if ( solver.numberOfEquations < 60 ) {

//     uOvert.display("THE SOLUTION");

//     real error2=0.;
//     for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )   {
//       getIndex(cg[grid].indexRange(),I1,I2,I3,1);  
//       where( cg[grid].mask()(I1,I2,I3)!=0 )
// 	error2=max(error,max(abs(u[grid](I1,I2,I3)-uOvert[grid](I1,I2,I3))));
//     }
    
//     printf("DIFFERENCE betw. Overture & Petsc=%f\n", error2);
//   }    


  PetscFinalize();

  return 0;
}
