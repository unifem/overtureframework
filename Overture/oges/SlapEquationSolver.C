#include "SlapEquationSolver.h"
#include "SparseRep.h"


#define CGESL3C EXTERN_C_NAME(cgesl3c)

extern "C"
{
  void CGESL3C( int & id, real & rd, int & job, int & neq, int & nze, int & ndia, 
		int & ia, int & ndja, int & ja, int & nda, real & a, real & sol, real & rhs, 
		int & ndiwk, int & iwk, int & ndwk, real & wk, int & nit, real & tol, 
		int & icg, int & ipc, int & debug, int & iter, real & resmx, int & nsave, int & ierr );
}

SlapEquationSolver::
SlapEquationSolver(Oges & oges_)  : EquationSolver(oges_) 
{
  name="SLAP";

  solverJob=7;
}

SlapEquationSolver::
~SlapEquationSolver()
{
}


real SlapEquationSolver::
sizeOf( FILE *file /* =NULL */  )
// return number of bytes allocated 
{
  real size=(iwk.elementCount()*sizeof(int)+
	     wk.elementCount()*sizeof(real));
  return size;
}


int SlapEquationSolver::
solve(realCompositeGridFunction & u,
      realCompositeGridFunction & f)
{
  if( Oges::debug & 1 ) 
    printf("SlapEquationSolver: solve: solverJob=%i, oges.initialized=%i, oges.shouldBeInitialized=%i \n",
	   solverJob,oges.initialized,oges.shouldBeInitialized);

  int ierr;
  if( !oges.initialized || oges.shouldBeInitialized )
  {
    if( Oges::debug & 1 ) cout << " *** SlapEquationSolver::solve: initialize... ****\n";
    solverJob=7;  
  }
  
  int shouldUpdateMatrix= (solverJob & 2) || oges.refactor==TRUE;

  
  double timeBuild=getCPU();
  if( shouldUpdateMatrix ) 
  {
    if( Oges::debug & 1 ) cout << "...Build Matrix\n";
    //..build compressed-row matrix ia,ja,a
    // solverJob=7; // *********************************************
    // oges.initialized=FALSE;


    bool allocateSpace = TRUE;           // fix these ********************
    bool factorMatrixInPlace = FALSE;    // allocate space for non-zeros only

    int newNumberOfEquations, newNumberOfNonzeros;
    oges.formMatrix(newNumberOfEquations,newNumberOfNonzeros,
                    Oges::uncompressed,allocateSpace,factorMatrixInPlace);

    bool allocate=( // (solverJob & 1) || 
		    newNumberOfEquations!=numberOfEquations ||
		    ( fabs(newNumberOfNonzeros-numberOfNonzeros)/(1.+numberOfNonzeros) > .1 ) );
    
    numberOfEquations=newNumberOfEquations;
    numberOfNonzeros=newNumberOfNonzeros;
    
    if( allocate )
    {
      allocateWorkSpace();
    }
    
    oges.initialized=TRUE;
  }

  if( Oges::debug & 1 ) cout << "...Build RHS\n";
  // convert grid function data to a single long vector
  oges.formRhsAndSolutionVectors(u,f); 

  timeBuild=getCPU()-timeBuild;

  real time0=getCPU();
  if( Oges::debug & 1 ) cout << "...Actually solving...\n";
  oges.numberOfIterations=0;
  ierr = solve();
  real timeSolve=getCPU()-time0;
  if( Oges::debug & 1 ) 
  {
    cout << "++Slap TIMINGS \n";
    cout << " build="<<timeBuild;
    cout << ", solve="<<timeSolve<<"."<<endl;
  }
  
  // convert vectors back to grid functions
  oges.storeSolutionIntoGridFunction();

  return ierr;
}

int SlapEquationSolver::
allocateWorkSpace()
// ==========================================================================
// /Description:
//   Allocate work space needed by Slap. This is only a guess.
// ==========================================================================
{
  //c.........SLAP conjugate gradient routines
  //c            nel=lower triangle+diag
  //c            nu = upper triangle+diag
  int leniw, lenw;
  
  switch (parameters.solverMethod)
  {
  case OgesParameters::biConjugateGradient:
    switch (parameters.preconditioner)
    {
    case OgesParameters::noPreconditioner:
      //c           ssdbcg:
      //c         ... rwork:  lenw >= 8*n.
      //c         ... iwork: leniw >= 10
      //c             storage: iwk=2*neq+2*nqs+10  rwk=2*neq+nqs+8*neq
      lenw =8*numberOfEquations + 10;
      leniw=20;
      break;
    default:
      //c           sslubc:
      //c         ... rwork:  lenw >= nel+nu+8*n
      //c         ... iwork: leniw >= nel+nu+4*n+12
      //c             storage: iwk=2*neq+2*nqs+9neq+nze  rwk=2*neq+nqs+5neq+nze
      lenw =numberOfNonzeros+9*numberOfEquations;
      leniw=numberOfNonzeros+5*numberOfEquations+12;
    }
    break;
      
  case OgesParameters::biConjugateGradientSquared:
    switch (parameters.preconditioner)
    {
    case OgesParameters::noPreconditioner:
      //c           ssdcgs:
      //c         ... rwork:  lenw >= 8*n.
      //c         ... iwork: leniw >= 10
      //c             storage: iwk=2*neq+2*nqs  rwk=2*neq+nqs+8neq
      lenw =8*numberOfEquations +10;
      leniw=20;
      break;
    default:
      //c           sslucs:
      //c         ... rwork:  lenw >= nel+nu+8*n
      //c         ... iwork: leniw >= nel+nu+4*n+12
      //c             storage: iwk=2*neq+2*nqs+9neq+nze  rwk=2*neq+nqs+5neq+nze
      lenw =numberOfNonzeros+9*numberOfEquations;
      leniw =numberOfNonzeros+5*numberOfEquations+12;
      break;
    }
    break;

  case OgesParameters::gmres:
    //c         ...GMRES
    switch (parameters.preconditioner)
    {
    case OgesParameters::noPreconditioner:
      //c           ssdgmr:
      //c         ... rwork:  lenw >= 1+(nsave+7)*n.
      //c         ... iwork: leniw >= 30
      //C         Length of the real workspace, RWORK.  LENW >= 1 + N*(NSAVE+7)
      //C         + NSAVE*(NSAVE+3).
      lenw =1+(parameters.gmresRestartLength+7)*numberOfEquations+
              parameters.gmresRestartLength*(parameters.gmresRestartLength+3)+1;
      leniw=31;
      break;
    default:
      //c           sslugm:
      //c         ... rwork:  lenw >= 1 + n*(nsave+7)+nsave*(nsave+3)+nel+nu
      //c         ... iwork: leniw >= nel+nu+4*n+32
      //C         Length of the real workspace, RWORK. LENW >= 1 + N*(NSAVE+7)
      //C         +  NSAVE*(NSAVE+3)+NEL+NU.

      lenw =numberOfNonzeros+(parameters.gmresRestartLength+8)*numberOfEquations
	+parameters.gmresRestartLength*(parameters.gmresRestartLength+3)+1;
      leniw=numberOfNonzeros+5*numberOfEquations+32;
      break;
    }
    break;      
  default:
    {
      cerr << " SlapEquationSolver: allocate: error unknown value for solverMethod=" 
           << (int)parameters.solverMethod << endl;
      Overture::abort("SlapEquationSolver:ERROR");
    }
  }

  printf("SlapEquationSolver: allocate: numberOfEquations=%i, numberOfNonzeros=%i, lenw=%i, leniw=%i\n"
         "                    RestartLength=%i \n",
	 numberOfEquations,numberOfNonzeros,lenw,leniw,parameters.gmresRestartLength);

  if( iwk.getLength(axis1) < leniw )
  {
    ndiwk=leniw;
    iwk.redim(ndiwk+1);
  }
  if( wk.getLength(axis1) < lenw )
  {
    ndwk=lenw;
    wk.redim(ndwk+1);
  }

  return 0;
}


int SlapEquationSolver::
solve()
//=====================================================================
// call the slap solver
//
//
//=====================================================================
{
  int ierr=0;
  assert( wk.getLength(0)>0 );
  
  maximumResidual=0.;
  numberOfIterations=0;
  
  int id0;  // dummy disk arrays
  real rd0;
  
  //      Default value for job
  //        1=create work spaces
  //        2=generate matrix and factor
  //        4=re-order (if available)
  if( oges.refactor )
  {
    solverJob |= 2;
    if( oges.reorder )
      solverJob|=4;
  }
  if( parameters.solveForTranspose )
    solverJob |= 32;

  if( Oges::debug & 4 )
    cout << "callSparseSolver: calling SLAP solver CGESL3..." << endl;

  //   ...convergence tolerance is proportional to the number
  //      of grid points in one direction (?)
  //      and must be at least 500*drmach(3)
  real tolerance=parameters.relativeTolerance;
  if( tolerance <= 0. )
    tolerance=max( real(numberOfEquations),500.)*REAL_EPSILON;
    
  int maximumNumberOfIterations=parameters.maximumNumberOfIterations;
  if( maximumNumberOfIterations <= 0 )
    maximumNumberOfIterations=int( 6*pow(real(numberOfEquations),.5) );

    // on input numberOfIterations = minimum number of iterations
  numberOfIterations=parameters.minimumNumberOfIterations;

  int conjugateGradientType=(parameters.solverMethod==OgesParameters::biConjugateGradient ? 0 :
			     parameters.solverMethod==OgesParameters::biConjugateGradientSquared ? 1 :
			     2);
  int conjugateGradientPreconditioner=(parameters.preconditioner==OgesParameters::noPreconditioner ? 0 :
				       parameters.preconditioner==OgesParameters::diagonalPreconditioner ? 1 :
				       parameters.preconditioner==OgesParameters::incompleteLUPreconditioner ? 2 :
				       3);

  CGESL3C( id0,rd0,solverJob,numberOfEquations,numberOfNonzeros,
	   oges.ndia,*oges.ia.getDataPointer(),
           oges.ndja,*oges.ja.getDataPointer(),
           oges.nda,*oges.a.getDataPointer(),
           *oges.sol.getDataPointer(),*oges.rhs.getDataPointer(),
	   ndiwk,*iwk.getDataPointer(),ndwk,*wk.getDataPointer(),
	   maximumNumberOfIterations,
	   tolerance,conjugateGradientType,
	   conjugateGradientPreconditioner,Oges::debug,
	   numberOfIterations,maximumResidual,parameters.gmresRestartLength,ierr );

  oges.numberOfIterations=numberOfIterations;
  
  if( ierr < 0 )
  {
    cout << "Warning from CGESL3, ierr=" << ierr << endl;
    ierr=0;
  }
  else if( ierr > 0 )
  {
    cerr << "Error return from CGESL3, ierr=" << ierr << endl;
    if( ierr==2 )
    {
      cerr << "ierr=2: no convergence!\n";
    }
    exit(1);
  }

  // Save values from the extra equations *wdh* May 12, 2016
  if( oges.numberOfExtraEquations>0 )
  {
    if( !oges.dbase.has_key("extraEquationValues") )
    {
      oges.dbase.put<RealArray>("extraEquationValues");
    }

    RealArray & extraEquationValues = oges.dbase.get<RealArray>("extraEquationValues");
    extraEquationValues.redim(oges.numberOfExtraEquations);
    realArray & sol = oges.sol;
    for( int i=0; i<oges.numberOfExtraEquations; i++ )
    {
      extraEquationValues(i)=sol(oges.extraEquationNumber(i)-1);

      // printF("Slap: setting extra equation %i eqn=%i to %12.4e\n",
      //            i,oges.extraEquationNumber(i),extraEquationValues(i));
    }
  }

  solverJob=0;
  
  return ierr;
}
