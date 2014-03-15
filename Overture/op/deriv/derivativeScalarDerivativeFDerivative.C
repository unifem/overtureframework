//================================================================================
//
// LAST MODIFIED:  April 15, 1999
//
// NOTES: 
//   Implements conservative difference approximation to
//      d/dx_i (scalar d/dx_j)
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
#include "xD.h"

#undef ARITHMETIC_AVERAGE
#define ARITHMETIC_AVERAGE(s,I1,I2,I3,J1,J2,J3) (s(I1,I2,I3) + s(J1,J2,J3))

// undef HARMONIC_AVERAGE 
// define HARMONIC_AVERAGE(s,I1,I2,I3,J1,J2,J3) (s(I1,I2,I3)*s(J1,J2,J3)/(s(I1,I2,I3)+s(J1,J2,J3)+REAL_EPSILON))

#undef RXI
#undef SXI
#undef TXI
#define RXI(I1,I2,I3,dir)  (inverseVertexDerivative(I1,I2,I3, numberOfDimensions *(dir) + 0))
#define SXI(I1,I2,I3,dir)  (inverseVertexDerivative(I1,I2,I3, numberOfDimensions *(dir) + 1))
#define TXI(I1,I2,I3,dir)  (inverseVertexDerivative(I1,I2,I3, numberOfDimensions *(dir) + 2))

void 
derivativeScalarDerivativeFDerivative43(const realMappedGridFunction & ugf,
                                        const realMappedGridFunction & s,
                                        RealDistributedArray & derivative,
                                        const int & direction1,
                                        const int & direction2,
                                        const Index & I1,
                                        const Index & I2,
                                        const Index & I3,
                                        const Index & N,
                                        MappedGridOperators & mgop )
// 3d fourth order
{                                                                        
    printf("Sorry: derivativeScalarDerivative is not implemented yet for 3D, 4th order\n");
    Overture::abort("error");
}

void 
derivativeScalarDerivativeFDerivative42(const realMappedGridFunction & ugf,
                                        const realMappedGridFunction & s,
                                        RealDistributedArray & derivative,
                                        const int & direction1,
                                        const int & direction2,
                                        const Index & I1,
                                        const Index & I2,
                                        const Index & I3,
                                        const Index & N,
                                        MappedGridOperators & mgop )
// 2d fourth order
{                                                                        
    printf("Sorry: derivativeScalarDerivative is not implemented yet for 2D, 4th order\n");
    Overture::abort("error");
    
}


void 
derivativeScalarDerivativeFDerivative23(const realMappedGridFunction & ugf,
                                          const realMappedGridFunction & s,
                                          RealDistributedArray & derivative,
                                          const int & direction1,
                                          const int & direction2,
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

  if ( direction1 > 2 || direction2 > 2)
  {
    cout << "direction1 = " << direction1 << endl;
    cout << "direction2 = " << direction2 << endl;
    cout << "You gave a coordinate an invalid coordinate direction" << endl;
    Overture::abort("Error");
  }
    
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();

  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency
    
  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();


    // Face value averaging factor ( = 0.5 for arithmetic,  2.0 for harmonic averaging)
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;

    
  // Cell Spacing
  RealArray d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
  RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;
    
  Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
  Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
  Index J3 = Range(I3.getBase()-1,I3.getBound()+1);

  RealDistributedArray a11(J1,I2,I3), a12(J1,I2,I3), a13(J1,I2,I3),
    a21(I1,J2,I3), a22(I1,J2,I3), a23(I1,J2,I3), 
    a33(I1,I2,J3), a31(I1,I2,J3), a32(I1,I2,J3);
    
  const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();
  
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

  // Estimate D{-r}(i-1/2,j,k)
  RealDistributedArray urr(J1,I2,I3,N);
  urr =u(J1,I2,I3,N)-u(J1-1,I2,I3,N);

    // Estimate D{-s}(i-1/2,j+1/2,k)
  RealDistributedArray urs(J1,I2,I3,N);    
  urs = 0.25 * (u(J1-1,I2+1,I3,N) + u(J1,I2+1,I3,N) - u(J1-1,I2-1,I3,N) - u(J1,I2-1,I3,N));

    // Estimate D{-t}(i-1/2,j,k+1/2)
  RealDistributedArray urt(J1,I2,I3,N);    
  urt = 0.25 * (u(J1-1,I2,I3+1,N) + u(J1,I2,I3+1,N) - u(J1-1,I2,I3-1,N) - u(J1,I2,I3-1,N));    

  // Vertex centered values of the transformation derivatives (ROW 2)
    
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
  usr = 0.25 * (u(I1+1,J2-1,I3,N) + u(I1+1,J2,I3,N) - u(I1-1,J2-1,I3,N) - u(I1-1,J2,I3,N));
  
    // Estimate D{-s}(i,j-1/2,k)
  RealDistributedArray uss(I1,J2,I3,N);
  uss = u(I1,J2,I3,N)-u(I1,J2-1,I3,N);

    // Estimate D{-t}(i,j-1/2,k+1/2)
  RealDistributedArray ust(I1,J2,I3,N);
  ust = 0.25 * (u(I1,J2-1,I3+1,N) + u(I1,J2,I3+1,N) - u(I1,J2-1,I3-1,N) - u(I1,J2,I3-1,N));

  // Vertex centered values of the transformation derivatives (ROW 3)
    
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
  utr = 0.25 * (u(I1+1,I2,J3-1,N) + u(I1+1,I2,J3,N) - u(I1-1,I2,J3-1,N) - u(I1-1,I2,J3,N));   
    
    // Estimate D{-s}(i,j+1/2,k-1/2)
  RealDistributedArray uts(I1,I2,J3,N);
  uts = 0.25 * (u(I1,I2+1,J3-1,N) + u(I1,I2+1,J3,N) - u(I1,I2-1,J3-1,N) - u(I1,I2-1,J3,N));   
    
    // Estimate D{-t}(i,j,k-1/2)
  RealDistributedArray utt(I1,I2,J3,N);
  utt =u(I1,I2,J3,N)-u(I1,I2,J3-1,N);   

    // The inverse of the jacobian
  const RealDistributedArray & jInverse = evaluate(1./j(I1,I2,I3));
    
  //NOTE:The spacing on the cross derivative terms is only 1/dx
  d12 *= 2.0;
    
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
    
  // d12 /= 2.0;
    
}


void 
derivativeScalarDerivativeFDerivative23R(const realMappedGridFunction & ugf,
                              const realMappedGridFunction & s,
                              RealDistributedArray & derivative,
                              const int & direction1,
                              const int & direction2,
                              const Index & I1,
                              const Index & I2,
                              const Index & I3,
                              const Index & N,
                              MappedGridOperators & mgop )
// 2nd-order, 3-D, Rectangular grid
{
  // derivativeScalarDerivativeFDerivative23(ugf,s,derivative,direction1,direction2,I1,I2,I3,N,mgop );
    
//     d/d(xi) (s d/d(xj) ) =
//                             d/dr ( a11 d/dr   +   a12 d/ds   +   a13 d/dt ) +
//                             d/ds ( a21 d/dr   +   a22 d/ds   +   a23 d/dt ) +
//                             d/dt ( a31 d/dr   +   a32 d/ds   +   a33 d/dt )

  if ( direction1 > 2 || direction2 > 2)
  {
    cout << "direction1 = " << direction1 << endl;
    cout << "direction2 = " << direction2 << endl;
    cout << "You gave a coordinate an invalid coordinate direction" << endl;
    Overture::abort("Error");
  }
    
  // const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();

  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency
    
    // Face value averaging factor ( = 0.5 for arithmetic,  2.0 for harmonic averaging)
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;
    
  // Cell Spacing
  real h21c[3],h22c[3];
#define h21(n) h21c[n]
#define h22(n) h22c[n]
  int axis;
  for( axis=0; axis<3; axis++ )
  {
    h21(axis)=1./(2.*mgop.dx[axis]); 
    h22(axis)=1./SQR(mgop.dx[axis]);
  }
    
  Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
  Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
  Index J3 = Range(I3.getBase()-1,I3.getBound()+1);


  const real rx1d1 = direction1==0 ? 1. : 0.;
  const real rx1d2 = direction2==0 ? 1. : 0.;
  const real rx2d1 = direction1==1 ? 1. : 0.;
  const real rx2d2 = direction2==1 ? 1. : 0.;
  const real rx3d1 = direction1==2 ? 1. : 0.;
  const real rx3d2 = direction2==2 ? 1. : 0.;
  

  // Vertex centered values of the transformation derivatives (ROW 1)
  RealDistributedArray a11(J1,I2,I3), a12(J1,I2,I3), a13(J1,I2,I3);
    
  a11 = (rx1d1*rx1d2)*s(J1,I2,I3); 
  a12 = (rx1d1*rx2d2)*s(J1,I2,I3);
  a13 = (rx1d1*rx3d2)*s(J1,I2,I3);

    // Get face-averaged values of the aijs
  J1 = Range(I1.getBase(),I1.getBound()+1);
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
  {
    a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    a12(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a12,J1,I2,I3,J1-1,I2,I3);
    a13(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a13,J1,I2,I3,J1-1,I2,I3);
  }
  else    
  {
//     a11(J1,I2,I3) = factor * HARMONIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
//     a12(J1,I2,I3) = factor * HARMONIC_AVERAGE (a12,J1,I2,I3,J1-1,I2,I3);
//     a13(J1,I2,I3) = factor * HARMONIC_AVERAGE (a13,J1,I2,I3,J1-1,I2,I3);
    a11(J1,I2,I3) = factor * mgop.harmonic(a11(J1,I2,I3),a11(J1-1,I2,I3));
    a12(J1,I2,I3) = factor * mgop.harmonic(a12(J1,I2,I3),a12(J1-1,I2,I3));
    a13(J1,I2,I3) = factor * mgop.harmonic(a13(J1,I2,I3),a13(J1-1,I2,I3));
  }
    
  // Estimate D{-r}(i-1/2,j,k)
  RealDistributedArray urr(J1,I2,I3,N);
  urr =u(J1,I2,I3,N)-u(J1-1,I2,I3,N);

    // Estimate D{-s}(i-1/2,j+1/2,k)
  RealDistributedArray urs(J1,I2,I3,N);    
  urs = 0.25 * (u(J1-1,I2+1,I3,N) + u(J1,I2+1,I3,N) - u(J1-1,I2-1,I3,N) - u(J1,I2-1,I3,N));

    // Estimate D{-t}(i-1/2,j,k+1/2)
  RealDistributedArray urt(J1,I2,I3,N);    
  urt = 0.25 * (u(J1-1,I2,I3+1,N) + u(J1,I2,I3+1,N) - u(J1-1,I2,I3-1,N) - u(J1,I2,I3-1,N));    



    // Vertex centered values of the transformation derivatives (ROW 2)
  RealDistributedArray a21(I1,J2,I3), a22(I1,J2,I3), a23(I1,J2,I3);
    
  a21 = (rx2d1*rx1d2)*s(I1,J2,I3); 
  a22 = (rx2d1*rx2d2)*s(I1,J2,I3);
  a23 = (rx2d1*rx3d2)*s(I1,J2,I3);
    
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
//     a21(I1,J2,I3) = factor * HARMONIC_AVERAGE (a21,I1,J2,I3,I1,J2-1,I3);
//     a22(I1,J2,I3) = factor * HARMONIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
//     a23(I1,J2,I3) = factor * HARMONIC_AVERAGE (a23,I1,J2,I3,I1,J2-1,I3);
    a21(I1,J2,I3) = factor * mgop.harmonic(a21(I1,J2,I3),a21(I1,J2-1,I3));
    a22(I1,J2,I3) = factor * mgop.harmonic(a22(I1,J2,I3),a22(I1,J2-1,I3));
    a23(I1,J2,I3) = factor * mgop.harmonic(a23(I1,J2,I3),a23(I1,J2-1,I3));
  }   

  // Estimate D{-r}(i+1/2,j-1/2,k)
  RealDistributedArray usr(I1,J2,I3,N);
  usr = 0.25 * (u(I1+1,J2-1,I3,N) + u(I1+1,J2,I3,N) - u(I1-1,J2-1,I3,N) - u(I1-1,J2,I3,N));
  
    // Estimate D{-s}(i,j-1/2,k)
  RealDistributedArray uss(I1,J2,I3,N);
  uss = u(I1,J2,I3,N)-u(I1,J2-1,I3,N);

    // Estimate D{-t}(i,j-1/2,k+1/2)
  RealDistributedArray ust(I1,J2,I3,N);
  ust = 0.25 * (u(I1,J2-1,I3+1,N) + u(I1,J2,I3+1,N) - u(I1,J2-1,I3-1,N) - u(I1,J2,I3-1,N));

  // Vertex centered values of the transformation derivatives (ROW 3)
  RealDistributedArray a31(I1,I2,J3), a32(I1,I2,J3), a33(I1,I2,J3);
    
  a31 = (rx3d1*rx1d2)*s(I1,I2,J3); 
  a32 = (rx3d1*rx2d2)*s(I1,I2,J3);
  a33 = (rx3d1*rx3d2)*s(I1,I2,J3);
    
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
//     a31(I1,I2,J3) = factor * HARMONIC_AVERAGE (a31,I1,I2,J3,I1,I2,J3-1);
//     a32(I1,I2,J3) = factor * HARMONIC_AVERAGE (a32,I1,I2,J3,I1,I2,J3-1);
//     a33(I1,I2,J3) = factor * HARMONIC_AVERAGE (a33,I1,I2,J3,I1,I2,J3-1);
    a31(I1,I2,J3) = factor * mgop.harmonic(a31(I1,I2,J3),a31(I1,I2,J3-1));
    a32(I1,I2,J3) = factor * mgop.harmonic(a32(I1,I2,J3),a32(I1,I2,J3-1));
    a33(I1,I2,J3) = factor * mgop.harmonic(a33(I1,I2,J3),a33(I1,I2,J3-1));
  }
    
  // Estimate D{-r}(i+1/2,j,k-1/2)
  RealDistributedArray utr(I1,I2,J3,N);
  utr = 0.25 * (u(I1+1,I2,J3-1,N) + u(I1+1,I2,J3,N) - u(I1-1,I2,J3-1,N) - u(I1-1,I2,J3,N));   
    
    // Estimate D{-s}(i,j+1/2,k-1/2)
  RealDistributedArray uts(I1,I2,J3,N);
  uts = 0.25 * (u(I1,I2+1,J3-1,N) + u(I1,I2+1,J3,N) - u(I1,I2-1,J3-1,N) - u(I1,I2-1,J3,N));   
    
    // Estimate D{-t}(i,j,k-1/2)
  RealDistributedArray utt(I1,I2,J3,N);
  utt =u(I1,I2,J3,N)-u(I1,I2,J3-1,N);   

  //NOTE:The spacing on the cross derivative terms is only 1/dx
  for( axis=0; axis<3; axis++ )
    h21(axis)*= 2.0;
//  h21 *= 2.0;
    
  // Evaluate the derivative
  for( int n=N.getBase(); n<=N.getBound(); n++ )                        
  {
    derivative(I1,I2,I3,n)=
      (
	(a11(I1+1,I2  ,I3  )*urr(I1+1,I2  ,I3  ,n) - a11(I1,I2,I3)*urr(I1,I2,I3,n))*h22(axis1)+
	(a22(I1  ,I2+1,I3  )*uss(I1  ,I2+1,I3  ,n) - a22(I1,I2,I3)*uss(I1,I2,I3,n))*h22(axis2)+
	(a33(I1  ,I2  ,I3+1)*utt(I1  ,I2  ,I3+1,n) - a33(I1,I2,I3)*utt(I1,I2,I3,n))*h22(axis3)+
	(a21(I1  ,I2+1,I3  )*usr(I1  ,I2+1,I3  ,n) - a21(I1,I2,I3)*usr(I1,I2,I3,n) +
	 a12(I1+1,I2  ,I3  )*urs(I1+1,I2  ,I3  ,n) - a12(I1,I2,I3)*urs(I1,I2,I3,n))*(h21(axis1)*h21(axis2))+
	(a31(I1  ,I2  ,I3+1)*utr(I1  ,I2  ,I3+1,n) - a31(I1,I2,I3)*utr(I1,I2,I3,n) +
	 a13(I1+1,I2  ,I3  )*urt(I1+1,I2  ,I3  ,n) - a13(I1,I2,I3)*urt(I1,I2,I3,n))*(h21(axis1)*h21(axis3))+
	(a32(I1  ,I2  ,I3+1)*uts(I1  ,I2  ,I3+1,n) - a32(I1,I2,I3)*uts(I1,I2,I3,n) +
	 a23(I1  ,I2+1,I3  )*ust(I1  ,I2+1,I3  ,n) - a23(I1,I2,I3)*ust(I1,I2,I3,n))*(h21(axis2)*h21(axis3)) );
  }
    
//  h21 /= 2.0;

    
}

void 
derivativeScalarDerivativeFDerivative22(const realMappedGridFunction & ugf,
                               const realMappedGridFunction & s,
                               RealDistributedArray & derivative,
                               const int & direction1,
                               const int & direction2,
                               const Index & I1,
                               const Index & I2,
                               const Index & I3,
                               const Index & N,
                               MappedGridOperators & mgop )
// 2nd-order, 2d
{                                                                        
    
  // Averaging factor for face values ( = 0.5 for arithmetic averaging, 2.0 for harmonic averaging)
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;
   
  //cout << "In derivSderiv22" << endl;
  //cout << "direction 1 = " << direction1 << endl;
  //cout << "direction 2 = " << direction2 << endl;
 
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();

  // Dont allow  d/dz (s d/dxi) or d/dxi (s d/dz) since this is 2-D
  if ( direction1 > 1 || direction2 > 1) 
  {
    cout << "direction1 = " << direction1 << endl;
    cout << "direction2 = " << direction2 << endl;
    cout << "You may not differentiate wrt. the 3rd Coordinate in 2-D" << endl;
    Overture::abort("Error");
  }
    

  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency
    
  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();
    
    // Cell Spacing
//   RealArray d12 = mgop.d12;  // 1/ (2 dx)
//   RealArray d22; d22 = mgop.d22;  // 1/ (dx*dx)
  RealArray d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
  RealArray d22; d22 = 1./SQR(mgop.mappedGrid.gridSpacing()); // mgop.d22;
    
  Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
  Index J2 = Range(I2.getBase()-1,I2.getBound()+1);


    // s.display("Here is the scalar passed into derivScalarDeriv");


  const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();

  //sj.display("Here is sj");

  // Vertex centered values of the transformation derivatives (ROW 1)
  RealDistributedArray a11(J1,I2,I3), a12(J1,I2,I3);
  RealDistributedArray a21(I1,J2,I3), a22(I1,J2,I3);
    

  //a11.display("Here is a11 in DERIVATIVE before");
  //a12.display("Here is a12 in DERIVATIVE before");

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


  //a11.display("Here is a11 in DERIVATIVE");
  //a12.display("Here is a12 in DERIVATIVE");
    
  // Estimate D{-r}(i-1/2,j)
  RealDistributedArray urr(J1,I2,I3,N);
  urr =u(J1,I2,I3,N)-u(J1-1,I2,I3,N);   // Delta_{-}

    // Estimate D{-s}(i-1/2,j+1/2)
  RealDistributedArray urs(J1,I2,I3,N);    
  urs = 0.25 * (u(J1-1,I2+1,I3,N) + u(J1,I2+1,I3,N) - u(J1-1,I2-1,I3,N) - u(J1,I2-1,I3,N));

    // Vertex centered values of the transformation derivatives (ROW 2)

  //a21.display("Here is a21 in DERIVATIVE before");
  //a22.display("Here is a22 in DERIVATIVE before");
    
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


  //a21.display("Here is a21 in DERIVATIVE");
  //a22.display("Here is a22 in DERIVATIVE");
    
  // Estimate D{-s}(i,j-1/2)
  RealDistributedArray uss(I1,J2,I3,N);
  uss = u(I1,J2,I3,N)-u(I1,J2-1,I3,N);   // Delta_{-}

    // Estimate D{-r}(i+1/2,j-1/2)
  RealDistributedArray usr(I1,J2,I3,N);
  usr = 0.25 * (u(I1+1,J2-1,I3,N) + u(I1+1,J2,I3,N) - u(I1-1,J2-1,I3,N) - u(I1-1,J2,I3,N));
    
    // The inverse jacobian
  const RealDistributedArray & jInverse = evaluate(1./j(I1,I2,I3));

  // The spacing on the cross derivative terms is only 1/dx
  d12 *= 2.0;
    
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
    
//  d12 /= 2.0;
    
}


void 
derivativeScalarDerivativeFDerivative22R(const realMappedGridFunction & ugf,
                              const realMappedGridFunction & s,
                              RealDistributedArray & derivative,
                              const int & direction1,
                              const int & direction2,
                              const Index & I1,
                              const Index & I2,
                              const Index & I3,
                              const Index & N,
                              MappedGridOperators & mgop )
// 2nd-order, 2-D, Rectangular grid
{
  // derivativeScalarDerivativeFDerivative22(ugf,s,derivative,direction1,direction2,I1,I2,I3,N,mgop );
    
  // Averaging factor for face values ( = 0.5 for arithmetic averaging, 2.0 for harmonic averaging)
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;
   
 
  // const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();

  // Dont allow  d/dz (s d/dxi) or d/dxi (s d/dz) since this is 2-D
  if ( direction1 > 1 || direction2 > 1) 
  {
    cout << "direction1 = " << direction1 << endl;
    cout << "direction2 = " << direction2 << endl;
    cout << "You may not differentiate wrt. the 3rd Coordinate in 2-D" << endl;
    Overture::abort("Error");
  }
    

  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency
    
    // Cell Spacing
  real h21c[3],h22c[3];
#define h21(n) h21c[n]
#define h22(n) h22c[n]
  int axis;
  for( axis=0; axis<3; axis++ )
  {
    h21(axis)=1./(2.*mgop.dx[axis]); 
    h22(axis)=1./SQR(mgop.dx[axis]);
  }
    
  Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
  Index J2 = Range(I2.getBase()-1,I2.getBound()+1);


  // Vertex centered values of the transformation derivatives (ROW 1)
  RealDistributedArray a11(J1,I2,I3), a12(J1,I2,I3);
    
  const real rx1d1 = direction1==0 ? 1. : 0.;
  const real rx1d2 = direction2==0 ? 1. : 0.;
  const real rx2d1 = direction1==1 ? 1. : 0.;
  const real rx2d2 = direction2==1 ? 1. : 0.;
  

  a11 = (rx1d1*rx1d2)*s(J1,I2,I3);
  a12 = (rx1d1*rx2d2)*s(J1,I2,I3);

  //a11.display("Here is a11 in DERIVATIVE before");
  //a12.display("Here is a12 in DERIVATIVE before");

  J1 = Range(I1.getBase(),I1.getBound()+1);
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
  {
    a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    a12(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a12,J1,I2,I3,J1-1,I2,I3);
  }
  else    
  {
    //cout << "Using harmonic average" << endl;
//     a11(J1,I2,I3) = factor * HARMONIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
//     a12(J1,I2,I3) = factor * HARMONIC_AVERAGE (a12,J1,I2,I3,J1-1,I2,I3);
    a11(J1,I2,I3) = factor * mgop.harmonic(a11(J1,I2,I3),a11(J1-1,I2,I3));
    a12(J1,I2,I3) = factor * mgop.harmonic(a12(J1,I2,I3),a12(J1-1,I2,I3));
  }

  //a11.display("Here is a11 in DERIVATIVE");
  //a12.display("Here is a12 in DERIVATIVE");
    
  // Estimate D{-r}(i-1/2,j)
  RealDistributedArray urr(J1,I2,I3,N);
  urr =u(J1,I2,I3,N)-u(J1-1,I2,I3,N);   // Delta_{-}

  // Estimate D{-s}(i-1/2,j+1/2)
  RealDistributedArray urs(J1,I2,I3,N);    
  urs = 0.25 * (u(J1-1,I2+1,I3,N) + u(J1,I2+1,I3,N) - u(J1-1,I2-1,I3,N) - u(J1,I2-1,I3,N));

  // Vertex centered values of the transformation derivatives (ROW 2)
  RealDistributedArray a21(I1,J2,I3), a22(I1,J2,I3);
    
  a21 = (rx2d1*rx1d2)*s(I1,J2,I3);
  a22 = (rx2d1*rx2d2)*s(I1,J2,I3);

  J2 = Range(I2.getBase()  ,I2.getBound()+1);
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
  {
    a22(I1,J2,I3) = factor * ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
    a21(I1,J2,I3) = factor * ARITHMETIC_AVERAGE (a21,I1,J2,I3,I1,J2-1,I3);
  }
  else   
  {
    //cout << "Using harmonic average" << endl;
//    a22(I1,J2,I3) = factor * HARMONIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
//    a21(I1,J2,I3) = factor * HARMONIC_AVERAGE (a21,I1,J2,I3,I1,J2-1,I3);
    a22(I1,J2,I3) = factor * mgop.harmonic(a22(I1,J2,I3),a22(I1,J2-1,I3));
    a21(I1,J2,I3) = factor * mgop.harmonic(a21(I1,J2,I3),a21(I1,J2-1,I3));
  }
  // Estimate D{-s}(i,j-1/2)
  RealDistributedArray uss(I1,J2,I3,N);
  uss = u(I1,J2,I3,N)-u(I1,J2-1,I3,N);   // Delta_{-}
  // Estimate D{-r}(i+1/2,j-1/2)
  RealDistributedArray usr(I1,J2,I3,N);
  usr = 0.25 * (u(I1+1,J2-1,I3,N) + u(I1+1,J2,I3,N) - u(I1-1,J2-1,I3,N) - u(I1-1,J2,I3,N));
    
  // The spacing on the cross derivative terms is only 1/dx
//  h21 *= 2.0;
  for( axis=0; axis<3; axis++ )
    h21(axis)*= 2.0;
    
  // Calculate the derivative
  for( int n=N.getBase(); n<=N.getBound(); n++ )                        
  {
    derivative(I1,I2,I3,n)=
      ((a11(I1+1,I2  ,I3)*urr(I1+1,I2  ,I3,n) - a11(I1,I2,I3)*urr(I1,I2,I3,n))*h22(axis1)+
       (a22(I1  ,I2+1,I3)*uss(I1  ,I2+1,I3,n) - a22(I1,I2,I3)*uss(I1,I2,I3,n))*h22(axis2)+
       (a21(I1  ,I2+1,I3)*usr(I1  ,I2+1,I3,n) - a21(I1,I2,I3)*usr(I1,I2,I3,n) +
	a12(I1+1,I2  ,I3)*urs(I1+1,I2  ,I3,n) - a12(I1,I2,I3)*urs(I1,I2,I3,n))*
       (h21(axis1)*h21(axis2)));
  }
    
//   if( direction1==0 && direction2==0 )
//   {
//     a11.display("DSD: a11");
//     a22.display("DSD: a22");
//     a12.display("DSD: a12");
//     a21.display("DSD: a21");
//     urr.display("urr");
//     uss.display("uss");
//   }
  

//  h21 /= 2.0;
    
}

void 
derivativeScalarDerivativeFDerivative21(const realMappedGridFunction & ugf,
                             const realMappedGridFunction & s,
                             RealDistributedArray & derivative,
                             const int & direction1,
                             const int & direction2,
                             const Index & I1,
                             const Index & I2,
                             const Index & I3,
                             const Index & N,
                             MappedGridOperators & mgop )
// 2nd-order, 1d
{                                                                        
    
   // Averaging factor for face values ( = 0.5 for arithmetic averaging, 2.0 for harmonic averaging)
    const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;

 
    const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
    
    if (direction1 != 0 || direction2 != 0)
    {
        cout << "direction1 = " << direction1 << endl;
        cout << "direction2 = " << direction2 << endl;
        cout << "You may not differentiate the 2nd or 3rd coordinate directions in 1-D" << endl;
        Overture::abort("Error");
    }
    

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

    a11 = (RXI(J1,I2,I3,direction1) * RXI(J1,I2,I3,direction2))*sj(J1,I2,I3);

    J1 = Range(I1.getBase(),I1.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
        a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    }
    else
    {
//        a11(J1,I2,I3) = factor * HARMONIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
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
derivativeScalarDerivativeFDerivative21R(const realMappedGridFunction & ugf,
                              const realMappedGridFunction & s,
                              RealDistributedArray & derivative,
                              const int & direction1,
                              const int & direction2,
                              const Index & I1,
                              const Index & I2,
                              const Index & I3,
                              const Index & N,
                              MappedGridOperators & mgop )
// 2nd-order, 1-D, Rectangular grid
{                                                                        
  //  derivativeScalarDerivativeFDerivative21(ugf,s,derivative,direction1,direction2,I1,I2,I3,N,mgop );
    
   // Averaging factor for face values ( = 0.5 for arithmetic averaging, 2.0 for harmonic averaging)
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;
 
  // const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
    
  if (direction1 != 0 || direction2 != 0)
  {
    cout << "direction1 = " << direction1 << endl;
    cout << "direction2 = " << direction2 << endl;
    cout << "You may not differentiate the 2nd or 3rd coordinate directions in 1-D" << endl;
    Overture::abort("Error");
  }
    

  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency
    
  real h22c[3];
#define h22(n) h22c[n]
  for( int axis=0; axis<3; axis++ )
    h22(axis)=1./SQR(mgop.dx[axis]);
 
  Index J1 = Range(I1.getBase()-1,I1.getBound()+1);

  RealDistributedArray a11(J1,I2,I3);
  a11 = s(J1,I2,I3);

  J1 = Range(I1.getBase(),I1.getBound()+1);
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    a11(J1,I2,I3) = factor * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
  else
    a11(J1,I2,I3) = factor * mgop.harmonic(a11(J1,I2,I3),a11(J1-1,I2,I3));
//    a11(J1,I2,I3) = factor * HARMONIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);

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
derivativeScalarDerivativeFDerivative(const realMappedGridFunction & ugf,
                           const realMappedGridFunction & s,
                           RealDistributedArray & derivative,
                           const int & direction1,
                           const int & direction2,
                           const Index & I1,
                           const Index & I2,
                           const Index & I3,
                           const Index & N,
                           MappedGridOperators & mgop )
{                                                                        
  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  int & orderOfAccuracy = mgop.orderOfAccuracy;

    //cout << "In derivSderivFderiv" << endl;
    //cout << "direction 1 = " << direction1 << endl;
    //cout << "direction 2 = " << direction2 << endl;

  derivative = 0.0;
 
  if( mgop.isRectangular() )
  { // The grid is rectangular
    if( orderOfAccuracy==2 )
    {
      if( numberOfDimensions==1 )
      {
	derivativeScalarDerivativeFDerivative21R(ugf,s,derivative,direction1,direction2,I1,I2,I3,N,mgop );  // 1-D Rect
      }
      else if(numberOfDimensions==2 )
      {   
	derivativeScalarDerivativeFDerivative22R(ugf,s,derivative,direction1,direction2,I1,I2,I3,N,mgop );  // 2-D Rect
      }
      else 
      {   
	derivativeScalarDerivativeFDerivative23R(ugf,s,derivative,direction1,direction2,I1,I2,I3,N,mgop );  // 3-D Rect
      }
    }
    else   // ====== 4th order =======
    {
      Overture::abort("error");
/* ----
   RealArray & h41 = mgop.h41;
   RealArray & h42 = mgop.h42;
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
   ---- */
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
	derivativeScalarDerivativeFDerivative21(ugf,s,derivative,direction1,direction2,I1,I2,I3,N,mgop );
      else if(numberOfDimensions==2 )
	derivativeScalarDerivativeFDerivative22(ugf,s,derivative,direction1,direction2,I1,I2,I3,N,mgop );
      else // ======= 3D ================
	derivativeScalarDerivativeFDerivative23(ugf,s,derivative,direction1,direction2,I1,I2,I3,N,mgop );
    }
    else   // ====== 4th order =======
    {
      Overture::abort("error");
            
/* ----
   if( numberOfDimensions==1 )
   {
   RealArray & d14 = mgop.d14;
   RealArray & d24 = mgop.d24;
   for( n=N.getBase(); n<=N.getBound(); n++ )                        
   derivative(I1,I2,I3,n)=LAPLACIAN41(I1,I2,I3,n);                  
   }
   else if(numberOfDimensions==2 )
   {
   derivativeScalarDerivativeFDerivative42(ugf,s,derivative,direction1,direction2,I1,I2,I3,N,mgop );
                
   }
   else  // ======= 3D ================
   {
   derivativeScalarDerivativeFDerivative43(ugf,s,derivative,direction1,direction2,I1,I2,I3,N,mgop );
   }
   -----	    */ 
    }
  }
}


#undef ARITHMETIC_AVERAGE
#undef HARMONIC_AVERAGE
#undef RXI
#undef SXI
#undef TXI
