#include "Overture.h"
#include "BoxMapping.h"
#include "CylinderMapping.h"
#include "Sphere.h"
#include "display.h"

// 
// extern "C" 
// {
// int srandom(int);
// long random();
// }

int
crossCheck( Mapping & map )
// ================================================================================
//   
// ================================================================================
{
  printf(" **** crossCheck **** :Check the countCrossingsWithAPolygon routine in 3D **** \n");

//  srandom(getpid());
  srandom(Pi*12345678.);

//   map.approximateGlobalInverse->initialize();   // make sure bounding boxes are created
  map.getGrid();  // for bounds?
// **
  const int rangeDimension=map.getRangeDimension();
  assert( rangeDimension==3 );

  // get bounds on the grid
  RealArray xBound(2,3);
  // determine bounds on all the mappings that will be plotted
  Bound b;
  Range R(0,rangeDimension-1);
  xBound(Start,R)=REAL_MAX;
  xBound(End,  R)=-REAL_MAX;
  
  int axis;
  for( axis=0; axis<rangeDimension; axis++ )
  {
    Bound b = map.getRangeBound(Start,axis);
    if( b.isFinite() )
      xBound(Start,axis)=min(xBound(Start,axis),(real)b);
    else
      printf("crossCheck: WARNING: mapping has a infinite range bound! (side=Start,axis=%i) \n",axis);
	  
    b = map.getRangeBound(End,axis);
    if( b.isFinite() )
      xBound(End,axis)=max(xBound(End,axis),(real)b);
    else
      printf("crossCheck: WARNING: mapping has a infinite range bound! (side=End  ,axis=%i)\n",axis);

    
  }

  // check some points to see if they are inside or outside
  int n=9;
  int numberOfRegularPoints=pow(n,rangeDimension);
  
  int numberOfRandomPoints=400;
  
  int numberOfPoints=numberOfRegularPoints+numberOfRandomPoints;
  RealArray r(numberOfPoints,3), x(numberOfPoints,3), xCross(numberOfPoints,3,5);
  IntegerArray crossings(numberOfPoints), mask;
  
  Range I(0,numberOfPoints-1);

  int i;
  real rMax=-REAL_MAX, rMin=-rMax;
  
  // first make some points on a regular grid -- avoid checking points on the boundary of a cube
  i=0;
  real rr[3];
  for( int i3=1; i3<n+1; i3++ )
  {
    rr[axis3]=real(i3)/(n+1.);
    for( int i2=1; i2<n+1; i2++ )
    {
      rr[axis2]=real(i2)/(n+1.);
      for( int i1=1; i1<n+1; i1++ )
      {
	rr[axis1]=real(i1)/(n+1.);
        for( axis=0; axis<rangeDimension; axis++ )
	  x(i,axis)=xBound(Start,axis)+rr[axis]*(xBound(End,axis)-xBound(Start,axis));
        i++;
      }
    }
  }
  
    // make the bounding box bigger for the random points
  for( axis=0; axis<rangeDimension; axis++ )
  {
    real dist = (xBound(End,axis)-xBound(Start,axis))*.025;
    xBound(Start,axis)-=dist;
    xBound(End  ,axis)+=dist;
  }
  
  for( int j=0; j<numberOfRandomPoints; j++ )
  {
    for( axis=0; axis<rangeDimension; axis++ )
    {
      real r = (random() & 2047)/2047.;
      rMax=max(rMax,r);
      rMin=min(rMin,r);
      // printf("r=%e \n",r);
      x(i,axis)=xBound(Start,axis)+r*(xBound(End,axis)-xBound(Start,axis));
    }
    i++;
  }
  assert( i==numberOfPoints );
  
  printf(" ++ random number statistics: (min,max)=(%e,%e) \n",rMin,rMax);
  
  // display(x,"Here are the random points");
  
  crossings=0;
  for( axis=axis1; axis<3; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      map.approximateGlobalInverse->countCrossingsWithPolygon(x, crossings, side,axis,xCross, mask );
    }
  }
  // printf("number of Crossings=%i, x=(%e,%e,%e) \n",crossings(0),x(0,axis1),x(0,axis2),x(0,axis3));

  if( Mapping::debug & 8 )
    display(crossings,"Here is crossings");

  map.inverseMap(x,r);
  
  IntegerArray inside(numberOfPoints), mistake(numberOfPoints);

  inside(I)= fabs(r(I,axis1)-.5) <= .5 && fabs(r(I,axis2)-.5)  <= .5 &&  fabs(r(I,axis3)-.5) <= .5 ;

  if( Mapping::debug & 8 )
  {
    display(r,"Here is r");
    display(inside,"Here is inside");
  }
  
  // (crossings(I) % 2).display("Here is crossings(I) % 2");

  RealArray rDistanceToBoundary(I);
  rDistanceToBoundary = min(fabs(r(I,0)),min(fabs(r(I,0)-1.),min(fabs(r(I,1)),min(fabs(r(I,1)-1.),
                                min(fabs(r(I,2)),fabs(r(I,2)-1.))))));

  mistake(I) = !( inside(I) == (crossings(I) % 2) );
  
  int numberOfMistakes = sum(mistake(I));

  // since we only check the polygon approximation so the surface we could make allowable mistakes
  // Make a guess that the relative error for such mistakes is about:
  const real tolerance=1.e-3;
  mistake(I) = mistake(I) && rDistanceToBoundary > tolerance;
  int numberOfRealMistakes= sum(mistake(I));
  

  int numberInside = sum( crossings(I) % 2 );
  int numberOutside=numberOfPoints-numberInside;
  
  printf("Number of real mistakes = %i, (total mistakes=%i), number inside=%i, number outside=%i \n",
      numberOfRealMistakes,numberOfMistakes, numberInside,numberOutside);
  for( i=0; i<numberOfPoints; i++ )
  {
    if( mistake(i) )
    {
      real rDistanceToBoundary = min(fabs(r(i,0)),min(fabs(r(i,0)-1.),fabs(r(i,1)),fabs(r(i,1)-1.),
                                fabs(r(i,2)),fabs(r(i,2)-1.)));
      printf(" real mistake at point x=(%e,%e,%e), r=(%e,%e,%e), crossings=%i, r-distance from boundary=%e \n",
          x(i,0),x(i,1),x(i,2),r(i,0),r(i,1),r(i,2),crossings(i), rDistanceToBoundary);
    }
  }
  return numberOfMistakes;
}



int 
main()
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  printf(" **** Check the countCrossingsWithAPolygon routine in 3D **** \n");

  BoxMapping box;
  CylinderMapping cylinder;
  SphereMapping sphere;

  if( TRUE )
  {
    printf("\n ******* Check a box **** \n");
    crossCheck(box);

    cylinder.setGridDimensions(axis1,41);   // put a few more points here

    printf("\n ******* Check a cylinder **** \n");
    crossCheck(cylinder);

    sphere.setGridDimensions(axis1,41);
    sphere.setGridDimensions(axis2,41);
  
    printf("\n ******* Check a sphere **** \n");
    crossCheck(sphere);
  }
  

//  Mapping & map = box;
    Mapping & map = cylinder;
//  Mapping & map = sphere;

// **  map.getGrid();   // ********************* re-evaluate ????

  int numberOfPoints=1;
  RealArray r(numberOfPoints,3), x(numberOfPoints,3), xCross(numberOfPoints,3,5);
  IntegerArray crossings(numberOfPoints), mask;

  printf("**Check mapping: %s\n",(const char *)map.getName(Mapping::mappingName));
  
  Mapping::debug=7;
  for( ;; )
  {
    cout << "Enter a point to check, x,y,z \n";
    cin >> x(0,axis1) >> x(0,axis2) >> x(0,axis3);
    
    crossings=0;
    for( int axis=axis1; axis<3; axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
	map.approximateGlobalInverse->countCrossingsWithPolygon(x, crossings, side,axis,xCross, mask );
      }
    }
    printf("number of Crossings=%i, x=(%e,%e,%e) \n",crossings(0),x(0,axis1),x(0,axis2),x(0,axis3));

    map.inverseMap(x,r);
    bool inside = max(fabs(r-.5)) <= .5;
    
    if( inside && crossings(0) % 2 == 1 )
    {
      printf("point is inside (checks with inverseMap)\n");
    }
    else if( !inside && crossings(0) % 2 == 0 )
    {
      printf("point is outside (checks with inverseMap)\n");
    }
    else
    {
      printf("***ERROR*** inverseMap says point is %s, countCrossings says %s \n",
	     (inside ? "inside" : "outside"), (crossings(0) % 2 == 0 ? "outside" : "inside"));
    }
  }
  


  return 0;
}
