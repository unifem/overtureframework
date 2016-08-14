#ifndef OGEN_H 
#define OGEN_H "ogen.h"

#include "Overture.h"
#include "GenericGraphicsInterface.h"
#include "MappingInformation.h"
#include "ExplicitHoleCutter.h"

#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;

int 
checkOverlappingGrid( const CompositeGrid & cg, const int & option=0, bool onlyCheckBaseGrids=true );

// forward class declarations:
class UpdateRefinementData;

class Ogen
{
 public:
  enum MovingGridOption
  {
    useOptimalAlgorithm=0,
    minimizeOverlap=1,
    useFullAlgorithm
  };

  enum OgenParameterEnum
  {
    THEimproveQualityOfInterpolation,
    THEqualityBound,
    THEoutputGridOnFailure,
    THEabortOnAlgorithmFailure,
    THEcomputeGeometryForMovingGrids,
    THEmaximumAngleDifferenceForNormalsOnSharedBoundaries
  };


  Ogen();
  Ogen(GenericGraphicsInterface & ps);
  ~Ogen();
  
  int buildACompositeGrid(CompositeGrid & cg, 
			  MappingInformation & mapInfo, 
			  const IntegerArray & mapList,
			  const int & numberOfMultigridLevels =1,
                          const bool useAnOldGrid=FALSE );

  static bool canDiscretize( MappedGrid & g, const int iv[3], bool checkOneSidedAtBoundaries=true );

  // interactively change parameters
  int changeParameters( CompositeGrid & cg, MappingInformation *mapInfo=NULL );

  //  Check that there are enough parallel ghost lines for ogen: 
  static int checkParallelGhostWidth( CompositeGrid & cg );

  // check the results from updateRefinement
  static int checkUpdateRefinement( GridCollection & gc );

  static int displayCompositeGridParameters( CompositeGrid & cg, FILE *file=stdout );

  // output a list of orphan points
  int printOrphanPoints(CompositeGrid & cg );

  static int saveGridToAFile(CompositeGrid & cg, aString & gridFileName, aString & gridName ) ;

  // set the values of some selected parameters
  void set(const OgenParameterEnum option, const bool value);
  void set(const OgenParameterEnum option, const int value);
  void set(const OgenParameterEnum option, const real value);

  void turnOnHybridHoleCutting() {classifyHolesForHybrid = TRUE;}
  void turnOffHybridHoleCutting() {classifyHolesForHybrid = FALSE;}

  // build a composite grid interactively from scratch
  int updateOverlap( CompositeGrid & cg, MappingInformation & mapInfo );

  // build a composite grid non-interactively using the component grids found
  // in cg. This function might be called if one or more grids have changed.
  int updateOverlap( CompositeGrid & cg );

  // build a composite grid when some grids have moved, tryng to use optimized algorithms.
  int updateOverlap(CompositeGrid & cg, 
		    CompositeGrid & cgNew, 
		    const LogicalArray & hasMoved, 
		    const MovingGridOption & option =useOptimalAlgorithm );

  // update refinement level(s) on a composite grid.
  int updateRefinement(CompositeGrid & cg, 
                       const int & refinementLevel=-1 );

  // new parallel version
  int updateRefinementNew(CompositeGrid & cg, 
                          const int & refinementLevel=-1 );

  // newer parallel version
  int updateRefinementNewer(CompositeGrid & cg, 
                            const int & refinementLevel=-1 );

  int updateRefinementFillInterpolationData(CompositeGrid & cg, UpdateRefinementData & urd);

  int updateRefinementFillInterpolationDataNew(CompositeGrid & cg, UpdateRefinementData & urd);


 protected:

  int buildBounds( CompositeGrid & cg );
  
  int buildCutout(CompositeGrid & cg, MappingInformation & cutMapInfo );

  
  int chooseASide( MappedGrid & mg, int & side, int & axis );
  
  bool interpolateAPoint(CompositeGrid & cg, int grid, int iv[3], bool interpolatePoint,
			bool checkInterpolationCoords, bool checkBoundaryPoint, int infoLevel );
  
  int interpolatePoints(CompositeGrid & cg, int grid, int numberToInterpolate, const IntegerArray & ia, 
                        IntegerArray & interpolates );
  
  int interpolateMixedBoundary(CompositeGrid & cg, int mixedBoundaryNumber );
  
  int interpolateMixedBoundary(CompositeGrid & cg, int mixedBoundaryNumber,
                               int side, int axis, int grid, MappedGrid & g, int grid2, MappedGrid & g2,
			       int offset[3], real rScale[3],real rOffset[3]);

  int checkInterpolationOnBoundaries(CompositeGrid & cg);

  int classifyPoints(CompositeGrid & cg,
		     realSerialArray & invalidPoint, 
		     int & numberOfInvalidPoints,
		     const int & level,
		     CompositeGrid & cg0 );

  int classifyRedundantPoints( CompositeGrid& cg, const int & grid, 
			       const int & level, 
			       CompositeGrid & cg0  );

  int computeOverlap( CompositeGrid & cg, 
		      CompositeGrid & cgOld,
                      const int & level=0,
		      const bool & movingGrids =FALSE, 
		      const IntegerArray & hasMoved = Overture::nullIntArray() );

  int checkForOrphanPointsOnBoundaries(CompositeGrid & cg );

  // older version:
  int cutHoles(CompositeGrid & cg );
  
  // current default version: 
  int cutHolesNew(CompositeGrid & cg );

  // new parallel version
  int cutHolesNewer(CompositeGrid & cg );

  // explicit hole cutting: 
  int explicitHoleCutting( CompositeGrid & cg );

  int getHoleWidth( CompositeGrid & cg,
		    MappedGrid & g2, 
		    int pHoleMarker[3], 
		    IntegerArray & holeCenter, 
		    IntegerArray & holeMask, 
		    IntegerArray & holeWidth, 
		    RealArray & r,
		    RealArray & x,
		    const int *pIndexRange2,
		    const int *pExtendedIndexRange2,
		    const int *plocalIndexBounds2,
		    int iv[3], int jv[3], int jpv[3],
                    bool isPeriodic2[3], bool isPeriodic2p[3], 
		    const Index Iv[3], 
		    const int &grid, const int &grid2,
		    int &ib, int &ib2,
		    int & skipThisPoint,
		    int & initialPoint,
		    const int & numberOfDimensions,
		    const int & axisp1, const int & axisp2, const real & cellCenterOffset,
                    const int & maximumHoleWidth, int & numberOfHoleWidthWarnings );

  int countCrossingsWithRealBoundary(CompositeGrid & cg, 
				     const realArray & x, 
				     IntegerArray & crossings );
  
  int findTrueBoundary(CompositeGrid & cg);

  int interpolateAll(CompositeGrid & cg, IntegerArray & numberOfInterpolationPoints, CompositeGrid & cg0);

  bool isNeededForDiscretization(MappedGrid& g, const int iv[3] );
  bool isOnInterpolationBoundary(MappedGrid& g, const int iv[3], const int & width=1 );
  bool isNeededForMultigridRestriction(CompositeGrid& c,
				       const int & grid,
				       const int & l,
				       const int iv[3]);
  real computeInterpolationQuality(CompositeGrid & cg, const int & grid,
                                   const int & i1, const int & i2, const int & i3,
                                   real & qForward, real & qReverse, 
                                   const int qualityAlgorithm );
  
  int markMaskAtGhost( CompositeGrid & cg );

  int markPointsNeededForInterpolation( CompositeGrid & cg, const int & grid, const int & lowerOrUpper=-1 );

  // parallel version:
  int markPointsNeededForInterpolationNew( CompositeGrid & cg, const int & grid, const int & lowerOrUpper=-1 );
  
  int markPointsReallyNeededForInterpolation( CompositeGrid & cg );

//   int markPartiallyPeriodicBoundaries( CompositeGrid & cg,
// 				       intArray *iInterp  );
  
  int improveQuality( CompositeGrid & cg, const int & grid, RealArray & removedPointBound );
  int updateCanInterpolate( CompositeGrid & cg, CompositeGrid & cg0, RealArray & removedPointBound );

  int plot(const aString & title,
	   CompositeGrid & cg,
           const int & queryForChanges =TRUE );
  
  // new: 
  int projectToParameterBoundary( const real rv0[3], const real rv1[3], real rv[3], 
	  			  const int numberOfDimensions, const int grid );

  int projectToBoundary( CompositeGrid & cg,
			 const int & grid, 
			 const realArray & r,
			 const int iv[3], 
			 const int ivp[3], 
			 real rv[3] );

  int queryAPoint(CompositeGrid & cg);
  
  // int projectGhostPoints(CompositeGrid & cg);

  int removeExteriorPoints(CompositeGrid & cg, 
			   const bool boundariesHaveCutHoles= FALSE );
  
  int removeExteriorPointsNew(CompositeGrid & cg, 
			   const bool boundariesHaveCutHoles= FALSE );
  
  int sweepOutHolePoints(CompositeGrid & cg );
  
  int unmarkBoundaryInterpolationPoints( CompositeGrid & cg, const int & grid );

  int unmarkInterpolationPoints( CompositeGrid & cg, const bool & unMarkAll=FALSE );

  int updateGeometry(CompositeGrid & cg,
		     CompositeGrid & cgOld,
		     const bool & movingGrids=FALSE, 
		     const IntegerArray & hasMoved = Overture::nullIntArray() );

 public:
  int debug;   // for turning on debug info and extra plotting
  int info;    // bit flag for turning on info messages
  bool useNewMovingUpdate;
  int isMovingGridProblem; 
  int defaultInterpolationIsImplicit;
  int myid;  // processor number
  int defaultNumberOfGhostPoints;
  bool loadBalanceGrids;       // load balance cg when it is created
  bool doubleCheckInterpolation; // double check interpolation in checkOverlappingGrid

  // This database contains the parameters and data for Ogen *new way* 
  DataBase dbase;

 protected:
  // repeat some enumerators to simplify  
  enum
  {
    THEinverseMap = CompositeGrid::THEinverseMap,
    THEmask = CompositeGrid::THEmask,
    resetTheGrid=123456789,
    ISnonCuttingBoundaryPoint=MappedGrid::ISreservedBit1
  };

  GenericGraphicsInterface *ps;
  GraphicsParameters psp;
  bool plotTitles;
  bool makeAdjustmentsForNearbyBoundaries;
  
  bool outputGridOnFailure;  // save the grid in a file if the algorithm fails
  bool abortOnAlgorithmFailure;

  FILE *logFile,*plogFile;    // log file to save info for users (plogFile = log file for a given processor)
  FILE *checkFile;  // contains data on the grid that we can use to check when we change the code.
  
  real boundaryEps;  // We can still interpolate from a grid provided we are this close (in r)
  RealArray gridScale;  // gridScale(grid) = maximum length of the bounding box for a grid
  RealArray rBound;     // rBound(0:1,.) : bounds on r for valid interpolation
  real maximumAngleDifferenceForNormalsOnSharedBoundaries;
  
  int computeGeometryForMovingGrids;

  IntegerArray geometryNeedsUpdating;  // true if the geometry needs to be updated after changes in parameters
  bool numberOfGridsHasChanged;  // true if we need to update stuff that depends on the number of grids
  bool checkForOneSided;
  IntegerArray isNew;           // this grid is in the list of new grids (for the incremental algorithm)
  
  int numberOfHolePoints;
  RealArray holePoint;
  IntegerArray plotHolePoints;       // plotHolePoints(grid) : 0 = no, 1=colour black, 2=colour by grid

  int numberOfOrphanPoints;
  RealArray orphanPoint;
  IntegerArray plotOrphanPoints;
  
  int maximumNumberOfPointsToInvertAtOneTime;  // limit the number of pts we invert at a time to save memory

  int maskRatio[3];  // for multigrid: ratio of coarse to fine grid spacing for ray tracing
  
  int holeCuttingOption;   // 0=old way, 1=new way
  bool useAnOldGrid;
  
  int numberOfArrays;   // to check for memory leaks, track of how many arrays we have.
  
  // warnForSharedSides : TRUE if we have warned about possible shared sides not being marked properly
  IntegerArray warnForSharedSides;  // warnForSharedSides(grid,side+2*axis,grid2,side2+2*dir)

  // for mixed physical-interpolation boundaries
  int numberOfMixedBoundaries;
  IntegerArray mixedBoundary;
  RealArray mixedBoundaryValue;

  // for manual hole cutting
  int numberOfManualHoles;
  IntegerArray manualHole;
  RealArray manualHoleValue;

  // For explicit hole cutters
  bool plotExplicitHoleCutters;
  std::vector<ExplicitHoleCutter> explicitHoleCutter;

  // for manual shared sides 
  int numberOfManualSharedBoundaries;
  IntegerArray manualSharedBoundary;
  RealArray manualSharedBoundaryValue;

  // For over-riding the default shared boundary tolerances
  int numberOfSharedBoundaryTolerances;
  IntegerArray sharedBoundaryTolerances;
  RealArray sharedBoundaryTolerancesValue;


  // For specifying non-cutting boundary points (to prevent a portion of a boundary from cutting holes)
  int numberOfNonCuttingBoundaries;
  IntegerArray nonCuttingBoundaryPoints;
  

  intSerialArray *backupValues;  // holds info on backup values.
  IntegerArray backupValuesUsed;   // backupValuesUsed(grid) = TRUE is some backup values have been used
  
  IntegerArray preInterpolate;  // for grids that pre-interpolate

  MappingInformation cutMapInfo;  // put here temporarily. Holds curves that cut holes.
  bool useBoundaryAdjustment;
  bool improveQualityOfInterpolation;
  real qualityBound;
  bool minimizeTheOverlap;    // *** this should be in the CompositeGrid
  bool allowHangingInterpolation;
  bool allowBackupRules;
  bool classifyHolesForHybrid;
  int incrementalHoleSweep;   // if >0 holds the number of sweeps.
  bool useLocalBoundingBoxes;  // new  parallel option

  char buff[200];

  real totalTime;   // total CPU time used to generate the grid
  real timeUpdateGeometry;
  real timeInterpolateBoundaries;
  real timeCutHoles;
  real timeCheckHoleCutting;
  real timeFindTrueBoundary;
  real timeRemoveExteriorPoints;
  real timeImproperInterpolation;
  real timeProperInterpolation;
  real timeAllInterpolation;
  real timeRemoveRedundant;
  real timeImproveQuality;
  real timePreInterpolate;
  
  int adjustBoundary(CompositeGrid & cg,
                     const Integer&      k1,
		     const Integer&      k2,
		     const intSerialArray& i1,
		     const realSerialArray&    x);
  int adjustBoundarySerial(CompositeGrid & cg,
			   const Integer&      k1,
			   const Integer&      k2,
			   const intSerialArray& i1,
			   const realSerialArray&    x);
 int oppositeBoundaryIndex(MappedGrid & g, const int & ks, const int & kd );
#ifdef USE_PPP
  int adjustBoundary(CompositeGrid & cg,
                     const Integer&      k1,
		     const Integer&      k2,
		     const intArray& i1,
		     const realArray&    x);
#endif

  int adjustForNearbyBoundaries(CompositeGrid & cg,
                                IntegerArray & numberOfInterpolationPoints,
                                intSerialArray *iInterp );
  
  int determineBoundaryPointsToAdjust(CompositeGrid & cg, 
				      const int grid, 
				      const int grid2,
				      IntegerArray & sidesShare, 
				      const int ks1, const int kd1, const int ks2, const int kd2, 
				      BoundaryAdjustment & bA , bool & first, bool & needAdjustment,
                                      int numberOfDirectionsAdjusted, bool & directionAdjusted, bool & wasAdjusted,
				      Range & R, IntegerArray & ia, IntegerArray & ok, 
                                      const int it, real shareTol[3][2],
				      RealArray & r, RealArray & r2, RealArray & r3, RealArray & rOk, 
				      RealArray & xx, RealArray & x2, RealArray & x3  );
  
  int checkBoundaryAdjustment(CompositeGrid & cg, 
			      const int grid, 
			      const int grid2,
			      const int ks1, const int kd1,
			      BoundaryAdjustment & bA ,
			      int numberOfDirectionsAdjusted, 
			      Range & R, IntegerArray & ia, IntegerArray & ok,
			      RealArray & r, RealArray & r2, RealArray & r3, RealArray & rOk, 
			      RealArray & xx, RealArray & x2, RealArray & x3);
  
  int checkForBoundaryAdjustments(CompositeGrid & cg, int k1, int k2, IntegerArray & sidesShare,
                                  bool & needAdjustment, int manualSharedBoundaryNumber[2][3]  );

  int getAdjustmentVectors(CompositeGrid & cg, BoundaryAdjustment& bA, 
                           int grid, int grid2, bool & needAdjustment, int numberOfPoints,
			   int it, int ks1, int kd1, int ks2, int kd2, Index Iv[3], RealArray & x1 );

public:
  static int checkCanInterpolate(CompositeGrid & cg ,
		                 int grid, int donor, int numberToCheck,
                                 RealArray & r, IntegerArray & interpolates,
				 IntegerArray & useBackupRules );

  static int checkCanInterpolate(CompositeGrid & cg ,
		                 int grid, int donor, RealArray & r, IntegerArray & interpolates,
          	   	         IntegerArray & useBackupRules );

protected:
  bool canInterpolate(CompositeGrid & cg,
		      const Integer&      k10,
		      const Integer&      k20,
		      const RealArray&    r,
		      const LogicalArray& ok,
		      const LogicalArray& useBackupRules,
		      const Logical       checkForOneSided);

  int checkCrossings(CompositeGrid & cg,
		     const int & numToCheck, 
		     const IntegerArray & ia, 
		     intArray & mask,
		     realArray & x,
		     realArray & vertex,
		     IntegerArray & crossings,
		     const Range & Rx,
                     const int & usedPoint );
  
  int checkHoleCutting(CompositeGrid & cg);
  
  int computeInterpolationStencil(CompositeGrid & cg, 
				  const int & grid, 
				  const int & gridI, 
				  const real r[3], 
				  int stencil[3][2],
				  bool useOneSidedAtBoundaries = true,
				  bool useOddInterpolationWidth = false  );

  int conformToCmpgrd( CompositeGrid & cg );

  int determineMinimalIndexRange( CompositeGrid & cg );

  int estimateSharedBoundaryTolerance(CompositeGrid & cg);

  int findBestGuess(CompositeGrid & cg, 
		    const int & grid, 
		    const int & numberToCheck, 
		    intSerialArray & ia, 
		    realSerialArray & x, 
		    realSerialArray & r,
                    realSerialArray & rI,
                    intSerialArray & inverseGrid,
                    const realSerialArray & center );

  int findClosestBoundaryPoint( MappedGrid & mg, real *x, int *iv, int *ivb, int & sideb, int & axisb );

  int generateInterpolationArrays( CompositeGrid & cg, 
				   const IntegerArray & numberOfInterpolationPoints,
				   intSerialArray *iInterp );
  
  int initialize();

  int lastChanceInterpolation(CompositeGrid & cg,
                              CompositeGrid & cg0, 
			      const int & grid,
			      const IntegerArray & ia,
                              const IntegerArray & ok,
                              intSerialArray & interpolates,
			      int & numberOfInvalidPoints,
			      realSerialArray & invalidPoint,
			      const int & printDiagnosticMessages = false,
                              const bool & tryBackupRules = false,
                              const bool saveInvalidPoints = true,
                              int lastChanceOption = 0  );

  int movingUpdate(CompositeGrid & cg, 
		   CompositeGrid & cgOld, 
		   const LogicalArray & hasMoved, 
		   const MovingGridOption & option =useOptimalAlgorithm );

  int movingUpdateNew(CompositeGrid & cg, 
		      CompositeGrid & cgOld, 
		      const LogicalArray & hasMoved, 
		      const MovingGridOption & option =useOptimalAlgorithm );

  int preInterpolateGrids(CompositeGrid & cg);
  
public:
  int resetGrid( CompositeGrid & cg );
  
protected:
  int setGridParameters(CompositeGrid & cg );

  inline bool sidesShareBoundary( CompositeGrid & cg, int grid1, int side1, int dir1, int grid2, int side2, int  dir2 ) const;

  void getSharedBoundaryTolerances( CompositeGrid & cg, int grid1, int side1, int dir1, int grid2, int side2, int  dir2,
                                    real & rTol, real & xTol, real & nTol ) const;

  int updateBoundaryAdjustment( CompositeGrid & cg, 
				const int & grid, 
				const int & grid2,
				intSerialArray *iag,
				realSerialArray *rg,
				realSerialArray *xg,
				IntegerArray & sidesShare );

 public:
  int updateParameters(CompositeGrid & cg, const int level = -1, 
                       const RealArray & minimumOverlap =  Overture::nullRealArray() );
 protected:
  // int updateMaskPeriodicity(MappedGrid & c, const int & i1, const int & i2, const int & i3 );
  int getNormal(const int & numberOfDimensions, const int & side, const int & axis, 
                const RealArray & xr, real signForJacobian, RealArray & normal);

//   int markOffAxisRefinementMask( int numberOfDimensions, Range Ivr[3], Range Ivb[3], int rf[3], 
//                          intArray & mask, const intArray & maskb );

//   int setRefinementMaskFace(intArray & mask,
//                             int side, int axis, 
// 			    int numberOfDimensions, int rf[3],
// 			    Range & I1r, Range & I2r, Range & I3r,
// 			    const intArray & mask00, 
// 			    const intArray & mask10,
// 			    const intArray & mask01,
// 			    const intArray & mask11);
  
  int markOffAxisRefinementMask( int numberOfDimensions, Index Ivr[3], Index Ivb[3], int rf[3], 
                               intSerialArray & mask, const intSerialArray & maskb );

  int setRefinementMaskFace(intSerialArray & mask,
                            int side, int axis, 
			    int numberOfDimensions, int rf[3],
			    Index & I1r, Index & I2r, Index & I3r,
			    const intSerialArray & mask00, 
			    const intSerialArray & mask10,
			    const intSerialArray & mask01,
			    const intSerialArray & mask11);
  
  int checkRefinementInterpolation( CompositeGrid & cg );
  int checkRefinementInterpolationNew( CompositeGrid & cg );
  
};

//! return true if grid1 face=(side1,dir1) shares a boundary with grid2 face=(side2,dir2)
bool Ogen::
sidesShareBoundary( CompositeGrid & cg, int grid1, int side1, int dir1, int grid2, int side2, int dir2 ) const
{
  int share1= cg[grid1].sharedBoundaryFlag(side1,dir1);
  int share2= cg[grid2].sharedBoundaryFlag(side2,dir2);
  
  int bc1=cg[grid1].boundaryCondition(side1,dir1);
  int bc2=cg[grid2].boundaryCondition(side2,dir2);
  if( share1==share2 && share1!=0 && share2!=0 && bc1>0 && bc2>0 )
  {
    return true;
  }
  for( int n=0; n<numberOfManualSharedBoundaries; n++ )
  {
    if( manualSharedBoundary(n,0)==grid1 && manualSharedBoundary(n,3)==grid2 &&
	manualSharedBoundary(n,1)==side1 && manualSharedBoundary(n,2)==dir1 &&
	manualSharedBoundary(n,4)==side2 && manualSharedBoundary(n,5)==dir2 )
    {
      return true;
    }
  }

  return false;
}


#endif

