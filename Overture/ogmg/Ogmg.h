#ifndef OGMG_H
#define OGMG_H "Ogmg.h"

#include "Overture.h"
#include "Oges.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "Ogmg.h"
#include "OgmgParameters.h"
#include "GenericGraphicsInterface.h"
#include "GraphicsParameters.h"
#include "Interpolate.h"
#include "MultigridCompositeGrid.h"


Index IndexBB(int base, int bound, int stride=1 );
Index IndexBB(Index I, const int stride );

class TridiagonalSolver;  // forward declaration
class OGFunction;
class InterpolationData;

//-------------------------------------------------------------------------------------------------------
//    Overlapping Grid Multigrid Solver
//-------------------------------------------------------------------------------------------------------
class Ogmg
{
 public:


//   enum BoundaryConditionEnum  // these are the numbers defining values in the bc array
//   {
//     dirichlet=1,
//     neumann=2,
//     mixed=3,
//     equation,
//     extrapolation,
//     combination
//   };

  enum TransferTypesEnum
  {
    fullWeighting=0,
    restrictedFullWeighting,
    injection
  };

  enum OptionEnum
  {
    assumeSparseStencilOnRectangularGrids
  };
  

  enum SmootherTypeEnum
  {
    Jacobi                 =OgmgParameters::Jacobi,
    GaussSeidel            =OgmgParameters::GaussSeidel,
    redBlack               =OgmgParameters::redBlack,
    lineJacobiInDirection1 =OgmgParameters::lineJacobiInDirection1,
    lineJacobiInDirection2 =OgmgParameters::lineJacobiInDirection2,
    lineJacobiInDirection3 =OgmgParameters::lineJacobiInDirection3,
    lineZebraInDirection1  =OgmgParameters::lineZebraInDirection1,
    lineZebraInDirection2  =OgmgParameters::lineZebraInDirection2,
    lineZebraInDirection3  =OgmgParameters::lineZebraInDirection3,
    alternatingLineJacobi  =OgmgParameters::alternatingLineJacobi,
    alternatingLineZebra   =OgmgParameters::alternatingLineZebra,
    numberOfSmoothers
  };

  enum
  {
    allGrids=-99,
    allLevels=-100
  };


  Ogmg();
  Ogmg( CompositeGrid & mg, GenericGraphicsInterface *ps=0);
  ~Ogmg();
  
  
  // set and get parameters for Ogmg using the next object
  OgmgParameters parameters;
  
  void displaySmoothers(const aString & label, FILE *file=stdout);

  CompositeGrid & getCompositeGrid(){ return multigridCompositeGrid(); }  // return Ogmg's grid containing the multigrid levels

  FILE *getInfoFile(){return infoFile;} // Ogmg's info file
  FILE *getCheckFile(){return checkFile;} // Ogmg's check file

  realCompositeGridFunction & getDefect() { return defectMG;} 
  void computeDefect(int level){ defect(level); }  // determine defect on a level (for plotting etc)

  realCompositeGridFunction & getRhs() { return fMG;}  // RHS GF

  real getMaximumResidual() const;

  // Return the "mean" value of a grid function. Use a particular
  // definition of mean.
  real getMean(realCompositeGridFunction & u);

  int getNumberOfIterations() const;

  // return the order of extrapolation to use for a given order of accuracy and level 
  int getOrderOfExtrapolation( const int level ) const;

  int loadBalance( CompositeGrid & mg, CompositeGrid & mgcg );

  void set( GenericGraphicsInterface *ps );

  // Set the MultigridCompositeGrid to use: 
  void set( MultigridCompositeGrid & mgcg );

  // Set the name of the composite grid (for info in files)
  void setGridName( const aString & name );

  // Set the name of this instance of Ogmg (for info in debug files)
  void setSolverName( const aString & name );

  // set parameters equal to another parameter object.
  int setOgmgParameters(OgmgParameters & parameters );

  int setOption(OptionEnum option, bool trueOrFalse );
  //  int set( OptionEnum option, int value, real rvalue );
  //  int get( OptionEnum option, int & value, real & rvalue ) const;
  
  int setOrderOfAccuracy(const int & orderOfAccuracy);

  // Supply the coefficients (for all multigrid levels), optionally supply BC's
  int setCoefficientArray( realCompositeGridFunction & coeff,
                           const IntegerArray & boundaryConditions=Overture::nullIntArray(),
                           const RealArray & bcData=Overture::nullRealArray() );


  // Set the boundary conditions for corners and edges:
  int setCornerBoundaryConditions( BoundaryConditionParameters & bcParams, const int level );

  // Choose a predefined equation to solve. 
  int setEquationAndBoundaryConditions( OgesParameters::EquationEnum equation, CompositeGridOperators & op,
                                        const IntegerArray & boundaryConditions,
                                        const RealArray & bcData, 
                                        const RealArray & constantCoeff=Overture::nullRealArray(),
                                        realCompositeGridFunction *variableCoeff=NULL );

  virtual real sizeOf( FILE *file=NULL ) const ; // return number of bytes allocated by Oges, print info to a file

  int chooseBestSmoother();

  int update( GenericGraphicsInterface & gi ); // update parameters interactively
  int update( GenericGraphicsInterface & gi, CompositeGrid & cg ); // update parameters interactively
  void updateToMatchGrid( CompositeGrid & mg );

  // solve 
  int solve( realCompositeGridFunction & u, realCompositeGridFunction & f );

  void printStatistics(FILE *file=stdout) const;

  int smoothTest(GenericGraphicsInterface & ps, int plotOption);

  int coarseToFineTest();

  int fineToCoarseTest();
  
  int bcTest();
  
  // test the accuracy of the coarse grid equations
  int coarseGridSolverTest( int plotOption=0 );

  int get( const GenericDataBase & dir, const aString & name);
  int put( GenericDataBase & dir, const aString & name) const;

  static int debug;
  static OGFunction *pExactSolution;  // for testing
  static aString infoFileCaption[5]; // for captions on the convergence table in the info file.

// protected:   
  public:  // do this for now

  enum Timing
  {
    timeForDefect=0,
    timeForSmooth,
    timeForFineToCoarse,
    timeForFineToCoarseBC,
    timeForCoarseToFine,
    timeForDirectSolver,
    timeForSolve,
    timeForInterpolation,
    timeForBoundaryConditions,
    timeForDefectInSmooth,
    timeForTridiagonalSolverInSmooth,
    timeForTridiagonalFactorInSmooth,
    timeForRelaxInSmooth,
    timeForBoundarySmooth,
    timeForInterpolationSmooth,
    timeForInitialize,
    timeForBuildExtraLevels,
    timeForOperatorAveraging,
    timeForBuildPredefinedEquations,
    timeForFullMultigrid,
    timeForDefectNorm,
    timeForOgesSmootherInit,
    timeForMiscellaneous,
    timeForGhostBoundaryUpdate,
    timeForInterpolateCoarseFromFine,
    numberOfThingsToTime  // counts the number of elements in this list
  };

  enum InterpolationQualityEnum
  {
    canInterpolateQuality1=0,     // best quality, interpolates to correct order
    canInterpolateQuality2,       // 2nd best best quality, interpolates to correct order minus 1
    canInterpolateQuality3,       // 3rd best quality, interpolates to correct order minus 2
    canInterpolateWithExtrapolation, // bad quality -- should replace by a better quality result
    canInterpolateQualityBad,     // bad quality -- should replace by a better quality result
    canInterpolateQualityVeryBad, // worst quality-- should replace by a better quality result
    canNotInterpolate             // unable to interpolate at all
  };


  enum StencilTypeEnum
  {
   general=0, 
   sparse=1, 
   constantCoeff=2, 
   sparseConstantCoefficients=3,
   variableCoefficients=4,
   sparseVariableCoefficients=5
  };

  Oges directSolver;                  // direct solver for coarsest level
  Oges *ogesSmoother;                 // Oges smoother.

  MultigridCompositeGrid multigridCompositeGrid;   // this object keeps track of the multigrid hierachy and can be shared amongst different solvers

  aString gridName;  // name of the composite grid (used to name gridCheckFile)
  aString solverName;  // name of this instance of Ogmg

  realCompositeGridFunction uMG,fMG;   // local reference copies
  realCompositeGridFunction defectMG;  // holds defect
  realCompositeGridFunction cMG;     // coefficients stored here
  realCompositeGridFunction rightNullVector;  // right null vector used for singular problems.

  
  realCompositeGridFunction *leftNullVector;
  bool leftNullVectorIsComputed;
  
  realCompositeGridFunction *v;    // for singular problems.

  realCompositeGridFunction uOld;

  bool useForcingAsBoundaryConditionOnAllLevels;  // set to true for testing coarse to fine

  RealArray alpha;                       // compatibility value for singular problems.
  RealArray workUnits;           // number of work units used per level (one cycle only)
  RealArray constantCoefficients;  
  RealArray equationCoefficients;  // for predefined equations
  realCompositeGridFunction *varCoeff;  // for predefined equations.

  enum CycleResultsEnum
  {
    defectPerCycle=0,
    workUnitsPerCycle,
    grid0DefectPerCycle,
    numberOfCycleResults
  };
  RealArray cycleResults;        // holds some results for outputing for matlab and documentation etc.
  
  IntegerArray interpolantWasCreated;
  IntegerArray lineSmoothIsInitialized;   // (axis,grid,level)

  // The boundary condition type: 
  //     equation: means the interior equation is applied on the boundary and there is some
  //               equation on the ghost point (which may be explicitly set if bcSupplied==true, or
  //               which may only be defined by the values in the coefficient matrix.
  //     extrapolation : means there is a dirichlet like condition applied on the boundary
  IntegerArray boundaryCondition;         // values are one of equation, extrapolation, or combination

  // In some cases the boundary conditions are explicitly set (e.g. predefined or user defined)
  bool bcSupplied;                        // true if the booundary conditions are explicitly set
  bool bcDataSupplied;                    // true if extra bc data has been provided (e.g. mixed BC coeff's)
  IntegerArray bc;                        // values are dirichlet, neumann, mixed or extrapolation
  RealArray boundaryConditionData;        // 

  int subSmoothReferenceGrid;   // reference grid (uses 1 sub-smooth) for variable sub-smooths
  RealArray defectRatio;        // holds ratios of defects for auto-subSmooth
  
  IntegerArray isConstantCoefficients;  // isConstantCoefficients(grid)

  IntegerArray active;   // active(grid) = false if we do not need to solve on a grid.

  BoundaryConditionParameters bcParams;

  TridiagonalSolver ****tridiagonalSolver;  // [level][grid][axis]

  CompositeGridOperators *operatorsForExtraLevels;   // array of operators for extra levels.
  Interpolant **interpolant; 

  GenericGraphicsInterface *ps;
  GraphicsParameters psp;
  
  int myid;  // processor number

  // numberOfInstances counts the number of instances of Ogmg (so we can have different names for debug files)
  static int numberOfInstances;  
  FILE *debugFile;       // debug file
  FILE *pDebugFile;      // pDebugFile = debug file for each processor.
  FILE *infoFile;
  FILE *checkFile;
  FILE *gridCheckFile;   // check file for the multigrid levels constructed by Ogmg (to compare serial/paralle results)

  bool initialized; // TRUE if initialized with a grid.
  bool assumeSparseStencilForRectangularGrids;

  int orderOfAccuracy;         

  int width1,width2,width3,halfWidth1,halfWidth2,halfWidth3;
  int numberOfGridPoints;
  int numberOfSolves;               // total number of solves
  int totalNumberOfCycles;          // total number of cycles over all solves
  int numberOfCycles;               // number of cycles in last solve
  int numberOfIterations;           // number for the last call to solve
  int numberOfExtraLevels;
  int levelZero;                    // base level (for use with full multigrid)
  int *iterationCount;                // counts of iterations per level, for use with an F-cycle

  real workUnit, timeForAddition, timeForMultiplication, timeForDivision;
  real totalWorkUnits, totalResidualReduction, fullMultigridWorkUnits;
  real averageEffectiveConvergenceRate, sumTotalWorkUnits, l2NormRightHandSide;
  real maximumResidual;
  real tm[numberOfThingsToTime];     // for timings
  int timerGranularity;              // granularity of the timer (to avoid overhead in calling getCPU)
  int totalNumberOfCoarseGridIterations; // counts iterations used to solve coarse grid equations

  OgesParameters::EquationEnum equationToSolve;

  Interpolate interp;  // AMR interpolation for fine-to-coarse and coarse-to-fine

  char buff[100];

  int *nipn, *ndipn, **ipn, numberOfIBSArrays;  // for IBS -- could be shared amongst solvers with the same CompositeGrid.
  static real bogusRealArgument1,bogusRealArgument2;  // for a default args

  int applyInitialConditions();
  int applyFinalConditions();

  void assignBoundaryConditionCoefficients( realMappedGridFunction & coeff, int grid, int level, 
					    int sideToCheck=-1, int axisToCheck=-1 );
  void checkParameters();

  void init();

  void setup(CompositeGrid & mg );
  void setMean(realCompositeGridFunction & u, const real meanValue, int level);
  real l2Norm(const realCompositeGridFunction & e );
  real l2Norm(const realMappedGridFunction & e );
  real maxNorm(const realCompositeGridFunction & e );
  real maxNorm(const realMappedGridFunction & e );
  
  real l2Error(const realCompositeGridFunction & u, const realCompositeGridFunction & v );
  
  int initializeBoundaryConditions(realCompositeGridFunction & coeff);
  int initializeConstantCoefficients();

  int createNullVector();
  int saveLeftNullVector();
  int readLeftNullVector();
  real rightNullVectorDotU( const int & level, const RealCompositeGridFunction & u );

  // build coarse grid levels:
  int buildExtraLevels(CompositeGrid & mg);
  //   .. parallel version:
  int buildExtraLevelsNew(CompositeGrid & mg);

  // build the predefined equations
  int buildPredefinedEquations(CompositeGridOperators & cgop);
  int buildPredefinedCoefficientMatrix( int level, bool buildRectangular, bool buildCurvilinear );
  
  int buildPredefinedVariableCoefficients( RealCompositeGridFunction & coeff, const int level );

  int cycle(const int & level, const int & iteration, real & maximumDefect, const int & numberOfCycleIterations );  // cycle at level l

  OgmgParameters::FourthOrderBoundaryConditionEnum
    getGhostLineBoundaryCondition( int bc, int ghostLine, int grid, int level, 
                               int & orderOfExtrapolation, aString *bcName=NULL ) const;

  int setBoundaryConditions(const IntegerArray & boundaryConditions,
                            const RealArray & bcData=Overture::nullRealArray() );

  void smooth(const int & level, int numberOfSmoothingSteps, int cycleNumber );
  void smoothJacobi(const int & level, const int & grid, int smootherChoice = 0);
  void smoothGaussSeidel(const int & level, const int & grid);
  void smoothRedBlack(const int & level, const int & grid);
  void smoothLine(const int & level, const int & grid, const int & direction, bool useZebra=true,
                  const int smoothBoundarySide = -1 );
  void alternatingLineSmooth(const int & level, const int & grid, bool useZebra=true);
  
  void applyOgesSmoother(const int level, const int grid);

  void smoothBoundary(int level, 
	  	      int grid, 
		      int bcOption[6], 
		      int numberOfLayers=1, 
		      int numberOfIterations=1 );

  void smoothInterpolationNeighbours(int level, int grid );

  void computeDefectRatios( int level );

  bool useEquationOnGhostLineForDirichletBC(MappedGrid & mg, int level);
  bool useEquationOnGhostLineForNeumannBC(MappedGrid & mg, int level);

  void defect(const int & level);
  void defect(const int & level, const int & grid);
  
  void fineToCoarse(const int & level, bool transferForcing = false);
  void fineToCoarse(const int & level, const int & grid, bool transferForcing = false );

  void coarseToFine(const int & level);
  void coarseToFine(const int & level, const int & grid);

  // these can be used to compute an approximate norm (fast)
  real defectMaximumNorm(const int & level, int approximationStride=1 );  // max-norm
  real defectNorm(const int & level, const int & grid, int option=0, int approximationStride=8 ); 

  real getDefect(const int & level, 
		 const int & grid, 
		 realArray & f,      // could be const, except reshape needed
		 realArray & u,      // could be const, except reshape needed
		 const Index & I1,
		 const Index & I2,
		 const Index & I3,
		 realArray & defect,
		 const int lineSmoothOption = -1,
		 const int defectOption = 0,
                 real & defectL2Norm=bogusRealArgument1, real & defectMaxNorm=bogusRealArgument2 );

  void evaluateTheDefectFormula(const int & level, 
				const int & grid, 
				const realArray & c,
				const realArray & u,  
				const realArray & f, 
				realArray & defect, 
				MappedGrid & mg,
				const Index & I1,
				const Index & I2,
				const Index & I3,
				const Index & I1u,
				const Index & I2u,
				const Index & I3u,
                                const int lineSmoothOption);
  
  int fullMultigrid();

  int interpolate(realCompositeGridFunction & u, const int & grid =-1, int level=-1 );

  // here is the new way:
//   int assignBoundaryConditions(const int & level,  
// 			       const int & grid, 
// 			       RealMappedGridFunction & u, 
// 			       RealMappedGridFunction & f );

  int applyBoundaryConditions(const int & level,  
			      const int & grid, 
                              RealMappedGridFunction & u, 
                              RealMappedGridFunction & f );
  int applyBoundaryConditions( const int & level, RealCompositeGridFunction & u, RealCompositeGridFunction & f );
  
  // form coarse grid operator by averaging the fine grid operator
  int operatorAveraging(RealCompositeGridFunction & coeff, const int & level);
  int operatorAveraging(RealMappedGridFunction & coeffFine,
			RealMappedGridFunction & coeffCoarse,
			const IntegerArray & coarseningRatio,
			int grid  =0,
			int level =0 );
  
  int averageCoefficients(Index & I1, Index & I2, Index & I3,
			  Index & I1p, Index & I2p, Index & I3p,
			  Index & J1, Index & J2, Index & J3,
			  TransferTypesEnum option[3],
			  const realSerialArray & cFine, 
			  realSerialArray & cCoarse, int ipar[] );
  
  int markGhostPoints( CompositeGrid & cg );


  int getInterpolationCoordinates( CompositeGrid & cg0, // finer grid
				   CompositeGrid & cg1, // new coarser grid
				   const IntegerArray & ib,     // check these points...
				   const int grid,            // ..on this grid
                                   const IntegerArray & gridsToCheck, // ..from these grids
				   realSerialArray & rb,      // return these values
				   const bool isRectangular,
				   int iv0[3], real dx0[3], real xab0[2][3],   // these are used by Macros!
				   int iv1[3], real dx1[3], real xab1[2][3] );


// parallel version:
  int getInterpolationCoordinates( CompositeGrid & cg0, // finer grid
				   CompositeGrid & cg1, // new coarser grid
				   const IntegerArray & ib,     // check these points...
				   const int grid,            // ..on this grid
                                   const IntegerArray & gridsToCheck, // ..from these grids
				   realSerialArray & rb,      // return these values
				   const bool isRectangular,
				   int iv0[3], real dx0[3], real xab0[2][3],   // these are used by Macros!
				   int iv1[3], real dx1[3], real xab1[2][3],
                                   InterpolationData & ipd );


  int getInterpolationCoordinatesNew( CompositeGrid & cg0, // finer grid
				      CompositeGrid & cg1, // new coarser grid
				      const IntegerArray & ib,     // check these points...
				      const RealArray & xa,      // (x-coord's of the interp. pts)
				      const int grid,            // ..on this grid
				      const IntegerArray & gridsToCheck, // ..from these grids
				      realSerialArray & rb,      // return these values
				      const bool isRectangular,
				      int iv0[3], real dx0[3], real xab0[2][3],   // these are used by Macros!
				      int iv1[3], real dx1[3], real xab1[2][3],
				      InterpolationData & ipd,
                                      IntegerArray & ia0,           // list of all interp points
                                      realSerialArray & donorDist   // distance to donor
                                       );

  // old "new" parallel Version
  int getInterpolationCoordinatesNewOld( CompositeGrid & cg0, // finer grid
				      CompositeGrid & cg1, // new coarser grid
				      const IntegerArray & ib,     // check these points...
				      const RealArray & xa,  
				      const int grid,            // ..on this grid
				      const IntegerArray & gridsToCheck, // ..from these grids
				      realSerialArray & rb,      // return these values
				      const bool isRectangular,
				      int iv0[3], real dx0[3], real xab0[2][3],   // these are used by Macros!
				      int iv1[3], real dx1[3], real xab1[2][3],
				      InterpolationData & ipd );


  // old "scalar" version
  int getInterpolationCoordinates(CompositeGrid & cg0, // finer grid
				  CompositeGrid & cg1, // new coarser grid
				  int i,               // check this point
				  int grid,
                                  int iv[],
				  int jv[],
				  realSerialArray & r,
                                  bool isRectangular, 
                                  int iv0[3], real dx0[3], real xab0[2][3], int iv1[3], real dx[3], real xab[2][3] );

  InterpolationQualityEnum getInterpolationStencil(CompositeGrid & cg0,
						   CompositeGrid & cg1,
						   int i,
						   int iv[3],
						   int grid,
						   int l,
						   intSerialArray & inverseGrid,
						   intSerialArray & interpoleeGrid,
						   intSerialArray & interpoleeLocation,
						   intSerialArray & interpolationPoint,
						   intSerialArray & variableInterpolationWidth,
						   realSerialArray & interpolationCoordinates,
						   realSerialArray & inverseCoordinates );
  
  int checkForBetterQualityInterpolation( realSerialArray & x, int gridI, InterpolationQualityEnum & interpolationQuality,
					  CompositeGrid & cg0,
					  CompositeGrid & cg1,
					  int i,
					  int iv[3],
					  int grid,
					  int l,
					  intSerialArray & inverseGrid,
					  intSerialArray & interpoleeGrid,
					  intSerialArray & interpoleeLocation,
					  intSerialArray & interpolationPoint,
					  intSerialArray & variableInterpolationWidth,
					  realSerialArray & interpolationCoordinates,
					  realSerialArray & inverseCoordinates );
  
  // output results to a matlab file:
  int outputCycleInfo();

  // output some results at every cycle
  int outputResults(const int & level, const int & iteration, real & maximumDefect, real & defectNew, real & defectOld);
  int buildCoefficientArrays();

  int addAdjustmentForSingularProblem(int level, int iteration );
  int removeAdjustmentForSingularProblem(int level, int iteration );
  int getSingularParameter(int level);
  int computeLeftNullVector();
  
};


#endif
