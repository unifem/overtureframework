//================================================================================
//   Define coefficients of the Gradient for Finite Differences
//
// Who to blame: Bill Henshaw
//================================================================================

#include "MappedGridOperators.h"
#include "xDC.h"

void 
gradFDerivCoefficients(RealDistributedArray & derivative,
		       const Index & I1,
		       const Index & I2,
		       const Index & I3,
		       const Index & E,
		       const Index & C,
		       MappedGridOperators & mgop )
{                                                                        
  cout << "ERROR: gradFDerivCoefficients: this function should not be called! \n";
  Overture::abort("ERROR: gradFDerivCoefficients: this function should not be called! \n");

}
