#ifndef CONTOUR_SURFACE_H
#define CONTOUR_SURFACE_H

#include "GL_GraphicsInterface.h"

// This class holds a contour surface for the 3D contour plotter
class ContourSurface
{
 public:

ContourSurface();

~ContourSurface();

ContourSurface(const ContourSurface & x);

ContourSurface & operator=(const ContourSurface & x);


// init an inactive surface.
void init();

enum SurfaceTypeEnum
{
  unknownSurfaceType,
  coordinateSurface,
  contourPlane,
  isoSurface
} surfaceType;

enum SurfaceStatusEnum
{
  inactive,
  notBuilt,
  built
} surfaceStatus;
  
// surfaces can be coloured in different ways
enum SurfaceColourTypeEnum
{
  colourSurfaceDefault,
  colourSurfaceByIndex
} surfaceColourType;
  

// destroy data and reset to inactive:
void destroy(){ csData.redim(0); init(); } 

int getGlobalID() const { return globalID;}  // 

RealArray csData;
  
int side,axis,index,grid;  // for coordinate surfaces

real normal[3], point[3];        // for contour planes
real tangent1[3], tangent2[3];   // for contour planes
real width1, width2;             // for contour planes

real value;  // for iso-surface

real minValue, maxValue;  // min and max value of the solution on the contour/coordinate plane

int colourIndex;  // index to getXColour()

protected:

int globalID;    // we use the globalID for identifying surfaces by picking
static Integer globalIDCounter;

};
#endif
