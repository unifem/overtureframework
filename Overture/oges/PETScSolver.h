#ifndef PETSC_SOLVER

// **************************************************************************
// *********** PETScSolver: Oges interface to parallel PETsc ****************
// ***********         PARALLEL VERSION                      ****************
// **************************************************************************

// extern "C"
// {
#define PETSC_SKIP_COMPLEX
#undef PETSC_HAVE_CXX_COMPLEX
#include "petscksp.h"
// }

#include "Overture.h"
#include "ParallelUtility.h"
#include "EquationSolver.h"

//extern "C"
//{
//#include "petscksp.h"
//}

class PETScSolver : public EquationSolver
{
public:

enum SingularProblemEnum
{
  notSingular=0,
  specifyConstantNullVector,
  specifyNullVector,
  addExtraEquation
};


PETScSolver(Oges & oges_);
virtual ~PETScSolver();

virtual int solve( realCompositeGridFunction & u, realCompositeGridFunction & f );


int destroy();

int buildGlobalIndexing(CompositeGrid & cg );

int buildGlobalIndexingOld(CompositeGrid & cg, realCompositeGridFunction & uu );

// Return the current memory usage in Mb:
static real getCurrentMemoryUsage();

inline int getGlobalIndex( int n, int *iv, int grid, int p ) const;
int getGlobalIndex( int n, int *iv, int grid, realArray & ug ) const;

real getMaximumResidual();

int getNumberOfIterations() const; 

aString getSolverName() const;

int buildMatrix( realCompositeGridFunction & coeff, realCompositeGridFunction & u );

// -- print a description of the solver and options used
virtual int printSolverDescription( const aString & label, FILE *file = stdout ) const; 

virtual int printStatistics( FILE *file = stdout ) const;   // output any relevant statistics 

int setProblemIsSingular( SingularProblemEnum singularOption=specifyConstantNullVector );

int buildSolver();

int fillInterpolationCoefficients(realCompositeGridFunction & uu);

// Find the locations for extra equations: 
virtual int findExtraEquations();

// assign values to rhs for the the extra equations 
virtual int setExtraEquationRightHandSideValues( realCompositeGridFunction & f, real *value );

// return solution values from the extra equations
// Old way:
virtual int getExtraEquationValues( const realCompositeGridFunction & u, real *value, const int maxNumberToReturn=1 );

// Convert an Equation Number to a point on a grid (inverse of equationNo)
virtual void equationToIndex( const int eqnNo0, int & n, int & i1, int & i2, int & i3, int & grid );

// convert a component and grid point into an equation number
virtual int equationNo( const int n, const int i1, const int i2, const int i3, const int grid );

// evaluate the dot product of an extra equation times u 
virtual int evaluateExtraEquation( const realCompositeGridFunction & u, real & value, int extraEquation=0 );

virtual int evaluateExtraEquation( const realCompositeGridFunction & u, real & value, 
                                   real & sumOfExtraEquationCoefficients, int extraEquation=0 );

virtual real sizeOf( FILE *file=NULL ); // return number of bytes allocated 

int setPetscParameters();
int setPetscRunTimeParameters();

int initializePETSc();
int finalizePETSc();

static EquationSolver* newPETScSolver(Oges &oges);

static int debug;
static int instancesOfPETSc;  // keeps count of how many different applications use PETSc 

  Vec            x,b;      /* approx solution, RHS */
  Mat            A;        /* linear system matrix */
  KSP            ksp;      /* linear solver context */
  PC             pc;        // Preconditioner ctx
  MatNullSpace   nsp;       // for singular problems
  Vec            *nullVector;

//  PetscRandom    rctx;     /* random number generator context */
//  PetscReal      norm;     /* norm of solution error */
  PetscInt       i,j,I,J,Istart,Iend; 
  PetscErrorCode ierr;
  PetscBool      flg;
  PetscScalar    v; 

  bool turnOnPETScMemoryTracing;    // have PETSc keep track of allocated memory.

real relativeTol;
int numberOfProcessors; 
int numberOfGridPoints,numberOfGridPointsThisProcessor, numberOfUnknowns, numberOfUnknownsThisProcessor;
int *pnab,*pnoffset; 
int numberOfComponents;
realCompositeGridFunction *pCoeff;  // pointer to coefficients 
realCompositeGridFunction *diagonalScale; // for scaling the equations

SingularProblemEnum problemIsSingular; 

bool initialized;
bool reInitialize;

bool globalIndexingIsComputed;

bool useDiagonalScaling;
bool adjustPeriodicCoefficients;

bool processorIsActive;       // set to true if this processor is in PETSc communicator
bool allProcessorsAreActive;  // allProcessorsAreActive==true if all processors are in PETSC_COMM_WORLD 

};


#endif
