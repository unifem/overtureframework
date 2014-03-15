//================================================================================
//
//  LAST MODIFIED:  April 19, 1999
//
//
//  NOTES:
//
//  Define the coefficient matrix for the operator d/dxi ( scalar d/dxj )
//
//   where i and j are integers representing the (Cartesian) coordinate
//   directions x,y, or z (0,1, or 2)
//
//   1-D:  i = j = 0
//
//   2-D:  i = 0 or 1
//         j = 0 or 1
//
//   3-D:  i = 0, 1, or 2
//         j = 0, 1, or 2
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

#undef RXI
#undef SXI
#undef TXI
#define RXI(I1,I2,I3,dir)  (inverseVertexDerivative(I1,I2,I3, numberOfDimensions * dir + 0))
#define SXI(I1,I2,I3,dir)  (inverseVertexDerivative(I1,I2,I3, numberOfDimensions * dir + 1))
#define TXI(I1,I2,I3,dir)  (inverseVertexDerivative(I1,I2,I3, numberOfDimensions * dir + 2))

void 
derivativeScalarDerivativeFDerivCoefficients42(RealDistributedArray & derivative,
                                               const realMappedGridFunction & s,
                                               const int & direction1,
                                               const int & direction2,
                                               const Index & I1,
                                               const Index & I2,
                                               const Index & I3,
                                               const Index & E,
                                               const Index & C,
                                               MappedGridOperators & mgop )
// 4th order, 2d
{                                                                        
  cout << "Sorry: derivativeScalarDerivativeCoefficients, 4th order, 2d, is not implemented yet." << endl;
  Overture::abort( "error"); 
}

void 
derivativeScalarDerivativeFDerivCoefficients43(RealDistributedArray & derivative,
                                               const realMappedGridFunction & s,
                                               const int & direction1,
                                               const int & direction2,
                                               const Index & I1,
                                               const Index & I2,
                                               const Index & I3,
                                               const Index & E,
                                               const Index & C,
                                               MappedGridOperators & mgop )
// 4th order 3d
{                                                                        
  cout << "Sorry: derivativeScalarDerivativeCoefficients, 4th order, 3d, is not implemented yet." << endl;
  Overture::abort( "error"); 
}


void 
derivativeScalarDerivativeFDerivCoefficients2(RealDistributedArray & derivative,
                                              const realMappedGridFunction & s,
                                              const int & direction1,
                                              const int & direction2,
                                              const Index & I1,
                                              const Index & I2,
                                              const Index & I3,
                                              const Index & E,
                                              const Index & C,
                                              MappedGridOperators & mgop )
{                                                                        
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
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;
  
  
  if( numberOfDimensions==1 )
  { // get coefficients for the first component
      
      if (direction1 != 0 || direction2 != 0)
      {
          cout << "direction1 = " << direction1 << endl;
          cout << "direction2 = " << direction2 << endl;
          cout << "You may not differentiate the 2nd or 3rd coordinate directions in 1-D" << endl;
          Overture::abort("Error");
      }
      
      Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
      
      RealDistributedArray rx(J1,I2,I3), a11(J1,I2,I3);
      
      rx = RX(J1,I2,I3);
      a11 = s(J1,I2,I3);
      a11 *= rx;
      
      
      const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? .5 : 2.;
      
      J1 = Range(I1.getBase(),I1.getBound()+1);
      if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
      {
          a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
      }
      else    
      {
          a11(J1,I2,I3) = factor * mgop.harmonic(a11(J1,I2,I3),a11(J1-1,I2,I3));
      }
      J1 = Range(I1.getBase()-1,I1.getBound()+1); // reset
      
      a11.reshape(1,J1,I2,I3);
      rx.reshape(1,J1,I2,I3);
      
      derivative(MCE(-1, 0, 0),I1,I2,I3)=                    a11(0,I1,I2,I3) *rx(0,I1,I2,I3);
      derivative(MCE( 0, 0, 0),I1,I2,I3)=-(a11(0,I1+1,I2,I3)+a11(0,I1,I2,I3))*rx(0,I1,I2,I3);
      derivative(MCE(+1, 0, 0),I1,I2,I3)=  a11(0,I1+1,I2,I3)                 *rx(0,I1,I2,I3);
      
  }
  
  else if( numberOfDimensions==2 )
  {
     

      // Dont allow  d/dz (s d/dxi) or d/dxi (s d/dz) since this is 2-D
      if ( direction1 > 1 || direction2 > 1) 
      {
          cout << "direction1 = " << direction1 << endl;
          cout << "direction2 = " << direction2 << endl;
          cout << "You may not differentiate wrt. the 3rd Coordinate in 2-D" << endl;
          Overture::abort("Error");
      }
      
      Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
      Index J2 = Range(I2.getBase()-1,I2.getBound()+1);      

      RealDistributedArray a11(J1,I2,I3), a12(J1,I2,I3);
      RealDistributedArray a21(I1,J2,I3), a22(I1,J2,I3);

      const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();
      if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
      {
	RealDistributedArray sj;
	sj = s(J1,J2,I3) * j(J1,J2,I3);

	a11 = (RXI(J1,I2,I3,direction1) * RXI(J1,I2,I3,direction2))*sj(J1,I2,I3);
	a12 = (RXI(J1,I2,I3,direction1) * SXI(J1,I2,I3,direction2))*sj(J1,I2,I3);
	a21 = (SXI(I1,J2,I3,direction1) * RXI(I1,J2,I3,direction2))*sj(I1,J2,I3);
	a22 = (SXI(I1,J2,I3,direction1) * SXI(I1,J2,I3,direction2))*sj(I1,J2,I3);
    
	J1 = Range(I1.getBase(),I1.getBound()+1);
	a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
	a12(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a12,J1,I2,I3,J1-1,I2,I3);
      }
      else    
      {
	// only use harmonic average on the scalar since harmonic average doesn't work
	// well for arguments of opposite sign (the factor a12 may change sign)
	a11 = (RXI(J1,I2,I3,direction1) * RXI(J1,I2,I3,direction2))*j(J1,I2,I3);
	a12 = (RXI(J1,I2,I3,direction1) * SXI(J1,I2,I3,direction2))*j(J1,I2,I3);
	a21 = (SXI(I1,J2,I3,direction1) * RXI(I1,J2,I3,direction2))*j(I1,J2,I3);
	a22 = (SXI(I1,J2,I3,direction1) * SXI(I1,J2,I3,direction2))*j(I1,J2,I3);

	J1 = Range(I1.getBase(),I1.getBound()+1);
	realArray sh = mgop.harmonic(s(J1,I2,I3),s(J1-1,I2,I3));
	a11(J1,I2,I3) = ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3)*sh;
	a12(J1,I2,I3) = ARITHMETIC_AVERAGE (a12,J1,I2,I3,J1-1,I2,I3)*sh;
      }

      J1 = Range(I1.getBase()-1,I1.getBound()+1); // reset

      a11.reshape(1,J1,I2,I3);
      a12.reshape(1,J1,I2,I3);

      // In this formulation, d12 should be 1/dx, not 1/(2*dx)
      real dxy = 4.0 * d12(axis1) * d12(axis2);      
      a11 *= d22(axis1);      
      a12 *= dxy;
      
      J2 = Range(I2.getBase()  ,I2.getBound()+1);
      if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
      {
	a21(I1,J2,I3) = factor * ARITHMETIC_AVERAGE (a21,I1,J2,I3,I1,J2-1,I3);
	a22(I1,J2,I3) = factor * ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
      }
      else   
      {
	// only use harmonic average on the scalar since harmonic average doesn't work
	// well for arguments of opposite sign
	realArray sh = mgop.harmonic(s(I1,J2,I3),s(I1,J2-1,I3));
	a21(I1,J2,I3) = ARITHMETIC_AVERAGE (a21,I1,J2,I3,I1,J2-1,I3)*sh;
	a22(I1,J2,I3) = ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3)*sh;
      }

      J2 = Range(I2.getBase()-1,I2.getBound()+1); // reset
      a21.reshape(1,I1,J2,I3);
      a22.reshape(1,I1,J2,I3);
      
      a21 *= dxy;
      a22 *= d22(axis2);
      
 
      RealDistributedArray jInverse = evaluate(1./j(I1,I2,I3));
      jInverse.reshape(1,I1,I2,I3);
      
      derivative(MCE(-1,-1, 0),I1,I2,I3) =  0.25*(a12(0,I1  ,I2  ,I3) + a21(0,I1  ,I2  ,I3)) * jInverse;
      derivative(MCE(+1,-1, 0),I1,I2,I3) = -0.25*(a12(0,I1+1,I2  ,I3) + a21(0,I1  ,I2  ,I3)) * jInverse;
      derivative(MCE(-1,+1, 0),I1,I2,I3) = -0.25*(a12(0,I1  ,I2  ,I3) + a21(0,I1  ,I2+1,I3)) * jInverse;
      derivative(MCE(+1,+1, 0),I1,I2,I3) =  0.25*(a12(0,I1+1,I2  ,I3) + a21(0,I1  ,I2+1,I3)) * jInverse;
      
      derivative(MCE( 0,-1, 0),I1,I2,I3) = (a22(0,I1  ,I2  ,I3) + 0.25*(a12(0,I1  ,I2  ,I3) - a12(0,I1+1,I2  ,I3)))*jInverse;
      derivative(MCE(-1, 0, 0),I1,I2,I3) = (a11(0,I1  ,I2  ,I3) + 0.25*(a21(0,I1  ,I2  ,I3) - a21(0,I1  ,I2+1,I3)))*jInverse;
      derivative(MCE(+1, 0, 0),I1,I2,I3) = (a11(0,I1+1,I2  ,I3) + 0.25*(a21(0,I1  ,I2+1,I3) - a21(0,I1  ,I2  ,I3)))*jInverse;
      derivative(MCE( 0,+1, 0),I1,I2,I3) = (a22(0,I1  ,I2+1,I3) + 0.25*(a12(0,I1+1,I2  ,I3) - a12(0,I1  ,I2  ,I3)))*jInverse;
      
      derivative(MCE( 0, 0, 0),I1,I2,I3) = -(a11(0,I1+1,I2  ,I3) + a11(0,I1  ,I2  ,I3) + 
                                             a22(0,I1  ,I2+1,I3) + a22(0,I1  ,I2  ,I3)) * jInverse;
     
      
  }
  else // ======= 3D ================
  {

      // Dont allow  d/dz (s d/dxi) or d/dxi (s d/dz) since this is 2-D
      if ( direction1 > 2 || direction2 > 2) 
      {
          cout << "direction1 = " << direction1 << endl;
          cout << "direction2 = " << direction2 << endl;
          cout << "You entered an invalid coordinate" << endl;
          Overture::abort("Error");
      }
      
      Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
      Index J2 = Range(I2.getBase()-1,I2.getBound()+1);      
      Index J3 = Range(I3.getBase()-1,I3.getBound()+1);
      
      const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();
      
      // Vertex centered values of the transformation derivatives (ROW 1)
      RealDistributedArray a11(J1,I2,I3), a12(J1,I2,I3), a13(J1,I2,I3);
      RealDistributedArray a21(I1,J2,I3), a22(I1,J2,I3), a23(I1,J2,I3);
      RealDistributedArray a31(I1,I2,J3), a32(I1,I2,J3), a33(I1,I2,J3);
      
      if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
      {
	RealDistributedArray sj(J1,J2,J3);
	sj = s(J1,J2,J3) * j(J1,J2,J3);

	a11 = (RXI(J1,I2,I3,direction1) * RXI(J1,I2,I3,direction2))*sj(J1,I2,I3); 
	a12 = (RXI(J1,I2,I3,direction1) * SXI(J1,I2,I3,direction2))*sj(J1,I2,I3);
	a13 = (RXI(J1,I2,I3,direction1) * TXI(J1,I2,I3,direction2))*sj(J1,I2,I3);
	a21 = (SXI(I1,J2,I3,direction1) * RXI(I1,J2,I3,direction2))*sj(I1,J2,I3);
	a22 = (SXI(I1,J2,I3,direction1) * SXI(I1,J2,I3,direction2))*sj(I1,J2,I3);
	a23 = (SXI(I1,J2,I3,direction1) * TXI(I1,J2,I3,direction2))*sj(I1,J2,I3);
	a31 = (TXI(I1,I2,J3,direction1) * RXI(I1,I2,J3,direction2))*sj(I1,I2,J3);
	a32 = (TXI(I1,I2,J3,direction1) * SXI(I1,I2,J3,direction2))*sj(I1,I2,J3);
	a33 = (TXI(I1,I2,J3,direction1) * TXI(I1,I2,J3,direction2))*sj(I1,I2,J3);
   
	J1 = Range(I1.getBase(),I1.getBound()+1);
	a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
	a12(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a12,J1,I2,I3,J1-1,I2,I3);
	a13(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a13,J1,I2,I3,J1-1,I2,I3);
      }
      else    
      {
   
	a11 = (RXI(J1,I2,I3,direction1) * RXI(J1,I2,I3,direction2))*j(J1,I2,I3); 
	a12 = (RXI(J1,I2,I3,direction1) * SXI(J1,I2,I3,direction2))*j(J1,I2,I3);
	a13 = (RXI(J1,I2,I3,direction1) * TXI(J1,I2,I3,direction2))*j(J1,I2,I3);
	a21 = (SXI(I1,J2,I3,direction1) * RXI(I1,J2,I3,direction2))*j(I1,J2,I3);
	a22 = (SXI(I1,J2,I3,direction1) * SXI(I1,J2,I3,direction2))*j(I1,J2,I3);
	a23 = (SXI(I1,J2,I3,direction1) * TXI(I1,J2,I3,direction2))*j(I1,J2,I3);
	a31 = (TXI(I1,I2,J3,direction1) * RXI(I1,I2,J3,direction2))*j(I1,I2,J3);
	a32 = (TXI(I1,I2,J3,direction1) * SXI(I1,I2,J3,direction2))*j(I1,I2,J3);
	a33 = (TXI(I1,I2,J3,direction1) * TXI(I1,I2,J3,direction2))*j(I1,I2,J3);

	J1 = Range(I1.getBase(),I1.getBound()+1);

	realArray sh;
	sh=mgop.harmonic(s(J1,I2,I3),s(J1-1,I2,I3));
	a11(J1,I2,I3) = ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3) *sh;
	a12(J1,I2,I3) = ARITHMETIC_AVERAGE (a12,J1,I2,I3,J1-1,I2,I3) *sh;
	a13(J1,I2,I3) = ARITHMETIC_AVERAGE (a13,J1,I2,I3,J1-1,I2,I3) *sh;

      }
      J1 = Range(I1.getBase()-1,I1.getBound()+1); // reset
      
      a11.reshape(1,J1,I2,I3);
      a12.reshape(1,J1,I2,I3);
      a13.reshape(1,J1,I2,I3);
      
      // In this formulation, d12 should be 1/dx, not 1/(2*dx)
      real d_12 = 4.0 * d12(axis1) * d12(axis2);      
      real d_13 = 4.0 * d12(axis1) * d12(axis3);
      real d_23 = 4.0 * d12(axis2) * d12(axis3);
      
      // Scale the matrix coefficients by the discretization widths
      a11 *= d22(axis1);      
      a12 *= d_12;
      a13 *= d_13;

      J2 = Range(I2.getBase()  ,I2.getBound()+1);
      if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
      {
	a21(I1,J2,I3) = factor * ARITHMETIC_AVERAGE (a21,I1,J2,I3,I1,J2-1,I3);
	a22(I1,J2,I3) = factor * ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
	a23(I1,J2,I3) = factor * ARITHMETIC_AVERAGE (a23,I1,J2,I3,I1,J2-1,I3);
      }
      else    
      {
	realArray sh;
	sh=mgop.harmonic(s(I1,J2,I3),s(I1,J2-1,I3));

	a21(I1,J2,I3) = ARITHMETIC_AVERAGE (a21,I1,J2,I3,I1,J2-1,I3) *sh;
	a22(I1,J2,I3) = ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3) *sh;
	a23(I1,J2,I3) = ARITHMETIC_AVERAGE (a23,I1,J2,I3,I1,J2-1,I3) *sh;
      }   
      J2 = Range(I2.getBase()-1,I2.getBound()+1); // reset
      a21.reshape(1,I1,J2,I3);
      a22.reshape(1,I1,J2,I3);
      a23.reshape(1,I1,J2,I3);

      // Scale the matrix coefficients by the discretization widths
      a21 *= d_12;
      a22 *= d22(axis2);
      a23 *= d_23;
      

      // Vertex centered values of the transformation derivatives (ROW 2)
      J3 = Range(I3.getBase()  ,I3.getBound()+1);
      if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
      {
	a31(I1,I2,J3) = factor * ARITHMETIC_AVERAGE (a31,I1,I2,J3,I1,I2,J3-1);
	a32(I1,I2,J3) = factor * ARITHMETIC_AVERAGE (a32,I1,I2,J3,I1,I2,J3-1);
	a33(I1,I2,J3) = factor * ARITHMETIC_AVERAGE (a33,I1,I2,J3,I1,I2,J3-1);
      }
      else    
      {
	realArray sh;
	sh=mgop.harmonic(s(I1,I2,J3),s(I1,I2,J3-1));

	a31(I1,I2,J3) = ARITHMETIC_AVERAGE (a31,I1,I2,J3,I1,I2,J3-1) *sh;
	a32(I1,I2,J3) = ARITHMETIC_AVERAGE (a32,I1,I2,J3,I1,I2,J3-1) *sh;
	a33(I1,I2,J3) = ARITHMETIC_AVERAGE (a33,I1,I2,J3,I1,I2,J3-1) *sh;

      }
      J3 = Range(I3.getBase()-1,I3.getBound()+1);
      a31.reshape(1,I1,I2,J3);
      a32.reshape(1,I1,I2,J3);
      a33.reshape(1,I1,I2,J3);
      
      // Scale the matrix coefficients by the discretization widths
      a31 *= d_13;
      a32 *= d_12;
      a33 *= d22(axis3);
      
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

      derivative(MCE(0 ,+1,-1),I1,I2,I3) = -0.25*(a23(0,I1  ,I2+1,I3  ) + a32(0,I1  ,I2  ,I3  )) * jInverse;
      derivative(MCE(0 ,+1,+1),I1,I2,I3) =  0.25*(a23(0,I1  ,I2+1,I3  ) + a32(0,I1  ,I2  ,I3+1)) * jInverse;
      derivative(MCE(0 ,-1,+1),I1,I2,I3) = -0.25*(a23(0,I1  ,I2  ,I3  ) + a32(0,I1  ,I2  ,I3+1)) * jInverse;
      derivative(MCE(0 ,-1,-1),I1,I2,I3) =  0.25*(a23(0,I1  ,I2  ,I3  ) + a32(0,I1  ,I2  ,I3  )) * jInverse;

      derivative(MCE(+1, 0,-1),I1,I2,I3) = -0.25*(a13(0,I1+1,I2  ,I3  ) + a31(0,I1  ,I2  ,I3  )) * jInverse;
      derivative(MCE(+1, 0,+1),I1,I2,I3) =  0.25*(a13(0,I1+1,I2  ,I3  ) + a31(0,I1  ,I2  ,I3+1)) * jInverse;
      derivative(MCE(-1, 0,+1),I1,I2,I3) = -0.25*(a13(0,I1  ,I2  ,I3  ) + a31(0,I1  ,I2  ,I3+1)) * jInverse;
      derivative(MCE(-1, 0,-1),I1,I2,I3) =  0.25*(a13(0,I1  ,I2  ,I3  ) + a31(0,I1  ,I2  ,I3  )) * jInverse;

      derivative(MCE(+1,-1, 0),I1,I2,I3) = -0.25*(a12(0,I1+1,I2  ,I3  ) + a21(0,I1  ,I2  ,I3  )) * jInverse;
      derivative(MCE(+1,+1, 0),I1,I2,I3) =  0.25*(a12(0,I1+1,I2  ,I3  ) + a21(0,I1  ,I2+1,I3  )) * jInverse;
      derivative(MCE(-1,+1, 0),I1,I2,I3) = -0.25*(a12(0,I1  ,I2  ,I3  ) + a21(0,I1  ,I2+1,I3  )) * jInverse;
      derivative(MCE(-1,-1, 0),I1,I2,I3) =  0.25*(a12(0,I1  ,I2  ,I3  ) + a21(0,I1  ,I2  ,I3  )) * jInverse;
      
      derivative(MCE( 0, 0,-1),I1,I2,I3) = (a33(0,I1,I2,I3)+0.25*(a13(0,I1,I2,I3)-a13(0,I1+1,I2  ,I3  )+
                                            a23(0,I1,I2,I3)-a23(0,I1  ,I2+1,I3  ))) *jInverse;
 
      derivative(MCE( 0, 0,+1),I1,I2,I3) = (a33(0,I1  ,I2  ,I3+1)+0.25*(a13(0,I1+1,I2  ,I3  )-a13(0,I1,I2,I3)+
                                             a23(0,I1  ,I2+1,I3  )-a23(0,I1,I2,I3))) * jInverse;

      derivative(MCE( 0,-1, 0),I1,I2,I3) = (a22(0,I1,I2,I3)+0.25*(a32(0,I1,I2,I3)-a32(0,I1  ,I2  ,I3+1) +
                                            a12(0,I1,I2,I3)-a12(0,I1+1,I2  ,I3  )))*jInverse;
      
      derivative(MCE( 0,+1, 0),I1,I2,I3) = (a22(0,I1  ,I2+1,I3  )+0.25*(a32(0,I1  ,I2  ,I3+1)-a32(0,I1,I2,I3) +
                                            a12(0,I1+1,I2  ,I3  )-a12(0,I1,I2,I3)))*jInverse;
 
      derivative(MCE(-1, 0, 0),I1,I2,I3) = (a11(0,I1,I2,I3)+0.25*(a31(0,I1,I2,I3)-a31(0,I1  ,I2  ,I3+1) +
                                            a21(0,I1,I2,I3)-a21(0,I1  ,I2+1,I3  )))*jInverse;
      
      derivative(MCE(+1, 0, 0),I1,I2,I3) = (a11(0,I1+1,I2  ,I3  )+0.25*(a31(0,I1  ,I2  ,I3+1)-a31(0,I1,I2,I3) +
                                            a21(0,I1  ,I2+1,I3  )-a21(0,I1,I2,I3)))*jInverse;
      
      derivative(MCE( 0, 0, 0),I1,I2,I3) = -(  a33(0,I1,I2,I3)+  a33(0,I1  ,I2  ,I3+1) +
                                               a22(0,I1,I2,I3)+  a22(0,I1  ,I2+1,I3  ) +
                                               a11(0,I1,I2,I3)+  a11(0,I1+1,I2  ,I3  ))*jInverse;
     
      

//    loopBody2ndOrder3d(\
//    0,(a23(i1,i2,i3)+a32(i1,i2,i3))/jac(i1,i2,i3),0,(a13(i1,i2,i3)+a31(i1,i2,i3))/jac(i1,i2,i3), \
//    -(a13(i1+1,i2,i3)-a33(i1,i2,i3)-a13(i1,i2,i3)-a23(i1,i2,i3)+a23(i1,i2+1,i3))/jac(i1,i2,i3), \
//    -(a13(i1+1,i2,i3)+a31(i1,i2,i3))/jac(i1,i2,i3),0, \
//    -(a23(i1,i2+1,i3)+a32(i1,i2,i3))/jac(i1,i2,i3),0,(a12(i1,i2,i3)+a21(i1,i2,i3))/jac(i1,i2,i3), \
//    (-a32(i1,i2,i3+1)-a12(i1+1,i2,i3)+a32(i1,i2,i3)+a22(i1,i2,i3)+a12(i1,i2,i3))/jac(i1,i2,i3), \
//    -(a21(i1,i2,i3)+a12(i1+1,i2,i3))/jac(i1,i2,i3), \
//    (-a31(i1,i2,i3+1)+a11(i1,i2,i3)+a31(i1,i2,i3)-a21(i1,i2+1,i3)+a21(i1,i2,i3))/jac(i1,i2,i3), \
//    -(a11(i1+1,i2,i3)+a11(i1,i2,i3)+a22(i1,i2+1,i3)+a22(i1,i2,i3)+a33(i1,i2,i3+1)+a33(i1,i2,i3))/jac(i1,i2,i3), \
//    -(-a11(i1+1,i2,i3)+a21(i1,i2,i3)+a31(i1,i2,i3)-a21(i1,i2+1,i3)-a31(i1,i2,i3+1))/jac(i1,i2,i3), \
//    -(a21(i1,i2+1,i3)+a12(i1,i2,i3))/jac(i1,i2,i3), \
//    -(-a32(i1,i2,i3+1)-a12(i1+1,i2,i3)-a22(i1,i2+1,i3)+a12(i1,i2,i3)+a32(i1,i2,i3))/jac(i1,i2,i3), \
//    (a21(i1,i2+1,i3)+a12(i1+1,i2,i3))/jac(i1,i2,i3),0, \
//    -(a32(i1,i2,i3+1)+a23(i1,i2,i3))/jac(i1,i2,i3),0, \
//    -(a13(i1,i2,i3)+a31(i1,i2,i3+1))/jac(i1,i2,i3), \
//    -(-a13(i1+1,i2,i3)+a13(i1,i2,i3)-a33(i1,i2,i3+1)-a23(i1,i2+1,i3)+a23(i1,i2,i3))/jac(i1,i2,i3), \
//    (a13(i1+1,i2,i3)+a31(i1,i2,i3+1))/jac(i1,i2,i3),0, \
//    (a23(i1,i2+1,i3)+a32(i1,i2,i3+1))/jac(i1,i2,i3),0 )

  }
}


void 
derivativeScalarDerivativeFDerivCoefficients(RealDistributedArray & derivative,
                                             const realMappedGridFunction & s,
                                             const int & direction1,
                                             const int & direction2,
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
  if( mgop.isRectangular() && FALSE )
  { 
    if( orderOfAccuracy==2 )
    {
      derivativeScalarDerivativeFDerivCoefficients2(derivative,s,direction1,direction2,I1,I2,I3,E,C,mgop );

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
      derivativeScalarDerivativeFDerivCoefficients2(derivative,s,direction1,direction2,I1,I2,I3,E,C,mgop );
    }
    else   // ====== 4th order =======
    {
      Overture::abort("error");
    }
  }
  // time=getCPU()-time;
  // printf("derivativeScalarDerivativeFDerivCoefficients: time = %e \n",time);
  
  // now make copies for other components
  Index M(0,stencilSize);
  for( int c=C.getBase(); c<=C.getBound(); c++ )                        
  for( int e=E.getBase(); e<=E.getBound(); e++ )                        
    if( c!=c0 || e!=e0 )
      derivative(M+CE(c,e),I1,I2,I3)=derivative(M+CE(c0,e0),I1,I2,I3);
}


#undef CE
