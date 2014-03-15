#include "Mapping.h"
#include <float.h>

// realArray floor2( const realArray & r );   // ***** until the A+= floor get fixed

inline real 
determinant( realSerialArray & xt, const int domainDimension )
{
  if( domainDimension==1 )
    return xt(0,0,0);
  else if( domainDimension==2 )
    return xt(0,0,0)*xt(0,1,1)-xt(0,1,0)*xt(0,0,1);
  else
    return (xt(0,0,1)*xt(0,1,2)-xt(0,1,1)*xt(0,0,2))*xt(0,2,0)
          +(xt(0,0,2)*xt(0,1,0)-xt(0,1,2)*xt(0,0,0))*xt(0,2,1)
	  +(xt(0,0,0)*xt(0,1,1)-xt(0,1,0)*xt(0,0,1))*xt(0,2,2);
}
inline void 
invert( const int domainDimension, const real det, realSerialArray & xt, realSerialArray & xti )
{
  if( domainDimension==1 )
    xti(0,0,0)=1./det;
  else if(domainDimension==2 )
  {
    xti(0,0,0)= xt(0,1,1)/det;
    xti(0,1,0)=-xt(0,1,0)/det;
    xti(0,0,1)=-xt(0,0,1)/det;
    xti(0,1,1)= xt(0,0,0)/det;
  }
  else
  {
    xti(0,0,0)=(xt(0,1,1)*xt(0,2,2)-xt(0,1,2)*xt(0,2,1))/det;
    xti(0,1,0)=(xt(0,1,2)*xt(0,2,0)-xt(0,1,0)*xt(0,2,2))/det;
    xti(0,2,0)=(xt(0,1,0)*xt(0,2,1)-xt(0,1,1)*xt(0,2,0))/det;
    xti(0,0,1)=(xt(0,2,1)*xt(0,0,2)-xt(0,2,2)*xt(0,0,1))/det;
    xti(0,1,1)=(xt(0,2,2)*xt(0,0,0)-xt(0,2,0)*xt(0,0,2))/det;
    xti(0,2,1)=(xt(0,2,0)*xt(0,0,1)-xt(0,2,1)*xt(0,0,0))/det;
    xti(0,0,2)=(xt(0,0,1)*xt(0,1,2)-xt(0,0,2)*xt(0,1,1))/det;
    xti(0,1,2)=(xt(0,0,2)*xt(0,1,0)-xt(0,0,0)*xt(0,1,2))/det;
    xti(0,2,2)=(xt(0,0,0)*xt(0,1,1)-xt(0,0,1)*xt(0,1,0))/det;
  }
  
}


//=============================================================================
// Check the mapping, return 0 if ok
//
//  Check directions that are claimed to be periodic
//  Check the jacobian derivatives returned my map using finite differences
//  Check the derivatives returned by map and inverseMap for consistency
//  Check for the det(jacobian) bounded away from zero
//
//  Who to blame:
//   Bill Henshaw (..follows cgckmp and cgckcv)
//=============================================================================
int Mapping::
checkMapping()
{
  int returnValue=0;

  printf("checkMapping2: check properties of Mapping:%s \n",(const char*)getName(mappingName));
  
  real bigReal=REAL_MAX;  
  real epsilon=REAL_EPSILON;
  const int iters=10;
  const real hmax=1./128.;
  const real toler=100.;   // tolerance factor above the value of epsilon
  

  Index Axes(0,domainDimension);
  Index xAxes(0,rangeDimension);
  
  int axis,dir;
  real detmin=bigReal;
  real detmax=-detmin;
  real h=min(hmax,pow(epsilon/20.,1./5.)); // optimal h for 4th order differences
  real d=SQR(h);
  bool badinv=FALSE;
  bool badjac=FALSE;
  bool singular=FALSE;
  IntegerArray iv(3),imin(3),imax(3);  imax=1; imin=1;
  int & i1 = iv(axis1);
  int & i2 = iv(axis2);
  int & i3 = iv(axis3);
  realSerialArray badin(1,domainDimension);                badin=0.;
  IntegerArray bad(1,rangeDimension,domainDimension);    bad=FALSE;
  realSerialArray badi(1,rangeDimension,domainDimension);  badi=0.;
  realSerialArray badj(1,rangeDimension,domainDimension);  badj=0.;
  realSerialArray dxmx(1,rangeDimension,domainDimension); dxmx=0.;

  realSerialArray dr(1,3),dt(1,3),r(1,3),t(1,3),x(1,3),xr(1,3,3),rx(1,3,3);
  realSerialArray rA(10,3),xA(10,3),xrA(10,3,3),tA(10,3),rxA(10,3,3);
  
  IntegerArray dimension(3);  dimension=1;
  dimension(Axes)=int(32/pow(2,domainDimension)); // number of points to use, 16, 8 or 4
                   
//   if( FALSE )
//   {
//     // use dimensions from the grid
//     for( axis=axis1; axis<domainDimension; axis++ )
//       dimension(axis)=getGridDimensions(axis)*2;
//   }

  for( axis=axis1; axis<=axis3; axis++ )
    dr(0,axis)=1./dimension(axis);

  realSerialArray tt(1,3),t2(2,3),t4(4,3),xx(4,3),xxtt(4,3,3),xt(1,3,3),dx(1,3,3),tx(1,3,3),xti(1,3,3),txi(1,3,3);
  real anorm,det;
  bool isInvertible = getInvertible();
  
  // --- make some checks to test if the mapping is properly implemented
  // ...First check map
  r=.5; t=.5; x=bigReal; xr=bigReal;
  mapS( r,x,xr );
  if( max(x(0,xAxes))==bigReal )
  {
    printf("CheckMapping:Error map( r,x,xr ) does not compute x properly \n"
           " ...some elements have not been assigned Values! \n");
    returnValue+=1;
  }
  if( max(abs(r-t))!=0. )
  {
    printf("CheckMapping:Error map( r,x,xr ) overwrites r \n");
    returnValue+=1;
  }
  if( max(xr(0,xAxes,Axes))==bigReal )
  {
    printf("CheckMapping:Error map( r,x,xr ) does not compute xr properly\n"
           " ...some elements have not been assigned Values! \n");
    returnValue+=1;
  }
  r=t;  x=bigReal;
  mapS( r,x );
  if( max(x(0,xAxes))==bigReal )
  {
    printf("CheckMapping:Error map( r,x ) does not compute x properly\n"
           " ...some elements have not been assigned Values! \n");
    returnValue+=1;
  }
  xr=bigReal;
  mapS( r,Overture::nullRealArray(),xr );
  if( max(xr(0,xAxes,Axes))==bigReal )
  {
    printf("CheckMapping:Error map( r,Overture::nullRealArray(),xr ) does not compute xr properly \n"
           " ...some elements have not been assigned Values! \n");
    returnValue+=1;
  }
  
  mapS( r,x );
  if( isInvertible )
  {
    // -- now check inverseMap: 
    t=x; r=bigReal;  rx=bigReal;
    inverseMapS( x,r,rx );
    if( max(abs(x-t))!=0. )
    {
      printf("CheckMapping:Error inverseMap( x,r,rx ) overwrites x \n");
      returnValue+=1;
    }
    if( max(r(0,Axes))==bigReal )
    {
      printf("CheckMapping:Error inverseMap( x,r,rx ) does not compute r properly\n"
	     " ...some elements have not been assigned Values! \n");
      returnValue+=1;
    }
    if( max(rx(0,Axes,xAxes))==bigReal )
    {
      printf("CheckMapping:Error inverseMap( x,r,rx ) does not compute rx properly \n"
	     " ...some elements have not been assigned Values! \n");
      returnValue+=1;
    }

    x=t; r=bigReal;
    inverseMapS( x,r );
    if( max(abs(x-t))!=0. )
    {
      printf("CheckMapping:Error inverseMap( x,r ) overwrites x \n");
      returnValue+=1;
    }
    if( max(r(0,Axes))==bigReal )
    {
      printf("CheckMapping:Error inverseMap( x,r ) does not compute r properly \n"
	     " ...some elements have not been assigned Values! \n");
      returnValue+=1;
    }

    x=t;  rx=bigReal;
    inverseMapS( x,Overture::nullRealArray(),rx );
    if( max(abs(x-t))!=0. )
    {
      printf("CheckMapping:Error inverseMap( x,Overture::nullRealArray(),rx ) overwrites x \n");
      returnValue+=1;
    }
    if( max(rx(0,Axes,xAxes))==bigReal )
    {
      printf("CheckMapping:Error inverseMap( x,Overture::nullRealArray(),rx ) does not compute rx properly \n"
	     " ...some elements have not been assigned Values! \n");
      returnValue+=1;
    }
  }
  
  // Check forward and inverse maps with an array of values
  Range R(0,9);
  for( int i=0; i<10; i++ )
    rA(i,Axes)=(i+1)/11.;  

  mapS(rA,xA,xrA);
  tA=bigReal;
  if( isInvertible )
  {
    inverseMapS(xA,tA,rxA);
    real diff = max(fabs(tA(R,Axes)-rA(R,Axes)));  

    if( diff > epsilon*10. )
    {
      if( diff < FLT_EPSILON*10. )
      {
	printf("CheckMapping:WARNING in evaluating an array of points, error in "
	       "inverseMap(map) =%e\n",diff);
      }
      else
      {
	returnValue+=1;
	printf("CheckMapping:ERROR in evaluating an array of points, error in "
	       "inverseMap(map) =%e\n",diff);
	rA(R,Axes).display("Here is r");
	xA(R,xAxes).display("Here is map(r)");
	tA(R,Axes).display("Here is inverseMap(map(r))");
      }
    }
  }
  

  // ---Check periodicity
  for( axis=axis1; axis<domainDimension; axis++ )
  {
    if( getIsPeriodic(axis) )
    {
      if( getBoundaryCondition(Start,axis)>=0 || getBoundaryCondition(Start,axis)>=0 )
      {
        returnValue+=1;

        printf("Error: Mapping is periodic for axis=%i, but\n",axis);
	printf("       boundary conditions are not both negative: (%i,%i)\n",
	       getBoundaryCondition(Start,axis),getBoundaryCondition(End,axis));
      }
      t2=.5;
      t2(0,axis)=0.;  // evaluate mapping at left and right edge
      t2(1,axis)=1.;
      xx=0.;
      mapS( t2,xx,xxtt );
      real d=max( abs(xx(1,xAxes)-xx(0,xAxes)) );
      real dd=max( abs(xxtt(1,xAxes,Axes)-xxtt(0,xAxes,Axes)) );
      real xNorm = max(REAL_MIN*100,max(fabs(xx)));
      anorm=SQRT(sum(SQR(xxtt(0,xAxes,Axes))));
      if( getIsPeriodic(axis) == functionPeriodic )
      {
        if( d > toler*epsilon*anorm )
	{
          returnValue+=1;
          printf("Warning: Mapping is periodic for axis=%i, but\n",axis);
	  printf("         Mapping does not match at r(%i)=0 and r(%i)=1. |x-xp|=%8.2e\n",axis,axis,d);
          printf("         x(%2.1f,%2.1f,%2.1f)=(%e,%e,%e), \n"
                 "         x(%2.1f,%2.1f,%2.1f)=(%e,%e,%e)  \n",
                           t2(0,0),t2(0,1),t2(0,2),xx(0,0),xx(0,1),xx(0,2),
                           t2(1,0),t2(1,1),t2(1,2),xx(1,0),xx(1,1),xx(1,2));
	  printf("         Relative error=%e \n",d/xNorm);
	}
      }
      if( dd > toler*pow(epsilon,2./5.)*anorm )
      {
        returnValue+=1;
        printf("Warning: Mapping is periodic for axis=%i, but\n",axis);
	printf("         Derivative of Mapping does not match at r(%i)=0 and r(%i)=1.\n",axis,axis);
	printf("         Relative error=%e \n", dd/anorm );
      }
      // evaluate inverse mapping outside to see if the inverse is mapped back to [0,1]
      if( isInvertible )
      {
	r=.5;
	r(0,axis)=-.1;  // evaluate mapping outside left edge
	mapS( r,x );
	inverseMapS(x,r);
	// r.display(" checkMapping: r");
	bool mappedBack=max(fabs(r-.5))<=.5;

	r=.5;
	r(0,axis)=1.1;  // evaluate mapping outside right edge
	// floor2(r).display("floor2(r=1.1)");
	mapS( r,x );
	// r.display("r");
	inverseMapS(x,r);
	mappedBack= mappedBack && (max(fabs(r-.5))<=.5);
      
	if( !mappedBack )
	{
	  returnValue+=1;
	  printf("Error: Mapping is periodic for axis=%i, but\n",axis);
	  printf("       the inverse of a point outside the region is not mapped into the domain\n");
	}
      }
      
    }
  }


  // Check singularities
  for( axis=axis1; axis<domainDimension; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      if( getTypeOfCoordinateSingularity(side,axis)==polarSingularity )
      {
        r=0.;
	r(0,axis)=(real)side;
        mapS( r,x,xt );  // evaluate map on the edge to see if the jacobian is singular
        det=determinant( xt,domainDimension );
        real xtNorm=max(fabs(xt));
	if( fabs(det) > epsilon*pow(xtNorm,3.) )
	{
          returnValue+=1;
	  printf("ERROR: (axis,side)=(%i,%i) is supposed to have a polar singularity but "
                 " fabs(det(xr)) = %e > 0 \n",axis,side,fabs(det));
	}
      }
    }
  }


  // --- Check the derivative at some points.
  //    By default check the mapping at points outside the domain too. (unless there is a singularity)
  
  bool first=TRUE; // print extra information the first time through
  bool firstXR=TRUE; // print extra information the first time through
  int iStart[3]={0,0,0}, iEnd[3]={0,0,0};
  for( axis=0; axis<domainDimension; axis++ )
  {
    iStart[axis]=0;
    if( getTypeOfCoordinateSingularity(Start,axis)!=noCoordinateSingularity )
      iStart[axis]++;
    iEnd[axis]=dimension(axis)+1;
    if( getTypeOfCoordinateSingularity(End  ,axis)!=noCoordinateSingularity )
      iEnd[axis]--;
  }
  
  for( i3=iStart[2]; i3<=iEnd[2]; i3++ )
  {
    r(0,axis3)=(i3-.5)*dr(0,axis3);
    for( i2=iStart[1]; i2<=iEnd[1]; i2++ )
    {
      r(0,axis2)=(i2-.5)*dr(0,axis2);
      for( i1=iStart[0]; i1<=iEnd[0]; i1++ )
      {
        r(0,axis1)=(i1-.5)*dr(0,axis1);
        mapS( r,x,xt );  // evaluate map

        for( int i=0; i<4; i++ )
          t4(i,Axes)=r(0,Axes);
        anorm=0.;
        for( axis=axis1; axis<domainDimension; axis++ )
	{
          t4(0,axis)=r(0,axis)-1.5*h;
          t4(1,axis)=r(0,axis)-0.5*h;
          t4(2,axis)=r(0,axis)+0.5*h;
          t4(3,axis)=r(0,axis)+1.5*h;
          mapS( t4,xx );	  
          t4(Range(0,3),axis)=r(0,axis);

          // 4th order accurate approximation to the first derivative:
          dx(0,xAxes,axis)=(27.*(xx(2,xAxes)-xx(1,xAxes))
                               -(xx(3,xAxes)-xx(0,xAxes)))/(24.*h);

	  anorm+=sum(SQR(dx(0,xAxes,axis)));
	}
        anorm=SQRT(anorm);
	for( axis=axis1; axis<domainDimension; axis++ )
	  for( dir=axis1; dir<rangeDimension; dir++ )
	    if( fabs(dx(0,dir,axis)-xt(0,dir,axis)) > d*anorm )
	    {
	      returnValue+=1;
              if( returnValue < 15 )
	      {
		printf("Warning:  xr(%i,%i) is inaccurate at r=(%6.3f,%6.3f,%6.3f) \n",
		       dir,axis,r(0,axis1),r(0,axis2),domainDimension==2 ? 0. : r(0,axis3));
		printf(" xr(%i,%i)=%12.4e <-> Dx=%12.4e, |Dx|=%12.4e, h=%12.4e \n",
		       dir,axis,xt(0,dir,axis),dx(0,dir,axis),anorm,h);
	      }
	      else if( returnValue==15 )
                printf("Too many warnings for inaccurate xr. I am not printing anymore!\n");
	      bad(0,dir,axis)=TRUE;
	      dxmx(0,dir,axis)=max(dxmx(0,dir,axis),fabs(dx(0,dir,axis)-xt(0,dir,axis))/anorm);
	    }

        if( isInvertible && domainDimension==rangeDimension ) // keep track of minimum determinant
	{
	  det=determinant( xt,domainDimension );
	  if( det<detmin )
	  {
	    detmin=det;
	    imin=iv;
	  }
	  if( det>detmax )
	  {
	    detmax=det;
	    imax=iv;
	  }
	  if( det==0. )
	  {
	    if( !badjac )  // print a message the first time
	    {
	      returnValue+=1;
	      printf("checkMap:Warning:  The determinant of the jacobian matrix is singular somewhere\n");
              r.display("checkMapping:Here is r");
              x.display("checkMapping:Here is x");
              xt.display("checkMapping:Here is the singular matrix, xr");
	    }
	    badjac=TRUE;
	  }
	  else
	    invert( domainDimension,det,xt,xti );
	}
	
        // --- now check the inverse mapping
        if( isInvertible )
	{
	  tt=-9999999.; tx=-9999999.;
	  inverseMapS( x,tt,tx );

	  if( domainDimension==rangeDimension )
	  {
	    det=determinant( tx,domainDimension );
	    if( det==0. )
	    {
	      if( !badinv )  // first time, print a message
	      {
		returnValue+=1;
		printf("checkMap:Warning:  The jacobian of the inverse is singular!\n");
		x.display("checkMapping:Here is x");
		tt.display("checkMapping:Here is r");
		tx.display("checkMapping:Here is the singular matrix, rx");
	      }
	      badinv=TRUE;
	    }
	    else
	    {
              for( axis=0; axis<domainDimension; axis++ )
		if( getIsPeriodic(axis)==functionPeriodic )
		  r(0,axis)=fmod(r(0,axis)+1.,1.);

	      invert( domainDimension,det,tx,txi );

	      badin(0,Axes)=max(badin(0,Axes),fabs(tt(0,Axes)-r(0,Axes))); 
	      if( max(badin(0,Axes)) > toler*epsilon && first )
	      {
		first=FALSE;
		printf("checkMapping:Warning:  the inverse mapping is inaccurate. Here is one of the problems: \n");
                if( domainDimension==2 )
		{
                  printf(" x=(%8.3e,%8.3e) r=(%8.3e,%8.3e) r'=inverseMap(x)=(%8.3e,%8.3e)\n",
			 x(0,0),x(0,1),r(0,0),r(0,1),tt(0,0),tt(0,1));
		}
		else
		{
		  x(0,Axes).display("Here is x");
		  r(0,Axes).display("Here is r");
		  tt(0,Axes).display("Here is inverseMap(map(r))");
		}
		  
	      }
	      badi(0,xAxes,Axes)=max(badi(0,xAxes,Axes),fabs(txi(0,xAxes,Axes)-xt(0,xAxes,Axes))/anorm);

	      real txNorm=SQRT(sum(SQR(tx(0,Axes,xAxes))));
	      badj(0,xAxes,Axes)=max(badj(0,xAxes,Axes),fabs(xti(0,xAxes,Axes)-tx(0,xAxes,Axes))/txNorm);

	      if( max(badj(0,xAxes,Axes)) > 100.*toler*epsilon && firstXR ) // print if really bad
	      {
		firstXR=FALSE;
		printf("\n checkMapping:Warning:  xr^{-1} != rx; Here is an example: \n");
		if( domainDimension==3 && rangeDimension==3 )
		{
		  printF("xr^{-1} = [%9.2e %9.2e %9.2e]\n"
			 "          [%9.2e %9.2e %9.2e]\n"
                         "          [%9.2e %9.2e %9.2e]\n"
			 "rx      = [%9.2e %9.2e %9.2e]\n"
			 "          [%9.2e %9.2e %9.2e]\n"
                         "          [%9.2e %9.2e %9.2e]\n"
			 "diff    = [%9.2e %9.2e %9.2e]\n"
			 "          [%9.2e %9.2e %9.2e]\n"
                         "          [%9.2e %9.2e %9.2e]\n",
			 xti(0,0,0),xti(0,1,0),xti(0,2,0),
			 xti(0,0,1),xti(0,1,1),xti(0,2,1),
			 xti(0,0,2),xti(0,1,2),xti(0,2,2),
			 tx(0,0,0),tx(0,1,0),tx(0,2,0),
			 tx(0,0,1),tx(0,1,1),tx(0,2,1),
			 tx(0,0,2),tx(0,1,2),tx(0,2,2),
			 xti(0,0,0)-tx(0,0,0),xti(0,1,0)-tx(0,1,0),xti(0,2,0)-tx(0,2,0),
			 xti(0,0,1)-tx(0,0,1),xti(0,1,1)-tx(0,1,1),xti(0,2,1)-tx(0,2,1),
			 xti(0,0,2)-tx(0,0,2),xti(0,1,2)-tx(0,1,2),xti(0,2,2)-tx(0,2,2));
		}
		else if( domainDimension==2 && rangeDimension==2 )
		{
		  printF("xr^{-1} = [%9.2e %9.2e]\n"
			 "          [%9.2e %9.2e]\n"
			 "rx      = [%9.2e %9.2e]\n"
			 "          [%9.2e %9.2e]\n"
			 "diff    = [%9.2e %9.2e]\n"
			 "          [%9.2e %9.2e]\n",
			 xti(0,0,0),xti(0,1,0),
			 xti(0,0,1),xti(0,1,1),
			 tx(0,0,0),tx(0,1,0),
			 tx(0,0,1),tx(0,1,1),
			 xti(0,0,0)-tx(0,0,0),xti(0,1,0)-tx(0,1,0),
			 xti(0,0,1)-tx(0,0,1),xti(0,1,1)-tx(0,1,1));
		}
		else
		{
		  xti(0,xAxes,Axes).display("checkMapping: Here is inverse of x.r");
		  tx(0,xAxes,Axes).display("checkMapping: Here is r.x");
		}
		
	      }
	    }
	  }
	}
      }
    }
  }
  

  for( axis=axis1; axis<domainDimension; axis++ )
  {
    if( badin(0,axis) > toler*epsilon )
    {
      printf("checkMapping:Warning:  the inverse mapping for r(%i) is inaccurate \n",axis);
      printf("checkMapping:maximum error in the inverse is %e\n",badin(axis));
    }

    for( dir=axis1; dir<rangeDimension; dir++ )
    {
      if( bad(0,dir,axis) )
      {
	returnValue+=1;
        printf("checkMapping:Warning:  the map derivative xr(%i,%i) is inaccurate; "
               "relative error =%e \n",dir,axis,dxmx(0,dir,axis));
      }
      if( badi(0,dir,axis) > toler*epsilon && !badinv )
      {
	returnValue+=1;
        printf("checkMapping:Warning:  rx^{-1}(%i,%i) != xr(%i,%i); relative error =%e"
               ", expected accuracy=%e\n",
               dir,axis,dir,axis,badi(0,dir,axis),toler*epsilon);
      }
      if( badj(0,dir,axis) > toler*epsilon && !badjac )
      {
	returnValue+=1;
        printf("checkMapping:Warning:  xr^{-1}(%i,%i) != rx(%i,%i); relative error =%e"
               ", expected accuracy=%e\n",
               dir,axis,dir,axis,badj(0,dir,axis),toler*epsilon);
      }      
    }
  }
  
  if( domainDimension==rangeDimension )
  { // try and find where the det reaches a minimum
    real sgn;
    if( detmin*detmax > 0. ) 
    {
      if( detmin > 0. )
      {
	//  Jacobian is positive so far.
	sgn=1.;
	for( axis=axis1; axis<domainDimension; axis++ )
	  t(0,axis)=(imin(axis)-.5)*dr(0,axis);
      }
      else
      {
	// Jacobian is negative so far.
	sgn=-1.;
	for( axis=axis1; axis<domainDimension; axis++ )
	  t(0,axis)=(imax(axis)-.5)*dr(0,axis);
	detmin=-detmax;
        detmax=-detmin;
      }
      //  Follow a path of decreasing absolute value of the jacobian.

      dt(0,Axes)=.2*dr(0,Axes);
      t(0,Axes)=min(max(t(0,Axes)+dt(0,Axes),0.),1.);
      for( int k=1; k<=iters && !singular; k++ )
      {
	for( axis=axis1; axis<domainDimension; axis++ )
	{
	  mapS( t,x,xt );
	  det=determinant( xt,domainDimension )*sgn;
	  if( det <= 0 )
	  {
	    singular=TRUE; 
	    break;
	  }
          if( det<detmin )
	    dt(0,Axes) =  .9*dt(0,Axes);
          else
	    dt(0,Axes) =  -.9*dt(0,Axes);
	  t(0,axis)=min(max(t(0,axis)+dt(0,axis),0.),1.);
	}
      }
      if( sgn<0. )
	printf("checkMapping: The jacobian of the transformation is negative.\n");
    }
    else if( isInvertible )
    {
      printf("checkMapping:Warning:  The jacobian of the transformation is singular.\n");
      printf("                       detmin=%e, detmax=%e\n",detmin,detmax);
      returnValue+=1;
      return returnValue;
    }

    if( singular )
    {
      returnValue+=1;
      printf("checkMapping:Warning:  The jacobian of the transformation is singular");
      if( domainDimension==1 )
	printf("  near r=%f, x=%e \n",t(0,axis1),x(0,axis1));
      else if( domainDimension==2 )
	printf("  near r=(%f,%f), x=(%e,%e) \n",t(0,axis1),t(0,axis2),x(0,axis1),x(0,axis2));
      else
	printf("  near r=(%f,%f,%f), x=(%e,%e,%e) \n",t(0,axis1),t(0,axis2),t(0,axis3),
	       x(0,axis1),x(0,axis2),x(0,axis3));
    }
  }

  printf("checkMapping::finished checking the mapping,");
  if( returnValue==0 )
    printf(" no errors were found\n");
  else
    printf(" some errors were found\n");

  return returnValue;
}

