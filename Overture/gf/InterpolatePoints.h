#ifndef INTERPOLATE_POINTS_CG_H
#define INTERPOLATE_POINTS_CG_H


// class that can be used to interpolate arbitrary points on a Composite grid.


class InterpolatePoints
{
 public:

  enum InterpolationStatusEnum
  {
    notInterpolated=0,
    interpolated,
    extrapolated
  };

  enum 
  {
    defaultNumberOfValidGhostPoints=-123456,
    interpolateAllGhostPoints=-234567
  };



  InterpolatePoints();
  ~InterpolatePoints();
  
  int interpolatePoints(const RealArray & positionToInterpolate,
		  const realCompositeGridFunction & u,
		  RealArray & uInterpolated, 
		  const Range & R0=nullRange,           
		  const Range & R1=nullRange,
		  const Range & R2=nullRange,
		  const Range & R3=nullRange,
		  const Range & R4=nullRange );

  int interpolationCoefficients(const CompositeGrid &cg,
	  		        RealArray & uInterpolationCoeff);

  int interpolateAllPoints(const realCompositeGridFunction & uFrom,
			         realCompositeGridFunction & uTo, 
                           const Range & componentsFrom=nullRange, 
                           const Range & componentsTo=nullRange,
                           const int numberOfGhostPointsToInterpolate=interpolateAllGhostPoints );
  
  int interpolateAllPoints(const realCompositeGridFunction & uFrom,
			         realMappedGridFunction & uTo, 
                           const Range & componentsFrom=nullRange, 
                           const Range & componentsTo=nullRange,
                           const int numberOfGhostPointsToInterpolate=interpolateAllGhostPoints );
  

  // Here is a way to interpolate in two steps, this will save the interpolation info
  int buildInterpolationInfo(const RealArray & positionToInterpolate,CompositeGrid & cg,
                             RealArray *projectedPoints=NULL,
                             IntegerArray *checkTheseGrids=NULL );
  int interpolatePoints(const realCompositeGridFunction & u,
			RealArray & uInterpolated, 
			const Range & R0=nullRange,           
			const Range & R1=nullRange,
			const Range & R2=nullRange,
			const Range & R3=nullRange,
			const Range & R4=nullRange );

  // return the status array for the last interpolation.
  const IntegerArray & getStatus() const;

  // return the index values and interpoleeGrid for the last interpolation.
  int getInterpolationInfo(CompositeGrid & cg, IntegerArray & indexValues, IntegerArray & interpoleeGrid) const;

  // flag for specfying what information messages should be printed:
  int setInfoLevel( int info );

  // Set the offset in grid lines from the unit cube where we are allowed to interpolate
  int setInterpolationOffset( real widthInGridLines );

  // set the number of valid ghost points 
  int setNumberOfValidGhostPoints( int numValidGhost=defaultNumberOfValidGhostPoints );

 static int debug;

 protected:

  IntegerArray *indirection;
  IntegerArray *interpolationLocation;
  IntegerArray *interpolationLocationPlus;
  RealArray *interpolationCoordinates;

  IntegerArray numberOfInterpolationPoints;
  IntegerArray status;

  real interpolationOffset;  // offset in grid lines from the unit cube where we are allowed to interpolate

  int infoLevel;  // flag for specfying what information messages should be printed.
  int numberOfValidGhostPoints;

};

#endif
