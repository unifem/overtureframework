#include "Overture.h"
#include "TridiagonalSolver.h"
#include "display.h"

// test the PARALLEL tridiagonal solver


// nested disection:

int
parallelFactor()
{
  printF("*** Entering parallel factor\n");
  
  // distribute unknowns 
  
  const int np = 2; // number of processors
  
  RealArray *pa = new RealArray [np];
  RealArray *pb = new RealArray [np];
  RealArray *pc = new RealArray [np];

  RealArray *pr = new RealArray [np]; // rhs
  RealArray *px = new RealArray [np]; // solution
  

  int n=5;  // points on each processor

  // --- setup equations ---
  for( int p=0; p<np; p++ )
  {
    RealArray & a = pa[p];
    RealArray & b = pb[p];
    RealArray & c = pc[p];

    RealArray & r = pr[p];
    RealArray & x = px[p];
    
    a.redim(n); b.redim(n); c.redim(n); r.redim(n); x.redim(n);
    
    Range I1(n);
    int base =I1.getBase();
    int bound=I1.getBound();
    int i1;
    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
    {
      a(i1)=  -(i1+1);
      b(i1)= 4*(i1+1);
      c(i1)=-2*(i1+1);
      r(i1)=   (i1+1);
    }
    r(base) -=a(base);
    r(bound)-=c(bound);

  }
  
  // ------------------------------------------------------------
  // Factor the tridiagonal system:
  //
  //            a[0] | b[0] c[0]                     |
  //                 | a[1] b[1] c[1]                |
  //                 |      a[2] b[2] c[2]           |
  //                 |            .    .    .        |
  //                 |                a[.] b[.] c[.] |
  //                 |                     a[n] b[n] | c[n]
  //
  // --------------------------------------------------------------

  // ---- factor: stage I
  for( int p=0; p<np; p++ )
  {
    RealArray & a = pa[p];
    RealArray & b = pb[p];
    RealArray & c = pc[p];

    RealArray & r = pr[p];
    RealArray & x = px[p];

    Range I1(n);
    int base =I1.getBase();
    int bound=I1.getBound();

    // --- forward elimination of a[i] ---
    wl(base)=a(base);   // left column
    for( int i=base+1; i<=bound; i++ )
    {
      real fact=a(i)/b(i-1);
      b(i) = b(i) - c(i-1)*fact;
      wl(i)= -fact*wl(i-1);  // left column
      a(i)=fact;             // save factor here
    }
    

    // ------------------------------------------------------------
    //
    //            w[0] | b[0] c[0]                     |
    //            w[1] |      b[1] c[1]                |
    //            w[2] |           b[2] c[2]           |
    //                 |                 .    .        |
    //                 |                     b[.] c[.] |
    //            w[n] |                          b[n] | c[n]
    //
    // --------------------------------------------------------------

    // --- forward elimination of c[i] ---
    wr(bound)=c(base);   // right column
    for( int i=bound-1; i>=base; i-- )
    {
      real fact=c(i)/b(i+1);
      wr(i)= -fact*wr(i-1);          // right column
      wl(i)= wl(i+1)-fact*wl(i+1);   // adjust left column
      c(i)=fact;                     // save factor here 
    }

    // ------------------------------------------------------------
    //
    //            w[0] | b[0]                          | wr[0]
    //            w[1] |      b[1]                     | wr[1]
    //            w[2] |           b[2]                |
    //                 |                 .    .        | wr[n-2]
    //                 |                     b[.]      | wr[n-1]
    //            w[n] |                          b[n] | wr[n]
    //
    // --------------------------------------------------------------

  }  //  end for p 
  

  // -- New system: 
  //       w[0]*x[-1] +b[0]*x[0]  + wr[0]*x[n+1] = 
  //       w[n]*x[-1] +b[n]*x[n]  + wr[n]*x[n+1] = 


  return 0;
}



int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture

  real worstError=0.;

  parallelFactor();


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
  

 
  Overture::finish();          
  return 0;
}
