#ifndef HYPERBOLIC_MAPPING_H
#define HYPERBOLIC_MAPPING_H 

#include "Mapping.h"
#include "GenericGraphicsInterface.h"
#include "DataPointMapping.h"
#include "MappingProjectionParameters.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <vector>
OV_USINGNAMESPACE(std);
#else
#include <vector.h>
#endif

#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;

class TridiagonalSolver;
class DialogData;
class MatchingCurve;

//----------------------------------------------------------------------
/// \brief  Define a Mapping by Hyperbolic grid generation.
//----------------------------------------------------------------------
class HyperbolicMapping : public Mapping
{
public:

enum HyperbolicParameter
{
  numberOfRegionsInTheNormalDirection=0,
  stretchingInTheNormalDirection,
  linesInTheNormalDirection,  
  dissipation,
  distanceToMarch, 
  spacing, 
  volumeParameters, 
  barthImplicitness,
  axisParameters,
  growInTheReverseDirection,
  growInBothDirections,
  projectGhostBoundaries,
  THEboundaryConditions,
  THEtargetGridSpacing,  // tangential directions
  THEinitialGridSpacing, // initial grid spacing in marching direction for volume grids
  THEspacingType,
  THEspacingOption,
  THEgeometricFactor,     // geometric spacing ratio
  THEghostBoundaryConditions,
  THEapplyBoundaryConditionsToStartCurve
};

enum Direction
{
  bothDirections=0,
  forwardDirection,
  reverseDirection
};
  

enum SpacingType
{
  constantSpacing,
  geometricSpacing,
  inverseHyperbolicSpacing,
  oneDimensionalMappingSpacing,
  userDefinedSpacing    
};

enum BoundaryCondition
{
  freeFloating=1,
  outwardSplay,  
  fixXfloatYZ,
  fixYfloatXZ,
  fixZfloatXY,
  floatXfixYZ,
  floatYfixXZ,
  floatZfixXY,
  floatCollapsed,
  periodic,
  xSymmetryPlane,
  ySymmetryPlane,
  zSymmetryPlane,
  singularAxis,
  matchToMapping,
  matchToPlane,
  trailingEdge,
  matchToABoundaryCurve,
  parallelGhostBoundary,
  numberOfBoundaryConditions
};

static aString boundaryConditionName[numberOfBoundaryConditions];

enum GhostBoundaryCondition
{
  defaultGhostBoundaryCondition,
  orthogonalBlendGhostBoundaryCondition,
  normalGhostBoundaryCondition,
  evenSymmetryGhostBoundaryCondition,
  numberOfGhostBoundaryConditions
};

static aString ghostBoundaryConditionName[numberOfGhostBoundaryConditions];

enum InitialCurveEnum
{
  initialCurveFromEdges,
  initialCurveFromCoordinateLine0,
  initialCurveFromCoordinateLine1,
  initialCurveFromCurveOnSurface,
  initialCurveFromBoundaryCurves
};

enum PickingOptionEnum
{
  pickToChooseInitialCurve,
  pickToChooseBoundaryConditionMapping,
  pickToEditAMapping,
  pickToCreateBoundaryCurve,
  pickToDeleteBoundaryCurve,
  pickToHideSubSurface,
  pickToChooseInteriorMatchingCurve,
  pickToQueryAPoint,
  pickOff
} pickingOption;

enum BCoptionEnum
{
  leftForward=0,
  leftBackward,
  rightForward,
  rightBackward
} bcOption;

enum PlotOptionEnum
{
  setPlotBoundsFromGlobalBounds
};
  
 
enum SpacingOptionEnum
{
  spacingFromDistanceAndLines,
  distanceFromLinesAndSpacing,
  linesFromDistanceAndSpacing
} spacingOption;


HyperbolicMapping();

// Copy constructor is deep by default
HyperbolicMapping( const HyperbolicMapping &, const CopyType copyType=DEEP );
HyperbolicMapping(Mapping & surface_);
// for a surface grid:
HyperbolicMapping(Mapping & surface_, Mapping & startingCurve);
  
~HyperbolicMapping();

HyperbolicMapping & operator =( const HyperbolicMapping & X0 );

bool isDefined() const; // return true if the Mapping has been defined.

virtual void display( const aString & label=blankString) const;
void  estimateMarchingParameters( real & estimatedDistanceToMarch, 
				  int & estimatedLinesToMarch, 
				  int directionToMarch,
				  GenericGraphicsInterface & gi );
int getBoundaryCurves( int & numberOfBoundaryCurves_, Mapping **&boundaryCurves_ );
int setBoundaryCurves( const int & numberOfBoundaryCurves_, Mapping **boundaryCurves_ );
int addBoundaryCurves( const int & numberOfExtraBoundaryCurves, Mapping **extraBoundaryCurves  );
int deleteBoundaryCurves();
int deleteBoundaryCurves(IntegerArray & curvesToDelete );
  
// supply a mapping to match a boundary condition to.
int setBoundaryConditionMapping(const int & side, 
				const int & axis,
				Mapping & map,
				const int & mapSide=-1, 
				const int & mapAxis=-1);
  
// supply a curve/surface 
int setSurface(Mapping & surface, bool isSurfaceGrid=true, bool init=true );
void setIsSurfaceGrid( bool trueOrFalse );
  
// supply a starting curve for a surface grid
int setStartingCurve(Mapping & startingCurve, bool init=true );

int setParameters(const HyperbolicParameter & par, 
		  real value,
		  const Direction & direction = bothDirections );

int setParameters(const HyperbolicParameter & par, 
		  int  value,
		  const Direction & direction = bothDirections );

int setParameters(const HyperbolicParameter & par, 
		  const IntegerArray & ipar= Overture::nullIntArray(), 
		  const RealArray & rpar = Overture::nullRealArray(),
		  const Direction & direction = bothDirections );

// save the reference surface and starting curve when 'put' is called.
int saveReferenceSurfaceWhenPut(bool trueOrFalse = TRUE);

int setPlotOption( PlotOptionEnum option, int value );

int generateOld();  // uses hypegen
int generate(const int & numberOfAdditionalSteps = 0 );
  
int generateSerial( const int & numberOfAdditionalSteps = 0 );
int generateParallel( const int & numberOfAdditionalSteps = 0 );

virtual void useRobustInverse(const bool trueOrFalse=TRUE );

virtual void map( const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
		  MappingParameters & params =Overture::nullMappingParameters() );

virtual void basicInverse(const realArray & x, 
			  realArray & r,
			  realArray & rx =Overture::nullRealDistributedArray(),
			  MappingParameters & params =Overture::nullMappingParameters());


virtual void mapS( const RealArray & r, RealArray & x, RealArray &xr = Overture::nullRealArray(),
		   MappingParameters & params =Overture::nullMappingParameters());

virtual void basicInverseS(const RealArray & x, 
			   RealArray & r,
			   RealArray & rx =Overture::nullRealArray(),
			   MappingParameters & params =Overture::nullMappingParameters());

virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

Mapping *make( const aString & mappingClassName );
aString getClassName() const { return HyperbolicMapping::className; }

int update( MappingInformation & mapInfo );
int update( MappingInformation & mapInfo,
	    const aString & command,
	    DialogData *interface  =NULL  ) ;

int useNurbsToEvaluate( bool trueOrFalse );
int setDegreeOfNurbs( int degree );

const Mapping * getSurface() const { return surface; }

const DataPointMapping* getDataPointMapping() const { return dpm; }

protected:

int buildMarchingParametersDialog(DialogData & marchingParametersDialog, aString bcChoices[]);
int assignMarchingParametersDialog(DialogData & marchingParametersDialog, aString bcChoices[] );
bool updateMarchingParameters(aString & answer, DialogData & marchingParametersDialog, aString bcChoices[],
			      MappingInformation & mapInfo );

int buildPlotOptionsDialog(DialogData & plotOptionsDialog,
			   GraphicsParameters & parameters);
bool updatePlotOptions(aString & answer, DialogData & plotOptionsDialog,
		       MappingInformation & mapInfo,
		       GraphicsParameters & parameters,
		       GraphicsParameters & referenceSurfaceParameters );

int buildSurfaceGridParametersDialog(DialogData & plotOptionsDialog);

bool updateSurfaceGridParameters(aString & answer, DialogData & surfaceGridParametersDialog,
				 MappingInformation & mapInfo,
				 GraphicsParameters & referenceSurfaceParameters );

int buildBoundaryConditionMappingDialog(DialogData & dialog );
int updateBoundaryConditionMappingDialog(DialogData & dialog );
bool updateBoundaryConditionMappings(aString & answer, 
				     DialogData & dialog, 
				     bool checkSelection,
				     SelectionInfo & select,
				     MappingInformation & mapInfo );

//    int buildSmoothDialog(DialogData & smoothDialog );
//    bool updateSmoothingOptions(aString & answer, DialogData & smoothDialog, MappingInformation & mapInfo );
//    int smoothGrid();

//    int buildStretchDialog(DialogData & stretchDialog );
//    bool updateStretchOptions(aString & answer, DialogData & dialog, MappingInformation & mapInfo );

int buildMarchingSpacingDialog(DialogData & marchingSpacingDialog );
bool updateMarchingSpacingOptions(aString & answer, DialogData & dialog, MappingInformation & mapInfo );

int buildStartCurveSpacingDialog(DialogData & stretchDialog );
bool updateStartCurveSpacingOptions(aString & answer, DialogData & dialog, MappingInformation & mapInfo );

// old way:
bool updateOld(aString & answer,
	       MappingInformation & mapInfo,
	       GraphicsParameters & referenceSurfaceParameters );

int initialize();
int setup();
int updateForInitialCurve(bool updateNumberOfGridLines = true );
  
int setBoundaryConditionAndOffset(int side, int axis, int bc, int offset=-1 );

int initializeHyperbolicGridParameters();
int hypgen(GenericGraphicsInterface & gi, GraphicsParameters & parameters);
int smooth(GenericGraphicsInterface & gi, GraphicsParameters & parameters);

int evaluateStartCurve( RealArray & xStart );

int initializeMarchingParameters(int numberOfAdditionalSteps, int & i3Start );
int initializeMarchingArrays( int i3Start, int numberOfAdditionalSteps, RealArray & x, RealArray & xt,
			      RealArray & xr, RealArray & normal, RealArray & xrr,
			      RealArray & s, RealArray & ss, RealArray & xrrDotN,
			      RealArray & kappa);
  
int initializeSurfaceGrid(int direction,
			  int numberOfAdditionalSteps, int i3Start, int i3Begin,
			  RealArray & x, RealArray & xt,
			  RealArray & xr, 
			  RealArray & normal, RealArray & ds, RealArray & s, RealArray & ss, RealArray & xrr, 
			  RealArray & normXr, RealArray & normXs,
			  RealArray & xrSave );
  
int removeNormalComponentOfSmoothing(int axis, const Index & I1, const Index & I2, const Index & I3,
				     RealArray & xrr, RealArray & xrrDotN, 
				     RealArray & normal, RealArray & xte );
  
int getNormalAndSurfaceArea(const RealArray & x, 
			    const int & i3, 
			    RealArray & normal, 
			    RealArray & s,
			    RealArray & xr, 
			    RealArray & xrr,
			    const real & dSign,
			    RealArray & normXr,
			    RealArray & normXs,
			    RealArray & ss,
			    const int & marchingDirection,
			    int stepNumber=0  );

int getCurvatureDependentSpeed(RealArray & ds, 
			       RealArray & kappa,
			       const RealArray & xrr, 
			       const RealArray & normal, 
			       const RealArray & normXr, 
			       const RealArray & normXs);
  
int getDistanceToStep(const int & i3Delta, RealArray & ds, const int & growthDirection );
int adjustDistanceToMarch(const int & numberOfAdditionalSteps, const int & growthDirection );
int applyBoundaryConditions( const RealArray & x, const RealArray & x0,
			     const int & marchingDirection,
			     RealArray & normal,
			     RealArray & xr,
			     bool initialStep = false,
			     int stepNumber=0 );

int applyBoundaryConditionMatchToMapping(const RealArray & x, 
					 const int & marchingDirection,
					 RealArray & normal, 
					 RealArray & xr,
					 bool initialStep = false,
					 int option=0 );

int matchToCurve( bool projectBoundary,
		  Index & I1, Index & I2,
		  Mapping & matchingMapping,
		  MappingProjectionParameters & mpParams,
		  int i1Shift, 
		  int i2Shift, 
		  const RealArray & x, 
		  const int & marchingDirection,
		  RealArray & normal, 
		  RealArray & xr,
		  const int sideBlend, const int axisBlend, // side axis for numberOfLinesForNormalBlend
		  bool initialStep = false,
		  int option  =0   );
  
int projectNormalsToMatchCurve(Mapping & matchingMapping,
			       MappingProjectionParameters & mpParams,
			       Index & I1, Index & I2,
			       RealArray & normal, RealArray & nDot );
  
int blendNormals(RealArray & normal, RealArray & xr,
		 int numberOfLinesToBlend,
		 Index Iv[3],
		 int axis, int side);
  

int jacobiSmooth( RealArray & ss, const int & numberOfSmooths );
int formCMatrix(RealArray & xr, 
		RealArray & xt,
		const int & i3Mod2,
		RealArray & normal, 
		RealArray & normXr,
		const int & direction );
  
int computeNonlinearDiffussionCoefficient(const RealArray & normXr, 
					  const RealArray & normXs, 
					  const RealArray & normXt, 
					  const int & direction,
					  int stepNumber );
  
int formBlockTridiagonalSystem(const int & direction, RealArray & xTri);

int implicitSolve(RealArray & xTri, 
		  const int & i3Mod2,
		  RealArray & xr,
		  RealArray & xt,
		  RealArray & normal,
		  RealArray & normXr,
		  RealArray & normXs,
		  RealArray & normXt,
		  TridiagonalSolver & tri,
		  int stepNumber);
  

int createCurveOnSurface( GenericGraphicsInterface & gi,
			  SelectionInfo & select, 
			  Mapping* &curve,
			  Mapping *mapPointer = NULL,
			  real *xCoord = NULL, 
			  real *rCoord = NULL,
			  int *boundaryCurveChosen = NULL,
			  bool resetBoundaryConditions = true );
  
// build a curve from selected curves chosen interactively:
int buildCurve( GenericGraphicsInterface & gi, GraphicsParameters & parameters, DialogData & dialog,
		aString & answer, SelectionInfo & select, Mapping *&newCurve, const aString & buildCurveColour,
		bool resetBoundaryConditions = true );

int computeCellVolumes(const RealArray & xt, const int & i3Mod2,
		       real & minCellVolume, real & maxCellVolume, 
		       const real & dSign );

int equidistributeAndStretch( const int & i3, const RealArray & x, const real & weight, 
			      const int marchingDirection=1, bool stretchGrid = true );
  
RealArray normalize( const RealArray & u );
  
int project( const RealArray & x, 
	     const int & marchingDirection,
	     RealArray & xr, 
	     const bool & setBoundaryConditions= TRUE,
	     bool initialStep = false,
	     int stepNumber = 0);

virtual int project( RealArray & x, 
		     MappingProjectionParameters & mpParams ){ return Mapping::project(x,mpParams); }

int correctProjectionOfInitialCurve(RealArray & x,  RealArray & xr,
				    CompositeSurface & cs,
				    const int & marchingDirection,
				    MappingProjectionParameters & mpParams);
  
void 
plotDirectionArrows(GenericGraphicsInterface & gi, GraphicsParameters & params);

int plotCellQuality(GenericGraphicsInterface & gi, 
		    GraphicsParameters & parameters);
  
int printStatistics(FILE *file=stdout );

// find boundary condition curves to match to
int findMatchingBoundaryCurve(int side, int axis, int directionToMarch, 
			      GenericGraphicsInterface & gi, bool promptForChanges=false );  

int findNormalsToStartCurve(RealArray & r, RealArray & normal, int directionToMarch);
  
// this next function is only intended to be called by the update function
int drawReferenceSurface(GenericGraphicsInterface & gi, 
			 GraphicsParameters & referenceSurfaceParameters,
			 const real & surfaceOffset,
			 const aString & referenceSurfaceColour,
			 const aString & edgeCurveColour );

// this next function is only intended to be called by the update function 
int drawReferenceSurfaceEdges(GenericGraphicsInterface & gi,
			      GraphicsParameters & parameters,
			      const aString *boundaryColour);
// this next function is only intended to be called by the update function 
int drawHyperbolicGrid(GenericGraphicsInterface & gi, 
		       GraphicsParameters & parameters, 
		       bool plotNonPhysicalBoundaries,
		       const real & initialOffset, 
		       const aString & hyperbolicMappingColour );
  
// this next function is only intended to be called by the update function
int drawBoundariesAndCurves(GenericGraphicsInterface& gi, 
			    GraphicsParameters & parameters, 
			    GraphicsParameters & referenceSurfaceParameters, 
			    const real & surfaceOffset, 
			    const real & initialOffset,
			    const aString & boundaryConditionMappingColour,
			    const aString & referenceSurfaceColour,
			    const aString & edgeCurveColour,
			    const aString & buildCurveColour,
			    const aString *boundaryColour );

  
// int destroyInteriorMatchingCurves();

real getDissipationCoefficient( int stepNumber );
  
void setLinesAndDistanceLabels(DialogData & dialog );
void updateLinesAndDistanceToMarch();

// old:  int chooseBoundaryConditionMappings(MappingInformation & mapInfo );
  
bool evaluateTheSurface;   // if TRUE the surface is re-evaluated
RealArray xSurface;        // holds the surface grid 

// parameters for Bill's generator
int growthOption;  // +1, -1 or 2=grow in both directions
real distance[2];
int linesToMarch[2];
real upwindDissipationCoefficient, uniformDissipationCoefficient;
real boundaryUniformDissipationCoefficient;
int dissipationTransition;
  
int numberOfVolumeSmoothingIterations;
real curvatureSpeedCoefficient;
real implicitCoefficient;
bool removeNormalSmoothing;
real equidistributionWeight;
real targetGridSpacing;

int numberOfSmoothingIterations;  // for smoothGrid

IntegerArray boundaryCondition, projectGhostPoints, indexRange,gridIndexRange,dimension;
IntegerArray ghostBoundaryCondition;
Mapping *boundaryConditionMapping[2][2]; // [side][axis] pointers to Mappings used for a boundary condition.

bool boundaryConditionMappingWasNewed[2][2]; // true if the mapping was new'd locally.
// Note: MappingProjectionParameters are quite lite-weight
MappingProjectionParameters boundaryConditionMappingProjectionParameters[2][2];
// surfaceMappingProjectionParameters[0] = backward, [1]=forward, [2]=for initial curve
MappingProjectionParameters surfaceMappingProjectionParameters[3]; // for projecting onto a surface grid
    
// We may match an interior grid line to a curve
vector<MatchingCurve> matchingCurves; // array of matching curves

//  int numberOfMatchingCurves;    // number of interior lines matched
//    real *matchingCurvePosition;    // r coordinate on the start curve where matchin curve starts
//    int *matchingCurveDirection;   // match when growing in this direction (forward or backward)
//    Mapping **matchingCurve;       // project grid line onto this mapping.
//    MappingProjectionParameters **matchingProjectionParameters;

int numberOfLinesForNormalBlend[2][2];
real splayFactor[2][2];   // factor for outward splay BC.
int boundaryOffset[2][3];  // =1 -> move boundary one line in and use last line as the ghost line.
bool boundaryOffsetWasApplied; // this means the grid dimensions were adjusted by the boundaryOffset
int boundaryOffsetOption;    // 0=old way: requires boundaryOffset=1 in marching direction; 1=new way
  
enum GhostLineOptions
{
  extrapolateAnExtraGhostLine,           // ghost line values are extrapolated in the DataPointMapping.
  useLastLineAsGhostLine    // compute ghost lines when marching
} ghostLineOption;  

// why are these all here??
RealArray gridDensityWeight;   // holds the inverse of the desired grid spacing (relative values only)
RealArray xHyper;              // holds the hyperbolic grid.
RealArray xtHyper;             
RealArray normalCC;            // normal at cell centre.
RealArray at,bt,ct,c,lambda;  
  

SpacingType spacingType;
real geometricFactor;
real geometricNormalization[2];
real initialSpacing;
int numberOfLinesWithConstantSpacing;

real minimumGridSpacing;      // stop marching the surface grid when spacing is less than
real arcLengthWeight, curvatureWeight, normalCurvatureWeight;
  
real matchToMappingOrthogonalFactor;

Mapping *normalDistribution;

bool applyBoundaryConditionsToStartCurve;

bool surfaceGrid;      // true if we are generating a surface grid
Mapping *startCurve;  // defines the starting curve for a surface grid
int numberOfPointsOnStartCurve;
real startCurveStart, startCurveEnd;  // bounds on parameter space for the start curve, normally 0. and 1.

bool useStartCurveStretchingWhileMarching;  //  we can stretch just the start curve or also in the interior.
RealArray *pStartCurveStretchParams; //  defines stretching of the starting curve or surface
Mapping *startCurveStretchMapping;
  
bool projectInitialCurve;  // true if we project starting curve onto the surface
bool projectNormalsOnMatchingBoundaries; // if true, project both points and normals to matching boundary curves/surfaces
bool correctProjectionOfInitialCurves; // if true correct the projection of the initial curve to match edges on the triangluation

bool evalAsNurbs;
int nurbsDegree;

bool useTriangulation;      // if true, use the triangulation of a CompositeSurface for marching.
bool projectOntoReferenceSurface;  // if true, project points found using the triangulation onto the actual surface
bool stopOnNegativeCells;   // stop the generation when a negative cell is detected

int saveReferenceSurface;  // // 0=do not save, 1=save for 2D grids, 2=save for all grids 
bool plotBoundaryConditionMappings;
bool plotHyperbolicSurface;
bool plotObject;
bool plotDirectionArrowsOnInitialCurve;
bool plotReferenceSurface;
bool plotGhostPoints;
bool plotTriangulation;
bool plotNegativeCells;
bool plotBoundaryCurves;
bool plotGridPointsOnStartCurve;
int numberOfGhostLinesToPlot;

bool referenceSurfaceHasChanged;
bool choosePlotBoundsFromReferenceSurface;
bool choosePlotBoundsFromGlobalBounds;
  
InitialCurveEnum initialCurveOption;
real edgeCurveMatchingTolerance; // for deciding when edge curves should be joined.
real distanceToBoundaryCurveTolerance; // for deciding if boundary curves match to the start curve

RealArray trailingEdgeDirection;   // for airfoil like trailing edge singularities.

// variables for the surface grid stuff
bool smoothAndProject; // if true smooth and project onto original surface definition.

int numberOfBoundaryCurves;
Mapping **boundaryCurves;
bool initialCurveIsABoundaryCurve;

int totalNumberOfSteps;
enum TimingsEnum
{
  totalTime=0,
  timeForProject,
  timeForSetupRHS,
  timeForImplicitSolve,
  timeForTridiagonalSolve,
  timeForFormBlockTridiagonalSystem,
  timeForFormCMatrix,
  timeForNormalAndSurfaceArea,
  timeForSmoothing,
  timeForUpdate,
  timeForBoundaryConditions,
  numberOfTimings
};
real timing[numberOfTimings];
  

// Hypgen parameters
//   int nzreg,izstrt,ibcja,ibcjb,ibcka,ibckb,imeth,ivspec,itsvol,iaxis;
//   real smu2, epsss, timj,timk , exaxis, volres;
//  intArray npzreg;
//  RealArray zreg,dz0,dz1;

int info;

private:
aString className;
Mapping *surface;       // here is the mapping that we start with
DataPointMapping *dpm;  // Here is where the mapping is defined.

static FILE *debugFile;
static FILE *pDebugFile;
static FILE *checkFile;

// This database contains parameters (new way)
DataBase dbase;

private:

//
//  Virtual member functions used only through class ReferenceCounting:
//
virtual ReferenceCounting& operator=(const ReferenceCounting& x)
{ return operator=((HyperbolicMapping &)x); }
virtual void reference( const ReferenceCounting& x) 
{ reference((HyperbolicMapping &)x); }     // *** Conversion to this class for the virtual = ****
virtual ReferenceCounting* virtualConstructor( const CopyType copyType = DEEP ) const
{ return ::new HyperbolicMapping(*this, copyType); }

};


#endif  
