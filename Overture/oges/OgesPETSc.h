#ifndef PETSC_EQUATION_SOLVER_H
#define PETSC_EQUATION_SOLVER_H

//
//  Petsc solvers in Overture
//
//  $Id: OgesPETSc.h,v 1.4 2004/04/19 18:14:29 chand Exp $
// 

//kkc 040415 #include <iostream.h>
#include "OvertureDefine.h"
#include OV_STD_INCLUDE(iostream)

#include <math.h>
#include <assert.h>

#include "mpi.h"
#include "Overture.h"
#include "Oges.h"

#include "EquationSolver.h"

extern "C"
{
#include "sles.h"
}

class PETScEquationSolver : public EquationSolver
{
 public:
  PETScEquationSolver(Oges & oges_);
  virtual ~PETScEquationSolver();

  virtual int solve(realCompositeGridFunction & u,
		    realCompositeGridFunction & f);

  SLES          sles;      // Linear solver ConTeXt
  KSP           ksp;       // Krylov Space solver ctx
  PC            pc;        // Preconditioner ctx
  Vec           xsol,brhs;
  Mat           Amx;

  int solvePETSc(realCompositeGridFunction & u,
		 realCompositeGridFunction & f);


  void initializePetscSLES();
  void setPetscParameters();

  //....Aux to solve
  void buildPetscMatrix();
  void preallocRowStorage();
  void getCsortWorkspace(int nWorkSpace00);
  void computeDiagScaling();
  void buildRhsAndSolVector(realCompositeGridFunction & u,
			    realCompositeGridFunction & f);

  //....Logging
  double        timePrecond;
  double        timeSolve;

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
  double        *aval;        // local POINTERS to the matrix in Oges
  int           *ia_,*ja_;
  int           *iWorkRow;    // Workspace for CSORT(=sorts the columns of A)
  int           nWorkRow;
  int           *nzzAlloc;    // num. columns on each row--> for prealloc.
  double        *dscale;      // Diagonal reSCALING to set rownorms==1

  //END CRITICAL----------------------------------

  bool          isMatrixAllocated;
  bool          shouldUpdateMatrix;
  bool          optionsChanged;

};


#endif
