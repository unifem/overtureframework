// optimised C version

#include "DataPointMapping.h"
#include "display.h"
#include "ParallelUtility.h"

// static int useNewAPP = FALSE;


// *************************************
//  tri-linear interpolant: 
//    INT_2D means the domain dimension is 2
// *************************************
#define INT_1D_ORDER_2(dr,x111,x211)  \
      ( (1.-(dr))*(x111)+(dr)*(x211) )

#define INT_1D_ORDER_2_R(dr,x111,x211)  \
      ( delta[0]*( (x211)-(x111) ) )

#define INT_2D_ORDER_2(dr,ds,x111,x211,x121,x221)  \
      ( (1.-(ds))*((1.-(dr))*(x111)+(dr)*(x211))+(ds)*((1.-(dr))*(x121)+(dr)*(x221)) )

#define INT_2D_ORDER_2_R(dr,ds,x111,x211,x121,x221)  \
      ( ((1.-(ds))*( (x211)-(x111) ) +(ds)*( (x221)-(x121) ))*delta[0] )

#define INT_2D_ORDER_2_S(dr,ds,x111,x211,x121,x221)  \
      ( ((1.-(dr))*( (x121)-(x111) ) +(dr)*( (x221)-(x211) ))*delta[1] )

#define INT_3D_ORDER_2(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222)  \
  (                                                    \
     (1.-(dt))*((1.-(ds))*((1.-(dr))*(x111)+(dr)*(x211))+(ds)*((1.-(dr))*(x121)+(dr)*(x221))) \
     +    (dt)*((1.-(ds))*((1.-(dr))*(x112)+(dr)*(x212))+(ds)*((1.-(dr))*(x122)+(dr)*(x222))) )

#define INT_3D_ORDER_2_R(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222)  \
  (                                                    \
     ((1.-(dt))*((1.-(ds))*((x211)-(x111))+(ds)*((x221)-(x121))) \
   +      (dt)* ((1.-(ds))*((x212)-(x112))+(ds)*((x222)-(x122))))*delta[0] )

#define INT_3D_ORDER_2_S(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222)  \
  (  \
     ((1.-(dt))*((1.-(dr))*((x121)-(x111))+(dr)*((x221)-(x211))) \
   +      (dt) *((1.-(dr))*((x122)-(x112))+(dr)*((x222)-(x212))))*delta[1] )

#define INT_3D_ORDER_2_T(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222)  \
  (                                                    \
    ( (1.-(dr))*((1.-(ds))*((x112)-(x111))+(ds)*((x122)-(x121))) \
  +       (dr) *((1.-(ds))*((x212)-(x211))+(ds)*((x222)-(x221))) )*delta[2] )

// ***************************************
// Define jacobian entries by differencing
// ***************************************
#define XYRS2(i1,i2,i3,axis,dir) \
           (xy(i1+i1d(dir),i2+i2d(dir),i3,axis)            \
           -xy(i1-i1d(dir),i2-i2d(dir),i3,axis))*deltaByTwo[dir] 

#define XYRS3(i1,i2,i3,axis,dir) \
           (xy(i1+i1d(dir),i2+i2d(dir),i3+i3d(dir),axis)            \
           -xy(i1-i1d(dir),i2-i2d(dir),i3-i3d(dir),axis))*deltaByTwo[dir] 

// ******************
// cubic interpolant:
// ******************
#define q03(z)  (-oneSixth  *((z)-1.)*((z)-2.)*((z)-3.))
#define q13(z)  ( .5*(z)       *((z)-2.)*((z)-3.))
#define q23(z)  (-.5*(z)*((z)-1.)       *((z)-3.))
#define q33(z)  ( oneSixth*(z)*((z)-1.)*((z)-2.))
#define q03d(z) ( -oneSixth*(11.+(z)*(-12.+3.*(z))))
#define q13d(z) ( 3.+(z)*(-5.+1.5*(z)))
#define q23d(z) ( -1.5+(z)*(4.-1.5*(z)))
#define q33d(z) (oneSixth*(2.+(z)*(-6.+3.*(z))))

#define q1x(i1,i2,i3,axis)  \
      (  a0* XY(i1  ,i2  ,i3  ,axis) \
        +a1* XY(i1+1,i2  ,i3  ,axis) \
        +a2* XY(i1+2,i2  ,i3  ,axis) \
        +a3* XY(i1+3,i2  ,i3  ,axis) )
#define q2x(i1,i2,i3,axis)  \
      (  b0*q1x(i1  ,i2  ,i3  ,axis) \
        +b1*q1x(i1  ,i2+1,i3  ,axis) \
        +b2*q1x(i1  ,i2+2,i3  ,axis) \
        +b3*q1x(i1  ,i2+3,i3  ,axis) )
#define q3x(i1,i2,i3,axis)  \
      (  c0*q2x(i1  ,i2  ,i3  ,axis) \
        +c1*q2x(i1  ,i2  ,i3+1,axis) \
        +c2*q2x(i1  ,i2  ,i3+2,axis) \
        +c3*q2x(i1  ,i2  ,i3+3,axis) )
#define q1xr(i1,i2,i3,axis)   \
                   (    a0r*XY(i1  ,i2  ,i3  ,axis)   \
                       +a1r*XY(i1+1,i2  ,i3  ,axis)   \
                       +a2r*XY(i1+2,i2  ,i3  ,axis)   \
                       +a3r*XY(i1+3,i2  ,i3  ,axis) )
#define q2xr(i1,i2,i3,axis)   \
                    (   b0*q1xr(i1  ,i2  ,i3  ,axis)   \
                       +b1*q1xr(i1  ,i2+1,i3  ,axis)   \
                       +b2*q1xr(i1  ,i2+2,i3  ,axis)   \
                       +b3*q1xr(i1  ,i2+3,i3  ,axis) )
#define q3xr(i1,i2,i3,axis)   \
                     (  c0*q2xr(i1  ,i2  ,i3  ,axis)   \
                       +c1*q2xr(i1  ,i2  ,i3+1,axis)   \
                       +c2*q2xr(i1  ,i2  ,i3+2,axis)   \
                       +c3*q2xr(i1  ,i2  ,i3+3,axis)  ) 

#define q1xs(i1,i2,i3,axis)   \
                   (    a0*  XY(i1  ,i2  ,i3  ,axis)   \
                       +a1*  XY(i1+1,i2  ,i3  ,axis)   \
                       +a2*  XY(i1+2,i2  ,i3  ,axis)   \
                       +a3*  XY(i1+3,i2  ,i3  ,axis)  ) 
#define q2xs(i1,i2,i3,axis)   \
                     (  b0r*q1xs(i1  ,i2  ,i3  ,axis)   \
                       +b1r*q1xs(i1  ,i2+1,i3  ,axis)   \
                       +b2r*q1xs(i1  ,i2+2,i3  ,axis)   \
                       +b3r*q1xs(i1  ,i2+3,i3  ,axis))   
#define q3xs(i1,i2,i3,axis)   \
                     (  c0*q2xs(i1  ,i2  ,i3  ,axis)   \
                       +c1*q2xs(i1  ,i2  ,i3+1,axis)   \
                       +c2*q2xs(i1  ,i2  ,i3+2,axis)   \
                       +c3*q2xs(i1  ,i2  ,i3+3,axis) )  

#define q1xt(i1,i2,i3,axis)   \
                     (  a0*  XY(i1  ,i2  ,i3  ,axis)   \
                       +a1*  XY(i1+1,i2  ,i3  ,axis)   \
                       +a2*  XY(i1+2,i2  ,i3  ,axis)   \
                       +a3*  XY(i1+3,i2  ,i3  ,axis) )  
#define q2xt(i1,i2,i3,axis)   \
                    (   b0*q1xt(i1  ,i2  ,i3  ,axis)   \
                       +b1*q1xt(i1  ,i2+1,i3  ,axis)   \
                       +b2*q1xt(i1  ,i2+2,i3  ,axis)   \
                       +b3*q1xt(i1  ,i2+3,i3  ,axis) )  
#define q3xt(i1,i2,i3,axis)   \
                     (  c0r*q2xt(i1  ,i2  ,i3  ,axis)   \
                       +c1r*q2xt(i1  ,i2  ,i3+1,axis)   \
                       +c2r*q2xt(i1  ,i2  ,i3+2,axis)   \
                       +c3r*q2xt(i1  ,i2  ,i3+3,axis) )  


void DataPointMapping::
mapScalar(const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params, 
          int base, int bound, bool computeMap, bool computeMapDerivative )
// =========================================================================================================
//    evaluate the DPM using scalar indexing
//
// =========================================================================================================
{
  if( params.coordinateType != cartesian )
    cerr << "DataPointMapping::map - coordinateType != cartesian " << endl;

  real *rPeriodicp=NULL;
  
  const real oneSixth=1./6.;
  
  int axis,i;

  const int rDim0 = r.getRawDataSize(0);
  real *rp = r.Array_Descriptor.Array_View_Pointer1;
#define R(i,m) rp[(i)+rDim0*(m)]
  const int xDim0 = x.getRawDataSize(0);
  real *xp = x.Array_Descriptor.Array_View_Pointer1;
#define X(i,m) xp[(i)+xDim0*(m)]

  const int xrDim0 = xr.getRawDataSize(0);
  const int xrDim1 = xr.getRawDataSize(1);
  real *xrp = xr.Array_Descriptor.Array_View_Pointer2;
#define XR(i,m,n) xrp[(i)+xrDim0*(m+xrDim1*(n))]

  // NOTE: in parallel dpmScalar requires parallel ghost points to work properly 
  #ifdef USE_PPP
    assert( xy.getGhostBoundaryWidth(0)>0 );
  #endif

  OV_GET_SERIAL_ARRAY(real,xy,xyLocal);

  const int xyDim0 = xyLocal.getRawDataSize(0);
  const int xyDim1 = xyLocal.getRawDataSize(1);
  const int xyDim2 = xyLocal.getRawDataSize(2);
  real *xyp = xyLocal.Array_Descriptor.Array_View_Pointer3;
#define XY(i1,i2,i3,m) xyp[(i1)+xyDim0*(i2+xyDim1*(i3+xyDim2*(m)))]

  int *girp = gridIndexRange.Array_Descriptor.Array_View_Pointer1;
#define GIR(m,n) girp[(m)+2*(n)]

//   int *dimp = dimension.Array_Descriptor.Array_View_Pointer1;
// #define DIM(m,n) dimp[(m)+2*(n)]

  // DIM should be LOCAL bounds

  bool periodic[3]={false,false,false}; //  true if function periodic 
  int pdim[6]={0,0,0,0,0,0}; // 
  #define dim(side,axis) pdim[(side)+2*(axis)]
  for( axis=0; axis<domainDimension; axis++ )
  {
    dim(0,axis)=max(xyLocal.getBase(axis),dimension(0,axis));  // we don't want parallel ghost points on "ends" 
    dim(1,axis)=min(xyLocal.getBound(axis),dimension(1,axis));
    // periodic -- in parallel : only true if local array is the full extent  
    #ifdef USE_PPP
      periodic[axis] = isPeriodic[axis]==functionPeriodic && 
                       xyLocal.getBase(axis)==xy.getBase(axis) && 
                       xyLocal.getBound(axis)==xy.getBound(axis);
    #else
      periodic[axis] = isPeriodic[axis]==functionPeriodic;
    #endif 
  }

//   bool periodic0, periodic1=false, periodic2=false;
//   periodic0=isPeriodic[axis1]==functionPeriodic;
//   if( domainDimension>1 ) periodic1=isPeriodic[axis2]==functionPeriodic;
//   if( domainDimension>2 ) periodic2=isPeriodic[axis3]==functionPeriodic;


  // ----------------------------------------------------------------------------
  // ---  (i11(i),i21(i),i31(i)) : lower left corner of interpolation stencil ---
  //  For 2nd order we need to keep 
  //               iX1>=dimension(Start,X) + 1    X=1,2,3
  //               iX1<=dimension(End,X  ) -2     X=1,2,3
  //  since the derivative computation uses the values from iX1-1 to iX1+2
  // 
  //  For fourth we use iX1,iX1+1,..,iX1+3
  //    so we need   iX1>=dimension(Start,X) && X1<=dimension(End,X  ) -3
  // ----------------------------------------------------------------------------

//  const int shift=0, lShift=1, rShift=2;  // (lshift,rshift) : leave space on ends for computing derivatives too.
//   if( orderOfInterpolation==4 )
//   {
//     shift=1;   // shift = 0 (2nd) or 1 (4th)
//     lShift=0;
//     rShift=1+shift*2;  // rshift=3
//   }

  int i20=dim(End,axis2);
  int i30=dim(End,axis3);
  int i11,i21,i31, i12,i22,i32;
  real r0,r1,r2,dr,ds,dt;
  
  if( orderOfInterpolation==2 )  // linear interpolation
  {
    for( i=base; i<=bound; i++ ) 
    {
      r0=R(i,0);
      if( periodic[0] && (r0<0. || r0>1.) )
	r0=delta[0]*fmod(r0+1.,1.);  // mape to [0,1] if periodic
      else
	r0*=delta[0];

      i11=int( floor(r0) );
      i11=min(dim(End,axis1)-2,max(dim(Start,axis1)+1,i11+GIR(Start,axis1)) );  // NOTE +1, +2 for eval of derivs
      i12=i11+1;

      dr=r0-(i11-GIR(Start,axis1));

      if( domainDimension > 1 )
      {
	r1=R(i,1);
	if( periodic[1] && (r1<0. || r1>1.)  )
	  r1=delta[1]*fmod(r1+1.,1.);
	else
	  r1*=delta[1];

	// *wdh* r1=delta[axis2]*R(i,1); // *wdh* bug found 030918
	i21=int( floor(r1) );
	i21=min(dim(End,axis2)-2,max(dim(Start,axis2)+1,i21+GIR(Start,axis2)) ); // NOTE +1, +2 for eval of derivs
	i22=i21+1;
	ds=r1-(i21-GIR(Start,axis2));
      }
      if( domainDimension>2 )
      {
	r2=R(i,2);
	if( periodic[2]  && (r2<0. || r2>1.) )
	  r2=delta[2]*fmod(r2+1.,1.);
	else
	  r2*=delta[2];
	i31=int( floor(r2) );
	i31=min(dim(End,axis3)-2,max(dim(Start,axis3)+1,i31+GIR(Start,axis3)));
	i32=i31+1;
	dt=r2-(i31-GIR(Start,axis3));
      }
    
      if( computeMap )
      {
	if( domainDimension==2 )
	{
	  for( axis=axis1; axis<rangeDimension; axis++ )
	  {
	    X(i,axis)=INT_2D_ORDER_2(dr,ds
				     ,XY(i11,i21,i30,axis),XY(i12,i21,i30,axis)
				     ,XY(i11,i22,i30,axis),XY(i12,i22,i30,axis));
	  }
	  if( false && (bound-base)<6 )
	  {
            ::display(xyLocal,"dpmScalar: xyLocal",pDebugFile,"%6.3f ");
	    fprintf(pDebugFile,"dpmScalar: (r0,r1)=(%e,%e) (dr,ds)=(%e,%e) (i11,i21,i30)=(%i,%i,%i) x=(%e,%e)\n",r0,r1,dr,ds,i11,i21,i30,X(i,0),X(i,1));
	    fprintf(pDebugFile,"dpmScalar: X=[%e, %e, %e, %e]\n",XY(i11,i21,i30,0),XY(i12,i21,i30,0)
		    ,XY(i11,i22,i30,0),XY(i12,i22,i30,0));
	    fprintf(pDebugFile,"dpmScalar: X=[%e, %e, %e, %e]\n",XY(i11,i21,i30,1),XY(i12,i21,i30,1)
		    ,XY(i11,i22,i30,1),XY(i12,i22,i30,1));
            fflush(0);
	  }
	  
	}
	else if( domainDimension==3 )
	{
	  // printf("i=%i, dr=%e, ds=%e, dt=%e, i11,...,=(%i,%i,%i,%i,%i,%i)\n",i,dr,ds,dt,i11,i12,i21,i22,i31,i32);
	  
	  for( axis=axis1; axis<rangeDimension; axis++ )
	  {
	    X(i,axis)=INT_3D_ORDER_2(dr,ds,dt
				     ,XY(i11,i21,i31,axis),XY(i12,i21,i31,axis)
				     ,XY(i11,i22,i31,axis),XY(i12,i22,i31,axis)
				     ,XY(i11,i21,i32,axis),XY(i12,i21,i32,axis)
				     ,XY(i11,i22,i32,axis),XY(i12,i22,i32,axis));
	  }
	}
	else if( domainDimension==1 )
	{
	  for( axis=axis1; axis<rangeDimension; axis++ )
	  {
	    X(i,axis)=INT_1D_ORDER_2(dr,XY(i11,i20,i30,axis),XY(i12,i20,i30,axis));
	  }
	}
	else
	{
	  throw "error";
	}
      }
      if( computeMapDerivative )
      {

	if( domainDimension==2 )
	{
	  real xr11,xr21,xr12,xr22;

	  for( axis=axis1; axis<rangeDimension; axis++ )
	  {
	    real d12=.5*delta[0];
	    if( i11>dim(Start,0) )
	    {
	      xr11=(XY(i12  ,i21  ,i30,axis)-XY(i11-1,i21  ,i30,axis))*d12;
	      xr12=(XY(i12  ,i22  ,i30,axis)-XY(i11-1,i22  ,i30,axis))*d12;
	    }
	    else
	    {
	      xr11=(XY(i12  ,i21  ,i30,axis)-XY(i11  ,i21  ,i30,axis))*delta[0]; // one-sided
	      xr12=(XY(i12  ,i22  ,i30,axis)-XY(i11  ,i22  ,i30,axis))*delta[0];
	    }
	    if( i12<dim(End,0) )
	    {
	      xr21=(XY(i12+1,i21  ,i30,axis)-XY(i11  ,i21  ,i30,axis))*d12;
	      xr22=(XY(i12+1,i22  ,i30,axis)-XY(i11  ,i22  ,i30,axis))*d12;
	    }
	    else
	    {
	      xr21=(XY(i12  ,i21  ,i30,axis)-XY(i11  ,i21  ,i30,axis))*delta[0]; // one-sided
	      xr22=(XY(i12  ,i22  ,i30,axis)-XY(i11  ,i22  ,i30,axis))*delta[0];
	    }
	    
	    XR(i,axis,0)=INT_2D_ORDER_2(dr,ds,xr11,xr21,xr12,xr22);

	    d12=.5*delta[1];
	    if( i21>dim(Start,1) )
	    {
	      xr11=(XY(i11  ,i22  ,i30,axis)-XY(i11  ,i21-1,i30,axis))*d12;
	      xr12=(XY(i11  ,i22+1,i30,axis)-XY(i11  ,i21  ,i30,axis))*d12;
	    }
	    else
	    {
	      xr11=(XY(i11  ,i22  ,i30,axis)-XY(i11  ,i21  ,i30,axis))*delta[1]; // one-sided
	      xr12=(XY(i11  ,i22+1,i30,axis)-XY(i11  ,i22  ,i30,axis))*delta[1];
	    }
	    if( i21<dim(End,1) )
	    {
	      xr21=(XY(i12  ,i22  ,i30,axis)-XY(i12  ,i21-1,i30,axis))*d12;
	      xr22=(XY(i12  ,i22+1,i30,axis)-XY(i12  ,i21  ,i30,axis))*d12;
	    }
	    else
	    {
	      xr21=(XY(i12  ,i21  ,i30,axis)-XY(i12  ,i21-1,i30,axis))*delta[1]; // one-sided
	      xr22=(XY(i12  ,i22  ,i30,axis)-XY(i12  ,i21  ,i30,axis))*delta[1];
	    }
	    
	    XR(i,axis,1)=INT_2D_ORDER_2(dr,ds,xr11,xr21,xr12,xr22);
	    
	  }
	}
	else if( domainDimension==3 )
	{
	  real xr111,xr211,xr121,xr221,xr112,xr212,xr122,xr222;

	  for( axis=axis1; axis<rangeDimension; axis++ )
	  {
	    real d12=.5*delta[0];
	    if( i11>dim(Start,0) )
	    {
	      xr111=(XY(i12  ,i21  ,i31  ,axis)-XY(i11-1,i21  ,i31  ,axis))*d12;
	      xr121=(XY(i12  ,i22  ,i31  ,axis)-XY(i11-1,i22  ,i31  ,axis))*d12;
	      xr112=(XY(i12  ,i21  ,i32  ,axis)-XY(i11-1,i21  ,i32  ,axis))*d12;
	      xr122=(XY(i12  ,i22  ,i32  ,axis)-XY(i11-1,i22  ,i32  ,axis))*d12;
	    }
	    else
	    {
	      xr111=(XY(i12  ,i21  ,i31  ,axis)-XY(i11  ,i21  ,i31  ,axis))*delta[0]; // one-sided
	      xr121=(XY(i12  ,i22  ,i31  ,axis)-XY(i11  ,i22  ,i31  ,axis))*delta[0];
	      xr112=(XY(i12  ,i21  ,i32  ,axis)-XY(i11  ,i21  ,i32  ,axis))*delta[0];
	      xr122=(XY(i12  ,i22  ,i32  ,axis)-XY(i11  ,i22  ,i32  ,axis))*delta[0];
	    }
	    if( i12<dim(End,0) )
	    {
	      xr211=(XY(i12+1,i21  ,i31  ,axis)-XY(i11  ,i21  ,i31  ,axis))*d12;
	      xr221=(XY(i12+1,i22  ,i31  ,axis)-XY(i11  ,i22  ,i31  ,axis))*d12;
	      xr212=(XY(i12+1,i21  ,i32  ,axis)-XY(i11  ,i21  ,i32  ,axis))*d12;
	      xr222=(XY(i12+1,i22  ,i32  ,axis)-XY(i11  ,i22  ,i32  ,axis))*d12;
	    }
	    else
	    {
	      xr211=(XY(i12  ,i21  ,i31  ,axis)-XY(i11  ,i21  ,i31  ,axis))*delta[0]; // one-sided
	      xr221=(XY(i12  ,i22  ,i31  ,axis)-XY(i11  ,i22  ,i31  ,axis))*delta[0];
	      xr212=(XY(i12  ,i21  ,i32  ,axis)-XY(i11  ,i21  ,i32  ,axis))*delta[0];
	      xr222=(XY(i12  ,i22  ,i32  ,axis)-XY(i11  ,i22  ,i32  ,axis))*delta[0];
	    }

	    XR(i,axis,0)=INT_3D_ORDER_2(dr,ds,dt,xr111,xr211, xr121,xr221, xr112,xr212,xr122, xr222);

	    d12=.5*delta[1];
	    if( i21>dim(Start,1) )
	    {
	      xr111=(XY(i11  ,i22  ,i31  ,axis)-XY(i11  ,i21-1,i31  ,axis))*d12;
	      xr211=(XY(i12  ,i22  ,i31  ,axis)-XY(i12  ,i21-1,i31  ,axis))*d12;
	      xr112=(XY(i11  ,i22  ,i32  ,axis)-XY(i11  ,i21-1,i32  ,axis))*d12;
	      xr212=(XY(i12  ,i22  ,i32  ,axis)-XY(i12  ,i21-1,i32  ,axis))*d12;
	    }
	    else
	    {
	      xr111=(XY(i11  ,i22  ,i31  ,axis)-XY(i11  ,i21  ,i31  ,axis))*delta[1]; // one-sided
	      xr211=(XY(i12  ,i22  ,i31  ,axis)-XY(i12  ,i21  ,i31  ,axis))*delta[1];
	      xr112=(XY(i11  ,i22  ,i32  ,axis)-XY(i11  ,i21  ,i32  ,axis))*delta[1];
	      xr212=(XY(i12  ,i22  ,i32  ,axis)-XY(i12  ,i21  ,i32  ,axis))*delta[1];
	    }
	    if( i22<dim(End,1) )
	    {
	      xr121=(XY(i11  ,i22+1,i31  ,axis)-XY(i11  ,i21  ,i31  ,axis))*d12;
	      xr221=(XY(i12  ,i22+1,i31  ,axis)-XY(i12  ,i21  ,i31  ,axis))*d12;
	      xr122=(XY(i11  ,i22+1,i32  ,axis)-XY(i11  ,i21  ,i32  ,axis))*d12;
	      xr222=(XY(i12  ,i22+1,i32  ,axis)-XY(i12  ,i21  ,i32  ,axis))*d12;
	    }
	    else
	    {
	      xr121=(XY(i11  ,i22  ,i31  ,axis)-XY(i11  ,i21  ,i31  ,axis))*delta[1]; // one-sided
	      xr221=(XY(i12  ,i22  ,i31  ,axis)-XY(i12  ,i21  ,i31  ,axis))*delta[1];
	      xr122=(XY(i11  ,i22  ,i32  ,axis)-XY(i11  ,i21  ,i32  ,axis))*delta[1];
	      xr222=(XY(i12  ,i22  ,i32  ,axis)-XY(i12  ,i21  ,i32  ,axis))*delta[1];
	    }

	    XR(i,axis,1)=INT_3D_ORDER_2(dr,ds,dt,xr111,xr211, xr121,xr221, xr112,xr212,xr122, xr222);

	    d12=.5*delta[2];
	    if( i31>dim(Start,2) )
	    {
	      xr111=(XY(i11  ,i21  ,i32  ,axis)-XY(i11  ,i21  ,i31-1,axis))*d12;
	      xr211=(XY(i12  ,i21  ,i32  ,axis)-XY(i12  ,i21  ,i31-1,axis))*d12;
	      xr121=(XY(i11  ,i22  ,i32  ,axis)-XY(i11  ,i22  ,i31-1,axis))*d12;
	      xr221=(XY(i12  ,i22  ,i32  ,axis)-XY(i12  ,i22  ,i31-1,axis))*d12;
	    }
	    else
	    {
	      xr111=(XY(i11  ,i21  ,i32  ,axis)-XY(i11  ,i21  ,i31  ,axis))*delta[2]; // one-sided
	      xr211=(XY(i12  ,i21  ,i32  ,axis)-XY(i12  ,i21  ,i31  ,axis))*delta[2];
	      xr121=(XY(i11  ,i22  ,i32  ,axis)-XY(i11  ,i22  ,i31  ,axis))*delta[2];
	      xr221=(XY(i12  ,i22  ,i32  ,axis)-XY(i12  ,i22  ,i31  ,axis))*delta[2];
	    }
	    if( i32<dim(End,2) )
	    {
	      xr112=(XY(i11  ,i21  ,i32+1,axis)-XY(i11  ,i21  ,i31  ,axis))*d12;
	      xr212=(XY(i12  ,i21  ,i32+1,axis)-XY(i12  ,i21  ,i31  ,axis))*d12;
	      xr122=(XY(i11  ,i22  ,i32+1,axis)-XY(i11  ,i22  ,i31  ,axis))*d12;
	      xr222=(XY(i12  ,i22  ,i32+1,axis)-XY(i12  ,i22  ,i31  ,axis))*d12;
	    }
	    else
	    {
	      xr112=(XY(i11  ,i21  ,i32  ,axis)-XY(i11  ,i21  ,i31  ,axis))*delta[2]; // one-sided
	      xr212=(XY(i12  ,i21  ,i32  ,axis)-XY(i12  ,i21  ,i31  ,axis))*delta[2];
	      xr122=(XY(i11  ,i22  ,i32  ,axis)-XY(i11  ,i22  ,i31  ,axis))*delta[2];
	      xr222=(XY(i12  ,i22  ,i32  ,axis)-XY(i12  ,i22  ,i31  ,axis))*delta[2];
	    }

	    XR(i,axis,2)=INT_3D_ORDER_2(dr,ds,dt,xr111,xr211, xr121,xr221, xr112,xr212,xr122, xr222);
	  }
	  
	}
	else if( domainDimension==1 )
	{
	  real xr1,xr2;
	  real d12=.5*delta[0];
	  for( axis=axis1; axis<rangeDimension; axis++ )
	  {
	    if( i11>dim(Start,0) )
	    {
	      xr1=(XY(i12  ,i20  ,i30,axis)-XY(i11-1,i20  ,i30,axis))*d12;
	    }
	    else
	    {
	      xr1=(XY(i12  ,i20  ,i30,axis)-XY(i11  ,i20  ,i30,axis))*delta[0]; // one-sided
	    }
	    if( i12<dim(End,0) )
	    {
	      xr2=(XY(i12+1,i20  ,i30,axis)-XY(i11  ,i20  ,i30,axis))*d12;
	    }
	    else
	    {
	      xr2=(XY(i12  ,i20  ,i30,axis)-XY(i11  ,i20  ,i30,axis))*delta[0]; // one-sided
	    }
	    XR(i,axis,0)=INT_1D_ORDER_2(dr,xr1,xr2);
	  }
	  
	}
      }
    } // end for i
    
  }
  else if( orderOfInterpolation==4 )
  {

    // --- cubic interpolation, use 4, points
    // 
    //          ---+----+-X--+----+---
    //            i1   i1+1 i1+2 i1+3 
    //  i1 = (closet point less than r) -1 

    real a0,a0r,a1,a1r,a2,a2r,a3,a3r;
    real b0,b0r,b1,b1r,b2,b2r,b3,b3r;
    real c0,c0r,c1,c1r,c2,c2r,c3,c3r;

    for( i=base; i<=bound; i++ ) 
    {
      r0=R(i,0);
      if( periodic[0] && (r0<0. || r0>1.) )
	r0=delta[0]*fmod(r0+1.,1.);  // map to [0,1] if periodic
      else
	r0*=delta[0];

      i11=int(floor(r0))-1;  // check this for r<0 ! 
      i11=min(dim(End,axis1)-3,max(dim(Start,axis1),i11+GIR(Start,axis1)) );
      dr=r0-(i11-GIR(Start,axis1));

//        if( false && fabs(R(i,0)-1.)<.01 )
//        {
//  	printf("dpm4: INFO: R(i,0)=%8.3e, r0=%8.2e, delta=%8.2e, i11=%i dr=%8.2e DIM=[%i,%i] GIR=[%i,%i]\n",
//                R(i,0),r0,delta[0],i11,dr,dim(Start,axis1),dim(End,axis1),GIR(Start,axis1),GIR(End,axis1)  );
//          printf(" x(i11)...x(i11+3) = [%9.3e,%9.3e,%9.3e,%9.3e]\n",
//  	       XY(i11,i20,i30,0),XY(i11+1,i20,i30,0),XY(i11+2,i20,i30,0),XY(i11+3,i20,i30,0));
	
//        }
      

      if( i11>dim(Start,axis1) && i11<(dim(End,axis1)-3) && 
          (dr<1. || dr>2.) )
      {
	printf("dpm4: ERROR: R(i,0)=%8.3e, r0=%8.2e, delta=%8.2e, i11=%i dr=%8.2e DIM=[%i,%i] GIR=[%i,%i]\n",
              R(i,0),r0,delta[0],i11,dr,dim(Start,axis1),dim(End,axis1),GIR(Start,axis1),GIR(End,axis1) );
      }

      if( domainDimension > 1 )
      {
	r1=R(i,1);
	if( periodic[1] && (r1<0. || r1>1.)  )
	  r1=delta[1]*fmod(r1+1.,1.);
	else
	  r1*=delta[1];

	i21=int(floor(r1))-1;
	i21=min(dim(End,axis2)-3,max(dim(Start,axis2),i21+GIR(Start,axis2)) );
	ds=r1-(i21-GIR(Start,axis2));

	if( i21>dim(Start,axis2) && i21<(dim(End,axis2)-3) && 
	    (ds<1. || ds>2.) )
	{
	  printf("dpm4: ERROR: R(i,1)=%8.3e, r1=%8.2e, delta=%8.2e, i21=%i ds=%8.2e \n",R(i,1),r1,delta[1],i21,ds);
	}

      }
      if( domainDimension>2 )
      {
	r2=R(i,2);
	if( periodic[2]  && (r2<0. || r2>1.) )
	  r2=delta[2]*fmod(r2+1.,1.);
	else
	  r2*=delta[2];
	i31=int(floor(r2))-1;
	i31=min(dim(End,axis3)-3,max(dim(Start,axis3),i31+GIR(Start,axis3)));
	dt=r2-(i31-GIR(Start,axis3));

	if( i31>dim(Start,axis3) && i31<(dim(End,axis3)-3) && 
	    (dt<1. || dt>2.) )
	{
	  printf("dpm4: ERROR: i31=%i dt=%8.2e \n",i31,dt);
	}
      }


      a0=q03(dr);
      a1=q13(dr);
      a2=q23(dr);
      a3=q33(dr);
      a0r=q03d(dr)*delta[axis1];
      a1r=q13d(dr)*delta[axis1];
      a2r=q23d(dr)*delta[axis1];
      a3r=q33d(dr)*delta[axis1];
      if( domainDimension>=2 )
      {
	b0=q03(ds);
	b1=q13(ds);
	b2=q23(ds);
	b3=q33(ds);
	b0r=q03d(ds)*delta[axis2];
	b1r=q13d(ds)*delta[axis2];
	b2r=q23d(ds)*delta[axis2];
	b3r=q33d(ds)*delta[axis2];
	
	if( domainDimension==2 )
	{
	  for( axis=axis1; axis<rangeDimension; axis++ )
	  {
	    if( computeMap )
	    {
	      X(i,axis)=q2x(i11,i21,i30,axis);
	    }
	    if( computeMapDerivative )
	    {
	      XR(i,axis,axis1)=q2xr(i11,i21,i30,axis);
	      XR(i,axis,axis2)=q2xs(i11,i21,i30,axis);
	    }
	  }
	}
	else if( domainDimension==3 )
	{
	  c0=q03(dt);
	  c1=q13(dt);
	  c2=q23(dt);
	  c3=q33(dt);
	  c0r=q03d(dt)*delta[axis3];
	  c1r=q13d(dt)*delta[axis3];
	  c2r=q23d(dt)*delta[axis3];
	  c3r=q33d(dt)*delta[axis3];
	  for( axis=axis1; axis<rangeDimension; axis++ )
	  {
	    if( computeMap )
	    {
	      X(i,axis)=q3x(i11,i21,i31,axis);
	    }
	    if( computeMapDerivative )
	    {
	      XR(i,axis,axis1)=q3xr(i11,i21,i31,axis);
	      XR(i,axis,axis2)=q3xs(i11,i21,i31,axis);
	      XR(i,axis,axis3)=q3xt(i11,i21,i31,axis);
	    }
	  }
	}
      }
      else if( domainDimension==1 )
      {
	for( axis=axis1; axis<rangeDimension; axis++ )
	{
	  if( computeMap )
	  {
	    X(i,axis)=q1x(i11,i20,i30,axis);
	  }
	  if( computeMapDerivative )
	  {
	    XR(i,axis,axis1)=q1xr(i11,i20,i30,axis);
	  }
	}
      }
      else
      {
	Overture::abort("dpmScalar:ERROR: unexpected domainDimension");
      }
      
    } // end for i
  } // end order of interpolation
  
}
