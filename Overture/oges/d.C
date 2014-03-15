//
// Try out some Discretization approaches
//


#include <A++.h>

// Here we use single precision
typedef float real;
typedef floatArray RealArray;  


#define RX(I1,I2,I3,n1,n2) rx(I1,I2,I3,(n1)+numberOfDimensions*(n2))

#define DX1(I1,I2,I3,m1,m2,m3) Dx(I1,I2,I3,(m1)+numberOfDimensions*(m2+numberOfDimensions*(m3)))
#define DXX1(I1,I2,I3,m1,m2,m3) Dxx(I1,I2,I3,(m1)+numberOfDimensions*(m2+numberOfDimensions*(m3)))

#define rx_(n1,n2) (n1)+numberOfDimensions*(n2)
#define Dx_(m1,m2,m3) (m1)+numberOfDimensions*(m2+numberOfDimensions*(m3))
#define Dxx_(m1,m2,m3) (m1)+numberOfDimensions*(m2+numberOfDimensions*(m3))
#define C_(m1,m2,m3) (m1)+numberOfDimensions*(m2+numberOfDimensions*(m3))


#define DX(I1,I2,I3,m1,m2,m3) (rx(I1,I2,I3,rx_(0,0))*Dr(m1,0)  \
                              +rx(I1,I2,I3,rx_(1,0))*Dr(m2,1)  \
                              +rx(I1,I2,I3,rx_(2,0))*Dr(m2,2))

#define DY(I1,I2,I3,m1,m2,m3) rx(I1,I2,I3,rx_(1,1))*Dr(m2,1)

#define DXX(I1,I2,I3,m1,m2,m3) pow(rx(I1,I2,I3,rx_(1,1)),2)*Dr(m1,0)
#define DYY(I1,I2,I3,m1,m2,m3) pow(rx(I1,I2,I3,rx_(1,1)),2)*Dr(m2,1)


#define ForBoundary(side,axis)   for( axis=0; axis<numberOfDimensions; axis++ ) \
                                 for( side=0; side<=1; side++ )

#define ForStencil(m1,m2,m3)    for( m1=0; m1<numberOfDimensions; m1++) \
                                for( m2=0; m2<numberOfDimensions; m2++) \
                                for( m3=0; m3<numberOfDimensions; m3++) 

#define getBoundaryIndex( side,axis,Ib1,Ib2,Ib3,normal) \
    IntegerArray start(3),count(3);          \
    for( int kd=0; kd<3; kd++ )       \
    {   \
      start(kd)=0;   \
      count(kd)=10;   \
    }   \
    start(axis)=1+side*9;   \
    count(axis)=1;   \
    Ib1=Index(start(0),count(0));   \
    Ib2=Index(start(1),count(1));   \
    Ib3=Index(start(2),count(2));   \
    RealArray normal(Range(start(0),count(0)+start(0)-1),   \
                     Range(start(1),count(1)+start(1)-1),   \
                     Range(start(2),count(2)+start(2)-1),Range(0,1));   \
    normal=1.;   \

#define getGhostIndex1( side,axis,Ig1,Ig2,Ig3) \
    {  \
    IntegerArray start(3),count(3);          \
    for( int kd=0; kd<3; kd++ )       \
    {   \
      start(kd)=0;   \
      count(kd)=10;   \
    }   \
    start(axis)=side*10;   \
    count(axis)=1;   \
    Ig1=Index(start(0),count(0));   \
    Ig2=Index(start(1),count(1));   \
    Ig3=Index(start(2),count(2));   \
     }   


#define getInteriorIndex(I1,I2,I3) \
  I1=Index(0,10); \
  I2=Index(0,10); \
  I3=Index(0,10); 
 
#define COEFF(I1,I2,I3,m1,m2,m3) C(I1,I2,I3,C_(m1,m2,m3))


void main()
{

  cout << "====== Starting Test of Discretization Approaches =====" << endl;

  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  int numberOfDimensions=3;
  int orderOfAccuracy=2;
  int width=2*orderOfAccuracy-1;
  
  RealArray Dr(width,numberOfDimensions);
  Dr=1.;
  
  Range R1(0,10); 
  Range R2(0,10); 
  Range R3(0,10); 
  
  RealArray rx(R1,R2,R3,Range(0,numberOfDimensions*numberOfDimensions)); 
  
  rx=1.;
    
  Range Rd(0,27);
  
  RealArray Dx(R1,R2,R3,Rd);
//  RealArray Dx(R1,R2,R3,numberOfDimensions*numberOfDimensions*numberOfDimensions);

  RealArray Dxx(R1,R2,R3,Rd);
  RealArray C(R1,R2,R3,Rd);

  IntegerArray bc(2,3);
  bc=1;


  //-----------------------------------------------

  // Define the Matrix C:
  // Here we do Interior Points

  RealArray Cxx(R1,R2,R3),Cyy(R1,R2,R3);  // coefficient matrices
  
  Cxx=1.;
  Cyy=1.;

  int m1,m2,m3;
  Index I1,I2,I3;

  getInteriorIndex(I1,I2,I3);
  ForStencil(m1,m2,m3)
    COEFF(I1,I2,I3,m1,m2,m3)=Cxx(I1,I2,I3)*DXX(I1,I2,I3,m1,m2,m3)
                            +Cyy(I1,I2,I3)*DYY(I1,I2,I3,m1,m2,m3);

  
  // Now do Boundary Points:

  int side,axis;
  Index Ib1,Ib2,Ib3;

  ForBoundary(side,axis)
  {
    getBoundaryIndex( side,axis,Ib1,Ib2,Ib3,normal);
    if( bc(side,axis)==1 )
      ForStencil(m1,m2,m3)
        COEFF(Ib1,Ib2,Ib3,m1,m2,m3)=normal(Ib1,Ib2,Ib3,0)*DX(Ib1,Ib2,Ib3,m1,m2,m3)
                                   +normal(Ib1,Ib2,Ib3,1)*DY(Ib1,Ib2,Ib3,m1,m2,m3);
    else
      ForStencil(m1,m2,m3)
        COEFF(Ib1,Ib2,Ib3,m1,m2,m3)=normal(Ib1,Ib2,Ib3,0)*DX(Ib1,Ib2,Ib3,m1,m2,m3);
  }


  // Suppose we want to apply an equation at the ghostpoint, centred on the boundary:
  // But we need to tell Oges where the equation is centred!

  Index Ig1,Ig2,Ig3;

  ForBoundary(side,axis)
  {
    getBoundaryIndex( side,axis,Ib1,Ib2,Ib3,normal);
    getGhostIndex1( side,axis,Ig1,Ig2,Ig3);   // Index's for first set of ghost points

    // watch out : are Ig1 and Ib1 the same length?

    if( bc(side,axis)==1 )
      ForStencil(m1,m2,m3)
        COEFF(Ig1,Ig2,Ig3,m1,m2,m3)=normal(Ib1,Ib2,Ib3,0)*DX(Ib1,Ib2,Ib3,m1,m2,m3)
                                   +normal(Ib1,Ib2,Ib3,1)*DY(Ib1,Ib2,Ib3,m1,m2,m3);
    else
      ForStencil(m1,m2,m3)
        COEFF(Ig1,Ig2,Ig3,m1,m2,m3)=normal(Ib1,Ib2,Ib3,0)*DX(Ib1,Ib2,Ib3,m1,m2,m3);
  }



}
