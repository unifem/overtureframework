void localExactInverse( const RealArray & x, RealArray & r, 
  RealArray & rx = Overture::nullRealArray() )
{ // Compute the inverse of the mapping
  //   The initial guess must be good enough for Newton to converge

  Index I = getIndex( x,r,rx );

  eps=1.e-4  // *** Fix this ***
  maxit=log(eps)/log(.5)  // needs <math.h>
  
  if( (debug %2) == 1 )
    cout << "Newton: eps << maxit = " << eps,maxit << endl;

  RealArray y(rangeDimension,bound-base+1); y.setBase(base);
  RealArray dy(rangeDimension,bound-base+1); dy.setBase(base);
  RealArray dr(rangeDimension,bound-base+1); dr.setBase(base);
  RealArray yr(rangeDimension,domainDimension,bound-base+1); yr.setBase(base);
  RealArray ry(rangeDimension,domainDimension,bound-base+1); ry.setBase(base);

  // Do some Newton iterations.
  int converged=FALSE;
  for( int iter=1; iter<=maxit; iter++ )
  {
    r=max(r,-.1);  // is this correct?
    r=min(r,1.1);

    map( r,y,yr );
    dy=x-y;
    invert( dr,yr,dy, ry,detInverse );   // get dr=yr^(-1)*dy

    r=r+dr;
    
    periodicShift( r );   // shift r into [0,1] if the mapping is periodic

    if( (debug %2) == 1 )
      cout << "Newton: it,fabs(dr) = " << it << fabs(dr) << endl;

    //  Check for convergence.
    if( max(fabs(dr)) < eps )
    {
      converged=TRUE;
     break;
    }

  }
  if( !converged )
    cerr << "ERROR...";

  if( computeMappingDerivative )
    for( int m=axis1; m < rangeDimension; m++ )
      for( int n=axis1; n < domainDimension; n++ )
        for( int axis=axis1; axis < domainDimension; axis++ )
          rx(m,n,i)=ry(m,n,i)*detInverse(i);
}

inline void periodicShift( RealArray & x ):
{
  for( int axis=axis1; axis < domainDimension; axis++ )
  {
    if( isPeriodic(axis) )
      x(axis1,I)=x(axis1,I)-floor(y(axis1,I));   // shift y into the interval [0.,1]
  }
}

inline void invert( RealArray & dr, RealArray & yr, RealArray & dy, RealArray & ry,
  RealArray & detInverse )
{ // Input yr,dy
  // Output : ry = yr^(-1)/detInverse   (unnormalized inverse)
  //          dr = yr^(-1)*dy
  //
  int i;
  switch(  domainDimension ) 
  {
    case 1:
      for( i=base; i<= bound; i++ )
      { 
        ry(1,1,i)=1./yr(1,1,i);
        dr(1,i)=ry(1,1,i)*dy(1,i);
      }
      break; 
    case 2:
      for( i=base; i<= bound; i++ )
      { 
        detInverse(i)=1./(yr(1,1,i)*yr(2,2,i)-yr(1,2,i)*yr(2,1,i));
        ry(1,1,i)= yr(2,2,i);
        ry(1,2,i)=-yr(1,2,i);
        ry(2,1,i)=-yr(2,1,i);
        ry(2,2,i)= yr(1,1,i);
        dr(1,i)=ry(1,1,i)*dy(1,i)+ry(1,2,i)*dy(2,i);
        dr(2,i)=ry(2,1,i)*dy(1,i)+ry(2,2,i)*dy(2,i);
      }
      break; 

    case 3:
      for( i=base; i<= bound; i++ )
      { 
        detInverse(i)=1./(
          (yr(1,2,i)*yr(2,3,i)-yr(2,2,i)*yr(1,3,i),i)*yr(3,1,i)
         +(yr(1,3,i)*yr(2,1,i)-yr(2,3,i)*yr(1,1,i),i)*yr(3,2,i)
         +(yr(1,1,i)*yr(2,2,i)-yr(2,1,i)*yr(1,2,i),i)*yr(3,3,i) );
// or should the neyr values just be scalars:
        ry(1,1,i)=yr(2,2,i)*yr(3,3,i)-yr(2,3,i)*yr(3,2,i);
        ry(2,1,i)=yr(2,3,i)*yr(3,1,i)-yr(2,1,i)*yr(3,3,i);
        ry(3,1,i)=yr(2,1,i)*yr(3,2,i)-yr(2,2,i)*yr(3,1,i);
        ry(1,2,i)=yr(3,2,i)*yr(1,3,i)-yr(3,3,i)*yr(1,2,i);
        ry(2,2,i)=yr(3,3,i)*yr(1,1,i)-yr(3,1,i)*yr(1,3,i);
        ry(3,2,i)=yr(3,1,i)*yr(1,2,i)-yr(3,2,i)*yr(1,1,i);
        ry(1,3,i)=yr(1,2,i)*yr(2,3,i)-yr(1,3,i)*yr(2,2,i);
        ry(2,3,i)=yr(1,3,i)*yr(2,1,i)-yr(1,1,i)*yr(2,3,i);
        ry(3,3,i)=yr(1,1,i)*yr(2,2,i)-yr(1,2,i)*yr(2,1,i);
        dr(1,i)=ry(1,1,i)*dy(1,i)+ry(1,2,i)*dy(2,i)+ry(1,3,i)*dy(3,i);
        dr(2,i)=ry(2,1,i)*dy(1,i)+ry(2,2,i)*dy(2,i)+ry(2,3,i)*dy(3,i);
        dr(3,i)=ry(3,1,i)*dy(1,i)+ry(3,2,i)*dy(2,i)+ry(3,3,i)*dy(3,i);
      }
      for( i=base; i<=bound; i++ )
      {
        dr(1,i)=dr(1,i)*detInverse(i);
        dr(2,i)=dr(2,i)*detInverse(i);
        dr(3,i)=dr(3,i)*detInverse(i);
      }
  }
}

/* ***************************
inline void periodicSpaceShift( RealArray & x, RealArray & y ):
{
  if( periodicityOfSpace == 0 )
    return x;
  else
  {
    switch( domainDimension )
    {
    case 1:
      y(axis1,I)=fpos(axis1,axis1)*x(axis1,I);
      y(axis1,I)=y(axis1,I)-floor(y(axis1,I)+.5);   // shift y into the interval [-.5,.5]
      x(axis1,I)=bpos(axis1,axis1)*y(axis1,I);
      break;
    case 2:
      y(axis1,I)=fpos(axis1,axis1)*x(axis1,I)+fpos(axis1,axis2)*x(axis2,I);
      y(axis2,I)=fpos(axis2,axis1)*x(axis1,I)+fpos(axis2,axis2)*x(axis2,I);
      y(axis1,I)=y(axis1,I)-floor(y(axis1,I)+.5);   // shift y into the interval [-.5,.5]
      y(axis2,I)=y(axis2,I)-floor(y(axis2,I)+.5); 
      x(axis1,I)=bpos(axis1,axis1)*y(axis1,I)+bos(axis1,axis2)*y(axis2,I);
      x(axis2,I)=bpos(axis2,axis1)*y(axis2,I)+bos(axis2,axis2)*y(axis2,I);
      break:
    case 3:
      y(axis1,I)=fpos(axis1,axis1)*x(axis1,I)+fpos(axis1,axis2)*x(axis2,I)
                +fpos(axis1,axis3)*x(axis3,I);
      y(axis2,I)=fpos(axis2,axis1)*x(axis1,I)+fpos(axis2,axis2)*x(axis2,I)
                +fpos(axis2,axis3)*x(axis3,I);
      y(axis3,I)=fpos(axis3,axis1)*x(axis1,I)+fpos(axis3,axis2)*x(axis2,I)
                +fpos(axis3,axis3)*x(axis3,I);
      y(axis1,I)=y(axis1,I)-floor(y(axis1,I)+.5);   // shift y into the interval [-.5,.5]
      y(axis2,I)=y(axis2,I)-floor(y(axis2,I)+.5); 
      y(axis3,I)=y(axis3,I)-floor(y(axis3,I)+.5); 
      x(axis1,I)=bpos(axis1,axis1)*y(axis1,I)+bpos(axis1,axis2)*y(axis2,I)
                +bpos(axis1,axis3)*y(axis3,I);
      x(axis2,I)=bpos(axis2,axis1)*y(axis1,I)+bpos(axis2,axis2)*y(axis2,I)
                +bpos(axis2,axis3)*y(axis3,I);
      x(axis3,I)=bpos(axis3,axis1)*y(axis1,I)+bpos(axis3,axis2)*y(axis2,I)
                +bpos(axis3,axis3)*y(axis3,I);
    }
  }
}

 ***************************  */
