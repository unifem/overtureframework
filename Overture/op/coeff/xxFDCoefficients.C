//================================================================================
//   Define the coefficient matrix for the xx derivative
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
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  int & orderOfAccuracy    = mgop.orderOfAccuracy;
  int & width              = mgop.width;
  int & halfWidth1         = mgop.halfWidth1;
  int & halfWidth2         = mgop.halfWidth2;
  int & halfWidth3         = mgop.halfWidth3;
  int & stencilSize        = mgop.stencilSize;
  int & numberOfComponentsForCoefficients = mgop.numberOfComponentsForCoefficients;

  int m1,m2,m3;
  int dum;
  Range aR0,aR1,aR2,aR3;

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
      RealArray & d12 = mgop.d12;
      // RealArray & d22 = mgop.d22;
      RealArray & Dr = mgop.Dr;
      RealArray & Ds = mgop.Ds;
      RealArray & Dt = mgop.Dt;
      RealArray & Drr= mgop.Drr;
      // RealArray & Drs= mgop.Drs;
      // RealArray & Drt= mgop.Drt;
      RealArray & Dss= mgop.Dss;
      // RealArray & Dst= mgop.Dst;
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
      // RealArray & d24 = mgop.d24;
      RealArray & Dr4 = mgop.Dr4;
      RealArray & Ds4 = mgop.Ds4;
      RealArray & Dt4 = mgop.Dt4;
      RealArray & Drr4= mgop.Drr4;
      // RealArray & Drs4= mgop.Drs4;
      // RealArray & Drt4= mgop.Drt4;
      RealArray & Dss4= mgop.Dss4;
      // RealArray & Dst4= mgop.Dst4;
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
--- */
  }
  else if( mgop.isRectangular() )
  { 
    // ***** optimized NEW way *****

    if( orderOfAccuracy==2 )
    {
      real h22c[3];
#define h22(n) h22c[n]
      for( int axis=0; axis<3; axis++ )
	h22(axis)=1./SQR(mgop.dx[axis]);

      if( numberOfDimensions>1 )
      {
        int bound= numberOfDimensions==2 ? 8 : 26;
        Range M(0,bound);
        derivative(M+CE(c0,e0),I1,I2,I3)=0.;
      }
      derivative(MCE(-1, 0, 0),I1,I2,I3)=    h22(axis1);
      derivative(MCE( 0, 0, 0),I1,I2,I3)=-2.*h22(axis1);
      derivative(MCE(+1, 0, 0),I1,I2,I3)=    h22(axis1);
    }
    else   // ====== 4th order =======
    {
      //         UX42R(I1,I2,I3,KD) ( (8.*(U(I1+1,I2,I3,KD)-U(I1-1,I2,I3,KD))  \
      //                                -(U(I1+2,I2,I3,KD)-U(I1-2,I2,I3,KD)))*h41(axis1) )

      real h42c[3];
#define h42(n) h42c[n]
      for( int axis=0; axis<3; axis++ )
	h42(axis)=1./(12.*SQR(mgop.dx[axis]));
      if( numberOfDimensions>1 )
      {
	int bound= numberOfDimensions==2 ? 24 : 124;
	Range M(0,bound);
	derivative(M+CE(c0,e0),I1,I2,I3)=0.;
      }
      derivative(MCE(-2, 0, 0),I1,I2,I3)=    -h42(axis1);
      derivative(MCE(-1, 0, 0),I1,I2,I3)= 16.*h42(axis1);
      derivative(MCE( 0, 0, 0),I1,I2,I3)=-30.*h42(axis1);
      derivative(MCE(+1, 0, 0),I1,I2,I3)= 16.*h42(axis1);
      derivative(MCE(+2, 0, 0),I1,I2,I3)=    -h42(axis1);
    }
  }
  else
  {
    // *** optimized curvilinear ****

    // UXX23(I1,I2,I3,KD) (                                      
    //         SQR(RX(I1,I2,I3)) *URR2(I1,I2,I3,KD)              
    //        +SQR(SX(I1,I2,I3)) *USS2(I1,I2,I3,KD)              
    //        +SQR(TX(I1,I2,I3)) *UTT2(I1,I2,I3,KD)              
    //        +2.*RX(I1,I2,I3)*SX(I1,I2,I3)*URS2(I1,I2,I3,KD)    
    //        +2.*RX(I1,I2,I3)*TX(I1,I2,I3)*URT2(I1,I2,I3,KD)    
    //        +2.*SX(I1,I2,I3)*TX(I1,I2,I3)*UST2(I1,I2,I3,KD)    
    //        +RXX23(I1,I2,I3)*UR2(I1,I2,I3,KD)                  
    //        +SXX23(I1,I2,I3)*US2(I1,I2,I3,KD)                  
    //        +TXX23(I1,I2,I3)*UT2(I1,I2,I3,KD)                  

    if( orderOfAccuracy==2 )
    {
      RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
      RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;

      if( numberOfDimensions==1 )
      { // get coefficients for the first component
        realArray rxSq; rxSq = d22(axis1)*(RX(I1,I2,I3)*RX(I1,I2,I3));
	realArray rxx; rxx = d12(axis1)*RXX2(I1,I2,I3);

        rxSq.reshape(1,I1,I2,I3);
        rxx .reshape(1,I1,I2,I3);

        derivative(MCE(-1, 0, 0),I1,I2,I3)=    rxSq -rxx;
        derivative(MCE( 0, 0, 0),I1,I2,I3)=-2.*rxSq;
        derivative(MCE(+1, 0, 0),I1,I2,I3)=    rxSq +rxx;
      }
      else if( numberOfDimensions==2 )
      {
        realArray rxSq; rxSq = d22(axis1)*(RX(I1,I2,I3)*RX(I1,I2,I3));
	realArray rxx; rxx = d12(axis1)*RXX2(I1,I2,I3);

        realArray sxSq; sxSq = d22(axis2)*(SX(I1,I2,I3)*SX(I1,I2,I3));
	realArray sxx; sxx = d12(axis2)*SXX2(I1,I2,I3);
	realArray rsx; rsx = (2.*d12(axis1)*d12(axis2))*(RX(I1,I2,I3)*SX(I1,I2,I3));

        rxSq.reshape(1,I1,I2,I3);
        rxx .reshape(1,I1,I2,I3);
        sxSq.reshape(1,I1,I2,I3);
        sxx .reshape(1,I1,I2,I3);
        rsx .reshape(1,I1,I2,I3);
	
        derivative(MCE(-1,-1, 0),I1,I2,I3)=                                                     rsx;
        derivative(MCE( 0,-1, 0),I1,I2,I3)=                    sxSq -sxx;
        derivative(MCE(+1,-1, 0),I1,I2,I3)=                                                    -rsx; 
        derivative(MCE(-1, 0, 0),I1,I2,I3)=     rxSq      -rxx;
        derivative(MCE( 0, 0, 0),I1,I2,I3)=-2.*(rxSq+sxSq);
        derivative(MCE(+1, 0, 0),I1,I2,I3)=     rxSq      +rxx;
        derivative(MCE(-1,+1, 0),I1,I2,I3)=                                                    -rsx; 
        derivative(MCE( 0,+1, 0),I1,I2,I3)=                    sxSq +sxx;
        derivative(MCE(+1,+1, 0),I1,I2,I3)=                                                     rsx; 
      }
      else // ======= 3D ================
      {
        realArray rxSq; rxSq = d22(axis1)*(RX(I1,I2,I3)*RX(I1,I2,I3));
	realArray rxx; rxx = d12(axis1)*(RXX23(I1,I2,I3));

        realArray sxSq; sxSq = d22(axis2)*(SX(I1,I2,I3)*SX(I1,I2,I3));
	realArray sxx; sxx = d12(axis2)*(SXX23(I1,I2,I3));

        realArray txSq; txSq = d22(axis3)*(TX(I1,I2,I3)*TX(I1,I2,I3));
	realArray txx; txx = d12(axis3)*(TXX23(I1,I2,I3));

	realArray rsx; rsx = (2.*d12(axis1)*d12(axis2))*(RX(I1,I2,I3)*SX(I1,I2,I3));
	realArray rtx; rtx = (2.*d12(axis1)*d12(axis3))*(RX(I1,I2,I3)*TX(I1,I2,I3));
	realArray stx; stx = (2.*d12(axis2)*d12(axis3))*(SX(I1,I2,I3)*TX(I1,I2,I3));

        rxSq.reshape(1,I1,I2,I3);
        rxx .reshape(1,I1,I2,I3);
        sxSq.reshape(1,I1,I2,I3);
        sxx .reshape(1,I1,I2,I3);
        txSq.reshape(1,I1,I2,I3);
        txx .reshape(1,I1,I2,I3);

        rsx .reshape(1,I1,I2,I3);
        rtx .reshape(1,I1,I2,I3);
        stx .reshape(1,I1,I2,I3);
	
        derivative(MCE(-1,-1,-1),I1,I2,I3)= 0.;
        derivative(MCE( 0,-1,-1),I1,I2,I3)=                                   stx;
        derivative(MCE(+1,-1,-1),I1,I2,I3)= 0.;
        derivative(MCE(-1, 0,-1),I1,I2,I3)=                                   rtx;
        derivative(MCE( 0, 0,-1),I1,I2,I3)=               txSq -txx;
        derivative(MCE(+1, 0,-1),I1,I2,I3)=                                  -rtx;
        derivative(MCE(-1,+1,-1),I1,I2,I3)= 0.;
        derivative(MCE( 0,+1,-1),I1,I2,I3)=                                  -stx;
        derivative(MCE(+1,+1,-1),I1,I2,I3)= 0.;

        derivative(MCE(-1,-1, 0),I1,I2,I3)=                                   rsx;
        derivative(MCE( 0,-1, 0),I1,I2,I3)=                    sxSq -sxx;
        derivative(MCE(+1,-1, 0),I1,I2,I3)=                                  -rsx; 
        derivative(MCE(-1, 0, 0),I1,I2,I3)=     rxSq      -rxx;
        derivative(MCE( 0, 0, 0),I1,I2,I3)=-2.*(rxSq+sxSq+txSq);
        derivative(MCE(+1, 0, 0),I1,I2,I3)=     rxSq      +rxx;
        derivative(MCE(-1,+1, 0),I1,I2,I3)=                                  -rsx; 
        derivative(MCE( 0,+1, 0),I1,I2,I3)=                    sxSq +sxx;
        derivative(MCE(+1,+1, 0),I1,I2,I3)=                                   rsx; 

        derivative(MCE(-1,-1,+1),I1,I2,I3)= 0.;
        derivative(MCE( 0,-1,+1),I1,I2,I3)=                                  -stx;
        derivative(MCE(+1,-1,+1),I1,I2,I3)= 0.;
        derivative(MCE(-1, 0,+1),I1,I2,I3)=                                  -rtx;
        derivative(MCE( 0, 0,+1),I1,I2,I3)=               txSq +txx;
        derivative(MCE(+1, 0,+1),I1,I2,I3)=                                   rtx;
        derivative(MCE(-1,+1,+1),I1,I2,I3)= 0.;
        derivative(MCE( 0,+1,+1),I1,I2,I3)=                                   stx;
        derivative(MCE(+1,+1,+1),I1,I2,I3)= 0.;
      }
    }
    else   // ====== 4th order =======
    {
      RealArray d14; d14 = 1./(12.*mgop.mappedGrid.gridSpacing()); // mgop.d14;
      RealArray d24; d24 = 1./(12.*SQR(mgop.mappedGrid.gridSpacing())); // mgop.d24;

      if( numberOfDimensions==1 )
      {
        realArray rxSq; rxSq = d24(axis1)*RX(I1,I2,I3)*RX(I1,I2,I3);
	realArray rxx; rxx = d14(axis1)*RXX4(I1,I2,I3);

        rxSq.reshape(1,I1,I2,I3);
        rxx .reshape(1,I1,I2,I3);

        derivative(MCE(-2, 0, 0),I1,I2,I3)=    -rxSq    +rxx;
        derivative(MCE(-1, 0, 0),I1,I2,I3)= 16.*rxSq -8.*rxx;
	derivative(MCE( 0, 0, 0),I1,I2,I3)=-30.*rxSq;
        derivative(MCE(+1, 0, 0),I1,I2,I3)= 16.*rxSq +8.*rxx;
        derivative(MCE(+2, 0, 0),I1,I2,I3)=    -rxSq    -rxx;
      }
      else if( numberOfDimensions==2 )
      {
        // UXX42(I1,I2,I3,KD)   (   \
        //        (SQR(RX(I1,I2,I3))                )*URR4(I1,I2,I3,KD) 
        //       +2.*(RX(I1,I2,I3)*SX(I1,I2,I3)                           )  
        //                                         *URS4(I1,I2,I3,KD)   
        //     +(SQR(SX(I1,I2,I3))                )*USS4(I1,I2,I3,KD)   
        //           +(RXX4(I1,I2,I3)              )*UR4(I1,I2,I3,KD)   
        //           +(SXX4(I1,I2,I3)              )*US4(I1,I2,I3,KD)   

        realArray rxSq; rxSq = d24(axis1)*(RX(I1,I2,I3)*RX(I1,I2,I3));
	realArray rxx; rxx = d14(axis1)*(RXX4(I1,I2,I3));

        realArray sxSq; sxSq = d24(axis2)*(SX(I1,I2,I3)*SX(I1,I2,I3));
	realArray sxx; sxx = d14(axis2)*(SXX4(I1,I2,I3));
	realArray rsx; rsx = (2.*d14(axis1)*d14(axis2))*(RX(I1,I2,I3)*SX(I1,I2,I3));

        rxSq.reshape(1,I1,I2,I3);
        rxx .reshape(1,I1,I2,I3);
        sxSq.reshape(1,I1,I2,I3);
        sxx .reshape(1,I1,I2,I3);
        rsx .reshape(1,I1,I2,I3);
	
	Range M(0,24);
	derivative(M+CE(c0,e0),I1,I2,I3)=0.;

        derivative(MCE( 0,-2, 0),I1,I2,I3)=                     -sxSq    +sxx;
        derivative(MCE( 0,-1, 0),I1,I2,I3)=                  16.*sxSq -8.*sxx;

        derivative(MCE(-2, 0, 0),I1,I2,I3)=    -rxSq    +rxx;
        derivative(MCE(-1, 0, 0),I1,I2,I3)= 16.*rxSq -8.*rxx;
	derivative(MCE( 0, 0, 0),I1,I2,I3)=-30.*(rxSq           +sxSq);
        derivative(MCE(+1, 0, 0),I1,I2,I3)= 16.*rxSq +8.*rxx;
        derivative(MCE(+2, 0, 0),I1,I2,I3)=    -rxSq    -rxx;

        derivative(MCE( 0,+1, 0),I1,I2,I3)=                  16.*sxSq +8.*sxx;
        derivative(MCE( 0,+2, 0),I1,I2,I3)=                     -sxSq    -sxx;

        derivative(MCE(-2,-2, 0),I1,I2,I3)=     rsx;
        derivative(MCE(-1,-2, 0),I1,I2,I3)= -8.*rsx; 
        derivative(MCE(+1,-2, 0),I1,I2,I3)=  8.*rsx; 
        derivative(MCE(+2,-2, 0),I1,I2,I3)=    -rsx;
        derivative(MCE(-2,-1, 0),I1,I2,I3)= -8.*rsx;
        derivative(MCE(-1,-1, 0),I1,I2,I3)= 64.*rsx; 
        derivative(MCE(+1,-1, 0),I1,I2,I3)=-64.*rsx; 
        derivative(MCE(+2,-1, 0),I1,I2,I3)=  8.*rsx;
        derivative(MCE(-2, 1, 0),I1,I2,I3)=  8.*rsx;
        derivative(MCE(-1, 1, 0),I1,I2,I3)=-64.*rsx; 
        derivative(MCE(+1, 1, 0),I1,I2,I3)= 64.*rsx; 
        derivative(MCE(+2, 1, 0),I1,I2,I3)= -8.*rsx;
        derivative(MCE(-2, 2, 0),I1,I2,I3)=    -rsx;
        derivative(MCE(-1, 2, 0),I1,I2,I3)=  8.*rsx; 
        derivative(MCE(+1, 2, 0),I1,I2,I3)= -8.*rsx; 
        derivative(MCE(+2, 2, 0),I1,I2,I3)=     rsx;

      }
      else  // ======= 3D ================
      {
        // printf("xxCoeff: 3D 4th order \n");

        realArray rxSq; rxSq = d24(axis1)*(RX(I1,I2,I3)*RX(I1,I2,I3));
	realArray rxx; rxx = d14(axis1)*(RXX43(I1,I2,I3));

        realArray sxSq; sxSq = d24(axis2)*(SX(I1,I2,I3)*SX(I1,I2,I3));
	realArray sxx; sxx = d14(axis2)*(SXX43(I1,I2,I3));

        realArray txSq; txSq = d24(axis3)*(TX(I1,I2,I3)*TX(I1,I2,I3));
	realArray txx; txx = d14(axis3)*(TXX43(I1,I2,I3));

	realArray rsx; rsx = (2.*d14(axis1)*d14(axis2))*(RX(I1,I2,I3)*SX(I1,I2,I3));
	realArray rtx; rtx = (2.*d14(axis1)*d14(axis3))*(RX(I1,I2,I3)*TX(I1,I2,I3));
	realArray stx; stx = (2.*d14(axis2)*d14(axis3))*(SX(I1,I2,I3)*TX(I1,I2,I3));

        rxSq.reshape(1,I1,I2,I3);
        rxx .reshape(1,I1,I2,I3);
        sxSq.reshape(1,I1,I2,I3);
        sxx .reshape(1,I1,I2,I3);
        txSq.reshape(1,I1,I2,I3);
        txx .reshape(1,I1,I2,I3);

        rsx .reshape(1,I1,I2,I3);
        rtx .reshape(1,I1,I2,I3);
        stx .reshape(1,I1,I2,I3);
	
/* ----
        printf(" max(abs(rxSq)) = %e \n",max(abs(rxSq)));
        printf(" max(abs(sxSq)) = %e \n",max(abs(sxSq)));
        printf(" max(abs(txSq)) = %e \n",max(abs(txSq)));

        printf(" max(abs(rxx )) = %e \n",max(abs(rxx )));
        printf(" max(abs(sxx )) = %e \n",max(abs(sxx )));
        printf(" max(abs(txx )) = %e \n",max(abs(txx )));
	
        printf(" max(abs(rsx )) = %e \n",max(abs(rsx )));
        printf(" max(abs(rtx )) = %e \n",max(abs(rtx )));
        printf(" max(abs(stx )) = %e \n",max(abs(stx )));
---- */	

        Range M(0,124);
	derivative(M+CE(c0,e0),I1,I2,I3)=0.;
	
        derivative(MCE( 0, 0,-2),I1,I2,I3)=                                   -txSq    +txx;
        derivative(MCE( 0, 0,-1),I1,I2,I3)=                                16.*txSq -8.*txx;

        derivative(MCE( 0,-2, 0),I1,I2,I3)=                     -sxSq    +sxx;
        derivative(MCE( 0,-1, 0),I1,I2,I3)=                  16.*sxSq -8.*sxx;

        derivative(MCE(-2, 0, 0),I1,I2,I3)=     -rxSq    +rxx;
        derivative(MCE(-1, 0, 0),I1,I2,I3)=  16.*rxSq -8.*rxx;
	derivative(MCE( 0, 0, 0),I1,I2,I3)=-30.*(rxSq           +sxSq      +   txSq );
        derivative(MCE(+1, 0, 0),I1,I2,I3)=  16.*rxSq +8.*rxx;
        derivative(MCE(+2, 0, 0),I1,I2,I3)=     -rxSq    -rxx;

        derivative(MCE( 0,+1, 0),I1,I2,I3)=                  16.*sxSq +8.*sxx;
        derivative(MCE( 0,+2, 0),I1,I2,I3)=                     -sxSq    -sxx;

        derivative(MCE( 0, 0, 1),I1,I2,I3)=                                16.*txSq +8.*txx;
        derivative(MCE( 0, 0, 2),I1,I2,I3)=                                   -txSq    -txx;

        derivative(MCE(-2,-2, 0),I1,I2,I3)=     rsx;
        derivative(MCE(-1,-2, 0),I1,I2,I3)= -8.*rsx; 
        derivative(MCE(+1,-2, 0),I1,I2,I3)=  8.*rsx; 
        derivative(MCE(+2,-2, 0),I1,I2,I3)=    -rsx;
        derivative(MCE(-2,-1, 0),I1,I2,I3)= -8.*rsx;
        derivative(MCE(-1,-1, 0),I1,I2,I3)= 64.*rsx; 
        derivative(MCE(+1,-1, 0),I1,I2,I3)=-64.*rsx; 
        derivative(MCE(+2,-1, 0),I1,I2,I3)=  8.*rsx;
        derivative(MCE(-2, 1, 0),I1,I2,I3)=  8.*rsx;
        derivative(MCE(-1, 1, 0),I1,I2,I3)=-64.*rsx; 
        derivative(MCE(+1, 1, 0),I1,I2,I3)= 64.*rsx; 
        derivative(MCE(+2, 1, 0),I1,I2,I3)= -8.*rsx;
        derivative(MCE(-2, 2, 0),I1,I2,I3)=    -rsx;
        derivative(MCE(-1, 2, 0),I1,I2,I3)=  8.*rsx; 
        derivative(MCE(+1, 2, 0),I1,I2,I3)= -8.*rsx; 
        derivative(MCE(+2, 2, 0),I1,I2,I3)=     rsx;

        derivative(MCE(-2, 0,-2),I1,I2,I3)=     rtx;
        derivative(MCE(-1, 0,-2),I1,I2,I3)= -8.*rtx; 
        derivative(MCE(+1, 0,-2),I1,I2,I3)=  8.*rtx; 
        derivative(MCE(+2, 0,-2),I1,I2,I3)=    -rtx;
        derivative(MCE(-2, 0,-1),I1,I2,I3)= -8.*rtx;
        derivative(MCE(-1, 0,-1),I1,I2,I3)= 64.*rtx; 
        derivative(MCE(+1, 0,-1),I1,I2,I3)=-64.*rtx; 
        derivative(MCE(+2, 0,-1),I1,I2,I3)=  8.*rtx;
        derivative(MCE(-2, 0, 1),I1,I2,I3)=  8.*rtx;
        derivative(MCE(-1, 0, 1),I1,I2,I3)=-64.*rtx; 
        derivative(MCE(+1, 0, 1),I1,I2,I3)= 64.*rtx; 
        derivative(MCE(+2, 0, 1),I1,I2,I3)= -8.*rtx;
        derivative(MCE(-2, 0, 2),I1,I2,I3)=    -rtx;
        derivative(MCE(-1, 0, 2),I1,I2,I3)=  8.*rtx; 
        derivative(MCE(+1, 0, 2),I1,I2,I3)= -8.*rtx; 
        derivative(MCE(+2, 0, 2),I1,I2,I3)=     rtx;

        derivative(MCE( 0,-2,-2),I1,I2,I3)=     stx;
        derivative(MCE( 0,-1,-2),I1,I2,I3)= -8.*stx; 
        derivative(MCE( 0,+1,-2),I1,I2,I3)=  8.*stx; 
        derivative(MCE( 0,+2,-2),I1,I2,I3)=    -stx;
        derivative(MCE( 0,-2,-1),I1,I2,I3)= -8.*stx;
        derivative(MCE( 0,-1,-1),I1,I2,I3)= 64.*stx; 
        derivative(MCE( 0,+1,-1),I1,I2,I3)=-64.*stx; 
        derivative(MCE( 0,+2,-1),I1,I2,I3)=  8.*stx;
        derivative(MCE( 0,-2, 1),I1,I2,I3)=  8.*stx;
        derivative(MCE( 0,-1, 1),I1,I2,I3)=-64.*stx; 
        derivative(MCE( 0,+1, 1),I1,I2,I3)= 64.*stx; 
        derivative(MCE( 0,+2, 1),I1,I2,I3)= -8.*stx;
        derivative(MCE( 0,-2, 2),I1,I2,I3)=    -stx;
        derivative(MCE( 0,-1, 2),I1,I2,I3)=  8.*stx; 
        derivative(MCE( 0,+1, 2),I1,I2,I3)= -8.*stx; 
        derivative(MCE( 0,+2, 2),I1,I2,I3)=     stx;

      }
    }
  }
  // time=getCPU()-time;
  // printf("xxFDerivCoefficients: time = %e \n",time);
  
  // now make copies for other components
  Index M(0,stencilSize);
  for( int c=C.getBase(); c<=C.getBound(); c++ )                        
  for( int e=E.getBase(); e<=E.getBound(); e++ )                        
    if( c!=c0 || e!=e0 )
      derivative(M+CE(c,e),I1,I2,I3)=derivative(M+CE(c0,e0),I1,I2,I3);
}


#undef CE
