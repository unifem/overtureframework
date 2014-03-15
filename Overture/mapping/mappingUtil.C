#include "Mapping.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

int Mapping::
determineResolution(int numberOfGridPoints[], 
                    bool collapsedEdge[2][3],
                    real averageArclength[],
                    real elementDensityTolerance /* = .05 */ )
// ====================================================================================================
// Description:
//    Determine the number of grids points that are needed in order to represent the mapping based
//    on the curvature.  
//  
// /numberOfGridPoints[] (output): estimated number of points required.
// /collapsedEdge (output) : collapsedEdge[side][axis]==true if this edge is collapsed to a point.
// /averageArclength (output) : average arclength in each direction.
// /elementDensityTolerance (input) : a tolerance that determines how many points are needed. 
//   The number of points is related to the curvature/elementDensityTolerance. The default
//   value is .05 -- smaller values lead to finer grids.
//
// ====================================================================================================
{
  int debugr=0;
  
  if( domainDimension!=2 || rangeDimension!=3 )
  {
    printf("Mapping::determineResolution:ERROR: only implemented for 3d surfaces\n");
    return 1;
  }
  real time1=getCPU();
  const realArray & x = getGrid();
  real gridTime=getCPU()-time1;
  
  int xBase0 =0;
  int xBound0=getGridDimensions(0)-1;
  int xBase1 =0;
  int xBound1=getGridDimensions(1)-1;
  int xDim0=xBound0-xBase0+1;
  int xDim1=xBound1-xBase1+1;

  // printf("getResolution: initial grid points = (%i,%i). time for getGrid=%8.2e \n",xDim0,xDim1,gridTime);

  real xScale=0.;
  // compute from rangeBound
  int axis;
  for( axis=0; axis<rangeDimension; axis++ )
  {
    real xa=getRangeBound(Start,axis);
    real xb=getRangeBound(End,axis);
    xScale=max(xScale,xb-xa);
  }

  time1=getCPU();
  int i1,i2;
  int numberOfGridPoints0=xBound0-xBase0+1;
  int numberOfGridPoints1=xBound1-xBase1+1;
  
  const real epsx = SQR(FLT_EPSILON*10.*xScale);
  const real sqrtEpsx=sqrt(epsx);

  bool leftCollapsed=false, rightCollapsed=false, bottomCollapsed=false, topCollapsed=false;

  real arcLength=1., curvature=0., maxCurvature=0.;

  // ***first determine an appropriate number of grid lines to use***
  bool useOptimizedC=true;
  if( useOptimizedC )
  {
  const int xyDim0 = x.getRawDataSize(0);
  const int xyDim1 = x.getRawDataSize(1);
  const int xyDim2 = x.getRawDataSize(2);
  real *xyp = x.Array_Descriptor.Array_View_Pointer3;
#define X(i1,i2,i3,m) xyp[(i1)+xyDim0*(i2+xyDim1*(i3+xyDim2*(m)))]



    Range I1(xBase0+1,xBound0-1), I2(xBase1+1,xBound1-1);
    Range I(xBase0+1,xBound0);
    realArray ds(I);
    averageArclength[0]=0;

  real *dsp = ds.Array_Descriptor.Array_View_Pointer0;
#define DS(i) dsp[i]

    int i;
  
    int numberOfLinesToCheck=min(5,xDim1);
    for( int i20=0; i20<numberOfLinesToCheck; i20++ )
    {
      i2=int( xBase1+(xBound1-xBase1)*i20/real(numberOfLinesToCheck-1)+.5 );

//       ds=SQRT( SQR( x(I,i2,0,0)-x(I-1,i2,0,0) ) + 
// 	       SQR( x(I,i2,0,1)-x(I-1,i2,0,1) ) +  
// 	       SQR( x(I,i2,0,2)-x(I-1,i2,0,2) ) );
//    arcLength=sum(ds);
      arcLength=0.;
      const int base0=I.getBase(), bound0=I.getBound();

      assert( rangeDimension==3 );  // fix this
      
      for( i=base0; i<=bound0; i++ )
      {
	DS(i)=sqrt( SQR( X(i,i2,0,0)-X(i-1,i2,0,0) ) + 
		    SQR( X(i,i2,0,1)-X(i-1,i2,0,1) ) +  
		    SQR( X(i,i2,0,2)-X(i-1,i2,0,2) ) );
        arcLength+=DS(i);
      }

      averageArclength[0]+=arcLength;
      // printf(" i2=%i, arcLength=%e, sqrtEpsx=%e\n",i2,arcLength,sqrtEpsx);
      if( arcLength<sqrtEpsx )
      {
	if( i2==xBase1 )
	{
	  bottomCollapsed=true;
	  printf("*** bottom side collapsed\n");
	}
	else if( i2==xBound1 )
	{
	  topCollapsed=true;
	  printf("*** top side collapsed\n");
	}
      }
      else
      {
	arcLength=max(arcLength,xScale*REAL_EPSILON*100.); 
      
	if( xDim0>2 )
	{

	  // ds=1./max(ds,xScale*REAL_EPSILON*100.);
          for( i=base0; i<=bound0; i++ )
	  {
	    DS(i)=1./max(DS(i),xScale*REAL_EPSILON*100.);
	  }
	  
	  // use the approximation 
	  //    u'' =   [ (u(i+1)-u(i))/ds(i+1/2) - (u(i)-u(i-1))/ds(i-1/2) ]/( .5*(ds(i+1/2)+ds(i-1/2) )
	  // use undivided 2nd difference/ds : d2 = u'' * ( .5*(ds(i+1/2)+ds(i-1/2) )
	  //             
// 	  for( int axis=0; axis<rangeDimension; axis++ )
// 	    d2(I1,axis)=((x(I1+1,i2,0,axis)-x(I1  ,i2,0,axis))*ds(I1+1)-
// 			 (x(I1  ,i2,0,axis)-x(I1-1,i2,0,axis))*ds(I1) );

// 	  real curvatureMax = SQRT(max( SQR(d2(I1,0))+SQR(d2(I1,1))+SQR(d2(I1,2)) ));
// 	  real curvatureAve = SQRT( sum( SQR(d2(I1,0))+SQR(d2(I1,1))+SQR(d2(I1,2)) )/(xBound0-xBase0-1) );

          const int base1=I1.getBase(), bound1=I1.getBound();
          real d20,d21,d22,dd;
	  real curvatureMax=0.;
	  real curvatureAve=0.;
	  if( rangeDimension==3 )
	  {
	    for( i=base1; i<=bound1; i++ )
	    {
	      d20=((X(i+1,i2,0,axis1)-X(i  ,i2,0,axis1))*DS(i+1)-
		   (X(i  ,i2,0,axis1)-X(i-1,i2,0,axis1))*DS(i) );
	      d21=((X(i+1,i2,0,axis2)-X(i  ,i2,0,axis2))*DS(i+1)-
		   (X(i  ,i2,0,axis2)-X(i-1,i2,0,axis2))*DS(i) );
	      d22=((X(i+1,i2,0,axis3)-X(i  ,i2,0,axis3))*DS(i+1)-
		   (X(i  ,i2,0,axis3)-X(i-1,i2,0,axis3))*DS(i) );

              dd=d20*d20+d21*d21+d22*d22;
              curvatureMax=max(curvatureMax, dd);
              curvatureAve+=dd;
	    }
            curvatureMax=sqrt(curvatureMax);
	  }
          else if( rangeDimension==2 )
	  {
	    for( i=base1; i<=bound1; i++ )
	    {
	      d20=((X(i+1,i2,0,axis1)-X(i  ,i2,0,axis1))*DS(i+1)-
		   (X(i  ,i2,0,axis1)-X(i-1,i2,0,axis1))*DS(i) );
	      d21=((X(i+1,i2,0,axis2)-X(i  ,i2,0,axis2))*DS(i+1)-
		   (X(i  ,i2,0,axis2)-X(i-1,i2,0,axis2))*DS(i) );

              dd=d20*d20+d21*d21;
              curvatureMax=max(curvatureMax, dd);
              curvatureAve+=dd;
	    }
            curvatureMax=sqrt(curvatureMax);
	  }
	  else
	  {
	    for( i=base1; i<=bound1; i++ )
	    {
	      d20=((X(i+1,i2,0,axis1)-X(i  ,i2,0,axis1))*DS(i+1)-
		   (X(i  ,i2,0,axis1)-X(i-1,i2,0,axis1))*DS(i) );
              dd=d20*d20;
              curvatureMax=max(curvatureMax, dd);
              curvatureAve+=dd;
	    }
            curvatureMax=sqrt(curvatureMax);
	  }
	  
	  curvatureAve /= (xBound0-xBase0-1);

	  curvature=.5*curvatureMax+.5*curvatureAve;
	
	  if( debugr & 2 ) printf(" i2=%i, arcLength=%e, curvature/arc=%e\n",i2,arcLength,curvature);

	  maxCurvature= max( maxCurvature,curvature );
	}
      }
      
    }
    averageArclength[0]/=numberOfLinesToCheck; // (xBound1-xBase1+1);
  
    if( debugr & 2 ) 
      printf("determineResolution: dir 1: maxCurvature=%e, elementDensityTolerance=%e\n",maxCurvature,
	     elementDensityTolerance);
    // numberOfGridPoints0 = int( SQRT( maxCurvature/elementDensityTolerance ) * (xBound0-xBase0+1)+.5 );
    numberOfGridPoints0 = int( 4*maxCurvature/elementDensityTolerance );
    numberOfGridPoints0 = max(4,numberOfGridPoints0); // ***** 3 ****
  
    I=Range(xBase1+1,xBound1);
    ds.redim(I);
// need to reset the pointer!
    dsp = ds.Array_Descriptor.Array_View_Pointer0;
    maxCurvature=0.;
    averageArclength[1]=0.;
    // for( i1=xBase0; i1<=xBound0; i1++ )
    numberOfLinesToCheck=min(5,xDim0);
    for( int i10=0; i10<numberOfLinesToCheck; i10++ )
    {
      i1=int( xBase1+(xBound0-xBase0)*i10/real(numberOfLinesToCheck-1)+.5 );

//       ds(0,I)=SQRT( SQR( x(i1,I,0,0)-x(i1,I-1,0,0) ) + 
// 		    SQR( x(i1,I,0,1)-x(i1,I-1,0,1) ) +  
// 		    SQR( x(i1,I,0,2)-x(i1,I-1,0,2) ) );
//       arcLength=sum(ds);

      arcLength=0.;
      const int base0=I.getBase(), bound0=I.getBound();
      for( i=base0; i<=bound0; i++ )
      {
	DS(i)=sqrt( SQR( X(i1,i,0,0)-X(i1,i-1,0,0) ) + 
		    SQR( X(i1,i,0,1)-X(i1,i-1,0,1) ) +  
		    SQR( X(i1,i,0,2)-X(i1,i-1,0,2) ) );
        arcLength+=DS(i);
      }

      // printf(" i1=%i, arcLength=%e\n",i1,arcLength);
      averageArclength[1]+=arcLength;
      if( arcLength<sqrtEpsx )
      {
	if( i1==xBase0 )
	{
	  leftCollapsed=true;
	  if( Mapping::debug & 4 ) printf("*** determineResolution:left side collapsed\n");
	}
	else if( i1==xBound0 )
	{
	  rightCollapsed=true;
	  if( Mapping::debug & 4 ) printf("*** determineResolution:right side collapsed\n");
	}
      }
      else
      {
	arcLength=max(arcLength,xScale*REAL_EPSILON*100.); 

	if( xDim1>2 )
	{
//	  ds=1./max(ds,xScale*REAL_EPSILON*100.);
          for( i=base0; i<=bound0; i++ )
	  {
	    DS(i)=1./max(DS(i),xScale*REAL_EPSILON*100.);
	  }

// 	  for( int axis=0; axis<rangeDimension; axis++ )
// 	    d2(0,I2,axis)=((x(i1,I2+1,0,axis)-x(i1,I2  ,0,axis))*ds(0,I2+1)-
// 			   (x(i1,I2  ,0,axis)-x(i1,I2-1,0,axis))*ds(0,I2) );
	

// 	  real curvatureMax = SQRT(max( SQR(d2(0,I2,0))+SQR(d2(0,I2,1))+SQR(d2(0,I2,2)) ));
// 	  real curvatureAve = SQRT( sum( SQR(d2(0,I2,0))+SQR(d2(0,I2,1))+SQR(d2(0,I2,2)) )/(xBound1-xBase1-1) );

          const int base1=I2.getBase(), bound1=I2.getBound();
          real d20,d21,d22,dd;
	  real curvatureMax=0.;
	  real curvatureAve=0.;
	  if( rangeDimension==3 )
	  {
	    for( i=base1; i<=bound1; i++ )
	    {
	      d20=((X(i1,i+1,0,axis1)-X(i1,i  ,0,axis1))*DS(i+1)-
		   (X(i1,i  ,0,axis1)-X(i1,i-1,0,axis1))*DS(i) );
	      d21=((X(i1,i+1,0,axis2)-X(i1,i  ,0,axis2))*DS(i+1)-
		   (X(i1,i  ,0,axis2)-X(i1,i-1,0,axis2))*DS(i) );
	      d22=((X(i1,i+1,0,axis3)-X(i1,i  ,0,axis3))*DS(i+1)-
		   (X(i1,i  ,0,axis3)-X(i1,i-1,0,axis3))*DS(i) );

              dd=d20*d20+d21*d21+d22*d22;
              curvatureMax=max(curvatureMax, dd);
              curvatureAve+=dd;
	    }
            curvatureMax=sqrt(curvatureMax);
	  }
          else if( rangeDimension==2 )
	  {
	    for( i=base1; i<=bound1; i++ )
	    {
	      d20=((X(i1,i+1,0,axis1)-X(i1,i  ,0,axis1))*DS(i+1)-
		   (X(i1,i  ,0,axis1)-X(i1,i-1,0,axis1))*DS(i) );
	      d21=((X(i1,i+1,0,axis2)-X(i1,i  ,0,axis2))*DS(i+1)-
		   (X(i1,i  ,0,axis2)-X(i1,i-1,0,axis2))*DS(i) );

              dd=d20*d20+d21*d21;
              curvatureMax=max(curvatureMax, dd);
              curvatureAve+=dd;
	    }
            curvatureMax=sqrt(curvatureMax);
	  }
	  else
	  {
	    for( i=base1; i<=bound1; i++ )
	    {
	      d20=((X(i1,i+1,0,axis1)-X(i1,i  ,0,axis1))*DS(i+1)-
		   (X(i1,i  ,0,axis1)-X(i1,i-1,0,axis1))*DS(i) );
              dd=d20*d20;
              curvatureMax=max(curvatureMax, dd);
              curvatureAve+=dd;
	    }
            curvatureMax=sqrt(curvatureMax);
	  }
	  
	  curvatureAve /= (xBound1-xBase1-1);

	  curvature=.5*curvatureMax+.5*curvatureAve;

	  if( debugr & 2 ) printf(" i1=%i, arcLength=%e, curvature/arc=%e, max=%e\n",i1,arcLength,curvature,maxCurvature);

	  maxCurvature= max( maxCurvature,curvature );
	}
      }
    }
    averageArclength[1]/=numberOfLinesToCheck; // (xBound0-xBase0+1);

  }
  else
  { // *** A++ version ****
#if 0
    Range I1(xBase0+1,xBound0-1), I2(xBase1+1,xBound1-1);
    Range I(xBase0+1,xBound0);

    realArray d2(I1,Range(rangeDimension)), ds(I);
    averageArclength[0]=0;

  // for( i2=xBase1; i2<=xBound1; i2++ )
    int numberOfLinesToCheck=min(5,xDim1);
    for( int i20=0; i20<numberOfLinesToCheck; i20++ )
    {
      i2=int( xBase1+(xBound1-xBase1)*i20/real(numberOfLinesToCheck-1)+.5 );

      ds=SQRT( SQR( x(I,i2,0,0)-x(I-1,i2,0,0) ) + 
	       SQR( x(I,i2,0,1)-x(I-1,i2,0,1) ) +  
	       SQR( x(I,i2,0,2)-x(I-1,i2,0,2) ) );
    
      arcLength=sum(ds);

      averageArclength[0]+=arcLength;
      // printf(" i2=%i, arcLength=%e, sqrtEpsx=%e\n",i2,arcLength,sqrtEpsx);
      if( arcLength<sqrtEpsx )
      {
	if( i2==xBase1 )
	{
	  bottomCollapsed=true;
	  if( Mapping::debug & 4 ) printf("*** bottom side collapsed\n");
	}
	else if( i2==xBound1 )
	{
	  topCollapsed=true;
	  if( Mapping::debug & 4 ) printf("*** top side collapsed\n");
	}
      }
      else
      {
	arcLength=max(arcLength,xScale*REAL_EPSILON*100.); 
      
	if( xDim0>2 )
	{
// 	for( int axis=0; axis<rangeDimension; axis++ )
// 	  d2(I1,axis)=x(I1+1,i2,0,axis)-2.*x(I1,i2,0,axis)+x(I1-1,i2,0,axis);
// 	curvature = SQRT(max( SQR(d2(I1,0))+SQR(d2(I1,1))+SQR(d2(I1,2)) ))/ arcLength;

	  ds=1./max(ds,xScale*REAL_EPSILON*100.);

	  // use the approximation 
	  //    u'' =   [ (u(i+1)-u(i))/ds(i+1/2) - (u(i)-u(i-1))/ds(i-1/2) ]/( .5*(ds(i+1/2)+ds(i-1/2) )
	  // use undivided 2nd difference/ds : d2 = u'' * ( .5*(ds(i+1/2)+ds(i-1/2) )
	  //             
	  for( int axis=0; axis<rangeDimension; axis++ )
	    d2(I1,axis)=((x(I1+1,i2,0,axis)-x(I1  ,i2,0,axis))*ds(I1+1)-
			 (x(I1  ,i2,0,axis)-x(I1-1,i2,0,axis))*ds(I1) );

	  real curvatureMax = SQRT(max( SQR(d2(I1,0))+SQR(d2(I1,1))+SQR(d2(I1,2)) ));
	  real curvatureAve = SQRT( sum( SQR(d2(I1,0))+SQR(d2(I1,1))+SQR(d2(I1,2)) )/(xBound0-xBase0-1) );
	  curvature=.5*curvatureMax+.5*curvatureAve;
	
	  if( debugr & 2 ) printf(" i2=%i, arcLength=%e, curvature/arc=%e\n",i2,arcLength,curvature);

	  maxCurvature= max( maxCurvature,curvature );
	}
      }
      
    }
    averageArclength[0]/=numberOfLinesToCheck; // (xBound1-xBase1+1);
  
    if( debugr & 2 ) 
      printf("determineResolution: dir 1: maxCurvature=%e, elementDensityTolerance=%e\n",maxCurvature,
	     elementDensityTolerance);
    // numberOfGridPoints0 = int( SQRT( maxCurvature/elementDensityTolerance ) * (xBound0-xBase0+1)+.5 );
    numberOfGridPoints0 = int( 4*maxCurvature/elementDensityTolerance );
    numberOfGridPoints0 = max(4,numberOfGridPoints0); // ***** 3 ****
  
    I=Range(xBase1+1,xBound1);
    d2.redim(1,I2,Range(rangeDimension));
    ds.redim(1,I);
// need to reset the pointer!
    dsp = ds.Array_Descriptor.Array_View_Pointer0;
    maxCurvature=0.;
    averageArclength[1]=0.;
    // for( i1=xBase0; i1<=xBound0; i1++ )
    numberOfLinesToCheck=min(5,xDim0);
    for( int i10=0; i10<numberOfLinesToCheck; i10++ )
    {
      i1=int( xBase1+(xBound0-xBase0)*i10/real(numberOfLinesToCheck-1)+.5 );

      ds(0,I)=SQRT( SQR( x(i1,I,0,0)-x(i1,I-1,0,0) ) + 
		    SQR( x(i1,I,0,1)-x(i1,I-1,0,1) ) +  
		    SQR( x(i1,I,0,2)-x(i1,I-1,0,2) ) );
      arcLength=sum(ds);

      // printf(" i1=%i, arcLength=%e\n",i1,arcLength);
      averageArclength[1]+=arcLength;
      if( arcLength<sqrtEpsx )
      {
	if( i1==xBase0 )
	{
	  leftCollapsed=true;
	  printf("*** determineResolution:left side collapsed\n");
	}
	else if( i1==xBound0 )
	{
	  rightCollapsed=true;
	  printf("*** determineResolution:right side collapsed\n");
	}
      }
      else
      {
	arcLength=max(arcLength,xScale*REAL_EPSILON*100.); 

	if( xDim1>2 )
	{
	  ds=1./max(ds,xScale*REAL_EPSILON*100.);
//	ds(0,I2)=ds(0,I2)/ds(0,I2+1);

// 	for( int axis=0; axis<rangeDimension; axis++ )
// 	  d2(0,I2,axis)=x(i1,I2+1,0,axis)-2.*x(i1,I2,0,axis)+x(i1,I2-1,0,axis);
// 	curvature = SQRT(max( SQR(d2(0,I2,0))+SQR(d2(0,I2,1))+SQR(d2(0,I2,2)) ))/ arcLength;

	  for( int axis=0; axis<rangeDimension; axis++ )
	    d2(0,I2,axis)=((x(i1,I2+1,0,axis)-x(i1,I2  ,0,axis))*ds(0,I2+1)-
			   (x(i1,I2  ,0,axis)-x(i1,I2-1,0,axis))*ds(0,I2) );
	

	  real curvatureMax = SQRT(max( SQR(d2(0,I2,0))+SQR(d2(0,I2,1))+SQR(d2(0,I2,2)) ));
	  real curvatureAve = SQRT( sum( SQR(d2(0,I2,0))+SQR(d2(0,I2,1))+SQR(d2(0,I2,2)) )/(xBound1-xBase1-1) );
	  curvature=.5*curvatureMax+.5*curvatureAve;

	  if( debugr & 2 ) printf(" i1=%i, arcLength=%e, curvature/arc=%e, max=%e\n",i1,arcLength,curvature,maxCurvature);

	  maxCurvature= max( maxCurvature,curvature );
	}
      }
    }
    averageArclength[1]/=numberOfLinesToCheck; // (xBound0-xBase0+1);

#endif
  }
  
  if( debugr & 2 ) 
    printf("determineResolution: dir 2: maxCurvature=%e, elementDensityTolerance=%e\n",maxCurvature,
           elementDensityTolerance);
//  numberOfGridPoints1 = int( SQRT( maxCurvature/elementDensityTolerance ) * (xBound1-xBase1+1) +.5 );
  numberOfGridPoints1 = int( 4*maxCurvature/elementDensityTolerance );
  numberOfGridPoints1 = max(4,numberOfGridPoints1);
  
  if( debugr & 2 )
    printf("determineResolution: current=(%i,%i), Estimated number of grid points needed = (%i,%i)\n",
	   xBound0-xBase0+1,xBound1-xBase1+1,numberOfGridPoints0,numberOfGridPoints1);

    
  collapsedEdge[0][0]=leftCollapsed;
  collapsedEdge[1][0]=rightCollapsed;
  collapsedEdge[0][1]=bottomCollapsed;
  collapsedEdge[1][1]=topCollapsed;
  if( domainDimension==3 )
  {
    collapsedEdge[0][2]=false;
    collapsedEdge[1][2]=false;
  }

  // restrict the average cell spacings to have a maximum aspect ratio.
  real maximumAspectRatio=5.;
  real dx = averageArclength[0]/numberOfGridPoints0;
  real dy = averageArclength[1]/numberOfGridPoints1;
  
  
  if( dx/dy > maximumAspectRatio )
  {
    numberOfGridPoints0=int( numberOfGridPoints0*dx/(dy*maximumAspectRatio)+.5);
  }
  else if( dy/dx > maximumAspectRatio )
  {
    numberOfGridPoints1=int( numberOfGridPoints1*dy/(dx*maximumAspectRatio)+.5);
  }
  if( debugr & 2 )
    printf(" determineResolution: **** aspect ratio dx/dy = %e. Adjusted grid points=(%i,%i) \n",dx/dy,
	   numberOfGridPoints0,numberOfGridPoints1 );
  
  numberOfGridPoints[0]=numberOfGridPoints0;
  numberOfGridPoints[1]=numberOfGridPoints1;
  
  if( debugr & 2 ) 
     printf("getResolution: time for getGrid=%8.2e, time for arc/curvature=%8.2e \n",gridTime,getCPU()-time1);

  return 0;
}
#undef X


real Mapping::
getArcLength()
// =====================================================================================
/// \details 
///     Return the arcLength for a curve, otherwise return -1.
///   Compute the arclength using the current values for the grid dimensions (i.e. the current
///   size of the grid).
// =====================================================================================
{
  if( domainDimension==1 )
  {
    if( arcLength<0. )
    {
      // compute the arcLength

      const realArray & x = getGrid();

      const int xyDim0 = x.getRawDataSize(0);
      real *xyp = x.Array_Descriptor.Array_View_Pointer3;

#define X(i1,m) xyp[(i1)+xyDim0*(m)]

      const int base0=x.getBase(0), bound0=x.getBound(0);

      arcLength=0.;
      if( rangeDimension==2 )
      {
	for( int i=base0+1; i<=bound0; i++ )
	{
	  arcLength+=sqrt( SQR( X(i,0)-X(i-1,0) ) + 
			   SQR( X(i,1)-X(i-1,1) ) );
	}
      }
      else if( rangeDimension==3 )
      {
	for( int i=base0+1; i<=bound0; i++ )
	{
	  arcLength+=sqrt( SQR( X(i,0)-X(i-1,0) ) + 
			   SQR( X(i,1)-X(i-1,1) ) +  
			   SQR( X(i,2)-X(i-1,2) ) );
	}
      }
      else if( rangeDimension==1 )
      {
	for( int i=base0+1; i<=bound0; i++ )
	{
	  arcLength+=fabs(X(i,0)-X(i-1,0));
	}
      }
      else
      {
	printf("Mapping::getArcLength:ERROR: unknown rangeDimension=%i !\n",rangeDimension);
        arcLength=-1.;
      }
    }
    return arcLength;
  }
  else
  {
    return -1.;
  }
}
#undef X

void Mapping::
setArcLength(real length)
// =====================================================================================
/// \details 
///     Set the arcLength for a curve. Set to -1. to force the arclength to be recomputed
///   on the next call to getArcLength.
// =====================================================================================
{
  arcLength=length;
}


int Mapping::
getGridMinAndMax(const realArray & u, const Range & R1, const Range & R2, const Range & R3, 
                 real uMin[3], real uMax[3], bool local /* =false */ )
// =================================================================================================
/// \details 
///     Compute the min and max values of an array holding the grid points.
///        uMin[axis] = min( u(R1,R2,R3,axis)  )
///        uMax[axis] = max( u(R1,R2,R3,axis)  )
///  
/// \param u (input):
/// \param R[3] (input): range of values to use.
/// \param uMin (output) : array of minimum values.
/// \param uMax (output) : array of maximum values.
/// \param local (input) : if local=true then only compute the min and max over points on this processor, otherwise
///                   compute the min and max over all points on all processors
///  
// ================================================================================================
{

  const int rangeDimension=u.getLength(3);
  
  for( int axis=0; axis<rangeDimension; axis++ )  
  {
    uMin[axis]=FLT_MAX;
    uMax[axis]=-FLT_MAX;
  }
  
  #ifdef USE_PPP
    realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
  #else
    const realSerialArray & uLocal = u;
  #endif

  Index I1=R1, I2=R2, I3=R3;
  if( I1==nullRange ) I1=u.dimension(0);
  if( I2==nullRange ) I2=u.dimension(1);
  if( I3==nullRange ) I3=u.dimension(2);
  
  bool ok = true;
  #ifdef USE_PPP
    int includeGhost=1;
    ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
  #endif
  if( ok )
  {
    real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
    const int uDim0=uLocal.getRawDataSize(0);
    const int uDim1=uLocal.getRawDataSize(1);
    const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

    for( int axis=0; axis<rangeDimension; axis++ ) 
    {
      int i1,i2,i3;
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	uMin[axis]=min(uMin[axis],U(i1,i2,i3,axis));
	uMax[axis]=max(uMax[axis],U(i1,i2,i3,axis));
      }
    } 
#undef U
  }
  
 // compute min and max over all processors
 #ifdef USE_PPP
  if( !local )
  {
    ParallelUtility::getMinValues(uMin,uMin,rangeDimension);
    ParallelUtility::getMaxValues(uMax,uMax,rangeDimension);
  }
 #endif

 return 0;

}

#ifdef USE_PPP
// SERIAL array version

int Mapping::
getGridMinAndMax(const RealArray & u, const Range & R1, const Range & R2, const Range & R3, 
                 real uMin[3], real uMax[3], bool local  /* = false */ )
// =================================================================================================
// /Description:
//    Compute the min and max values of an array holding the grid points.
//       uMin[axis] = min( u(R1,R2,R3,axis)  )
//       uMax[axis] = max( u(R1,R2,R3,axis)  )
// 
// /u (input):
// /R[3] (input): range of values to use.
// /uMin (output) : array of minimum values.
// /uMax (output) : array of maximum values.
// /local (input) : if local=true then only compute the min and max over points on this processor, otherwise
//                  compute the min and max over all points on all processors
// 
//\end{MappingInclude.tex}
// ================================================================================================
{
  const int rangeDimension=u.getLength(3);
  
  for( int axis=0; axis<rangeDimension; axis++ )  
  {
    uMin[axis]=FLT_MAX;
    uMax[axis]=-FLT_MAX;
  }
  
  Index I1=R1, I2=R2, I3=R3;
  if( I1==nullRange ) I1=u.dimension(0);
  if( I2==nullRange ) I2=u.dimension(1);
  if( I3==nullRange ) I3=u.dimension(2);
  real *up = u.Array_Descriptor.Array_View_Pointer3;
  const int uDim0=u.getRawDataSize(0);
  const int uDim1=u.getRawDataSize(1);
  const int uDim2=u.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

  int i1,i2,i3;
  for( int axis=0; axis<rangeDimension; axis++ ) 
  {
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      uMin[axis]=min(uMin[axis],U(i1,i2,i3,axis));
      uMax[axis]=max(uMax[axis],U(i1,i2,i3,axis));
    }
  } 
#undef U
  

 return 0;

}

#endif
