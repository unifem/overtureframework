#include "Ogmg.h"
#include "ParallelUtility.h"
#include "gridFunctionNorms.h"

// Define an Index from a base and bound and optional stride
Index IndexBB(int base, int bound, int stride )
{
//  return Range(base,bound,stride);
  return Index(base,(bound-base+stride)/stride,stride);
}

// Change the stride on an Index
Index IndexBB(Index I, const int stride )
{
  return Index(I.getBase(),(I.getBound()-I.getBase()+stride)/stride,stride);
}

#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( int i3=I3Base; i3<=I3Bound; i3++ )  \
  for( int i2=I2Base; i2<=I2Bound; i2++ )  \
  for( int i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase(),\
  I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();\
  for( int i3=I3Base; i3<=I3Bound; i3++ )  \
  for( int i2=I2Base; i2<=I2Bound; i2++ )  \
  for( int i1=I1Base; i1<=I1Bound; i1++ )


//\begin{>>OgmgInclude.tex}{\subsection{getNumberOfIterations}} 
int Ogmg::
chooseBestSmoother()
// ======================================================================================
// /Description:
//     Attempt to guess which smoother will work best on each grid.
//\end{OgmgInclude.tex}
// ======================================================================================
{
//   int level=0;
//   CompositeGrid & cg = mgcg;
  
//   for( int grid=0; grid<c.numberOfComponentGrids(); grid++ )
//   {
//   }
  

  return 0;
}





//----------------------------------------------------------------
//  Return the "mean" value of a grid function
//  This is a special definition of the mean, used by Ogmg
//  The mean is just the sum of all points with mask!=0
//----------------------------------------------------------------
real Ogmg::
getMean(realCompositeGridFunction & u)
{
  bool useOpt=true; // use new optimized version

  CompositeGrid & cg = (CompositeGrid&)(*u.gridCollection);
  real mean=0.;
  Index I1,I2,I3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    getIndex(cg[grid].extendedIndexRange(),I1,I2,I3);
    if( useOpt )
    {
      OV_GET_SERIAL_ARRAY(real,u[grid],uLocal);
      int includeGhost=0; // do NOT include parallel ghost points
      bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost);
      if( ok )
      {
	real *up = uLocal.Array_Descriptor.Array_View_Pointer2;
	const int uDim0=uLocal.getRawDataSize(0);
	const int uDim1=uLocal.getRawDataSize(1);
        #define U(i0,i1,i2) up[i0+uDim0*(i1+uDim1*(i2))]

        OV_GET_SERIAL_ARRAY(int,cg[grid].mask(),maskLocal);
	const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
	const int maskDim0=maskLocal.getRawDataSize(0);
	const int maskDim1=maskLocal.getRawDataSize(1);
        #define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]
	int i1,i2,i3;
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  if( MASK(i1,i2,i3)!=0 )
	  {
	    mean+=U(i1,i2,i3);
	  }
	}
      }
    }
    else
    {
      // non-opt version
      where( cg[grid].mask()(I1,I2,I3)!=0 )
      {
	mean+=sum(u[grid](I1,I2,I3));
      }
    }
    
  }
  mean =ParallelUtility::getSum(mean);
  
  return mean;
}
#undef U
#undef MASK

//---------------------------------------------------------------
/// \brief Set the "mean" of a grid function 
/// The mean is just the sum of all points with mask!=0
//--------------------------------------------------------------
void Ogmg::
setMean(realCompositeGridFunction & u, const real meanValue, int level)
{
  if( !parameters.assignMeanValueForSingularProblem ) return;

  bool useOpt=true; // use new optimized version

  CompositeGrid & cg = (CompositeGrid&)(*u.gridCollection);
  real mean=0.;  // holds mean of u 
  int count=0;   // counts number of valid points on the grid 
  Index I1,I2,I3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    getIndex(cg[grid].extendedIndexRange(),I1,I2,I3);
    if( useOpt ) // *wdh* 100604
    {
      OV_GET_SERIAL_ARRAY(real,u[grid],uLocal);
      int includeGhost=0; // do NOT include parallel ghost points
      bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost);
      if( ok )
      {
	real *up = uLocal.Array_Descriptor.Array_View_Pointer2;
	const int uDim0=uLocal.getRawDataSize(0);
	const int uDim1=uLocal.getRawDataSize(1);
        #define U(i0,i1,i2) up[i0+uDim0*(i1+uDim1*(i2))]

        OV_GET_SERIAL_ARRAY(int,cg[grid].mask(),maskLocal);
	const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
	const int maskDim0=maskLocal.getRawDataSize(0);
	const int maskDim1=maskLocal.getRawDataSize(1);
        #define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]
	int i1,i2,i3;
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  if( MASK(i1,i2,i3)!=0 )
	  {
	    mean+=U(i1,i2,i3);
	    count++;
	  }
	}
        #undef U
        #undef MASK
      }
    }
    else
    { // non-opt version

      intArray mask = cg[grid].mask()(I1,I2,I3)!=0;
      where( mask )
      {
	mean+=sum(u[grid](I1,I2,I3));
	// doesn't work: count++;
      }
      count+=sum(mask);   
    }
  }
  
  // Sum mean and count over all processors
  real val[2]={mean, count}, valSum[2]; // 
  ParallelUtility::getSums(val,valSum,2);
  mean=valSum[0]; 
  count=int( valSum[1] +.5 );

//   mean =ParallelUtility::getSum(mean);
//   count=ParallelUtility::getSum(count);

  if( Ogmg::debug & 4 )
  {
    printF("%*.1s Ogmg::setMean: level=%i, actual-mean/count=%g, count=%i, meanValue - actual-mean=%8.2e\n",
	   level*4," ",level,mean/max(1,count),count,meanValue-mean);
  }

  const real value=(meanValue-mean)/max(1,count);

  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    getIndex(cg[grid].dimension(),I1,I2,I3);  // shift all points ****
    if( useOpt )
    {
      OV_GET_SERIAL_ARRAY(real,u[grid],uLocal);
      bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,1);
      if( ok )
      {
	real *up = uLocal.Array_Descriptor.Array_View_Pointer2;
	const int uDim0=uLocal.getRawDataSize(0);
	const int uDim1=uLocal.getRawDataSize(1);
        #define U(i0,i1,i2) up[i0+uDim0*(i1+uDim1*(i2))]
	int i1,i2,i3;
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  U(i1,i2,i3)+=value;
	}
        #undef U
      }
    }
    else
    { // non-opt version
      u[grid](I1,I2,I3)+=value;
    }
  }

  
  u.periodicUpdate();  
}

#define l2normOpt EXTERN_C_NAME(l2normopt)
#define l2AndMaxNormOpt EXTERN_C_NAME(l2andmaxnormopt)
#define maxNormOpt EXTERN_C_NAME(maxnormopt)
#define l2ErrorOpt EXTERN_C_NAME(l2erroropt)
#define getL2AndMaxNormOpt EXTERN_C_NAME(getl2andmaxnormopt)
#define getMaxNormOpt EXTERN_C_NAME(getmaxnormopt)
#define getL2ErrorOpt EXTERN_C_NAME(getl2erroropt)

extern "C"
{

  void l2normOpt( const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
		  const int &nd3a, const int &nd3b,
		  const int &n1a, const int &n1b, 
		  const int &n2a, const int &n2b, 
		  const int &n3a, const int &n3b, 
		  const real & u, const int & mask, real & norm, int & count );

  void l2AndMaxNormOpt( const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
		  const int &nd3a, const int &nd3b,
		  const int &n1a, const int &n1b, 
		  const int &n2a, const int &n2b, 
		  const int &n3a, const int &n3b, 
		  const real & u, const int & mask, real & norm, int & count, real & uMax );

  void maxNormOpt( const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
		  const int &nd3a, const int &nd3b,
		  const int &n1a, const int &n1b, 
		  const int &n2a, const int &n2b, 
		  const int &n3a, const int &n3b, 
		  const real & u, const int & mask, real & uMax );

  void l2ErrorOpt( const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
		  const int &nd3a, const int &nd3b,
		  const int &n1a, const int &n1b, 
		  const int &n2a, const int &n2b, 
		  const int &n3a, const int &n3b, 
		  const real & u, const real & v, const int & mask, real & norm, int & count );


// --- These versions take a maskOption
void getL2normOpt( const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
		   const int &nd3a, const int &nd3b,
		   const int &n1a, const int &n1b, 
		   const int &n2a, const int &n2b, 
		   const int &n3a, const int &n3b, 
		   const real & u, const int & mask, real & norm, int & count, const int & maskOption );


void getMaxNormOpt( const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
		    const int &nd3a, const int &nd3b,
		    const int &n1a, const int &n1b, 
		    const int &n2a, const int &n2b, 
		    const int &n3a, const int &n3b, 
		    const real & u, const int & mask, real & uMax, const int & maskOption );

void getL2AndMaxNormOpt( const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
			 const int &nd3a, const int &nd3b,
			 const int &n1a, const int &n1b, 
			 const int &n2a, const int &n2b, 
			 const int &n3a, const int &n3b, 
			 const real & u, const int & mask, real & norm, int & count, real & uMax, 
                         const int & maskOption );


}

//---------------------------------------------------------
//  Return    sqrt(  (SUM u^2) / number of points )
//---------------------------------------------------------
real Ogmg::
l2Norm(const realCompositeGridFunction & u )
{
  real returnValue=0.;
  if( true )
  {
    // *wdh* 110311 Use version from gridFunctionNorms:
    returnValue=::l2Norm(u);
  }
  else
  {
    // old
    CompositeGrid & c = (CompositeGrid&)(*u.gridCollection);
    int count=0;
    Index I1,I2,I3;
    for( int grid=0; grid<c.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg =c[grid];
      getIndex(mg.gridIndexRange(),I1,I2,I3);
      if( parameters.useOptimizedVersion )
      {
	const IntegerArray & d = mg.dimension();
	int countg;
	real uSquared;
	l2normOpt( d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2),
		   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
		   *getDataPointer(u[grid]),  *getDataPointer(mg.mask()), uSquared, countg);
	returnValue+=uSquared;
	count+=countg;
      }
      else
      {
	const intArray & mask = evaluate( mg.mask()(I1,I2,I3)!=0 );
	count+=sum(mask);
	where( mask )
	{
	  returnValue+=sum(u[grid](I1,I2,I3)*u[grid](I1,I2,I3));
	}
      }
    }
  
    returnValue=sqrt(returnValue/max(1,count));
  }
  
  return returnValue;
}


real Ogmg::
l2Norm(const realMappedGridFunction & u )
{
  MappedGrid & mg = *u.getMappedGrid();
  real returnValue=0.;
  int count=0;
  if( parameters.useOptimizedVersion )
  {
    #ifdef USE_PPP
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
      const intArray & maskd = mg.mask();
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(maskd,maskLocal);
      IntegerArray gid; gid = mg.gridIndexRange();
      for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      {
	gid(0,axis) = max(gid(0,axis),maskLocal.getBase(axis) +maskd.getGhostBoundaryWidth(axis));
	gid(1,axis) = min(gid(1,axis),maskLocal.getBound(axis)-maskd.getGhostBoundaryWidth(axis));
      }
    #else
      const realSerialArray & uLocal = u;
      const intSerialArray & maskLocal = mg.mask();
      const IntegerArray & gid = mg.gridIndexRange();
    #endif

    l2normOpt( maskLocal.getBase(0),maskLocal.getBound(0),
	       maskLocal.getBase(1),maskLocal.getBound(1),
	       maskLocal.getBase(2),maskLocal.getBound(2),
	       gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
	       *getDataPointer(uLocal),  *getDataPointer(maskLocal), returnValue, count );

    #ifdef USE_PPP
      returnValue=ParallelUtility::getSum(returnValue);
      count=ParallelUtility::getSum(count);
    #endif

    returnValue=sqrt(returnValue/max(1,count));

    // printf(" l2Norm: count=%i, returnValue=%8.2e\n",count,returnValue);
    
  }
  else
  {
    Index I1,I2,I3;
    getIndex(mg.gridIndexRange(),I1,I2,I3);
    const intArray & mask = evaluate( mg.mask()(I1,I2,I3)!=0 );
    count+=sum(mask);
    where( mask )
    {
      returnValue+=sum(u(I1,I2,I3)*u(I1,I2,I3));
    }
    returnValue=sqrt(returnValue/max(1,count));
  }
  
  return returnValue;
}

#undef l2normOpt 

//---------------------------------------------------------
//  Return    max |u| 
//---------------------------------------------------------
real Ogmg::
maxNorm(const realCompositeGridFunction & u )
{
  CompositeGrid & c = (CompositeGrid&)(*u.gridCollection);
  real returnValue=0.;
  for( int grid=0; grid<c.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = c[grid];
    if( parameters.useOptimizedVersion )
    {
      #ifdef USE_PPP
        realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
        const intArray & maskd = mg.mask();
        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(maskd,maskLocal);
        IntegerArray gid; gid = mg.gridIndexRange();
        for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	{
	  gid(0,axis) = max(gid(0,axis),maskLocal.getBase(axis) +maskd.getGhostBoundaryWidth(axis));
	  gid(1,axis) = min(gid(1,axis),maskLocal.getBound(axis)-maskd.getGhostBoundaryWidth(axis));
	}
      #else
        const realSerialArray & uLocal = u[grid];
        const intSerialArray & maskLocal = mg.mask();
        const IntegerArray & gid = mg.gridIndexRange();
      #endif    

      real maxNorm=0.;
      const int maskOption=1;  // *wdh* 110321 -- compute for mask>0 instead of mask!=0
      getMaxNormOpt( maskLocal.getBase(0),maskLocal.getBound(0),
		     maskLocal.getBase(1),maskLocal.getBound(1),
		     maskLocal.getBase(2),maskLocal.getBound(2),
		     gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
		     *getDataPointer(uLocal),  *getDataPointer(maskLocal), maxNorm, maskOption );
      #ifdef USE_PPP
        maxNorm=ParallelUtility::getMaxValue(maxNorm);
      #endif
      returnValue=max(returnValue,maxNorm);
    }
    else
    {
     Index I1,I2,I3;
      getIndex(mg.gridIndexRange(),I1,I2,I3);
      where( mg.mask()(I1,I2,I3)!=0 )
      {
	returnValue=max(returnValue,max(fabs(u[grid](I1,I2,I3))));
      }
    }
    
  }
  return returnValue;

}

real Ogmg::
maxNorm(const realMappedGridFunction & u )
{
  MappedGrid & mg = *u.getMappedGrid();
  real returnValue=0.;
  if( parameters.useOptimizedVersion )
  {
    #ifdef USE_PPP
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
      const intArray & maskd = mg.mask();
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(maskd,maskLocal);
      IntegerArray gid; gid = mg.gridIndexRange();
      for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	{
	  gid(0,axis) = max(gid(0,axis),maskLocal.getBase(axis) +maskd.getGhostBoundaryWidth(axis));
	  gid(1,axis) = min(gid(1,axis),maskLocal.getBound(axis)-maskd.getGhostBoundaryWidth(axis));
	}
    #else
      const realSerialArray & uLocal = u;
      const intSerialArray & maskLocal = mg.mask();
      const IntegerArray & gid = mg.gridIndexRange();
    #endif

    real maxNorm=0.;
    const int maskOption=1;  // *wdh* 110321 -- compute for mask>0 instead of mask!=0
    getMaxNormOpt( maskLocal.getBase(0),maskLocal.getBound(0),
		   maskLocal.getBase(1),maskLocal.getBound(1),
		   maskLocal.getBase(2),maskLocal.getBound(2),
		   gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
		   *getDataPointer(uLocal),  *getDataPointer(maskLocal), returnValue, maskOption );
    #ifdef USE_PPP
      returnValue=ParallelUtility::getMaxValue(returnValue);
    #endif
  }
  else
  {
    Index I1,I2,I3;
    getIndex(mg.gridIndexRange(),I1,I2,I3);
    where( mg.mask()(I1,I2,I3)!=0 )
    {
      returnValue=max(returnValue,max(fabs(u(I1,I2,I3))));
    }
  }
  
  return returnValue;

}

//---------------------------------------------------------
//  Return    sqrt(  (SUM (u-v)^2) / number of points )
//---------------------------------------------------------
real Ogmg::
l2Error(const realCompositeGridFunction & u, const realCompositeGridFunction & v )
{
  CompositeGrid & c = (CompositeGrid&)(*u.gridCollection);
  real returnValue=0.;
  int count=0;
  Index I1,I2,I3;
  for( int grid=0; grid<c.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg =c[grid];
    if( parameters.useOptimizedVersion )
    {
      #ifdef USE_PPP
        realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
        realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v[grid],vLocal);
        const intArray & maskd = mg.mask();
        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(maskd,maskLocal);
        IntegerArray gid; gid = mg.gridIndexRange();
        for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
        {
  	  gid(0,axis) = max(gid(0,axis),maskLocal.getBase(axis) +maskd.getGhostBoundaryWidth(axis));
  	  gid(1,axis) = min(gid(1,axis),maskLocal.getBound(axis)-maskd.getGhostBoundaryWidth(axis));
        }
      #else
        const realSerialArray & uLocal = u[grid];
        const realSerialArray & vLocal = v[grid];
        const intSerialArray & maskLocal = mg.mask();
        const IntegerArray & gid = mg.gridIndexRange();
      #endif

      int countg=0;
      real uSquared;
      l2ErrorOpt( maskLocal.getBase(0),maskLocal.getBound(0),
		  maskLocal.getBase(1),maskLocal.getBound(1),
		  maskLocal.getBase(2),maskLocal.getBound(2),
		  gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
		  *getDataPointer(uLocal), *getDataPointer(vLocal), 
		  *getDataPointer(maskLocal), uSquared, countg);
      returnValue+=uSquared;
      count+=countg;
    }
    else
    {
      getIndex(mg.gridIndexRange(),I1,I2,I3);
      const intArray & mask = evaluate( mg.mask()(I1,I2,I3)!=0 );
      count+=sum(mask);
      realArray w; 
      w = u[grid](I1,I2,I3)-v[grid](I1,I2,I3);
      where( mask )
      {
	returnValue+=sum(w*w);
      }
    }
  }
  #ifdef USE_PPP
    returnValue=ParallelUtility::getSum(returnValue);
    count=ParallelUtility::getSum(count);
  #endif

  returnValue=sqrt(returnValue/max(1,count));
  return returnValue;
}

