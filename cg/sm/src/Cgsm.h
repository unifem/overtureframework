#ifndef SOLID_MECHANICS_H
#define SOLID_MECHANICS_H

// ***************************************************
// ************ Solver for Solid Mechanics ***********
// ***************************************************

#include "Overture.h"
#include "ArraySimple.h"
#include "UnstructuredMapping.h"
#include "InterfaceInfo.h"

#include "DomainSolver.h"

class GL_GraphicsInterface;
class GUIState;
class MappedGridOperators;
class OGFunction;
class Ogshow;
class DialogData;
class ShowFileReader;
class RadiationBoundaryCondition;
class Oges;


class Cgsm : public DomainSolver 
{
 public:

  enum ElementTypeEnum
  {
    structuredElements,
    triangles,
    quadrilaterals,
    hexahedra,
    defaultUnstructured
  };

  enum InitialConditionEnum
  {
    defaultInitialCondition,
    planeWaveInitialCondition,
    gaussianPlaneWave,
    gaussianPulseInitialCondition,
    squareEigenfunctionInitialCondition,  
    annulusEigenfunctionInitialCondition,
    zeroInitialCondition,
    planeWaveScatteredFieldInitialCondition,
    planeMaterialInterfaceInitialCondition,
    gaussianIntegralInitialCondition,   // from Tom Hagstrom
    twilightZoneInitialCondition,
    parabolicInitialCondition,
    specialInitialCondition,
    hempInitialCondition,
    knownSolutionInitialCondition,
    userDefinedInitialCondition
  };

  enum ForcingEnum
  {
    noForcing,
    gaussianSource,
    twilightZoneForcing,
    planeWaveBoundaryForcing,
    gaussianChargeSource,
    userDefinedForcingOption
  };

//   enum TwlightZoneForcingEnum
//   {
//     polynomialTwilightZone,
//     trigonometricTwilightZone,
//     pulseTwilightZone
//   } twilightZoneOption;

  // Here are known solutions 
  enum KnownSolutionEnum
  {
    noKnownSolution,
    twilightZoneKnownSolution,
    squareEigenfunctionKnownSolution,
    annulusEigenfunctionKnownSolution
  } knownSolutionOption;


  enum EvaluationOptionsEnum
  {
    computeInitialConditions,
    computeErrors
  };


  Cgsm(CompositeGrid & cg, 
       GenericGraphicsInterface *ps=NULL, 
       Ogshow *show=NULL, 
       const int & plotOption=1 );

  ~Cgsm();
  


int 
addDissipation( int current, real t, real dt, realMappedGridFunction *fields, const Range & C );

static int 
addPrefix(const aString label[], const aString & prefix, aString cmd[], const int maxCommands);

void 
advance( int current, real t, real dt );
  
// advance the solution as a first-order system
void 
advanceFOS( int current, real t, real dt,
            RealCompositeGridFunction *ut = NULL, 
	    real tForce= 0. );

// advance a time step using the method of lines
void
advanceMethodOfLines( int current, real t, real dt, int correction=0, AdvanceOptions *pAdvanceOptions=NULL );

// advance the solution as a second-order system
void 
advanceSOS( int current, real t, real dt,
            RealCompositeGridFunction *ut = NULL, 
	    real tForce= 0. );



void
applyBoundaryConditions( int option, real dt, int current, int prev );

// Here is apply BC function from the DomainSolver: 
virtual int
applyBoundaryConditions(GridFunction & cgf,
                        const int & option =-1,
                        int grid_= -1,
                        GridFunction *puOld=NULL, 
                        const real & dt =-1. );
int
assignAnnulusEigenfunction(const int gfIndex, const EvaluationOptionsEnum evalOption );

void
assignBoundaryConditions( int option, int grid, real t, real dt, realMappedGridFunction & u, 
			  realMappedGridFunction & uOld, int current );
  
void 
assignBoundaryConditionsFOS( int option, int grid, real t, real dt, realMappedGridFunction & u, 
			    realMappedGridFunction & uOld, int current );
  
void 
assignBoundaryConditionsSOS( int option, int grid, real t, real dt, realMappedGridFunction & u, 
			    realMappedGridFunction & uOld, int current );
  
int 
assignGaussianPulseInitialConditions(int gfIndex);

int 
assignHempInitialConditions(int gfIndex);

virtual int 
assignInitialConditions(int gfIndex);
  
void 
assignInterfaceBoundaryConditions( int current, real t, real dt );

int 
assignParabolicInitialConditions(int gfIndex);

int 
assignSpecialInitialConditions(int gfIndex, const EvaluationOptionsEnum evalOption);

int 
assignTwilightZoneInitialConditions(int gfIndex);

int 
buildRunTimeDialog();

int 
buildVariableDissipation();

void 
checkArrays(const aString & label);

void 
checkDisplacementAndStress( const int current, real t );

void 
computeDissipation( int current, real t, real dt );

void 
computeNumberOfStepsAndAdjustTheTimeStep(const real & t,
					 const real & tFinal,
					 const real & nextTimeToPlot, 
					 int & numberOfSubSteps, 
					 real & dtNew,
					 const bool & adjustTimeStep =true );

// base cale version: 
virtual realCompositeGridFunction & 
getAugmentedSolution( GridFunction & gf0, realCompositeGridFunction & v );

realCompositeGridFunction& 
getAugmentedSolution(int current, realCompositeGridFunction & v, const real t);

bool 
getBoundsForPML( MappedGrid & mg, Index Iv[3], int extra=0 );

// Return the list of interface data needed by a given interface:
virtual int
getInterfaceDataOptions( GridFaceDescriptor & info, int & interfaceDataOptions ) const;

void 
getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
                                     IntegerArray & gidLocal, 
                                     IntegerArray & dimensionLocal, 
                                     IntegerArray & bcLocal );

virtual void 
getUt( GridFunction & cgf, 
       const real & t, 
       RealCompositeGridFunction & ut, 
       real tForce );

void 
getUtFOS(GridFunction & cgf, 
	 const real & t, 
	 RealCompositeGridFunction & ut, 
	 real tForce );

void
getUtSOS(GridFunction & cgf, 
	 const real & t, 
	 RealCompositeGridFunction & ut, 
	 real tForce );

void
getVelocityAndStress( const int current, real t, 
		      realCompositeGridFunction *pv =NULL, int vComponent =0,
		      realCompositeGridFunction *ps =NULL, int sComponent =0,
		      bool computeMaxNorms = true );

int 
endTimeStep( real & t0, real & dt0, AdvanceOptions & advanceOptions );

void 
getEnergy( int current, real t, real dt );

void 
getErrors( int current, real t, real dt, const aString & label =nullString );
  
//  void getField( real x, real y, real t, real *eField );

int 
getForcing( int current, int grid, realArray & u , real t, real dt, int option=0 );
  
virtual int 
getInitialConditions(const aString & command = nullString,
		     DialogData *interface =NULL,
		     GUIState *guiState = NULL,
		     DialogState *dialogState = NULL );

void 
getMaxDivAndCurl( const int current, real t, 
		  realCompositeGridFunction *pu=NULL, int component=0, 
		  realCompositeGridFunction *pvor=NULL, int vorComponent=0,
		  realCompositeGridFunction *pDensity=NULL, int rhoComponent=0,
		  bool computeMaxNorms= true );

int
getMethodName( aString & methodName );

  //       ===Choose time step====
virtual real 
getTimeStep( GridFunction & gf);

void 
initializeInterfaces();

void 
initializeKnownSolution();

int 
initializeRadiationBoundaryConditions();

virtual int 
initializeTimeStepping( real & t0, real & dt0 );

virtual int
interfaceRightHandSide( InterfaceOptionsEnum option, 
                        int interfaceDataOptions,
                        GridFaceDescriptor & info, 
			GridFaceDescriptor & gfd, 
			int gfIndex, real t );

int 
outputResults( int current, real t, real dt );

int 
outputResultsAfterEachTimeStep( int current, real t, real dt, int stepNumber );

int 
plot(int current, real t, real dt );
  
int 
printMemoryUsage(FILE *file = stdout );

// int 
// printStatistics(FILE *file  = stdout);

virtual void 
printTimeStepInfo( const int & step, const real & t, const real & cpuTime );

int 
project( int numberOfStepsTaken, int current, real t, real dt );

//int 
// saveParametersToShowFile();

// void 
// saveShow( int current, real t, real dt );

virtual void
saveShow( GridFunction & gf0 );

virtual void
saveShowFileComments( Ogshow &show );

int 
setBoundaryCondition( aString & answer, IntegerArray & originalBoundaryCondition );

int 
setParametersInteractively();

virtual int 
setPlotTitle(const real &t, const real &dt);

int 
setupGrids();
int 
setupGridFunctions();

virtual int 
setupPde(aString & reactionName, bool restartChosen, IntegerArray & originalBoundaryCondition);

virtual int 
setupUserDefinedForcing();

virtual int 
setupUserDefinedInitialConditions();

void  
smoothDivergence(realCompositeGridFunction & u, const int numberOfSmooths );

int 
solve();
  
virtual int 
startTimeStep( real & t0, real & dt0, int & currentGF, int & nextGF, AdvanceOptions & advanceOptions );

// *old* this next function is used by the cgmp 
// virtual void 
// takeOneStep( real & t, real & dt, int stepNumber, int & numberOfSubSteps );

int
takeTimeStep( real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions );

virtual int
updateForAdaptiveGrids(CompositeGrid & cg);

virtual int 
updateGeometryArrays(GridFunction & cgf);

virtual int
userDefinedBoundaryValues(const real & t, 
			  GridFunction & gf0,
			  const int & grid,
			  int side0 = -1,
			  int axis0 = -1,
			  ForcingTypeEnum forcingType=computeForcing );

virtual int
userDefinedForcing( realArray & f, const realMappedGridFunction & u, int iparam[], real rparam[] );

virtual void 
userDefinedForcingCleanup();

virtual int
userDefinedInitialConditions(CompositeGrid & cg, realCompositeGridFunction & u );

virtual void 
userDefinedInitialConditionsCleanup();


int 
updateProjectionEquation();

//  int updateShowFile(const aString & command= nullString, DialogData *interface =NULL );

bool 
usingPMLBoundaryConditions() const;


protected:

int 
buildTimeSteppingDialog(DialogData & dialog );

int 
getTimeSteppingOption(const aString & answer, DialogData & dialog );

int 
buildForcingOptionsDialog(DialogData & dialog );
int 
getForcingOption(const aString & command, DialogData & dialog );

int
buildGeneralOptionsDialog(DialogData & dialog );
int 
getGeneralOption(const aString & answer, DialogData & dialog );

int 
buildInputOutputOptionsDialog(DialogData & dialog );
int 
getInputOutputOption(const aString & command, DialogData & dialog );

int 
buildPlotOptionsDialog(DialogData & dialog );
int 
getPlotOption(const aString & command, DialogData & dialog );

int 
buildParametersDialog(DialogData & dialog ); // parameters that can be changed at run time.

int 
saveSequenceInfo( real t0, RealArray & sequenceData );
int 
saveSequencesToShowFile();

void 
setup(const real & time = 0. );

int  
updateForNewTimeStep(GridFunction & cgf, real & dt );

void 
writeParameterSummary( FILE *file );

public: //  should be protected:

  ForcingEnum forcingOption;
  InitialConditionEnum initialConditionOption;

  real c1,c2;
  real omegaForInterfaceIteration;
  int materialInterfaceOption;
  
  std::vector<InterfaceInfo> interfaceInfo;  // holds info and work-space for interfaces

  int kx,ky,kz;

  real deltaT;

  real betaGaussianPlaneWave,x0GaussianPlaneWave,y0GaussianPlaneWave,z0GaussianPlaneWave;
  real gaussianSourceParameters[5];  // gamma,omega,x0,y0,z0

  enum { maxNumberOfGaussianPulses=20};
  int numberOfGaussianPulses;
  real gaussianPulseParameters[maxNumberOfGaussianPulses][6];   // beta scale, exponent, x0,y0,z0

  enum { maxNumberOfGaussianChargeSources=10};
  int numberOfGaussianChargeSources;
  real gaussianChargeSourceParameters[maxNumberOfGaussianChargeSources][9];   // a,beta,p, x0,y0,z0, v0,v1,v2

  int numberOfStepsTaken;
  RealArray dxMinMax; // holds min and max grid spacings
  real divUMax,gradUMax,vorUMax;
  int numberOfIterationsForInterfaceBC;

  int orderOfArtificialDissipation;
  real artificialDissipation;
  int artificialDissipationInterval;
  real divergenceDamping;
  
  bool checkErrors;
  bool computeEnergy;
  bool plotDivergence, plotVorticity, plotErrors, plotDissipation, plotScatteredField, plotTotalField, plotRho;
  bool plotVelocity, plotStress;

  bool compareToReferenceShowFile;
  aString nameOfReferenceShowFile;
  RealArray maximumError, solutionNorm;
  real totalEnergy,initialTotalEnergy;
  real dScale; // displacement scale factor (for plotting the displacement)

  real radiusForCheckingErrors;  // if >0 only check errors in a disk (sphere) of this radius

  ElementTypeEnum elementType;

  real cylinderRadius, cylinderAxisStart,cylinderAxisEnd; // for eigenfunctions of the cylinder
  
  int numberLinesForPML; // width of the PML in grid lines
  int pmlPower;
  real pmlLayerStrength;
  int pmlErrorOffset;  // only check errors within this many lines of the pml 

  realArray *vpml;  // for PML

// int orderOfAccuracyInSpace;
//  int orderOfAccuracyInTime;
  bool useConservative;
  bool useVariableDissipation;

  real initialConditionParameters[10]; // holds parameters for various initial conditions
  real slowStartInterval;

  aString probeFileName;     // save the probes in this file
  ArraySimple<real> probes;  // output solution at these points
  ArraySimple<int> probeGridLocation;  // holds closest grid and grid point
  int frequencyToSaveProbes;   // save probe data every this many steps

  FILE *probeFile;           // save probe data to this file


  // for plane material interface
  real normalPlaneMaterialInterface[3], x0PlaneMaterialInterface[3];

  // For radiation boundaryConditions:
  int radbcGrid[2], radbcSide[2], radbcAxis[2];
  // RadiationBoundaryCondition *radiationBoundaryCondition;


  CompositeGridOperators *cgop;
  realCompositeGridFunction *cgdissipation;
  realCompositeGridFunction *cgerrp;
  realCompositeGridFunction *variableDissipation;
  realCompositeGridFunction *knownSolution; // for holding a known solution that may be expensive to recompute

  // These next variables are for higher order time stepping
  int numberOfTimeLevels;  // keep this many time levels of u

  int myid;                // processor number
  int numberOfFunctions;   // number of f=du/dt 
  int currentFn;
  realCompositeGridFunction *cgfn;

  GUIState *runTimeDialog;
  DialogData *pPlotOptionsDialog, *pParametersDialog;

  aString movieFileName;
  int movieFrame;
  int plotChoices, plotOptions;
  // int numberOfGridPoints;
  int totalNumberOfArrays;  // for counting A++ array

  aString nameOfGridFile;
  ShowFileReader *referenceShowFileReader;     // for comparing to a reference solution

  // for sequences saved in the show file
  int sequenceCount,numberOfSequences;
  RealArray timeSequence,sequence;

  bool useStreamMode;
  int frequencyToSaveInShowFile, showFileFrameForGrid;
  bool saveDivergenceInShowFile, saveErrorsInShowFile; 
  bool saveVelocityInShowFile, saveStressInShowFile;  

//   enum TimingEnum
//   { 
//     totalTime=0,
//     timeForInitialize,
//     timeForInitialConditions,
//     timeForAdvance,
//     timeForAdvanceRectangularGrids,
//     timeForAdvanceCurvilinearGrids,
//     timeForAdvanceUnstructuredGrids,
//     timeForAdvOpt,
//     timeForDissipation,
//     timeForBoundaryConditions,
//     timeForInterfaceBC,
//     timeForRadiationBC,
//     timeForRaditionKernel,
//     timeForInterpolate,
//     timeForUpdateGhostBoundaries,
//     timeForGetError,
//     timeForForcing,
//     timeForProject,
//     timeForComputingDeltaT,
//     timeForPlotting,
//     timeForShowFile,
//     timeForWaiting,
//     maximumNumberOfTimings      // number of entries in this list
//   };
//   RealArray timing;                                     // for timings, cpu time for some function
//   aString timingName[maximumNumberOfTimings];    // name of the function being timed

  real sizeOfLocalArraysForAdvance;
};

#endif
