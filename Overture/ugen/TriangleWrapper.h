#ifndef __OV_TRIANGLEWRAPPER_H__
#define __OV_TRIANGLEWRAPPER_H__

#include "aString.H"
// AP #include "Overture.h"
#include "OvertureTypes.h"
#include "wdhdefs.h" // for sPrintF, etc.
#include "mathutil.h" // for min, max, etc.

class TriangleWrapperParameters
{

public:
  TriangleWrapperParameters( real minAng=-1, bool freeze=false, real maxA=-1, bool vor=true,
           bool neighbours=false, bool quiet=true, int verbose=0  ) : 
    minAngle(minAng), freezeSegments(freeze), maxArea(maxA), voronoi(vor), saveNeighbours(neighbours),
        quietMode(quiet), verboseMode(verbose) { }

  TriangleWrapperParameters( const TriangleWrapperParameters & triw ) : 
    minAngle(triw.minAngle), freezeSegments(triw.freezeSegments), maxArea(triw.maxArea) { }

  ~TriangleWrapperParameters() { }

  void setMinimumAngle(real angle) { minAngle = angle; }
  void toggleFreezeSegments()      { freezeSegments = !freezeSegments; }
  void setMaximumArea(real area)   { maxArea = area; }
  void toggleVoronoi()             { voronoi = !voronoi; }
  void saveVoronoi(bool trueOrFalse=true){ voronoi=trueOrFalse; }
  void saveNeighbourList(bool trueOrFalse=true){ saveNeighbours=trueOrFalse; }
  void setQuietMode(bool trueOrFalse=true){ quietMode=trueOrFalse; }
  // turn on verbose mode, level=1 is "V", level=2 is "VV" level=3 is "VVV"
  void setVerboseMode(int level){ verboseMode=level; }

  real getMinimumAngle() const   { return minAngle; }
  bool getFreezeSegments() const { return freezeSegments; }
  real getMaximumArea() const    { return maxArea; }
  bool getVoronoi() const        { return voronoi; }
  bool getSaveNeighbours()       { return saveNeighbours; }

  aString generateArgumentString() const;

private:
  
  real minAngle;
  bool freezeSegments;
  real maxArea;
  bool voronoi;
  bool saveNeighbours;
  bool quietMode;
  int verboseMode;
  
};


class TriangleWrapper
{

public:

  TriangleWrapper();

  TriangleWrapper( intArray & faces, realArray & xyz ) 
  { initialize( faces, xyz ) ; }

  ~TriangleWrapper() { }

  void initialize( const intArray & faces, const realArray & xyz );
  void setHoles( const realArray &holes );
  void generate();
  
  TriangleWrapperParameters & getParameters() { return parameters; }

  int getNumberOfEdges() const { return numberOfEdges; }  //
  int getNumberOfBoundaryEdges() const { return numberOfBoundaryEdges; }  //

  const intArray & generateElementList() const { return triangles; }
  const realArray & getPoints() const { return points; }
  const intArray & getInitialFaces2TriangleMapping() const { return initialFaces2TriangleMapping; }
  const intArray & getNeighbours() const { return neighbours; }

protected:
  TriangleWrapperParameters parameters;

private:
  intArray initialFaces;
  realArray initialPoints;
  realArray holes;

  intArray triangles;
  realArray points;
  intArray initialFaces2TriangleMapping;
  intArray neighbours;
  
  int numberOfEdges, numberOfBoundaryEdges;
  
};

#endif
