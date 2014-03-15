#include "LineSolve.h"


LineSolve::
LineSolve()
// ======================================================================================
// This class holds the state and data associated with the cg line solver. 
// ======================================================================================
{
  pTridiagonalSolvers=NULL;
  maximumSizeAllocated=0.;
}

LineSolve::
~LineSolve()
{
  delete [] pTridiagonalSolvers;
}


real LineSolve::
sizeOf( FILE *file /* =NULL */ ) const
// return number of bytes allocated, print info to a file
{
  if( file!=NULL )
  {
    fprintf(file," LineSolve: maximum size was %8.2 MBytes\n",maximumSizeAllocated/SQR(1024));
  }
  return maximumSizeAllocated;
}
