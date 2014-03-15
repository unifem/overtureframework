//
// Another Test out the Mapping Class Library
//    Play around with the Inverse Function
//

#include "SmoothedPolygon.h"

void f()
{
  SmoothedPolygon sp;
  RealArray r(10,2),x(10,2),rx(10,2,2);
  x=.5;
  sp.inverseMap(x,r,rx);
}


int 
main()
{

  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems

  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  cout << "====== Test of the mapping class =====" << endl;

  int i;
    
  RealArray r(1,3);
  RealArray x(1,3);
  RealArray xr(1,3,3);
  RealArray rx(1,3,3);
  RealArray t(1,3);
  RealArray tx(1,3,3);

  int numberOfPoints;
  
  RealArray r1,r2,x1,x2,xr1,xr2,rx2;
  RealArray Overture::nullRealArray();


  numberOfPoints=11;

  r1.redim(numberOfPoints,3); r2.redim(numberOfPoints,3);
  x1.redim(numberOfPoints,3); x2.redim(numberOfPoints,3);
  xr1.redim(numberOfPoints,3,3); xr2.redim(numberOfPoints,3,3); rx2.redim(numberOfPoints,3,3);
  r1=0.;  r2=0.; x1=0.; x2=0.;
  
// -- Define a Mapping

  SmoothedPolygon sp1;
  SmoothedPolygon sp2;

  int nx=(int)SQRT(real(numberOfPoints));
  int ny=(int)(numberOfPoints/real(nx)+.5);
  int i1,i2;
  
  for( i=0; i<numberOfPoints; i++ )
  {
    i1=i % nx;
    i2=i/nx;
    r1(i,axis1)=i1/real(nx); 
    r1(i,axis2)=i2/real(ny); 
    r1(i,axis3)=0.;
  }

  sp1.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points

  sp1.inverseMap( x2,r2,rx2 );  
  f();
  sp2.inverseMap( x2,r2,rx2 );  

  x2.redim(5,2);
  x2=.5;
  sp2.inverseMap( x2,r2,rx2 );

  return 0;
}
