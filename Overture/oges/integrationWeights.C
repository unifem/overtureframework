//=====================================================================================
//  Oges example: 
//     Compute integration weights on an Overlapping Grid
//       o solve for the left null vector of the Neumann Problem
//       o scale the left null vector so that the entries become integration weights
//====================================================================================
#include "Overture.h"  
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "OGPolyFunction.h"

main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  Oges::debug=7; 

  aString nameOfOGFile;
  cout << "Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;
  if( nameOfOGFile[0]!='.' )
    nameOfOGFile="/home/henshaw/res/ogen/"+nameOfOGFile;  // tack on Bill's path for grids.

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  // make a grid function to hold the coefficients
  Range all;
  int stencilSize=pow(3,cg.numberOfDimensions())+1;  // add 1 for interpolation equations
  realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
    
  // create grid functions: 
  realCompositeGridFunction weights(cg),f(cg);

  CompositeGridOperators op(cg);                            // create some differential operators
  op.setStencilSize(stencilSize);
  coeff.setOperators(op);
  
  coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  // fill in the coefficients for the boundary conditions
  coeff.setOperators(op);
  coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,BCTypes::allBoundaries);
  coeff.finishBoundaryConditions();

  Oges solver( cg );                       // create a solver
  solver.setCoefficientArray( coeff );     // supply coefficients
  solver.setCompatibilityConstraint(TRUE); // system is singular so add an extra equation
  solver.setTranspose(1);                  // solve the transpose system (we want the left null vector)
  solver.initialize();

  // assign the rhs: f=0 except for the rhs to the compatibility equation which we set to 1
  // (this will cause the sum of the interior weights to be 1)
  f=0.;
  // find the equation where the compatibility constraint is put (some unused point)
  int n,i1,i2,i3,grid ;
  solver.equationToIndex(solver.extraEquationNumber(0),n,i1,i2,i3,grid);
  f[grid](i1,i2,i3,n)=1.;
     
  solver.solve( weights,f );   // solve for the (unscaled weights)

  weights.display("Here are the unscaled weights");

  // scale the weights (by a constant) so that they will be integration weights
  solver.scaleIntegrationCoefficients( weights ); 
  weights.display("Here are the scaled weights");
  
  // integrate a function:
  int degreeOfPolynomial=0;
  OGPolyFunction poly(degreeOfPolynomial,cg.numberOfDimensions());    // define a function
  poly.assignGridFunction(f);

  real volumeIntegral,surfaceIntegral;
  solver.integrate( weights,f,volumeIntegral,surfaceIntegral );   // Integrate f

  return(0);

}
