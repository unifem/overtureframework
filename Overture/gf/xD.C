//================================================================================
// NOTE: This file is processed by the perl script xD.p to generate
// functions for spatial derivatives x,y,z,xx
//
// Different functions are created for
//    .order = 2,4     : order of accuracy
//    .nd = 2,3,2R,3R  : number of space dimensions and for rectangular grids
//
// This file is the source for the following files
//             xD22.C, yD22.C, zD22.C, xxD22.C, ...
//             xD23.C, yD23.C, zD23.C, xxD23.C, ...
//             xD42.C, yD42.C, zD42.C, xxD42.C, ...
//             xD43.C, yD43.C, zD43.C, xxD43.C, ...
//
//             xD22R.C, yD22R.C, zD22R.C, xxD22R.C, ...
//             xD23R.C, yD23R.C, zD23R.C, xxD23R.C, ...
//             xD42R.C, yD42R.C, zD42R.C, xxD42R.C, ...
//             xD43R.C, yD43R.C, zD43R.C, xxD43R.C, ...
//================================================================================

#include "MappedGridOperators.h"
#include "xD.h"


void 
xxDerivative.order.nd(const realMappedGridFunction & u,
	      RealArray & derivative,
	      const Index & I1,
	      const Index & I2,
	      const Index & I3,
	      const Index & N,
	      MappedGridOperators & mgop )
{                                                                        
  int numberOfDimensions = mgop.numberOfDimensions;
  RealArray & d12 = mgop.d12;
  RealArray & d22 = mgop.d22;
  RealArray & d14 = mgop.d14;
  RealArray & d24 = mgop.d24;

  RealArray & h21 = mgop.h21;
  RealArray & h22 = mgop.h22;
  RealArray & h41 = mgop.h41;
  RealArray & h42 = mgop.h42;
  
  RealArray & ur = mgop.ur;
  RealArray & us = mgop.us;
  RealArray & ut = mgop.ut;
  realMappedGridFunction & inverseVertexDerivative = mgop.mappedGrid.inverseVertexDerivative;

  int n;
  for( n=N.getBase(); n<=N.getBound(); n++ )                        
    derivative(I1,I2,I3,n)=UXX.order.nd(I1,I2,I3,n);                  
}
