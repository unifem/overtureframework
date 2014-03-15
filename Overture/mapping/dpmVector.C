#include "DataPointMapping.h"
#include "MappingInformation.h"
#include "arrayGetIndex.h"
#include <string.h>
#include "conversion.h"
#include "display.h"

// static int useNewAPP = FALSE;


// *************************************
//  tri-linear interpolant: 
//    INT_2D means the domain dimension is 2
// *************************************
#define INT_1D_ORDER_2(dr,x111,x211)  \
      ( (1.-dr)*(x111)+dr*(x211) )

#define INT_1D_ORDER_2_R(dr,x111,x211)  \
      ( delta[0]*( (x211)-(x111) ) )

#define INT_2D_ORDER_2(dr,ds,x111,x211,x121,x221)  \
      ( (1.-ds)*((1.-dr)*(x111)+dr*(x211))+ds*((1.-dr)*(x121)+dr*(x221)) )

#define INT_2D_ORDER_2_R(dr,ds,x111,x211,x121,x221)  \
      ( ((1.-ds)*( (x211)-(x111) ) +ds*( (x221)-(x121) ))*delta[0] )

#define INT_2D_ORDER_2_S(dr,ds,x111,x211,x121,x221)  \
      ( ((1.-dr)*( (x121)-(x111) ) +dr*( (x221)-(x211) ))*delta[1] )

#define INT_3D_ORDER_2(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222)  \
  (                                                    \
     (1.-dt)*((1.-ds)*((1.-dr)*(x111)+dr*(x211))+ds*((1.-dr)*(x121)+dr*(x221))) \
     +    dt*((1.-ds)*((1.-dr)*(x112)+dr*(x212))+ds*((1.-dr)*(x122)+dr*(x222))) )

#define INT_3D_ORDER_2_R(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222)  \
  (                                                    \
     ((1.-dt)*((1.-ds)*((x211)-(x111))+ds*((x221)-(x121))) \
   +      dt* ((1.-ds)*((x212)-(x112))+ds*((x222)-(x122))))*delta[0] )

#define INT_3D_ORDER_2_S(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222)  \
  (  \
     ((1.-dt)*((1.-dr)*((x121)-(x111))+dr*((x221)-(x211))) \
   +      dt *((1.-dr)*((x122)-(x112))+dr*((x222)-(x212))))*delta[1] )

#define INT_3D_ORDER_2_T(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222)  \
  (                                                    \
    ( (1.-dr)*((1.-ds)*((x112)-(x111))+ds*((x122)-(x121))) \
  +       dr *((1.-ds)*((x212)-(x211))+ds*((x222)-(x221))) )*delta[2] )

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
      (  a0(I)* xy(i1  ,i2  ,i3  ,axis) \
        +a1(I)* xy(i1+1,i2  ,i3  ,axis) \
        +a2(I)* xy(i1+2,i2  ,i3  ,axis) \
        +a3(I)* xy(i1+3,i2  ,i3  ,axis) )
#define q2x(i1,i2,i3,axis)  \
      (  b0(I)*q1x(i1  ,i2  ,i3  ,axis) \
        +b1(I)*q1x(i1  ,i2+1,i3  ,axis) \
        +b2(I)*q1x(i1  ,i2+2,i3  ,axis) \
        +b3(I)*q1x(i1  ,i2+3,i3  ,axis) )
#define q3x(i1,i2,i3,axis)  \
      (  c0(I)*q2x(i1  ,i2  ,i3  ,axis) \
        +c1(I)*q2x(i1  ,i2  ,i3+1,axis) \
        +c2(I)*q2x(i1  ,i2  ,i3+2,axis) \
        +c3(I)*q2x(i1  ,i2  ,i3+3,axis) )
#define q1xr(i1,i2,i3,axis)   \
                   (    a0r(I)*xy(i1  ,i2  ,i3  ,axis)   \
                       +a1r(I)*xy(i1+1,i2  ,i3  ,axis)   \
                       +a2r(I)*xy(i1+2,i2  ,i3  ,axis)   \
                       +a3r(I)*xy(i1+3,i2  ,i3  ,axis) )
#define q2xr(i1,i2,i3,axis)   \
                    (   b0(I)*q1xr(i1  ,i2  ,i3  ,axis)   \
                       +b1(I)*q1xr(i1  ,i2+1,i3  ,axis)   \
                       +b2(I)*q1xr(i1  ,i2+2,i3  ,axis)   \
                       +b3(I)*q1xr(i1  ,i2+3,i3  ,axis) )
#define q3xr(i1,i2,i3,axis)   \
                     (  c0(I)*q2xr(i1  ,i2  ,i3  ,axis)   \
                       +c1(I)*q2xr(i1  ,i2  ,i3+1,axis)   \
                       +c2(I)*q2xr(i1  ,i2  ,i3+2,axis)   \
                       +c3(I)*q2xr(i1  ,i2  ,i3+3,axis)  ) 

#define q1xs(i1,i2,i3,axis)   \
                   (    a0(I)*  xy(i1  ,i2  ,i3  ,axis)   \
                       +a1(I)*  xy(i1+1,i2  ,i3  ,axis)   \
                       +a2(I)*  xy(i1+2,i2  ,i3  ,axis)   \
                       +a3(I)*  xy(i1+3,i2  ,i3  ,axis)  ) 
#define q2xs(i1,i2,i3,axis)   \
                     (  b0r(I)*q1xs(i1  ,i2  ,i3  ,axis)   \
                       +b1r(I)*q1xs(i1  ,i2+1,i3  ,axis)   \
                       +b2r(I)*q1xs(i1  ,i2+2,i3  ,axis)   \
                       +b3r(I)*q1xs(i1  ,i2+3,i3  ,axis))   
#define q3xs(i1,i2,i3,axis)   \
                     (  c0(I)*q2xs(i1  ,i2  ,i3  ,axis)   \
                       +c1(I)*q2xs(i1  ,i2  ,i3+1,axis)   \
                       +c2(I)*q2xs(i1  ,i2  ,i3+2,axis)   \
                       +c3(I)*q2xs(i1  ,i2  ,i3+3,axis) )  

#define q1xt(i1,i2,i3,axis)   \
                     (  a0(I)*  xy(i1  ,i2  ,i3  ,axis)   \
                       +a1(I)*  xy(i1+1,i2  ,i3  ,axis)   \
                       +a2(I)*  xy(i1+2,i2  ,i3  ,axis)   \
                       +a3(I)*  xy(i1+3,i2  ,i3  ,axis) )  
#define q2xt(i1,i2,i3,axis)   \
                    (   b0(I)*q1xt(i1  ,i2  ,i3  ,axis)   \
                       +b1(I)*q1xt(i1  ,i2+1,i3  ,axis)   \
                       +b2(I)*q1xt(i1  ,i2+2,i3  ,axis)   \
                       +b3(I)*q1xt(i1  ,i2+3,i3  ,axis) )  
#define q3xt(i1,i2,i3,axis)   \
                     (  c0r(I)*q2xt(i1  ,i2  ,i3  ,axis)   \
                       +c1r(I)*q2xt(i1  ,i2  ,i3+1,axis)   \
                       +c2r(I)*q2xt(i1  ,i2  ,i3+2,axis)   \
                       +c3r(I)*q2xt(i1  ,i2  ,i3+3,axis) )  


void DataPointMapping::
mapVector(const realArray & r_, realArray & x, realArray & xr, MappingParameters & params, const Index & I)
// =========================================================================================================
//         ************ NOT USED ANYMORE *************
// 
//    evaluate the DPM using Array operations 
//
// These variables are assumed assigned:
//          const int & base, const int & bound, const bool & computeMap, 
//          const bool & computeMapDerivative  )
// =========================================================================================================
{
  if( params.coordinateType != cartesian )
    cerr << "DataPointMapping::map - coordinateType != cartesian " << endl;

  int axis;
  const real oneSixth=1./6.;

  // map periodic directions:
  realArray r(I,domainDimension); 
  r=r_(I,Index(0,domainDimension));
  for( axis=0; axis<domainDimension; axis++ )
  {
    if( getIsPeriodic(axis)==functionPeriodic )
    {
      r(I,axis)=fmod(r(I,axis)+1.,1.);
    }
  }


  int is[3], &is1=is[0], &is2=is[1], &is3=is[2];

  realArray dr(I),ds(I),dt(I);  
  intArray i11(I),i21(I),i31(I); 

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

  int shift=0, lShift=1, rShift=2;  // (lshift,rshift) : leave space on ends for computing derivatives too.
  if( orderOfInterpolation==4 )
  {
    shift=1;   // shift = 0 (2nd) or 1 (4th)
    lShift=0;
    rShift=1+shift*2;  // rshift=3
  }

// @PD realArray2[dr,ds,dt,r,x,xya,xr11,xr21,xr12,xr22,xr0] Range[I] intArray1[i11,i21,i31]


  i11(I)=(floor(delta[axis1]*r(I,axis1))-shift).convertToIntArray();  // @PA
//  for( i=base; i<=bound; i++ )  // do this because no mixed A++ operations
//    i11(i)=int( floor(delta[axis1]*r(i,axis1))-shift );

  // ::display(r(I,axis1),"r(I,axis1)");
  // ::display(i11(I),"i11(I) before");
  
  i11=min(dimension(End,axis1)-rShift,max(dimension(Start,axis1)+lShift,i11+gridIndexRange(Start,axis1)) ); // @PA


  // ::display(i11(I),"i11(I) after");
  dr(I)=delta[axis1]*r(I,axis1)-(i11-gridIndexRange(Start,axis1)).convertToRealArray();    // @PA
//  for( i=base; i<=bound; i++ )  // do this because no mixed A++ operations
//    dr(i)=delta[axis1]*r(i,axis1)-(i11(i)-gridIndexRange(Start,axis1));

  if( domainDimension > 1 )
  {
    i21=(floor(delta[axis2]*r(I,axis2))-shift).convertToIntArray(); // @PA
    // for( i=base; i<=bound; i++ )  // do this because no mixed A++ operations
    //   i21(i)=int( floor(delta[axis2]*r(i,axis2))-shift );

    i21=min(dimension(End,axis2)-rShift,max(dimension(Start,axis2)+lShift,i21+gridIndexRange(Start,axis2)) ); // @PA

    ds(I)=delta[axis2]*r(I,axis2)-(i21-gridIndexRange(Start,axis2)).convertToRealArray();   // @PA
    // for( i=base; i<=bound; i++ )  // do this because no mixed A++ operations
    //   ds(i)=delta[axis2]*r(i,axis2)-(i21(i)-gridIndexRange(Start,axis2));
  }
  if( domainDimension>2 )
  {
    i31=(floor(delta[axis3]*r(I,axis3))-shift).convertToIntArray(); // @PA
    // for( i=base; i<=bound; i++ )  // do this because no mixed A++ operations
    //   i31(i)=int( floor(delta[axis3]*r(i,axis3))-shift );

    i31=min(dimension(End,axis3)-rShift,max(dimension(Start,axis3)+lShift,i31+gridIndexRange(Start,axis3)) ); // @PA

    dt(I)=delta[axis3]*r(I,axis3)-(i31-gridIndexRange(Start,axis3)).convertToRealArray();   // @PA

//     for( i=base; i<=bound; i++ ) 
//       i31(i)=int( floor(delta[axis3]*r(i,axis3))-shift );
//     i31=min(dimension(End,axis3)-rShift,max(dimension(Start,axis3)+lShift,i31+gridIndexRange(Start,axis3)));
//     for( i=base; i<=bound; i++ ) 
//       dt(i)=delta[axis3]*r(i,axis3)-(i31(i)-gridIndexRange(Start,axis3));
  }

  int i20=dimension(End,axis2);
  int i30=dimension(End,axis3);


  Range all;
  if( orderOfInterpolation==2 )  // linear interpolation
  {
    
    if( domainDimension==2 )
    {
      for( axis=axis1; axis<rangeDimension; axis++ )
      {
        const realArray & xya = xy(all,all,i30,axis);
	if( computeMap )
	{

	  x(I,axis)=(1.-ds)*( (1.-dr)*xya(i11,i21)   + dr*xya(i11+1,i21) )       // @PANS
	               +ds *( (1.-dr)*xya(i11,i21+1) + dr*xya(i11+1,i21+1) );

	}
	if( computeMapDerivative )
	{
//	  realArray xr11(I),xr21(I),xr12(I),xr22(I);
	  for( int dir=0; dir<domainDimension; dir++ )
	  {
	    
	    real d12=.5*delta[dir];
	    is[0]=is[1]=0;
	    is[dir]=1;
	    const int ip1=is1+1, ip2=is2+1;
	    const int im1=is1-1, im2=is2-1;

            const realArray & xr1 = xr(I,axis,dir);
            realArray & xr0 = (realArray &)xr1;
	    
//             if( false )
// 	    {
// 	      xr11(I)=(xya(i11(I)  +is1,i21(I)  +is2)-xya(i11(I)  -is1,i21(I)  -is2))*d12, 
// 	      xr21(I)=(xya(i11(I)+1+is1,i21(I)  +is2)-xya(i11(I)+1-is1,i21(I)  -is2))*d12,
// 	      xr12(I)=(xya(i11(I)  +is1,i21(I)+1+is2)-xya(i11(I)  -is1,i21(I)+1-is2))*d12,
// 	      xr22(I)=(xya(i11(I)+1+is1,i21(I)+1+is2)-xya(i11(I)+1-is1,i21(I)+1-is2))*d12;

// 	      // xr(I,axis,dir)=INT_2D_ORDER_2(dr(I),ds(I),xr11,xr21,xr12,xr22);

// 	      xr0=(1.-ds)*( (1.-dr)*xr11 + dr*xr21 )      
// 		+ds *( (1.-dr)*xr12 + dr*xr22 );
// 	    }
	    
	    xr0=((1.-ds)*( (1.-dr)*(xya(i11(I)+is1,i21(I)+is2)-xya(i11(I)-is1,i21(I)-is2)) +   // @PANS
                               dr *(xya(i11(I)+ip1,i21(I)+is2)-xya(i11(I)-im1,i21(I)-is2)) )       
	      +ds *( (1.-dr)*(xya(i11(I)+is1,i21(I)+ip2)-xya(i11(I)-is1,i21(I)-im2)) + 
                          dr*(xya(i11(I)+ip1,i21(I)+ip2)-xya(i11(I)-im1,i21(I)-im2))))*d12;

	  }
	}
      }
    }
    else if( domainDimension==3 )
    {
// @PD realArray3[dr,ds,dt,r,x,xya,xr111,xr211,xr121,xr221,xr112,xr212,xr122,xr222,xr0] Range[I] intArray1[i11,i21,i31]

      for( axis=axis1; axis<rangeDimension; axis++ )
      {
        const realArray & xya = xy(all,all,all,axis);
	if( computeMap )
	{
          x(I,axis)=(1.-dt)*(                                                            // @PANS
	      (1.-ds)*( (1.-dr)*xya(i11,i21  ,i31) + dr*xya(i11+1,i21,i31) ) 
	         +ds *( (1.-dr)*xya(i11,i21+1,i31) + dr*xya(i11+1,i21+1,i31) ))+
	      dt *(                                                           
		(1.-ds)*( (1.-dr)*xya(i11,i21  ,i31+1) + dr*xya(i11+1,i21  ,i31+1) ) 
		   +ds *( (1.-dr)*xya(i11,i21+1,i31+1) + dr*xya(i11+1,i21+1,i31+1) ));

// 	  for( i=base; i<= bound; i++ )
// 	  {
// 	    x(i,axis)=INT_3D_ORDER_2(dr(i),ds(i),dt(i)
// 				     ,xy(i11(i),i21(i),i31(i),axis),xy(i12(i),i21(i),i31(i),axis)
// 				     ,xy(i11(i),i22(i),i31(i),axis),xy(i12(i),i22(i),i31(i),axis)
// 				     ,xy(i11(i),i21(i),i32(i),axis),xy(i12(i),i21(i),i32(i),axis)
// 				     ,xy(i11(i),i22(i),i32(i),axis),xy(i12(i),i22(i),i32(i),axis));
// 	  }
	}
	if( computeMapDerivative )
	{
// 	  realArray xr111(I),xr211(I),xr121(I),xr221(I),xr112(I),xr212(I),xr122(I),xr222(I);
	  for( int dir=0; dir<domainDimension; dir++ )
	  {
	    
	    real d12=.5*delta[dir];
	    is[0]=is[1]=is[2]=0;
	    is[dir]=1;
	    const int ip1=is1+1, ip2=is2+1, ip3=is3+1;
	    const int im1=is1-1, im2=is2-1, im3=is3-1;

// 	    xr111(I)=(xya(i11(I)+is1,i21(I)+is2,i31(I)+is3)-xya(i11(I)-is1,i21(I)-is2,i31(I)-is3))*d12,
// 	    xr211(I)=(xya(i11(I)+ip1,i21(I)+is2,i31(I)+is3)-xya(i11(I)-im1,i21(I)-is2,i31(I)-is3))*d12,
// 	    xr121(I)=(xya(i11(I)+is1,i21(I)+ip2,i31(I)+is3)-xya(i11(I)-is1,i21(I)-im2,i31(I)-is3))*d12,
// 	    xr221(I)=(xya(i11(I)+ip1,i21(I)+ip2,i31(I)+is3)-xya(i11(I)-im1,i21(I)-im2,i31(I)-is3))*d12,
// 	    xr112(I)=(xya(i11(I)+is1,i21(I)+is2,i31(I)+ip3)-xya(i11(I)-is1,i21(I)-is2,i31(I)-im3))*d12,
// 	    xr212(I)=(xya(i11(I)+ip1,i21(I)+is2,i31(I)+ip3)-xya(i11(I)-im1,i21(I)-is2,i31(I)-im3))*d12,
// 	    xr122(I)=(xya(i11(I)+is1,i21(I)+ip2,i31(I)+ip3)-xya(i11(I)-is1,i21(I)-im2,i31(I)-im3))*d12,
// 	    xr222(I)=(xya(i11(I)+ip1,i21(I)+ip2,i31(I)+ip3)-xya(i11(I)-im1,i21(I)-im2,i31(I)-im3))*d12;

// 	    const realArray & xr0 = xr(I,axis,dir);
// 	    xr0=(1.-dt)*(                                                           
// 	      (1.-ds)*( (1.-dr)*xr111 + dr*xr211 ) 
// 	      +ds *( (1.-dr)*xr121 + dr*xr221 ))+
// 	      dt *(                                                           
// 		(1.-ds)*( (1.-dr)*xr112 + dr*xr212 ) 
// 		+ds *( (1.-dr)*xr122 + dr*xr222 ));


	    const realArray & xr1 = xr(I,axis,dir);
	    realArray & xr0 = (realArray &)xr1;
	    
	    xr0=d12*(1.-dt)*(                                                            // @PANS
	      (1.-ds)*( (1.-dr)*(xya(i11(I)+is1,i21(I)+is2,i31(I)+is3)-xya(i11(I)-is1,i21(I)-is2,i31(I)-is3)) +
                            dr *(xya(i11(I)+ip1,i21(I)+is2,i31(I)+is3)-xya(i11(I)-im1,i21(I)-is2,i31(I)-is3)) ) 
	      +ds *(    (1.-dr)*(xya(i11(I)+is1,i21(I)+ip2,i31(I)+is3)-xya(i11(I)-is1,i21(I)-im2,i31(I)-is3)) + 
                            dr *(xya(i11(I)+ip1,i21(I)+ip2,i31(I)+is3)-xya(i11(I)-im1,i21(I)-im2,i31(I)-is3)) ))+
	      dt *(                                                           
	      (1.-ds)*( (1.-dr)*(xya(i11(I)+is1,i21(I)+is2,i31(I)+ip3)-xya(i11(I)-is1,i21(I)-is2,i31(I)-im3)) + 
                            dr *(xya(i11(I)+ip1,i21(I)+is2,i31(I)+ip3)-xya(i11(I)-im1,i21(I)-is2,i31(I)-im3)) ) 
		 +ds *( (1.-dr)*(xya(i11(I)+is1,i21(I)+ip2,i31(I)+ip3)-xya(i11(I)-is1,i21(I)-im2,i31(I)-im3)) + 
                            dr *(xya(i11(I)+ip1,i21(I)+ip2,i31(I)+ip3)-xya(i11(I)-im1,i21(I)-im2,i31(I)-im3)) ));

// 	  xr(I,axis,dir)=INT_3D_ORDER_2(dr(I),ds(I),dt(I),xr111,xr211, xr121,xr221, xr112,xr212, 
// 					xr122, xr222);
	  }
	}
      }
    }
    else if( domainDimension==1 )
    {
// @PD realArray2[dr,r,x,xya,xr1,xr2,xr0] Range[I] intArray1[i11]
      axis=0;
      const realArray & xya = xy(all,i20,i30,axis);
      if( computeMap )
      {
        x(I,axis)=(1.-dr)*xya(i11) + dr*xya(i11+1);       // @PANS
// 	for( i=base; i<= bound; i++ )
// 	{
// 	  x(i,axis)=INT_1D_ORDER_2(dr(i),xy(i11(i),i20,i30,axis),xy(i12(i),i20,i30,axis));
// 	}
      }
      if( computeMapDerivative )
      {
//	realArray xr1(I),xr2(I);
	for( int dir=0; dir<domainDimension; dir++ )
	{
	    
	  real d12=.5*delta[0];
	  is[0]=1;

	  // approximate dx/dr by a centred difference
// 	  xr1(I)=(xya(i11(I)  +is1)-xya(i11(I)  -is1))*d12,  
// 	  xr2(I)=(xya(i11(I)+1+is1)-xya(i11(I)+1-is1))*d12;

//           ::display(xy,"xy");
//           ::display(xr1,"xr1");
//           ::display(xr2,"xr2");
	  
// 	  int iStart=base;
// 	  if( i11(base)==dimension(Start,dir) )
// 	    iStart++;
// 	  int iEnd  = bound;
// 	  if( (i11(bound)+1)==dimension(End,dir) )
// 	    iEnd--;
// 	  if( iStart>base )
// 	  { // use a one side approximation at the end
// 	    i=base;
// 	    xr1(i)=(xy(i11(i)  +is1,i20,i30,axis)-xy(i11(i)  ,i20,i30,axis))*d12*2.;
// 	    xr2(i)=(xy(i11(i)+1+is1,i20,i30,axis)-xy(i11(i)+1,i20,i30,axis))*d12*2.;
// 	  }
// 	  if( iEnd<bound )
// 	  {
// 	    i=bound;
// 	    xr1(i)=(xy(i11(i)  ,i20,i30,axis)-xy(i11(i)  -is1,i20,i30,axis))*d12*2.;
// 	    xr2(i)=(xy(i11(i)+1,i20,i30,axis)-xy(i11(i)+1-is1,i20,i30,axis))*d12*2.;
// 	  }

          const realArray & xr1 = xr(I,axis,dir); 
	  realArray & xr0 = (realArray &)xr1;
	  xr0=((1.-dr)*(xya(i11(I)+is1)-xya(i11(I)-is1))+dr*(xya(i11(I)+1+is1)-xya(i11(I)+1-is1)))*d12;    // @PANS
	}
      }
    }
    else
    {
      printf("DPM:ERROR: unknown domainDimension\n");
      throw "error";
    }
    

  }
  else if( orderOfInterpolation==4 )
  {
    // do not compile on sgi with egcs
#if defined(__gcc) && defined(__sgi)
   cout << "DataPointMapping::map:ERROR - orderOfInterpolation==4 not available \n";
   {throw "error";}
#else    
    //..........cubic interpolation (4 points)
    //    (i11,i12,i13) = closest point less than r minus one

   realArray a0,a0r,a1,a1r,a2,a2r,a3,a3r;
   realArray b0,b0r,b1,b1r,b2,b2r,b3,b3r;
   realArray c0,c0r,c1,c1r,c2,c2r,c3,c3r;

    a0.redim(I);     a0r.redim(I);       a1.redim(I);     a1r.redim(I); 
    a2.redim(I);     a2r.redim(I);       a3.redim(I);     a3r.redim(I); 
    b0.redim(I);     b0r.redim(I);       b1.redim(I);     b1r.redim(I); 
    b2.redim(I);     b2r.redim(I);       b3.redim(I);     b3r.redim(I); 
    if( domainDimension>2 )
    {
      c0.redim(I);     c0r.redim(I);       c1.redim(I);     c1r.redim(I); 
      c2.redim(I);     c2r.redim(I);       c3.redim(I);     c3r.redim(I); 
    }


    a0(I)=q03(dr(I));
    a1(I)=q13(dr(I));
    a2(I)=q23(dr(I));
    a3(I)=q33(dr(I));
    b0(I)=q03(ds(I));
    b1(I)=q13(ds(I));
    b2(I)=q23(ds(I));
    b3(I)=q33(ds(I));
    a0r(I)=q03d(dr(I))*delta[axis1];
    a1r(I)=q13d(dr(I))*delta[axis1];
    a2r(I)=q23d(dr(I))*delta[axis1];
    a3r(I)=q33d(dr(I))*delta[axis1];
    b0r(I)=q03d(ds(I))*delta[axis2];
    b1r(I)=q13d(ds(I))*delta[axis2];
    b2r(I)=q23d(ds(I))*delta[axis2];
    b3r(I)=q33d(ds(I))*delta[axis2];
    if( domainDimension==2 )
    {
      for( axis=axis1; axis<rangeDimension; axis++ )
      {
        if( computeMap )
	{
          x(I,axis)=q2x(i11(I),i21(I),i30,axis);
	}
        if( computeMapDerivative )
        {
          xr(I,axis,axis1)=q2xr(i11(I),i21(I),i30,axis);
          xr(I,axis,axis2)=q2xs(i11(I),i21(I),i30,axis);
	}
      }
    }
    else if( domainDimension==3 )
    {
      c0(I)=q03(dt(I));
      c1(I)=q13(dt(I));
      c2(I)=q23(dt(I));
      c3(I)=q33(dt(I));
      c0r(I)=q03d(dt(I))*delta[axis3];
      c1r(I)=q13d(dt(I))*delta[axis3];
      c2r(I)=q23d(dt(I))*delta[axis3];
      c3r(I)=q33d(dt(I))*delta[axis3];
      for( axis=axis1; axis<rangeDimension; axis++ )
      {
        if( computeMap )
	{
          x(I,axis)=q3x(i11(I),i21(I),i31(I),axis);
	}
        if( computeMapDerivative )
        {
          xr(I,axis,axis1)=q3xr(i11(I),i21(I),i31(I),axis);
          xr(I,axis,axis2)=q3xs(i11(I),i21(I),i31(I),axis);
          xr(I,axis,axis3)=q3xt(i11(I),i21(I),i31(I),axis);
	}
      }
    }
    else if( domainDimension==1 )
    {
      for( axis=axis1; axis<rangeDimension; axis++ )
      {
        if( computeMap )
	{
          x(I,axis)=q1x(i11(I),i20,i30,axis);
	}
        if( computeMapDerivative )
        {
          xr(I,axis,axis1)=q1xr(i11(I),i20,i30,axis);
          xr(I,axis,axis2)=q1xs(i11(I),i20,i30,axis);
	}
      }
    }
#endif
  }
}

