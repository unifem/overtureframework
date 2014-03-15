#include "Oges.h"
#ifdef GETLENGTH
#define GET_LENGTH dimension
#else
#define GET_LENGTH getLength
#endif

//=======================================================================
// Define discrete coefficients for Oges
//
//=======================================================================

// MERGE0 : use for A++ operations when the first index is a scalar
//      a(i0,I1,I2,I3)=
// you must define the following in your code
//  int dum;
//  Range aR0,aR1,aR2,aR3;
#define MERGE0(a,i0,I1,I2,I3) \
  for(  \
      aR0=Range(a.getBase(0),a.getBound(0)),   \
      aR1=Range(a.getBase(1),a.getBound(1)),   \
      aR2=Range(a.getBase(2),a.getBound(2)),   \
      aR3=Range(a.getBase(3),a.getBound(3)),   \
      a.reshape(Range(0,aR0.length()*aR1.length()-1),aR2,aR3), \
      dum=0; dum<1; dum++,  \
      a.reshape(aR0,aR1,aR2,aR3) ) \
    a(Index(i0-aR0.getBase()+aR0.length()*(I1.getBase()-aR1.getBase()),   \
      I1.length(),aR0.length()),I2,I3)


#define M123(m1,m2,m3) (m1+halfWidth+width*(m2+halfWidth+width*(m3+halfWidth3)))
#undef  M123N
#define M123N(m1,m2,m3,n) (M123(m1,m2,m3)+stencilLength0*(n))
// define COEFF(I1,I2,I3,m1,m2,m3) coeff0(I1,I2,I3,MN(m1,m2,m3))
// define EQUATIONNUMBER(I1,I2,I3,m1,m2,m3) equationNumber0(I1,I2,I3,MN(m1,m2,m3))

// Use this for A++ index operations:
#undef COEFF
#define COEFF(m1,m2,m3,n,I1,I2,I3) MERGE0(coeff0,M123N(m1,m2,m3,n),I1,I2,I3)
#undef EQUATIONNUMBER
#define EQUATIONNUMBER(m1,m2,m3,n,I1,I2,I3) MERGE0(equationNumber0,M123N(m1,m2,m3,n),I1,I2,I3)
// Scalar indexing:
#undef COEFFS
#define COEFFS(m1,m2,m3,n,I1,I2,I3) coeff0(M123N(m1,m2,m3,n),I1,I2,I3)
#undef EQUATIONNUMBERS
#define EQUATIONNUMBERS(m1,m2,m3,n,I1,I2,I3) equationNumber0(M123N(m1,m2,m3,n),I1,I2,I3)


// include "ogesux2.h"  // define 2nd order derivatives for vertex centred

#define ForBoundary(side,axis)   for( axis=0; axis<numberOfDimensions; axis++ ) \
                                 for( side=0; side<=1; side++ )

#define ForStencil(m1,m2,m3)   \
    for( m3=-halfWidth3; m3<=halfWidth3; m3++) \
    for( m2=-halfWidth2; m2<=halfWidth2;  m2++) \
    for( m1=-halfWidth1; m1<=halfWidth1;  m1++) 

#define ForAllGridPoints( i1,i2,i3 ) \
  for( i3=c.dimension()(Start,axis3); i3<=c.dimension()(End,axis3); i3++ ) \
  for( i2=c.dimension()(Start,axis2); i2<=c.dimension()(End,axis2); i2++ ) \
  for( i1=c.dimension()(Start,axis1); i1<=c.dimension()(End,axis1); i1++ )

 
#define CELL_AVE(u,i1,i2,i3) \
                    (.25*( u(i1,i2,i3)+u(i1+1,i2,i3)+u(i1,i2+1,i3)+u(i1+1,i2+1,i3) ))
//  Define's for conservation form:

#define B11(I1,I2,I3) \
                    ( (SQR(RX(I1,I2,I3))*alpha1(I1,I2,I3) \
                      +SQR(RY(I1,I2,I3))*alpha2(I1,I2,I3))*detxr(I1,I2,I3) )
#define B12(I1,I2,I3) \
                    ( (RX(I1,I2,I3)*SX(I1,I2,I3)*alpha1(I1,I2,I3) \
                      +RY(I1,I2,I3)*SY(I1,I2,I3)*alpha2(I1,I2,I3))*detxr(I1,I2,I3) )
#define B21(I1,I2,I3) A12(I1,I2,I3)
#define B22(I1,I2,I3) \
                    ( (SQR(SX(I1,I2,I3))*alpha1(I1,I2,I3)  \
                      +SQR(SY(I1,I2,I3))*alpha2(I1,I2,I3))*detxr(I1,I2,I3) )
#define DM1(u,i1,i2,i3) ((u(i1,i2,i3)-u(i1,i2-1,i3))/c.gridSpacing()(axis1))
#define DM2(u,i1,i2,i3) ((u(i1,i2,i3)-u(i1,i2-1,i3))/c.gridSpacing()(axis2))

#define MU1(u,i1,i2,i3) (.5*u(i1+1,i2,i3)+.5*u(i1,i2,i3))
#define MU2(u,i1,i2,i3) (.5*u(i1,i2+1,i3)+.5*u(i1,i2,i3))

//===============================================================================
//===============================================================================
void Oges::laplaceDirichletCellConservative( const int grid )
{

  realMappedGridFunction & coeff0 = Oges::coeff[grid];
  intMappedGridFunction & equationNumber0 = Oges::equationNumber[grid];
  MappedGrid & c = cg[grid];
  int stencilLength0 = coeff0.GET_LENGTH(axis1);

  // Check to see if there is enough space in the arrays
  if( stencilLength0 < pow(3,numberOfDimensions)*numberOfComponents )
  {
    cerr << "laplaceDirichletCellConservative:ERROR stencilLength0 is too small! " << endl;
    exit(1);
  }

/* --------------
  RealArray alpha1(R1,R2,R3), alpha2(R1,R2,R3);
  alpha1=1.;
  alpha2=1.;
	
  // detxr = det( dx/dr )
  RealArray detxr(R1,R2,R3);
  detxr(I1,I2,I3)=1./(RX(I1,I2,I3)*SY(I1,I2,I3)-RY(I1,I2,I3)*SX(I1,I2,I3));
  ForStencil(m1,m2,m3)
    COEFF(m1,m2,m3,n,I1,I2,I3)=(
      MU2(B11,I1,I2,I3)*URR2(I1,I2,I3,m1,m2,m3)     // (a11*u_r)_r
       +D01(B11,I1,I2,I3)*UR2(I1,I2,I3,m1,m2,m3)
     +MU12(B12,I1,I2,I3)*URS2(I1,I2,I3,m1,m2,m3)        // (a12*u_s)_r
       +D01(A12,I1,I2,I3)*US2(I1-1,I2,I3,m1,m2,m3)
     +AVE2(A22,I1,I2,I3)*USS2(I1,I2,I3,m1,m2,m3)     // (a22*u_s)_s
       +D02(A22,I1,I2,I3)*US2(I1,I2,I3,m1,m2,m3)
     +A21(I1,I2+1,I3)*URS2(I1,I2,I3,m1,m2,m3)        // (a21*u_r)_s
       +D02(A21,I1,I2,I3)*UR2(I1,I2-1,I3,m1,m2,m3)

  // Now do Boundary Points:
  ForBoundary(side,axis)
  { // Dirchlet BC: average ghost cell + first line in
    if( c.boundaryCondition()(side,axis) > 0 )
    {
      // Apply equation on ghost line!
      getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);   // Index's for ghost points
      int n1= axis==axis1 ? 1-2*side : 0;
      int n2= axis==axis2 ? 1-2*side : 0;
      int n3= axis==axis3 ? 1-2*side : 0;
      m1=-halfWidth1;  m2=-halfWidth2;   m3=-halfWidth3;  
      COEFF(m1  ,m2,m3,n,Ig1,Ig2,Ig3)=.5;
      COEFF(m1+1,m2,m3,n,Ig1,Ig2,Ig3)=.5;
      EQUATIONNUMBER(m1  ,m2,m3,n,Ig1,Ig2,Ig3)=equationNo(n,Ig1,Ig2,Ig3,grid);
      EQUATIONNUMBER(m1+1,m2,m3,n,Ig1,Ig2,Ig3)=equationNo(n,Ig1+n1,Ig2+n2,Ig3+n3,grid);
      EQUATIONNUMBER(m1+2,m2,m3,n,Ig1,Ig2,Ig3)=0;  // signals last entry
    }
  }
------------------- */

}

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
void Oges::laplaceNeumannCellConservative( const int grid )
{

  realMappedGridFunction & coeff0 = Oges::coeff[grid];
  intMappedGridFunction & equationNumber0 = Oges::equationNumber[grid];
  MappedGrid & c = cg[grid];
  int stencilLength0 = coeff0.GET_LENGTH(axis1);

}

