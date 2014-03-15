#include "Overture.h"
#include "display.h"
#include "InterpolatePoints.h"
#include "display.h"
#include "MappingProjectionParameters.h"
#include "ParallelUtility.h"

int InterpolatePoints::debug=0;



// The macro MODR shifts a point back into the main periodic region
#define NRM(axis)  ( indexRange(End,axis)-indexRange(Start,axis)+1 )
#define MODR(i,axis)  ( \
  ( (i-indexRange(Start,axis)+NRM(axis)) % NRM(axis)) \
      +indexRange(Start,axis) \
                           )

static int localDebug=0;   // 1+2+4+8;

InterpolatePoints::
InterpolatePoints()
{
  indirection=NULL;
  interpolationLocation=NULL;
  interpolationLocationPlus=NULL;
  interpolationCoordinates=NULL;

  interpolationOffset=2.5;  // offset in grid lines from the unit cube where we are allowed to interpolate

  infoLevel=1;

  numberOfValidGhostPoints=defaultNumberOfValidGhostPoints;
  
}

InterpolatePoints::
~InterpolatePoints()
{
  delete [] indirection;
  delete [] interpolationLocation;
  delete [] interpolationLocationPlus;
  delete [] interpolationCoordinates;

}


// ====================================================================================
/// \brief: Set the number of valid ghost points 
// ====================================================================================
int InterpolatePoints::
setNumberOfValidGhostPoints( int numValidGhost /* =defaultNumberOfValidGhostPoints */ )
{
  numberOfValidGhostPoints=numValidGhost;
  return 0;
}



int InterpolatePoints::
setInfoLevel( int info )
// ==================================================================================
// /Description:
//    Set the flag for specfying what information messages should be printed.
// info=0 mean no info. info=1, 1+2, 1+2+4, ... gives succesively more info.
// ==================================================================================
{
  infoLevel=info;
  return 0;
}



int InterpolatePoints::
setInterpolationOffset( real widthInGridLines )
// ==================================================================================
// /Description:
//    Set the offset in grid lines from the unit cube where we are allowed to interpolate
// ==================================================================================
{
  interpolationOffset=widthInGridLines;
  return 0;
}


const IntegerArray & InterpolatePoints::
getStatus() const
// ==================================================================================
// /Description:
//    Return the status array for the last interpolation. The values in status are from the
// InterpolationStatusEnum.
// ==================================================================================
{
  return status;
}


int InterpolatePoints::
getInterpolationInfo(CompositeGrid & cg, IntegerArray & indexValues,IntegerArray & interpoleeGrid) const
// ==================================================================================
// /Description:
//   Return the index values and interpoleeGrid for the last interpolation.
// ==================================================================================
{
  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  const int rangeDimension = numberOfDimensions;
  const int domainDimension = cg[0].domainDimension();

  int totalNumberOfInterpolationPoints=sum(numberOfInterpolationPoints);
  
  if( totalNumberOfInterpolationPoints==0 )
  {
    indexValues.redim(0);
    interpoleeGrid.redim(0);
    return 1;
  }
  
  assert( indirection!=NULL );

  indexValues.redim(totalNumberOfInterpolationPoints,3);
  indexValues=0;
  interpoleeGrid.redim(totalNumberOfInterpolationPoints);
  interpoleeGrid=0;
  
  // We must check the highest priority grid first since this was the order the points were generated.
  // A point may be extrpolated on a higher priority grid but then interpolated on a lower priority grid.
  int grid;
  for( grid=numberOfComponentGrids-1; grid>=0; grid-- )  // check highest priority grid first
  {
    const int num=numberOfInterpolationPoints(grid);
    if( num>0 )
    {
      IntegerArray & ia = indirection[grid];
      IntegerArray & ip = interpolationLocation[grid];
      const int *iap = ia.Array_Descriptor.Array_View_Pointer0;
#define IA(i0) iap[i0]
      const int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
      const int ipDim0=ip.getRawDataSize(0);
#define IP(i0,i1) ipp[i0+ipDim0*(i1)]

      for( int i=0; i<num; i++ )
      {
	for( int axis=0; axis<domainDimension; axis++ )
	  indexValues(IA(i),axis)=IP(i,axis);
	interpoleeGrid(IA(i))=grid;
      }
      
    }
  }
  
  return 0;

}

#undef IP
#undef IA


int InterpolatePoints::
buildInterpolationInfo(const RealArray & positionToInterpolate, 
                       CompositeGrid & cg,
                       RealArray *projectedPoints /* =NULL */,
                       IntegerArray *checkTheseGrids /* =NULL */ )
// =================================================================================================
// /Description:
//    Build the interpolation location arrays that can be used to interpolate a grid function
//  at some specified points. For surface grids, optionally return the points projected onto one
//  of the surface grids. 
//
// /positionToInterpolate (input) : positionToInterpolate(i,0:domainDimension-1) position of point i
// /cg (input) : Composite grid. 
// /projectedPoints (output) : If projectedPoints!=NULL AND the CompositeGrid consists of surface grids,
//     then this array will hold the values of the point in positionToInterpolate that have been
//     projected onto the surface (underlying the grid that the point was interpolated from).   
// /checkTheseGrids (input): if not NULL then this array indicates which grids to check for an
//   interpolation point, i.e. (*checkTheseGrids)(grid)!=0 means this grid can be used for interpolation.
//
// /Return value: if negative then the absolute value of the return value is the number of points 
//   not assigned.
//
// /Method: 
// Check grids starting from the highest priority grid. 
// Keep looking until we find a grid that we can interpolate from properly.
// If, before finding a grid we can interpolate from,  we find we can extrapolate, then save the
// extrapolation info. Thus, when finished, either we can interpolate, extrapolate or 
// we could not assign a point at all.
// 
// /Notes: wdh: 070228 -- support for surface grids in 3D
// =================================================================================================
{
  if( cg.numberOfComponentGrids()==0 )
  {
    return 0;
  }

  // debug=3; // *********************
  

  // cg.update(MappedGrid::THEboundingBox);
  // *wdh* 070618 -- we need to make sure the bounding box is created properly ---
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    if( cg[grid].isRectangular() )
      cg[grid].update( MappedGrid::THEboundingBox );
    else
      cg[grid].update( MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEboundingBox );
  }

  const int numberOfDimensions = cg.numberOfDimensions();
  const int rangeDimension = numberOfDimensions;
  const int domainDimension = cg[0].domainDimension();

  const bool surfaceGrid=domainDimension!=rangeDimension;
  const bool projectSurfaceGridPoints = surfaceGrid && projectedPoints!=NULL;
  

  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  Range Axes=domainDimension;

  int grid, axis;
  for( grid=0; grid<numberOfComponentGrids; grid++ )
  {
    if( !cg[grid].isAllVertexCentered() && !cg[grid].isAllCellCentered() )
    {
      cout << "interpolatePoints:ERROR: grids must be either vertex or cell centered, no mongrels! \n";
      return 1;
    }      
  }

  int numberOfPointsToInterpolate=positionToInterpolate.getLength(0);

  const real epsi=1.e-3;
  int extrap,pointWasExtrapolated;
  int returnValue=0;  // 0=ok, >0 error, <0 some points extrapolated


  delete [] indirection;
  delete [] interpolationLocation;
  delete [] interpolationLocationPlus;
  delete [] interpolationCoordinates;

  indirection = new IntegerArray [numberOfComponentGrids];
  interpolationLocation = new IntegerArray[numberOfComponentGrids];
  interpolationLocationPlus = new IntegerArray[numberOfComponentGrids];
  interpolationCoordinates = new RealArray[numberOfComponentGrids];

  numberOfInterpolationPoints.redim(numberOfComponentGrids);
  numberOfInterpolationPoints=0;
  int *numberOfInterpolationPointsp = numberOfInterpolationPoints.Array_Descriptor.Array_View_Pointer0;
#define NUMBEROFINTERPOLATIONPOINTS(i0) numberOfInterpolationPointsp[i0]

  // *****************************************************
  // ******** Find an interpolation stencil to use *******
  // *****************************************************

  Range R=numberOfPointsToInterpolate;
  

  IntegerArray ia0(R);
  status.redim(R);
  int *statusp = status.Array_Descriptor.Array_View_Pointer0;
#define STATUS(i0) statusp[i0]
  int *ia0p = ia0.Array_Descriptor.Array_View_Pointer0;
#define IA0(i0) ia0p[i0]


  status=notInterpolated;

  const RealArray & x =positionToInterpolate;
  real bb[2][3];

  // For projecting points onto surfaces:
  MappingProjectionParameters mpParams;
  const realArray & rProject = surfaceGrid ? mpParams.getRealArray(MappingProjectionParameters::r) : 
                   Overture::nullRealDistributedArray();

  const RealArray & xProject = projectSurfaceGridPoints ? *projectedPoints : x;
  if( projectSurfaceGridPoints )
  {
    if( xProject.dimension(0)!=x.dimension(0) )
    {
      ((realArray &)xProject).redim(x.dimension(0),Range(rangeDimension));
    }
  }

  real *xProjectp = xProject.Array_Descriptor.Array_View_Pointer1;
  const int xProjectDim0=xProject.getRawDataSize(0);
#define XPROJECT(i0,i1) xProjectp[i0+xProjectDim0*(i1)]
 
  // 
  // Check grids starting from the highest priority grid. 
  // Keep looking until we find a grid that we can interpolate from properly.
  // If, before finding a grid we can interpolate from,  we find we can extrapolate, then save the
  // extrapolation info. Thus, when finished, either we can interpolate, extrapolate or 
  // we could not assign a point at all.
  // 
  for( grid=numberOfComponentGrids-1; grid>=0; grid-- )  // check highest priority grid first
  { 
    if( checkTheseGrids!=NULL && (*checkTheseGrids)(grid)==0 ) continue; // skip this grid
    
    MappedGrid & mg = cg[grid];
    if( mg.getGridType()==MappedGrid::unstructuredGrid )
    {
      // for now we skip unstructured grids
      printF("InterpolatePoints::buildInterpolationInfo: skipping grid=%i since it is an unstructured grid.\n",grid);
    }
    

    Mapping & mapping = mg.mapping().getMapping();

    const intArray & mask = mg.mask();
    
    const RealArray & gridSpacing = mg.gridSpacing();
    const IntegerArray & indexRange = mg.indexRange();
    const IntegerArray & dimension  = mg.dimension();
    const IntegerArray & isPeriodic = mg.isPeriodic();
    const real shift = (bool)mg.isAllVertexCentered() ? 0. : .5; // shift position for cell centered grids

    const int *dimensionp = dimension.Array_Descriptor.Array_View_Pointer1;
    const int dimensionDim0=dimension.getRawDataSize(0);
#define DIMENSION(i0,i1) dimensionp[i0+dimensionDim0*(i1)]
    const int *indexRangep = indexRange.Array_Descriptor.Array_View_Pointer1;
    const int indexRangeDim0=indexRange.getRawDataSize(0);
#define INDEXRANGE(i0,i1) indexRangep[i0+indexRangeDim0*(i1)]
    const real *gridSpacingp = gridSpacing.Array_Descriptor.Array_View_Pointer0;
#define GRIDSPACING(i0) gridSpacingp[i0]


    // get the bounding box for this grid --- *** increase bounding box a bit ??

    const RealArray & boundingBox = mg.boundingBox();
    // boundingBox.display("Here is the boundingBox");

    // increase the size of the bounding box to allow for interp. from ghost point
    real scale=0.;
    for( axis=0; axis<numberOfDimensions; axis++ )
      scale=max(scale,boundingBox(1,axis)-boundingBox(0,axis));
 
    const IntegerArray & egir = extendedGridIndexRange(mg);
    // rbb(side,axis) : bounding box for valid points on the unit square, including ghost lines ob bc=0 boundaries
    real prbb[6];
    #define rbb(side,axis) (prbb[(side)+2*(axis)])  

    const real delta=scale*.25;  
    for( axis=0; axis<rangeDimension; axis++ )
    {
      bb[0][axis]=boundingBox(0,axis)-delta;
      bb[1][axis]=boundingBox(1,axis)+delta;

      for( int side=0; side<=1; side++ )
      {
	rbb(side,axis)= side + (egir(side,axis)-mg.gridIndexRange(side,axis))*mg.gridSpacing(axis) - (1-2*side)*REAL_EPSILON*100.;
	// rbb(side,axis)= side + (egir(side,axis)-mg.gridIndexRange(side,axis))*mg.gridSpacing(axis);
      }
    }
    
    // printF(" ********  grid=%i r-bounding box=[%9.3e,%9.3e][%9.3e,%9.3e][%9.3e,%9.3e]\n",
    // 	   grid,rbb(0,0),rbb(1,0),rbb(0,1),rbb(1,1),rbb(0,2),rbb(1,2));

    const real *xp = x.Array_Descriptor.Array_View_Pointer1;
    const int xDim0=x.getRawDataSize(0);
#define X(i0,i1) xp[i0+xDim0*(i1)]

    // make a list of points inside the bounding box
    int j=0;
    if( rangeDimension==2 )
    {
      for( int i=0; i<numberOfPointsToInterpolate; i++ )
      {
	if( STATUS(i)!=interpolated )  // check notInterpolated and extrapolated
	{
	  real x0=X(i,0);
	  real y0=X(i,1);
	  if( x0>=bb[0][0] && x0<=bb[1][0] &&
	      y0>=bb[0][1] && y0<=bb[1][1] ) 
	  {
	    IA0(j)=i;
	    j++;
	  }
	}
      }
    }
    else
    {
      for( int i=0; i<numberOfPointsToInterpolate; i++ )
      {
	if( STATUS(i)!=interpolated )  // check notInterpolated and extrapolated
	{
	  real x0=X(i,0);
	  real y0=X(i,1);
	  real z0=X(i,2);
	  if( x0>=bb[0][0] && x0<=bb[1][0] &&
	      y0>=bb[0][1] && y0<=bb[1][1] &&
	      z0>=bb[0][2] && z0<=bb[1][2] ) 
	  {
	    IA0(j)=i;
	    j++;
	  }
	}
      }
    }
    
    
    int numberToCheck=j;
    Range I=numberToCheck;
    RealArray ra,xa;
    IntegerArray & ia = indirection[grid];
    if( numberToCheck>0 )
    {
      ra.redim(I,domainDimension); 
      xa.redim(I,rangeDimension);
      ia.redim(I);
    }
    
    real *rap = ra.Array_Descriptor.Array_View_Pointer1;
    const int raDim0=ra.getRawDataSize(0);
#define RA(i0,i1) rap[i0+raDim0*(i1)]

    real *xap = xa.Array_Descriptor.Array_View_Pointer1;
    const int xaDim0=xa.getRawDataSize(0);
#define XA(i0,i1) xap[i0+xaDim0*(i1)]


    int *iap = ia.Array_Descriptor.Array_View_Pointer0;
#define IA(i0) iap[i0]


    if( numberToCheck>0 )
    {
      // attempt to interpolate points from this grid.


      ia(I)=ia0(I);
      if( rangeDimension==2 )
      {
	for( int i=0; i<numberToCheck; i++ )
	{
	  XA(i,0)=X(IA(i),0);
	  XA(i,1)=X(IA(i),1);
	}
      }
      else
      {
	for( int i=0; i<numberToCheck; i++ )
	{
	  XA(i,0)=X(IA(i),0);
	  XA(i,1)=X(IA(i),1);
	  XA(i,2)=X(IA(i),2);
	}
      }
      
      ra=-1;
    }
    
    if( !surfaceGrid )
    {
      // mapping.useRobustInverse(true); // *****************
      
#ifdef USE_PPP
      mapping.inverseMapS(xa,ra);
#else
      mapping.inverseMap(xa,ra);
#endif
    }
    else
    {
      // mapping.inverseMap(xa,ra);  // fix this -- get ra from mp
      ((RealArray&)rProject).redim(I,domainDimension);

#ifdef USE_PPP
      mapping.projectS(xa,mpParams);
#else
      mapping.project(xa,mpParams);
#endif

      real *rProjectp = rProject.Array_Descriptor.Array_View_Pointer1;
      const int rProjectDim0=rProject.getRawDataSize(0);
#define RPROJECT(i0,i1) rProjectp[i0+rProjectDim0*(i1)]

      for( int i=0; i<numberToCheck; i++ )
      {
	RA(i,0)=RPROJECT(i,0);
	RA(i,1)=RPROJECT(i,1); 
      }
    }
      
    if( debug & 8 )
    {
      ::display(ra,sPrintF("InterpolatePoints:: ra after inversion (grid=%i)",grid),"%6.3f ");
      if( surfaceGrid )
	::display(xa,sPrintF("InterpolatePoints:: xa after projection (grid=%i)",grid),"%6.3f ");
    }
      

    // ********************************************************************
    // **** compress possible points based on unit square coordinates *****
    // ********************************************************************
      
    
      
    
    real offset0, offset1, offset2;
    
    // allowExtrapolation : If we turn this on we also need to make sure a point outside is not marked at interpolated,
    //                      cf. sib grids 
    bool allowExtrapolation=false;  
    if( allowExtrapolation )
    {
      offset0=offset1=offset2=9.;
    }
    else
    {
      offset0=.5+gridSpacing(0)*interpolationOffset;  
      offset1=.5+gridSpacing(1)*interpolationOffset;  
      offset2=.5+gridSpacing(2)*interpolationOffset;  
    }
    

    // double check the inverse for the NurbsMapping -- this seems broken for twoPipes!, *wdh* 080326
    // printF(" grid=%i mapping.getClassName()=%s\n",grid,(const char*)mapping.getClassName());
    
//     if( true && numberToCheck>0 && domainDimension==3 && mapping.getClassName()=="NurbsMapping" )
//     {
//       RealArray xb(I,rangeDimension);  // for double checking the inverse map
//       mapping.mapS(ra,xb);
//       Range Rx=3;
//       const real xScale = fabs(bb[1][0]-bb[0][0]) + fabs(bb[1][1]-bb[0][1]) + fabs(bb[1][2]-bb[0][2]);
//       const real xEps = xScale*1.e-3;
//       for( int i=0; i<numberToCheck; i++ )
//       {
// 	if( fabs(RA(i,0)-.5)<offset0 && fabs(RA(i,1)-.5)<offset1 && fabs(RA(i,2)-.5)<offset2 )
// 	{
// 	  real diff = max(fabs(xb(i,Rx)-xa(i,Rx)));
// 	  if( diff>xEps )
// 	  {
// 	    printF(" InterpolatePoints:INFO: error inverting grid=%i, i=%i IA=%i x=(%9.3e,%9.3e,%9.3e) r=(%9.3e,%9.3e,%9.3e)"
//                    " map(r)=(%9.3e,%9.3e,%9.3e) err(x)=%9.3e, will ignore... \n",grid,i,IA(i),
// 		   XA(i,0),XA(i,1),XA(i,2),RA(i,0),RA(i,1),RA(i,2),xb(i,0),xb(i,1),xb(i,2),diff);

//             RA(i,0)=Mapping::bogus;
//             RA(i,1)=Mapping::bogus;
//             RA(i,2)=Mapping::bogus;

	    
// 	  }
	  
// 	}
//       }
	      
//     }



    j=0;
    int i;
    if( domainDimension==2 )
    {
      for( i=0; i<numberToCheck; i++ )
      {
	if( fabs(RA(i,0)-.5)<offset0 && fabs(RA(i,1)-.5)<offset1 )
	{
	  if( i!=j )
	  {
	    IA(j)=IA(i);
	    RA(j,0)=RA(i,0);
	    RA(j,1)=RA(i,1);
	    if( projectSurfaceGridPoints )
	    { // we need to save the projected xa points too in this case
	      XA(j,0)=XA(i,0);
	      XA(j,1)=XA(i,1);
	      XA(j,2)=XA(i,2);
	    }
	      
	  }
	  j++;
	}
      }
    }
    else
    {
      for( i=0; i<numberToCheck; i++ )
      {
	if( fabs(RA(i,0)-.5)<offset0 && fabs(RA(i,1)-.5)<offset1 && fabs(RA(i,2)-.5)<offset2 )
	{
          // printF(" InterpolatePoints: grid=%i i=%i IA=%i x=(%9.3e,%9.3e,%9.3e) r=(%9.3e,%9.3e,%9.3e)\n",grid,i,IA(i),
          //         XA(i,0),XA(i,1),XA(i,2),RA(i,0),RA(i,1),RA(i,2));
	  
	  if( i!=j )
	  {
	    IA(j)=IA(i);
	    RA(j,0)=RA(i,0);
	    RA(j,1)=RA(i,1);
	    RA(j,2)=RA(i,2);
	  }
	  j++;
	}
      }
    }
      
    numberToCheck=j;

//     if(numberToCheck==0 )
//       continue;
      
    if( numberToCheck> 0 )  // -----------------
    {
      I=numberToCheck;
      
      IntegerArray & ip  = interpolationLocation[grid];
      IntegerArray & ip1 = interpolationLocationPlus[grid];

      ip.redim(I,numberOfDimensions); ip1.redim(I,numberOfDimensions);
      int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
      const int ipDim0=ip.getRawDataSize(0);
#define IP(i0,i1) ipp[i0+ipDim0*(i1)]
      int *ip1p = ip1.Array_Descriptor.Array_View_Pointer1;
      const int ip1Dim0=ip1.getRawDataSize(0);
#define IP1(i0,i1) ip1p[i0+ip1Dim0*(i1)]

      RealArray & dra = interpolationCoordinates[grid];
      dra.redim(I,numberOfDimensions);
      real *drap = dra.Array_Descriptor.Array_View_Pointer1;
      const int draDim0=dra.getRawDataSize(0);
#define DRA(i0,i1) drap[i0+draDim0*(i1)]

      RealArray dr(I,numberOfDimensions);
      real *drp = dr.Array_Descriptor.Array_View_Pointer1;
      const int drDim0=dr.getRawDataSize(0);
#define DR(i0,i1) drp[i0+drDim0*(i1)]

      for( axis=0; axis<domainDimension; axis++ )
      { 
	for( i=0; i<numberToCheck; i++ )
	{
          real rr = RA(i,axis)/GRIDSPACING(axis)+INDEXRANGE(0,axis);
	  IP(i,axis)=rr>=0. ? int(rr+.5) : int(rr-.5); // closest point to r
//  	  IP(i,axis)=rr>=0. ? int(rr) : int(rr-1.); // closest point <= to r

	  IP(i,axis)=min(DIMENSION(End,axis)-1,max(DIMENSION(Start,axis)+1,IP(i,axis)));  // may no longer be < r
	  DR(i,axis)=rr-IP(i,axis)-shift;
	}
      }

      // dra(I,Axes)=min(fabs(dr(I,Axes)),1.);
      dra(I,Axes)=fabs(dr(I,Axes));
      // dra(I,Axes)=dr(I,Axes);

      //...........only use 4 points if dra bigger than epsilon, otherwise just use 2 points (ip1==ip),
      //    this lets us  interpolate near interpolation boundaries
      for( axis=0; axis<domainDimension; axis++ )
      {
	for( i=0; i<numberToCheck; i++ )
	    IP1(i,axis)=IP(i,axis)+( DR(i,axis)>0. ? 1 : -1);

	if( isPeriodic(axis) )    // ........periodic wrap
	  for( i=0; i<numberToCheck; i++ )
	    IP1(i,axis)=MODR(IP1(i,axis),axis);
      }

      // define the valid subset of points for interpolation   *wdh* 021015
      // We allow interpolation from ghost points on physical-boundaries too; the number of ghost points
      // allowed depends on the discretization width
      const int *gridIndexRangep = &mg.gridIndexRange(0,0);
#define gridIndexRange(side,axis) gridIndexRangep[(side)+2*(axis)]
      int iRangep[6];
#define iRange(side,axis) iRangep[(side)+2*(axis)]

      int extra=0;
      if( numberOfValidGhostPoints!=defaultNumberOfValidGhostPoints )
        extra=numberOfValidGhostPoints;
      else
        extra=mg.discretizationWidth(0)/2;
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	// iRange(0,axis)=max(dimension(0,axis),gridIndexRange(0,axis)-extra);
	// iRange(1,axis)=min(dimension(1,axis),gridIndexRange(1,axis)+extra);
	for( int side=0; side<=1; side++ )
	{
	  if( mg.boundaryCondition(side,axis)==0 ) // **************************** turn off for testing 
	  { // include ghost points on interpolation boundaries *wdh* 080327 
	    iRange(side,axis)=egir(side,axis);
	  }
	  else
	  {
            if( side==0 )
	      iRange(0,axis)=max(dimension(0,axis),gridIndexRange(0,axis)-extra);
            else
	      iRange(1,axis)=min(dimension(1,axis),gridIndexRange(1,axis)+extra);
	  }
	}
	
      }

      const real rBound=.5+ REAL_EPSILON*100.; // for checking inside the unit square -- not really true for interp boundaries 

      // compress interpolation info into arrays ia,ip,ip1,
      j=0;
      if( domainDimension==2 )
      {
	const int * maskp = mask.Array_Descriptor.Array_View_Pointer1;
	const int maskDim0=mask.getRawDataSize(0);
#define MASK(i0,i1) maskp[i0+maskDim0*(i1)]

	for( i=0; i<numberToCheck; i++ )
	{

          const int iai=ia(i);

	  if( debug & 3 )
	    printf(" Point i=%i ia=%i ip=(%i,%i) x=(%9.3e,%9.3e) r=(%9.3e,%9.3e) dr=(%9.3e,%9.3e) \n",
		   i,IA(i),IP(i,0),IP(i,1),x(iai,0),x(iai,1),RA(i,0),RA(i,1),DR(i,0),DR(i,1));


          // Are we inside the range of valid grid points: (iRange includes some number of ghost points)
	  bool inside = (min(IP(i,0),IP1(i,0))>=iRange(0,0) && max(IP(i,0),IP1(i,0))<=iRange(1,0) &&
			 min(IP(i,1),IP1(i,1))>=iRange(0,1) && max(IP(i,1),IP1(i,1))<=iRange(1,1) );


          bool canInterpolate = ( MASK(IP(i,0),IP (i,1))!=0 && MASK(IP1(i,0),IP (i,1))!=0 &&
				  MASK(IP(i,0),IP1(i,1))!=0 && MASK(IP1(i,0),IP1(i,1))!=0 );
	  
          bool keepThisPoint=false;
          if( inside && canInterpolate )
	  {
            // If we are not inside the unit square, then consider this to be extrapolation which will be replaced
            // by a better point if one is found. 
  	    // bool insideUnitSquare = fabs(RA(i,0)-.5)<rBound && fabs(RA(i,1)-.5)<rBound;
  	    bool insideUnitSquare = (RA(i,0)>=rbb(0,0) && RA(i,0)<=rbb(1,0) && 
				     RA(i,1)>=rbb(0,1) && RA(i,1)<=rbb(1,1) );

	    STATUS(IA(i)) = insideUnitSquare ? interpolated : extrapolated;
            keepThisPoint=true;
	  }
	  else if( !inside  && STATUS(IA(i)) != extrapolated ) // use previously extrapolated value is possible
	  {
	    // If we are not inside the valid range of grid points, shift the interpolation box (IP,IP1)
            // to the nearest valid set of point 
	    if( debug & 3 )
	      printf("**Outside: Point i=%i ip=(%i,%i) x=(%9.3e,%9.3e) r=(%9.3e,%9.3e) dr=(%9.3e,%9.3e)\n",
		     i,IP(i,0),IP(i,1),x(iai,0),x(iai,1),RA(i,0),RA(i,1),DR(i,0),DR(i,1));


	    for( axis=0; axis<numberOfDimensions; axis++ )
	    {
              int ip0=IP(i,axis);
	      if( IP(i,axis)<IP1(i,axis) )
	      {
		IP(i,axis) = max(iRange(0,axis),min(iRange(1,axis)-1,IP(i,axis)));
		IP1(i,axis)=IP(i,axis)+1;
	      }
	      else
	      {
		IP(i,axis) = max(iRange(0,axis)+1,min(iRange(1,axis),IP(i,axis)));
		IP1(i,axis)=IP(i,axis)-1;
	      }
              // DRA = absolute value of the distance of rr to IP 
	      DRA(i,axis)+=abs(IP(i,axis)-ip0); // shift the interpolation weight --> now we extrapolate 

	    }
	    canInterpolate = ( MASK(IP(i,0),IP (i,1))!=0 && MASK(IP1(i,0),IP (i,1))!=0 &&
			       MASK(IP(i,0),IP1(i,1))!=0 && MASK(IP1(i,0),IP1(i,1))!=0 );


	    if( canInterpolate )
	    {
	      STATUS(IA(i)) = extrapolated;
	      keepThisPoint=true;
	    }
	    
	    if( debug & 3 )
	      printf("** --> Point i=%i ip=(%i,%i) x=(%9.3e,%9.3e) r=(%9.3e,%9.3e) dr=(%9.3e,%9.3e) "
                     " canInterpolate=%i status=%i?\n",
		     i,IP(i,0),IP(i,1),x(iai,0),x(iai,1),RA(i,0),RA(i,1),DR(i,0),DR(i,1),canInterpolate,STATUS(IA(i)));
	  }
	  if( keepThisPoint )
	  {
	    if( i!=j )
	    {
	      IP(j,0)= IP(i,0);  IP(j,1)= IP(i,1); 
	      IP1(j,0)=IP1(i,0); IP1(j,1)=IP1(i,1); 
	      DRA(j,0)=DRA(i,0); DRA(j,1)=DRA(i,1); 

	      IA(j)=IA(i);
	    }
	    if( projectSurfaceGridPoints )  
	    {
	      // save the projected values 
	      int ii=IA(j);
	      XPROJECT(ii,0)=XA(i,0);
	      XPROJECT(ii,1)=XA(i,1);
	      XPROJECT(ii,2)=XA(i,2);
	    }
	    if( debug & 3 ) printf(" --> Keep this point Point j=%i (IA(j)=%i) status=%i\n",j,IA(j),STATUS(IA(j)));

	    j++;
	  }
	  
	} // end for ( i 
      }
      else // 3D
      {
	const int * maskp = mask.Array_Descriptor.Array_View_Pointer2;
	const int maskDim0=mask.getRawDataSize(0);
	const int maskDim1=mask.getRawDataSize(1);
#undef MASK
#define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]

	for( i=0; i<numberToCheck; i++ )
	{

          const int iai=ia(i);

	  if( debug & 3 )
	    printf(" Point i=%i ia=%i ip=(%i,%i,%i) ip1=(%i,%i,%i) x=(%9.3e,%9.3e,%9.3e) grid=%i r=(%9.3e,%9.3e,%9.3e) dr=(%9.3e,%9.3e,%9.3e) \n",
		   i,IA(i),IP(i,0),IP(i,1),IP(i,2),IP1(i,0),IP1(i,1),IP1(i,2),x(iai,0),x(iai,1),x(iai,2),grid,RA(i,0),RA(i,1),RA(i,2),DR(i,0),DR(i,1),DR(i,2));



          // Are we inside the range of valid grid points: (iRange includes some number of ghost points)
	  bool inside = (min(IP(i,0),IP1(i,0))>=iRange(0,0) && max(IP(i,0),IP1(i,0))<=iRange(1,0) &&
			 min(IP(i,1),IP1(i,1))>=iRange(0,1) && max(IP(i,1),IP1(i,1))<=iRange(1,1) &&
			 min(IP(i,2),IP1(i,2))>=iRange(0,2) && max(IP(i,2),IP1(i,2))<=iRange(1,2) );
	  
          bool canInterpolate = ( MASK(IP(i,0),IP (i,1),IP (i,2))!=0 && MASK(IP1(i,0),IP (i,1),IP (i,2))!=0 &&
				  MASK(IP(i,0),IP1(i,1),IP (i,2))!=0 && MASK(IP1(i,0),IP1(i,1),IP (i,2))!=0 &&
				  MASK(IP(i,0),IP (i,1),IP1(i,2))!=0 && MASK(IP1(i,0),IP (i,1),IP1(i,2))!=0 &&
				  MASK(IP(i,0),IP1(i,1),IP1(i,2))!=0 && MASK(IP1(i,0),IP1(i,1),IP1(i,2))!=0 );
	  
          bool keepThisPoint=false;
          if( inside && canInterpolate )
	  {
            // If we are not inside the unit square, then consider this to be extrapolation which will be replaced
            // by a better point if one is found. 
  	    // bool insideUnitSquare = fabs(RA(i,0)-.5)<rBound && fabs(RA(i,1)-.5)<rBound && fabs(RA(i,2)-.5)<rBound;
  	    bool insideUnitSquare = (RA(i,0)>=rbb(0,0) && RA(i,0)<=rbb(1,0) && 
				     RA(i,1)>=rbb(0,1) && RA(i,1)<=rbb(1,1) && 
				     RA(i,2)>=rbb(0,2) && RA(i,2)<=rbb(1,2) );
	    

	    STATUS(IA(i)) = insideUnitSquare ? interpolated : extrapolated;
	    if( debug & 3 )
	      printF(" --> This point is inside and can be %s from grid=%i\n",(insideUnitSquare ? "interpolated" : "extrapolated"),
                        grid);
	    
            keepThisPoint=true;
	  }
	  else if( !inside  && STATUS(IA(i)) != extrapolated ) // use previously extrapolated value is possible
	  {
	    // If we are not inside the valid range of grid points, shift the interpolation box (IP,IP1)
            // to the nearest valid set of point 

	    for( axis=0; axis<numberOfDimensions; axis++ )
	    {
              int ip0=IP(i,axis);
	      if( IP(i,axis)<IP1(i,axis) )
	      {
		IP(i,axis) = max(iRange(0,axis),min(iRange(1,axis)-1,IP(i,axis)));
		IP1(i,axis)=IP(i,axis)+1;
	      }
	      else
	      {
		IP(i,axis) = max(iRange(0,axis)+1,min(iRange(1,axis),IP(i,axis)));
		IP1(i,axis)=IP(i,axis)-1;
	      }
              // DRA = absolute value of the distance of rr to IP 
	      DRA(i,axis)+=abs(IP(i,axis)-ip0); // shift the interpolation weight --> now we extrapolate 

	    }
	    canInterpolate = ( MASK(IP(i,0),IP (i,1),IP (i,2))!=0 && MASK(IP1(i,0),IP (i,1),IP (i,2))!=0 &&
			       MASK(IP(i,0),IP1(i,1),IP (i,2))!=0 && MASK(IP1(i,0),IP1(i,1),IP (i,2))!=0 &&
			       MASK(IP(i,0),IP (i,1),IP1(i,2))!=0 && MASK(IP1(i,0),IP (i,1),IP1(i,2))!=0 &&
			       MASK(IP(i,0),IP1(i,1),IP1(i,2))!=0 && MASK(IP1(i,0),IP1(i,1),IP1(i,2))!=0 );

	    if( canInterpolate )
	    {
	      STATUS(IA(i)) = extrapolated;
	      keepThisPoint=true;

	      if( debug & 3 ) printF(" --> This point is outside and can be extrapolated from grid=%i\n",grid);

	    }
	  }
	  if( keepThisPoint )
	  {
	    if( i!=j )
	    {
	       IP(j,0)= IP(i,0);  IP(j,1)= IP(i,1);  IP(j,2)= IP(i,2);   
	      IP1(j,0)=IP1(i,0); IP1(j,1)=IP1(i,1); IP1(j,2)=IP1(i,2); 
	      DRA(j,0)=DRA(i,0); DRA(j,1)=DRA(i,1); DRA(j,2)=DRA(i,2);  

	      IA(j)=IA(i);
	    }
	    j++;
	  }

/* -------------------


	  if( MASK(IP(i,0),IP (i,1),IP (i,2))!=0 && MASK(IP1(i,0),IP (i,1),IP (i,2))!=0 &&
	      MASK(IP(i,0),IP1(i,1),IP (i,2))!=0 && MASK(IP1(i,0),IP1(i,1),IP (i,2))!=0 &&
              MASK(IP(i,0),IP (i,1),IP1(i,2))!=0 && MASK(IP1(i,0),IP (i,1),IP1(i,2))!=0 &&
	      MASK(IP(i,0),IP1(i,1),IP1(i,2))!=0 && MASK(IP1(i,0),IP1(i,1),IP1(i,2))!=0 )
	  {
	    if( i!=j )
	    {
	       IP(j,0)= IP(i,0);  IP(j,1)= IP(i,1);  IP(j,2)= IP(i,2);   
	      IP1(j,0)=IP1(i,0); IP1(j,1)=IP1(i,1); IP1(j,2)=IP1(i,2); 
	      DRA(j,0)=DRA(i,0); DRA(j,1)=DRA(i,1); DRA(j,2)=DRA(i,2);  

	      IA(j)=IA(i);
	    }

	    if(min(IP(j,0),IP1(j,0))>=iRange(0,0) && max(IP(j,0),IP1(j,0))<=iRange(1,0) &&
	       min(IP(j,1),IP1(j,1))>=iRange(0,1) && max(IP(j,1),IP1(j,1))<=iRange(1,1) &&
	       min(IP(j,2),IP1(j,2))>=iRange(0,2) && max(IP(j,2),IP1(j,2))<=iRange(1,2) )
	    {
	      STATUS(IA(j)) = interpolated;
	      j++;
	    }
	    else 
	    {
              if( STATUS(IA(j)) != extrapolated )  // if already extrapolated, use that value.
	      {
		STATUS(IA(j)) = extrapolated;
		j++;
	      }
	    }
	  }
	  --------------------- */


	}
	
      }  // end else 3d
      NUMBEROFINTERPOLATIONPOINTS(grid)=j;
      
    } // end if numberToCheck>0 
    
  }
  

  // ************ TO-DO *******************
  
  // If we are requested to assign a value to ALL points then 
  //   Make a list of un-assigned points
  //   
  //   Find the nearest valid grid point --> then use zeroeth order extrapolation 


  int numberInterpolated=0;
  int numberExtrapolated=0;
  int j=0;
  for( int i=0; i<numberOfPointsToInterpolate; i++ )
  {
    // printf(" DONE: i=%i status=%i\n",i,STATUS(i));

    if( STATUS(i)==interpolated )
    {
      numberInterpolated++;
    }
    else
    {
      if( STATUS(i)==extrapolated ) 
	numberExtrapolated++;
      else
      {
	if( infoLevel & 2 )
	  printF("buildInterpolationInfo: WARNING: point not assigned: i=%i xv=(%9.3e,%9.3e,%9.3e)\n",
		 i,x(i,0),x(i,1),(numberOfDimensions==2 ? 0 : x(i,2)));
	
      }
      
	
      // IA0(j)=i;
      // j++;
    }
  }
  int numNotAssigned=numberOfPointsToInterpolate-numberInterpolated-numberExtrapolated;
    
  if( infoLevel & 1 )
    printF("InterpolatePoints::buildInterpolationInfo: total interpolated=%i extrapolated=%i not assigned=%i\n",
	   numberInterpolated,numberExtrapolated,numNotAssigned);
  if( numNotAssigned>0 && infoLevel & 1 )  
    printF("InterpolatePoints::buildInterpolationInfo: WARNING: %i points not assigned!\n",numNotAssigned);
  
  returnValue=-numNotAssigned;
  return returnValue;
}

#undef DIMENSION
#undef INDEXRANGE
#undef GRIDSPACING
#undef gridIndexRange
#undef iRange
#undef X
#undef XA
#undef STATUS
#undef IA0
#undef IA
#undef RA
#undef IP
#undef IP1
#undef DR
#undef DRA


//\begin{>interpolatePointsInclude.tex}{}
int InterpolatePoints::
interpolatePoints(const realCompositeGridFunction & u,
		  RealArray & uInterpolated, 
		  const Range & R0/* =nullRange */,           
		  const Range & R1/* =nullRange */,
		  const Range & R2/* =nullRange */,
		  const Range & R3/* =nullRange */,
		  const Range & R4/* =nullRange */ )
//=======================================================================================================
//  /Description:
//    Given some points in space, determine the values of a grid function u. If interpolation
//    is not possible then extrapolate from the nearest grid point. The extrapolation is zero-order
//    so that the value is just set equal to the value from the boundary.
//  /u (input): interpolate values from this grid function
//  /uInterpolated (output): uInterpolated(0:numberOfPointsToInterpolate-1,R0,R1,R2,R3,R4) : interpolated
//      values
//  /R0,R1,...,R4 (input): interpolate these components of the grid function. R0 is the range of values for
//     the first component of u, R1 the values for the second component, etc. By default all components
//      of u are interpolated.
// ==========================================================================================================
{
  // ***************************************************
  // ****** Interpolate points given the stencil *******
  // ***************************************************
  CompositeGrid & cg = *u.getCompositeGrid();

  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();

  const int rangeDimension = numberOfDimensions;
  const int domainDimension = cg[0].domainDimension();

  // determine component ranges to use:
  Range Ra[5] = {R0,R1,R2,R3,R4};  
  int i;
  for( i=0; i<5; i++ )
  {
    if( Ra[i].length()<=0 ) //     if( Ra[i]==nullRange )
    {
      // Ra[i] = Range(u.getComponentBase(i),u.getComponentBound(i));  
      // *wdh* 050515 -- take bounds from uInterpolated
      Ra[i] = Range(uInterpolated.getBase(i+1),uInterpolated.getBound(i+1));  
    }
    if( Ra[i].getBase()<u.getComponentBase(i) || Ra[i].getBound()>u.getComponentBound(i) )
    {
      cout << "interpolatePoints:ERROR: the component Range R" << i << " is out of range! \n";
      printf("R%i =(%i,%i) but the dimensions for component %i of u are (%i,%i) \n",i,
	     Ra[i].getBase(),Ra[i].getBound(),i,u.getComponentBase(i),u.getComponentBound(i));
      Overture::abort("error");
    }
    else if( i<3 && (Ra[i].getBase()<uInterpolated.getBase(i+1) || Ra[i].getBound()>uInterpolated.getBound(i+1)) )
    {
      cout << "interpolatePoints:ERROR: the component Range R" << i << " is out of range! \n";
      printf("R%i =(%i,%i) but the dimensions for index %i of uInterpolated are (%i,%i) \n",i,
	     Ra[i].getBase(),Ra[i].getBound(),i+1,uInterpolated.getBase(i+1),uInterpolated.getBound(i+1));
      Overture::abort("error");
    }
  }


  // We must check the highest priority grid first since this was the order the points were generated.
  // A point may be extrpolated on a higher priodirt grid but then interpolated on a lower priority grid.
  int grid;
  for( grid=numberOfComponentGrids-1; grid>=0; grid-- )  // check highest priority grid first
  {
    // interpolate from this grid.

    const int num=numberOfInterpolationPoints(grid);
    if( num>0 )
    {
      // printf("----interpolatePointsNew: interp %i points from grid %i\n",num,grid);

      IntegerArray & ia = indirection[grid];
      IntegerArray & ip = interpolationLocation[grid];
      IntegerArray & ip1= interpolationLocationPlus[grid];
      RealArray & dra = interpolationCoordinates[grid];
      const IntegerArray & gid = cg[grid].gridIndexRange();
      
      // display(ia,"ia");
      // display(ip,"ip");
      // display(dra,"dra");
      
      #ifdef USE_PPP
        realSerialArray ug; getLocalArrayWithGhostBoundaries(u[grid],ug);
      #else
        const realSerialArray & ug = u[grid];
      #endif

      const int *iap = ia.Array_Descriptor.Array_View_Pointer0;
#define IA(i0) iap[i0]

      const int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
      const int ipDim0=ip.getRawDataSize(0);
#define IP(i0,i1) ipp[i0+ipDim0*(i1)]
      const int *ip1p = ip1.Array_Descriptor.Array_View_Pointer1;
      const int ip1Dim0=ip1.getRawDataSize(0);
#define IP1(i0,i1) ip1p[i0+ip1Dim0*(i1)]

      const real *drap = dra.Array_Descriptor.Array_View_Pointer1;
      const int draDim0=dra.getRawDataSize(0);
#define DRA(i0,i1) drap[i0+draDim0*(i1)]

      real *uInterpolatedp = uInterpolated.Array_Descriptor.Array_View_Pointer1;
      const int uInterpolatedDim0=uInterpolated.getRawDataSize(0);
#define UINTERPOLATED(i0,i1) uInterpolatedp[i0+uInterpolatedDim0*(i1)]


      // ...........Bi-Linear Interpolation:
      if( domainDimension==2 )
      {
	const real *ugp = ug.Array_Descriptor.Array_View_Pointer2;
	const int ugDim0=ug.getRawDataSize(0);
	const int ugDim1=ug.getRawDataSize(1);
#define UG(i0,i1,i2) ugp[i0+ugDim0*(i1+ugDim1*(i2))]

	for( int c0=Ra[0].getBase(); c0<=Ra[0].getBound(); c0++)  // *** add more components ****
	{
	  for( int i=0; i<num; i++ )
	  {
	    UINTERPOLATED(IA(i),c0)= 
	      (1.-DRA(i,1))*(
		(1.-DRA(i,0))*UG(IP (i,0),IP(i,1),c0)
		   +DRA(i,0) *UG(IP1(i,0),IP(i,1),c0))
	      + DRA(i,1) *(
		(1.-DRA(i,0))*UG( IP(i,0),IP1(i,1),c0)
		   +DRA(i,0) *UG(IP1(i,0),IP1(i,1),c0));

//  	    if( c0==3 )
//  	    {
//  	      printf(" grid=%i i=%i ia=%i dra=(%7.1e,%7.1e) ip=(%i,%i) ip1=(%i,%i) gid=%i,%i "
//                       "u=(%9.3e,%9.3e,%9.3e,%9.3e) uI=%9.3e\n",
//  		     grid,i,ia(i),dra(i,0),dra(i,1),
//  		     ip(i,0),ip(i,1),ip1(i,0),ip1(i,1),
//                       gid(0,0),gid(1,0),
//                       UG(IP (i,0),IP(i,1),c0),UG(IP1(i,0),IP(i,1),c0),
//                       UG( IP(i,0),IP1(i,1),c0),UG(IP1(i,0),IP1(i,1),c0),
//                       uInterpolated(ia(i),c0));
//  	    }

	  }
	}
      }
      else // 3D
      {
	const real *ugp = ug.Array_Descriptor.Array_View_Pointer3;
	const int ugDim0=ug.getRawDataSize(0);
	const int ugDim1=ug.getRawDataSize(1);
	const int ugDim2=ug.getRawDataSize(2);
#undef UG
#define UG(i0,i1,i2,i3) ugp[i0+ugDim0*(i1+ugDim1*(i2+ugDim2*(i3)))]

	for( int c0=Ra[0].getBase(); c0<=Ra[0].getBound(); c0++)  // *** add more components ****
	{
	  for( int i=0; i<num; i++ )
	  {

	    UINTERPOLATED(IA(i),c0)= 
              (1.-DRA(i,2))*(
  	        (1.-DRA(i,1))*(
		  (1.-DRA(i,0))*UG(IP (i,0),IP(i,1),IP(i,2),c0)
		     +DRA(i,0) *UG(IP1(i,0),IP(i,1),IP(i,2),c0))
	          + DRA(i,1) *(
	  	  (1.-DRA(i,0))*UG( IP(i,0),IP1(i,1),IP(i,2),c0)
		     +DRA(i,0) *UG(IP1(i,0),IP1(i,1),IP(i,2),c0))
		            )
                 + DRA(i,2)*(
  	        (1.-DRA(i,1))*(
	  	  (1.-DRA(i,0))*UG(IP (i,0),IP(i,1),IP1(i,2),c0)
		     +DRA(i,0) *UG(IP1(i,0),IP(i,1),IP1(i,2),c0))
	         + DRA(i,1) *(
	  	  (1.-DRA(i,0))*UG( IP(i,0),IP1(i,1),IP1(i,2),c0)
		     +DRA(i,0) *UG(IP1(i,0),IP1(i,1),IP1(i,2),c0))
		           );

// 	    if( i==26112 ) // IA(i)==30843 )
// 	    {
// 	      printF(" @@interp: i=%i ia=%i ip=(%i,%i,%i) ip1=(%i,%i,%i) dra=(%9.3e,%9.3e,%9.3e)\n",
// 		     i,IA(i),IP (i,0),IP(i,1),IP(i,2),IP1(i,0),IP1(i,1),IP1(i,2),DRA(i,0),DRA(i,1),DRA(i,2));
//               printF(" @@interp:  uInterp=%9.3e  u=[%9.3e,%9.3e][%9.3e,%9.3e][%9.3e,%9.3e][%9.3e,%9.3e]\n",
// 		     UINTERPOLATED(IA(i),c0), UG(IP (i,0),IP(i,1),IP(i,2),c0),UG(IP1(i,0),IP(i,1),IP(i,2),c0),
// 		     UG( IP(i,0),IP1(i,1),IP(i,2),c0),UG(IP1(i,0),IP1(i,1),IP(i,2),c0),
//                      UG(IP (i,0),IP(i,1),IP1(i,2),c0),UG(IP1(i,0),IP(i,1),IP1(i,2),c0),
//                      UG( IP(i,0),IP1(i,1),IP1(i,2),c0),UG(IP1(i,0),IP1(i,1),IP1(i,2),c0));
// 	    }

	  }
	}
      }
    }
  }
  
  return 0;

}

#undef IP
#undef IP1
#undef UG
#undef IA
#undef UINTERPOLATED

//\begin{>interpolatePointsInclude.tex}{}
int InterpolatePoints::
interpolationCoefficients(const CompositeGrid &cg,
			  RealArray & uInterpolationCoeff )

//=======================================================================================================
//  /Description:
//    Return the coefficients for the interpolation of a grid function u at some points in space. (kkc)
//    If interpolation
//    is not possible then extrapolate from the nearest grid point. The extrapolation is zero-order
//    so that the value is just set equal to the value from the boundary.
//  /cg (input): interpolate values from this grid 
//  /uInterpolationCoeff (output): uInterpolationCoeff(0:numberOfPointsToInterpolate-1, 2^numberOfDimensions)
//      interpolation coefficients
//\end{interpolatePointsInclude.tex}  
// ==========================================================================================================
{


  // 030228 kkc, most of this code is from interpolatePoints, the only difference is that the coefficients are stored
  //             instead of the interpolated value

  // note the extra index (the second in the list) to u, this entry holds the index to the coefficent for a 
  //      node in the parametric element.  they are ordered by index in parameter space. Hence,
  //      in a 3D grid, the coefficient at (ir1,ir2,ir3) = ir1 + ( 2*(ir2 + 2*ir3)), so (0,0,1) would be at
  //      index 4.

  //CompositeGrid & cg = *u.getCompositeGrid();

  if( cg.numberOfComponentGrids()==0 )
  {
    return 0;
  }


  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();

  const int rangeDimension = numberOfDimensions;
  const int domainDimension = cg[0].domainDimension();

  // determine component ranges to use:
//   Range Ra[5] = {R0,R1,R2,R3,R4};  
//   int i;
//   for( i=0; i<5; i++ )
//   {
//     if( Ra[i].length()<=0 ) //     if( Ra[i]==nullRange )
//       Ra[i] = Range(u.getComponentBase(i),u.getComponentBound(i));  
//     else if( Ra[i].getBase()<u.getComponentBase(i) || Ra[i].getBound()>u.getComponentBound(i) )
//     {
//       cout << "interpolationCoeffictions:ERROR: the component Range R" << i << " is out of range! \n";
//       printf("R%i =(%i,%i) but the dimensions for component %i of u are (%i,%i) \n",i,
// 	     Ra[i].getBase(),Ra[i].getBound(),i,u.getComponentBase(i),u.getComponentBound(i));
//       Overture::abort("error");
//     }
//     else if( i<3 && (Ra[i].getBase()<uInterpolated.getBase(i+1) || Ra[i].getBound()>uInterpolated.getBound(i+1)) )
//     {
//       cout << "interpolationCoefficients:ERROR: the component Range R" << i << " is out of range! \n";
//       printf("R%i =(%i,%i) but the dimensions for index %i of uInterpolated are (%i,%i) \n",i,
// 	     Ra[i].getBase(),Ra[i].getBound(),i+1,uInterpolated.getBase(i+1),uInterpolated.getBound(i+1));
//       Overture::abort("error");
//     }
//   }


  // We must check the highest priority grid first since this was the order the points were generated.
  // A point may be extrpolated on a higher priodirt grid but then interpolated on a lower priority grid.
  int grid;
  for( grid=numberOfComponentGrids-1; grid>=0; grid-- )  // check highest priority grid first
  {
    // interpolate from this grid.

    const int num=numberOfInterpolationPoints(grid);
    if( num>0 )
    {
      // printf("----interpolatePointsNew: interp %i points from grid %i\n",num,grid);

      IntegerArray & ia = indirection[grid];
      IntegerArray & ip = interpolationLocation[grid];
      IntegerArray & ip1= interpolationLocationPlus[grid];
      RealArray & dra = interpolationCoordinates[grid];
      const IntegerArray & gid = cg[grid].gridIndexRange();
      
      // display(ia,"ia");
      // display(ip,"ip");
      // display(dra,"dra");
      

      //      const realArray & ug = u[grid];


      const int *iap = ia.Array_Descriptor.Array_View_Pointer0;
#define IA(i0) iap[i0]

      const int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
      const int ipDim0=ip.getRawDataSize(0);
#define IP(i0,i1) ipp[i0+ipDim0*(i1)]
      const int *ip1p = ip1.Array_Descriptor.Array_View_Pointer1;
      const int ip1Dim0=ip1.getRawDataSize(0);
#define IP1(i0,i1) ip1p[i0+ip1Dim0*(i1)]

      const real *drap = dra.Array_Descriptor.Array_View_Pointer1;
      const int draDim0=dra.getRawDataSize(0);
#define DRA(i0,i1) drap[i0+draDim0*(i1)]

      real *uInterpolatedp = uInterpolationCoeff.Array_Descriptor.Array_View_Pointer1;
      const int uInterpolatedDim0=uInterpolationCoeff.getRawDataSize(0);
      // kkc#define UINTERPOLATED(i0,i1) uInterpolatedp[i0+uInterpolatedDim0*(i1)]
      int nD = domainDimension==2 ? 4 : 8;
#define UINTERPOLATED(i0,ix,iy,iz) uInterpolatedp[i0+ uInterpolatedDim0*( (ix)+2*((iy) +2*(iz)))]


      // ...........Bi-Linear Interpolation:
      if( domainDimension==2 )
      {
// 	const real *ugp = ug.Array_Descriptor.Array_View_Pointer2;
// 	const int ugDim0=ug.getRawDataSize(0);
// 	const int ugDim1=ug.getRawDataSize(1);
#define UG(i0,i1,i2) ugp[i0+ugDim0*(i1+ugDim1*(i2))]

	//	for( int c0=Ra[0].getBase(); c0<=Ra[0].getBound(); c0++)  // *** add more components ****
	//	{
	int c0=0;
	  for( int i=0; i<num; i++ )
	  {
// 	    UINTERPOLATED(IA(i),c0)= 
// 	      (1.-DRA(i,1))*(
// 		(1.-DRA(i,0))*UG(IP (i,0),IP(i,1),c0)
// 		   +DRA(i,0) *UG(IP1(i,0),IP(i,1),c0))
// 	      + DRA(i,1) *(
// 		(1.-DRA(i,0))*UG( IP(i,0),IP1(i,1),c0)
// 		   +DRA(i,0) *UG(IP1(i,0),IP1(i,1),c0));

	    UINTERPOLATED(IA(i),0,0,0)= 
	      (1.-DRA(i,1))*(1.-DRA(i,0));

	    UINTERPOLATED(IA(i),0,1,0)= 
	      DRA(i,1) *(1.-DRA(i,0));

	    UINTERPOLATED(IA(i),1,0,0)= 
	      (1.-DRA(i,1))*DRA(i,0);

	    UINTERPOLATED(IA(i),1,1,0)= 
	      DRA(i,1) * DRA(i,0);

//  	    if( c0==3 )
//  	    {
//  	      printf(" grid=%i i=%i ia=%i dra=(%7.1e,%7.1e) ip=(%i,%i) ip1=(%i,%i) gid=%i,%i "
//                       "u=(%9.3e,%9.3e,%9.3e,%9.3e) uI=%9.3e\n",
//  		     grid,i,ia(i),dra(i,0),dra(i,1),
//  		     ip(i,0),ip(i,1),ip1(i,0),ip1(i,1),
//                       gid(0,0),gid(1,0),
//                       UG(IP (i,0),IP(i,1),c0),UG(IP1(i,0),IP(i,1),c0),
//                       UG( IP(i,0),IP1(i,1),c0),UG(IP1(i,0),IP1(i,1),c0),
//                       uInterpolated(ia(i),c0));
//  	    }

	  }
	  //	}
      }
      else // 3D
      {
// 	const real *ugp = ug.Array_Descriptor.Array_View_Pointer3;
// 	const int ugDim0=ug.getRawDataSize(0);
// 	const int ugDim1=ug.getRawDataSize(1);
// 	const int ugDim2=ug.getRawDataSize(2);
#undef UG
#define UG(i0,i1,i2,i3) ugp[i0+ugDim0*(i1+ugDim1*(i2+ugDim2*(i3)))]

	//	for( int c0=Ra[0].getBase(); c0<=Ra[0].getBound(); c0++)  // *** add more components ****
	//	{
	int c0=0;
	  for( int i=0; i<num; i++ )
	  {
// 	    UINTERPOLATED(IA(i),c0)= 
//               (1.-DRA(i,2))*(
//   	      (1.-DRA(i,1))*(
// 		(1.-DRA(i,0))*UG(IP (i,0),IP(i,1),IP(i,2),c0)
// 		   +DRA(i,0) *UG(IP1(i,0),IP(i,1),IP(i,2),c0))
// 	      + DRA(i,1) *(
// 		(1.-DRA(i,0))*UG( IP(i,0),IP1(i,1),IP(i,2),c0)
// 		   +DRA(i,0) *UG(IP1(i,0),IP1(i,1),IP(i,2),c0))
//                            )
//               + DRA(i,2)*(
//   	      (1.-DRA(i,1))*(
// 		(1.-DRA(i,0))*UG(IP (i,0),IP(i,1),IP1(i,2),c0)
// 		   +DRA(i,0) *UG(IP1(i,0),IP(i,1),IP1(i,2),c0))
// 	      + DRA(i,1) *(
// 		(1.-DRA(i,0))*UG( IP(i,0),IP1(i,1),IP1(i,2),c0)
// 		   +DRA(i,0) *UG(IP1(i,0),IP1(i,1),IP1(i,2),c0))
// 		         );

	    UINTERPOLATED(IA(i),0,0,0)= (1.-DRA(i,2))*(1.-DRA(i,1))*(1.-DRA(i,0));
	    UINTERPOLATED(IA(i),0,1,0)= (1.-DRA(i,2))*DRA(i,1) *(1.-DRA(i,0));
	    UINTERPOLATED(IA(i),1,0,0)= (1.-DRA(i,2))*(1.-DRA(i,1))*DRA(i,0);
	    UINTERPOLATED(IA(i),1,1,0)= (1.-DRA(i,2))*DRA(i,1)*DRA(i,0);

	    UINTERPOLATED(IA(i),0,0,1)= DRA(i,2)*(1.-DRA(i,1))*(1.-DRA(i,0));
	    UINTERPOLATED(IA(i),0,1,1)= DRA(i,2)*DRA(i,1) *(1.-DRA(i,0));
	    UINTERPOLATED(IA(i),1,0,1)= DRA(i,2)*(1.-DRA(i,1))*DRA(i,0);
	    UINTERPOLATED(IA(i),1,1,1)= DRA(i,2)*DRA(i,1)*DRA(i,0);

	  }
	  //}
      }
    }
  }
  
  return 0;

}

#undef IP
#undef IP1
#undef UG
#undef IA
#undef UINTERPOLATED


//\begin{>interpolatePointsInclude.tex}{}
int InterpolatePoints::
interpolatePoints(const RealArray & positionToInterpolate,
		     const realCompositeGridFunction & u,
		     RealArray & uInterpolated, 
		     const Range & R0/* =nullRange */,           
		     const Range & R1/* =nullRange */,
		     const Range & R2/* =nullRange */,
		     const Range & R3/* =nullRange */,
		     const Range & R4/* =nullRange */ )
//=======================================================================================================
//  /Description:
//    Given some points in space, determine the values of a grid function u. If interpolation
//    is not possible then extrapolate from the nearest grid point. The extrapolation is zero-order
//    so that the value is just set equal to the value from the boundary.
//  /positionToInterpolate (input):
//     positionToInterpolate(0:numberOfPointsToInterpolate-1,0:numberOfDimensions-1) : (x,y[,z]) positions
//          to interpolate. The first dimension of this array determines how many points to interpolate.
//  /u (input): interpolate values from this grid function
//  /uInterpolated (output): uInterpolated(0:numberOfPointsToInterpolate-1,R0,R1,R2,R3,R4) : interpolated
//      values
//  /R0,R1,...,R4 (input): interpolate these components of the grid function. R0 is the range of values for
//     the first component of u, R1 the values for the second component, etc. By default all components
//      of u are interpolated.
//  /indexGuess (input/ouput): indexGuess(0:numberOfPointsToInterpolate-1,0:numberOfDimensions-1) : 
//    (i1,i2[,i3]) values for initial 
//        guess for searches. Not required by default.
//  /interpoleeGrid(.) (input/output): interpoleeGrid(0:numberOfPointsToInterpolate-1) : try
//        this grid first. Not required by default. 
//  /wasInterpolated(.) (output) : If provided as an argument, on output wasInterpolated(i)=TRUE if the point
//     was successfully interpolated, or wasInterpolated(i)=FALSE if the point was extrapolated.
//  /Errors:  This routine in principle should always be able to interpolate or extrapolate.
//  /Return Values:
//    \begin{itemize}
//      \item 0 = success
//      \item 1 = error, unable to interpolate (this should never happen)
//      \item -N = could not interpolate N points, but could extrapolate -- extrapolation was performed
//         from the nearest grid point.
//    \end{itemize}
//  /Author: WDH
//\end{interpolatePointsInclude.tex}  
// =======================================================================================================
{

  int returnValue=0;
  CompositeGrid & cg = *u.getCompositeGrid();
  
  returnValue=buildInterpolationInfo(positionToInterpolate,cg );

  interpolatePoints(u,uInterpolated,R0,R1,R2,R3,R4);
  
  return returnValue;
  
}

#undef NRM
#undef MODR

//\begin{>interpolateAllPointsInclude.tex}{}
int InterpolatePoints::
interpolateAllPoints(const realCompositeGridFunction & uFrom,
		     realCompositeGridFunction & uTo, 
		     const Range & componentsFrom /* =nullRange */, 
		     const Range & componentsTo /* =nullRange */,
                     const int numberOfGhostPointsToInterpolate /* =interpolateAllGhostPoints */ )
//==============================================================================
//
// /Description:
//     Interpolate all values on one CompositeGridFunction, {\ff uTo},  
//   from the values of another CompositeGridFunction,
//   {\ff uFrom}. Values on {\ff uTo} are extrapolated if they lie outside the region covered by {\ff uFrom}.
//   This routine calls the {\ff interpolatePoints} function.
// /uFrom (input):
//      Use these values to interpolate from.
// /uTo (output):
//      Fill in all values on this grid (including ghost-points).
// /componentsFrom (input) : interpolate these components from uFrom (by default interpolate all components)
// /componentsTo   (input) : interpolate these components to uTo
// /numberOfGhostPointsToInterpolate (input) : only interpolate this many ghost points (by default interpolate all)
// /Errors:  This routine in principle should always be able to interpolate or extrapolate all
//   values.
// /Return Values:
//     \begin{itemize}
//       \item 0 = success
//       \item 1 = error, unable to interpolate 
//       \item -N = could not interpolate N points, but could extrapolate -- extrapolation was performed
//          from the nearest grid point.
//     \end{itemize}
//
// /Author: WDH
//
//\end{interpolateAllPointsInclude.tex}
//==============================================================================
{
  CompositeGrid & cgTo= (CompositeGrid&) *uTo.gridCollection;
  int numberOfExtrapolatedPoints=0;
  for( int grid=0; grid<cgTo.numberOfComponentGrids(); grid++)
  {
    numberOfExtrapolatedPoints+=interpolateAllPoints(uFrom,uTo[grid],componentsFrom,componentsTo,
                                                     numberOfGhostPointsToInterpolate);
  }
  return numberOfExtrapolatedPoints;

/* ---
  
  Range C0 = Range(uTo.getComponentBase(0),uTo.getComponentBound(0));


  // Index I1,I2,I3;
  int numberOfExtrapolatedPoints=0;
  int grid;
  for( grid=0; grid<cgTo.numberOfComponentGrids(); grid++)
  {
    // make a list of points to interpolate. No need to interpolate points with mask==0
    const intArray & mask = cgTo[grid].mask();
    const realArray & center = cgTo[grid].center();
    
    intArray ia;
    ia = (mask!=0).indexMap();
    
    if( ia.getLength(0)>0 )
    {
      Range I=ia.getLength(0);
      realArray x(I,cgTo.numberOfDimensions()), uInterpolated(I,C0);
    
      const int i3=center.getBase(2);
      if( cgTo.numberOfDimensions()==2 )
      {
	for( int axis=0; axis<2; axis++ )
	  x(I,axis)=center(ia(I,0),ia(I,1),i3,axis);
      }
      else
      {
	for( int axis=0; axis<3; axis++ )
	  x(I,axis)=center(ia(I,0),ia(I,1),ia(I,2),axis);
      }
    
      int num=interpolatePoints(x,uFrom,uInterpolated);

      realArray & u = uTo[grid];
      if( cgTo.numberOfDimensions()==2 )
      {
	for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // *** could avoid this copy if right shape
	  u(ia(I,0),ia(I,1),i3,c0)=uInterpolated(I,c0);
      }
      else
      {
	for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // *** could avoid this copy if right shape
	  u(ia(I,0),ia(I,1),ia(I,2),c0)=uInterpolated(I,c0);
      }

      // printf("interpolatePoints: number of extrapolated points on grid %i = %i\n",grid,num);
      numberOfExtrapolatedPoints-=num;

    }
    
  }
  return numberOfExtrapolatedPoints;
#else
  cout << "interpolateAllPoints:Error: not implemented for P++ yet \n";
  Overture::abort("error");
  return 0;
#endif
---- */

}

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


//\begin{>>interpolateAllPointsInclude.tex}{}
int InterpolatePoints::
interpolateAllPoints(const realCompositeGridFunction & uFrom,
                     realMappedGridFunction & uTo, 
		     const Range & componentsFrom /* =nullRange */, 
		     const Range & componentsTo /* =nullRange */,
                     const int numberOfGhostPointsToInterpolate /* =interpolateAllGhostPoints */ )
//==============================================================================
//
// /Description:
//     Interpolate all values on a realMappedGridFunction, {\ff uTo},  
//   from the values of another CompositeGridFunction,
//   {\ff uFrom}. Values on {\ff uTo} are extrapolated if they lie outside the region covered by {\ff uFrom}.
//   This routine calls the {\ff interpolatePoints} function.
// /uFrom (input):
//      Use these values to interpolate from.
// /uTo (output):
//      Fill in all values on this grid (including ghost-points).
// /componentsFrom (input) : interpolate these components from uFrom (by default interpolate all components)
// /componentsTo   (input) : interpolate these components to uTo
// /numberOfGhostPointsToInterpolate (input) : only interpolate this many ghost points (by default interpolate all)
// /Errors:  This routine in principle should always be able to interpolate or extrapolate all
//   values.
// /Return Values:
//     \begin{itemize}
//       \item 0 = success
//       \item 1 = error, unable to interpolate 
//       \item -N = could not interpolate N points, but could extrapolate -- extrapolation was performed
//          from the nearest grid point.
//     \end{itemize}
//
// /Author: WDH
//
//\end{interpolateAllPointsInclude.tex}
//==============================================================================
{
  int numberOfExtrapolatedPoints=0;
  
  MappedGrid & mg= *uTo.getMappedGrid();
  
  // make a list of points to interpolate. No need to interpolate points with mask==0
  mg.update(MappedGrid::THEmask);
  
  #ifdef USE_PPP
    const intSerialArray & mask = mg.mask().getLocalArray();
  #else
    const intSerialArray & mask = mg.mask();
  #endif
    
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  if( numberOfGhostPointsToInterpolate==interpolateAllGhostPoints )
  {
    getIndex(mg.dimension(),I1,I2,I3);
  }
  else
  {
    assert( numberOfGhostPointsToInterpolate>-20 && numberOfGhostPointsToInterpolate<20 );  // sanity check 
    
    getIndex(extendedGridIndexRange(mg),I1,I2,I3,numberOfGhostPointsToInterpolate);
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    { // limit bounds to the dimension array
      int ia=max(Iv[axis].getBase() ,mg.dimension(0,axis));
      int ib=min(Iv[axis].getBound(),mg.dimension(1,axis));
      if( ib<ia ) ib=ia;  // what should we do in this case? Just return ?
      Iv[axis]=Range(ia,ib);
    }
    
  }
  
    
  const int count = I1.getLength()*I2.getLength()*I3.getLength();
  intSerialArray ia(count,3);
  // ia = (mask!=0).indexMap();   // interpolate pts with mask!=0 

  int *iap = ia.Array_Descriptor.Array_View_Pointer1;
  const int iaDim0=ia.getRawDataSize(0);
#define IA(i0,i1) iap[i0+iaDim0*(i1)]
    

  const int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
  const int maskDim0=mask.getRawDataSize(0);
  const int maskDim1=mask.getRawDataSize(1);
#define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]

  // Here is the list of points that we will try to interpolate: 
  int i=0;
  int i1,i2,i3;
  FOR_3D(i1,i2,i3,I1,I2,I3)
  {
    if( MASK(i1,i2,i3)!=0 )
    {
      IA(i,0)=i1; IA(i,1)=i2; IA(i,2)=i3;
      i++;
    }
  }
  const int numToInterpolate=i;


  bool isRectangular=mg.isRectangular();
  if( !isRectangular )
    mg.update(MappedGrid::THEcenter);


  // const int numToInterpolate=ia.getLength(0);
  Range I=numToInterpolate;
  RealArray x, uInterpolated;


  Range C0 = componentsTo  ==nullRange ? Range(uTo.getComponentBase(0),uTo.getComponentBound(0)) : componentsTo;
  Range C1 = componentsFrom==nullRange ? Range(uFrom.getComponentBase(0),uFrom.getComponentBound(0)) : componentsFrom;

  if( C0.getLength()!=C1.getLength() )
  {
    printF("InterpolatePoints::interpolateAllPoints:ERROR: Trying to interpolate %i components from uFrom to %i components in uTo\n"
           "                                               These must be the same number of components!\n",
	   C1.getLength(),C0.getLength());
    Overture::abort("InterpolatePoints::interpolateAllPoints:ERROR");
  }
  

  if( numToInterpolate>0 )
  {
    x.redim(I,mg.numberOfDimensions()), uInterpolated.redim(I,C0);
    
    real *xp = x.Array_Descriptor.Array_View_Pointer1;
    const int xDim0=x.getRawDataSize(0);
#define X(i0,i1) xp[i0+xDim0*(i1)]
    real *uInterpolatedp = uInterpolated.Array_Descriptor.Array_View_Pointer1;
    const int uInterpolatedDim0=uInterpolated.getRawDataSize(0);
#define UINTERPOLATED(i0,i1) uInterpolatedp[i0+uInterpolatedDim0*(i1)]


    if( isRectangular )
    {
      real dx[3],xab[2][3];
      mg.getRectangularGridParameters( dx, xab );

      const int i0a=mg.gridIndexRange(0,0);
      const int i1a=mg.gridIndexRange(0,1);
      const int i2a=mg.gridIndexRange(0,2);

      const real xa=xab[0][0], dx0=dx[0];
      const real ya=xab[0][1], dy0=dx[1];
      const real za=xab[0][2], dz0=dx[2];
	
#define COORD0(i0,i1,i2) (xa+dx0*(i0-i0a))
#define COORD1(i0,i1,i2) (ya+dy0*(i1-i1a))
#define COORD2(i0,i1,i2) (za+dz0*(i2-i2a))

      if( mg.numberOfDimensions()==2 )
      {
        for( int i=0; i<numToInterpolate; i++ )
	{
	  X(i,0)=COORD0(IA(i,0),IA(i,1),0);
	  X(i,1)=COORD1(IA(i,0),IA(i,1),0);
	}
      }
      else
      {
        for( int i=0; i<numToInterpolate; i++ )
	{
	  X(i,0)=COORD0(IA(i,0),IA(i,1),IA(i,2));
	  X(i,1)=COORD1(IA(i,0),IA(i,1),IA(i,2));
	  X(i,2)=COORD2(IA(i,0),IA(i,1),IA(i,2));
	}
      }
    }
    else
    {
      
      #ifdef USE_PPP
        const realSerialArray & center = mg.center().getLocalArray();
      #else
        const realSerialArray & center = mg.center();
      #endif

      const real *centerp = center.Array_Descriptor.Array_View_Pointer3;
      const int centerDim0=center.getRawDataSize(0);
      const int centerDim1=center.getRawDataSize(1);
      const int centerDim2=center.getRawDataSize(2);
#define CENTER(i0,i1,i2,i3) centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(i3)))]
      const int i3=center.getBase(2);
      if( mg.numberOfDimensions()==2 )
      {
        for( int i=0; i<numToInterpolate; i++ )
	{
	  X(i,0)=CENTER(IA(i,0),IA(i,1),i3,0);
	  X(i,1)=CENTER(IA(i,0),IA(i,1),i3,1);
	}
      }
      else
      {
        for( int i=0; i<numToInterpolate; i++ )
	{
	  X(i,0)=CENTER(IA(i,0),IA(i,1),IA(i,2),0);
	  X(i,1)=CENTER(IA(i,0),IA(i,1),IA(i,2),1);
	  X(i,2)=CENTER(IA(i,0),IA(i,1),IA(i,2),2);
	}
      }
    }

//     if( numToInterpolate>26112 )   // ***********************************************
//     {
//       int i=26112;
//       printF(" @@@ Interp: fill point i=%i IA=(%i,%i,%i) x=(%9.3e,%9.3e,%9.3e) \n",i,IA(i,0),IA(i,1),IA(i,2),
// 	     X(i,0),X(i,1),X(i,2));
//     }

  } // end if numToInterpolate>0 
  
    

    // ------ interpolate the points ------
  int num=interpolatePoints(x,uFrom,uInterpolated);

  if( numToInterpolate>0 )
  {
      
    #ifdef USE_PPP
      realSerialArray u; getLocalArrayWithGhostBoundaries(uTo,u);
    #else
      realSerialArray & u = uTo;
    #endif

    real *uInterpolatedp = uInterpolated.Array_Descriptor.Array_View_Pointer1;
    const int uInterpolatedDim0=uInterpolated.getRawDataSize(0);

    real *up = u.Array_Descriptor.Array_View_Pointer3;
    const int uDim0=u.getRawDataSize(0);
    const int uDim1=u.getRawDataSize(1);
    const int uDim2=u.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

    if( mg.numberOfDimensions()==2 )
    {
      const int i3=u.getBase(2);
      for( int c0=C0.getBase(), c1=C1.getBase(); c0<=C0.getBound(); c0++,c1++ )  // *** could avoid this copy if right shape
	for( int i=0; i<numToInterpolate; i++ )
	  U(IA(i,0),IA(i,1),i3,c0)=UINTERPOLATED(i,c1);
    }
    else
    {
      for( int c0=C0.getBase(), c1=C1.getBase(); c0<=C0.getBound(); c0++,c1++ )  // *** could avoid this copy if right shape
	for( int i=0; i<numToInterpolate; i++ )
	{
	  U(IA(i,0),IA(i,1),IA(i,2),c0)=UINTERPOLATED(i,c1);
//           if( IA(i,0)==69 && IA(i,1)==11 && IA(i,2)==5 )   // ********************************
// 	  {
//             printF("@@@ Fill in point (%i,%i,%i) i=%i u=%9.3e\n",IA(i,0),IA(i,1),IA(i,2),i,U(IA(i,0),IA(i,1),IA(i,2),c0));
// 	  }
	  
	}
      
    }

    // printf("interpolatePoints: number of extrapolated points on grid %i = %i\n",grid,num);
    numberOfExtrapolatedPoints-=num;

  } // end if numToInterpolate>0

  #ifdef USE_PPP
   uTo.updateGhostBoundaries();
  #endif
   uTo.periodicUpdate();  // *wdh* 080324 
  return numberOfExtrapolatedPoints;

}

#undef CENTER
#undef IA
#undef X
#undef UINTERPOLATED
#undef COORD0
#undef COORD1
#undef COORD2
#undef U
