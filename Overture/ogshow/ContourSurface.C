#include "ContourSurface.h"


// ============================================================================
// \brief constructor.
// ============================================================================
ContourSurface::
ContourSurface()
{ 
  init(); 
}

// ============================================================================
/// \brief: Initialize an inactive surface.
// ============================================================================
void ContourSurface::
init() 
{ 
  surfaceType=unknownSurfaceType; surfaceStatus=inactive; side=-1; axis=-1, index=-1; 
  grid=-1; value=0.; minValue=0.; maxValue=1.;

  globalID=globalIDCounter;
  globalIDCounter++;

  surfaceColourType=colourSurfaceDefault;
  colourIndex=0;

  normal[0]=1., normal[1]=0., normal[2]=0.;
  point[0]=1., point[1]=0., point[2]=0.;
  width1=REAL_MAX, width2=REAL_MAX;  // contour planes are infinite in width by default
  // contour plane tangents are only needed if widths are finite
  tangent1[0]=0., tangent1[1]=1., tangent1[2]=0.;
  tangent2[0]=0., tangent2[1]=0., tangent2[2]=1.;
}


// ========================================================================================
// \brief Copy constructor.
// ========================================================================================
ContourSurface::
ContourSurface(const ContourSurface & x)
{
  init();  // should we increment the globalID ??
  *this=x;
}

// ========================================================================================
// \brief Equals operator (shallow copy of any arrays).
// ========================================================================================
ContourSurface & ContourSurface::
operator=(const ContourSurface & x)
{
  surfaceType=x.surfaceType; 
  surfaceStatus=x.surfaceStatus; 
  side=x.side; axis=x.axis; index=x.index; 
  grid=x.grid; value=x.value; minValue=x.minValue; maxValue=x.maxValue;

  globalID=x.globalID;

  surfaceColourType=x.surfaceColourType;
  colourIndex=x.colourIndex;

  for( int axis=0; axis<3; axis++ )
  {
    normal[axis]=x.normal[axis];
    point[axis]=x.point[axis];
    tangent1[axis]=x.tangent1[axis];
    tangent2[axis]=x.tangent2[axis];
  }
  width1=x.width1; width2=x.width2;
  
  csData.redim(0);
  csData=x.csData;

  return *this;
}


// ========================================================================================
// \brief Destructor.
// ========================================================================================
ContourSurface::
~ContourSurface()
{
  
}
