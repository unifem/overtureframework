//================================================================================
//   Define the coefficient matrix for divScalarGrad
//================================================================================

#include "MappedGridOperators.h"
#include "xDC.h"

void 
divScalarGradFDerivCoefficients(RealDistributedArray & derivative,
				const realMappedGridFunction & s,
				const Index & I1,
				const Index & I2,
				const Index & I3,
				const Index & E,
				const Index & C,
				MappedGridOperators & mgop );

void 
laplaceFDerivCoefficients42(RealDistributedArray & derivative,
		     const Index & I1,
		     const Index & I2,
		     const Index & I3,
		     const Index & E,
		     const Index & C,
		     MappedGridOperators & mgop )
// 4th order, 2d
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  // int & orderOfAccuracy    = mgop.orderOfAccuracy;
  int & width              = mgop.width;
  int & halfWidth1         = mgop.halfWidth1;
  int & halfWidth2         = mgop.halfWidth2;
  int & halfWidth3         = mgop.halfWidth3;
  int & stencilSize        = mgop.stencilSize;
  int & numberOfComponentsForCoefficients = mgop.numberOfComponentsForCoefficients;

  Range aR0,aR1,aR2,aR3;

  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
      : mgop.mappedGrid.inverseCenterDerivative();

  int e0=E.getBase();
  int c0=C.getBase();


  RealArray d14; d14 = 1./(12.*mgop.mappedGrid.gridSpacing()); // mgop.d14;
  RealArray d24; d24 = 1./(12.*SQR(mgop.mappedGrid.gridSpacing())); // mgop.d24;

  //      LAPLACIAN22(I1,I2,I3,KD)  (                             
  //    (SQR(RX(I1,I2,I3))+SQR(RY(I1,I2,I3)))*URR2(I1,I2,I3,KD)   
  //   +2.*(RX(I1,I2,I3)*SX(I1,I2,I3)+ RY(I1,I2,I3)*SY(I1,I2,I3)) 
  //                                     *URS2(I1,I2,I3,KD)       
  //   +(SQR(SX(I1,I2,I3))+SQR(SY(I1,I2,I3)))*USS2(I1,I2,I3,KD)   
  //        +(RXX2(I1,I2,I3)+RYY2(I1,I2,I3))*UR2(I1,I2,I3,KD)     
  //        +(SXX2(I1,I2,I3)+SYY2(I1,I2,I3))*US2(I1,I2,I3,KD)     
  //	)
  const realArray & rx = RX(I1,I2,I3);
  const realArray & sx = SX(I1,I2,I3);

  const realArray & ry = RY(I1,I2,I3);
  const realArray & sy = SY(I1,I2,I3);

  const realArray & rxx4 = RXX4(I1,I2,I3);
  const realArray & ryy4 = RYY4(I1,I2,I3);
	
  const realArray & sxx4 = SXX4(I1,I2,I3);
  const realArray & syy4 = SYY4(I1,I2,I3);
	
    
  realArray rxSq =evaluate(d24(axis1)*(rx*rx+ry*ry));
  realArray rxx  =evaluate( d14(axis1)*(rxx4+ryy4) );

  realArray sxSq = evaluate( d24(axis2)*(sx*sx+sy*sy) );
  realArray sxx =  evaluate( d14(axis2)*(sxx4+syy4) );
  realArray rsx  = evaluate( (2.*d14(axis1)*d14(axis2))*(rx*sx+ ry*sy) );

  rxSq.reshape(1,I1,I2,I3);
  rxx .reshape(1,I1,I2,I3);
  sxSq.reshape(1,I1,I2,I3);
  sxx .reshape(1,I1,I2,I3);
  rsx .reshape(1,I1,I2,I3);
	

  derivative(MCE( 0,-2, 0),I1,I2,I3)=                     -sxSq    +sxx;
  derivative(MCE( 0,-1, 0),I1,I2,I3)=                  16.*sxSq -8.*sxx;

  derivative(MCE(-2, 0, 0),I1,I2,I3)=    -rxSq    +rxx;
  derivative(MCE(-1, 0, 0),I1,I2,I3)= 16.*rxSq -8.*rxx;
  derivative(MCE( 0, 0, 0),I1,I2,I3)=-30.*(rxSq           +sxSq);
  derivative(MCE(+1, 0, 0),I1,I2,I3)= 16.*rxSq +8.*rxx;
  derivative(MCE(+2, 0, 0),I1,I2,I3)=    -rxSq    -rxx;

  derivative(MCE( 0,+1, 0),I1,I2,I3)=               16.*sxSq +8.*sxx;
  derivative(MCE( 0,+2, 0),I1,I2,I3)=                  -sxSq    -sxx;

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

void 
laplaceFDerivCoefficients43(RealDistributedArray & derivative,
		     const Index & I1,
		     const Index & I2,
		     const Index & I3,
		     const Index & E,
		     const Index & C,
		     MappedGridOperators & mgop )
// 4th order 3d
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  // int & orderOfAccuracy    = mgop.orderOfAccuracy;
  int & width              = mgop.width;
  int & halfWidth1         = mgop.halfWidth1;
  int & halfWidth2         = mgop.halfWidth2;
  int & halfWidth3         = mgop.halfWidth3;
  int & stencilSize        = mgop.stencilSize;
  int & numberOfComponentsForCoefficients = mgop.numberOfComponentsForCoefficients;

  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
      : mgop.mappedGrid.inverseCenterDerivative();

  int e0=E.getBase();
  int c0=C.getBase();


  RealArray d14; d14 = 1./(12.*mgop.mappedGrid.gridSpacing()); // mgop.d14;
  RealArray d24; d24 = 1./(12.*SQR(mgop.mappedGrid.gridSpacing())); // mgop.d24;

    // printf("lapCoeff: 3D 4th order \n");
  const realArray & rx = RX(I1,I2,I3);
  const realArray & sx = SX(I1,I2,I3);
  const realArray & tx = TX(I1,I2,I3);

  const realArray & ry = RY(I1,I2,I3);
  const realArray & sy = SY(I1,I2,I3);
  const realArray & ty = TY(I1,I2,I3);

  const realArray & rz = RZ(I1,I2,I3);
  const realArray & sz = SZ(I1,I2,I3);
  const realArray & tz = TZ(I1,I2,I3);

  const realArray & rxx4 = RXX43(I1,I2,I3);
  const realArray & ryy4 = RYY43(I1,I2,I3);
  const realArray & rzz4 = RZZ43(I1,I2,I3);
	
  const realArray & sxx4 = SXX43(I1,I2,I3);
  const realArray & syy4 = SYY43(I1,I2,I3);
  const realArray & szz4 = SZZ43(I1,I2,I3);
	
  const realArray & txx4 = TXX43(I1,I2,I3);
  const realArray & tyy4 = TYY43(I1,I2,I3);
  const realArray & tzz4 = TZZ43(I1,I2,I3);
	
  realArray rxSq = evaluate( d24(axis1)*(rx*rx+ry*ry+rz*rz) );
  realArray rxx  = evaluate( d14(axis1)*(rxx4+ryy4+rzz4) );

  realArray sxSq = evaluate( d24(axis2)*(sx*sx+sy*sy+sz*sz ) );
  realArray sxx  = evaluate( d14(axis2)*(sxx4+syy4+szz4) );

  realArray txSq = evaluate( d24(axis3)*(tx*tx+ty*ty+tz*tz) );
  realArray txx  = evaluate( d14(axis3)*(txx4+tyy4+tzz4) );

  realArray rsx  = evaluate( (2.*d14(axis1)*d14(axis2))*(rx*sx+ry*sy+rz*sz) );
  realArray rtx  = evaluate( (2.*d14(axis1)*d14(axis3))*(rx*tx+ry*ty+rz*tz) );
  realArray stx  = evaluate( (2.*d14(axis2)*d14(axis3))*(sx*tx+sy*ty+sz*tz) );

  rxSq.reshape(1,I1,I2,I3);
  rxx .reshape(1,I1,I2,I3);
  sxSq.reshape(1,I1,I2,I3);
  sxx .reshape(1,I1,I2,I3);
  txSq.reshape(1,I1,I2,I3);
  txx .reshape(1,I1,I2,I3);

  rsx .reshape(1,I1,I2,I3);
  rtx .reshape(1,I1,I2,I3);
  stx .reshape(1,I1,I2,I3);
	
  Range M(0,124);
  derivative(M+CE(c0,e0),I1,I2,I3)=0.;

  derivative(MCE( 0, 0,-2),I1,I2,I3)=                                    -txSq    +txx;
  derivative(MCE( 0, 0,-1),I1,I2,I3)=                                 16.*txSq -8.*txx;

  derivative(MCE( 0,-2, 0),I1,I2,I3)=                     -sxSq    +sxx;
  derivative(MCE( 0,-1, 0),I1,I2,I3)=                  16.*sxSq -8.*sxx;

  derivative(MCE(-2, 0, 0),I1,I2,I3)=    -rxSq    +rxx;
  derivative(MCE(-1, 0, 0),I1,I2,I3)= 16.*rxSq -8.*rxx;
  derivative(MCE( 0, 0, 0),I1,I2,I3)=-30.*(rxSq           +sxSq  +        txSq );
  derivative(MCE(+1, 0, 0),I1,I2,I3)= 16.*rxSq +8.*rxx;
  derivative(MCE(+2, 0, 0),I1,I2,I3)=    -rxSq    -rxx;

  derivative(MCE( 0,+1, 0),I1,I2,I3)=                  16.*sxSq +8.*sxx;
  derivative(MCE( 0,+2, 0),I1,I2,I3)=                     -sxSq    -sxx;

  derivative(MCE( 0, 0, 1),I1,I2,I3)=                                 16.*txSq +8.*txx;
  derivative(MCE( 0, 0, 2),I1,I2,I3)=                                    -txSq    -txx;

  const realArray & rsx8  = evaluate( rsx*8. );
  const realArray & rsx64 = evaluate( rsx*64. );

  const realArray & rtx8  = evaluate( rtx*8. );
  const realArray & rtx64 = evaluate( rtx*64. );

  const realArray & stx8  = evaluate( stx*8. );
  const realArray & stx64 = evaluate( stx*64. );

  derivative(MCE(-2,-2, 0),I1,I2,I3)=     rsx;
  derivative(MCE(-1,-2, 0),I1,I2,I3)= -  rsx8; 
  derivative(MCE(+1,-2, 0),I1,I2,I3)=    rsx8; 
  derivative(MCE(+2,-2, 0),I1,I2,I3)=    -rsx;
  derivative(MCE(-2,-1, 0),I1,I2,I3)= -  rsx8;
  derivative(MCE(-1,-1, 0),I1,I2,I3)=   rsx64; 
  derivative(MCE(+1,-1, 0),I1,I2,I3)=-  rsx64; 
  derivative(MCE(+2,-1, 0),I1,I2,I3)=    rsx8;
  derivative(MCE(-2, 1, 0),I1,I2,I3)=    rsx8;
  derivative(MCE(-1, 1, 0),I1,I2,I3)=-  rsx64; 
  derivative(MCE(+1, 1, 0),I1,I2,I3)=   rsx64; 
  derivative(MCE(+2, 1, 0),I1,I2,I3)= -  rsx8;
  derivative(MCE(-2, 2, 0),I1,I2,I3)=    -rsx;
  derivative(MCE(-1, 2, 0),I1,I2,I3)=    rsx8; 
  derivative(MCE(+1, 2, 0),I1,I2,I3)= -  rsx8; 
  derivative(MCE(+2, 2, 0),I1,I2,I3)=     rsx;

  derivative(MCE(-2, 0,-2),I1,I2,I3)=     rtx;
  derivative(MCE(-1, 0,-2),I1,I2,I3)= -  rtx8; 
  derivative(MCE(+1, 0,-2),I1,I2,I3)=    rtx8; 
  derivative(MCE(+2, 0,-2),I1,I2,I3)=    -rtx;
  derivative(MCE(-2, 0,-1),I1,I2,I3)= -  rtx8;
  derivative(MCE(-1, 0,-1),I1,I2,I3)=   rtx64; 
  derivative(MCE(+1, 0,-1),I1,I2,I3)=-  rtx64; 
  derivative(MCE(+2, 0,-1),I1,I2,I3)=    rtx8;
  derivative(MCE(-2, 0, 1),I1,I2,I3)=    rtx8;
  derivative(MCE(-1, 0, 1),I1,I2,I3)=-  rtx64; 
  derivative(MCE(+1, 0, 1),I1,I2,I3)=   rtx64; 
  derivative(MCE(+2, 0, 1),I1,I2,I3)= -  rtx8;
  derivative(MCE(-2, 0, 2),I1,I2,I3)=    -rtx;
  derivative(MCE(-1, 0, 2),I1,I2,I3)=    rtx8; 
  derivative(MCE(+1, 0, 2),I1,I2,I3)= -  rtx8; 
  derivative(MCE(+2, 0, 2),I1,I2,I3)=     rtx;

  derivative(MCE( 0,-2,-2),I1,I2,I3)=     stx;
  derivative(MCE( 0,-1,-2),I1,I2,I3)= -  stx8; 
  derivative(MCE( 0,+1,-2),I1,I2,I3)=    stx8; 
  derivative(MCE( 0,+2,-2),I1,I2,I3)=    -stx;
  derivative(MCE( 0,-2,-1),I1,I2,I3)= -  stx8;
  derivative(MCE( 0,-1,-1),I1,I2,I3)=   stx64; 
  derivative(MCE( 0,+1,-1),I1,I2,I3)=-  stx64; 
  derivative(MCE( 0,+2,-1),I1,I2,I3)=    stx8;
  derivative(MCE( 0,-2, 1),I1,I2,I3)=    stx8;
  derivative(MCE( 0,-1, 1),I1,I2,I3)=-  stx64; 
  derivative(MCE( 0,+1, 1),I1,I2,I3)=   stx64; 
  derivative(MCE( 0,+2, 1),I1,I2,I3)= -  stx8;
  derivative(MCE( 0,-2, 2),I1,I2,I3)=    -stx;
  derivative(MCE( 0,-1, 2),I1,I2,I3)=    stx8; 
  derivative(MCE( 0,+1, 2),I1,I2,I3)= -  stx8; 
  derivative(MCE( 0,+2, 2),I1,I2,I3)=     stx;
}


void 
laplaceFDerivCoefficients2(RealDistributedArray & derivative,
		     const Index & I1,
		     const Index & I2,
		     const Index & I3,
		     const Index & E,
		     const Index & C,
		     MappedGridOperators & mgop )
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  // int & orderOfAccuracy    = mgop.orderOfAccuracy;
  int & width              = mgop.width;
  int & halfWidth1         = mgop.halfWidth1;
  int & halfWidth2         = mgop.halfWidth2;
  int & halfWidth3         = mgop.halfWidth3;
  int & stencilSize        = mgop.stencilSize;
  int & numberOfComponentsForCoefficients = mgop.numberOfComponentsForCoefficients;

//  Range aR0,aR1,aR2,aR3;

  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
      : mgop.mappedGrid.inverseCenterDerivative();

  int e0=E.getBase();
  int c0=C.getBase();

  RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
  RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;
  if( numberOfDimensions==1 )
  { // get coefficients for the first component
    realArray rxSq; rxSq = d22(axis1)*RX(I1,I2,I3)*RX(I1,I2,I3);
    realArray rxx; rxx = d12(axis1)*RXX2(I1,I2,I3);

    rxSq.reshape(1,I1,I2,I3);
    rxx .reshape(1,I1,I2,I3);

    derivative(MCE(-1, 0, 0),I1,I2,I3)=    rxSq -rxx;
    derivative(MCE( 0, 0, 0),I1,I2,I3)=-2.*rxSq;
    derivative(MCE(+1, 0, 0),I1,I2,I3)=    rxSq +rxx;
  }
  else if( numberOfDimensions==2 )
  {
    //      LAPLACIAN22(I1,I2,I3,KD)  (                             
    //    (SQR(RX(I1,I2,I3))+SQR(RY(I1,I2,I3)))*URR2(I1,I2,I3,KD)   
    //   +2.*(RX(I1,I2,I3)*SX(I1,I2,I3)+ RY(I1,I2,I3)*SY(I1,I2,I3)) 
    //                                     *URS2(I1,I2,I3,KD)       
    //   +(SQR(SX(I1,I2,I3))+SQR(SY(I1,I2,I3)))*USS2(I1,I2,I3,KD)   
    //        +(RXX2(I1,I2,I3)+RYY2(I1,I2,I3))*UR2(I1,I2,I3,KD)     
    //        +(SXX2(I1,I2,I3)+SYY2(I1,I2,I3))*US2(I1,I2,I3,KD)     
    //	)

    const realArray & rx = RX(I1,I2,I3);
    const realArray & sx = SX(I1,I2,I3);

    const realArray & ry = RY(I1,I2,I3);
    const realArray & sy = SY(I1,I2,I3);

    realArray rxSq = evaluate( d22(axis1)*(rx*rx+ry*ry) );
    realArray rxx  = evaluate( d12(axis1)*(RXX2(I1,I2,I3)+RYY2(I1,I2,I3)) );

    realArray sxSq = evaluate( d22(axis2)*(sx*sx+sy*sy) );
    realArray sxx  = evaluate( d12(axis2)*(SXX2(I1,I2,I3)+SYY2(I1,I2,I3)) );
    realArray rsx  = evaluate( (2.*d12(axis1)*d12(axis2))*(rx*sx+ry*sy) );

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
    // define LAPLACIAN23(I1,I2,I3,KD)    (                             
    //       (SQR(RX(I1,I2,I3))+SQR(RY(I1,I2,I3))+SQR(RZ(I1,I2,I3)))    
    //                                        *URR2(I1,I2,I3,KD)        
    //      +(SQR(SX(I1,I2,I3))+SQR(SY(I1,I2,I3))+SQR(SZ(I1,I2,I3)))    
    //                                        *USS2(I1,I2,I3,KD)        
    //      +(SQR(TX(I1,I2,I3))+SQR(+TY(I1,I2,I3))+SQR(TZ(I1,I2,I3)))   
    //                                        *UTT2(I1,I2,I3,KD)        
    //      +2.*(RX(I1,I2,I3)*SX(I1,I2,I3)+ RY(I1,I2,I3)*SY(I1,I2,I3)   
    //          +RZ(I1,I2,I3)*SZ(I1,I2,I3))   *URS2(I1,I2,I3,KD)        
    //      +2.*(RX(I1,I2,I3)*TX(I1,I2,I3)+ RY(I1,I2,I3)*TY(I1,I2,I3)   
    //          +RZ(I1,I2,I3)*TZ(I1,I2,I3))   *URT2(I1,I2,I3,KD)        
    //      +2.*(SX(I1,I2,I3)*TX(I1,I2,I3)+ SY(I1,I2,I3)*TY(I1,I2,I3)   
    //          +SZ(I1,I2,I3)*TZ(I1,I2,I3))   *UST2(I1,I2,I3,KD)        
    //           +(RXX23(I1,I2,I3)+RYY23(I1,I2,I3)+RZZ23(I1,I2,I3))     
    //                                        *UR2(I1,I2,I3,KD)         
    //           +(SXX23(I1,I2,I3)+SYY23(I1,I2,I3)+SZZ23(I1,I2,I3))     
    //                                        *US2(I1,I2,I3,KD)         
    //           +(TXX23(I1,I2,I3)+TYY23(I1,I2,I3)+TZZ23(I1,I2,I3))     
    //                                        *UT2(I1,I2,I3,KD)         
    //                                   )                            

    const realArray & rx = RX(I1,I2,I3);
    const realArray & sx = SX(I1,I2,I3);
    const realArray & tx = TX(I1,I2,I3);

    const realArray & ry = RY(I1,I2,I3);
    const realArray & sy = SY(I1,I2,I3);
    const realArray & ty = TY(I1,I2,I3);

    const realArray & rz = RZ(I1,I2,I3);
    const realArray & sz = SZ(I1,I2,I3);
    const realArray & tz = TZ(I1,I2,I3);

    realArray rxSq = evaluate( d22(axis1)*(rx*rx+ry*ry+rz*rz) );
    realArray rxx  = evaluate( d12(axis1)*(RXX23(I1,I2,I3)+RYY23(I1,I2,I3)+RZZ23(I1,I2,I3)) );

    realArray sxSq = evaluate( d22(axis2)*(sx*sx+sy*sy+sz*sz) );
    realArray sxx  = evaluate( d12(axis2)*(SXX23(I1,I2,I3)+SYY23(I1,I2,I3)+SZZ23(I1,I2,I3)) );

    realArray txSq = evaluate( d22(axis3)*(tx*tx+ty*ty+tz*tz) );
    realArray txx  = evaluate( d12(axis3)*(TXX23(I1,I2,I3)+TYY23(I1,I2,I3)+TZZ23(I1,I2,I3)) );

    realArray rsx  = evaluate( (2.*d12(axis1)*d12(axis2))*(rx*sx+ry*sy+rz*sz) );
    realArray rtx  = evaluate( (2.*d12(axis1)*d12(axis3))*(rx*tx+ry*ty+rz*tz) );
    realArray stx  = evaluate( (2.*d12(axis2)*d12(axis3))*(sx*tx+sy*ty+sz*tz) );

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


void 
laplaceFDerivCoefficients(RealDistributedArray & derivative,
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

//   int m1,m2,m3;
//   int dum;
//   Range aR0,aR1,aR2,aR3;
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
/* ----    
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
	  UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=LAPLACIAN21(I1,I2,I3,c0); 
	}
      }
      else if( numberOfDimensions==2 )
      {
	ForStencil(m1,m2,m3)                                            
	{                                                               
	  UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=LAPLACIAN22(I1,I2,I3,c0);    
	}
      }
      else // ======= 3D ================
      {
	ForStencil(m1,m2,m3)                                            
	{                                                               
	  UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=LAPLACIAN23(I1,I2,I3,c0);
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
	  UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=LAPLACIAN41(I1,I2,I3,c0);  
	}
      }
      else if( numberOfDimensions==2 )
      {
	ForStencil(m1,m2,m3)                                            
	{                                                               
	  UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=LAPLACIAN42(I1,I2,I3,c0);  
	}
      }
      else  // ======= 3D ================
      {
	ForStencil(m1,m2,m3)                                            
	{                                                               
	  UX_MERGED(m1,m2,m3,c0,e0,I1,I2,I3)=LAPLACIAN43(I1,I2,I3,c0);  
	}
      }
    }
--- */
  }
  else if( mgop.isRectangular() )
  { // ***** optimized NEW way *****
    if( orderOfAccuracy==2 )
    {
      real h22c[3];
#define h22(n) h22c[n]
      for( int axis=0; axis<3; axis++ )
	h22(axis)=1./SQR(mgop.dx[axis]);
      if( numberOfDimensions==1 )
      { // get coefficients for the first component
        derivative(MCE(-1, 0, 0),I1,I2,I3)=    h22(axis1);
        derivative(MCE( 0, 0, 0),I1,I2,I3)=-2.*h22(axis1);
        derivative(MCE(+1, 0, 0),I1,I2,I3)=    h22(axis1);
      }
      else if( numberOfDimensions==2 )
      {
        derivative(MCE(-1,-1, 0),I1,I2,I3)=0.;
        derivative(MCE( 0,-1, 0),I1,I2,I3)=                h22(axis2);
        derivative(MCE(+1,-1, 0),I1,I2,I3)=0.;
        derivative(MCE(-1, 0, 0),I1,I2,I3)=     h22(axis1);
        derivative(MCE( 0, 0, 0),I1,I2,I3)=-2.*(h22(axis1)+h22(axis2));
        derivative(MCE(+1, 0, 0),I1,I2,I3)=     h22(axis1);
        derivative(MCE(-1,+1, 0),I1,I2,I3)=0.;
        derivative(MCE( 0,+1, 0),I1,I2,I3)=                h22(axis2);
        derivative(MCE(+1,+1, 0),I1,I2,I3)=0.;
      }
      else // ======= 3D ================
      {
        derivative(MCE(-1,-1,-1),I1,I2,I3)=0.;
        derivative(MCE( 0,-1,-1),I1,I2,I3)=0.;
        derivative(MCE(+1,-1,-1),I1,I2,I3)=0.;
        derivative(MCE(-1, 0,-1),I1,I2,I3)=0.;
        derivative(MCE( 0, 0,-1),I1,I2,I3)=                           h22(axis3);
        derivative(MCE(+1, 0,-1),I1,I2,I3)=0.;
        derivative(MCE(-1,+1,-1),I1,I2,I3)=0.;
        derivative(MCE( 0,+1,-1),I1,I2,I3)=0.;
        derivative(MCE(+1,+1,-1),I1,I2,I3)=0.;

        derivative(MCE(-1,-1, 0),I1,I2,I3)=0.;
        derivative(MCE( 0,-1, 0),I1,I2,I3)=                h22(axis2);
        derivative(MCE(+1,-1, 0),I1,I2,I3)=0.;
        derivative(MCE(-1, 0, 0),I1,I2,I3)=     h22(axis1);
        derivative(MCE( 0, 0, 0),I1,I2,I3)=-2.*(h22(axis1)+h22(axis2)+h22(axis3));
        derivative(MCE(+1, 0, 0),I1,I2,I3)=     h22(axis1);
        derivative(MCE(-1,+1, 0),I1,I2,I3)=0.;
        derivative(MCE( 0,+1, 0),I1,I2,I3)=                h22(axis2);
        derivative(MCE(+1,+1, 0),I1,I2,I3)=0.;

        derivative(MCE(-1,-1, 1),I1,I2,I3)=0.;
        derivative(MCE( 0,-1, 1),I1,I2,I3)=0.;
        derivative(MCE(+1,-1, 1),I1,I2,I3)=0.;
        derivative(MCE(-1, 0, 1),I1,I2,I3)=0.;
        derivative(MCE( 0, 0, 1),I1,I2,I3)=                           h22(axis3);
        derivative(MCE(+1, 0, 1),I1,I2,I3)=0.;
        derivative(MCE(-1,+1, 1),I1,I2,I3)=0.;
        derivative(MCE( 0,+1, 1),I1,I2,I3)=0.;
        derivative(MCE(+1,+1, 1),I1,I2,I3)=0.;

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
        derivative(MCE(-2, 0, 0),I1,I2,I3)=    -h42(axis1);
        derivative(MCE(-1, 0, 0),I1,I2,I3)= 16.*h42(axis1);
        derivative(MCE( 0, 0, 0),I1,I2,I3)=-30.*h42(axis1);
        derivative(MCE(+1, 0, 0),I1,I2,I3)= 16.*h42(axis1);
        derivative(MCE(+2, 0, 0),I1,I2,I3)=    -h42(axis1);
      }
      else if( numberOfDimensions==2 )
      {
	Range M(0,24);
	derivative(M+CE(c0,e0),I1,I2,I3)=0;
	derivative(MCE( 0,-2, 0),I1,I2,I3)=                     -h42(axis2);
	derivative(MCE( 0,-1, 0),I1,I2,I3)=                  16.*h42(axis2);
	derivative(MCE(-2, 0, 0),I1,I2,I3)=     -h42(axis1);
	derivative(MCE(-1, 0, 0),I1,I2,I3)= 16.* h42(axis1);
	derivative(MCE( 0, 0, 0),I1,I2,I3)=-30.*(h42(axis1)     +h42(axis2));
	derivative(MCE( 1, 0, 0),I1,I2,I3)= 16.* h42(axis1);
	derivative(MCE( 2, 0, 0),I1,I2,I3)=     -h42(axis1);
	derivative(MCE(  , 1, 0),I1,I2,I3)=                  16.*h42(axis2);
	derivative(MCE( 0, 2, 0),I1,I2,I3)=                     -h42(axis2);
      }
      else  // ======= 3D ================
      {
	Range M(0,124);
	derivative(M+CE(c0,e0),I1,I2,I3)=0;
	derivative(MCE( 0, 0,-2),I1,I2,I3)=                                     -h42(axis3);
	derivative(MCE( 0,  ,-1),I1,I2,I3)=                                  16.*h42(axis3);
	derivative(MCE( 0,-2, 0),I1,I2,I3)=                     -h42(axis2);
	derivative(MCE( 0,-1, 0),I1,I2,I3)=                  16.*h42(axis2);
	derivative(MCE(-2, 0, 0),I1,I2,I3)=     -h42(axis1);
	derivative(MCE(-1, 0, 0),I1,I2,I3)= 16.* h42(axis1);
	derivative(MCE( 0, 0, 0),I1,I2,I3)=-30.*(h42(axis1)     +h42(axis2)     +h42(axis3));
	derivative(MCE( 1, 0, 0),I1,I2,I3)= 16.* h42(axis1);
	derivative(MCE( 2, 0, 0),I1,I2,I3)=     -h42(axis1);
	derivative(MCE(  , 1, 0),I1,I2,I3)=                  16.*h42(axis2);
	derivative(MCE( 0, 2, 0),I1,I2,I3)=                    -h42(axis2);
	derivative(MCE( 0,  , 1),I1,I2,I3)=                                  16.*h42(axis3);
	derivative(MCE( 0, 0, 2),I1,I2,I3)=                                     -h42(axis3);
      }
    }

  }
  else
  {
    // *** optimized curvilinear ****

    if( mgop.usingConservativeApproximations() )
    {
      printf(">>> evaluate the conservative form of laplacian\n");
      RealMappedGridFunction scalar(mgop.mappedGrid);  // do this for now **** fix this ****
      scalar=1.;
      divScalarGradFDerivCoefficients(derivative,scalar,I1,I2,I3,E,C,mgop);
      return;
    }


    if( orderOfAccuracy==2 )
    {
      laplaceFDerivCoefficients2(derivative,I1,I2,I3,E,C,mgop );
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
        laplaceFDerivCoefficients42(derivative,I1,I2,I3,E,C,mgop );
      }
      else  // ======= 3D ================
      {
        laplaceFDerivCoefficients43(derivative,I1,I2,I3,E,C,mgop );
      }
    }
  }
  // time=getCPU()-time;
  // printf("laplaceFDerivCoefficients: time = %e \n",time);
  
  // now make copies for other components
  Index M(0,stencilSize);
  for( int c=C.getBase(); c<=C.getBound(); c++ )                        
  for( int e=E.getBase(); e<=E.getBound(); e++ )                        
    if( c!=c0 || e!=e0 )
      derivative(M+CE(c,e),I1,I2,I3)=derivative(M+CE(c0,e0),I1,I2,I3);
}


#undef CE
