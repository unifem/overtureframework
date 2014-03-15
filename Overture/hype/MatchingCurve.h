#ifndef MATCHING_CURVE_H
#define  MATCHING_CURVE_H

#include "Mapping.h"

class MappingProjectionParameters;

/// \brief Define a matching curve for use with the HyperbolicMapping and hyperbolic grid generator.
class MatchingCurve
{
public:
MatchingCurve();
MatchingCurve(const MatchingCurve & mc);
~MatchingCurve();

MatchingCurve& 
operator=(const MatchingCurve & mc );

void setCurve( Mapping & curve );

// change parameters interactively
int update( GenericGraphicsInterface & gi );

real x[3];            // x position
real curvePosition;   // r coordinate on the start curve where matching curve starts
int gridLine;
int curveDirection;   // match when growing in this direction (forward=1 or backward=-1 or both=0)
Mapping *curve;       // project grid line onto this mapping.
MappingProjectionParameters *projectionParameters;
int numberOfLinesForNormalBlend;

};

#endif
