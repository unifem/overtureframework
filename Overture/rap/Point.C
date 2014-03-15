#include "Point.h"
#include <GL/gl.h>
#include <GL/glu.h>

Point::
Point() // constructor
{
  coordinate[0] = coordinate[1] = coordinate[2] = 0.0;
}

Point::
~Point() // destructor
{
}

void Point::
operator=(Point & source)
{
  if (this == &source) return;
  for (int i=0; i<3; i++)
    coordinate[i] = source.coordinate[i];
}


void Point::
plot(GenericGraphicsInterface & gi, GraphicsParameters & gp /* =Overture::defaultGraphicsParameters() */) 
// plot the point and make it selectable
{
// check that the graphicswindow is open!!!
  if( !gi.isGraphicsWindowOpen() )
    return;

  int list = gi.generateNewDisplayList(false, true, false, true);  // get a new display list to use
  assert(list!=0);

// debug:
  // printf("Point::plot Point globalID=%i, display list #%i\n", getGlobalID(), list);

// AP: Need a way to allow for 2-D coordinates too!
//  gi.setAxesDimension(3);
      
  glNewList(list,GL_COMPILE);

// do we need to expand the bounding box?
//      setGlobalBound(xBound);
      
// assign a name for picking
  glPushName(getGlobalID()); 

  real pointSize;
  aString pointColour;
  gp.get(GI_POINT_SIZE, pointSize);
  gp.get(GI_POINT_COLOUR, pointColour);
  
  glPointSize(pointSize * gi.getLineWidthScaleFactor(gi.getCurrentWindow()) );   // need access functions!!!
  gi.setColour(pointColour);

  glBegin(GL_POINTS);  
#ifndef OV_USE_DOUBLE
  glVertex3f(coordinate[0], coordinate[1], coordinate[2]);
#else
  glVertex3d(coordinate[0], coordinate[1], coordinate[2]);
#endif
  glEnd();

  glPopName();

  glEndList(); 

}

PointList::
PointList(int listSize /* = 100 */) // constructor
{
  assert( listSize > 0 );
  
  maxPoints = listSize;
  points = new Point[maxPoints]; // reference counting?
  nPoints = 0;
}

PointList::
~PointList() // destructor
{
  delete[] points;
}

void PointList::
add( Point & newPoint )
{
  if (nPoints >= maxPoints)
  {
// resize the points array!
    printf("PointList:: add(): Resizing the point array\n");
// allocate new array
    Point *newArray = new Point[maxPoints+100];
// copy old data
    for (int i=0; i<maxPoints; i++)
      newArray[i] = points[i];
// free up old memory
    delete [] points;
// copy the pointer
    points = newArray;
    maxPoints += 100;
  }
  points[nPoints++] = newPoint;
}

void PointList::
clearAll()
{
  nPoints = 0;
}

void PointList::
clearLast()
{
  if (nPoints > 0)
    nPoints--;
  else
  {
    nPoints = 0;
    printf("PointList:: clearLast(): There are no elements to clear\n");
  }
}

Point & PointList::
operator []( const int pointIndex )
{
// check the index
  assert (pointIndex >= 0 && pointIndex < nPoints);
  return points[pointIndex];
}

void PointList::
plot(GenericGraphicsInterface & gi, GraphicsParameters & gp = Overture::defaultGraphicsParameters()) 
// plot the point and make it selectable
{
// check that the graphicswindow is open!!!
  if( !gi.isGraphicsWindowOpen() )
    return;

// setup the bounding box first
  int i, axes;
  RealArray xBound(2,3);
  for (axes=0; axes<3; axes++)
  {
    xBound(0,axes) = REAL_MAX;
    xBound(1,axes) =-REAL_MAX;
  }
  
  for (i=0; i<nPoints; i++)
  {
    for (axes=0; axes<3; axes++)
    {
      if (points[i].coordinate[axes] < xBound(0,axes))
	xBound(0,axes) = points[i].coordinate[axes];
      
      if (points[i].coordinate[axes] > xBound(1,axes))
	xBound(1,axes) = points[i].coordinate[axes];
    }
  }
  gi.setGlobalBound(xBound);

// plot all points
  for (i=0; i<nPoints; i++)
    points[i].plot(gi, gp);
  
}

    

