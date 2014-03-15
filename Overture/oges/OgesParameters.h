#ifndef OGES_PARAMETERS_H
#define OGES_PARAMETERS_H

// This class holds parameters used by Oges.
// You can save commonly used parameter values in an object of this class.
// You can interactively update parameter values.
// To make these parameters known to a particular Oges object, use the
// setOgesParameters function.

#include "Overture.h"
#include "ShowFileParameter.h"

class Oges;  // forward declaration
class GenericGraphicsInterface;
class Ogmg;
class OgmgParameters;

class OgesParameters
{
 public:  

  enum EquationEnum
  {
    userDefined,
    laplaceEquation,
    divScalarGradOperator,              // div( s(x) grad )
    heatEquationOperator,               // I + c0*Delta
    variableHeatEquationOperator,       // I + s(x)*Delta
    divScalarGradHeatEquationOperator,  // I + div( s(x) grad )
    secondOrderConstantCoefficients
  };

  enum BoundaryConditionEnum  // these are possible boundary conditions for pre-defined equations.
  {                           // **The first few entries should match those in OgmgParameters.h**
    dirichlet=1,
    neumann=2,
    mixed=3,
    equation=4,         // for ogmg
    extrapolate=5,
    combination=6,      // for ogmg
    axisymmetric,
    dirichletAndEvenSymmetry,
    dirichletAndOddSymmetry
  };


  enum OptionEnum  // ** remember to change the documentation if you change this list.
  {
    THEabsoluteTolerance,
    THEisAxisymmetric,       // for predefined equations
    THEbestIterativeSolver,  // choose the 'best' iterative solver and options.
    THEbestDirectSolver,     // choose the 'best' direct solver and options.
    THEcompatibilityConstraint,
    THEexternalSolver,
    THEfillinRatio,
    THEfillinRatio2,
    THEfixupRightHandSide,
    THEgmresRestartLength,
    THEharwellPivotingTolerance,
    THEincompleteLUExpectedFill,
    THEincompleteLUDropTolerance,       // =dt for ILUT(dt), ILUTK(dt,k) (from DH preconditioners)
    THEincompleteLUSparseACoefficient,  // =sparseA for sparsifying matrix prior to ILU (from DH)
    THEincompleteLUSparseFCoefficient,  // =sparseF for sparsifying factorization after ILU (from DH)
    THEincompleteLUTypeInDH,            // =ilu_type in DH preconditioners
    THEiterativeImprovement,
    THEkeepCoefficientGridFunction, // keep a reference to the user's coeff grid function
    THEkeepSparseMatrix,            // keep ia,ja,a sparse matrix even it not needed by the solver
    THEmatrixCutoff,
    THEmatrixOrdering,
    THEmaximumInterpolationWidth,
    THEmaximumNumberOfIterations,
    THEminimumNumberOfIterations,
    THEnullVectorScaling,
    THEnumberOfIncompleteLULevels,
    THEorderOfExtrapolation,         // for predefined equation ghost points
    THEpreconditioner,
    THEparallelExternalSolver,
    THEparallelPreconditioner,
    THEparallelSolverMethod,
    THErelativeTolerance,
    THEremoveSolutionAndRHSVector,      // de-allocate sol and rhs vector after every solve
    THEremoveSparseMatrixFactorization, // de-allocate any factorization info after every solve.
    THErescaleRowNorms,
    THEsolveForTranspose,
    THEsolverMethod,
    THEsolverType,
    THEtolerance,
    THEuserSuppliedCompatibilityConstraint,
    THEzeroRatio
  };
  
  enum SolverEnum
  {
    defaultSolver,
    sor,
    yale,
    harwell,
    SLAP,
    PETSc,
    multigrid,
    PETScNew,    // new parallel solver
    userSolver1, // these are reserved for new user defined solvers.
    userSolver2,
    userSolver3,
    userSolver4,
    userSolver5
  };


  enum SolverMethodEnum
  {
    defaultSolverMethod,
    richardson,
    chebychev,
    conjugateGradient,
      cg=conjugateGradient,           // cg= short PETSc name
    biConjugateGradient,
      bicg=biConjugateGradient,
    biConjugateGradientSquared,
    conjugateGradientSquared,
      cgs=conjugateGradientSquared,
    biConjugateGradientStabilized,    
      bcgs=biConjugateGradientStabilized,
    generalizedMinimalResidual,
      gmres=generalizedMinimalResidual,
    transposeFreeQuasiMinimalResidual, 
      tfqmr=transposeFreeQuasiMinimalResidual,
    transposeFreeQuasiMinimalResidual2,         // tcqmr Tony Chan's version
      tcqmr=transposeFreeQuasiMinimalResidual2,
    conjugateResidual,
      cr=conjugateResidual,
    leastSquares,
      lsqr=leastSquares,
    preonly,
    qcg                     // minimize a quadratic function
  };
  
  enum PreconditionerEnum
  {
    defaultPreconditioner,
    noPreconditioner,
    jacobiPreconditioner,
    sorPreconditioner,
    luPreconditioner,
    shellPreconditioner,
    DHILUPreconditioner,
    blockJacobiPreconditioner,
    multigridPreconditioner,
    eisenstatPreconditioner,
    incompleteCholeskyPreconditioner,
    incompleteLUPreconditioner,
    additiveSchwarzPreconditioner,
    kspPreconditioner,  // was slesPreconditioner
    compositePreconditioner,
    redundantPreconditioner,
    diagonalPreconditioner,
    ssorPreconditioner,
    sparseApproximateInversePreconditioner,
    neumannNeumannPreconditioner,
    chloleskyPreconditioner,
    samgPreconditioner,
    pointBlockJacobiPreconditioner,
    matrixPreconditioner,
    hyprePreconditioner,
    fieldSplitPreconditioner,
    tfsPreconditioner,
    mlPreconditioner,
    prometheusPreconditioner
  };
  


  enum MatrixOrderingEnum
  {
    defaultMatrixOrdering,
    naturalOrdering,
    nestedDisectionOrdering,
    oneWayDisectionOrdering,
    reverseCuthillMcKeeOrdering,
    quotientMinimumDegreeOrdering,
    rowlengthOrdering
   };


  // there are different options we can use for the external solver with PETSc
  enum ExternalSolverEnum
  {
    defaultExternalSolver,
    superlu,
    superlu_dist,
    superlu_mp,
    mumps,
    hypre
  };


  OgesParameters();
  ~OgesParameters();
  OgesParameters& operator=(const OgesParameters& x);
  
  aString getSolverName() const;                      // return the name (composite of solver, preconditioner,...)
  aString getSolverTypeName(SolverEnum solverType = defaultSolver) const;         
  aString getSolverMethodName(SolverMethodEnum solverMethod = defaultSolverMethod ) const;         
  aString getPreconditionerName(PreconditionerEnum preconditioner = defaultPreconditioner) const; 
  aString getMatrixOrderingName(MatrixOrderingEnum matrixOrdering = defaultMatrixOrdering ) const;         

  int setParameters( const Oges & oges);       // set all parameters equal to those values found in oges.
  int update( GenericGraphicsInterface & gi, CompositeGrid & cg ); // update parameters interactively

  // ----------------Functions to set parameters -----------------------------
  int set( SolverEnum option );
  int set( SolverMethodEnum option );
  int set( MatrixOrderingEnum option );
  int set( PreconditionerEnum option );
	   
  int set( OptionEnum option, int value=0 );
  int set( OptionEnum option, float value );
  int set( OptionEnum option, double value );
	   		      
  // set a PETSc option 
  int setPetscOption( const aString & name, const aString & value );
  bool getPetscOption( const aString & name, aString & value ) const;


  SolverEnum getSolverType() const;

  int get( OptionEnum option, int & value ) const;
  int get( OptionEnum option, real & value ) const;

  OgmgParameters* getOgmgParameters() const; 
  OgmgParameters& buildOgmgParameters(); 
  
  int get( const GenericDataBase & dir, const aString & name);
  int put( GenericDataBase & dir, const aString & name) const;
  
  int isAvailable( SolverEnum solverType );
  
  // print out current values of parameters
  int display(FILE *file = stdout);

  ListOfShowFileParameters petscOptions; // holds options for PETSc

 protected:

  int set( OptionEnum option, int value, real rvalue );
  int get( OptionEnum option, int & value, real & rvalue ) const;

  SolverEnum solver;
  aString solverName;
  
  SolverMethodEnum solverMethod, parallelSolverMethod;
  PreconditionerEnum preconditioner, parallelPreconditioner;
  MatrixOrderingEnum matrixOrdering;
  ExternalSolverEnum externalSolver, parallelExternalSolver;
  
  real relativeTolerance;          // relative tolerance. if <=0. routine should choose a value.
  real absoluteTolerance;
  real maximumAllowableIncreaseInResidual; // stop iterations if residual increases greater than this value.

  bool compatibilityConstraint;
  bool userSuppliedCompatibilityConstraint;    // set to true if the user has supplied a right null vector

  int gmresRestartLength;
  int numberOfIncompleteLULevels;
  real incompleteLUExpectedFill;
  int minimumNumberOfIterations;
  int maximumNumberOfIterations;
  int solveForTranspose;
  int rescaleRowNorms;

  int blockSize;           // block size for block matrices (e.g. for systems of equations)

  real matrixCutoff;        // epsz
  bool fixupRightHandSide; // zero out rhs at interp., extrap and periodic Points?

  real zeroRatio;         // zratio
  real fillinRatio;       // fratio
  real fillinRatio2;      // fratio2
  real harwellPivotingTolerance;  // tolerance for harwell pivoting
  real nullVectorScaling;

  int preconditionBoundary;
  int preconditionRightHandSide;
  int maximumInterpolationWidth;
  int iterativeImprovement; 
  
  int orderOfExtrapolation;  // for predefined equations (-1: use default)

  real sorOmega;                              // omega

  int isAxisymmetric;   // is this problem axisymmetric (for predefined equations)

  // parameters for experimental DH preconditioner, used with PETSc **pf**
  real incompleteLUDropTolerance;          // =dt for ILUT(dt), ILUTK(dt,k) (from DH preconditioners)
  real incompleteLUSparseACoefficient;     // =sparseA for sparsifying matrix prior to ILU (from DH)
  real incompleteLUSparseFCoefficient;     // =sparseF for sparsifying factorization after ILU (from DH)
  int  incompleteLUTypeInDH;               // =ilu_type in DH preconditioners: 1=iluk with sparsify

  // parameters for memory management: These are all false by default.
  bool keepCoefficientGridFunction; // keep a reference to the user's coeff grid function
  bool keepSparseMatrix;            // keep ia,ja,a sparse matrix even it not needed by the solver
  bool removeSolutionAndRHSVector;    // de-allocate sol and rhs vector after every solve
  bool removeSparseMatrixFactorization; // de-allocate sparse matrix factorization after solving.

  OgmgParameters *ogmgParameters;

  friend class Oges;
  friend class EquationSolver;
  friend class PETScEquationSolver;
  friend class YaleEquationSolver;
  friend class HarwellEquationSolver;
  friend class SlapEquationSolver;
  friend class Ogmg;
  friend class PETScSolver;
  
};

#endif
