//================================================================================
//
//  MODIFIED:  April 19, 1999 WJW
//             990609 WDH : cleaned up a bit.
//             000618 : WDH fixed harmonic averaging
//
//  NOTES:
//
//  Define the coefficient matrix for the operator div ( scalar grad )
//
//   Who to Blame:  Bill Wangard
//
//   Errors you will find:  None!
//
//   Fully tested?
//
//                 2-D... yes
//                 3-D... yes
//                 1-D... no
//
//   To do:  Optimize these operators for uniform rectangular grids.
//           Implement 4th-order flavors.
//
//================================================================================

#include "MappedGridOperators.h"
#include "xDC.h"

#undef ARITHMETIC_AVERAGE
#define ARITHMETIC_AVERAGE(s,I1,I2,I3,J1,J2,J3) (s(I1,I2,I3) + s(J1,J2,J3))

// undef HARMONIC_AVERAGE 
// define HARMONIC_AVERAGE(s,i,I1,I2,I3,J1,J2,J3) (s(i,I1,I2,I3)*s(i,J1,J2,J3)/(s(i,I1,I2,I3)+s(i,J1,J2,J3) + REAL_EPSILON))

void 
divScalarGradFDerivCoefficients42(RealDistributedArray & derivative,
                                               const realMappedGridFunction & s,
                                               const Index & I1,
                                               const Index & I2,
                                               const Index & I3,
                                               const Index & E,
                                               const Index & C,
                                               MappedGridOperators & mgop )
// 4th order, 2d
{                                                                        
  cout << "Sorry: divScalarGradCoefficients, 4th order, 2d, is not implemented yet." << endl;
  Overture::abort( "error"); 
}

void 
divScalarGradFDerivCoefficients43(RealDistributedArray & derivative,
                                               const realMappedGridFunction & s,
                                               const Index & I1,
                                               const Index & I2,
                                               const Index & I3,
                                               const Index & E,
                                               const Index & C,
                                               MappedGridOperators & mgop )
// 4th order 3d
{                                                                        
  cout << "Sorry: divScalarGradCoefficients, 4th order, 3d, is not implemented yet." << endl;
  Overture::abort( "error"); 
}


void 
divScalarGradFDerivCoefficients2(RealDistributedArray & derivative,
                                              const realMappedGridFunction & s,
                                              const Index & I1,
                                              const Index & I2,
                                              const Index & I3,
                                              const Index & E,
                                              const Index & C,
                                              MappedGridOperators & mgop )
{                                                                        
  // printf(" ****** divScalarGradFDerivCoefficients ************* \n");

  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();

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

  RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
  RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;
  
// Averaging factor for face values ( = 0.5 for arithmetic averaging, 2.0 for harmonic averaging)
  real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;
  
  
  if( numberOfDimensions==1 )
  { // get coefficients for the first component
      
    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
      
    RealDistributedArray rx(J1,I2,I3), a11(J1,I2,I3);
      
    rx = RX(J1,I2,I3);
      
    const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? .5 : 2.;
      
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a11 = s(J1,I2,I3)*rx;
      J1 = Range(I1.getBase(),I1.getBound()+1);
      a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    }
    else    
    {
      J1 = Range(I1.getBase(),I1.getBound()+1);
      a11(J1,I2,I3) = ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3)*mgop.harmonic(s(J1,I2,I3),s(J1-1,I2,I3));
    }
      
    a11.reshape(1,a11.dimension(0),I2,I3);
    rx.reshape(1,J1,I2,I3);

    derivative(MCE(-1, 0, 0),I1,I2,I3)=                    a11(0,I1,I2,I3) *rx(0,I1,I2,I3);
    derivative(MCE( 0, 0, 0),I1,I2,I3)=-(a11(0,I1+1,I2,I3)+a11(0,I1,I2,I3))*rx(0,I1,I2,I3);
    derivative(MCE(+1, 0, 0),I1,I2,I3)=  a11(0,I1+1,I2,I3)                 *rx(0,I1,I2,I3);
      
  }
  else if( numberOfDimensions==2 )
  {
    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
    Index J2 = Range(I2.getBase()-1,I2.getBound()+1);      

    const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();

      // Vertex centered values of the transformation derivatives 
    RealDistributedArray a11(J1,J2,I3), a12(J1,J2,I3), a22(J1,J2,I3);
    realArray a21;

    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      RealDistributedArray sj(J1,J2,I3);
      sj = s(J1,J2,I3) * j(J1,J2,I3);
      a11 = (RX(J1,J2,I3) * RX(J1,J2,I3) + RY(J1,J2,I3) * RY(J1,J2,I3))*sj; 
      a12 = (RX(J1,J2,I3) * SX(J1,J2,I3) + RY(J1,J2,I3) * SY(J1,J2,I3))*sj; 
      a22 = (SX(J1,J2,I3) * SX(J1,J2,I3) + SY(J1,J2,I3) * SY(J1,J2,I3))*sj;
      a21 = a12;
      
      J1 = Range(I1.getBase(),I1.getBound()+1);

      a11(J1,I2,I3) = (factor*d22(axis1)           )*ARITHMETIC_AVERAGE(a11,J1,I2,I3,J1-1,I2,I3);
      a12(J1,I2,I3) = (factor*d12(axis1)*d12(axis2))*ARITHMETIC_AVERAGE(a12,J1,I2,I3,J1-1,I2,I3);
      
    }
    else    
    {
      a11 = (RX(J1,J2,I3) * RX(J1,J2,I3) + RY(J1,J2,I3) * RY(J1,J2,I3))*j(J1,J2,I3); 
      a12 = (RX(J1,J2,I3) * SX(J1,J2,I3) + RY(J1,J2,I3) * SY(J1,J2,I3))*j(J1,J2,I3); 
      a22 = (SX(J1,J2,I3) * SX(J1,J2,I3) + SY(J1,J2,I3) * SY(J1,J2,I3))*j(J1,J2,I3);
      a21 = a12;
      
      J1 = Range(I1.getBase(),I1.getBound()+1);
      realArray sh = mgop.harmonic(s(J1,I2,I3),s(J1-1,I2,I3));
      a11(J1,I2,I3) =(d22(axis1)           )*ARITHMETIC_AVERAGE(a11,J1,I2,I3,J1-1,I2,I3)*sh;
      a12(J1,I2,I3) =(d12(axis1)*d12(axis2))*ARITHMETIC_AVERAGE(a12,J1,I2,I3,J1-1,I2,I3)*sh;
    }

    J2 = Range(I2.getBase()  ,I2.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a22(I1,J2,I3) = (factor*d22(axis2)           )*ARITHMETIC_AVERAGE(a22,I1,J2,I3,I1,J2-1,I3);
      a21(I1,J2,I3) = (factor*d12(axis1)*d12(axis2))*ARITHMETIC_AVERAGE(a21,I1,J2,I3,I1,J2-1,I3);
    }
    else   
    {
      realArray sh = mgop.harmonic(s(I1,J2,I3),s(I1,J2-1,I3));
      a22(I1,J2,I3) = (d22(axis2)           )*ARITHMETIC_AVERAGE(a22,I1,J2,I3,I1,J2-1,I3)*sh;
      a21(I1,J2,I3) = (d12(axis1)*d12(axis2))*ARITHMETIC_AVERAGE(a21,I1,J2,I3,I1,J2-1,I3)*sh;
    }

    J1=a11.dimension(0);
    J2=a11.dimension(1);
    a11.reshape(1,J1,J2,I3);
    a12.reshape(1,J1,J2,I3);
    a21.reshape(1,J1,J2,I3);
    a22.reshape(1,J1,J2,I3);

    RealDistributedArray jInverse = evaluate(1./j(I1,I2,I3));
    jInverse.reshape(1,I1,I2,I3);
      
    derivative(MCE(-1,-1, 0),I1,I2,I3) =  (a12(0,I1  ,I2  ,I3) + a21(0,I1  ,I2  ,I3)) * jInverse;
    derivative(MCE(+1,-1, 0),I1,I2,I3) = -(a12(0,I1+1,I2  ,I3) + a21(0,I1  ,I2  ,I3)) * jInverse;
    derivative(MCE(-1,+1, 0),I1,I2,I3) = -(a12(0,I1  ,I2  ,I3) + a21(0,I1  ,I2+1,I3)) * jInverse;
    derivative(MCE(+1,+1, 0),I1,I2,I3) =  (a12(0,I1+1,I2  ,I3) + a21(0,I1  ,I2+1,I3)) * jInverse;
      
    derivative(MCE( 0,-1, 0),I1,I2,I3)=(a22(0,I1  ,I2  ,I3)+a12(0,I1  ,I2  ,I3)-a12(0,I1+1,I2  ,I3))*jInverse;
    derivative(MCE(-1, 0, 0),I1,I2,I3)=(a11(0,I1  ,I2  ,I3)+a21(0,I1  ,I2  ,I3)-a21(0,I1  ,I2+1,I3))*jInverse;
    derivative(MCE(+1, 0, 0),I1,I2,I3)=(a11(0,I1+1,I2  ,I3)+a21(0,I1  ,I2+1,I3)-a21(0,I1  ,I2  ,I3))*jInverse;
    derivative(MCE( 0,+1, 0),I1,I2,I3)=(a22(0,I1  ,I2+1,I3)+a12(0,I1+1,I2  ,I3)-a12(0,I1  ,I2  ,I3))*jInverse;
      
    derivative(MCE( 0, 0, 0),I1,I2,I3) = -(a11(0,I1+1,I2  ,I3) + a11(0,I1  ,I2  ,I3) + 
					   a22(0,I1  ,I2+1,I3) + a22(0,I1  ,I2  ,I3)) * jInverse;
     
  }
  else // ======= 3D ================
  {

    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
    Index J2 = Range(I2.getBase()-1,I2.getBound()+1);      
    Index J3 = Range(I3.getBase()-1,I3.getBound()+1);
      
    const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();
      
    // Vertex centered values of the transformation derivatives (ROW 1)
    RealDistributedArray a11(J1,J2,J3), a12(J1,J2,J3), a13(J1,J2,J3),
      a22(J1,J2,J3), a23(J1,J2,J3), a33(J1,J2,J3);
      
    realArray a21;
    realArray a31;
    realArray a32;
    

    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      RealDistributedArray sj(J1,J2,J3);
      sj = s(J1,J2,J3) * j(J1,J2,J3);

      a11 = (RX(J1,J2,J3) * RX(J1,J2,J3) + RY(J1,J2,J3) * RY(J1,J2,J3) + RZ(J1,J2,J3) * RZ(J1,J2,J3))*sj; 
      a12 = (RX(J1,J2,J3) * SX(J1,J2,J3) + RY(J1,J2,J3) * SY(J1,J2,J3) + RZ(J1,J2,J3) * SZ(J1,J2,J3))*sj; 
      a13 = (RX(J1,J2,J3) * TX(J1,J2,J3) + RY(J1,J2,J3) * TY(J1,J2,J3) + RZ(J1,J2,J3) * TZ(J1,J2,J3))*sj; 
      a22 = (SX(J1,J2,J3) * SX(J1,J2,J3) + SY(J1,J2,J3) * SY(J1,J2,J3) + SZ(J1,J2,J3) * SZ(J1,J2,J3))*sj; 
      a23 = (SX(J1,J2,J3) * TX(J1,J2,J3) + SY(J1,J2,J3) * TY(J1,J2,J3) + SZ(J1,J2,J3) * TZ(J1,J2,J3))*sj; 
      a33 = (TX(J1,J2,J3) * TX(J1,J2,J3) + TY(J1,J2,J3) * TY(J1,J2,J3) + TZ(J1,J2,J3) * TZ(J1,J2,J3))*sj; 
      a21 = a12;
      a31 = a13;
      a32 = a23;
      
      J1 = Range(I1.getBase(),I1.getBound()+1);

      a11(J1,I2,I3) = (factor*d22(axis1)           )*ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
      a12(J1,I2,I3) = (factor*d12(axis1)*d12(axis2))*ARITHMETIC_AVERAGE (a12,J1,I2,I3,J1-1,I2,I3);
      a13(J1,I2,I3) = (factor*d12(axis1)*d12(axis3))*ARITHMETIC_AVERAGE (a13,J1,I2,I3,J1-1,I2,I3);
    }
    else    
    {
      a11 = (RX(J1,J2,J3) * RX(J1,J2,J3) + RY(J1,J2,J3) * RY(J1,J2,J3) + RZ(J1,J2,J3) * RZ(J1,J2,J3))*j(J1,J2,J3); 
      a12 = (RX(J1,J2,J3) * SX(J1,J2,J3) + RY(J1,J2,J3) * SY(J1,J2,J3) + RZ(J1,J2,J3) * SZ(J1,J2,J3))*j(J1,J2,J3); 
      a13 = (RX(J1,J2,J3) * TX(J1,J2,J3) + RY(J1,J2,J3) * TY(J1,J2,J3) + RZ(J1,J2,J3) * TZ(J1,J2,J3))*j(J1,J2,J3); 
      a22 = (SX(J1,J2,J3) * SX(J1,J2,J3) + SY(J1,J2,J3) * SY(J1,J2,J3) + SZ(J1,J2,J3) * SZ(J1,J2,J3))*j(J1,J2,J3); 
      a23 = (SX(J1,J2,J3) * TX(J1,J2,J3) + SY(J1,J2,J3) * TY(J1,J2,J3) + SZ(J1,J2,J3) * TZ(J1,J2,J3))*j(J1,J2,J3); 
      a33 = (TX(J1,J2,J3) * TX(J1,J2,J3) + TY(J1,J2,J3) * TY(J1,J2,J3) + TZ(J1,J2,J3) * TZ(J1,J2,J3))*j(J1,J2,J3); 
      a21 = a12;
      a31 = a13;
      a32 = a23;
      
      J1 = Range(I1.getBase(),I1.getBound()+1);

      realArray sh = mgop.harmonic(s(J1,I2,I3),s(J1-1,I2,I3));

      a11(J1,I2,I3) = (d22(axis1)           )*ARITHMETIC_AVERAGE(a11,J1,I2,I3,J1-1,I2,I3)*sh;
      a12(J1,I2,I3) = (d12(axis1)*d12(axis2))*ARITHMETIC_AVERAGE(a12,J1,I2,I3,J1-1,I2,I3)*sh;
      a13(J1,I2,I3) = (d12(axis1)*d12(axis3))*ARITHMETIC_AVERAGE(a13,J1,I2,I3,J1-1,I2,I3)*sh;

    }
      
    J2 = Range(I2.getBase()  ,I2.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a21(I1,J2,I3) = (factor*d12(axis2)*d12(axis1))* ARITHMETIC_AVERAGE (a21,I1,J2,I3,I1,J2-1,I3);
      a22(I1,J2,I3) = (factor*d22(axis2))*ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
      a23(I1,J2,I3) = (factor*d12(axis2)*d12(axis3))*ARITHMETIC_AVERAGE (a23,I1,J2,I3,I1,J2-1,I3);
    }
    else   
    {
      realArray sh = mgop.harmonic(s(I1,J2,I3),s(I1,J2-1,I3));
      a21(I1,J2,I3) = (d12(axis2)*d12(axis1))*ARITHMETIC_AVERAGE(a21,I1,J2,I3,I1,J2-1,I3)*sh;
      a22(I1,J2,I3) = (d22(axis2)           )*ARITHMETIC_AVERAGE(a22,I1,J2,I3,I1,J2-1,I3)*sh;
      a23(I1,J2,I3) = (d12(axis2)*d12(axis3))*ARITHMETIC_AVERAGE(a23,I1,J2,I3,I1,J2-1,I3)*sh;

    }
      
    J3 = Range(I3.getBase()  ,I3.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a31(I1,I2,J3) = (factor*d12(axis3)*d12(axis1))*ARITHMETIC_AVERAGE (a31,I1,I2,J3,I1,I2,J3-1);
      a32(I1,I2,J3) = (factor*d12(axis3)*d12(axis2))*ARITHMETIC_AVERAGE (a32,I1,I2,J3,I1,I2,J3-1);
      a33(I1,I2,J3) = (factor*d22(axis3))*ARITHMETIC_AVERAGE (a33,I1,I2,J3,I1,I2,J3-1);    
    }
    else
    {

      realArray sh = mgop.harmonic(s(I1,I2,J3),s(I1,I2,J3-1));
      a31(I1,I2,J3) = (d12(axis3)*d12(axis1))*ARITHMETIC_AVERAGE(a31,I1,I2,J3,I1,I2,J3-1)*sh;
      a32(I1,I2,J3) = (d12(axis3)*d12(axis2))*ARITHMETIC_AVERAGE(a32,I1,I2,J3,I1,I2,J3-1)*sh;
      a33(I1,I2,J3) = (d22(axis3)           )*ARITHMETIC_AVERAGE(a33,I1,I2,J3,I1,I2,J3-1)*sh;    

    }
      
    J1=a11.dimension(0);
    J2=a11.dimension(1);
    J3=a11.dimension(2);
    a11.reshape(1,J1,J2,J3);
    a12.reshape(1,J1,J2,J3);
    a13.reshape(1,J1,J2,J3);
    a21.reshape(1,J1,J2,J3);
    a22.reshape(1,J1,J2,J3);
    a23.reshape(1,J1,J2,J3);
    a31.reshape(1,J1,J2,J3);
    a32.reshape(1,J1,J2,J3);
    a33.reshape(1,J1,J2,J3);

    // The inverse of the jacobian
    RealDistributedArray jInverse = evaluate(1./j(I1,I2,I3));
    jInverse.reshape(1,I1,I2,I3);

      // The corners are not used
    derivative(MCE(-1,-1,-1),I1,I2,I3)= 0.;
    derivative(MCE(+1,-1,-1),I1,I2,I3)= 0.;     
    derivative(MCE(-1,+1,-1),I1,I2,I3)= 0.;      
    derivative(MCE(+1,+1,-1),I1,I2,I3)= 0.;
    derivative(MCE(-1,-1,+1),I1,I2,I3)= 0.;
    derivative(MCE(+1,-1,+1),I1,I2,I3)= 0.;
    derivative(MCE(-1,+1,+1),I1,I2,I3)= 0.;
    derivative(MCE(+1,+1,+1),I1,I2,I3)= 0.;

    derivative(MCE(0 ,+1,-1),I1,I2,I3) = -(a23(0,I1  ,I2+1,I3  ) + a32(0,I1  ,I2  ,I3  )) * jInverse;
    derivative(MCE(0 ,+1,+1),I1,I2,I3) =  (a23(0,I1  ,I2+1,I3  ) + a32(0,I1  ,I2  ,I3+1)) * jInverse;
    derivative(MCE(0 ,-1,+1),I1,I2,I3) = -(a23(0,I1  ,I2  ,I3  ) + a32(0,I1  ,I2  ,I3+1)) * jInverse;
    derivative(MCE(0 ,-1,-1),I1,I2,I3) =  (a23(0,I1  ,I2  ,I3  ) + a32(0,I1  ,I2  ,I3  )) * jInverse;

    derivative(MCE(+1, 0,-1),I1,I2,I3) = -(a13(0,I1+1,I2  ,I3  ) + a31(0,I1  ,I2  ,I3  )) * jInverse;
    derivative(MCE(+1, 0,+1),I1,I2,I3) =  (a13(0,I1+1,I2  ,I3  ) + a31(0,I1  ,I2  ,I3+1)) * jInverse;
    derivative(MCE(-1, 0,+1),I1,I2,I3) = -(a13(0,I1  ,I2  ,I3  ) + a31(0,I1  ,I2  ,I3+1)) * jInverse;
    derivative(MCE(-1, 0,-1),I1,I2,I3) =  (a13(0,I1  ,I2  ,I3  ) + a31(0,I1  ,I2  ,I3  )) * jInverse;
					  
    derivative(MCE(+1,-1, 0),I1,I2,I3) = -(a12(0,I1+1,I2  ,I3  ) + a21(0,I1  ,I2  ,I3  )) * jInverse;
    derivative(MCE(+1,+1, 0),I1,I2,I3) =  (a12(0,I1+1,I2  ,I3  ) + a21(0,I1  ,I2+1,I3  )) * jInverse;
    derivative(MCE(-1,+1, 0),I1,I2,I3) = -(a12(0,I1  ,I2  ,I3  ) + a21(0,I1  ,I2+1,I3  )) * jInverse;
    derivative(MCE(-1,-1, 0),I1,I2,I3) =  (a12(0,I1  ,I2  ,I3  ) + a21(0,I1  ,I2  ,I3  )) * jInverse;
      
    derivative(MCE( 0, 0,-1),I1,I2,I3) = (a33(0,I1  ,I2  ,I3  )+a13(0,I1  ,I2  ,I3  )-a13(0,I1+1,I2  ,I3  )+
					  a23(0,I1  ,I2  ,I3  )-a23(0,I1  ,I2+1,I3  )) *jInverse;
 
    derivative(MCE( 0, 0,+1),I1,I2,I3) = (a33(0,I1  ,I2  ,I3+1)+a13(0,I1+1,I2  ,I3  )-a13(0,I1  ,I2  ,I3  )+
					  a23(0,I1  ,I2+1,I3  )-a23(0,I1  ,I2  ,I3  )) * jInverse;

    derivative(MCE( 0,-1, 0),I1,I2,I3) = (a22(0,I1  ,I2  ,I3  )+a32(0,I1  ,I2  ,I3  )-a32(0,I1  ,I2  ,I3+1) +
					  a12(0,I1  ,I2  ,I3  )-a12(0,I1+1,I2  ,I3  ))*jInverse;
      
    derivative(MCE( 0,+1, 0),I1,I2,I3) = (a22(0,I1  ,I2+1,I3  )+a32(0,I1  ,I2  ,I3+1)-a32(0,I1  ,I2  ,I3  ) +
					  a12(0,I1+1,I2  ,I3  )-a12(0,I1  ,I2  ,I3  ))*jInverse;
 
    derivative(MCE(-1, 0, 0),I1,I2,I3) = (a11(0,I1  ,I2  ,I3  )+a31(0,I1  ,I2  ,I3  )-a31(0,I1  ,I2  ,I3+1) +
					  a21(0,I1  ,I2  ,I3  )-a21(0,I1  ,I2+1,I3  ))*jInverse;
      
    derivative(MCE(+1, 0, 0),I1,I2,I3) = (a11(0,I1+1,I2  ,I3  )+a31(0,I1  ,I2  ,I3+1)-a31(0,I1  ,I2  ,I3  ) +
					  a21(0,I1  ,I2+1,I3  )-a21(0,I1  ,I2  ,I3  ))*jInverse;
      
    derivative(MCE( 0, 0, 0),I1,I2,I3) = -(  a33(0,I1  ,I2  ,I3  )+  a33(0,I1  ,I2  ,I3+1) +
					     a22(0,I1  ,I2  ,I3  )+  a22(0,I1  ,I2+1,I3  ) +
					     a11(0,I1  ,I2  ,I3  )+  a11(0,I1+1,I2  ,I3  ))*jInverse;
     
  }
}

void 
divScalarGradFDerivCoefficients2R(RealDistributedArray & derivative,
                                              const realMappedGridFunction & s,
                                              const Index & I1,
                                              const Index & I2,
                                              const Index & I3,
                                              const Index & E,
                                              const Index & C,
                                              MappedGridOperators & mgop )
// ==================================================================================================
// /Description:
//    Optimized for rectangular grid.
// ==================================================================================================
{                                                                        
  // printf(" ****** divScalarGradFDerivCoefficients ************* \n");

  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();

  int & width              = mgop.width;
  int & halfWidth1         = mgop.halfWidth1;
  int & halfWidth2         = mgop.halfWidth2;
  int & halfWidth3         = mgop.halfWidth3;
  int & stencilSize        = mgop.stencilSize;
  int & numberOfComponentsForCoefficients = mgop.numberOfComponentsForCoefficients;

  int e0=E.getBase();
  int c0=C.getBase();

      real h22c[3];
#define h22(n) h22c[n]
      for( int axis=0; axis<3; axis++ )
	h22(axis)=1./SQR(mgop.dx[axis]);
  
// Averaging factor for face values ( = 0.5 for arithmetic averaging, 2.0 for harmonic averaging)
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;
  
  
  if( numberOfDimensions==1 )
  { // get coefficients for the first component
      
    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
      
    RealDistributedArray a11(J1,I2,I3);
      
    a11 = s(J1,I2,I3);
      
    const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? .5 : 2.;
      
    J1 = Range(I1.getBase(),I1.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
      a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    else    
      a11(J1,I2,I3) = factor * mgop.harmonic (a11(J1,I2,I3),a11(J1-1,I2,I3));
//      a11(J1,I2,I3) = factor * HARMONIC_AVERAGE  (a11,J1,I2,I3,J1-1,I2,I3);
      
    a11.reshape(1,a11.dimension(0),I2,I3);

    derivative(MCE(-1, 0, 0),I1,I2,I3)=                    a11(0,I1,I2,I3) *h22(0);
    derivative(MCE( 0, 0, 0),I1,I2,I3)=-(a11(0,I1+1,I2,I3)+a11(0,I1,I2,I3))*h22(0);
    derivative(MCE(+1, 0, 0),I1,I2,I3)=  a11(0,I1+1,I2,I3)                 *h22(0);
      
  }
  else if( numberOfDimensions==2 )
  {
    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
    Index J2 = Range(I2.getBase()-1,I2.getBound()+1);      

      // Vertex centered values of the transformation derivatives 
    RealDistributedArray a11(J1,J2,I3), a22(J1,J2,I3);
 
    a11 = s(J1,J2,I3);
    a22 = s(J1,J2,I3);

    J1 = Range(I1.getBase(),I1.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
      a11(J1,I2,I3) = (factor*h22(axis1))*ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    else    
      a11(J1,I2,I3) = (factor*h22(axis1))*mgop.harmonic(a11(J1,I2,I3),a11(J1-1,I2,I3));
//      a11(J1,I2,I3) = (factor*h22(axis1))*HARMONIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);

    J2 = Range(I2.getBase()  ,I2.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
      a22(I1,J2,I3) = (factor*h22(axis2))*ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
    else   
      a22(I1,J2,I3) = (factor*h22(axis2))*mgop.harmonic(a22(I1,J2,I3),a22(I1,J2-1,I3));
//      a22(I1,J2,I3) = (factor*h22(axis2))*HARMONIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);

    J1=a11.dimension(0);
    J2=a11.dimension(1);
    a11.reshape(1,J1,J2,I3);
    a22.reshape(1,J1,J2,I3);
      
    derivative(MCE(-1,-1, 0),I1,I2,I3) = 0.;   // is this needed??
    derivative(MCE(+1,-1, 0),I1,I2,I3) = 0.;
    derivative(MCE(-1,+1, 0),I1,I2,I3) = 0.;
    derivative(MCE(+1,+1, 0),I1,I2,I3) = 0.;
      
    derivative(MCE( 0,-1, 0),I1,I2,I3)=a22(0,I1  ,I2  ,I3);
    derivative(MCE(-1, 0, 0),I1,I2,I3)=a11(0,I1  ,I2  ,I3);
    derivative(MCE(+1, 0, 0),I1,I2,I3)=a11(0,I1+1,I2  ,I3);
    derivative(MCE( 0,+1, 0),I1,I2,I3)=a22(0,I1  ,I2+1,I3);
      
    derivative(MCE( 0, 0, 0),I1,I2,I3) = -(a11(0,I1+1,I2  ,I3) + a11(0,I1  ,I2  ,I3) + 
					   a22(0,I1  ,I2+1,I3) + a22(0,I1  ,I2  ,I3));
     
  }
  else // ======= 3D ================
  {

    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
    Index J2 = Range(I2.getBase()-1,I2.getBound()+1);      
    Index J3 = Range(I3.getBase()-1,I3.getBound()+1);
      
    // Vertex centered values of the transformation derivatives (ROW 1)
    RealDistributedArray a11(J1,J2,J3), a22(J1,J2,J3), a33(J1,J2,J3);
      
    a11 = s(J1,J2,J3);
    a22 = s(J1,J2,J3);
    a33 = s(J1,J2,J3);
      
      
    J1 = Range(I1.getBase(),I1.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
      a11(J1,I2,I3) = (factor*h22(axis1))*ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    else    
      a11(J1,I2,I3) = (factor*h22(axis1))*mgop.harmonic(a11(J1,I2,I3),a11(J1-1,I2,I3));
      
    J2 = Range(I2.getBase()  ,I2.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
      a22(I1,J2,I3) = (factor*h22(axis2))*ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
    else   
      a22(I1,J2,I3) = (factor*h22(axis2))*mgop.harmonic(a22(I1,J2,I3),a22(I1,J2-1,I3));
      
    J3 = Range(I3.getBase()  ,I3.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
      a33(I1,I2,J3) = (factor*h22(axis3))*ARITHMETIC_AVERAGE (a33,I1,I2,J3,I1,I2,J3-1);    
    else
      a33(I1,I2,J3) = (factor*h22(axis3))*mgop.harmonic(a33(I1,I2,J3),a33(I1,I2,J3-1));
      
    J1=a11.dimension(0);
    J2=a11.dimension(1);
    J3=a11.dimension(2);

    a11.reshape(1,J1,J2,J3);
    a22.reshape(1,J1,J2,J3);
    a33.reshape(1,J1,J2,J3);

      // The corners are not used
    derivative(MCE(-1,-1,-1),I1,I2,I3)= 0.;
    derivative(MCE(+1,-1,-1),I1,I2,I3)= 0.;     
    derivative(MCE(-1,+1,-1),I1,I2,I3)= 0.;      
    derivative(MCE(+1,+1,-1),I1,I2,I3)= 0.;
    derivative(MCE(-1,-1,+1),I1,I2,I3)= 0.;
    derivative(MCE(+1,-1,+1),I1,I2,I3)= 0.;
    derivative(MCE(-1,+1,+1),I1,I2,I3)= 0.;
    derivative(MCE(+1,+1,+1),I1,I2,I3)= 0.;

    derivative(MCE(0 ,+1,-1),I1,I2,I3) = 0.;
    derivative(MCE(0 ,+1,+1),I1,I2,I3) = 0.;
    derivative(MCE(0 ,-1,+1),I1,I2,I3) = 0.;
    derivative(MCE(0 ,-1,-1),I1,I2,I3) = 0.;
					
    derivative(MCE(+1, 0,-1),I1,I2,I3) = 0.;
    derivative(MCE(+1, 0,+1),I1,I2,I3) = 0.;
    derivative(MCE(-1, 0,+1),I1,I2,I3) = 0.;
    derivative(MCE(-1, 0,-1),I1,I2,I3) = 0.;
					 
    derivative(MCE(+1,-1, 0),I1,I2,I3) = 0.;
    derivative(MCE(+1,+1, 0),I1,I2,I3) = 0.;
    derivative(MCE(-1,+1, 0),I1,I2,I3) = 0.;
    derivative(MCE(-1,-1, 0),I1,I2,I3) = 0.;
      
    derivative(MCE( 0, 0,-1),I1,I2,I3) = a33(0,I1  ,I2  ,I3  );
    derivative(MCE( 0, 0,+1),I1,I2,I3) = a33(0,I1  ,I2  ,I3+1);
    derivative(MCE( 0,-1, 0),I1,I2,I3) = a22(0,I1  ,I2  ,I3  );
    derivative(MCE( 0,+1, 0),I1,I2,I3) = a22(0,I1  ,I2+1,I3  );
    derivative(MCE(-1, 0, 0),I1,I2,I3) = a11(0,I1  ,I2  ,I3  );
    derivative(MCE(+1, 0, 0),I1,I2,I3) = a11(0,I1+1,I2  ,I3  );
    derivative(MCE( 0, 0, 0),I1,I2,I3) = -(  a33(0,I1  ,I2  ,I3  )+  a33(0,I1  ,I2  ,I3+1) +
					     a22(0,I1  ,I2  ,I3  )+  a22(0,I1  ,I2+1,I3  ) +
					     a11(0,I1  ,I2  ,I3  )+  a11(0,I1+1,I2  ,I3  ));
     
  }
}


void 
divScalarGradFDerivCoefficients(RealDistributedArray & derivative,
                                             const realMappedGridFunction & s,
                                             const Index & I1,
                                             const Index & I2,
                                             const Index & I3,
                                             const Index & E,
                                             const Index & C,
                                             MappedGridOperators & mgop )
{                                                                        

  int & orderOfAccuracy    = mgop.orderOfAccuracy;
  int & stencilSize        = mgop.stencilSize;
  int & numberOfComponentsForCoefficients = mgop.numberOfComponentsForCoefficients;

  int e0=E.getBase();
  int c0=C.getBase();

  // real time=getCPU();
  if( mgop.isRectangular() )
  { 
    if( orderOfAccuracy==2 )
    {
      divScalarGradFDerivCoefficients2R(derivative,s,I1,I2,I3,E,C,mgop );

    }
    else   // ====== 4th order =======
    {
      Overture::abort("error");
    }
  }
  else
  {
    // *** optimized curvilinear ****
    if( orderOfAccuracy==2 )
    {
      divScalarGradFDerivCoefficients2(derivative,s,I1,I2,I3,E,C,mgop );
    }
    else   // ====== 4th order =======
    {
      Overture::abort("error");
    }
  }
  // time=getCPU()-time;
  // printf("divScalarGradFDerivCoefficients: time = %e \n",time);
  
  // now make copies for other components
  Index M(0,stencilSize);
  for( int c=C.getBase(); c<=C.getBound(); c++ )                        
  for( int e=E.getBase(); e<=E.getBound(); e++ )                        
    if( c!=c0 || e!=e0 )
      derivative(M+CE(c,e),I1,I2,I3)=derivative(M+CE(c0,e0),I1,I2,I3);
}


#undef CE
