#include "MappedGridOperators.h"
#include "SparseRep.h"
#include "display.h"
#include "MappedGridOperatorsInclude.h"
#include "ParallelUtility.h"
#include "display.h"

// This next include file defines the prototype for assignBoundaryConditions
#include "assignBoundaryConditions.h"

#define FOR_4D(m,i1,i2,i3,M,I1,I2,I3) \
int mBase=M.getBase(), mBound=M.getBound(); \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++) \
for(m=mBase; m<=mBound; m++)

#define FOR_4(m,i1,i2,i3,M,I1,I2,I3) \
mBase=M.getBase(), mBound=M.getBound(); \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++) \
for(m=mBase; m<=mBound; m++)

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


//! Build the coefficient matrix used to apply the neumann BC
void MappedGridOperators::
buildNeumannCoefficientMatrix(MappedGridOperators & op,
                              realSerialArray & nmCoeff, Index & M, Index &I1, Index &I2,Index &I3,
			      realSerialArray & normalLocal, real b0, real b1, int numberOfDimensions,
                              int side, int axis, const BoundaryConditionParameters & bcParameters )
{
  
  realSerialArray  opX;
  const int n1=0, n2=1, n3=2;
  #ifdef USE_PPP 
    // for now we need to allocate for the entire domain **** fix this ****
    MappedGrid & mg = op.mappedGrid; 
    const intArray & mask = mg.mask();
  
    int n1a,n1b,n2a,n2b,n3a,n3b;
    
    if( axis!=0 )
    {
      n1a = max(I1.getBase() , normalLocal.getBase(0)+mask.getGhostBoundaryWidth(0));
      n1b = min(I1.getBound(),normalLocal.getBound(0)-mask.getGhostBoundaryWidth(0));
    }
    else
    {
      n1a = max(I1.getBase() , normalLocal.getBase(0));
      n1b = min(I1.getBound(),normalLocal.getBound(0));
    }
    if( axis!=1 )
    {
      n2a = max(I2.getBase() , normalLocal.getBase(1)+mask.getGhostBoundaryWidth(1));
      n2b = min(I2.getBound(),normalLocal.getBound(1)-mask.getGhostBoundaryWidth(1));
    }
    else
    {
      n2a = max(I2.getBase() , normalLocal.getBase(1));
      n2b = min(I2.getBound(),normalLocal.getBound(1));
    }
    if( axis!=2 )
    {
      n3a = max(I3.getBase() , normalLocal.getBase(2)+mask.getGhostBoundaryWidth(2));
      n3b = min(I3.getBound(),normalLocal.getBound(2)-mask.getGhostBoundaryWidth(2));
    }
    else
    {
      n3a = max(I3.getBase() , normalLocal.getBase(2));
      n3b = min(I3.getBound(),normalLocal.getBound(2));
    }
    

    if( n1a>n1b || n2a>n2b || n3a>n3b ) return; 

    Index J1 = Range(n1a,n1b), J2 = Range(n2a,n2b), J3 = Range(n3a,n3b);


//    Range D1=normal.dimension(0), D2=normal.dimension(1), D3=normal.dimension(2);

    nmCoeff.redim(M,J1,J2,J3);
    opX.redim(M,J1,J2,J3);
    

    const real nSign = 2*side-1.;  // sign for the normal when we use rx
    const realSerialArray & opXLocal     = opX; 
    const realSerialArray & nmCoeffLocal = nmCoeff; 

  #else
    // index into the normal=vertexBoundaryNormalArray

    Index J1 = I1, J2 = I2, J3 = I3;

    nmCoeff.resize(M,I1,I2,I3);
    opX.redim(M,I1,I2,I3);

    const realSerialArray & opXLocal     = opX;
    const realSerialArray & nmCoeffLocal = nmCoeff;

  #endif


  // ::display(normalLocal(J1,J2,J3,n1),"neumann BC: normalx","%6.2f ");
  // ::display(normalLocal(J1,J2,J3,n2),"neumann BC: normaly","%6.2f ");
  
  const bool useOpt=true;
  if( useOpt )
  {
    real *normalp = normalLocal.Array_Descriptor.Array_View_Pointer3;
    const int normalDim0=normalLocal.getRawDataSize(0);
    const int normalDim1=normalLocal.getRawDataSize(1);
    const int normalDim2=normalLocal.getRawDataSize(2);
    #undef NORMAL
    #define NORMAL(i0,i1,i2,i3) normalp[i0+normalDim0*(i1+normalDim1*(i2+normalDim2*(i3)))]

    real *opXp = opXLocal.Array_Descriptor.Array_View_Pointer3;
    const int opXDim0=opXLocal.getRawDataSize(0);
    const int opXDim1=opXLocal.getRawDataSize(1);
    const int opXDim2=opXLocal.getRawDataSize(2);
    #undef OPX
    #define OPX(i0,i1,i2,i3) opXp[i0+opXDim0*(i1+opXDim1*(i2+opXDim2*(i3)))]

    real *nmCoeffp = nmCoeffLocal.Array_Descriptor.Array_View_Pointer3;
    const int nmCoeffDim0=nmCoeffLocal.getRawDataSize(0);
    const int nmCoeffDim1=nmCoeffLocal.getRawDataSize(1);
    const int nmCoeffDim2=nmCoeffLocal.getRawDataSize(2);
    #undef NMCOEFF
    #define NMCOEFF(i0,i1,i2,i3) nmCoeffp[i0+nmCoeffDim0*(i1+nmCoeffDim1*(i2+nmCoeffDim2*(i3)))]
    

    op.assignCoefficients(MappedGridOperators::xDerivative,opX,J1,J2,J3,0,0);
    // ::display(opXLocal(M,J1,J2,J3),"neumann: opX","%6.2f ");
//      int m;
//      for( m=M.getBase(); m<=M.getBound(); m++ )
//        opXLocal(m,J1,J2,J3)*=normalLocal(0,J1,J2,J3,n1);
//      nmCoeffLocal(M,J1,J2,J3)=opXLocal(M,J1,J2,J3);

    int m,i1,i2,i3;
    FOR_4D(m,i1,i2,i3,M,J1,J2,J3)
    {
      NMCOEFF(m,i1,i2,i3)= OPX(m,i1,i2,i3)*NORMAL(i1,i2,i3,n1);
    }



  //  ::display(opXLocal,"buildNeumannCoefficientMatrix: opXLocal after step 1");
  //  ::display(nmCoeff,"buildNeumannCoefficientMatrix: nmCoeff after step 1");



    if( numberOfDimensions > 1 )
    {
      op.assignCoefficients(MappedGridOperators::yDerivative,opX,J1,J2,J3,0,0);

//        for( m=M.getBase(); m<=M.getBound(); m++ )
//  	opXLocal(m,J1,J2,J3)*=normalLocal(0,J1,J2,J3,n2);
//        nmCoeffLocal(M,J1,J2,J3)+=opXLocal(M,J1,J2,J3);

      FOR_4(m,i1,i2,i3,M,J1,J2,J3)
      {
	NMCOEFF(m,i1,i2,i3)+= OPX(m,i1,i2,i3)*NORMAL(i1,i2,i3,n2);
      }

    }
    if( numberOfDimensions > 2 )
    {
      op.assignCoefficients(MappedGridOperators::zDerivative,opX,J1,J2,J3,0,0);
//        for( m=M.getBase(); m<=M.getBound(); m++ )
//  	opXLocal(m,J1,J2,J3)*=normalLocal(0,J1,J2,J3,n3);
//        nmCoeffLocal(M,J1,J2,J3)+=opXLocal(M,J1,J2,J3);

      FOR_4(m,i1,i2,i3,M,J1,J2,J3)
      {
	NMCOEFF(m,i1,i2,i3)+= OPX(m,i1,i2,i3)*NORMAL(i1,i2,i3,n3);
      }
    }
  
    // -- alter the operator for mixed BC's
//     #ifdef USE_PPP
//       // we need to normalize the normal when using the rx array
//       real scale=b1*nSign;
//       if( numberOfDimensions==1 )
//       {
// 	FOR_4(m,i1,i2,i3,M,J1,J2,J3)
// 	{
// 	  NMCOEFF(m,i1,i2,i3)*= scale/fabs(NORMAL(i1,i2,i3,n1));
// 	}
//       }
//       else if( numberOfDimensions==2 )
//       {
// 	FOR_4(m,i1,i2,i3,M,J1,J2,J3)
// 	{
// 	  NMCOEFF(m,i1,i2,i3)*= scale/sqrt(SQR(NORMAL(i1,i2,i3,n1))+SQR(NORMAL(i1,i2,i3,n2)));
// 	}
//       }
//       else
//       {
// 	FOR_4(m,i1,i2,i3,M,J1,J2,J3)
// 	{
// 	  NMCOEFF(m,i1,i2,i3)*= scale/sqrt(SQR(NORMAL(i1,i2,i3,n1))+SQR(NORMAL(i1,i2,i3,n2))+SQR(NORMAL(i1,i2,i3,n3)));
// 	}
//       }
//     #else

    if( b1 != 1. )
      nmCoeffLocal(M,J1,J2,J3)*=b1;

    if( b0 !=0. )
    {
      op.assignCoefficients(MappedGridOperators::identityOperator,opX,J1,J2,J3,0,0);
      nmCoeffLocal(M,J1,J2,J3)+=b0*opXLocal(M,J1,J2,J3);
    }

  }
  else
  {
    // old way 
    realSerialArray & normalnc = (realSerialArray&)normalLocal; // cast away const
    normalnc.reshape(1,normalnc.dimension(0),normalnc.dimension(1),normalnc.dimension(2),normalnc.dimension(3));


    op.assignCoefficients(MappedGridOperators::xDerivative,opX,I1,I2,I3,0,0);
    ::display(opXLocal(M,J1,J2,J3),"neumann: opX","%6.2f ");

    int m;
    for( m=M.getBase(); m<=M.getBound(); m++ )
      opXLocal(m,J1,J2,J3)*=normalLocal(0,J1,J2,J3,n1);
    nmCoeffLocal(M,J1,J2,J3)=opXLocal(M,J1,J2,J3);
    if( numberOfDimensions > 1 )
    {
      op.assignCoefficients(MappedGridOperators::yDerivative,opX,I1,I2,I3,0,0);
      for( m=M.getBase(); m<=M.getBound(); m++ )
	opXLocal(m,J1,J2,J3)*=normalLocal(0,J1,J2,J3,n2);
      nmCoeffLocal(M,J1,J2,J3)+=opXLocal(M,J1,J2,J3);
    }
    if( numberOfDimensions > 2 )
    {
      op.assignCoefficients(MappedGridOperators::zDerivative,opX,I1,I2,I3,0,0);
      for( m=M.getBase(); m<=M.getBound(); m++ )
	opXLocal(m,J1,J2,J3)*=normalLocal(0,J1,J2,J3,n3);
      nmCoeffLocal(M,J1,J2,J3)+=opXLocal(M,J1,J2,J3);
    }
  
    normalnc.reshape(normalnc.dimension(1),normalnc.dimension(2),normalnc.dimension(3),normalnc.dimension(4));

  // -- alter the operator for mixed BC's
    if( b1 != 1. )
      nmCoeffLocal(M,J1,J2,J3)*=b1;
    if( b0 !=0. )
    {
      op.assignCoefficients(MappedGridOperators::identityOperator,opX,I1,I2,I3,0,0);
      nmCoeffLocal(M,J1,J2,J3)+=b0*opXLocal(M,J1,J2,J3);
    }
  }

  
  //  ::display(nmCoeffLocal,"neumann: nmCoeffLocal at end");

}

// These are for indexing into a coefficient matrix
#undef M2
#define M2(m1,m2) ((m1)+1+3*((m2)+1))
#undef M3
#define M3(m1,m2,m3) ((m1)+1+3*((m2)+1+3*((m3)+1)))

void MappedGridOperators::
applyBCneumann(realMappedGridFunction & u, 
	       const int side,
	       const int axis,
	       const Index & Components,
	       const BCTypes::BCNames & bcType,
	       const int & bc,
	       const real & scalarData,
	       const RealArray & arrayData,
	       const RealArray & arrayDataD,
	       const realMappedGridFunction & gfData,
	       const real & t,
               const IntegerArray & uC, const IntegerArray & fC, IntegerDistributedArray & mask,
	       const BoundaryConditionParameters & bcParameters,
	       const BoundaryConditionOption bcOption,
	       const int & grid  )
// 
// Apply a Neumann BC or mixed boundary condition, (b0 + b1 n.grad) u = g
//
{
  real time0=getCPU();

  if( orderOfAccuracy!=2 )
  {
    printf("MappedGridOperators:: Sorry, the Neumann boundary condition is only implemented for\n"
           " orderOfAccuracy=2, requested orderOfAccuracy=%i. Continuing with 2nd-order\n",orderOfAccuracy);
  }

  MappedGrid & c = mappedGrid;

  if( !boundaryNormalsUsed && !rectangular )
  {
    boundaryNormalsUsed=TRUE;
    mappedGrid.update(MappedGrid::THEvertexBoundaryNormal);
  }
  
  RealDistributedArray & uA = u;
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
  #else
    const realSerialArray & uLocal = u;
  #endif
  #ifdef USE_PPP
    const realSerialArray & normalLocal  = !rectangular? mappedGrid.vertexBoundaryNormalArray(side,axis) : uLocal; 
  #else
    const realSerialArray & normalLocal  = !rectangular? mappedGrid.vertexBoundaryNormal(side,axis) : uLocal; 
  #endif


  int n;
  real b0=0., b1=1.;
  if( bcType==BCTypes::mixed )
  {
    const RealArray & a = bcParameters.a;
    
    if( a.getLength(0) < 2 )
    {
      Overture::abort("MappedGridOperators::applyBoundaryCondition:mixed BC:ERROR: array `a' does not"
                      " have at least 2 values");
    }
    if( a.getLength(1)==2 && a.getLength(2)>=numberOfDimensions && a.getLength(3)>grid )
    {
      b0=a(0,side,axis,grid);
      b1=a(1,side,axis,grid);
    }
    else
    {
      b0=a(0);
      b1=a(1);
    }
    // real rhs = bcOption==arrayForcing ? arrayData(fC(uC.getBase(0)),side,axis,grid) : scalarData;
    // printf("mixed BC: b0=%e, b1=%e, rhs=%e \n",b0,b1,rhs);
    if( b1==0. )
    {
      printF("MappedGridOperators::applyBoundaryCondition:mixed BC:ERROR: a(1)==0 \n");
      Overture::abort("MappedGridOperators::applyBoundaryCondition:mixed BC:ERROR: a(1)==0");
    }
  }

  Index I1,I2,I3;
  getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,0,bcParameters.extraInTangentialDirections);

  typedef int POINTER2[2];
  POINTER2 *coeffIsSet = bcType==BCTypes::neumann ? nCoeffIsSet : mCoeffIsSet; 

//  RealDistributedArray uNew;

  const bool useOpt=true;
  if( useOpt )
  {
    // **********************
    //    use optimised BC 
    // **********************

    #define coeffSaved(side,axis,m) (mCoeffValues[(side)+2*((axis)+3*(m))])

    bool buildMatrix = !( rectangular || numberOfDimensions==1 ) && !coeffIsSet[axis][side];
    if( bcType==BCTypes::mixed && coeffIsSet[axis][side] ) // *wdh* 080724 we need to check if the mixed coeff's b0,b1 have changed!
    {
      buildMatrix = (b0 != coeffSaved(side,axis,0)) || (b1 !=coeffSaved(side,axis,1));

      // if( buildMatrix )
      // 	printF("\n +++MappedGridOperators::neumannBC:mixed BC:INFO: mixed BC coefficients have changed (side,axis,grid)=(%i,%i,%i)! "
      // 	       "Rebuild BC matrix +++ \n\n",side,axis,grid);
    
    }
    if( buildMatrix )
    {
      // *** In the curvilinear case we build a coeff matrix on the boundary to represent the normal derivative
      if( !coeffIsSet[axis][side] )
	createBoundaryMatrix(side,axis,bcType);

      if( buildMatrix )
      { // generate coefficients if they have not already been set
        realSerialArray & nmCoeff = bcType==BCTypes::neumann ? neumannCoeff[axis][side] : mixedDerivativeCoeff[axis][side];
        Index M(0,int(pow(3,numberOfDimensions)+.5));
	coeffIsSet[axis][side]=true; 
	buildNeumannCoefficientMatrix(*this,nmCoeff,M,I1,I2,I3,(realSerialArray&)normalLocal,b0,b1,
                                      numberOfDimensions,side,axis,bcParameters);
	if( bcType==BCTypes::mixed )
	{ // 
	  coeffSaved(side,axis,0)=b0;  // save the coefficients so we can check if they are changed 
	  coeffSaved(side,axis,1)=b1;
	}
      }
    }
    
    real twoDeltaX=1.;
    if( rectangular )
      twoDeltaX = 2.*dx[0]; // 1./h21(axis);
    else if ( c.numberOfDimensions()==1 )
      twoDeltaX=2.*c.vertexDerivative()(I1.getBase(),I2.getBase(),I3.getBase(),0,0)*c.gridSpacing(0);

    #ifdef USE_PPP
      realSerialArray gfDataLocal; getLocalArrayWithGhostBoundaries(gfData,gfDataLocal);
      const realSerialArray & arrayDataDLocal=arrayDataD;
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
      // const intSerialArray & cmaskLocal = c.mask().getLocalArrayWithGhostBoundaries();
    #else
      const realSerialArray & gfDataLocal = gfData;
      const realSerialArray & arrayDataDLocal = arrayDataD;

      const intSerialArray & maskLocal = mask;
      // const intSerialArray & maskLocal = c.mask();
    #endif
//    realSerialArray *gfDatap=(realSerialArray *)(&gfDataLocal);

    
    // *wdh* 040930
    int nv[2][3], &n1a=nv[0][0], &n1b=nv[1][0], &n2a=nv[0][1], &n2b=nv[1][1], &n3a=nv[0][2], &n3b=nv[1][2]; 

    bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b); 
    if( !ok ) return;

    real par[3]={b0,b1,twoDeltaX};
    int ipar[1]={0};
    
    real dr[3];
    for( int dir=0; dir<3; dir++ )
      dr[dir]=c.gridSpacing(dir); 

    realSerialArray *gfDatap=(realSerialArray*)(&gfDataLocal);
    realSerialArray *rhsp=NULL;
    
    // option from : parameter( scalarForcing=0,gfForcing=1,arrayForcing=2, vectorForcing=3 )
    int option=-1;
    if( twilightZoneFlow )
    {
      // Fill in the rhs array with TZ forcing
      option=1;
      Range F(min(fC),max(fC));

      rhsp = new realSerialArray(I1,I2,I3,F);
      realSerialArray & rhs = *rhsp;
      
      gfDatap=&rhs;
      
      // *wdh* 2012/11/24 -- do NOT dimension ux,uy,uz with multiple components
      realSerialArray ux(I1,I2,I3),uy,uz;
      if( !rectangular )
      {
	if( numberOfDimensions>1 )
	  uy.redim(I1,I2,I3);
	if( numberOfDimensions>2 )
	  uz.redim(I1,I2,I3);
      }
      
      // *wdh* 2012/11/24 : old way
//       realSerialArray ux(I1,I2,I3,F),uy,uz;
//       if( !rectangular )
//       {
// 	if( numberOfDimensions>1 )
// 	  uy.redim(I1,I2,I3,F);
// 	if( numberOfDimensions>2 )
// 	  uz.redim(I1,I2,I3,F);
//       }
      
      c.update(MappedGrid::THEcenter);
      realArray & x= c.center();
      #ifdef USE_PPP
        realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
      #else
        const realSerialArray & xLocal = x;
      #endif  

      real *normalp = normalLocal.Array_Descriptor.Array_View_Pointer3;
      const int normalDim0=normalLocal.getRawDataSize(0);
      const int normalDim1=normalLocal.getRawDataSize(1);
      const int normalDim2=normalLocal.getRawDataSize(2);
      #undef NORMAL
      #define NORMAL(i0,i1,i2,i3) normalp[i0+normalDim0*(i1+normalDim1*(i2+normalDim2*(i3)))]

      real *rhsp = rhs.Array_Descriptor.Array_View_Pointer3;
      const int rhsDim0=rhs.getRawDataSize(0);
      const int rhsDim1=rhs.getRawDataSize(1);
      const int rhsDim2=rhs.getRawDataSize(2);
      #undef RHS
      #define RHS(i0,i1,i2,i3) rhsp[i0+rhsDim0*(i1+rhsDim1*(i2+rhsDim2*(i3)))]


      real *uxp = ux.Array_Descriptor.Array_View_Pointer2;
      const int uxDim0=ux.getRawDataSize(0);
      const int uxDim1=ux.getRawDataSize(1);
      #undef UX
      #define UX(i0,i1,i2) uxp[i0+uxDim0*(i1+uxDim1*(i2))]

      real *uyp = uy.Array_Descriptor.Array_View_Pointer2;
      const int uyDim0=uy.getRawDataSize(0);
      const int uyDim1=uy.getRawDataSize(1);
      #undef UY
      #define UY(i0,i1,i2) uyp[i0+uyDim0*(i1+uyDim1*(i2))]

      real *uzp = uz.Array_Descriptor.Array_View_Pointer2;
      const int uzDim0=uz.getRawDataSize(0);
      const int uzDim1=uz.getRawDataSize(1);
      #undef UZ
      #define UZ(i0,i1,i2) uzp[i0+uzDim0*(i1+uzDim1*(i2))]

      // uC.display("neumann: uC");  
      // fC.display("neumann: fC");  
      
      int i1,i2,i3;
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
      {
	const int mm=fC(n);

	// printF(">>> neumann: n=%i, mm=%i\n",n,mm); 
	
	
        bool isRectangular=false; // do this for now
        if( rectangular || numberOfDimensions==1 )
	{
          int ntd=0, ndx[3]={0,0,0}; //
          ndx[axis]=1;  // evaluate an x,y or z derivative for axis=0,1, or 2
          (*e).gd( ux,xLocal,c.numberOfDimensions(),isRectangular,ntd,ndx[0],ndx[1],ndx[2],I1,I2,I3,mm,t);

	  FOR_3D(i1,i2,i3,I1,I2,I3)
	    RHS(i1,i2,i3,mm)=b1*UX(i1,i2,i3)*(2*side-1); 

	}
	else
	{
	  (*e).gd( ux,xLocal,c.numberOfDimensions(),isRectangular,0,1,0,0,I1,I2,I3,mm,t);
  	  (*e).gd( uy,xLocal,c.numberOfDimensions(),isRectangular,0,0,1,0,I1,I2,I3,mm,t);
          if( numberOfDimensions==2 )
	  {
            FOR_3D(i1,i2,i3,I1,I2,I3)
  	      RHS(i1,i2,i3,mm)=b1*(UX(i1,i2,i3)*NORMAL(i1,i2,i3,0)+UY(i1,i2,i3)*NORMAL(i1,i2,i3,1));
	  }
	  else if( numberOfDimensions==3 )
	  {
  	    (*e).gd( uz,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,1,I1,I2,I3,mm,t);
	    
            FOR_3D(i1,i2,i3,I1,I2,I3)
  	      RHS(i1,i2,i3,mm)=b1*(UX(i1,i2,i3)*NORMAL(i1,i2,i3,0)+ 
                                   UY(i1,i2,i3)*NORMAL(i1,i2,i3,1)+
                                   UZ(i1,i2,i3)*NORMAL(i1,i2,i3,2));
	  }
	}
        // *wdh* 070924 -- fixed TZ forcing to include b0 and b1 !!
	if( b0!=0. )
	{
          (*e).gd( ux,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,mm,t);
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	    RHS(i1,i2,i3,mm)+=b0*UX(i1,i2,i3);  // (UX holds ue)

	}
	

      }
    }
    else if( bcOption==scalarForcing )
    {
      option=0;
    }
    else if( bcOption==arrayForcing )
    {
      // check: 
//       const IntegerArray & d = c.dimension();
//       bool ok = arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
// 	arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
// 	arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
// 	arrayDataD.getBase(3)<=min(fC(uC.dimension(0))) && arrayDataD.getBound(3)>=max(fC(uC.dimension(0)));
    
//       printf(" neumann: arrayForcing: arrrayDataD bounds=[%i,%i][%i,%i][%i,%i][%i,%i] \n"
//              "                                    uLocal=[%i,%i][%i,%i][%i,%i][%i,%i] \n"
//              "                                 dimension=[%i,%i][%i,%i][%i,%i]\n"
//              "                                (I1,I2,I3)=[%i,%i][%i,%i][%i,%i]  -> ok=%i\n",
//              arrayDataD.getBase(0),arrayDataD.getBound(0),
//              arrayDataD.getBase(1),arrayDataD.getBound(1),
//              arrayDataD.getBase(2),arrayDataD.getBound(2),
//              arrayDataD.getBase(3),arrayDataD.getBound(3),
//              uLocal.getBase(0),uLocal.getBound(0),
//              uLocal.getBase(1),uLocal.getBound(1),
//              uLocal.getBase(2),uLocal.getBound(2),
//              uLocal.getBase(3),uLocal.getBound(3),
//              d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2),
//              I1.getBase(),I1.getBound(), I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),ok );

      if( arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
	  arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
	  arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
	  arrayDataD.getBase(3)<=min(fC(uC.dimension(0))) && arrayDataD.getBound(3)>=max(fC(uC.dimension(0))) )
      {
        option=1;
        gfDatap=(realSerialArray *)(&arrayDataDLocal); // use arrayDataD(I1,I2,I3,fC(n))
      }
      else if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
      {
        option=2; // use arrayData(fC(n),side,axis,grid)
      }
      else
        option=3;  // use arrayData(fC(n))
    }
    else if( bcOption==gridFunctionForcing )
    {
      option=1;
      gfDatap=(realSerialArray *)(&gfDataLocal);
    }
    else
      {throw "Invalid value for bcOption! (neumann)";}
    
    assert( option>=0 );
    
    assert( gfDatap!=NULL );
    const realSerialArray & gfd = *gfDatap;

    const int gridType = rectangular ? 0 : 1;
    const int ca = uC.getBase(0);
    const int cb = uC.getBound(0);

    const int useWhereMask = (useWhereMaskOnBoundary[axis][side] && bcParameters.useMixedBoundaryMask) ||
                              bcParameters.getUseMask() ;

    
    real *up = getDataPointer(uLocal);
    real *adp=getDataPointer(arrayData);
    const real *arrayDatap=adp!=NULL ? adp : up;
    
    int *mp=getDataPointer(maskLocal);
    const int *maskp = mp!=NULL ? mp : &uC(uC.getBase(0));

    realSerialArray *nmCoeffp;
    if( coeffIsSet[axis][side] )
    {
      nmCoeffp= bcType==BCTypes::neumann ? &neumannCoeff[axis][side] : &mixedDerivativeCoeff[axis][side];
    }
    else
    {
      nmCoeffp=&((realSerialArray&)uLocal);
    }
    #ifdef USE_PPP
      const realSerialArray & nmCoeff = *nmCoeffp; 
    #else
      const realSerialArray & nmCoeff = *nmCoeffp; 
    #endif

//      printf(" gfd.getBase(0)=%i gfd.getBound(0)=%i \n",gfd.getBase(0),gfd.getBound(0));
//      printf(" gfd.getBase(1)=%i gfd.getBound(1)=%i \n",gfd.getBase(1),gfd.getBound(1));
//      printf(" gfd.getBase(2)=%i gfd.getBound(2)=%i \n",gfd.getBase(2),gfd.getBound(2));

    const real *rxp = up;  // Jacobian not needed for Neumann BC's

    if( n1a<=n1b && n2a<=n2b && n3a<=n3b ) // In parallel, the boundary may not be on this processor
    {

      // *wdh* 040930 -- check the mask bounds -- the mask values may be on the ghost line ---
      int ndm[2][3];  // base/bound for maskLocal
      for( int dir=0; dir<3; dir++ )
      {
        ndm[0][dir]=maskLocal.getBase(dir);
        ndm[1][dir]=maskLocal.getBound(dir);
        // The mask may come from the ghost -- shift the base/bound of the mask so we can index the mask in the
        // same way as the solution
        if( useWhereMask && axis==dir )
	{
	  if( ndm[0][dir]==ndm[1][dir] && nv[0][dir]==nv[1][dir] && ndm[0][dir]!=nv[0][dir] )
	  { // mask base/bound do not match boundary bounds, shift the mask bounds
	    ndm[0][dir]=nv[0][dir];
	    ndm[1][dir]=nv[1][dir];
	  }
	}
      }
      if( useWhereMask )
      {
        for( int dir=0; dir<3; dir++ )
	{
	  if( nv[0][dir]<ndm[0][dir] || nv[1][dir]>ndm[1][dir] )
	  {
	    printf("neumann:ERROR:mask bounds are not valid!\n"
                   " n1a,n1b,n2a,n2b,n3a,n3b = %i,%i,%i,%i,%i,%i \n"
                   " mask bounds = [%i,%i][%i,%i][%i,%i]\n",n1a,n1b,n2a,n2b,n3a,n3b,
                   ndm[0][0],ndm[1][0],ndm[0][1],ndm[1][1],ndm[0][2],ndm[1][2]);
            Overture::abort("ERROR:neumann");
	  }
	}
      }

      assignBoundaryConditions( c.numberOfDimensions(), 
       n1a,n1b,n2a,n2b,n3a,n3b,
       uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
       uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
       uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
       uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
       nmCoeff.getBase(0),nmCoeff.getBound(0),nmCoeff.getBase(1),nmCoeff.getBound(1),
       nmCoeff.getBase(2),nmCoeff.getBound(2),nmCoeff.getBase(3),nmCoeff.getBound(3),
       gfd.getBase(0),gfd.getBound(0),gfd.getBase(1),gfd.getBound(1),
       gfd.getBase(2),gfd.getBound(2),gfd.getBase(3),gfd.getBound(3),
       arrayData.getBase(0),arrayData.getBound(0),arrayData.getBase(1),arrayData.getBound(1),
       arrayData.getBase(2),arrayData.getBound(2),arrayData.getBase(3),arrayData.getBound(3),
       arrayData.getBase(0),arrayData.getBound(0),
       ndm[0][0],ndm[1][0],ndm[0][1],ndm[1][1],ndm[0][2],ndm[1][2],  // dimensions for mask
       uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),uLocal.getBase(2),uLocal.getBound(2),
       *rxp,
       *up,*up,*getDataPointer(nmCoeff), *maskp,
       scalarData,*getDataPointer(gfd),*arrayDatap,*arrayDatap,
       dx[0],dr[0],ipar[0], par[0], ca,cb, uC.getBase(0),uC(uC.getBase(0)), fC.getBase(0),fC(fC.getBase(0)),
       side,axis,grid, (int)bcType, option,gridType,orderOfAccuracy,useWhereMask,bcParameters.lineToAssign );
    }
    

    delete rhsp;

//      Index I1m,I2m,I3m;
//      getGhostIndex( c.indexRange(),side,axis,I1m,I2m,I3m,+1); // first ghost line
//      uNew=u(I1m,I2m,I3m,u.dimension(3));
    
    timeForNeumann+=getCPU()-time0;
    return;  // *********************
    
  }
  
// * 
// *   Index I1f,I2f,I3f;
// *   // **** Evaluate the forcing at these points
// *   getGhostIndex( c.indexRange(),side,axis,I1f,I2f,I3f,bcParameters.lineToAssign,bcParameters.extraInTangentialDirections);
// * 
// * 
// * 
// *   Index I1m,I2m,I3m;
// *   getGhostIndex( c.indexRange(),side,axis,I1m,I2m,I3m,+1,bcParameters.extraInTangentialDirections); // first ghost line
// * 
// *   Index I1p,I2p,I3p;
// *   getGhostIndex( c.indexRange(),side,axis,I1p,I2p,I3p,-1,bcParameters.extraInTangentialDirections); // first line in
// * 
// *   if( rectangular || numberOfDimensions==1 )
// *   {
// *     // rectangular grid : NOTE: we do not have to worry about +/- twoDeltaX*g  because
// *     // the normal changes direction on either end introducing another +/-
// *     real twoDeltaX;
// *     if( rectangular )
// *       twoDeltaX = 2.*dx[0]; // 1./h21(axis);
// *     else // 1D, non-rectangular:   u.n = (+/-) (1/x.r) D0r u
// *       twoDeltaX=2.*c.vertexDerivative()(I1.getBase(),I2.getBase(),I3.getBase(),axis1,axis1)
// * 	*c.gridSpacing()(axis1);
// * 	  
// *     if( b0==0. )
// *     {
// *       // pure neumann BC
// *       for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
// * 	WHERE_MASK( uA(I1m,I2m,I3m,uC(n))=uA(I1p,I2p,I3p,uC(n)); )
// *     }
// *     else
// *     {
// *       // mixed BC
// *       for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
// * 	WHERE_MASK( uA(I1m,I2m,I3m,uC(n))=uA(I1p,I2p,I3p,uC(n)) + uA(I1,I2,I3,uC(n))*(-twoDeltaX*b0/b1); )
// *     }
// * 	  
// *     if( twilightZoneFlow )
// *     {
// *       for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
// *       {
// *         if( axis==axis1 )
// * 	{
// * 	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=(e->x(c,I1,I2,I3,fC(n),t)*((2*side-1)*twoDeltaX/b1) ); )
// * 	}
// * 	else if( axis==axis2 )
// * 	{
// * 	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=(e->y(c,I1,I2,I3,fC(n),t)*((2*side-1)*twoDeltaX/b1) );)
// * 	}
// * 	else
// * 	{
// * 	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=(e->z(c,I1,I2,I3,fC(n),t)*((2*side-1)*twoDeltaX/b1) );)
// * 	}
// * // 	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=(e->x(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis1)
// * // 				  +e->y(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis2)
// * // 				  +e->z(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis3))*(twoDeltaX/b1); )
// * // 	else
// * // 	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+= e->x(c,I1,I2,I3,fC(n),t)*((2*side-1)*twoDeltaX/b1); )
// *       }
// *     }
// *     else if( bcOption==scalarForcing )
// *     {
// *       if( scalarData != 0. )
// * 	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
// * 	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=scalarData*(twoDeltaX/b1); )
// *     }
// *     else if( bcOption==arrayForcing )
// *     {
// *       if( arrayDataD.getBase(0)<=I1f.getBase() && arrayDataD.getBound(0)>=I1f.getBound() &&
// * 	  arrayDataD.getBase(1)<=I2f.getBase() && arrayDataD.getBound(1)>=I2f.getBound() &&
// * 	  arrayDataD.getBase(2)<=I3f.getBase() && arrayDataD.getBound(2)>=I3f.getBound() &&
// * 	  arrayDataD.getBase(3)<=min(fC(uC.dimension(0))) && arrayDataD.getBound(3)>=max(fC(uC.dimension(0))) )
// *       {
// * 	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
// * 	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=arrayDataD(I1f,I2f,I3f,fC(n))*(twoDeltaX/b1); )
// *       }
// *       else if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
// *       {
// * 	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
// * 	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=arrayData(fC(n),side,axis,grid)*(twoDeltaX/b1); )
// *       }
// *       else
// * 	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
// * 	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=arrayData(fC(n))*(twoDeltaX/b1); )
// *       
// *     }
// *     else if( bcOption==gridFunctionForcing )
// *     {
// *       for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
// * 	WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=gfData(I1f,I2f,I3f,fC(n))*(twoDeltaX/b1); )
// *     }
// *     else
// *       {throw "Invalid value for bcOption! (neumann)";}
// *   }
// *   else 
// *   {
// *     // cout << "Boundary conditions: apply real Neumann BC\n";
// *     // generate coeff's for n.grad
// *     // Solve for the ghost point from: (n.grad)u=
// *     Index M(0,int(pow(3,numberOfDimensions)+.5));
// *     int is1 = (axis==axis1) ? 1-2*side : 0;   
// *     int is2 = (axis==axis2) ? 1-2*side : 0;           
// *     int is3 = (axis==axis3) ? 1-2*side : 0;           
// *     int mGhost = numberOfDimensions==2 ? M2(-is1,-is2) : M3(-is1,-is2,-is3);    // coefficient index for ghost value
// * 
// *     RealDistributedArray uDotN(I1,I2,I3);
// *     RealDistributedArray & rhs = uDotN;
// * 
// * //    typedef int POINTER2[2];
// * //    POINTER2 *coeffIsSet = bcType==BCTypes::neumann ? nCoeffIsSet : mCoeffIsSet; 
// *     if( !coeffIsSet[axis][side] )
// *       createBoundaryMatrix(side,axis,bcType);
// * 
// *     realSerialArray & nmCoeff   = bcType==BCTypes::neumann ? neumannCoeff[axis][side] : mixedDerivativeCoeff[axis][side];
// * 
// *     if( !coeffIsSet[axis][side] )
// *     { // generate coefficients if they have not already been set
// *       coeffIsSet[axis][side]=TRUE; 
// *       buildNeumannCoefficientMatrix(*this,nmCoeff,M,I1,I2,I3,normal,b0,b1,numberOfDimensions,side,axis);
// *     }
// * 	  
// * 
// *     for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
// *       WHERE_MASK( uA(I1m,I2m,I3m,uC(n))=0.; )// zero this out so we can use it in the rhs of the expressions below
// * 
// *     uA.reshape(1,uA.dimension(0),uA.dimension(1),uA.dimension(2),uA.dimension(3));
// *     mask.reshape(1,mask.dimension(0),mask.dimension(1),mask.dimension(2));
// *     for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
// *     {
// *       if( twilightZoneFlow )
// *       { 
// * 	rhs(I1,I2,I3)=e->x(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis1)
// * 	  +e->y(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis2);
// * 	if( numberOfDimensions==3 )
// * 	  rhs(I1,I2,I3)+=e->z(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis3);
// *       }
// *       else if( bcOption==scalarForcing )
// * 	rhs(I1,I2,I3)=scalarData;
// *       else if( bcOption==arrayForcing )
// *       {
// * 	if( arrayDataD.getBase(0)<=I1f.getBase() && arrayDataD.getBound(0)>=I1f.getBound() &&
// * 	    arrayDataD.getBase(1)<=I2f.getBase() && arrayDataD.getBound(1)>=I2f.getBound() &&
// * 	    arrayDataD.getBase(2)<=I3f.getBase() && arrayDataD.getBound(2)>=I3f.getBound() &&
// * 	    arrayDataD.getBase(3)<=min(fC(uC.dimension(0))) && arrayDataD.getBound(3)>=max(fC(uC.dimension(0))) )
// * 	  rhs(I1,I2,I3)=arrayDataD(I1f,I2f,I3f,fC(n));
// * 	else if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
// * 	  rhs(I1,I2,I3)=arrayData(fC(n),side,axis,grid);
// * 	else
// * 	  rhs(I1,I2,I3)=arrayData(fC(n));
// *       }
// *       else if( bcOption==gridFunctionForcing )
// *       {
// * 	rhs(I1,I2,I3)=gfData(I1f,I2f,I3f,fC(n));
// *       }
// *       else
// * 	throw "Invalid value for bcOption! (neumann)";
// * 
// *       rhs.reshape(1,rhs.dimension(0),rhs.dimension(1),rhs.dimension(2),rhs.dimension(3));
// *       if( numberOfDimensions==2 )
// *       {
// * 	WHERE_MASK0( uA(0,I1m,I2m,I3m,uC(n))=( 
// * 	  rhs(0,I1,I2,I3) - (
// * 	    nmCoeff(M2( 0,-1),I1,I2,I3)*uA(0,I1  ,I2-1,I3,uC(n))
// * 	    +nmCoeff(M2(-1, 0),I1,I2,I3)*uA(0,I1-1,I2  ,I3,uC(n))
// * 	    +nmCoeff(M2( 0, 0),I1,I2,I3)*uA(0,I1  ,I2  ,I3,uC(n))
// * 	    +nmCoeff(M2(+1, 0),I1,I2,I3)*uA(0,I1+1,I2  ,I3,uC(n))
// * 	    +nmCoeff(M2( 0,+1),I1,I2,I3)*uA(0,I1  ,I2+1,I3,uC(n))
// * 	    ))/nmCoeff(mGhost,I1,I2,I3); )
// *       }
// *       else
// *       {
// * 	WHERE_MASK0( uA(0,I1m,I2m,I3m,uC(n))=( 
// * 	  rhs(0,I1,I2,I3) - (
// * 	     nmCoeff(M3( 0, 0,-1),I1,I2,I3)*uA(0,I1  ,I2  ,I3-1,uC(n))
// * 	    +nmCoeff(M3( 0,-1, 0),I1,I2,I3)*uA(0,I1  ,I2-1,I3  ,uC(n))
// * 	    +nmCoeff(M3(-1, 0, 0),I1,I2,I3)*uA(0,I1-1,I2  ,I3  ,uC(n))
// * 	    +nmCoeff(M3( 0, 0, 0),I1,I2,I3)*uA(0,I1  ,I2  ,I3  ,uC(n))
// * 	    +nmCoeff(M3(+1, 0, 0),I1,I2,I3)*uA(0,I1+1,I2  ,I3  ,uC(n))
// * 	    +nmCoeff(M3( 0,+1, 0),I1,I2,I3)*uA(0,I1  ,I2+1,I3  ,uC(n))
// * 	    +nmCoeff(M3( 0, 0,+1),I1,I2,I3)*uA(0,I1  ,I2  ,I3+1,uC(n))
// * 	    ))/nmCoeff(mGhost,I1,I2,I3); )
// *       }
// *       rhs.reshape(rhs.dimension(1),rhs.dimension(2),rhs.dimension(3),rhs.dimension(4));
// *     }
// *     uA.reshape(uA.dimension(1),uA.dimension(2),uA.dimension(3),uA.dimension(4));
// *     mask.reshape(mask.dimension(1),mask.dimension(2),mask.dimension(3));
// *   }
// * 
// * //    n=uC(0);
// * //    real err=max(fabs(uNew(I1m,I2m,I3m,n)-uA(I1m,I2m,I3m,n)));
// * //    if( err > 1.e-9 )
// * //    {
// * //      printf(" neumann:ERROR : err=%8.2e\n",err);
// * //      uNew(I1m,I2m,I3m,n).display("uNew");
// * //      uA(I1m,I2m,I3m,n).display("uA");
// * //      uA.display("uA");
// * //      abort();
// * //    }
// *   
// *   timeForNeumann+=getCPU()-time0;
// * 

}
// These are for indexing into a coefficient matrix
#undef M2
#undef M3
