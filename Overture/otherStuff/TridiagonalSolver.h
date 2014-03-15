#ifndef TRIDIAGONAL_SOLVER_H
#define TRIDIAGONAL_SOLVER_H

#include "GenericDataBase.h"
#include "wdhdefs.h"

class TridiagonalSolver
{
 public:
  enum SystemType
  {
    normal=0,
    extended,
    periodic
  };
  
  TridiagonalSolver();
  ~TridiagonalSolver();
  
  // factor a tri-diagonal system:
  int factor( RealArray & a, 
	      RealArray & b, 
	      RealArray & c, 
	      const SystemType & type=normal, 
	      const int & axis=0,
              const int & block=1 );

  // factor a penta-diagonal system:
  int factor( RealArray & a, 
	      RealArray & b, 
	      RealArray & c, 
	      RealArray & d, 
	      RealArray & e, 
	      const SystemType & type=normal, 
	      const int & axis=0,
              const int & block=1 );

  int solve( const RealArray & r,        // this is not really const
             const Range & R1=nullRange, 
             const Range & R2=nullRange, 
             const Range & R3=nullRange );

  virtual real sizeOf( FILE *file=NULL ) const ; // return number of bytes allocated, print info to a file

 protected:

  int tridiagonalFactor();
  int tridiagonalSolve( RealArray & r );
  int periodicTridiagonalFactor();
  int periodicTridiagonalSolve( RealArray & r );
  
  int invert(RealArray & d, const int & i1 );
  int invert(RealArray & d, const Index & I1, const Index & I2, const Index & I3 );

  RealArray multiply( const RealArray & d, const int & i1, const RealArray & e, const int & j1);
  RealArray matrixVectorMultiply( const RealArray & d, const int & i1, const RealArray & e, const int & j1);
  RealArray matrixVectorMultiply( const RealArray & d, const int & i1, const RealArray & e);

  RealArray multiply(const RealArray & d, const Index & I1, const Index & I2, const Index & I3, 
		     const RealArray & e, const Index & J1, const Index & J2, const Index & J3);
  RealArray matrixVectorMultiply(const RealArray & d, const Index & I1, const Index & I2, const Index & I3, 
				 RealArray & e, const Index & J1, const Index & J2, const Index & J3);

  int blockFactor();
  int blockSolve(RealArray & r);
  int blockPeriodicFactor();
  int blockPeriodicSolve(RealArray & r);
  int scalarBlockFactor(int i1, int i2, int i3 );
  int scalarBlockSolve(RealArray & r, int i1, int i2, int i3);
  int scalarBlockPeriodicFactor(int i1, int i2, int i3);
  int scalarBlockPeriodicSolve(RealArray & r, int i1, int i2, int i3);
  
  SystemType systemType;
  int axis;
  RealArray a,b,c,d,e,w1,w2;
  Range Iv[3], &I1, &I2, &I3;

  int blockSize;  // we can do 1x1, 2x2 and 3x3 blocks
  bool scalarSystem;
  int bandWidth;   // 3 or 5 for tridiagonal or penta-diagonal systems

  bool useOptimizedC;
  
};

#endif
