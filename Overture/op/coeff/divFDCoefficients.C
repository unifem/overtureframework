//================================================================================
//   Define coefficients of the Divergence for Finite Differences
//
// Who to blame: Bill Henshaw
//================================================================================

#include "MappedGridOperators.h"
#include "xDC.h"

#define DERIVATIVE(type)                                                      \
  UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=UX ## type(I1,I2,I3,0)+UY ## type(I1,I2,I3,1); \
  if( mgop.numberOfDimensions==3 )                                            \
    UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)+=UZ ## type(I1,I2,I3,2);                    

void 
divFDerivCoefficients(RealDistributedArray & derivative,
		      const Index & I1,
		      const Index & I2,
		      const Index & I3,
		      const Index & E,
		      const Index & C,
		      MappedGridOperators & mgop)
{                                                                        
  cout << "ERROR: unable to obtain coefficients of the divergence! not implemented yet\n";
  Overture::abort("ERROR: unable to obtain coefficients of the divergence! not implemented yet");

}

#undef DERIVATIVE
