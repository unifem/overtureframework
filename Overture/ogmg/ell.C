#include "TridiagonalSolver.h"
#include "display.h"

// test the tridiagonal solver


int
main(int argc, char *argv[])
{
  int n=11;
  real cc=.5;  // u.x(0)
  if( argc > 1 )
    sscanf(argv[1],"%e",&cc);
  if( argc > 2 )
    sscanf(argv[2],"%i",&n);

 printf(" n=%i, cc=%e \n",n,cc);
  

  TridiagonalSolver tri;

  Range I1(0,n-1);
  RealArray a(I1),b(I1),c(I1), a1(I1),b1(I1),c1(I1), r(I1), alpha(I1), x(I1), dx(I1), rhs(I1), f(I1);
  RealArray u(I1);
  
  const int m=n-1;
  const real h = 1./(n-1.);
  
  Range I(1,n-2);

  // assign BC's and rhs:
  rhs=0;
  rhs(m)=1.;
  
  // alpha=1-x;
  x.seqAdd(0.,h);
//  display(x,"x");
  alpha=1.-x;
  alpha=-alpha/(cc);
  
  r=0.; dx=0.;
  u=x;   // initial guess
  
  for( int it=0; it<20; it++ )
  {
    // solve u_xx+alpha*f(u)*u_x = 0 
    a(0)=0.;  b(0)=1.; c(0)=0.;
    a(m)=0.;  b(m)=1.; c(m)=0.;

    // f(I)=(u(I+1)-u(I-1))/(2.*h);  // f=u.x
    if( it<1 )
      f(I)=2.*cc;
    real omega=h;
    if( it>=1 )
      f(I)=(1.-omega)*f(I)+omega*f(1);
    
//    else
//      f(I)=(u(2)-2.*u(1)+u(0))/(h*h);   // u.xx(h)

    if( it>0 )
      f(1)=( (u(2)-u(1))/h - cc )/h;     // f=u.xx(0) = (u.x(h)-u.x(0))/h
    printf(" f(1)=%e, uxx(h)=%e, f(n-2)=%e \n",f(1),(u(2)-2.*u(1)+u(0))/(h*h),f(n-2));
    
    a(I)=1./(h*h)-alpha(I)*f(I)/(2.*h);   
    b(I)=-2./(h*h);  
    c(I)=1./(h*h)+alpha(I)*f(I)/(2.*h);
    r(I)=a(I)*u(I-1)+b(I)*u(I)+c(I)*u(I+1);

    a1=a; b1=b; c1=c;
    
    tri.factor(a1,b1,c1,TridiagonalSolver::normal,axis1);

    u=rhs;
    tri.solve(u,I1);
    // display(r,"Here is the residual");
    real resMax=max(abs(r));
    printf(" ----------- iteration=%i, maximum residual=%e -------------------\n",it,resMax);
  }
  
  display(u,"Here is u");

  Range J(1,n-1);
  dx(J)=(u(J)-u(J-1))/h;
  display(dx(J),"Here is the relative grid spacing");

  return 0;
}

