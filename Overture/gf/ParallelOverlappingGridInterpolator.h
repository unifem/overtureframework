#ifndef ParallelOverlappingGridInterpolator_h
#define ParallelOverlappingGridInterpolator_h

#include "Overture.h"
#include "A++.h"
#include "OvertureDefine.h"

#include "SparseArray.h"

#ifndef OV_USE_DOUBLE
// typedef float real;
// typedef floatArray realArray;
// typedef floatSerialArray realSerialArray;
// #define MPI_Real MPI_FLOAT
#else
// typedef double real;
// typedef doubleArray realArray;
// typedef doubleSerialArray realSerialArray;
// #define MPI_Real MPI_DOUBLE
#endif

#ifndef _CompositeGrid
class realCompositeGridFunction;  // forward declaration
#endif


class ParallelOverlappingGridInterpolator
{
 public:
  
  enum ExplicitInterpolationStorageOptionEnum
  {
    precomputeAllCoefficients,     // requires w^d coefficients per interp pt (w=width of interp stencil)
    precomputeSomeCoefficients,    // requires w*d coefficients per interp pt (d=dimension, 1,2, or 3)
    precomputeNoCoefficients       // requires d coefficients per interp point
  };

  ParallelOverlappingGridInterpolator();
  ~ParallelOverlappingGridInterpolator();


 // turn on or off the computation of the residual (for implicit interpolation)
 void turnOnResidualComputation(const bool trueOrFalse=true );

 int getMaximumRefinementLevelToInterpolate() const;

 // return the maximum residual from the last call to interpolate
 real getMaximumResidual() const;

 int interpolate( realCompositeGridFunction & u, 
		   const Range & C0 = nullRange,      // optionally specify components to interpolate
		   const Range & C1 = nullRange,  
		   const Range & C2 = nullRange );

 int interpolate( int gridToInterpolate,             // only interpolate this grid.
                  realCompositeGridFunction & u, 
		  const Range & C0 = nullRange,      // optionally specify components to interpolate
		  const Range & C1 = nullRange,  
		  const Range & C2 = nullRange );

 int interpolate( realCompositeGridFunction & u, 
                  const IntegerArray & gridsToInterpolate,  // specify which grids to interpolate
		  const Range & C0 = nullRange,      // optionally specify components to interpolate
		  const Range & C1 = nullRange,  
		  const Range & C2 = nullRange );

 int interpolate( realCompositeGridFunction & u,
                  const IntegerArray & gridsToInterpolate,      // specify which grids to interpolate
                  const IntegerArray & gridsToInterpolateFrom,  // specify which grids to interpolate from
		  const Range & C0 = nullRange,      // optionally specify components to interpolate
		  const Range & C1 = nullRange,  
		  const Range & C2 = nullRange );

 int interpolate( realArray & ui,                    // save results here
                  int gridToInterpolate,             // only interpolate values on this grid that
                  int interpoleeGrid,                // interpolate from this grid.
                  realCompositeGridFunction & u, 
		  const Range & C0 = nullRange,      // optionally specify components to interpolate
		  const Range & C1 = nullRange,  
		  const Range & C2 = nullRange );


 int setExplicitInterpolationStorageOption( ExplicitInterpolationStorageOptionEnum option);

 int setMaximumRefinementLevelToInterpolate(int maxLevelToInterpolate );

 int setup( realCompositeGridFunction & u );

 int setup();
  
 // return size of this object  
 real sizeOf(FILE *file = NULL ) const;

 int updateToMatchGrid(CompositeGrid & cg, int refinementLevel =0 );
 int updateToMatchGrid(realCompositeGridFunction & u, int refinementLevel =0 );


 // these next functions are for testing

 int bruteForceInterpolate( realCompositeGridFunction & u );

 real computeError();

 int resetSolution();

 static int debug;

 FILE *debugFile;

 protected:
   ExplicitInterpolationStorageOptionEnum explicitInterpolationStorageOption;

  int initializeExplicitInterpolation();

  // Here is the routine that actually performs the interpolation
  int internalInterpolate( realCompositeGridFunction & u, 
			   const Range & C0, 
			   const Range & C1,
			   const Range & C2,
			   const IntegerArray *gridsToInterpolate = NULL,      // specify which grids to interpolate
			   const IntegerArray *gridsToInterpolateFrom = NULL );

  // destroy all data
  int destroy();


  int numberOfDimensions,numberOfComponentGrids,numberOfBaseGrids;
  intSerialArray numberOfInterpolationPoints;
  intSerialArray *interpolationPoint, *interpoleeLocation, *interpoleeGrid, *variableInterpolationWidth;
  realSerialArray *interpolationCoordinates;
 
  intSerialArray numberOfInterpolationPointsPerDonor, interpolationStartEndIndex;
  // todo: SparseArray<int> numberOfInterpolationPointsPerDonor, interpolationStartEndIndex;

  intSerialArray *dimension, *indexRange, *isCellCentered;
  realSerialArray *gridSpacing;

  realArray *ucg;
  realArray *vcg;  // for brute force solution

  int maxInterpolationWidth;
  int coeffWidthDimension;  // dimension of the 2nd component of coeff
  int maximumRefinementLevelToInterpolate;

  // Store interp data in sparse arrays:
  SparseArray<int> nila,nipa;
  SparseArray<int*> ila,ipa;
  SparseArray<real*> cia,coeffa;

  bool computeResidual;
  real maximumResidual;

  bool allGridsHaveLocalData, onlyAmrGridsHaveLocalData, noGridsHaveLocalData;

  #ifdef USE_PPP
    MPI_Comm POGI_COMM;  // Communicator for the parallel interpolator
  #else
    int POGI_COMM;
  #endif

};


#endif
