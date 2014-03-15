#ifndef HELPFUL_FUNCTIONS_H
#define HELPFUL_FUNCTIONS_H
#include "Overture.h"
#include "TwilightZone.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "Mapping.h"
#include "SquareMapping.h"


void
setValues (realMappedGridFunction &u,
	   const Index & C,
	   OGFunction* e = NULL);

void
setValues (realGridCollectionFunction &adaptiveGridSolution, 
	   const Index & C, 
	   OGFunction* e = NULL);

OGFunction *
setTwilightZoneFlowFunction (const TwilightZoneFlowFunctionType & TZType, 
			     const int & numberOfComponents,
			     const int & numberOfDimensions);

/*
void
makeMappedGrid (MappedGrid& grid, AMRProblemParameters& probParams);
*/
#endif
