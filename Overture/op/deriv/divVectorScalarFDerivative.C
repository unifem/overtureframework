//================================================================================
//
//  Author:  Bill Wangard
//
//  Date:    August 26, 1999
//
//  NOTES: 
//
//  Implements conservative difference approximation to div (S phi)
//  where S is a known vector and phi is the unknown (n-component) scalar function.
//
//  Fully tested   3-D ... yes
//                 2-D ... yes 
//                 1-D ... no
//
//  To do:  Implement 4th-order flavors.
//
//================================================================================

#include "MappedGridOperators.h"
#include "xD.h"

#undef ARITHMETIC_AVERAGE
#define ARITHMETIC_AVERAGE(s,I1,I2,I3,J1,J2,J3) (s(I1,I2,I3) + s(J1,J2,J3))

// undef HARMONIC_AVERAGE 
// define HARMONIC_AVERAGE(s,I1,I2,I3,J1,J2,J3) (s(I1,I2,I3)*s(J1,J2,J3)/(s(I1,I2,I3)+s(J1,J2,J3)+REAL_EPSILON))

void 
divVectorScalarFDerivative43(const realMappedGridFunction & ugf,
                                        const realMappedGridFunction & s,
                                        RealDistributedArray & derivative,
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
divVectorScalarFDerivative42(const realMappedGridFunction & ugf,
                                        const realMappedGridFunction & s,
                                        RealDistributedArray & derivative,
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
divVectorScalarFDerivative23(const realMappedGridFunction & ugf,
                                          const realMappedGridFunction & s,
                                          RealDistributedArray & derivative,
                                          const Index & I1,
                                          const Index & I2,
                                          const Index & I3,
                                          const Index & N,
                                          MappedGridOperators & mgop )
// 2nd-order, 3d
{                                                                        
//           _
//     div ( S phi )  = d/dr ( a11 phi ) + d/ds (a22 phi) + d/dt (a33 phi)
//
//                a11 = J (rx Sx + ry Sy + rz Sz)
//                a22 = J (sx Sx + sy Sy + sz Sz)
//                a33 = J (tx Sx + ty Sy + tz Sz)
//                      _            _ 
//                where S = scalar * V,  where V is a vector, but is likely to be the fluid velocity.
//
//                      _       _       _       _
//                      S = (Sx ex + Sy ey + Sz ez)
//
//                     rx = dr/dx
//                     ry = dr/dy
//                     rz = dr/dz

    int numberOfDimensions = mgop.mappedGrid.numberOfDimensions();

    // cast to a RealDistributedArray for efficiency
    RealDistributedArray & u = (RealDistributedArray&) ugf;   
    
    RealDistributedArray & inverseVertexDerivative = 
        int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
        : mgop.mappedGrid.inverseCenterDerivative();


    // Face value averaging factor ( = 0.5 for arithmetic,  2.0 for harmonic averaging)
    const real factor = mgop.getAveragingType()== 
                        GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;

    // Cell Spacing
//    RealArray & d12 = mgop.d12;  // 1/ (2 dx)
    RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
    
    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
    Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
    Index J3 = Range(I3.getBase()-1,I3.getBound()+1);

    const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();

    RealDistributedArray a11(J1,J2,J3), a22(J1,J2,J3), a33(J1,J2,J3);
    
    a11 = (RX(J1,J2,J3) * s(J1,J2,J3,0) 
         + RY(J1,J2,J3) * s(J1,J2,J3,1) 
         + RZ(J1,J2,J3) * s(J1,J2,J3,2)) * j(J1,J2,J3);

    a22 = (SX(J1,J2,J3) * s(J1,J2,J3,0) 
         + SY(J1,J2,J3) * s(J1,J2,J3,1) 
         + SZ(J1,J2,J3) * s(J1,J2,J3,2)) * j(J1,J2,J3);

    a33 = (TX(J1,J2,J3) * s(J1,J2,J3,0) 
         + TY(J1,J2,J3) * s(J1,J2,J3,1) 
         + TZ(J1,J2,J3) * s(J1,J2,J3,2)) * j(J1,J2,J3);

    // Get face-averaged values of the a11s
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
    
    // Get face-averaged values of the a22s
    J2 = Range(I2.getBase()  ,I2.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
        a22(I1,J2,I3) = factor * ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
    }
    else    
    {
//        a22(I1,J2,I3) = factor * HARMONIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
        a22(I1,J2,I3) = factor * mgop.harmonic(a22(I1,J2,I3),a22(I1,J2-1,I3));
    }   

    // Get face-averaged values of the a33s
    J3 = Range(I3.getBase()  ,I3.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
        a33(I1,I2,J3) = factor * ARITHMETIC_AVERAGE (a33,I1,I2,J3,I1,I2,J3-1);
    }
    else    
    {
//        a33(I1,I2,J3) = factor * HARMONIC_AVERAGE (a33,I1,I2,J3,I1,I2,J3-1);
        a33(I1,I2,J3) = factor * mgop.harmonic(a33(I1,I2,J3),a33(I1,I2,J3-1));
    }
    

    // The inverse of the jacobian
    const RealDistributedArray & jInverse = evaluate(1./j(I1,I2,I3));
    
    // Evaluate the derivative
    for( int n=N.getBase(); n<=N.getBound(); n++ )                        
    {
        derivative(I1,I2,I3,n)=
            (
                 (a11(I1+1,I2  ,I3  )*(u(I1+1,I2  ,I3  ,n)+u(I1  ,I2  ,I3  ,n)) -
                 a11(I1  ,I2  ,I3  )*(u(I1  ,I2  ,I3  ,n)+u(I1-1,I2  ,I3  ,n)) ) * d12(axis1)
            +
                 (a22(I1  ,I2+1,I3  )*(u(I1  ,I2+1,I3  ,n)+u(I1  ,I2  ,I3  ,n)) -
                 a22(I1  ,I2  ,I3  )*(u(I1  ,I2  ,I3  ,n)+u(I1  ,I2-1,I3  ,n)) ) * d12(axis2)
            +
                 (a33(I1  ,I2  ,I3+1)*(u(I1  ,I2  ,I3+1,n)+u(I1  ,I2  ,I3  ,n)) -
                 a33(I1  ,I2  ,I3  )*(u(I1  ,I2  ,I3  ,n)+u(I1  ,I2  ,I3-1,n)) ) * d12(axis3)
            ) * jInverse;
    }
    
}


void 
divVectorScalarFDerivative23R(const realMappedGridFunction & ugf,
                              const realMappedGridFunction & s,
                              RealDistributedArray & derivative,
                              const Index & I1,
                              const Index & I2,
                              const Index & I3,
                              const Index & N,
                              MappedGridOperators & mgop )
//
// 2nd-order, 3-D, Rectangular grid (optimized)
//
{
//     div ( S phi )  = d/dx ( Sx phi ) + d/dy ( Sy phi) + d/dz ( Sz phi)
//
//                where S = scalar * V,  where V is a vector, but is likely to be the fluid velocity.
//                      _       _       _       _
//                      S = (Sx ex + Sy ey + Sz ez)
//



    // cast to a RealDistributedArray for efficiency
    RealDistributedArray & u = (RealDistributedArray&) ugf;   

    // Face value averaging factor ( = 0.5 for arithmetic,  2.0 for harmonic averaging)
    const real factor = mgop.getAveragingType()== 
                        GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;

    // Cell Spacing
    real h21c[3];
    for( int axis=0; axis<3; axis++ )
      h21c[axis]= 1./(2.*mgop.dx[axis]); 
#define h21(n) h21c[n]
    
    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
    Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
    Index J3 = Range(I3.getBase()-1,I3.getBound()+1);

    const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();

    RealDistributedArray a11(J1,J2,J3), a22(J1,J2,J3), a33(J1,J2,J3);
    
    a11 = s(J1,J2,J3,0);
    a22 = s(J1,J2,J3,1);
    a33 = s(J1,J2,J3,2);

    // Get face-averaged values of the a11s
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
    
    // Get face-averaged values of the a22s
    J2 = Range(I2.getBase()  ,I2.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
        a22(I1,J2,I3) = factor * ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
    }
    else    
    {
//        a22(I1,J2,I3) = factor * HARMONIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
        a22(I1,J2,I3) = factor * mgop.harmonic(a22(I1,J2,I3),a22(I1,J2-1,I3));
    }   

    // Get face-averaged values of the a33s
    J3 = Range(I3.getBase()  ,I3.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
        a33(I1,I2,J3) = factor * ARITHMETIC_AVERAGE (a33,I1,I2,J3,I1,I2,J3-1);
    }
    else    
    {
//        a33(I1,I2,J3) = factor * HARMONIC_AVERAGE (a33,I1,I2,J3,I1,I2,J3-1);
        a33(I1,I2,J3) = factor * mgop.harmonic(a33(I1,I2,J3),a33(I1,I2,J3-1));
    }

    // Evaluate the derivative
    for( int n=N.getBase(); n<=N.getBound(); n++ )                        
    {
        derivative(I1,I2,I3,n)=
            (
                 (a11(I1+1,I2  ,I3  )*(u(I1+1,I2  ,I3  ,n)+u(I1  ,I2  ,I3  ,n)) -
                 a11(I1  ,I2  ,I3  )*(u(I1  ,I2  ,I3  ,n)+u(I1-1,I2  ,I3  ,n)) ) * h21(axis1)
            +
                 (a22(I1  ,I2+1,I3  )*(u(I1  ,I2+1,I3  ,n)+u(I1  ,I2  ,I3  ,n)) -
                 a22(I1  ,I2  ,I3  )*(u(I1  ,I2  ,I3  ,n)+u(I1  ,I2-1,I3  ,n)) ) * h21(axis2)
            +
                 (a33(I1  ,I2  ,I3+1)*(u(I1  ,I2  ,I3+1,n)+u(I1  ,I2  ,I3  ,n)) -
                 a33(I1  ,I2  ,I3  )*(u(I1  ,I2  ,I3  ,n)+u(I1  ,I2  ,I3-1,n)) ) * h21(axis3)
            );
    }
}

void 
divVectorScalarFDerivative22(const realMappedGridFunction & ugf,
                               const realMappedGridFunction & s,
                               RealDistributedArray & derivative,
                               const Index & I1,
                               const Index & I2,
                               const Index & I3,
                               const Index & N,
                               MappedGridOperators & mgop )
// 2nd-order, 2d
{                                                                        
    
//     div ( u phi )  = d/dr ( a11 phi ) + d/ds (a22 phi)
//
//                a11 = J (rx Sx + ry Sy)
//                a22 = J (sx Sx + sy Sy)
//                      _            _ 
//                where S = scalar * V
//                      _       _       _
//                      S = (Sx ex + Sy ey )
//
//                     rx = dr/dx
//                     ry = dr/dy

    int numberOfDimensions = mgop.mappedGrid.numberOfDimensions();
    // cast to a RealDistributedArray for efficiency
    RealDistributedArray & u = (RealDistributedArray&) ugf;   
    
    RealDistributedArray & inverseVertexDerivative = 
        int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
        : mgop.mappedGrid.inverseCenterDerivative();


    // Face value averaging factor ( = 0.5 for arithmetic,  2.0 for harmonic averaging)
    const real factor = mgop.getAveragingType()== 
                        GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;

    // Cell Spacing
//    RealArray & d12 = mgop.d12;  // 1/ (2 dx)
    RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
    
    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
    Index J2 = Range(I2.getBase()-1,I2.getBound()+1);

    const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();

    RealDistributedArray a11(J1,J2,I3), a22(J1,J2,I3);
    
    a11 = (RX(J1,J2,I3) * s(J1,J2,I3,0) + 
           RY(J1,J2,I3) * s(J1,J2,I3,1)) * j(J1,J2,I3);

    a22 = (SX(J1,J2,I3) * s(J1,J2,I3,0) + 
           SY(J1,J2,I3) * s(J1,J2,I3,1)) * j(J1,J2,I3);

    // Get face-averaged values of the a11s
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
    
    // Get face-averaged values of the a22s
    J2 = Range(I2.getBase()  ,I2.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
        a22(I1,J2,I3) = factor * ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
    }
    else    
    {
//        a22(I1,J2,I3) = factor * HARMONIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
        a22(I1,J2,I3) = factor * mgop.harmonic(a22(I1,J2,I3),a22(I1,J2-1,I3));
    }   


    // The inverse of the jacobian
    const RealDistributedArray & jInverse = evaluate(1./j(I1,I2,I3));
    
    // Evaluate the derivative
    for( int n=N.getBase(); n<=N.getBound(); n++ )                        
    {
        derivative(I1,I2,I3,n)=
            (
                 (a11(I1+1,I2  ,I3  )*(u(I1+1,I2  ,I3  ,n)+u(I1  ,I2  ,I3  ,n)) -
                  a11(I1  ,I2  ,I3  )*(u(I1  ,I2  ,I3  ,n)+u(I1-1,I2  ,I3  ,n))) * d12(axis1)
            +
                 (a22(I1  ,I2+1,I3  )*(u(I1  ,I2+1,I3  ,n)+u(I1  ,I2  ,I3  ,n)) -
                  a22(I1  ,I2  ,I3  )*(u(I1  ,I2  ,I3  ,n)+u(I1  ,I2-1,I3  ,n))) * d12(axis2) 
            ) * jInverse;
    }
    
}


void 
divVectorScalarFDerivative22R(const realMappedGridFunction & ugf,
                              const realMappedGridFunction & s,
                              RealDistributedArray & derivative,
                              const Index & I1,
                              const Index & I2,
                              const Index & I3,
                              const Index & N,
                              MappedGridOperators & mgop )
//
// 2nd-order, 2-D, Rectangular grid (optimized)
//
{
//     div ( S phi )  = d/dx ( Sx phi ) + d/dy ( Sy phi) 
//
//                where S = scalar * V,  where V is a vector, but is likely to be the fluid velocity.
//                      _       _       _  
//                      S = (Sx ex + Sy ey)
//



    // cast to a RealDistributedArray for efficiency
    RealDistributedArray & u = (RealDistributedArray&) ugf;   

    // Face value averaging factor ( = 0.5 for arithmetic,  2.0 for harmonic averaging)
    const real factor = mgop.getAveragingType()== 
                        GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;

    // Cell Spacing
//    RealArray & h21 = mgop.h21;  // 1/ (2 dx)
    real h21c[3];
    for( int axis=0; axis<3; axis++ )
      h21c[axis]= 1./(2.*mgop.dx[axis]); 
#define h21(n) h21c[n]
    
    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
    Index J2 = Range(I2.getBase()-1,I2.getBound()+1);

    const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();

    RealDistributedArray a11(J1,J2,I3), a22(J1,J2,I3);
    
    a11 = s(J1,J2,I3,0);
    a22 = s(J1,J2,I3,1);

    // Get face-averaged values of the a11s
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
    
    // Get face-averaged values of the a22s
    J2 = Range(I2.getBase()  ,I2.getBound()+1);
    if( mgop.getAveragingType()==GenericMappedGridOperators::arithmeticAverage )
    {
        a22(I1,J2,I3) = factor * ARITHMETIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
    }
    else    
    {
//        a22(I1,J2,I3) = factor * HARMONIC_AVERAGE (a22,I1,J2,I3,I1,J2-1,I3);
        a22(I1,J2,I3) = factor * mgop.harmonic(a22(I1,J2,I3),a22(I1,J2-1,I3));
    }   

    // Evaluate the derivative
    for( int n=N.getBase(); n<=N.getBound(); n++ )                        
    {
        derivative(I1,I2,I3,n)=
            (
                 (a11(I1+1,I2  ,I3  )*(u(I1+1,I2  ,I3  ,n)+u(I1  ,I2  ,I3  ,n)) -
                 a11(I1  ,I2  ,I3  )*(u(I1  ,I2  ,I3  ,n)+u(I1-1,I2  ,I3  ,n)) ) * h21(axis1)
            +
                 (a22(I1  ,I2+1,I3  )*(u(I1  ,I2+1,I3  ,n)+u(I1  ,I2  ,I3  ,n)) -
                 a22(I1  ,I2  ,I3  )*(u(I1  ,I2  ,I3  ,n)+u(I1  ,I2-1,I3  ,n)) ) * h21(axis2)
            );
    }
}

void 
divVectorScalarFDerivative21(const realMappedGridFunction & ugf,
                             const realMappedGridFunction & s,
                             RealDistributedArray & derivative,
                             const Index & I1,
                             const Index & I2,
                             const Index & I3,
                             const Index & N,
                             MappedGridOperators & mgop )
// 2nd-order, 1d
{                                                                        
    
//     div ( u phi )  = d/dr ( a11 phi ) 
//
//                a11 = J (rx Sx )
//                      _            _ 
//                where S = scalar * V
//
//                      S = (Sx ex)
//
//                     rx = dr/dx


    // cast to a RealDistributedArray for efficiency
    RealDistributedArray & u = (RealDistributedArray&) ugf;   
    
    RealDistributedArray & inverseVertexDerivative = 
        int(mgop.mappedGrid.isAllVertexCentered()) ? mgop.mappedGrid.inverseVertexDerivative()  
        : mgop.mappedGrid.inverseCenterDerivative();


    // Face value averaging factor ( = 0.5 for arithmetic,  2.0 for harmonic averaging)
    const real factor = mgop.getAveragingType()== 
                        GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;

    // Cell Spacing
    RealArray d12; d12 = 1./(2.*mgop.mappedGrid.gridSpacing()); // mgop.d12;
    
    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);

    const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();

    RealDistributedArray a11(J1,I2,I3);
    
    a11 = RX(J1,I2,I3) * s(J1,I2,I3,0) * j(J1,I2,I3);

    // Get face-averaged values of the a11s
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
    

    // The inverse of the jacobian
    const RealDistributedArray & jInverse = evaluate(1./j(I1,I2,I3));
    
    // Evaluate the derivative
    for( int n=N.getBase(); n<=N.getBound(); n++ )                        
    {
        derivative(I1,I2,I3,n)=
            (
                 (a11(I1+1,I2  ,I3  )*(u(I1+1,I2  ,I3  ,n)+u(I1  ,I2  ,I3  ,n)) -
                  a11(I1  ,I2  ,I3  )*(u(I1  ,I2  ,I3  ,n)+u(I1-1,I2  ,I3  ,n))) * d12(axis1)
            ) * jInverse;
    }
    
}

void 
divVectorScalarFDerivative21R(const realMappedGridFunction & ugf,
                              const realMappedGridFunction & s,
                              RealDistributedArray & derivative,
                              const Index & I1,
                              const Index & I2,
                              const Index & I3,
                              const Index & N,
                              MappedGridOperators & mgop )
//
// 2nd-order, 1-D, Rectangular grid (optimized)
//
{
//     div ( S phi )  = d/dx ( Sx phi )
//
//                where S = scalar * V,  where V is a vector, but is likely to be the fluid velocity.
//                      _       _   
//                      S = (Sx ex )
//



    // cast to a RealDistributedArray for efficiency
    RealDistributedArray & u = (RealDistributedArray&) ugf;   

    // Face value averaging factor ( = 0.5 for arithmetic,  2.0 for harmonic averaging)
    const real factor = mgop.getAveragingType()== 
                        GenericMappedGridOperators::arithmeticAverage ? (1./2.) : 2.;

    // Cell Spacing
    real h21c[3];
    for( int axis=0; axis<3; axis++ )
      h21c[axis]= 1./(2.*mgop.dx[axis]); 
#define h21(n) h21c[n]
    
    Index J1 = Range(I1.getBase()-1,I1.getBound()+1);

    const RealDistributedArray & j = mgop.mappedGrid.centerJacobian();

    RealDistributedArray a11(J1,I2,I3);
    
    a11 = s(J1,I2,I3,0);

    // Get face-averaged values of the a11s
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
    
    // Evaluate the derivative
    for( int n=N.getBase(); n<=N.getBound(); n++ )                        
    {
        derivative(I1,I2,I3,n)=
            (
                 (a11(I1+1,I2  ,I3  )*(u(I1+1,I2  ,I3  ,n)+u(I1  ,I2  ,I3  ,n)) -
                 a11(I1  ,I2  ,I3  )*(u(I1  ,I2  ,I3  ,n)+u(I1-1,I2  ,I3  ,n)) ) * h21(axis1)
            );
    }
}

void 
divVectorScalarFDerivative(const realMappedGridFunction & ugf,
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
	divVectorScalarFDerivative21R(ugf,s,derivative,I1,I2,I3,N,mgop );  // 1-D Rect
      }
      else if(numberOfDimensions==2 )
      {   
	divVectorScalarFDerivative22R(ugf,s,derivative,I1,I2,I3,N,mgop );  // 2-D Rect
      }
      else 
      {   
	divVectorScalarFDerivative23R(ugf,s,derivative,I1,I2,I3,N,mgop );  // 3-D Rect
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
	divVectorScalarFDerivative21(ugf,s,derivative,I1,I2,I3,N,mgop );
      else if(numberOfDimensions==2 )
	divVectorScalarFDerivative22(ugf,s,derivative,I1,I2,I3,N,mgop );
      else // ======= 3D ================
	divVectorScalarFDerivative23(ugf,s,derivative,I1,I2,I3,N,mgop );
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
   divVectorScalarFDerivative42(ugf,s,derivative,I1,I2,I3,N,mgop );
                
   }
   else  // ======= 3D ================
   {
   divVectorScalarFDerivative43(ugf,s,derivative,I1,I2,I3,N,mgop );
   }
   -----	    */ 
    }
  }
}


#undef ARITHMETIC_AVERAGE

