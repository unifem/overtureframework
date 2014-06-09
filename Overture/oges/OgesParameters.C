#include "OgesParameters.h"
#include "GenericGraphicsInterface.h"
#include "OgmgParameters.h"
#include "Oges.h"

//\begin{>OgesParametersInclude.tex}{\subsection{constructor}} 
OgesParameters::
OgesParameters()
//==================================================================================
// /Description:
//   Constructor for an OgesParameters object. Use this class to set
// parameters for Oges.
//
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  #ifndef USE_PPP
    // default solver in serial: 
    solver=yale;
  #else
    // default solver in parallel:
    solver=PETScNew;
  #endif
  solverName=getSolverTypeName();
  
  parallelSolverMethod=gmres;
  #ifdef USE_PPP
    solverMethod=preonly;   // by default in parallel use the PC only on each block (each processor)
  #else
    solverMethod=gmres;
  #endif
  
  preconditioner=incompleteLUPreconditioner;
  parallelPreconditioner=blockJacobiPreconditioner;
  
  // With PETSc there are various external solvers available
  externalSolver=defaultExternalSolver;
  parallelExternalSolver=defaultExternalSolver;

  matrixOrdering=reverseCuthillMcKeeOrdering;
  
  relativeTolerance=0.;    // 0. means choose a resonable value
  absoluteTolerance=0.;    // 0. means choose a resonable value
  maximumAllowableIncreaseInResidual=1.e5;   // stop iterations if residual increases greater than this value.

  blockSize=1;  // block size for block matrices (e.g. for systems of equations)

  compatibilityConstraint=false;
  userSuppliedCompatibilityConstraint=false;

  gmresRestartLength=20;
  numberOfIncompleteLULevels=1;
  incompleteLUExpectedFill=3.;
  minimumNumberOfIterations=0;
  maximumNumberOfIterations=0; // 0 means have Oges choose a resaonable value
  solveForTranspose=false;

  matrixCutoff=0.;         // epsz
  fixupRightHandSide=true; // zero out rhs at interp., extrap and periodic Points?
  
  zeroRatio=0.;         // zratio
  fillinRatio=0.;       // fratio
  fillinRatio2=0.;      // fratio2
  harwellPivotingTolerance=.1;  // tolerance for harwell pivoting
  nullVectorScaling=1.;

  preconditionBoundary=false;
  preconditionRightHandSide=false;
  maximumInterpolationWidth=100;
  iterativeImprovement=false; 
  rescaleRowNorms=true;

  keepCoefficientGridFunction=true; // keep a reference to the user's coeff grid function
  keepSparseMatrix=false;   // keep ia,ja,a sparse matrix even it not needed by the solver
  removeSolutionAndRHSVector=false;
  removeSparseMatrixFactorization=false;
  
  ogmgParameters=NULL;

  //..Parameters for experimental DH preconditioner, used with PETSc
  incompleteLUDropTolerance       =0.;    // =dt for ILUT(dt), ILUTK(dt,k) (from DH preconditioners)
  incompleteLUSparseACoefficient  =0.;    // =sparseA for sparsifying matrix prior to ILU (from DH)
  incompleteLUSparseFCoefficient  =1e-5;  // =sparseF for sparsifying factorization after ILU (from DH)
  incompleteLUTypeInDH            =1;     // =ilu_type in DH preconditioners: 1=iluk with sparsify

  sorOmega = 1.01;
  
  orderOfExtrapolation=-1;  // for predefined equations (-1: use default)
  
  isAxisymmetric=0;   // is this problem axisymmetric (for predefined equations)
}

OgesParameters::
~OgesParameters()
{
  // printf("OgesParameters:: destructor: delete ogmgParameters=%i \n",ogmgParameters);
  
  delete ogmgParameters;
}

//\begin{>OgesParametersInclude.tex}{\subsection{operator=}} 
OgesParameters& OgesParameters:: 
operator=(const OgesParameters& x)
//==================================================================================
// /Description:
//   deep copy of data.
//
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  blockSize=x.blockSize;

  solver=x.solver;
  solverName=x.solverName;
  
  solverMethod=x.solverMethod;
  parallelSolverMethod=x.parallelSolverMethod;
  
  preconditioner=x.preconditioner;
  parallelPreconditioner=x.parallelPreconditioner;
  
  externalSolver=x.externalSolver;
  parallelExternalSolver=x.parallelExternalSolver;

  matrixOrdering=x.matrixOrdering;
  
  relativeTolerance=x.relativeTolerance;          
  absoluteTolerance=x.absoluteTolerance;
  maximumAllowableIncreaseInResidual=x.maximumAllowableIncreaseInResidual;

  compatibilityConstraint=x.compatibilityConstraint;
  userSuppliedCompatibilityConstraint=x.userSuppliedCompatibilityConstraint;
  
  gmresRestartLength=x.gmresRestartLength;
  numberOfIncompleteLULevels=x.numberOfIncompleteLULevels;
  incompleteLUExpectedFill=x.incompleteLUExpectedFill;
  minimumNumberOfIterations=x.minimumNumberOfIterations;
  maximumNumberOfIterations=x.maximumNumberOfIterations;
  solveForTranspose=x.solveForTranspose;
  rescaleRowNorms=x.rescaleRowNorms;
  
  matrixCutoff=x.matrixCutoff;      
  fixupRightHandSide=x.fixupRightHandSide;

  zeroRatio=x.zeroRatio;        
  fillinRatio=x.fillinRatio;    
  fillinRatio2=x.fillinRatio2;  
  harwellPivotingTolerance=x.harwellPivotingTolerance;
  nullVectorScaling=x.nullVectorScaling;

  preconditionBoundary=x.preconditionBoundary;
  preconditionRightHandSide=x.preconditionRightHandSide;
  maximumInterpolationWidth=x.maximumInterpolationWidth;
  iterativeImprovement=x.iterativeImprovement; 
  
  sorOmega=x.sorOmega;                           

  orderOfExtrapolation=x.orderOfExtrapolation;
  
  keepCoefficientGridFunction=x.keepCoefficientGridFunction;
  keepSparseMatrix=x.keepSparseMatrix;            
  removeSolutionAndRHSVector=x.removeSolutionAndRHSVector;  
  removeSparseMatrixFactorization=x.removeSparseMatrixFactorization; 

  if( x.ogmgParameters!=NULL )
  {
    if( ogmgParameters==NULL )
      ogmgParameters = new OgmgParameters();
    *ogmgParameters=*x.ogmgParameters;
  }
  else
  {
    if( ogmgParameters!=NULL )
    {
      delete ogmgParameters;
      ogmgParameters=NULL;
    }
  }

  petscOptions=x.petscOptions;

  //..Parameters for experimental DH preconditioner, used with PETSc **pf**
  incompleteLUDropTolerance      =x.incompleteLUDropTolerance; 
  incompleteLUSparseACoefficient =x.incompleteLUSparseACoefficient;
  incompleteLUSparseFCoefficient =x.incompleteLUSparseFCoefficient;
  incompleteLUTypeInDH           =x.incompleteLUTypeInDH;
  
  return *this;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{getSolverName}}
aString OgesParameters::
getSolverName() const                   
//==================================================================================
// /Description:
//  Return the name of the solver, a composite of the solver type, method and preconditioner.
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  aString name;
  name=getSolverTypeName();
  if( Communication_Manager::Number_Of_Processors > 0 )
  {
    // here is the name of the outer parallel solver:
    if( parallelSolverMethod!=preonly )
      name=name+", "+getSolverMethodName(parallelSolverMethod)+" (parallel)";
  }
  name=name+", "+getSolverMethodName();
  char buff[80];
  if( solver==SLAP || solver==PETSc || solver==PETScNew )
  {
    if( Communication_Manager::Number_Of_Processors > 0 )
    {
      name+", "+getPreconditionerName(parallelPreconditioner)+" (parallel)";
    }
    if( (solver==PETSc || solver==PETScNew) && preconditioner==incompleteLUPreconditioner )
    {
      name+=sPrintF(buff,", ILU(%i)",numberOfIncompleteLULevels);
    }
    else
    {
      name=name+", "+getPreconditionerName();
    }
  }
  if( solver==PETSc || solver==PETScNew )
    name=name+", "+getMatrixOrderingName();
  

  return name;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{getSolverTypeName}} 
aString OgesParameters::
getSolverTypeName(SolverEnum solverType /* = defaultSolver */ ) const
//==================================================================================
// /Description:
//   Return the name of the solverType such as "yale", "harwell", "SLAP", ...
// By default return the name of the currently chosen solver.
// /solverType (input) : return the name of this solver type.
//    By default return the name of the currently chosen solver.
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  if( solverType==defaultSolver )
    solverType=solver;
  aString name;
  switch (solverType)
  {
  case sor:
    name="sor";
    break;
  case yale:
    name="yale";
    break;
  case harwell:
    name="harwell";
    break;
  case SLAP:
    name="SLAP";
    break;
  case PETSc:
    name="PETSc";
    break;
  case PETScNew:
    name="PETScNew";
    break;
  case multigrid:
    name="multigrid";
    break;
  case userSolver1:
    name="userSolver1";
    break;
  case userSolver2:
    name="userSolver2";
    break;
  case userSolver3:
    name="userSolver3";
    break;
  case userSolver4:
    name="userSolver4";
    break;
  case userSolver5:
    name="userSolver5";
    break;
  default:
    name="unknown solver";
  }
  return name;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{getSolverMethodName}} 
aString OgesParameters::
getSolverMethodName(SolverMethodEnum solverMethodType /* = defaultSolverMethod */ ) const
//==================================================================================
// /Description:
//   Return the name of the solver method such as "gmres".
// By default return the name of the currently chosen method.
// /solverMethodType (input):
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  if( solverMethodType==defaultSolverMethod )
    solverMethodType=solverMethod;

  aString name;
  if( solver==yale )
  {
    name="direct sparse solver, no pivoting";
  }
  else if( solver==harwell )
  {
    name="direct sparse solver, partial pivoting";
  }
  else if( solver==sor )
  {
    name="iterative solver";
  }
  else if( solver==multigrid )
  {
    name="iterative solver";
  }
  else
  {
    switch (solverMethodType)
    {
    case richardson:
      name="richardson iteration";
      break;
    case chebychev:
      name="chebychev iteration";
      break;
    case conjugateGradient:
      name="conjugateGradient iteration";
      break;
    case biConjugateGradient:
      name="bi-conjugate gradient iteration";
      break;
    case conjugateGradientSquared:
      name="conjugate gradient squared iteration";
      break;
    case biConjugateGradientSquared:
      name="bi-conjugate gradient squared iteration";
      break;
    case biConjugateGradientStabilized:
      name="bi-conjugate gradient stabilized";
      break;
    case generalizedMinimalResidual:
      name="generalized minimal residual iteration";
      break;
    case transposeFreeQuasiMinimalResidual:
      name="transpose free quasi-minimal residual (tfqmr)";
      break;
    case transposeFreeQuasiMinimalResidual2:
      name="transpose free quasi-minimal residual (tcqmr)";
      break;
    case conjugateResidual:
      name="conjugate residual iteration";
      break;
    case leastSquares:
      name="leastSquares iteration";
      break;
    case preonly:
      name="preonly iteration";
      if( Communication_Manager::Number_Of_Processors>1 )
      {
	if( parallelExternalSolver==superlu  )
	  name="superlu";
	else if( parallelExternalSolver==superlu_mp  )
	  name="superlu_mp";
	else if( parallelExternalSolver==superlu_dist  )
	  name="superlu_dist";
	else if( parallelExternalSolver==mumps  )
	  name="mumps";
	else if( parallelExternalSolver==hypre  )
	  name="hypre";
      }
      
      break;
    case qcg:
      name="qcg iteration";
      break;
    default:
      name+="unknown method";
      break;
    }
  }
  return name;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{getPreconditionerName}} 
aString OgesParameters::
getPreconditionerName(PreconditionerEnum preconditionerType /*  = defaultPreconditioner */) const
//==================================================================================
// /Description:
//   Return the name of the preconditioner. By default return the name of the currently
// chosen preconditioner.
// /preconditionerType (input):
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  if( preconditionerType == defaultPreconditioner )
    preconditionerType=preconditioner;
  
  aString name;
  switch (preconditionerType)
  {
  case noPreconditioner:
    name="no preconditioner";
    break;
  case jacobiPreconditioner:
    name="jacobi preconditioner";
    break;
  case sorPreconditioner:
    name="sor preconditioner";
    break;
  case luPreconditioner:
    name="lu preconditioner";

    if( externalSolver==superlu  )
      name="superlu";
    else if( externalSolver==superlu_mp  )
      name="superlu_mp";
    else if( externalSolver==superlu_dist  )
      name="superlu_dist";
    else if( externalSolver==mumps  )
      name="mumps";
    else if( externalSolver==hypre  )
      name="hypre";

    break;
  case shellPreconditioner:
    name="shell preconditioner";
    break;
  case DHILUPreconditioner:
    name="experimental D.Hysom ILU preconditioner";
  case blockJacobiPreconditioner:
    name="block Jacobi preconditioner";
    break;
  case multigridPreconditioner:
    name="multigrid preconditioner";
    break;
  case eisenstatPreconditioner:
    name="eisenstat preconditioner";
    break;
  case incompleteCholeskyPreconditioner:
    name="incomplete Cholesky preconditioner";
    break;
  case incompleteLUPreconditioner:
    name="incomplete LU preconditioner";
    break;
  case additiveSchwarzPreconditioner:
    name="additive Schwarz preconditioner";
    break;
  case kspPreconditioner:
    name="ksp preconditioner";
    break;
  case compositePreconditioner:
    name="composite preconditioner";
    break;
  case redundantPreconditioner:
    name="redundant preconditioner";
    break;
  case diagonalPreconditioner:
    name="diagonal preconditioner";
    break;
  case ssorPreconditioner:
    name="ssor preconditioner";
    break;
  default:
    name="unknown preconditioner";
  }
  return name;
  
}

//\begin{>>OgesParametersInclude.tex}{\subsection{getMatrixOrderingName}} 
aString OgesParameters::
getMatrixOrderingName(MatrixOrderingEnum matrixOrderingType /* = defaultMatrixOrdering */) const
//==================================================================================
// /Description:
//   Return the name of the matrix ordering. By default return the name of the
// currently chosen matrix ordering.
// /matrixOrderingType (input) :
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  if( matrixOrderingType == defaultMatrixOrdering )
    matrixOrderingType =matrixOrdering;

  aString name;
  
  switch (matrixOrderingType)
  {
  case naturalOrdering:
    name="natural ordering";
    break;
  case nestedDisectionOrdering:
    name="nested disection ordering";
    break;
  case oneWayDisectionOrdering:
    name="oneWay disection ordering";
    break;
  case reverseCuthillMcKeeOrdering:
    name="reverse Cuthill-McKee ordering";
    break;
  case quotientMinimumDegreeOrdering:
    name="quotient minimum degree ordering";
    break;
  case rowlengthOrdering:
    name="rowlength ordering";
    break;
  default:
    name="unknown ordering";
  }
  return name;
}


//\begin{>>OgesParametersInclude.tex}{\subsection{set( OptionEnum , int )}} 
int OgesParameters::
set( OptionEnum option, int value /* = 0 */ )
//==================================================================================
// /Description:
//   Set an int option from the {\tt OptionEnum}.
// \begin{verbatim}
//   enum OptionEnum
//   {
//     THEabsoluteTolerance,
//     THEisAxisymmetric,       // for predefined equations
//     THEbestIterativeSolver,  // choose the 'best' iterative solver and options.
//     THEbestDirectSolver,     // choose the 'best' direct solver and options.
//     THEcompatibilityConstraint,
//     THEfillinRatio,
//     THEfillinRatio2,
//     THEfixupRightHandSide,
//     THEgmresRestartLength,
//     THEharwellPivotingTolerance,
//     THEincompleteLUExpectedFill,
//     THEiterativeImprovement,
//     THEkeepCoefficientGridFunction, // keep a reference to the user's coeff grid function
//     THEkeepSparseMatrix,            // keep ia,ja,a sparse matrix even it not needed by the solver
//     THEmatrixCutoff,
//     THEmatrixOrdering,
//     THEmaximumInterpolationWidth,
//     THEmaximumNumberOfIterations,
//     THEminimumNumberOfIterations,
//     THEnullVectorScaling,
//     THEnumberOfIncompleteLULevels,
//     THEsolveForTranspose,
//     THEpreconditioner,
//     THEparallelPreconditioner,
//     THEexternalSolver,
//     THEparallelExternalSolver,
//     THEremoveSolutionAndRHSVector,      // de-allocate sol and rhs vector after every solve
//     THEremoveSparseMatrixFactorization, // de-allocate any factorization info after every solve.
//     THErelativeTolerance,
//     THErescaleRowNorms,
//     THEsolverType,
//     THEsolverMethod,
//     THEparallelSolverMethod,
//     THEtolerance,
//     THEuserSuppliedCompatibilityConstraint,
//     THEzeroRatio
//   };
// \end{verbatim}
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  return set(option,value,(real)value);
}

//\begin{>>OgesParametersInclude.tex}{\subsection{set( OptionEnum , float )}} 
int OgesParameters::
set( OptionEnum option, float value )
//==================================================================================
// /Description:
//    Set a real valued option from the {\tt OptionEnum}.
// \begin{verbatim}
// \end{verbatim}
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  return set(option,(int)value,(real)value);
}

//\begin{>>OgesParametersInclude.tex}{\subsection{set( OptionEnum , double )}} 
int OgesParameters::
set( OptionEnum option, double value )
//==================================================================================
// /Description:
//    Set a real valued option from the {\tt OptionEnum}.
// \begin{verbatim}
// \end{verbatim}
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  return set(option,(int)value,(real)value);
}


int OgesParameters::
set( OptionEnum option, int value, real rvalue )
//==================================================================================
//  /Description:
//    generic set for int and real valued options.
//==================================================================================
{
  switch (option) 
  {
  case THEabsoluteTolerance:
    absoluteTolerance=rvalue;
    break;
  case THEisAxisymmetric:
    isAxisymmetric=value;
    break;
  case THEbestIterativeSolver:  // choose the 'best' iterative solver and options.
   #ifndef USE_PPP
    if( isAvailable(PETSc) )
    {
      solver=PETSc;
      solverMethod=biConjugateGradientStabilized;
      preconditioner=incompleteLUPreconditioner;
      numberOfIncompleteLULevels=1;    
    }
   #else
    if( isAvailable(PETScNew) )
    { // use the new PETSc solver in parallel
      solver=PETScNew;
      solverMethod=preonly; // biConjugateGradientStabilized;
      parallelSolverMethod=biConjugateGradientStabilized;

      preconditioner=incompleteLUPreconditioner;
      parallelPreconditioner=blockJacobiPreconditioner,
      numberOfIncompleteLULevels=1;    
    }
   #endif
    else
    {
      solver=SLAP;
      solverMethod=gmres;
      preconditioner=incompleteLUPreconditioner;
      numberOfIncompleteLULevels=0;  // only zero available ?
    }
    break;
  case THEbestDirectSolver:
    solver=yale;
    break;
  case THEsolverType:
    set( (SolverEnum)value );
    break;
  case THEsolverMethod:
    set( (SolverMethodEnum)value );
    break;
  case THEparallelSolverMethod:
    parallelSolverMethod=(SolverMethodEnum)value;
    break;
  case THEmatrixOrdering:
    set( (MatrixOrderingEnum)value );
    break;
  case THEpreconditioner:
    set( (PreconditionerEnum)value );
    break;
  case THEparallelPreconditioner:
    parallelPreconditioner=(PreconditionerEnum)value;
    break;
  case THEexternalSolver:
    externalSolver=(ExternalSolverEnum)value;
    break;
  case THEparallelExternalSolver:
    parallelExternalSolver=(ExternalSolverEnum)value;
    break;
  case THEmaximumNumberOfIterations:
    maximumNumberOfIterations=value;
    break;
  case THEminimumNumberOfIterations:
    minimumNumberOfIterations=value;
    break;
  case THEgmresRestartLength:
    gmresRestartLength=value;
    break;
  case THEnumberOfIncompleteLULevels:
    numberOfIncompleteLULevels=value;
    break;
  case THEincompleteLUExpectedFill:
    incompleteLUExpectedFill=rvalue;
    break;
  case THEincompleteLUDropTolerance:       // =dt for ILUT(dt), ILUTK(dt,k) (from DH preconditioners)
    incompleteLUDropTolerance=rvalue;
    break;
  case THEincompleteLUSparseACoefficient:  // =sparseA for sparsifying matrix prior to ILU (from DH)
    incompleteLUSparseACoefficient=rvalue;
    break;
  case THEincompleteLUSparseFCoefficient:  // =sparseF for sparsifying factorization after ILU (from DH)
    incompleteLUSparseFCoefficient=rvalue;
    break;
  case THEincompleteLUTypeInDH:            // =ilu_type in DH preconditioners
    incompleteLUTypeInDH=value;
    break;
  case THEharwellPivotingTolerance:
    harwellPivotingTolerance=rvalue;
    break;
  case THEzeroRatio:
    zeroRatio=rvalue;
    break;
  case THEfillinRatio:
    fillinRatio=rvalue;
    break;
  case THEfillinRatio2:
    fillinRatio2=rvalue;
    break;
  case THEnullVectorScaling:
    nullVectorScaling=rvalue;
    break;
  case THEsolveForTranspose:
    solveForTranspose=value;
    break;
  case THEmatrixCutoff:
    matrixCutoff=rvalue;
    break;
  case THEfixupRightHandSide:
    fixupRightHandSide=value;
    break;
  case THEcompatibilityConstraint:
    compatibilityConstraint=value;
    break;
  case THEuserSuppliedCompatibilityConstraint:
    userSuppliedCompatibilityConstraint=value;
    break;
  case THEmaximumInterpolationWidth:
    maximumInterpolationWidth=value;
    break;
  case THEiterativeImprovement:
    iterativeImprovement=value;
    break;
  case THEorderOfExtrapolation:
    orderOfExtrapolation=value;
    break;
  case THErescaleRowNorms:
    rescaleRowNorms=value;
    break;
  case THEtolerance:
    relativeTolerance=rvalue;
    printF("OgesParameters: relativeTolerance=%e \n",relativeTolerance);
    break;
  case THErelativeTolerance:
    relativeTolerance=rvalue;
    printF("OgesParameters: relativeTolerance=%e \n",relativeTolerance);
    break;
  case THEkeepCoefficientGridFunction: // keep a reference to the user's coeff grid function
    keepCoefficientGridFunction=value;
    break;
  case THEkeepSparseMatrix:         // keep ia,ja,a sparse matrix even it not needed by the solver
    keepSparseMatrix=value;
    break;
  case THEremoveSolutionAndRHSVector:      // de-allocate sol and rhs vector after every solve
    removeSolutionAndRHSVector=value;
    break;
  case THEremoveSparseMatrixFactorization: // de-allocate any factorization info after every solve.
    removeSparseMatrixFactorization=value;
    break;
  default:
    printF("OgesParameters::set: Unknown option=%i! This should not happen\n",option);
    OV_ABORT("error");
  }
  return 0;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{set( SolverEnum )}} 
int OgesParameters::
set( SolverEnum option )
//==================================================================================
// /Description:
//   Set the solver, a value from the {\tt SolverEnum}.
// \begin{verbatim}
//   enum SolverEnum
//   {
//     defaultSolver,
//     sor,
//     yale,
//     harwell,
//     SLAP,
//     PETSc,
//     multigrid,
//     PETScNew,
//     userSolver1, // these are reserved for new user defined solvers.
//     userSolver2,
//     userSolver3,
//     userSolver4,
//     userSolver5
//  };
// \end{verbatim}
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  solver=option;
  if( solver==defaultSolver )
    solver = yale;
    
  solverName=getSolverTypeName();
  
  return 0;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{set( SolverMethodEnum  )}} 
int OgesParameters::
set( SolverMethodEnum option )
//==================================================================================
// /Description:
//   Set the solver method, a value from the {\tt SolverMethodEnum}.
// \begin{verbatim}
//   enum SolverMethodEnum
//   {
//     richardson,
//     chebychev,
//     conjugateGradient,
//       cg=conjugateGradient,           // cg= short PETSc name
//     biConjugateGradient,
//       bicg=biConjugateGradient,
//     conjugateGradientSquared,
//       cgs=conjugateGradientSquared,
//     biConjugateGradientSquared,    
//     biConjugateGradientStabilized,    
//       bcgs=biConjugateGradientStabilized,
//     generalizedMinimalResidual,
//       gmres=generalizedMinimalResidual,
//     transposeFreeQuasiMinimalResidual, 
//       tfqmr=transposeFreeQuasiMinimalResidual,
//     transposeFreeQuasiMinimalResidual2,         // tcqmr Tony Chan's version
//       tcqmr=transposeFreeQuasiMinimalResidual,
//     conjugateResidual,
//       cr=conjugateResidual,
//     leastSquares,
//       lsqr=leastSquares,
//     preonly,
//   };
// \end{verbatim}
//\end{OgesParametersInclude.tex} 
//==================================================================================
  //....Solver options (from PETSc, see the documentation, Chap. 4)
  // KSP Options:
  //   KSPRICHARDSON   = Richardson iter.
  //   KSPCHEBYSHEV    = Chebyshev iter.
  //   KSPCG           = Conj. Gradients (only for SPD systems)
  //   KSPGMRES        = Restarted GMRes
  //   KSPTCQMR        = Transp. Free QMR (T.F.Chan version); SLOW!
  //   KSPBCGS         = BiConj. Gradient method
  //   KSPCGS          = Conj. Gradient Squared
  //   KSPTFQMR        = Transp. Free QMR (Freund's version): Fast, _default_
  //   KSPCR           = Conj. Resid.
  //   KSPPREONLY      = Use only the preconditioner
{
  solverMethod=option;
  return 0;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{set( PreconditionerEnum )}} 
int OgesParameters::
set( PreconditionerEnum option )
//==================================================================================
// /Description:
//   Set the preconditioner, a value from the {\tt PreconditionerEnum}.
// \begin{verbatim}
//   enum PreconditionerEnum
//   {
//     noPreconditioner,
//     jacobiPreconditioner,
//     sorPreconditioner,
//     luPreconditioner,
//     shellPreconditioner,
//     blockJacobiPreconditioner,
//     multigridPreconditioner,
//     eisenstatPreconditioner,
//     incompleteCholeskyPreconditioner,
//     incompleteLUPreconditioner,
//     additiveSchwarzPreconditioner,
//     kspPreconditioner,
//     compositePreconditioner,
//     redundantPreconditioner,
//     diagonalPreconditioner,
//     ssorPreconditioner
//   };
// \end{verbatim}
//\end{OgesParametersInclude.tex} 
//==================================================================================
  // PC Options:
  //   PCJACOBI        = Jacobi
  //   PCBJACOBI       = Block Jacobi
  //   PCBGS           = Block Gauss-Seidel
  //   PCSOR           = SOR
  //   PCEISENSTAT     = SOR with Eisenstat trick
  //   PCICC           = Incomplete Cholesky (for Symmetric only)
  //   PCILU           = Incomplete LU, ...................... _default_
  //   PCASM           = Additive Schwarz (does it work??)
  //   PCNONE          = No preconditioner
  //  ( and others, you could set your own PC, see the docs.)
{
  preconditioner=option;
  return 0;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{set( MatrixOrderingEnum )}} 
int OgesParameters::
set( MatrixOrderingEnum option )
//==================================================================================
// /Description:
//   Set the matrix ordering, a value from the {\tt MatrixOrderingEnum}.
// \begin{verbatim}
//   enum MatrixOrderingEnum
//   {
//     naturalOrdering,
//     nestedDisectionOrdering,
//     oneWayDisectionOrdering,
//     reverseCuthillMcKeeOrdering,
//     quotientMinimumDegreeOrdering,
//     rowlengthOrdering
//    };
// \end{verbatim}
//\end{OgesParametersInclude.tex} 
//==================================================================================
  // Matrix Reorderings
  //   ORDER_NATURAL  = no reorder
  //   ORDER_ND       = Nested Dissection
  //   ORDER_1WD      = One-way Dissection
  //   ORDER_RCM      = Reverse Cuthill-McKee
  //   ORDER_QMD etc  = Quotient Minimum Degree
  //
{
  matrixOrdering=option;
  return 0;
}


//\begin{>>OgesParametersInclude.tex}{\subsection{setPetscOption}}
int OgesParameters::
setPetscOption( const aString & name, const aString & value )
//==================================================================================
// /Description:
//   Set a PETSc option: example: name="-ksp\_type" value="bcgs"
// /name (input) : the name of a Petsc option, e.g. name="-ksp\_type"
// /value (input) : the value (as a string) of the petsc option, e.g. value="bcgs" or value="1.0" 
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  petscOptions.push_back(ShowFileParameter(name,value));
  return 0;
}


//\begin{>>OgesParametersInclude.tex}{\subsection{setPetscOption}}
bool OgesParameters::
getPetscOption( const aString & name, aString & value ) const
//==================================================================================
// /Description:
//   Get a PETSc option (if it exists): example: name="-ksp\_type" value="bcgs"
// /name (input) : the name of a Petsc option, e.g. name="-ksp\_type"
// /value (output) :the value (as a string) of the petsc option, e.g. value="bcgs" or value="1.0" 
// /return value; true if found, false if not found
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  return petscOptions.getParameter(name,value);
}

//\begin{>>OgesParametersInclude.tex}{\subsection{getSolverType}}
OgesParameters::SolverEnum OgesParameters::
getSolverType() const
//==================================================================================
// /Description:
//   Return the solverType.
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  return solver;
}


//\begin{>>OgesParametersInclude.tex}{\subsection{get( OptionEnum , int \& )}} 
int OgesParameters::
get( OptionEnum option, int & value ) const
//==================================================================================
// /Description:
//  Get the value of an `int' valued option.
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  real rvalue=0.;
  return get(option,value,rvalue);
}

//\begin{>>OgesParametersInclude.tex}{\subsection{get( OptionEnum , real \&  )}} 
int OgesParameters::
get( OptionEnum option, real & value ) const
//==================================================================================
// /Description:
//  Get the value of an `real' valued option.
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  int ivalue=0;
  return get(option,ivalue,value);
}

int OgesParameters::
get( OptionEnum option, int & value, real & rvalue ) const
//==================================================================================
// /Description:
//   Generic get.
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  switch (option) 
  {
  case THEabsoluteTolerance:
    rvalue=absoluteTolerance;
    break;
  case THEisAxisymmetric:
    value=isAxisymmetric;
    break;
  case THEsolverType:
    value=solver;
    break;
  case THEsolverMethod:
    value=solverMethod;
    break;
  case THEparallelSolverMethod:
    value=parallelSolverMethod; 
    break;
  case THEmatrixOrdering:
    value=matrixOrdering;
    break;
  case THEpreconditioner:
    value=preconditioner;
    break;
  case THEparallelPreconditioner:
    value=parallelPreconditioner;
    break;
  case THEexternalSolver:
    value=externalSolver; 
    break;
  case THEparallelExternalSolver:
    value=parallelExternalSolver;
    break;
  case THEmaximumNumberOfIterations:
    value=maximumNumberOfIterations;
    break;
  case THEminimumNumberOfIterations:
    value=minimumNumberOfIterations;
    break;
  case THEgmresRestartLength:
    value=gmresRestartLength;
    break;
  case THEnumberOfIncompleteLULevels:
    value=numberOfIncompleteLULevels;
    break;
  case THEincompleteLUExpectedFill:
    rvalue=incompleteLUExpectedFill;
    break;
  case THEharwellPivotingTolerance:
    rvalue=harwellPivotingTolerance;
    break;
  case THEzeroRatio:
    rvalue=zeroRatio;
    break;
  case THEfillinRatio:
    rvalue=fillinRatio;
    break;
  case THEfillinRatio2:
    rvalue=fillinRatio2;
    break;
  case THEnullVectorScaling:
    rvalue=nullVectorScaling;
    break;
  case THEsolveForTranspose:
    value=solveForTranspose;
    break;
  case THEmatrixCutoff:
    rvalue=matrixCutoff;
    break;
  case THEfixupRightHandSide:
    value=fixupRightHandSide;
    break;
  case THEcompatibilityConstraint:
    value=compatibilityConstraint;
    break;
  case THEuserSuppliedCompatibilityConstraint:
    value=userSuppliedCompatibilityConstraint;
    break;
  case THEmaximumInterpolationWidth:
    value=maximumInterpolationWidth;
    break;
  case THEiterativeImprovement:
    value=iterativeImprovement;
    break;
  case THEorderOfExtrapolation:
    value=orderOfExtrapolation;
    break;
  case THErescaleRowNorms:
    value=rescaleRowNorms;
    break;
  case THEtolerance:
    rvalue=relativeTolerance;
    break;
  case THErelativeTolerance:
    rvalue=relativeTolerance;
    break;
  case THEkeepCoefficientGridFunction: // keep a reference to the user's coeff grid function
    value=keepCoefficientGridFunction;
    break;
  case THEkeepSparseMatrix:         // keep ia,ja,a sparse matrix even it not needed by the solver
    value=keepSparseMatrix;
    break;
  case THEremoveSolutionAndRHSVector:      // de-allocate sol and rhs vector after every solve
    value=removeSolutionAndRHSVector;
    break;
  case THEremoveSparseMatrixFactorization: // de-allocate any factorization info after every solve.
    value=removeSparseMatrixFactorization;
    break;
  default:
    printf("OgesParameters::set: Unknown option=%i! This should not happen\n",option);
    Overture::abort("error");
  }
  return 0;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{getOgmgParameters}} 
OgmgParameters* OgesParameters::
getOgmgParameters() const
//==================================================================================
// /Description:
//   Return a pointer to the OgmgParameters object. This pointer may be NULL.
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  return ogmgParameters;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{buildOgmgParameters}}
OgmgParameters& OgesParameters::
buildOgmgParameters()
//==================================================================================
// /Description:
//   Create the OgmgParameters object if it is not there; return a reference to the object.
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  if( ogmgParameters==NULL )
    ogmgParameters = new OgmgParameters;
  return *ogmgParameters;
}



// This next macro can be used to get or put stuff to the dataBase.
#define GET_PUT_LIST(getPut)  \
  subDir.getPut(solveForTranspose,"solveForTranspose" );  \
  subDir.getPut(zeroRatio,"zeroRatio" );  \
  subDir.getPut(fillinRatio,"fillinRatio" );  \
  subDir.getPut(fillinRatio2,"fillinRatio2" );  \



//\begin{>>OgesParametersInclude.tex}{\subsection{get from a data base}} 
int OgesParameters::
get( const GenericDataBase & dir, const aString & name)
//==================================================================================
// /Description:
//   Get a copy of the OgesParameters from a database file
// /dir (input): get from this directory of the database.
// /name (input): the name of Oges on the database.
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"OgesParameters");

  subDir.setMode(GenericDataBase::streamInputMode);

  GET_PUT_LIST(get);

  return 0;
}

//\begin{>>OgesParametersInclude.tex}{\subsection{put to a data base}} 
int OgesParameters::
put( GenericDataBase & dir, const aString & name) const   
//==================================================================================
// /Description:
//   Output an image of OgesParameters to a data base. 
// /dir (input): put onto this directory of the database.
// /name (input): the name of Oges on the database.
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Oges");                      // create a sub-directory 

  subDir.setMode(GenericDataBase::streamOutputMode);

  GET_PUT_LIST(put);
  
  return 0;
}


//\begin{>>OgesParametersInclude.tex}{\subsection{display}} 
int OgesParameters::
display(FILE *file /* = stdout */ )
//==================================================================================
// /Description:
//   Print out current values of parameters
// /file (input) : print to this file (standard output by default).
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  fprintf(file,"name = %s\n",(const char*)getSolverName());

  fprintf(file,"solverType = %s\n",(const char*)getSolverTypeName());
  fprintf(file,"solverMethod = %s\n",(const char*)getSolverMethodName());
  fprintf(file,"parallel solverMethod = %s\n",(const char*)getSolverMethodName(parallelSolverMethod));
  fprintf(file,"preconditioner = %s\n",(const char*)getPreconditionerName());
  fprintf(file,"parallelPreconditioner = %s\n",(const char*)getPreconditionerName(parallelPreconditioner));
  fprintf(file,"matrix ordering = %s\n",(const char*)getMatrixOrderingName());

  fprintf(file,"maximumNumberOfIterations = %i\n",maximumNumberOfIterations);
  fprintf(file,"minimumNumberOfIterations = %i\n",minimumNumberOfIterations);
  
  fprintf(file,"relativeTolerance = %8.2e\n",relativeTolerance);
  fprintf(file,"absoluteTolerance = %8.2e\n",absoluteTolerance);

  fprintf(file,"maximumAllowableIncreaseInResidual = %8.2e\n",maximumAllowableIncreaseInResidual);

  fprintf(file,"compatibilityConstraint = %i\n",compatibilityConstraint);
  fprintf(file,"gmresRestartLength = %i\n",gmresRestartLength);
  fprintf(file,"numberOfIncompleteLULevels = %i\n",numberOfIncompleteLULevels);
  fprintf(file,"incompleteLUExpectedFill = %i\n",gmresRestartLength);
  fprintf(file,"minimumNumberOfIterations = %i\n",gmresRestartLength);
  fprintf(file,"maximumNumberOfIterations = %i\n",gmresRestartLength);
  fprintf(file,"solveForTranspose = %i\n",gmresRestartLength);
  fprintf(file,"rescaleRowNorms = %i\n",rescaleRowNorms);

  fprintf(file,"matrixCutoff= %8.2e\n",matrixCutoff);     
  fprintf(file,"fixupRightHandSide= %i\n",fixupRightHandSide);

  fprintf(file,"zeroRatio    = %8.2e\n",zeroRatio);
  fprintf(file,"fillinRatio  = %8.2e\n",fillinRatio);
  fprintf(file,"fillinRatio2 = %8.2e\n",fillinRatio2);
  fprintf(file,"harwellPivotingTolerance = %8.2e\n",harwellPivotingTolerance);
  fprintf(file,"nullVectorScaling = %8.2e\n",nullVectorScaling);
  fprintf(file,"maximumInterpolationWidth = %i\n",maximumInterpolationWidth);
  fprintf(file,"iterativeImprovement = %i\n",iterativeImprovement);
  fprintf(file,"sorOmega = %8.2e\n",sorOmega);

  fprintf(file,"userSuppliedCompatibilityConstraint = %i\n",userSuppliedCompatibilityConstraint);

//   int preconditionBoundary;
//   int preconditionRightHandSide;

  return 0;
}



//\begin{>>OgesParametersInclude.tex}{\subsection{update}}
int OgesParameters::
update( GenericGraphicsInterface & gi, CompositeGrid & cGrid )
// =====================================================================================
// /Description:
//   Update parameters interactively.
// /gi: use this graphics interface.
// /cg: parameters will apply to this grid.
//\end{OgesParametersInclude.tex} 
//==================================================================================
{
  const int myid=max(0,Communication_Manager::My_Process_Number);
  
  char buff[180];  // buffer for sprintf
  aString answer,line; 

  aString menu[] = 
    {
      "!Oges parameters",
      "choose best direct solver",
      "choose best iterative solver",
      ">solver type",
        "yale",
        "harwell",
        "SLAP",
        "PETSc",
        "multigrid",
        "PETScNew",
        "userSolver1",
        "userSolver2",
        "userSolver3",
        "userSolver4",
        "userSolver5",
      "<>Yale parameters",
        "matrix cutoff",
        "zero ratio",
        "fillin ratio",
        "fillin ratio2",
      "<>Harwell parameters",
        "pivoting tolerance",
        "matrix cutoff",
        "zero ratio",
        "fillin ratio",
        "fillin ratio2",
      "<>SLAP parameters",
        ">method",
          "sor",
          "conjugate gradient",
          "gmres",
          "bi-conjugate gradient squared",
        "<>preconditioner",
          "no preconditioner",
          "incomplete LU preconditioner",
          "diagonal preconditioner",
          "ssor preconditioner",
        "<number of GMRES vectors",
        "relative tolerance",
        "absolute tolerance",
        "maximum number of iterations",
        "minimum number of iterations",
        "omega for sor",
      "<>PETSc parameters",
        "define petscOption <option> <value>",
        ">method",
          "richardson",
          "chebychev",
          "conjugate gradient",
          "bi-conjugate gradient",
          "conjugate gradient squared",
          "bi-conjugate gradient squared",
          "bi-conjugate gradient stabilized",
          "generalized minimal residual",
          "transpose free quasi-minimal residual (tfqmr)",
          "transpose free quasi-minimal residual (tcqmr)",
          "conjugate residual",
          "least squares",
          "preonly",
          "qcg",
        "<>parallel method",
          "parallel richardson",
          "parallel chebychev",
          "parallel conjugate gradient",
          "parallel bi-conjugate gradient",
          "parallel conjugate gradient squared",
          "parallel bi-conjugate gradient squared",
          "parallel bi-conjugate gradient stabilized",
          "parallel generalized minimal residual",
          "parallel transpose free quasi-minimal residual (tfqmr)",
          "parallel transpose free quasi-minimal residual (tcqmr)",
          "parallel conjugate residual",
          "parallel least squares",
          "parallel preonly",
          "parallel qcg",
        "<>preconditioner",
          "no preconditioner",
          "jacobi preconditioner",
          "sor preconditioner",
          "lu preconditioner",
          "shell preconditioner",
          "block jacobi preconditioner",
          "multigrid preconditioner",
          "eisenstat preconditioner",
          "incomplete Cholesky preconditioner",
          "incomplete LU preconditioner",
          "additive Schwarz preconditioner",
          "ksp preconditioner",
          "composite preconditioner",
          "redundant preconditioner",
        "<>parallel preconditioner",
          "parallel block jacobi preconditioner",
          "parallel additive Schwarz preconditioner",
         "<>DH preconditioner",
           "use DH ILU preconditioner (experimental)",
           "drop tolerance for ILUT(dt)",
           "matrix sparsification (sparseA)",
           "factorization sparsification (sparseF)",
           "DH ILU type (ilu_type)",
        "<>matrix ordering",
          "natural ordering",
          "nested disection ordering",
          "oneWay disection ordering",
          "reverse Cuthill McKee ordering",
          "quotient minimum degree ordering",
          "rowlength ordering",
        "<>external solvers",
          "superlu",
          "superlu_mp",
        "<>parallel external solvers",
          "parallel superlu_dist",
          "parallel mumps",
          "parallel hypre",
        "<>convergence parameters",
          "relative tolerance",
          "absolute tolerance",
          "maximum allowable increase in the residual",
        "<gmres restart length",
        "relative tolerance",
        "absolute tolerance",
        "maximum number of iterations",
        "minimum number of iterations",
        "maximum allowable residual",
        "number of incomplete LU levels",
        "incomplete LU expected fill",
      "<multigrid parameters",
      ">other options",
        "debug",
        "null vector scaling",
        "turn on the compatibility constraint",
        "turn off the compatibility constraint",
        "solve for transpose",
        "do not solve for transpose",
        "use iterative improvement",
        "do not use iterative improvement",
        "block size",
        "scale rows",
        "do not scale rows",
//        "precondition boundary",
//        "precondition right hand side",
//        "transpose",
      "<help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "choose best direct solver : make a guess",
      "choose best iterative solver : make a guess",
      "solver type",
      "  yale : direct sparse solver, no pivoting",
      "  harwell : direct sparse solver, partial pivoting",
      "  SLAP : a package of iterative solvers",
      "  PETSc : a package of iterative solvers",
      "<>Yale parameters",
        "matrix cutoff",
        "zero ratio",
        "fillin ratio",
        "fillin ratio2",
      "<>Harwell parameters",
        "pivoting tolerance",
        "matrix cutoff",
        "zero ratio",
        "fillin ratio",
        "fillin ratio2",
      "<>SLAP parameters",
        ">method",
          "sor",
          "conjugate gradient",
          "gmres",
          "bi-conjugate gradient",
          "bi-conjugate gradient squared",
        "<>preconditioner",
          "no preconditioner",
          "incomplete LU preconditioner",
          "diagonal preconditioner",
          "ssor preconditioner",
        "<number of GMRES vectors",
        "relative tolerance",
        "absolute tolerance",
        "maximum number of iterations",
        "minimum number of iterations",
        "omega for sor",
      "<>PETsc parameters",
        ">method",
          "richardson",
          "chebychev",
          "conjugate gradient",
          "bi-conjugate gradient",
          "conjugate gradient squared",
          "bi-conjugate gradient squared",
          "bi-conjugate gradient stabilized",
          "generalized minimal residual",
          "transpose free quasi-minimal residual (tfqmr)",
          "transpose free quasi-minimal residual (tcqmr)",
          "conjugate residual",
          "least squares",
          "preonly",
          "qcg",
        "<>preconditioner",
          "no preconditioner",
          "jacobi preconditioner",
          "sor preconditioner",
          "lu preconditioner",
          "shell preconditioner",
          "block jacobi preconditioner",
          "multigrid preconditioner",
          "eisenstat preconditioner",
          "incomplete Cholesky preconditioner",
          "incomplete LU preconditioner",
          "additive Schwarz preconditioner",
          "ksp preconditioner",
          "composite preconditioner",
          "redundant preconditioner",
        "<>matrix ordering",
          "natural ordering",
          "nested disection ordering",
          "oneWay disection ordering",
          "reverse Cuthill McKee ordering",
          "quotient minimum degree ordering",
          "rowlength ordering",
        "<>convergence parameters",
          "relative tolerance",
          "absolute tolerance",
          "maximum allowable increase in the residual",
        "<gmres restart length",
        "relative tolerance",
        "absolute tolerance",
        "maximum number of iterations",
        "minimum number of iterations",
        "maximum allowable residual",
        "number of incomplete LU levels",
        "incomplete LU expected fill",
      "<multigrid parameters",
      ">other options",
        "debug",
        "null vector scaling",
        "turn on the compatibility constraint",
        "turn off the compatibility constraint",
        "solve for transpose",
        "do not solve for transpose",
        "use iterative improvement",
        "do not use iterative improvement",
        "block size",
        "scale rows : scale the rows in the matrix",
        "do not scale rows : do not scale the rows in the matrix",
      "<help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  bool isDHPreconditionerAvailable=false;
#ifdef USE_DH_PRECONDITIONER
  isDHPreconditionerAvailable = true;
#endif

  gi.appendToTheDefaultPrompt("Oges>"); // set the default prompt
  int len=0;
  
  for( int it=0;; it++ )
  {
    gi.getMenuItem(menu,answer);
 
    if( answer=="choose best direct solver" )
    {
      set(THEbestDirectSolver);
      printF("Choosing: %s\n",(const char*)getSolverName());
    }
    else if( answer=="choose best iterative solver" )
    {
      set(THEbestIterativeSolver);
      printF("Choosing: %s\n",(const char*)getSolverName());
    }
    else if( answer=="yale" )
    {
      solver=yale;
      printF("solver type = yale\n");
    }
    else if( answer=="harwell" )
    {
      solver=harwell;
      printF("solver type = harwell\n");
    }
    else if( answer=="SLAP" || answer=="slap" || answer=="Slap" )
    {
      solver=SLAP;
      printF("solver type = SLAP\n");
    }
    else if( answer=="PETScNew" || answer=="petscnew" || answer=="PetscNew" )
    {
      solver=PETScNew;
      printF("solver type = PETScNew\n");
    }
    else if( answer=="PETSc" || answer=="petsc" || answer=="Petsc" )
    {
      solver=PETSc;
      printF("solver type = PETSc\n");
    }
    else if( answer=="multigrid" )
    {
      solver=multigrid;
      printF("solver type = multigrid\n");
    }
    else if( answer=="multigrid parameters" )
    {
      // printF("multigrid parameters: to finish...\n");
      if( ogmgParameters==NULL )
 	ogmgParameters = new OgmgParameters;
      ogmgParameters->update(gi,cGrid);
    }
    // --- "Yale parameters" ----
    else if( answer=="matrix cutoff" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the matrix cutoff (default=%e) \n",matrixCutoff));
      if( line!="" )
      {
        sScanF(line,"%e",&matrixCutoff);
        printF("matrix cutoff = %e\n",matrixCutoff);
      } 
    }
    else if( answer=="zero ratio" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the zero ratio (default=%e) \n",zeroRatio));
      if( line!="" )
      {
        sScanF(line,"%e",&zeroRatio);
        printF("zero ratio = %e\n",zeroRatio);
      } 
    }
    else if( answer=="fillin ratio" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the fillin ratio (default=%e) \n",fillinRatio));
      if( line!="" )
      {
        sScanF(line,"%e",&fillinRatio);
        printF("fillin ratio = %e\n",fillinRatio);
      } 
    }
    else if( answer=="fillin ratio2" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the fillin ratio (default=%e) \n",fillinRatio2));
      if( line!="" )
      {
        sScanF(line,"%e",&fillinRatio2);
        printF("fillin ratio = %e\n",fillinRatio2);
      } 
    }
    // ----Harwell parameters-----
    else if( answer=="pivoting tolerance" )
    {
    }
    // else if( answer=="matrix cutoff" )
    // else if( answer=="zero ratio" )
    // else if( answer=="fillin ratio" )
    // else if( answer=="fillin ratio2" )
    // ----- SLAP parameters ----
    //   ---method
    else if( answer=="sor" )
    {
      set(THEsolverType,sor); 
    }
    else if( answer=="conjugate gradient" )
    {
      set(THEsolverMethod,conjugateGradient);
    }
    else if( answer=="gmres" )
    {
      set(THEsolverMethod,gmres);
    }
    else if( answer=="bi-conjugate gradient" )
    {
      set(THEsolverMethod,biConjugateGradient);
    }
    else if( answer=="bi-conjugate gradient squared" )
    {
      set(THEsolverMethod,biConjugateGradientSquared);
    }
    //    ---preconditioner
    else if( answer=="no preconditioner" )
    {
      set(THEpreconditioner,noPreconditioner);
    }
    else if( answer=="incomplete LU preconditioner" )
    {
      set(THEpreconditioner,incompleteLUPreconditioner);
    }
    else if( answer=="diagonal preconditioner" )
    {
      set(THEpreconditioner,diagonalPreconditioner);
    }
    else if( answer=="ssor preconditioner" )
    {
      set(THEpreconditioner,ssorPreconditioner);
    }
    else if( answer=="number of GMRES vectors" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the GMRES restart length (default=%i)",gmresRestartLength));
      if( line!="" )
        sScanF(line,"%i",&gmresRestartLength);
    }
    else if( answer=="relative tolerance" || answer=="tolerance" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the relative tolerance (default=%e) ",relativeTolerance));
      if( line!="" )
        sScanF(line,"%e",&relativeTolerance);
    }
    else if( answer=="absolute tolerance" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the absolute tolerance (default=%e) ",absoluteTolerance));
      if( line!="" )
        sScanF(line,"%e",&absoluteTolerance);
    }
    else if( answer=="maximum number of iterations" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the maximum number of iterations (default=%i) ",
              maximumNumberOfIterations));
      if( line!="" )
        sScanF(line,"%i",&maximumNumberOfIterations);
    }
    else if( answer=="minimum number of iterations" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the minimum number of iterations (default=%i) ",
               minimumNumberOfIterations));
      if( line!="" )
        sScanF(line,"%i",&minimumNumberOfIterations);
    }
    else if( answer=="omega for sor" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the omega for sor (default=%e) ",sorOmega));
      if( line!="" )
        sScanF(line,"%e",&sorOmega);
    }
    // ------PETsc parameters-------
    //    ----method
    else if( answer=="richardson" )
    {
      set(THEsolverMethod,richardson); 
    }
    else if( answer=="chebychev" )
    {
      set(THEsolverMethod,chebychev);
    }
    else if( answer=="conjugate gradient" || answer=="cg" )
    {
      set(THEsolverMethod,conjugateGradient);
    }
    else if( answer=="conjugate gradient squared" || answer=="cgs" )
    {
      set(THEsolverMethod,conjugateGradientSquared);
    }
    else if( answer=="bi-conjugate gradient" || answer=="bicg" )
    {
      set(THEsolverMethod,biConjugateGradient);
    }
    else if( answer=="bi-conjugate gradient squared" )
    {
      set(THEsolverMethod,biConjugateGradientSquared);
    }
    else if( answer=="bi-conjugate gradient stabilized" || answer=="bcgs" )
    {
      solverMethod=biConjugateGradientStabilized;
    }
    else if( answer=="generalized minimal residual" || answer=="gmres" )
    {
      set(THEsolverMethod,gmres);
    }
    else if( answer=="transpose free quasi-minimal residual (tcqmr)" || answer=="tcqmr" )
    {
      set(THEsolverMethod,tcqmr);
    }
    else if( answer=="transpose free quasi-minimal residual (tfqmr)" || answer=="tfqmr" )
    {
      set(THEsolverMethod,tfqmr);
    }
    else if( answer=="conjugate residual" )
    {
      set(THEsolverMethod,conjugateResidual);
    }
    else if( answer=="least squares" )
    {
      set(THEsolverMethod,leastSquares);
    }
    else if( answer=="preonly" )
    {
      set(THEsolverMethod,preonly);
    }
    else if( answer=="qcg" )
    {
      set(THEsolverMethod,qcg);
    }

    //    ----parallel method
    else if( answer=="parallel richardson" )
    {
      parallelSolverMethod=richardson; 
    }
    else if( answer=="parallel chebychev" )
    {
      parallelSolverMethod=chebychev;
    }
    else if( answer=="parallel conjugate gradient" || answer=="parallel cg" )
    {
      parallelSolverMethod=conjugateGradient;
    }
    else if( answer=="parallel conjugate gradient squared" || answer=="parallel cgs" )
    {
      parallelSolverMethod=conjugateGradientSquared;
    }
    else if( answer=="parallel bi-conjugate gradient" || answer=="parallel bicg" )
    {
      parallelSolverMethod=biConjugateGradient;
    }
    else if( answer=="parallel bi-conjugate gradient squared" )
    {
      parallelSolverMethod=biConjugateGradientSquared;
    }
    else if( answer=="parallel bi-conjugate gradient stabilized" || answer=="parallel bcgs" )
    {
      parallelSolverMethod=biConjugateGradientStabilized;
    }
    else if( answer=="parallel generalized minimal residual" || answer=="parallel gmres" )
    {
      parallelSolverMethod=gmres;
    }
    else if( answer=="parallel transpose free quasi-minimal residual (tcqmr)" || answer=="parallel tcqmr" )
    {
      parallelSolverMethod=tcqmr;
    }
    else if( answer=="parallel transpose free quasi-minimal residual (tfqmr)" || answer=="parallel tfqmr" )
    {
      parallelSolverMethod=tfqmr;
    }
    else if( answer=="parallel conjugate residual" )
    {
      parallelSolverMethod=conjugateResidual;
    }
    else if( answer=="parallel least squares" )
    {
      parallelSolverMethod=leastSquares;
    }
    else if( answer=="parallel preonly" )
    {
      parallelSolverMethod=preonly;
    }
    else if( answer=="parallel qcg" )
    {
      parallelSolverMethod=qcg;
    }


    // ----preconditioner
    else if( answer=="no preconditioner" )
    {
      set(THEpreconditioner,noPreconditioner);
    }
    else if( answer=="jacobi preconditioner" )
    {
      set(THEpreconditioner,jacobiPreconditioner);
    }
    else if( answer=="sor preconditioner" )
    {
      set(THEpreconditioner,sorPreconditioner);
    }
    else if( answer=="lu preconditioner" )
    {
      set(THEpreconditioner,luPreconditioner);
    }
    else if( answer=="shell preconditioner" )
    {
      set(THEpreconditioner,shellPreconditioner);
    }
    else if( answer=="block jacobi preconditioner" )
    {
      set(THEpreconditioner,blockJacobiPreconditioner);
    }
    else if( answer=="multigrid preconditioner" )
    {
      set(THEpreconditioner,multigridPreconditioner);
    }
    else if( answer=="eisenstat preconditioner" )
    {
      set(THEpreconditioner,eisenstatPreconditioner);
    }
    else if( answer=="incomplete Cholesky preconditioner" )
    {
      set(THEpreconditioner,incompleteCholeskyPreconditioner);
    }
    else if( answer=="incomplete LU preconditioner" )
    {
      set(THEpreconditioner,incompleteLUPreconditioner);
    }
    else if( answer=="additive Schwarz preconditioner" )
    {
      set(THEpreconditioner,additiveSchwarzPreconditioner);
    }
    else if( answer=="ksp preconditioner" )
    {
      set(THEpreconditioner,kspPreconditioner);
    }
    else if( answer=="composite preconditioner" )
    {
      set(THEpreconditioner,compositePreconditioner);
    }
    else if( answer=="redundant preconditioner" )
    {
      set(THEpreconditioner,redundantPreconditioner);
    }
    else if( answer=="parallel block jacobi preconditioner" )
    {
      parallelPreconditioner=blockJacobiPreconditioner;
    }
    else if( answer=="parallel additive Schwarz preconditioner" )
    {
      parallelPreconditioner=additiveSchwarzPreconditioner;
    }

    // -- DH ILU preconditioner (experimental)
    else if( answer == "use DH ILU preconditioner")
    {
      int iluType00;
      if ( !isDHPreconditionerAvailable ) 
      {
	gi.outputString("WARNING: DH Preconditioner is not available!");
	gi.outputString("         Compile with USE_DH_PRECONDITIONER");
      }
      gi.inputString(line,sPrintF(buff,
	   "Enter ILU type for DH sparsifying preconditioner (1=ILUK, 2=ILUT, 3=ILUTK, currently=%i):",
	       incompleteLUTypeInDH ));
	    
      if( line!="" ) sScanF(line,"%i", &iluType00 );

      set( THEincompleteLUTypeInDH,  iluType00);
    }
    else if( answer == "drop tolerance for ILUT(dt)" )
    {
      real iluDt00;
      if ( !isDHPreconditionerAvailable ) 
      {
	gi.outputString("WARNING: DH Preconditioner is not available!");
	gi.outputString("         Compile with USE_DH_PRECONDITIONER");
      }
      gi.inputString(line,sPrintF(buff,
	   "Enter drop tolerance for ILUT(dt) (current=%e):",
	       incompleteLUDropTolerance ));
	    
      if( line!="" ) sScanF(line,"%e", &iluDt00 );
      set( THEincompleteLUDropTolerance, iluDt00);
    }
    else if( answer == "matrix sparsification (sparseA)" )
    {
      real iluSparse00;
      if ( !isDHPreconditionerAvailable ) 
      {
	gi.outputString("WARNING: DH Preconditioner is not available!");
	gi.outputString("         Compile with USE_DH_PRECONDITIONER");
      }
      gi.inputString(line,sPrintF(buff,
	   "Enter sparsification coeff. for the matrix (sparseA, current=%e):",
	       incompleteLUSparseACoefficient ));
	    
      if( line!="" ) sScanF(line,"%e", &iluSparse00 );
      set( THEincompleteLUSparseACoefficient, iluSparse00 );
    }
    else if( answer == "factorization sparsification (sparseF)" )
    {
      real iluSparse00;
      if ( !isDHPreconditionerAvailable ) 
      {
	gi.outputString("WARNING: DH Preconditioner is not available!");
	gi.outputString("         Compile with USE_DH_PRECONDITIONER");
      }
      gi.inputString(line,sPrintF(buff,
	   "Enter sparsification coeff. for the factorization (sparseF, current=%e):",
	       incompleteLUSparseFCoefficient ));
	    
      if( line!="" ) sScanF(line,"%e", &iluSparse00 );
      set( THEincompleteLUSparseFCoefficient, iluSparse00 );
    }
    else if( answer == "DH ILU type (ilu_type)")
    {
      int iluType00;
      if ( !isDHPreconditionerAvailable ) 
      {
	gi.outputString("WARNING: DH Preconditioner is not available!");
	gi.outputString("         Compile with USE_DH_PRECONDITIONER");
      }
      gi.inputString(line,sPrintF(buff,
	   "Enter ILU type for DH [1=ILUK, 2=ILUT, 3=ILUTK]  (current=%i):",
	       incompleteLUTypeInDH ));
	    
      if( line!="" ) sScanF(line,"%i", &iluType00 );
      set( THEincompleteLUTypeInDH, iluType00 );
    }
   
    // there are for serial machines or as the solver on each processor
    else if( answer=="superlu" )
    {
      externalSolver=superlu;
    }
    else if( answer=="superlu_mp" )
    {
      externalSolver=superlu_mp;
    }
    else if( answer=="superlu_dist" )
    {
      externalSolver=superlu_dist;
    }
    else if( answer=="mumps" )
    {
      externalSolver=mumps;
    }
    // These are for the highest level "lu" solver on distributed memory machines:
    else if( answer=="parallel superlu_dist" )
    {
      parallelExternalSolver=superlu_dist;
    }
    else if( answer=="parallel mumps" )
    {
      parallelExternalSolver=mumps;
    }
    else if( answer=="parallel hypre" )
    {
      parallelExternalSolver=hypre;
    }
    
    // ---matrix ordering
    else if( answer=="natural ordering" )
    {
      set(THEmatrixOrdering,naturalOrdering);
    }
    else if( answer=="nested disection ordering" )
    {
      set(THEmatrixOrdering,nestedDisectionOrdering);
    }
    else if( answer=="oneWay disection ordering" )
    {
      set(THEmatrixOrdering,oneWayDisectionOrdering);
    }
    else if( answer=="reverse Cuthill McKee ordering" )
    {
      set(THEmatrixOrdering,reverseCuthillMcKeeOrdering);
    }
    else if( answer=="quotient minimum degree ordering" )
    {
      set(THEmatrixOrdering,quotientMinimumDegreeOrdering);
    }
    else if( answer=="rowlength ordering" )
    {
      set(THEmatrixOrdering,rowlengthOrdering);
    }
    // -----convergence parameters
    else if( answer=="maximum allowable increase in the residual" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the maximum allowed increase in residual (default=%e) ",
                     maximumAllowableIncreaseInResidual));
      if( line!="" )
	sScanF(line,"%e",&maximumAllowableIncreaseInResidual);
    }
    // else if( answer=="<gmres restart length" )
    // else if( answer=="maximum number of iterations" )
    // else if( answer=="minimum number of iterations" )
    else if( answer=="number of incomplete LU levels" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the number of incomplete LU levels (default=%i) ",
            numberOfIncompleteLULevels));
      if( line!="" )
	sScanF(line,"%i",&numberOfIncompleteLULevels);
    }
    else if( answer=="incomplete LU expected fill" )
    {
      gi.inputString(line,sPrintF(buff,"incomplete LU expected fill (default=%e) ",incompleteLUExpectedFill));
      if( line!="" )
	sScanF(line,"%e",&incompleteLUExpectedFill);
    }

    // -------- other options -------
    else if( answer=="null vector scaling" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the null vector scaling (default=%e) ",nullVectorScaling));
      if( line!="" )
	sScanF(line,"%e",&nullVectorScaling);
    }
    else if( answer=="turn on the compatibility constraint" )
    {
      set(THEcompatibilityConstraint,true);
    }
    else if( answer=="turn off the compatibility constraint" ) 
    {
      set(THEcompatibilityConstraint,false);
    }
    else if( answer=="solve for transpose" )
    {
      set(THEsolveForTranspose,true);
    }
    else if( answer=="do not solve for transpose" )
    {
      set(THEsolveForTranspose,false);
    }
    else if( answer=="use iterative improvement" )
    {
      set(THEiterativeImprovement,true);
    }
    else if( answer=="do not use iterative improvement" )
    {
      set(THEiterativeImprovement,false);
    }
    else if( answer=="block size" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the block size for block matricies, current=%i",blockSize));
      if( line!="" )
        sScanF(line,"%i",&blockSize);
      printF(" Using blockSize=%i\n",blockSize);
    }
    else if( answer=="scale rows" )
    {
      rescaleRowNorms=true;
      printF("Setting rescaleRowNorms=%i\n",(int)rescaleRowNorms);
    }
    else if( answer=="do not scale rows" )
    {
      rescaleRowNorms=false;
      printF("Setting rescaleRowNorms=%i\n",(int)rescaleRowNorms);
    }
    else if( answer=="debug" )
    {
      gi.inputString(line,sPrintF(buff,"Enter debug (Oges::debug) (current=%i) ",
              Oges::debug));
      if( line!="" )
        sScanF(line,"%i",&Oges::debug);
      printF("Setting Oges::debug=%i\n",Oges::debug);
    }
    else if( (len=answer.matches("define petscOption" )) )
    {
      const int length=answer.length();
      int iStart=len;
      while(  iStart<length && answer[iStart]==' ' ) iStart++;  // skip leading blanks
      int iEnd=iStart;
      while( iEnd<length && answer[iEnd]!=' ' ) iEnd++;       // now look for a blank to end the name
      iEnd--;
      if( iStart<=iEnd )
      {
	aString name = answer(iStart,iEnd);

	iStart=iEnd+1;
	iEnd=length-1;
	while( iStart<iEnd && answer[iStart]==' ' ) iStart++;
	while( iEnd>iStart && answer[iEnd]==' ' ) iEnd--;
	aString value=answer(iStart,iEnd);
	  
	printF(" Adding the PETSc option [%s] [%s]\n",(const char*)name,(const char*)value);
	petscOptions.push_back(ShowFileParameter(name,value));
      }
      else
      {
	printF("ERROR parsing the petsc option: answer=[%s]\n",(const char*) answer);
	printF("Answer show be for the form `define petscOption -ksp_type bcgs'\n");
      }

    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="exit" )
      break;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }
  }
  
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
}

