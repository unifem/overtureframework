//
// Another Test out the Mapping Class Library
//    Play around with the Inverse Function
//

#include <A++.h>
#include <string.h>

#include "realPrecision.h"  // define real to be float or double

#include "Mapping.h"       // Base Class
#include "MatrixMapping.h"
#include "ComposeMapping.h"
#include "Stretch.h"       // stetching routines
#include "Square.h"        // square
#include "Sphere.h"        // sphere


int main()
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems

//  Mapping::debug=15;         // set the debug parameter for mappings
  cout << "Enter debug \n";
  cin >> Mapping::debug;       // set the debug parameter for mappings
  Index::setBoundsCheck(on);   //  Turn on A++ array bounds checking


  cout << "====== Test of the mapping class =====" << endl;

//  double a,b;
//  for( ;; )
//  {
//    cout << "Enter a and b for fmod( a,b ) " ;
//    cin >> a >> b;
//    if( a==0. && b==0. ) break;
//    cout << "fmod(a,b) =" << fmod(a,b) 
//         << ",  a-floor(a)-.5 = " << a-floor(a+.5) << endl;
//  }


  RealArray r(3);
  RealArray x(3);
  RealArray xr(3,3);
  RealArray rx(3,3);
  RealArray t(3);
  RealArray tx(3,3);

  RealArray r1(3,10), r2(3,10);
  RealArray x1(3,10), x2(3,10);
  RealArray xr1(3,3,10), xr2(3,3,10), rx2(3,3,10);

// -- Define a Sphere Mapping

  SphereMapping sphere(.5,1.,0.,0.,0.);     // Define a sphere, inner radius .5, outer radius 1, 
                                            // centre the origin
  sphere.setName(Mapping::mappingName,"sphere");

  cout << " sphere.getDomainDimension() = " 
         << sphere.getDomainDimension() << endl;
  cout << " sphere.getRangeDimension()  = " 
       <<   sphere.getRangeDimension()  << endl;
  cout << " sphere:getName(Mapping::mappingName) = " 
         << sphere.getName(Mapping::mappingName)  << endl;

  cout << " ---Call sphere map with an array of values:" << endl;
  for( int i=0; i<10; i++ )
  {
    r1(axis1,i)=i/9.; 
    r1(axis2,i)=i/9.; 
    r1(axis3,i)=i/9.; 
  }
  sphere.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points
  for( i=0; i<10; i++ )
    printf(" Sphere: r= (%6.3f,%6.3f,%6.3f) x = (%6.3f,%6.3f,%6.3f)\n",
      r1(axis1,i),r1(axis2,i),r1(axis3,i),x1(axis1,i),x1(axis2,i),x1(axis3,i));

  cout << " ---Call sphere inverseMap with an array of values:" << endl;
  for( i=0; i<10; i++ )
  {
    x2(axis1,i)=x1(axis1,i); 
    x2(axis2,i)=x1(axis2,i); 
    x2(axis3,i)=x1(axis3,i); 
  }
  sphere.inverseMap( x2,r2,rx2 );  
  for( i=0; i<10; i++ )
    printf(" Sphere: x= (%6.3f,%6.3f,%6.3f) r = (%6.3f,%6.3f,%6.3f)\n",
      x2(axis1,i),x2(axis2,i),x2(axis3,i),r2(axis1,i),r2(axis2,i),r2(axis3,i));



// -- Define a Mapping

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
    printf(" Square: r= (%6.3f,%6.3f) x = (%7.4f,%7.4f)\n",
      r1(axis1,i),r1(axis2,i),x1(axis1,i),x1(axis2,i));

  cout << " ---Call square inverseMap with an array of values:" << endl;
  for( i=0; i<10; i++ )
  {
    x2(axis1,i)=1.5*x1(axis1,i); 
    x2(axis2,i)=x1(axis2,i); 
  }
  square.inverseMap( x2,r2,rx2 );  
  for( i=0; i<10; i++ )
    printf(" Square: x= (%6.3f,%6.3f) r = (%7.4f,%7.4f)\n",
      x2(axis1,i),x2(axis2,i),r2(axis1,i),r2(axis2,i));

  MappingParameters periodicParams;
// here is where we set the periodicity of Space, this should be consistent
// with the periodicity of ALL mappings
  periodicParams.periodicityOfSpace=1;
  periodicParams.periodicityVector(axis1,axis1)=1.; // set vector to (2,0)
  periodicParams.periodicityVector(axis2,axis1)=0.;

  cout << "=============Periodic in Space=============" << endl;
  cout << " ---Call square map with an array of values:" << endl;
  for( i=0; i<10; i++ )
  {
    r1(axis1,i)=i/9.; 
    r1(axis2,i)=i/9.; 
  }
  square.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points
  for( i=0; i<10; i++ )
    printf(" Square: r= (%6.3f,%6.3f) x = (%7.4f,%7.4f)\n",
      r1(axis1,i),r1(axis2,i),x1(axis1,i),x1(axis2,i));

  cout << " ---Call square inverseMap with an array of values:" << endl;
  for( i=0; i<10; i++ )
  {
    x2(axis1,i)=1.5*x1(axis1,i); 
    x2(axis2,i)=x1(axis2,i); 
  }
  square.inverseMap( x2,r2,rx2,periodicParams );  
  for( i=0; i<10; i++ )
    printf(" Square: x= (%6.3f,%6.3f) r = (%7.4f,%7.4f)\n",
      x2(axis1,i),x2(axis2,i),r2(axis1,i),r2(axis2,i));





  MatrixMapping rotate(2,2)  ;     // Define a matrix mapping, R^2 -> R^2 
  rotate.setName(Mapping::mappingName,"rotate");

//  const real Pi = 4.*atan(1.);
  rotate.rotate( zAxis, Pi/4. );  // rotate about z axis
//  rotate.scale( 2.,1.,1. );    // scale by 2 in x-direction
//  rotate.shift( 0.,1.,0. );   // shift by 1 in y direction
  
// Now compose two mappings 

  ComposeMapping rotsq( square,rotate );    // define a mapping by composition
  rotsq.setName(Mapping::mappingName,"rotsq");

  cout << " ---Call rotsq map with an array of values:" << endl;
  for( i=0; i<10; i++ )
  {
    r1(axis1,i)=i/9.; 
    r1(axis2,i)=i/9.; 
    x1(axis1,i)=-999.;
    x1(axis2,i)=-999.;
  }
  rotsq.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points
  for( i=0; i<10; i++ )
    printf(" Rotsq: r= (%6.3f,%6.3f) x = (%7.4f,%7.4f)\n",
      r1(axis1,i),r1(axis2,i),x1(axis1,i),x1(axis2,i));

  cout << " ---Call rotsq inverseMap with an array of values:" << endl;
  for( i=0; i<10; i++ )
  {
    x2(axis1,i)=x1(axis1,i); 
    x2(axis2,i)=x1(axis2,i); 
  }
  rotsq.inverseMap( x2,r2,rx2 );  
  for( i=0; i<10; i++ )
    printf(" Rotsq: x= (%6.3f,%6.3f) r = (%7.4f,%7.4f)\n",
      x2(axis1,i),x2(axis2,i),r2(axis1,i),r2(axis2,i));

  printf("Finished test...\n");

  return 0;
}
