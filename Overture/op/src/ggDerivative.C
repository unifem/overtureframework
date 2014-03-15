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
xxFDerivative(const realMappedGridFunction & ugf,
	     RealArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
{                                                                        
  int & numberOfDimensions = mgop.numberOfDimensions();
  int & orderOfAccuracy = mgop.orderOfAccuracy;
  RealArray & u = (RealArray &)ugf;

  int n;
  if( mgop.rectangular )
  { // The grid is rectangular
    if( orderOfAccuracy==2 )
    {
      RealArray & h21 = mgop.h21;  // these are used in the macros
      RealArray & h22 = mgop.h22;
      if( numberOfDimensions==1 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=UXX21R(I1,I2,I3,n);                  
      }
      else if(numberOfDimensions==2 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=UXX22R(I1,I2,I3,n);                  
      }
      else // ======= 3D ================
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=UXX23R(I1,I2,I3,n);                  
      }
    }
    else   // ====== 4th order =======
    {
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
  }
  else 
  { // Ths grid is not rectangular
    RealArray & ur = mgop.ur;
    RealArray & us = mgop.us;
    RealArray & ut = mgop.ut;

    RealArray & inverseVertexDerivative = 
          int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative  
                                                   : mgop.mappedGrid.inverseCenterDerivative;

    if( orderOfAccuracy==2 )
    {
      RealArray & d12 = mgop.d12;
      RealArray & d22 = mgop.d22;
      if( numberOfDimensions==1 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=UXX21(I1,I2,I3,n);                  
      }
      else if(numberOfDimensions==2 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
	{
          derivative(I1,I2,I3,n) =UXX22A(I1,I2,I3,n);
          derivative(I1,I2,I3,n)+=UXX22B(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX22C(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX22D(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX22E(I1,I2,I3,n);                  
	}
      }
      else // ======= 3D ================
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
	{
          derivative(I1,I2,I3,n) =UXX23A(I1,I2,I3,n);
          derivative(I1,I2,I3,n)+=UXX23B(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX23C(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX23D(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX23E(I1,I2,I3,n);                  
	}
      }
    }
    else   // ====== 4th order =======
    {
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
	{
          derivative(I1,I2,I3,n) =UXX42A(I1,I2,I3,n);
          derivative(I1,I2,I3,n)+=UXX42B(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX42C(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX42D(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX42E(I1,I2,I3,n);                  
	}
      }
      else  // ======= 3D ================
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )
	{
          derivative(I1,I2,I3,n) =UXX43A(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX43B(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX43C(I1,I2,I3,n);
          derivative(I1,I2,I3,n)+=UXX43D(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX43E(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX43F(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX43G(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX43H(I1,I2,I3,n);                  
          derivative(I1,I2,I3,n)+=UXX43I(I1,I2,I3,n);                  
	}
      }
    }
  }
}
