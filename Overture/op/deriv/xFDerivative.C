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
//             xFDerivative.C, 
//             xyFDerivative.C, 
//             xzFDerivative.C, 
//              ... etc ...
//================================================================================

#include "MappedGridOperators.h"
#include "xD.h"

void 
xFDerivative42(const realMappedGridFunction & ugf,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 4th order 2d
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  const int & orderOfAccuracy = mgop.orderOfAccuracy;
  const RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  int n;
  const RealDistributedArray & ur = *mgop.urp;
  const RealDistributedArray & us = *mgop.usp;
  const RealDistributedArray & ut = *mgop.utp;

  const RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();

   // ====== 4th order =======

  RealArray d14; d14 = 1./(12.*mgop.mappedGrid.gridSpacing()); // mgop.d14;
  RealArray d24; d24 = 1./(12.*SQR(mgop.mappedGrid.gridSpacing())); // mgop.d24;
  if( numberOfDimensions==0 )
  { // these lines also prevent warnings about unused variables.
    printf("xFDerivative:ERROR: numberOfDimensions=%i, orderOfAccuracy=%i\n",numberOfDimensions,orderOfAccuracy);
    u.display("u"); ur.display("ur"); us.display("us"); ut.display("ut"); d14.display("d14"); d24.display("d24"); 
    inverseVertexDerivative.display("inverseVertexDerivative");
    Overture::abort("error");
  }

  for( n=N.getBase(); n<=N.getBound(); n++ )                        
    derivative(I1,I2,I3,n)=UX42(I1,I2,I3,n);                  
}

void 
xFDerivative43(const realMappedGridFunction & ugf,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 4th order 3d
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  const int & orderOfAccuracy = mgop.orderOfAccuracy;
  const RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  int n;
  const RealDistributedArray & ur = *mgop.urp;
  const RealDistributedArray & us = *mgop.usp;
  const RealDistributedArray & ut = *mgop.utp;

  const RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();

  RealArray d14; d14 = 1./(12.*mgop.mappedGrid.gridSpacing()); // mgop.d14;
  RealArray d24; d24 = 1./(12.*SQR(mgop.mappedGrid.gridSpacing())); // mgop.d24;

  if( numberOfDimensions==0 )
  { // these lines also prevent warnings about unused variables.
    printf("xFDerivative:ERROR: numberOfDimensions=%i, orderOfAccuracy=%i\n",numberOfDimensions,orderOfAccuracy);
    u.display("u"); ur.display("ur"); us.display("us"); ut.display("ut"); d14.display("d14"); d24.display("d24"); 
    inverseVertexDerivative.display("inverseVertexDerivative");
    Overture::abort("error");
  }
  // ====== 4th order =======

  for( n=N.getBase(); n<=N.getBound(); n++ )                        
      derivative(I1,I2,I3,n)=UX43(I1,I2,I3,n);                  
}

void 
xFDerivative22(const realMappedGridFunction & ugf,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  const int & orderOfAccuracy = mgop.orderOfAccuracy;
  const RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  int n;
  const RealDistributedArray & ur = *mgop.urp;
  const RealDistributedArray & us = *mgop.usp;
  const RealDistributedArray & ut = *mgop.utp;

  const RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();

  RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
  RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;

  if( numberOfDimensions==0 )
  { // these lines also prevent warnings about unused variables.
    printf("xFDerivative:ERROR: numberOfDimensions=%i, orderOfAccuracy=%i\n",numberOfDimensions,orderOfAccuracy);
    u.display("u"); ur.display("ur"); us.display("us"); ut.display("ut"); d12.display("d12"); d22.display("d22"); 
    inverseVertexDerivative.display("inverseVertexDerivative");
    Overture::abort("error");
  }

  for( n=N.getBase(); n<=N.getBound(); n++ )                        
    derivative(I1,I2,I3,n)=UX22(I1,I2,I3,n);                  
}

void 
xFDerivative23(const realMappedGridFunction & ugf,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  const int & orderOfAccuracy = mgop.orderOfAccuracy;
  const RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  int n;
  const RealDistributedArray & ur = *mgop.urp;
  const RealDistributedArray & us = *mgop.usp;
  const RealDistributedArray & ut = *mgop.utp;

  const RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();

  RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
  RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;

  if( numberOfDimensions==0 )
  { // these lines also prevent warnings about unused variables.
    printf("xFDerivative:ERROR: numberOfDimensions=%i, orderOfAccuracy=%i\n",numberOfDimensions,orderOfAccuracy);
    u.display("u"); ur.display("ur"); us.display("us"); ut.display("ut"); d12.display("d12"); d22.display("d22"); 
    inverseVertexDerivative.display("inverseVertexDerivative");
    Overture::abort("error");
  }

  for( n=N.getBase(); n<=N.getBound(); n++ )                        
    derivative(I1,I2,I3,n)=UX23(I1,I2,I3,n);                  
}


void 
xFDerivative(const realMappedGridFunction & ugf,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  const int & orderOfAccuracy = mgop.orderOfAccuracy;
  const RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  int n;
  if( mgop.isRectangular() )
  { // The grid is rectangular
    if( orderOfAccuracy==2 )
    {
      real h21c[3],h22c[3];
#define h21(n) h21c[n]
#define h22(n) h22c[n]
      for( int axis=0; axis<3; axis++ )
      {
	h21(axis)=1./(2.*mgop.dx[axis]); 
	h22(axis)=1./SQR(mgop.dx[axis]);
      }
//       const RealArray & h21 = mgop.h21;  // these are used in the macros
//       const RealArray & h22 = mgop.h22;
      if( numberOfDimensions==1 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=UX21R(I1,I2,I3,n);                  
      }
      else if(numberOfDimensions==2 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=UX22R(I1,I2,I3,n);                  
      }
      else if( numberOfDimensions==3 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=UX23R(I1,I2,I3,n);                  
      }
      else
      {  // these lines also prevent warnings about unused variables.
	printf("xFDerivative:ERROR: numberOfDimensions=%i, orderOfAccuracy=%i\n",numberOfDimensions,orderOfAccuracy);
	u.display("u"); // h21.display("h21"); h22.display("h22"); 
	Overture::abort("error");
      }
    }
    else   // ====== 4th order =======
    {
//       const RealArray & h41 = mgop.h41;
//       const RealArray & h42 = mgop.h42;
      real h41c[3],h42c[3];
#define h41(n) h41c[n]
#define h42(n) h42c[n]
      for( int axis=0; axis<3; axis++ )
      {
	h41(axis)=1./(12.*mgop.dx[axis]);
	h42(axis)=1./(12.*SQR(mgop.dx[axis]));
      }

      if( numberOfDimensions==1 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=UX41R(I1,I2,I3,n);                  
      }
      else if(numberOfDimensions==2 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=UX42R(I1,I2,I3,n);                  
      }
      else if( numberOfDimensions==3 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=UX43R(I1,I2,I3,n);                  
      }
      else
      {  // these lines also prevent warnings about unused variables.
	printf("xFDerivative:ERROR: numberOfDimensions=%i, orderOfAccuracy=%i\n",numberOfDimensions,orderOfAccuracy);
	u.display("u"); // h41.display("h21"); h42.display("h22"); 
	Overture::abort("error");
      }
    }
  }
  else 
  { // Ths grid is not rectangular
    const RealDistributedArray & ur = *mgop.urp;
    const RealDistributedArray & us = *mgop.usp;
    const RealDistributedArray & ut = *mgop.utp;

    const RealDistributedArray & inverseVertexDerivative = 
        int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative() 
                                                : mgop.mappedGrid.inverseCenterDerivative();

    if( orderOfAccuracy==2 )
    {
      RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
      RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;
      if( numberOfDimensions==1 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=UX21(I1,I2,I3,n);                  
      }
      else if(numberOfDimensions==2 )
        xFDerivative22(ugf,derivative,I1,I2,I3,N,mgop );
      else if( numberOfDimensions==3 )
        xFDerivative23(ugf,derivative,I1,I2,I3,N,mgop );
      else
      {  // these lines also prevent warnings about unused variables.
	printf("xFDerivative:ERROR: numberOfDimensions=%i, orderOfAccuracy=%i\n",numberOfDimensions,orderOfAccuracy);
	u.display("u"); ur.display("ur"); us.display("us"); ut.display("ut"); 
	inverseVertexDerivative.display("inverseVertexDerivative");
	Overture::abort("error");
      }
    }
    else   // ====== 4th order =======
    {
      RealArray d14; d14 = 1./(12.*mgop.mappedGrid.gridSpacing()); // mgop.d14;
      RealArray d24; d24 = 1./(12.*SQR(mgop.mappedGrid.gridSpacing())); // mgop.d24;
      if( numberOfDimensions==1 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=UX41(I1,I2,I3,n);                  
      }
      else if(numberOfDimensions==2 )
        xFDerivative42(ugf,derivative,I1,I2,I3,N,mgop );
      else if( numberOfDimensions==3 )
        xFDerivative43(ugf,derivative,I1,I2,I3,N,mgop );
      else
      {  // these lines also prevent warnings about unused variables.
	printf("xFDerivative:ERROR: numberOfDimensions=%i, orderOfAccuracy=%i\n",numberOfDimensions,orderOfAccuracy);
	u.display("u"); ur.display("ur"); us.display("us"); ut.display("ut"); 
	inverseVertexDerivative.display("inverseVertexDerivative");
	Overture::abort("error");
      }
    }
  }
}
