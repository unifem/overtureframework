#ifndef TESTUTILS_H
#define TESTUTILS_H
#include <Overture.h>
#include "OGFunction.h"

void 
printMaxNormOfScalar (realCompositeGridFunction & scalar, CompositeGrid & cg);

real 
getMaxNormOfScalarOnAllGrids (realCompositeGridFunction & scalar, CompositeGrid & cg);

real
getMaxNormOfScalarOnGrid (realMappedGridFunction & scalar, MappedGrid & mg);

void 
printMaxNormOfProjectedDivergence (realCompositeGridFunction & divergence, CompositeGrid & cg);

void 
printMaxNormOfVelocity (realCompositeGridFunction & velocity, CompositeGrid & cg);

void 
printMaxNormOfDifference (const realMappedGridFunction & u, const realMappedGridFunction & v);

void 
printMaxNormOfDifference (const realGridCollectionFunction & u, const realGridCollectionFunction & v);

void 
printMaxNormOfGridFunction (const realMappedGridFunction & mgf, const int& extra=0);

void
printMaxNormOfDifference (const realMappedGridFunction & mgf, const realMappedGridFunction & v, const int& extra=0);

void 
printMaxNormOfDifference (const realGridCollectionFunction & gcf,
			  OGFunction* exactSolutionFunction,
			  const Index& Components = nullIndex,
			  const real time = 0.0,
			  const bool includeGhosts = LogicalFalse,
			  const int& extra = 0);
void 
printMaxNormOfDifference (const realMappedGridFunction & mgf,  
			  OGFunction * exactSolutionFunction, 
			  const Index& Components,
			  const real time = 0.0, 
			  const bool includeGhosts = LogicalFalse,
			  const int& extra=0,
			  const bool& calledFromAbove=LogicalFalse);


real
printMaxNormOfDifference (const realMappedGridFunction & mgf,  
			  OGFunction * exactSolutionFunction, 
			  const Index& I1,
			  const Index& I2,
			  const Index& I3,
			  const Index& Components,
			  const real time = 0.0, 
			  const bool includeGhosts = LogicalFalse,
			  const int& extra=0,
			  const bool& calledFromAbove=LogicalFalse);



#endif
