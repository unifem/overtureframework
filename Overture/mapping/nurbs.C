//
// Test NurbsMapping
//

#include "Mapping.h"
#include "NurbsMapping.h"
#include "PlotStuff.h"
#include "HDF_DataBase.h"


int
main()
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking


  cout << "====== Test Nurbs =====" << endl;

  int option=0;
  cout <<
    "Enter option: 0 = define a nurbs passing through given points (interpolate) \n"
    "              1 = test reparameterization \n"
    "              2 = test insertKnot, removeKnot \n"
    "              3 = test merge \n";
  cin >> option;

  NurbsMapping nurbs;
  int rangeDimension=2;
  
/* ---
  cout << "nurbs.getRangeBound(0,0) =" << nurbs.getRangeBound(0,0) << endl;
  cout << "nurbs.getRangeBound(1,0) =" << nurbs.getRangeBound(1,0) << endl;
  cout << "nurbs.getRangeBound(0,1) =" << nurbs.getRangeBound(0,1) << endl;
  cout << "nurbs.getRangeBound(1,1) =" << nurbs.getRangeBound(1,1) << endl;
--- */  

  PlotStuff ps;
  GraphicsParameters params;

  if( option==0 )
  {
    RealArray x(2,2);
    x(0,0)=0.; x(0,1)=0.;
    x(1,0)=1.; x(1,1)=0.;
//    x(2,0)=1.; x(2,1)=1.;
//  x(3,0)=0.; x(3,1)=1.;
//    x(4,0)=0.; x(4,1)=0.;
    nurbs.interpolate(x);
    PlotIt::plot(ps,nurbs);
  }
  else if( option==1 )
  {
  
    PlotIt::plot(ps,nurbs);
    nurbs.reparameterize(.1,.9, .5,1.);
    PlotIt::plot(ps,nurbs);
    nurbs.checkMapping();
    nurbs.reparameterize(1.,0.);
    PlotIt::plot(ps,nurbs);
    nurbs.checkMapping();
  }
  else if( option==2 )
  {
    // test insertKnot, removeKnot    
 
    int p1=3;
    int n1=p1+1;
    int m1=n1+p1+1;
    rangeDimension=2;

    // knots are clamped
    realArray uKnot,cPoint;
  
    uKnot.redim(m1+1);
    uKnot(0)=0.; uKnot(1)=0.; uKnot(2)=0.; uKnot(3)=0.;
    uKnot(4)=.5;
    uKnot(m1-3)=1.; uKnot(m1-2)=1.; uKnot(m1-1)=1.; uKnot(m1)=1.; 

    // control points (holds weight in last position)
    cPoint.redim(n1+1,3);   
    cPoint(0,0)=0.;  cPoint(0,1)=0.; cPoint(0,2)=1.;
    cPoint(1,0)=.25; cPoint(1,1)=.7; cPoint(1,2)=1.;

    cPoint(2,0)=.75; cPoint(2,1)=.7; cPoint(2,2)=1.;

    cPoint(n1-1,0)=.75; cPoint(n1-1,1)=1.; cPoint(n1-1,2)=1.;
    cPoint(n1,0)=1.;  cPoint(n1,1)=0.; cPoint(n1,2)=1.;

    nurbs.specify( m1,n1,p1,uKnot,cPoint,rangeDimension);
    
    params.set(GI_TOP_LABEL,"original nurb");  // set title
    PlotIt::plot(ps,nurbs,params);

    nurbs.insertKnot(uKnot(4));
    params.set(GI_TOP_LABEL,"knot inserted"); 
    PlotIt::plot(ps,nurbs,params);
  
    int numberRemoved;
    nurbs.removeKnot(5,1,numberRemoved);
    params.set(GI_TOP_LABEL,"knot removed"); 
    PlotIt::plot(ps,nurbs,params);
  }
  else if( option==3 )
  {
    // test merge
  
    realArray uKnot,cPoint;

    NurbsMapping nurbs2;
    int p1=2;
    int n1=2;
    int m1=n1+p1+1;

    uKnot.redim(m1+1);
    uKnot(0)=0.; uKnot(1)=0.; uKnot(2)=0.; 
    uKnot(m1-2)=1.; uKnot(m1-1)=1.; uKnot(m1)=1.; 

    // control points (holds weight in last position)
    cPoint.redim(n1+1,3);   
    cPoint(0,0)=1.; cPoint(0,1)=0.;  cPoint(0,2)=1.;
    cPoint(1,0)=1.; cPoint(1,1)=1.;  cPoint(1,2)=SQRT(2.)/2.;
    cPoint(2,0)=0.; cPoint(2,1)=1.;  cPoint(2,2)=1.;

    nurbs2.specify( m1,n1,p1,uKnot,cPoint,rangeDimension);
    params.set(GI_TOP_LABEL,"nurbs 2, 90 degree arc");
    PlotIt::plot(ps,nurbs2,params);

    // another 90 degree arc
    NurbsMapping nurbs3;
    p1=2;
    n1=2;
    m1=n1+p1+1;

    uKnot.redim(m1+1);
    uKnot(0)=0.; uKnot(1)=0.; uKnot(2)=0.; 
    uKnot(m1-2)=1.; uKnot(m1-1)=1.; uKnot(m1)=1.; 

    // control points (holds weight in last position)
    cPoint.redim(n1+1,3);   
    cPoint(0,0)=0.;  cPoint(0,1)=1.;  cPoint(0,2)=1.;
    cPoint(1,0)=-1.; cPoint(1,1)=1.;  cPoint(1,2)=SQRT(2.)/2.;
    cPoint(2,0)=-1.; cPoint(2,1)=0.;  cPoint(2,2)=1.;

    nurbs3.specify( m1,n1,p1,uKnot,cPoint,rangeDimension);
    params.set(GI_TOP_LABEL,"nurbs 3, 90 degree arc");
    PlotIt::plot(ps,nurbs3,params);

    printf("check curve \n");
    nurbs3.checkMapping();

    nurbs3.merge(nurbs2);
    params.set(GI_TOP_LABEL,"merged nurb, 180 degree arc");

/* ---
  Mapping::debug=63;
  realArray r(1,1),x(1,2),t(1,1);
  r=.5;
  nurbs3.map(r,x);
  nurbs3.inverseMap(x,t);
  r.display("Here is r");
  x.display("here is x");
  t.display(" inverseMap(map(r)) ");
  Mapping::debug=0;
---- */

    PlotIt::plot(ps,nurbs3,params);
//  printf("check merged curve \n");
//  nurbs3.checkMapping();
  
    HDF_DataBase db;
    db.mount("nurbs.hdf","I");
  
    nurbs3.put(db,"nurbs3");
  
    NurbsMapping nurbs4;
    nurbs4.get(db,"nurbs3");
    params.set(GI_TOP_LABEL,"nurbs 4 from the data base");
    PlotIt::plot(ps,nurbs4,params);
  }
  
  return 0;
}
