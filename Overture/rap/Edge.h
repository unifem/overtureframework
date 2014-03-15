#ifndef EDGE_H
#define EDGE_H

#include "NurbsMapping.h"

class Edge
{
public:
Edge(); // constructor
~Edge(); // destructor

int nKnots;
realArray x; // put knots in here
NurbsMapping *spline_;
  
int subSurface;
bool mappingIsTrimmed;
int trimCurve;
int subTrimCurve;

};
#endif
