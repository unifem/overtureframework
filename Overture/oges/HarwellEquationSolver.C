#include "HarwellEquationSolver.h"
#include "SparseRep.h"


#define CGESL2C EXTERN_C_NAME(cgesl2c)

extern "C"
{
  void CGESL2C( int & id, real & rd, int & job, int & neq, int & nze, int & nqs, int & ndia, 
		int & ia, int & ndja, int & ja, int & nda, real & a, real & sol, real & rhs, 
		int & ikeep, int & iwh, real & wh, real & fratio, real & fratio2, real & uh, 
		int & ias, int & jas, real & as, int & itimp, int & debug, real & fratio0, real & fratio20, 
		int & ierr );
}

HarwellEquationSolver::
HarwellEquationSolver(Oges & oges_)  : EquationSolver(oges_) 
{
  name="harwell";

  solverJob=7;
}

HarwellEquationSolver::
~HarwellEquationSolver()
{
}


real HarwellEquationSolver::
sizeOf( FILE *file /* =NULL */  )
// return number of bytes allocated 
{
  real size=(ikeep.elementCount()*sizeof(int)+
	     iwh.elementCount()*sizeof(int)+
	     wh.elementCount()*sizeof(real));
  return size;
}


int HarwellEquationSolver::
solve(realCompositeGridFunction & u,
      realCompositeGridFunction & f)
{
  printf("HarwellEquationSolver: solve: solverJob=%i, oges.initialized=%i, oges.shouldBeInitialized=%i \n",
          solverJob,oges.initialized,oges.shouldBeInitialized);
  

  int ierr;
  if( !oges.initialized || oges.shouldBeInitialized )
  {
    if( Oges::debug & 1 ) cout << " *** HarwellEquationSolver::solve: initialize... ****\n";
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

    const int stencilSize = oges.coeff[0].sparse->stencilSize;
    fillinRatio=parameters.fillinRatio>0 ? parameters.fillinRatio : stencilSize+10;  // +5; *wdh* 971024
    if( parameters.compatibilityConstraint && oges.numberOfDimensions==3 )
      fillinRatio+=5;
    parameters.fillinRatio=fillinRatio;

    //   ...fratio2 is another fill-in ratio for Harwell:
    if( parameters.fillinRatio2<=0. )
      parameters.fillinRatio2=2.;


    bool allocateSpace = true;           // fix these ********************
    bool factorMatrixInPlace = true; // !parameters.keepSparseMatrix <- not good enough 

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
    cout << "++Harwell TIMINGS \n";
    cout << " build="<<timeBuild;
    cout << ", solve="<<timeSolve<<"."<<endl;
  }
  
  // convert vectors back to grid functions
  oges.storeSolutionIntoGridFunction();

  return ierr;
}

int HarwellEquationSolver::
allocateWorkSpace()
// ==========================================================================
// /Description:
//   Allocate work space needed by Harwell. This is only a guess.
// ==========================================================================
{
    
  //.........  Harwell arrays:
  //         storage: iwk=15*neq+nqs+nfill     rwk=2*neq+nfill
  if( wh.getLength(0)!=numberOfEquations+1 )
    wh.redim(numberOfEquations+1); 
  if( ikeep.getLength(0)!=numberOfEquations*5+1 )
    ikeep.redim(numberOfEquations*5+1); 
  if( iwh.getLength(0)!=numberOfEquations*8+1 )
    iwh.redim(numberOfEquations*8+1);   


  return 0;
}


int HarwellEquationSolver::
solve()
//=====================================================================
// call the Harwell solver
//
//
//=====================================================================
{
  int ierr=0;
  assert( wh.getLength(0)>0 );
  
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
    cout << "callSparseSolver: calling Harwell solver CGESL2C..." << endl;

  ierr=0;
  
  maximumResidual=0.;
  numberOfIterations=0;

  CGESL2C(id0,rd0,solverJob,numberOfEquations,numberOfNonzeros,oges.numberOfNonzerosBound,
	  oges.ndia,*oges.ia.getDataPointer(),
	  oges.ndja,*oges.ja.getDataPointer(),
          oges.nda,*oges.a.getDataPointer(),
	  *oges.sol.getDataPointer(),*oges.rhs.getDataPointer(),
	  *ikeep.getDataPointer(),
	  *iwh.getDataPointer(),*wh.getDataPointer(),
	  parameters.fillinRatio,parameters.fillinRatio2,
          parameters.harwellPivotingTolerance,
	  *oges.ia.getDataPointer(),*oges.ja.getDataPointer(),*oges.a.getDataPointer(),
	  parameters.iterativeImprovement,
	  Oges::debug,oges.actualFillinRatio,oges.actualFillinRatio2,ierr );

  if( ierr != 0 )
  {
    cerr << "Error return from Harwell solver CGESL2, ierr=" << ierr << endl;
    exit(1);
  }

  solverJob=0;
  
  return ierr;
}
