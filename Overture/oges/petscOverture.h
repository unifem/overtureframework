//                                              -*- c++ -*-
//  Petsc solvers in Overture
//  ..............................................................
//  * See the examples moveAndPetsc, tcmPetsc for usage.
//  * Petsc command line arguments are supported and have priority over
//    options set with the access routines. See the Petsc documentation 
//    (Chap 4.) for a list of useful command line args.
//
//      E.g. -ksp_type bcgs -ksp_xmonitor -pc_ilu_level 10
//          picks BiCGSTAB, shows the convergence history in an xwindow,
//          uses ILU(10) as precond.
// 
//  Written: April 1999, Petri Fast, Courant Institute.
//  $Id: petscOverture.h,v 1.4 2004/04/19 18:14:29 chand Exp $  
//
#ifndef PETSCOVERTURE_H
#define PETSCOVERTURE_H

//kkc 040415 #include <iostream.h>
#include "OvertureDefine.h"
#include OV_STD_INCLUDE(iostream)

//-- Overture & Petsc include files: Order is important, won't work otherwise
#include "mpi.h"
#include "Overture.h"
#include "Oges.h"
extern "C"
{
#include "sles.h"
}

typedef MatOrderingType MatReorderingType;
#define PCILUSetMatReordering PCILUSetMatOrdering
#define ORDER_RCM MATORDERING_RCM
//.....................................................
class PetscOverture {
public:

  PetscOverture(Oges & ogesCtx  );
  PetscOverture( );
  PetscOverture( CompositeGrid & cg00 );
  PetscOverture(Oges & ogesCtx, CompositeGrid & cg00);
  ~PetscOverture();
  
  //..Interface
  void setCompositeGrid( CompositeGrid & cg );
  void setCoefficientArray( realCompositeGridFunction & coeff);
  int updateToMatchGrid( CompositeGrid & cg );

  //.... set INITIAL GUESS & RHS --> return solution in 'f'
  void solve(realCompositeGridFunction & u,
	     realCompositeGridFunction & f);

  //.... data access routines
  void setScaleRowNorms(bool flag00);
  void setKrylovSpaceMethod(KSPType cgType00);
  void setPreconditioner(PCType pcType00);
  void setReordering(MatReorderingType order00);
  void setGMRESRestartLength(int len00);
  void setILULevels(int lev00);
  void setILUExpectedFill(double dfill00);
  void setTolerance(double rtol00,double atol00=1e-50,double dtol00=1e50);
  void setMaximumIterations(int maxit00);

  //..AUX routines
  //....Aux to constructor
  //private:
  void constructor( CompositeGrid * pCg00);
  void constructor( Oges & ogesCtx, CompositeGrid * pCg00); 

  void setup( Oges & ogesCtx );
  void initializePetscSLES();
  void setPetscParameters();

  //....Aux to solve
  void buildPetscMatrix();
  void preallocRowStorage();
  void getCsortWorkspace(int nWorkSpace00);
  void computeDiagScaling();
  void buildRhsAndSolVector(realCompositeGridFunction & u,
			    realCompositeGridFunction & f);

  //..Data
 public:
  int           numberOfNonzeros; 
  int           numberOfEquations;
  //....Logging
  double        timePrecond;
  double        timeSolve;
  int           numberOfIterations;
  //double *convHistory??

  //private:
  //..The remaining data is essentially PRIVATE, 
  //  shouldn't be accessed or modified directly from outside the class
  //......N.B: if you want to modify these objects, write
  //......     access routines -- irect modification from 
  //......     outside the class is discouraged, and may not
  //......     supported in future revisions.

  //CRITICAL------These are NOT to be tampered with from outside this class!!
  CompositeGrid *pCg;
  Oges          *pOgesCtx;
  bool          ogesIsMine;   // =1 if oges was created in this class

  int           neqBuilt;     // size of CURRENT matrix & vectors
  double        *aval;        // local POINTERS to the matrix in Oges
  int           *ia,*ja;
  int           *iWorkRow;    // Workspace for CSORT(=sorts the columns of A)
  int           nWorkRow;
  int           *nzzAlloc;    // num. columns on each row--> for prealloc.
  double        *dscale;      // Diagonal reSCALING to set rownorms==1
  bool          rownormScale; //
  //END CRITICAL----------------------------------

  //....Petsc stuff
  SLES          sles;      // Linear solver ConTeXt
  KSP           ksp;       // Krylov Space solver ctx
  PC            pc;        // Preconditioner ctx
  Vec           xsol,brhs;
  Mat           Amx;
  bool          isMatrixAllocated;
  bool          shouldUpdateMatrix;
  bool          optionsChanged;

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
  //
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
  //
  // Matrix Reorderings
  //   ORDER_NATURAL  = no reorder
  //   ORDER_ND       = Nested Dissection
  //   ORDER_1WD      = One-way Dissection
  //   ORDER_RCM      = Reverse Cuthill-McKee
  //   ORDER_QMD etc  = Quotient Minimum Degree
  //
  KSPType   conjugateGradientType; //  == KSPTFQMR
  PCType    preconditionerType;    //  == PCILU
  int       gmresRestartLength;    //  == 30 default
  int       iluLevels;             //  == 3  default 
  double    iluExpectedFill;       //  == 3 for ILU(3)
  bool      rownormRescale;        //

  MatReorderingType  matOrdering;           //  == ORDER_RCM default;  

  //..Tolerances: Atol=1e-50, dtol=1e50, no interest in setting those differ.
  double    rtol;                  // default ==1e-9, 
  double    atol,dtol;             // Hardwired to 1e-50, 1e50
  int       maxits;                // default = 900  -- Hopefully not needed:)
};

#endif
