//================================================================================
// Special Cases
//
// NOTE: This file is processed by the perl script xD.p to generate
// functions for special types of derivatives
//
//  This file is the source for
//        div22.C, div23.C, div42.C, div43.C
//        div22R.C, div23R.C, div42R.C, div43R.C
//        grad22.C, grad23.C, grad42.C, grad43.C
//        grad22R.C, grad23R.C, grad42R.C, grad43R.C
//
//================================================================================

#include "MappedGridOperators.h"
#include "xD.h"


void 
div(const realMappedGridFunction & u,
    RealArray & derivative,
    const Index & I1,
    const Index & I2,
    const Index & I3,
    const Index & N,
    MappedGridOperators & mgop )
{                                                                        
  int & numberOfDimensions = mgop.numberOfDimensions;
  int & orderOfAccuracy = mgop.orderOfAccuracy;

  int n;
  if( mgop.rectangular )
  { // The grid is rectangular
    RealArray & h21 = mgop.h21;  // these are used in the macros
    RealArray & h22 = mgop.h22;
    RealArray & h41 = mgop.h41;
    RealArray & h42 = mgop.h42;
    if( numberOfDimensions==2 )
    {
      if( orderOfAccuracy==2 )
      {
	derivative(I1,I2,I3)=UX22R(I1,I2,I3,0)+UY22R(I1,I2,I3,1);
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3)+=UZ22R(I1,I2,I3,2);                    
      }
      else // ====== 4th order =======
      {
	derivative(I1,I2,I3)=UX42R(I1,I2,I3,0)+UY42R(I1,I2,I3,1);
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3)+=UZ42R(I1,I2,I3,2);                    
      }
    }
    else   // ======= 3D ================
    {
      if( orderOfAccuracy==2 )
      {
	derivative(I1,I2,I3)=UX32R(I1,I2,I3,0)+UY32R(I1,I2,I3,1);
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3)+=UZ32R(I1,I2,I3,2);                    
      }
      else  // ====== 4th order =======
      {
	derivative(I1,I2,I3)=UX43R(I1,I2,I3,0)+UY43R(I1,I2,I3,1);
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3)+=UZ43R(I1,I2,I3,2);                    
      }
    }
  }
  else 
  { // Ths grid is not rectangular
    RealArray & d12 = mgop.d12;
    RealArray & d22 = mgop.d22;
    RealArray & d14 = mgop.d14;
    RealArray & d24 = mgop.d24;

    RealArray & ur = mgop.ur;
    RealArray & us = mgop.us;
    RealArray & ut = mgop.ut;

    realMappedGridFunction & inverseVertexDerivative = 
      int(mgop.mappedGrid.isAllVertexCentered) ? mgop.mappedGrid.inverseVertexDerivative  
	: mgop.mappedGrid.inverseCenterDerivative;

    if( numberOfDimensions==2 )
    {
      if( orderOfAccuracy==2 )
      {
	derivative(I1,I2,I3)=UX22(I1,I2,I3,0)+UY22(I1,I2,I3,1);
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3)+=UZ22(I1,I2,I3,2);                    
      }
      else // ====== 4th order =======
      {
	derivative(I1,I2,I3)=UX42(I1,I2,I3,0)+UY42(I1,I2,I3,1);
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3)+=UZ42(I1,I2,I3,2);                    
      }
    }
    else   // ======= 3D ================
    {
      if( orderOfAccuracy==2 )
      {
	derivative(I1,I2,I3)=UX32(I1,I2,I3,0)+UY32(I1,I2,I3,1);
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3)+=UZ32(I1,I2,I3,2);                    
      }
      else  // ====== 4th order =======
      {
	derivative(I1,I2,I3)=UX43(I1,I2,I3,0)+UY43(I1,I2,I3,1);
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3)+=UZ43(I1,I2,I3,2);                    
      }
    }
  }
}


void 
grad(const realMappedGridFunction & u,
     RealArray & derivative,
     const Index & I1,
     const Index & I2,
     const Index & I3,
     const Index & N,
     MappedGridOperators & mgop )
{                                                                        
  int & numberOfDimensions = mgop.numberOfDimensions;
  int & orderOfAccuracy = mgop.orderOfAccuracy;

  int n;
  if( mgop.rectangular )
  { // The grid is rectangular
    RealArray & h21 = mgop.h21;  // these are used in the macros
    RealArray & h22 = mgop.h22;
    RealArray & h41 = mgop.h41;
    RealArray & h42 = mgop.h42;
    if( numberOfDimensions==2 )
    {
      if( orderOfAccuracy==2 )
      {
	derivative(I1,I2,I3,0)=UX22R(I1,I2,I3,0);                    
	derivative(I1,I2,I3,1)=UY22R(I1,I2,I3,0);                    
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3,2)=UZ22R(I1,I2,I3,0);                   
      }
      else // ====== 4th order =======
      {
	derivative(I1,I2,I3,0)=UX42R(I1,I2,I3,0);                    
	derivative(I1,I2,I3,1)=UY42R(I1,I2,I3,0);                    
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3,2)=UZ42R(I1,I2,I3,0);                   
      }
    }
    else   // ======= 3D ================
    {
      if( orderOfAccuracy==2 )
      {
	derivative(I1,I2,I3,0)=UX23R(I1,I2,I3,0);                    
	derivative(I1,I2,I3,1)=UY23R(I1,I2,I3,0);                    
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3,2)=UZ23R(I1,I2,I3,0);                   
      }
      else  // ====== 4th order =======
      {
	derivative(I1,I2,I3,0)=UX43R(I1,I2,I3,0);                    
	derivative(I1,I2,I3,1)=UY43R(I1,I2,I3,0);                    
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3,2)=UZ43R(I1,I2,I3,0);                   
      }
    }
  }
  else 
  { // Ths grid is not rectangular
    RealArray & d12 = mgop.d12;
    RealArray & d22 = mgop.d22;
    RealArray & d14 = mgop.d14;
    RealArray & d24 = mgop.d24;

    RealArray & ur = mgop.ur;
    RealArray & us = mgop.us;
    RealArray & ut = mgop.ut;

    realMappedGridFunction & inverseVertexDerivative = 
      int(mgop.mappedGrid.isAllVertexCentered) ? mgop.mappedGrid.inverseVertexDerivative  
	: mgop.mappedGrid.inverseCenterDerivative;

    if( numberOfDimensions==2 )
    {
      if( orderOfAccuracy==2 )
      {
	derivative(I1,I2,I3,0)=UX22(I1,I2,I3,0);                    
	derivative(I1,I2,I3,1)=UY22(I1,I2,I3,0);                    
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3,2)=UZ22(I1,I2,I3,0);                   
      }
      else // ====== 4th order =======
      {
	derivative(I1,I2,I3,0)=UX42(I1,I2,I3,0);                    
	derivative(I1,I2,I3,1)=UY42(I1,I2,I3,0);                    
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3,2)=UZ42(I1,I2,I3,0);                   
      }
    }
    else   // ======= 3D ================
    {
      if( orderOfAccuracy==2 )
      {
	derivative(I1,I2,I3,0)=UX23(I1,I2,I3,0);                    
	derivative(I1,I2,I3,1)=UY23(I1,I2,I3,0);                    
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3,2)=UZ23(I1,I2,I3,0);                   
      }
      else  // ====== 4th order =======
      {
	derivative(I1,I2,I3,0)=UX43(I1,I2,I3,0);                    
	derivative(I1,I2,I3,1)=UY43(I1,I2,I3,0);                    
	if( mgop.numberOfDimensions==3 )                                       
	  derivative(I1,I2,I3,2)=UZ43(I1,I2,I3,0);                   
      }
    }
  }
}
