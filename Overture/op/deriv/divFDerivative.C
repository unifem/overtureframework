//================================================================================
//  Define the divergence
//
// Who to blame:  Bill Henshaw
//================================================================================

#include "MappedGridOperators.h"
#include "xD.h"

#define DERIVATIVE(type)                                              \
  if( mgop.mappedGrid.numberOfDimensions()==2 )                                    \
    derivative(I1,I2,I3)=UX ## type(I1,I2,I3,n)+UY ## type(I1,I2,I3,n+1); \
  else if( mgop.mappedGrid.numberOfDimensions()==3 )                                    \
    derivative(I1,I2,I3)=UX ## type(I1,I2,I3,n)+UY ## type(I1,I2,I3,n+1)+UZ ## type(I1,I2,I3,n+2); \
  else \
    derivative(I1,I2,I3)=UX ## type(I1,I2,I3,n);

void 
divFDerivative(const realMappedGridFunction & ugf,
	       RealDistributedArray & derivative,
	       const Index & I1,
	       const Index & I2,
	       const Index & I3,
	       const Index & N,
	       MappedGridOperators & mgop )
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  int & orderOfAccuracy = mgop.orderOfAccuracy;
  RealDistributedArray & u = (RealDistributedArray &) ugf;

  int n=N.getBase();
  if( mgop.isRectangular() )
  { // The grid is rectangular
    if( orderOfAccuracy==2 )
    {
//      RealArray & h21 = mgop.h21;  // these are used in the macros
      real h21c[3];
#define h21(n) h21c[n]
      for( int axis=0; axis<3; axis++ )
	h21(axis)=1./(2.*mgop.dx[axis]); 

      if( numberOfDimensions==1 )
      {
        DERIVATIVE(21R)
      }
      else if(numberOfDimensions==2 )
      {
        DERIVATIVE(22R)
      }
      else // ======= 3D ================
      {
        DERIVATIVE(23R)
      }
    }
    else   // ====== 4th order =======
    {
//      RealArray & h41 = mgop.h41;
      real h41c[3];
#define h41(n) h41c[n]
      for( int axis=0; axis<3; axis++ )
	h41(axis)=1./(12.*mgop.dx[axis]);
      if( numberOfDimensions==1 )
      {
        DERIVATIVE(41R)
      }
      else if(numberOfDimensions==2 )
      {
        DERIVATIVE(42R)
      }
      else  // ======= 3D ================
      {
        DERIVATIVE(43R)
      }
    }
  }
  else 
  { // Ths grid is not rectangular
    RealDistributedArray & ur = *mgop.urp;
    RealDistributedArray & us = *mgop.usp;
    RealDistributedArray & ut = *mgop.utp;

    RealDistributedArray & inverseVertexDerivative = 
          int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
                                                   : mgop.mappedGrid.inverseCenterDerivative();
    if( orderOfAccuracy==2 )
    {
      RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
      if( !mgop.usingConservativeApproximations() )
      {
	if( numberOfDimensions==1 )
	{
	  DERIVATIVE(21)
	}
	else if(numberOfDimensions==2 )
	{
	  DERIVATIVE(22)
	}
	else // ======= 3D ================
	{
	  DERIVATIVE(23)
	}
      }
      else
      {
	if( numberOfDimensions==1 )
	{
	  DERIVATIVE(21)  // already conservative
	}
	else if(numberOfDimensions==2 )
	{
	  const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();
	  derivative(I1,I2,I3,n)=
	    (
	      (j(I1+1,I2,I3)*( RX(I1+1,I2,I3)*u(I1+1,I2,I3,n)+RY(I1+1,I2,I3)*u(I1+1,I2,I3,n+1) )-
               j(I1-1,I2,I3)*( RX(I1-1,I2,I3)*u(I1-1,I2,I3,n)+RY(I1-1,I2,I3)*u(I1-1,I2,I3,n+1) ) )*d12(axis1)+
	      (j(I1,I2+1,I3)*( SX(I1,I2+1,I3)*u(I1,I2+1,I3,n)+SY(I1,I2+1,I3)*u(I1,I2+1,I3,n+1) )-
               j(I1,I2-1,I3)*( SX(I1,I2-1,I3)*u(I1,I2-1,I3,n)+SY(I1,I2-1,I3)*u(I1,I2-1,I3,n+1) ) )*d12(axis2)
	    )/j(I1,I2,I3);
	}
	else // ======= 3D ================
	{
	  const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();
	  derivative(I1,I2,I3,n)=
	    (
	      (j(I1+1,I2,I3)*( RX(I1+1,I2,I3)*u(I1+1,I2,I3,n  )+
                               RY(I1+1,I2,I3)*u(I1+1,I2,I3,n+1)+
                               RZ(I1+1,I2,I3)*u(I1+1,I2,I3,n+2) )-
               j(I1-1,I2,I3)*( RX(I1-1,I2,I3)*u(I1-1,I2,I3,n  )+
                               RY(I1-1,I2,I3)*u(I1-1,I2,I3,n+1)+
                               RZ(I1-1,I2,I3)*u(I1-1,I2,I3,n+2) ) )*d12(axis1)+
	      (j(I1,I2+1,I3)*( SX(I1,I2+1,I3)*u(I1,I2+1,I3,n  )+
                               SY(I1,I2+1,I3)*u(I1,I2+1,I3,n+1)+
                               SZ(I1,I2+1,I3)*u(I1,I2+1,I3,n+2) )-
               j(I1,I2-1,I3)*( SX(I1,I2-1,I3)*u(I1,I2-1,I3,n  )+
                               SY(I1,I2-1,I3)*u(I1,I2-1,I3,n+1)+
                               SZ(I1,I2-1,I3)*u(I1,I2-1,I3,n+2) ) )*d12(axis2)+
	      (j(I1,I2,I3+1)*( TX(I1,I2,I3+1)*u(I1,I2,I3+1,n  )+
                               TY(I1,I2,I3+1)*u(I1,I2,I3+1,n+1)+
                               TZ(I1,I2,I3+1)*u(I1,I2,I3+1,n+2) )-
               j(I1,I2,I3-1)*( TX(I1,I2,I3-1)*u(I1,I2,I3-1,n  )+
                               TY(I1,I2,I3-1)*u(I1,I2,I3-1,n+1)+
                               TZ(I1,I2,I3-1)*u(I1,I2,I3-1,n+2) ) )*d12(axis3)
	    )/j(I1,I2,I3);
	  
	}
      }
    }
    else   // ====== 4th order =======
    {
      if( numberOfDimensions==1 )
      {
        DERIVATIVE(41)
      }
      else if(numberOfDimensions==2 )
      {
        DERIVATIVE(42)
      }
      else  // ======= 3D ================
      {
        DERIVATIVE(43)
      }
    }
  }
}
#undef DERIVATIVE
