#ifdef OVERTURE_USE_PETSC

//
//  Petsc solvers in Overture
//
// Author: Petri Fast 
// Changes: Bill Henshaw.
// 
//  ADDED: sizeOf returns ALL memory in use by PETSc 10/13/99
//
#include "PETScEquationSolver.h"

typedef MatOrderingType MatReorderingType;
#define PCILUSetMatReordering PCILUSetMatOrdering
#define ORDER_RCM MATORDERING_RCM

#define CSORT   csort_
extern "C" 
{
  extern int PetscSetUseTrMalloc_Private(void);
  int    CSORT(int &n, real &a, int &ja, int &ia, int &iwork);
}


//\begin{>PETScEquationSolverInclude.tex}{\subsection{PETSc EquationSolver}}\label{PETScEquationSolver}
//  //\no function header:
//
//   The {\tt PETScEquationSolver} class can be used to solve a problem with PETSc\cite{petsc-user-guide}.
// 
//\end{PETScEquationSolverInclude.tex} 


//\begin{>>PETScEquationSolverInclude.tex}{\subsection{constructor}} 
PETScEquationSolver::
PETScEquationSolver(Oges & oges_) : EquationSolver(oges_) 
//==================================================================================
// /Description:
//\end{PETScEquationSolverInclude.tex} 
//==================================================================================
{
  name="PETSc";

  petscInitialized=FALSE;
  
  isMatrixAllocated=FALSE;
  shouldUpdateMatrix=FALSE;
//  optionsChanged=FALSE;

  solverMethod=-1;
  preconditioner=-1;             // initialize to an invalid value so we assign it later
  matrixOrdering=-1;
  numberOfIncompleteLULevels=-1;
  gmresRestartLength=-1;
  
  Amx=NULL;

  // ..Workspace for CSORT
  nWorkRow=0;
  iWorkRow=NULL;
  dscale=NULL;
  nzzAlloc=NULL;


}

PETScEquationSolver::
{
  if (dscale!=NULL)   delete(dscale);
  if (nzzAlloc!=NULL) delete(nzzAlloc);
  if (iWorkRow!=NULL) delete(iWorkRow);
  //  PetscFinalize();

  if ( isMatrixAllocated )
  {
    int ierr;
    ierr=MatDestroy( Amx ); CHKERRA( ierr );
    ierr=VecDestroy( xsol ); CHKERRA( ierr );
    ierr=VecDestroy( brhs ); CHKERRA( ierr );
  }

}



real PETScEquationSolver::
sizeOf( FILE *file /* =NULL */  )
// return number of bytes allocated 
{
  real size=0.;  
  int ierr;
  MatInfo matInfo;
  ierr=MatGetInfo(Amx,MAT_GLOBAL_SUM,&matInfo); CHKERRA(ierr);

#ifdef TRACK_PETSC_MEMORY
  PLogDouble space, fragments, maximumBytes;
  ierr = PetscTrSpace( &space, &fragments, &maximumBytes);  CHKERRA(ierr);

  PLogDouble mem;
  PetscGetResidentSetSize(&mem); //  maximum memory used

  size=space;
#else
  size=matInfo.memory;
#endif

  if (Oges::debug & 2) {
    cout << ">> PETSC: bytes used to store matrix=" << matInfo.memory << endl;
#ifdef TRACK_PETSC_MEMORY
    cout << ">> PETSC: bytes in use="<< space
         << ",  fragments="<< fragments
       << ",  maximum num. of bytes in use="<< maximumBytes
       << endl;
    cout << ">> PETSC: maximum memory used="<< mem << endl;
#endif
  }

#ifdef TRACK_PETSC_MEMORY
  if (Oges::debug & 4 ) {
      PetscTrDump(stdout);
  }
#endif

// --------------------THIS would be nice if it worked.
//   Mat *factoredMatrix;
//   PCGetFactoredMatrix(pc,factoredMatrix);
//   MatGetInfo(*factoredMatrix,MAT_GLOBAL_SUM,&matInfo);
//   printf("petsc: ILU matrix memory= %i \n",(int)matInfo.memory);
 
  return size;
}



//
//........................................................
//


static char help[]="PETSc being used by the Overture `Oges' equation solver.";

//..Initialize Petsc linear solver from local options
void PETScEquationSolver::
initializePetscSLES()
{
  int ierr, numProcs;
  if( !petscInitialized )
  {
    if( Oges::debug & 1 ) printf("Initialize PETSc...\n");
    // int argc=0;
    // char **args=NULL;
      
    PetscInitialize(&oges.argc,&oges.argv,(char *)0,help);
    MPI_Comm_size(PETSC_COMM_WORLD, &numProcs);
    if (numProcs !=1)  SETERRA(1,0,"This is a uniprocessor code only!");

#ifdef TRACK_PETSC_MEMORY
    // Activate logging of PETSC's malloc call
    int ierr;
    ierr = PetscSetUseTrMalloc_Private(); CHKERRQ(ierr);
    ierr = PetscTrLog();                  CHKERRA( ierr );
#endif 

    petscInitialized=TRUE;
  }
  else
  {
    // ..Check that we are running on 1 proc.
    MPI_Comm_size(PETSC_COMM_WORLD, &numProcs);
    if ( numProcs!=1 )
    {
      SETERRA(1,0,"This is a uniprocessor code ONLY!!");
    }
  }
  
  // ..Create linear solver context (SLES), and init from params.
  
  isMatrixAllocated=FALSE;
  nzzAlloc=NULL;
  dscale=NULL;
  ierr = SLESCreate(PETSC_COMM_WORLD, &sles); CHKERRA(ierr);
  ierr = SLESGetKSP(sles,&ksp);               CHKERRA(ierr);
  ierr = SLESGetPC(sles, &pc);                CHKERRA(ierr);

  // setPetscParameters();
}

void PETScEquationSolver::
setPetscParameters() 
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

  int ierr;
  if( parameters.solverMethod!=solverMethod )
  {
    KSPType krylovSpaceMethod;

    switch( parameters.solverMethod )
    {
    case OgesParameters::richardson:
      krylovSpaceMethod=KSPRICHARDSON;
      break;
    case OgesParameters::chebychev:
      krylovSpaceMethod=KSPCHEBYCHEV;
      break;
    case OgesParameters::conjugateGradient:
      krylovSpaceMethod=KSPCG;
      break;
    case OgesParameters::gmres:
      krylovSpaceMethod=KSPGMRES;
      break;
    case OgesParameters::biConjugateGradientStabilized: // bcgs
      krylovSpaceMethod=KSPBCGS;
      break;
    case OgesParameters::tcqmr:
      krylovSpaceMethod=KSPTCQMR;
      break;
    case OgesParameters::tfqmr:
      krylovSpaceMethod=KSPTFQMR;
      break;
    case OgesParameters::conjugateResidual:
      krylovSpaceMethod=KSPCR;
      break;
    case OgesParameters::leastSquares:
      krylovSpaceMethod=KSPLSQR;
      break;
    case OgesParameters::preonly:
      krylovSpaceMethod=KSPPREONLY;
      break;
    case OgesParameters::qcg:
      krylovSpaceMethod=KSPQCG;
      break;
    case OgesParameters::biConjugateGradient:
      krylovSpaceMethod=KSPBICG;
      break;
    case OgesParameters::biConjugateGradientSquared:   // do this ***
      printf("PETScEquationSolver: WARNING: no biConjugateGradientSquared, using conjugateGradientSquared\n");
      krylovSpaceMethod=KSPCGS;
      break;
    case OgesParameters::conjugateGradientSquared:
      krylovSpaceMethod=KSPCGS;
      break;
    default:
      printf("****WARNING**** Unknown preconditionner for PETSc\n");
      krylovSpaceMethod=KSPGMRES;
    }
    if( Oges::debug & 1 ) 
      printf(" ********** PETScEquationSolver: set krylov space method ***********\n");

    ierr = KSPSetType(ksp, krylovSpaceMethod); CHKERRA(ierr);
    solverMethod=parameters.solverMethod;
  }

  if( parameters.preconditioner!=preconditioner )
  {
    PCType  petscPreconditioner;    //  == PCILU
    switch (parameters.preconditioner )
    {
    case OgesParameters::noPreconditioner:
      petscPreconditioner=PCNONE ;
      break;
    case OgesParameters::jacobiPreconditioner:
      petscPreconditioner=PCJACOBI;
      break;
    case OgesParameters::sorPreconditioner:
      petscPreconditioner=PCSOR;
      break;
    case OgesParameters::luPreconditioner:
      petscPreconditioner=PCLU;
      break;
    case OgesParameters::shellPreconditioner:
      petscPreconditioner=PCSHELL;
      break;
    case OgesParameters::blockJacobiPreconditioner:
      petscPreconditioner=PCBJACOBI;
      break;
    case OgesParameters::multigridPreconditioner:
      petscPreconditioner=PCMG;
      break;
    case OgesParameters::eisenstatPreconditioner:
      petscPreconditioner=PCEISENSTAT;
      break;
    case OgesParameters::incompleteCholeskyPreconditioner:
      petscPreconditioner=PCICC;
      break;
    case OgesParameters::incompleteLUPreconditioner:
      petscPreconditioner=PCILU;
      break;
    case OgesParameters::additiveSchwarzPreconditioner:
      petscPreconditioner=PCASM;
      break;
    case OgesParameters::slesPreconditioner:
      petscPreconditioner=PCSLES;
      break;
    case OgesParameters::compositePreconditioner:
      petscPreconditioner=PCCOMPOSITE;
      break;
    case OgesParameters::redundantPreconditioner:
      petscPreconditioner=PCREDUNDANT;
      break;
    default:
      printf("****WARNING**** Unknown preconditionner for PETSc\n");
      petscPreconditioner=PCILU;
    }
    if( Oges::debug & 1 ) 
      printf(" ********** PETScEquationSolver: set preconditioner ***********\n");
    ierr = PCSetType(pc,      petscPreconditioner);                    CHKERRA(ierr);
    preconditioner=parameters.preconditioner;
  
  }
  
  if( parameters.matrixOrdering!=matrixOrdering && 
      parameters.preconditioner==OgesParameters::incompleteLUPreconditioner)
  {
    MatOrderingType  matOrdering;           //  == ORDER_RCM default;  
    switch( parameters.matrixOrdering )
    {
    case OgesParameters::naturalOrdering:
      matOrdering=MATORDERING_NATURAL;
      break;
    case OgesParameters::nestedDisectionOrdering:
      matOrdering=MATORDERING_ND;
      break;
    case OgesParameters::oneWayDisectionOrdering:
      matOrdering=MATORDERING_1WD;
      break;
    case OgesParameters::reverseCuthillMcKeeOrdering:
      matOrdering=MATORDERING_RCM;
      break;
    case OgesParameters::quotientMinimumDegreeOrdering:
      matOrdering=MATORDERING_QMD;
      break;
    case OgesParameters::rowlengthOrdering:
      matOrdering=MATORDERING_ROWLENGTH;
      break;
    default:
      printf("****WARNING**** Unknown matrix ordering PETSc\n");
      matOrdering=MATORDERING_NATURAL;
    }
    if( parameters.preconditioner==OgesParameters::incompleteLUPreconditioner ) // ******* fix other cases *****
    {
      if( Oges::debug & 1 ) 
	printf(" ********** PETScEquationSolver: set matrix ordering ***********\n");

      ierr = PCILUSetMatReordering(pc, matOrdering); CHKERRA(ierr);
      matrixOrdering=parameters.matrixOrdering;
    }
    
  }
  
  if( parameters.solverMethod==OgesParameters::gmres && parameters.gmresRestartLength!=gmresRestartLength )
  {
    ierr = KSPGMRESSetRestart(ksp, parameters.gmresRestartLength); CHKERRA(ierr);
    gmresRestartLength=parameters.gmresRestartLength;
  }

  // rtol : reduce residual by this factor.
  // atol : absolute tolerance
  // dtol : divergence detector
  //    convergence:  | r_k |_2 < max( rtol*| r_0 |_2, atol )

  double rtol=parameters.relativeTolerance>0. ? parameters.relativeTolerance : REAL_EPSILON*1000.;
  double atol=parameters.absoluteTolerance>0. ? parameters.absoluteTolerance : 
              max( real(numberOfEquations),500.)*REAL_EPSILON;
  double dtol=parameters.maximumAllowableIncreaseInResidual;
  int maxits = parameters.maximumNumberOfIterations > 0 ?  parameters.maximumNumberOfIterations : 900;
  
  if( Oges::debug & 1 ) 
     printf(" PETScEquationSolver: rtol=%e, atol=%e, dtol=%e\n",rtol,atol,dtol);
  
  ierr = KSPSetTolerances(ksp, rtol, atol, dtol, maxits); CHKERRA(ierr);

  if( parameters.numberOfIncompleteLULevels!=numberOfIncompleteLULevels &&
      parameters.preconditioner==OgesParameters::incompleteLUPreconditioner )
  {
    if( Oges::debug & 1 ) 
      printf(" ********** PETScEquationSolver: set ilu levels ***********\n");

    ierr = PCILUSetLevels(pc, parameters.numberOfIncompleteLULevels);  CHKERRA(ierr);
    ierr = PCILUSetFill(pc,   parameters.incompleteLUExpectedFill);    CHKERRA(ierr);

    numberOfIncompleteLULevels=parameters.numberOfIncompleteLULevels;
  }
  

  //ierr = PCLUSetReuseReordering(pc, 1);          CHKERRA(ierr);}
  //   optionsChanged=TRUE;
}

int PETScEquationSolver::
solve(realCompositeGridFunction & u,
      realCompositeGridFunction & f)
{
  int ierr;

  shouldUpdateMatrix=oges.refactor==TRUE;

  if( !petscInitialized )
  {
    initializePetscSLES();
    if( Oges::debug & 1 ) 
      cout << "...Set command line options\n";
    //.. allow use of command line arguments
    ierr = SLESSetFromOptions(sles); CHKERRA(ierr);
  }
  if( !oges.initialized || oges.shouldBeInitialized )
  {
    shouldUpdateMatrix=TRUE;  // is this correct?
  }
  
  real timeBuild=getCPU();

  // optionsChanged = preconditioner!=oges.parameters.preconditioner || 
  //                    matrixOrdering!=oges.parameters.matrixOrdering;
  //  shouldUpdateMatrix=shouldUpdateMatrix || optionsChanged; // *****

  // set any parameters that have changed
  setPetscParameters();

  if(shouldUpdateMatrix) 
  {
    if( Oges::debug & 1 ) cout << "...Build Matrix\n";
    buildPetscMatrix();
    if( Oges::debug & 1 ) cout << "...Set operators\n";
    ierr = SLESSetOperators(sles,Amx,Amx,DIFFERENT_NONZERO_PATTERN);
    CHKERRA(ierr);

    oges.initialized=TRUE;
    oges.shouldBeInitialized=FALSE;

  }

  if( Oges::debug & 1 ) cout << "...Build RHS\n";
  buildRhsAndSolVector(u,f);
  timeBuild=getCPU()-timeBuild;

  ierr = KSPSetInitialGuessNonzero(ksp); CHKERRA(ierr); 
  real time0=getCPU();
  if( Oges::debug & 1 ) cout << "...Preconditioner\n";
  ierr = SLESSetUp(sles,brhs,xsol);      CHKERRA(ierr);
  timePrecond=getCPU()-time0;

  time0=getCPU();
  if( Oges::debug & 1 ) cout << "...Actually solving...\n";
  oges.numberOfIterations=0;
  ierr = SLESSolve(sles,brhs,xsol,&oges.numberOfIterations); CHKERRA(ierr);
  timeSolve=getCPU()-time0;

  if( Oges::debug & 1 ) 
  {
    cout << "++Petsc TIMINGS (for "<<oges.numberOfIterations<<" its):\n";
    cout << " build="<<timeBuild;
    cout << ", precond="<<timePrecond;
    cout << ", solve="<<timeSolve<<"."<<endl;
  }
  
  // *Take soln --> stick it to Overture vector & Move soln to the grid.
  Scalar *soln;
  real *ovSol;
  int i;
  ierr=VecGetArray(xsol,&soln);     CHKERRA(ierr);
  ovSol= oges.sol.getDataPointer();

  for( i=0; i<oges.numberOfEquations; i++)   // can we avoid this copy?
    ovSol[i]=soln[i];

  oges.storeSolutionIntoGridFunction();

  ierr=VecRestoreArray(xsol,&soln); CHKERRA(ierr);

  return ierr;
}


void PETScEquationSolver::
getCsortWorkspace(int nWorkSpace00)
{
  if( nWorkSpace00>nWorkRow ) 
  {
    if( Oges::debug & 1 )
      cout << "+++(PetscOverture)Reallocating workrow for Csort,"
	   << " need "<<nWorkSpace00<<", currently "<<nWorkRow<<endl;
    if (iWorkRow!=NULL) delete(iWorkRow);
    iWorkRow = new int[nWorkSpace00];
    assert( iWorkRow != NULL );
    nWorkRow = nWorkSpace00;
  }
}

 
//..Build Petsc MATRIX: rescale & prealloc the matrix
void PETScEquationSolver::
buildPetscMatrix()
{
  //..Check that we're running on a single processor
  int numProcs;
  MPI_Comm_size(PETSC_COMM_WORLD, &numProcs);
  if ( numProcs!=1 )
  {
    SETERRA(1,0,"This is a uniprocessor code ONLY!!");
  }


  //..Build Overture matrix
  bool allocateSpace = TRUE;
  bool factorMatrixInPlace = FALSE; // allocate space for matrix only
  oges.formMatrix(numberOfEquations,numberOfNonzeros,Oges::compressedRow,allocateSpace,factorMatrixInPlace);

  aval= oges.a.getDataPointer();
  ia_=  oges.ia.getDataPointer();
  ja_=  oges.ja.getDataPointer();

  //..SORT the columns of the matrix:
  getCsortWorkspace(2*numberOfNonzeros+1);
  if( Oges::debug & 1 )
    cout << "+++(PetscOverture)Sorting the columns of the matrix...\n";
  CSORT( numberOfEquations, aval[0], ja_[0], ia_[0], iWorkRow[0] );

  //..Allocate Petsc matrix
  int & neq=numberOfEquations; // ..shorthand for convenience
  int & nnz=numberOfNonzeros; 

  int        ierr;
  InsertMode insrtOrAdd; // for Petsc MatSetValues
  //.........ALWAYS get rid of old matrix!!
  //if ( isMatrixAllocated && (numberOfEquations!=neqBuilt) ) {
  // ..allocated wrong size for Matrix & vectors in Petsc
  if ( isMatrixAllocated )
  {
    ierr=MatDestroy( Amx ); CHKERRA( ierr );
    ierr=VecDestroy( xsol ); CHKERRA( ierr );
    ierr=VecDestroy( brhs ); CHKERRA( ierr );
    isMatrixAllocated=FALSE;
  }
  if (!isMatrixAllocated)
  {
    if( Oges::debug & 1 )
      cout << "+++(PetscOverture)Allocating the matrix...\n";
    preallocRowStorage(); assert(nzzAlloc!=NULL);
    ierr = MatCreateSeqAIJ(PETSC_COMM_SELF,neq,neq,
			   numberOfNonzeros,nzzAlloc,&Amx);
    //ierr = MatCreate(PETSC_COMM_SELF,neq,neq,&Amx);
    CHKERRA(ierr);
    ierr = VecCreateSeq(PETSC_COMM_SELF,neq,&xsol); CHKERRA(ierr);
    ierr = VecCreateSeq(PETSC_COMM_SELF,neq,&brhs); CHKERRA(ierr);

    isMatrixAllocated=TRUE;
    neqBuilt=neq; //..remember size of current matrix
    insrtOrAdd=INSERT_VALUES;
  }
  //...NOT USED ANYMORE, 'ADD_VALUES' leads to SuperSLOW MxBUILD?!!!
  //else {       //..We have a matrix of right size, just reset it
  //  cout << "+++++++++ -------- USE OLD MATRIX ----- +++++++\n";
  //  ierr = MatZeroEntries(Amx); CHKERRA( ierr );
  //  insrtOrAdd=ADD_VALUES;
  //}

  //..Loop through the Overture matrix to build it in Petsc
  computeDiagScaling();
  int irowm1, j,jcolm1;
  real dsc;
  Scalar  v;
  // real rownorm;

  if( Oges::debug & 1 )
    cout << "+++(PetscOverture)Building the Petsc matrix...\n";
  for( irowm1=0; irowm1<neq; irowm1++ ) {
    dsc=dscale[irowm1];
    // rownorm=0.0;
    for( j=ia_[irowm1]; j<=ia_[irowm1+1]-1; j++ ) 
    {
      v=dsc*aval[ j-1 ];
      jcolm1=ja_[ j-1 ]-1;
      // rownorm = rownorm+ fabs(v);
      // To matrix Amx, Insert row=irow, col=jcol, INSERT (NEW) VALUE 
      ierr=MatSetValues(Amx,1,&irowm1,1,&jcolm1,&v,insrtOrAdd); CHKERRA(ierr);
    } 
  }
  //..Finish up Matrix Assembly, set updateMX flag
  ierr = MatAssemblyBegin(Amx,MAT_FINAL_ASSEMBLY); CHKERRA(ierr);
  ierr = MatAssemblyEnd(Amx,MAT_FINAL_ASSEMBLY);   CHKERRA(ierr);
  shouldUpdateMatrix=FALSE; 

  if( !parameters.keepSparseMatrix )
  {
    oges.ia.redim(0);       // these are no longer needed.
    oges.ja.redim(0);
    oges.a.redim(0);
  }
}

void PETScEquationSolver::
preallocRowStorage()
{
   int i,j;
  int & neq=numberOfEquations; // shorthand
  int & nnz=numberOfNonzeros;
  

  if (nzzAlloc!=NULL) delete( nzzAlloc );
  nzzAlloc = new int [neq]; assert( nzzAlloc != NULL );

  for( i=0; i<neq; i++) 
  {
    nzzAlloc[i]=0;
    assert( (ia_[i]<=nnz+1) && (ia_[i+1]<=nnz+1) );
    nzzAlloc[i]=ia_[i+1]-ia_[i];
  }
 
}

//..Allocate space for diag scaling, set to 1 if no scaling,
//   or 1/rownorms otherwise
void PETScEquationSolver::
computeDiagScaling()
{
  int i,j;
  real rownorm;
  int & neq=numberOfEquations; // shorthand
  int & nnz=numberOfNonzeros;
  

  if (dscale!=NULL) delete( dscale );
  dscale = new real [neq]; assert( dscale != NULL );

  if (!parameters.rescaleRowNorms) 
  {     //..... don't scale the rows in the Matrix
    //cout << "+++++++++++ NOT SCALING ROWS +++++++++++\n";
    for( i=0; i<neq; i++) 
      dscale[i]=1.0;
  } 
  else 
  {                 //..... scale the rows
    //cout << "+++++++++++ SCALING the ROWS +++++++++++\n";
    for( i=0; i<neq; i++)
    {
      rownorm=0.0;
      assert( (ia_[i]<=nnz+1) && (ia_[i+1]<=nnz+1) );
      for( j=ia_[i]; j<ia_[i+1]; j++) 
      {
	rownorm += fabs( aval[j-1] );
      } 
      assert( rownorm > 1.0e-15);
      dscale[i] = 1.0/rownorm;
    }
  }
}

// ..Build PETSC rhs and solution vector
//
void PETScEquationSolver::
buildRhsAndSolVector(realCompositeGridFunction & u,
		     realCompositeGridFunction & f)
{

  if (!isMatrixAllocated) {
    cout << "---------ERROR(petscOverture): Can't build RHS"
	 << " if it is not allocated! ..Exiting..\n";
    exit(-77);
  }
  assert( dscale!=NULL );
  int ierr;
  ierr=oges.formRhsAndSolutionVectors(u,f); assert(ierr==0);

  //Loop to set solution(=initial guess), and the RHS
  real *ovSol, *ovRhs; // Overture solution and rhs
  int i;
  Scalar v;
  ovSol= oges.sol.getDataPointer();
  ovRhs= oges.rhs.getDataPointer();

  for( i=0; i<numberOfEquations; i++) 
  {
    v=ovSol[i];
    ierr=VecSetValues(xsol,1,&i,&v,INSERT_VALUES); CHKERRA(ierr);

    v=ovRhs[i]*dscale[i]; // SCALE rhs as the matrix!!
    ierr=VecSetValues(brhs,1,&i,&v,INSERT_VALUES); CHKERRA(ierr);
  }

  ierr = VecAssemblyBegin(xsol); CHKERRA(ierr);
  ierr = VecAssemblyBegin(brhs); CHKERRA(ierr);
  ierr = VecAssemblyEnd(xsol);   CHKERRA(ierr);
  ierr = VecAssemblyEnd(brhs);   CHKERRA(ierr);
}



#endif /*  OVERTURE_USE_PETSC */

