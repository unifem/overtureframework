#include "Overture.h"
#include "TridiagonalSolver.h"
#include "display.h"

// test the tridiagonal solver


int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture

  real worstError=0.;

  TridiagonalSolver tri;

//  Test non-block systems

  Range I1(0,11), I2(0,2), I3(0,2);
  RealArray a(I1,I2,I3),b(I1,I2,I3),c(I1,I2,I3);
  RealArray u(I1,I2,I3),r(I1,I2,I3);
  
  int axis=0;
//  cout << "Enter axis to solve along (0,1,2)\n";
//  cin >> axis;


  int base =I1.getBase();
  int bound=I1.getBound();
  int i1;
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
  {
    a(i1,I2,I3)=  -(i1+1);
    b(i1,I2,I3)= 4*(i1+1);
    c(i1,I2,I3)=-2*(i1+1);
    r(i1,I2,I3)=   (i1+1);
  }
  r(base,I2,I3) -=a(base,I2,I3);
  r(bound,I2,I3)-=c(bound,I2,I3);
  
  tri.factor(a,b,c,TridiagonalSolver::normal,axis);
  a.redim(0);
  tri.solve(r,I1,I2,I3);
  a.redim(I1,I2,I3);
  
  real error = max(abs(r-1.));
  worstError=max(worstError,error);
  printf(" ****maximum error=%8.2e in the normal case.\n",error);
  // r.display("Here is the solution, should be 1");
  

  for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
  {
    a(i1,I2,I3)=  -(i1+1);
    b(i1,I2,I3)= 4*(i1+1);
    c(i1,I2,I3)=-2*(i1+1);
    r(i1,I2,I3)=   (i1+1);
  }
  
  tri.factor(a,b,c,TridiagonalSolver::extended,axis);
  tri.solve(r);

  // r.display("Here is the solution from the extended system, should be 1");
  error = max(abs(r-1.));
  worstError=max(worstError,error);
  printf(" ****maximum error=%8.2e in the extended case.\n",error);
  
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
  {
    a(i1,I2,I3)=  -(i1+1);
    b(i1,I2,I3)= 4*(i1+1);
    c(i1,I2,I3)=-2*(i1+1);
    r(i1,I2,I3)=   (i1+1);
  }

  tri.factor(a,b,c,TridiagonalSolver::periodic,axis);
  tri.solve(r);
   
  // r.display("Here is the solution from the periodic system, should be 1");
  error = max(abs(r-1.));
  worstError=max(worstError,error);
  printf(" ****maximum error=%8.2e in the periodic case.\n",error);


  // r.display("Here is the solution from the periodic system, should be 1");

  // ---------- test the block tridiagonal solvers ------------------
  for( int ii=0; ii<4; ii++ )  
  {
    const int axis=max(0,ii-1);
    const int m=6;
    Range I1(0,0),I2(0,0),I3(0,0), I1e(0,0),I2e(0,0),I3e(0,0);
    int is[3]={0,0,0};
    if( ii==0 )
    {
      // scalar 1d system
      I1=Range(0,m); I1e=Range(-1,m+1);
    }
    else if( ii==1 )
    {
      // system, axis==axis1
      I1=Range(0,m); I1e=Range(-1,m+1);
      I2=Range(0,2); I2e=I2;
      I3=Range(0,1); I3e=I3;
    }
    else if( ii==2 )
    {
      // system, axis==axis2
      I1=Range(0,2); I1e=I1;
      I2=Range(0,m); I2e=Range(-1,m+1);
      I3=Range(0,1); I3e=I3;
    }
    else 
    {
      // system, axis==axis3
      I1=Range(0,2); I1e=I1;
      I2=Range(0,1); I2e=I2;
      I3=Range(0,m); I3e=Range(-1,m+1);
    }
      
    for( int system=0; system<3; system++ )
    {
      TridiagonalSolver::SystemType systemType = (TridiagonalSolver::SystemType)system;
      for( int block=2; block<=3; block++ )
      {
	Range B(0,block-1);
	realArray a(B,B,I1e,I2e,I3e),b(B,B,I1e,I2e,I3e),c(B,B,I1e,I2e,I3e),r(B,I1,I2,I3),x(B,I1e,I2e,I3e);
    
	int i1,i2,i3;
	for( i3=I3e.getBase(); i3<=I3e.getBound(); i3++)
	  for( i2=I2e.getBase(); i2<=I2e.getBound(); i2++)
	    for( i1=I1e.getBase(); i1<=I1e.getBound(); i1++)
	    {
	      for( int b2=0; b2<block; b2++ )
	      {
		for( int b1=0; b1<block; b1++ )
		{
		  a(b1,b2,i1,i2,i3)= .25*(.5-b1-.5*b2)*(i1+2*i2-i3);
		  b(b1,b2,i1,i2,i3)= .25*(-.5*b1+1.5*b2)*(i1+i2-i3);
		  c(b1,b2,i1,i2,i3)= .25*(.125+.25*b1+.75*b2)*(i1-i2+i3*2);
		  if( b1==b2 )
		  {
		    a(b1,b2,i1,i2,i3)+=10.;
		    b(b1,b2,i1,i2,i3)+=40.;
		    c(b1,b2,i1,i2,i3)+=10.;
		  }
		}
		x(b2,i1,i2,i3)=b2+1.;
	      }
	    }
	if( systemType==TridiagonalSolver::normal )
	{
	  if( axis==0 )
	  {
	    x(B,-1,I2,I3)=0.;
	    x(B,m+1,I2,I3)=0.;
	  }
	  else if( axis==1 )
	  {
	    x(B,I1,-1,I3)=0.;
	    x(B,I1,m+1,I3)=0.;
	  }
	  else
	  {
	    x(B,I1,I2,-1)=0.;
	    x(B,I1,I2,m+1)=0.;
	  }
	}
	else if( systemType==TridiagonalSolver::extended )
	{
	  if( axis==0 )
	  {
	    x(B,-1,I2,I3)=x(B,2,I2,I3);
	    x(B,m+1,I2,I3)=x(B,m-2,I2,I3);
	  }
	  else if( axis==1 )
	  {
	    x(B,I1,-1,I3)=x(B,I1,2,I3);
	    x(B,I1,m+1,I3)=x(B,I1,m-2,I3);
	  }
	  else
	  {
	    x(B,I1,I2,-1)=x(B,I1,I2,2);
	    x(B,I1,I2,m+1)=x(B,I1,I2,m-2);
	  }
	}
	else
	{ // periodic
	  if( axis==0 )
	  {
	    x(B,-1,I2,I3)=x(B,m,I2,I3);
	    x(B,m+1,I2,I3)=x(B,0,I2,I3);
	  }
	  else if( axis==1 )
	  {
	    x(B,I1,-1,I3)=x(B,I1,m,I3);
	    x(B,I1,m+1,I3)=x(B,I1,0,I3);
	  }
	  else
	  {
	    x(B,I1,I2,-1)=x(B,I1,I2,m);
	    x(B,I1,I2,m+1)=x(B,I1,I2,0);
	  }
	}
       
    
	r.reshape(B,1,I1,I2,I3);
	x.reshape(B,1,I1e,I2e,I3e);
	is[axis]=1;
	for( int n=0; n<block; n++ )
	{
	  if( block==2 )
	  {
	    r(n,0,I1,I2,I3)=(a(0,n,I1,I2,I3)*x(0,0,I1-is[0],I2-is[1],I3-is[2])+
			     a(1,n,I1,I2,I3)*x(1,0,I1-is[0],I2-is[1],I3-is[2]) + 
			     b(0,n,I1,I2,I3)*x(0,0,I1  ,I2,I3)+
			     b(1,n,I1,I2,I3)*x(1,0,I1  ,I2,I3) + 
			     c(0,n,I1,I2,I3)*x(0,0,I1+is[0],I2+is[1],I3+is[2])+
			     c(1,n,I1,I2,I3)*x(1,0,I1+is[0],I2+is[1],I3+is[2]));
	  }
	  else
	  {
	    r(n,0,I1,I2,I3)=
	      (a(0,n,I1,I2,I3)*x(0,0,I1-is[0],I2-is[1],I3-is[2])+
	       a(1,n,I1,I2,I3)*x(1,0,I1-is[0],I2-is[1],I3-is[2])+
	       a(2,n,I1,I2,I3)*x(2,0,I1-is[0],I2-is[1],I3-is[2]) + 
	       b(0,n,I1,I2,I3)*x(0,0,I1  ,I2,I3)+
	       b(1,n,I1,I2,I3)*x(1,0,I1  ,I2,I3)+
	       b(2,n,I1,I2,I3)*x(2,0,I1  ,I2,I3) + 
	       c(0,n,I1,I2,I3)*x(0,0,I1+is[0],I2+is[1],I3+is[2])+
	       c(1,n,I1,I2,I3)*x(1,0,I1+is[0],I2+is[1],I3+is[2])+
	       c(2,n,I1,I2,I3)*x(2,0,I1+is[0],I2+is[1],I3+is[2]));
	  }
	   
	}
	r.reshape(B,I1,I2,I3);
	x.reshape(B,I1e,I2e,I3e);
	if( FALSE )
	  display(r,"Here is the rhs");

//	tri.factor(a(B,B,I1,I2,I3),b(B,B,I1,I2,I3),c(B,B,I1,I2,I3),systemType,axis,block);
        realArray aa,bb,cc;
	aa=a(B,B,I1,I2,I3);
	bb=b(B,B,I1,I2,I3);
	cc=c(B,B,I1,I2,I3);
	
	tri.factor(aa,bb,cc,systemType,axis,block);

	tri.solve(r);
	real error = max(abs(r-x(B,I1,I2,I3)));
        worstError=max(worstError,error);
	if( FALSE )
	  display(r-x(Range(0,1),I1,I2,I3),"here is the error");
	printf(" ****maximum error=%8.2e in the %s %ix%i block system ",error,
	       systemType==TridiagonalSolver::normal ? "normal  " :
	       systemType==TridiagonalSolver::extended ? "extended" : "periodic", block,block);
	if( ii==0 )
	  printf(" (1d scalar)\n");
	else
	  printf(" (system, axis=%i) \n",ii-1);

      }
    }
  }

  if( worstError < REAL_EPSILON*1000. )
  {
    printf("\n======== Test successful. Worst error = %e ============\n",worstError);
  }
  else
  {
    printf("\n******** Test apparently FAILED Worst error = %e ***************\n",worstError);
  }
  

  // Peform some timings:


//  Test non-block systems

  // const int nx=11, ny=3, nz=3;
  const int n1=51, n2=50, n3=52;


  if( true )
  {

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

    // int n1=101, n2=100, n3=102;
    // int n1=51, n2=50, n3=52;
    // int n1=11, n2=11, n3=11;
    // printf("\n ****** test timings for n1=%i, n2=%i, n3=%i *******\n",n1,n2,n3);
  printf("\n\n"
         "******************************************************************************\n"
         "**********************  tridiagonal systems **********************************\n"
         "********************** n1=%i n2=%i n3=%i    **********************************\n"
         "******************************************************************************\n\n",n1,n2,n3);
    
    I1=Range(n1), I2=Range(n2), I3=Range(n3);
    RealArray a(I1,I2,I3),b(I1,I2,I3),c(I1,I2,I3);
    RealArray u(I1,I2,I3),r(I1,I2,I3);
  
    for( int system=0; system<3; system++ )
    {
      TridiagonalSolver::SystemType systemType = (TridiagonalSolver::SystemType)system;

      int axis;
      for( axis=0; axis<=2; axis++ )
//    for( axis=2; axis>=0; axis-- )
      {
	const int base =Iv[axis].getBase();
	const int bound=Iv[axis].getBound();
	J1=I1, J2=I2, J3=I3;
	for( int i=base; i<=bound; i++)
	{
	  Jv[axis]=i;
	  a(J1,J2,J3)=  -(i+1);
	  b(J1,J2,J3)= 4*(i+1);
	  c(J1,J2,J3)=-2*(i+1);
	  r(J1,J2,J3)=   (i+1);
	}
        if( systemType==TridiagonalSolver::normal )
	{
	  Jv[axis]=base;
	  r(J1,J2,J3) -=a(J1,J2,J3);
	  Jv[axis]=bound;
	  r(J1,J2,J3)-=c(J1,J2,J3);
	}
	
	real time0=getCPU();
	tri.factor(a,b,c,systemType,axis);

	real timeFactor=getCPU()-time0;
	time0=getCPU();
	tri.solve(r,I1,I2,I3);
	real timeSolve=getCPU()-time0;
  
	real error = max(abs(r-1.));
	worstError=max(worstError,error);
	aString name=systemType==TridiagonalSolver::normal ?   "normal  " :
                     systemType==TridiagonalSolver::extended ? "extended" :  "periodic";

	printf(" ****maximum error=%8.2e in the %s case, axis=%i. factor=%8.2e(s) solve=%8.2e(s)\n",
               error,(const char*)name,axis,timeFactor,timeSolve);
      }
      
    }
    
  }
  

  // **********************************************************************************
  // **********test pentadiagonal systems**********************************************
  // **********************************************************************************


  printf("\n\n"
         "******************************************************************************\n"
         "********************** pentadiagonal systems**********************************\n"
         "********************** n1=%i n2=%i n3=%i    **********************************\n"
         "******************************************************************************\n\n",n1,n2,n3);
 
  Range J1,J2,J3;
    
  for( int system=0; system<3; system++ )
  {
    TridiagonalSolver::SystemType systemType = (TridiagonalSolver::SystemType)system;

    for( axis=0; axis<3; axis++ )
    {

//        if( system==2 && axis>0 )
//          continue;


      I1=Range(0,n1-1), I2=Range(0,n2-1), I3=Range(0,n3-1);
      a.redim(I1,I2,I3);
      b.redim(I1,I2,I3);
      c.redim(I1,I2,I3);
      r.redim(I1,I2,I3);
      RealArray d(I1,I2,I3),e(I1,I2,I3),f(I1,I2,I3);
  
      for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
      {
	a(i1,I2,I3)=-1.*(i1+1);
	b(i1,I2,I3)=-3.*(i1+1);
	c(i1,I2,I3)= 8.*(i1+1);
	d(i1,I2,I3)=-2.*(i1+1);
	e(i1,I2,I3)= 1.*(i1+1);
	r(i1,I2,I3)= 3.*(i1+1);

      }
      if( systemType==TridiagonalSolver::normal )
      {
	base =I1.getBase();
	bound=I1.getBound();
	r(base,I2,I3)   -=a(base,I2,I3)+b(base,I2,I3);
	r(base+1,I2,I3) -=a(base+1,I2,I3);
	r(bound-1,I2,I3)-=e(bound-1,I2,I3);
	r(bound,I2,I3)  -=d(bound,I2,I3)+e(bound,I2,I3);
      
      }
    
      RealArray aa,bb,cc,dd,ee,rr;
      if( axis==0 )
      {
	J1=I1;
	J2=I2;
	J3=I3;
	aa=a; bb=b; cc=c; dd=d; ee=e; rr=r;
      }
      else if( axis==1 )
      {
	J1=I2;
	J2=I1;
	J3=I3;
	aa.redim(I2,I1,I3);
	bb.redim(I2,I1,I3);
	cc.redim(I2,I1,I3);
	dd.redim(I2,I1,I3);
	ee.redim(I2,I1,I3);
	rr.redim(I2,I1,I3);
	for( int i2=I2.getBase(); i2<=I2.getBound(); i2++)
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
	  {
	    aa(i2,i1,I3)=a(i1,i2,I3);
	    bb(i2,i1,I3)=b(i1,i2,I3);
	    cc(i2,i1,I3)=c(i1,i2,I3);
	    dd(i2,i1,I3)=d(i1,i2,I3);
	    ee(i2,i1,I3)=e(i1,i2,I3);
	    rr(i2,i1,I3)=r(i1,i2,I3);
	  }
      }
      else 
      {
	J1=I2;
	J2=I3;
	J3=I1;
	aa.redim(I2,I3,I1);
	bb.redim(I2,I3,I1);
	cc.redim(I2,I3,I1);
	dd.redim(I2,I3,I1);
	ee.redim(I2,I3,I1);
	rr.redim(I2,I3,I1);
	for( int i3=I3.getBase(); i3<=I3.getBound(); i3++)
	  for( int i2=I2.getBase(); i2<=I2.getBound(); i2++)
	    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
	    {
	      aa(i2,i3,i1)=a(i1,i2,i3);
	      bb(i2,i3,i1)=b(i1,i2,i3);
	      cc(i2,i3,i1)=c(i1,i2,i3);
	      dd(i2,i3,i1)=d(i1,i2,i3);
	      ee(i2,i3,i1)=e(i1,i2,i3);
	      rr(i2,i3,i1)=r(i1,i2,i3);
	    }
      }

      real time0=getCPU();
      tri.factor(aa,bb,cc,dd,ee,systemType,axis);
      real timeFactor=getCPU()-time0;

      time0=getCPU();
      tri.solve(rr,J1,J2,J3);
      real timeSolve=getCPU()-time0;

      error = max(abs(rr-1.));
      worstError=max(worstError,error);

      aString name=systemType==TridiagonalSolver::normal ?   "normal  " :
	systemType==TridiagonalSolver::extended ? "extended" :  "periodic";
      printf(" ****maximum error=%8.2e in the %s system, axis=%i. cpu: factor=%8.2e solve=%8.2e\n"
             ,error,(const char*)name,axis,timeFactor,timeSolve);
      // rr.display("Here is the solution, should be 1");
    }
    
  }

//  axis=0;
  

//    // *******************EXTENDED**************************************************
//      I1=Range(0,11), I2=Range(0,2), I3=Range(0,2);
//      a.redim(I1,I2,I3);
//      b.redim(I1,I2,I3);
//      c.redim(I1,I2,I3);
//      RealArray d(I1,I2,I3),e(I1,I2,I3),f(I1,I2,I3);

//    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
//    {
//      a(i1,I2,I3)=-1.*(i1+1);
//      b(i1,I2,I3)=-3.*(i1+1);
//      c(i1,I2,I3)= 8.*(i1+1);
//      d(i1,I2,I3)=-2.*(i1+1);
//      e(i1,I2,I3)= 1.*(i1+1);

//      r(i1,I2,I3)= 3.*(i1+1);

//    }
//    tri.factor(a,b,c,d,e,TridiagonalSolver::extended,axis);
//    a.redim(0);
//    tri.solve(r,I1,I2,I3);
//    a.redim(I1,I2,I3);
  
//    error = max(abs(r-1.));
//    worstError=max(worstError,error);
//    printf(" ****maximum error=%8.2e in the extended case.\n",error);
//    // r.display("Here is the solution, should be 1");
  

//    // *******************PERIODIC**************************************************

//    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
//    {
//      a(i1,I2,I3)=-1.*(i1+1);
//      b(i1,I2,I3)=-3.*(i1+1);
//      c(i1,I2,I3)= 8.*(i1+1);
//      d(i1,I2,I3)=-2.*(i1+1);
//      e(i1,I2,I3)= 1.*(i1+1);

//      r(i1,I2,I3)= 3.*(i1+1);

//    }
//    tri.factor(a,b,c,d,e,TridiagonalSolver::periodic,axis);
//    a.redim(0);
//    tri.solve(r,I1,I2,I3);
//    a.redim(I1,I2,I3);
  
//    error = max(abs(r-1.));
//    worstError=max(worstError,error);
//    printf(" ****maximum error=%8.2e in the periodic case.\n",error);
  // r.display("Here is the solution, should be 1");
  

//    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
//    {
//      a(i1,I2,I3)=  -(i1+1);
//      b(i1,I2,I3)= 4*(i1+1);
//      c(i1,I2,I3)=-2*(i1+1);
//      r(i1,I2,I3)=   (i1+1);
//    }
  
//    tri.factor(a,b,c,TridiagonalSolver::extended,axis);
//    tri.solve(r);

//    // r.display("Here is the solution from the extended system, should be 1");
//    error = max(abs(r-1.));
//    worstError=max(worstError,error);
//    printf(" ****maximum error=%8.2e in the extended case.\n",error);
  
//    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
//    {
//      a(i1,I2,I3)=  -(i1+1);
//      b(i1,I2,I3)= 4*(i1+1);
//      c(i1,I2,I3)=-2*(i1+1);
//      r(i1,I2,I3)=   (i1+1);
//    }

//    tri.factor(a,b,c,TridiagonalSolver::periodic,axis);
//    tri.solve(r);
   
//    // r.display("Here is the solution from the periodic system, should be 1");
//    error = max(abs(r-1.));
//    worstError=max(worstError,error);
//    printf(" ****maximum error=%8.2e in the periodic case.\n",error);






  Overture::finish();          
  return 0;
}
