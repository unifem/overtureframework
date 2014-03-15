#ifndef __EDIT_NURBS_CURVE__
#define __EDIT_NURBS_CURVE__

#include "GenericGraphicsInterface.h"
#include "GUIState.h"
#include "NurbsMapping.h"
#include "IntersectionMapping.h"
#include "MappingInformation.h"

#define SC (char *)(const char *)

enum MouseSelectMode{
  nothing=0,
  buildPoint,
  interpolateCurve,
  hideCurve,
  lineSegmentJoin,
  endpointMove,
  intersection,
  split,
  updateCurve,
  curveAssembly,
  numberOfMouseModes
};
			

int snapCurvesToIntersection(GenericGraphicsInterface & gi, 
			     NurbsMapping & curve, 
			     int &curve1, int &curve2, 
			     int curve1End, int curve2End,
			     const real *xSelect,
			     const real *c1click );

int assembleSubCurves(int & currentCurve,
		      GenericGraphicsInterface & gi, 
		      NurbsMapping & curve,
		      NurbsMapping & newCurve,
		      int & numberOfAssembledCurves,
		      NurbsMapping ** & assemblyCurves,
		      MouseSelectMode & mouseMode,
		      bool & curveRebuilt,
		      bool & plotCurve );

int editNurbsCurve( NurbsMapping &curve, GenericGraphicsInterface& gi );

#endif

