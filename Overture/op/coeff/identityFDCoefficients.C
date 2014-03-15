//================================================================================
//   Define coefficients of the Identity and related operators
//
// Who to blame: Bill Henshaw
//================================================================================

#include "MappedGridOperators.h"
#include "xDC.h"

void 
identityFDerivCoefficients(RealDistributedArray & derivative,
		      const Index & I1,
		      const Index & I2,
		      const Index & I3,
		      const Index & E,
		      const Index & C,
		      MappedGridOperators & mgop)
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  int & orderOfAccuracy    = mgop.orderOfAccuracy;
  int & width              = mgop.width;
  int & halfWidth1         = mgop.halfWidth1;
  int & halfWidth2         = mgop.halfWidth2;
  int & halfWidth3         = mgop.halfWidth3;
  int & stencilSize        = mgop.stencilSize;
  int & numberOfComponentsForCoefficients = mgop.numberOfComponentsForCoefficients;

  //  Range aR0,aR1,aR2,aR3;
  
  int bound;
  if( orderOfAccuracy==2 )
    bound = numberOfDimensions==2 ? 8 : ( numberOfDimensions==3 ? 26 : 2);  
  else
    bound = numberOfDimensions==2 ? 24 : ( numberOfDimensions==3 ? 124 : 4);  
  Range M(0,bound);

  for( int c=C.getBase(); c<=C.getBound(); c++ )                        
  {
    for( int e=E.getBase(); e<=E.getBound(); e++ )                        
    {
      derivative(M+CE(c,e),I1,I2,I3)=0.;
      derivative(M123(0, 0, 0)+CE(c,e),I1,I2,I3)=1.;
    }
  }
}
