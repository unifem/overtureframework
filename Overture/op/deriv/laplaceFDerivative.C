//================================================================================
// NOTES: 
//================================================================================

#include "MappedGridOperators.h"
#include "xD.h"

void 
divScalarGradFDerivative(const realMappedGridFunction & ugf,
			 const realMappedGridFunction & s,
			 RealDistributedArray & derivative,
			 const Index & I1,
			 const Index & I2,
			 const Index & I3,
			 const Index & N,
			 MappedGridOperators & mgop );


void 
laplaceFDerivative43(const realMappedGridFunction & ugf,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 3d fourth order
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  // int & orderOfAccuracy = mgop.orderOfAccuracy;
  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  int n;

  RealDistributedArray & ur = *mgop.urp;
  RealDistributedArray & us = *mgop.usp;
  RealDistributedArray & ut = *mgop.utp;

  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();

  RealArray d14; d14 = 1./(12.*mgop.mappedGrid.gridSpacing()); // mgop.d14;
  RealArray d24; d24 = 1./(12.*SQR(mgop.mappedGrid.gridSpacing())); // mgop.d24;

  // ======= 3D ================
//        for( n=N.getBase(); n<=N.getBound(); n++ )                        
//          derivative(I1,I2,I3,n)=LAPLACIAN43(I1,I2,I3,n);                  
  const realArray & rx = RX(I1,I2,I3);
  const realArray & ry = RY(I1,I2,I3);
  const realArray & rz = RZ(I1,I2,I3);

  const realArray & sx = SX(I1,I2,I3);
  const realArray & sy = SY(I1,I2,I3);
  const realArray & sz = SZ(I1,I2,I3);

  const realArray & tx = TX(I1,I2,I3);
  const realArray & ty = TY(I1,I2,I3);
  const realArray & tz = TZ(I1,I2,I3);

  const realArray & rxx = RXX43(I1,I2,I3);
  const realArray & ryy = RYY43(I1,I2,I3);
  const realArray & rzz = RZZ43(I1,I2,I3);

  const realArray & sxx = SXX43(I1,I2,I3);
  const realArray & syy = SYY43(I1,I2,I3);
  const realArray & szz = SZZ43(I1,I2,I3);

  const realArray & txx = TXX43(I1,I2,I3);
  const realArray & tyy = TYY43(I1,I2,I3);
  const realArray & tzz = TZZ43(I1,I2,I3);

	
  for( n=N.getBase(); n<=N.getBound(); n++ )                        
  {
    const realArray & urr = URR4(I1,I2,I3,n);
    const realArray & uss = USS4(I1,I2,I3,n);
    const realArray & utt = UTT4(I1,I2,I3,n);
    const realArray & urs = URS4(I1,I2,I3,n);
    const realArray & urt = URT4(I1,I2,I3,n);
    const realArray & ust = UST4(I1,I2,I3,n);
	  
    derivative(I1,I2,I3,n)=
      (                             
	(SQR(rx)+SQR(ry)+SQR(rz))     
	*urr
	+(SQR(sx)+SQR(sy)+SQR(sz))     
	*uss
	+(SQR(tx)+SQR(ty)+SQR(tz))    
	*utt
	+2.*(rx*sx+ ry*sy    
	     +rz*sz)   *urs
	+2.*(rx*tx+ ry*ty    
	     +rz*tz)   *urt
	+2.*(sx*tx+ sy*ty    
	     +sz*tz)   *ust
	+(rxx+ryy+rzz)      
	*UR4(I1,I2,I3,n)         
	+(sxx+syy+szz)      
	*US4(I1,I2,I3,n)         
	+(txx+tyy+tzz)      
	*UT4(I1,I2,I3,n)
	);        
  }
}

void 
laplaceFDerivative42(const realMappedGridFunction & ugf,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 2d fourth order
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  // int & orderOfAccuracy = mgop.orderOfAccuracy;
  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  int n;

  RealDistributedArray & ur = *mgop.urp;
  RealDistributedArray & us = *mgop.usp;

  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();

  RealArray d14; d14 = 1./(12.*mgop.mappedGrid.gridSpacing()); // mgop.d14;
  RealArray d24; d24 = 1./(12.*SQR(mgop.mappedGrid.gridSpacing())); // mgop.d24;

//        for( n=N.getBase(); n<=N.getBound(); n++ )                        
//          derivative(I1,I2,I3,n)=LAPLACIAN42(I1,I2,I3,n);                  
  const realArray & rx = RX(I1,I2,I3);
  const realArray & ry = RY(I1,I2,I3);

  const realArray & sx = SX(I1,I2,I3);
  const realArray & sy = SY(I1,I2,I3);

  const realArray & rxx = RXX4(I1,I2,I3);
  const realArray & ryy = RYY4(I1,I2,I3);

  const realArray & sxx = SXX4(I1,I2,I3);
  const realArray & syy = SYY4(I1,I2,I3);

	
  for( n=N.getBase(); n<=N.getBound(); n++ )                        
  {
    const realArray & urr = URR4(I1,I2,I3,n);
    const realArray & uss = USS4(I1,I2,I3,n);
    const realArray & urs = URS4(I1,I2,I3,n);
    derivative(I1,I2,I3,n)=
      (                             
	(SQR(rx)+SQR(ry))*urr
	+2.*(rx*sx+ ry*sy) 
	*urs
	+(SQR(sx)+SQR(sy))*uss
	+(rxx+ryy)*UR4(I1,I2,I3,n)     
	+(sxx+syy)*US4(I1,I2,I3,n)     
	);
	  
  }
}

void 
laplaceFDerivative23(const realMappedGridFunction & ugf,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 2nd-order, 3d
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  int n;
  RealDistributedArray & ur = *mgop.urp;
  RealDistributedArray & us = *mgop.usp;
  RealDistributedArray & ut = *mgop.utp;

  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();

  RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
  RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;
  // ======= 3D ================

//        for( n=N.getBase(); n<=N.getBound(); n++ )                        
//          derivative(I1,I2,I3,n)=LAPLACIAN23(I1,I2,I3,n);                  
  const realArray & rx = RX(I1,I2,I3);
  const realArray & ry = RY(I1,I2,I3);
  const realArray & rz = RZ(I1,I2,I3);

  const realArray & sx = SX(I1,I2,I3);
  const realArray & sy = SY(I1,I2,I3);
  const realArray & sz = SZ(I1,I2,I3);

  const realArray & tx = TX(I1,I2,I3);
  const realArray & ty = TY(I1,I2,I3);
  const realArray & tz = TZ(I1,I2,I3);

  const realArray & rxx = RXX23(I1,I2,I3);
  const realArray & ryy = RYY23(I1,I2,I3);
  const realArray & rzz = RZZ23(I1,I2,I3); 

  const realArray & sxx = SXX23(I1,I2,I3);
  const realArray & syy = SYY23(I1,I2,I3);
  const realArray & szz = SZZ23(I1,I2,I3);

  const realArray & txx = TXX23(I1,I2,I3);
  const realArray & tyy = TYY23(I1,I2,I3);
  const realArray & tzz = TZZ23(I1,I2,I3);

  for( n=N.getBase(); n<=N.getBound(); n++ )                        
  {
    const realArray & urr = URR2(I1,I2,I3,n);
    const realArray & uss = USS2(I1,I2,I3,n);
    const realArray & utt = UTT2(I1,I2,I3,n);
    const realArray & urs = URS2(I1,I2,I3,n);
    const realArray & urt = URT2(I1,I2,I3,n);
    const realArray & ust = UST2(I1,I2,I3,n);

    derivative(I1,I2,I3,n)=
      (                             
	(SQR(rx)+SQR(ry)+SQR(rz))     
	*urr
	+(SQR(sx)+SQR(sy)+SQR(sz))     
	*uss
	+(SQR(tx)+SQR(ty)+SQR(tz))    
	*utt
	+2.*(rx*sx+ ry*sy    
	     +rz*sz)   *urs
	+2.*(rx*tx+ ry*ty    
	     +rz*tz)   *urt
	+2.*(sx*tx+ sy*ty    
	     +sz*tz)   *ust
	+(rxx+ryy+rzz)      
	*UR2(I1,I2,I3,n)         
	+(sxx+syy+szz)      
	*US2(I1,I2,I3,n)         
	+(txx+tyy+tzz)      
	*UT2(I1,I2,I3,n)
	);        
  }
}

void 
laplaceFDerivative22(const realMappedGridFunction & ugf,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 2nd-order, 2d
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  int n;
  RealDistributedArray & ur = *mgop.urp;
  RealDistributedArray & us = *mgop.usp;
  // RealDistributedArray & ut = *mgop.utp;

  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();

  RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
  RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;

  for( n=N.getBase(); n<=N.getBound(); n++ )                        
    derivative(I1,I2,I3,n)=LAPLACIAN22(I1,I2,I3,n);                  

}


void 
laplaceFDerivative(const realMappedGridFunction & ugf,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  int & orderOfAccuracy = mgop.orderOfAccuracy;
  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  int n;
  if( mgop.isRectangular() )
  { // The grid is rectangular
    if( orderOfAccuracy==2 )
    {
      real h22c[3];
#define h22(n) h22c[n]
      for( int axis=0; axis<3; axis++ )
      {
	h22(axis)=1./SQR(mgop.dx[axis]);
      }
      if( numberOfDimensions==1 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=LAPLACIAN21R(I1,I2,I3,n);                  
      }
      else if(numberOfDimensions==2 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=LAPLACIAN22R(I1,I2,I3,n);                  
      }
      else // ======= 3D ================
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=LAPLACIAN23R(I1,I2,I3,n);                  
      }
    }
    else   // ====== 4th order =======
    {
      real h42c[3];
#define h42(n) h42c[n]
      for( int axis=0; axis<3; axis++ )
	h42(axis)=1./(12.*SQR(mgop.dx[axis]));
      if( numberOfDimensions==1 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=LAPLACIAN41R(I1,I2,I3,n);                  
      }
      else if(numberOfDimensions==2 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=LAPLACIAN42R(I1,I2,I3,n);                  
      }
      else  // ======= 3D ================
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=LAPLACIAN43R(I1,I2,I3,n);                  
      }
    }
  }
  else 
  { // Ths grid is not rectangular
    if( mgop.usingConservativeApproximations() && orderOfAccuracy==2 )
    {
      realMappedGridFunction scalar(mgop.mappedGrid);   // **** do this for now -- fix this 
      scalar=1.;
      divScalarGradFDerivative(ugf,scalar,derivative,I1,I2,I3,N,mgop);
      return;
    }

    RealDistributedArray & inverseVertexDerivative = 
          int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
                                                   : mgop.mappedGrid.inverseCenterDerivative();

    if( orderOfAccuracy==2 )
    {
      RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;

      if( numberOfDimensions==1 )
      {
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=LAPLACIAN21(I1,I2,I3,n);                  
      }
      else if(numberOfDimensions==2 )
      {
        // for( n=N.getBase(); n<=N.getBound(); n++ )                        
        //  derivative(I1,I2,I3,n)=LAPLACIAN22(I1,I2,I3,n);                  
        laplaceFDerivative22(ugf,derivative,I1,I2,I3,N,mgop );
      }
      else // ======= 3D ================
      {
//        for( n=N.getBase(); n<=N.getBound(); n++ )                        
//          derivative(I1,I2,I3,n)=LAPLACIAN23(I1,I2,I3,n);                  
        laplaceFDerivative23(ugf,derivative,I1,I2,I3,N,mgop );
      }
    }
    else   // ====== 4th order =======
    {
      if( numberOfDimensions==1 )
      {
        RealArray d24; d24 = 1./(12.*SQR(mgop.mappedGrid.gridSpacing())); // mgop.d24;
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=LAPLACIAN41(I1,I2,I3,n);                  
      }
      else if(numberOfDimensions==2 )
      {
//        for( n=N.getBase(); n<=N.getBound(); n++ )                        
//          derivative(I1,I2,I3,n)=LAPLACIAN42(I1,I2,I3,n);                  
        laplaceFDerivative42(ugf,derivative,I1,I2,I3,N,mgop );

      }
      else  // ======= 3D ================
      {
        laplaceFDerivative43(ugf,derivative,I1,I2,I3,N,mgop );
//        for( n=N.getBase(); n<=N.getBound(); n++ )                        
//          derivative(I1,I2,I3,n)=LAPLACIAN43(I1,I2,I3,n);                  
      }
    }
  }
}















