#ifndef YALE_EQUATION_SOLVER_H
#define YALE_EQUATION_SOLVER_H

//
//  Yale solver for Oges
// 

//kkc 040415 #include <iostream.h>
#include "OvertureDefine.h"
#include OV_STD_INCLUDE(iostream)

#include <math.h>
#include <assert.h>

#include "EquationSolver.h"


class YaleEquationSolver : public EquationSolver
{
 public:
  YaleEquationSolver(Oges & oges_);
  virtual ~YaleEquationSolver();

  virtual int solve(realCompositeGridFunction & u,
		    realCompositeGridFunction & f);

  virtual real sizeOf( FILE *file=NULL ); // return number of bytes allocated 
  virtual int printStatistics( FILE *file = stdout ) const;   // output any relevant statistics 
 protected:
  int solve();
  int allocateWorkSpace();
  

  IntegerArray perm,iperm;
  int nsp;        
  RealArray rsp;
  int solverJob;
  int yaleExcessWorkSpace;

  real fillinRatio;

  static real timeForBuild, timeForSolve, timeForTransfer;


};


#endif
