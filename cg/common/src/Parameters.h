#ifndef PARAMETERS
#define PARAMETERS

#include <string>

#include "Overture.h"
#include "OGFunction.h"
#include "display.h"
#include "PlotStuffParameters.h"
#include "MovingGrids.h"
#include "ShowFileParameter.h"
#include "BoundaryData.h"

#include OV_STD_INCLUDE(map)
#include "ParallelOverlappingGridInterpolator.h"
#include "ArraySimple.h"

#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;

// --- forward declarations of classes --
class GenericGraphicsInterface;
class Ogshow;
class Reactions;
class FileOutput;
class Regrid;
class InterpolateRefinements;
class ErrorEstimator;
class Ogen;
class DialogData;
class GUIState;
class Insbc4WorkSpace;
class GridFunction;
class ListOfEquationDomains;
class SurfaceEquation;
class ShowFileReader;
class BodyForceRegionParameters;

int
addPrefix(const aString label[], const aString & prefix, aString cmd[], const int maxCommands);

// Here are the run time and PDE parameters
class Parameters
{
public:

/// The database holds almost all parameters, see \ref dbaseDescription "dbase entries".
mutable DataBase dbase; 


enum ForcingType 
{
  noForcing = 0,
  showfileForcing,
  numberOfForcingTypes
};

enum Stuff
{
  defaultValue=-12345678,
  maximumNumberOfOutputFiles=10
};

aString pdeName;
int numberOfBCNames;

enum TurbulenceModel
{
  noTurbulenceModel=0,
  BaldwinLomax=1,
  kEpsilon=2,
  kOmega=3,
  SpalartAllmaras=4,
  LargeEddySimulation=5,
  numberOfTurbulenceModels
};

static aString turbulenceModelName[numberOfTurbulenceModels+1];

/// A list of all possible time stepping methods.
enum TimeSteppingMethod
{
  forwardEuler,                              ///< forward euler 
  adamsBashforth2,                           ///< second-order Adams-Bashforth 
  adamsPredictorCorrector2,                  ///< second-order Adams predcitor-corrector
  adamsPredictorCorrector4,
  variableTimeStepAdamsPredictorCorrector,
  laxFriedrichs,                          
  implicit,                                  ///< implicit predictor-corrector   
  rKutta,
  midPoint,
  implicitAllSpeed,
  implicitAllSpeed2,
  nonMethodOfLines,
  steadyStateRungeKutta,
  steadyStateNewton,
  secondOrderSystemTimeStepping,
  adi,
  numberOfTimeSteppingMethods
};


static aString timeSteppingName[numberOfTimeSteppingMethods+1];

// These must match those in inspf.bf
/// A list of boundary conditions that are common to many solvers.
enum BoundaryCondition
{
  interpolation=0,
  symmetry=11,
  dirichletBoundaryCondition=12,
  axisymmetric=13,
  interfaceBoundaryCondition=17, 
  neumannBoundaryCondition=18,
  dirichletInterface=21,
  neumannInterface=22,

  // these appear to be needed by the turbulence models
  //       and are so common we might as well have them here
  noSlipWall=1,
  slipWall=4,
  noSlipWallInterface=19,
  slipWallInterface=20,

  // free surface with surface tension:
  freeSurfaceBoundaryCondition=31,

  // here is one for penalty boundary conditions
  penaltyBoundaryCondition=100,
};

// Here are the different types of interfaces that we know about
enum InterfaceTypeEnum
{
  noInterface,                    // no interface conditions are imposed
  heatFluxInterface,              // [ T.n ] = g
  tractionInterface,              // [ n.tau ] = g 
  tractionAndHeatFluxInterface
};  

// Here are the different data that can be requested at an interface. Multiple items are
// chosen by bit-wise or of the different options
enum InterfaceDataEnum
{
  heatFluxInterfaceData         = 1                             , // bit  0
  positionInterfaceData         = heatFluxInterfaceData     << 1, // bit  1
  velocityInterfaceData         = positionInterfaceData     << 1, // bit  2
  accelerationInterfaceData     = velocityInterfaceData     << 1, // bit  3
  tractionInterfaceData         = accelerationInterfaceData << 1, // bit  4
  tractionRateInterfaceData     = tractionInterfaceData     << 1  // bit  5
};


/// A list of different forms that a boundary condition can take.
enum BoundaryConditionType
{
  uniformInflow,                               ///< inflow profile is constant in space.
  uniformInflowRamped,                         ///< inflow profile is constant in space, ramped in time.
  uniformInflowOscillating,                    ///< inflow profile is constant in space, oscillating in time.
  uniformInflowWithTimeDependence,  // use this instead ?
  parabolicInflow,                             ///< inflow profile is parabolic in space.
  parabolicInflowRamped,                       ///< inflow profile is parabolic in space, ramped in time.
  parabolicInflowOscillating,                  ///< inflow profile is parabolic in space, oscillating in time.
  parabolicInflowUserDefinedTimeDependence,    ///< inflow profile is parabolic in space, user defined time dependence.
  parabolicInflowWithTimeDependence,
  blasiusProfile,                              ///< kkc added for bl and turbulence model testing
  rampInflow,
  jetInflow,
  jetInflowRamped,
  jetInflowOscillating,
  jetInflowUserDefinedTimeDependence,
  jetInflowWithTimeDependence,
  ramped,                  // here are new more generic types -- also look at bc to get real type
  parabolicRamped,
  parabolicOscillating,
  parabolicUserDefinedTimeDependence,
  numberOfPredefinedBoundaryConditionTypes  // userDefinedBoundaryData,
};


enum ImplicitMethod
{
  notImplicit=0,
  backwardEuler,
  secondOrderBDF,
  crankNicolson,
  lineImplicit,
  trapezoidal,
  approximateFactorization
};

enum ImplicitOption
{
  computeAllTerms,
  doNotComputeImplicitTerms,
  computeImplicitTermsSeparately,
  computeAllWithWeightedImplicitTerms
};

 std::map<int,aString> bcNames, icNames, bcmNames;
typedef std::map<int,aString>::iterator BCIterator;

bool registerBC(int id, const aString & name, bool replace= false );

bool registerInterfaceType(int id, const aString & name, bool replace= false );


class BCModifier {
 public:
  BCModifier(const aString &nm) : name(nm) {}
  virtual ~BCModifier() {}

  aString name;
  virtual const bool isPenaltyBC() const;

  virtual bool inputFromGI(GenericGraphicsInterface &gi)=0;
  virtual bool applyBC(Parameters &parameters, 
		     const real & t, const real &dt,
		     realMappedGridFunction &u,
		     const int & grid,
		     int side0 /* = -1 */,
		     int axis0 /* = -1 */,
		     realMappedGridFunction *gridVelocity = 0)=0;

  virtual bool setBCCoefficients(Parameters &parameters, 
				 const real & t, const real &dt,
				 realMappedGridFunction &u,
				 realMappedGridFunction &coeff,
				 const int & grid,
				 int side0 /* = -1 */,
				 int axis0 /* = -1 */,
				 realMappedGridFunction *gridVelocity = 0)=0;
  

  virtual bool addPenaltyForcing(Parameters &parameters, 
				 const real & t, const real &dt,
				 const realMappedGridFunction &u,
				 realMappedGridFunction &dudt,
				 const int & grid,
				 int side0 /* = -1 */,
				 int axis0 /* = -1 */,
				 const realMappedGridFunction *gridVelocity = 0)=0;


};



typedef BCModifier*(*CreateBCModifierFromName)(const aString &name);
typedef std::map<size_t,BCModifier*>::iterator BCModifierIterator;
typedef std::map<std::string,CreateBCModifierFromName>::iterator BCModCreatorIterator;

std::map<size_t,BCModifier*> bcModifiers;
std::map<std::string, CreateBCModifierFromName> bcModCreators;

bool registerBCModifier(const aString &name, Parameters::CreateBCModifierFromName createBCMod, bool replace=false);


enum InitialConditionOption
{
  noInitialConditionChosen,
  uniformInitialCondition,
  readInitialConditionFromShowFile,
  readInitialConditionFromRestartFile,
  userDefinedInitialCondition,
  stepFunctionInitialCondition,
  twilightZoneFunctionInitialCondition,
  spinDownInitialCondition,
  knownSolutionInitialCondition
};


enum TwilightZoneChoice
{
  polynomial,
  trigonometric,
  pulse
};

enum ReactionTypeEnum
{
  noReactions=0,
  oneStep,
  branching,
  ignitionAndGrowth,
  oneEquationMixtureFraction,
  twoEquationMixtureFractionAndExtentOfReaction,
  chemkinReaction,
  oneStepPress,
  igDesensitization,
  ignitionPressureReactionRate // 9 
};

enum InterpolationTypeEnum
{
  defaultInterpolationType,     // interpolate conservative/primitive variables based on computational variables
  interpolateConservativeVariables, // interpolate conservative variables 
  interpolatePrimitiveVariables,    // interpolate primitive variables (rho,u,T)
  interpolatePrimitiveAndPressure   // interpolate primitive and pressure, (rho,u,p) 
};

// This enum defines the frame of reference for the PDEs in a domain.
// The frame of reference is needed so that we know how to transform the PDE when the grids move. 
// Often the PDEs are defined in a fixed reference frame (even if some boundaries are moving). 
// If we are solving for a PDE inside a moving rigid body, then the PDE (e.g. the heat equation) 
// may be defined in the frame of reference of the rigid body. 
enum ReferenceFrameEnum
{
  fixedReferenceFrame=0,             // PDEs are defined in a fixed reference frame
  rigidBodyReferenceFrame,           // PDEs are defined w.r.t. the motion of a rigid body.
  specifiedReferenceFrame            // PDEs are defined w.r.t. some other specified reference frame.
};


// Here are some known solutions that we can compare to 
enum KnownSolutionsEnum
{
  noKnownSolution=0,
  userDefinedKnownSolution, // put new known solutions here
  knownSolutionFromAShowFile
//  supersonicFlowInAnExpandingChannel  // put this here for now -- add this to userDefined ---
};


// public functions
Parameters(const int & numberOfDimensions0=3);
virtual ~Parameters();
  
// ** note: functions are in alphabetical order ***

virtual
int addShowVariable( const aString & name, int component, bool variableIsOn = TRUE );

int addTiming( const aString & timeVariableName, const aString & timeLabel );

virtual
int assignParameterValues(const aString & label, RealArray & values,
			  const int & numRead, aString *c, real val[],
			  char *extraName1 = 0, const int & extraValue1Location = 0, 
			  char *extraName2 = 0, const int & extraValue2Location = 0, 
			  char *extraName3 = 0, const int & extraValue3Location = 0 );
virtual
int assignParameterValues(const aString & label, RealArray & values,
			  const int & numRead, char c[][10], real val[],
			  char *extraName1 = 0, const int & extraValue1Location = 0, 
			  char *extraName2 = 0, const int & extraValue2Location = 0, 
			  char *extraName3 = 0, const int & extraValue3Location = 0 );

virtual
int bcIsAnInterface(int bc) const; 

virtual
int bcIsTimeDependent(int side, int axis, int grid) const; 

virtual
int bcType(int side, int axis,int grid) const;

virtual
int bcVariesInSpace(int side, int axis, int grid) const; 

virtual
int bcVariesInSpace(const Index & side = nullIndex, 
		    const Index & axis = nullIndex, 
		    const Index & grid = nullIndex ) const;

virtual int 
buildBodyForceRegionsDialog(DialogData & dialog, BodyForceRegionParameters & regionPar );

virtual
int buildErrorEstimator();
  
virtual int
buildForcingProfilesDialog(DialogData & dialog, BodyForceRegionParameters & regionPar );

virtual int 
buildMaterialParametersDialog(DialogData & dialog, BodyForceRegionParameters & regionPar );

virtual
int buildReactions();
  
virtual int
buildTemperatureBoundaryConditionsDialog(DialogData & dialog, BodyForceRegionParameters & regionPar );

virtual 
int checkForValidBoundaryCondition( const int & bc, bool reportErrors=true );

virtual
int chooseUserDefinedBoundaryValues(int side, int axis, int grid, CompositeGrid & cg);

virtual int 
conservativeToPrimitive(GridFunction & gf, int gridToConvert=-1, int fixupUnsedPoints=false);

virtual
int defineBoundaryConditions(CompositeGrid & cg, 
			     const IntegerArray & originalBoundaryCondition,
			     const aString & command =nullString,
			     DialogData *interface=NULL);

virtual
int defineVariableBoundaryValues(int side, int axis, int grid, CompositeGrid & cg);

virtual
int displayPdeParameters(FILE *file = stdout );

virtual
void displayPolynomialCoefficients(RealArray & cx, RealArray & ct, aString * componentName, 
				   int numberOfComponents, FILE *file);
virtual
int get( const GenericDataBase & dir, const aString & name);

virtual int 
getBodyForceRegionsOption(const aString & answer,
			  BodyForceRegionParameters & regionPar,
			  DialogData & dialog, 
                          CompositeGrid & cg );

// allocate the boundary data for a given side of a grid and return the array
RealArray &
getBoundaryData(int side, int axis, int grid, MappedGrid & mg );

// return the boundary data for a grid
BoundaryData::BoundaryDataArray&
getBoundaryData(int grid);

virtual int
getForcingProfilesOption(const aString & answer,
			 BodyForceRegionParameters & regionPar,
			 DialogData & dialog );
virtual
int getComponents( IntegerArray &components );

virtual int 
getDerivedFunction( const aString & name, const realCompositeGridFunction & u,
                    realCompositeGridFunction & v, const int component, const real t,
                    Parameters & parameters);
virtual int 
getDerivedFunction( const aString & name, const realMappedGridFunction & u, 
                    realMappedGridFunction & v, 
                    const int component, const int grid, const real t, Parameters & parameters);

// virtual int 
// getDerivedFunction( const aString & name, const realCompositeGridFunction & u, realCompositeGridFunction & v, 
//                     const int component, Parameters & parameters);
// virtual int 
// getDerivedFunction( const aString & name, const realMappedGridFunction & u, realMappedGridFunction & vIn, 
//                     const int component, Parameters & parameters);

// Return 1 or 2 if grid was chosen to be implicit AND time stepping is implicit:
virtual
int getGridIsImplicit(int grid) const;

// evaluate the known solution: (if there is one defined)
virtual
realCompositeGridFunction& getKnownSolution( CompositeGrid & cg, real t );
virtual
realMappedGridFunction& getKnownSolution(real t, int grid,  
					 const Index & I1, const Index &I2, const Index &I3, 
					 bool initialCall=false );

virtual int 
getMaterialParametersOption(const aString & answer,
			    BodyForceRegionParameters & regionPar,
			    DialogData & dialog );

// compute the normal force on a boundary (for moving grid problems)
virtual 
int getNormalForce( realCompositeGridFunction & u, realSerialArray & normalForce, int *ipar, real *rpar );

const ReferenceFrameEnum
getReferenceFrame();

int 
getShowVariable( const aString & name, int & component, bool & variableIsOn ) const;

virtual int 
getTemperatureBoundaryConditionsOption(const aString & answer,
			  BodyForceRegionParameters & regionPar,
				       DialogData & dialog );
virtual
int getTimeDependenceBoundaryConditionParameters(int side, int axis, int grid, RealArray & values) const;

virtual
aString getTimeSteppingName() const;

virtual
int getUserBoundaryConditionParameters(int side, int axis, int grid, RealArray & values) const;

virtual
int getUserDefinedKnownSolution(real t, CompositeGrid & cg, int grid, realArray & ua, 
				const Index & I1, const Index &I2, const Index &I3 );
virtual
bool gridIsMoving(int grid) const;

virtual
int howManyBcTypes(const Index & side, 
		   const Index & axis, 
		   const Index & grid, 
		   BoundaryConditionType bc) const;

virtual 
int initializeTimings();

virtual
int inputParameterValues(const aString & answer, const aString & label, RealArray & values );

virtual
bool isAdaptiveGridProblem() const{ return dbase.get<bool>("adaptiveGridProblem");} //
virtual
bool isAxisymmetric() const;

virtual 
bool isMixedBC( int bc );

virtual
bool isMovingGridProblem() const;

virtual
bool isSteadyStateSolver() const;


virtual
int numberOfGhostPointsNeeded() const;  // number of ghost points needed by this method.

virtual
int numberOfGhostPointsNeededForImplicitMatrix() const;  // number of ghost points needed for implicit matrix

virtual
int openLogFiles(const aString & name);

virtual
int parseValues( const aString & answer, aString *name, real *value, int maxNumber );

virtual int 
primitiveToConservative(GridFunction & gf, int gridToConvert=-1, int fixupUnsedPoints=false);

virtual
int put( GenericDataBase & dir, const aString & name) const;

virtual
int 
readFromAShowFile(ShowFileReader & showFileReader,
                  CompositeGrid & cgRef,
                  CompositeGrid & cg,
                  realCompositeGridFunction & u,
                  int & solutionNumber );

// return true if we save the linearized solution for implicit methods.
virtual 
bool saveLinearizedSolution();  

virtual
int saveParametersToShowFile();

virtual
int setBcIsTimeDependent(int side, int axis, int grid, bool trueOrFalse=true); 

virtual
int setBcType(int side, int axis,int grid, BoundaryConditionType bc);

virtual 
int setBcModifier(int side, int axis, int grid, int bcm);

virtual
int setBcVariesInSpace(int side, int axis, int grid, bool trueOrFalse=true); 

virtual 
int 
setBoundaryConditionValues(const aString & answer,
			   const IntegerArray & originalBoundaryCondition,
			   CompositeGrid & cg);
virtual 
int setDefaultDataForABoundaryCondition(const int & side,
					const int & axis,
					const int & grid,
					CompositeGrid & cg);
virtual
int setGridIsImplicit(int grid=-1, int value=1 );

virtual int
setInfoFile(FILE *file);

virtual
int setParameters(const int & numberOfDimensions0=2, 
		  const aString & reactionName =nullString);

virtual
int setPdeParameters(CompositeGrid & cg,
                     const aString & command =nullString,
		     DialogData *interface=NULL);

int 
setShowVariable( const aString & name, const bool variableIsOn );

virtual
int setUserDefinedParameters();  // allow user defined pdeParameters to be passed to C or Fortran routines.

virtual
int setTimeDependenceBoundaryConditionParameters(int side, int axis, int grid, RealArray & values);

virtual
int setTwilightZoneFunction(const TwilightZoneChoice & choice,
			    const int & degreeSpace=2, 
			    const int & degreeTime=2 );

virtual
int setTwilightZoneParameters(CompositeGrid & cg,
                              const aString & command =nullString,
			      DialogData *interface=NULL );
  
virtual
int setUserBcType(int side, int axis,int grid, int bc); // for user defined BC's

virtual
int thereAreTimeDependentUserBoundaryConditions(const Index & side, 
						const Index & axis, 
						const Index & grid) const;

virtual int 
setupBodyForcing(CompositeGrid & cg);


virtual
int setUserBoundaryConditionParameters(int side, int axis, int grid, RealArray & values);

virtual
int updateKnownSolutionToMatchGrid( CompositeGrid & cg );

virtual
int updatePDEparameters();

virtual
int updateShowFile(const aString & command = nullString,
		   DialogData *interface=NULL);
  
virtual
int updateToMatchGrid( CompositeGrid & cg, 
		       IntegerArray & sharedBoundaryCondition = Overture::nullIntArray() );
  
virtual
int updateTurbulenceModels(CompositeGrid & cg);

virtual int 
updateUserDefinedEOS(GenericGraphicsInterface & gi);

virtual
int updateUserDefinedKnownSolution(GenericGraphicsInterface & gi);

virtual
bool useConservativeVariables(int grid=-1) const;  // if true we are using a solver that uses conservative variables

virtual
int userBcType(int side, int axis,int grid) const;

virtual
int userDefinedDeformingSurface( DeformingBodyMotion & deformingBody,
				 real t1, real t2, real t3, 
				 GridFunction & cgf1,
				 GridFunction & cgf2,
				 GridFunction & cgf3,
				 int option );

virtual
void userDefinedDeformingSurfaceCleanup( DeformingBodyMotion & deformingBody );

virtual
int userDefinedDeformingSurfaceSetup( DeformingBodyMotion & deformingBody );

// ** note: functions are in alphabetical order ***



static real spalartAllmarasScaleFactor;  // factor to artificially increase the SA production term
static real spalartAllmarasDistanceScale;  // small parameter to add to the distance to the wall
static int checkForFloatingPointErrors; // check for nan's and inf's

};


#endif
