//================================================================================
//  Define the vorticity
//
// Who to blame:  Bill Henshaw
//================================================================================

#include "MappedGridOperators.h"
#include "xD.h"

#define DERIVATIVE(type)                           \
  if( numberOfDimensions==2 )                 \
  {                                                \
    derivative(I1,I2,I3,0)=UX ## type(I1,I2,I3,1) - UY ## type(I1,I2,I3,0);   \
  }                                                 \
  else if( numberOfDimensions==3 )                 \
  {                                                \
    derivative(I1,I2,I3,0)=UY ## type(I1,I2,I3,2) - UZ ## type(I1,I2,I3,1);   \
    derivative(I1,I2,I3,1)=UZ ## type(I1,I2,I3,0) - UX ## type(I1,I2,I3,2);   \
    derivative(I1,I2,I3,2)=UX ## type(I1,I2,I3,1) - UY ## type(I1,I2,I3,0);   \
  }

void 
vorticityFDerivative(const realMappedGridFunction & ugf,
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

  if( mgop.isRectangular() )
  { // The grid is rectangular
    if( orderOfAccuracy==2 )
    {
      real h21c[3];
      for( int axis=0; axis<3; axis++ )
        h21c[axis]= 1./(2.*mgop.dx[axis]); 
#define h21(n) h21c[n]
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
      real h41c[3];
      for( int axis=0; axis<3; axis++ )
       h41c[axis]=1./(12.*mgop.dx[axis]);
#define h41(n) h41c[n]
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
