#include "Ogen.h"
#include "Overture.h"
#include "display.h"
#include "conversion.h"
#include "ParallelUtility.h"

static const int ISneededPoint = CompositeGrid::ISreservedBit2;  // from Cgsh.h
// Define a macro to index an A++ array with 3 dimensions *NOTE* a legal macro  --> #define MASK
// #define DEF_ARRAY_MACRO_3D(int,mask,MASK) \
//   int * mask ## p = mask.Array_Descriptor.Array_View_Pointer2;\
//   const int mask ## Dim0=mask.getRawDataSize(0);\
//   const int mask ## Dim1=mask.getRawDataSize(1);\
// #define MASK(i0,i1,i2) mask ## p[i0+mask ## Dim0*(i1+mask ## Dim1*(i2))]

// Macro to extract a local array with ghost boundaries
//  type = int/float/double/real
//  xd = distributed array
//  xs = serial array 
#ifdef USE_PPP
 #define GET_LOCAL(type,xd,xs)\
    type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
 #define GET_LOCAL_CONST(type,xd,xs)\
    type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
#else
 #define GET_LOCAL(type,xd,xs)\
    type ## SerialArray & xs = xd
 #define GET_LOCAL_CONST(type,xd,xs)\
    const type ## SerialArray & xs = xd
#endif

int Ogen::
determineMinimalIndexRange( CompositeGrid & cg )
// =============================================================================================================
// /Description:
//    Given a valid CompositeGrid, determine the actual range of useful points for each grid.
//
//  For now this is purely informational
// 
// =============================================================================================================
{
  if( debug & 1 )
  {
    const int numberOfBaseGrids = cg.numberOfBaseGrids();
  
    for( int grid=0; grid<numberOfBaseGrids; grid++ )
    {
      const intArray & maskd = cg[grid].mask();
      const IntegerArray & dimension = cg[grid].dimension();

      GET_LOCAL_CONST(int,maskd,mask);
      
      intSerialArray ia;
      if( mask.getLength(0)>0 )
        ia=(mask!=0).indexMap();
      Range R=ia.dimension(0);

      // displayMask(mask,"mask");

      IntegerArray indexRange(2,3);
      indexRange=0;
      if( R.getLength()>0 )
      {
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
	  indexRange(Start,axis)=min(ia(R,axis));
	  indexRange(End  ,axis)=max(ia(R,axis));
	}
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
	  indexRange(Start,axis)=ParallelUtility::getMinValue(indexRange(Start,axis));
	  indexRange(End  ,axis)=ParallelUtility::getMaxValue(indexRange(End  ,axis));
	}
      }
      printF("Minimal index range is [%2i,%5i]x[%2i,%5i]x[%2i,%5i], dimension=[%2i,%5i]x[%2i,%5i]x[%2i,%5i] for grid %s \n",
	     indexRange(0,0),indexRange(1,0), indexRange(0,1),indexRange(1,1), indexRange(0,2),indexRange(1,2),
	     dimension(0,0),dimension(1,0), dimension(0,1),dimension(1,1), dimension(0,2),dimension(1,2),
	     (const char*)cg[grid].mapping().getName(Mapping::mappingName));
    }
  }
  
  return 0;
}




int 
displayMaskNeeded( const intArray & mask, 
             const aString & label /* =nullString */ )
// =======================================================================================
// /Description:
// Display the mask array in a MappedGrid in a reasonable way
// The mask array in a MappedGrid is a bit-mapping that is difficult to look at
// if displayed in the formal way. This routine will display the mask in a more
// compact form (although some information is not printed) where each entry printed will mean:
// \begin{description}
//   \item[1] : ISdiscretizationPoint
//   \item[2] : ISghostPoint
//   \item[-1] : ISinterpolationPoint
// \end{description}   
//\end{displayInclude.tex}
// =======================================================================================
{
  
  intArray m;
  m.redim(mask);
  m=0;
  where( mask & MappedGrid::ISdiscretizationPoint )
    m=1;
  where( mask<0 && mask>-100 )
    m=mask;
  elsewhere( mask<0 )
    m=-1;
  elsewhere( mask & MappedGrid::ISghostPoint )
    m=2;

  where( mask & ISneededPoint )
  {
    m*=3;
  }

  display(m,label,"%3i");
  return 0;
}

static inline real
square( const real & x )
{
  return x*x;
}


// define XR(m,n) xr(i1,i2,i3,(m)+numberOfDimensions*(n))
// define XR2(m,n) xr2(j1,j2,j3,(m)+numberOfDimensions*(n))
#define XR(m,n) xra[n][m]
#define XR2(m,n) xra2[n][m]

real Ogen::
computeInterpolationQuality(CompositeGrid & cg, const int & grid, 
                            const int & i1, const int & i2, const int & i3,
                            real & qForward, real & qReverse )
// compute the quaility of the interpolation
// qForward : measure quality of interpolation
// qReverse :  measures quality if we interpolated in the opposite direction.

{
  real quality=0.;

  const int numberOfBaseGrids = cg.numberOfBaseGrids();
  const int numberOfDimensions = cg.numberOfDimensions();

  MappedGrid & c = cg[grid];
  intArray & inverseGrid = cg.inverseGrid[grid];

  const int grid2=inverseGrid(i1,i2,i3);
  assert( grid2>=0 && grid2<cg.numberOfComponentGrids() && grid2!=grid );


  MappedGrid & g2 = cg[grid2];
  const RealArray & dr2 = g2.gridSpacing();

  const bool useSizeQuality=TRUE;
  int axis;

  if( useSizeQuality )
  {
    realArray & rI = cg.inverseCoordinates[grid];
    const realArray & vertex  = c.vertex();
    const realArray & vertex2 = g2.vertex();
    const real offset = c.isAllCellCentered() ? 1. : .5;

    // base the quality on the relative sizes of the cells
    real cellSize, cellSize2;
    int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      jv[axis]=int( rI(i1,i2,i3,axis)/dr2(axis)+g2.indexRange(0,axis)+offset ); // closest point
      jv[axis]=max(g2.dimension(0,axis),min(g2.dimension(1,axis)-1,jv[axis]));
    }
    if( numberOfDimensions==2 )
    {
      jv[2]=g2.dimension(0,axis3);
      real xra[2][2], xra2[2][2];
      for(axis=0; axis<numberOfDimensions; axis++ ) 
      {
	xra[0][axis]=(vertex(i1+1,i2,i3,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(0);
	xra[1][axis]=(vertex(i1,i2+1,i3,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(1);
	xra2[0][axis]=(vertex2(j1+1,j2,j3,axis)-vertex2(j1,j2,j3,axis))/g2.gridSpacing(0);
	xra2[1][axis]=(vertex2(j1,j2+1,j3,axis)-vertex2(j1,j2,j3,axis))/g2.gridSpacing(1);
      }
      cellSize=fabs(XR(0,0)*XR(1,1)-XR(1,0)*XR(0,1));
      cellSize2=fabs(XR2(0,0)*XR2(1,1)-XR2(1,0)*XR2(0,1));
    }
    else if( numberOfDimensions==3 )
    {
      real xra[3][3], xra2[3][3];
      for(axis=0; axis<numberOfDimensions; axis++ ) 
      {
	xra[0][axis]=(vertex(i1+1,i2,i3,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(0);
	xra[1][axis]=(vertex(i1,i2+1,i3,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(1);
	xra[2][axis]=(vertex(i1,i2,i3+1,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(2);
	xra2[0][axis]=(vertex2(j1+1,j2,j3,axis)-vertex2(j1,j2,j3,axis))/g2.gridSpacing(0);
	xra2[1][axis]=(vertex2(j1,j2+1,j3,axis)-vertex2(j1,j2,j3,axis))/g2.gridSpacing(1);
	xra2[2][axis]=(vertex2(j1,j2,j3+1,axis)-vertex2(j1,j2,j3,axis))/g2.gridSpacing(2);
      }
      cellSize=fabs(XR(0,0)*(XR(1,1)*XR(2,2)-XR(1,2)*XR(2,1))  +
                    XR(1,0)*(XR(2,1)*XR(0,2)-XR(2,2)*XR(0,1))  +
                    XR(2,0)*(XR(0,1)*XR(1,2)-XR(0,2)*XR(1,1)) );
      cellSize2=fabs(XR2(0,0)*(XR2(1,1)*XR2(2,2)-XR2(1,2)*XR2(2,1))  +
                     XR2(1,0)*(XR2(2,1)*XR2(0,2)-XR2(2,2)*XR2(0,1))  +
                     XR2(2,0)*(XR2(0,1)*XR2(1,2)-XR2(0,2)*XR2(1,1)) );
    }
    else
    { // 1D
      jv[2]=g2.dimension(0,axis3);
      jv[2]=g2.dimension(0,axis3);
      cellSize=fabs((vertex(i1+1,i2,i3,axis1)-vertex(i1,i2,i3,axis1))/c.gridSpacing(0));
      cellSize2=fabs((vertex2(j1+1,j2,j3,axis1)-vertex2(j1,j2,j3,axis1))/g2.gridSpacing(0));
    }
    
    qForward=cellSize2;
    qReverse=cellSize;
    
    quality=cellSize/max(cellSize2,REAL_MIN);
  }
  else
  {
    Mapping & map2 = g2.mapping().getMapping();
    realArray & vertex = c.vertex();

    realArray xx(1,3),rr(1,3),rx(1,3,3);
    rr=-1.;
    
    for( axis=0; axis<numberOfDimensions; axis++ )
      xx(0,axis)=vertex(i1,i2,i3,axis);
      
    map2.inverseMap(xx,rr,rx);   // could do better here.

    real q=0., q2;
    qForward=0.; // measure quality of interpolation
    qReverse=0.; // measures quality if we interpolated in the opposite direction.
    for( int j=0; j<numberOfDimensions; j++ )
    {
      if( numberOfDimensions==2 )
      {
	real xra[2][2];
	for(axis=0; axis<numberOfDimensions; axis++ ) 
	{
	  xra[0][axis]=(vertex(i1+1,i2,i3,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(0);
	  xra[1][axis]=(vertex(i1,i2+1,i3,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(1);
	}
	q2=(square((rx(0,0,0)*XR(0,j)+rx(0,0,1)*XR(1,j))/dr2(0)) +
	    square((rx(0,1,0)*XR(0,j)+rx(0,1,1)*XR(1,j))/dr2(1)) );
      }
      else
      {
	real xra[3][3];
	for(axis=0; axis<numberOfDimensions; axis++ ) 
	{
	  xra[0][axis]=(vertex(i1+1,i2,i3,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(0);
	  xra[1][axis]=(vertex(i1,i2+1,i3,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(1);
	  xra[2][axis]=(vertex(i1,i2,i3+1,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(2);
	}
	q2=(square((rx(0,0,0)*XR(0,j)+rx(0,0,1)*XR(1,j)+rx(0,0,2)*XR(2,j))/dr2(0)) +
	    square((rx(0,1,0)*XR(0,j)+rx(0,1,1)*XR(1,j)+rx(0,1,2)*XR(2,j))/dr2(1)) +
	    square((rx(0,2,0)*XR(0,j)+rx(0,2,1)*XR(1,j)+rx(0,2,2)*XR(2,j))/dr2(2)));
      }
      q2=SQRT(q2)*c.gridSpacing(j);
      // q2 = ratio of grid spacing on the interpolation grid to the interpolee grid.
      q+=q2+1./max(.001,q2);
      qReverse=max(qReverse,q2);
      qForward=max(qForward,1./q2);
    }
    //real quality=.5*q/numberOfDimensions;   // the smaller the better (1=min)
    quality=qReverse/qForward;         
  }
      
  return quality;
}
#undef XR

int Ogen::
improveQuality( CompositeGrid & cg, const int & grid, RealArray & removedPointBound )
// =============================================================================================================
// /Description:
//    Try to remove interpolation points on this grid so as to improve the quality of the interpolation.
//  This routine must be called after computing all interpolation points.
//
//  The basic algorithm is to start at the highest priority grids (which tend to lose fewer points) and
// see if it makes sense to remove some of the interpolation points (which are not required) in order
// to get better quality interpolation.
// =============================================================================================================
{
  const int numberOfBaseGrids = cg.numberOfBaseGrids();
  const int numberOfDimensions = cg.numberOfDimensions();

  MappedGrid & c = cg[grid];
  intArray & mask = c.mask();
  intArray & inverseGrid = cg.inverseGrid[grid];
//  realArray & rI = cg.inverseCoordinates[grid];

  // displayMaskNeeded( mask, "mask with needed points");

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

  // *wdh* getIndex(extendedGridIndexRange(c),I1,I2,I3);
  getIndex(c.extendedIndexRange(),I1,I2,I3);  // *wdh* 000322 : do not check far side periodic bndries

  int axis;
//   for( axis=0; axis<c.numberOfDimensions(); axis++ )
//   {
//     if( c.boundaryFlag(Start,axis)==MappedGridData::mixedPhysicalInterpolationBoundary )
//       Iv[axis]=Range(c.gridIndexRange(Start,axis)-1,Iv[axis].getBound());
//     if( c.boundaryFlag(End,axis)==MappedGridData::mixedPhysicalInterpolationBoundary )
//       Iv[axis]=Range(Iv[axis].getBase(),c.gridIndexRange(End,axis)+1);
//   }

  int iv[3];
  int & i1 = iv[0];
  int & i2 = iv[1];
  int & i3 = iv[2];
  int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];

  //   int l =0;  // multigrid level
  realArray xx(1,3),rr(1,3),rx(1,3,3);
  
  // ***** first make a list of interpolation points on this grid that are
  //       on the outer boundary of interpolation points and are not needed for discretization 
  int i=0;
  intArray ia(I1.length()*I2.length()*I3.length()+1,3);
  
  int width=1;
  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
  {
    for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
    {
      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
      {
	if( mask(i1,i2,i3) & MappedGrid::ISinterpolationPoint  ) 
	{
	  const int grid2=inverseGrid(i1,i2,i3);
	  assert( grid2>=0 && grid2<numberOfBaseGrids ); //  && grid2!=grid );

	  if( grid2<grid ) 
	  {
	    // int m=mask(i1,i2,i3);
	    if( isOnInterpolationBoundary(c,iv,width) && !isNeededForDiscretization(c,iv)  ) 
	    {

              ia(i,0)=i1;
	      ia(i,1)=i2;
	      ia(i,2)=i3;
	      i++;
	    }
	  }
	}
      }
    }
  }
  int num=i;
  if( num==0 )
    return 0;

  realArray quality(I1,I2,I3);
  quality=0.;
  const real pointWasChecked = REAL_MAX;
  const real pointInList = REAL_MAX*.5;
  real qForward,qReverse;
  
  Range R(0,num-1);
  quality(ia(R,0),ia(R,1),ia(R,2))=pointInList;
  int level=0;  // MG level
  int numberOfPointsRemoved=0;
  for( i=0; i<num; i++ )
  {
    i1=ia(i,0);
    i2=ia(i,1);
    i3=ia(i,2);
    if( quality(i1,i2,i3)!=pointWasChecked && !isNeededForDiscretization(c,iv)  ) 
    {
//       if( grid==2 && i1==15 && i2==14 )
//       {
// 	aString ans;
//         cout << "hello\n";
// 	cin >> ans;
//       }
      
      const int grid2=inverseGrid(i1,i2,i3);
      assert( grid2>=0 && grid2<numberOfBaseGrids && grid2!=grid );

      MappedGrid & g2 = cg[grid2];

      // compute the quaility of the interpolation
      quality(i1,i2,i3) =computeInterpolationQuality(cg,grid,i1,i2,i3,qForward,qReverse);

      if( debug & 4 )
        printf("improveQuality: pt (%i,%i,%i) on grid %i has interp. quality=%6.2e (f=%6.2e,r=%6.2e) from grid %i\n",
              i1,i2,i3,grid,quality(i1,i2,i3),qForward,qReverse,grid2);

      if( quality(i1,i2,i3) > qualityBound )
      {
        // remove this point if one interpolation neighbour has a better quality interpolation.
        real q=quality(i1,i2,i3);
        const int j1Min = i1 > c.extendedIndexRange(0,axis1) ? i1-1 : i1;
        const int j1Max = i1 < c.extendedIndexRange(1,axis1) ? i1+1 : i1;
        const int j2Min = i2 > c.extendedIndexRange(0,axis2) ? i2-1 : i2;
        const int j2Max = i2 < c.extendedIndexRange(1,axis2) ? i2+1 : i2;
        const int j3Min = i3 > c.extendedIndexRange(0,axis3) ? i3-1 : i3;
        const int j3Max = i3 < c.extendedIndexRange(1,axis3) ? i3+1 : i3;
        bool notDone=TRUE;
        for( j3=j3Min; j3<=j3Max && notDone; j3++ )
        for( j2=j2Min; j2<=j2Max && notDone; j2++ )
        for( j1=j1Min; j1<=j1Max; j1++ )
	{
          if( mask(j1,j2,j3)<0 )
	  {
	    if( quality(j1,j2,j3)==0. )
	    {
	      // compute the quality for this point
	      quality(j1,j2,j3)=computeInterpolationQuality(cg,grid,j1,j2,j3,qForward,qReverse);
	    }
	    // printf("              : neighbour: quality(%i,%i,%i)=%6.2e)\n",j1,j2,j3,quality(j1,j2,j3));

	    if( quality(j1,j2,j3) < q )
	    {
              if( debug & 4 )
  	        printf("         ***  : remove point (%i,%i,%i) on grid %i, quality=%6.2e (quality(%i,%i,%i)=%6.2e)\n",
                  i1,i2,i3,grid,quality(i1,i2,i3),j1,j2,j3,quality(j1,j2,j3));
	      mask(i1,i2,i3)=0;
              numberOfPointsRemoved++;
	      quality(i1,i2,i3)=pointWasChecked;
	      notDone=FALSE;
              // Keep track of the region in the unit square that encloses all removed points. This is used later.
              real r;
              for( axis=0; axis<numberOfDimensions; axis++ )
	      {
                // over-estimate the discretization cell width by a factor of two. May be needed for boundaries.
                const real width = cg.interpolationWidth(axis,grid,grid2,level)*c.gridSpacing(axis);
                r = (iv[axis]-c.indexRange(Start,axis))*c.gridSpacing(axis);
                removedPointBound(Start,axis,grid)=min(removedPointBound(Start,axis,grid),r-width);
                removedPointBound(End  ,axis,grid)=max(removedPointBound(End  ,axis,grid),r+width);
	      }
	      break;
	    }
	  }
	}
        if( !notDone )
	{
          // if the point was removed, add any unchecked neighbouring points to the list
	  for( int j3=j3Min; j3<=j3Max; j3++ )
	    for( int j2=j2Min; j2<=j2Max; j2++ )
	      for( int j1=j1Min; j1<=j1Max; j1++ )
	      {
		if( mask(j1,j2,j3)<0 && quality(j1,j2,j3)!=pointWasChecked && quality(j1,j2,j3)!=pointInList )
		{
		  const int grid3=inverseGrid(j1,j2,j3);
		  assert( grid3>=0 && grid3<numberOfBaseGrids && grid3!=grid );
                  if( grid3<grid )
		  {
		    ia(num,0)=j1; 
		    ia(num,1)=j2; 
		    ia(num,2)=j3;
		    num++;
		    quality(j1,j2,j3)=pointInList; // *** fix this ***
		  }
		}
	      }
	}
	
      }  // end if quality < bound
      
      quality(i1,i2,i3)=pointWasChecked;  

/* ----
      real distanceToBoundary=1000.;   // distance in grid lines to the boundary of the interpolee
      for( axis=0; axis<c.numberOfDimensions(); axis++ )
      {
	if( !g2.isPeriodic(axis) )
	{
	  real ri = rI(i1,i2,i3,axis)/g2.gridSpacing(axis)+g2.indexRange(Start,axis);  
	  distanceToBoundary=min(distanceToBoundary,ri-g2.indexRange(Start,axis),g2.indexRange(End,axis)-ri);
	}
      }

      if( distanceToBoundary<2. )
      {
        printf("improveQuality: remove point (%i,%i,%i) on grid %i, dist=%6.2e \n",i1,i2,i3,grid,distanceToBoundary);
        mask(i1,i2,i3)=0;
        inverseGrid(i1,i2,i3)=-inverseGrid(i1,i2,i3);
      }
---- */
      
    }
  }
//   if(numberOfPointsRemoved>0 )
//   {
//     c.mask().periodicUpdate(); // ***** 000322
//   }
  
  if( info & 4 )
    printF("Grid %s: Number of points removed to improve quality = %i\n",
       (const char*)c.mapping().getName(Mapping::mappingName),numberOfPointsRemoved);
  
  return 0;
}
