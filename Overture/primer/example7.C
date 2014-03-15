#include "Overture.h"  
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "display.h"

int
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ----------------------------------------------------------------------------- \n");
  printf("Use the operators to create the matrix for the discrete Laplacian operator with\n");
  printf("  boundary conditions.                                                         \n");
  printf("Use the Oges class to solve the system of equations.                           \n");
  printf(" ----------------------------------------------------------------------------- \n");

  aString nameOfOGFile;
  cout << "example7>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  int orderOfAccuracy=2;

  // make a grid function to hold the coefficients
  Range all;
  int stencilSize=int( pow(3,cg.numberOfDimensions())+1.5 );  // add 1 for interpolation equations
  realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
  const int numberOfGhostLines=orderOfAccuracy/2;
  coeff.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines);  
    
  // create grid functions: 
  realCompositeGridFunction u(cg),f(cg);

  CompositeGridOperators op(cg);                            // create some differential operators
  op.setStencilSize(stencilSize);
  coeff.setOperators(op);
  
  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  // fill in the coefficients for the boundary conditions
  coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
  coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries); // extrap ghost line
  coeff.finishBoundaryConditions();

  Oges::debug=3;
  
  Oges solver( cg );                     // create a solver
  // solver.set(OgesParameters::THEkeepSparseMatrix,true);  // turn this on if we want to save the matrix

  solver.setCoefficientArray( coeff );   // supply coefficients

  
  // assign the rhs:  Laplacian(u)=1, u=0 on the boundary
  Index I1,I2,I3;
  Index Ib1,Ib2,Ib3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    // displayCoeff(coeff[grid],sPrintF("coeff for grid=%i",grid),stdout); // un-comment this line to display the equations 

    MappedGrid & mg = cg[grid];
    getIndex(mg.indexRange(),I1,I2,I3);  

    f[grid](I1,I2,I3)=1.;
    for( int side=Start; side<=End; side++ )
    for( int axis=axis1; axis<cg.numberOfDimensions(); axis++ )
    {
      if( mg.boundaryCondition()(side,axis) > 0 )
      {
	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	f[grid](Ib1,Ib2,Ib3)=0.;
      }
    }
  }
  
  solver.solve( u,f );   // solve the equations

  // solver.writeMatrixToFile("matrix.dat");  // save the sparse matrix to a file (uncomment the line above too)
  
  // 

  u.display("Here is the solution to Laplacian(u)=1, u=0 on the boundary");

  Overture::finish();          
  return(0);

}
