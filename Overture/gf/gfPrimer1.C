#include "Overture.h"
#include "MappedGridOperators.h"
void createInverseVertexDerivative( CompositeGrid & og );


//===========================================================================================
//  Examples showing how use MappedGridFunction's without using a grid from CMPGRD
//=========================================================================================
int main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  int numberOfGridPoints=11;
  LineMapping unitInterval(0.,1.,numberOfGridPoints);
  MappedGrid mg(unitInterval);  // 1D MappedGrid

  Index I;
  Range all;                                             // null Range 
  realMappedGridFunction u(mg,all,Range(0,0)),           // define some component grid functions, 
                         v(mg,all,Range(0,0));   

  MappedGridOperators operators(mg);                     // define some differential operators
  u.setOperators(operators);                             // Tell u which operators to use

  getIndex(mg.dimension,I1);                     
  u(I1)=sin(mg.vertex1D(I1));                            // u=sin(x)*cos(y)

  u.display("here is u");
  getIndex(mg.indexRange,I1);                            // assign I1
  operators.x(u).display("Here is operators.x(u)");      // one way to compute u.x
  u.x().display("Here is u.x");                          // another way to compute u.x

  v=u;
  v.x().display("v=u; here is v.x");
  

  real error = max(fabs(u.x()(I1)- cos(mg.vertex1D(I1))));
    
  cout << "Maximum error (2nd order) = " << error << endl;
  u.operators->setOrderOfAccuracy(4);             // now compute to 4th order 
  error = max(fabs(u.x()(I1)-cos(mg.vertex1D(I1))));
    
  cout << "Maximum error (4th order) = " << error << endl;

  cout << "Program Terminated Normally! \n";
  return 0;
}
