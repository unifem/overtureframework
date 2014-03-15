#include "Mapping.h"
#include "BoxMapping.h"

int
main()
{
  int axis1=0;  int axis2=1;  int axis3=2;  

  // -- Define a box in 3D

  BoxMapping cube(1.,2.,1.,2.,1.,2.)  ;                   // create a cube: [1,2]x[1,2]x[1,2]

  cube.setName(Mapping::mappingName,"cube");              // give the mapping a name

  cube.setIsPeriodic(axis1,Mapping::derivativePeriodic);  // periodic in x direction

  RealArray r(1,3),x(1,3),xr(1,3,3),rx(1,3,3);            // evaluate only 1 point

  r(0,axis1)=.25; r(0,axis2)=.5; r(0,axis3)=.75;
  r.display("here is r");
  cube.map( r,x,xr );                            // evaluate the mapping and derivatives: r --> (x,xr)
  x.display("here is x after map");

  r=0;
  cube.inverseMap( x,r,rx );                     // evaluate the inverse mapping: x --> (r,rx)
  r.display("here is r after inverseMap");

  return 0;
}  

