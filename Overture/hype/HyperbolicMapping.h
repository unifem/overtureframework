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
int generateNew(const int & numberOfAdditionalSteps = 0 );  // same as above.
  
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

int evaluateStartCurve( realArray & xStart );

int initializeMarchingParameters(int numberOfAdditionalSteps, int & i3Start );
int initializeMarchingArrays( int i3Start, int numberOfAdditionalSteps, realArray & x, realArray & xt,
			      realArray & xr, realArray & normal, realArray & xrr,
			      realArray & s, realArray & ss, realArray & xrrDotN,
			      realArray & kappa);
  
int initializeSurfaceGrid(int direction,
			  int numberOfAdditionalSteps, int i3Start, int i3Begin,
			  realArray & x, realArray & xt,
			  realArray & xr, 
			  realArray & normal, realArray & ds, realArray & s, realArray & ss, realArray & xrr, 
			  realArray & normXr, realArray & normXs,
			  realArray & xrSave );
  
int removeNormalComponentOfSmoothing(int axis, const Index & I1, const Index & I2, const Index & I3,
				     realArray & xrr, realArray & xrrDotN, 
				     realArray & normal, realArray & xte );
  
int getNormalAndSurfaceArea(const realArray & x, 
			    const int & i3, 
			    realArray & normal, 
			    realArray & s,
			    realArray & xr, 
			    realArray & xrr,
			    const real & dSign,
			    realArray & normXr,
			    realArray & normXs,
			    realArray & ss,
			    const int & marchingDirection,
			    int stepNumber=0  );

int getCurvatureDependentSpeed(realArray & ds, 
			       realArray & kappa,
			       const realArray & xrr, 
			       const realArray & normal, 
			       const realArray & normXr, 
			       const realArray & normXs);
  
int getDistanceToStep(const int & i3Delta, realArray & ds, const int & growthDirection );
int adjustDistanceToMarch(const int & numberOfAdditionalSteps, const int & growthDirection );
int applyBoundaryConditions( const realArray & x, const realArray & x0,
			     const int & marchingDirection,
			     realArray & normal,
			     realArray & xr,
			     bool initialStep = false,
			     int stepNumber=0 );

int applyBoundaryConditionMatchToMapping(const realArray & x, 
					 const int & marchingDirection,
					 realArray & normal, 
					 realArray & xr,
					 bool initialStep = false,
					 int option=0 );

int matchToCurve( bool projectBoundary,
		  Index & I1, Index & I2,
		  Mapping & matchingMapping,
		  MappingProjectionParameters & mpParams,
		  int i1Shift, 
		  int i2Shift, 
		  const realArray & x, 
		  const int & marchingDirection,
		  realArray & normal, 
		  realArray & xr,
		  const int sideBlend, const int axisBlend, // side axis for numberOfLinesForNormalBlend
		  bool initialStep = false,
		  int option  =0   );
  
int projectNormalsToMatchCurve(Mapping & matchingMapping,
			       MappingProjectionParameters & mpParams,
			       Index & I1, Index & I2,
			       realArray & normal, realArray & nDot );
  
int blendNormals(realArray & normal, realArray & xr,
		 int numberOfLinesToBlend,
		 Index Iv[3],
		 int axis, int side);
  

int jacobiSmooth( realArray & ss, const int & numberOfSmooths );
int formCMatrix(realArray & xr, 
		realArray & xt,
		const int & i3Mod2,
		realArray & normal, 
		realArray & normXr,
		const int & direction );
  
int computeNonlinearDiffussionCoefficient(const realArray & normXr, 
					  const realArray & normXs, 
					  const realArray & normXt, 
					  const int & direction,
					  int stepNumber );
  
int formBlockTridiagonalSystem(const int & direction, realArray & xTri);

int implicitSolve(realArray & xTri, 
		  const int & i3Mod2,
		  realArray & xr,
		  realArray & xt,
		  realArray & normal,
		  realArray & normXr,
		  realArray & normXs,
		  realArray & normXt,
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

int computeCellVolumes(const realArray & xt, const int & i3Mod2,
		       real & minCellVolume, real & maxCellVolume, 
		       const real & dSign );

int equidistributeAndStretch( const int & i3, const realArray & x, const real & weight, 
			      const int marchingDirection=1, bool stretchGrid = true );
  
realArray normalize( const realArray & u );
  
int project( const realArray & x, 
	     const int & marchingDirection,
	     realArray & xr, 
	     const bool & setBoundaryConditions= TRUE,
	     bool initialStep = false,
	     int stepNumber = 0);

virtual int project( realArray & x, 
		     MappingProjectionParameters & mpParams ){ return Mapping::project(x,mpParams); }

int correctProjectionOfInitialCurve(realArray & x,  realArray & xr,
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

int findNormalsToStartCurve(realArray & r, realArray & normal, int directionToMarch);
  
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
realArray xSurface;        // holds the surface grid 

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
realArray gridDensityWeight;   // holds the inverse of the desired grid spacing (relative values only)
realArray xHyper;              // holds the hyperbolic grid.
realArray xtHyper;             
realArray normalCC;            // normal at cell centre.
realArray at,bt,ct,c,lambda;  
  

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

realArray trailingEdgeDirection;   // for airfoil like trailing edge singularities.

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
