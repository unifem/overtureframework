//
// Another Test out the Mapping Class Library
//    Test the Graphics Interface
//



#include "StretchMapping.h"       // stetching routines
#include "Square.h"        // square
#include "SmoothedPolygon.h"      
#include "DataPointMapping.h"
#include "GraphicsInterface.h"  


void main()
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems

  Mapping::debug=0;         // set the debug parameter for mappings
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking


  cout << "====== Test of the mapping class =====" << endl;

  RealArray r(3);
  RealArray x(3);
  RealArray xr(3,3);
  RealArray rx(3,3);
  RealArray t(3);
  RealArray tx(3,3);

  RealArray r1(3,10), r2(3,10);
  RealArray x1(3,10), x2(3,10);
  RealArray xr1(3,3,10), xr2(3,3,10), rx2(3,3,10);


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
  for( int i=0; i<10; i++ )
  {
    r1(axis1,i)=i/9.; 
    r1(axis2,i)=i/9.; 
  }
  square.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points
  for( i=0; i<10; i++ )
    printf(" Square: r= (%6.3f,%6.3f) x = (%7.4f,%7.4f)\n",
      r1(axis1,i),r1(axis2,i),x1(axis1,i),x1(axis2,i));


  GraphicsInterface gi; 

//  gi.openPhigs(); // Optional.
    
  gi.openWorkstation();
  gi.setCurrentWorkstation(GraphicsInterface::activateIO);
  gi.setCurrentWorkstation(GraphicsInterface::activateGraphics);

//  const int view = 1;
//  gi.specifyViewParameters(view);

  SmoothedPolygon poly;
    
  poly.interactiveConstructor( gi );  // assign parameters interactively
    
  poly.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points
  for( i=0; i<10; i++ )
    printf(" Poly: r= (%6.3f,%6.3f) x = (%7.4f,%7.4f)\n",
      r1(axis1,i),r1(axis2,i),x1(axis1,i),x1(axis2,i));

  // Mapping defined from data points:
  DataPointMapping dataPoint;
    
  dataPoint.interactiveConstructor( gi );  // assign parameters interactively
    
  dataPoint.map( r1,x1,xr1 );  // get x1 and xr1 at an array of points
  for( i=0; i<10; i++ )
    printf(" DataPoint: r= (%6.3f,%6.3f) x = (%7.4f,%7.4f)\n",
      r1(axis1,i),r1(axis2,i),x1(axis1,i),x1(axis2,i));

  gi.closeWorkstation();

  cout << "Finished test..." << endl;

}
