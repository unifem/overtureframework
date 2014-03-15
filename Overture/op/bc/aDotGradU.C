// From Philip Blakely -- 080419 --


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


//! Build the coefficient matrix used to apply the aDotGradU BC
void static
buildADotGradUCoefficientMatrix(MappedGridOperators & op,
				realSerialArray & adguCoeff, Index & M, Index &I1, Index &I2,Index &I3,
				realSerialArray & normalLocal, real b0, real b1, real b2, int numberOfDimensions,
				int side, int axis)
{
  /*
  cout << "aDotGradUCoeffMatrix: side=" << side << " axis=" << axis << endl;
  cout << "I1 = (" << I1.getBase() << "," << I1.getBound() << ")" << endl;
  cout << "I2 = (" << I2.getBase() << "," << I2.getBound() << ")" << endl;
  cout << "I3 = (" << I3.getBase() << "," << I3.getBound() << ")" << endl;
  */
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

    adguCoeff.redim(M,J1,J2,J3);
    opX.redim(M,J1,J2,J3);
    

    const real nSign = 2*side-1.;  // sign for the normal when we use rx
    const realSerialArray & opXLocal     = opX; 
    const realSerialArray & adguCoeffLocal = adguCoeff; 

  #else
    // index into the normal=vertexBoundaryNormalArray

    MappedGrid & mg = op.mappedGrid;
    const intArray & mask = mg.mask();

    Index J1 = I1, J2 = I2, J3 = I3;

    adguCoeff.resize(M,I1,I2,I3);
    opX.redim(M,I1,I2,I3);

    const realSerialArray & opXLocal     = opX;
    const realSerialArray & adguCoeffLocal = adguCoeff;

  #endif

    IntegerArray mask_Local; getLocalArrayWithGhostBoundaries( mg.mask(), mask_Local);
    int *mask_ptr = mask_Local.Array_Descriptor.Array_View_Pointer3;
    const int mask_Dim0 = mask_Local.getRawDataSize(0);
    const int mask_Dim1 = mask_Local.getRawDataSize(1)*mask_Dim0;
    const int mask_Dim2 = mask_Local.getRawDataSize(2)*mask_Dim1;

#define mask(i,j,k) mask_ptr[ (i) + (j)*mask_Dim0 + (k)*mask_Dim1]

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

    real *adguCoeffp = adguCoeffLocal.Array_Descriptor.Array_View_Pointer3;
    const int adguCoeffDim0=adguCoeffLocal.getRawDataSize(0);
    const int adguCoeffDim1=adguCoeffLocal.getRawDataSize(1);
    const int adguCoeffDim2=adguCoeffLocal.getRawDataSize(2);
    #undef ADGUCOEFF
    #define ADGUCOEFF(i0,i1,i2,i3) adguCoeffp[i0+adguCoeffDim0*(i1+adguCoeffDim1*(i2+adguCoeffDim2*(i3)))]

    op.assignCoefficients(MappedGridOperators::xDerivative,opX,J1,J2,J3,0,0);
    // ::display(opXLocal(M,J1,J2,J3),"neumann: opX","%6.2f ");
//      int m;
//      for( m=M.getBase(); m<=M.getBound(); m++ )
//        opXLocal(m,J1,J2,J3)*=normalLocal(0,J1,J2,J3,n1);
//      adguCoeffLocal(M,J1,J2,J3)=opXLocal(M,J1,J2,J3);

    int m,i1,i2,i3;
      FOR_4D(m,i1,i2,i3,M,J1,J2,J3)
	{
	  ADGUCOEFF(m,i1,i2,i3)= b0*OPX(m,i1,i2,i3);
	};

  //  ::display(opXLocal,"buildNeumannCoefficientMatrix: opXLocal after step 1");
  //  ::display(adguCoeff,"buildNeumannCoefficientMatrix: adguCoeff after step 1");



    if( numberOfDimensions > 1 )
    {
      op.assignCoefficients(MappedGridOperators::yDerivative,opX,J1,J2,J3,0,0);

//        for( m=M.getBase(); m<=M.getBound(); m++ )
//  	opXLocal(m,J1,J2,J3)*=normalLocal(0,J1,J2,J3,n2);
//        adguCoeffLocal(M,J1,J2,J3)+=opXLocal(M,J1,J2,J3);


      FOR_4(m,i1,i2,i3,M,J1,J2,J3)
	{
	  ADGUCOEFF(m,i1,i2,i3)+= b1*OPX(m,i1,i2,i3);
	}

    };

    if( numberOfDimensions > 2 )
    {
      op.assignCoefficients(MappedGridOperators::zDerivative,opX,J1,J2,J3,0,0);
//        for( m=M.getBase(); m<=M.getBound(); m++ )
//  	opXLocal(m,J1,J2,J3)*=normalLocal(0,J1,J2,J3,n3);
//        adguCoeffLocal(M,J1,J2,J3)+=opXLocal(M,J1,J2,J3);

      FOR_4D(m,i1,i2,i3,M,J1,J2,J3)
      {
	ADGUCOEFF(m,i1,i2,i3)+= b2*OPX(m,i1,i2,i3);
      }
    }

    //Have problems if (b0,b1,b2) is parallel to boundary,
    //in which case extrapolate second order along normal.
    //At least we get a boundary condition this way.

    if( numberOfDimensions == 2 )
      {
	FOR_3D(i1,i2,i3,J1,J2,J3)
	  {
	    double dot_prod = b0*NORMAL(i1,i2,i3,0) + b1*NORMAL(i1,i2,i3,1);
	    bool point_outside_grid = false;
	    if(i1==I1Base || i1==I1Bound || i2==I2Base || i2==I2Bound)
	      point_outside_grid = true;
	    else
	      for(int i=-1 ; i<=1 ; i+=2)
		for(int j=-1 ; j<=1 ; j+=2)
		  if( ( mask(i1+i, i2+j, i3) & MappedGrid::ISdiscretizationPoint ) == 0 )
		    point_outside_grid = true;
	    //We definitely need the first condition, we may or may not need the second condition - PMB
	    // 1e-1 here corresponds to about 6 degrees; making it smaller may remove its effectiveness.
	    if( fabs(dot_prod) < 1e-1 || point_outside_grid){
	      if(Mapping::debug & 2)
		cout << "aDotGradU: a is parallel to boundary at (" << i1 << "," << i2 << ") - correcting" << endl;

	      for(int i=0 ; i<9 ; i++)
		ADGUCOEFF(i,i1,i2,i3) = 0;

	      ADGUCOEFF(4,i1,i2,i3) = -2;

	      if(axis==0){
		ADGUCOEFF(3,i1,i2,i3) = 1;
		ADGUCOEFF(5,i1,i2,i3) = 1;
	      };
	      if(axis==1){
		ADGUCOEFF(7,i1,i2,i3) = 1;
		ADGUCOEFF(1,i1,i2,i3) = 1;
	      };
	    };
	    /*
	    else if( fabs(1-dot_prod) < 1e-1 )
	      {
		if(Mapping::debug & 2)
		  cout << "aDotGradU: a is perpendicular to boundary at (" << i1 << "," << i2 << ") - correcting" << endl;

		for(int i=0 ; i<9 ; i++)
		  ADGUCOEFF(i,i1,i2,i3) = 0;

		ADGUCOEFF(4,i1,i2,i3) = -1;

		if(axis==0)
		  if(side == 0)
		    ADGUCOEFF(3,i1,i2,i3) = 1;
		  else
		    ADGUCOEFF(5,i1,i2,i3) = 1;
		else if(side == 0)
		  ADGUCOEFF(1,i1,i2,i3) = 1;
		else
		  ADGUCOEFF(7,i1,i2,i3) = 1;
	      };
	    */
	  }

      };

    if( numberOfDimensions == 3)
      {
	FOR_3D(i1,i2,i3,J1,J2,J3)
	  {
	    double dot_prod = b0*NORMAL(i1,i2,i3,0) + b1*NORMAL(i1,i2,i3,1) + b2*NORMAL(i1,i2,i3,2);
	    bool point_outside_grid = false;
	    if(i1==I1Base || i1==I1Bound || i2==I2Base || i2==I2Bound || i3==I3Base || i3==I3Bound)
	      point_outside_grid = true;
	    else
	      for(int i=-1 ; i<=1 ; i+=2)
		for(int j=-1 ; j<=1 ; j+=2)
		  for(int k=-1 ; k<=1 ; k+=2)
		    if( ( mask(i1+i, i2+j, i3+k) & MappedGrid::ISdiscretizationPoint ) == 0 )
		      point_outside_grid = true;

	    if( fabs(dot_prod) < 1e-1 || point_outside_grid){
	      //if(point_outside_grid)
	      //cout << "Some points on boundary matrix outside grid - correcting" << endl;
	      if(Mapping::debug & 2)
		cout << "aDotGradU: a is parallel to boundary at (" << i1 << "," << i2 << "," << i3 << ") - correcting" << endl;

	      for(int i=0 ; i<27 ; i++)
		ADGUCOEFF(i,i1,i2,i3) = 0;
	      
	      ADGUCOEFF(13,i1,i2,i3) = -2;
	      
	      if(axis==0){
		ADGUCOEFF(12,i1,i2,i3) = 1;
		ADGUCOEFF(14,i1,i2,i3) = 1;
	      };
	      if(axis==1){
		ADGUCOEFF(10,i1,i2,i3) = 1;
		ADGUCOEFF(16,i1,i2,i3) = 1;
	      };
	      if(axis==2){
		ADGUCOEFF(4,i1,i2,i3) = 1;
		ADGUCOEFF(22,i1,i2,i3) = 1;
	      };
	    }
	    
	  };
      };

    // -- alter the operator for mixed BC's
//     #ifdef USE_PPP
//       // we need to normalize the normal when using the rx array
//       real scale=b1*nSign;
//       if( numberOfDimensions==1 )
//       {
// 	FOR_4(m,i1,i2,i3,M,J1,J2,J3)
// 	{
// 	  ADGUCOEFF(m,i1,i2,i3)*= scale/fabs(NORMAL(i1,i2,i3,n1));
// 	}
//       }
//       else if( numberOfDimensions==2 )
//       {
// 	FOR_4(m,i1,i2,i3,M,J1,J2,J3)
// 	{
// 	  ADGUCOEFF(m,i1,i2,i3)*= scale/sqrt(SQR(NORMAL(i1,i2,i3,n1))+SQR(NORMAL(i1,i2,i3,n2)));
// 	}
//       }
//       else
//       {
// 	FOR_4(m,i1,i2,i3,M,J1,J2,J3)
// 	{
// 	  ADGUCOEFF(m,i1,i2,i3)*= scale/sqrt(SQR(NORMAL(i1,i2,i3,n1))+SQR(NORMAL(i1,i2,i3,n2))+SQR(NORMAL(i1,i2,i3,n3)));
// 	}
//       }
//     #else

  }
  else
  {
    /*
    cout << "Doing aDotGradU the old way..." << endl;
    // old way 
    realSerialArray & normalnc = (realSerialArray&)normalLocal; // cast away const
    normalnc.reshape(1,normalnc.dimension(0),normalnc.dimension(1),normalnc.dimension(2),normalnc.dimension(3));


    op.assignCoefficients(MappedGridOperators::xDerivative,opX,I1,I2,I3,0,0);
    ::display(opXLocal(M,J1,J2,J3),"neumann: opX","%6.2f ");

    int m;
    for( m=M.getBase(); m<=M.getBound(); m++ )
      opXLocal(m,J1,J2,J3)*=normalLocal(0,J1,J2,J3,n1);
    adguCoeffLocal(M,J1,J2,J3)=opXLocal(M,J1,J2,J3);
    if( numberOfDimensions > 1 )
    {
      op.assignCoefficients(MappedGridOperators::yDerivative,opX,I1,I2,I3,0,0);
      for( m=M.getBase(); m<=M.getBound(); m++ )
	opXLocal(m,J1,J2,J3)*=normalLocal(0,J1,J2,J3,n2);
      adguCoeffLocal(M,J1,J2,J3)+=opXLocal(M,J1,J2,J3);
    }
    if( numberOfDimensions > 2 )
    {
      op.assignCoefficients(MappedGridOperators::zDerivative,opX,I1,I2,I3,0,0);
      for( m=M.getBase(); m<=M.getBound(); m++ )
	opXLocal(m,J1,J2,J3)*=normalLocal(0,J1,J2,J3,n3);
      adguCoeffLocal(M,J1,J2,J3)+=opXLocal(M,J1,J2,J3);
    }
  
    normalnc.reshape(normalnc.dimension(1),normalnc.dimension(2),normalnc.dimension(3),normalnc.dimension(4));
    */



    /*
  // -- alter the operator for mixed BC's
    if( b1 != 1. )
      adguCoeffLocal(M,J1,J2,J3)*=b1;
    if( b0 !=0. )
    {
      op.assignCoefficients(MappedGridOperators::identityOperator,opX,I1,I2,I3,0,0);
      adguCoeffLocal(M,J1,J2,J3)+=b0*opXLocal(M,J1,J2,J3);
    }
    */
  }

  
  //  ::display(adguCoeffLocal,"neumann: adguCoeffLocal at end");

}

// These are for indexing into a coefficient matrix
#undef M2
#define M2(m1,m2) ((m1)+1+3*((m2)+1))
#undef M3
#define M3(m1,m2,m3) ((m1)+1+3*((m2)+1+3*((m3)+1)))

void MappedGridOperators::
applyBCaDotGradU(realMappedGridFunction & u, 
		 const int side,
		 const int axis,
		 const real & scalarData,
		 const RealArray & arrayData,
		 const RealArray & forcing2d,
		 const realMappedGridFunction & gfData,
		 const real & t,
		 const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
		 const BoundaryConditionParameters & bcParameters,
		 const BoundaryConditionOption bcOption,
		 const int & grid  )
// 
// Apply an aDotGradU BC: (a.grad) u = g
//
{
  real time0=getCPU();

  if( orderOfAccuracy!=2 )
  {
    printf("MappedGridOperators:: Sorry, the aDotGradU boundary condition is only implemented for\n"
           " orderOfAccuracy=2, requested orderOfAccuracy=%i. Continuing with 2nd-order\n",orderOfAccuracy);
  }

  MappedGrid & c = mappedGrid;

  //Specializations made for neumann in the Cartesian case aren't valid for aDotGradU
  const bool rectangular_ = false;

  if( !boundaryNormalsUsed && !rectangular_ )
  {
    boundaryNormalsUsed=true;
    mappedGrid.update(MappedGrid::THEvertexBoundaryNormal | MappedGrid::THEvertexDerivative);
  }
  
  RealDistributedArray & uA = u;
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
  #else
    const realSerialArray & uLocal = u;
  #endif
  #ifdef USE_PPP
    const realSerialArray & normalLocal  = !rectangular_? mappedGrid.vertexBoundaryNormalArray(side,axis) : uLocal; 
  #else
    const realSerialArray & normalLocal  = !rectangular_? mappedGrid.vertexBoundaryNormal(side,axis) : uLocal; 
  #endif


  int n;
  real b0=1., b1=0., b2=0.;
  const RealArray & a = bcParameters.a;
  
  if( a.getLength(0) < numberOfDimensions )
  {
    Overture::abort("MappedGridOperators::applyBoundaryCondition:mixed BC:ERROR: array `a' does not"
		    " have at least a number of values equal to the number of dimensions.");
  }
  if( a.getLength(1)==2 && a.getLength(2)>=numberOfDimensions && a.getLength(3)>grid )
  {
    b0=a(0,side,axis,grid);
    b1=a(1,side,axis,grid);
    b2=a(2,side,axis,grid);
  }
  else
  {
    b0=a(0);
    b1=a(1);
    b2=a(2);
  }

  Index I1,I2,I3;
  getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,0,bcParameters.extraInTangentialDirections);

  bool ok = ParallelUtility::getLocalArrayBounds(u, uLocal, I1, I2, I3, 0);

  //typedef int POINTER2[2];
  //POINTER2 *coeffIsSet = aCoeffIsSet; 

//  RealDistributedArray uNew;

  const bool useOpt=true;
  if( useOpt )
  {
    // **********************
    //    use optimised BC 
    // **********************

    #define coeffSaved(side,axis,m) (aCoeffValues[(side)+2*((axis)+3*(m))])

    bool buildMatrix = !( rectangular_ || numberOfDimensions==1 ) && !aCoeffIsSet[axis][side];
    if( !buildMatrix && aCoeffIsSet[axis][side] ) // *wdh* 080724 we need to check if the coeff's have changed!
    {
      buildMatrix = (b0 != coeffSaved(side,axis,0)) || (b1 !=coeffSaved(side,axis,1)) || (b2 !=coeffSaved(side,axis,2));
      // if( buildMatrix )
      //  printF("\n +++MappedGridOperators::aDotGradU BC:INFO: coefficients have changed! Rebuild BC matrix +++ \n\n");
    }
    if( buildMatrix )
    {
      // *** In the curvilinear case we build a coeff matrix on the boundary to represent BC
      if( !aCoeffIsSet[axis][side] )
	createBoundaryMatrix(side,axis,BCTypes::aDotGradU);

      if( buildMatrix )
      { // generate coefficients if they have not already been set
        realSerialArray & adguCoeff = aDotGradUCoeff[axis][side];
        Index M(0,int(pow(3,numberOfDimensions)+.5));
	aCoeffIsSet[axis][side]=true; 
	buildADotGradUCoefficientMatrix(*this,adguCoeff,M,I1,I2,I3,(realSerialArray &) normalLocal, b0,b1,b2,numberOfDimensions,side,axis);
	coeffSaved(side,axis,0)=b0;  // save the coefficients so we can check if they are changed 
	coeffSaved(side,axis,1)=b1;
	coeffSaved(side,axis,2)=b2;
      }
    }
    
    real twoDeltaX=1.;
    if( rectangular_ )
      twoDeltaX = 2.*dx[0]; // 1./h21(axis);
    else if ( c.numberOfDimensions()==1 )
      twoDeltaX=2.*c.vertexDerivative()(I1.getBase(),I2.getBase(),I3.getBase(),0,0)*c.gridSpacing(0);

#ifdef USE_PPP
      realSerialArray gfDataLocal; getLocalArrayWithGhostBoundaries(gfData,gfDataLocal);
      realSerialArray arrayDataDLocal = arrayData;
      //realSerialArray arrayDataDLocal; getLocalArrayWithGhostBoundaries(arrayDataD,arrayDataDLocal);
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
      // const intSerialArray & cmaskLocal = c.mask().getLocalArrayWithGhostBoundaries();
#else
      const realSerialArray & gfDataLocal = gfData;
      const realSerialArray & arrayDataDLocal = arrayData;

      const intSerialArray & maskLocal = mask;
      // const intSerialArray & maskLocal = c.mask();
#endif
//    realSerialArray *gfDatap=(realSerialArray *)(&gfDataLocal);

    
    // *wdh* 040930
    int nv[2][3], &n1a=nv[0][0], &n1b=nv[1][0], &n2a=nv[0][1], &n2b=nv[1][1], &n3a=nv[0][2], &n3b=nv[1][2]; 
    bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b); 
    if( !ok ) return;

    real par[4]={b0,b1,b2,twoDeltaX};
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
      
      realSerialArray ux(I1,I2,I3,F),uy,uz;
      if( !rectangular_ )
      {
	if( numberOfDimensions>1 )
	  uy.redim(I1,I2,I3,F);
	if( numberOfDimensions>2 )
	  uz.redim(I1,I2,I3,F);
      }
      
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

      int i1,i2,i3;
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
      {
	const int mm=fC(n);
	
        if( rectangular_ || numberOfDimensions==1 )
	{
          int ntd=0, ndx[3]={0,0,0}; //
          ndx[axis]=1;  // evaluate an x,y or z derivative for axis=0,1, or 2
	  bool isRectangular=false; // do this for now
          (*e).gd( ux,xLocal,c.numberOfDimensions(),isRectangular,ntd,ndx[0],ndx[1],ndx[2],I1,I2,I3,mm,t);

	  FOR_3D(i1,i2,i3,I1,I2,I3)
	    RHS(i1,i2,i3,mm)=UX(i1,i2,i3)*(2*side-1); 

// 	  if( axis==axis1 )
// 	    rhs(I1,I2,I3,fC(n))=e->x(c,I1,I2,I3,fC(n),t)*(2*side-1);
// 	  else if( axis==axis2 )
// 	    rhs(I1,I2,I3,fC(n))=e->y(c,I1,I2,I3,fC(n),t)*(2*side-1);
// 	  else
// 	    rhs(I1,I2,I3,fC(n))=e->z(c,I1,I2,I3,fC(n),t)*(2*side-1);

	}
	else
	{
	  bool isRectangular=false; // do this for now
	  (*e).gd( ux,xLocal,c.numberOfDimensions(),isRectangular,0,1,0,0,I1,I2,I3,mm,t);
  	  (*e).gd( uy,xLocal,c.numberOfDimensions(),isRectangular,0,0,1,0,I1,I2,I3,mm,t);
          if( numberOfDimensions==2 )
	  {
            FOR_3D(i1,i2,i3,I1,I2,I3)
  	      RHS(i1,i2,i3,mm)=UX(i1,i2,i3)*NORMAL(i1,i2,i3,0)+UY(i1,i2,i3)*NORMAL(i1,i2,i3,1);
	  }
	  else if( numberOfDimensions==3 )
	  {
  	    (*e).gd( uz,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,1,I1,I2,I3,mm,t);
	    
            FOR_3D(i1,i2,i3,I1,I2,I3)
  	      RHS(i1,i2,i3,mm)=UX(i1,i2,i3)*NORMAL(i1,i2,i3,0)+ 
                               UY(i1,i2,i3)*NORMAL(i1,i2,i3,1)+
                               UZ(i1,i2,i3)*NORMAL(i1,i2,i3,2);
	  }
	  
// 	  rhs(I1,I2,I3,fC(n))=e->x(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis1)
// 	                     +e->y(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis2);
// 	  if( numberOfDimensions==3 )
// 	    rhs(I1,I2,I3,fC(n))+=e->z(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis3);
	}
      }
    }
    else if( bcOption==scalarForcing )
    {
      option=0;
    }
    else if( bcOption==arrayForcing )
    {
      if( arrayDataDLocal.getBase(0)<=I1.getBase() && arrayDataDLocal.getBound(0)>=I1.getBound() &&
	  arrayDataDLocal.getBase(1)<=I2.getBase() && arrayDataDLocal.getBound(1)>=I2.getBound() &&
	  arrayDataDLocal.getBase(2)<=I3.getBase() && arrayDataDLocal.getBound(2)>=I3.getBound() &&
	  arrayDataDLocal.getBase(3)<=min(fC(uC.dimension(0))) && arrayDataDLocal.getBound(3)>=max(fC(uC.dimension(0))) )
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

    const int gridType = rectangular_ ? 0 : 1;
    const int ca = uC.getBase(0);
    const int cb = uC.getBound(0);

    const int useWhereMask = (useWhereMaskOnBoundary[axis][side] && bcParameters.useMixedBoundaryMask) ||
                              bcParameters.getUseMask() ;

    
    real *up = getDataPointer(uLocal);
    real *adp=getDataPointer(arrayData);
    const real *arrayDatap=adp!=NULL ? adp : up;
    
    int *mp=getDataPointer(maskLocal);
    int *uC_ptr=uC.Array_Descriptor.Array_View_Pointer0;
    const int *maskp = mp!=NULL ? mp : uC_ptr;//was &uC(0) doesn't nec. exist

    realSerialArray *adguCoeffp;
    if( aCoeffIsSet[axis][side] )
    {
      adguCoeffp= &aDotGradUCoeff[axis][side];
    }
    else
    {
      adguCoeffp=&((realSerialArray&)uLocal);
    }
    #ifdef USE_PPP
      const realSerialArray & adguCoeff = *adguCoeffp; 
    #else
      const realSerialArray & adguCoeff = *adguCoeffp; 
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
	    printf("aDotGradU:ERROR:mask bounds are not valid!\n"
                   " n1a,n1b,n2a,n2b,n3a,n3b = %i,%i,%i,%i,%i,%i \n"
                   " mask bounds = [%i,%i][%i,%i][%i,%i]\n",n1a,n1b,n2a,n2b,n3a,n3b,
                   ndm[0][0],ndm[1][0],ndm[0][1],ndm[1][1],ndm[0][2],ndm[1][2]);
            Overture::abort("ERROR:aDotGradU");
	  }
	}
      }

      int * fC_ptr = fC.Array_Descriptor.Array_View_Pointer0;

      int bcType=BCTypes::aDotGradU;
      assignBoundaryConditions( c.numberOfDimensions(), 
       n1a,n1b,n2a,n2b,n3a,n3b,
       uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
       uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
       uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
       uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
       adguCoeff.getBase(0),adguCoeff.getBound(0),adguCoeff.getBase(1),adguCoeff.getBound(1),
       adguCoeff.getBase(2),adguCoeff.getBound(2),adguCoeff.getBase(3),adguCoeff.getBound(3),
       gfd.getBase(0),gfd.getBound(0),gfd.getBase(1),gfd.getBound(1),
       gfd.getBase(2),gfd.getBound(2),gfd.getBase(3),gfd.getBound(3),
       arrayData.getBase(0),arrayData.getBound(0),arrayData.getBase(1),arrayData.getBound(1),
       arrayData.getBase(2),arrayData.getBound(2),arrayData.getBase(3),arrayData.getBound(3),
       arrayData.getBase(0),arrayData.getBound(0),
       ndm[0][0],ndm[1][0],ndm[0][1],ndm[1][1],ndm[0][2],ndm[1][2],  // dimensions for mask
       uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),uLocal.getBase(2),uLocal.getBound(2),
       *rxp,
       *up,*up,*getDataPointer(adguCoeff), *maskp,
       scalarData,*getDataPointer(gfd),*arrayDatap,*arrayDatap,
       dx[0],dr[0],ipar[0], par[0], ca,cb, uC.getBase(0),uC(uC.getBase(0)), fC.getBase(0),fC(fC.getBase(0)),
       side,axis,grid, (int)bcType, option,gridType,orderOfAccuracy,useWhereMask,bcParameters.lineToAssign );
    }
    

    delete rhsp;

//      Index I1m,I2m,I3m;
//      getGhostIndex( c.indexRange(),side,axis,I1m,I2m,I3m,+1); // first ghost line
//      uNew=u(I1m,I2m,I3m,u.dimension(3));
    
    timeForADotGradU+=getCPU()-time0;
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
// *     realSerialArray & adguCoeff   = bcType==BCTypes::neumann ? neumannCoeff[axis][side] : mixedDerivativeCoeff[axis][side];
// * 
// *     if( !coeffIsSet[axis][side] )
// *     { // generate coefficients if they have not already been set
// *       coeffIsSet[axis][side]=true; 
// *       buildNeumannCoefficientMatrix(*this,adguCoeff,M,I1,I2,I3,normal,b0,b1,numberOfDimensions,side,axis);
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
// * 	    adguCoeff(M2( 0,-1),I1,I2,I3)*uA(0,I1  ,I2-1,I3,uC(n))
// * 	    +adguCoeff(M2(-1, 0),I1,I2,I3)*uA(0,I1-1,I2  ,I3,uC(n))
// * 	    +adguCoeff(M2( 0, 0),I1,I2,I3)*uA(0,I1  ,I2  ,I3,uC(n))
// * 	    +adguCoeff(M2(+1, 0),I1,I2,I3)*uA(0,I1+1,I2  ,I3,uC(n))
// * 	    +adguCoeff(M2( 0,+1),I1,I2,I3)*uA(0,I1  ,I2+1,I3,uC(n))
// * 	    ))/adguCoeff(mGhost,I1,I2,I3); )
// *       }
// *       else
// *       {
// * 	WHERE_MASK0( uA(0,I1m,I2m,I3m,uC(n))=( 
// * 	  rhs(0,I1,I2,I3) - (
// * 	     adguCoeff(M3( 0, 0,-1),I1,I2,I3)*uA(0,I1  ,I2  ,I3-1,uC(n))
// * 	    +adguCoeff(M3( 0,-1, 0),I1,I2,I3)*uA(0,I1  ,I2-1,I3  ,uC(n))
// * 	    +adguCoeff(M3(-1, 0, 0),I1,I2,I3)*uA(0,I1-1,I2  ,I3  ,uC(n))
// * 	    +adguCoeff(M3( 0, 0, 0),I1,I2,I3)*uA(0,I1  ,I2  ,I3  ,uC(n))
// * 	    +adguCoeff(M3(+1, 0, 0),I1,I2,I3)*uA(0,I1+1,I2  ,I3  ,uC(n))
// * 	    +adguCoeff(M3( 0,+1, 0),I1,I2,I3)*uA(0,I1  ,I2+1,I3  ,uC(n))
// * 	    +adguCoeff(M3( 0, 0,+1),I1,I2,I3)*uA(0,I1  ,I2  ,I3+1,uC(n))
// * 	    ))/adguCoeff(mGhost,I1,I2,I3); )
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
