#ifndef FACE_INFO_H
#define FACE_INFO_H

#include "Mapping.h"
#include "NurbsMapping.h"
#include "GenericGraphicsInterface.h"
#include "CompositeSurface.h"
#include "TrimmedMapping.h"

class EdgeInfo;
class FaceInfo;
class Loop;
class EdgeInfoArray;

// ====================================================================================================
/// \brief Define a curve segment for the trimmed mapping and topology.
// ====================================================================================================
class CurveSegment
{
public:
CurveSegment();
CurveSegment(NurbsMapping &newSegment, int sp, int surf, NurbsMapping *sLoop, NurbsMapping *sCurve);
~CurveSegment();
NurbsMapping *
getNURBS(){return map;}
// note that usage includes the initialCurve pointer in the EdgeInfo object,
// so for a boundary curve, both the curve and initialCurve will point to the same CurveSegment.
bool
isBoundary(){return usage == 2;} 
bool
isManifold(){return usage == 3;}
bool
isNonManifold(){return usage > 3;}
static int
getGlobalCount(){return globalCount;}
static void
resetGlobalCount(){globalCount=0;}
int
getCurveNumber(){return curveNumber;}
int
put(GenericDataBase & dir, const aString & name, CompositeSurface & cs);
int
get(GenericDataBase & dir, const aString & name, CompositeSurface & cs, NurbsMapping **allSurfaceLoops);

int startingPoint, endingPoint; 
int newStartPoint, newEndPoint;

int usage; // delete the object when the usage goes down to 0. A usage of 2 for all sub-curves indicates
// a topologically closed model, usage==1 is a boundary curve and usage>=3 indicates non-manifold geometry.

int numberOfGridPoints; // be aware that numberOfGridPoints really is redundant and it is the 
// number of grid points from getNURBS()->getGridDimension(axis) that is used when plotting and triangulating
real arcLength;
int surfaceNumber; 

// surfaceLoop is the trimming curve in parameter space and subCurve 
// is the subCurve of the trimming curve corresponding to this edge (newSegment).
// For untrimmed surfaces surfaceLoop points to a curve around the unit square and subCurve points
// to one of its segments.
NurbsMapping *surfaceLoop, *subCurve; 

// old way: just store the number for the trimming curve and sub-curve. This makes it hard to deal with
// splitting and joining of edges. BoundaryCurve was used for untrimmed surfaces, to indicate the side 
// and axis of the unit square.
//int surfaceLoop, subCurve, boundaryCurve;
private:
NurbsMapping *map; // all curves are represented as NURBS mappings
static int globalCount;
int curveNumber;
};

// ====================================================================================================
/// \brief Object that holds information about a curve edge for the trimmed mapping and topology.
// ====================================================================================================
class EdgeInfo
{
public:
EdgeInfo();
EdgeInfo(CurveSegment *newCurve, int l, int f, int o, int e);
~EdgeInfo();
int
getStartPoint();
int
getEndPoint();
bool
setStartPoint(int np, realArray & endPoint, real mergeTolerance, int firstEdgeNumber, EdgeInfoArray &masterEdge,
	      EdgeInfoArray &unusedEdges);
bool
setEndPoint(int np, realArray & endPoint, real mergeTolerance, int firstEdgeNumber, EdgeInfoArray &masterEdge,
	    EdgeInfoArray &unusedEdges);
bool
adjustOneSegmentEndPoints(realArray & endPoint, real mergeTolerance);
void
eraseEdge();
int
masterEdgeNumber();
void
setUnused(EdgeInfoArray &unusedEdges);
int
put(GenericDataBase & dir, const aString & name);
int
get(GenericDataBase & dir, const aString & name, CurveSegment * allCurveSegments[]);
void
assignPointers(EdgeInfo * allEdgeInfos[]);

EdgeInfo *next, *prev, *slave, *master;
Loop *loopy; // pointer to the loop where this edge lives (useful for removal of zero-length edges)

int orientation, loopNumber, faceNumber, edgeNumber, dList, startLastChangedBy, endLastChangedBy;
CurveSegment *curve, *initialCurve;
enum EdgeCurveStatusEnum
{
  edgeCurveIsBoundary=0,
  edgeCurveIsMaster,
  edgeCurveIsSlave,
  edgeCurveIsNotUsed
} status;

private:
EdgeInfo(EdgeInfo & dum){}
// only used during a get() before all pointers have been assigned
int prevNumber, nextNumber, slaveNumber, masterNumber; 
};


// ====================================================================================================
/// \brief Class that holds an array of EdgeInfo objects.
// ====================================================================================================
class EdgeInfoArray
{
public:
EdgeInfoArray();
~EdgeInfoArray();
void
resize(int size);
int
getLength(){return nMax;}
void
push(EdgeInfo &e);
EdgeInfo *
pop();
int
put(GenericDataBase & dir, const aString & name) const;
int 
get(GenericDataBase & dir, const aString & name, EdgeInfo * allEdgeInfos[]);

int nMax;
EdgeInfo **array;
private:
EdgeInfoArray(EdgeInfoArray & dum){}
int sp;
};


// ====================================================================================================
/// \brief Class that holds a loop of edges for the topology computation.
// ====================================================================================================
class Loop
{
public:
Loop();
~Loop();

void 
insertEdge(EdgeInfo * newEdge);

bool
addEdge(EdgeInfo * newEdge, EdgeInfo * loc);

bool
replaceEdge(EdgeInfo *newEdge, EdgeInfo * oldEdge);

bool
removeEdge(EdgeInfo * oldEdge);

bool
deleteEdge(EdgeInfo * oldEdge);

int
numberOfEdges();

void
assignEndPointNumbers();

bool
edgeInLoop(EdgeInfo * oldEdge);

int
put(GenericDataBase & dir, const aString & name);

int 
get(GenericDataBase & dir, const aString & name, EdgeInfo * allEdgeInfos[]);

EdgeInfo *firstEdge, *lastEdge;
int trimOrientation; // 1 for outer curves, -1 for inner curves
};


// ====================================================================================================
/// \brief Class that holds information about a face for the topology computation.
// ====================================================================================================
class FaceInfo
{
public:

FaceInfo();

~FaceInfo();

void
allocateLoops(int nol);

int
put(GenericDataBase & dir, const aString & name);

int 
get(GenericDataBase & dir, const aString & name, EdgeInfo * allEdgeInfos[]);

int numberOfLoops;
Loop *loop;
};

#endif
