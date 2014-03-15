#ifndef INTERPOLATE_POINTS_ONAGRID_H
#define INTERPOLATE_POINTS_ONAGRID_H


#include "SparseArray.h"


/// \brief This class can be used to interpolate arbitrary points on a Composite grid
///            (*new* parallel version).
class InterpolatePointsOnAGrid
{
public:

enum InterpolationStatusEnum
{
  notInterpolated=0,
  extrapolated=1,
  interpolated=2
};

enum 
{
  defaultNumberOfValidGhostPoints=-123456,
  interpolateAllGhostPoints=-234567
};

enum ExplicitInterpolationStorageOptionEnum
{
  precomputeAllCoefficients,     // requires w^d coefficients per interp pt (w=width of interp stencil)
  precomputeSomeCoefficients,    // requires w*d coefficients per interp pt (d=dimension, 1,2, or 3)
  precomputeNoCoefficients       // requires d coefficients per interp point
};


enum InterpolationTypeEnum
{
  implicitInterpolation=0,
  explicitInterpolation
};


InterpolatePointsOnAGrid();
~InterpolatePointsOnAGrid();
  
// This function will find the nearest valid (mask!=0) grid point on a CompositeGrid
static int findNearestValidGridPoint( CompositeGrid & cg, const RealArray & x, IntegerArray & il, RealArray & ci );

int getMaximumRefinementLevelToInterpolate() const;

// These functions return info about the last interpolation operation
int getNumberBackupInterpolation() const;
int getNumberExtrapolated() const;
int getNumberInterpolated() const;
int getNumberUnassigned() const;


// Utility function to compute the lower left index in the interpolation stencil
static int getInterpolationStencil( MappedGrid & mg, const int width, const real *rv, int *iv );

// return the index values and interpoleeGrid for the last interpolation.
int getInterpolationInfo(CompositeGrid & cg, IntegerArray & indexValues, IntegerArray & interpoleeGrid) const;

// return the index values, interpoleeGrid and interpolationCoordinates for the last interpolation.
int getInterpolationInfo(CompositeGrid & cg, IntegerArray & indexValues, IntegerArray & interpoleeGrid, RealArray & interpolationCoordinates ) const;

// return the status array for the last interpolation.
const IntegerArray & getStatus() const;

// return the total number of points that are assigned from a grid
int getTotalNumberOfPointsAssigned( int grid );

// Interpolate points : The first time this function is called the interpolation schedule will be generated.
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

// Indicate whether all points should be assigned, using extrapolation if necessary
int setAssignAllPoints( bool trueOrFalse=true );

// return size of this object  
real sizeOf(FILE *file = NULL ) const;

int setExplicitInterpolationStorageOption( ExplicitInterpolationStorageOptionEnum option);

// flag for specfying what information messages should be printed:
int setInfoLevel( int info );

// Set the width the interpolation stencil.
int setInterpolationWidth( int width );

// Set the offset in grid lines from the unit cube where we are allowed to interpolate
int setInterpolationOffset( real widthInGridLines );

// Specify whether to use implicit or explicit interpolation
int setInterpolationType( InterpolationTypeEnum interpType );

int setMaximumRefinementLevelToInterpolate(int maxLevelToInterpolate );

// Set the number of valid ghost points that can be used when interpolating from a grid function
int setNumberOfValidGhostPoints( int numValidGhost=defaultNumberOfValidGhostPoints );

static int debug;

protected:

friend class InterfaceTransfer;

int getInternalInterpolationInfo(CompositeGrid & cg, IntegerArray & indexValues,IntegerArray & interpoleeGrid, 
				 RealArray & interpolationCoordinates, const int option ) const;

int parallelInterpolate( RealArray & ui,                    // save results here
			 const realCompositeGridFunction & u,     // interpolate from this grid function.
			 const Range & C0 = nullRange,      // optionally specify components to interpolate
			 const Range & C1 = nullRange,  
			 const Range & C2 = nullRange );

// Setup routine that creates the parallel communication schedule for interpolation a list of points
int parallelSetup( CompositeGrid & cg,
		   const RealArray & xp, 
		   const IntegerArray & numberOfInterpolationPoints, 
		   const RealArray *interpolationCoordinates, 
		   const IntegerArray *indirection, 
		   const IntegerArray *interpolationLocation,
		   const IntegerArray *variableInterpolationWidth );

int interpolationIsInitialized;  // this is set to true when the interpolation has been initialized.

int numberOfBackupInterpolation;
int numberOfExtrapolated;
int numberOfInterpolated;
int numberOfUnassigned;

IntegerArray *indirection;
IntegerArray *interpolationLocation;
IntegerArray *variableInterpolationWidth;
RealArray *interpolationCoordinates;

IntegerArray numberOfInterpolationPoints;
IntegerArray status;

real interpolationOffset;     // offset in grid lines from the unit cube where we are allowed to interpolate

int interpolationWidth;       // width of the interpolation stencil.
InterpolationTypeEnum interpolationType;        // explicit or implicit interpolation 
int infoLevel;                // flag for specifying what information messages should be printed.
int numberOfValidGhostPoints;

bool assignAllPoints;  // if true assign all points using extrapolation if necessary

// For interpolateAll:
int totalNumToInterpolate;
IntegerArray *pInterpAllIndirection;
int *numToInterpolatePerGrid;

// ParallelOverlappingGridInterpolatePoints *pogip; // for parallel interpolation of points


FILE *logFile,*plogFile;    // log file to save info for users (plogFile = log file for a given processor)

// -- replace the next by plogFile: 
FILE *debugFile;

ExplicitInterpolationStorageOptionEnum explicitInterpolationStorageOption;

int initializeExplicitInterpolation(CompositeGrid & cg);

// Here is the routine that actually performs the interpolation in serial
int internalInterpolate( RealArray & ui,                       // save results here
			 const realCompositeGridFunction & u,  // interpolate from this grid function.
			 const Range & C0, 
			 const Range & C1,
			 const Range & C2 );

// Here is the routine that actually performs the interpolation in parallel
int parallelInternalInterpolate( RealArray & ui,                       // save results here
				 const realCompositeGridFunction & u,  // interpolate from this grid function.
				 const Range & C0, 
				 const Range & C1,
				 const Range & C2 );

// destroy all data
int destroy();

// put this here for now:
//   static int checkCanInterpolate(CompositeGrid & cg ,
// 			         int grid, int donor, RealArray & r, IntegerArray & interpolates,
// 			         IntegerArray & useBackupRules );

int numberOfDimensions,numberOfComponentGrids,numberOfBaseGrids;
int maxInterpolationWidth;
int coeffWidthDimension;  // dimension of the 2nd component of coeff
int maximumRefinementLevelToInterpolate;

// Store interp data in sparse arrays:
SparseArray<int> nila,nipa;
SparseArray<int*> ila,ipa;
SparseArray<real*> cia,coeffa;

int npr,nps;
int *pMapr, *pMaps;

bool allGridsHaveLocalData, onlyAmrGridsHaveLocalData, noGridsHaveLocalData;

#ifdef USE_PPP
 MPI_Comm POGI_COMM;  // Communicator for the parallel interpolator
#else
 int POGI_COMM;
#endif

};

#endif
