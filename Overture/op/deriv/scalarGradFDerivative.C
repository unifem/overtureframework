//================================================================================
// NOTES: 
//   Implements conservative and non-conservative difference approximation to
//        s grad u 
//================================================================================

#include "MappedGridOperators.h"
#include "xD.h"

void 
scalarGradFDerivative43(const realMappedGridFunction & ugf,
             const realMappedGridFunction & s,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 3d fourth order
{                                                                        
  printf("Sorry: scalarGrad not implemented yet for 3D, 4th order\n");
  Overture::abort("error");

}

void 
scalarGradFDerivative42(const realMappedGridFunction & ugf,
             const realMappedGridFunction & s,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 2d fourth order
{                                                                        
  printf("scalarGradFDerivative:ERROR: sorry, conservative form not implemented for 4th order\n");
  Overture::abort("error");
}

#undef ARITHMETIC_AVERAGE
#define ARITHMETIC_AVERAGE(s,I1,I2,I3,J1,J2,J3) (s(I1,I2,I3) + s(J1,J2,J3))

#define ARITHMETC_AVE(s,RX,I1,I2,I3,J1,J2,J3) (s(I1,I2,I3)*RX(I1,I2,I3)+s(J1,J2,J3)*RX(J1,J2,J3))

// define HARMONIC_AVE(s,RX,I1,I2,I3,J1,J2,J3) (s(I1,I2,I3)*RX(I1,I2,I3)*s(J1,J2,J3)*RX(J1,J2,J3)/  \
//		                    (s(I1,I2,I3)*RX(I1,I2,I3)+s(J1,J2,J3)*RX(J1,J2,J3))  )


                
void 
scalarGradFDerivative23(const realMappedGridFunction & ugf,
             const realMappedGridFunction & s,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 2nd-order, 3d
{                                                                        
  // printf("***scalarGradFDerivative23\n");

  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  // int & orderOfAccuracy = mgop.orderOfAccuracy;
  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();

  RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;

  Index J1 = Range(I1.getBase(),I1.getBound()+1);
  Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
  Index J3 = Range(I3.getBase()-1,I3.getBound()+1);

  RealDistributedArray ur(J1,J2,J3),a11(J1,J2,J3),a12(J1,J2,J3),a13(J1,J2,J3);
  ur =u(J1,J2,J3)-u(J1-1,J2,J3);   // Delta_{-}
                      
  
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? .5 : 2.;

  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
  {
    a11(J1,J2,J3)=ARITHMETC_AVE(s,RX, J1,J2,J3, J1-1,J2,J3)*ur; 
    a12(J1,J2,J3)=ARITHMETC_AVE(s,RY, J1,J2,J3, J1-1,J2,J3)*ur;
    a13(J1,J2,J3)=ARITHMETC_AVE(s,RZ, J1,J2,J3, J1-1,J2,J3)*ur;
  }
  else
  {
    realArray suh = mgop.harmonic(s(J1,J2,J3),s(J1-1,J2,J3))*.5*ur;
    a11(J1,J2,J3)=ARITHMETIC_AVERAGE(RX, J1,J2,J3, J1-1,J2,J3)*suh;
    a12(J1,J2,J3)=ARITHMETIC_AVERAGE(RY, J1,J2,J3, J1-1,J2,J3)*suh;
    a13(J1,J2,J3)=ARITHMETIC_AVERAGE(RZ, J1,J2,J3, J1-1,J2,J3)*suh;

  }
  

  J1 = Range(I1.getBase()-1,I1.getBound()+1);
  J2 = Range(I2.getBase(),I2.getBound()+1);
  RealDistributedArray us(J1,J2,J3),a21(J1,J2,J3),a22(J1,J2,J3),a23(J1,J2,J3);
  us =u(J1,J2,J3)-u(J1,J2-1,J3);   // Delta_{-}

  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
  {
    a21(J1,J2,J3)=ARITHMETC_AVE(s,SX, J1,J2,J3, J1,J2-1,J3)*us;
    a22(J1,J2,J3)=ARITHMETC_AVE(s,SY, J1,J2,J3, J1,J2-1,J3)*us;
    a23(J1,J2,J3)=ARITHMETC_AVE(s,SZ, J1,J2,J3, J1,J2-1,J3)*us;
  }
  else
  {
    realArray suh = mgop.harmonic(s(J1,J2,J3),s(J1,J2-1,J3))*.5*us;
    a21(J1,J2,J3)=ARITHMETIC_AVERAGE(SX, J1,J2,J3, J1,J2-1,J3)*suh;
    a22(J1,J2,J3)=ARITHMETIC_AVERAGE(SY, J1,J2,J3, J1,J2-1,J3)*suh;
    a23(J1,J2,J3)=ARITHMETIC_AVERAGE(SZ, J1,J2,J3, J1,J2-1,J3)*suh;
  }
  
  J2 = Range(I2.getBase()-1,I2.getBound()+1);
  J3 = Range(I3.getBase(),I3.getBound()+1);
  RealDistributedArray ut(J1,J2,J3),a31(J1,J2,J3),a32(J1,J2,J3),a33(J1,J2,J3);
  ut =u(J1,J2,J3)-u(J1,J2,J3-1);   // Delta_{-}

  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
  {
    a31(J1,J2,J3)=ARITHMETC_AVE(s,TX, J1,J2,J3, J1,J2,J3-1)*ut; 
    a32(J1,J2,J3)=ARITHMETC_AVE(s,TY, J1,J2,J3, J1,J2,J3-1)*ut;
    a33(J1,J2,J3)=ARITHMETC_AVE(s,TZ, J1,J2,J3, J1,J2,J3-1)*ut;
  }
  else
  {
    realArray suh = mgop.harmonic(s(J1,J2,J3),s(J1,J2,J3-1))*.5*ut;
    a31(J1,J2,J3)=ARITHMETIC_AVERAGE(TX, J1,J2,J3, J1,J2,J3-1)*suh;
    a32(J1,J2,J3)=ARITHMETIC_AVERAGE(TY, J1,J2,J3, J1,J2,J3-1)*suh;
    a33(J1,J2,J3)=ARITHMETIC_AVERAGE(TZ, J1,J2,J3, J1,J2,J3-1)*suh;
  }
  
  const int n=N.getBase();
  derivative(I1,I2,I3,n  )=( a11(I1+1,I2,I3)+a11(I1,I2,I3) )*(factor*d12(axis1)) +
                          +( a21(I1,I2+1,I3)+a21(I1,I2,I3) )*(factor*d12(axis2)) +
                          +( a31(I1,I2,I3+1)+a31(I1,I2,I3) )*(factor*d12(axis3));
  
  derivative(I1,I2,I3,n+1)=( a12(I1+1,I2,I3)+a12(I1,I2,I3) )*(factor*d12(axis1)) +
                          +( a22(I1,I2+1,I3)+a22(I1,I2,I3) )*(factor*d12(axis2)) +
                          +( a32(I1,I2,I3+1)+a32(I1,I2,I3) )*(factor*d12(axis3));

  derivative(I1,I2,I3,n+2)=( a13(I1+1,I2,I3)+a13(I1,I2,I3) )*(factor*d12(axis1)) +
                          +( a23(I1,I2+1,I3)+a23(I1,I2,I3) )*(factor*d12(axis2)) +
                          +( a33(I1,I2,I3+1)+a33(I1,I2,I3) )*(factor*d12(axis3));

}

void 
scalarGradFDerivative23R(const realMappedGridFunction & ugf,
             const realMappedGridFunction & s,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 2nd-order, 3d, Rectangular
{                                                                        
  // printf("***scalarGradFDerivative23R\n");

  // const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  // int & orderOfAccuracy = mgop.orderOfAccuracy;
  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

//  RealArray & h21 = mgop.h21;
      real h21c[3];
      for( int axis=0; axis<3; axis++ )
        h21c[axis]= 1./(2.*mgop.dx[axis]); 
#define h21(n) h21c[n]

  Index J1 = Range(I1.getBase(),I1.getBound()+1);
  Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
  Index J3 = Range(I3.getBase()-1,I3.getBound()+1);

  RealDistributedArray ur(J1,J2,J3),a11(J1,J2,J3);
  ur =u(J1,J2,J3)-u(J1-1,J2,J3);   // Delta_{-}
                      
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? .5 : 2.;

  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    a11(J1,J2,J3)=(s(J1,J2,J3)+s(J1-1,J2,J3))*ur;
  else
    a11(J1,J2,J3)=mgop.harmonic(s(J1,J2,J3),s(J1-1,J2,J3))*ur;

  J1 = Range(I1.getBase()-1,I1.getBound()+1);
  J2 = Range(I2.getBase(),I2.getBound()+1);
  RealDistributedArray us(J1,J2,J3),a22(J1,J2,J3);
  us =u(J1,J2,J3)-u(J1,J2-1,J3);   // Delta_{-}

  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    a22(J1,J2,J3)=(s(J1,J2,J3)+s(J1,J2-1,J3))*us;
  else
    a22(J1,J2,J3)=mgop.harmonic(s(J1,J2,J3),s(J1,J2-1,J3))*us;

  J2 = Range(I2.getBase()-1,I2.getBound()+1);
  J3 = Range(I3.getBase(),I3.getBound()+1);
  RealDistributedArray ut(J1,J2,J3),a33(J1,J2,J3);
  ut =u(J1,J2,J3)-u(J1,J2,J3-1);   // Delta_{-}

  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    a33(J1,J2,J3)=(s(J1,J2,J3)+s(J1,J2,J3-1))*ut;
  else
    a33(J1,J2,J3)=mgop.harmonic(s(J1,J2,J3),s(J1,J2,J3-1))*ut;

  const int n=N.getBase();
  derivative(I1,I2,I3,n  )=( a11(I1+1,I2,I3)+a11(I1,I2,I3) )*(factor*h21(axis1));
  derivative(I1,I2,I3,n+1)=( a22(I1,I2+1,I3)+a22(I1,I2,I3) )*(factor*h21(axis2));
  derivative(I1,I2,I3,n+2)=( a33(I1,I2,I3+1)+a33(I1,I2,I3) )*(factor*h21(axis3));
}


void 
scalarGradFDerivative22(const realMappedGridFunction & ugf,
             const realMappedGridFunction & s,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 2nd-order, 2d
{                                                                        
  // printf("***scalarGradFDerivative22\n");

  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  // int & orderOfAccuracy = mgop.orderOfAccuracy;
  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();

  RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;

  Index J1 = Range(I1.getBase(),I1.getBound()+1);
  Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
  Index J3 = I3;

  RealDistributedArray ur(J1,J2,J3),a11(J1,J2,J3),a12(J1,J2,J3);
  ur =u(J1,J2,J3)-u(J1-1,J2,J3);   // Delta_{-}
                      
  
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? .5 : 2.;

  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
  {
    a11(J1,J2,J3)=ARITHMETC_AVE(s,RX, J1,J2,J3, J1-1,J2,J3)*ur;
    a12(J1,J2,J3)=ARITHMETC_AVE(s,RY, J1,J2,J3, J1-1,J2,J3)*ur;
  }
  else
  {
    realArray suh = mgop.harmonic(s(J1,J2,J3),s(J1-1,J2,J3))*.5*ur;
    a11(J1,J2,J3)=ARITHMETIC_AVERAGE(RX, J1,J2,J3, J1-1,J2,J3)*suh;
    a12(J1,J2,J3)=ARITHMETIC_AVERAGE(RY, J1,J2,J3, J1-1,J2,J3)*suh;
  }

  J1 = Range(I1.getBase()-1,I1.getBound()+1);
  J2 = Range(I2.getBase(),I2.getBound()+1);
  RealDistributedArray us(J1,J2,J3),a21(J1,J2,J3),a22(J1,J2,J3);
  us =u(J1,J2,J3)-u(J1,J2-1,J3);   // Delta_{-}

  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
  {
    a21(J1,J2,J3)=ARITHMETC_AVE(s,SX, J1,J2,J3, J1,J2-1,J3)*us;
    a22(J1,J2,J3)=ARITHMETC_AVE(s,SY, J1,J2,J3, J1,J2-1,J3)*us;
  }
  else
  {
    realArray suh = mgop.harmonic(s(J1,J2,J3),s(J1,J2-1,J3))*.5*us;
    a21(J1,J2,J3)=ARITHMETIC_AVERAGE(SX, J1,J2,J3, J1,J2-1,J3)*suh;
    a22(J1,J2,J3)=ARITHMETIC_AVERAGE(SY, J1,J2,J3, J1,J2-1,J3)*suh;

  }
  
  const int n=N.getBase();
  derivative(I1,I2,I3,n  )=( a11(I1+1,I2,I3)+a11(I1,I2,I3) )*(factor*d12(axis1)) +
                          +( a21(I1,I2+1,I3)+a21(I1,I2,I3) )*(factor*d12(axis2));
  
  derivative(I1,I2,I3,n+1)=( a12(I1+1,I2,I3)+a12(I1,I2,I3) )*(factor*d12(axis1)) +
                          +( a22(I1,I2+1,I3)+a22(I1,I2,I3) )*(factor*d12(axis2));
}

void 
scalarGradFDerivative22R(const realMappedGridFunction & ugf,
             const realMappedGridFunction & s,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 2nd-order, 2d, rectangular grid
{                                                                        
  // printf("***scalarGradFDerivative22R\n");

  // const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  // int & orderOfAccuracy = mgop.orderOfAccuracy;
  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

//  RealArray & h21 = mgop.h21;
      real h21c[3];
      for( int axis=0; axis<3; axis++ )
        h21c[axis]= 1./(2.*mgop.dx[axis]); 
#define h21(n) h21c[n]

  Index J1 = Range(I1.getBase(),I1.getBound()+1);
  Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
  Index J3 = I3;

  RealDistributedArray ur(J1,J2,J3),a11(J1,J2,J3);
  ur =u(J1,J2,J3)-u(J1-1,J2,J3);   // Delta_{-}
                      
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? .5 : 2.;

  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    a11(J1,J2,J3)=(s(J1,J2,J3)+s(J1-1,J2,J3))*ur;
  else
    a11(J1,J2,J3)=mgop.harmonic(s(J1,J2,J3),s(J1-1,J2,J3))*ur;

  J1 = Range(I1.getBase()-1,I1.getBound()+1);
  J2 = Range(I2.getBase(),I2.getBound()+1);
  RealDistributedArray us(J1,J2,J3),a22(J1,J2,J3);
  us =u(J1,J2,J3)-u(J1,J2-1,J3);   // Delta_{-}

  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    a22(J1,J2,J3)=(s(J1,J2,J3)+s(J1,J2-1,J3))*us;
  else
    a22(J1,J2,J3)=mgop.harmonic(s(J1,J2,J3),s(J1,J2-1,J3))*us;

  const int n=N.getBase();
  derivative(I1,I2,I3,n  )=( a11(I1+1,I2,I3)+a11(I1,I2,I3) )*(factor*h21(axis1));
  
  derivative(I1,I2,I3,n+1)=( a22(I1,I2+1,I3)+a22(I1,I2,I3) )*(factor*h21(axis2));


}


void 
scalarGradFDerivative21(const realMappedGridFunction & ugf,
             const realMappedGridFunction & s,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 2nd-order, 1d
{                                                                        
  // const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  // int & orderOfAccuracy = mgop.orderOfAccuracy;
  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  RealDistributedArray & inverseVertexDerivative = 
    int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
    : mgop.mappedGrid.inverseCenterDerivative();

  RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;

  Index J1 = Range(I1.getBase(),I1.getBound()+1);
  Index J2 = I2;
  Index J3 = I3;

  RealDistributedArray ur(J1,J2,J3),a11(J1,J2,J3);
  ur =u(J1,J2,J3)-u(J1-1,J2,J3);   // Delta_{-}
  
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? .5 : 2.;
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    a11(J1,J2,J3)=ARITHMETC_AVE(s,RX, J1,J2,J3, J1-1,J2,J3)*ur;
  else
    a11(J1,J2,J3)=mgop.harmonic(evaluate(s(J1,J2,J3)*RX(J1,J2,J3)), evaluate(s(J1-1,J2,J3)*RX(J1-1,J2,J3)) )*ur;
//    a11(J1,J2,J3)=HARMONIC_AVE(s,RX, J1,J2,J3, J1-1,J2,J3)*ur;

  const int n=N.getBase();
  derivative(I1,I2,I3,n  )=( a11(I1+1,I2,I3)+a11(I1,I2,I3) )*(factor*d12(axis1));
}

void 
scalarGradFDerivative21R(const realMappedGridFunction & ugf,
             const realMappedGridFunction & s,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
// 2nd-order, 1d, rectangular grid
{                                                                        
  // const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  // int & orderOfAccuracy = mgop.orderOfAccuracy;
  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

//  RealArray & h21 = mgop.h21;
      real h21c[3];
      for( int axis=0; axis<3; axis++ )
        h21c[axis]= 1./(2.*mgop.dx[axis]); 
#define h21(n) h21c[n]

  Index J1 = Range(I1.getBase(),I1.getBound()+1);
  Index J2 = I2;
  Index J3 = I3;

  RealDistributedArray ur(J1,J2,J3),a11(J1,J2,J3);
  ur =u(J1,J2,J3)-u(J1-1,J2,J3);   // Delta_{-}
                      
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? .5 : 2.;
  if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    a11(J1,J2,J3)=(s(J1,J2,J3)+s(J1-1,J2,J3))*ur;
  else
    a11(J1,J2,J3)=mgop.harmonic(s(J1,J2,J3),s(J1-1,J2,J3))*ur;

  const int n=N.getBase();
  derivative(I1,I2,I3,n  )=( a11(I1+1,I2,I3)+a11(I1,I2,I3) )*(factor*h21(axis1));
}


void 
scalarGradFDerivative(const realMappedGridFunction & ugf,
             const realMappedGridFunction & s,
	     RealDistributedArray & derivative,
	     const Index & I1,
	     const Index & I2,
	     const Index & I3,
	     const Index & N,
	     MappedGridOperators & mgop )
{                                                                        
  // printf("***scalarGradFDerivative\n");

  const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  int & orderOfAccuracy = mgop.orderOfAccuracy;
  RealDistributedArray & u = (RealDistributedArray&) ugf;   // cast to a RealDistributedArray for efficiency

  int n;
  if( mgop.isRectangular() )
  { // The grid is rectangular
    if( orderOfAccuracy==2 )
    {
      // RealArray & h21 = mgop.h21;  // these are used in the macros
      // RealArray & h22 = mgop.h22;
      if( numberOfDimensions==1 )
        scalarGradFDerivative21R(ugf,s,derivative,I1,I2,I3,N,mgop );
      else if(numberOfDimensions==2 )
        scalarGradFDerivative22R(ugf,s,derivative,I1,I2,I3,N,mgop );
      else // ======= 3D ================
        scalarGradFDerivative23R(ugf,s,derivative,I1,I2,I3,N,mgop );
    }
    else   // ====== 4th order =======
    {
      if( &ugf )
      {
        printf("scalarGradFDerivative:ERROR: sorry, conservative form not implemented for 4th order\n");
        Overture::abort("error");
      }
      if( numberOfDimensions==1 )
      {
      }
      else if(numberOfDimensions==2 )
      {
      }
      else  // ======= 3D ================
      {
      }
    }
  }
  else 
  { // Ths grid is not rectangular
    // RealDistributedArray & ur = mgop.ur;
    // RealDistributedArray & us = mgop.us;
    // RealDistributedArray & ut = mgop.ut;

    RealDistributedArray & inverseVertexDerivative = 
          int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
                                                   : mgop.mappedGrid.inverseCenterDerivative();

    if( orderOfAccuracy==2 )
    {
      // RealArray & d12 = mgop.d12;
      // RealArray & d22 = mgop.d22;
      if( numberOfDimensions==1 )
        scalarGradFDerivative21(ugf,s,derivative,I1,I2,I3,N,mgop );
      else if(numberOfDimensions==2 )
        scalarGradFDerivative22(ugf,s,derivative,I1,I2,I3,N,mgop );
      else // ======= 3D ================
        scalarGradFDerivative23(ugf,s,derivative,I1,I2,I3,N,mgop );
    }
    else   // ====== 4th order =======
    {
      if( numberOfDimensions==1 )
      {
	// RealArray & d14 = mgop.d14;
        RealArray d24; d24 = 1./(12.*SQR(mgop.mappedGrid.gridSpacing())); // mgop.d24;
        for( n=N.getBase(); n<=N.getBound(); n++ )                        
          derivative(I1,I2,I3,n)=LAPLACIAN41(I1,I2,I3,n);                  
      }
      else if(numberOfDimensions==2 )
        scalarGradFDerivative42(ugf,s,derivative,I1,I2,I3,N,mgop );
      else  // ======= 3D ================
        scalarGradFDerivative43(ugf,s,derivative,I1,I2,I3,N,mgop );
    }
  }
}

#undef ARITHMETC_AVE















