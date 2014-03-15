#include "Mapping.h"
#include "HyperbolicMapping.h"
#include "ReparameterizationTransform.h"
#include "FaceInfo.h"
#include "UnstructuredMapping.h"

// This file is placed in the static library so that the following global variables
// are initialized properly when using a dyanmic library (libOverture.so)

// *** NOTE: do not create any static P++ arrays here since they should not be consctructed before P++ is initialized

int ReparameterizationTransform::
localParamsAreBeingUsed[ReparameterizationTransform::maximumNumberOfRecursionLevels]={0,0,0,0,0,0,0,0,0,0}; 

MappingParameters *ReparameterizationTransform::
localParams[ReparameterizationTransform::maximumNumberOfRecursionLevels]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}; 


aString HyperbolicMapping::
boundaryConditionName[]=
{
  "unknown",
  "freeFloating", 
  "outwardSplay",   
  "fixXfloatYZ",
  "fixYfloatXZ",
  "fixZfloatXY",
  "floatXfixYZ",
  "floatYfixXZ",
  "floatZfixXY",
  "floatCollapsed",
  "periodic",
  "xSymmetryPlane",
  "ySymmetryPlane",
  "zSymmetryPlane",
  "singularAxis",
  "matchToMapping",
  "matchToPlane",
  "trailingEdge",
  "matchToABoundaryCurve",
  "parallelGhostBoundary"
};

aString HyperbolicMapping::
ghostBoundaryConditionName[]=
{
  "defaultGhostBoundaryCondition",
  "orthogonalBlendGhostBoundaryCondition",
  "normalGhostBoundaryCondition",
  "evenSymmetryGhostBoundaryCondition"
};

/// an array usefull for diagnostics involving EntityTypeEnum
aString UnstructuredMapping::EntityTypeStrings[] = { "Vertex",
						     "Edge",
						     "Face",
						     "Region",
						     "Mesh" };		

aString UnstructuredMapping::ElementTypeStrings[] = { "triangle",
						      "quadrilateral",
						      "tetrahedron",
						      "pyramid",
						      "triPrism",
						      "septahedron",  
						      "hexahedron",
						      "other",
						      "boundary" };
	      

// This is from FaceInfo.h
int CurveSegment::globalCount = 0;

void 
initStaticMappingVariables()
{
}
