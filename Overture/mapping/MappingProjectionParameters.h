#ifndef MAPPING_PROJECTION_PARAMETERS_H
#define MAPPING_PROJECTION_PARAMETERS_H

#include "Mapping.h"

//--------------------------------------------------------------------------
/// \brief  This class holds parameters for the Mapping project function.
/// 
/// \details
///   It holds state values that depend on the type of Mapping being projected,
/// either a standard Mapping or a CompositeSurface
//--------------------------------------------------------------------------
class MappingProjectionParameters
{
 public:

  enum IntegerArrayName
  {
    subSurfaceIndex=0,           
    ignoreThisSubSurface=1,
    elementIndex=2,
    numberOfIntegerArrayParameters=3
  };
  enum RealArrayName
  {
    r=0,
    x=1,
    xr=2,
    normal=3,
    numberOfRealArrayParameters=4
  };


  MappingProjectionParameters();
  ~MappingProjectionParameters();

  MappingProjectionParameters & operator =( const MappingProjectionParameters & x );

  // indicate whether the projection is being used with a marching algorithm
  int setIsAMarchingAlgorithm(const bool & trueOrFalse =true );
  bool isAMarchingAlgorithm() const;

  // adjust for corners when marching over a surface.
  int setAdjustForCornersWhenMarching(const bool & trueOrFalse =true );
  bool adjustForCornersWhenMarching(){return adjustForCorners;} //

  // This next option is used when first projecting onto the triangulation before
  // projecting onto the CompositeSurface -- since we just want the subsurface info
  // but do not want to change the positions of points, unless they were adjusted at corners.
  int setOnlyChangePointsAdjustedForCornersWhenMarching(const bool & trueOrFalse =true );
  bool onlyChangePointsAdjustedForCornersWhenMarching(){return onlyChangePointsAdjustedForCorners;} // fixed 070427 *wdh* 

  // if on a corner, choose the normal which best matches the input normal.
  int setMatchNormals(const bool & trueOrFalse =true );
  bool getMatchNormals() const{ return matchNormals;} //
  
  // Project onto the reference surface (if false, use the surface triangulation if it exists)
  int setProjectOntoReferenceSurface(const bool & trueOrFalse =true );
  bool projectOntoReferenceSurface() const{ return projectOntoTheReferenceSurface;} //
  
  // reset the parameters:
  int reset();

  // for tree search on unstructured grids:
  int setSearchBoundingBoxSize( real estimated, real maximumAllowed=0. );
  
  IntegerDistributedArray & getIntArray(const IntegerArrayName & name);

  RealDistributedArray & getRealArray(const RealArrayName & name);

  real searchBoundingBoxSize, searchBoundingBoxMaximumSize;

 private:

  bool marching;
  bool adjustForCorners;
  bool matchNormals;
  bool onlyChangePointsAdjustedForCorners;
  bool projectOntoTheReferenceSurface;

  // All the arrays are saved in the following two arrays of pointers
  intArray *integerArrayParameter[numberOfIntegerArrayParameters];
  realArray *realArrayParameter[numberOfRealArrayParameters];

};


#endif  
