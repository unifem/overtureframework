#include "Overture.h"
#include "MappedGridOperators.h"
#include "Square.h"
//================================================================================
//  Examples showing how to differentiate realMappedGridFunctions
//     o evaluate using the x,y,... member functions
//     o evaluate in an effficient manner by computing many derivatives at once.
//================================================================================
int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  SquareMapping square(0.,1.,0.,1.);                   // Make a mapping, unit square
  square.setGridDimensions(axis1,11);                  // axis1==0, set no. of grid points
  square.setGridDimensions(axis2,11);                  // axis2==1, set no. of grid points
  MappedGrid mg(square);                               // MappedGrid for a square
  mg.update();                                         // create default variables

  Index I1,I2,I3;
  Range all;                                             // null Range 
  realMappedGridFunction u(mg,all,all,all,Range(0,0)),   // define some component grid functions, 
                         v(mg,all,all,all,Range(0,0)),   // in 3D
                         w(mg,all,all,all,Range(0,1));

  MappedGridOperators operators(mg);                     // define some differential operators
  u.setOperators(operators);                             // Tell u which operators to use
  v.setOperators(operators);
  w.setOperators(operators);                             // Tell u which operators to use

  getIndex(mg.dimension(),I1,I2,I3);                                             // assign I1,I2,I3
  u(I1,I2,I3)=sin(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2));   // u=sin(x)*cos(y)
  w(I1,I2,I3,0)=sin(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2));   // first component
  w(I1,I2,I3,1)=sin(mg.vertex()(I1,I2,I3,axis1))*sin(mg.vertex()(I1,I2,I3,axis2));   // second component

  u.display("here is u");

  // compute the derivatives at interior and boundary points (there is 1 ghost line by default)
  getIndex(mg.indexRange(),I1,I2,I3);                                // assign I1,I2,I3
  
  operators.x(u).display("Here is operators.x(u)");                // one way to compute u.x
  u.x().display("Here is u.x");                                    // another way to compute u.x

  v=u;
  v.x().display("v=u; here is v.x");
  

  real error = max(fabs(u.x()(I1,I2,I3)- cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2))));
  cout << "Maximum error (2nd order) = " << error << endl;

  // here we compute the derivatives of only some components of w
  v=w.x(all,all,all,0)+w.y(all,all,all,1);
  v.display("here is w.x(0)+w.y(1)");


  // now compute to 4th order 
  operators.setOrderOfAccuracy(4);          
  // 4th order has a 5 point stencil -- therefore on compute on interior points 
  getIndex(mg.indexRange(),I1,I2,I3,-1);

  // compute the derivatives at interior and boundary points (there is 1 ghost line by default)

  error = max(fabs(u.x()(I1,I2,I3)-cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2))));
  cout << "Maximum error (4th order) = " << error << endl;

  // --- Here is a more complicated expression:
  v(I1,I2,I3)=u(I1,I2,I3)*u.x()(I1,I2,I3)+v(I1,I2,I3)*u.y()(I1,I2,I3)-.1*(u.xx()(I1,I2,I3)+u.yy()(I1,I2,I3));

  // --- make a list of derivatives to evaluate all at once (this is more efficient) ---
  RealArray ux,uy;                           // these arrays will hold the answers
  operators.setNumberOfDerivativesToEvaluate( 2 );
  operators.setDerivativeType( 0, MappedGridOperators::xDerivative, ux );
  operators.setDerivativeType( 1, MappedGridOperators::yDerivative, uy );

  // reset order of accuracy to 2
  u.getOperators()->setOrderOfAccuracy(2);  // This is the same as operators.setOrderOfAccuracy(2);

  // compute the x and y derivatives of u and save in the arrays ux and uy
  operators.getDerivatives(u,I1,I2,I3);    
  // this next line is another way to do exactly the same thing
  u.getDerivatives(I1,I2,I3);              

  error = max(fabs(ux(I1,I2,I3)-cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2))));
  cout << "Maximum error in ux: (2nd order) = " << error << endl;
  error = max(fabs(uy(I1,I2,I3)+sin(mg.vertex()(I1,I2,I3,axis1))*sin(mg.vertex()(I1,I2,I3,axis2))));
  cout << "Maximum error in uy: (2nd order) = " << error << endl;


  // compute the y derivative only
  ux=-123.;  // init with bogus values
  uy=-123.;
  u.getDerivatives(I1,I2,I3,all,1);    // all=all components, 1=derivative number 1 (yDerivative)         

  error = max(fabs(ux(I1,I2,I3)-cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2))));
  cout << "Maximum error in ux: (2nd order) (should be bad, only uy computed)= " << error << endl;
  error = max(fabs(uy(I1,I2,I3)+sin(mg.vertex()(I1,I2,I3,axis1))*sin(mg.vertex()(I1,I2,I3,axis2))));
  cout << "Maximum error in uy: (2nd order) = " << error << endl;
  
  // ***** now compute derivatives of a grid function with multiple components

  getIndex(mg.indexRange(),I1,I2,I3);                                // assign I1,I2,I3
  w.getDerivatives(I1,I2,I3);
  
  ux=-123.;  // init with bogus values
  uy=-123.;
  w.getDerivatives(I1,I2,I3,0,1);    // 0=component, 1=yDerivative         
  w.getDerivatives(I1,I2,I3,1,0);    // 1=component, 0=xDerivative

  ux.display("ux for w");
  uy.display("uy for w");

  error = max(fabs(uy(I1,I2,I3,0)+sin(mg.vertex()(I1,I2,I3,axis1))*sin(mg.vertex()(I1,I2,I3,axis2))));
  cout << "Maximum error in w(0).y: (2nd order) = " << error << endl;
  error = max(fabs(ux(I1,I2,I3,1)-cos(mg.vertex()(I1,I2,I3,axis1))*sin(mg.vertex()(I1,I2,I3,axis2))));
  cout << "Maximum error in w(1).x: (2nd order) = " << error << endl;


  cout << "Program Terminated Normally! \n";
  Overture::finish();          
  return 0;
}
