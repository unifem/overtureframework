#include "Overture.h"
#include "MappedGridOperators.h"

#define ForBoundary(mg,side,axis)   for( axis=0; axis<mg.numberOfDimensions; axis++ ) \
                                    for( side=0; side<=1; side++ )

void createInverseVertexDerivative( CompositeGrid & og );
//================================================================================
//  Examples showing how to create coefficient matrices for Derivative operators
//================================================================================
int main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  aString nameOfOGFile, nameOfDirectory=".";
  cout << "Enter the name of the composite grid file (in the cguser directory)" << endl;
  cin >> nameOfOGFile;   nameOfOGFile="/usr/snurp/henshaw/cgap/cguser/" + nameOfOGFile;

  MultigridCompositeGrid mgcog(nameOfOGFile,nameOfDirectory);
  CompositeGrid & cg=mgcog[0];                            // use multigrid level 0
  MappedGrid & mg = cg[0];                                // alias for grid 0
  createInverseVertexDerivative( cg );                    // create inverseVertexDerivative
  Index I1,I2,I3;
  realMappedGridFunction u(mg,1,3),v(mg,1,3);             // define some component grid functions

  realMappedGridFunction coeff(mg,9,0);

  MappedGridOperators operators(mg);                      // define some differential operators
  u.setOperators(operators);                              // Tell u which operators to use


  operators.X().display("Here are the coefficients for u.X");

  coeff=operators.XX()+operators.YY();
  coeff.display("Here are the coefficients for u.xx+u.yy");

  Range M(0,8);
  int side,axis;
  Index Ib1,Ib2,Ib3;
  ForBoundary(mg,side,axis)
  {
    getBoundaryIndex(mg.gridIndexRange,side,axis,Ib1,Ib2,Ib3);
    coeff(M,Ib1,Ib2,Ib3)=operators.X(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3)
                        +operators.Y(Ib1,Ib2,Ib3)(M,Ib1,Ib2,Ib3);
  }
  coeff.display("Here are the coefficients for u.xx+u.yy with boundary conditions");

  return 0;
}
