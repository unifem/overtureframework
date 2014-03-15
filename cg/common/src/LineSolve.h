#ifndef LINE_SOLVE_H
#define LINE_SOLVE_H

#include "Overture.h"

class TridiagonalSolver;

// This class holds the state and data associated with the cg line solver. 

class LineSolve
{
public:

LineSolve();
~LineSolve();

real sizeOf( FILE *file=NULL ) const ; // return number of bytes allocated, print info to a file



TridiagonalSolver **pTridiagonalSolvers;
IntegerArray lineSolveIsInitialized; 

real maximumSizeAllocated;  // keep track of the maximum number of bytes allocated


};


#endif
