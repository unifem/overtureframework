#include "Overture.h"
#include "CompositeGridOperators.h"
#include "display.h"

//================================================================================
//  Examples showing how to differentiate realCompositeGridFunctions
//     o evaluate using the x,y,... member functions
//     o evaluate in an effficient manner by computing many derivatives at once.
//================================================================================
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  aString nameOfOGFile;
  cout << "Enter the name of the overlapping grid data base file " << endl;
  cin >> nameOfOGFile;
  
  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEinverseVertexDerivative );

  Index I1,I2,I3;
  Range all;                                                // null Range (defaults to entire Range when used)
  realCompositeGridFunction  u(cg,all,all,all,Range(0,0)),   // define some component grid functions in 3D
                            v2(cg,all,all,all,Range(0,0)),  
                            v4(cg,all,all,all,Range(0,0)),
                             q(cg,all,all,all,Range(0,1));   // q has 2 components

  CompositeGridOperators operators(cg);                     // define some differential operators
  u.setOperators(operators);                                // Tell u which operators to use
  q.setOperators(operators); 

  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];                                                  // mg is an alias for cg[grid]
    getIndex(mg.dimension(),I1,I2,I3);                                             // assign I1,I2,I3
    u[grid](I1,I2,I3)=sin(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2));   // u=sin(x)*cos(y)

//     realMappedGridFunction ivd;
//     ivd=cg[grid].inverseVertexDerivative();
//     ::display(ivd,"ivd");
    
  }
  
  u.display("here is u");
  operators.x(u).display("Here is operators.x(u)");                // one way to compute u.x
  u.x().display("Here is u.x");                                    // another way to compute u.x

  v2=u.x();                                       // save x derivative (2nd-order)
  
  Range c0(0,0),c1(1,1);
  q(c0)=1.;                            // assign component 0 of q. This is cute but relatively expensive
  q(c1)=2.;                            // assign component 1 of q.
  q.display("here is q");
  q(c0)=q(c0)*q.x(c0)+q(c1)*q.y(c0);  

  operators.setOrderOfAccuracy(4);                // now compute to 4th order 
  v4=u.x();                                       //  save x derivative (4th-order)

  operators.setOrderOfAccuracy(2);             // reset back to 2nd order

  // print the errors
  real error;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];                                                  // mg is an alias for cg[grid]
    // compute errors on interior points and boundary
    getIndex(mg.indexRange(),I1,I2,I3);                                // assign I1,I2,I3
    error = max(fabs(v2[grid](I1,I2,I3)- cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2))));
    cout << "Maximum error (2nd order) = " << error << endl;

    error = max(fabs(v4[grid](I1,I2,I3)-cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2))));
    cout << "Maximum error (4th order) = " << error << endl;
  }
  
  // Now we compute the derivatives in a more efficient way. To do this we loop over the
  // component grids. 

  // The arrays ux and uy are used to save the results in. These arrays are re-used for all
  // the different component grids (thus saving space)
  RealArray ux,uy;                          
  // --- make a list of derivatives to evaluate on each component grid
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    operators[grid].setNumberOfDerivativesToEvaluate( 2 );
    operators[grid].setDerivativeType( 0, MappedGridOperators::xDerivative, ux );
    operators[grid].setDerivativeType( 1, MappedGridOperators::yDerivative, uy );
    operators[grid].setOrderOfAccuracy(2);
  }

  // Now evaluate the derivatives 
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];

    // compute the x and y derivatives of u and save in the arrays ux and uy
    operators[grid].getDerivatives(u[grid],I1,I2,I3);    
    // this next line is another way to do exactly the same thing
    u[grid].getDerivatives(I1,I2,I3);              

    error = max(fabs(ux(I1,I2,I3)-cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2))));
    cout << "Maximum error in ux: (2nd order) = " << error << endl;
    error = max(fabs(uy(I1,I2,I3)+sin(mg.vertex()(I1,I2,I3,axis1))*sin(mg.vertex()(I1,I2,I3,axis2))));
    cout << "Maximum error in uy: (2nd order) = " << error << endl;
  }
  
  Overture::finish();          
  cout << "Program Terminated Normally! \n";
  return 0;
}
