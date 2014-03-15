#include "Overture.h"
#include "CompositeGrid.h"
#include "ReductionMapping.h"



#define loop3d(i1,i2,i3,I1,I2,I3) \
    I3Bound=I3.getBound(); I2Bound=I2.getBound(); I1Bound=I1.getBound();\
    for( i3=I3.getBase(); i3<=I3Bound; i3++ )\
    for( i2=I2.getBase(); i2<=I2Bound; i2++ )\
    for( i1=I1.getBase(); i1<=I1Bound; i1++ )

#define FOR_3(i1,i2,i3,I1,I2,I3) \
    I3Bound=I3.getBound(); I2Bound=I2.getBound(); I1Bound=I1.getBound();\
    for( i3=I3.getBase(); i3<=I3Bound; i3++ )\
    for( i2=I2.getBase(); i2<=I2Bound; i2++ )\
    for( i1=I1.getBase(); i1<=I1Bound; i1++ )


int 
boundaryDistance(CompositeGrid & cg, realCompositeGridFunction & d, const IntegerArray & wall )
// ========================================================================
// /Description:
//   Determine the distance from each point to the nearest solid wall
//   We compute the distance for all points in the vertex array.
//
// /wall(i,0:2) (input) : wall(i,0:2)=(grid,side,axis) for a wall
// ========================================================================
{

  const int numberOfDimensions=cg.numberOfDimensions();
  
  const int numberOfWalls=wall.getLength(0);
  if( numberOfWalls==0)
  {
    d=sqrt(REAL_MAX*.01);
    return 0;
  }

  real time0=getCPU();
  
  
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg=cg[grid];
    if( !mg.isRectangular() )
      mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
  }
  

  d=REAL_MAX;  // initialize distance

  real dx[3],xab[2][3];
  real xv[3];
  int i0a,i1a,i2a;
  real xa,ya,za,dx0,dy0,dz0;

  real *vertexp=NULL;
  int vertexDim0,vertexDim1,vertexDim2;
  
  // We will build a mapping for each wall
  ReductionMapping *wallMappings = new ReductionMapping[numberOfWalls];

  Index I1,I2,I3,all;
  int I1Bound,I2Bound,I3Bound;
  
  int totalGridPoints=0;

  // First Step:
  //   A point on a given grid is most likely nearest to a wall on the same grid.
  //   For each wall -- mark points on the same grid
  for( int w=0; w<numberOfWalls; w++ )
  {
    const int gridw=wall(w,0);
    const int sidew=wall(w,1);
    const int axisw=wall(w,2);

    MappedGrid & mg=cg[gridw];
    Mapping & map = mg.mapping().getMapping();
    const IntegerArray & dimension = mg.dimension();

    realArray & dw = d[gridw];
    real * dwp = dw.Array_Descriptor.Array_View_Pointer2;
    const int dwDim0=dw.getRawDataSize(0);
    const int dwDim1=dw.getRawDataSize(1);
#define DW(i0,i1,i2) dwp[i0+dwDim0*(i1+dwDim1*(i2))]

    ReductionMapping & wallMap = wallMappings[w];
    wallMap.set(map,axisw,(real)sidew);

    const realArray & vertex = mg.vertex();
    
    const bool isRectangular = mg.isRectangular();
    if( isRectangular )
    {
      mg.getRectangularGridParameters( dx, xab );

      i0a=mg.gridIndexRange(0,0);
      i1a=mg.gridIndexRange(0,1);
      i2a=mg.gridIndexRange(0,2);
    
      xa=xab[0][0], dx0=dx[0];
      ya=xab[0][1], dy0=dx[1];
      za=xab[0][2], dz0=dx[2];
    }
    else
    {
      vertexp = vertex.Array_Descriptor.Array_View_Pointer3;
      vertexDim0=vertex.getRawDataSize(0);
      vertexDim1=vertex.getRawDataSize(1);
      vertexDim2=vertex.getRawDataSize(2);
    }
    
#define VERTEX0(i0,i1,i2) (xa+dx0*(i0-i0a))
#define VERTEX1(i0,i1,i2) (ya+dy0*(i1-i1a))
#define VERTEX2(i0,i1,i2) (za+dz0*(i2-i2a))

#define VERTEX(i0,i1,i2,i3) vertexp[i0+vertexDim0*(i1+vertexDim1*(i2+vertexDim2*(i3)))]

    getIndex(dimension,I1,I2,I3);
    const int maxNum = I1.getLength()*I2.getLength()*I3.getLength();
    totalGridPoints+=maxNum;

    intArray ia(maxNum,3);
    realArray xx(maxNum,3);

    int *iap = ia.Array_Descriptor.Array_View_Pointer1;
    const int iaDim0=ia.getRawDataSize(0);
#define IA(i0,i1) iap[i0+iaDim0*(i1)]
    real *xxp = xx.Array_Descriptor.Array_View_Pointer1;
    const int xxDim0=xx.getRawDataSize(0);
#define XX(i0,i1) xxp[i0+xxDim0*(i1)]

    const RealArray & bb = wallMap.getBoundingBox();
    const real *bbp = bb.Array_Descriptor.Array_View_Pointer1;
    const int bbDim0=bb.getRawDataSize(0);
#define BB(i0,i1) bbp[i0+bbDim0*(i1)]


    // make a list of points to check
    int i1,i2,i3,dir;
    int i=0;
    FOR_3(i1,i2,i3,I1,I2,I3)
    {
      if( isRectangular )
      {
        xv[0]=VERTEX0(i1,i2,i3);
        xv[1]=VERTEX1(i1,i2,i3);
        xv[2]=VERTEX2(i1,i2,i3);
      }
      else
      {
	for( dir=0; dir<numberOfDimensions; dir++ )
	  xv[dir]=VERTEX(i1,i2,i3,dir);
      }

      // compute min-dist to the bounding box
      real distance=0.;
      for( dir=0; dir<numberOfDimensions; dir++ )
      {
	real dist= max(max(BB(Start,dir)-xv[dir],xv[dir]-BB(End,dir)),0.);
	distance+=SQR(dist);
      }

      if( distance < DW(i1,i2,i3) )
      {
	IA(i,0)=i1; IA(i,1)=i2;  IA(i,2)=i3;
	XX(i,0)=xv[0]; XX(i,1)=xv[1]; XX(i,2)=xv[2];
        i++;
      }
    }
    
    const int num=i;
    if( num==0 ) continue;

    Range R=num;
    realArray ra(num,numberOfDimensions-1);
    ra=-1;  // could do better -- project to index space on boundary
    wallMap.inverseMap(xx(R,all),ra);

    realArray xw(num,numberOfDimensions);
    wallMap.map(ra,xw);  // xw : closest point on the wall

    for( i=0; i<num; i++ )
    {
      real dist;
      if( numberOfDimensions==2 )
        dist= SQR(XX(i,0)-xw(i,0))+SQR(XX(i,1)-xw(i,1));
      else
        dist= SQR(XX(i,0)-xw(i,0))+SQR(XX(i,1)-xw(i,1))+SQR(XX(i,2)-xw(i,2));
      
      if( dist < DW(IA(i,0),IA(i,1),IA(i,2)) )
      {
	DW(IA(i,0),IA(i,1),IA(i,2))=dist;   // holds square of the L2 distance until later
      }
    }
  }
  

  // Now loop through walls and check the distance to points that are on other grids from the wall

  // Could use the bounding box tree from the approximate global inverse to do a more
  // careful distance check!
  for( int w=0; w<numberOfWalls; w++ )
  {
    const int gridw=wall(w,0);
    const int sidew=wall(w,1);
    const int axisw=wall(w,2);

    MappedGrid & mgw =cg[gridw];
//      Mapping & map = mg.mapping().getMapping();
//      const IntegerArray & dimension = mg.dimension();

    ReductionMapping & wallMap = wallMappings[w];

    const RealArray & bb = wallMap.getBoundingBox();
    const real *bbp = bb.Array_Descriptor.Array_View_Pointer1;
    const int bbDim0=bb.getRawDataSize(0);


    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      if( grid==gridw )   // this case was done above.
        continue; 

      MappedGrid & mg=cg[grid];
      realArray & dw = d[grid];
      real * dwp = dw.Array_Descriptor.Array_View_Pointer2;
      const int dwDim0=dw.getRawDataSize(0);
      const int dwDim1=dw.getRawDataSize(1);
      
      const realArray & vertex = mg.vertex();

      const bool isRectangular = mg.isRectangular();
      if( isRectangular )
      {
	mg.getRectangularGridParameters( dx, xab );

	i0a=mg.gridIndexRange(0,0);
	i1a=mg.gridIndexRange(0,1);
	i2a=mg.gridIndexRange(0,2);
    
	xa=xab[0][0], dx0=dx[0];
	ya=xab[0][1], dy0=dx[1];
	za=xab[0][2], dz0=dx[2];
      }
      else
      {
	vertexp = vertex.Array_Descriptor.Array_View_Pointer3;
	vertexDim0=vertex.getRawDataSize(0);
	vertexDim1=vertex.getRawDataSize(1);
	vertexDim2=vertex.getRawDataSize(2);
      }
    
      getIndex(mg.dimension(),I1,I2,I3);
      const int maxNum = I1.getLength()*I2.getLength()*I3.getLength();
      intArray ia(maxNum,3);
      realArray xx(maxNum,3);

      int *iap = ia.Array_Descriptor.Array_View_Pointer1;
      const int iaDim0=ia.getRawDataSize(0);
      real *xxp = xx.Array_Descriptor.Array_View_Pointer1;
      const int xxDim0=xx.getRawDataSize(0);

      // make a list of points to check
      int i1,i2,i3,dir;
      int i=0;
      FOR_3(i1,i2,i3,I1,I2,I3)
      {
	if( isRectangular )
	{
	  xv[0]=VERTEX0(i1,i2,i3);
	  xv[1]=VERTEX1(i1,i2,i3);
	  xv[2]=VERTEX2(i1,i2,i3);
	}
	else
	{
	  for( dir=0; dir<numberOfDimensions; dir++ )
	    xv[dir]=vertex(i1,i2,i3,dir);
	}

	// compute min-dist to the bounding box
	real distance=0.;
	for( dir=0; dir<numberOfDimensions; dir++ )
	{
	  real dist= max(max(BB(Start,dir)-xv[dir],xv[dir]-BB(End,dir)),0.);
	  distance+=SQR(dist);
	}

	if( distance < DW(i1,i2,i3) )
	{
	  IA(i,0)=i1; IA(i,1)=i2;  IA(i,2)=i3;
	  XX(i,0)=xv[0]; XX(i,1)=xv[1]; XX(i,2)=xv[2];
	  i++;
	}
      }
    
      const int num=i;
      if( num==0 ) continue;

      Range R=num;
      realArray ra(num,numberOfDimensions-1);
      ra=-1;  
      wallMap.inverseMap(xx(R,all),ra);

      realArray xw(num,numberOfDimensions);
      wallMap.map(ra,xw);  // xw : closest point on the wall

      for( i=0; i<num; i++ )
      {
	real dist;
	if( numberOfDimensions==2 )
	  dist= SQR(XX(i,0)-xw(i,0))+SQR(XX(i,1)-xw(i,1));
	else
	  dist= SQR(XX(i,0)-xw(i,0))+SQR(XX(i,1)-xw(i,1))+SQR(XX(i,2)-xw(i,2));
      
	if( dist < DW(IA(i,0),IA(i,1),IA(i,2)) )
	{
	  DW(IA(i,0),IA(i,1),IA(i,2))=dist;   // holds square of the L2 distance until later
	}
      }
    }
  }
  


  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    realArray & dw = d[grid];
    dw=sqrt(dw);
  }

  real time=getCPU()-time0;
  printf(" boundaryDistance: time to compute the boundary distance for %i grid points was %8.2e\n",
	 totalGridPoints,time);
  
  return 0;
}
