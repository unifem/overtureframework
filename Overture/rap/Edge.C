#include "Edge.h"

Edge::
Edge() // constructor
{
  spline_=NULL; 
  nKnots=0;

  subSurface = -1;
  mappingIsTrimmed = false;
  trimCurve = -1;
  subTrimCurve = -1;
}

Edge::
~Edge() // destructor
{
  if (spline_ && spline_->decrementReferenceCount() == 0)
    delete spline_;
}

