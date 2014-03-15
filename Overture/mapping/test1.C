//
// Test 1 of the Mapping Class Library
//

#include <A++.h>
// #include <string.h>
#include "Dsk.h"

#include "Square.h"     // Define a Square

//const real Pi = 4.*atan(1.);
//const real twoPi = 2.*Pi;

#include "Annulus.h"    // Define an Annulus

#include "maputil.h"    // Utility Mappings, Matrix, Compose
#include "StretchMapping.h"    // Stretch Mappings

#include "Circle.h"  //  Here is a derived class to define a circle in 2D

#include "Cylinder.h"  //  Here is a derived class to define a Cylindrical Surface in 3D



void main()
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems

//  Mapping::debug=15;         // set the debug parameter for mappings
//  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking


  cout << "====== Test 1 of the mapping class =====" << endl;


  RealArray r(3);
  RealArray x(3);
  RealArray xr(3,3);
  RealArray rx(3,3);
  RealArray t(3);
  RealArray tx(3,3);

  RealArray r1(3,10), r2(3,10);
  RealArray x1(3,10), x2(3,10);
  RealArray xr1(3,3,10), xr2(3,3,10);

// -- Define an annulus

  AnnulusMapping annulus(.5,1.,0.,0.);     // Define an annulus, inner radius=.5,
                                           // outer radius=1, centre = (0,0)
  annulus.setName(Mapping::mappingName,"annulus");

  cout << " annulus.getDomainDimension() = " 
       <<   annulus.getDomainDimension() << endl;
  cout << " annulus.getRangeDimension()  = " 
       <<   annulus.getRangeDimension()  << endl;
  cout << " annulus:getName(Mapping::mappingName) = " 
       <<   annulus.getName(Mapping::mappingName)  << endl;

  r(axis1)=.5; r(axis2)=.5;

  annulus.map( r,x,xr );
  cout << " For annulus: Here is x(.5,.5) : " << x(axis1) 
       << " , " << x(axis2) << endl; 

  cout << " ---Call annulus map with an array of values:" << endl;
  for( int i=0; i<10; i++ )
  {
    r1(axis1,i)=i/9.; 
    r1(axis2,i)=i/9.; 
  }
  
  annulus.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points

  for( i=0; i<10; i++ )
    printf(" Annulus: r= (%7.4f,%7.4f) , x = (%7.4f,%7.4f) \n",
      r1(axis1,i),r1(axis2,i),x1(axis1,i),x1(axis2,i));

  cout << " ---Call annulus inverseMap with an array of values:" << endl;
  
  annulus.inverseMap( x1,r2,xr2 );  // get x1 and xr1 at an array of points

  for( i=0; i<10; i++ )
    printf(" Annulus: x= (%7.4f,%7.4f) , r = (%7.4f,%7.4f) \n",
      x1(axis1,i),x1(axis2,i),r2(axis1,i),r2(axis2,i));


// -- Define a Square Mapping

  SquareMapping square(-1.,1.,-1.,1.);     // Define a curve
  square.setName(Mapping::mappingName,"square");

  cout << " square.getDomainDimension() = " 
         << square.getDomainDimension() << endl;
  cout << " square.getRangeDimension()  = " 
       <<   square.getRangeDimension()  << endl;
  cout << " square:getName(Mapping::mappingName) = " 
         << square.getName(Mapping::mappingName)  << endl;

  cout << " ---Call square map with an array of values:" << endl;
  for( i=0; i<10; i++ )
  {
    r1(axis1,i)=i/9.; 
    r1(axis2,i)=i/9.; 
  }
  square.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points
  for( i=0; i<10; i++ )
    printf(" Square: r= (%7.4f,%7.4f) x = (%7.4f,%7.4f)\n",
      r1(axis1,i),r1(axis2,i),x1(axis1,i),x1(axis2,i));

  cout << " ---Call square inverseMap with an array of values:" << endl;
  for( i=0; i<10; i++ )
  {
    x2(axis1,i)=1.5*x1(axis1,i); 
    x2(axis2,i)=x1(axis2,i); 
  }
  square.inverseMap( x2,r2,xr2 );  
  for( i=0; i<10; i++ )
    printf(" Square: x= (%7.4f,%7.4f) r = (%7.4f,%7.4f)\n",
      x2(axis1,i),x2(axis2,i),r2(axis1,i),r2(axis2,i));

  printf("Finished test...\n");

}
