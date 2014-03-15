#include "YaleEquationSolver.h"
#include "SparseRep.h"


#define CGESL1C EXTERN_C_NAME(cgesl1c)

extern "C"
{
  void CGESL1C( int & id, real & rd, int & job, int & neq, int & nqs, int & ndia, int & ia,
		int & ndja, int & ja, int & nda, real & a,int & perm, int & iperm, real & sol,
		real & rhs, int & nsp, real & isp, real & rsp, int & esp, real & fratio, int & nze,
		int & debug, real & fratio0, int & ierr );
  
}

real YaleEquationSolver::timeForBuild=0.;
real YaleEquationSolver::timeForSolve=0.; 
real YaleEquationSolver::timeForTransfer=0.;

YaleEquationSolver::
YaleEquationSolver(Oges & oges_)  : EquationSolver(oges_) 
{
  name="yale";

  solverJob=7;
}

YaleEquationSolver::
~YaleEquationSolver()
{
}


real YaleEquationSolver::
sizeOf( FILE *file /* =NULL */  )
// return number of bytes allocated 
{
  real size=(perm.elementCount()*sizeof(int)+
	     iperm.elementCount()*sizeof(int)+
	     rsp.elementCount()*sizeof(real));
  return size;
}

int YaleEquationSolver::
printStatistics(FILE *file /* = stdout */ ) const
//===================================================================================
// /Description:
//   Output any relevant statistics
//===================================================================================
{
  real totalTime=timeForBuild+timeForSolve+timeForTransfer;
  fprintf(file,"============== YaleEquationSolver Statistics =============\n"
               "   time to build matrix...... %8.2e \n"
               "   time to solve............. %8.2e \n" 
               "   time to transfer data..... %8.2e \n"
               "   total of above.............%8.2e \n"
               "==========================================================\n"
	  ,timeForBuild,timeForSolve,timeForTransfer,totalTime);
  
  return 0;
}

int YaleEquationSolver::
solve(realCompositeGridFunction & u,
      realCompositeGridFunction & f)
{
  // printf("YaleEquationSolver: solve: solverJob=%i\n",solverJob);
  

  int ierr;
  if( !oges.initialized || oges.shouldBeInitialized )
  {
    if( Oges::debug & 1 ) cout << " *** YaleEquationSolver::solve: initialize... ****\n";
    solverJob=7;  
  }
  
  int shouldUpdateMatrix= (solverJob & 2) || oges.refactor==true;

  
  double timeBuild=getCPU();
  if( shouldUpdateMatrix ) 
  {
    if( Oges::debug & 1 ) cout << "...Build Matrix\n";
    //..build compressed-row matrix ia,ja,a
    // solverJob=7; // *********************************************
    // oges.initialized=false;

    const int stencilSize = oges.stencilSize;
    assert( stencilSize>0 );

    bool fillinRatioSpecified = parameters.fillinRatio>0;
    fillinRatio=fillinRatioSpecified>0 ? parameters.fillinRatio : stencilSize+10;  // +5; *wdh* 971024
    if( !fillinRatioSpecified && parameters.compatibilityConstraint && oges.numberOfDimensions==3 )
      fillinRatio+=5;
    parameters.fillinRatio=fillinRatio;

    bool allocateSpace = true;           // fix these ********************
    bool factorMatrixInPlace = false;

    bool allocate=true;
    if( !oges.solvingSparseSubset )
    {
      int newNumberOfEquations, newNumberOfNonzeros;
      oges.formMatrix(newNumberOfEquations,newNumberOfNonzeros,
		      Oges::compressedRow,allocateSpace,factorMatrixInPlace);

      allocate=( // (solverJob & 1) || 
	newNumberOfEquations!=numberOfEquations ||
	( fabs(newNumberOfNonzeros-numberOfNonzeros)/(1.+numberOfNonzeros) > .1 ) );
    
      numberOfEquations=newNumberOfEquations;
      numberOfNonzeros=newNumberOfNonzeros;
    }
    else
    {
      // do this for now: 
      numberOfEquations=oges.numberOfEquations;
      numberOfNonzeros=oges.numberOfNonzeros;
      oges.ndia=oges.ia.getLength(0);
      oges.ndja=oges.ja.getLength(0);
      oges.nda =oges.a.getLength(0);

      oges.initialized         = false;
      oges.shouldBeInitialized = false;      
    }
    
    if( !fillinRatioSpecified && oges.numberOfDimensions==3 )
    {
      parameters.fillinRatio=max(parameters.fillinRatio,.15*pow(real(numberOfNonzeros),.5));
      fillinRatio=parameters.fillinRatio;
      printf("***Oges:Increasing fillinRatio for workspace to %e\n",parameters.fillinRatio);
      printf("  (to over-ride this value you should explicitly set the fillin ratio)\n");
    }

    if( allocate )
    {
      allocateWorkSpace();
    }
    
    oges.initialized=true;

    timeForBuild+=timeBuild;
  }

  if( !oges.solvingSparseSubset )
  {
    if( Oges::debug & 1 ) cout << "...Build RHS\n";
    // convert grid function data to a single long vector
    real time1=getCPU();
    oges.formRhsAndSolutionVectors(u,f); 
    timeForTransfer+=getCPU()-time1;
  }
  
  timeBuild=getCPU()-timeBuild;
  
  real time0=getCPU();
  if( Oges::debug & 1 ) cout << "...Actually solving...\n";
  oges.numberOfIterations=0;
  ierr = solve();
  real timeSolve=getCPU()-time0;

  timeForSolve+=timeSolve;
  
  if( Oges::debug & 1 ) 
  {
    cout << "++Yale TIMINGS : ";
    cout << " build="<<timeBuild;
    cout << ", solve="<<timeSolve<<"."<<endl;
  }
  
  if( !oges.solvingSparseSubset )
  {
    // convert vectors back to grid functions
    real time1=getCPU();
    oges.storeSolutionIntoGridFunction();
    timeForTransfer+=getCPU()-time1;
  }
  
  return ierr;
}

int YaleEquationSolver::
allocateWorkSpace()
// ==========================================================================
// /Description:
//   Allocate work space needed by Yale. This is only a guess.
// ==========================================================================
{
  // nv    - nsp   - declared dimension of rsp.  nsp generally must
  //       -           be larger than  8n+2 + 2k  (where  k = (number of
  //       -           nonzero entries in m)).

  // for ODRV: 
  //     nsp  - declared dimension of the one-dimensional array isp.  nsp
  //           must be at least  3n+4k,  where k is the number of nonzeroes
  //           in the strict upper triangle of m 
  perm.redim(numberOfEquations+1);
  iperm.redim(numberOfEquations+1);

  printF("Oges::allocateWorkSpace: numberOfNonzeros=%i fillinRatio=%e\n",numberOfNonzeros,fillinRatio);
  
  if( rsp.getLength(axis1) < int(numberOfNonzeros*fillinRatio+.5) )
  {
    nsp=max( int( numberOfNonzeros*fillinRatio+.5 ),
	     10*numberOfEquations+2+2*numberOfNonzeros + 100 );

    if( true || Oges::debug & 2 ) 
      cout << "allocateWorkSpace: numberOfEquations=" << numberOfEquations << ", nsp = " << nsp 
           << ", fillinRatio= " << fillinRatio
	   << ", numberOfNonzeros = " << numberOfNonzeros << endl;
    rsp.redim(nsp+1); 
  }
  return 0;
}


int YaleEquationSolver::
solve()
//=====================================================================
// call the yale solver
//
//
//=====================================================================
{
  int ierr=0;
  assert( perm.getLength(0)>0 && rsp.getLength(0) > 0 );
  
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
    printF("callSparseSolver: calling CGESL1 job=%i (2=refactor, 4=reorder) debug=%i\n",solverJob,Oges::debug);

  const int numberOfTries=3;
  int tryToFactor;
  for( tryToFactor=0; tryToFactor<numberOfTries; tryToFactor++ )
  {
    CGESL1C( id0,rd0,solverJob,numberOfEquations,
             oges.numberOfNonzerosBound,          // ******************
	     oges.ndia, *oges.ia.getDataPointer(),
             oges.ndja, *oges.ja.getDataPointer(),
             oges.nda,  *oges.a.getDataPointer(),
	     *perm.getDataPointer(), *iperm.getDataPointer(),        
	     *oges.sol.getDataPointer(), *oges.rhs.getDataPointer(),
	     nsp,*rsp.getDataPointer(),*rsp.getDataPointer(),
             yaleExcessWorkSpace,fillinRatio,
	     numberOfNonzeros,Oges::debug,oges.actualFillinRatio,ierr );
    if( ierr<0 || ierr==100 )
    { // insufficient storage ... try again
      if( tryToFactor<numberOfTries-1 )
      {
	if( true || Oges::debug & 1 || ierr==100 )
	  cout << "YaleEquationSolver: not enough storage to factor, nsp= " << nsp << endl
	       << "... resizing arrays to try again..." << endl;

        // Remember that excess storage is thrown away so nsp may be less than the original estiamte:
        int nsp0 =max( int( numberOfNonzeros*fillinRatio+.5 ),
		       10*numberOfEquations+2+2*numberOfNonzeros + 100 );

        if( nsp> .9*nsp0 )
	{
	  parameters.fillinRatio*=1.5;  // increase fillin ratio
	}

	nsp=int( nsp*1.5 );
	rsp.resize(nsp+1); 
      }
      else
      {
	cerr << "Error return from CGESL1C, ierr=" << ierr << endl;
	exit(1);
      }
    }
    else
      break;
  }
  if( ierr != 0 )
  {
    if( ierr==100 )

    cerr << "YaleEquationSolver: Error return from CGESL1C, ierr=" << ierr << endl;
    exit(1);
  }
  // return excess storage, provided there is a significant amount and provided that
  // we have allocated space on this call ( solverJob & 1 )
  // For moving grid problems we may want to keep a little extra around anyway
  if( solverJob & 1 && yaleExcessWorkSpace > 10000+numberOfNonzeros*.1 )
  {
    if( Oges::debug & 1 )
    {
      cout << "YaleEquationSolver: excessWorkSpace = " << yaleExcessWorkSpace << endl;
      cout << "...resizing arrays to restore excess space..." << endl;
    }
    nsp-=(yaleExcessWorkSpace);

    nsp= int(nsp*1.1);  // keep 10% extra for safety *wdh* 000913
    
    rsp.resize(nsp+1); 
    yaleExcessWorkSpace=0;

    if( !parameters.keepSparseMatrix )
    {
      oges.ia.redim(0);       // these are no longer needed.
      oges.ja.redim(0);
      oges.a.redim(0);
    }
  }

  solverJob=0;
  
  return ierr;
}


