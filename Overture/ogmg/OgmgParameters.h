#ifndef OGMG_PARAMETERS_H
#define OGMG_PARAMETERS_H

// This class holds parameters used by Ogmg.
// You can save commonly used parameter values in an object of this class.
// You can interactively update parameter values.
// To make these parameters known to a particular Ogmg object, use the
// Ogmg::setOgmgParameters function.

#include "Overture.h"
#include "OgesParameters.h"
#include "DBase.hh"
using namespace DBase;

// forward declarations:
class Ogmg;  
class GenericGraphicsInterface;
class DialogData;
class LoadBalancer;


class OgmgParameters
{
 public:  

  enum OptionEnum  // ** remember to change the documentation if you change this list.
  {
    THEnumberOfCycles,                  // number of times to iterate on each level (=1 -> V-cycle, 2=W-cycle)
    THEnumberOfPreSmooths,                 // (minimum) number of pre-smooths (level)
    THEnumberOfPostSmooths,                 // (minimum) number of postsmooths (level)
    THEnumberOfSmooths,                 // will go away
    THEnumberOfSubSmooths,              // number of sub smooths (grid,level)
    THEsmootherType,                    // type of smooth (grid,level)
    THEsmoothingRateCutoff,             // continue smoothing until smoothing rate is bigger than this
    THEuseDirectSolverOnCoarseGrid,     // if true use Oges, if false use a 'smoother' on the coarse grid.
    THEresidualTolerance, 
    THEabsoluteTolerance,
    THEerrorTolerance, 
    THEmeanValueForSingularProblem,
    THEprojectRightHandSideForSingularProblem,
    THEassignMeanValueForSingularProblem,
    THEmaximumNumberOfIterations,
    THEmaximumNumberOfExtraLevels,
    THEfineToCoarseTransferWidth,
    THEcoarseToFineTransferWidth,
    THEnumberOfInitialSmooths,
    THEuseFullMultigrid,
    THEorderOfAccuracy
  };

  enum CycleTypeEnum
  {
    cycleTypeC,      // normal "gamma" cycle, gamma=1: V, gamma=2 : W
    cycleTypeF,      // F cycle
    cycleAdaptive   // adaptive cycle (not implemented yet)
  };


  // Here are smoothers:
  enum SmootherTypeEnum
  {
    Jacobi=0,
    GaussSeidel,
    redBlack,                // red-black Gauss-Seidel
    redBlackJacobi,
    lineJacobiInDirection1,
    lineJacobiInDirection2,
    lineJacobiInDirection3,
    lineZebraInDirection1,
    lineZebraInDirection2,
    lineZebraInDirection3,
    alternatingLineJacobi,
    alternatingLineZebra,
    ogesSmoother,
    numberOfSmoothers
  };
  
  
  // Convergence can be determined in different ways: *wdh* 110310
  enum ConvergenceCriterionEnum
  {
    residualConverged,        // l2Norm(residual) < residualTolerance*l2Nnorm(f) + absoluteTolerance
    errorEstimateConverged,   // max(error estimate) < errorTolerance
    residualConvergedOldWay   // max(residual) < relativeTolerance*numberOfGridPoints (old way - keep as an option for now)
  };
    

  // these are the numbers defining values in the cg.boundaryCondition(side,axis) array
  // **These should match those in OgesParameters.h**
  enum BoundaryConditionEnum  
  {                           
    dirichlet=1,
    neumann=2,
    mixed=3,
    equation=4,
    extrapolate=5,
    combination=6,
    equationToSecondOrder=7,
    mixedToSecondOrder=8,
    evenSymmetry=9,
    oddSymmetry=10,
    extrapolateTwoGhostLines=11,
    parallelGhostBoundary=20
  };

  enum NullVectorOptionsEnum
  {
    computeNullVector=0,
    computeAndSaveNullVector,
    readOrComputeNullVector,
    readOrComputeAndSaveNullVector
  };


  enum
  {
    allGrids=-99,
    allLevels=-100,
    useLevelsInGrid=-1
  };

  OgmgParameters();
  OgmgParameters(CompositeGrid & cg);  // apply parameters to this grid.
  ~OgmgParameters();

  virtual OgmgParameters& operator=(const OgmgParameters& par);

  // Automatically choose good parameters (smoothers etc.) for a grid
  int chooseGoodMultigridParameters(CompositeGrid & cg, int maxLevels=useLevelsInGrid, int robustnessLevel=1 );

  // print out current values of parameters
  int display(FILE *file = stdout);

  int get( OptionEnum option, int & value ) const;
  int get( OptionEnum option, real & value ) const;

  int get( const GenericDataBase & dir, const aString & name);

  int put( GenericDataBase & dir, const aString & name) const;

  int set( CompositeGrid & cg);  // apply parameters to this grid.
  
  // aString getSolverName();         

  // ----------------Functions to set parameters -----------------------------
  int set( OptionEnum option, int value=0 );
  int set( OptionEnum option, float value );
  int set( OptionEnum option, double value );
	   		      
  // Set the absolute tolerance for the residual based convergence criteria.
  int setAbsoluteTolerance(const real absoluteTolerance );

  // Set the tolerance for the error, iterate until the norm of the estimated error is 
  // less than "errorTolerance"
  int setErrorTolerance(const real errorTolerance );

  int setMaximumNumberOfIterations( const int max );

  // set the mean value of the solution for a singular problem
  int setMeanValueForSingularProblem( const real meanValue );

  // set option concerning the null vector for singular problems
  int setNullVectorOption( NullVectorOptionsEnum option, const aString & fileName );

  // specify the number of iterations per level (1=V cycle, 2=W cycle)
  int setNumberOfCycles( const int & number, const int & level=allLevels );

  // Set the number of smooths on a given level
  int setNumberOfSmooths(const int numberOfPreSmooths, const int numberOfPostSmooths, const int level);

  int setNumberOfSubSmooths( const int & numberOfSmooths, const int & grid, const int & level=allLevels);

  int setParameters( const Ogmg & ogmg);       // set all parameters equal to those values found in ogmg.

  // Indicate if the problem is singular
  int setProblemIsSingular( const bool trueOrFalse=TRUE );

  // Set the relative tolerance for the residual based convergence criteria (scaled by the l2Norm(f))
  int setResidualTolerance(const real residualTolerance );

  int setSmootherType(const SmootherTypeEnum & smoother, 
		      const int & grid=allGrids, 
		      const int & level=allLevels );

  int updateToMatchGrid( CompositeGrid & cg, int maxLevels=useLevelsInGrid );  
  int update( GenericGraphicsInterface & gi, CompositeGrid & cg ); // update parameters interactively

  /// Here is a database to hold parameters (new way)
  mutable DataBase dbase; 

 protected:

  int buildOptionsDialog( DialogData & dialog );

  int numberOfMultigridLevels() const;  // this is how many levels the parameters thinks exists
  int numberOfComponentGrids() const;  // this is how many grids the parameters thinks exists
  

  void init();
  int initializeGridDependentParameters(CompositeGrid & cg, int maxLevels=useLevelsInGrid );
  
  int set( OptionEnum option, int value, real rvalue );
  int get( OptionEnum option, int & value, real & rvalue ) const;

  CycleTypeEnum cycleType;

  IntegerArray numberOfCycles;      // number of times to iterate on each level (=1 -> V-cycle, 2=W-cycle)
  IntegerArray numberOfSmooths;         // (minimum) number of smooths (level)
  IntegerArray numberOfSubSmooths;      // number of sub smooths (grid,level)
  IntegerArray smootherType;            // type of smooth (grid,level)

  // Lower and upper bounds for the adaptive smoothing algorithm:
  real defectRatioLowerBound,defectRatioUpperBound;
  real defectRatioLowerBoundLineSmooth,defectRatioUpperBoundLineSmooth;


  real omegaJacobi;                     // relaxation parameter for Jacobi smoother
  real omegaGaussSeidel;                // relaxation parameter for Gauss-Seidel smoother
  real omegaRedBlack;                   // relaxation parameter for red-black smoother
  real omegaLineJacobi;
  real omegaLineZebra;
  real variableOmegaScaleFactor;        // for scaling automatically chosen omega
  bool useLocallyOptimalOmega;                // use locally optimal omega on non-uniform grids
  bool useLocallyOptimalLineOmega;            // use locally optimal omega on non-uniform grids (line solvers)
  bool useSplitStepLineSolver;
  bool interpolateAfterSmoothing;
  bool useNewRedBlackSmoother;  // this one works in parallel

  real smoothingRateCutoff;             // continue smoothing until smoothing rate is bigger than this
  bool useDirectSolverOnCoarseGrid;     // if false use a 'smoother' on the coarse grid.
  int numberOfIterationsOnCoarseGrid;   // if iterating on the coarse grid
  bool useDirectSolverForOneLevel;      // if true call direct solver if there is only 1 level, otherwise use smoother
  
  int maximumNumberOfSubSmooths;      
  int maximumNumberOfLineSubSmooths;
  int subSmoothReferenceGrid;   // reference grid (uses 1 sub-smooth) for variable sub-smooths

  bool useFullMultigrid;            // use a full MG cycle (i.e. start solution process from the coarse grid)
  int minimumNumberOfInitialSmooths;       // smooth at least this many times on the first iteration.

  bool problemIsSingular;
  bool projectRightHandSideForSingularProblem;
  bool assignMeanValueForSingularProblem;
  bool adjustEquationsForSingularProblem;
  NullVectorOptionsEnum nullVectorOption;
  aString nullVectorFileName;
  OgesParameters *nullVectorParameters;  // Oges parameters when solving for the null vector

  
  ConvergenceCriterionEnum convergenceCriteria;

  real residualTolerance, errorTolerance, absoluteTolerance, meanValueForSingularProblem;
  int maximumNumberOfIterations;
  bool useErrorEstimateForConvergence;  // if true check the error estimate in the convergence test

  OgesParameters ogesParameters;  // for the coarse grid solver
  CompositeGrid *cgPointer;

  OgesParameters *ogesSmoothParameters; // for Oges solvers used as smoothers
  IntegerArray activeGrids;

  LoadBalancer *loadBalancer;  // for parallel load balancing 

  aString *smootherName;
  
  bool interpolateTheDefect;
  int maximumNumberOfExtraLevels;

  // we can read/save the multigrid composite grid instead of generating it:
  bool saveMultigridCompositeGrid, readMultigridCompositeGrid;
  aString nameOfMultigridCompositeGrid;

  bool autoSubSmoothDetermination;
  bool useNewAutoSubSmooth;  // use defects computed earlier for determining defect ratios

  bool showSmoothingRates;
  bool outputMatlabFile;
  bool useOptimizedVersion;  // if true use the new optimized smoothers etc.
  bool decoupleCoarseGridEquations;  // if true set interpolation points to zero on all coarser levels.

  int maximumNumberOfLevels;

  int fineToCoarseTransferWidth;  // 1=injection, 3= full weighting
  int coarseToFineTransferWidth;  // 2=linear interpolation, 4=cubic interpolation


  // boundary conditions are : extrapolation, equation, combination
  //  extrapolation: usually means a dirichlet BC on the boundary
  //  equation : usually means a neumann BC on the ghost line

  // possible conditions on the boundary or ghost line are
  //       imposeDirichlet          : impose a dirichlet BC
  //       imposeExtrapolation    : explicitly impose an extrapolation equation
  //       injection             : 
  //       partialWeighting   : full weighting in the tangential directions
  //       halfWeighting      : full weighting but exclude ghost points.
  //       lumpedPartialWeighting : lump partial weighting coefficients in tangential directions.
  //       imposeNeumann
  enum BoundaryAveragingOption
  {
    imposeDirichlet,
    imposeExtrapolation,
    injection,
    partialWeighting,
    halfWeighting,
    lumpedPartialWeighting,
    imposeNeumann
  };

  // These hold the default options for the boundary and ghostline
  BoundaryAveragingOption boundaryAveragingOption[2];  // [0] : for extrapolation BC, [1] : for equation BC
  BoundaryAveragingOption ghostLineAveragingOption[2];
  
  enum AveragingOption
  {
    averageCoarseGridEquations,
    doNotAverageCoarseGridEquations,
    doNotAverageCoarseCurvilinearGridEquations
  };
  AveragingOption averagingOption;

  int useSymmetryForNeumannOnLowerLevels;  // replace Neumann BC by even symmetry for l>0
  int useSymmetryForDirichletOnLowerLevels;  // replace Neumann BC by even symmetry for l>0
  int useSymmetryCornerBoundaryCondition;
  int useEquationForDirichletOnLowerLevels;  // for 4th order
  int useEquationForNeumannOnLowerLevels;  // for 4th order
  int solveEquationWithBoundaryConditions;  // for 4th order, solve PDE on boundary with BC's

  enum FourthOrderBoundaryConditionEnum
  {
    useUnknown=-1,   // bogus value
    useSymmetry,
    useEquationToFourthOrder,
    useEquationToSecondOrder,
    useExtrapolation,
  };

  FourthOrderBoundaryConditionEnum dirichletFirstGhostLineBC;
  FourthOrderBoundaryConditionEnum dirichletSecondGhostLineBC;
  FourthOrderBoundaryConditionEnum lowerLevelDirichletFirstGhostLineBC;
  FourthOrderBoundaryConditionEnum lowerLevelDirichletSecondGhostLineBC;
  int orderOfExtrapolationForDirichlet;
  int orderOfExtrapolationForDirichletOnLowerLevels;

  FourthOrderBoundaryConditionEnum neumannFirstGhostLineBC;
  FourthOrderBoundaryConditionEnum neumannSecondGhostLineBC;
  FourthOrderBoundaryConditionEnum lowerLevelNeumannFirstGhostLineBC;
  FourthOrderBoundaryConditionEnum lowerLevelNeumannSecondGhostLineBC;
  int orderOfExtrapolationForNeumann;
  int orderOfExtrapolationForNeumannOnLowerLevels;
  
  int orderOfAccuracy;  

  int fourthOrderBoundaryConditionOption;

  int numberOfBoundaryLayersToSmooth; // extra smoothing at boundaries
  int numberOfBoundarySmoothIterations;
  int numberOfLevelsForBoundarySmoothing;

  bool combineSmoothsWithIBS;               // if true then merge regular smooths with IBS smooths
  int numberOfInterpolationLayersToSmooth; // for smoothing points near interpolation points
  int numberOfInterpolationSmoothIterations;  // for smoothing interpolation neighbours
  int numberOfLevelsForInterpolationSmoothing;
  int numberOfIBSIterations;                  // global iterations of interp. boundary smoothing

  int gridOrderingForSmooth;  // 0= 1...ng  1= ng...1 2=alternate
  int totalNumberOfSmooths;
  IntegerArray totalNumberOfSmoothsPerLevel;  // counts smooths per level
  IntegerArray totalNumberOfSubSmooths; // counts sub-smooths per grid

  int coarseGridInterpolationWidth;  // -1 = use default

  int alternateSmoothingDirections;  // 0=no, 1=yes

  bool useNewFineToCoarseBC;

  int maximumNumberOfInterpolationIterations; 

  bool gridDependentParametersInitialized;

  bool allowInterpolationFromGhostPoints;  // for coarse level interpolation points that are outside a grid
  bool allowExtrapolationOfInterpolationPoints;  // for coarse level interpolation points that are outside a grid
 
  bool saveGridCheckFile; // save the coarse grid check file 

  int chooseGoodParametersOption;  // choose good parameters has different levels of robustness, 0=OFF

  friend class Ogmg;
};

#endif
