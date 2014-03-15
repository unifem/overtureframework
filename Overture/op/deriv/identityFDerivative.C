//================================================================================
//  Define the identity operator
//
// Who to blame:  Bill Henshaw
//================================================================================

#include "MappedGridOperators.h"
#include "xD.h"

void 
identityFDerivative(const realMappedGridFunction & u,
	       RealDistributedArray & derivative,
	       const Index & I1,
	       const Index & I2,
	       const Index & I3,
	       const Index & N,
	       MappedGridOperators & mgop )
{                                                                        
  derivative(I1,I2,I3,N)=u(I1,I2,I3,N);                  
}
