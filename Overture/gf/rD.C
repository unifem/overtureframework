//================================================================================
// NOTE: This file is processed by the perl script xD.p to generate
// functions for r,s,t derivatives and for order of accuracy  .order = 2 and 4 
//
// This file is the source for the following files
//             rD2.C, sD2.C, tD2.C, rrD2.C, ...
//             rD4.C, sD4.C, tD4.C, rrD4.C, ...
//
//================================================================================

#include "MappedGridOperators.h"
#include "xD.h"


void 
rrDerivative.order(const realMappedGridFunction & u,
		   RealArray & derivative,
		   const Index & I1,
		   const Index & I2,
		   const Index & I3,
		   const Index & N,
		   MappedGridOperators & mgop )
{                                                                        
  RealArray & d12 = mgop.d12;
  RealArray & d22 = mgop.d22;
  RealArray & d14 = mgop.d14;
  RealArray & d24 = mgop.d24;
  
  int n;
  for( n=N.getBase(); n<=N.getBound(); n++ )                        
    derivative(I1,I2,I3,n)=URR.order(I1,I2,I3,n);                        
}
