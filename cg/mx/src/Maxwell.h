#ifndef MAXWELL_H
#define MAXWELL_H

#include "Overture.h"
#include "ArraySimple.h"
#include "UnstructuredMapping.h"
#include "InterfaceInfo.h"
#include "MxParameters.h"
#include "GridFunction.h"

// #include OV_STD_INCLUDE(map)
#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;


class GL_GraphicsInterface;
class GUIState;
class MappedGridOperators;
class OGFunction;
class Ogshow;
class DialogData;
class ShowFileReader;
class RadiationBoundaryCondition;
class Oges;
class DispersiveMaterialParameters;

// Define the type of array we stored the chirped parameters in
typedef ArraySimpleFixed<real,10,1,1,1> ChirpedArrayType;


class Maxwell
{
 public:

  enum GridTypeEnum
  {
    square,
    rotatedSquare,
    sineSquare,
    skewedSquare,
    chevron,
    squareByTriangles,
    squareByQuads,
    sineByTriangles,
    annulus,
    box,
    chevbox,
    perturbedSquare,     //  square with random perturbations
    perturbedBox,        //  box with random perturbations
    compositeGrid,
    unknown
  };

  enum ElementTypeEnum
  {
    structuredElements,
    triangles,
    quadrilaterals,
    hexahedra,
    defaultUnstructured
  };

  // This next enum should match bcDefineFortranInclude.h
  enum BoundaryConditionEnum
  {
    periodic=0,
    dirichlet=1,
    perfectElectricalConductor,
    perfectMagneticConductor,
    planeWaveBoundaryCondition,
    symmetry,
    interfaceBoundaryCondition,   // for the interface between two regions with different properties
    abcEM2,         // absorbing BC, Engquist-Majda order 2 
    abcPML,         // perfectly matched layer
    abc3,           // future absorbing BC
    abc4,           // future absorbing BC
    abc5,           // future absorbing BC
    rbcNonLocal,    // radiation BC, non-local
    rbcLocal,       // radiation BC, local
    numberOfBCNames
  };
  static aString bcName[numberOfBCNames];

  enum BoundaryConditionOptionEnum
  {
    useAllPeriodicBoundaryConditions=0,
    useAllDirichletBoundaryConditions,
    useAllPerfectElectricalConductorBoundaryConditions,
    useGeneralBoundaryConditions
  };

  // This next enum should match icDefineFortranInclude.h
  enum InitialConditionEnum
  {
    defaultInitialCondition=0,
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
    userDefinedInitialConditionsOption,
    userDefinedKnownSolutionInitialCondition, 
    numberOfInitialConditionNames
  };

  // This next enum should match forcingDefineFortranInclude.h
  enum ForcingEnum
  {
    noForcing,
    magneticSinusoidalPointSource,
    gaussianSource,
    twilightZoneForcing,
    // planeWaveBoundaryForcing, // moved below, 2016/08/07 
    gaussianChargeSource,
    userDefinedForcingOption,
    numberOfForcingNames
  };

  enum BoundaryForcingEnum
  {
    noBoundaryForcing=0,
    planeWaveBoundaryForcing,
    chirpedPlaneWaveBoundaryForcing
  };

  enum TwlightZoneForcingEnum
  {
    polynomialTwilightZone,
    trigonometricTwilightZone,
    pulseTwilightZone
  } twilightZoneOption;


  // Here are known solutions 
  enum KnownSolutionEnum
  {
    noKnownSolution,
    twilightZoneKnownSolution,
    planeWaveKnownSolution,
    gaussianPlaneWaveKnownSolution,
    gaussianIntegralKnownSolution,
    planeMaterialInterfaceKnownSolution,
    scatteringFromADiskKnownSolution,
    scatteringFromADielectricDiskKnownSolution,
    scatteringFromASphereKnownSolution,
    scatteringFromADielectricSphereKnownSolution,
    squareEigenfunctionKnownSolution,
    annulusEigenfunctionKnownSolution,
    eigenfunctionsOfASphereKnownSolution,    // not implemented yet 
    userDefinedKnownSolution
  } knownSolutionOption;



  // kkc enum used in getCGField, {EH}Field is mainly used by the unstructured algorithms
  enum FieldEnum
  {
    EField,
    E100,
    E010,
    E001,
    HField,
    H100,
    H010,
    H001
  };

  enum MethodEnum
  {
    defaultMethod=0,
    yee,
    dsi,
    dsiNew,
    dsiMatVec, // Matrix-Vector DSI implementation
    nfdtd,     // non-orthogonal FDTD
    sosup      // second-order system upwind scheme
  };


  enum TimeSteppingMethodEnum
  {
    defaultTimeStepping=0,
    adamsBashforthSymmetricThirdOrder,
    rungeKuttaFourthOrder,
    stoermerTimeStepping, 
    modifiedEquationTimeStepping
  } timeSteppingMethod;


  enum DispersionModelEnum
  {
    noDispersion=0,
    drude,
  } dispersionModel;

  

  enum StageOptionEnum
  {
    nullStageOperation=0,
    computeUtInStage=1,
    updateInteriorInStage=2,
    addDissipationInStage=4,
    applyBCInStage=8
  };

    

  Maxwell();
  ~Maxwell();
  

  int addDissipation( int current, real t, real dt, realMappedGridFunction *fields, const Range & C );

  void addFilter( int current, real t, real dt );

  static int addPrefix(const aString label[], const aString & prefix, aString cmd[], const int maxCommands);

  bool adjustBoundsForPML( MappedGrid & mg, Index Iv[3], int extra=0 );

  void advanceC( int current, real t, real dt, realMappedGridFunction *fields );

  void advanceDSI( int current, real t, real dt); // kkc method that uses cg and check grid types (for hybrid grids)

  void advanceFDTD(  int numberOfStepsTaken, int current, real t, real dt );

  // new version
  void advanceNew( int current, real t, real dt, realMappedGridFunction *fields );

  void advanceNFDTD( int numberOfStepsTaken, int current, real t, real dt );
  
  void advanceSOSUP( int numberOfStepsTaken, int current, real t, real dt );

  void advanceUnstructuredDSI( int current, real t, real dt, realMappedGridFunction *fields );

  void advanceUnstructuredDSIMV( int current, real t, real dt, realMappedGridFunction *fields );

  void assignBoundaryConditions( int option, int grid, real t, real dt, realMappedGridFunction & u, 
                                 realMappedGridFunction & uOld, int current );
  
  void assignInitialConditions(int current, real t, real dt);
  
  void assignInterfaceBoundaryConditions( int current, real t, real dt, 
                                          bool assignInterfaceValues,
                                          bool assignInterfaceGhostValues );

  int assignUserDefinedKnownSolutionInitialConditions(int current, real t, real dt );

  int buildRunTimeDialog();

  int buildVariableDissipation();


  void checkArrays(const aString & label);

  void computeDissipation( int current, real t, real dt );

  int computeIntensity(int current, real t, real dt, int stepNumber, real nextTimeToPlot);

  void computeNumberOfStepsAndAdjustTheTimeStep(const real & t,
						const real & tFinal,
						const real & nextTimeToPlot, 
						int & numberOfSubSteps, 
						real & dtNew,
						const bool & adjustTimeStep =true );

  int computeTimeStep();

  // Define material regions and bodies that are defined by a mask 
  int defineRegionsAndBodies();

  void displayBoundaryConditions(FILE *file = stdout);

  // return true if the equations are forced (external forcing)
  bool forcingIsOn() const;

  realCompositeGridFunction& getAugmentedSolution(int current, realCompositeGridFunction & v, const real t);

  bool getBoundsForPML( MappedGrid & mg, Index Iv[3], int extra=0 );

  void getChargeDensity( int current, real t, realCompositeGridFunction &u, int component=0 );

  void getChargeDensity( real t, realMappedGridFunction & u, int component =0 );

  DispersiveMaterialParameters & getDomainDispersiveMaterialParameters( const int domain );

  DispersiveMaterialParameters & getDispersiveMaterialParameters( const int grid );

  void getEnergy( int current, real t, real dt );

  void getErrors( int current, real t, real dt );
  
  void getField( real x, real y, real t, real *eField );

  int getForcing( int current, int grid, realArray & u , real t, real dt, int option=0 );
  
  void getMaxDivergence( const int current, real t, 
                         realCompositeGridFunction *pu=NULL, int component=0, 
                         realCompositeGridFunction *pDensity=NULL, int rhoComponent=0,
                         bool computeMaxNorms= true );

  static real getMaxValue(real value, int processor=-1);  // get max over all processors
  static int getMaxValue(int value, int processor=-1);

  static real getMinValue(real value, int processor=-1);  // get min over all processors
  static int getMinValue(int value, int processor=-1);

  void getTimeSteppingLabel( real dt, aString & label ) const;

  int getUserDefinedKnownSolution(real t, CompositeGrid & cg, int grid, realArray & ua, 
				  const Index & I1, const Index &I2, const Index &I3, int numberOfTimeDerivatives = 0 );

  int getValuesFDTD(int option, int *iparam, int current, real t, real dt, realCompositeGridFunction *v= NULL );

  void initializeKnownSolution();

  void initializeInterfaces();

  int initializePlaneMaterialInterface();

  int initializeRadiationBoundaryConditions();

  int interactiveUpdate(GL_GraphicsInterface &gi );

  void outputHeader();

  int outputResults( int current, real t, real dt );

  int outputResultsAfterEachTimeStep( int current, real t, real dt, int stepNumber, real nextTimeToPlot );

  int plot(int current, real t, real dt );
  
  int printMemoryUsage(FILE *file = stdout );

  int printStatistics(FILE *file  = stdout);

  void printTimeStepInfo( const int current, const int & step, const real & t, const real & dt, const real & cpuTime );

  // project the fields to satisfy div( eps E ) = rho
  int project( int numberOfStepsTaken, int current, real t, real dt );

  // project the interpolation points to satisfy div( eps E ) = rho
  int projectInterpolationPoints( int numberOfStepsTaken, int current, real t, real dt );

  void saveShow( int current, real t, real dt );

  int saveParametersToShowFile();

  int setBoundaryCondition( aString & answer, GL_GraphicsInterface & gi, IntegerArray & originalBoundaryCondition );

  int setDispersionParameters( GL_GraphicsInterface &gi );

  int setupGrids();

  int setupGridFunctions();

  int setupUserDefinedForcing();

  int setupUserDefinedInitialConditions();

  void smoothDivergence(realCompositeGridFunction & u, const int numberOfSmooths );

  int solve(GL_GraphicsInterface &gi );
  
  int updateProjectionEquation();

  int updateUserDefinedKnownSolution(GenericGraphicsInterface & gi, CompositeGrid & cg);

  bool usingPMLBoundaryConditions() const;

  int updateShowFile(const aString & command= nullString, DialogData *interface =NULL );


  int userDefinedForcing( realArray & f, int iparam[], real rparam[] );

  void userDefinedForcingCleanup();

  int userDefinedInitialConditions(int current, real t, real dt );

  void userDefinedInitialConditionsCleanup();

  bool vertexArrayIsNeeded( int grid ) const;


protected:
  int buildTimeSteppingOptionsDialog(DialogData & dialog );
  int buildForcingOptionsDialog(DialogData & dialog );
  int buildInitialConditionsOptionsDialog(DialogData & dialog );
  int buildPlotOptionsDialog(DialogData & dialog );
  int buildInputOutputOptionsDialog(DialogData & dialog );
  int buildPdeParametersDialog(DialogData & dialog);

  int buildParametersDialog(DialogData & dialog ); // parameters that can be changed at run time.

  int saveSequenceInfo( real t0, RealArray & sequenceData );
  int saveSequencesToShowFile();

  int setupMultiStageAlgorithm( GL_GraphicsInterface &gi, DialogData & dialog );

public: //  should be protected:

  MxParameters parameters;  // parameters class

  // The database is the new place to store parameters
  mutable DataBase dbase; 

  MethodEnum method;
  aString methodName;
  BoundaryConditionOptionEnum bcOption;
  ForcingEnum forcingOption;
  InitialConditionEnum initialConditionOption;

  int ex,ey,ez,hx,hy,hz;  // component numbers for Ex, Ey, Ez, Hx, ...
  int ext,eyt,ezt,hxt,hyt,hzt;  // component numbers for Ext, Eyt, Ezt, Hxt, ...
  int pxc,pyc,pzc,qxc,qyc,qzc,rxc,ryc,rzc; // component numbers for Q, R, C (dispersive models)

  int rc;                 // component number for rho (in TZ functions)
  int epsc,muc,sigmaEc,sigmaHc;    // components for eps, mu, sigmaE and sigmaH in TZ functions
  int numberOfComponentsRectangularGrid;
  int ex10,ey10,ex01,ey01,hz11,numberOfComponentsCurvilinearGrid;

  real frequency;
  
  real eps,mu,c;
  RealArray epsGrid,muGrid,cGrid,sigmaEGrid,sigmaHGrid;  // holds variable coefficients that are constant on each grid
  bool gridHasMaterialInterfaces;
  real omegaForInterfaceIteration;
  int materialInterfaceOption, interfaceEquationsOption;
  bool useNewInterfaceRoutines;

  int numberOfMaterialRegions;           // for variable coefficients -- number of piecewise constant regions
  IntegerArray media;                    // for variable coefficients 
  RealArray epsv, muv, sigmaEv, sigmaHv; // for variable coefficients 
  int maskBodies;                        // mask bodies for the Yee scheme
  intSerialArray *pBodyMask;
  bool useTwilightZoneMaterials;          // if true define eps, mu, .. using the twilight-zone functions

  std::vector<InterfaceInfo> interfaceInfo;  // holds info and work-space for interfaces

  real kx,ky,kz;  // plane wave wave-numbers
  real pwc[6];    // plane wave coefficients (E and H)
  enum
  {
    numberOfPlaneMaterialInterfaceCoefficients=33
  };
  real pmc[numberOfPlaneMaterialInterfaceCoefficients];   // plane material interface coefficients

  int nx[3];
  real deltaT;
  real xab[2][3];

  IntegerArray adjustFarFieldBoundariesForIncidentField; // subtract out the incident field before apply NRBC's

  real betaGaussianPlaneWave,x0GaussianPlaneWave,y0GaussianPlaneWave,z0GaussianPlaneWave;
  real gaussianSourceParameters[5];  // gamma,omega,x0,y0,z0

  enum { maxNumberOfGaussianPulses=20};
  int numberOfGaussianPulses;
  real gaussianPulseParameters[maxNumberOfGaussianPulses][6];   // beta scale, exponent, x0,y0,z0

  enum { maxNumberOfGaussianChargeSources=10};
  int numberOfGaussianChargeSources;
  real gaussianChargeSourceParameters[maxNumberOfGaussianChargeSources][9];   // a,beta,p, x0,y0,z0, v0,v1,v2

  real cfl;
  real tFinal,tPlot;
  int numberOfStepsTaken;
  RealArray dxMinMax; // holds min and max grid spacings
  real divEMax,gradEMax,divHMax,gradHMax;
  int maximumNumberOfIterationsForImplicitInterpolation;
  int numberOfIterationsForInterfaceBC;

  int orderOfArtificialDissipation;
  real artificialDissipation, artificialDissipationCurvilinear;
  int artificialDissipationInterval;
  real divergenceDamping;
  
  // parameters for the high order filter: 
  bool applyFilter;              // true : apply the high order filter  
  int orderOfFilter;		 // this means use default order	     
  int filterFrequency;		 // apply filter every this many steps  
  int numberOfFilterIterations;	 // number of iterations in the filter  
  real filterCoefficient;        // coefficient in the filter

  // parameters for new divergence cleaning method
  bool useDivergenceCleaning;
  real divergenceCleaningCoefficient;

  bool useChargeDensity;             // if true, this problem includes a charge density
  realCompositeGridFunction *pRho;    // pointer to the charge density
  realCompositeGridFunction *pPhi,*pF;   // pointer to phi and f for projection
  Oges *poisson;                     // Poisson solver for projection
  bool projectFields;                // if true, project the fields to have the correct divergence
  int frequencyToProjectFields;      // apply the project every this many steps
  int numberOfConsecutiveStepsToProject;
  int numberOfInitialProjectionSteps; // always project this first number of steps
  int numberOfDivergenceSmooths;

  int numberOfProjectionIterations;  // number of iterations in the projection solve
  bool initializeProjection;
  bool useConservativeDivergence;    // evaluate the conservative form of the divergence
  bool projectInitialConditions;     
  bool projectInterpolation;   // if true, project interp. pts so that div(E)=0

  bool solveForElectricField,solveForMagneticField; // in 3D we can solve for either or both of E and H

  bool checkErrors;
  bool computeEnergy;
  bool plotDivergence, plotErrors, plotDissipation, plotScatteredField, plotTotalField, plotRho, plotEnergyDensity;
  bool plotIntensity;  // plot time averaged intensity for time periodic problems
  int intensityOption;  // 0=compute from time average, 1=compute from just two solutions
  real omegaTimeHarmonic;  // the intensity computation needs to know the frequency in time, omegaTimeHarmonic = omega/(2 pi)
  real intensityAveragingInterval; // average intensity over this many periods

  bool plotHarmonicElectricFieldComponents;  // plot Er and Ei assuming : E(x,t) = Er(x)*cos(w*t) + Ei(x)*sin(w*t) 

  bool compareToReferenceShowFile;
  aString nameOfReferenceShowFile;
  bool plotDSIPoints;
  bool plotDSIMaxVertVals;
  RealArray maximumError, solutionNorm;
  int errorNorm; // set to 1 or 2 to compute L1 and L2 error norms
  
  real totalEnergy,initialTotalEnergy;

  real radiusForCheckingErrors;  // if >0 only check errors in a disk (sphere) of this radius

  RealArray initialConditionBoundingBox;  // limit initial conditions to lie in this box
  real boundingBoxDecayExponent; // initial condition has a smooth transition outside the bounding box

  GridTypeEnum gridType;
  ElementTypeEnum elementType;

  real chevronFrequency, chevronAmplitude;
  real cylinderRadius, cylinderAxisStart,cylinderAxisEnd; // for eigenfunctions of the cylinder
  
  realMappedGridFunction *fields;
  realMappedGridFunction *dissipation,*e_dissipation;
  realMappedGridFunction *errp;
  MappedGrid *mgp;
  MappedGridOperators *op;

  int numberLinesForPML; // width of the PML in grid lines
  int pmlPower;
  real pmlLayerStrength;
  int pmlErrorOffset;  // only check errors within this many lines of the pml 

  RealArray *vpml;  // for PML

  int orderOfAccuracyInSpace;
  int orderOfAccuracyInTime;
  bool useConservative;
  bool useVariableDissipation;
  int numberOfVariableDissipationSmooths;  // number of times to smooth the variable dissipation function 

  int degreeSpace,degreeSpaceX,degreeSpaceY,degreeSpaceZ, degreeTime;  // For TZ polynomial
  real omega[4]; // For trig poynomial: fx,fy,fz,ft;    

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
  RadiationBoundaryCondition *radiationBoundaryCondition;

  CompositeGrid *cgp;
  CompositeGridOperators *cgop;
  int numberOfFields;
  GridFunction *gf;  // new way to store fields 

  realCompositeGridFunction *cgfields;
  realCompositeGridFunction *cgdissipation,*e_cgdissipation;
  realCompositeGridFunction *cgerrp;
  realCompositeGridFunction *variableDissipation;
  realCompositeGridFunction *knownSolution; // for holding a known solution that may be expensive to recompute

  Interpolant *pinterpolant;

  realCompositeGridFunction *dsi_cgfieldsE0;
  realCompositeGridFunction *dsi_cgfieldsE1;
  realCompositeGridFunction *dsi_cgfieldsH;

  realCompositeGridFunction *pIntensity;             // holds the time averaged intensity
  realCompositeGridFunction *pHarmonicElectricField; // holds the time of Er and Ei
  
  realCompositeGridFunction &getCGField(Maxwell::FieldEnum f, int tn) ;

  ArraySimple<ArraySimple<int> > ulinksE,ulinksH;
  ArraySimple<ArraySimple<UnstructuredMappingAdjacencyIterator> > ulinks;

  //kkc sparse matrix (CSR) arrays for dsi Mat-Vec implementation (should be an Overture sparse-rep?)
  void setupDSICoefficients();
  void reconstructDSIField( real t, FieldEnum field, realMappedGridFunction &from, realMappedGridFunction &to );
  bool reconstructDSIAtEntities( real t, FieldEnum field, IntegerArray &entities, realMappedGridFunction &from, RealArray &to);
  bool useGhostInReconstruction;

  enum DSIBCOption {
    defaultDSIBC=0x0,
    forceExtrap=0x1,
    zeroBC=forceExtrap<<1,
    curlHBC=zeroBC<<1,
    curlcurlHBC=curlHBC<<1,
    curlcurlcurlHBC=curlcurlHBC<<1,
    forcedBC = curlHBC|curlcurlHBC|curlcurlcurlHBC
  };

  void applyDSIBC( realMappedGridFunction &gf, real t, bool isEField, bool isProjection=true, int bcopt = defaultDSIBC );
  void applyDSIForcing( realMappedGridFunction &gf, real t, real dt,bool isEField, bool isProjection=true );
  int getCenters( MappedGrid &mg, UnstructuredMapping::EntityTypeEnum cent, realArray &xe );



  ArraySimple<real> Ecoeff, Hcoeff;
  ArraySimple<int> Eindex, Eoffset, Hindex, Hoffset;
  ArraySimple<int> Dindex, Doffset;
  ArraySimple<real> Dcoeff, dispCoeff,E_dispCoeff; // Dcoeff are the non-zeros in the curl-curl operator, dispCoeff is the dissipation scaling
  ArraySimple< ArraySimple<real> > REcoeff, RHcoeff;
  ArraySimple< ArraySimple<int> > REindex, RHindex, SEindex, SHindex;
  //  ArraySimple<real> REcoeff, RHcoeff;
  //  ArraySimple<int> REindex, REoffset, RHindex, RHoffset;
  //  ArraySimple<real> REcoeff, RHcoeff;
  //  ArraySimple<int> REindex, REoffset, RHindex, RHoffset;
  
  ArraySimple<real> A,As;
  ArraySimple<int> Aindex,Aoffset,Asindex,Asoffset;

  ArraySimple<int> CCindex, CCoffset;
  ArraySimple<real> CCcoeff;

  realArray faceAreaNormals, edgeAreaNormals;

  // These next variables are for higher order time stepping
  int numberOfTimeLevels;  // keep this many time levels of u

  int myid;                // processor number
  int numberOfFunctions;   // number of f=du/dt 
  int currentFn;
  realArray *fn;
  realCompositeGridFunction *cgfn;

  GenericGraphicsInterface *gip;
  GraphicsParameters psp;
  
  GUIState *runTimeDialog;
  DialogData *pPlotOptionsDialog, *pParametersDialog;

  aString movieFileName;
  int movieFrame;
  int plotChoices, plotOptions;
  int debug;
  real numberOfGridPoints;
  int totalNumberOfArrays;  // for coutning A++ array

  aString nameOfGridFile;
  FILE *logFile;
  FILE *debugFile, *pDebugFile;  // pDebugFile = debug file for each processor.
  FILE *checkFile;

  bool useTwilightZone;
  OGFunction *tz;

  Ogshow *show;              // Show file
  ShowFileReader *referenceShowFileReader;     // for comparing to a reference solution

  // for sequences saved in the show file
  int sequenceCount,numberOfSequences;
  RealArray timeSequence,sequence;

  bool useStreamMode,saveGridInShowFile;
  int frequencyToSaveInShowFile, showFileFrameForGrid;
  bool saveDivergenceInShowFile, saveErrorsInShowFile; 


  enum TimingEnum
  { 
    totalTime=0,
    timeForInitialize,
    timeForDSIMatrix,
    timeForInitialConditions,
    timeForAdvance,
    timeForAdvanceRectangularGrids,
    timeForAdvanceCurvilinearGrids,
    timeForAdvanceUnstructuredGrids,
    timeForAdvOpt,
    timeForDissipation,
    timeForBoundaryConditions,
    timeForInterfaceBC,
    timeForRadiationBC,
    timeForRaditionKernel,
    timeForInterpolate,
    timeForUpdateGhostBoundaries,
    timeForForcing,
    timeForGetError,
    timeForProject,
    timeForIntensity,
    timeForComputingDeltaT,
    timeForPlotting,
    timeForShowFile,
    timeForWaiting,
    maximumNumberOfTimings      // number of entries in this list
  };
  RealArray timing;                                     // for timings, cpu time for some function
  aString timingName[maximumNumberOfTimings];    // name of the function being timed

  real sizeOfLocalArraysForAdvance;
};

#endif
