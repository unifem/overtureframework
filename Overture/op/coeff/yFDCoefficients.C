//================================================================================
//   Define the coefficient matrix for the y derivative
//================================================================================

#include "MappedGridOperators.h"
#include "xDC.h"

void 
yFDerivCoefficients(RealDistributedArray & derivative,
		     const Index & I1,
		     const Index & I2,
		     const Index & I3,
		     const Index & E,
		     const Index & C,
		     MappedGridOperators & mgop )
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  int & orderOfAccuracy    = mgop.orderOfAccuracy;
  int & width              = mgop.width;
  int & halfWidth1         = mgop.halfWidth1;
  int & halfWidth2         = mgop.halfWidth2;
  int & halfWidth3         = mgop.halfWidth3;
  int & stencilSize        = mgop.stencilSize;
  int & numberOfComponentsForCoefficients = mgop.numberOfComponentsForCoefficients;

  assert( numberOfDimensions>1 );
  
  int m1,m2,m3;
  int dum;
  Range aR0,aR1,aR2,aR3;
//  RealArray & delta = mgop.delta;

  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
      : mgop.mappedGrid.inverseCenterDerivative();

  int e0=E.getBase();
  int c0=C.getBase();

  // real time=getCPU();
  if( ! mgop.useNewOperators )
  {
    Overture::abort("error");
/* ---    
    // ***** old way *******
    if( orderOfAccuracy==2 )
    {
      // RealArray & d12 = mgop.d12;
      // RealArray & d22 = mgop.d22;
      RealArray & Dr = mgop.Dr;
      RealArray & Ds = mgop.Ds;
      RealArray & Dt = mgop.Dt;
      // RealArray & Drr= mgop.Drr;
      // RealArray & Drs= mgop.Drs;
      // RealArray & Drt= mgop.Drt;
      // RealArray & Dss= mgop.Dss;
      // RealArray & Dst= mgop.Dst;
      // RealArray & Dtt= mgop.Dtt;
      if( numberOfDimensions==2 )
      {
	ForStencil(m1,m2,m3)                                            
	{                                                               
	  UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=UY22(I1,I2,I3,c0);    
	}
      }
      else // ======= 3D ================
      {
	ForStencil(m1,m2,m3)                                            
	{                                                               
	  UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=UY23(I1,I2,I3,c0);
	}
      }
    }
    else   // ====== 4th order =======
    {
      // RealArray & d14 = mgop.d14;
      // RealArray & d24 = mgop.d24;
      RealArray & Dr4 = mgop.Dr4;
      RealArray & Ds4 = mgop.Ds4;
      RealArray & Dt4 = mgop.Dt4;
      // RealArray & Drr4= mgop.Drr4;
      // RealArray & Drs4= mgop.Drs4;
      // RealArray & Drt4= mgop.Drt4;
      // RealArray & Dss4= mgop.Dss4;
      // RealArray & Dst4= mgop.Dst4;
      // RealArray & Dtt4= mgop.Dtt4;
      if( numberOfDimensions==2 )
      {
	ForStencil(m1,m2,m3)                                            
	{                                                               
	  UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=UY42(I1,I2,I3,c0);  
	}
      }
      else  // ======= 3D ================
      {
	ForStencil(m1,m2,m3)                                            
	{                                                               
	  UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=UY43(I1,I2,I3,c0);  
	}
      }
    }
---- */
  }
  else if( mgop.isRectangular() )
  { 
    // ***** optimized NEW way *****

    if( orderOfAccuracy==2 )
    {
      real h21c[3];
      for( int axis=0; axis<3; axis++ )
        h21c[axis]= 1./(2.*mgop.dx[axis]); 
#define h21(n) h21c[n]
      if( numberOfDimensions==1 )
        derivative(MCE(0, 0, 0),I1,I2,I3)=0.;
      else
      {
	int bound= numberOfDimensions==2 ? 8 : 26;
        Range M(0,bound);
        derivative(M+CE(c0,e0),I1,I2,I3)=0.;
      }
      derivative(MCE( 0,-1, 0),I1,I2,I3)=-h21(axis2);
      derivative(MCE( 0,+1, 0),I1,I2,I3)= h21(axis2);
    }
    else   // ====== 4th order =======
    {
      //         UX42R(I1,I2,I3,KD) ( (8.*(U(I1+1,I2,I3,KD)-U(I1-1,I2,I3,KD))  \
      //                                -(U(I1+2,I2,I3,KD)-U(I1-2,I2,I3,KD)))*h41(axis1) )
      real h41c[3];
      for( int axis=0; axis<3; axis++ )
       h41c[axis]=1./(12.*mgop.dx[axis]);
#define h41(n) h41c[n]
      if( numberOfDimensions==1 )
        derivative(MCE(0, 0, 0),I1,I2,I3)=0.;
      else
      {
        int bound= numberOfDimensions==2 ? 24 : 124;
        Range M(0,bound);
        derivative(M+CE(c0,e0),I1,I2,I3)=0.;
      }
      derivative(MCE( 0,-2, 0),I1,I2,I3)=    +h41(axis2);
      derivative(MCE( 0,-1, 0),I1,I2,I3)= -8.*h41(axis2);
      derivative(MCE( 0,+1, 0),I1,I2,I3)=  8.*h41(axis2);
      derivative(MCE( 0,+2, 0),I1,I2,I3)=    -h41(axis2);
    }
  }
  else
  {
    // *** optimized curvilinear ****
    // UX23(I1,I2,I3,KD) ( RX(I1,I2,I3)*UR2(I1,I2,I3,KD)  
    //                     +SX(I1,I2,I3)*US2(I1,I2,I3,KD) 
    //                     +TX(I1,I2,I3)*UT2(I1,I2,I3,KD) )
    if( orderOfAccuracy==2 )
    {
      RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing());     
      if( numberOfDimensions==2 )
      {
        realArray rx; rx = d12(axis1)*RY(I1,I2,I3); 
        realArray sx; sx = d12(axis2)*SY(I1,I2,I3); 
        rx.reshape(1,I1,I2,I3);
        sx.reshape(1,I1,I2,I3);

        Range M(0,8);
        derivative(M+CE(c0,e0),I1,I2,I3)=0.;
        derivative(MCE( 0,-1, 0),I1,I2,I3)=-sx;
        derivative(MCE(-1, 0, 0),I1,I2,I3)=-rx;
        derivative(MCE(+1, 0, 0),I1,I2,I3)= rx;
        derivative(MCE( 0,+1, 0),I1,I2,I3)= sx;
      }
      else // ======= 3D ================
      {
        realArray rx; rx = d12(axis1)*RY(I1,I2,I3); 
        realArray sx; sx = d12(axis2)*SY(I1,I2,I3); 
        realArray tx; tx = d12(axis3)*TY(I1,I2,I3); 
        rx.reshape(1,I1,I2,I3);
        sx.reshape(1,I1,I2,I3);
        tx.reshape(1,I1,I2,I3);

        Range M(0,26);
        derivative(M+CE(c0,e0),I1,I2,I3)=0.;
        derivative(MCE( 0, 0,-1),I1,I2,I3)=-tx;
        derivative(MCE( 0,-1, 0),I1,I2,I3)=-sx;
        derivative(MCE(-1, 0, 0),I1,I2,I3)=-rx;
        derivative(MCE(+1, 0, 0),I1,I2,I3)= rx;
        derivative(MCE( 0,+1, 0),I1,I2,I3)= sx;
        derivative(MCE( 0, 0,+1),I1,I2,I3)= tx;

      }
    }
    else   // ====== 4th order =======
    {

      RealArray d14; d14 = 1./(12.*mgop.mappedGrid.gridSpacing());
      if( numberOfDimensions==2 )
      {
        realArray rx; rx = d14(axis1)*(RY(I1,I2,I3));
        realArray sx; sx = d14(axis2)*(SY(I1,I2,I3));

        rx.reshape(1,I1,I2,I3);
        sx.reshape(1,I1,I2,I3);

        Range M(0,24);
	derivative(M+CE(c0,e0),I1,I2,I3)=     0.;

        derivative(MCE( 0,-2, 0),I1,I2,I3)=     sx;
        derivative(MCE( 0,-1, 0),I1,I2,I3)= -8.*sx;

        derivative(MCE(-2, 0, 0),I1,I2,I3)=     rx;
        derivative(MCE(-1, 0, 0),I1,I2,I3)= -8.*rx;
        derivative(MCE(+1, 0, 0),I1,I2,I3)=  8.*rx;
        derivative(MCE(+2, 0, 0),I1,I2,I3)=    -rx;

        derivative(MCE( 0,+1, 0),I1,I2,I3)= +8.*sx;
        derivative(MCE( 0,+2, 0),I1,I2,I3)=    -sx;

      }
      else  // ======= 3D ================
      {
        realArray rx; rx = d14(axis1)*(RY(I1,I2,I3));
        realArray sx; sx = d14(axis2)*(SY(I1,I2,I3));
        realArray tx; tx = d14(axis3)*(TY(I1,I2,I3));

        rx.reshape(1,I1,I2,I3);
        sx.reshape(1,I1,I2,I3);
        tx.reshape(1,I1,I2,I3);

        Range M(0,124);
	derivative(M+CE(c0,e0),I1,I2,I3)=     0.;

        derivative(MCE( 0, 0,-2),I1,I2,I3)=     tx;
        derivative(MCE( 0, 0,-1),I1,I2,I3)= -8.*tx;

        derivative(MCE( 0,-2, 0),I1,I2,I3)=     sx;
        derivative(MCE( 0,-1, 0),I1,I2,I3)= -8.*sx;

        derivative(MCE(-2, 0, 0),I1,I2,I3)=     rx;
        derivative(MCE(-1, 0, 0),I1,I2,I3)= -8.*rx;
        derivative(MCE(+1, 0, 0),I1,I2,I3)=  8.*rx;
        derivative(MCE(+2, 0, 0),I1,I2,I3)=    -rx;

        derivative(MCE( 0,+1, 0),I1,I2,I3)= +8.*sx;
        derivative(MCE( 0,+2, 0),I1,I2,I3)=    -sx;

        derivative(MCE( 0, 0, 1),I1,I2,I3)=  8.*tx;
        derivative(MCE( 0, 0, 2),I1,I2,I3)=    -tx;

      }
    }
  }
  // time=getCPU()-time;
  // printf("yFDerivCoefficients: time = %e \n",time);
  
  // now make copies for other components
  Index M(0,stencilSize);
  for( int c=C.getBase(); c<=C.getBound(); c++ )                        
  for( int e=E.getBase(); e<=E.getBound(); e++ )                        
    if( c!=c0 || e!=e0 )
      derivative(M+CE(c,e),I1,I2,I3)=derivative(M+CE(c0,e0),I1,I2,I3);
}


#undef CE
