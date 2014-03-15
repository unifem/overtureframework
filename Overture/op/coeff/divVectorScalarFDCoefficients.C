//================================================================================
//
//  MODIFIED:  April 19, 1999 WJW
//             990609 WDH : cleaned up a bit.
//
//
//  NOTES:
//
//  Define the coefficient matrix for the operator div ( S phi)
//
//  where S is a known vector and phi is the unknown (n-component) scalar function
//
//   Who to Blame:  Bill Wangard
//
//   Fully tested?
//
//                 2-D... not yet
//                 3-D... not yet 
//                 1-D... not yet
//
//================================================================================

#include "MappedGridOperators.h"
#include "xDC.h"

#undef ARITHMETIC_AVERAGE
#define ARITHMETIC_AVERAGE(s,I1,I2,I3,J1,J2,J3) (s(I1,I2,I3) + s(J1,J2,J3))

// undef HARMONIC_AVERAGE 
// define HARMONIC_AVERAGE(s,i,I1,I2,I3,J1,J2,J3) (s(i,I1,I2,I3)*s(i,J1,J2,J3)/(s(i,I1,I2,I3)+s(i,J1,J2,J3) + REAL_EPSILON))

void 
divVectorScalarFDerivCoefficients42(RealDistributedArray & derivative,
                                               const realMappedGridFunction & s,
                                               const Index & I1,
                                               const Index & I2,
                                               const Index & I3,
                                               const Index & E,
                                               const Index & C,
                                               MappedGridOperators & mgop )
// 4th order, 2d
{                                                                        
  cout << "Sorry: divVectorScalarCoefficients, 4th order, 2d, is not implemented yet." << endl;
  Overture::abort( "error"); 
}

void 
divVectorScalarFDerivCoefficients43(RealDistributedArray & derivative,
                                               const realMappedGridFunction & s,
                                               const Index & I1,
                                               const Index & I2,
                                               const Index & I3,
                                               const Index & E,
                                               const Index & C,
                                               MappedGridOperators & mgop )
// 4th order 3d
{                                                                        
  cout << "Sorry: divVectorScalarCoefficients, 4th order, 3d, is not implemented yet." << endl;
  Overture::abort( "error"); 
}


void 
divVectorScalarFDerivCoefficients2(RealDistributedArray & derivative,
                                              const realMappedGridFunction & s,
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
  
// Averaging factor for face values ( = 0.5 for arithmetic averaging, 2.0 for harmonic averaging)
  const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;

  if( numberOfDimensions==1 )
  { // get coefficients for the first component
      
    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
      
    RealDistributedArray rx(J1,I2,I3), a11(J1,I2,I3);
      
    rx = RX(J1,I2,I3);  // This is jInverse
    a11 = s(J1,I2,I3,0);  // j*rx = 1.0 in 1-D
      
    const real factor = mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage ? .5 : 2.;
      
    J1 = Range(I1.getBase(),I1.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a11(J1,I2,I3) = (factor * d12(axis1)) * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    }
    else    
    {
      a11(J1,I2,I3) = (factor * d12(axis1)) * mgop.harmonic(a11(J1,I2,I3),a11(J1-1,I2,I3));
    }
    a11.reshape(1,J1,I2,I3);
    rx.reshape(1,J1,I2,I3);
      
    derivative(MCE(-1, 0, 0),I1,I2,I3)=                   -a11(0,I1,I2,I3) *rx(0,I1,I2,I3);
    derivative(MCE( 0, 0, 0),I1,I2,I3)= (a11(0,I1+1,I2,I3)-a11(0,I1,I2,I3))*rx(0,I1,I2,I3);
    derivative(MCE(+1, 0, 0),I1,I2,I3)=  a11(0,I1+1,I2,I3)                 *rx(0,I1,I2,I3);
      
  }
  else if( numberOfDimensions==2 )
  {
    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
    Index J2 = Range(I2.getBase()-1,I2.getBound()+1);      

    const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();

      // Vertex centered values of the transformation derivatives 
    RealDistributedArray a11(J1,J2,I3), a22(J1,J2,I3);

    a11 = (RX(J1,J2,I3) * s(J1,J2,I3,0) + RY(J1,J2,I3) * s(J1,J2,I3,1)) * j(J1,J2,I3);
    a22 = (SX(J1,J2,I3) * s(J1,J2,I3,0) + SY(J1,J2,I3) * s(J1,J2,I3,1)) * j(J1,J2,I3);

    J1 = Range(I1.getBase(),I1.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a11(J1,I2,I3) = (factor * d12(axis1)) * ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    }
    else    
    {
      a11(J1,I2,I3) = (factor * d12(axis1)) * mgop.harmonic(a11(J1,I2,I3),a11(J1-1,I2,I3));
    }

    J2 = Range(I2.getBase()  ,I2.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a22(I1,J2,I3) = (factor * d12(axis2)) * ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
    }
    else   
    {
      a22(I1,J2,I3) = (factor * d12(axis2)) * mgop.harmonic(a22(I1,J2,I3),a22(I1,J2-1,I3));
    }

    a11.reshape(1,J1,J2,I3);
    a22.reshape(1,J1,J2,I3);
      
    RealDistributedArray jInverse = evaluate(1./j(I1,I2,I3));
    jInverse.reshape(1,I1,I2,I3);
      
    derivative(MCE(-1,-1, 0),I1,I2,I3) =  0.;

    derivative(MCE(-1, 0, 0),I1,I2,I3) =                       - a11(0,I1  ,I2  ,I3)  *jInverse;

    derivative(MCE(-1,+1, 0),I1,I2,I3) =  0.; 

    derivative(MCE( 0,-1, 0),I1,I2,I3) =                        -a22(0,I1  ,I2  ,I3)  *jInverse;

    derivative(MCE( 0, 0, 0),I1,I2,I3) =  (a11(0,I1+1,I2  ,I3) - a11(0,I1  ,I2  ,I3) + 
					   a22(0,I1  ,I2+1,I3) - a22(0,I1  ,I2  ,I3)) * jInverse;

    derivative(MCE( 0,+1, 0),I1,I2,I3) =   a22(0,I1  ,I2+1,I3)                        *jInverse;
     
    derivative(MCE(+1,-1, 0),I1,I2,I3) =  0.;

    derivative(MCE(+1, 0, 0),I1,I2,I3) =   a11(0,I1+1,I2  ,I3)                        *jInverse;

    derivative(MCE(+1,+1, 0),I1,I2,I3) =  0.; 
      
      
  }
  else // ======= 3D ================
  {

    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
    Index J2 = Range(I2.getBase()-1,I2.getBound()+1);      
    Index J3 = Range(I3.getBase()-1,I3.getBound()+1);
      
    RealDistributedArray sj(J1,J2,J3);
    const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();
      
    // Vertex centered values of the transformation derivatives (ROW 1)
    RealDistributedArray  a11(J1,J2,J3), a22(J1,J2,J3), a33(J1,J2,J3);

    a11 = (RX(J1,J2,J3) * s(J1,J2,J3,0)
         + RY(J1,J2,J3) * s(J1,J2,J3,1)
         + RZ(J1,J2,J3) * s(J1,J2,J3,2)) * j(J1,J2,J3);

    a22 = (SX(J1,J2,J3) * s(J1,J2,J3,0)
         + SY(J1,J2,J3) * s(J1,J2,J3,1)
         + SZ(J1,J2,J3) * s(J1,J2,J3,2)) * j(J1,J2,J3);

    a33 = (TX(J1,J2,J3) * s(J1,J2,J3,0)
         + TY(J1,J2,J3) * s(J1,J2,J3,1)
         + TZ(J1,J2,J3) * s(J1,J2,J3,2)) * j(J1,J2,J3);

      
    J1 = Range(I1.getBase(),I1.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a11(J1,I2,I3) = (factor*d12(axis1))*ARITHMETIC_AVERAGE (a11,J1,I2,I3,J1-1,I2,I3);
    }
    else    
    {
      a11(J1,I2,I3) = (factor*d12(axis1))*mgop.harmonic(a11(J1,I2,I3),a11(J1-1,I2,I3));
    }
      
    J2 = Range(I2.getBase()  ,I2.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a22(I1,J2,I3) = (factor*d12(axis2))*ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
    }
    else   
    {
      a22(I1,J2,I3) = (factor*d12(axis2))*mgop.harmonic(a22(I1,J2,I3),a22(I1,J2-1,I3));
    }
      
    J3 = Range(I3.getBase()  ,I3.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
      a33(I1,I2,J3) = (factor*d12(axis3))*ARITHMETIC_AVERAGE (a33,I1,I2,J3,I1,I2,J3-1);    
    }
    else
    {
      a33(I1,I2,J3) = (factor*d12(axis3))*mgop.harmonic(a33(I1,I2,J3),a33(I1,I2,J3-1));
    }
      
    a11.reshape(1,J1,J2,J3);
    a22.reshape(1,J1,J2,J3);
    a33.reshape(1,J1,J2,J3);
      
    // The inverse of the jacobian
    RealDistributedArray jInverse = evaluate(1./j(I1,I2,I3));
    jInverse.reshape(1,I1,I2,I3);

    // Assign matrix values
    derivative(MCE(-1,-1,-1),I1,I2,I3) = 0.;
    derivative(MCE(+1,-1,-1),I1,I2,I3) = 0.;     
    derivative(MCE(-1,+1,-1),I1,I2,I3) = 0.;      
    derivative(MCE(+1,+1,-1),I1,I2,I3) = 0.;
    derivative(MCE(-1,-1,+1),I1,I2,I3) = 0.;
    derivative(MCE(+1,-1,+1),I1,I2,I3) = 0.;
    derivative(MCE(-1,+1,+1),I1,I2,I3) = 0.;
    derivative(MCE(+1,+1,+1),I1,I2,I3) = 0.;

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
      
    derivative(MCE( 0, 0,-1),I1,I2,I3) = -a33(0,I1  ,I2  ,I3  )*jInverse;
					
    derivative(MCE( 0, 0,+1),I1,I2,I3) =  a33(0,I1  ,I2  ,I3+1)*jInverse;
					
    derivative(MCE( 0,-1, 0),I1,I2,I3) = -a22(0,I1  ,I2  ,I3  )*jInverse;
      
    derivative(MCE( 0,+1, 0),I1,I2,I3) =  a22(0,I1  ,I2+1,I3  )*jInverse;
 
    derivative(MCE(-1, 0, 0),I1,I2,I3) = -a11(0,I1  ,I2  ,I3  )*jInverse;
      
    derivative(MCE(+1, 0, 0),I1,I2,I3) =  a11(0,I1+1,I2  ,I3  )*jInverse;
      
    derivative(MCE( 0, 0, 0),I1,I2,I3) =  (a33(0,I1  ,I2  ,I3+1) -  a33(0,I1  ,I2  ,I3  ) +
					   a22(0,I1  ,I2+1,I3  ) -  a22(0,I1  ,I2  ,I3  ) +
					   a11(0,I1+1,I2  ,I3  ) -  a11(0,I1  ,I2  ,I3  ))*jInverse;
  }
}


void 
divVectorScalarFDerivCoefficients(RealDistributedArray & derivative,
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
  if( mgop.isRectangular() && FALSE )
  { 
    if( orderOfAccuracy==2 )
    {
      divVectorScalarFDerivCoefficients2(derivative,s,I1,I2,I3,E,C,mgop );

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
      divVectorScalarFDerivCoefficients2(derivative,s,I1,I2,I3,E,C,mgop );
    }
    else   // ====== 4th order =======
    {
      Overture::abort("error");
    }
  }
  // time=getCPU()-time;
  // printf("divVectorScalarFDerivCoefficients: time = %e \n",time);
  
  // now make copies for other components
  Index M(0,stencilSize);
  for( int c=C.getBase(); c<=C.getBound(); c++ )                        
  for( int e=E.getBase(); e<=E.getBound(); e++ )                        
    if( c!=c0 || e!=e0 )
      derivative(M+CE(c,e),I1,I2,I3)=derivative(M+CE(c0,e0),I1,I2,I3);
}


#undef CE
