//===============================================================================
//  Define norms of grid functions 
//
//
//==============================================================================
#include "Overture.h"
#include "ParallelUtility.h"
#include "gridFunctionNorms.h"

#define getL2normOpt EXTERN_C_NAME(getl2normopt)
#define getMaxNormOpt EXTERN_C_NAME(getmaxnormopt)
#define getL2AndMaxNormOpt EXTERN_C_NAME(getl2andmaxnormopt)
#define getLpNormOpt EXTERN_C_NAME(getlpnormopt)
#define getAreaWeightedLpNorm EXTERN_C_NAME(getareaweightedlpnorm)
extern "C"
{

  void getL2normOpt( const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
		  const int &nd3a, const int &nd3b,
		  const int &n1a, const int &n1b, 
		  const int &n2a, const int &n2b, 
		  const int &n3a, const int &n3b, 
		  const real & u, const int & mask, real & norm, int & count, 
                  const int & maskOption  );

   void getMaxNormOpt( const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
		  const int &nd3a, const int &nd3b,
		  const int &n1a, const int &n1b, 
		  const int &n2a, const int &n2b, 
		  const int &n3a, const int &n3b, 
		  const real & u, const int & mask, real & uMax, 
                  const int & maskOption );


   void getL2AndMaxNormOpt( const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
		            const int &nd3a, const int &nd3b,
			    const int &n1a, const int &n1b, 
			    const int &n2a, const int &n2b, 
			    const int &n3a, const int &n3b, 
			    const real & u, const int & mask, 
			    real & uSquared, int & count,
			    real & uMax, 
			    const int & maskOption );

  void getLpNormOpt( const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
		  const int &nd3a, const int &nd3b,
		  const int &n1a, const int &n1b, 
		  const int &n2a, const int &n2b, 
		  const int &n3a, const int &n3b, 
		  const real & u, const int & mask, real & norm, int & count, 
                  const int & maskOption, const int & p  );

  void getAreaWeightedLpNorm( const int & nd, const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
		  const int &nd3a, const int &nd3b,
		  const int &n1a, const int &n1b, 
		  const int &n2a, const int &n2b, 
		  const int &n3a, const int &n3b, 
		  const real & u, const int & mask, const real & rsxy, int & ipar, real & rpar );

}

//---------------------------------------------------------
//  Return    sqrt(  (SUM u^2) / number of points )
//
// maskOption(input) : maskOption=0 : check points where mask(i1,i2,i3).ne.0
//                     maskOption>0 : check points where mask(i1,i2,i3)>0
// extra (input) : check gridIndexRange() plus extra points in each direction. Set extra=1 to get
//                 one line of ghost points, for example.
//---------------------------------------------------------
real 
l2Norm(const realCompositeGridFunction & u, const int cc /* = 0 */, int maskOption /* = 0 */, int extra /* = 0 */ )
{
  CompositeGrid & c = (CompositeGrid&)(*u.gridCollection);
  real returnValue=0.;
  int count=0;
  Index I1,I2,I3;
  for( int grid=0; grid<c.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = c[grid];
    const intArray & maskd = mg.mask();
    #ifdef USE_PPP
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(maskd,maskLocal);
    #else
      const realSerialArray & uLocal = u[grid];
      const intSerialArray & maskLocal = mg.mask();
    #endif    
    IntegerArray gid; gid = mg.gridIndexRange();
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
    	gid(0,axis) = max(gid(0,axis)-extra,maskLocal.getBase(axis) +maskd.getGhostBoundaryWidth(axis));
    	gid(1,axis) = min(gid(1,axis)+extra,maskLocal.getBound(axis)-maskd.getGhostBoundaryWidth(axis));
    }

    const realSerialArray & uu = uLocal;

    real uSquared=0.;
    int countg=0;
    getL2normOpt( maskLocal.getBase(0),maskLocal.getBound(0),
		  maskLocal.getBase(1),maskLocal.getBound(1),
		  maskLocal.getBase(2),maskLocal.getBound(2),
		  gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
		  uu(uu.getBase(0),uu.getBase(1),uu.getBase(2),cc),  *getDataPointer(maskLocal), 
		  uSquared, countg, maskOption );
    #ifdef USE_PPP
      uSquared=ParallelUtility::getSum(uSquared);
      countg=ParallelUtility::getSum(countg);
    #endif

    returnValue+=uSquared;
    count+=countg;
  }
  
  returnValue=sqrt(returnValue/max(1,count));
  return returnValue;
}

real 
l2Norm(const realMappedGridFunction & u, const int cc /* = 0 */, int maskOption /* = 0 */, int extra /* = 0 */ )
{
  MappedGrid & mg = *u.getMappedGrid(); 
  real returnValue=0.;
  int count=0;
  Index I1,I2,I3;
  const intArray & maskd = mg.mask();
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(maskd,maskLocal);
  #else
    const realSerialArray & uLocal = u;
    const intSerialArray & maskLocal = mg.mask();
  #endif    
  IntegerArray gid; gid = mg.gridIndexRange();
  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
    gid(0,axis) = max(gid(0,axis)-extra,maskLocal.getBase(axis) +maskd.getGhostBoundaryWidth(axis));
    gid(1,axis) = min(gid(1,axis)+extra,maskLocal.getBound(axis)-maskd.getGhostBoundaryWidth(axis));
  }

  const realSerialArray & uu = uLocal;

  real uSquared=0.;
  int countg=0;
  getL2normOpt( maskLocal.getBase(0),maskLocal.getBound(0),
		  maskLocal.getBase(1),maskLocal.getBound(1),
		  maskLocal.getBase(2),maskLocal.getBound(2),
		  gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
		  uu(uu.getBase(0),uu.getBase(1),uu.getBase(2),cc),  *getDataPointer(maskLocal), 
		  uSquared, countg, maskOption );
  #ifdef USE_PPP
    uSquared=ParallelUtility::getSum(uSquared);
    countg=ParallelUtility::getSum(countg);
  #endif

  returnValue+=uSquared;
  count+=countg;
  
  returnValue=sqrt(returnValue/max(1,count));
  return returnValue;
}

//---------------------------------------------------------
//  Return    max |u| 
// maskOption(input) : maskOption=0 : check points where mask(i1,i2,i3).ne.0
//                     maskOption>0 : check points where mask(i1,i2,i3)>0
// extra (input) : check gridIndexRange() plus extra points in each direction. Set extra=1 to get
//                 one line of ghost points, for exmaple.
//---------------------------------------------------------
real 
maxNorm(const realCompositeGridFunction & u, const int cc /* = 0 */, int maskOption /* = 0 */, int extra /* = 0 */ )
{
  CompositeGrid & c = (CompositeGrid&)(*u.gridCollection);
  real returnValue=0.;
  for( int grid=0; grid<c.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = c[grid];
    const intArray & maskd = mg.mask();
    #ifdef USE_PPP
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(maskd,maskLocal);
    #else
      const realSerialArray & uLocal = u[grid];
      const intSerialArray & maskLocal = mg.mask();
    #endif    

    IntegerArray gid; gid = mg.gridIndexRange();
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      gid(0,axis) = max(gid(0,axis)-extra,maskLocal.getBase(axis) +maskd.getGhostBoundaryWidth(axis));
      gid(1,axis) = min(gid(1,axis)+extra,maskLocal.getBound(axis)-maskd.getGhostBoundaryWidth(axis));
    }

    const realSerialArray & uu = uLocal;

    real maxNorm=0.;
    getMaxNormOpt( maskLocal.getBase(0),maskLocal.getBound(0),
		   maskLocal.getBase(1),maskLocal.getBound(1),
		   maskLocal.getBase(2),maskLocal.getBound(2),
		   gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
		   uu(uu.getBase(0),uu.getBase(1),uu.getBase(2),cc),  *getDataPointer(maskLocal), 
		   maxNorm, maskOption );
    #ifdef USE_PPP
      maxNorm=ParallelUtility::getMaxValue(maxNorm);
    #endif
    returnValue=max(returnValue,maxNorm);

    // printf("maxNorm: grid=%i, maxNorm=%9.2e\n",grid,maxNorm);

  }
  return returnValue;

}

real 
maxNorm(const realMappedGridFunction & u, const int cc /* = 0 */, int maskOption /* = 0 */, int extra /* = 0 */ )
{
  MappedGrid & mg = *u.getMappedGrid(); 
  real returnValue=0.;
  const intArray & maskd = mg.mask();
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(maskd,maskLocal);
  #else
    const realSerialArray & uLocal = u;
    const intSerialArray & maskLocal = mg.mask();
  #endif    
  IntegerArray gid; gid = mg.gridIndexRange();
  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
    gid(0,axis) = max(gid(0,axis)-extra,maskLocal.getBase(axis) +maskd.getGhostBoundaryWidth(axis));
    gid(1,axis) = min(gid(1,axis)+extra,maskLocal.getBound(axis)-maskd.getGhostBoundaryWidth(axis));
  }

  const realSerialArray & uu = uLocal;

  real maxNorm=0.;
  getMaxNormOpt( maskLocal.getBase(0),maskLocal.getBound(0),
		   maskLocal.getBase(1),maskLocal.getBound(1),
		   maskLocal.getBase(2),maskLocal.getBound(2),
		   gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
		   uu(uu.getBase(0),uu.getBase(1),uu.getBase(2),cc),  *getDataPointer(maskLocal), 
		   maxNorm, maskOption );
  #ifdef USE_PPP
    maxNorm=ParallelUtility::getMaxValue(maxNorm);
  #endif
  returnValue=max(returnValue,maxNorm);
    
  return returnValue;

}

//-----------------------------------------------------------------------------------------------------
//  Return the Lp norm of u:   
//
// normOption==0 : 
//          (  (SUM |u_i|^p) / number of points )^(1/p)
//
// normOption==1 : area-weighted divided by the total area: 
//          (SUM |u_i|^p dV_i /  SUM dV_i )^(1/p)
//
// normOption==2 : area-weighted 
//          (SUM |u_i|^p dV_i /  SUM dV_i )^(1/p)
//
// p (input) : the value of "p" in the p-Norm
// maskOption(input) : maskOption=0 : check points where mask(i1,i2,i3).ne.0
//                     maskOption>0 : check points where mask(i1,i2,i3)>0
// extra (input) : check gridIndexRange() plus extra points in each direction. Set extra=1 to get
//                 one line of ghost points, for example.
//
// NOTE: We do not include points that are hidden by refinement
//------------------------------------------------------------------------------------------------------
real 
lpNorm(const int p, const realCompositeGridFunction & u, const int cc /*=0 */, int maskOption /*=0 */, 
       int extra /*=0 */, int normOption /*=0 */ )
{
  CompositeGrid & c = (CompositeGrid&)(*u.gridCollection);
  real returnValue=0.;
  Index I1,I2,I3;

  real up = 0., vol=0.;
  int count = 0;
 
  for( int grid=0; grid<c.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = c[grid];
    const intArray & maskd = mg.mask();
    #ifdef USE_PPP
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(maskd,maskLocal);
    #else
      const realSerialArray & uLocal = u[grid];
      const intSerialArray & maskLocal = mg.mask();
    #endif    
    IntegerArray gid; gid = mg.gridIndexRange();
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      gid(0,axis) = max(gid(0,axis)-extra,maskLocal.getBase(axis) +maskd.getGhostBoundaryWidth(axis));
      gid(1,axis) = min(gid(1,axis)+extra,maskLocal.getBound(axis)-maskd.getGhostBoundaryWidth(axis));
    }

    const realSerialArray & uu = uLocal;

    real upg = 0.;
    int countg = 0;
    if( normOption==0 )
    {
      getLpNormOpt( maskLocal.getBase(0),maskLocal.getBound(0),
		    maskLocal.getBase(1),maskLocal.getBound(1),
		    maskLocal.getBase(2),maskLocal.getBound(2),
		    gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
		    uu(uu.getBase(0),uu.getBase(1),uu.getBase(2),cc),  *getDataPointer(maskLocal), 
		    upg, countg, maskOption, p );
      up+=upg;
      count+=countg;
    }
    else
    {
      int ipar[20];
      real rpar[20];

      bool isRectangular = mg.isRectangular();
      if( !isRectangular )
      {
	mg.update(MappedGrid::THEinverseVertexDerivative );
      }
      
      #ifdef USE_PPP
        realSerialArray rsxyLocal; 
        if( !isRectangular ) getLocalArrayWithGhostBoundaries(mg.inverseVertexDerivative(),rsxyLocal);
      #else
        const realSerialArray & rsxyLocal = mg.inverseVertexDerivative();
      #endif  
      real *prsxy = isRectangular ? uLocal.getDataPointer() : rsxyLocal.getDataPointer();

      ipar[0]=maskOption;
      ipar[1]=p;
      ipar[2]=isRectangular ? 0 : 1;

      real dx[3]={1.,1.,1.}; // 
      if( isRectangular )
      {
	mg.getDeltaX(dx);
      }
      rpar[0]=dx[0];
      rpar[1]=dx[1];
      rpar[2]=dx[2];
      rpar[3]=mg.gridSpacing(0);
      rpar[4]=mg.gridSpacing(1);
      rpar[5]=mg.gridSpacing(2);
      rpar[6]=REAL_MIN*100.;
      
      getAreaWeightedLpNorm( mg.numberOfDimensions(),maskLocal.getBase(0),maskLocal.getBound(0),
		    maskLocal.getBase(1),maskLocal.getBound(1),
		    maskLocal.getBase(2),maskLocal.getBound(2),
		    gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
		    uu(uu.getBase(0),uu.getBase(1),uu.getBase(2),cc),  *getDataPointer(maskLocal), 
		    *prsxy, *ipar,*rpar );

      upg=rpar[10];
      up+=upg;

      real volg=rpar[11];
      vol+=volg;
  
      // printF(" lpNorm:INFO: p=%i, grid=%i, cc=%i:   SUM [u_i]^p dV_i =%9.3e,    SUM dV_i=%9.3e\n",p,grid,cc,upg,volg);
      

    }
  }
  up=ParallelUtility::getSum(up);
  if( normOption==0 ) 
  {
    count=ParallelUtility::getSum(count);
    up/=count;
  }
  else if( normOption==1 )
  {
    // divide by the total volume: 
    vol=ParallelUtility::getSum(vol);
    up/=max(REAL_MIN*100.,vol);
  }
  
  returnValue=pow(up,1./p);

  return returnValue;
}

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

int GridFunctionNorms::
getBounds(const realCompositeGridFunction & u, 
	  RealArray & uMin,
	  RealArray & uMax, 
	  const Range & C /* =nullRange */ )
// =================================================================================================
// /Description:
//    Compute min and max of some or all components of a grid function
// /u (input):
// /uMin (output) : array of minimum values.
// /uMax (output) : array of maximum values.
// /C (input) : Specify components to check. By default determine max and min for all components
// ================================================================================================
{
  const CompositeGrid & cg = *u.getCompositeGrid();

  int cBase,cBound;
  if( C==nullRange )
  {
    cBase =  u.getComponentBase(0);
    cBound = u.getComponentBound(0);
  }
  else
  {
    cBase=C.getBase();
    cBound=C.getBound();
  }

  if( uMin.getBase(0) > cBase || uMin.getBound(0)<cBound )
    uMin.redim(Range(cBase,cBound));
  if( uMax.getBase(0) > cBase || uMax.getBound(0)<cBound )
    uMax.redim(Range(cBase,cBound));
  
  uMin=FLT_MAX;
  uMax=-FLT_MAX;

  real *puMin = uMin.getDataPointer();
  real *puMax = uMax.getDataPointer();
  #define UMIN(n) puMin[n]
  #define UMAX(n) puMax[n]
  Index I1,I2,I3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    const MappedGrid & mg = cg[grid];
    getIndex(mg.gridIndexRange(),I1,I2,I3 );

    const intArray & mask = mg.mask();
    realArray & ua = u[grid];

    realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(ua,uLocal);

    bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3);
    if( !ok ) continue;
    
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
    const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=maskLocal.getRawDataSize(0);
    const int maskDim1=maskLocal.getRawDataSize(1);
    const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]
    real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
    const int uDim0=uLocal.getRawDataSize(0);
    const int uDim1=uLocal.getRawDataSize(1);
    const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

    int i1,i2,i3;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      if( MASK(i1,i2,i3)!=0 )
      {
	for( int n=cBase; n<=cBound; n++ )
	{
	  UMIN(n)=min(UMIN(n),U(i1,i2,i3,n));
	  UMAX(n)=max(UMAX(n),U(i1,i2,i3,n));
	}
      } 
    }
#undef MASK
#undef U
       
  }
  // compute min and max over all processors
  #ifdef USE_PPP
   const int numberOfComponents=cBound-cBase+1;
   ParallelUtility::getMinValues(&uMin(cBase),&uMin(cBase),numberOfComponents);
   ParallelUtility::getMaxValues(&uMax(cBase),&uMax(cBase),numberOfComponents);
  #endif

  return 0;

#undef UMIN
#undef UMAX

}

int GridFunctionNorms::
getBounds(const realMappedGridFunction & u, 
	  RealArray & uMin,
	  RealArray & uMax, 
	  const Range & C /* =nullRange */ )
// =================================================================================================
// /Description:
//    Compute min and max of some or all components of a grid function
// /u (input):
// /uMin (output) : array of minimum values.
// /uMax (output) : array of maximum values.
// /C (input) : Specify components to check. By default determine max and min for all components
// ================================================================================================
{
  const MappedGrid & mg = *u.getMappedGrid();

  int cBase,cBound;
  if( C==nullRange )
  {
    cBase =  u.getComponentBase(0);
    cBound = u.getComponentBound(0);
  }
  else
  {
    cBase=C.getBase();
    cBound=C.getBound();
  }

  if( uMin.getBase(0) > cBase || uMin.getBound(0)<cBound )
    uMin.redim(Range(cBase,cBound));
  if( uMax.getBase(0) > cBase || uMax.getBound(0)<cBound )
    uMax.redim(Range(cBase,cBound));
  
  uMin=FLT_MAX;
  uMax=-FLT_MAX;

  real *puMin = uMin.getDataPointer();
  real *puMax = uMax.getDataPointer();
  #define UMIN(n) puMin[n]
  #define UMAX(n) puMax[n]
  Index I1,I2,I3;

  getIndex(mg.gridIndexRange(),I1,I2,I3 );

  const intArray & mask = mg.mask();
  const realArray & ua = u;

  realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(ua,uLocal);

  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);
  if( ok )
  {
    
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
    const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=maskLocal.getRawDataSize(0);
    const int maskDim1=maskLocal.getRawDataSize(1);
    const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]
    real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
    const int uDim0=uLocal.getRawDataSize(0);
    const int uDim1=uLocal.getRawDataSize(1);
    const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

    int i1,i2,i3;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      if( MASK(i1,i2,i3)!=0 )
      {
	for( int n=cBase; n<=cBound; n++ )
	{
	  UMIN(n)=min(UMIN(n),U(i1,i2,i3,n));
	  UMAX(n)=max(UMAX(n),U(i1,i2,i3,n));
	}
      } 
    }
#undef MASK
#undef U
  }
  
 // compute min and max over all processors
 #ifdef USE_PPP
  const int numberOfComponents=cBound-cBase+1;
  ParallelUtility::getMinValues(&uMin(cBase),&uMin(cBase),numberOfComponents);
  ParallelUtility::getMaxValues(&uMax(cBase),&uMax(cBase),numberOfComponents);
 #endif

 return 0;

#undef UMIN
#undef UMAX

}
