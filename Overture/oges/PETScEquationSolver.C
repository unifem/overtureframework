#ifdef OVERTURE_USE_PETSC

//
//  Petsc solvers in Overture **** SERIAL VERSION ****
//
// Author: Petri Fast
// Changes: Bill Henshaw, Kristopher Buschelman..
//
//
//

#include "PETScEquationSolver.h"

typedef MatOrderingType MatReorderingType;
#define PCILUSetMatReordering PCILUSetMatOrdering
#define ORDER_RCM MATORDERING_RCM

#define OVCSORT EXTERN_C_NAME(ovcsort)
extern "C"
{
  extern int PetscSetUseTrMalloc_Private(void);
  int OVCSORT(int &n, real &a, int &ja, int &ia, int &iwork);
}


//\begin{>PETScEquationSolverInclude.tex}{\subsection{PETSc EquationSolver}}\label{PETScEquationSolver}
//  //\no function header:
//
//   The {\tt PETScEquationSolver} class can be used to solve a problem with PETSc\cite{petsc-user-guide}.
//
//\end{PETScEquationSolverInclude.tex}

// =============================================================================
//  \brief Here is the function that Overture::finish() calls to shutdown PETSc
// =============================================================================
static void
finalizePETSc()
{
  #ifdef OVERTURE_USE_PETSC
  int ierr = PetscFinalize();
  #endif
}

// =======================================================================================
/// \brief This function is called by buildEquationSolvers to create a PETScEquationSolver
// =======================================================================================
EquationSolver* PETScEquationSolver::newPETScEquationSolver(Oges &oges)
{
  PETScEquationSolver *pETScEquationSolver =  new PETScEquationSolver(oges);
  return pETScEquationSolver;
}

// ========================================================================================
/// \brief Call this function (before using Oges) if you want to use the PETSc solvers
// =======================================================================================
#ifndef USE_PPP
void
initPETSc()
{
  Oges::petscIsAvailable=true;                                    // set to true if PETSc is available
  Oges::createPETSc=PETScEquationSolver::newPETScEquationSolver;  // pointer to a function that can "new" a PETSc instance
  Overture::shutDownPETSc = &finalizePETSc;                       // set the function that will shut down PETSc
}
#endif


#undef __FUNC__
#define __FUNC__ "PETScEquationSolver::PETScEquationSolver"
//\begin{>>PETScEquationSolverInclude.tex}{\subsection{constructor}}
PETScEquationSolver::
PETScEquationSolver(Oges & oges_) : EquationSolver(oges_)
//==================================================================================
// /Description:
//\end{PETScEquationSolverInclude.tex}
//==================================================================================
{
//  PetscFunctionBegin;

  name                    ="PETSc";
  petscInitialized        =FALSE;
  turnOnPETScMemoryTracing=false; // *wdh* 010318 TRUE;

  // *wdh* 2017/06/11   comm  = max(1,PETSC_COMM_WORLD);  // PETSC_COMM_WORLD is only valid after PetscInitialize is called

  if( PETSC_COMM_WORLD == MPI_COMM_NULL )
    comm = MPI_COMM_WORLD;
  else
    comm = PETSC_COMM_WORLD;

  isMatrixAllocated       =FALSE;
  shouldUpdateMatrix      =FALSE;
//  optionsChanged=FALSE;

  // Using "other" can be slow in the setup phase -- probably related to the row values
  // not being sorted?
  // matrixFormat=Oges::other;   // avoids building the (ia,ja,a) arrays
  matrixFormat=Oges::compressedRow;  // requires building the (ia,ja,a) arrays
  if( parameters.solveForTranspose )
  {
    // when solving for the transpose we must use the (ia,ja,a) arrays.
    if( Oges::debug & 2 )
      printf("PETScEquationSolver: solving for transpose\n");
    matrixFormat=Oges::compressedRow;  // requires building the (ia,ja,a) arrays
  }

  solverMethod              =-1;
  preconditioner            =-1;             // initialize to an invalid value so we assign it later
  matrixOrdering            =-1;
  numberOfIncompleteLULevels=-1;
  gmresRestartLength        =-1;

  Amx=NULL;

  // If we are using double precision then we can avoid copying the solution from PETSc
#ifdef OV_USE_DOUBLE
  copyOfSolutionNeeded=false;
#else
  copyOfSolutionNeeded=true;
#endif


  // ..Workspace for OVCSORT
  nWorkRow=0;
  iWorkRow=NULL;
  dscale=NULL;
  nzzAlloc=NULL;

  isDHPreconditioner = false;
#ifdef USE_DH_PRECONDITIONER
  dh_ctx = NULL;
#endif

//  PetscFunctionReturnVoid();
}

#undef __FUNC__
#define __FUNC__ "PETScEquationSolver::~PETScEquationSolver"
PETScEquationSolver::
~PETScEquationSolver()
{
//  PetscFunctionBegin;

  delete [] dscale;
  delete [] nzzAlloc;
  delete [] iWorkRow;

  if ( isMatrixAllocated )
  {
    int ierr;
    ierr=MatDestroy( &Amx );  // CHKERRQ( ierr );
    ierr=VecDestroy( &xsol ); // CCHKERRQ( ierr );
    ierr=VecDestroy( &brhs ); // CCHKERRQ( ierr );
  }

#ifdef USE_DH_PRECONDITIONER
  if ( dh_ctx != NULL ) MyILU_destroy( dh_ctx );
#endif

  if( petscInitialized ) // i.e., if PETSc was initialized by Overture...
  {

    // *new* way 10026
    Overture::decrementReferenceCountForPETSc();
    petscInitialized=false;

    // PetscStackPop;
    // if( Oges::debug & 1 ) printf("PETScEquationSolver:call PetscFinalize...\n");
    // PetscFinalize();
  }

//  PetscFunctionReturnVoid();
}


#undef __FUNC__
#define __FUNC__ "PETScEquationSolver::sizeOf"
real PETScEquationSolver::
sizeOf( FILE *file /* =NULL */  )
// return number of bytes allocated
{
//  PetscFunctionBegin;

  FILE *outputFile = file==NULL ? stdout : file;

  real size=0.;
  MatInfo matInfo;
  if( Amx!=NULL )
  {
    ierr=MatGetInfo(Amx,MAT_GLOBAL_SUM,&matInfo); CHKERRQ(ierr);
  }

  // 2.2.1  PetscLogDouble space=0, fragments=0, maximumBytes=0, mem=0;
  PetscLogDouble mem=0;
  if( turnOnPETScMemoryTracing )
  {
    // 2.2.1 ierr = PetscTrSpace( &space, &fragments, &maximumBytes);  CHKERRQ(ierr);
    // 2.2.1 PetscGetResidentSetSize(&mem); //  maximum memory used
    PetscMemoryGetMaximumUsage(&mem);       //  maximum memory used

    size=mem;
  }
  else
  {
    size=matInfo.memory;
  }


  if( Oges::debug & 2 )
  {
    // 2.2.1  fprintf(outputFile,
    // 2.2.1          ">> PETSC: %e Kbytes in use.  (maximum used=%e Kbytes, total memory use=%e Kbytes)\n"
    // 2.2.1  	    "  matrix=%e Kbytes, fragments=%e\n",
    // 2.2.1  	    space*.001,maximumBytes*.001,mem*.001,matInfo.memory*.001,fragments);
    fprintf(outputFile,">> PETSC: maximum memory used=%e (Kbytes)\n",mem/1000);

  }

  if( Oges::debug & 4 )
  {
    // 2.2.1 PetscTrDump(stdout);
    PetscMallocDump(stdout);  // Dumps the allocated memory blocks to a file
  }

// --------------------THIS would be nice if it worked.
//   Mat *factoredMatrix;
//   PCGetFactoredMatrix(pc,factoredMatrix);
//   MatGetInfo(*factoredMatrix,MAT_GLOBAL_SUM,&matInfo);
//   printf("petsc: ILU matrix memory= %i \n",(int)matInfo.memory);

//  PetscFunctionReturn(size);
  return size;
}


//
//........................................................
//


static char _p_ov_help[]="PETSc being used by the Overture `Oges' equation solver.";

#undef __FUNC__
#define __FUNC__ "PETScEquationSolver::initializePetscKSP"
int PETScEquationSolver::
initializePetscKSP()
//..Initialize Petsc linear solver from local options
{
//  PetscFunctionBegin;
  #ifdef USE_PPP
  printf("\n\n *** PETScEquationSolver::ERROR: This should not be called in parallel! Use PETScSolver instead!\n");
  Overture::abort("error");
  #endif

  if( Oges::debug & 2 ) printf("***initializePetscKSP...\n");

  Overture::incrementReferenceCountForPETSc();

  if( petscInitialized )
  {
    return 0;
    //     PetscFunctionReturnVoid();
  }

  petscInitialized=TRUE;
  int ierr;
  int numProcs;
  // *wdh* if( !petscInitialized )
  // The following doesn't hurt, but better can be written to work with PetscFinalize
  if (!PetscInitializeCalled)
  {
    if( Oges::debug & 2 ) printf("Initialize PETSc, oges.argc=%i...\n",oges.argc);

    // *new* way 10026
    // int argc=0;
    // char **args=NULL;
    PetscInitialize(&oges.argc,&oges.argv,(char *)0,_p_ov_help);

    comm=PETSC_COMM_WORLD;  // PETSC_COMM_WORLD is only valid after PetscInitialize is called

    if( turnOnPETScMemoryTracing )
    {
      // Activate logging of PETSC's malloc call
      // *wdh* ierr = PetscSetUseTrMalloc_Private(); CHKERRQ(ierr);
      // ierr = PetscTrLog();                  CHKERRQ( ierr );
      //ierr = PetscMallocDumpSetLog();                  CHKERRQ( ierr );

      //To avoid error message, turn on the -malloc_log in .petscrc
      printF("...After Petsc Initialized...\n");
      PetscLogDouble space =0;
      ierr =  PetscMallocGetCurrentUsage(&space);   CHKERRQ(ierr);
      printF("Current space PetscMalloc()ed %i M\n",(int)(space/(1024.*1024.)));

      ierr =  PetscMallocGetMaximumUsage(&space);   CHKERRQ(ierr);
      printF("Max space PetscMalloced() %i M\n",(int)(space/(1024.*1024.)));

      ierr =  PetscMemoryGetCurrentUsage(&space);   CHKERRQ(ierr);
      printF("Current process memory %i M\n",(int)(space/(1024.*1024.)));

      ierr =  PetscMemoryGetMaximumUsage(&space);   CHKERRQ(ierr);
      printF("Max process memory %i M\n",(int)(space/(1024.*1024.)));
      ierr = PetscMallocDumpLog(stdout); CHKERRQ( ierr );
      //ierr =  PetscMallocDump(stdout);          CHKERRQ(ierr);
    }

  }
  // ..Check that we are running on 1 proc.
  ierr=  MPI_Comm_size(comm, &numProcs); CHKERRQ(ierr);
  if ( numProcs!=1 )
  {
    SETERRQ(comm,1,"This is a uniprocessor code ONLY!!");
  }

  // Add options to PETSc's list of options
  ListOfShowFileParameters & petscOptions = oges.parameters.petscOptions;
  std::list<ShowFileParameter>::iterator iter;
  for(iter = petscOptions.begin(); iter!=petscOptions.end(); iter++ )
  {
    ShowFileParameter & param = *iter;
    aString name; ShowFileParameter::ParameterType type; int ivalue; real rvalue; aString stringValue;
    param.get( name, type, ivalue, rvalue, stringValue );

    if( type==ShowFileParameter::stringParameter )
    {
      printF("PETScSolver::buildSolver: INFO: adding option=[%s] value=[%s]\n",(const char*)name,(const char*)stringValue);
      PetscOptionsSetValue(NULL,name,stringValue);
    }
    else
    {
      Overture::abort("error");
    }
  }

  // ..Create linear solver context, and init from params.

//  isMatrixAllocated=FALSE;
//  nzzAlloc=NULL;
//  dscale=NULL;

  ierr = KSPCreate(comm , &ksp); CHKERRQ(ierr);
  ierr = KSPGetPC(ksp, &pc);    CHKERRQ(ierr);

  // *wdh* 061111 -- these next were moved here from solve
  // set any parameters that have changed
  setPetscParameters();

  if( Oges::debug & 2 )
    cout << "...Set command line options\n";
  //.. allow use of command line arguments
  ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);

  return 0; // PetscFunctionReturnVoid();
}

#undef __FUNC__
#define __FUNC__ "PETScEquationSolver::setPetscParameters"
int PETScEquationSolver::
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
//  PetscFunctionBegin;

  if( parameters.solverMethod!=solverMethod )
  {
    KSPType krylovSpaceMethod;

    switch( parameters.solverMethod )
    {
    case OgesParameters::richardson:
      krylovSpaceMethod=KSPRICHARDSON;
      break;
    case OgesParameters::chebychev:
      krylovSpaceMethod=KSPCHEBYSHEV;
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
    if( false && parameters.solveForTranspose ) // this didn't seem to work
    {
      krylovSpaceMethod = KSPPREONLY;
      if (Oges::debug & 2)
        printf("Solution of transpose requires krylovSpaceMethod = KSPPREONLY\n");
    }
    if( Oges::debug & 2 )
      printf(" ********** PETScEquationSolver: set krylov space method ***********\n");

    ierr = KSPSetType(ksp, krylovSpaceMethod); CHKERRQ(ierr);
    solverMethod=parameters.solverMethod;
  }

  if( parameters.preconditioner!=preconditioner )
  {
    PCType  petscPreconditioner;    //  == PCILU
    isDHPreconditioner=false;
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
    case OgesParameters::DHILUPreconditioner:
      petscPreconditioner=PCSHELL;
      isDHPreconditioner=true;
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
    case OgesParameters::kspPreconditioner:
      petscPreconditioner=PCKSP;
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
    if( Oges::debug & 2 )
      printf(" ********** PETScEquationSolver: set preconditioner ***********\n");
    ierr = PCSetType(pc,      petscPreconditioner);  CHKERRQ(ierr);
    preconditioner=parameters.preconditioner;

  }

  if( parameters.matrixOrdering!=matrixOrdering &&
      parameters.preconditioner==OgesParameters::incompleteLUPreconditioner)
  {
    MatOrderingType  matOrdering;           //  == ORDER_RCM default;
    switch( parameters.matrixOrdering )
    {
    case OgesParameters::naturalOrdering:
      matOrdering=(char*)MATORDERINGNATURAL;
      break;
    case OgesParameters::nestedDisectionOrdering:
      matOrdering=(char*)MATORDERINGND;
      break;
    case OgesParameters::oneWayDisectionOrdering:
      matOrdering=(char*)MATORDERING1WD;
      break;
    case OgesParameters::reverseCuthillMcKeeOrdering:
      matOrdering=(char*)MATORDERINGRCM;
      break;
    case OgesParameters::quotientMinimumDegreeOrdering:
      matOrdering=(char*)MATORDERINGQMD;
      break;
    case OgesParameters::rowlengthOrdering:
      matOrdering=(char*)MATORDERINGROWLENGTH;
      break;
    default:
      printf("****WARNING**** Unknown matrix ordering PETSc\n");
      matOrdering=(char*)MATORDERINGNATURAL;
    }
    if( parameters.preconditioner==OgesParameters::incompleteLUPreconditioner ) // ******* fix other cases *****
    {
      if( Oges::debug & 2 )
	printf(" ********** PETScEquationSolver: set matrix ordering ***********\n");

      // 2.2.1 ierr = PCILUSetMatReordering(pc, matOrdering); CHKERRQ(ierr);
#if (PETSC_VERSION_MAJOR==3)
      ierr = PCFactorSetMatOrderingType(pc, matOrdering); CHKERRQ(ierr);
#else
      ierr = PCFactorSetMatOrdering(pc, matOrdering); CHKERRQ(ierr);
#endif
      matrixOrdering=parameters.matrixOrdering;
    }
    if( isDHPreconditioner )
    {
      if( Oges::debug & 2 )
	printf(" ******** PETScEquationSolver: can NOT set matrix reordering for DHILU ****\n");
    }

  }

  if( parameters.solverMethod==OgesParameters::gmres && parameters.gmresRestartLength!=gmresRestartLength )
  {
    ierr = KSPGMRESSetRestart(ksp, parameters.gmresRestartLength); CHKERRQ(ierr);
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

//   if( parameters.solveForTranspose )
//     dtol=DBL_MAX; // ** assume we are solving for the left null vector

  if( Oges::debug & 1 )
     printf(" PETScEquationSolver: rtol=%e, atol=%e, dtol=%e\n",rtol,atol,dtol);

  ierr = KSPSetTolerances(ksp, rtol, atol, dtol, maxits); CHKERRQ(ierr);

  if( parameters.numberOfIncompleteLULevels!=numberOfIncompleteLULevels &&
      parameters.preconditioner==OgesParameters::incompleteLUPreconditioner )
  {
    if( Oges::debug & 2 )
      printf(" ********** PETScEquationSolver: set ilu levels =%i ***********\n",parameters.numberOfIncompleteLULevels);

    // 2.2.1 ierr = PCILUSetLevels(pc, parameters.numberOfIncompleteLULevels);  CHKERRQ(ierr);
    // 2.2.1 ierr = PCILUSetFill(pc,   parameters.incompleteLUExpectedFill);    CHKERRQ(ierr);

    ierr = PCFactorSetLevels(pc, parameters.numberOfIncompleteLULevels);  CHKERRQ(ierr);
    ierr = PCFactorSetFill(pc,   parameters.incompleteLUExpectedFill);    CHKERRQ(ierr);

    numberOfIncompleteLULevels=parameters.numberOfIncompleteLULevels;
  }

#ifdef USE_DH_PRECONDITIONER
  dh_setParameters();
#endif

  //ierr = PCLUSetReuseReordering(pc, 1);          CHKERRQ(ierr);
  //   optionsChanged=TRUE;

//  PetscFunctionReturnVoid();

  return 0;
}


int PETScEquationSolver::
setPetscRunTimeParameters()
{
  const int myid=Communication_Manager::My_Process_Number;

  bool parametersHaveChanged=false;  // set to true if any parameters have changed

//   if( true ) return 0; // ************************************************************************ 051112 * temp

  // rtol : reduce residual by this factor.
  // atol : absolute tolerance
  // dtol : divergence detector
  //    convergence:  | r_k |_2 < max( rtol*| r_0 |_2, atol )

  double rtol=parameters.relativeTolerance>0. ? parameters.relativeTolerance : REAL_EPSILON*1000.;
  double atol=parameters.absoluteTolerance>0. ? parameters.absoluteTolerance :
              max( real(numberOfEquations),500.)*REAL_EPSILON;
  double dtol=parameters.maximumAllowableIncreaseInResidual;
  int maxits = parameters.maximumNumberOfIterations > 0 ?  parameters.maximumNumberOfIterations : 900;

//   if( parameters.solveForTranspose )
//     dtol=DBL_MAX; // ** assume we are solving for the left null vector

  if( Oges::debug & 4 )
     printF(" PETScSolver: rtol=%e, atol=%e, dtol=%e\n",rtol,atol,dtol);

  ierr = KSPSetTolerances(ksp, rtol, atol, dtol, maxits); CHKERRQ(ierr);

  return ierr;
}


#undef __FUNC__
#define __FUNC__ "PETScEquationSolver::getMaximumResidual"
real PETScEquationSolver::
getMaximumResidual()
{
//  PetscFunctionBegin;
  double rnorm;
  KSPGetResidualNorm(ksp,&rnorm);
  maximumResidual=rnorm;

  return maximumResidual;  //  PetscFunctionReturn(maximumResidual);
}

// =====================================================================================
// \brief Return the number of iterations used in the last solve.
// =====================================================================================
int PETScEquationSolver::
getNumberOfIterations() const
{
  return oges.numberOfIterations;
}


#undef __FUNC__
#define __FUNC__ "PETScEquationSolver::solve"
int PETScEquationSolver::
solve(realCompositeGridFunction & u,
      realCompositeGridFunction & f)
// ======================================================================================================
/// \brief Solve the equations.
// ======================================================================================================
{
//  PetscFunctionBegin;

  shouldUpdateMatrix=oges.refactor==TRUE;

  if( !petscInitialized )
  {
    initializePetscKSP();
  }
  if( !oges.initialized || oges.shouldBeInitialized )
  {
    shouldUpdateMatrix=TRUE;  // is this correct?
  }

  real timeBuild=getCPU();

//   // set any parameters that have changed
//   setPetscParameters();


//   if( Oges::debug & 2 )
//     cout << "...Set command line options\n";
//   //.. allow use of command line arguments
//   ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);

  if( shouldUpdateMatrix )
  {
    if( Oges::debug & 2 ) cout << "...Build Matrix\n";

    buildPetscMatrix();

    if( Oges::debug & 2 ) cout << "...Set operators\n";
    if ( oges.recomputePreconditioner )
    {
      // *v3.4.5
      ierr = KSPSetOperators(ksp,Amx,Amx);  CHKERRQ(ierr);
      // *** FIX ME: 3.6.1 is not working yet
      // *v3.6.1:
      // printF(" --PETSC-- KSPSetOperators ksp=%i\n",ksp);
      // ierr = KSPSetReusePreconditioner(ksp,PETSC_FALSE);  CHKERRQ(ierr);
      // ierr = KSPSetOperators(ksp,Amx,Amx);  CHKERRQ(ierr);
    }
    else
    {
      // *wdh* v2.3.2
      ierr = KSPSetOperators(ksp,Amx,Amx);  CHKERRQ(ierr);

      // *** FIX ME: 3.6.1 is not working yet
      // reuse the current preconditioner:
      // ierr = KSPSetReusePreconditioner(ksp,PETSC_TRUE);  CHKERRQ(ierr);
      // ierr = KSPSetOperators(ksp,Amx,Amx);  CHKERRQ(ierr);

    }

    if( parameters.rescaleRowNorms && matrixFormat==Oges::other )
    {
      ierr = KSPSetDiagonalScale(ksp,PETSC_TRUE); CHKERRQ(ierr);
    }
    oges.initialized=TRUE;
    oges.shouldBeInitialized=FALSE;

  }

  // set any run time parameters that may have changed (e.g. tolerances) *wdh* 061111
  setPetscRunTimeParameters();

  if( Oges::debug & 2 )
  {
    cout << "...Build RHS, numberOfEquations="  << numberOfEquations << endl;
  }
  buildRhsAndSolVector(u,f);
  timeBuild=getCPU()-timeBuild;

  PetscBool flg=PETSC_TRUE;  // the initial guess is non-zero
  ierr = KSPSetInitialGuessNonzero(ksp,flg); CHKERRQ(ierr);

  real time0=getCPU();
  if( Oges::debug & 2 ) cout << "...Preconditioner\n";
  ierr = setupPreconditioner( ksp,brhs,xsol );  CHKERRQ(ierr); // see below for this routine

  //  ierr = SLESSetUp(sles,brhs,xsol);      CHKERRQ(ierr);
  timePrecond=getCPU()-time0;

#ifdef USE_DH_PRECONDITIONER
  dh_start();
#endif

  time0=getCPU();
  if( Oges::debug & 2 ) cout << "...Actually solving...\n";
  oges.numberOfIterations=0;

  // ierr = SLESSolve(sles,brhs,xsol,&oges.numberOfIterations);CHKERRQ(ierr);

  ierr = KSPSolve(ksp,brhs,xsol);CHKERRQ(ierr);

  if( turnOnPETScMemoryTracing)
  {
    //To avoid error message, turn on the -malloc_log in .petscrc
    printF("...After one KSPSolve...\n");
    PetscLogDouble space =0;
    ierr =  PetscMallocGetCurrentUsage(&space);   CHKERRQ(ierr);
    printF("Current space PetscMalloc()ed %i M\n",(int)(space/(1024.*1024.)));

    ierr =  PetscMallocGetMaximumUsage(&space);   CHKERRQ(ierr);
    printF("Max space PetscMalloced() %i M\n",(int)(space/(1024.*1024.)));

    ierr =  PetscMemoryGetCurrentUsage(&space);   CHKERRQ(ierr);
    printF("Current process memory %i M\n",(int)(space/(1024.*1024.)));

    ierr =  PetscMemoryGetMaximumUsage(&space);   CHKERRQ(ierr);
    printF("Max process memory %i M\n",(int)(space/(1024.*1024.)));
    ierr = PetscMallocDumpLog(stdout);            CHKERRQ(ierr);
    //ierr =  PetscMallocDump(stdout);          CHKERRQ(ierr);
  }

  KSPConvergedReason reason;
  ierr = KSPGetConvergedReason(ksp,&reason);
  if( reason<0 )
  {
    // typedef enum {/* converged */
    //               KSP_CONVERGED_RTOL               =  2,
    //               KSP_CONVERGED_ATOL               =  3,
    //               KSP_CONVERGED_ITS                =  4,
    //               KSP_CONVERGED_STCG_NEG_CURVE     =  5,
    //               KSP_CONVERGED_STCG_CONSTRAINED   =  6,
    //               KSP_CONVERGED_STEP_LENGTH        =  7,
    //               /* diverged */
    //               KSP_DIVERGED_NULL                = -2,
    //               KSP_DIVERGED_ITS                 = -3,
    //               KSP_DIVERGED_DTOL                = -4,
    //               KSP_DIVERGED_BREAKDOWN           = -5,
    //               KSP_DIVERGED_BREAKDOWN_BICG      = -6,
    //               KSP_DIVERGED_NONSYMMETRIC        = -7,
    //               KSP_DIVERGED_INDEFINITE_PC       = -8,
    //               KSP_DIVERGED_NAN                 = -9,
    //               KSP_DIVERGED_INDEFINITE_MAT      = -10,
    //
    //               KSP_CONVERGED_ITERATING          =  0} KSPConvergedReason;

    printF("PETScEquationSolver:ERROR: Solution diverged! reason=%i: \n",(int)reason);
    printF("     KSP_DIVERGED_NULL                = -2,\n"
           "     KSP_DIVERGED_ITS                 = -3,\n"
           "     KSP_DIVERGED_DTOL                = -4,\n"
           "     KSP_DIVERGED_BREAKDOWN           = -5,\n"
           "     KSP_DIVERGED_BREAKDOWN_BICG      = -6,\n"
           "     KSP_DIVERGED_NONSYMMETRIC        = -7,\n"
           "     KSP_DIVERGED_INDEFINITE_PC       = -8,\n"
           "     KSP_DIVERGED_NAN                  = -9,\n"
           "     KSP_DIVERGED_INDEFINITE_MAT      = -10\n");
    printF("NOTE 1: to see more information turn on the '-info' PETSc option (e.g. in your .petscrc)\n");
    printF("NOTE 2: to avoid the divergence error '-4' you can set the Oges option 'maximum allowable increase in the residual' \n");
    // OV_ABORT("error");

    if( true ) // July 28, 2016 -- try this --
    {
      if( reason==-3 || reason==-4 || reason==-5 || reason==-6 )
      {
	printF("--PTSC-- KSP failed, try to solve again with zero initial guess...\n");
        PetscBool flg=PETSC_FALSE;  // the initial guess is ZERO
        ierr = KSPSetInitialGuessNonzero(ksp,flg); CHKERRQ(ierr);

        ierr = KSPSolve(ksp,brhs,xsol);CHKERRQ(ierr);

	ierr = KSPGetConvergedReason(ksp,&reason);
	if( reason<0 )
	{
	  printF("--PTSC--- SOLVE AGAIN: ERROR Solution diverged! reason=%i: \n",(int)reason);
	  printF("     KSP_DIVERGED_NULL                = -2,\n"
		 "     KSP_DIVERGED_ITS                 = -3,\n"
		 "     KSP_DIVERGED_DTOL                = -4,\n"
		 "     KSP_DIVERGED_BREAKDOWN           = -5,\n"
		 "     KSP_DIVERGED_BREAKDOWN_BICG      = -6,\n"
		 "     KSP_DIVERGED_NONSYMMETRIC        = -7,\n"
		 "     KSP_DIVERGED_INDEFINITE_PC       = -8,\n"
		 "     KSP_DIVERGED_NAN                  = -9,\n"
		 "     KSP_DIVERGED_INDEFINITE_MAT      = -10\n");
	  printF("NOTE 1: to see more information turn on the '-info' PETSc option (e.g. in your .petscrc)\n");
	  printF("NOTE 2: to avoid the divergence error '-4' you can set the Oges option 'maximum allowable increase in the residual' \n");
	}
	else
	{
	  printF("--PTSC-- Solve again WORKED!\n");

	}

      }

    }



  }


  PetscInt its;
  ierr = KSPGetIterationNumber(ksp,&its);CHKERRQ(ierr);
  // ierr = KSPGetIterationNumber(ksp,&oges.numberOfIterations);CHKERRQ(ierr);
  oges.numberOfIterations=its;

  if(Oges::debug & 4 )
   printF(" PETScEquationSolver::solve numberOfIterations=%i\n",oges.numberOfIterations);
  if( oges.numberOfIterations==0 )
    printF(" PETScEquationSolver::solve:WARNING numberOfIterations=0! Could be trouble for MG, maybe decrease atol\n");

//   ierr = KSPSolve(ksp,brhs,xsol);CHKERRQ(ierr);
//   ierr = KSPGetIterationNumber(ksp,&oges.numberOfIterations);CHKERRQ(ierr);
//   printF(" PETScEquationSolver: numberOfIterations=%i (SOLVE AGAIN)\n",oges.numberOfIterations);


  timeSolve=getCPU()-time0;

#ifdef USE_DH_PRECONDITIONER
  dh_stop();
  dh_solveTime= dh_getCpuTime();
#endif

  if( Oges::debug & 1 )
  {
    cout << "++Petsc TIMINGS (for "<<oges.numberOfIterations<<" its, "
	 <<  "size of matrix n = " << numberOfEquations << " ):\n";
    cout << " build="<<timeBuild;
    cout << ", precond="<<timePrecond;
    cout << ", solve="<<timeSolve<<", total="<< timeBuild+timePrecond+timeSolve <<"."<<endl;

    // View solver info; we could instead use the option -ksp_view
    // *wdh* KSPView(ksp,PETSC_VIEWER_STDOUT_WORLD);
  }

  // *Take soln --> stick it to Overture vector & Move soln to the grid.

  PetscScalar *soln;
  if( copyOfSolutionNeeded || (matrixFormat!=Oges::other && parameters.rescaleRowNorms)  )
  {
    real *ovSol;
    int i;
    ierr=VecGetArray(xsol,&soln);     CHKERRQ(ierr);
    ovSol= oges.sol.getDataPointer();

    for( i=0; i<oges.numberOfEquations; i++)   // can we avoid this copy?
      ovSol[i]=soln[i];

    if( Oges::debug & 32  )
    {
      printF("PETSC: sol: ");
      for( i=0; i<oges.numberOfEquations; i++)   // can we avoid this copy?
	printF("(%i,%7.3f)",i,soln[i]);
      printF("\n");

    }

  }

  // Save values from the extra equations *wdh* May 8, 2016
  if( oges.numberOfExtraEquations>0 )
  {
    if( !oges.dbase.has_key("extraEquationValues") )
    {
      oges.dbase.put<RealArray>("extraEquationValues");
    }

    RealArray & extraEquationValues = oges.dbase.get<RealArray>("extraEquationValues");
    extraEquationValues.redim(oges.numberOfExtraEquations);
    RealArray & sol = oges.sol;
    for( int i=0; i<oges.numberOfExtraEquations; i++ )
    {
      extraEquationValues(i)=sol(oges.extraEquationNumber(i)-1);
      // printF("--PETScEQ-- Extra equation %i: eqn=%d value=%12.4e\n",
      //        i,oges.extraEquationNumber(i)-1,extraEquationValues(i));

    }
  }

  // oges.solvingSparseSubset : is true if we are solving a smaller implicit system (e.g. interface eqns)
  if( !oges.solvingSparseSubset )
    oges.storeSolutionIntoGridFunction();

  if( copyOfSolutionNeeded  || (matrixFormat!=Oges::other && parameters.rescaleRowNorms) )
  {
    ierr=VecRestoreArray(xsol,&soln); CHKERRQ(ierr);
  }
  return ierr; //  PetscFunctionReturn(ierr);
}


void PETScEquationSolver::
getCsortWorkspace(int nWorkSpace00)
{
  if( nWorkSpace00>nWorkRow )
  {
    if( Oges::debug & 2 )
      cout << "+++(PetscOverture)Reallocating workrow for Csort,"
	   << " need "<<nWorkSpace00<<", currently "<<nWorkRow<<endl;
    delete [] iWorkRow;
    iWorkRow = new int[nWorkSpace00];
    assert( iWorkRow != NULL );
    nWorkRow = nWorkSpace00;
  }
}


//..Build Petsc MATRIX: rescale & prealloc the matrix
#undef __FUNC__
#define __FUNC__ "PETScEquationSolver::buildPetscMatrix"
int PETScEquationSolver::
buildPetscMatrix()
{
//  PetscFunctionBegin;

  //..Check that we're running on a single processor
  int numProcs;
  MPI_Comm_size(comm, &numProcs);
  if ( numProcs!=1 )
  {
    SETERRQ(comm,1,"This is a uniprocessor code ONLY!!");
  }


  //..Build Overture matrix
  bool allocateSpace = TRUE;
  bool factorMatrixInPlace = FALSE; // allocate space for matrix only


  if( matrixFormat==Oges::other )
  {
    // in this case formMatrix will directly supply PETSc with the matrix elements
    oges.formMatrix(numberOfEquations,numberOfNonzeros,Oges::other,allocateSpace,factorMatrixInPlace);

    //..Finish up Matrix Assembly, set updateMatrix flag
    ierr = MatAssemblyBegin(Amx,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
    ierr = MatAssemblyEnd(Amx,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

    shouldUpdateMatrix=FALSE;
  }
  else
  {
    // in this option the matrix will first be loaded into (ia,ja,a)

    if( !oges.solvingSparseSubset )
      oges.formMatrix(numberOfEquations,numberOfNonzeros,Oges::compressedRow,allocateSpace,factorMatrixInPlace);
    else
    {
      numberOfEquations=oges.numberOfEquations;
      numberOfNonzeros=oges.numberOfNonzeros;
    }

    aval= oges.a.getDataPointer();
    ia_=  oges.ia.getDataPointer();
    ja_=  oges.ja.getDataPointer();

    //..SORT the columns of the matrix:
    getCsortWorkspace(2*numberOfNonzeros+1);
    if( Oges::debug & 2 )
      cout << "+++(PetscOverture)Sorting the columns of the matrix...\n";

    OVCSORT( numberOfEquations, aval[0], ja_[0], ia_[0], iWorkRow[0] );

    //..Allocate Petsc matrix
    int & neq=numberOfEquations; // ..shorthand for convenience
    int & nnz=numberOfNonzeros;

    InsertMode insrtOrAdd; // for Petsc MatSetValues
    //.........ALWAYS get rid of old matrix!!
    //if ( isMatrixAllocated && (numberOfEquations!=neqBuilt) ) {
    // ..allocated wrong size for Matrix & vectors in Petsc
    if ( isMatrixAllocated )
    {
      ierr=MatDestroy( &Amx ); CHKERRQ( ierr );
      ierr=VecDestroy( &xsol ); CHKERRQ( ierr );
      ierr=VecDestroy( &brhs ); CHKERRQ( ierr );
      isMatrixAllocated=FALSE;
    }
    if (!isMatrixAllocated)
    {
      if( Oges::debug & 2 )
	printF("+++(PetscOverture)Allocating the matrix...\n");

      while ( numberOfEquations%parameters.blockSize != 0 ) parameters.blockSize--;

      PetscInt blockSize=parameters.blockSize;
      PetscBool optionWasSet;
      PetscOptionsGetInt(NULL,PETSC_NULL,"-mat_block_size",&blockSize,&optionWasSet);
      if( optionWasSet )
      {
        if( Oges::debug & 2 ) printF("PETScEquationSolver:Using -mat_block_size option: block size = %i\n"
               "                    This option over-rides the value in OgesParameters\n",blockSize);
      }
      else
      {
	blockSize=parameters.blockSize;
        if( Oges::debug & 2 ) printF("PETScEquationSolver:Using blockSize=%i from OgesParameters\n",blockSize);
      }

      preallocRowStorage(blockSize); assert(nzzAlloc!=NULL);

      if( blockSize==1 )
      {
	ierr = MatCreateSeqAIJ(PETSC_COMM_SELF,neq,neq,numberOfNonzeros,nzzAlloc,&Amx); CHKERRQ(ierr);
	//ierr = MatCreate(PETSC_COMM_SELF,neq,neq,&Amx);
      }
      else
      {
	// for block matrices:
	// bs=block size
	// nz=number of non-zero blocks per block row (if same per row)
	// nnz[] =number of non-zero blocks per block row (different for each row)

        if( Oges::debug & 2 )
          printF("\n *** PETScEquationSolver: build a block matrix BAIJ with blockSize=%i **** \n",blockSize);

	int nz=0;  // not used ?
	ierr = MatCreateSeqBAIJ(PETSC_COMM_SELF,blockSize,neq,neq,nz,nzzAlloc,&Amx); CHKERRQ(ierr);
      }


      if( Oges::debug & 2 )
      {
        printF("PETScEquationSolver::buildPetscMatrix: Allocate xsol and brhs using VecCreateSeq\n");
      }


      ierr = VecCreateSeq(PETSC_COMM_SELF,neq,&xsol); CHKERRQ(ierr);
      ierr = VecCreateSeq(PETSC_COMM_SELF,neq,&brhs); CHKERRQ(ierr);

      isMatrixAllocated=TRUE;
      neqBuilt=neq; //..remember size of current matrix
      insrtOrAdd=INSERT_VALUES;
    }
    //...NOT USED ANYMORE, 'ADD_VALUES' leads to SuperSLOW MxBUILD?!!!
    //else {       //..We have a matrix of right size, just reset it
    //  cout << "+++++++++ -------- USE OLD MATRIX ----- +++++++\n";
    //  ierr = MatZeroEntries(Amx); CHKERRQ( ierr );
    //  insrtOrAdd=ADD_VALUES;
    //}

    //..Loop through the Overture matrix to build it in Petsc
    computeDiagScaling();
    int irowm1, j,jcolm1;
    real dsc;
    PetscScalar  v;
    // real rownorm;

    const real coeffScale=1.;  // fix this
    real eps= parameters.rescaleRowNorms ? coeffScale*REAL_EPSILON*100. : 0.;  // cutoff tolerance for keeping coefficients

    if( Oges::debug & 2 )
      cout << "+++(PetscOverture)Building the Petsc matrix...\n";
    for( irowm1=0; irowm1<neq; irowm1++ ) {
      dsc=dscale[irowm1];
      // rownorm=0.0;
      for( j=ia_[irowm1]; j<=ia_[irowm1+1]-1; j++ )
      {
	v=dsc*aval[ j-1 ];
	if( fabs(v) <eps ) continue;   // *wdh* May 23, 2015 -- to catch small user supplied eqn coeffs

	jcolm1=ja_[ j-1 ]-1;
	// rownorm = rownorm+ fabs(v);
	// To matrix Amx, Insert row=irow, col=jcol, INSERT (NEW) VALUE
	ierr=MatSetValues(Amx,1,&irowm1,1,&jcolm1,&v,insrtOrAdd); CHKERRQ(ierr);
      }
    }
    //..Finish up Matrix Assembly, set updateMX flag
    ierr = MatAssemblyBegin(Amx,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
    ierr = MatAssemblyEnd(Amx,MAT_FINAL_ASSEMBLY);   CHKERRQ(ierr);
    shouldUpdateMatrix=FALSE;

    if( !parameters.keepSparseMatrix )
    {
      #ifndef __clang__
        // trouble here with clang on the Mac:
        oges.ia.redim(0);       // these are no longer needed.
	oges.ja.redim(0);
	oges.a.redim(0);
      #endif

    }
  }

  if( Oges::debug & 4 )
  {
    printF("============= PETScSolver: Here is the sparse matrix ==============\n");
    ierr = MatView(Amx,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);
  }

  if( false )
  { // save the matrix in a file that can me read in matalb by typing 'petscMatrix'
    printf("Saving matrix in matlab format file=`petscMatrix.m'\n");
    PetscViewer viewer;
    PetscViewerASCIIOpen(PETSC_COMM_WORLD,"petscMatrix.m",&viewer);
    PetscViewerSetFormat(viewer,PETSC_VIEWER_ASCII_MATLAB);
    ierr = MatView(Amx,viewer);CHKERRQ(ierr);

  }

  return 0;

}

void PETScEquationSolver::
preallocRowStorage(int blockSize)
// ===================================================================================================
// Determine the number of entries in each row so that we can tell petsc how much space is needed
//
// If the blockSize>1 then we need to determine how many blocks are needed in the row-blocks
// ===================================================================================================
{
  int i,j;
  int & neq=numberOfEquations; // shorthand
  int & nnz=numberOfNonzeros;


  delete [] nzzAlloc;

  assert( neq % blockSize == 0 );  // sanity check

  int numBlocks = neq/blockSize;

  nzzAlloc = new int [numBlocks]; assert( nzzAlloc != NULL );

  if( blockSize==1 )
  {
    for( i=0; i<neq; i++)
    {
      assert( (ia_[i]<=nnz+1) && (ia_[i+1]<=nnz+1) );
      nzzAlloc[i]=ia_[i+1]-ia_[i];
    }
  }
  else
  {
    // count the number of blocks in each row-block
    int *blockIsUsed = new int[numBlocks];
    for( int jb=0; jb<numBlocks; jb++ ) blockIsUsed[jb]=0;

    i=0;
    for( int b=0; b<numBlocks; b++,i+=blockSize )
    {
      // look at all the rows in this row-block b
      for ( int k=0; k<blockSize; k++ )
      {
	assert( (ia_[i+k]<=nnz+1) && (ia_[i+k+1]<=nnz+1) );

	int j1=ia_[i+k], j2=ia_[i+k+1]-1;
	for( int j=j1; j<=j2; j++ )
	{
	  int jblock = (ja_[j-1]-1)/blockSize;   // there is an entry in block "jblock"
	  blockIsUsed[jblock]=1;           // mark this block as used

	  // printf(" block=%i row=i+k=%i j=%i ja-1=%i jblock=%i\n",b,i+k,j,ja_[j]-1,jblock);

	}
      }
      int numberOfBlocksThisRow=0;  // count blocks
      for( int jb=0; jb<numBlocks; jb++ )
      {
	numberOfBlocksThisRow+=blockIsUsed[jb];
	blockIsUsed[jb]=0;  // reset for next row-block
      }

      nzzAlloc[b]=numberOfBlocksThisRow;
      // printf(" preallocRowStorage: block=%i number-of-nonzero-blocks = %i \n",b,nzzAlloc[b]);

    }
    delete [] blockIsUsed;

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

  delete [] dscale;
  dscale = new real [neq]; assert( dscale != NULL );

  if (!parameters.rescaleRowNorms)
  {     //..... don't scale the rows in the Matrix
    if( Oges::debug & 4 ) cout << "+++++++++++ NOT SCALING ROWS +++++++++++\n";
    for( i=0; i<neq; i++)
      dscale[i]=1.0;
  }
  else
  {                 //..... scale the rows
    if( Oges::debug & 4 ) cout << "+++++++++++ SCALING the ROWS +++++++++++\n";
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
#undef __FUNC__
#define __FUNC__ "PETScEquationSolver::buildRhsAndSolVector"
int PETScEquationSolver::
buildRhsAndSolVector(realCompositeGridFunction & u,
		     realCompositeGridFunction & f)
// ===========================================================================================================
/// \brief Transfer the right-hand-side (f) and initial guess (u) to the PETSc vectors
// ===========================================================================================================
{
  if( Oges::debug & 2 )
    printF("PETScEquationSolver::buildRhsAndSolVector:START: rescaleRowNorms=%i, copyOfSolutionNeeded=%i\n",
	   (int)parameters.rescaleRowNorms,(int)copyOfSolutionNeeded);

  if (!isMatrixAllocated)
  {
    cout << "---------ERROR(petscOverture): Can't build RHS" << " if it is not allocated! ..Exiting..\n";
    exit(-77);
  }
  if( matrixFormat!=Oges::other )
    assert( dscale!=NULL );

  if( !oges.solvingSparseSubset )
  {
    ierr=oges.formRhsAndSolutionVectors(u,f); assert(ierr==0);
  }

  real *ovSol, *ovRhs; // Overture solution and rhs
  ovSol= oges.sol.getDataPointer();
  ovRhs= oges.rhs.getDataPointer();

  if( copyOfSolutionNeeded || (matrixFormat!=Oges::other && parameters.rescaleRowNorms) )
  {
    if( Oges::debug & 2 )
      printF("PETScEquationSolver::buildRhsAndSolVector ++++ scale RHS ++++ rescaleRowNorms=%i, copyOfSolutionNeeded=%i\n",
	     (int)parameters.rescaleRowNorms,(int)copyOfSolutionNeeded);


    PetscScalar v;
    int i;
    for( i=0; i<numberOfEquations; i++)
    {
      v=ovSol[i];
      ierr=VecSetValues(xsol,1,&i,&v,INSERT_VALUES); CHKERRQ(ierr);

      v=ovRhs[i]*dscale[i]; // SCALE rhs as the matrix!!
      ierr=VecSetValues(brhs,1,&i,&v,INSERT_VALUES); CHKERRQ(ierr);

      if( false && Oges::debug & 2 )
        printF("--PES-- RHS: i=%6i, ovRhs=%11.4e scale=%9.2e b=%11.4e\n",i,ovRhs[i],dscale[i],v);

    }
  }
  else
  {
#ifdef OV_USE_DOUBLE
    if( Oges::debug & 2 )
    {
      printF("***PETScEquationSolver::buildRhsAndSolVector: Create xsol and brhs using VecCreateSeqWithArray\n");
    }

    int n = oges.rhs.elementCount()-1;
    int blockSize=parameters.blockSize; // is this right?
    ierr = VecCreateSeqWithArray(comm,blockSize,n,ovRhs,&brhs);CHKERRQ(ierr);
    ierr = VecCreateSeqWithArray(comm,blockSize,n,ovSol,&xsol);CHKERRQ(ierr);
#endif
  }

  // Insert initial guess for extra equations *wdh* July 29, 2016
  if( oges.numberOfExtraEquations>0 )
  {
    if( oges.dbase.has_key("extraEquationInitialValues") )
    {
      RealArray & extraEquationInitialValues = oges.dbase.get<RealArray>("extraEquationInitialValues");
      PetscScalar v;

      for( int j=0; j<oges.numberOfExtraEquations; j++ )
      {
	v = extraEquationInitialValues(j);
        int i = oges.extraEquationNumber(j)-1;  // **check me**
	printF("--PES-- extra equation %i: set initial value: eqn=%i value=%12.4e [ovSol=%12.4e]\n",j,i,v,ovSol[i]);
        ierr=VecSetValues(xsol,1,&i,&v,INSERT_VALUES); CHKERRQ(ierr);
      }
    }
    else
    {
      for( int j=0; j<oges.numberOfExtraEquations; j++ )
      {
        int i = oges.extraEquationNumber(j)-1;  // **check me**
	printF("--PES-- extra equation %i: eqn=%i guess in ovSol=%12.4e\n",j,i,ovSol[i]);
      }
    }


  }

  ierr = VecAssemblyBegin(xsol); CHKERRQ(ierr);
  ierr = VecAssemblyBegin(brhs); CHKERRQ(ierr);
  ierr = VecAssemblyEnd(xsol);   CHKERRQ(ierr);
  ierr = VecAssemblyEnd(brhs);   CHKERRQ(ierr);

  return 0;
}


#undef __FUNC__
#define __FUNC__ "PETScEquationSolver::allocateMatrix"
int PETScEquationSolver::
allocateMatrix(int ndia,int ndja,int nda,int N)
{
//  PetscFunctionBegin;

  if (isMatrixAllocated)
  {
    ierr = MatDestroy(&Amx);CHKERRQ(ierr);
    ierr = VecDestroy(&xsol);CHKERRQ(ierr);
    ierr = VecDestroy(&brhs);CHKERRQ(ierr);
    isMatrixAllocated = FALSE;
  }
  if (!isMatrixAllocated)
  {
    if (Oges::debug & 2)
    {
      cout << "+++(PetscOverture)Allocating the matrix, matrixFormat=" << matrixFormat << "...\n";
    }
    ierr = MatCreateSeqAIJ(comm,N,N,nda/N+1+1+1+1,PETSC_NULL,&Amx);CHKERRQ(ierr);

    isMatrixAllocated = TRUE;
    neqBuilt = N; //..remember size of current matrix
  }

  return ierr; //  PetscFunctionReturn(ierr);
}

#undef __FUNC__
#define __FUNC__ "PETScEquationSolver::setMatrixElement"
int PETScEquationSolver::
setMatrixElement(int nzcounter,int i,int j,real val)
{
//  PetscFunctionBegin;

  i--;
  j--;
#ifdef OV_USE_DOUBLE
  ierr = MatSetValues(Amx,1,&i,1,&j,&val,INSERT_VALUES);CHKERRQ(ierr);
#else
  PetscScalar sval=val;
  ierr = MatSetValues(Amx,1,&i,1,&j,&sval,INSERT_VALUES);CHKERRQ(ierr);
#endif

  return ierr; //  PetscFunctionReturn(ierr);
}

#undef __FUNC__
#define __FUNC__ "PETScEquationSolver::displayMatrix"
int PETScEquationSolver::
displayMatrix()
{
//  PetscFunctionBegin;

  ierr = MatView(Amx,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(ierr);

  return 0;

}

//
//  saveBinaryMatrixAndRhs -- save current PETSc matrix & rhs
//   NOTE: 1) the fileformat is probably not portable across platforms
//         2) these matrices are a lot smaller than saving as text
//
int
PETScEquationSolver::
saveBinaryMatrix(aString filename00,
		 realCompositeGridFunction & u,
		 realCompositeGridFunction & f)
{
  shouldUpdateMatrix=oges.refactor==TRUE;

  if( !petscInitialized )
  {
    initializePetscKSP();
    if( Oges::debug & 2 )
      cout << "...Set command line options\n";
    //.. allow use of command line arguments
    ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);
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
    if( Oges::debug & 2 ) cout << "...Build Matrix\n";
    buildPetscMatrix();
    if( Oges::debug & 2 ) cout << "...Set operators\n";
    ierr = KSPSetOperators(ksp,Amx,Amx);
    // *v3.6.1:
    // ierr = KSPSetOperators(ksp,Amx,Amx);
    CHKERRQ(ierr);

    oges.initialized=TRUE;
    oges.shouldBeInitialized=FALSE;
  }

  PetscViewer viewer;   // Petsc viewer for saving the matrix

  buildRhsAndSolVector( u, f );

  // 2.2.1 ierr = PetscViewerBinaryOpen( comm, filename00, PETSC_FILE_CREATE , &viewer);
  ierr = PetscViewerBinaryOpen( comm, filename00, FILE_MODE_WRITE , &viewer);

  CHKERRQ( ierr );
  ierr = MatView( Amx,  viewer);  CHKERRQ( ierr );
  ierr = VecView( brhs, viewer);  CHKERRQ( ierr );
  ierr = PetscViewerDestroy( &viewer ); CHKERRQ( ierr );

  return 0;
}



//
// ..setup preconditioner -- should only be used when matrix
//   has changed!!
//
int
PETScEquationSolver::
setupPreconditioner(KSP ksp00, Vec brhs00, Vec xsol00 )
{
#ifndef USE_DH_PRECONDITIONER
  if( Oges::debug & 2 )
    cout << "+++(PetscOverture)SetupPreconditioner...\n";
  // return( KSPSetUp(ksp00,brhs00,xsol00) );
  int ierr;
  // ierr=KSPSetRhs(ksp,brhs00);CHKERRQ(ierr);
  // ierr=KSPSetSolution(ksp,xsol00);CHKERRQ(ierr);
  ierr=KSPSetUp(ksp);CHKERRQ(ierr);
  return ierr;
#else
  int ierr;
  if ( isDHPreconditioner )
  {
    // NOTE: the CHKERRQ(ierr)'s should be left for the calling prog!!
    if (parameters.incompleteLUTypeInDH )
    {
      if ( dh_ctx != NULL ) MyILU_destroy( dh_ctx );
      ierr = MyILU_create(&dh_ctx);       CHKERRQ(ierr);
      dh_initialize();
      dh_setParameters();
      //dh_ctx->n = numberOfEquations;  //NOT NEEDED in DH_V3
      //dh_ctx->A = Amx;                // ditto
      ierr = PCSetType(pc,PCSHELL);       CHKERRQ(ierr);
      ierr = PCShellSetApply(pc,MyILU_apply,(void*)dh_ctx); CHKERRQ(ierr);

      /* "extracts" (i.e, copies) matrix from A, and
	 optionally applies sparcification */
      ierr = MyILU_setup(dh_ctx, Amx);  CHKERRQ(ierr);

      dh_start();
      ierr = MyILU_factor(dh_ctx); CHKERRQ(ierr);
      dh_stop();
      dh_setupTime = dh_getCpuTime();
    }
    else
    {
      dh_start();
      ierr = SLESSetUp(sles00,brhs00,xsol00);      CHKERRQ(ierr);
      dh_stop();
      dh_setupTime = dh_getCpuTime();
    }
  }
  else ierr = SLESSetUp(sles00,brhs00,xsol00);

  return( ierr );
#endif
}


//
// ----------- Experimental interface to the Hysom/Chow preconditioners
// --------------> DHILU
//

void
PETScEquationSolver::dh_initialize()
{
#ifndef USE_DH_PRECONDITIONER
  cout << "ERROR --"
       << " PETScEquationSolver::dh_initialize called -- Not Available!"
       << endl;
  cout << "      -- Compile with -DUSE_DH_PRECONDITIONER"<<endl;
#else

  // need to rescale rows for sparsification --> not needed, DH-PC rescales
  //oges.set( OgesParameters::THErescaleRowNorms, true );
#endif
}

void
PETScEquationSolver::dh_setParameters()
{
#ifndef USE_DH_PRECONDITIONER
  cout << "ERROR --"
       << " PETScEquationSolver::dh_setParameters called -- Not Available!"
       << endl;
  cout << "      -- Compile with -DUSE_DH_PRECONDITIONER"<<endl;
#else

  if( isDHPreconditioner )
  {
    if ( dh_ctx != NULL )
    {
      dh_ctx->pcType     = parameters.incompleteLUTypeInDH;
      dh_ctx->level      = parameters.numberOfIncompleteLULevels;
      dh_ctx->droptol    = parameters.incompleteLUDropTolerance;
      dh_ctx->sparseTolA = parameters.incompleteLUSparseACoefficient;
      dh_ctx->sparseTolF = parameters.incompleteLUSparseFCoefficient;
    }
    else if ( (dh_ctx ==NULL) && (Oges::debug & 2) )
    {
      cout << "PETScEquationSolver::dh_setParameters WARNING:"
	   << " DH Preconditioner is  not initialized. \n";
    }
  }
#endif
}

void
PETScEquationSolver::dh_computeResidualReduction( double & residReduction )
{
#ifndef USE_DH_PRECONDITIONER
  cout << "ERROR --"
       << " PETScEquationSolver::dh_computeResidualReduction called --"
       << " Not Available!"
       << endl;
  cout << "      -- Compile with -DUSE_DH_PRECONDITIONER"<<endl;
  residReduction = 0.;
#else

  double r_norm, b_norm;
  double neg_one=-1.;
  Vec residVec;
  int ierr;
  ierr = VecDuplicate(brhs,&residVec); CHKERRQ(ierr);
  ierr = VecNorm(brhs,  NORM_2, &b_norm); CHKERRQ(ierr);/* starting residual norm, since x_init=0 */
  ierr = MatMult(Amx, xsol, residVec); CHKERRQ(ierr);
  ierr = VecAXPY(&neg_one, brhs, residVec); CHKERRQ(ierr);       /* u is final residual */
  ierr = VecNorm(residVec,  NORM_2, &r_norm); CHKERRQ(ierr);
  residReduction = r_norm / b_norm;

#endif
}

#endif /*  OVERTURE_USE_PETSC */
