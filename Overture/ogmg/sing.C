#include "Overture.h"
#include "display.h"

// **** test methods for 1D singular problems ******


int
main(int argc, char *argv[])
{
  const int n=10;
  const real h = 1./n;
  
  Range N(-1,n+1), I(0,n), J(1,n-1);

  
  realArray u(N),f(N),res(N), x(N), uOld(N);
  res=0.;
  
  x.seqAdd(-h,h);
  // display(x,"x","%6.2e ");

  if( false )
  {
    // solve u.xx = f -alpha*r  x in [0,1]
    //   u.x=0 at x=0, x=1
    // r^Tu = 0

    f=x-.5;   // integral(f)=0
    f+=1.;  // integral(f)=1


    u=0.;   // initial guess
    real alpha=0.;
    real omega=.8;
  
    real resMaxOld=1., resMax,conv, estConv, estResMaxOld=1.;
    for( int it=0; it<400; it++ )
    {
      uOld=u;
    
      u(I)+= omega*( .5*(u(I+1)-2.*u(I)+u(I-1)) - (.5*h*h)*(f(I)-alpha) );
      if( TRUE )
      {
	u(-1)=u(1);
	u(n+1)=u(n-1);
      }
    

      real sumU=sum(u(I));
      real uAve=sumU/(n+1); // set the mean of u
      u-=uAve;

      res(I)=f(I)-alpha - (u(I+1)-2.*u(I)+u(I-1))*(1./(h*h));
    
      // estimate the actual residual as   min ( res - beta*r )
      //                                   beta
      real sumRes = sum(res(I))/(n+1);
      real beta = sumRes;
    
      real estResMax = max(fabs(res(I)-beta));
      estConv=estResMax/estResMaxOld;
      estResMaxOld=estResMax;

      // display(u,"u","%6.2e ");
      // display(res,"res","%6.2e ");

      resMax=max(fabs(res(I)));
      real diffMax=max(fabs(u-uOld));
    
      conv= resMax/resMaxOld;

      if( (it % 5) ==0 )
	printf(" it=%i, sum(u)=%6.2e, diff=%6.2e, res=%6.2e, estRes=%6.2e, CR=%7.2f estCR=%7.2f\n",it,
	       sumU,diffMax,resMax,
	       estResMax,conv,estConv);

      resMaxOld=resMax;
    }
  }
  else if( false ) 
  {
    
    // Solve
    //       Au + alpha r = F
    //        r.u = 0
    // by 
    //    smooth u
    //    u <- u + beta v
    //    alpha <- alpha + beta 
    //  beta= - r.u/r.v

    real omega=.8;


    realArray v(N);
    v=1.;
    // v=.5*x*(1-x);               // v should really approx satisfy A v = -r
    int it;
    if( false )
    {
      for( it=0; it<100; it++ )
      {
	v(I)+=omega*( .5*(v(I+1)-2.*v(I)+v(I-1)) - (.5*h*h)*( -1. ) );  // solve for Lv=-r

	v(0)=0.; // what should these be?
	v(n)=0.;
	v(-1)=0.;
	v(n+1)=0.;
      }
    }
    


    f=x-.5;   // integral(f)=0
    f+=1.;  // integral(f)=1


    u=0.;   // initial guess
    real alpha=0.;
  
    real resMaxOld=1., resMax,conv, estConv, estResMaxOld=1.;
    for( it=0; it<400; it++ )
    {
      uOld=u;
    
      u(I)+= omega*( .5*(u(I+1)-2.*u(I)+u(I-1)) - (.5*h*h)*(f(I)-alpha) );

      v(I)+=omega*( .5*(v(I+1)-2.*v(I)+v(I-1)) - (.5*h*h)*( -1. ) );  // solve for Lv=-r

      u(-1)=u(1);
      u(n+1)=u(n-1);

      if( false )
      {
	v(-1)=v(1);
	v(n+1)=v(n-1);
        // should normalize v ?
        real normV=sqrt( sum(v(I)*v(I)) );
        v/=normV;

      }
      else  
      { // this seems to work better
	v(0)=0.; // what should these be?
	v(n)=0.;
	v(-1)=0.;
	v(n+1)=0.;
      }
      
      real sumU=sum(u(I));

      real beta = -sum(u(I))/sum(v(I));
      // printf(" beta=%8.2e \n",beta);

      u+=beta*v;
      alpha+=beta;



      res(I)=f(I)-alpha - (u(I+1)-2.*u(I)+u(I-1))*(1./(h*h));
    
      // estimate the actual residual as   min ( res - beta*r )
      //                                   beta
      real sumRes = sum(res(I))/(n+1);
    
      real estResMax = max(fabs(res(I)-sumRes));
      estConv=estResMax/estResMaxOld;
      estResMaxOld=estResMax;

      // display(u,"u","%6.2e ");
      // display(res,"res","%6.2e ");

      resMax=max(fabs(res(I)));
      real diffMax=max(fabs(u-uOld));
    
      conv= resMax/resMaxOld;

      if( (it % 5) ==0 )
	printf(" it=%i, alpha=%8.1e, sum(u)=%6.2e, diff=%6.2e, res=%6.2e, estRes=%6.2e, CR=%7.2f estCR=%7.2f\n",it,
	       alpha,sumU,diffMax,resMax,
	       estResMax,conv,estConv);

      resMaxOld=resMax;
    }

    display(u,"Here is u","%6.2e ");
  }
  else if( true )
  {
    // ============ **** 2D **** ==============

    // Solve
    //       Au + alpha r = F
    //        r.u = 0
    // by 
    //    smooth u
    //    u <- u + beta v
    //    alpha <- alpha + beta 
    //  beta= - r.u/r.v

    Range N(-1,n+1), I(0,n), J(0,n);

  
    realArray u(N,N),f(N,N),res(N,N), x(N,N), y(N,N), uOld(N,N);
    res=0.;
  
    int i,j;
    for( j=-1; j<=n+1; j++ )
      x(N,j).seqAdd(-h,h);

    for( i=-1; i<=n+1; i++ )
      y(i,N).seqAdd(-h,h);


    real omega=.8;

    realArray v(N,N);
    // v=1.;
    v=.25*( x*(1.-x) + y*(1.-y) );               // v should really approx satisfy A v = -r
    int it;
//     if( false )
//     {
//       for( it=0; it<100; it++ )
//       {
// 	v(I)+=omega*( .5*(v(I+1)-2.*v(I)+v(I-1)) - (.5*h*h)*( -1. ) );  // solve for Lv=-r

// 	v(0)=0.; // what should these be?
// 	v(n)=0.;
// 	v(-1)=0.;
// 	v(n+1)=0.;
//       }
//     }
    
    f=x-.5 + y-.5;   // integral(f)=0
    f+=1.;  // integral(f)=1


    u=0.;   // initial guess
    real alpha=0.;
  
    real resMaxOld=1., resMax,conv, estConv, estResMaxOld=1.;
    for( it=0; it<400; it++ )
    {
      uOld=u;
    
      u(I,J)+= omega*( .25*(u(I+1,J)-4.*u(I,J)+u(I-1,J)+u(I,J+1)+u(I,J-1)) - (.25*h*h)*(f(I,J)-alpha) );

//      v(I)+=omega*( .5*(v(I+1)-2.*v(I)+v(I-1)) - (.5*h*h)*( -1. ) );  // solve for Lv=-r

      u(-1,N)=u(1,N);
      u(n+1,N)=u(n-1,N);
      u(N,-1)=u(N,1);
      u(N,n+1)=u(N,n-1);

//       if( false )
//       {
// 	v(-1)=v(1);
// 	v(n+1)=v(n-1);
//         // should normalize v ?
//         real normV=sqrt( sum(v(I)*v(I)) );
//         v/=normV;

//       }
//       else  
//       { // this seems to work better
// 	v(0)=0.; // what should these be?
// 	v(n)=0.;
// 	v(-1)=0.;
// 	v(n+1)=0.;
//       }
      
      real sumU=sum(u(I,J));
      if( true || (it % 50) ==0 )
      {

	real beta = -sum(u(I,J))/sum(v(I,J));
	// printf(" beta=%8.2e \n",beta);

	u+=beta*v;
	alpha+=beta;
      }
      
      res(I,J)=f(I,J)-alpha - (u(I+1,J)-4.*u(I,J)+u(I-1,J)+u(I,J+1)+u(I,J-1))*(1./(h*h));
    
      // estimate the actual residual as   min ( res - beta*r )
      //                                   beta
      real sumRes = sum(res(I,J))/((n+1)*(n+1));
    
      real estResMax = max(fabs(res(I,J)-sumRes));
      estConv=estResMax/estResMaxOld;
      estResMaxOld=estResMax;

      // display(u,"u","%6.2e ");
      // display(res,"res","%6.2e ");

      resMax=max(fabs(res(I,J)));
      real diffMax=max(fabs(u-uOld));
    
      conv= resMax/resMaxOld;

      if( (it % 5) ==0 )
	printf(" it=%i, alpha=%8.1e, sum(u)=%6.2e, diff=%6.2e, res=%6.2e, estRes=%6.2e, CR=%7.2f estCR=%7.2f\n",it,
	       alpha,sumU,diffMax,resMax,
	       estResMax,conv,estConv);

      resMaxOld=resMax;
    }

    display(u,"Here is u","%6.2e ");

  }
  else
  {
    // Solve:
    //    Au + alpha r = F
    //    r.u = 0 
    // by iterating on:
    //    A u = F - alpha r - (r.u)r 
    //    alpha = r.( f-Au ) / r.r

    const real a=1., b=-2.-.1, c=1.+.1;
    
    f=x-.5;
    f+=.1;
    u=.2;   // initial guess
    real alpha=0.;
    real omega=.8;
  
    real resMaxOld=1., resMax,conv, estConv, estResMaxOld=1.;
    for( int it=0; it<400; it++ )
    {
      uOld=u;
      real rDotU = sum( u(I) );
    
      u(I)+= omega*( -((a/b)*u(I+1)+u(I)+(c/b)*u(I-1)) + (h*h/b)*(f(I)-alpha + rDotU ) );
      if( TRUE )
      {
	u(-1)=u(1);
	u(n+1)=u(n-1);
      }
      real sumU=sum(u(I));

      res(I)=f(I)- (a*u(I+1)+b*u(I)+c*u(I-1))*(1./(h*h));  // residual without alpha*r
      alpha=sum(res(I))/(n+1);

      resMax=max(fabs(res(I)-alpha));
      real diffMax=max(fabs(u-uOld));
    
      conv= resMax/resMaxOld;

      if( (it % 5) ==0 )
	printf(" it=%4i, alpha=%8.1e, sum(u)=%8.1e, diff=%6.2e, res=%6.2e, CR=%7.2f \n",it,alpha,
	       sumU,diffMax,resMax,conv);

      resMaxOld=resMax;

    }
  }
  

  return 0;
}

