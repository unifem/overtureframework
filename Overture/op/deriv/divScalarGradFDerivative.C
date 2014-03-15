//================================================================================
//
//            990609  : WDH
// MODIFIED:  April 15, 1999 WJW
//         : June 18, 2000 WDH - only use harmonic averaging on the scalar (not the jacobian elements)
//
// NOTES: 
//   Implements conservative difference approximation to
//      div (scalar grad)
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
#include "xD.h"

#undef ARITHMETIC_AVERAGE
#define ARITHMETIC_AVERAGE(s,I1,I2,I3,J1,J2,J3) (s(I1,I2,I3) + s(J1,J2,J3))

// undef HARMONIC_AVERAGE 
// define HARMONIC_AVERAGE(s,I1,I2,I3,J1,J2,J3) (s(I1,I2,I3)*s(J1,J2,J3)/(s(I1,I2,I3)+s(J1,J2,J3)+REAL_EPSILON))

void 
divScalarGradFDerivative43(const realMappedGridFunction & ugf,
			   const realMappedGridFunction & s,
			   RealDistributedArray & derivative,
			   const Index & I1,
			   const Index & I2,
			   const Index & I3,
			   const Index & N,
			   MappedGridOperators & mgop )
// 3d fourth order
{                                                                        
  printf("Sorry: divScalarGrad is not implemented yet for 3D, 4th order\n");
  Overture::abort("error");
}

void 
divScalarGradFDerivative42(const realMappedGridFunction & ugf,
			   const realMappedGridFunction & s,
			   RealDistributedArray & derivative,
			   const Index & I1,
			   const Index & I2,
			   const Index & I3,
			   const Index & N,
			   MappedGridOperators & mgop )
// 2d fourth order
{                                                                        
  printf("Sorry: divScalarGrad is not implemented yet for 2D, 4th order\n");
  Overture::abort("error");
    
}


void 
divScalarGradFDerivative23(const realMappedGridFunction & ugf,
			   const realMappedGridFunction & s,
			   RealDistributedArray & derivative,
			   const Index & I1,
			   const Index & I2,
			   const Index & I3,
			   const Index & N,
			   MappedGridOperators & mgop )
// 2nd-order, 3d
{                                                                        
    
//     d/d(xi) (s d/d(xj) ) =
//                             d/dr ( a11 d/dr   +   a12 d/ds   +   a13 d/dt ) +
//                             d/ds ( a21 d/dr   +   a22 d/ds   +   a23 d/dt ) +
//                             d/dt ( a31 d/dr   +   a32 d/ds   +   a33 d/dt )


  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();

  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency
    
  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();


  // Face value averaging factor ( = 0.5 for arithmetic,  2.0 for harmonic averaging)
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;
    
  // Cell Spacing
  RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
  RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;
    
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
    a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    a12(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a12,J1,I2,I3,J1-1,I2,I3);
    a13(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a13,J1,I2,I3,J1-1,I2,I3);
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

    realArray sh;
    sh=mgop.harmonic(s(J1,I2,I3),s(J1-1,I2,I3));
    a11(J1,I2,I3) = ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3) *sh;
    a12(J1,I2,I3) = ARITHMETIC_AVERAGE (a12,J1,I2,I3,J1-1,I2,I3) *sh;
    a13(J1,I2,I3) = ARITHMETIC_AVERAGE (a13,J1,I2,I3,J1-1,I2,I3) *sh;

  }
    
  // Estimate D{-r}(i-1/2,j,k)
  RealDistributedArray urr(J1,I2,I3,N);
  urr =u(J1,I2,I3,N)-u(J1-1,I2,I3,N);

  // Estimate D{-s}(i-1/2,j+1/2,k)
  RealDistributedArray urs(J1,I2,I3,N);    
  urs = (u(J1-1,I2+1,I3,N) + u(J1,I2+1,I3,N) - u(J1-1,I2-1,I3,N) - u(J1,I2-1,I3,N));

  // Estimate D{-t}(i-1/2,j,k+1/2)
  RealDistributedArray urt(J1,I2,I3,N);    
  urt = (u(J1-1,I2,I3+1,N) + u(J1,I2,I3+1,N) - u(J1-1,I2,I3-1,N) - u(J1,I2,I3-1,N));    


  // Get face-averaged values of the aijs
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

  // Estimate D{-r}(i+1/2,j-1/2,k)
  RealDistributedArray usr(I1,J2,I3,N);
  usr = (u(I1+1,J2-1,I3,N) + u(I1+1,J2,I3,N) - u(I1-1,J2-1,I3,N) - u(I1-1,J2,I3,N));
  
  // Estimate D{-s}(i,j-1/2,k)
  RealDistributedArray uss(I1,J2,I3,N);
  uss = u(I1,J2,I3,N)-u(I1,J2-1,I3,N);

  // Estimate D{-t}(i,j-1/2,k+1/2)
  RealDistributedArray ust(I1,J2,I3,N);
  ust = (u(I1,J2-1,I3+1,N) + u(I1,J2,I3+1,N) - u(I1,J2-1,I3-1,N) - u(I1,J2,I3-1,N));

    

  // Get face-averaged values of the aijs
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
    
  // Estimate D{-r}(i+1/2,j,k-1/2)
  RealDistributedArray utr(I1,I2,J3,N);
  utr = (u(I1+1,I2,J3-1,N) + u(I1+1,I2,J3,N) - u(I1-1,I2,J3-1,N) - u(I1-1,I2,J3,N));   
    
  // Estimate D{-s}(i,j+1/2,k-1/2)
  RealDistributedArray uts(I1,I2,J3,N);
  uts = (u(I1,I2+1,J3-1,N) + u(I1,I2+1,J3,N) - u(I1,I2-1,J3-1,N) - u(I1,I2-1,J3,N));   
    
  // Estimate D{-t}(i,j,k-1/2)
  RealDistributedArray utt(I1,I2,J3,N);
  utt =u(I1,I2,J3,N)-u(I1,I2,J3-1,N);   



  // The inverse of the jacobian
  const RealDistributedArray & jInverse = evaluate(1./j(I1,I2,I3));
    
  //NOTE:The spacing on the cross derivative terms is only 1/dx
    
  // Evaluate the derivative
  for( int n=N.getBase(); n<=N.getBound(); n++ )                        
  {
    derivative(I1,I2,I3,n)=
      (
	(a11(I1+1,I2  ,I3  )*urr(I1+1,I2  ,I3  ,n) - a11(I1,I2,I3)*urr(I1,I2,I3,n))*d22(axis1)+
	(a22(I1  ,I2+1,I3  )*uss(I1  ,I2+1,I3  ,n) - a22(I1,I2,I3)*uss(I1,I2,I3,n))*d22(axis2)+
	(a33(I1  ,I2  ,I3+1)*utt(I1  ,I2  ,I3+1,n) - a33(I1,I2,I3)*utt(I1,I2,I3,n))*d22(axis3)+
	(a21(I1  ,I2+1,I3  )*usr(I1  ,I2+1,I3  ,n) - a21(I1,I2,I3)*usr(I1,I2,I3,n) +
	 a12(I1+1,I2  ,I3  )*urs(I1+1,I2  ,I3  ,n) - a12(I1,I2,I3)*urs(I1,I2,I3,n))*(d12(axis1)*d12(axis2))+
	(a31(I1  ,I2  ,I3+1)*utr(I1  ,I2  ,I3+1,n) - a31(I1,I2,I3)*utr(I1,I2,I3,n) +
	 a13(I1+1,I2  ,I3  )*urt(I1+1,I2  ,I3  ,n) - a13(I1,I2,I3)*urt(I1,I2,I3,n))*(d12(axis1)*d12(axis3))+
	(a32(I1  ,I2  ,I3+1)*uts(I1  ,I2  ,I3+1,n) - a32(I1,I2,I3)*uts(I1,I2,I3,n) +
	 a23(I1  ,I2+1,I3  )*ust(I1  ,I2+1,I3  ,n) - a23(I1,I2,I3)*ust(I1,I2,I3,n))*(d12(axis2)*d12(axis3))
	)*jInverse;
  }
    
}


void 
divScalarGradFDerivative23R(const realMappedGridFunction & ugf,
			    const realMappedGridFunction & s,
			    RealDistributedArray & derivative,
			    const Index & I1,
			    const Index & I2,
			    const Index & I3,
			    const Index & N,
			    MappedGridOperators & mgop )
// 2nd-order, 3-D, Rectangular grid
{
  //printf("Sorry: divScalarGrad (2nd order, 3-D) is not yet optimized for uniform rectangular grids.\n");
  // divScalarGradFDerivative23(ugf,s,derivative,I1,I2,I3,N,mgop );

  // const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();

  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency
    
  // Face value averaging factor ( = 0.5 for arithmetic,  2.0 for harmonic averaging)
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;
    
  // Cell Spacing
  real h22c[3];
#define h22(n) h22c[n]
  for( int axis=0; axis<3; axis++ )
    h22(axis)=1./SQR(mgop.dx[axis]);
    
  Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
  Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
  Index J3 = Range(I3.getBase()-1,I3.getBound()+1);

  // Vertex centered values of the transformation derivatives (ROW 1)
  RealDistributedArray a11(J1,J2,J3), a22(J1,J2,J3), a33(J1,J2,J3);
    
  a11 = s(J1,J2,J3);
  a22 = s(J1,J2,J3);
  a33 = s(J1,J2,J3);
   
  // Get face-averaged values of the aijs
  J1 = Range(I1.getBase(),I1.getBound()+1);
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
  else    
    a11(J1,I2,I3) = factor * mgop.harmonic(a11(J1,I2,I3),a11(J1-1,I2,I3));
//  a11(J1,I2,I3) = factor * HARMONIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    
  // Estimate D{-r}(i-1/2,j,k)
  RealDistributedArray urr(J1,I2,I3,N);
  urr =u(J1,I2,I3,N)-u(J1-1,I2,I3,N);

  // Get face-averaged values of the aijs
  J2 = Range(I2.getBase()  ,I2.getBound()+1);
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    a22(I1,J2,I3) = factor * ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
  else    
    a22(I1,J2,I3) = factor * mgop.harmonic(a22(I1,J2,I3),a22(I1,J2-1,I3));
//    a22(I1,J2,I3) = factor * HARMONIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);

  // Estimate D{-s}(i,j-1/2,k)
  RealDistributedArray uss(I1,J2,I3,N);
  uss = u(I1,J2,I3,N)-u(I1,J2-1,I3,N);

  // Get face-averaged values of the aijs
  J3 = Range(I3.getBase()  ,I3.getBound()+1);
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    a33(I1,I2,J3) = factor * ARITHMETIC_AVERAGE (a33,I1,I2,J3,I1,I2,J3-1);
  else    
    a33(I1,I2,J3) = factor * mgop.harmonic(a33(I1,I2,J3),a33(I1,I2,J3-1));
//    a33(I1,I2,J3) = factor * HARMONIC_AVERAGE (a33,I1,I2,J3,I1,I2,J3-1);
    
  // Estimate D{-t}(i,j,k-1/2)
  RealDistributedArray utt(I1,I2,J3,N);
  utt =u(I1,I2,J3,N)-u(I1,I2,J3-1,N);   

  // Evaluate the derivative
  for( int n=N.getBase(); n<=N.getBound(); n++ )                        
  {
    derivative(I1,I2,I3,n)=
      (
	(a11(I1+1,I2  ,I3  )*urr(I1+1,I2  ,I3  ,n) - a11(I1,I2,I3)*urr(I1,I2,I3,n))*h22(axis1)+
	(a22(I1  ,I2+1,I3  )*uss(I1  ,I2+1,I3  ,n) - a22(I1,I2,I3)*uss(I1,I2,I3,n))*h22(axis2)+
	(a33(I1  ,I2  ,I3+1)*utt(I1  ,I2  ,I3+1,n) - a33(I1,I2,I3)*utt(I1,I2,I3,n))*h22(axis3));
  }
}

void 
divScalarGradFDerivative22(const realMappedGridFunction & ugf,
			   const realMappedGridFunction & s,
			   RealDistributedArray & derivative,
			   const Index & I1,
			   const Index & I2,
			   const Index & I3,
			   const Index & N,
			   MappedGridOperators & mgop )
// 2nd-order, 2d
{                                                                        
    
  // Averaging factor for face values ( = 0.5 for arithmetic averaging, 2.0 for harmonic averaging)
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;
   
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();


  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency
    
  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();
    
  // Cell Spacing
  RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
  RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;
    
  Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
  Index J2 = Range(I2.getBase()-1,I2.getBound()+1);


  const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();


  // Vertex centered values of the transformation derivatives
  RealDistributedArray a11(J1,J2,I3), a12(J1,J2,I3), a22(J1,J2,I3);
  realArray a21; 
    
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
  {
    RealDistributedArray sj;
    sj = s(J1,J2,I3) * j(J1,J2,I3);

    a11 = (RX(J1,J2,I3) * RX(J1,J2,I3) + RY(J1,J2,I3) * RY(J1,J2,I3))*sj;
    a22 = (SX(J1,J2,I3) * SX(J1,J2,I3) + SY(J1,J2,I3) * SY(J1,J2,I3))*sj;
    a12 = (RX(J1,J2,I3) * SX(J1,J2,I3) + RY(J1,J2,I3) * SY(J1,J2,I3))*sj;
    a21 = a12;

    J1 = Range(I1.getBase(),I1.getBound()+1);

    a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    a12(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a12,J1,I2,I3,J1-1,I2,I3);
  }
  else    
  {
    // only use harmonic average on the scalar since harmonic average doesn't work
    // well for arguments of opposite sign (the factor a12 may change sign)
    a11 = (RX(J1,J2,I3) * RX(J1,J2,I3) + RY(J1,J2,I3) * RY(J1,J2,I3))*j(J1,J2,I3);
    a22 = (SX(J1,J2,I3) * SX(J1,J2,I3) + SY(J1,J2,I3) * SY(J1,J2,I3))*j(J1,J2,I3);
    a12 = (RX(J1,J2,I3) * SX(J1,J2,I3) + RY(J1,J2,I3) * SY(J1,J2,I3))*j(J1,J2,I3);
    a21 = a12;

    J1 = Range(I1.getBase(),I1.getBound()+1);
    realArray sh = mgop.harmonic(s(J1,I2,I3),s(J1-1,I2,I3));
    a11(J1,I2,I3) = ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3)*sh;
    a12(J1,I2,I3) = ARITHMETIC_AVERAGE (a12,J1,I2,I3,J1-1,I2,I3)*sh;
  }

  // Estimate D{-r}(i-1/2,j)
  RealDistributedArray urr(J1,I2,I3,N);
  urr =u(J1,I2,I3,N)-u(J1-1,I2,I3,N);   // Delta_{-}

  // Estimate D{-s}(i-1/2,j+1/2)
  RealDistributedArray urs(J1,I2,I3,N);    
  urs = (u(J1-1,I2+1,I3,N) + u(J1,I2+1,I3,N) - u(J1-1,I2-1,I3,N) - u(J1,I2-1,I3,N));


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
    
  // Estimate D{-s}(i,j-1/2)
  RealDistributedArray uss(I1,J2,I3,N);
  uss = u(I1,J2,I3,N)-u(I1,J2-1,I3,N);   // Delta_{-}

  // Estimate D{-r}(i+1/2,j-1/2)
  RealDistributedArray usr(I1,J2,I3,N);
  usr = (u(I1+1,J2-1,I3,N) + u(I1+1,J2,I3,N) - u(I1-1,J2-1,I3,N) - u(I1-1,J2,I3,N));
    
  // The inverse jacobian
  const RealDistributedArray & jInverse = evaluate(1./j(I1,I2,I3));

  // The spacing on the cross derivative terms is only 1/dx
    
  // Calculate the derivative
  for( int n=N.getBase(); n<=N.getBound(); n++ )                        
  {
    derivative(I1,I2,I3,n)=
      ((a11(I1+1,I2  ,I3)*urr(I1+1,I2  ,I3,n) - a11(I1,I2,I3)*urr(I1,I2,I3,n))*d22(axis1)+
       (a22(I1  ,I2+1,I3)*uss(I1  ,I2+1,I3,n) - a22(I1,I2,I3)*uss(I1,I2,I3,n))*d22(axis2)+
       (a21(I1  ,I2+1,I3)*usr(I1  ,I2+1,I3,n) - a21(I1,I2,I3)*usr(I1,I2,I3,n) +
	a12(I1+1,I2  ,I3)*urs(I1+1,I2  ,I3,n) - a12(I1,I2,I3)*urs(I1,I2,I3,n))*
       (d12(axis1)*d12(axis2)))*jInverse;
  }
    
}


void 
divScalarGradFDerivative22R(const realMappedGridFunction & ugf,
			    const realMappedGridFunction & s,
			    RealDistributedArray & derivative,
			    const Index & I1,
			    const Index & I2,
			    const Index & I3,
			    const Index & N,
			    MappedGridOperators & mgop )
// 2nd-order, 2-D, Rectangular grid
{
  //printf("Sorry: divScalarGrad (2nd order, 2-D) is not yet optimized for uniform rectangular grids.\n");
  // divScalarGradFDerivative22(ugf,s,derivative,I1,I2,I3,N,mgop );
  // Averaging factor for face values ( = 0.5 for arithmetic averaging, 2.0 for harmonic averaging)
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;
   
  // const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();


  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency
    
  // Cell Spacing
  real h22c[3];
#define h22(n) h22c[n]
  for( int axis=0; axis<3; axis++ )
    h22(axis)=1./SQR(mgop.dx[axis]);
    
  Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
  Index J2 = Range(I2.getBase()-1,I2.getBound()+1);

  // Vertex centered values of the transformation derivatives
  RealDistributedArray a11(J1,J2,I3), a22(J1,J2,I3);
    
  a11 = s(J1,J2,I3);
  a22 = s(J1,J2,I3);

  J1 = Range(I1.getBase(),I1.getBound()+1);
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
  {
    a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
  }
  else    
  {
//    a11(J1,I2,I3) = factor * HARMONIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    a11(J1,I2,I3) = factor * mgop.harmonic(a11(J1,I2,I3),a11(J1-1,I2,I3));
  }

  // Estimate D{-r}(i-1/2,j)
  RealDistributedArray urr(J1,I2,I3,N);
  urr =u(J1,I2,I3,N)-u(J1-1,I2,I3,N);   // Delta_{-}

  J2 = Range(I2.getBase()  ,I2.getBound()+1);
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
  {
    a22(I1,J2,I3) = factor * ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
  }
  else   
  {
//    a22(I1,J2,I3) = factor * HARMONIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
    a22(I1,J2,I3) = factor * mgop.harmonic(a22(I1,J2,I3),a22(I1,J2-1,I3));
  }
    
  // Estimate D{-s}(i,j-1/2)
  RealDistributedArray uss(I1,J2,I3,N);
  uss = u(I1,J2,I3,N)-u(I1,J2-1,I3,N);   // Delta_{-}

  // The spacing on the cross derivative terms is only 1/dx
    
  // Calculate the derivative
  for( int n=N.getBase(); n<=N.getBound(); n++ )                        
  {
    derivative(I1,I2,I3,n)=
      ((a11(I1+1,I2  ,I3)*urr(I1+1,I2  ,I3,n) - a11(I1,I2,I3)*urr(I1,I2,I3,n))*h22(axis1)+
       (a22(I1  ,I2+1,I3)*uss(I1  ,I2+1,I3,n) - a22(I1,I2,I3)*uss(I1,I2,I3,n))*h22(axis2));
  }
}

void 
divScalarGradFDerivative21(const realMappedGridFunction & ugf,
			   const realMappedGridFunction & s,
			   RealDistributedArray & derivative,
			   const Index & I1,
			   const Index & I2,
			   const Index & I3,
			   const Index & N,
			   MappedGridOperators & mgop )
// 2nd-order, 1d
{                                                                        
    
  // Averaging factor for face values ( = 0.5 for arithmetic averaging, 2.0 for harmonic averaging)
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;

  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency
    
  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();
    
  RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;
 
  Index J1 = Range(I1.getBase()-1,I1.getBound()+1);

  RealDistributedArray sj(J1,I2,I3);
  const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();

  sj = s(J1,I2,I3) * j(J1,I2,I3);

  RealDistributedArray a11(J1,I2,I3);

  a11 = RX(J1,I2,I3) * RX(J1,I2,I3);
  a11(J1,I2,I3) *= sj(J1,I2,I3);

  J1 = Range(I1.getBase(),I1.getBound()+1);
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
  {
    a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
  }
  else
  {
//    a11(J1,I2,I3) = factor * HARMONIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    a11(J1,I2,I3) = factor * mgop.harmonic(a11(J1,I2,I3),a11(J1-1,I2,I3));
  }

  // Estimate D{-r}(i-1/2,j)
  RealDistributedArray urr(J1,I2,I3,N);
  urr =u(J1,I2,I3,N)-u(J1-1,I2,I3,N);   // Delta_{-}


  // The inverse jacobian
  const RealDistributedArray & jInverse = evaluate(1./j(I1,I2,I3));

  for( int n=N.getBase(); n<=N.getBound(); n++ )                        
  {
    derivative(I1,I2,I3,n)=
      ((a11(I1+1,I2,I3)*urr(I1+1,I2,I3,n)-a11(I1,I2,I3)*urr(I1,I2,I3,n))*d22(axis1))*jInverse;
  }

}

void 
divScalarGradFDerivative21R(const realMappedGridFunction & ugf,
			    const realMappedGridFunction & s,
			    RealDistributedArray & derivative,
			    const Index & I1,
			    const Index & I2,
			    const Index & I3,
			    const Index & N,
			    MappedGridOperators & mgop )
// 2nd-order, 1-D, Rectangular grid
{                                                                        
  // divScalarGradFDerivative21(ugf,s,derivative,I1,I2,I3,N,mgop );
  // Averaging factor for face values ( = 0.5 for arithmetic averaging, 2.0 for harmonic averaging)
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;

  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency
    
  real h22c[3];
#define h22(n) h22c[n]
  for( int axis=0; axis<3; axis++ )
    h22(axis)=1./SQR(mgop.dx[axis]);
 
  Index J1 = Range(I1.getBase()-1,I1.getBound()+1);

  RealDistributedArray a11(J1,I2,I3);
  a11(J1,I2,I3) = s(J1,I2,I3);

  J1 = Range(I1.getBase(),I1.getBound()+1);
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
  {
    a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
  }
  else
  {
//    a11(J1,I2,I3) = factor * HARMONIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    a11(J1,I2,I3) = factor * mgop.harmonic(a11(J1,I2,I3),a11(J1-1,I2,I3));
  }

  // Estimate D{-r}(i-1/2,j)
  RealDistributedArray urr(J1,I2,I3,N);
  urr =u(J1,I2,I3,N)-u(J1-1,I2,I3,N);   // Delta_{-}


  for( int n=N.getBase(); n<=N.getBound(); n++ )                        
  {
    derivative(I1,I2,I3,n)=
      ((a11(I1+1,I2,I3)*urr(I1+1,I2,I3,n)-a11(I1,I2,I3)*urr(I1,I2,I3,n))*h22(axis1));
  }
}

void 
divScalarGradFDerivative(const realMappedGridFunction & ugf,
			 const realMappedGridFunction & s,
			 RealDistributedArray & derivative,
			 const Index & I1,
			 const Index & I2,
			 const Index & I3,
			 const Index & N,
			 MappedGridOperators & mgop )
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  int & orderOfAccuracy = mgop.orderOfAccuracy;

    
  derivative = 0.0;
 

  if( mgop.isRectangular() )
  { // The grid is rectangular
    if( orderOfAccuracy==2 )
    {


      if( numberOfDimensions==1 )
      {
	divScalarGradFDerivative21R(ugf,s,derivative,I1,I2,I3,N,mgop );  // 1-D Rect
      }
      else if(numberOfDimensions==2 )
      {   
	divScalarGradFDerivative22R(ugf,s,derivative,I1,I2,I3,N,mgop );  // 2-D Rect
      }
      else 
      {   
	divScalarGradFDerivative23R(ugf,s,derivative,I1,I2,I3,N,mgop );  // 3-D Rect
      }
    }
        
    else   // ====== 4th order =======
    {
      printf("divScalarGradFDerivative: 4th order: this case should not occur\n");
      Overture::abort("error");
    }
  }
  else 
  { // Ths grid is not rectangular
    RealDistributedArray & inverseVertexDerivative = 
      int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
      : mgop.mappedGrid.inverseCenterDerivative();
        
    if( orderOfAccuracy==2 )
    {
      if( numberOfDimensions==1 )
	divScalarGradFDerivative21(ugf,s,derivative,I1,I2,I3,N,mgop );
      else if(numberOfDimensions==2 )
	divScalarGradFDerivative22(ugf,s,derivative,I1,I2,I3,N,mgop );
      else // ======= 3D ================
	divScalarGradFDerivative23(ugf,s,derivative,I1,I2,I3,N,mgop );
    }
    else   // ====== 4th order =======
    {
      printf("divScalarGradFDerivative: 4th order: this case should not occur\n");
      Overture::abort("error");
    }
  }
}


#undef ARITHMETIC_AVERAGE
#undef RXI
#undef SXI
#undef TXI
