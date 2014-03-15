//================================================================================
// NOTES: 
//  o This file is processed by the perl script gDerivCoefficients.p to generate
//    functions for computing the coefficients of spatial derivatives x,y,z,xx,...
//  o These are used by the MappedGridOperators class
//  o see also pDerivCoefficients.C
//
// This file is the source for the following files
//             xFDerivCoefficients.C, 
//             yFDerivCoefficients.C, 
//             zFDerivCoefficients.C, 
//             xxFDerivCoefficients.C, 
//             xyFDerivCoefficients.C, 
//             xzFDerivCoefficients.C, 
//              ... etc ...
//================================================================================

#include "MappedGridOperators.h"
#include "xDC.h"

void 
xxFDerivCoefficients(RealDistributedArray & derivative,
		     const Index & I1,
		     const Index & I2,
		     const Index & I3,
		     const Index & E,
		     const Index & C,
		     MappedGridOperators & mgop )
{                                                                        
  int & numberOfDimensions = mgop.numberOfDimensions;
  int & orderOfAccuracy    = mgop.orderOfAccuracy;
  int & width              = mgop.width;
  int & halfWidth1         = mgop.halfWidth1;
  int & halfWidth2         = mgop.halfWidth2;
  int & halfWidth3         = mgop.halfWidth3;
  int & stencilSize        = mgop.stencilSize;
  int & numberOfComponentsForCoefficients = mgop.numberOfComponentsForCoefficients;

  int m1,m2,m3,n;
  int dum;
  Range aR0,aR1,aR2,aR3;
  RealArray & delta = mgop.delta;

  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
      : mgop.mappedGrid.inverseCenterDerivative();

  int e0=E.getBase();
  int c0=C.getBase();

  if( orderOfAccuracy==2 )
  {
    RealArray & d12 = mgop.d12;
    RealArray & d22 = mgop.d22;
    RealArray & Dr = mgop.Dr;
    RealArray & Ds = mgop.Ds;
    RealArray & Dt = mgop.Dt;
    RealArray & Drr= mgop.Drr;
    RealArray & Drs= mgop.Drs;
    RealArray & Drt= mgop.Drt;
    RealArray & Dss= mgop.Dss;
    RealArray & Dst= mgop.Dst;
    RealArray & Dtt= mgop.Dtt;
    if( numberOfDimensions==1 )
    { // get coefficients for the first component
      ForStencil(m1,m2,m3)                                            
      {                                                               
	UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=UXX21(I1,I2,I3,c0); 
      }
    }
    else if( numberOfDimensions==2 )
    {
      ForStencil(m1,m2,m3)                                            
      {                                                               
	UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=UXX22(I1,I2,I3,c0);    
      }
    }
    else // ======= 3D ================
    {
      ForStencil(m1,m2,m3)                                            
      {                                                               
	UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=UXX23(I1,I2,I3,c0);
      }
    }
  }
  else   // ====== 4th order =======
  {
    RealArray & d14 = mgop.d14;
    RealArray & d24 = mgop.d24;
    RealArray & Dr4 = mgop.Dr4;
    RealArray & Ds4 = mgop.Ds4;
    RealArray & Dt4 = mgop.Dt4;
    RealArray & Drr4= mgop.Drr4;
    RealArray & Drs4= mgop.Drs4;
    RealArray & Drt4= mgop.Drt4;
    RealArray & Dss4= mgop.Dss4;
    RealArray & Dst4= mgop.Dst4;
    RealArray & Dtt4= mgop.Dtt4;
    if( numberOfDimensions==1 )
    {
      ForStencil(m1,m2,m3)                                            
      {                                                               
	UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=UXX41(I1,I2,I3,c0);  
      }
    }
    else if( numberOfDimensions==2 )
    {
      ForStencil(m1,m2,m3)                                            
      {                                                               
	UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=UXX42(I1,I2,I3,c0);  
      }
    }
    else  // ======= 3D ================
    {
      ForStencil(m1,m2,m3)                                            
      {                                                               
	UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=UXX43(I1,I2,I3,c0);  
      }
    }
  }
  // now make copies for other components
  Index M(0,stencilSize);
  for( int c=C.getBase(); c<=C.getBound(); c++ )                        
  for( int e=E.getBase(); e<=E.getBound(); e++ )                        
    if( c!=c0 || e!=e0 )
      derivative(M+CE(c,e),I1,I2,I3)=derivative(M+CE(c0,e0),I1,I2,I3);
}


#undef CE
