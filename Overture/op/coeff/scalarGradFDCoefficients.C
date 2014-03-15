//================================================================================
//   Define the coefficient matrix for scalarGrad
//================================================================================

#include "MappedGridOperators.h"
#include "xDC.h"

void 
scalarGradFDerivCoefficients42(RealDistributedArray & derivative,
             const realMappedGridFunction & s,
		     const Index & I1,
		     const Index & I2,
		     const Index & I3,
		     const Index & E,
		     const Index & C,
		     MappedGridOperators & mgop )
// 4th order, 2d
{                                                                        

}

void 
scalarGradFDerivCoefficients43(RealDistributedArray & derivative,
             const realMappedGridFunction & s,
		     const Index & I1,
		     const Index & I2,
		     const Index & I3,
		     const Index & E,
		     const Index & C,
		     MappedGridOperators & mgop )
// 4th order 3d
{                                                                        
}

#define ARITHMETC_AVE(s,RX,I1,I2,I3,J1,J2,J3) (s(I1,I2,I3)*RX(I1,I2,I3)+s(J1,J2,J3)*RX(J1,J2,J3))

// define HARMONIC_AVE(s,RX,I1,I2,I3,J1,J2,J3) (s(I1,I2,I3)*RX(I1,I2,I3)*s(J1,J2,J3)*RX(J1,J2,J3)/  \
//		                    (s(I1,I2,I3)*RX(I1,I2,I3)+s(J1,J2,J3)*RX(J1,J2,J3))  )

void 
scalarGradFDerivCoefficients2(RealDistributedArray & derivative,
             const realMappedGridFunction & s,
			   const Index & I1,
			   const Index & I2,
			   const Index & I3,
			   const Index & E,
			   const Index & C,
			   MappedGridOperators & mgop )
// 2nd order
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

  RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
  if( numberOfDimensions==1 )
  { // get coefficients for the first component
    Index J1 = Range(I1.getBase(),I1.getBound()+1);
    Index J2 = I2;
    Index J3 = I3;

    RealDistributedArray a11(J1,J2,J3);
    const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? .5 : 2.;
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
      a11(J1,J2,J3)=ARITHMETC_AVE(s,RX, J1,J2,J3, J1-1,J2,J3)*(factor*d12(axis1));
    else
      a11(J1,J2,J3)=mgop.harmonic( evaluate(s(J1,J2,J3)*RX(J1,J2,J3)), 
				     evaluate(s(J1-1,J2,J3)*RX(J1-1,J2,J3)) )*(factor*d12(axis1));
//      a11(J1,J2,J3)=HARMONIC_AVE(s,RX, J1,J2,J3, J1-1,J2,J3)*(factor*d12(axis1));
    a11.reshape(1,J1,J2,J3); 

    // here is s partial_x
    c0=C.getBase();
    derivative(MCE(-1, 0, 0),I1,I2,I3)=                  -a11(0,I1,I2,I3);
    derivative(MCE( 0, 0, 0),I1,I2,I3)=-a11(0,I1+1,I2,I3)+a11(0,I1,I2,I3);
    derivative(MCE(+1, 0, 0),I1,I2,I3)= a11(0,I1+1,I2,I3);
  }
  else if( numberOfDimensions==2 )
  {
    Index J1 = Range(I1.getBase(),I1.getBound()+1);
    Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
    Index J3 = I3;

    RealDistributedArray a11(J1,J2,J3),a12(J1,J2,J3);
    const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? .5 : 2.;
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a11(J1,J2,J3)=ARITHMETC_AVE(s,RX, J1,J2,J3, J1-1,J2,J3)*(factor*d12(axis1));
      a12(J1,J2,J3)=ARITHMETC_AVE(s,RY, J1,J2,J3, J1-1,J2,J3)*(factor*d12(axis1));
    }
    else
    {
//       a11(J1,J2,J3)=HARMONIC_AVE(s,RX, J1,J2,J3, J1-1,J2,J3)*(factor*d12(axis1));
//       a12(J1,J2,J3)=HARMONIC_AVE(s,RY, J1,J2,J3, J1-1,J2,J3)*(factor*d12(axis1));
      a11(J1,J2,J3)=mgop.harmonic( evaluate(s(J1,J2,J3)*RX(J1,J2,J3)), 
				     evaluate(s(J1-1,J2,J3)*RX(J1-1,J2,J3)) )*(factor*d12(axis1));
      a12(J1,J2,J3)=mgop.harmonic( evaluate(s(J1,J2,J3)*RY(J1,J2,J3)),
				     evaluate(s(J1-1,J2,J3)*RY(J1-1,J2,J3)) )*(factor*d12(axis1));
    }
    a11.reshape(1,J1,J2,J3); 
    a12.reshape(1,J1,J2,J3); 

    J1 = Range(I1.getBase()-1,I1.getBound()+1);
    J2 = Range(I2.getBase(),I2.getBound()+1);
    RealDistributedArray a21(J1,J2,J3),a22(J1,J2,J3);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a21(J1,J2,J3)=ARITHMETC_AVE(s,SX, J1,J2,J3, J1,J2-1,J3)*(factor*d12(axis2));
      a22(J1,J2,J3)=ARITHMETC_AVE(s,SY, J1,J2,J3, J1,J2-1,J3)*(factor*d12(axis2));
    }
    else
    {
//       a21(J1,J2,J3)=HARMONIC_AVE(s,SX, J1,J2,J3, J1,J2-1,J3)*(factor*d12(axis2));
//       a22(J1,J2,J3)=HARMONIC_AVE(s,SY, J1,J2,J3, J1,J2-1,J3)*(factor*d12(axis2));
      a21(J1,J2,J3)=mgop.harmonic(evaluate(s(J1,J2,J3)*SX(J1,J2,J3)), 
				  evaluate(s(J1,J2-1,J3)*SX(J1,J2-1,J3)) )*(factor*d12(axis2));
      a22(J1,J2,J3)=mgop.harmonic(evaluate(s(J1,J2,J3)*SY(J1,J2,J3)), 
				  evaluate(s(J1,J2-1,J3)*SY(J1,J2-1,J3)) )*(factor*d12(axis2));
    }
    a21.reshape(1,J1,J2,J3); 
    a22.reshape(1,J1,J2,J3); 
  
    // derivative(I1,I2,I3,n  )=( a11(I1+1,I2,I3)+a11(I1,I2,I3) )*(factor*d12(axis1)) +
    //  +( a21(I1,I2+1,I3)+a21(I1,I2,I3) )*(factor*d12(axis2));
    // derivative(I1,I2,I3,n+1)=( a12(I1+1,I2,I3)+a12(I1,I2,I3) )*(factor*d12(axis1)) +
    //  +( a22(I1,I2+1,I3)+a22(I1,I2,I3) )*(factor*d12(axis2));

    // here is s partial_x
    c0=C.getBase();
    derivative(MCE(-1,-1, 0),I1,I2,I3)= 0.;
    derivative(MCE( 0,-1, 0),I1,I2,I3)=                                                    -a21(0,I1,I2,I3);
    derivative(MCE(+1,-1, 0),I1,I2,I3)= 0.; 
    derivative(MCE(-1, 0, 0),I1,I2,I3)=                  -a11(0,I1,I2,I3);
    derivative(MCE( 0, 0, 0),I1,I2,I3)=-a11(0,I1+1,I2,I3)+a11(0,I1,I2,I3)-a21(0,I1,I2+1,I3)+a21(0,I1,I2,I3);
    derivative(MCE(+1, 0, 0),I1,I2,I3)= a11(0,I1+1,I2,I3);
    derivative(MCE(-1,+1, 0),I1,I2,I3)= 0.; 
    derivative(MCE( 0,+1, 0),I1,I2,I3)=                                   a21(0,I1,I2+1,I3);
    derivative(MCE(+1,+1, 0),I1,I2,I3)= 0.;

    // here is s partial_y
    c0++;  // fill in next component
    derivative(MCE(-1,-1, 0),I1,I2,I3)= 0.; 
    derivative(MCE( 0,-1, 0),I1,I2,I3)=                                                    -a22(0,I1,I2,I3);
    derivative(MCE(+1,-1, 0),I1,I2,I3)= 0.;
    derivative(MCE(-1, 0, 0),I1,I2,I3)=                  -a12(0,I1,I2,I3);
    derivative(MCE( 0, 0, 0),I1,I2,I3)=-a12(0,I1+1,I2,I3)+a12(0,I1,I2,I3)-a22(0,I1,I2+1,I3)+a22(0,I1,I2,I3);
    derivative(MCE(+1, 0, 0),I1,I2,I3)= a12(0,I1+1,I2,I3);
    derivative(MCE(-1,+1, 0),I1,I2,I3)= 0.;
    derivative(MCE( 0,+1, 0),I1,I2,I3)=                                   a22(0,I1,I2+1,I3);
    derivative(MCE(+1,+1, 0),I1,I2,I3)= 0.;

  }
  else // ======= 3D ================
  {
    Index J1 = Range(I1.getBase(),I1.getBound()+1);
    Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
    Index J3 = Range(I3.getBase()-1,I3.getBound()+1);

    const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? .5 : 2.;
    RealDistributedArray a11(J1,J2,J3),a12(J1,J2,J3),a13(J1,J2,J3);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a11(J1,J2,J3)=ARITHMETC_AVE(s,RX, J1,J2,J3, J1-1,J2,J3)*(factor*d12(axis1)); 
      a12(J1,J2,J3)=ARITHMETC_AVE(s,RY, J1,J2,J3, J1-1,J2,J3)*(factor*d12(axis1));
      a13(J1,J2,J3)=ARITHMETC_AVE(s,RZ, J1,J2,J3, J1-1,J2,J3)*(factor*d12(axis1));
    }
    else
    {
      // a11(J1,J2,J3)=HARMONIC_AVE(s,RX, J1,J2,J3, J1-1,J2,J3)*(factor*d12(axis1)); 
      // a12(J1,J2,J3)=HARMONIC_AVE(s,RY, J1,J2,J3, J1-1,J2,J3)*(factor*d12(axis1));
      // a13(J1,J2,J3)=HARMONIC_AVE(s,RZ, J1,J2,J3, J1-1,J2,J3)*(factor*d12(axis1));

      a11(J1,J2,J3)=mgop.harmonic( evaluate(s(J1,J2,J3)*RX(J1,J2,J3)), 
				     evaluate(s(J1-1,J2,J3)*RX(J1-1,J2,J3)) )*(factor*d12(axis1));
      a12(J1,J2,J3)=mgop.harmonic( evaluate(s(J1,J2,J3)*RY(J1,J2,J3)),
				     evaluate(s(J1-1,J2,J3)*RY(J1-1,J2,J3)) )*(factor*d12(axis1));
      a13(J1,J2,J3)=mgop.harmonic( evaluate(s(J1,J2,J3)*RZ(J1,J2,J3)),
				     evaluate(s(J1-1,J2,J3)*RZ(J1-1,J2,J3)) )*(factor*d12(axis1));
    }
    a11.reshape(1,J1,J2,J3); 
    a12.reshape(1,J1,J2,J3); 
    a13.reshape(1,J1,J2,J3); 

    J1 = Range(I1.getBase()-1,I1.getBound()+1);
    J2 = Range(I2.getBase(),I2.getBound()+1);
    RealDistributedArray a21(J1,J2,J3),a22(J1,J2,J3),a23(J1,J2,J3);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a21(J1,J2,J3)=ARITHMETC_AVE(s,SX, J1,J2,J3, J1,J2-1,J3)*(factor*d12(axis2));
      a22(J1,J2,J3)=ARITHMETC_AVE(s,SY, J1,J2,J3, J1,J2-1,J3)*(factor*d12(axis2));
      a23(J1,J2,J3)=ARITHMETC_AVE(s,SZ, J1,J2,J3, J1,J2-1,J3)*(factor*d12(axis2));
    }
    else
    {
      // a21(J1,J2,J3)=HARMONIC_AVE(s,SX, J1,J2,J3, J1,J2-1,J3)*(factor*d12(axis2)); 
      // a22(J1,J2,J3)=HARMONIC_AVE(s,SY, J1,J2,J3, J1,J2-1,J3)*(factor*d12(axis2));
      // a23(J1,J2,J3)=HARMONIC_AVE(s,SZ, J1,J2,J3, J1,J2-1,J3)*(factor*d12(axis2));
      a21(J1,J2,J3)=mgop.harmonic(evaluate(s(J1,J2,J3)*SX(J1,J2,J3)), 
				  evaluate(s(J1,J2-1,J3)*SX(J1,J2-1,J3)) )*(factor*d12(axis2));
      a22(J1,J2,J3)=mgop.harmonic(evaluate(s(J1,J2,J3)*SY(J1,J2,J3)), 
				  evaluate(s(J1,J2-1,J3)*SY(J1,J2-1,J3)) )*(factor*d12(axis2));
      a23(J1,J2,J3)=mgop.harmonic(evaluate(s(J1,J2,J3)*SZ(J1,J2,J3)), 
				  evaluate(s(J1,J2-1,J3)*SZ(J1,J2-1,J3)) )*(factor*d12(axis2));
    }
    a21.reshape(1,J1,J2,J3); 
    a22.reshape(1,J1,J2,J3); 
    a23.reshape(1,J1,J2,J3); 
  
    J2 = Range(I2.getBase()-1,I2.getBound()+1);
    J3 = Range(I3.getBase(),I3.getBound()+1);
    RealDistributedArray a31(J1,J2,J3),a32(J1,J2,J3),a33(J1,J2,J3);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a31(J1,J2,J3)=ARITHMETC_AVE(s,TX, J1,J2,J3, J1,J2,J3-1)*(factor*d12(axis3)); 
      a32(J1,J2,J3)=ARITHMETC_AVE(s,TY, J1,J2,J3, J1,J2,J3-1)*(factor*d12(axis3));
      a33(J1,J2,J3)=ARITHMETC_AVE(s,TZ, J1,J2,J3, J1,J2,J3-1)*(factor*d12(axis3));
    }
    else
    {
      // a31(J1,J2,J3)=HARMONIC_AVE(s,TX, J1,J2,J3, J1,J2,J3-1)*(factor*d12(axis3)); 
      // a32(J1,J2,J3)=HARMONIC_AVE(s,TY, J1,J2,J3, J1,J2,J3-1)*(factor*d12(axis3));
      // a33(J1,J2,J3)=HARMONIC_AVE(s,TZ, J1,J2,J3, J1,J2,J3-1)*(factor*d12(axis3));
      a31(J1,J2,J3)=mgop.harmonic(evaluate(s(J1,J2,J3)*TX(J1,J2,J3)), 
				  evaluate(s(J1,J2,J3-1)*TX(J1,J2,J3-1)) )*(factor*d12(axis3));
      a32(J1,J2,J3)=mgop.harmonic(evaluate(s(J1,J2,J3)*TY(J1,J2,J3)), 
				  evaluate(s(J1,J2,J3-1)*TY(J1,J2,J3-1)) )*(factor*d12(axis3));
      a33(J1,J2,J3)=mgop.harmonic(evaluate(s(J1,J2,J3)*TZ(J1,J2,J3)), 
				  evaluate(s(J1,J2,J3-1)*TZ(J1,J2,J3-1)) )*(factor*d12(axis3));
    }
    a31.reshape(1,J1,J2,J3); 
    a32.reshape(1,J1,J2,J3); 
    a33.reshape(1,J1,J2,J3); 
  
    // here is s partial_x
    c0=C.getBase();
    derivative(MCE(-1,-1,-1),I1,I2,I3)= 0.;
    derivative(MCE( 0,-1,-1),I1,I2,I3)= 0.;
    derivative(MCE(+1,-1,-1),I1,I2,I3)= 0.;
    derivative(MCE(-1, 0,-1),I1,I2,I3)= 0.;
    derivative(MCE( 0, 0,-1),I1,I2,I3)=                              -a31(0,I1,I2,I3);
    derivative(MCE(+1, 0,-1),I1,I2,I3)= 0.;
    derivative(MCE(-1,+1,-1),I1,I2,I3)= 0.;
    derivative(MCE( 0,+1,-1),I1,I2,I3)= 0.;
    derivative(MCE(+1,+1,-1),I1,I2,I3)= 0.;

    derivative(MCE(-1,-1, 0),I1,I2,I3)= 0.;
    derivative(MCE( 0,-1, 0),I1,I2,I3)=                           -a21(0,I1,I2,I3);
    derivative(MCE(+1,-1, 0),I1,I2,I3)= 0.; 
    derivative(MCE(-1, 0, 0),I1,I2,I3)=                  -a11(0,I1,I2,I3);
    derivative(MCE( 0, 0, 0),I1,I2,I3)=-a11(0,I1+1,I2,I3)+a11(0,I1,I2,I3)
                                                -a21(0,I1,I2+1,I3)+a21(0,I1,I2,I3)
                                                   -a31(0,I1,I2,I3+1)+a31(0,I1,I2,I3);
    derivative(MCE(+1, 0, 0),I1,I2,I3)= a11(0,I1+1,I2,I3);
    derivative(MCE(-1,+1, 0),I1,I2,I3)= 0.; 
    derivative(MCE( 0,+1, 0),I1,I2,I3)=          a21(0,I1,I2+1,I3);
    derivative(MCE(+1,+1, 0),I1,I2,I3)= 0.;

    derivative(MCE(-1,-1,+1),I1,I2,I3)= 0.;
    derivative(MCE( 0,-1,+1),I1,I2,I3)= 0.;
    derivative(MCE(+1,-1,+1),I1,I2,I3)= 0.;
    derivative(MCE(-1, 0,+1),I1,I2,I3)= 0.;
    derivative(MCE( 0, 0,+1),I1,I2,I3)=             a31(0,I1,I2,I3+1);
    derivative(MCE(+1, 0,+1),I1,I2,I3)= 0.;
    derivative(MCE(-1,+1,+1),I1,I2,I3)= 0.;
    derivative(MCE( 0,+1,+1),I1,I2,I3)= 0.;
    derivative(MCE(+1,+1,+1),I1,I2,I3)= 0.;

    // here is s partial_y
    c0++;  // fill in next component
    derivative(MCE(-1,-1,-1),I1,I2,I3)= 0.;
    derivative(MCE( 0,-1,-1),I1,I2,I3)= 0.;
    derivative(MCE(+1,-1,-1),I1,I2,I3)= 0.;
    derivative(MCE(-1, 0,-1),I1,I2,I3)= 0.;
    derivative(MCE( 0, 0,-1),I1,I2,I3)=                              -a32(0,I1,I2,I3);
    derivative(MCE(+1, 0,-1),I1,I2,I3)= 0.;
    derivative(MCE(-1,+1,-1),I1,I2,I3)= 0.;
    derivative(MCE( 0,+1,-1),I1,I2,I3)= 0.;
    derivative(MCE(+1,+1,-1),I1,I2,I3)= 0.;

    derivative(MCE(-1,-1, 0),I1,I2,I3)= 0.;
    derivative(MCE( 0,-1, 0),I1,I2,I3)=                           -a22(0,I1,I2,I3);
    derivative(MCE(+1,-1, 0),I1,I2,I3)= 0.; 
    derivative(MCE(-1, 0, 0),I1,I2,I3)=                  -a12(0,I1,I2,I3);
    derivative(MCE( 0, 0, 0),I1,I2,I3)=-a12(0,I1+1,I2,I3)+a12(0,I1,I2,I3)
                                                -a22(0,I1,I2+1,I3)+a22(0,I1,I2,I3)
                                                   -a32(0,I1,I2,I3+1)+a32(0,I1,I2,I3);
    derivative(MCE(+1, 0, 0),I1,I2,I3)= a12(0,I1+1,I2,I3);
    derivative(MCE(-1,+1, 0),I1,I2,I3)= 0.; 
    derivative(MCE( 0,+1, 0),I1,I2,I3)=          a22(0,I1,I2+1,I3);
    derivative(MCE(+1,+1, 0),I1,I2,I3)= 0.;

    derivative(MCE(-1,-1,+1),I1,I2,I3)= 0.;
    derivative(MCE( 0,-1,+1),I1,I2,I3)= 0.;
    derivative(MCE(+1,-1,+1),I1,I2,I3)= 0.;
    derivative(MCE(-1, 0,+1),I1,I2,I3)= 0.;
    derivative(MCE( 0, 0,+1),I1,I2,I3)=             a32(0,I1,I2,I3+1);
    derivative(MCE(+1, 0,+1),I1,I2,I3)= 0.;
    derivative(MCE(-1,+1,+1),I1,I2,I3)= 0.;
    derivative(MCE( 0,+1,+1),I1,I2,I3)= 0.;
    derivative(MCE(+1,+1,+1),I1,I2,I3)= 0.;

    // here is s partial_z
    c0++;  // fill in next component
    derivative(MCE(-1,-1,-1),I1,I2,I3)= 0.;
    derivative(MCE( 0,-1,-1),I1,I2,I3)= 0.;
    derivative(MCE(+1,-1,-1),I1,I2,I3)= 0.;
    derivative(MCE(-1, 0,-1),I1,I2,I3)= 0.;
    derivative(MCE( 0, 0,-1),I1,I2,I3)=                              -a33(0,I1,I2,I3);
    derivative(MCE(+1, 0,-1),I1,I2,I3)= 0.;
    derivative(MCE(-1,+1,-1),I1,I2,I3)= 0.;
    derivative(MCE( 0,+1,-1),I1,I2,I3)= 0.;
    derivative(MCE(+1,+1,-1),I1,I2,I3)= 0.;

    derivative(MCE(-1,-1, 0),I1,I2,I3)= 0.;
    derivative(MCE( 0,-1, 0),I1,I2,I3)=                           -a23(0,I1,I2,I3);
    derivative(MCE(+1,-1, 0),I1,I2,I3)= 0.; 
    derivative(MCE(-1, 0, 0),I1,I2,I3)=                  -a13(0,I1,I2,I3);
    derivative(MCE( 0, 0, 0),I1,I2,I3)=-a13(0,I1+1,I2,I3)+a13(0,I1,I2,I3)
                                                -a23(0,I1,I2+1,I3)+a23(0,I1,I2,I3)
                                                   -a33(0,I1,I2,I3+1)+a33(0,I1,I2,I3);
    derivative(MCE(+1, 0, 0),I1,I2,I3)= a13(0,I1+1,I2,I3);
    derivative(MCE(-1,+1, 0),I1,I2,I3)= 0.; 
    derivative(MCE( 0,+1, 0),I1,I2,I3)=          a23(0,I1,I2+1,I3);
    derivative(MCE(+1,+1, 0),I1,I2,I3)= 0.;

    derivative(MCE(-1,-1,+1),I1,I2,I3)= 0.;
    derivative(MCE( 0,-1,+1),I1,I2,I3)= 0.;
    derivative(MCE(+1,-1,+1),I1,I2,I3)= 0.;
    derivative(MCE(-1, 0,+1),I1,I2,I3)= 0.;
    derivative(MCE( 0, 0,+1),I1,I2,I3)=             a33(0,I1,I2,I3+1);
    derivative(MCE(+1, 0,+1),I1,I2,I3)= 0.;
    derivative(MCE(-1,+1,+1),I1,I2,I3)= 0.;
    derivative(MCE( 0,+1,+1),I1,I2,I3)= 0.;
    derivative(MCE(+1,+1,+1),I1,I2,I3)= 0.;


  }
}


void 
scalarGradFDerivCoefficients(RealDistributedArray & derivative,
				const realMappedGridFunction & s,
				const Index & I1,
				const Index & I2,
				const Index & I3,
				const Index & E,
				const Index & C,
				MappedGridOperators & mgop )
{                                                                        
  // const int & numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
  int & orderOfAccuracy    = mgop.orderOfAccuracy;
  // int & width              = mgop.width;
  // int & halfWidth1         = mgop.halfWidth1;
  // int & halfWidth2         = mgop.halfWidth2;
  // int & halfWidth3         = mgop.halfWidth3;
  // int & stencilSize        = mgop.stencilSize;
  // int & numberOfComponentsForCoefficients = mgop.numberOfComponentsForCoefficients;

  // int m1,m2,m3,n;
  // int dum;
//   Range aR0,aR1,aR2,aR3;
  // RealArray & delta = mgop.delta;

  int e0=E.getBase();
  int c0=C.getBase();

  // real time=getCPU();
  if( mgop.isRectangular() && FALSE )
  { // ***** optimized NEW way *****
    if( orderOfAccuracy==2 )
    {
      scalarGradFDerivCoefficients2(derivative,s,I1,I2,I3,E,C,mgop );

    }
    else   // ====== 4th order =======
    {
      printf("scalarGradFDerivCoefficients: sorry, no implemented for 4th order\n");
      Overture::abort("error");
    }
  }
  else
  {
    // *** optimized curvilinear ****
    if( orderOfAccuracy==2 )
    {
      scalarGradFDerivCoefficients2(derivative,s,I1,I2,I3,E,C,mgop );
    }
    else   // ====== 4th order =======
    {
      printf("scalarGradFDerivCoefficients: sorry, no implemented for 4th order\n");
      Overture::abort("error");
    }
  }
  // time=getCPU()-time;
  // printf("scalarGradFDerivCoefficients: time = %e \n",time);
  
}


#undef CE
