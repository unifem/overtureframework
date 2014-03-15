//===============================================================================
//  Test the Overlapping Grid Show file class Ogshow
//
//   -----Test the moving grids version-------
//
//==============================================================================
#include "Overture.h"
#include "Ogshow.h"  

void rotateGrids( CompositeGrid & cg1, const real & t1, CompositeGrid & cg2, const real & t2 );

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  aString nameOfOGFile(80), nameOfShowFile(80);
  
  cout << "togshow>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  cout << "togshow>> Enter the name of the (new) show file (blank for none):" << endl;
  cin >> nameOfShowFile;

  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);          // read from a data base file
  cg.update();

  Ogshow show(nameOfShowFile);  // create a show file

  CompositeGrid cg1,cg2;

  show.setMovingGridProblem( TRUE );

  show.saveGeneralComment("Solution to the Navier-Stokes"); // save a general comment in the show file
  show.saveGeneralComment(" file written on April 1");      // save another general comment
    
  const int numberOfComponents=3;
  Range all;   // null Range used to declare coordinate directions:
  realCompositeGridFunction q(cg,all,all,all,numberOfComponents); // create a grid function with 3 components
  realCompositeGridFunction u,v,machNumber;  // create grid functions for components
  u.link(q,Range(0,0));                               // link u to the first component of q
  v.link(q,Range(1,1));                               // link v to the second component of q
  machNumber.link(q,Range(2,2));                      // ...

  q.setName("q");                  // assign name to grid function and components
  q.setName("u",0);
  q.setName("v",1);
  q.setName("Mach Number",2);
    
  char buffer[80];                           // buffer for sPrintF
  Index I1,I2,I3;
  int numberOfTimeSteps=5;
  for( int i=0; i<numberOfTimeSteps; i++ )  // Now save the grid functions at different time steps
  {
    show.startFrame();
    
    real t=i*.1;
    show.saveComment(0,sPrintF(buffer,"Here is solution %i",i));   // comment 0 (shown on plot)
    show.saveComment(1,sPrintF(buffer,"  t=%e ",t));               // comment 1 (shown on plot)
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);
      u[grid]=cos(cg[grid].vertex()(I1,I2,I3,axis1));           //  get u and v from some computation
      v[grid]=sin(cg[grid].vertex()(I1,I2,I3,axis2));           //  get u and v from some computation
     }
    machNumber=u*u+v*v;
    show.saveSolution( q );              // save the current grid function

    // for now just change cg
    cg1=cg;
    rotateGrids( cg1,t, cg,t+.1 );       // get a new composite grid cg1

    q.updateToMatchGrid( cg );
    u.link(q,Range(0,0));                         // restore links..
    v.link(q,Range(1,1));                          
    machNumber.link(q,Range(2,2));                 
    
    
  }
  return 0;
}


#define XR(I1,I2,I3,axis,dir) \
    vertexDerivative(I1,I2,I3,(axis)+numberOfDimensions*(dir))
//=================================================================================
//
//   rotate all component grids from time t1 to time t2
//
// Input
//  t1,cg1 : CG at time t1
//  t2     : new time for grid cg2
//=================================================================================
void 
rotateGrids( CompositeGrid & cg1, const real & t1, CompositeGrid & cg2, const real & t2 )
{
  Index I1,I2,I3;
  int numberOfDimensions=cg1.numberOfDimensions();
  
  Index Axes(0,numberOfDimensions);
  real theta;
  theta=2.*Pi*(t2-t1);
  
  RealArray x0(3); x0(axis1)=.5; x0(axis2)=.5; x0(axis3)=0.; // centre of rotation

  real cost=cos(theta);
  real sint=sin(theta);

//  cg2.reference(cg1);
//   cg2=cg1; // deep copy for now

  for( int grid=0; grid<cg1.numberOfComponentGrids(); grid++ )
  {
    getIndex( cg2[grid].dimension(),I1,I2,I3 );
    cg2[grid].vertex()(I1,I2,I3,axis1)=(cg1[grid].vertex()(I1,I2,I3,axis1)-x0(axis1))*cost
                                    -(cg1[grid].vertex()(I1,I2,I3,axis2)-x0(axis2))*sint + x0(axis1);
    cg2[grid].vertex()(I1,I2,I3,axis2)=(cg1[grid].vertex()(I1,I2,I3,axis1)-x0(axis1))*sint 
                                    +(cg1[grid].vertex()(I1,I2,I3,axis2)-x0(axis2))*cost + x0(axis2);
    
    cg2[grid].XR(I1,I2,I3,Axes,axis1)=
                           cg1[grid].XR(I1,I2,I3,Axes,axis1)*cost
                          -cg1[grid].XR(I1,I2,I3,Axes,axis2)*sint;
    cg2[grid].XR(I1,I2,I3,Axes,axis2)=
                           cg1[grid].XR(I1,I2,I3,Axes,axis1)*sint
                          +cg1[grid].XR(I1,I2,I3,Axes,axis2)*cost;
  }

}
