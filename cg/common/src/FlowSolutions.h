#ifndef FLOW_SLOUTIONS_H
#define FLOW_SLOUTIONS_H

#include "Overture.h"

#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;

class PistonMotion;

// =================================================================
// This class defines some known solutions for various "flows"
// =================================================================

class FlowSolutions
{
public:

 enum KnownSolutionsEnum
 {
   superSonicExpandingFlow,
   specifiedPistonMotion,
   forcedPistonMotion,
   obliqueShockFlow,
   deformingDiffuser,
   shockElasticPiston,
   rotatingElasticDiskInFluid
 };


FlowSolutions();
~FlowSolutions();

int 
getFlowSolution( const KnownSolutionsEnum knownSolution,
             realCompositeGridFunction & ua, int *ipar, real *rpar );

int 
getFlowSolution( const KnownSolutionsEnum knownSolution,
		 CompositeGrid & cg, int grid, RealArray & ua, int *ipar, real *rpar, 
		 const Index & I1, const Index &I2, const Index &I3  );

int 
getFlowSolution( PistonMotion & pistonMotion,
		 CompositeGrid & cg, int grid, RealArray & ua, int *ipar, real *rpar, 
		 const Index & I1, const Index &I2, const Index &I3  );

int 
getDeformingDiffuser( const KnownSolutionsEnum knownSolution,
		      CompositeGrid & cg, int grid, RealArray & ua, int *ipar, real *rpar, 
		      const Index & I1, const Index &I2, const Index &I3  );


int 
getObliqueShockFlow( const KnownSolutionsEnum knownSolution,
		     CompositeGrid & cg, int grid, RealArray & ua, int *ipar, real *rpar, 
		     const Index & I1, const Index &I2, const Index &I3  );

int 
getPistonFlow( const KnownSolutionsEnum knownSolution,
               CompositeGrid & cg, int grid, RealArray & ua, int *ipar, real *rpar, 
	       const Index & I1, const Index &I2, const Index &I3  );

int 
getShockElasticPistonFlow( const KnownSolutionsEnum knownSolution,
			   CompositeGrid & cg, int grid, RealArray & ua, int *ipar, real *rpar, 
			   const Index & I1, const Index &I2, const Index &I3  );

int 
getSupersonicExpandingFlow( realCompositeGridFunction & ua, int *ipar, real *rpar );

int 
getSupersonicExpandingFlow( CompositeGrid & cg, int grid, RealArray & ua, int *ipar, real *rpar, 
                            const Index & I1, const Index &I2, const Index &I3  );

int 
getRotatingElasticDiskInFluid( const KnownSolutionsEnum knownSolution,
			       CompositeGrid & cg, int grid, RealArray & ua, int *ipar, real *rpar, 
			       const Index & I1, const Index &I2, const Index &I3 );


protected:

// real rho0, p0, u0, v0, a0;

realCompositeGridFunction *v;
RealArray *wallDataPointer;

/// The database holds parameters needed by some flow solutions
mutable DataBase dbase; 

};

#endif
