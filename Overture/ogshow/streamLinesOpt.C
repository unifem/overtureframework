#include "Overture.h"
#include "GL_GraphicsInterface.h"
#include "ParallelUtility.h"
#include "PlotIt.h"

extern int colourTable[256][3];

#define CGNRST EXTERN_C_NAME(cgnrst)
#define CGNRSC EXTERN_C_NAME(cgnrsc)

extern "C"
{
  void CGNRST(const int & ndra, const int & ndrb, const int & ndsa, const int & ndsb, int & mrsab, const real & xy,
              real & x, real & y, int & ip, int & jp, real & distmn );
  void CGNRSC(const int & ndra, const int & ndrb, const int & ndsa, const int & ndsb, int & mrsab, const real & xy,
              real & x, real & y, int & ip, int & jp, real & distmn );
}


// ====================================================================================================
// 
//           Composite Grid Interpolation Routine
//
//  Given some points in space, determine the values of a grid function uv. If interpolation
//  is not possible then extrapolate from the nearest grid point.
//
//  Input-
//   numberOfPointsToInterpolate -
//   componentsToInterpolate(i0a:i0b)  - these int values define which components values to interpolate
//       Thus we interpolate values of component number zero that are equal to 
//          componentsToInterpolate(i) for i=i0a,...,i0b
//   positionToInterpolate(0:2,0,numberOfPointsToInterpolate-1) : (x,y,z) positions
//   indexGuess(0:3,numberOfPointsToInterpolate-1) : (i1,i2,i3,grid) values for initial guess for searches
//   gc - GridCollection
//   uv - realGridCollectionFunction
//  Output -
//   uInterpolated(0:numberOfPointsToInterpolate-1,i1a,i1b) - 
//
//  Return value:
//   0 = success
//   1 = error, unable to interpolate
//  -1 = could not interpolate, but could extrapolate -- extrapolation was performed
//       from the nearest grid point.
//
// Who to blame: Bill Henshaw
// =====================================================================================================
int
xInterpolateOpt(int numberOfComponents,
		const int *componentsToInterpolate_,
		const real *positionToInterpolate_,
		int *indexGuess_,
		real *uInterpolated_, 
		const realGridCollectionFunction & u,
		const GridCollection & gc,
		const int intopt)
{

#define componentsToInterpolate(i1) componentsToInterpolate_[i1]
#define positionToInterpolate(i1) positionToInterpolate_[i1]
#define indexGuess(i1) indexGuess_[i1]
#define uInterpolated(i1) uInterpolated_[i1]

  bool debug=FALSE;
  const real epsi=1.e-3;

  bool extrap;
  real distmn;
  int jac=(intopt/8) % 2;

  const int numberOfDimensions=gc.numberOfDimensions();
  const int numberOfGrids =gc.numberOfComponentGrids();
  
  int mrsab_[6];
#define mrsab(axis,side) mrsab_[axis+numberOfDimensions*(side)]

  int returnValue=1;  // 0=ok, 1=error, -1=extrapolate

  int grid=min(numberOfGrids-1,max(0,indexGuess(3)));  // here is the first grid we check

  int ip=indexGuess(0);
  int jp=indexGuess(1);
  real x=positionToInterpolate(0);
  real y=positionToInterpolate(1);

  real dist=-1.;
  // Loop through the grids until we find a point we can interpolate from ...
  for( int gridn=0; gridn<numberOfGrids; gridn++ )
  {
    if( gridn>0 ) 
      grid = (grid+1) % numberOfGrids;  // here is the next grid to try;

//    const int *dimensionp= gc[grid].dimension().Array_Descriptor.Array_View_Pointer1;
    const int *indexRangep = gc[grid].indexRange().Array_Descriptor.Array_View_Pointer1;
    const int *extendedIndexRangep = gc[grid].extendedIndexRange().Array_Descriptor.Array_View_Pointer1;
    const int *gridIndexRangep= gc[grid].gridIndexRange().Array_Descriptor.Array_View_Pointer1;

#define dimension(i0,i1) dimensionp[i0+2*(i1)]   
#define indexRange(i0,i1) indexRangep[i0+2*(i1)]   
#define extendedIndexRange(i0,i1) extendedIndexRangep[i0+2*(i1)]   
#define gridIndexRange(i0,i1) gridIndexRangep[i0+2*(i1)]   

#define NRM(axis)  ( indexRange(End,axis)-indexRange(Start,axis) )
#define MODR(i,axis)  ( \
  ( (i-indexRange(Start,axis)+NRM(axis)) % NRM(axis)) \
      +indexRange(Start,axis) \
                           )

    int i3=gridIndexRange(Start,axis3);
      
    const RealArray & center = gc[grid].center().getLocalArray();
    const IntegerArray & mask = gc[grid].mask().getLocalArray();
    const RealArray & centerDerivative = gc[grid].centerDerivative().getLocalArray();
    // the inverseCenterDerivative is normall not needed (usually jac==1)
    const RealArray & inverseCenterDerivative = jac==0 ? gc[grid].inverseCenterDerivative().getLocalArray() :
      Overture::nullRealArray();

    const int * maskp = mask.Array_Descriptor.Array_View_Pointer1;
    const int maskDim0=mask.getRawDataSize(0);
#define MASK(i0,i1) maskp[i0+maskDim0*(i1)]

    const real *centerp = center.Array_Descriptor.Array_View_Pointer3;
    const int centerDim0=center.getRawDataSize(0);
    const int centerDim1=center.getRawDataSize(1);
    const int centerDim2=center.getRawDataSize(2);
#define CENTER(i0,i1,i2,m) centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(m)))]


    const real *centerDerivativep = centerDerivative.Array_Descriptor.Array_View_Pointer3;
    const int centerDerivativeDim0=centerDerivative.getRawDataSize(0);
    const int centerDerivativeDim1=centerDerivative.getRawDataSize(1);
    const int centerDerivativeDim2=centerDerivative.getRawDataSize(2);
#define XR(i0,i1,i2,m,n) centerDerivativep[i0+centerDerivativeDim0*(i1+centerDerivativeDim1*(i2+centerDerivativeDim2*(m+numberOfDimensions*(n))))]

    const real *inverseCenterDerivativep = inverseCenterDerivative.Array_Descriptor.Array_View_Pointer3;
    const int inverseCenterDerivativeDim0=inverseCenterDerivative.getRawDataSize(0);
    const int inverseCenterDerivativeDim1=inverseCenterDerivative.getRawDataSize(1);
    const int inverseCenterDerivativeDim2=inverseCenterDerivative.getRawDataSize(2);
#define RX(i0,i1,i2,m,n) inverseCenterDerivativep[i0+inverseCenterDerivativeDim0*(i1+inverseCenterDerivativeDim1*(i2+inverseCenterDerivativeDim2*(m+numberOfDimensions*(n))))]



    const realArray & uu = u[grid];
    
    //    ....find the nearest point, (ip,jp), to (x,y) on grid k
    for( int axis=0; axis<numberOfDimensions; axis++ )
    {
      // *wdh* 081008
      //       mrsab(axis,Start)=indexRange(Start,axis);     
      //       mrsab(axis,End  )=indexRange(End  ,axis);     
      mrsab(axis,Start)=extendedIndexRange(Start,axis);     
      mrsab(axis,End  )=extendedIndexRange(End  ,axis);     
    }
    CGNRST(center.getBase(0),center.getBound(0),center.getBase(1),center.getBound(1),
	   mrsab(0,0),center(center.getBase(0),center.getBase(1),center.getBase(2),0),
	   x,y,ip,jp,distmn );
    if( debug )
    {
      printf("xInterpolate: CGNRST: x=%e, y=%e, ip=%i, jp=%i \n",x,y,ip,jp);
    }

    if( MASK(ip,jp)==0 )  
      continue;  //  ....Unable to interpolate, try another grid
    //
    //.............Iterpolate from the 4 points
    //               (ip ,jp1)   (ip1,jp1)
    //               (ip ,jp )   (ip1,jp )
    //
    real dr,ds,dra,dsa,dx,dy;
    if((bool)gc[grid].isAllVertexCentered())
    {
      dx=x-CENTER(ip,jp,i3,axis1);
      dy=y-CENTER(ip,jp,i3,axis2);
      if( jac==0 )
      { //...use rsxy array
	dr=RX(ip,jp,i3,axis1,axis1)*dx+RX(ip,jp,i3,axis1,axis2)*dy;
	ds=RX(ip,jp,i3,axis2,axis1)*dx+RX(ip,jp,i3,axis2,axis2)*dy;
      }	  
      else
      {// ...rsxy array is really xyrs
	real deti=XR(ip,jp,i3,axis1,axis1)*XR(ip,jp,i3,axis2,axis2)-
         	  XR(ip,jp,i3,axis1,axis2)*XR(ip,jp,i3,axis2,axis1);
	if( deti==0. )
	  cout << "xInterpolate:ERROR: det(x.r)==0 ! \n";
	deti=1./deti;
	    
	dr=( XR(ip,jp,i3,axis2,axis2)*dx-XR(ip,jp,i3,axis1,axis2)*dy)*deti;
	ds=(-XR(ip,jp,i3,axis2,axis1)*dx+XR(ip,jp,i3,axis1,axis1)*dy)*deti;
      }
    }
    else 
    {
      dx=x-CENTER(ip,jp,i3,axis1);
      dy=y-CENTER(ip,jp,i3,axis2);
      if(jac==0)
      {// ...use rsxy array
	dr=RX(ip,jp,i3,axis1,axis1)*dx+RX(ip,jp,i3,axis1,axis2)*dy;
	ds=RX(ip,jp,i3,axis2,axis1)*dx+RX(ip,jp,i3,axis2,axis2)*dy;
      }
      else
      {//       ...rsxyc array is really xyrs
	real deti=XR(ip,jp,i3,axis1,axis1)*XR(ip,jp,i3,axis2,axis2)-
  	          XR(ip,jp,i3,axis1,axis2)*XR(ip,jp,i3,axis2,axis1);
	if( deti==0. )
	{
	  cout << "xInterpolate:ERROR: det(x.r)==0 ! \n";
	  printf(" centerDerivative=(%e,%e,%e,%e)\n",XR(ip,jp,i3,axis1,axis1),
		 XR(ip,jp,i3,axis2,axis2), XR(ip,jp,i3,axis1,axis2),XR(ip,jp,i3,axis2,axis1));
	}
	deti=1./deti;
	dr=( XR(ip,jp,i3,axis2,axis2)*dx-XR(ip,jp,i3,axis1,axis2)*dy)*deti;
	ds=(-XR(ip,jp,i3,axis2,axis1)*dx+XR(ip,jp,i3,axis1,axis1)*dy)*deti;
      }
    }

    dr*=(gridIndexRange(End,axis1)-gridIndexRange(Start,axis1));
    ds*=(gridIndexRange(End,axis2)-gridIndexRange(Start,axis2));
    dra=min(fabs(dr),1.);
    dsa=min(fabs(ds),1.);
      
    //...........only use 4 points if dra bigger than epsilon, this lets us
    //           interpolate near interpolation boundaries
    int ip1=ip;
    if( dra>epsi )
      ip1+= dr>0. ? 1 : -1;
    int jp1=jp;
    if( dsa>epsi )
      jp1+= ds>0. ? 1 : -1;
    // ........periodic wrap
    if( (bool)gc[grid].isPeriodic(axis1) )
      if( fabs(dr)<1.5 )   // don't periodic wrap if we are a long way away
	ip1=MODR(ip1,axis1);
    if( (bool)gc[grid].isPeriodic(axis2) )
      if( fabs(ds)<1.5 )
	jp1=MODR(jp1,axis2);
      
    //.............Unable to interpolate if outside the current grid, but
    //             extrapolate (to zero order) if this is the closest point
    //             so far
    // if(ip1<indexRange(Start,axis1) || ip1>indexRange(End,axis1) ||
    //   jp1<indexRange(Start,axis2) || jp1>indexRange(End,axis2) )
    // *wdh* 08108 - use extended index range (we can use interpolation points)
    if(ip1<extendedIndexRange(Start,axis1) || ip1>extendedIndexRange(End,axis1) ||
       jp1<extendedIndexRange(Start,axis2) || jp1>extendedIndexRange(End,axis2) )
    {
      extrap=TRUE;
      if( distmn<dist || dist<0. )
      {
	dist=distmn;
	if(ip1<extendedIndexRange(Start,axis1) || ip1>extendedIndexRange(End,axis1))
	  ip1=ip;
	if(jp1<extendedIndexRange(Start,axis2) || jp1>extendedIndexRange(End,axis2))
	  jp1=jp;
	if( MASK(ip1,jp1)==0 ) 
	{  // *wdh* 08108
	  ip1=ip;
	  jp1=jp;
	}
      }
      else
	continue;    //  ....Unable to interpolate, try another grid
    }
    else
      extrap=FALSE;

    //  ... (check to see whether all marked interpolation points are valid)...
    if(MASK(ip ,jp)==0 || MASK(ip ,jp1)==0  ||
       MASK(ip1,jp)==0 || MASK(ip1,jp1)==0 )
      continue ;  //       ....Unable to interpolate, try another grid

    const real *uup = uu.Array_Descriptor.Array_View_Pointer3;
    const int uuDim0=uu.getRawDataSize(0);
    const int uuDim1=uu.getRawDataSize(1);
    const int uuDim2=uu.getRawDataSize(2);
#define UU(i0,i1,i2,c) uup[(i0)*d0+(i1)*d1+(i2)*d2+(c)*dc]
    int d0,d1,d2,dc;
    if( u.positionOfComponent(0)==3 )
    {
      d0=1; d1=uuDim0; d2=d1*uuDim1; dc=d2*uuDim2;
    }
    else if( u.positionOfComponent(0)==0 )
    {
      dc=1; d0=uuDim0; d1=d0*uuDim1; d2=d1*uuDim2; 
    }
    else
    {
      printf("xInterpolate:ERROR: not implemented for u.positionOfComponent(0)=%i\n",u.positionOfComponent(0));
      throw "error";
    }

    // ...........Bi-Linear Interpolation:
    for( int n0=0; n0<numberOfComponents; n0++ )
    {
      int c0=componentsToInterpolate(n0);
      uInterpolated(n0)=
	(1.-dsa)*((1.-dra)*UU(ip,jp ,i3,c0)+dra*UU(ip1,jp ,i3,c0))
	 +  dsa *((1.-dra)*UU(ip,jp1,i3,c0)+dra*UU(ip1,jp1,i3,c0));
    }
    // return the values used:
    indexGuess(0)=ip;
    indexGuess(1)=jp;
    indexGuess(3)=grid;   //  !extrap ? k: -k;
      
    if( extrap )
    {
      returnValue=-1;
    }
    else
    {
      returnValue=0;
      break;   // point has been successfully interpolated, try next point
    }
  }

  return returnValue;
}

#undef NRM
#undef MODR
#undef RX
#undef XR
#undef componentsToInterpolate
#undef positionToInterpolate
#undef indexGuess
#undef uInterpolated
#undef MASK
#undef dimension
#undef indexRange
#undef extendedIndexRange
#undef gridIndexRange


#define FOR_3(i1,i2,i3,I1,I2,I3) \
  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  \
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  \
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  \

#define ForBoundary(side,axis)   for( axis=0; axis<gc.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


//! Determine the maximum speed for stream lines
void
computeMaximumSpeed( const realGridCollectionFunction & uv, GridCollection & gc,
                     int numberOfGhostLinesToPlot, int uComponent, int vComponent,
                     real xa, real xb, real ya, real yb, real & uMin, real & uMax )
{
  const int numberOfGrids = gc.numberOfComponentGrids();
  uMax=0.;
  uMin=REAL_MAX;
  for( int grid=0; grid<numberOfGrids; grid++)
  {  
    const RealArray & coord = (int)gc[grid].isAllVertexCentered() ? 
      gc[grid].vertex().getLocalArray() : gc[grid].center().getLocalArray();
    const RealArray & u = uv[grid].getLocalArray();
    const IntegerArray & mask = gc[grid].mask().getLocalArray();
    Index I1,I2,I3;
    getIndex(gc[grid].gridIndexRange(),I1,I2,I3,numberOfGhostLinesToPlot);

    const int * maskp = mask.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=mask.getRawDataSize(0);
#define MASK(i0,i1) maskp[i0+maskDim0*(i1)]

    const real * coordp = coord.Array_Descriptor.Array_View_Pointer3;
    const int coordDim0=coord.getRawDataSize(0);
    const int coordDim1=coord.getRawDataSize(1);
    const int coordDim2=coord.getRawDataSize(2);
#define COORD(i0,i1,i2,i3) coordp[i0+coordDim0*(i1+coordDim1*(i2+coordDim2*(i3)))]

    const real * up = u.Array_Descriptor.Array_View_Pointer3;
    const int uDim0=u.getRawDataSize(0);
    const int uDim1=u.getRawDataSize(1);
    const int uDim2=u.getRawDataSize(2);
#define U(i0,i1,i2,c) up[(i0)*d0+(i1)*d1+(i2)*d2+(c)*dc]
    int d0,d1,d2,dc;
    if( uv.positionOfComponent(0)==3 )
    {
      d0=1; d1=uDim0; d2=d1*uDim1; dc=d2*uDim2;
    }
    else if( uv.positionOfComponent(0)==0 )
    {
      dc=1; d0=uDim0; d1=d0*uDim1; d2=d1*uDim2; 
    }
    else
    {
      printf("computeMaximumSpeed:ERROR: not implemented for u.positionOfComponent(0)=%i\n",uv.positionOfComponent(0));
      throw "error";
    }

    if( gc[grid].getGridType()!=GenericGrid::unstructuredGrid )  // ** fix this *** no mask 
    {
      int i1,i2,i3;
      FOR_3(i1,i2,i3,I1,I2,I3)
      {
	if( MASK(i1,i2)!=0 && 
	    COORD(i1,i2,i3,axis1) >= xa && COORD(i1,i2,i3,axis1) <= xb &&
	    COORD(i1,i2,i3,axis2) >= ya && COORD(i1,i2,i3,axis2) <= yb )
	{
	  real speed=SQR(U(i1,i2,i3,uComponent))+SQR(U(i1,i2,i3,vComponent));
	  if( speed > uMax )
	    uMax=speed;
	  else if( speed < uMin )
	    uMin=speed;
	}
      }
	      
    }
  }
  uMax=sqrt(uMax);
  uMin=sqrt(uMin);
  
}

//#define XSCALE(x) (psp.xScaleFactor*(x-rotationCenter[0])+rotationCenter[0])
//#define YSCALE(y) (psp.yScaleFactor*(y-rotationCenter[1])+rotationCenter[1])
//#define ZSCALE(z) (psp.zScaleFactor*(z-rotationCenter[2])+rotationCenter[2])

#define XSCALE(x) (psp.xScaleFactor*(x))
#define YSCALE(y) (psp.yScaleFactor*(y))
#define ZSCALE(z) (psp.zScaleFactor*(z))

//! Draw all streamlines.
void PlotIt::
plotStreamLines(GenericGraphicsInterface &gi, const GridCollection & gc, 
		const realGridCollectionFunction & uv, 
		IntegerArray & componentsToInterpolate,
		IntegerArray & maskForStreamLines_,
		real arrowSize,
                GraphicsParameters & psp,
                real & xa, real &ya, real & xb, real &yb, real &xba, real &yba, 
                real &uMin, real &uMax, int &nrsmx,
                int & nxg, int & nyg, int & intopt )
{

  int * maskForStreamLinesp = maskForStreamLines_.Array_Descriptor.Array_View_Pointer1;
  const int maskForStreamLinesDim0=maskForStreamLines_.getRawDataSize(0);
#define maskForStreamLines(i0,i1) maskForStreamLinesp[i0+maskForStreamLinesDim0*(i1)]

  real ui[2];  // we interpolate 1 point at a time and 2 component values (u,v)
  int indexGuessp[4]={1,1,0,0};

  glLineWidth(psp.size(GraphicsParameters::streamLineWidth)*psp.size(GraphicsParameters::lineWidth)*
 		gi.getLineWidthScaleFactor());
    
  for( int i=0; i<nxg; i++ )
  {
    for( int j=0; j<nyg; j++ )
    {
      if( maskForStreamLines(i,j)==0 )  // no streamline has pass through this point
      {
	real xtp=xa+xba*(i+.5)/nxg;  // ! starting point for streamline
	real ytp=ya+yba*(j+.5)/nyg;
	// first integrate backwards in time from this spot
	real cfl=-.5;
	drawAStreamLine(gi, gc, uv, &componentsToInterpolate(0), maskForStreamLines_, arrowSize, psp,
                        ui, indexGuessp,
			xa, ya,  xb, yb, xba, yba, uMin, uMax, cfl, nrsmx, nxg,  nyg, xtp, ytp,  intopt );
	// ---  Now plot the streamline in the forward direction ---
	xtp=xa+xba*(i+.5)/nxg;  
	ytp=ya+yba*(j+.5)/nyg;
	cfl= .5; 
	drawAStreamLine(gi, gc, uv, &componentsToInterpolate(0), maskForStreamLines_, arrowSize, psp,
                        ui, indexGuessp,
			xa, ya,  xb, yb, xba, yba, uMin, uMax, cfl, nrsmx, nxg,  nyg,  xtp, ytp,  intopt );

      }
    }
  }
}


//=======================================================================
//        drawAStreamLine
// 
// Integrate the streamline whose initial position is (xtp,ytp)
// Mark the cells in the array maskForStreamLines(i,j) i=1,nxg j=1,nyg
// which the streamline passes through and stop plotting the
// streamline if the streamline enters a cell with maskForStreamLines(i,j) >= 2
// Plot arrows on the streamline when the streamline enters a
// cell (i,j) satisfying mod(i,lax)=0 and mod(j,lay)=0 where
// (lax,lay) are assign below
//  Colour the contours according to the value of u**2+v**2.
//
//=======================================================================
void PlotIt:: 
drawAStreamLine(GenericGraphicsInterface &gi, const GridCollection & gc, 
		const realGridCollectionFunction & uv, 
		int *componentsToInterpolate,
		IntegerArray & maskForStreamLines_,
		real arrowSize,
                GraphicsParameters & psp,
                real *uip, int *indexGuessp,
                real & xa, real &ya, real & xb, real &yb, real &xba, real &yba, 
                real &uMin, real &uMax, real &cfl, int &nrsmx,
                int & nxg, int & nyg, real &xtp, real &ytp, int & intopt )
{
  int lax=5, lay=5; 

  real dxmx=max(xba,yba);

  real uvfact=1./(uMax-uMin);     // normalization factor for colour table

  int nt=nrsmx*2;   // maximum number of time steps

  real t=0.;
  real cfla=fabs(cfl)*dxmx/nrsmx;    // *** could use dxmx and nrsmx from a particular grid?? ****
  real dtmx=cfla*2.;
  real dtMax = 100.*cfla/uvfact;
  
  real xgf=nxg/xba, ygf=nyg/yba;

  real x=xtp, y=ytp, xi=x, yi=y;
  bool debug=FALSE;
  
//  int numberOfPointsToInterpolate=1;

  real xvp[2];
#define XV(i1) xvp[i1]
  XV(0)=x; XV(1)=y;

#define indexGuess(i) indexGuessp[i]
#define UI(i1) uip[i1]
  
    int * maskForStreamLinesp = maskForStreamLines_.Array_Descriptor.Array_View_Pointer1;
    const int maskForStreamLinesDim0=maskForStreamLines_.getRawDataSize(0);
#define maskForStreamLines(i0,i1) maskForStreamLinesp[i0+maskForStreamLinesDim0*(i1)]
  
//   int notOk = xInterpolate(numberOfPointsToInterpolate,componentsToInterpolate,xv,indexGuess,
//                            ui,uv,gc,intopt); 

  int notOk = xInterpolateOpt(2,componentsToInterpolate,xvp,indexGuessp,uip,uv,gc,intopt );
  // starting guess for next point is the start position of this point:
  const int indexGuess0=indexGuess(0),indexGuess1=indexGuess(1),indexGuess3=indexGuess(3); // save


//  const real aspectRatio=1.;
  
  GL_GraphicsInterface & gigl = (GL_GraphicsInterface &)gi;
  RealArray & gb = gigl.globalBound[0];
  real *rotationCenter = gigl.rotationCenter[0];

  bool globalBoundSet = gb(0,0)< 1.e100;
  const real aspectRatio = gi.getKeepAspectRatio() || !globalBoundSet ? 1. : (gb(1,1)-gb(0,1))/max(REAL_MIN,gb(1,0)-gb(0,0));
   //  printF("drawAStreamLine: aspectRatio=%6.3f globalBoundSet=%i\n",aspectRatio,(int)globalBoundSet);

  const bool glLines=false ; // line strip is a bit faster. true;
  const real epsU = psp.streamLineStoppingTolerance*uMax;   // stop streamlines when |u|+|v| < epsU )
  const real size=arrowSize*min(1.,40./max(nxg,nyg));   //  size for arrows
  
  if( notOk==0 ) // This means we can interpolate
  {
    if( debug) printf("***Start a line x=%e, y=%e \n",x,y);
    
//     glLineWidth(psp.size(GraphicsParameters::streamLineWidth)*psp.size(GraphicsParameters::lineWidth)*
// 		gi.getLineWidthScaleFactor());
    if( glLines )
      glBegin(GL_LINES); 
    else
      glBegin(GL_LINE_STRIP);
    
    gi.setColourFromTable( (sqrt( SQR(UI(0))+SQR(UI(1)) )-uMin)*uvfact,psp);
    glVertex2(XSCALE(x),YSCALE(y));

    int ixg0=-1, iyg0=-1, ixg1=int((x-xa)*xgf), iyg1=int((y-ya)*ygf);
    real dpath=0.;
    
    int index=-1, index2;  // for colour table
    // ...........Take time steps it=2,3,...,nt  (t=dt,2dt,...,(nt-1)*dt)
    for( int it=2; it<=nt; it++ )
    {
      xi=x;   yi=y;
      real uiabs=fabs(UI(0))+fabs(UI(1));
      if( uiabs < epsU )  // flow is too slow here to move anywhere
      {
        // if( it==2 ) printf("******** Exiting, flow too slow, it=%i *************\n",it);
        break;
      }
      
      // real dt= min(dtmx,cfla)/uiabs; // *wdh* 080131
      real dt= cfla/uiabs;
      dt= cfl > 0. ? dt : -dt;
      t+=dt;
      real xs=x+dt*UI(0);
      real ys=y+dt*UI(1);
      if( xs<xa || xs>xb || ys<ya || ys>yb )
      {
        if( debug ) printf("**Line leaves region xs=%e, ys=%e, dt=%e \n",xs,ys,dt);
        break;  //     ....outside plotting bounds
      }
      real uOld=UI(0), vOld=UI(1);
      
      //    interpolate velocity (UI(0),UI(1)) at (xs,ys)

      XV(0)=xs; XV(1)=ys;
      // notOk = xInterpolate(numberOfPointsToInterpolate,componentsToInterpolate,xv,indexGuess,
      //                   ui,uv,gc,intopt); 
      notOk = xInterpolateOpt(2,componentsToInterpolate,xvp,indexGuessp,uip,uv,gc,intopt );
      
      if( notOk!=0 ) // this means we cannot interpolate
      {
        if( debug ) printf("**Line ends (notOk) xs=%e, ys=%e, dt=%e \n",xs,ys,dt);
        break;   //  ....unable to interpolate
      }
      
      // trapezoidal rule
      uiabs=max(fabs(UI(0))+fabs(UI(1)),epsU);
      // *wdh* 080131 : do not recompute dt here -- but what if uiabs increases a lot ?
      // dt=min(dtmx,cfla)/uiabs; // *wdh* 080131 

      // dt = cfla/uiabs;  
      dt = min( fabs(dt), cfla/uiabs);  

      dt= cfl > 0. ? dt : -dt;
//       x=.5*(xs + x+dt*UI(0));
//       y=.5*(ys + y+dt*UI(1));
      x=x + .5*dt*(uOld+UI(0));
      y=y + .5*dt*(vOld+UI(1));
      
      if( debug )
        printf(" x=%e, y=%e, ui(0,0)=%e ui(0,1)=%e, dt=%e, cfl=%4.1f \n",x,y,UI(0),UI(1),dt,cfl);

      real uValue=(sqrt( SQR(UI(0))+SQR(UI(1)) )-uMin)*uvfact;
      if( psp.colourTable==GraphicsParameters::rainbow )
      {
	index2 = min(max(int(uValue*255+.5),0),255);
	if( index2!=index )
	{
	  index=index2;
	  glColor3f(colourTable[index][0]/255.,colourTable[index][1]/255.,colourTable[index][2]/255.);
	}
      }
      else 
        gi.setColourFromTable( uValue,psp);

      glVertex2(XSCALE(x),YSCALE(y));
      if( glLines ) glVertex2(XSCALE(x),YSCALE(y));  // what is this ?

      //  mark cell as being passed through
      int ixg=int((x-xa)*xgf);
      int iyg=int((y-ya)*ygf);
      
      if( ixg>=0 && ixg<nxg && iyg>=0 && iyg<nyg )
      {
        if( ixg!=ixg0 || iyg!=iyg0 )
	{
	  // only 2 streamlines allowed per cell
	  if( maskForStreamLines(ixg,iyg)>=2 )
	    break;
	  maskForStreamLines(ixg,iyg)++;
	  ixg0=ixg;
	  iyg0=iyg;
	  if( maskForStreamLines(ixg,iyg)==1 &&  (ixg % lax)==0 && (iyg % lay)==0 )
	  {
	    // draw arrow ...   ** it did not make a big difference in cpu to turn these arrows off 
            glEnd();   // end line so we can plot the arrow

            // real angle=atan2((double)UI(1),(double)UI(0))*180./Pi+90.;
            real xScale=XSCALE(x), yScale=YSCALE(y);
            real angle=atan2(double(UI(1)*psp.yScaleFactor),double(UI(0)*psp.xScaleFactor*aspectRatio))*180./Pi+90.;
            gi.setColour(GenericGraphicsInterface::textColour);  // arrow colour
	    gi.xLabel("V",xScale,yScale,size,0,angle,psp);

            glLineWidth(psp.size(GraphicsParameters::streamLineWidth)*
			psp.size(GraphicsParameters::lineWidth)*
			gi.getLineWidthScaleFactor());
            if( glLines )
              glBegin(GL_LINES);   // restart the line
            else
              glBegin(GL_LINE_STRIP);   // restart the line

	    real uValue=(sqrt( SQR(UI(0))+SQR(UI(1)) )-uMin)*uvfact;
	    if( psp.colourTable==GraphicsParameters::rainbow )
	    {
	      index = min(max(int(uValue*255+.5),0),255);
	      glColor3f(colourTable[index][0]/255.,colourTable[index][1]/255.,colourTable[index][2]/255.);
	    }
	    else 
	      gi.setColourFromTable( uValue,psp);

            glVertex2(xScale,yScale);
	  }
	}
      }
      //  try and check for closed loops, since we allow for 2 lines per cell we want
      // to prevent drawing a closed loop twice
      dpath+=fabs(x-xi)+fabs(y-yi);
      if( it>25 && ixg==ixg1 && iyg==iyg1 && dpath > 0.05*dxmx )
        break;
    }
    glEnd();
  }
  
  xtp=xi; 
  ytp=yi;

  // starting guess for next point is the start position of this point:
  indexGuess(0)=indexGuess0; indexGuess(1)=indexGuess1; indexGuess(3)=indexGuess3;   

}


