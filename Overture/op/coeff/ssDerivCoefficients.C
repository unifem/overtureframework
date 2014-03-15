//================================================================================
// NOTE: This file is processed by the perl script gDerivCoefficients.p to generate
// functions for computing the coefficients of the spatial derivatives x,y,z,xx
//
// This file is the source for the following files
//             rDerivCoefficients.C, 
//             sDerivCoefficients.C, 
//             tDerivCoefficients.C, 
//             ssDerivCoefficients.C, 
//             rsDerivCoefficients.C, 
//             rtDerivCoefficients.C, 
//              ... etc ...
//================================================================================

#include "MappedGridOperators.h"
#include "xDC.h"

void 
ssDerivCoefficients(RealDistributedArray & derivative,
		    const Index & I1,
		    const Index & I2,
		    const Index & I3,
		    const Index & E,
		    const Index & C,
		    MappedGridOperators & mgop )
{                                                                        
  // fix this for new version
  Overture::abort("error");
/* ---  
  int & numberOfDimensions = mgop.numberOfDimensions;
  int & orderOfAccuracy    = mgop.orderOfAccuracy;
  int & width              = mgop.width;
  int & halfWidth1         = mgop.halfWidth1;
  int & halfWidth2         = mgop.halfWidth2;
  int & halfWidth3         = mgop.halfWidth3;
  int stencilSize          = mgop.stencilSize;
  int & numberOfComponentsForCoefficients = mgop.numberOfComponentsForCoefficients;

  int m1,m2,m3;
  int dum;
  Range aR0,aR1,aR2,aR3;
  RealArray & delta = mgop.delta;
  int e0=E.getBase();
  int c0=C.getBase();

  if( orderOfAccuracy==2 )
  { 
    RealArray & Dr = mgop.Dr;
    RealArray & Ds = mgop.Ds;
    RealArray & Dt = mgop.Dt;
    RealArray & Drr= mgop.Drr;
    RealArray & Drs= mgop.Drs;
    RealArray & Drt= mgop.Drt;
    RealArray & Dss= mgop.Dss;
    RealArray & Dst= mgop.Dst;
    RealArray & Dtt= mgop.Dtt;
    RealArray & d12 = mgop.d12;
    RealArray & d22 = mgop.d22;
    if( numberOfDimensions==0 )
    { // these lines also prevent warnings about unused variables.
      printf("xFDerivative:ERROR: numberOfDimensions=%i, orderOfAccuracy=%i\n",numberOfDimensions,orderOfAccuracy);
      Dr.display("Dr"); Ds.display("Ds"); Dt.display("Dt"); Drr.display("Drr"); Drs.display("Drs"); 
      Drt.display("Drt"); Dss.display("Dss"); Dst.display("Dst"); Dtt.display("Dtt");
      d12.display("d12"); d22.display("d22"); 
      Overture::abort("error");
    }

    ForStencil(m1,m2,m3)                                            
    {                                                               
      UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=USS2(I1,I2,I3,c0);  
    }                                                               
  }
  else // ====== 4th order =======
  {
    RealArray & Dr4 = mgop.Dr4;
    RealArray & Ds4 = mgop.Ds4;
    RealArray & Dt4 = mgop.Dt4;
    RealArray & Drr4= mgop.Drr4;
    RealArray & Drs4= mgop.Drs4;
    RealArray & Drt4= mgop.Drt4;
    RealArray & Dss4= mgop.Dss4;
    RealArray & Dst4= mgop.Dst4;
    RealArray & Dtt4= mgop.Dtt4;
    RealArray & d14 = mgop.d14;
    RealArray & d24 = mgop.d24;
    if( numberOfDimensions==0 )
    { // these lines also prevent warnings about unused variables.
      printf("xFDerivative:ERROR: numberOfDimensions=%i, orderOfAccuracy=%i\n",numberOfDimensions,orderOfAccuracy);
      Dr4.display("Dr4"); Ds4.display("Ds4"); Dt4.display("Dt4"); Drr4.display("Drr4"); Drs4.display("Drs4"); 
      Drt4.display("Drt4"); Dss4.display("Dss4"); Dst4.display("Dst4"); Dtt4.display("Dtt4");
      d14.display("d14"); d24.display("d24"); 
      Overture::abort("error");
    }

    ForStencil(m1,m2,m3)                                            
    {                                                               
      UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=USS4(I1,I2,I3,c0);  
    }                                                               
  }

  // now make copies for other components
  Index M(0,stencilSize);
  for( int c=C.getBase(); c<=C.getBound(); c++ )                        
  for( int e=E.getBase(); e<=E.getBound(); e++ )                        
    if( c!=c0 || e!=e0 )
      derivative(M+CE(c,e),I1,I2,I3)=derivative(M+CE(c0,e0),I1,I2,I3);
---- */
}
