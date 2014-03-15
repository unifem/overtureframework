#ifndef COMPOSITE_TOPOLOGY_H
#define COMPOSITE_TOPOLOGY_H

#define AP_EXPERIMENT

#include "Mapping.h"
#include "ArraySimple.h"

class CompositeSurface;
class UnstructuredMapping;
class EdgeCurve;
class NurbsMapping;

#include "FaceInfo.h"

template<class T> class  GeometricADT;
typedef class GeometricADT<int> IntADT;
typedef class GeometricADT<EdgeInfo *> EdgeInfoADT;

// ==================================================================================
/// \brief Define the topology of a CompositeSurface through shared edges.
// ==================================================================================
class CompositeTopology
{
public:

enum EdgeCurveStatusEnum
{
  edgeCurveIsNotMerged=0,  // was 0
  edgeCurveIsSplit,      // was -1 
  edgeCurveIsMerged,     // was 1
  edgeCurveIsRemoved,     // was 2
  edgeCurveIsNotDefined
};

CompositeTopology(CompositeSurface & cs);
~CompositeTopology();

int 
cleanup();  
  
CompositeTopology & 
operator =( const CompositeTopology & X );

int 
update();

int 
getNumberOfEdgeCurves();
  
EdgeCurveStatusEnum 
getEdgeCurveStatus(int number);

Mapping& 
getEdgeCurve(int number);

int
getNearestEdge(real x[3]);

int 
findBoundaryCurves(int & numberOfBoundaryCurves, Mapping **& boundaryCurves );

UnstructuredMapping* 
getTriangulation() const { return globalTriangulationForVisibleSurfaces==0 ? 
			     globalTriangulation :globalTriangulationForVisibleSurfaces; } 

int 
buildTriangulationForVisibleSurfaces();

bool 
topologyDetermined(){ return signForNormal.getLength(0)>0; } // ** fix this ***
  
const IntegerArray & 
getSignForNormal() const { return signForNormal;} 
  
int 
getEdgeFromEndPoints(real *x0, real *x1);
  
virtual int 
get( const GenericDataBase & dir, const aString & name);    // get from a database file
virtual int 
put( GenericDataBase & dir, const aString & name) const;    // put to a database file

// kkc 
UnstructuredMapping *
getTriangulationSurface(int s) {return (triangulationSurface? triangulationSurface[s]: NULL);}
 
// kkc
 void setMaximumArea(real maxa);
 //kkc
 real getMaximumArea() const;
 // kkc
 void setDeltaS(real ds);
 real getDeltaS() const;
 // kkc 
 bool computeTopology(GenericGraphicsInterface & gi, int debug=0);
 // kkc
 void invalidateTopology();

// kkc moved to public interface for unstructured mesh generation (need to project onto these!)  
EdgeInfo *
edgeFromNumber(int n);
// kkc moved to public interface
int 
buildEdgeCurveSearchTree( int & debug );

protected:

void 
setupAllEdges();
void
adjustEndPoints();
bool
checkConsistency(bool quiet = false);

int 
initializeTopology();
  
int
computeNumberOfGridPoints( real arcLength, realArray & x, real & maxCurvature );
  
int 
buildEdgeSegment(Mapping & trimCurve,
		 Mapping & surface,
		 NurbsMapping *&edge,
		 int & numberOfGridPoints,
		 real & arcLength,
		 int & debug,
		 GenericGraphicsInterface & gi );

int 
buildEdgeCurves( GenericGraphicsInterface & gi  );

// build grid points on edge curves to be used with the triangulation.
int 
buildEdgeCurveBoundaryNodes();

// kkc moved to public interface
//int 
//buildEdgeCurveSearchTree( int & debug );
  
int 
getAnotherEdge(EdgeInfo* &edgeChosen, 
	       GenericGraphicsInterface & gi,
	       const aString & question, 
	       const aString & cancel );

int 
merge( EdgeInfo * e, int debug );
int 
mergeEdgeCurves(EdgeInfo & e, EdgeInfo & e2, int debug=0);  // try to force a merge
// workhorse, should not be called directly
bool 
mergeTwoEdges(EdgeInfo *e, EdgeInfo *eMin, int orientation, real tolerance, int debug); 

// try to join adjacent edge curves into one.
int 
joinEdgeCurves(EdgeInfo &e1, bool toNext, int debug =0);
  
int 
splitAndMergeEdgeCurves(GenericGraphicsInterface & gi, int & debug );

int 
splitEdge( EdgeInfo **er, real rSplit, int & debug, bool mergeNewEdges = true );

int 
triangulateCompositeSurface(int & debug, GenericGraphicsInterface & gi, GraphicsParameters & params);

int 
buildSubSurfaceTriangulation(int s, 
			     IntegerArray & numberOfBoundaryNodes,
			     realArray * rCoordinates,
			     IntegerArray *edgeNodeInfop,
			     IntegerArray *boundaryNodeInfop,
			     int & totalNumberOfNodes,
			     int & totalNumberOfFaces,
			     int & totalNumberOfElements,
			     real & totalTimeToBuildSeparateTriangulations,
			     real & totalTriangleTime,
			     real & totalNurbTime,
			     real & totalResolutionTime,
			     int & debug,
			     GenericGraphicsInterface & gi,
			     GraphicsParameters& params);

int 
printEdgeCurveInfo(GenericGraphicsInterface & gi);

void 
printInfoForAnEdgeCurve( EdgeInfo * e );

// kkc moved to public interface for unstructured mesh generation (need to project onto these!)  
//EdgeInfo *
//edgeFromNumber(int n);
void
unMergeEdge(EdgeInfo & e);

CompositeSurface & cs;

//! Holds the global triangulation
UnstructuredMapping *globalTriangulation;
//! Holds the triangulation when some surfaces are hidden.
UnstructuredMapping *globalTriangulationForVisibleSurfaces;
  
//! An array of pointers to edge curves (which has a pointer to the corresponding CurveSegment object).
EdgeInfo **allEdges; // pointer array to all boundary and master EdgeInfo's
int numberOfUniqueEdgeCurves; // number of boundary and master EdgeInfo's

int numberOfEdgeCurves; // number of EdgeInfos created 

//! tolerance for merging adjacent edge curves.
real mergeTolerance;
//! splitTolerance = mergeTolerance * splitToleranceFactor
real splitToleranceFactor;  
//! Suggested arclength distance between points on the edge curves of the global triangulation.
real deltaS;
real curvatureTolerance;
real curveResolutionTolerance; 

//! for increasing the size of bounding boxes when searching
real boundingBoxExtension;  
  
real maximumArea;

real maxDist; // default max distance from midpoint of edge to surface
bool improveTri;
  
//! Minimum number of points to place on an edge curve (over-rides deltaS).
int minNumberOfPointsOnAnEdge;
  
IntegerArray signForNormal;

EdgeInfoADT *searchTree;

ArraySimple<real> boundingBox;
  
//! An array of triangulations for each sub-surface
UnstructuredMapping **triangulationSurface;
  
//! True if the global triangulation has been built.
bool triangulationIsValid;

bool edgeCurvesAreBuilt;  
bool mergedCurvesAreValid;   // set to true when merged curves are consistent with current parameters
bool recomputeEdgeCurveBoundaryNodes; // set to true if user changes deltaS
  
real maximumMergeDistance;
real averageMergeDistance;
real minimumUnmergedDistance;

// end point variables
int numberOfEndPoints;
EdgeInfoArray masterEdge;  // pointer to the master edge
realArray endPoint;     // point coordinates

FaceInfo * faceInfoArray; // virtual topology datastructure
int numberOfFaces;

EdgeInfoArray unusedEdges;
};



#endif
