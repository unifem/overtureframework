#ifndef EXPOSED_POINTS_H
#define EXPOSED_POINTS_H

#include "Overture.h"

// This class is used to interpolate "exposed" points for moving grid computations.

// forward declarations:
class OGFunction;
class InterpolatePointsOnAGrid;


class ExposedPoints
{

public:

  // There are different types of exposed points
  enum ExposedPointTypeEnum
  {
    exposedPointIsNeededForDiscretization,
    exposedDiscretization
  };

 enum InterpolationQualityEnum
 {
   canInterpolateQuality1=0,     // best quality, interpolates to correct order
   canInterpolateQuality2,       // 2nd best best quality, interpolates to correct order minus 1
   canInterpolateQuality3,       // 3rd best quality, interpolates to correct order minus 2
   canInterpolateWithExtrapolation, // bad quality -- should replace by a better quality result
   canInterpolateQualityBad,     // bad quality -- should replace by a better quality result
   canInterpolateQualityVeryBad, // worst quality-- should replace by a better quality result
   canNotInterpolate,             // unable to interpolate at all
   numberOfInterpolationQualityTypes
 };

ExposedPoints();
~ExposedPoints();


// set whether or not to interpolate exposed interpolation points (kkc 110323)
void setFillExposedInterpolationPoints(bool trueOrFalse);

// return the total number of exposed points (if grid=-1), otherwise return the number of exposed points on a grid
int getNumberOfExposedPoints(const int grid=-1 ) const;

// build the lists of exposed points; determine how to interpolate
int initialize(CompositeGrid& cg1, CompositeGrid & cg2,
	       int stencilWidth = -1  );

// interpolate values at the exposed points
int interpolate(realCompositeGridFunction & u1,
		OGFunction *TZFlow  =NULL,
		real t =0. );

// indicate whether interpolation neighbours are already assigned.
void setAssumeInterpolationNeighboursAreAssigned( const bool trueOrFalse=true );

// choose the type of exposed points that are wanted (do this before calling initialize)
void setExposedPointType( const ExposedPointTypeEnum type );

// Set the width the interpolation stencil.
int setInterpolationWidth( int width );

// Set the number of valid ghost points that can be used when interpolating from a grid function
int setNumberOfValidGhostPoints( int numValidGhost );

// Choose the interpolation option
int setInterpolationOption( int option );

static int debug;
static int info;

protected:

ExposedPointTypeEnum exposedPointType;

int isInitialized;  // is set to 1 after initialization.
int ipogIsInitialized;  // set to 1 when the ipog object is initialized

int numberOfExposedPoints, totalNumberOfExposedPoints; 
int assumeInterpolationNeighboursAreAssigned; // default for stencilWidth=5
int interpolationWidth;
int numberOfValidGhostPoints;

int useIPOG;  // if true use ipog in serial too
InterpolatePointsOnAGrid *ipog;  // new way for parallel 
IntegerArray periodicUpdateNeeded;

IntegerArray ia_;
IntegerArray numberPerGrid, totalNumberPerGrid;
RealArray x_;

int *numDonor;

IntegerArray *ib;

IntegerArray exposedInterpoleeGrid,
  exposedInterpoleeLocation,
  exposedInterpolationPoint,
  exposedVariableInterpolationWidth,
  exposedInterpolationQuality;

RealArray exposedInterpolationCoordinates;

bool fillExposedInterpolationPoints; // kkc 110323

static FILE *debugFile;

static int 
getInterpolationStencil(CompositeGrid & cg1,
                        const int grid2,
                        const int numToCheck, IntegerArray & ia, IntegerArray & ibg, RealArray & r2, 
                        IntegerArray & interpolationQuality,
                        IntegerArray & interpoleeGrid,
                        IntegerArray & interpoleeLocation,
                        IntegerArray & interpolationPoint,
                        IntegerArray & variableInterpolationWidth,
                        RealArray & interpolationCoordinates );

static int 
checkForBetterQualityInterpolation( CompositeGrid & cg1, const int gridI, const int numToCheck, 
                                    IntegerArray & ia, IntegerArray & ic, 
                                    const RealArray & x, 
				    IntegerArray & interpolationQuality,
				    IntegerArray & interpoleeGrid,
				    IntegerArray & interpoleeLocation,
				    IntegerArray & interpolationPoint,
				    IntegerArray & variableInterpolationWidth,
				    RealArray & interpolationCoordinates );

static int 
interpolatePoints(const realCompositeGridFunction & u,
		  RealArray & uInterpolated_, 
		  int *numDonor, const IntegerArray & ia_, const IntegerArray *ib, 
		  const IntegerArray & interpoleeGrid_,
		  const IntegerArray & interpoleeLocation_,
		  const IntegerArray & interpolationPoint_,
		  const IntegerArray & variableInterpolationWidth_,
		  const RealArray & interpolationCoordinates_,
		  const Range & R0 =nullRange,           
		  const Range & R1 =nullRange,
		  const Range & R2 =nullRange,
		  const Range & R3 =nullRange,
		  const Range & R4 =nullRange );

};



#endif
