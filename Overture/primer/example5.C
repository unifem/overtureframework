#include "Overture.h"
#include "CompositeGridOperators.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" --------------------------------------------------------------------------- \n");
  printf(" Demonstrate the operators for taking derivatives of compositeGridFunction's \n");
  printf(" --------------------------------------------------------------------------- \n");

  aString nameOfOGFile;
  cout << "example5>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  CompositeGridOperators operators(cg);                        // operators for a CompositeGridFunction
  Range all;                                                   
  realCompositeGridFunction u(cg),ux(cg),w(cg,all,all,all,2);  // create some composite grid functions

  u.setOperators(operators);                                   // tell grid function which operators to use
  w.setOperators(operators); 

  u=1.;
  ux=u.x();                                                    // compute the x derivative of u
  ux.display("Here is the x derivative of u=1 (computed at interior and boundary points)");
  w=2.;
  w.y().display("Here is the y derivative of w");
  Range c0(0,0),c1(1,1);
  w.y(c0).display("Here is the y derivative of component 0 of w");
  w.y(c1).display("Here is the y derivative of component 1 of w");
    
  real error;
  Index I1,I2,I3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )        // loop over component grids
  {
    MappedGrid & mg = cg[grid];
    getIndex(mg.dimension(),I1,I2,I3);                           // assign I1,I2,I3 for dimension
    u[grid](I1,I2,I3)=sin(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2));  
    getIndex(mg.indexRange(),I1,I2,I3);                          // assign I1,I2,I3 for indexRange
    operators.setOrderOfAccuracy(2);                           // set order of accuracy to 4

    ux[grid](I1,I2,I3)=u[grid].x()(I1,I2,I3);                  // here is the x derivative of u[grid]

    error = max(fabs( ux[grid](I1,I2,I3)- cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2) )));
    cout << "Maximum error (2nd order) = " << error << endl;

    error = max(fabs( operators[grid].x(u[grid])(I1,I2,I3)      // another way to compute derivatives
                    - cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2) )));
    cout << "Maximum error (2nd order) = " << error << endl;

    operators.setOrderOfAccuracy(4);                           // set order of accuracy to 4
    getIndex(mg.indexRange(),I1,I2,I3,-1);                       // decrease ranges by 1 for 4th order
    error = max(fabs(u[grid].x()(I1,I2,I3)-cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2))));
    cout << "Maximum error (4th order) = " << error << endl;
  }
  Overture::finish();          
  return 0;
}

