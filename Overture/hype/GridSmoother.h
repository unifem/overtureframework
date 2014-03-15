#ifndef GRID_SMOOTHER_H
#define GRID_SMOOTHER_H

#include "Mapping.h"
#include "GenericGraphicsInterface.h"
#include "MappingProjectionParameters.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <vector>
OV_USINGNAMESPACE(std);
#else
#include <vector.h>
#endif

class MatchingCurve;
class DataPointMapping;
class DialogData;
class MappingInformation;

// ==================================================================================
/// \brief Smooth a volume or surface grid (used with the Hyperbolic grid generator).
// ==================================================================================
class GridSmoother
{
  public:

  enum BoundaryConditionEnum
  {
    periodic=-1,
    pointsFixed,
    pointsSlide,
    boundaryIsSmoothed
  };

  enum StretchingEnum
  {
    lineAttraction=1,   // must be the same as the value in ellipticSmooth.f
    pointAttraction 
  };

   GridSmoother(int domainDimension, int rangeDimension);
   ~GridSmoother();

  int buildDialog(DialogData & dialog);

  bool updateOptions(aString & answer, DialogData & dialog, MappingInformation & mapInfo );

  void reset();  // call this function when the initial grid has been recomputed and thus control function will change

  void setWeights( real arcLength, real curvature, real area );

  int setBoundaryConditions( int bc[2][3] );
  int setBoundaryConditions( IntegerArray & bc );

  int setBoundaryMappings( Mapping *boundaryMapping[2][3] );
  int setMatchingCurves(vector<MatchingCurve> & matchingCurves );

  int periodicUpdate( realArray & x, const 
		      IntegerArray & indexRange );

  int smooth(Mapping & map,
	     DataPointMapping & dpm, 
	     GenericGraphicsInterface & gi, 
	     GraphicsParameters & parameters,
             int projectGhost[2][3] );


  int applyBoundaryConditions(Mapping & map, DataPointMapping & dpm, 
			      realArray & x, 
			      const IntegerArray & indexRange, const IntegerArray & gids, 
			      const Index Iv[3], const Index Jv[3], const Index Kv[3], realArray & normal,
                              bool projectSurfaceGrids=true );


 protected:

  void computeNormals( realArray & normal, const Index & I1, const Index & I2, const Index & I3, const realArray & x );

  int domainDimension, rangeDimension;

  int numberOfIterations;
  int totalIterations;
  int numberOfEquidistributionIterations;

  int numberOfWeightSmooths;
  int numberOfLaplacianSmooths;
  int numberOfEllipticSmooths;

  int smoothNormals;
  int numberOfNormalSmooths;
  real blendingFactor;

  int useInitialGridAsControlGrid;
  int numberOfControlFunctionSmooths;
  bool controlFunctionComputed;

  int projectSmoothedGridOntoReferenceSurface;
  int smoothGridGhostPoints;
  int smoothingOffset[2][3];
  int smoothingRegion[2][3];
  real maximumProjectionCorrection;

  IntegerArray bc;
  Mapping *boundaryMapping[2][3];

  real omega;
  real arclengthWeight, curvatureWeight, areaWeight;
  IntegerArray regionsNotToProject;

  IntegerArray ipar;
  RealArray rpar;

  MappingProjectionParameters mpParams;
  vector<MatchingCurve> matchingCurves; // array of matching curves

  realArray source;  // holds control function

};


#endif
