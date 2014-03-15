#include "Overture.h"

extern "C"
{
  void timef(int & n, real *x, real *y, real *z);
}


int 
main()
{
  
  const int m=10000;
  real x[m],y[m],z[m];
  int i;
  for( int i=0; i<m; i++ )
  {
    x[i]=1.1;
    y[i]=2.2;
  }
  
  real t0=getCPU();
  for( int i=0; i<m; i++ )
    z[i]=x[i]+y[i];
  real t1=getCPU();
  printf("Time c loop = %e \n");
  t0=getCPU();
  timef(m,x,y,z)
  t1=getCPU();
  printf("Time c loop = %e \n");
  




  real time;
  real a=1.,b=1.,c=2.,d=.5;
  int n=1000000;
  printf("multiply (n=%i)...\n",n);
  time=getCPU();
  for( i=0; i<n; i++ )
  {
    b=b*c*d;
  }
  real timeM=getCPU()-time;

  b=1.;
  c=-3.;
  d=3.;
  printf("add...\n");
  time=getCPU();
  for( i=0; i<n; i++ )
  {
    b=b+c+d;
  }
  real timeA=getCPU()-time;


  printf("divide...\n");
  time=getCPU();
  for( i=0; i<n; i++ )
  {
    a=(b/c);
    c=a/b;
  }
  real timeD=getCPU()-time;
  printf("timeM=%e, timeA=%e, timeD=%e, timeM/timeA=%e, timeD/timeA=%e \n",timeM,timeA,timeD,timeM/timeA,timeD/timeA);
  
  return 0;
}

