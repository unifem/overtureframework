//================================================================================
// NOTE: This file is processed by the perl script gDeriavtive.p to generate
// functions for spatial derivatives x,y,z,xx
//
// Different functions are created for
//    .order = 2,4     : order of accuracy
//    .nd = 2,3,2R,3R  : number of space dimensions and for rectangular grids
//
// This file is the source for the following files
//             rDerivative.C, 
//             sDerivative.C, 
//             tDerivative.C, 
//             sDerivative.C, 
//             rsDerivative.C, 
//             xrterivative.C, 
//              ... etc ...
//================================================================================

#include "MappedGridOperators.h"
#include "rD.h"


void 
sDerivative(const realMappedGridFunction & ugf,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
{                                                                        
  int & orderOfAccuracy = mgop.orderOfAccuracy;
  RealDistributedArray & u = (RealDistributedArray&) ugf;
  int n;

  if( orderOfAccuracy==2 )
  {
    RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
    RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;
    if( mgop.mappedGrid.numberOfDimensions()==0 )
    { // these lines also prevent warnings about unused variables.
      printf("sDerivative:ERROR: numberOfDimensions=%i\n",mgop.mappedGrid.numberOfDimensions());
      Overture::abort("error");
    }
    for( n=N.getBase(); n<=N.getBound(); n++ )
      derivative(I1,I2,I3,n)=US2(I1,I2,I3,n);
  }
  else // ====== 4th order =======
  {
    RealArray d14; d14 = 1./(12.*mgop.mappedGrid.gridSpacing()); // mgop.d14;
    RealArray d24; d24 = 1./(12.*SQR(mgop.mappedGrid.gridSpacing())); // mgop.d24;
    if( mgop.mappedGrid.numberOfDimensions()==0 )
    { // these lines also prevent warnings about unused variables.
      printf("sDerivative:ERROR: numberOfDimensions=%i\n",mgop.mappedGrid.numberOfDimensions());
      Overture::abort("error");
    }
    for( n=N.getBase(); n<=N.getBound(); n++ )
      derivative(I1,I2,I3,n)=US4(I1,I2,I3,n);
  }
}
