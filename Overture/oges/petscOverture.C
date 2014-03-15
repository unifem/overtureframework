//
//  Petsc solvers in Overture
//
//  $Id: petscOverture.C,v 1.4 2008/12/15 18:50:43 henshaw Exp $
// 

#include <iostream.h>
#include <math.h>
#include <assert.h>

//-------Includes Overture & Petsc
#include "petscOverture.h"

#define CSORT EXTERN_C_NAME(csort)
extern "C" {
  int    CSORT(int &n, double &a, int &ja, int &ia, int &iwork);
}

//
//........................................................
//

PetscOverture::
PetscOverture( ) 
{
  constructor(NULL); // no grid
}

PetscOverture::
PetscOverture( CompositeGrid & cg00)
{
  constructor( &cg00 );
}

void
PetscOverture::
constructor( CompositeGrid *pCg00 )
{
  if (pCg00==NULL) pOgesCtx = new Oges();
  else             pOgesCtx = new Oges(*pCg00);
  assert(pOgesCtx!=NULL);
  pCg=pCg00;
  ogesIsMine=TRUE;

  setup(*pOgesCtx);
  initializePetscSLES();
}


PetscOverture::
PetscOverture(Oges & ogesCtx) 
{ 
  constructor( ogesCtx, NULL ); // no grid
}

PetscOverture::
PetscOverture(Oges & ogesCtx, CompositeGrid & cg00) 
{
  constructor( ogesCtx, &cg00 );
}

void
PetscOverture::
constructor( Oges & ogesCtx, CompositeGrid  *pCg00) 
{
  pCg=pCg00;

  pOgesCtx=&ogesCtx; assert( pOgesCtx != NULL);
  ogesIsMine=FALSE;
  setup(*pOgesCtx);
  initializePetscSLES();
}

PetscOverture::
~PetscOverture()
{
  if (ogesIsMine)     delete(pOgesCtx);
  if (dscale!=NULL)   delete(dscale);
  if (nzzAlloc!=NULL) delete(nzzAlloc);
  if (iWorkRow!=NULL) delete(iWorkRow);

  // .. get rid of SLES, Vec, Amx etc..?
}

//.......AUX to constructors

void
PetscOverture::
setup(Oges & ogesCtx)
{
  // *Set Oges solver to 'Yale'--> for building the Sparse matrix only
  int solverType=1; 
  ogesCtx.setSolverType((Oges::solvers)solverType);

  // ..Set local (PETSc) optionflags to the defaults
  conjugateGradientType = KSPTFQMR;  // Transp. free QMR (Freund's version)
  preconditionerType    = PCILU;
  gmresRestartLength    = 30;
  iluLevels             = 3;
  iluExpectedFill       = 3.0;
  matOrdering           = ORDER_RCM; // use Rev Cuthill-McKee

  rtol                  = 1e-8;
  atol                  = 1e-50;
  dtol                  = 1e50;
  maxits                = 900;

  // ..Workspace for CSORT
  nWorkRow=0;
  iWorkRow=NULL;

  // ..Options set from command line
  rownormScale          = TRUE;      // scale rows by default
  int ierr, flag;
  ierr=OptionsHasName(PETSC_NULL,"-nodiagscale",&flag); CHKERRA(ierr);
  if (1==flag) rownormScale=FALSE;

}

//..Initialize Petsc linear solver from local options
void
PetscOverture::
initializePetscSLES()
{
  // ..Check that we are running on 1 proc.
  int numProcs;
  MPI_Comm_size(PETSC_COMM_WORLD, &numProcs);
  if ( numProcs!=1 ){
    SETERRA(1,0,"This is a uniprocessor code ONLY!!");
  }

  // ..Create linear solver context (SLES), and init from params.
  int ierr;
  isMatrixAllocated=FALSE;
  nzzAlloc=NULL;
  dscale=NULL;
  ierr = SLESCreate(PETSC_COMM_WORLD, &sles); CHKERRA(ierr);
  ierr = SLESGetKSP(sles,&ksp);               CHKERRA(ierr);
  ierr = SLESGetPC(sles, &pc);                CHKERRA(ierr);

  setPetscParameters();
}

void
PetscOverture::
setPetscParameters() 
{
  int ierr;
  ierr = KSPSetType(ksp, conjugateGradientType); CHKERRA(ierr);
  if (conjugateGradientType==KSPGMRES) {
    ierr = KSPGMRESSetRestart(ksp, gmresRestartLength); CHKERRA(ierr);
  }
  ierr = KSPSetTolerances(ksp, rtol, atol, dtol, maxits); CHKERRA(ierr);

  ierr = PCSetType(pc,      preconditionerType); CHKERRA(ierr);
  ierr = PCILUSetLevels(pc, iluLevels);          CHKERRA(ierr);
  ierr = PCILUSetFill(pc,   iluExpectedFill);    CHKERRA(ierr);

  ierr = PCILUSetMatReordering(pc, matOrdering); CHKERRA(ierr);
  //ierr = PCLUSetReuseReordering(pc, 1);          CHKERRA(ierr);}
  optionsChanged=TRUE;
}

//..INTERFACE to the class
void
PetscOverture::
setCompositeGrid( CompositeGrid & cg )
{
  pCg = &cg;
  pOgesCtx->setCompositeGrid( *pCg );
  shouldUpdateMatrix=TRUE;
}
 
void
PetscOverture::
setCoefficientArray( realCompositeGridFunction & coeff )
{
  pOgesCtx->setCoefficientArray( coeff );
  shouldUpdateMatrix=TRUE;
}

int
PetscOverture::
updateToMatchGrid( CompositeGrid & cg00 )
{
  pCg = & cg00;
  pOgesCtx->updateToMatchGrid( *pCg );
  shouldUpdateMatrix=TRUE;

  return 0;
}

void
PetscOverture::
solve(realCompositeGridFunction & u,
	     realCompositeGridFunction & f)
{
  int ierr;

  double timeBuild=getCPU();
  cout << "...Set command line options\n";
  if (optionsChanged) {
    //.. allow use of command line arguments
    ierr = SLESSetFromOptions(sles); CHKERRA(ierr);
  }
  if(shouldUpdateMatrix) {
    cout << "...Build Matrix\n";
    buildPetscMatrix();
    cout << "...Set operators\n";
    ierr = SLESSetOperators(sles,Amx,Amx,DIFFERENT_NONZERO_PATTERN);
    CHKERRA(ierr);
  }

  cout << "...Build RHS\n";
  buildRhsAndSolVector(u,f);
  timeBuild=getCPU()-timeBuild;

  ierr = KSPSetInitialGuessNonzero(ksp); CHKERRA(ierr); 
  double time0=getCPU();
  cout << "...Preconditioner\n";
  ierr = SLESSetUp(sles,brhs,xsol);      CHKERRA(ierr);
  timePrecond=getCPU()-time0;

  time0=getCPU();
  cout << "...Actually solving...\n";
  ierr = SLESSolve(sles,brhs,xsol,&numberOfIterations); CHKERRA(ierr);
  timeSolve=getCPU()-time0;

  cout << "++Petsc TIMINGS (for "<<numberOfIterations<<" its):\n";
  cout << " build="<<timeBuild;
  cout << ", precond="<<timePrecond;
  cout << ", solve="<<timeSolve<<"."<<endl;

  // *Take soln --> stick it to Overture vector & Move soln to the grid.
  Scalar *soln;
  double *ovSol;
  int i;
  ierr=VecGetArray(xsol,&soln);     CHKERRA(ierr);
  ovSol= & (pOgesCtx->sol(1));

  for( i=0; i<numberOfEquations; i++) ovSol[i]=soln[i];
  pOgesCtx->storeSolIntoGridFunction( ierr );
  ierr=VecRestoreArray(xsol,&soln); CHKERRA(ierr);

}

// * Data access routines
void 
PetscOverture::
setScaleRowNorms(bool flag00) 
{rownormRescale=flag00;};

void 
PetscOverture::
setKrylovSpaceMethod(KSPType cgType00)
{
  conjugateGradientType=cgType00;
  int ierr = KSPSetType(ksp, conjugateGradientType); CHKERRA(ierr);
  if (conjugateGradientType==KSPGMRES) {
    int ierr = KSPGMRESSetRestart(ksp, gmresRestartLength); CHKERRA(ierr);
  }
  optionsChanged=TRUE;
};

void 
PetscOverture::
setPreconditioner(PCType pcType00)
{
  preconditionerType=pcType00;
  int ierr = PCSetType(pc,      preconditionerType); CHKERRA(ierr);
  optionsChanged=TRUE;
};

void 
PetscOverture::
setReordering(MatReorderingType order00) 
{
  if( matOrdering != order00 ){
    matOrdering=order00;
    int ierr = PCILUSetMatReordering(pc, matOrdering); CHKERRA(ierr);
    optionsChanged=TRUE;
  }
};

void 
PetscOverture::
setGMRESRestartLength(int len00)  
{
  gmresRestartLength=len00;
  if (conjugateGradientType==KSPGMRES) {
    int ierr = KSPGMRESSetRestart(ksp, gmresRestartLength); CHKERRA(ierr);
    optionsChanged=TRUE;
  };
};

void 
PetscOverture::
setILULevels(int lev00)           
{
  if (lev00 != iluLevels) { // change causes a refactor
    iluLevels=lev00;
    int ierr = PCILUSetLevels(pc, iluLevels);  CHKERRA(ierr);
    optionsChanged=TRUE;
  }
};

void 
PetscOverture::
setILUExpectedFill(double dfill00)
{
  if (dfill00 != iluExpectedFill) {
    iluExpectedFill=dfill00;
    int ierr = PCILUSetFill(pc,   iluExpectedFill); CHKERRA(ierr);
    optionsChanged=TRUE;
  }
};

void PetscOverture::
setMaximumIterations(int maxit00) 
{
  maxits=maxit00;
  int ierr;
  ierr = KSPSetTolerances(ksp, rtol, atol, dtol, maxits); CHKERRA(ierr);
  optionsChanged=TRUE;
};

void 
PetscOverture::
setTolerance(double rtol00,double atol00/*=1e-50*/,double dtol00/*=1e50*/)
{
  rtol=rtol00;
  atol=atol00;
  dtol=dtol00;
  int ierr;
  ierr = KSPSetTolerances(ksp, rtol, atol, dtol, maxits); CHKERRA(ierr);
  optionsChanged=TRUE;
}

void
PetscOverture::
getCsortWorkspace(int nWorkSpace00)
{
  if( nWorkSpace00>nWorkRow ) {
    cout << "+++(PetscOverture)Reallocating workrow for Csort,"
	 << " need "<<nWorkSpace00<<", currently "<<nWorkRow<<endl;
    if (iWorkRow!=NULL) delete(iWorkRow);
    iWorkRow = new int[nWorkSpace00];
    assert( iWorkRow != NULL );
    nWorkRow = nWorkSpace00;
  }
}

 
//..Build Petsc MATRIX: rescale & prealloc the matrix
void
PetscOverture::
buildPetscMatrix()
{
  //..Check that we're running on a single processor
  int numProcs;
  MPI_Comm_size(PETSC_COMM_WORLD, &numProcs);
  if ( numProcs!=1 ){
    SETERRA(1,0,"This is a uniprocessor code ONLY!!");
  }

  //..Build Overture matrix
  pOgesCtx->formMatrix();
  aval= &(pOgesCtx->a(1));
  ia=   &(pOgesCtx->ia(1));
  ja=   &(pOgesCtx->ja(1));
  numberOfEquations=pOgesCtx->numberOfEquations;
  numberOfNonzeros=pOgesCtx->numberOfNonzeros;

  //..SORT the columns of the matrix:
  getCsortWorkspace(2*numberOfNonzeros+1);
  cout << "+++(PetscOverture)Sorting the columns of the matrix...\n";
  CSORT( numberOfEquations, aval[0], ja[0], ia[0], iWorkRow[0] );

  //..Allocate Petsc matrix
  int & neq=numberOfEquations; // ..shorthand for convenience
  int & nnz=numberOfNonzeros; 

  int        ierr;
  InsertMode insrtOrAdd; // for Petsc MatSetValues
  //.........ALWAYS get rid of old matrix!!
  //if ( isMatrixAllocated && (numberOfEquations!=neqBuilt) ) {
  // ..allocated wrong size for Matrix & vectors in Petsc
  if ( isMatrixAllocated ) {
    ierr=MatDestroy( Amx ); CHKERRA( ierr );
    ierr=VecDestroy( xsol ); CHKERRA( ierr );
    ierr=VecDestroy( brhs ); CHKERRA( ierr );
    isMatrixAllocated=FALSE;
  }
  if (!isMatrixAllocated) {
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
  double dsc;
  Scalar  v;
  double rownorm;

  cout << "+++(PetscOverture)Building the Petsc matrix...\n";
  for( irowm1=0; irowm1<neq; irowm1++ ) {
    dsc=dscale[irowm1];
    rownorm=0.0;
    for( j=ia[irowm1]; j<=ia[irowm1+1]-1; j++ ) {
      v=dsc*aval[ j-1 ];
      jcolm1=ja[ j-1 ]-1;
      rownorm = rownorm+ fabs(v);
      // To matrix Amx, Insert row=irow, col=jcol, INSERT (NEW) VALUE 
      ierr=MatSetValues(Amx,1,&irowm1,1,&jcolm1,&v,insrtOrAdd); CHKERRA(ierr);
    } 
  }
  //..Finish up Matrix Assembly, set updateMX flag
  ierr = MatAssemblyBegin(Amx,MAT_FINAL_ASSEMBLY); CHKERRA(ierr);
  ierr = MatAssemblyEnd(Amx,MAT_FINAL_ASSEMBLY);   CHKERRA(ierr);
  shouldUpdateMatrix=FALSE; 
}

void
PetscOverture::
preallocRowStorage()
{
   int i,j;
  double rownorm;
  int & neq=numberOfEquations; // shorthand
  int & nnz=numberOfNonzeros;
  

  if (nzzAlloc!=NULL) delete( nzzAlloc );
  nzzAlloc = new int [neq]; assert( nzzAlloc != NULL );

  for( i=0; i<neq; i++) {
    nzzAlloc[i]=0;
    assert( (ia[i]<=nnz+1) && (ia[i+1]<=nnz+1) );
    nzzAlloc[i]=ia[i+1]-ia[i];
  }
 
}

//..Allocate space for diag scaling, set to 1 if no scaling,
//   or 1/rownorms otherwise
void 
PetscOverture::
computeDiagScaling()
{
  int i,j;
  double rownorm;
  int & neq=numberOfEquations; // shorthand
  int & nnz=numberOfNonzeros;
  

  if (dscale!=NULL) delete( dscale );
  dscale = new double [neq]; assert( dscale != NULL );

  if (!rownormScale) {     //..... don't scale the rows in the Matrix
    //cout << "+++++++++++ NOT SCALING ROWS +++++++++++\n";
    for( i=0; i<neq; i++) dscale[i]=1.0;
  } else {                 //..... scale the rows
    //cout << "+++++++++++ SCALING the ROWS +++++++++++\n";
    for( i=0; i<neq; i++) {
      rownorm=0.0;
      assert( (ia[i]<=nnz+1) && (ia[i+1]<=nnz+1) );
      for( j=ia[i]; j<ia[i+1]; j++) {
	rownorm += fabs( aval[j-1] );
      } 
      assert( rownorm > 1.0e-15);
      dscale[i] = 1.0/rownorm;
    }
  }
}

// ..Build PETSC rhs and solution vector
//
void 
PetscOverture::
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
  pOgesCtx->formRhsAndSolutionVectors(u,f,ierr); assert(ierr==0);

  //Loop to set solution(=initial guess), and the RHS
  double *ovSol, *ovRhs; // Overture solution and rhs
  int i;
  Scalar v;
  ovSol= & (pOgesCtx->sol(1));
  ovRhs= & (pOgesCtx->rhs(1));

  for( i=0; i<numberOfEquations; i++) {
    v=ovSol[i];
    ierr=VecSetValues(xsol,1,&i,&v,INSERT_VALUES); CHKERRA(ierr);

    v=ovRhs[i]*dscale[i]; // SCALE rhs as the matrix!!
    ierr=VecSetValues(brhs,1,&i,&v,INSERT_VALUES); CHKERRA(ierr);
  }

  ierr = VecAssemblyBegin(xsol); CHKERRA(ierr);
  ierr = VecAssemblyBegin(brhs); CHKERRA(ierr);
}

