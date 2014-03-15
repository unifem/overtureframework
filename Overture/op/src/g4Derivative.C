//================================================================================
// NOTES: 
//  o This file is processed by the perl script gDerivative.p to generate
//    functions for spatial derivatives x,y,z,xx,...
//  o These derivatives are used by the MappedGridOperators class
//
// This file is the source for the following files
//             xFDerivative.C, 
//             yFDerivative.C, 
//             zFDerivative.C, 
//             xxFDerivative.C, 
//             xyFDerivative.C, 
//             xzFDerivative.C, 
//              ... etc ...
//================================================================================

#include "MappedGridOperators.h"
#include "xD.h"

void 
xx4FDerivative(const realMappedGridFunction & u,
	     RealArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
{                                                                        
  int & numberOfDimensions = mgop.numberOfDimensions();
  assert( mgop.orderOfAccuracy==4 );

  int n;
  if( mgop.rectangular )
  { // The grid is rectangular
    RealArray & h41 = mgop.h41;
    RealArray & h42 = mgop.h42;
    if( numberOfDimensions==1 )
    {
      for( n=N.getBase(); n<=N.getBound(); n++ )                        
	derivative(I1,I2,I3,n)=UXX41R(I1,I2,I3,n);                  
    }
    else if(numberOfDimensions==2 )
    {
      for( n=N.getBase(); n<=N.getBound(); n++ )                        
	derivative(I1,I2,I3,n)=UXX42R(I1,I2,I3,n);                  
    }
    else  // ======= 3D ================
    {
      for( n=N.getBase(); n<=N.getBound(); n++ )                        
	derivative(I1,I2,I3,n)=UXX43R(I1,I2,I3,n);                  
    }
  }
  else 
  { // Ths grid is not rectangular
    RealArray & ur = mgop.ur;
    RealArray & us = mgop.us;
    RealArray & ut = mgop.ut;

    realMappedGridFunction & inverseVertexDerivative = 
      int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative  
	: mgop.mappedGrid.inverseCenterDerivative;

    RealArray & d14 = mgop.d14;
    RealArray & d24 = mgop.d24;
    if( numberOfDimensions==1 )
    {
      for( n=N.getBase(); n<=N.getBound(); n++ )                        
	derivative(I1,I2,I3,n)=UXX41(I1,I2,I3,n);                  
    }
    else if(numberOfDimensions==2 )
    {
      for( n=N.getBase(); n<=N.getBound(); n++ )                        
	derivative(I1,I2,I3,n)=UXX42(I1,I2,I3,n);                  
    }
    else  // ======= 3D ================
    {
      for( n=N.getBase(); n<=N.getBound(); n++ )                        
	derivative(I1,I2,I3,n)=UXX43(I1,I2,I3,n);                  
    }
  }
}
