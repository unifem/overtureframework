#include "TridiagonalSolver.h"
#include "display.h"

// test the tridiagonal solver

int
main()
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking


  TridiagonalSolver tri;
  // first solve a single tridiagonal system
  Range I1(0,10);
  RealArray a(I1),b(I1),c(I1),u(I1);
  a=1.;
  b=2.;
  c=1.;
  // choose the rhs so the answer will be 1
  u=4.;
  tri.factor(a,b,c,TridiagonalSolver::periodic);
  tri.solve(u);

  real error = max(abs(u-1.));
  printf(" ****maximum error=%e for the periodic case.\n",error);

  // Now solve a collection of tridiagonal systems
  Range I2(0,2), I3(0,2);
  a.redim(I1,I2,I3);
  b.redim(I1,I2,I3);
  c.redim(I1,I2,I3);
  u.redim(I1,I2,I3);
  
  int base =I1.getBase();
  int bound=I1.getBound();
  for( int i3=I3.getBase(); i3<=I3.getBound(); i3++)
  {
    for( int i2=I2.getBase(); i2<=I2.getBound(); i2++)
    {
      for( int i1=I1.getBase(); i1<=I1.getBound(); i1++)
      {
	a(i1,i2,i3)=  -(i1+i2+i3+1);
	b(i1,i2,i3)= 4*(i1+i2+i3+1);
	c(i1,i2,i3)=-2*(i1+i2+i3+1);
	u(i1,i2,i3)=   (i1+i2+i3+1);
      }
      u(base,i2,i3) -=a(base,i2,i3);
      u(bound,i2,i3)-=c(bound,i2,i3);
    }
  }
  
  tri.factor(a,b,c,TridiagonalSolver::normal,axis1);
  tri.solve(u,I1,I2,I3);
  
  error = max(abs(u-1.));
  printf(" ****maximum error=%e for the normal case.\n",error);

  return 0;
}
