#include "GridDistribution.h"
#include "Overture.h"

// **********************************************************************
//     This class defines a parallel distribution for Overture Grids
// **********************************************************************


// minNumberPerDimension[d] : holds the desired minimum number of points per dimension on each processor for
//                             a grid with d+1 dimensions. 
//  1d: require 21 pts
//  2d: require  9 pts --> this means a 17x17    grid will not be split (includes ghost pts)
//  3d: require  7 pts --> this means a 13x13x13 grid will not be split (includes ghost pts)
int GridDistribution::minNumberPerDimension[3]={21,9,7};  

GridDistribution::
GridDistribution(int grid_ /* = 0 */,
                 real workLoad_ /* = 0. */,
                 int rlevel /* =0 */,
                 int mglevel /* =0 */  )
// ========================================================================
/// \brief Create a GridDistribution. 
/// \param grid_ (input) : grid number
/// \param workLoad_ (input) : workload
/// \param rlevel (input) : refinement level
/// \param mgLevel (input) : multigrid level 
// 
// ========================================================================
{
  grid=grid_;
  workLoad=workLoad_;
  pStart=0; pEnd=0;
  refinementLevel=rlevel;
  multigridLevel=mglevel;
  dims[0]=dims[1]=dims[2]=1;
}

GridDistribution::
~GridDistribution()
{
}

GridDistribution::
GridDistribution(const GridDistribution & x)
// copy constructor
{
  *this=x;
}

GridDistribution& GridDistribution:: 
operator=(const GridDistribution & x)
{
  pStart=x.pStart;
  pEnd=x.pEnd;
  grid=x.grid;
  workLoad=x.workLoad;
  refinementLevel=x.refinementLevel;
  multigridLevel=x.multigridLevel;
  for( int d=0; d<3; d++ )
    dims[d]=x.dims[d];
  
  return *this;
}

int GridDistribution::
setProcessors(int pStart_, int pEnd_ )
// ========================================================================
/// \brief Set the range of processors over which this grid is distributed.
// 
// ========================================================================
{
  pStart=pStart_;
  pEnd=pEnd_;
  return 0;
}

int GridDistribution::
getProcessorRange( int & pStart_, int & pEnd_ ) const
// ========================================================================
/// \brief Return the range of processors over which this grid is distributed.
// 
// ========================================================================
{
  pStart_=pStart;
  pEnd_=pEnd;
  return 0;
}


int GridDistribution::getGrid() const
// ========================================================================
/// \brief Return the grid number associated with this GridDistribution
// 
// ========================================================================
{
  return grid;
}

int GridDistribution::getMultigridLevel() const
// ========================================================================
/// \brief Return the multigrid level for this grid
// 
// ========================================================================
{
  return multigridLevel;
}

int GridDistribution::getRefinementLevel() const
// ========================================================================
/// \brief Return the refinement level number for this grid
// 
// ========================================================================
{
  return refinementLevel;
}

int GridDistribution::setGrid( int grid_ )
// ========================================================================
/// \brief Set the grid number associated with this GridDistribution
// ========================================================================
{
  grid=grid_;
  return 0;
}

int GridDistribution::setGridAndRefinementLevel( int grid_, int rlevel )
// ========================================================================
/// \brief Set the grid number and refinement level associated with this GridDistribution
// ========================================================================
{
  grid=grid_;
  refinementLevel=rlevel;
  return 0;
}

int GridDistribution::setMultigridLevel( int mglevel )
// ========================================================================
/// \brief Set the refinement level associated with this GridDistribution
// ========================================================================
{
  multigridLevel=mglevel;
  return 0;
}

int GridDistribution::setRefinementLevel( int rlevel )
// ========================================================================
/// \brief Set the refinement level associated with this GridDistribution
// ========================================================================
{
  refinementLevel=rlevel;
  return 0;
}


real GridDistribution::getWorkLoad() const
// ========================================================================
/// \brief Return the gwork-load associated with this GridDistribution
// 
// ========================================================================
{
  return workLoad;
}

int GridDistribution::
getGridPoints( int gridPoints[3] ) const
// ========================================================================
/// \brief Return the number of grid points in each direction.
// ========================================================================
{
  for( int axis=0; axis<3; axis++ ) gridPoints[axis]=dims[axis];
  return 0;
}


// int GridDistribution::setWorkLoad( real workLoad_ )
// // ========================================================================
// // /Description:
// //    Set the work-load associated with this GridDistribution
// // 
// // ========================================================================
// {
//   workLoad=workLoad_;
//   return 0;
// }

int GridDistribution::setWorkLoadAndGridPoints( real workLoad_, int gridPoints[3] )
// ========================================================================
/// \brief Set the work-load and number of grid points associated with this GridDistribution
/// 
/// \param workLoad_ (input) : work-load
/// \param gridPoints (input) : gridPoints[d] d=0,1,2 - number of grid points in the 3 coordinate directions.
///     For a 2d grid set gridPoints[2]=1. 
// ========================================================================
{
  workLoad=workLoad_;
  setGridPoints( gridPoints );
  return 0;
}

int GridDistribution::setGridPoints( int gridPoints[3] )
// ========================================================================
/// \brief  Set the number of grid points associated with this GridDistribution
/// 
/// \param gridPoints (input) : gridPoints[d] d=0,1,2 - number of grid points in the 3 coordinate directions.
///     For a 2d grid set gridPoints[2]=1. 
// ========================================================================
{
  for( int d=0; d<3; d++ )
    dims[d]=gridPoints[d];

  return 0;
}

void GridDistribution::computeParallelArrayDistribution(int *dimProc )
// ====================================================================================
///  
/// \brief Compute a (tensor product) parallel distribution for a multi-dimensional array by allocating processors to
/// each distributed dimension. 
///
/// \param dimProc (output) : dimProc[d], d=0,1,...,nDims-1 are the number of processors used in dimension d.
///
/// Given a total of nProcs processors, determine how to distribute a multidimensional array
/// using dimProc[d] processors along dimension d such that: 
///
///      nProcs = dimProc[0]*dimproc[1]*...dimProc[nDims-1] 
/// 
///
/// This routine was taken from Block-PARTI function distribute
// ====================================================================================
{
  int nProcs=pEnd-pStart+1;
  int nDims=3;
  computeParallelArrayDistribution(nProcs,nDims,dims,dimProc );
  
}

namespace
{


int leastPrimeFactor(int n)
// ==============================================================
// Compute and return the smallest (prime) factor of n
// 
// This routine was taken from Block-PARTI
// ==============================================================
{
  int i;

  if (n < 0) 
  {
    printf("leastPrimeFactor:ERROR:can't factor a negative number\n");
    return n;
  }

  if (n < 2) return n;
  if (n % 2 == 0) return 2;

  int sqrtn = int(sqrt(double(n+.5)));
  for (i = 3; i <= sqrtn; i+=2) 
  {
    if (n % i == 0) return i;
  }
  return n;
}


int greatestPrimeFactor(int n)
// =============================================
// Return the largest prime factor of n 
// 
// This routine was taken from Block-PARTI
// ==============================================
{
  int lpf;
  if (n < 0) 
  {
    printf("greatestPrimeFactor:ERROR: can't factor a negative number\n");
    return n;
  }

  /* after dividing out all the smaller prime factors, what's left is the */
  /* largest one */
  for (lpf = leastPrimeFactor(n); lpf != n; lpf = leastPrimeFactor(n)) 
  {
    n = n/lpf;
  }

  return n;
}
 
}


void GridDistribution::
computeParallelArrayDistribution( const int nProcs, const int nDims, int *dimVec, int *dimProc )
// ====================================================================================
/// 
/// \brief Compute a (tensor product) parallel distribution for a multi-dimensional array by allocating processors to
/// each distributed dimension. 
///
/// \param nProcs (input): total number of processors to use
/// \param nDims (input) : number of dimensions to distribute
/// \param dimVec (input) : dimVec[d], d=0,1,...,nDims-1 are the number of grid points in dimension d
/// \param dimProc (ouptut) : dimProc[d], d=0,1,...,nDims-1 are the number of processors used in dimension d.
///
///
/// Given a total of nProcs processors, determine how to distribute a multidimensional array
/// using dimProc[d] processors along dimension d such that: 
///
///      nProcs = dimProc[0]*dimproc[1]*...dimProc[nDims-1] 
/// 
/// This routine was taken from Block-PARTI function distribute
// ====================================================================================
{
  int    i, used, bigRatioDim, factor;
  float  ratio, nRatio;

  // Consider the prime factorization of nProc:
  //    nProc = p_1*p_2*p_3*...*p_m  (with increasing factors, p_k >= p_{k-1})
  // 
  // Start by assuming that each dimension is allocated to 1 processor.
  // Look for the dimension with the largest number of grid points per processor
  // and allocate a factor of p_m processors to that dimension. Now repeat...
  //
  // When we are done we will have something like:
  //   dimProc[0] = p_1*p_3*...
  //   dimProc[1] = p_2*p_5*...
  //   dimProc[2] = p_4

  for (i=0; i<nDims; i++)
    dimProc[i] = 1;

  for( used=1; used < nProcs; ) 
  {
    bigRatioDim = 0;
    // look for the dimension with the current largest number of grid points per processor
    ratio = ((float)dimVec[0]) / ((float)dimProc[0]);
    for( i=1; i<nDims; i++ ) 
    {
      nRatio = ((float)dimVec[i]) / ((float) dimProc[i]);
      if( nRatio > ratio || (nRatio == ratio && nRatio > 1 &&  dimProc[i] < dimProc[bigRatioDim])) 
      {
	// if the ratios are the same, this favors the dimension that already is assigned fewer processors 
	ratio = nRatio;
	bigRatioDim = i;
      }
    }
    factor = greatestPrimeFactor(nProcs/used);
    dimProc[bigRatioDim] *= factor;
    used *= factor;
  }

}


int GridDistribution:: 
getMinimumNumberOfPointsPerDimensionPerProcessor(int numberOfDimensions)
// =====================================================================================
/// \brief Return the desired minimum number of points per dimension on each processor. This value
///  is used to limit the number of processors over-which a dimension is split.
/// 
/// \param numberOfDimensions (input) : the number of dimensions of the grid.
// =====================================================================================
{
  assert( numberOfDimensions>=1 && numberOfDimensions<=3 );
  return minNumberPerDimension[numberOfDimensions-1];
}

int GridDistribution:: 
setMinimumNumberOfPointsPerDimensionPerProcessor(int numberOfDimensions, int minNumber )
// =====================================================================================
/// \brief Set the desired minimum number of points per dimension on each processor. This value
///  is used to limit the number of processors over-which a dimension is split.
/// 
/// \param numberOfDimensions (input) : the number of dimensions of the grid.
/// \param minNumber (input) : the minimum number of points per dimension on each processor.
/// =====================================================================================
{
  assert( numberOfDimensions>=1 && numberOfDimensions<=3 );
  minNumberPerDimension[numberOfDimensions-1]=minNumber;
  return 0;
}


int GridDistribution::    
determineAGoodNumberOfProcessors( int & numProc, const int minProc, const int maxProc )
// ==================================================================================
/// 
/// \brief Determine a "good" value for the number of processors, near to numProc and 
///    in the range [minProc,maxProc].
///
/// \param numProc (input/output) : on input a desired number of processors, on output a good number to use.
/// \param minProc, maxProc (input): 
/// \param minimumNumberOfPoints (input) : the desired minimum number of points in any dimension, per processor.
///             This value cannot be enforced if the total number of grid points in a dimension is less than this.
/// 
///  \note A "good" value for the number of processors will be a value that gives
///  a good parallel distribution of the array. This distribution will be chosen so that 
///  each local array will have a miniumun
///  number of points in each coordinate direction. An attempt will be made to make each
///  local array as "square" as possible so as to reduce the size of communication boundaries.
// ==================================================================================
{
  const int debug=0;
  const int myid=max(0,Communication_Manager::My_Process_Number);

  int nProc=numProc; 

  int dimProc[3]={1,1,1};

  assert( dims[0]>1 || dims[1]>1 );

  // nDims: number of dimensions 
  int nDims=3;  
  for( int d=nDims-1; d>=0; d-- )
  {
    if( dims[d]==1 ) nDims--;
  }

  int minimumNumberOfPoints = getMinimumNumberOfPointsPerDimensionPerProcessor(nDims);
  

  // Here is a possible algorithm: -- BUT this is not implemented yet ---
  // (1) search for the best value between minProc and maxProc
  // (2) If there is no best value satisfying the minPts constraint then keep looking for values less than minProc


  int numIts=numProc;
  int npOpt=numProc;
  real ratioOpt=REAL_MAX/10.;
  for( int it=0; it<numIts; it++ )
  {
    computeParallelArrayDistribution(nProc,nDims,dims,dimProc );

    if( debug & 1 )
      printF("GD: grid=%i it=%i: nProc=%i proc-decomp=[%i]x[%i]x[%i], grid-pts=[%i]x[%i]x[%i], "
	     "grid-pts/proc=[%i]x[%i]x[%i]\n",
	     grid, it,nProc,dimProc[0],dimProc[1],dimProc[2],
	     dims[0],dims[1],dims[2],
	     dims[0]/dimProc[0],
	     dims[1]/dimProc[1],
	     dims[2]/dimProc[2]
	);

    // Now look for a better distribution:

    int minPts=INT_MAX;
    int maxPts=0;
    for( int axis=0; axis<nDims; axis++ )
    {
      if( dimProc[axis]>1 || dims[axis]>=minimumNumberOfPoints ) // some dimensions may not have many points 
      {
	int pointsPerProc=dims[axis]/dimProc[axis];
	minPts = min(minPts,pointsPerProc);
	maxPts = max(maxPts,pointsPerProc);
      }
    }
    bool ok=false;
    if( minPts >= minimumNumberOfPoints )
    {
      ok=true;
    }
    real ratio=real(maxPts)/real(minPts);
    if( debug & 1 ) printf(" myid=%i minPts=%i maxPts=%i ratio=%g   -> ok=%i \n",myid,minPts,maxPts,ratio,(int)ok);
    if( ok && fabs(ratio-1.) < fabs(ratioOpt-1.) )
    {
      ratioOpt=ratio;
      npOpt=nProc;
    }

    // current stopping criteria: 
    //   satisfy minPts condition plus maxRatio<=4 or an even number of processors.
    const real maxRatio=4.;  // square-ness ratio
    ok = ok && (ratio<=maxRatio || npOpt%2==0 );
    
    if( ok ) break; 
    
    nProc=max(1,nProc-1); // try one less processor
  }
  if( debug & 1 ) printf("GD: --> myid=%i grid=%i : good number of processors:  np=%i, ratio=%g\n",
                  myid,grid,npOpt,ratioOpt);


  numProc=npOpt;

  return 0;
}


int GridDistribution::
get( const GenericDataBase & dir, const aString & name)
// ====================================================================================
// \brief Get from a database file.
// ====================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"GridDistribution");

  subDir.setMode(GenericDataBase::streamInputMode);

  const int numiPar=8;
  int ipar[numiPar]; // ={pStart,pEnd,grid,refinementLevel,dims[0],dims[1],dims[2]}; //
  subDir.get(ipar,"ipar",numiPar);
  pStart         =ipar[0];
  pEnd           =ipar[1];
  grid           =ipar[2];
  refinementLevel=ipar[3];
  multigridLevel =ipar[4];
  dims[0]        =ipar[5];
  dims[1]        =ipar[6];
  dims[2]        =ipar[7];

  const int numRpar=1;
  real rpar[numRpar]; // ={workLoad};  //
  subDir.get(rpar,"rpar",numRpar);
  workLoad=rpar[0];

  delete &subDir;
  return true; 
}

int GridDistribution::
put( GenericDataBase & dir, const aString & name) const
// ====================================================================================
/// \brief Put to a database file.
// ====================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"GridDistribution");                 // create a sub-directory 

  subDir.setMode(GenericDataBase::streamOutputMode);

  const int numiPar=8;
  int ipar[numiPar]={pStart,pEnd,grid,refinementLevel,multigridLevel,dims[0],dims[1],dims[2]}; //
  subDir.put(ipar,"ipar",numiPar);

  const int numRpar=1;
  real rpar[numRpar]={workLoad};  //
  subDir.put(rpar,"rpar",numRpar);
  
  delete &subDir;
  return 0;
}




// **********************************************************************************************

GridDistributionList::
GridDistributionList()
// ======================================================================
/// \brief Create a list of GridDistributions
// ======================================================================
{
}

GridDistributionList::
~GridDistributionList()
// ======================================================================
/// \brief destructor
// ======================================================================
{
}

  
GridDistributionList::
GridDistributionList( const GridDistributionList & x )
// ======================================================================
/// \brief Copy constructor (deep copy)
// ======================================================================
{
  *this=x;
}

  
GridDistributionList& GridDistributionList::
operator=(const GridDistributionList & x )
// ======================================================================
/// \brief Operator = (deep copy)
// ======================================================================
{
  clear();
  for( int grid=0; grid<x.size(); grid++ )
  {
    push_back(x[grid]);
  }

  return *this;
}

  
int GridDistributionList::
display( const aString & label, FILE *file /* =stdout */ )
// ======================================================================
/// \brief Display the grid distribution info
/// \param label (input): label to use with the display
/// \param file (input) : output to this file.
// ======================================================================
{
  fPrintF(file,"GridDistributionList info: %s\n",(const char*)label);
  for(int grid=0; grid<size(); grid++ )
  {
    GridDistribution & gd = (*this)[grid];

    int g = gd.getGrid();
    int pStart, pEnd; gd.getProcessorRange( pStart,pEnd);
    int refinementLevel = gd.getRefinementLevel();
    real workLoad = gd.getWorkLoad();
    int dims[3];  gd.getGridPoints( dims );

    fPrintF(file," i=%i : grid=%i [pStart,pEnd]=[%i,%i] refineLevel=%i mgLevel=%i workLoad=%e gridPoints=[%i][%i][%i]\n",
	    grid,g,pStart,pEnd,refinementLevel,gd.getMultigridLevel(),workLoad,dims[0],dims[1],dims[2]);
  }
  
  return 0;
}



int GridDistributionList::
get( const GenericDataBase & dir, const aString & name)
// ====================================================================================
/// \brief Get from a database file.
// ====================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"GridDistributionList");

  // subDir.setMode(GenericDataBase::streamInputMode);

  int numberOfElements=0;
  subDir.get(numberOfElements,"size");
  // printF("GridDistributionList:get: numberOfElements=%i\n",numberOfElements);

  // make the list the correct size: 
  GridDistribution gd;
  while( size()<numberOfElements )
  {
    push_back(gd);
  }
  while( size()>numberOfElements )
  {
    erase(end());
  }
  
  for( int grid=0; grid<size(); grid++ )
  {
    (*this)[grid].get(subDir,sPrintF("GridDistribution%i",grid));
  }

  delete &subDir;
  return true; 
}

int GridDistributionList::
put( GenericDataBase & dir, const aString & name) const
// ====================================================================================
/// \brief Put to a database file.
// ====================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"GridDistributionList");            // create a sub-directory 

  // subDir.setMode(GenericDataBase::streamOutputMode);

  int numberOfElements=size();
  subDir.put(numberOfElements,"size");
  // printF("GridDistributionList:put: numberOfElements=%i\n",numberOfElements);
  for( int grid=0; grid<size(); grid++ )
  {
    (*this)[grid].put(subDir,sPrintF("GridDistribution%i",grid));
  }

  delete &subDir;
  return 0;
}
