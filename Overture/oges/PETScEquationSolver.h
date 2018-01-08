#ifndef PETSC_EQUATION_SOLVER_H
#define PETSC_EQUATION_SOLVER_H

//
//  Petsc solvers in Overture  **** SERIAL VERSION ****
//
//  $Id: PETScEquationSolver.h,v 1.12 2008/12/03 17:54:53 chand Exp $
//

//kkc 081124 #include <iostream.h>
#include <iostream>
#include <math.h>
#include <assert.h>

#include "mpi.h"
#include "Overture.h"
#include "Oges.h"

#include "EquationSolver.h"

// krb do not use extern "C" if PETSc is linked using BOPT=g_c++
//extern "C"
//{
// *xmj* 01/08/2018  Request PETSc to skip complex
#define PETSC_SKIP_COMPLEX
// *wdh* 2015/09/31  To avoid having PETSc include complex.h do this:
#include "petscconf.h"
#undef PETSC_HAVE_CXX_COMPLEX
#include "petscksp.h"
//}


// Experimental Preconditioner from David Hysom (Summer 2000)
#ifdef USE_DH_PRECONDITIONER
extern "C"
{
#include "dhPreconditioner.h"
#include "dh_timer.h"
}
#else
class MyPcData;
#endif /* USE_DH_PRECONDITIONER */

class PETScEquationSolver : public EquationSolver
{
 public:
  PETScEquationSolver(Oges & oges_);
  virtual ~PETScEquationSolver();

  virtual int solve(realCompositeGridFunction & u,
		    realCompositeGridFunction & f);

  virtual int saveBinaryMatrix(aString filename00,
			       realCompositeGridFunction & u,
			       realCompositeGridFunction & f);

  virtual real sizeOf( FILE *file=NULL ); // return number of bytes allocated

  MPI_Comm comm;
  KSP           ksp;      // Linear solver ConTeXt, Krylov Space solver ctx
  PC            pc;        // Preconditioner ctx
  Vec           xsol,brhs;
  Mat           Amx;

  virtual real getMaximumResidual();

  int getNumberOfIterations() const;

  int allocateMatrix(int,int,int,int);
  int setMatrixElement(int,int,int,real);
  int displayMatrix();

// So far a common data structure is used by all vector types, so there is no need to have these:
//  void setRHSVectorElement(int,real);
//  void setSolVectorElement(int,real);

  int solvePETSc(realCompositeGridFunction & u,
		 realCompositeGridFunction & f);


  int initializePetscKSP();
  int setPetscParameters();
  int setPetscRunTimeParameters();

  //....Aux to solve
  int buildPetscMatrix();
  void preallocRowStorage(int blockSize);
  void getCsortWorkspace(int nWorkSpace00);
  void computeDiagScaling();
  int buildRhsAndSolVector(realCompositeGridFunction & u,
	  	           realCompositeGridFunction & f);

  int setupPreconditioner(KSP ksp, Vec brhs, Vec xsol );

  //....Logging
  real          timePrecond;
  real          timeSolve;

  //private:
  //..The remaining data is essentially PRIVATE,
  //  shouldn't be accessed or modified directly from outside the class
  //......N.B: if you want to modify these objects, write
  //......     access routines -- irect modification from
  //......     outside the class is discouraged, and may not
  //......     supported in future revisions.

  //CRITICAL------These are NOT to be tampered with from outside this class!!
  bool          petscInitialized; // if tru PETSc has been initialized

  int           neqBuilt;     // size of CURRENT matrix & vectors
  real          *aval;        // local POINTERS to the matrix in Oges
  int           *ia_,*ja_;
  int           *iWorkRow;    // Workspace for CSORT(=sorts the columns of A)
  int           nWorkRow;
  int           *nzzAlloc;    // num. columns on each row--> for prealloc.
  real          *dscale;      // Diagonal reSCALING to set rownorms==1

  //END CRITICAL----------------------------------

  bool          isMatrixAllocated;
  bool          shouldUpdateMatrix;
  // bool          optionsChanged;
  bool copyOfSolutionNeeded;
  Oges::SparseStorageFormatEnum matrixFormat;

  bool turnOnPETScMemoryTracing;    // have PETSc keep track of allocated memory.

  // here we save the values of the current state so we can compare for any changes
  // with oges.parameters
  int solverMethod;
  int preconditioner;
  int matrixOrdering;
  int numberOfIncompleteLULevels;
  int gmresRestartLength;


  //
  // Information for new Hysom/Chow preconditioners
  //
  bool isDHPreconditioner;

  MyPcData   *dh_ctx;
  double      dh_setupTime;
  double      dh_solveTime;

  //int         dh_ilu_type;
  //int         dh_ilu_levels;
  //double      dh_dropTolerance;
  //double      dh_sparseA;
  //double      dh_sparseF;

  aString      dh_pcName;

  void dh_initialize();
  void dh_setParameters();

  void dh_computeResidualReduction( double & residReduction );

  static EquationSolver* newPETScEquationSolver(Oges &oges);


private:
  int  ierr;

};

// Note: The following macro will be distributed in versions of
//       PETSc after version 2.0.28, so this is included here to allow linking
//       with version 2.0.28 and earlier.
// *wdh* ?? #if !defined(PetscFunctionReturnVoid())
#if !defined(PetscFunctionReturnVoid)
#if defined(PETSC_USE_STACK)
#define PetscFunctionReturnVoid() \
  {\
  PetscStackPop;}
#else
#define PetscFunctionReturnVoid()
#endif
#endif

#endif
