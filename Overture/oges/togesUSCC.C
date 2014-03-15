//===============================================================================
//  Test the Overlapping Grid Equation Solver
//      User Supplied Coefficients -- Cell Centered Case
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

  aString nameOfOGFile;
  
  cout << "togesUSCC>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;
  nameOfOGFile="/n/c3servet/henshaw/res/cgsh/" + nameOfOGFile;

  cout << "Create a CompositeGrid..." << endl;
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
    
  cg.update();

  // make a grid function to hold the coefficients
  int stencilSize=3*3+2;    // add 1 since Oges stores interpolation equations here too
                            // add another 1 for compatibility constraint
  int positionOfStencil=0;  // coefficients appear first in the array
  realCompositeGridFunction coeff(cg,Range(0,stencilSize-1));

  // assign the coefficients
  Index I1,I2,I3,Ib1,Ib2,Ib3,R[3];  
  int side,axis;
  coeff=0.;  // initialize all values to 0
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    cg[grid].boundaryCondition().display("Here is the boundaryCondition");

    getIndex(cg[grid].indexRange(),I1,I2,I3);  // Indices for interior points
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
        getGhostIndex(cg[grid].indexRange(),side,axis,Ib1,Ib2,Ib3); 
        coeff[grid](4,Ib1,Ib2) = 1.;               // apply a Dirichlet BC on the first ghost line
      }
    }
  }

//  Oges solver( cg );                     // create a solver
  Oges solver;                     // create a solver
  solver.setCoefficientArray( coeff );   // supply coefficients
  solver.setCompatibilityConstraint( FALSE );
  solver.setOrderOfAccuracy(2);
  solver.setNumberOfGhostLines(1);  

  solver.updateToMatchGrid(cg);  // this will call initialize

  solver.setGhostLineOption(1,Oges::useGhostLineExceptCorner);

    
//  solver.initialize( );                  // initialize oges (assigns classify array used below)

  // create grid functions:
  Range all;
  realCompositeGridFunction u(cg,all,all,all),f(cg,all,all,all);

  // assign the rhs: u.xx+u.yy=1, u=0 on the boundary
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    f[grid]=0.;
    where( solver.classify[grid]>0 )
      f[grid]=1.;
    where( solver.classify[grid]==Oges::ghost1 )
      f[grid]=0.;
  }    

  solver.solve( u,f );   // solve the equations

  u.display("Here is the solution");
  for( grid=0; grid<u.numberOfComponentGrids(); grid++ )
    u[grid].isCellCentered().display("------Here is u.isCellCentered()--------");

  return(0);

}
