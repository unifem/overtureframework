#include "Mapping.h"
#include "Square.h"
#include "SmoothedPolygon.h"

MemoryManagerType memoryManager;  // This will delete allocated memory at the end

// --- pass a Mapping by value ----
void
passByValue( SmoothedPolygon map )
{
  cout << "*** pass a Mapping by value \n";

  RealArray r(3);
  RealArray x(3);
  RealArray xr(3,3);
  RealArray rx(3,3);

  r=0.;
  r(axis1)=.5;
  map.map( r,x,xr );
  cout << " PBV: For map: Here is x(.5) : " << x(axis1) << " , " << x(axis2) << endl; 
  r=0.;
  map.inverseMap(x,r,rx);
  cout << "PBV: For inverseMap: r=" << r(0) << endl;
}  

int 
main()
{
  Mapping::debug=7; 
//  SquareMapping map;
  SmoothedPolygon map;
  SmoothedPolygon map2;
  RealArray r(3),x(3);
  r=0.;
    
  map.map(r,x);
  map.inverseMap(x,r);

  map2.map(r,x);
  map2.inverseMap(x,r);
    
  cout << "test pass by value";
  passByValue( map );

   aString s="test";
// causes leak:   printf("test1= %s\n", (char*)s);
   printf("test2= %s\n", (const char*)s);

   if( s(0,1)=="te" )
     printf(" substring test true!\n");

  return 1;
}
