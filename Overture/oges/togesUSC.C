//===============================================================================
//  Test the Overlapping Grid Equation Solver
//      User Supplied Coefficients
//==============================================================================

#include "Oges.h"  

// -- here are some useful macros:
#define ForBoundary(side,axis)   for( axis=0; axis<cg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )
int
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  cout <<"Enter a value for Oges::debug \n";
  cin >> Oges::debug;              // set debug flag for Oges

  aString nameOfOGFile, nameOfShowFile, nameOfDirectory;
  
  cout << "toges1>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;
  cout << "toges1: Enter the directory to use:" << endl;
  cin >> nameOfDirectory;

  cout << "Create a CompositeGrid..." << endl;
  MultigridCompositeGrid mgcg(nameOfOGFile,nameOfDirectory);
  CompositeGrid & cg=mgcg[0];  // use multigrid level 0
  cg.update();
  createInverseVertexDerivative(cg);    // this should go away!

  // make a grid function to hold the coefficients
  int stencilSize=3*3+1;    // add 1 since Oges stores interpolation equations here too
  int positionOfStencil=0;  // coefficients appear first in the array
  realCompositeGridFunction coeff(cg,Range(0,stencilSize-1));

  // assign the coefficients
  Index I1,I2,I3,Ib1,Ib2,Ib3,R[3];  
  int side,axis;
  coeff=0.;  // initialize all values to 0
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    getIndex(cg[grid].indexRange(),I1,I2,I3,-1);  // Indices for interior points
    coeff[grid](0,I1,I2) =   0.;      //  coefficient of i1-1,i2-1 (no need to set as already zero)
    coeff[grid](1,I1,I2) =   1.;      //  coefficient of i1  ,i2-1
    coeff[grid](2,I1,I2) =   0.;      //  coefficient of i1+1,i2-1 (no need to set as already zero)
    coeff[grid](3,I1,I2) =   1.;      //  coefficient of i1-1,i2  
    coeff[grid](4,I1,I2) =  -4.;      //  coefficient of i1  ,i2  
    coeff[grid](5,I1,I2) =   1.;      //  coefficient of i1+1,i2  
    coeff[grid](6,I1,I2) =   0.;      //  coefficient of i1-1,i2+1 (no need to set as already zero) 
    coeff[grid](7,I1,I2) =   1.;      //  coefficient of i1  ,i2+1
    coeff[grid](8,I1,I2) =   0.;      //  coefficient of i1+1,i2+1 (no need to set as already zero)
    ForBoundary(side,axis)  // for each boundary (see macro above)
    {
      if( cg[grid].boundaryCondition()(side,axis) > 0 )
      {
        getBoundaryIndex(cg[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3); 
        coeff[grid](4,Ib1,Ib2) = 1.;               // apply a Dirichlet BC on the boundary
      }
    }
  }

  Oges solver( cg );                     // create a solver
  solver.setCoefficientArray( coeff );   // supply coefficients
  solver.setOrderOfAccuracy(2);
  solver.setNumberOfGhostLines(2);  
  solver.initialize( );                  // initialize oges (assigns classify array used below)

  // create grid functions:
  Range all;
  realCompositeGridFunction u(cg,all,all,all),f(cg,all,all,all);

  // assign the rhs: u.xx+u.yy=1, u=0 on the boundary
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    f[grid]=0.;
    where( solver.classify[grid]>0 )
      f[grid]=1.;
    where( solver.classify[grid]==Oges::boundary )
      f[grid]=0.;
  }    

  solver.solve( u,f );   // solve the equations

  u.display("Here is the solution");

  return(0);

}
