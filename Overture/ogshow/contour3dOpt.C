//#define BOUNDS_CHECK

#include "GL_GraphicsInterface.h"
#include "GridCollection.h"
#include "PlotIt.h"
#include "xColours.h"

#include "ContourSurface.h"

#include "UnstructuredMapping.h"
#include "ParallelUtility.h"

// local version so that we can change it: 
static int isHiddenByRefinement=MappedGrid::IShiddenByRefinement;

static int numberOfIsoSurfConfusedWarnings=0;


#define ISOSURF EXTERN_C_NAME(isosurf)
extern "C"
{
  void ISOSURF(const int &nTet, real & scalarFieldAtVertices, const int & leadingDimensionOfDAndPx, 
	       const int & numberOfDataComponents,
	       real & dataAtVertices, const int & cornerIndex1, const int & cornerIndex2, 
	       const int & cornerIndex3,
	       const int & numberOfContourLevels, real & contourLevels, 
	       const int & numberOfVertices,
	       real & vertexList,
	       int &ierr);
}


void 
getNormal(const realArray & x, 
	  const int iv[3],
          const int axis,
	  real normal[3],
          const int & recursion=TRUE );


#define FOR_3(i1,i2,i3,I1,I2,I3) \
  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  \
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  \
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  \

#define ForBoundary(side,axis)   for( axis=0; axis<gc.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


//
// void
// resize( RealArray & x )
// {
//   RealArray y;
//   y=x;
//   x.redim(y.getLength(0),y.getLength(1),y.getLength(2)*2);
//   x(y.dimension(0),y.dimension(1),y.dimension(2))=y;
// }

// ! resize arrays for more polygons and recompute pointers
static void
resizeForMorePolygons(RealArray & cd, real *& pcd, 
       IntegerArray & numberOfVerticesPerSurface, int *& numberOfVerticesPerSurfacep)
{
  int newSize=cd.getLength(2)*2;
  // printf("Increasing array size for polygons to %i (ns=%i)\n",newSize,numberOfVerticesPerSurface.getLength(0));
  cd.resize(cd.getLength(0),cd.getLength(1),newSize);  // double space 
//  pcd = cd.Array_Descriptor.Array_View_Pointer2;

  //               cd.display("cd");
//  IntegerArray temp;
//  temp=numberOfVerticesPerSurface;

  // This next array may have already been increased in size.
  if( newSize>numberOfVerticesPerSurface.getLength(1) )
    numberOfVerticesPerSurface.resize(numberOfVerticesPerSurface.getLength(0),newSize);
//  numberOfVerticesPerSurface(temp.dimension(0),temp.dimension(1))=temp;
  
  // printf(" n=%i, cd(0,0,0)=%8.2e\n",n,cd(0,0,0));
//  numberOfVerticesPerSurfacep = numberOfVerticesPerSurface.Array_Descriptor.Array_View_Pointer1;
}


//! Build the polygons that define a contour plane or isoSurface
/*!
  /param numberOfPolygonsPerSurface (output):
  /param cutData[n] (output): cutData[n](4,4,numberOfPolygons)
  /param isoData[n] (output): isoData[n](4,4,numberOfPolygons)
  /numberOfVerticesPerSurface(n,np) (output);
 */
static void 
computeContourPlanes(const MappedGrid & mg, RealMappedGridFunction & u, 
                     int component,
                     int numberOfRefinementLevels, 
                     int numberOfGhostLinesToPlot,
                     RealArray & contourLevel_, const RealArray & normal_,
                     IntegerArray & numberOfPolygonsPerSurface_,
		     IntegerArray & numberOfVerticesPerSurface_,
                     ContourSurface *cs,
                     RealArray & isoSurfaceValue_,
                     const int totalNumberOfSurfaces,
                     IntegerArray & coordinateSurfaceIndex, 
                     IntegerArray & contourPlaneIndex, 
                     IntegerArray & isoSurfaceIndex,
                     const int grid,
                     const IntegerArray & plotContourOnGridFace,
                     const IntegerArray & coordinatePlane,
                     GraphicsParameters & psp )
{
  #ifdef USE_PPP
    intSerialArray mask; getLocalArrayWithGhostBoundaries( mg.mask(),mask);
    realSerialArray uu; getLocalArrayWithGhostBoundaries(u,uu);
  #else
    const intArray & mask = mg.mask();
    const realSerialArray & uu = u;
  #endif
  const IntegerArray & dimension =mg.dimension();
  
  int numberOfContourPlanes = contourPlaneIndex.getLength(0);
  int totalNumberOfCoordinatePlanes =coordinateSurfaceIndex.getLength(0);  
  int numberOfIsoSurfaces=isoSurfaceIndex.getLength(0); 
  
  // const int totalNumberOfCoordinatePlanes=numberOfCoordinatePlanes+sum(plotContourOnGridFace);
  //  const int totalNumberOfCoordinatePlanes=coordinateSurfaceIndex.getLength();

//  const int totalNumberOfSurfaces=numberOfContourPlanes+numberOfIsoSurfaces+totalNumberOfCoordinatePlanes;

  const int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
  const int maskDim0=mask.getRawDataSize(0);
  const int maskDim1=mask.getRawDataSize(1);
#define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]

  const real *up = uu.Array_Descriptor.Array_View_Pointer3;
  const int uDim0=uu.getRawDataSize(0);
  const int uDim1=uu.getRawDataSize(1);
  const int uDim2=uu.getRawDataSize(2);
#define U(i0,i1,i2,c) up[(i0)*d0+(i1)*d1+(i2)*d2+(c)*dc]
  int d0,d1,d2,dc;
  if( u.positionOfComponent(0)==3 )
  {
    d0=1; d1=uDim0; d2=d1*uDim1; dc=d2*uDim2;
  }
  else if( u.positionOfComponent(0)==0 )
  {
    dc=1; d0=uDim0; d1=d0*uDim1; d2=d1*uDim2; 
  }
  else
  {
    printf("contour:ERROR: not implemented for u.positionOfComponent(0)=%i\n",u.positionOfComponent(0));
    throw "error";
  }
	    
  // * note reference * (in case array is resized)
  int *& numberOfVerticesPerSurfacep = numberOfVerticesPerSurface_.Array_Descriptor.Array_View_Pointer1;
  const int numberOfVerticesPerSurfaceDim0=numberOfVerticesPerSurface_.getRawDataSize(0);
#define numberOfVerticesPerSurface(i0,i1) numberOfVerticesPerSurfacep[i0+numberOfVerticesPerSurfaceDim0*(i1)]

  int *numberOfPolygonsPerSurfacep = numberOfPolygonsPerSurface_.Array_Descriptor.Array_View_Pointer0;
#define numberOfPolygonsPerSurface(i0) numberOfPolygonsPerSurfacep[i0]

  real *contourLevelp = contourLevel_.Array_Descriptor.Array_View_Pointer0;
#define contourLevel(i0) contourLevelp[i0]

  real *isoSurfaceValuep = isoSurfaceValue_.Array_Descriptor.Array_View_Pointer1;
  const int isoSurfaceValueDim0=isoSurfaceValue_.getRawDataSize(0);
#define isoSurfaceValue(i0,i1) isoSurfaceValuep[i0+isoSurfaceValueDim0*(i1)]

  const real *normalp = normal_.Array_Descriptor.Array_View_Pointer1;
  const int normalDim0=normal_.getRawDataSize(0);
#define normal(i0,i1) normalp[i0+normalDim0*(i1)]

  // * const bool isRectangular=mg.isRectangular();
  // -- use the vertex array if we are plotting with an adjustment for the "displacement"
  int adjustGridForDisplacement=0;
  psp.get(GI_ADJUST_GRID_FOR_DISPLACEMENT,adjustGridForDisplacement);
  const bool isRectangular=mg.isRectangular() && !adjustGridForDisplacement && 
                          !(mg->computedGeometry & MappedGrid::THEvertex);

  const bool isStructured = mg.getGridType()==MappedGrid::structuredGrid;

  #ifdef USE_PPP
     // this is either vertex or cell centred:
    const realArray & center = isRectangular ? u : mg.center();
    realSerialArray coord; getLocalArrayWithGhostBoundaries(center,coord);  
  #else
    const RealDistributedArray & coord = isRectangular ? u : mg.center();   // this is either vertex or cell centred
  #endif

  const real *coordp = coord.Array_Descriptor.Array_View_Pointer3;
  const int coordDim0=coord.getRawDataSize(0);
  const int coordDim1=coord.getRawDataSize(1);
  const int coordDim2=coord.getRawDataSize(2);
#define COORD(i0,i1,i2,i3) coordp[i0+coordDim0*(i1+coordDim1*(i2+coordDim2*(i3)))]

  real xa,ya,za,dx0,dy0,dz0;
  int ii0a,ii1a,ii2a;
  if( isRectangular )
  {
    real dx[3],xab[2][3];
    mg.getRectangularGridParameters( dx, xab );

    ii0a=mg.gridIndexRange(0,0);
    ii1a=mg.gridIndexRange(0,1);
    ii2a=mg.gridIndexRange(0,2);

    xa=xab[0][0], dx0=dx[0];
    ya=xab[0][1], dy0=dx[1];
    za=xab[0][2], dz0=dx[2];
    if( !(bool)mg.isAllVertexCentered() )
    {
      xa+=dx0*.5;
      ya+=dy0*.5;
      za+=dz0*.5;
    }
  }
  
#define CENTER0(i0,i1,i2) (xa+dx0*(i0-ii0a))
#define CENTER1(i0,i1,i2) (ya+dy0*(i1-ii1a))
#define CENTER2(i0,i1,i2) (za+dz0*(i2-ii2a))


  Index I1,I2,I3;

  bool gridHasPoints=true;
  if ( isStructured ) 
  {
    getIndex(extendedGridIndexRange(mg),I1,I2,I3,numberOfGhostLinesToPlot); 
      
    if( !(mg.isPeriodic(axis1)==Mapping::functionPeriodic) )
      I1=Range(I1.getBase(),I1.getBound()-1);
    if( !(mg.isPeriodic(axis2)==Mapping::functionPeriodic) )
      I2=Range(I2.getBase(),I2.getBound()-1);
    if( !(mg.isPeriodic(axis3)==Mapping::functionPeriodic) )
      I3=Range(I3.getBase(),I3.getBound()-1);
      
    gridHasPoints = ParallelUtility::getLocalArrayBounds(u,uu,I1,I2,I3);
    if( gridHasPoints )
    {
      I1=Range(max(I1.getBase(),uu.getBase(0)),min(I1.getBound(),uu.getBound(0)-1));
      I2=Range(max(I2.getBase(),uu.getBase(1)),min(I2.getBound(),uu.getBound(1)-1));
      I3=Range(max(I3.getBase(),uu.getBase(2)),min(I3.getBound(),uu.getBound(2)-1));

//       I1=Range(max(I1.getBase(),dimension(Start,0)),min(I1.getBound(),dimension(End,0)-1));
//       I2=Range(max(I2.getBase(),dimension(Start,1)),min(I2.getBound(),dimension(End,1)-1));
//       I3=Range(max(I3.getBase(),dimension(Start,2)),min(I3.getBound(),dimension(End,2)-1));
    }
    
  }
  else
  { // kkc 040315 For now we only handle node centered gridfunctions on unstructured grids
    UnstructuredMapping &umap = ((UnstructuredMapping&)(mg.mapping().getMapping()));
    I1 = Range(0,umap.size(UnstructuredMapping::Region)-1);
    I2 = Range(0,0);
    I3 = Range(0,0);
  }

  
  RealArray d(4,2,2,2),s(2,2,2,numberOfContourPlanes),sIso(2,2,2,numberOfIsoSurfaces);
  RealArray px_(4,4,5,max(numberOfContourPlanes,numberOfIsoSurfaces));
  IntegerArray numberOfVertices_(5,max(numberOfContourPlanes,numberOfIsoSurfaces));
  px_=0.;

  real a[3],b[3];
  
  const int *numberOfVerticesp = numberOfVertices_.Array_Descriptor.Array_View_Pointer1;
  const int numberOfVerticesDim0=numberOfVertices_.getRawDataSize(0);
#define numberOfVertices(i0,i1) numberOfVerticesp[i0+numberOfVerticesDim0*(i1)]


  real *pxp = px_.Array_Descriptor.Array_View_Pointer3;
  const int pxDim0=px_.getRawDataSize(0);
  const int pxDim1=px_.getRawDataSize(1);
  const int pxDim2=px_.getRawDataSize(2);
#define px(i0,i1,i2,i3) pxp[i0+pxDim0*(i1+pxDim1*(i2+pxDim2*(i3)))]

#define CD(i0,i1,i2) pcd[i0+cdDim0*(i1+cdDim1*(i2))]

  real *dp = d.Array_Descriptor.Array_View_Pointer3;
  const int dDim0=d.getRawDataSize(0);
  const int dDim1=d.getRawDataSize(1);
  const int dDim2=d.getRawDataSize(2);
#define D(i0,i1,i2,i3) dp[i0+dDim0*(i1+dDim1*(i2+dDim2*(i3)))]

  real *sp = s.Array_Descriptor.Array_View_Pointer3;
  const int sDim0=s.getRawDataSize(0);
  const int sDim1=s.getRawDataSize(1);
  const int sDim2=s.getRawDataSize(2);
#define S(i0,i1,i2,i3) sp[i0+sDim0*(i1+sDim1*(i2+sDim2*(i3)))]

  real *sIsop = sIso.Array_Descriptor.Array_View_Pointer3;
  const int sIsoDim0=sIso.getRawDataSize(0);
  const int sIsoDim1=sIso.getRawDataSize(1);
  const int sIsoDim2=sIso.getRawDataSize(2);
#define SISO(i0,i1,i2,i3) sIsop[i0+sIsoDim0*(i1+sIsoDim1*(i2+sIsoDim2*(i3)))]


  int n;
  
  const int ndat=4, ndatd=4;
  real normalV[3];
  const real sqrt3i =1./SQRT(3.);
  normalV[0]=sqrt3i; normalV[1]=sqrt3i; normalV[2]=sqrt3i;

  int i,i1,i2,i3;

  int nTet = 5;

  if( (numberOfContourPlanes>0 || numberOfIsoSurfaces>0) && gridHasPoints )
  {
    // Initialize min/max values for contour planes if this is the first grid
    if( grid==0 )
    {
      for( int n=0; n<numberOfContourPlanes; n++)
      {
	int ns=contourPlaneIndex(n); // contourPlaneStart+n; // number of surfaces
	assert( ns>=0 && ns<totalNumberOfSurfaces );
	cs[ns].minValue=     REAL_MAX;
	cs[ns].maxValue= -.1*REAL_MAX;
      }
    }

    FOR_3(i1,i2,i3,I1,I2,I3)
    {
      if ( isStructured ) 
      {
	if(MASK(i1,i2  ,i3  )==0 || MASK(i1+1,i2  ,i3  )==0 ||
	   MASK(i1,i2+1,i3  )==0 || MASK(i1+1,i2+1,i3  )==0 ||
	   MASK(i1,i2  ,i3+1)==0 || MASK(i1+1,i2  ,i3+1)==0 ||
	   MASK(i1,i2+1,i3+1)==0 || MASK(i1+1,i2+1,i3+1)==0)
	  continue;
	if( numberOfRefinementLevels>1 )
	{
	  // on refinement grids do not plot cells with ANY corner hidden by refinement *wdh* 030626
	  if( MASK(i1  ,i2  ,i3  )&isHiddenByRefinement || 
	      MASK(i1+1,i2  ,i3  )&isHiddenByRefinement ||
	      MASK(i1  ,i2+1,i3  )&isHiddenByRefinement || 
	      MASK(i1+1,i2+1,i3  )&isHiddenByRefinement ||
	      MASK(i1  ,i2  ,i3+1)&isHiddenByRefinement || 
	      MASK(i1+1,i2  ,i3+1)&isHiddenByRefinement ||
	      MASK(i1  ,i2+1,i3+1)&isHiddenByRefinement || 
	      MASK(i1+1,i2+1,i3+1)&isHiddenByRefinement )
	    continue;
	}
	  
	// put the 8 corners of a cell into the array d(0:2,0:1,0:1,0:1)
	// s(0:1,0:1,0:1,0:?) holds the values for the different contour planes at each vertex
	// sIso(0:1,0:1,0:1,0:?) holds the values for the different iso surfaces at each vertex
	for( int j3=0; j3<=1; j3++ )
	  for( int j2=0; j2<=1; j2++ )
	    for( int j1=0; j1<=1; j1++ )
	    {
	      real uValue=U(i1+j1,i2+j2,i3+j3,component);
		  
	      if( isRectangular )
	      {
		D(0,j1,j2,j3)=CENTER0(i1+j1,i2+j2,i3+j3);
		D(1,j1,j2,j3)=CENTER1(i1+j1,i2+j2,i3+j3);
		D(2,j1,j2,j3)=CENTER2(i1+j1,i2+j2,i3+j3);
	      }
	      else
	      {
		for( int axis=axis1; axis<=axis3; axis++)
		  D(axis,j1,j2,j3)=COORD(i1+j1,i2+j2,i3+j3,axis);
	      }
		  
	      D(axis3+1,j1,j2,j3)=uValue; // u.sa(i1+j1,i2+j2,i3+j3,component);
	      for( i=0; i<numberOfContourPlanes; i++ )
		S(j1,j2,j3,i)=normal(0,i)*D(0,j1,j2,j3)+normal(1,i)*D(1,j1,j2,j3)+normal(2,i)*D(2,j1,j2,j3);
	      for( i=0; i<numberOfIsoSurfaces; i++ )
		SISO(j1,j2,j3,i)=uValue; // u.sa(i1+j1,i2+j2,i3+j3,component);
	    }
      }
      else // unstructured
      {
	UnstructuredMapping &umap = ((UnstructuredMapping&)(mg.mapping().getMapping()));
	if ( !umap.isGhost(UnstructuredMapping::Region, i1) || numberOfGhostLinesToPlot>0 )
	{
	  bool masked = false;
	  const intArray &regions = umap.getEntities(UnstructuredMapping::Region);
	  
	  // put the 8 corners of a cell into the array d(0:2,0:1,0:1,0:1)
	  // note that for tri-prisms, pyramids and tets the hex will be degenerate

	  // s(0:1,0:1,0:1,0:?) holds the values for the different contour planes at each vertex
	  // sIso(0:1,0:1,0:1,0:?) holds the values for the different iso surfaces at each vertex

	  int jj[8],&j0=jj[0],&j1=jj[1],&j2=jj[2],&j3=jj[3],&j4=jj[4],&j5=jj[5],&j6=jj[6],&j7=jj[7];
	  real uval[8];
	  for ( int j=0; j<8; j++ )
	  {
	    jj[j] = regions(i1,j);
	    if ( jj[j]>-1 )
	      uval[j] = U(jj[j],0,0,component);
	  }

	  if ( j4==-1 ) 
	  {
	    // tet
	    nTet = 1;
	    for( int axis=axis1; axis<=axis3; axis++)
	    {
	      D(axis,0,0,0)=COORD(j0,0,0,axis);
	      D(axis,1,0,0)=COORD(j1,0,0,axis);
	      D(axis,0,1,0)=COORD(j2,0,0,axis);
	      D(axis,1,1,0)=COORD(j2,0,0,axis);

	      D(axis,0,0,1)=COORD(j3,0,0,axis);
	      D(axis,1,0,1)=COORD(j3,0,0,axis);
	      D(axis,0,1,1)=COORD(j3,0,0,axis);
	      D(axis,1,1,1)=COORD(j3,0,0,axis);
	    }
	    D(axis3+1,0,0,0) = uval[0];
	    D(axis3+1,1,0,0) = uval[1];
	    D(axis3+1,0,1,0) = uval[2];
	    D(axis3+1,1,1,0) = uval[2];
	    D(axis3+1,0,0,1) = uval[3];
	    D(axis3+1,1,0,1) = uval[3];
	    D(axis3+1,0,1,1) = uval[3];
	    D(axis3+1,1,1,1) = uval[3];

	  }
	  else if ( j5==-1 )
	  {
	    // pyramid
	    nTet = 2;
	    for( int axis=axis1; axis<=axis3; axis++)
	    {
	      D(axis,0,0,0)=COORD(j0,0,0,axis);
	      D(axis,1,0,0)=COORD(j1,0,0,axis);
	      D(axis,0,1,0)=COORD(j3,0,0,axis);
	      D(axis,1,1,0)=COORD(j2,0,0,axis);

	      D(axis,0,0,1)=COORD(j4,0,0,axis);
	      D(axis,1,0,1)=COORD(j4,0,0,axis);
	      D(axis,0,1,1)=COORD(j4,0,0,axis);
	      D(axis,1,1,1)=COORD(j4,0,0,axis);
	    }	      
	      
	    D(axis3+1,0,0,0) = uval[0];
	    D(axis3+1,1,0,0) = uval[1];
	    D(axis3+1,0,1,0) = uval[3];
	    D(axis3+1,1,1,0) = uval[2];
	    D(axis3+1,0,0,1) = uval[4];
	    D(axis3+1,1,0,1) = uval[4];
	    D(axis3+1,0,1,1) = uval[4];
	    D(axis3+1,1,1,1) = uval[4];
	  }
	  else if ( j6==-1 )
	  {
	    // tri-prism
	    nTet = 3;
	    for( int axis=axis1; axis<=axis3; axis++)
	    {
	      D(axis,0,0,0)=COORD(j0,0,0,axis);
	      D(axis,1,0,0)=COORD(j1,0,0,axis);
	      D(axis,0,1,0)=COORD(j2,0,0,axis);
	      D(axis,1,1,0)=COORD(j2,0,0,axis);

	      D(axis,0,0,1)=COORD(j3,0,0,axis);
	      D(axis,1,0,1)=COORD(j4,0,0,axis);
	      D(axis,0,1,1)=COORD(j5,0,0,axis);
	      D(axis,1,1,1)=COORD(j5,0,0,axis);
	    }	      
	    D(axis3+1,0,0,0) = uval[0];
	    D(axis3+1,1,0,0) = uval[1];
	    D(axis3+1,0,1,0) = uval[2];
	    D(axis3+1,1,1,0) = uval[2];
	    D(axis3+1,0,0,1) = uval[3];
	    D(axis3+1,1,0,1) = uval[4];
	    D(axis3+1,0,1,1) = uval[5];
	    D(axis3+1,1,1,1) = uval[5];
	  }
	  else if ( j7>-1 )
	  { // hex
	    for( int axis=axis1; axis<=axis3; axis++)
	    {
	      D(axis,0,0,0)=COORD(j0,0,0,axis);
	      D(axis,1,0,0)=COORD(j1,0,0,axis);
	      D(axis,0,1,0)=COORD(j3,0,0,axis);
	      D(axis,1,1,0)=COORD(j2,0,0,axis);

	      D(axis,0,0,1)=COORD(j4,0,0,axis);
	      D(axis,1,0,1)=COORD(j5,0,0,axis);
	      D(axis,0,1,1)=COORD(j7,0,0,axis);
	      D(axis,1,1,1)=COORD(j6,0,0,axis);
	    }	      
	    D(axis3+1,0,0,0) = uval[0];
	    D(axis3+1,1,0,0) = uval[1];
	    D(axis3+1,0,1,0) = uval[3];
	    D(axis3+1,1,1,0) = uval[2];
	    D(axis3+1,0,0,1) = uval[4];
	    D(axis3+1,1,0,1) = uval[5];
	    D(axis3+1,0,1,1) = uval[7];
	    D(axis3+1,1,1,1) = uval[6];
	  }
	  else
	    abort(); // we can't handle any other shape right now

	  for( int j3=0; j3<=1; j3++ )
	    for( int j2=0; j2<=1; j2++ )
	      for( int j1=0; j1<=1; j1++ )
	      {
		real uValue = D(axis3+1,j1,j2,j3);
		for( i=0; i<numberOfContourPlanes; i++ )
		  S(j1,j2,j3,i)=normal(0,i)*D(0,j1,j2,j3)+normal(1,i)*D(1,j1,j2,j3)+normal(2,i)*D(2,j1,j2,j3);
		for( i=0; i<numberOfIsoSurfaces; i++ )
		  SISO(j1,j2,j3,i)=uValue; // u.sa(i1+j1,i2+j2,i3+j3,component);
	      }
	}
      }

      // *** contour plane ****
      // This routine determines the intersection of the contour plane
      // with the cell by splitting the cell into tetrahedra and computing the itersections
      // with the tetrahedra. A set of polygons are returned.

      // why can't I move this up here?
//     ISOSURF(s(0,0,0,0),ndatd,ndat,d(0,0,0,0),i1,i2,i3,numberOfContourPlanes,
// 	    contourLevel(0),numberOfVertices(0,0),px(0,0,0,0));  // *** why is px dimensioned with n anymore?
      for( int n=0; n<numberOfContourPlanes; n++)
      {
	int ns=contourPlaneIndex(n); // contourPlaneStart+n; // number of surfaces
	assert( ns>=0 && ns<totalNumberOfSurfaces );

	RealArray & cd = cs[ns].csData; 
	real *& pcd = cd.Array_Descriptor.Array_View_Pointer2;  // * note reference * (in case cd is resized)
	const int cdDim0=cd.getRawDataSize(0);
	const int cdDim1=cd.getRawDataSize(1);
	int ierr=0;
	ISOSURF(nTet,S(0,0,0,n),ndatd,ndat,D(0,0,0,0),i1,i2,i3,1,
		contourLevel(n),numberOfVertices(0,n),px(0,0,0,n),ierr);  // *** why is px dimensioned with n anymore?
	const bool isoSurfGotConfused = ierr!=0;
	if ( isoSurfGotConfused )
	{
          numberOfIsoSurfConfusedWarnings++;
	  if( numberOfIsoSurfConfusedWarnings<10 )
	  {
	    printF("contour3d : WARNING : isosurf confused by cell (i1,i2,i3) on grid %i\n",i1,i2,i3,grid);
	  }
	  else if( numberOfIsoSurfConfusedWarnings==10 )
	  {
	    printF("contour3d : WARNING : too many isosurf confused warnings. I am not printing anymore\n");
	  }
	  
	}
	
	for( int ip=0; ip<nTet && !isoSurfGotConfused /*5*/; ip++) // there can be up to 5 triangles or squares
	{
	  int nv=numberOfVertices(ip,n);
	  if( nv>0 )       // number of vertices on this polygon
	  {
	    const int np=numberOfPolygonsPerSurface(ns);
	    // assert( np<cd.getLength(2) );
            if( np>=cd.getLength(2) )
	    {
              resizeForMorePolygons(cd,pcd,numberOfVerticesPerSurface_,numberOfVerticesPerSurfacep);
	    }

	    numberOfPolygonsPerSurface(ns)+=1;  // number of polygons
	    numberOfVerticesPerSurface(ns,np)=nv;
	    // printf(" ns=%i, np=%i, nv=%i\n",ns,np,nv);
	  
	    // check orientation of the polygon
	    //  a=px(.,1)-px(.,0)  b=px(.,2)-px(.,1)  check (aXb)*normal
	    a[0]=px(0,1,ip,n)-px(0,0,ip,n);
	    a[1]=px(1,1,ip,n)-px(1,0,ip,n);
	    a[2]=px(2,1,ip,n)-px(2,0,ip,n);
	    b[0]=px(0,2,ip,n)-px(0,1,ip,n);
	    b[1]=px(1,2,ip,n)-px(1,1,ip,n);
	    b[2]=px(2,2,ip,n)-px(2,1,ip,n);
	    // b=px(Range(0,2),2,ip,n)-px(Range(0,2),1,ip,n);
	    if(  (a[1]*b[2]-a[2]*b[1])*normal(0,n)
		 +(a[2]*b[0]-a[0]*b[2])*normal(1,n)
		 +(a[0]*b[1]-a[1]*b[0])*normal(2,n) > 0. )
	    {
	      for( int j=0; j<nv; j++ )
	      {
		CD(0,j,np)=px(0,j,ip,n);
		CD(1,j,np)=px(1,j,ip,n);
		CD(2,j,np)=px(2,j,ip,n);
		CD(3,j,np)=px(3,j,ip,n);

		cs[ns].minValue=min(cs[ns].minValue,CD(3,j,np));
		cs[ns].maxValue=max(cs[ns].maxValue,CD(3,j,np));
	      }
	    }
	    else
	    {
	      for( int j=0; j<nv; j++ )
	      {
		int jm=nv-1-j;
	      
		CD(0,j,np)=px(0,jm,ip,n);
		CD(1,j,np)=px(1,jm,ip,n);
		CD(2,j,np)=px(2,jm,ip,n);
		CD(3,j,np)=px(3,jm,ip,n);

		cs[ns].minValue=min(cs[ns].minValue,CD(3,j,np));
		cs[ns].maxValue=max(cs[ns].maxValue,CD(3,j,np));
	      }
	    }

	  }
	}
      }
      if( numberOfIsoSurfaces>0 )
      {
	// *** plot iso surfaces ****
	// This routine determines the intersection of the iso-surface
	// with the cell by splitting the cell into tetrahedra and computing the itersections
	// with the tetrahedra. A set of polygons are returned.
	int ierr=0;
	ISOSURF(nTet,SISO(0,0,0,0),ndatd,ndat,D(0,0,0,0),i1,i2,i3,numberOfIsoSurfaces,
		isoSurfaceValue(0,0),numberOfVertices(0,0),px(0,0,0,0),ierr);
	const bool isoSurfGotConfused = ierr!=0;
	if ( isoSurfGotConfused )
	{
          numberOfIsoSurfConfusedWarnings++;
	  if( numberOfIsoSurfConfusedWarnings<10 )
	  {
	    printF("contour3d : WARNING : isosurf confused by cell (i1,i2,i3) on grid %i.\n",i1,i2,i3,grid);
	  }
	  else if( numberOfIsoSurfConfusedWarnings==10 )
	  {
	    printF("contour3d : WARNING : too many isosurf confused warnings. I am not printing anymore.\n");
	  }
	}

	for( int n=0; n<numberOfIsoSurfaces && !isoSurfGotConfused; n++)
	{
	  int ns=isoSurfaceIndex(n); // isoSurfaceStart+n; // number of surfaces
	  assert( ns>=0 && ns<totalNumberOfSurfaces );

	  // cs[ns].colourIndex=ContourSurface::colourSurfaceDefault;  // reset this 

	  RealArray & cd =  cs[ns].csData; 
	  real *& pcd = cd.Array_Descriptor.Array_View_Pointer2; // * note reference * (in case cd is resized)
	  const int cdDim0=cd.getRawDataSize(0);
	  const int cdDim1=cd.getRawDataSize(1);


	  for( int ip=0; ip<nTet/*5*/; ip++) // there can be up to 5 triangles or squares
	  {
	    const int nv=numberOfVertices(ip,n);
	    if( nv>0 )       // number of vertices on this polygon
	    {
	      const int np=numberOfPolygonsPerSurface(ns);
	      // assert( np<cd.getLength(2) );
	      if( np>=cd.getLength(2) )
	      {
                resizeForMorePolygons(cd,pcd,numberOfVerticesPerSurface_,numberOfVerticesPerSurfacep);
	      }

	      numberOfPolygonsPerSurface(ns)+=1;  // number of polygons
	      numberOfVerticesPerSurface(ns,np)=nv;

	      // check orientation of the polygon
	      //  a=px(.,1)-px(.,0)  b=px(.,2)-px(.,1)  check (aXb)*normal
	      // a=px(Range(0,2),1,ip,n)-px(Range(0,2),0,ip,n);
	      // b=px(Range(0,2),2,ip,n)-px(Range(0,2),1,ip,n);
	      a[0]=px(0,1,ip,n)-px(0,0,ip,n);
	      a[1]=px(1,1,ip,n)-px(1,0,ip,n);
	      a[2]=px(2,1,ip,n)-px(2,0,ip,n);
	      b[0]=px(0,2,ip,n)-px(0,1,ip,n);
	      b[1]=px(1,2,ip,n)-px(1,1,ip,n);
	      b[2]=px(2,2,ip,n)-px(2,1,ip,n);

	      real n0=a[1]*b[2]-a[2]*b[1];
	      real n1=a[2]*b[0]-a[0]*b[2];
	      real n2=a[0]*b[1]-a[1]*b[0];
	      real norm =n0*n0+n1*n1+n2*n2;

	      // compare normal to previous normal **** this will not always work *****
	      bool orient= n0*normalV[0]+n1*normalV[1]+n2*normalV[2] > 0.;
	      if( norm>0. )
	      {
		norm= orient ? 1./sqrt(norm) : -1./sqrt(norm);
		normalV[0]=n0*norm;;
		normalV[1]=n1*norm;;
		normalV[2]=n2*norm;;
	      }
	      else
	      {
		normalV[0]=sqrt3i;
		normalV[1]=sqrt3i;
		normalV[2]=sqrt3i;
	      }

	      CD(0,nv,np)=normalV[0]; // save normal here
	      CD(1,nv,np)=normalV[1];
	      CD(2,nv,np)=normalV[2];

	      if( orient )
	      {
		for( int j=0; j<nv; j++ )
		{
		  CD(0,j,np)=px(0,j,ip,n);
		  CD(1,j,np)=px(1,j,ip,n);
		  CD(2,j,np)=px(2,j,ip,n);
		  CD(3,j,np)=px(3,j,ip,n);
		}
	      }
	      else
	      {
		for( int j=0; j<nv; j++ )
		{
		  int jm=nv-1-j;
	      
		  CD(0,j,np)=px(0,jm,ip,n);
		  CD(1,j,np)=px(1,jm,ip,n);
		  CD(2,j,np)=px(2,jm,ip,n);
		  CD(3,j,np)=px(3,jm,ip,n);
		}
	      }
	    }
	  }
	}
      }
    
    } // end FOR_3(i1,i2,i3,...
  }
  
  
  for( int n=0; n<numberOfContourPlanes; n++)
  {
    int ns=contourPlaneIndex(n); // contourPlaneStart+n; // number of surfaces
    assert( ns>=0 && ns<totalNumberOfSurfaces );
    cs[ns].minValue=ParallelUtility::getMinValue(cs[ns].minValue);
    cs[ns].maxValue=ParallelUtility::getMaxValue(cs[ns].maxValue);
    // printF("contour3dOpt:INFO: grid=%i, contour plane %i : [min,max]=[%8.2e,%8.2e]\n",ns,cs[ns].minValue,cs[ns].maxValue);
  }

  int i1a[4], i2a[4], i3a[4];
  int side,axis;
  
  // *****************************************************
  // **** save the data for the coordinate surfaces ******
  // *****************************************************

  for( n=0; n<totalNumberOfCoordinatePlanes && isStructured; n++ )
  {
    // const int ns=coordinatePlaneStart+n; // surface number
    int ns=coordinateSurfaceIndex(n); // isoSurfaceStart+n; // number of surfaces
    assert( ns>=0 && ns<totalNumberOfSurfaces );

    assert( cs[ns].surfaceType==ContourSurface::coordinateSurface );

    if( cs[ns].grid!=grid ) continue;
    
    side=cs[ns].side;
    axis=cs[ns].axis;
    assert( axis>=0 && axis<3 );
    if( side>=0 )
    {
      assert( side>=0 && side<=1 );
      getBoundaryIndex(extendedGridIndexRange(mg),side,axis,I1,I2,I3);
    }
    else
    {
      int index=cs[ns].index;
      getGhostIndex(mg.gridIndexRange(),Start,axis,I1,I2,I3,mg.indexRange(Start,axis)-index); 
    }
    
    RealArray & cd =  cs[ns].csData; 
    real *& pcd = cd.Array_Descriptor.Array_View_Pointer2; // * note reference * (in case cd is resized)
    const int cdDim0=cd.getRawDataSize(0);
    const int cdDim1=cd.getRawDataSize(1);


    if( (bool)mg.isAllVertexCentered() )
    { // don't do this for cell center grids so we get periodic edges
      I1= axis==0 ? I1 : Index(I1.getBase(),I1.length()-1);
      I2= axis==1 ? I2 : Index(I2.getBase(),I2.length()-1);
      I3= axis==2 ? I3 : Index(I3.getBase(),I3.length()-1);
    }

    // Initialize min/max values for coordinate planes
    cs[ns].minValue=     REAL_MAX;
    cs[ns].maxValue= -.1*REAL_MAX;
    

    gridHasPoints = ParallelUtility::getLocalArrayBounds(u,uu,I1,I2,I3);
    if( gridHasPoints )
    {
      FOR_3(i1,i2,i3,I1,I2,I3)
      {
	i1a[0]=i1;     i2a[0]=i2;     i3a[0]=i3;
	if( axis==0 )
	{
	  i1a[1]=i1;     i2a[1]=i2+1;   i3a[1]=i3;
	  i1a[2]=i1;     i2a[2]=i2+1;   i3a[2]=i3+1;
	  i1a[3]=i1;     i2a[3]=i2;     i3a[3]=i3+1;
	}
	else if( axis==1 )
	{
	  i1a[1]=i1;     i2a[1]=i2;     i3a[1]=i3+1;
	  i1a[2]=i1+1;   i2a[2]=i2;     i3a[2]=i3+1;
	  i1a[3]=i1+1;   i2a[3]=i2;     i3a[3]=i3;
	}
	else
	{
	  i1a[1]=i1+1;   i2a[1]=i2;     i3a[1]=i3;
	  i1a[2]=i1+1;   i2a[2]=i2+1;   i3a[2]=i3;
	  i1a[3]=i1;     i2a[3]=i2+1;   i3a[3]=i3;
	}

	if(MASK(i1a[0],i2a[0],i3a[0])==0 || MASK(i1a[1],i2a[1],i3a[1])==0 || 
	   MASK(i1a[2],i2a[2],i3a[2])==0 || MASK(i1a[3],i2a[3],i3a[3])==0 )
	  continue;  // skip this point

	if( numberOfRefinementLevels>1 )
	{
	  // on refinement grids do not plot cells with ANY corner hidden by refinement *wdh* 030626
	  if(MASK(i1a[0],i2a[0],i3a[0])&isHiddenByRefinement ||
	     MASK(i1a[1],i2a[1],i3a[1])&isHiddenByRefinement ||
	     MASK(i1a[2],i2a[2],i3a[2])&isHiddenByRefinement ||
	     MASK(i1a[3],i2a[3],i3a[3])&isHiddenByRefinement )
	    continue;
	}

	const int nv=4; // always quads
      
	const int np=numberOfPolygonsPerSurface(ns);
	// assert( np<cd.getLength(2) );
	if( np>=cd.getLength(2) )
	{
	  resizeForMorePolygons(cd,pcd,numberOfVerticesPerSurface_,numberOfVerticesPerSurfacep);
	}

	numberOfPolygonsPerSurface(ns)+=1;  // number of polygons
	numberOfVerticesPerSurface(ns,np)=nv;

	if( isRectangular )
	{
	  for( int j=0; j<4; j++ )
	  { 
	    CD(0,j,np)=CENTER0(i1a[j],i2a[j],i3a[j]);
	    CD(1,j,np)=CENTER1(i1a[j],i2a[j],i3a[j]);
	    CD(2,j,np)=CENTER2(i1a[j],i2a[j],i3a[j]);
	    CD(3,j,np)=U(i1a[j],i2a[j],i3a[j],component);  // treat general case using .sa

            cs[ns].minValue=min(cs[ns].minValue,CD(3,j,np));
            cs[ns].maxValue=max(cs[ns].maxValue,CD(3,j,np));

	  }
	}
	else
	{
	      
	  for( int j=0; j<4; j++ )
	  { 
	    CD(0,j,np)=COORD(i1a[j],i2a[j],i3a[j],0);
	    CD(1,j,np)=COORD(i1a[j],i2a[j],i3a[j],1);
	    CD(2,j,np)=COORD(i1a[j],i2a[j],i3a[j],2);
	    CD(3,j,np)=U(i1a[j],i2a[j],i3a[j],component);  // treat general case using .sa

            cs[ns].minValue=min(cs[ns].minValue,CD(3,j,np));
            cs[ns].maxValue=max(cs[ns].maxValue,CD(3,j,np));
	    
	    // *don't need normal if we don't shade ** getNormal(coord,iv,axis,normalV);
	  
	  }
	}
	
      }
    } // end hasGridPoints
    
    cs[ns].minValue=ParallelUtility::getMinValue(cs[ns].minValue);
    cs[ns].maxValue=ParallelUtility::getMaxValue(cs[ns].maxValue);
    printF("contour3dOpt:INFO: coordinate plane %i : [min,max]=[%8.2e,%8.2e]\n",ns,cs[ns].minValue,cs[ns].maxValue);

  }  // end for n (totalNumberOfCoordinatePlanes)
  
}

#undef numberOfVerticesPerSurface
#undef numberOfPolygonsPerSurface
#undef numberOfVertices
#undef px
#undef contourLevel
#undef isoSurfaceValue
#undef normal
#undef D
#undef S
#undef sIso
#undef COORD


// =============================================================================================
// Optimised version of the 3d contour plotter
// ============================================================================================
void PlotIt::
plot3dContours(GenericGraphicsInterface &gi, 
               const realGridCollectionFunction & uGCF, 
               GraphicsParameters & psp,
               int list, int lightList,
               bool & plotContours,
               bool & recomputeVelocityMinMax, real & uMin, real & uMax,
               ContourSurface *&cs,
               IntegerArray & numberOfPolygonsPerSurface_, 
               IntegerArray & numberOfVerticesPerSurface_ )
{
  const bool showTimings=false; // true;
  real time0=getCPU(), time1;

  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int graphicsProcessor = gi.getProcessorForGraphics();
  const bool plotOnThisProcessor = Communication_Manager::localProcessNumber()==gi.getProcessorForGraphics();

#ifdef USE_PPP
  if( false && PlotIt::parallelPlottingOption==1 )
  {
    printf("*** plot3dContours: START myid=%i ***\n",myid);
    fflush(0);
    MPI_Barrier(Overture::OV_COMM);
  }
#endif


  const GridCollection & gc = *(uGCF.gridCollection);
  const int numberOfGrids = gc.numberOfComponentGrids();
  
  bool & plotWireFrame        = psp.plotWireFrame;
  bool & colourLineContours   = psp.colourLineContours;
  bool & plotColourBar        = psp.plotColourBar;
  bool & plotContourLines     = psp.plotContourLines;
  bool & plotShadedSurface    = psp.plotShadedSurface;
  bool & plotTitleLabels      = psp.plotTitleLabels;
  int  & numberOfContourLevels= psp.numberOfContourLevels;
  
  int  & component            = psp.componentForContours;
  IntegerArray & gridsToPlot      = psp.gridsToPlot;
  IntegerArray & minAndMaxContourLevelsSpecified  = psp.minAndMaxContourLevelsSpecified;
  RealArray & minAndMaxContourLevels = psp.minAndMaxContourLevels;
  real & minimumContourSpacing = psp.minimumContourSpacing;

  int & numberOfIsoSurfaces     = psp.numberOfIsoSurfaces;
  RealArray & isoSurfaceValue   = psp.isoSurfaceValue;
  int & numberOfCoordinatePlanes= psp.numberOfCoordinatePlanes; 
  IntegerArray & coordinatePlane    = psp.coordinatePlane;
  // We use this array to plot contours on the faces of grids
  IntegerArray & plotContourOnGridFace = psp.plotContourOnGridFace;

  int & numberOfGhostLinesToPlot = psp.numberOfGhostLinesToPlot;

  isHiddenByRefinement = psp.plotHiddenRefinementPoints ? 0 : MappedGrid::IShiddenByRefinement;

  // ---- variables for contour planes ------
  int & numberOfContourPlanes=psp.numberOfContourPlanes;
  RealArray & contourPlane=psp.contourPlane;   // values 0:2 = normal, 3:5 = point on plane
  const RealArray & normal = contourPlane; // (Range(0,2),nullRange);

  const int totalNumberOfCoordinatePlanes=numberOfCoordinatePlanes+sum(plotContourOnGridFace);
  const int totalNumberOfSurfaces=totalNumberOfCoordinatePlanes+
                              numberOfContourPlanes+ numberOfIsoSurfaces;

  if( plotOnThisProcessor )
    glDeleteLists(list,1);  // clear the plot (AP: not necessary since it gets overwritten)

  int i;
  


  const RealArray & pt = contourPlane(Range(3,5),nullRange);
  RealArray & point = (RealArray &)pt;
  point.setBase(0,0);

  RealArray contourLevel;
  contourLevel.redim(numberOfContourPlanes);
  for( i=0; i<numberOfContourPlanes; i++ )
    contourLevel(i)=normal(0,i)*point(0,i)+normal(1,i)*point(1,i)+normal(2,i)*point(2,i);


//  ContourSurface *cs=NULL;


  IntegerArray coordinateSurfaceIndex, contourPlaneIndex, isoSurfaceIndex;
  IntegerArray coordinateSurfaceIndexNew, contourPlaneIndexNew, isoSurfaceIndexNew;

  const int maxNumberOfPolygons=numberOfVerticesPerSurface_.getLength(1); // 10000;

  int *& numberOfPolygonsPerSurfacep = numberOfPolygonsPerSurface_.Array_Descriptor.Array_View_Pointer0;
#define numberOfPolygonsPerSurface(i0) numberOfPolygonsPerSurfacep[i0]

  int grid;
  

  bool recomputeContourPlanes=true;   // ********************************************* fix this **********************

#define numberOfVerticesPerSurface(i0,i1) numberOfVerticesPerSurfacep[i0+numberOfVerticesPerSurfaceDim0*(i1)]

  if( recomputeContourPlanes )
  {
    // ***** allocate space ****
    //    build index arrays 

    coordinateSurfaceIndex.redim(totalNumberOfCoordinatePlanes);
    contourPlaneIndex.redim(numberOfContourPlanes);
    isoSurfaceIndex.redim(numberOfIsoSurfaces);

    // coordinateSurfaceIndexNew(nscNew) = index into cs[.] for *new* contour surfaces
    // 
    coordinateSurfaceIndexNew.redim(totalNumberOfCoordinatePlanes);
    contourPlaneIndexNew.redim(numberOfContourPlanes);
    isoSurfaceIndexNew.redim(numberOfIsoSurfaces);

    int ncs=0; // counts coord surfaces
    int ncp=0; // contour planes
    int ni=0;  // iso surfaces
    int ncsNew=0, ncpNew=0, niNew=0;  // these count new ones.
    Range all;

    int ns=0; // counts surfaces
    for( ns=0; ns<totalNumberOfSurfaces; ns++ )
    {
      if( cs[ns].surfaceType==ContourSurface::coordinateSurface )
      {
	if( cs[ns].surfaceStatus==ContourSurface::notBuilt )
	{
	  cs[ns].csData.redim(4,4,maxNumberOfPolygons); 
	  coordinateSurfaceIndexNew(ncsNew)=ns; ncsNew++;
	  numberOfPolygonsPerSurface_(ns)=0;
          numberOfVerticesPerSurface_(ns,all)=0;
	}
	coordinateSurfaceIndex(ncs)=ns;  ncs++;
      }
      else if( cs[ns].surfaceType==ContourSurface::contourPlane )
      {
	if( cs[ns].surfaceStatus==ContourSurface::notBuilt )
	{
	  cs[ns].csData.redim(4,4,maxNumberOfPolygons); 
	  contourPlaneIndexNew(ncpNew)=ns; ncpNew++;
	  numberOfPolygonsPerSurface_(ns)=0;
          numberOfVerticesPerSurface_(ns,all)=0;
	}
	contourPlaneIndex(ncp)=ns;  ncp++;
      }
      else if( cs[ns].surfaceType==ContourSurface::isoSurface )
      {
	if( cs[ns].surfaceStatus==ContourSurface::notBuilt )
	{
	  cs[ns].csData.redim(4,5,maxNumberOfPolygons);   // ** note 5 **
	  isoSurfaceIndexNew(niNew)=ns;   niNew++;
	  numberOfPolygonsPerSurface_(ns)=0;
          numberOfVerticesPerSurface_(ns,all)=0;
	}
	isoSurfaceIndex(ni)=ns;   ni++;
      }

      cs[ns].surfaceStatus=ContourSurface::built;
    }
  
    if( ncsNew>0 ) 
      coordinateSurfaceIndexNew.resize( ncsNew );
    else
      coordinateSurfaceIndexNew.redim(0);
    if( ncpNew>0 ) 
      contourPlaneIndexNew.resize( ncpNew );
    else
      contourPlaneIndexNew.redim(0);
    if( niNew>0 ) 
      isoSurfaceIndexNew.resize( niNew );
    else
      isoSurfaceIndexNew.redim(0);
    
    
    if( ncsNew+ncpNew+niNew > 0 )
    {
      real timea=getCPU();
      if( showTimings) printf("***** computeContourPlanes: cs=%i, cp=%i, iso=%i ...\n",ncsNew,ncpNew,niNew);

      for( grid=0; grid<numberOfGrids; grid++ )
      {
	if( !(gridsToPlot(grid)&2) )
	  continue;
	computeContourPlanes(gc[grid],uGCF[grid],component,gc.numberOfRefinementLevels(),numberOfGhostLinesToPlot,
			     contourLevel, normal,
			     numberOfPolygonsPerSurface_,
			     numberOfVerticesPerSurface_,
			     cs, 
			     isoSurfaceValue,totalNumberOfSurfaces,
			     coordinateSurfaceIndexNew, contourPlaneIndexNew, isoSurfaceIndexNew,
			     grid,plotContourOnGridFace,coordinatePlane,psp );
      }
      if( showTimings )
      {
	printf("Time for computeContourPlanes=%8.3e\n",getCPU()-timea);
    
	printf("Polygons per surface: ");
	for( i=0; i<totalNumberOfSurfaces; i++ )
	  printf(" %i ",numberOfPolygonsPerSurface_(i));
	printf("\n");
      }
      // numberOfPolygonsPerSurface_.display("numberOfPolygonsPerSurface");

#ifdef USE_PPP
      // *new* finish me
      // In parallel, Copy the cs[i].csData from all other processors to the graphicsProcessor for plotting ----
      //     add the data to graphicsProcessor::cs[i].csData, increase the counters such as numberOfPolygonsPerSurface(ns)
      //  

      // if( proc != graphicsProcessor )
      //
      //    build a buffer of csData for all new surfaces
      //    
      // Send the data to graphicsProcessor
      // Augment the cs[i].csData on graphicsProcessor

      // *** Gather up all the data from the new surfaces ****

      const MPI_Comm & OV_COMM = Overture::OV_COMM;
      const int debug=0; // 3;
      
      const int npm1=np-1;
      if( np>1 && PlotIt::parallelPlottingOption==1 )
      {
	int numData=0;  // holds total number of data values that must be transferred

	const int numberOfNewSurfaces = ncsNew+ncpNew+niNew;
	assert( numberOfNewSurfaces>0 );

	int *& numberOfVerticesPerSurfacep = numberOfVerticesPerSurface_.Array_Descriptor.Array_View_Pointer1;
	const int numberOfVerticesPerSurfaceDim0=numberOfVerticesPerSurface_.getRawDataSize(0);


	// numPoly[m] = number of polygons on the surface m (contour-surface, coord-plane or iso-surf)
	int *numPoly = NULL;
	int m=0; // counts new surfaces
	if( !plotOnThisProcessor )
	{
	  numPoly = new int [numberOfNewSurfaces+1];
	
	  // New coordinate surfaces:
	  for( int n=0; n<ncsNew; n++ )
	  {
	    int ns = coordinateSurfaceIndexNew(n);
	    // Copy: cs[ns].csData.redim(4,4,.); 
	    numData += (16+1)*numberOfPolygonsPerSurface(ns);  // 16+1 : 1=numberOfVertices is also saved
	    numPoly[m]=numberOfPolygonsPerSurface(ns); m++;
	  }
	  for( int n=0; n<ncpNew; n++ )
	  {
	    int ns=contourPlaneIndexNew(n); 
	    // copy : cs[ns].csData.redim(4,4,maxNumberOfPolygons); 
	    numData += (16+1)*numberOfPolygonsPerSurface(ns);
	    numPoly[m]=numberOfPolygonsPerSurface(ns); m++;
	  }
	  for( int n=0; n<niNew; n++ )
	  {
	    int ns=isoSurfaceIndexNew(niNew);      
	    // copy:   cs[ns].csData.redim(4,5,maxNumberOfPolygons);   // ** note 5 **
	    numData += (20+1)*numberOfPolygonsPerSurface(ns);
	    numPoly[m]=numberOfPolygonsPerSurface(ns); m++;
	  }
	  numPoly[m]=numData;  // save total data to send here
	  assert( m==numberOfNewSurfaces );
	}
      
	// --- see ogshow/CopyArray.bC ----

	// Send numPoly[] from processor "p" to the graphicsProcessor

	// --- post receives ---
	MPI_Request *receiveRequest=NULL; //      = new MPI_Request[np];  
	MPI_Request sendRequest, sendRequest2;
	// MPI_Request *sendRequestAnswers = new MPI_Request[np];  

	// numPolyPerProc(m,p) = number of polygons on surface m, processor p
	int *pNumPolyPerProc=NULL;
	const int nnsp1=numberOfNewSurfaces+1;
#define numPolyPerProc(m,p) pNumPolyPerProc[(m)+nnsp1*(p)]
	const int tag1=842903;
	if( plotOnThisProcessor )
	{
	  // --- On the graphics-processor: post receives from other processors ---

	  pNumPolyPerProc = new int [npm1*(numberOfNewSurfaces+1)];
	  receiveRequest     = new MPI_Request[npm1];  
	  for( int p=0, pr=0; p<np; p++ )
	  {  
	    if( p!=graphicsProcessor )
	    {
	      int tag=tag1+p;
	      MPI_Irecv( &numPolyPerProc(0,pr), nnsp1, MPI_INT, p, tag, OV_COMM, &receiveRequest[pr] );
              pr++;
	    }
	  }
	}
	else
	{
	  // On other processors: Send "numData" from processor "p" to the graphicsProcessor
	  if( debug & 1 )
	  {
	    printf("C3d: myid=%i : send to graphicsProcessor=%i: numPoly = ",myid,graphicsProcessor);
	    for( int m=0; m<numberOfNewSurfaces; m++ )
	    {
	      printf("%i, ",numPoly[m]);
	    }
	    printf(", total-data=%i\n",numPoly[numberOfNewSurfaces]);
	  }
	  
	  int tag=tag1+myid;
	  MPI_Isend(numPoly, nnsp1, MPI_INT, graphicsProcessor, tag, OV_COMM, &sendRequest );  
	}
	if( debug & 1 )
	{
	  fflush(0);
	  MPI_Barrier(OV_COMM);
	}

        real **rbuff=NULL;   // receive buffer   r[pr][i] : data from processor 
        real *sbuff=NULL;    // send buffer
	
	// --- wait for all the receives to finish ---
	MPI_Status *receiveStatus=NULL;
      	const int tag2=731892;
	if( plotOnThisProcessor )
	{
	  receiveStatus= new MPI_Status [npm1];  
	  MPI_Waitall(npm1,receiveRequest,receiveStatus);
      
          // allocate space for receive-buffers and post receives of polygon data

          rbuff = new real* [npm1];
	  for( int p=0, pr=0; p<np; p++ )
	  {
	    if( p!=graphicsProcessor )
	    {
	      int numDataToReceive = numPolyPerProc(numberOfNewSurfaces,pr);
	      
              rbuff[pr] = new real [max(1,numDataToReceive)];

              int tag=tag2+p;
	      MPI_Irecv( rbuff[pr], numDataToReceive, MPI_Real, p, tag, OV_COMM, &receiveRequest[pr] );

	      if( debug & 1 )
	      {
		for( int m=0; m<numberOfNewSurfaces; m++ )
		{
		  printf("C3d: myid=%i: Will receive %i polygons for surface %i, from p=%i\n",
			 myid,numPolyPerProc(m,pr),m,p);
		}
		printf(" C3d: myid=%i: will get total data =%i from p=%i\n",
		       myid,numPolyPerProc(numberOfNewSurfaces,pr),p);
	      }
	      
              pr++;
	    }
	  }
	}

	if( debug & 1 )
	{
	  fflush(0);
	  MPI_Barrier(OV_COMM);
	}

	if( !plotOnThisProcessor )
	{
          // pack up send-buffer and send the data to the graphicsProcessor

	  sbuff = new real [numData]; // holds data to send to graphicsProcessor 
	  
	  int i=0;  // counts entries into sbuff
	  for( int n=0; n<ncsNew; n++ )
	  {
	    int ns = coordinateSurfaceIndexNew(n);
	    RealArray & csData = cs[ns].csData;  // csData(4,4,.); 
            for( int j=0; j<numberOfPolygonsPerSurface(ns); j++ )
	    {
	      for( int m2=0; m2<4; m2++ )for( int m1=0; m1<4; m1++ ) 
	      {
		sbuff[i]=csData(m1,m2,j); i++;
	      }
              sbuff[i] = int(numberOfVerticesPerSurface(ns,j)+.5); i++;
	    }
	  }
	  for( int n=0; n<ncpNew; n++ )
	  {
	    int ns=contourPlaneIndexNew(n); 
	    RealArray & csData = cs[ns].csData;  // csData(4,4,.); 
            for( int j=0; j<numberOfPolygonsPerSurface(ns); j++ )
	    {
	      for( int m2=0; m2<4; m2++ )for( int m1=0; m1<4; m1++ ) 
	      {
		sbuff[i]=csData(m1,m2,j); i++;
	      }
              sbuff[i] = int(numberOfVerticesPerSurface(ns,j)+.5); i++;
	    }
	  }
	  for( int n=0; n<niNew; n++ )
	  {
	    int ns=isoSurfaceIndexNew(niNew);      
	    // copy:   cs[ns].csData.redim(4,5,maxNumberOfPolygons);   // ** note 5 **
	    RealArray & csData = cs[ns].csData;  // csData(4,4,.); 
            for( int j=0; j<numberOfPolygonsPerSurface(ns); j++ )
	    {
	      for( int m2=0; m2<5; m2++ )for( int m1=0; m1<4; m1++ ) 
	      {
		sbuff[i]=csData(m1,m2,j); i++;
	      }
              sbuff[i] = int(numberOfVerticesPerSurface(ns,j)+.5); i++;
	    }
	  }
          assert( i==numData );
	  
	  int tag=tag2+myid;
	  MPI_Isend(sbuff, numData, MPI_Real, graphicsProcessor, tag, OV_COMM, &sendRequest2);

	  if( debug & 4 )
	  {
	    printf("C3d: myid=%i: send sbuff=[",myid);
	    for( int i=0; i<numData; i++ )
	    {
	      printf("%3.1f ",sbuff[i]);
	    }
	    printf("]\n");
	  }
	}
	
	if( debug & 1 )
	{
	  fflush(0);
	  MPI_Barrier(OV_COMM);
	}

	// --- wait for all the receives to finish ---
	if( plotOnThisProcessor )
	{
	  MPI_Waitall(npm1,receiveRequest,receiveStatus);
	  

          // First count up the total number of new polygons that we add to each new surface.
	  IntegerArray numberOfPolygonsPerNewSurface(numberOfNewSurfaces);
	  numberOfPolygonsPerNewSurface=0;
          for( int pr=0; pr<npm1; pr++ )
	  {
	    for( int m=0; m<numberOfNewSurfaces; m++ )
	    {
	      numberOfPolygonsPerNewSurface(m)+=numPolyPerProc(m,pr);
	    }
	  }

          // allocate new space for the additional polygons
          int totalNumberOfPolygons=0;

	  int m=0;  // counts new surfaces
	  for( int n=0; n<ncsNew; n++ )
	  {
	    int ns = coordinateSurfaceIndexNew(n);
            int newNum = numberOfPolygonsPerSurface(ns) + numberOfPolygonsPerNewSurface(m);
            cs[ns].csData.resize(4,4,newNum);
	    m++;
	    totalNumberOfPolygons+=newNum;
	  }
	  for( int n=0; n<ncpNew; n++ )
	  {
	    int ns=contourPlaneIndexNew(n); 
            int newNum = numberOfPolygonsPerSurface(ns) + numberOfPolygonsPerNewSurface(m);
            cs[ns].csData.resize(4,4,newNum);
	    m++;
	    totalNumberOfPolygons+=newNum;
	  }
	  for( int n=0; n<niNew; n++ )
	  {
	    int ns=isoSurfaceIndexNew(niNew);      
            int newNum = numberOfPolygonsPerSurface(ns) + numberOfPolygonsPerNewSurface(m);
            cs[ns].csData.resize(4,5,newNum); // *note* 5 
	    m++;
	    totalNumberOfPolygons+=newNum;
	  }
	  assert( m==numberOfNewSurfaces );


          // allocate extra space in numberOfVerticesPerSurface_
	  if( totalNumberOfPolygons>numberOfVerticesPerSurface_.getLength(1) )
	    numberOfVerticesPerSurface_.resize(numberOfVerticesPerSurface_.getLength(0),totalNumberOfPolygons);

          // un-pack buffers

	  for( int p=0, pr=0; p<np; p++ )
	  {
	    if( p!=graphicsProcessor )
	    {
	      int numDataToReceive = numPolyPerProc(numberOfNewSurfaces,pr);

	      if( debug & 4 )
	      {
		printf("C3d: myid=%i: receive from p=%i : rbuff=[",myid,p);
		for( int i=0; i<numDataToReceive; i++ )
		{
		  printf("%3.1f ",rbuff[pr][i]);
		}
		printf("]\n");
	      }

	      int i=0;  // counts entries into rbuff[pr]
              int m=0;  // counts surfaces
	      for( int n=0; n<ncsNew; n++ )
	      {
		int ns = coordinateSurfaceIndexNew(n);
		int & nps = numberOfPolygonsPerSurface(ns);   // note reference 
		RealArray & csData = cs[ns].csData;  // csData(4,4,.); 
		for( int j=0; j<numPolyPerProc(m,pr); j++ )
		{
		  for( int m2=0; m2<4; m2++ )for( int m1=0; m1<4; m1++ ) 
		  {
		    csData(m1,m2,nps)=rbuff[pr][i];  i++; 
		  }
   	          numberOfVerticesPerSurface(ns,nps)=int(rbuff[pr][i]);  i++; 
		  nps++;
		}
		assert( nps <=csData.getBound(2)+1  &&  nps <= numberOfVerticesPerSurface_.getLength(1) );
		m++;
	      }
	      for( int n=0; n<ncpNew; n++ )
	      {
		int ns=contourPlaneIndexNew(n); 
		int & nps = numberOfPolygonsPerSurface(ns);   // note reference
		RealArray & csData = cs[ns].csData;  // csData(4,4,.); 
		for( int j=0; j<numPolyPerProc(m,pr); j++ )
		{
		  for( int m2=0; m2<4; m2++ )for( int m1=0; m1<4; m1++ ) 
		  {
		    csData(m1,m2,nps)=rbuff[pr][i]; i++; 
		  }

   	          numberOfVerticesPerSurface(ns,nps)=int(rbuff[pr][i]);  i++;   // ********* fix this -- there may not be enough space *****

		  // if( debug & 1 ) printf(" myid=%i: new poly %i has %i vertices\n",myid,nps,numberOfVerticesPerSurface(ns,nps));
		  assert( numberOfVerticesPerSurface(ns,nps)<7 );
		  
		  nps++;
		}
		assert( nps <=csData.getBound(2)+1  &&  nps <= numberOfVerticesPerSurface_.getLength(1) );
		m++;
	      }
	      for( int n=0; n<niNew; n++ )
	      {
		int ns=isoSurfaceIndexNew(niNew);      
		int & nps = numberOfPolygonsPerSurface(ns);   // note reference
		// copy:   cs[ns].csData.redim(4,5,maxNumberOfPolygons);   // ** note 5 **
		RealArray & csData = cs[ns].csData;  // csData(4,4,.); 
		for( int j=0; j<numPolyPerProc(m,pr); j++ )
		{
		  for( int m2=0; m2<5; m2++ )for( int m1=0; m1<4; m1++ ) 
		  {
		    csData(m1,m2,nps)=rbuff[pr][i]; i++; 
		  }
   	          numberOfVerticesPerSurface(ns,nps)=int(rbuff[pr][i]);  i++; 

                  nps++;
		}
		assert( nps <=csData.getBound(2)+1   &&  nps <= numberOfVerticesPerSurface_.getLength(1) );
		m++;
	      }
	      assert( i==numDataToReceive );
	      assert( m==numberOfNewSurfaces );
	      
	      pr++;
	    }
	  }
	  
	} // end if plotOnThisProcessor
	

	if( debug & 1 )
	{
	  fflush(0);
	  MPI_Barrier(OV_COMM);  // *** this is needed here, why???
	}
	
	
	// --- clean up  ----
	if( !plotOnThisProcessor )
	{
          // wait for send's to finish
          MPI_Status receiveStatus;
          MPI_Waitall(1,&sendRequest,&receiveStatus);
          MPI_Waitall(1,&sendRequest2,&receiveStatus);
	  
	}
	else
	{
	  for( int p=0; p<npm1; p++ )
            delete [] rbuff[p];
	  delete [] rbuff;
	}
	delete [] numPoly;
	delete [] pNumPolyPerProc;
	delete [] receiveStatus;
	delete [] receiveRequest;
	
	delete [] sbuff;

      } // end if np>1
      
#endif

    }
    else
    {
      if( showTimings ) printf("There are no new contour surfaces that need to be built\n");
    }

  } // end if( recomputeContourPlanes )
  
    // Get Bounds on u -- treat the general case when the component can be in any Index position of u
  if( !minAndMaxContourLevelsSpecified(component) )
  {
    if( psp.contour3dMinMaxOption==GraphicsParameters::baseMinMaxOnContourPlaneValues )
    {
      // Compute the min/max from the values that appear on the contour and coordinate planes
      uMin=    REAL_MAX;
      uMax=-.1*REAL_MAX;
      for( int ns=0; ns<totalNumberOfSurfaces; ns++ )
      {
	if( cs[ns].surfaceType==ContourSurface::coordinateSurface || cs[ns].surfaceType==ContourSurface::contourPlane )
	{
	  uMin=min(uMin,cs[ns].minValue);
	  uMax=max(uMax,cs[ns].maxValue);
	}
      }
      printF("contour3dOpt:INFO: min/max taken from contour/coordinate planes: min=%8.2e, max=%8.2e (num surfaces=%i)\n",
	     uMin,uMax,totalNumberOfSurfaces);
    }
    else if( recomputeVelocityMinMax )
    {
      recomputeVelocityMinMax=FALSE;
      getBounds(uGCF,uMin,uMax,psp,Range(component,component));
      // printF(" compute plot bounds, uMin=%e, uMax=%e \n",uMin,uMax);
    }
  }
  else
  {
    uMin=minAndMaxContourLevels(0,component);
    uMax=minAndMaxContourLevels(1,component);
    //  printf(" do NOT compute plot bounds, uMin=%e, uMax=%e \n",uMin,uMax);
  }

  real deltaU = uMax-uMin;
  if( deltaU==0. )
  {
    uMax+=.5;
    uMin-=.5;
  }
  else if( deltaU < minimumContourSpacing*numberOfContourLevels )
  {
    uMax+=minimumContourSpacing*numberOfContourLevels;
    uMin-=minimumContourSpacing*numberOfContourLevels;
    deltaU = uMax-uMin;
  }
      
  const real deltaUInverse =  deltaU==0. ? 1. : 1./deltaU;



  // --- the unlit list starts here ---
  if( plotOnThisProcessor ) 
  {
    glNewList(list,GL_COMPILE);

    gi.setColour("black");  // this line is needed for some bizzare reason*********
    // otherwise the colours are wrong for lighted 3d contours with labels turned off ********

    // Scale the picture to fit in [-1,1]

    glMatrixMode(GL_MODELVIEW);

    if( !plotWireFrame )
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glShadeModel(GL_SMOOTH);     // interpolate colours between vertices
    }
    else
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    glPolygonOffset(1.0,0.5*OFFSET_FACTOR); 
    glEnable(GL_POLYGON_OFFSET_FILL);  // so lines on surfaces are plotted correctly
  }
  

  int *& numberOfVerticesPerSurfacep = numberOfVerticesPerSurface_.Array_Descriptor.Array_View_Pointer1;
  const int numberOfVerticesPerSurfaceDim0=numberOfVerticesPerSurface_.getRawDataSize(0);
// #define numberOfVerticesPerSurface(i0,i1) numberOfVerticesPerSurfacep[i0+numberOfVerticesPerSurfaceDim0*(i1)]

#define XSCALE(x) (psp.xScaleFactor*(x))
#define YSCALE(y) (psp.yScaleFactor*(y))
#define ZSCALE(z) (psp.zScaleFactor*(z))

  // **************************************************************************
  // ************** Plot contours on planes and coordinate planes *************
  // **************************************************************************
  // We may want to have these lit or not
  time1=getCPU();
  if( plotShadedSurface && numberOfContourPlanes+totalNumberOfCoordinatePlanes > 0 )
  {
    for( int n=0; n<numberOfContourPlanes+totalNumberOfCoordinatePlanes; n++)
    {
      // number of surfaces:
      // const int ns=n<numberOfContourPlanes ? contourPlaneStart+n : coordinatePlaneStart+n-numberOfContourPlanes; 
      int ns;
      if( n<numberOfContourPlanes )
	ns=contourPlaneIndex(n);
      else
        ns=coordinateSurfaceIndex(n-numberOfContourPlanes);
      assert( ns>=0 && ns<totalNumberOfSurfaces );
      
      RealArray & cd =  cs[ns].csData; 
      real *& pcd = cd.Array_Descriptor.Array_View_Pointer2;  // * note reference * (in case cd is resized)
      const int cdDim0=cd.getRawDataSize(0);
      const int cdDim1=cd.getRawDataSize(1);

      if( plotOnThisProcessor ) 
      {
	const int contourPlaneID=123456+n;  // fix this 
	glPushName(contourPlaneID);

        assert( numberOfPolygonsPerSurface(ns) <= numberOfVerticesPerSurface_.getLength(1) );
        assert( numberOfPolygonsPerSurface(ns) <= cd.getLength(2) );
	
	for( int p=0; p<numberOfPolygonsPerSurface(ns); p++ )
	{
	  glBegin(GL_POLYGON); 
	  const int nv=numberOfVerticesPerSurface(ns,p);
	  assert( nv<10 );
	  for( int j=0; j<nv; j++ )
	  {
	    gi.setColourFromTable( (CD(3,j,p)-uMin)*deltaUInverse,psp );
	    // *wdh* 090425 glVertex3(CD(0,j,p),CD(1,j,p),CD(2,j,p));
	    glVertex3(XSCALE(CD(0,j,p)),YSCALE(CD(1,j,p)),ZSCALE(CD(2,j,p)));
	  }
	  glEnd();
	}
	glPopName();
      }

    }
  }
  if( showTimings ) printf("contour3d: contour/coord planes time=%8.3e\n",getCPU()-time1);
  time1=getCPU();


  // **** new ********************************************
  // ************** Plot contour Lines *****************
  // ***************************************************
  if( plotContourLines && numberOfContourPlanes+totalNumberOfCoordinatePlanes > 0 )
  {
    if( plotOnThisProcessor ) 
    {
      glLineWidth(psp.size(GraphicsParameters::majorContourWidth)*
		  psp.size(GraphicsParameters::lineWidth)*
		  gi.getLineWidthScaleFactor());  
      gi.setColour("black");
    }
    
    real deltaU=(uMax-uMin)/numberOfContourLevels;
    for( int n=0; n<numberOfContourPlanes+totalNumberOfCoordinatePlanes; n++)
    {
      // surface number:
      // const int ns=n<numberOfContourPlanes ? contourPlaneStart+n : coordinatePlaneStart+n-numberOfContourPlanes; 
      int ns;
      if( n<numberOfContourPlanes )
	ns=contourPlaneIndex(n);
      else
        ns=coordinateSurfaceIndex(n-numberOfContourPlanes);
      assert( ns>=0 && ns<totalNumberOfSurfaces );

      RealArray & cd =  cs[ns].csData; 
      real *& pcd = cd.Array_Descriptor.Array_View_Pointer2;  // * note reference * (in case cd is resized)
      const int cdDim0=cd.getRawDataSize(0);
      const int cdDim1=cd.getRawDataSize(1);
      const int np=numberOfPolygonsPerSurface(ns);
      
      if( plotOnThisProcessor ) 
      {
	for( int p=0; p<np; p++ )
	{
	  const int nv=numberOfVerticesPerSurface(ns,p);

	  real u0,u1; u0=CD(3,0,p); u1=u0;
	  for( int j=1; j<nv; j++ )
	  {
	    u0=min(u0,CD(3,j,p));
	    u1=max(u1,CD(3,j,p));
	  }
	  //	int iStart=int(ceil((u0-uMin)/deltaU));  // smallest int not less than (arg)
	  //	int iEnd  =int(floor((u1-uMin)/deltaU));

	  // kkc 040826 added some bounds to iStart and iEnd in case of 
	  //            unfornutate selections for uMin and uMax
	  int iStart=max(0,int(ceil((u0-uMin)/deltaU)));  // smallest int not less than (arg)
	  int iEnd  =min(int(floor((u1-uMin)/deltaU)),2*numberOfContourLevels);
	  if( u0>=u1 ) iStart++;   // don't draw lines on a flat "surface"
	  // loop over the possible contour levels than can pass through this square	
	  for( i =iStart; i<=iEnd; i++ )
	  {
	    real uv = i*deltaU+uMin;   // check this contour level 
	    glBegin(GL_LINES);  // draw line contours
	    for( int j=0; j<nv; j++ )  // loop over sides, 
	    {
	      int jp1 = (j+1) % nv;  // periodic wrap
	      if( (uv-CD(3,j,p))*(uv-CD(3,jp1,p))<=0. )  
	      { // contour level crosses this side
		real denom =CD(3,jp1,p)-CD(3,j,p);
		denom = (denom==0.) ? 1 : denom;
		real alpha = (uv-CD(3,j,p))/denom; 
		alpha=max(0.,min(1.,alpha));
		real uValue=CD(3,j,p)*(1.-alpha)+CD(3,jp1,p)*alpha;
		if( colourLineContours )
		  gi.setColourFromTable( (uValue-uMin)*deltaUInverse,psp );

		// glVertex3(CD(0,j,p)*(1.-alpha)+CD(0,jp1,p)*alpha, // *wdh* 090425 
		//	  CD(1,j,p)*(1.-alpha)+CD(1,jp1,p)*alpha,
		//	  CD(2,j,p)*(1.-alpha)+CD(2,jp1,p)*alpha);
		glVertex3(XSCALE(CD(0,j,p)*(1.-alpha)+CD(0,jp1,p)*alpha),
			  YSCALE(CD(1,j,p)*(1.-alpha)+CD(1,jp1,p)*alpha),
			  ZSCALE(CD(2,j,p)*(1.-alpha)+CD(2,jp1,p)*alpha));
	      }
	    }
	    glEnd();
	  } // end for (i=iStart...)
	} 
      }
    }  // end for n 
  }
  if( showTimings ) printf("contour3d: contour/coord planes:lines time=%8.3e\n",getCPU()-time1);
  time1=getCPU();

  if( plotOnThisProcessor ) 
  {
    glDisable(GL_POLYGON_OFFSET_FILL);
    glLineWidth(psp.size(GraphicsParameters::lineWidth)*
		gi.getLineWidthScaleFactor());  // reset
    glEndList();
  }
  

//
// the lit list starts here
//
  if( plotOnThisProcessor )
  {
    glNewList(lightList,GL_COMPILE);

    gi.setColour("black");  // this line is needed for some bizzare reason*********
     // otherwise the colours are wrong for lighted 3d contours with labels turned off ********

    // Scale the picture to fit in [-1,1]
    glMatrixMode(GL_MODELVIEW);

    if( !plotWireFrame )
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glShadeModel(GL_SMOOTH);     // interpolate colours between vertices
    }
    else
    {
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    //   glPolygonOffset(1.0,0.5*OFFSET_FACTOR); 
    //   glEnable(GL_POLYGON_OFFSET_FILL);  // so lines on surfaces are plotted correctly
    
  }
  
    // ****************************************************
  // ************** Plot iso surfaces       *************
  // ****************************************************
  time1=getCPU();
  if( plotShadedSurface && numberOfIsoSurfaces > 0 )
  {
    for( int n=0; n<numberOfIsoSurfaces; n++)
    {
      int ns=isoSurfaceIndex(n); // isoSurfaceStart+n; // number of surfaces
      assert( ns>=0 && ns<totalNumberOfSurfaces );

      RealArray & cd =  cs[ns].csData; 
      real *& pcd = cd.Array_Descriptor.Array_View_Pointer2;  // * note reference * (in case cd is resized)
      const int cdDim0=cd.getRawDataSize(0);
      const int cdDim1=cd.getRawDataSize(1);

      if( plotOnThisProcessor )
      {
        glPushName(cs[ns].getGlobalID()); // assign a name for picking

	printF("contour3dOpt: plot iso surface %i, surfaceColourType=%i, colourIndex=%i\n",
	       ns,(int)cs[ns].surfaceColourType,cs[ns].colourIndex);
	
	if( cs[ns].surfaceColourType==ContourSurface::colourSurfaceByIndex )
	{
          // set colour that was chosen by name (e.g. picked)
	  gi.setColour( getXColour(cs[ns].colourIndex) );
	}
	else
	{
	  // default: colour iso-surface by the value in the colour table
	  gi.setColourFromTable( (CD(3,0,0)-uMin)*deltaUInverse,psp ); // *** this colour is always the same ****
	  // assert( fabs(CD(0,nv,p))+fabs(CD(1,nv,p))+fabs(CD(2,nv,p)) > 0. );
	}
	

	for( int p=0; p<numberOfPolygonsPerSurface(ns); p++ )
	{
	  glBegin(GL_POLYGON); 
	  const int nv=numberOfVerticesPerSurface(ns,p);
	  for( int j=0; j<nv; j++ )
	  {
	    glNormal3(CD(0,nv,p),CD(1,nv,p),CD(2,nv,p));
	    // *wdh* 090425 glVertex3(CD(0,j,p),CD(1,j,p),CD(2,j,p));
	    glVertex3(XSCALE(CD(0,j,p)),YSCALE(CD(1,j,p)),ZSCALE(CD(2,j,p)));

	  }
	  glEnd();
	}
      }
    }
  }
  if( showTimings ) printf("contour3d: iso time=%8.3e\n",getCPU()-time1);

  if( plotOnThisProcessor )
  {
    //   glDisable(GL_POLYGON_OFFSET_FILL);
    glLineWidth(psp.size(GraphicsParameters::lineWidth)*
		gi.getLineWidthScaleFactor());  // reset

    glEndList(); // the lit list ends here
  }
  
}

