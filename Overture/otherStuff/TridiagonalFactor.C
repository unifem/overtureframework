#include "TridiagonalSolver.h"
#include "OvertureInit.h"

// *********** These routines should be compiled with high optimization -fast on Suns *******


//        ----- generic factor ----
//  For this macro you should define the two macros LI and RI to
//  turn a(LI i RI) into a(i,I2,I3) or a(I1,i,I3) or a(I1,I2,i)
//   
#undef FACTOR
#define FACTOR(I) \
    int base =I.getBase();  \
    int bound=I.getBound();  \
    if( systemType==extended )  \
    {  \
	 /* eliminate c[n]  */ \
      c(LI bound RI)/=a(LI bound-1 RI);   /* save the factor here */ \
      a(LI bound RI)-=b(LI bound-1 RI)*c(LI bound RI);  \
      b(LI bound RI)-=c(LI bound-1 RI)*c(LI bound RI);  \
  \
    }  \
    for( int i=base+1; i<=bound; i++ )  \
    {  \
      a(LI i RI)/=b(LI i-1 RI);  \
      b(LI i RI)-=a(LI i RI)*c(LI i-1 RI);  \
      if( i==base+1 && systemType==extended )  \
        c(LI base+1 RI)-=a(LI base+1 RI)*a(LI base RI); /* adjust c[1] */  \
    }  \

#undef FACTOR
#define FACTOR(I) \
    int base =I.getBase();  \
    int bound=I.getBound();  \
    for( int i=base+1; i<=bound; i++ )  \
    {  \
      a(LI i RI)/=b(LI i-1 RI);  \
      b(LI i RI)-=a(LI i RI)*c(LI i-1 RI);  \
      if( systemType==extended )  \
      {  \
        if( i==base+1 )  \
          c(LI base+1 RI)-=a(LI base+1 RI)*a(LI base RI); /* adjust c[1] */  \
        if( i==bound-1 )  \
        {  /* adjust row n at step n-1 */  \
          c(LI bound RI)/=b(LI i-1 RI);                   /* save the factor here */ \
          a(LI bound RI)-=c(LI bound RI)*c(LI i-1 RI);      \
        }  \
      }  \
    }  


int TridiagonalSolver::
tridiagonalFactor()
// ----------------------------------------------------
// Factor the tridiagonal system:
//
//            | b[0] c[0] a[0]                |
//            | a[1] b[1] c[1]                |
//        A = |      a[2] b[2] c[2]           |
//            |            .    .    .        |
//            |                a[.] b[.] c[.] |
//            |                c[n] a[n] b[n] |
//
// Input:  a, b, c : arrays denoting the 3 diagonals. 
//  extended system: a[0]!=0, c[n]!=0
//
// ------------------------------------------------------
{ 
  bool useOpt=true;
  
  if( blockSize==1 )
  {
    if( axis==axis1 )
    {
      if( false )
      {
	printf("call opt  factor, axis=0\n");
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);

	real *ap0 = a.Array_Descriptor.Array_View_Pointer2;
	real *bp0 = b.Array_Descriptor.Array_View_Pointer2;
	real *cp0 = c.Array_Descriptor.Array_View_Pointer2;

#define A(i) ap[i]
#define B(i) bp[i]
#define C(i) cp[i]

	const int base=I1.getBase();
	const int bound=I1.getBound();
	const int i2Bound=I2.getBound();
	const int i3Bound=I3.getBound();
	  
	for( int i3=I3.getBase(); i3<=i3Bound; i3++ )
	{
	  for( int i2=I2.getBase(); i2<=i2Bound; i2++ )
	  {
	    const int offset=aDim0*(i2+aDim1*(i3));
	      
	    real *ap = ap0 + offset;
	    real *bp = bp0 + offset;
	    real *cp = cp0 + offset;
	      
	    if( systemType!=extended )
	    {
	      for( int i=base+1; i<=bound; i++ )  
	      {  
		A(i)/=B(i-1);  
		B(i)-=A(i)*C(i-1);  
	      }  
	    }
	    else
	    {
	      for( int i=base+1; i<=bound; i++ )  
	      {  
		A(i)/=B(i-1);  
		B(i)-=A(i)*C(i-1);  
		if( i==base+1 )  
		  C(base+1)-=A(base+1)*A(base); /* adjust c[1] */  
		if( i==bound-1 )  
		{  /* adjust row n at step n-1 */  
		  C(bound)/=B(i-1);                   /* save the factor here */ 
		  A(bound)-=C(bound)*C(i-1);      
		}  
	      }  
	    }
	  }
	}
#undef A
#undef B
#undef C

      }
      else if( useOpt )
      {
        // this seems to a bit faster

	// printf("call new opt  factor, axis=0\n");  
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);

	real *ap = a.Array_Descriptor.Array_View_Pointer2;
	real *bp = b.Array_Descriptor.Array_View_Pointer2;
	real *cp = c.Array_Descriptor.Array_View_Pointer2;

	const int base=I1.getBase();
	const int bound=I1.getBound();
	const int i2Bound=I2.getBound();
	const int i3Bound=I3.getBound();

	const int i1Base=I1.getBase();
	const int i2Base=I2.getBase();
	const int i3Base=I3.getBase();
	  
#define A(i) ap[(i)+aDim0*(i2+aDim1*(i3))]
#define B(i) bp[(i)+aDim0*(i2+aDim1*(i3))]
#define C(i) cp[(i)+aDim0*(i2+aDim1*(i3))]

	if( systemType!=extended )
	{
	  for( int i3=i3Base; i3<=i3Bound; i3++ )
	  {
	    for( int i=base+1; i<=bound; i++ )  
	    {  
	      for( int i2=i2Base; i2<=i2Bound; i2++ )
	      {
		A(i)/=B(i-1);  
		B(i)-=A(i)*C(i-1);  
	      }  
	    }
	  }
	}
	else
	{
	  for( int i3=i3Base; i3<=i3Bound; i3++ )
	  {
	    for( int i=base+1; i<=bound; i++ )  
	    {  
	      for( int i2=i2Base; i2<=i2Bound; i2++ )
	      {
		A(i)/=B(i-1);  
		B(i)-=A(i)*C(i-1);  
	      }

	      if( i==base+1 )  
		for( int i2=i2Base; i2<=i2Bound; i2++ )
		  C(base+1)-=A(base+1)*A(base); /* adjust c[1] */  
	      if( i==bound-1 )  
	      {  /* adjust row n at step n-1 */  
		for( int i2=i2Base; i2<=i2Bound; i2++ )
		{
		  C(bound)/=B(i-1);                   /* save the factor here */ 
		  A(bound)-=C(bound)*C(i-1);      
		}
	      }  
	    }  
	  }
	}
      
#undef A
#undef B
#undef C


      }
      else
      {
        printf("call A++ factor, axis=0\n");

#undef LI
#define LI
#undef RI
#define RI ,I2,I3
        FACTOR(I1);
      }
	
    }
    else if( axis==axis2 )
    {
      if( false )
      {
	printf("call opt  factor, axis=1\n");
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);

	real *ap0 = a.Array_Descriptor.Array_View_Pointer2;
	real *bp0 = b.Array_Descriptor.Array_View_Pointer2;
	real *cp0 = c.Array_Descriptor.Array_View_Pointer2;

	const int i1Bound=I1.getBound();
	const int i3Bound=I3.getBound();

	const int base=I2.getBase();
	const int bound=I2.getBound();
	  
	  
	for( int i3=I3.getBase(); i3<=i3Bound; i3++ )
	{
	  for( int i1=I1.getBase(); i1<=i1Bound; i1++ )
	  {
	    const int stride=aDim0;
	    const int offset=i1+aDim0*(aDim1*(i3));
	      
	    real *ap = ap0 + offset;
	    real *bp = bp0 + offset;
	    real *cp = cp0 + offset;
	      
#define A(i) ap[(i)*stride]
#define B(i) bp[(i)*stride]
#define C(i) cp[(i)*stride]

	    if( systemType!=extended )
	    {
	      for( int i=base+1; i<=bound; i++ )  
	      {  
		A(i)/=B(i-1);  
		B(i)-=A(i)*C(i-1);  
	      }  
	    }
	    else
	    {
	      for( int i=base+1; i<=bound; i++ )  
	      {  
		A(i)/=B(i-1);  
		B(i)-=A(i)*C(i-1);  
		if( i==base+1 )  
		  C(base+1)-=A(base+1)*A(base); /* adjust c[1] */  
		if( i==bound-1 )  
		{  /* adjust row n at step n-1 */  
		  C(bound)/=B(i-1);                   /* save the factor here */ 
		  A(bound)-=C(bound)*C(i-1);      
		}  
	      }  
	    }
	  }
	}
#undef A
#undef B
#undef C

      }

      else if( true || useOpt )
      {
	// printf("call new opt factor, axis=1\n");
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);

	real *ap = a.Array_Descriptor.Array_View_Pointer2;
	real *bp = b.Array_Descriptor.Array_View_Pointer2;
	real *cp = c.Array_Descriptor.Array_View_Pointer2;

	const int i1Base=I1.getBase();
	const int i2Base=I2.getBase();
	const int i3Base=I3.getBase();
	const int i1Bound=I1.getBound();
	const int i2Bound=I2.getBound();
	const int i3Bound=I3.getBound();
	  
	  
#define A(i1,i2,i3) ap[i1+aDim0*(i2+aDim1*(i3))]
#define B(i1,i2,i3) bp[i1+aDim0*(i2+aDim1*(i3))]
#define C(i1,i2,i3) cp[i1+aDim0*(i2+aDim1*(i3))]

	      
	if( systemType!=extended )
	{
	  for( int i3=i3Base; i3<=i3Bound; i3++ )  
	  for( int i2=i2Base+1; i2<=i2Bound; i2++ ) // does this order matter?
          for( int i1=i1Base; i1<=i1Bound; i1++ )
	  {
	      
	    A(i1,i2,i3)/=B(i1,i2-1,i3);  
	    B(i1,i2,i3)-=A(i1,i2,i3)*C(i1,i2-1,i3);  
	  }
	}
	else
	{
	  for( int i3=i3Base; i3<=i3Bound; i3++ )  
	  {
	    for( int i2=i2Base+1; i2<=i2Bound; i2++ )
	    {
	      for( int i1=i1Base; i1<=i1Bound; i1++ )
	      {
		A(i1,i2,i3)/=B(i1,i2-1,i3);  
		B(i1,i2,i3)-=A(i1,i2,i3)*C(i1,i2-1,i3);  
	      }
	      if( i2==i2Base+1 )  
	      {
		for( int i1=i1Base; i1<=i1Bound; i1++ )
		{
		  C(i1,i2,i3)-=A(i1,i2,i3)*A(i1,i2-1,i3); /* adjust c[1] */  
		}
	      }
	    
	      if( i2==i2Bound-1 )  
	      {  /* adjust row n at step n-1 */  
		for( int i1=i1Base; i1<=i1Bound; i1++ )
		{
		  C(i1,i2Bound,i3)/=B(i1,i2-1,i3);                   /* save the factor here */ 
		  A(i1,i2Bound,i3)-=C(i1,i2Bound,i3)*C(i1,i2-1,i3);      
		}
	      }  
	    }  
	  }
	}
#undef A
#undef B
#undef C

      }



      else
      {
#undef LI
#define LI I1,
#undef RI
#define RI ,I3
	printf("call A++ factor, axis=1\n");
	FACTOR(I2)

      }
      
    }
    else if( axis==axis3 )
    {
      if( false )
      {
	// printf("call opt  factor, axis=2\n");
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);

	real *ap0 = a.Array_Descriptor.Array_View_Pointer2;
	real *bp0 = b.Array_Descriptor.Array_View_Pointer2;
	real *cp0 = c.Array_Descriptor.Array_View_Pointer2;

	const int i1Bound=I1.getBound();
	const int i2Bound=I2.getBound();

	const int base=I3.getBase();
	const int bound=I3.getBound();
	  
	  
	for( int i2=I2.getBase(); i2<=i2Bound; i2++ )
	{
	  for( int i1=I1.getBase(); i1<=i1Bound; i1++ )
	  {
	    const int stride=aDim0*aDim1;
	    const int offset=i1+aDim0*(i2);
	      
	    real *ap = ap0 + offset;
	    real *bp = bp0 + offset;
	    real *cp = cp0 + offset;
	      
#define A(i) ap[(i)*stride]
#define B(i) bp[(i)*stride]
#define C(i) cp[(i)*stride]

	    if( systemType!=extended )
	    {
	      for( int i=base+1; i<=bound; i++ )  
	      {  
		A(i)/=B(i-1);  
		B(i)-=A(i)*C(i-1);  
	      }  
	    }
	    else
	    {
	      for( int i=base+1; i<=bound; i++ )  
	      {  
		A(i)/=B(i-1);  
		B(i)-=A(i)*C(i-1);  
		if( i==base+1 )  
		  C(base+1)-=A(base+1)*A(base); /* adjust c[1] */  
		if( i==bound-1 )  
		{  /* adjust row n at step n-1 */  
		  C(bound)/=B(i-1);                   /* save the factor here */ 
		  A(bound)-=C(bound)*C(i-1);      
		}  
	      }  
	    }
	  }
	}
#undef A
#undef B
#undef C

      }
      else if( useOpt )
      {
	// printf("call new opt factor, axis=2\n");
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);

	real *ap = a.Array_Descriptor.Array_View_Pointer2;
	real *bp = b.Array_Descriptor.Array_View_Pointer2;
	real *cp = c.Array_Descriptor.Array_View_Pointer2;

	const int i1Bound=I1.getBound();
	const int i2Bound=I2.getBound();

	const int base=I3.getBase();
	const int bound=I3.getBound();
	  
	  
#define A(i1,i2,i3) ap[i1+aDim0*(i2+aDim1*(i3))]
#define B(i1,i2,i3) bp[i1+aDim0*(i2+aDim1*(i3))]
#define C(i1,i2,i3) cp[i1+aDim0*(i2+aDim1*(i3))]

	      
	if( systemType!=extended )
	{
	  for( int i3=base+1; i3<=bound; i3++ )  
	  {  
	    for( int i2=I2.getBase(); i2<=i2Bound; i2++ )
	    {
	      for( int i1=I1.getBase(); i1<=i1Bound; i1++ )
	      {
		A(i1,i2,i3)/=B(i1,i2,i3-1);  
		B(i1,i2,i3)-=A(i1,i2,i3)*C(i1,i2,i3-1);  
	      }
	    }
		
	  }  
	}
	else
	{
	  for( int i3=base+1; i3<=bound; i3++ )  
	  {  
	    for( int i2=I2.getBase(); i2<=i2Bound; i2++ )
	    {
	      for( int i1=I1.getBase(); i1<=i1Bound; i1++ )
	      {
		A(i1,i2,i3)/=B(i1,i2,i3-1);  
		B(i1,i2,i3)-=A(i1,i2,i3)*C(i1,i2,i3-1);  
	      }
	    }
	    
	    if( i3==base+1 )  
	    {
	      for( int i2=I2.getBase(); i2<=i2Bound; i2++ )
	      {
		for( int i1=I1.getBase(); i1<=i1Bound; i1++ )
		{
		  C(i1,i2,base+1)-=A(i1,i2,base+1)*A(i1,i2,base); /* adjust c[1] */  
		}
	      }
	    }
	    
	    if( i3==bound-1 )  
	    {  /* adjust row n at step n-1 */  
	      for( int i2=I2.getBase(); i2<=i2Bound; i2++ )
	      {
		for( int i1=I1.getBase(); i1<=i1Bound; i1++ )
		{
		  C(i1,i2,bound)/=B(i1,i2,i3-1);                   /* save the factor here */ 
		  A(i1,i2,bound)-=C(i1,i2,bound)*C(i1,i2,i3-1);      
		}
	      }
	    }  
	  }  
	}
#undef A
#undef B
#undef C

      }
      else
      {
#undef LI
#define LI I1,I2,
#undef RI
#define RI 
        FACTOR(I3)
      }
    }
    else
    {
      cout << "tridiagonalFactor::ERROR: invalid value for axis = " << axis << endl;
      Overture::abort("error");
    }
  }
  else
  {
    // block tridiagonal system
    blockFactor();
  }
  
  //a.display("a, After factor");
  //b.display("b, After factor");
  //c.display("c, After factor");

  return 0;
}
#undef FACTOR



//
//  For this macro you should define the two macros LI and RI to
//  turn a(LI i RI) into a(i,I2,I3) or a(I1,i,I3) or a(I1,I2,i)
//   
#undef FACTOR
#define FACTOR(I) \
    int base =I.getBase();  \
    int bound=I.getBound();  \
    if( bound-base+1<3 )  \
    {  \
      cout << "periodicTridiagonalFactor:ERROR bound-base+1<3 \n";  \
      Overture::abort("error");  \
    }  \
    w2(LI base RI)=a(LI base RI);  \
    for( int i=base+1; i<=bound-1; i++ )  \
    {  \
      a(LI i RI)/=b(LI i-1 RI);  \
      b(LI i RI)-=a(LI i RI)*c(LI i-1 RI);  \
      w2(LI i RI)=-a(LI i RI)*w2(LI i-1 RI);  \
      w1(LI i RI)=c(LI bound RI)/b(LI i-1 RI);  \
      c(LI bound RI)=-w1(LI i RI)*c(LI i-1 RI);  \
      b(LI bound RI)-=w1(LI i RI)*w2(LI i-1 RI);  \
    }  \
    w2(LI bound-1 RI)+=c(LI bound-1 RI);  \
    a(LI bound RI)+=c(LI bound RI);  \
    a(LI bound RI)/=b(LI bound-1 RI);  \
    b(LI bound RI)-=a(LI bound RI)*w2(LI bound-1 RI);


int TridiagonalSolver::
periodicTridiagonalFactor()
//====================================================================
// Solve the "Periodic" type tridiagonal system  ax=r where
//
//            | b[0] c[0]            a[0] |
//            | a[1] b[1] c[1]            |
//        a = |      a[2] b[2] c[2]       |      n > 2
//            |            .    .    .    |
//            |c[n-1]        a[n-1] b[n-1]|
//
// Input:  a[n], b[b], c[n] : arrays denoting the 3 diagonals. 
//         w1[2],w2[2] : two work arrays of size n
//
//====================================================================
{
/* ---
  if( axis==axis1 )
  {
    int base =I1.getBase();
    int bound=I1.getBound();

    if( bound-base+1<3 )
    {
      cout << "periodicTridiagonalFactor:ERROR bound-base+1<3 \n";
      Overture::abort("error");
    }

    w2(base,I2,I3)=a(base,I2,I3);
    for( int i1=base+1; i1<=bound-1; i1++ )
    {
      a(i1,I2,I3)/=b(i1-1,I2,I3);
      b(i1,I2,I3)-=a(i1,I2,I3)*c(i1-1);
      w2(i1,I2,I3)=-a(i1,I2,I3)*w2(i1-1,I2,I3);
      w1(i1,I2,I3)=c(bound,I2,I3)/b(i1-1,I2,I3);
      c(bound,I2,I3)=-w1(i1,I2,I3)*c(i1-1,I2,I3);
      b(bound,I2,I3)-=w1(i1,I2,I3)*w2(i1-1,I2,I3);
    }
  
    w2(bound-1,I2,I3)+=c(bound-1,I2,I3);
    a(bound,I2,I3)+=c(bound,I2,I3);
    a(bound,I2,I3)/=b(bound-1,I2,I3);
    b(bound,I2,I3)-=a(bound,I2,I3)*w2(bound-1,I2,I3);
  }
--- */
  if( blockSize==1 )
  {
    if( axis==axis1 )
    {

      if( true )
      {
	// printf("call opt periodic factor axis=0\n");  
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);
 	const int wDim0=w1.getRawDataSize(0);
 	const int wDim1=w1.getRawDataSize(1);

	real *ap = a.Array_Descriptor.Array_View_Pointer2;
	real *bp = b.Array_Descriptor.Array_View_Pointer2;
	real *cp = c.Array_Descriptor.Array_View_Pointer2;
	real *w1p= w1.Array_Descriptor.Array_View_Pointer2;
	real *w2p= w2.Array_Descriptor.Array_View_Pointer2;

	const int base=I1.getBase();
	const int bound=I1.getBound();
	const int i2Bound=I2.getBound();
	const int i3Bound=I3.getBound();

	const int i1Base=I1.getBase();
	const int i2Base=I2.getBase();
	const int i3Base=I3.getBase();
	  
#define A(i1) ap[(i1)+aDim0*(i2+aDim1*(i3))]
#define B(i1) bp[(i1)+aDim0*(i2+aDim1*(i3))]
#define C(i1) cp[(i1)+aDim0*(i2+aDim1*(i3))]
#define W1(i1) w1p[(i1)+wDim0*(i2+wDim1*(i3))]
#define W2(i1) w2p[(i1)+wDim0*(i2+wDim1*(i3))]

	if( bound-base+1<3 )
	{
	  cout << "periodicTridiagonalFactor:ERROR bound-base+1<3 \n";
	  Overture::abort("error");
	}

	for( int i3=i3Base; i3<=i3Bound; i3++ )
	{
	  for( int i2=i2Base; i2<=i2Bound; i2++ )
	  {
	    W2(base)=A(base);
	    for( int i=base+1; i<=bound-1; i++ )
	    {
	      A(i)/=B(i-1);
	      B(i)-=A(i)*C(i-1);
	      W2(i)=-A(i)*W2(i-1);
	      W1(i)=C(bound)/B(i-1);
	      C(bound)=-W1(i)*C(i-1);
	      B(bound)-=W1(i)*W2(i-1);
	    }
  
	    W2(bound-1)+=C(bound-1);
	    A(bound)+=C(bound);
	    A(bound)/=B(bound-1);
	    B(bound)-=A(bound)*W2(bound-1);
	  }
	}

#undef A
#undef B
#undef C
#undef W1
#undef W2

      }
      else
      {
        printf("call A++ periodic factor, axis=0\n");


#undef LI
#define LI
#undef RI
#define RI ,I2,I3
      FACTOR(I1)

	}
    }
    else if( axis==axis2 )
    {
      if( true )
      {
	// printf("call opt periodic factor axis=1\n");  
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);
 	const int wDim0=w1.getRawDataSize(0);
 	const int wDim1=w1.getRawDataSize(1);

	real *ap = a.Array_Descriptor.Array_View_Pointer2;
	real *bp = b.Array_Descriptor.Array_View_Pointer2;
	real *cp = c.Array_Descriptor.Array_View_Pointer2;
	real *w1p= w1.Array_Descriptor.Array_View_Pointer2;
	real *w2p= w2.Array_Descriptor.Array_View_Pointer2;

	const int base=I2.getBase();
	const int bound=I2.getBound();
	const int i1Bound=I1.getBound();
	const int i3Bound=I3.getBound();

	const int i1Base=I1.getBase();
	const int i2Base=I2.getBase();
	const int i3Base=I3.getBase();
	  
#define A(i2) ap[(i1)+aDim0*(i2+aDim1*(i3))]
#define B(i2) bp[(i1)+aDim0*(i2+aDim1*(i3))]
#define C(i2) cp[(i1)+aDim0*(i2+aDim1*(i3))]
#define W1(i2) w1p[(i1)+wDim0*(i2+wDim1*(i3))]
#define W2(i2) w2p[(i1)+wDim0*(i2+wDim1*(i3))]

	if( bound-base+1<3 )
	{
	  cout << "periodicTridiagonalFactor:ERROR bound-base+1<3 \n";
	  Overture::abort("error");
	}

        int i1;
	for( int i3=i3Base; i3<=i3Bound; i3++ )
	{
	  for( i1=i1Base; i1<=i1Bound; i1++ )
	    W2(base)=A(base);
	  for( int i=base+1; i<=bound-1; i++ )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1++ )
	    {
	      A(i)/=B(i-1);
	      B(i)-=A(i)*C(i-1);
	      W2(i)=-A(i)*W2(i-1);
	      W1(i)=C(bound)/B(i-1);
	      C(bound)=-W1(i)*C(i-1);
	      B(bound)-=W1(i)*W2(i-1);
	    }
	  }
	  for( i1=i1Base; i1<=i1Bound; i1++ )
	  {
	    W2(bound-1)+=C(bound-1);
	    A(bound)+=C(bound);
	    A(bound)/=B(bound-1);
	    B(bound)-=A(bound)*W2(bound-1);
	  }
	}

#undef A
#undef B
#undef C
#undef W1
#undef W2

      }
      else
      {
        printf("call A++ periodic factor, axis=1\n");

#undef LI
#define LI I1,
#undef RI
#define RI ,I3
      FACTOR(I2)
	}
    }
    
    else if( axis==axis3 )
    {
      if( true )
      {
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);
 	const int wDim0=w1.getRawDataSize(0);
 	const int wDim1=w1.getRawDataSize(1);

	real *ap = a.Array_Descriptor.Array_View_Pointer2;
	real *bp = b.Array_Descriptor.Array_View_Pointer2;
	real *cp = c.Array_Descriptor.Array_View_Pointer2;
	real *w1p= w1.Array_Descriptor.Array_View_Pointer2;
	real *w2p= w2.Array_Descriptor.Array_View_Pointer2;

	const int base=I3.getBase();
	const int bound=I3.getBound();
	const int i1Bound=I1.getBound();
	const int i2Bound=I2.getBound();

	const int i1Base=I1.getBase();
	const int i2Base=I2.getBase();
	const int i3Base=I3.getBase();
	  
#define A(i3) ap[(i1)+aDim0*(i2+aDim1*(i3))]
#define B(i3) bp[(i1)+aDim0*(i2+aDim1*(i3))]
#define C(i3) cp[(i1)+aDim0*(i2+aDim1*(i3))]
#define W1(i3) w1p[(i1)+wDim0*(i2+wDim1*(i3))]
#define W2(i3) w2p[(i1)+wDim0*(i2+wDim1*(i3))]

	if( bound-base+1<3 )
	{
	  cout << "periodicTridiagonalFactor:ERROR bound-base+1<3 \n";
	  Overture::abort("error");
	}

        int i1,i2;
	for( i2=i2Base; i2<=i2Bound; i2++ )
	{
	  for( i1=i1Base; i1<=i1Bound; i1++ )
	  {
	    W2(base)=A(base);
	  }
	}
	for( int i=base+1; i<=bound-1; i++ )
	{
	  for( i2=i2Base; i2<=i2Bound; i2++ )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1++ )
	    {
	      A(i)/=B(i-1);
	      B(i)-=A(i)*C(i-1);
	      W2(i)=-A(i)*W2(i-1);
	      W1(i)=C(bound)/B(i-1);
	      C(bound)=-W1(i)*C(i-1);
	      B(bound)-=W1(i)*W2(i-1);
	    }
	  }
	}
	for( i2=i2Base; i2<=i2Bound; i2++ )
	{
	  for( i1=i1Base; i1<=i1Bound; i1++ )
	  {
	
	    W2(bound-1)+=C(bound-1);
	    A(bound)+=C(bound);
	    A(bound)/=B(bound-1);
	    B(bound)-=A(bound)*W2(bound-1);
	  }
	}
	  
#undef A
#undef B
#undef C
#undef W1
#undef W2

      }
      else
      {
        printf("call A++ periodic factor, axis=2\n");
#undef LI
#define LI I1,I2,
#undef RI
#define RI 
      FACTOR(I3)
	}
    }
    else
    {
      cout << "tridiagonalFactor::ERROR: invalid value for axis = " << axis << endl;
      Overture::abort("error");
    }
  }
  else
  {
    // block tridiagonal system
    blockPeriodicFactor();
  }
  return 0;
}
#undef FACTOR





//
//  For this macro you should define the two macros LI and RI to
//  turn a(LI i RI) into a(i,I2,I3) or a(I1,i,I3) or a(I1,I2,i)
//   
#undef SOLVE
#define SOLVE(I) \
    int base =I.getBase();  \
    int bound=I.getBound();  \
/* forward elimination */  \
    if( systemType==extended )  \
      r(LI bound RI)-=c(LI bound RI)*r(LI bound-1 RI);  \
  \
    for( i=base+1; i<=bound; i++ )  \
      r(LI i RI)-=a(LI i RI)*r(LI i-1 RI);  \
  \
      /* back substitution */  \
    r(LI bound RI)/=b(LI bound RI);  \
    for( i=bound-1; i>=base; i-- )  \
      r(LI i RI)=( r(LI i RI)-c(LI i RI)*r(LI i+1 RI))/b(LI i RI);  \
  \
  \
    if( systemType==extended )  \
      r(LI base RI)-=a(LI base RI)*r(LI base+2 RI)/b(LI base RI);  \

#undef SOLVE
#define SOLVE(I) \
    int base =I.getBase();  \
    int bound=I.getBound();  \
    /* forward elimination */  \
    for( i=base+1; i<=bound; i++ )  \
    {  \
      r(LI i RI)-=a(LI i RI)*r(LI i-1 RI);  \
      if( systemType==extended &&  i==bound-1 )  \
        r(LI bound RI)-=c(LI bound RI)*r(LI i-1 RI);    /* adjust r[n] at step n-1 */  \
    }  \
  \
      /* back substitution */  \
    r(LI bound RI)/=b(LI bound RI);  \
    for( i=bound-1; i>=base; i-- )  \
      r(LI i RI)=( r(LI i RI)-c(LI i RI)*r(LI i+1 RI))/b(LI i RI);  \
  \
  \
    if( systemType==extended )  \
      r(LI base RI)-=a(LI base RI)*r(LI base+2 RI)/b(LI base RI);  \


int TridiagonalSolver::
tridiagonalSolve( RealArray & r )
// -----------------------------------------------------------------------------
// Solve the tridiagonal system Ax=r (A should be first factored by tridiagonalFactor)
// Input: 
//   n,a[n],b[n],c[n] : arrays created by calling tridiagonal Factor (once)
//   r[n] : right hand side (this will be over-written)
// Output: 
//   r[n] : The solution (over-writes the input values)
// -----------------------------------------------------------------------------
{
/* -----
  if( axis==axis1 )
  {
    int base =I1.getBase();
    int bound=I1.getBound();
    // forward elimination
    if( systemType==extended )
      r(bound,I2,I3)-=c(bound,I2,I3)*r(bound-1,I2,I3);

    for( int i1=base+1; i1<=bound; i1++ )
      r(i1,I2,I3)-=a(i1,I2,I3)*r(i1-1,I2,I3);

    // back substitution
    r(bound,I2,I3)/=b(bound,I2,I3);
    for( i1=bound-1; i1>=base; i1-- )
      r(i1,I2,I3)=(r(i1,I2,I3)-c(i1,I2,I3)*r(i1+1,I2,I3))/b(i1,I2,I3);


    if( systemType==extended )
      r(base,I2,I3)-=a(base,I2,I3)*r(base+2,I2,I3)/b(base,I2,I3);
    return 0;
  }
------ */

  bool useOpt=true;

  int i;
  if( blockSize==1 )
  {
    if( axis==axis1 )
    {
      if( useOpt )
      {
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);
	const int rDim0=r.getRawDataSize(0);
	const int rDim1=r.getRawDataSize(1);

	real *ap = a.Array_Descriptor.Array_View_Pointer2;
	real *bp = b.Array_Descriptor.Array_View_Pointer2;
	real *cp = c.Array_Descriptor.Array_View_Pointer2;
	real *rp=  r.Array_Descriptor.Array_View_Pointer2;

	const int base=I1.getBase();
	const int bound=I1.getBound();
	const int i2Bound=I2.getBound();
	const int i3Bound=I3.getBound();

	const int i1Base=I1.getBase();
	const int i2Base=I2.getBase();
	const int i3Base=I3.getBase();
	  
	const int i2Stride=I2.getStride();
	const int i3Stride=I3.getStride();
      
//           printf("opt tri solve: I1=(%i,%i,%i), I2=(%i,%i,%i), I3=(%i,%i,%i) systemType=%i\n",
//                 i1Base,bound,I1.getStride(),i2Base,i2Bound,i2Stride,i3Base,i3Bound,i3Stride,systemType);
//  	 cout << "rp = " << rp << endl;
	 
#define A(i1) ap[(i1)+aDim0*(i2+aDim1*(i3))]
#define B(i1) bp[(i1)+aDim0*(i2+aDim1*(i3))]
#define C(i1) cp[(i1)+aDim0*(i2+aDim1*(i3))]
#define R(i1) rp[(i1)+rDim0*(i2+rDim1*(i3))]

         // forward elimination
        int i1,i2,i3;
	for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	{
	  for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	  {
	    for( i1=base+1; i1<=bound; i1++ )
	    {
	      R(i1)-=A(i1)*R(i1-1);
	    }
	  }
	}
	if( systemType==extended )
	{
	  for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	  {
	    for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	    {
	      R(bound)-=C(bound)*R(bound-2);
	    }
	  }
	}

	// back substitution
	for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	{
	  for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	  {
	    R(bound)/=B(bound);
	  }
	}

	for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	{
	  for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	  {
	    for( i1=bound-1; i1>=base; i1-- )
	    {
	      R(i1)=(R(i1)-C(i1)*R(i1+1))/B(i1);
	    }
	  }
	}

	if( systemType==extended )
	{
	  for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	  {
	    for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	    {
	      R(base)-=A(base)*R(base+2)/B(base);
	    }
	  }
	}

#undef A
#undef B
#undef C
#undef R

      }
      else
      {
        printf("call A++ solve, axis=0\n");
#undef LI
#define LI
#undef RI
#define RI ,I2,I3
      SOLVE(I1)
	}
    }
    
    else if( axis==axis2 )
    {
      if( useOpt )
      {
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);
	const int rDim0=r.getRawDataSize(0);
	const int rDim1=r.getRawDataSize(1);

	real *ap = a.Array_Descriptor.Array_View_Pointer2;
	real *bp = b.Array_Descriptor.Array_View_Pointer2;
	real *cp = c.Array_Descriptor.Array_View_Pointer2;
	real *rp=  r.Array_Descriptor.Array_View_Pointer2;

	const int base=I2.getBase();
	const int bound=I2.getBound();
	const int i1Bound=I1.getBound();
	const int i3Bound=I3.getBound();

	const int i1Base=I1.getBase();
	const int i2Base=I2.getBase();
	const int i3Base=I3.getBase();
	  
	const int i1Stride=I1.getStride();
	const int i3Stride=I3.getStride();

#define A(i2) ap[(i1)+aDim0*(i2+aDim1*(i3))]
#define B(i2) bp[(i1)+aDim0*(i2+aDim1*(i3))]
#define C(i2) cp[(i1)+aDim0*(i2+aDim1*(i3))]
#define R(i2) rp[(i1)+rDim0*(i2+rDim1*(i3))]

         // forward elimination
        int i1,i,i3;
	for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	{
	  for( i=base+1; i<=bound; i++ )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    {
	      R(i)-=A(i)*R(i-1);
	    }
	  }
	}
	if( systemType==extended )
	{
	  for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    {
	      R(bound)-=C(bound)*R(bound-2);
	    }
	  }
	}

	// back substitution
	for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	{
	  for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	  {
	    R(bound)/=B(bound);
	  }
	}

	for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	{
	  for( i=bound-1; i>=base; i-- )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    {
	      R(i)=(R(i)-C(i)*R(i+1))/B(i);
	    }
	  }
	}

	if( systemType==extended )
	{
	  for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    {
	      R(base)-=A(base)*R(base+2)/B(base);
	    }
	  }
	}

#undef A
#undef B
#undef C
#undef R

      }
      else
      {
        printf("call A++ solve, axis=1\n");
#undef LI
#define LI I1,
#undef RI
#define RI ,I3
      SOLVE(I2)
	}
    }

    else if( axis==axis3 )
    {
      if( useOpt )
      {
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);
	const int rDim0=r.getRawDataSize(0);
	const int rDim1=r.getRawDataSize(1);

	real *ap = a.Array_Descriptor.Array_View_Pointer2;
	real *bp = b.Array_Descriptor.Array_View_Pointer2;
	real *cp = c.Array_Descriptor.Array_View_Pointer2;
	real *rp=  r.Array_Descriptor.Array_View_Pointer2;

	const int base=I3.getBase();
	const int bound=I3.getBound();
	const int i1Bound=I1.getBound();
	const int i2Bound=I2.getBound();

	const int i1Base=I1.getBase();
	const int i2Base=I2.getBase();
	const int i3Base=I3.getBase();
	  
	const int i1Stride=I1.getStride();
	const int i2Stride=I2.getStride();

#define A(i3) ap[(i1)+aDim0*(i2+aDim1*(i3))]
#define B(i3) bp[(i1)+aDim0*(i2+aDim1*(i3))]
#define C(i3) cp[(i1)+aDim0*(i2+aDim1*(i3))]
#define R(i3) rp[(i1)+rDim0*(i2+rDim1*(i3))]

         // forward elimination
        int i1,i2,i;
	for( i=base+1; i<=bound; i++ )
	{
	  for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    {
	      R(i)-=A(i)*R(i-1);
	    }
	  }
	}
	if( systemType==extended )
	{
	  for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    {
	      R(bound)-=C(bound)*R(bound-2);
	    }
	  }
	}

	// back substitution
	for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	{
	  for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	  {
	    R(bound)/=B(bound);
	  }
	}

	for( i=bound-1; i>=base; i-- )
	{
	  for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    {
	      R(i)=(R(i)-C(i)*R(i+1))/B(i);
	    }
	  }
	}

	if( systemType==extended )
	{
	  for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    {
	      R(base)-=A(base)*R(base+2)/B(base);
	    }
	  }
	}

#undef A
#undef B
#undef C
#undef R

      }
      else
      {
        printf("call A++ solve, axis=2\n");
#undef LI
#define LI I1,I2,
#undef RI
#define RI 
      SOLVE(I3)
	}
    }
    else
    {
      cout << "tridiagonalSolve::ERROR: invalid value for axis = " << axis << endl;
      Overture::abort("error");
    }
  }
  else
  {
    blockSolve(r);
  }
  return 0;
}
#undef SOLVE

//
//  For this macro you should define the two macros LI and RI to
//  turn a(LI i RI) into a(i,I2,I3) or a(I1,i,I3) or a(I1,I2,i)
//   
#undef SOLVE
#define SOLVE(I) \
    int base =I.getBase();  \
    int bound=I.getBound();  \
    if( bound-base+1<3 )  \
    {  \
      cout << "periodicTridiagonalSolve:ERROR bound-base+1<3 \n";  \
      Overture::abort("error");  \
    }  \
    for( i=base+1; i<bound; i++ )  \
    {  \
      r(LI i RI)-=a(LI i RI)*r(LI i-1 RI);  \
      r(LI bound RI)-=w1(LI i RI)*r(LI i-1 RI);  \
    }  \
    r(LI bound RI)=(r(LI bound RI)-a(LI bound RI)*r(LI bound-1 RI))/b(LI bound RI);  \
    i=bound-1;  \
    r(LI i RI)=(r(LI i RI)-w2(LI i RI)*r(LI bound RI))/b(LI i RI);  \
    for( i=bound-2; i>=base; i-- )  \
      r(LI i RI)=(r(LI i RI)-c(LI i RI)*r(LI i+1 RI)-w2(LI i RI)*r(LI bound RI))/b(LI i RI);  


int TridiagonalSolver::
periodicTridiagonalSolve( RealArray & r )
//====================================================================
// Solve the perioidc tridiagonal system Ax=r (A should be first factored by 
// periodicTridiagonalFactor)
// Input: 
//   n,a[n],b[n],c[n],w1[n],w2[n] : arrays created by calling the 
//        periodic tridiagonal Factor (once)
//   r[n] : right hand side (this will be over-written)
// Output: 
//   r[n] : The solution (over-writes the input values)
//====================================================================
{

/* ----
  if( axis==axis1 )
  {
    int base =I1.getBase();
    int bound=I1.getBound();

    if( bound-base+1<3 )
    {
      cout << "periodicTridiagonalFactor:ERROR bound-base+1<3 \n";
      Overture::abort("error");
    }

    for( int i1=base+1; i1<bound; i1++ )
    {
      r(i1,I2,I3)-=a(i1,I2,I3)*r(i1-1,I2,I3);
      r(bound,I2,I3)-=w1(i1,I2,I3)*r(i1-1,I2,I3);
    }
    r(bound,I2,I3)=(r(bound,I2,I3)-a(bound,I2,I3)*r(bound-1,I2,I3))/b(bound,I2,I3);

    i1=bound-1;
    r(i1,I2,I3)=(r(i1,I2,I3)-w2(i1,I2,I3)*r(bound,I2,I3))/b(i1,I2,I3);
    for( i1=bound-2; i1>=base; i1-- )
      r(i1,I2,I3)=(r(i1,I2,I3)-c(i1,I2,I3)*r(i1+1,I2,I3)-w2(i1,I2,I3)*r(bound,I2,I3))/b(i1,I2,I3);
  }
----- */
  bool useOpt=true;
  
  int i;
  if( blockSize==1 )
  {
    if( axis==axis1 )
    {
      if( useOpt )
      {
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);
	const int rDim0=r.getRawDataSize(0);
	const int rDim1=r.getRawDataSize(1);
 	const int wDim0=w1.getRawDataSize(0);
 	const int wDim1=w1.getRawDataSize(1);

	real *ap = a.Array_Descriptor.Array_View_Pointer2;
	real *bp = b.Array_Descriptor.Array_View_Pointer2;
	real *cp = c.Array_Descriptor.Array_View_Pointer2;
	real *rp=  r.Array_Descriptor.Array_View_Pointer2;
	real *w1p= w1.Array_Descriptor.Array_View_Pointer2;
	real *w2p= w2.Array_Descriptor.Array_View_Pointer2;

	const int base=I1.getBase();
	const int bound=I1.getBound();
	const int i2Bound=I2.getBound();
	const int i3Bound=I3.getBound();

	const int i1Base=I1.getBase();
	const int i2Base=I2.getBase();
	const int i3Base=I3.getBase();
	  
	const int i2Stride=I2.getStride();
	const int i3Stride=I3.getStride();

#define A(i1) ap[(i1)+aDim0*(i2+aDim1*(i3))]
#define B(i1) bp[(i1)+aDim0*(i2+aDim1*(i3))]
#define C(i1) cp[(i1)+aDim0*(i2+aDim1*(i3))]
#define R(i1) rp[(i1)+rDim0*(i2+rDim1*(i3))]
#define W1(i1) w1p[(i1)+wDim0*(i2+wDim1*(i3))]
#define W2(i1) w2p[(i1)+wDim0*(i2+wDim1*(i3))]

	if( bound-base+1<3 )
	{
	  cout << "periodicTridiagonalFactor:ERROR bound-base+1<3 \n";
	  Overture::abort("error");
	}

        int i,i2,i3;
	for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	{
	  for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	  {
	    for( i=base+1; i<bound; i++ )
	    {
	      R(i)-=A(i)*R(i-1);
	      R(bound)-=W1(i)*R(i-1);
	    }
	    R(bound)=(R(bound)-A(bound)*R(bound-1))/B(bound);
	  }
	}
	
	i=bound-1;
	for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	{
	  for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	  {
	    R(i)=(R(i)-W2(i)*R(bound))/B(i);
	  }
	}
	
	for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	{
	  for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	  {
	    for( i=bound-2; i>=base; i-- )
	      R(i)=(R(i)-C(i)*R(i+1)-W2(i)*R(bound))/B(i);
	  }
	}

#undef A
#undef B
#undef C
#undef R
#undef W1
#undef W2

      }
      else
      {
        printf("call A++ periodic solve, axis=0\n");

#undef LI
#define LI
#undef RI
#define RI ,I2,I3
      SOLVE(I1)

	// r.display("periodicTridiagonalSolve:r after");
	}
    }
    else if( axis==axis2 )
    {
      if( useOpt )
      {
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);
	const int rDim0=r.getRawDataSize(0);
	const int rDim1=r.getRawDataSize(1);
 	const int wDim0=w1.getRawDataSize(0);
 	const int wDim1=w1.getRawDataSize(1);

	real *ap = a.Array_Descriptor.Array_View_Pointer2;
	real *bp = b.Array_Descriptor.Array_View_Pointer2;
	real *cp = c.Array_Descriptor.Array_View_Pointer2;
	real *rp=  r.Array_Descriptor.Array_View_Pointer2;
	real *w1p= w1.Array_Descriptor.Array_View_Pointer2;
	real *w2p= w2.Array_Descriptor.Array_View_Pointer2;

	const int base=I2.getBase();
	const int bound=I2.getBound();
	const int i1Bound=I1.getBound();
	const int i3Bound=I3.getBound();

	const int i1Base=I1.getBase();
	const int i2Base=I2.getBase();
	const int i3Base=I3.getBase();
	  
	const int i1Stride=I1.getStride();
	const int i3Stride=I3.getStride();

#define A(i2) ap[(i1)+aDim0*(i2+aDim1*(i3))]
#define B(i2) bp[(i1)+aDim0*(i2+aDim1*(i3))]
#define C(i2) cp[(i1)+aDim0*(i2+aDim1*(i3))]
#define R(i2) rp[(i1)+rDim0*(i2+rDim1*(i3))]
#define W1(i2) w1p[(i1)+wDim0*(i2+wDim1*(i3))]
#define W2(i2) w2p[(i1)+wDim0*(i2+wDim1*(i3))]

	if( bound-base+1<3 )
	{
	  cout << "periodicTridiagonalFactor:ERROR bound-base+1<3 \n";
	  Overture::abort("error");
	}

        int i1,i,i3;
	for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	{
	  for( i=base+1; i<bound; i++ )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    {
	      R(i)-=A(i)*R(i-1);
	      R(bound)-=W1(i)*R(i-1);
	    }
	  }
	  for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    R(bound)=(R(bound)-A(bound)*R(bound-1))/B(bound);
	}
	
	i=bound-1;
	for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	{
	  for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	  {
	    R(i)=(R(i)-W2(i)*R(bound))/B(i);
	  }
	}
	
	for( i3=i3Base; i3<=i3Bound; i3+=i3Stride )
	{
	  for( i=bound-2; i>=base; i-- )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    {
	      R(i)=(R(i)-C(i)*R(i+1)-W2(i)*R(bound))/B(i);
	    }
	  }
	}

#undef A
#undef B
#undef C
#undef R
#undef W1
#undef W2

      }
      else
      {
        printf("call A++ periodic solve, axis=1\n");
#undef LI
#define LI I1,
#undef RI
#define RI ,I3
      SOLVE(I2)
	}
    }
    else if( axis==axis3 )
    {
      if( useOpt )
      {
	// optimized version
	const int aDim0=a.getRawDataSize(0);
	const int aDim1=a.getRawDataSize(1);
	const int rDim0=r.getRawDataSize(0);
	const int rDim1=r.getRawDataSize(1);
 	const int wDim0=w1.getRawDataSize(0);
 	const int wDim1=w1.getRawDataSize(1);

	real *ap = a.Array_Descriptor.Array_View_Pointer2;
	real *bp = b.Array_Descriptor.Array_View_Pointer2;
	real *cp = c.Array_Descriptor.Array_View_Pointer2;
	real *rp=  r.Array_Descriptor.Array_View_Pointer2;
	real *w1p= w1.Array_Descriptor.Array_View_Pointer2;
	real *w2p= w2.Array_Descriptor.Array_View_Pointer2;

	const int base=I3.getBase();
	const int bound=I3.getBound();
	const int i1Bound=I1.getBound();
	const int i2Bound=I2.getBound();

	const int i1Base=I1.getBase();
	const int i2Base=I2.getBase();
	const int i3Base=I3.getBase();
	  
	const int i1Stride=I1.getStride();
	const int i2Stride=I2.getStride();

#define A(i3) ap[(i1)+aDim0*(i2+aDim1*(i3))]
#define B(i3) bp[(i1)+aDim0*(i2+aDim1*(i3))]
#define C(i3) cp[(i1)+aDim0*(i2+aDim1*(i3))]
#define R(i3) rp[(i1)+rDim0*(i2+rDim1*(i3))]
#define W1(i3) w1p[(i1)+wDim0*(i2+wDim1*(i3))]
#define W2(i3) w2p[(i1)+wDim0*(i2+wDim1*(i3))]

	if( bound-base+1<3 )
	{
	  cout << "periodicTridiagonalFactor:ERROR bound-base+1<3 \n";
	  Overture::abort("error");
	}

        int i1,i2,i;
	for( i=base+1; i<bound; i++ )
	{
	  for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    {
	      R(i)-=A(i)*R(i-1);
	      R(bound)-=W1(i)*R(i-1);
	    }
	  }
	}
	for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	{
	  for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    R(bound)=(R(bound)-A(bound)*R(bound-1))/B(bound);
	}
	
	i=bound-1;
	for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	{
	  for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	  {
	    R(i)=(R(i)-W2(i)*R(bound))/B(i);
	  }
	}
	
	for( i=bound-2; i>=base; i-- )
	{
	  for( i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	  {
	    for( i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	    {
	      R(i)=(R(i)-C(i)*R(i+1)-W2(i)*R(bound))/B(i);
	    }
	  }
	}

#undef A
#undef B
#undef C
#undef R
#undef W1
#undef W2

      }
      else
      {
        printf("call A++ periodic solve, axis=2\n");
#undef LI
#define LI I1,I2,
#undef RI
#define RI 
      SOLVE(I3)
	}
    }
    else
    {
      cout << "tridiagonalSolve::ERROR: invalid value for axis = " << axis << endl;
      Overture::abort("error");
    }
  }
  else
  {
    // block tridiagonal system
    blockPeriodicSolve(r);
  }
  return 0;
}
#undef SOLVE





#undef A
#undef B
#undef C
#undef R
#undef W1
#undef W2


int TridiagonalSolver::
scalarBlockFactor( int i1, int i2, int i3 )
// =========================================================================================
// /access: protected: 
//    Internal optimised solver. 
// =========================================================================================
{
  // block tridiagonal system
  if( useOldBlockOrdering )
  { // *old way* (wrong -- used transpose)
    return scalarBlockFactorOld( i1, i2, i3 );
  }
  
      
  Range N(0,blockSize-1);
  int base =Iv[axis].getBase();
  int bound=Iv[axis].getBound();

  // *** assume a,b,c are the same size ?? ****
  const int aDim0=a.getRawDataSize(0);
  const int aDim1=a.getRawDataSize(1);
  const int aDim2=a.getRawDataSize(2);
  const int aDim3=a.getRawDataSize(3);
  const int aDim4=a.getRawDataSize(4);
  const int stride = axis==0 ? aDim0*aDim1 : axis==1 ? aDim0*aDim1*aDim2 : aDim0*aDim1*aDim2*aDim3;
  const int offset = aDim0*aDim1*(i1+aDim2*(i2+aDim3*(i3)));

//   printf(" scalarBlockFactor: stride=%i, offset=%i a.getBase(2)=%i a.getDataBase(2)=%i  "
//          "a.getLength(2)=%i a.getLength(3)=%i,\n",
//            stride,offset,a.getBase(2),a.getDataBase(2),a.getLength(2),a.getLength(3) );
  
  // real *ap = a.Array_Descriptor.Array_View_Pointer2;
  // real *bp = b.Array_Descriptor.Array_View_Pointer2;
  // real *cp = c.Array_Descriptor.Array_View_Pointer2;

  // *wdh* 2014/08/06
  real *ap = a.Array_Descriptor.Array_View_Pointer4;
  real *bp = b.Array_Descriptor.Array_View_Pointer4;
  real *cp = c.Array_Descriptor.Array_View_Pointer4;

  ap+=offset;
  bp+=offset;
  cp+=offset;

  if( systemType==normal )
  {
    if( useOptimizedC && blockSize==2 )
    {
      // printf("optimised scalar 2x2 blockFactor, base=%i\n",base);
      
#define A(m,i) ap[m+stride*(i)]
#define B(m,i) bp[m+stride*(i)]
#define C(m,i) cp[m+stride*(i)]

#define INVERT(B,i) \
      deti = 1./(B(0,i)*B(3,i)-B(2,i)*B(1,i)); \
      temp= B(0,i)*deti; \
      B(0,i)=B(3,i)*deti; \
      B(1,i)*=-deti; \
      B(2,i)*=-deti; \
      B(3,i)=temp; 

      real deti,temp,a0,a1,a2,a3, b0,b1,b2,b3, c0,c1,c2,c3;

      int i=0,j;
      // invert( b,base ); // invert b0
      INVERT(B,i);

      const int ib=bound-base;
      for( i=1; i<=ib; i++ )
      {
	// int i1=base+i;
	// A = [ a0  a2 ]  B=[ b0 b2 ]
	//     [ a1  a3 ]    [ b1 b3 ]
	j=i-1;
	a0=A(0,i); a1=A(1,i); a2=A(2,i); a3=A(3,i);
	b0=B(0,j); b1=B(1,j); b2=B(2,j); b3=B(3,j);
	// a(N,N,i) =multiply(a,i1, b,i-1); // save in a: a*b^{-1}
	A(0,i) = a0*B(0,j)+a2*B(1,j);
	A(1,i) = a1*B(0,j)+a3*B(1,j);
	A(2,i) = a0*B(2,j)+a2*B(3,j);
	A(3,i) = a1*B(2,j)+a3*B(3,j);

	a0=A(0,i); a1=A(1,i); a2=A(2,i); a3=A(3,i);
	c0=C(0,j); c1=C(1,j); c2=C(2,j); c3=C(3,j);
	// b(N,N,i)-=multiply(a,i1, c,i-1);
	B(0,i) -= a0*c0+a2*c1;
	B(1,i) -= a1*c0+a3*c1;
	B(2,i) -= a0*c2+a2*c3;
	B(3,i) -= a1*c2+a3*c3;
	// invert(b,i);
	INVERT(B,i);
      }

#undef A
#undef B
#undef C
#undef W1
#undef W2
#undef INVERT

    }
    else if( useOptimizedC && blockSize==3 )
    {
      // printf("optimised scalar 3x3 blockFactor, base=%i\n",base);
#define A(m,i) ap[m+stride*(i)]
#define B(m,i) bp[m+stride*(i)]
#define C(m,i) cp[m+stride*(i)]

      real deti;
      real a00,a10,a20,a01,a11,a21,a02,a12,a22;
      real b00,b10,b20,b01,b11,b21,b02,b12,b22;
      real c00,c10,c20,c01,c11,c21,c02,c12,c22;
      real d00,d10,d20,d01,d11,d21,d02,d12,d22;
      
      int i=0,j;
	// invert( b,base ); // invert b0

#define INVERT( B,i ) \
      b00=B(0,i), b10=B(1,i), b20=B(2,i);   \
      b01=B(3,i), b11=B(4,i), b21=B(5,i);   \
      b02=B(6,i), b12=B(7,i), b22=B(8,i);   \
      deti = 1./(b00*(b11*b22-b12*b21)+   \
		 b10*(b21*b02-b22*b01)+   \
		 b20*(b01*b12-b02*b11)  );   \
      d00= (b11*b22-b12*b21)*deti;   \
      d01= (b21*b02-b22*b01)*deti;   \
      d02= (b01*b12-b02*b11)*deti;   \
      d10= (b12*b20-b10*b22)*deti;   \
      d11= (b22*b00-b20*b02)*deti;   \
      d12= (b02*b10-b00*b12)*deti;   \
      d20= (b10*b21-b11*b20)*deti;   \
      d21= (b20*b01-b21*b00)*deti;   \
      d22= (b00*b11-b01*b10)*deti;   \
      B(0,i)=d00;   \
      B(1,i)=d10;   \
      B(2,i)=d20;   \
      B(3,i)=d01;   \
      B(4,i)=d11;   \
      B(5,i)=d21;   \
      B(6,i)=d02;   \
      B(7,i)=d12;   \
      B(8,i)=d22;

      INVERT(B,i);
      const int ib=bound-base;
      i=1;
      // for( int i1=base+1; i1<=bound; i1++,i++ )
      for( i=1; i<=ib; i++ )
      {
        //     [ a00 a01 a02 ]   [ a0 a3 a6 ]
        // A = [ a10 a11 a12 ] = [ a1 a4 a7 ]
        //     [ a20 a21 a22 ]   [ a2 a5 a8 ]

	j=i-1;
	a00=A(0,i), a10=A(1,i), a20=A(2,i); 
	a01=A(3,i), a11=A(4,i), a21=A(5,i); 
	a02=A(6,i), a12=A(7,i), a22=A(8,i); 

	b00=B(0,j), b10=B(1,j), b20=B(2,j); 
	b01=B(3,j), b11=B(4,j), b21=B(5,j); 
	b02=B(6,j), b12=B(7,j), b22=B(8,j); 

	// a(N,N,i) =multiply(a,i, b,i-1); // save in a: a*b^{-1}
	A(0,i) = a00*b00+a01*b10+a02*b20;  // A00 
	A(1,i) = a10*b00+a11*b10+a12*b20;  // A10
	A(2,i) = a20*b00+a21*b10+a22*b20;  // A20

	A(3,i) = a00*b01+a01*b11+a02*b21;  // A01
	A(4,i) = a10*b01+a11*b11+a12*b21;  // A11
	A(5,i) = a20*b01+a21*b11+a22*b21;  // A21

	A(6,i) = a00*b02+a01*b12+a02*b22;  // A02 
	A(7,i) = a10*b02+a11*b12+a12*b22;  // A12 
	A(8,i) = a20*b02+a21*b12+a22*b22;  // A22 

	a00=A(0,i), a10=A(1,i), a20=A(2,i); 
	a01=A(3,i), a11=A(4,i), a21=A(5,i); 
	a02=A(6,i), a12=A(7,i), a22=A(8,i); 

	c00=C(0,j), c10=C(1,j), c20=C(2,j); 
	c01=C(3,j), c11=C(4,j), c21=C(5,j); 
	c02=C(6,j), c12=C(7,j), c22=C(8,j); 

	// b(N,N,i)-=multiply(a,i1, c,i-1);
	B(0,i) -= ( a00*c00+a01*c10+a02*c20); // B00
	B(1,i) -= ( a10*c00+a11*c10+a12*c20); // B10
	B(2,i) -= ( a20*c00+a21*c10+a22*c20); // B20
					            
	B(3,i) -= ( a00*c01+a01*c11+a02*c21); // B01
	B(4,i) -= ( a10*c01+a11*c11+a12*c21); // B11
	B(5,i) -= ( a20*c01+a21*c11+a22*c21); // B21
					            
	B(6,i) -= ( a00*c02+a01*c12+a02*c22); // B02
	B(7,i) -= ( a10*c02+a11*c12+a12*c22); // B12
	B(8,i) -= ( a20*c02+a21*c12+a22*c22); // B22

	// invert(b,i);
	INVERT(B,i);
      }

#undef A
#undef B
#undef C
#undef W1
#undef W2
#undef INVERT

    }
    else
    { // general case
      invert( b,base ); // invert b0
      for( int i1=base+1; i1<=bound; i1++ )
      {
	a(N,N,i1) =multiply(a,i1, b,i1-1); // save in a: a*b^{-1}
	b(N,N,i1)-=multiply(a,i1, c,i1-1);
	invert(b,i1);
      }
    }
  }
  else if( systemType==extended )
  {
    // ***** finish this case ******

    // eliminate c[n]
    RealArray aa(N,N);
    aa=a(N,N,bound-1);
    invert(aa,0);
    c(N,N,bound)=multiply(c,bound, aa,0);    // save in c : c*a^{-1}
      
    a(N,N,bound)-=multiply(c,bound, b,bound-1);
    b(N,N,bound)-=multiply(c,bound, c,bound-1);
  
    invert( b,base ); // invert b0
    // first case is special
    int i1=base+1;
    a(N,N,i1) =multiply(a,i1, b,i1-1); // save in a: a*b^{-1}
    b(N,N,i1)-=multiply(a,i1, c,i1-1);
    invert(b,i1);
    c(N,N,base+1)-=multiply(a,base+1, a,base); // adjust c[1]
    for( i1=base+2; i1<=bound; i1++ )
    {
      a(N,N,i1) =multiply(a,i1, b,i1-1); // save in a: a*b^{-1}
      b(N,N,i1)-=multiply(a,i1, c,i1-1);
      invert(b,i1);
    }
  }
  return 0;
}


int TridiagonalSolver::
scalarBlockSolve(RealArray & r, int i1, int i2, int i3)
// ============================================================================================
// ============================================================================================
{
  if( useOldBlockOrdering )
  { // *old way* (wrong -- used transpose)
    return scalarBlockSolveOld( r, i1, i2, i3 );
  }

  Range N(0,blockSize-1);
  int base =Iv[axis].getBase();
  int bound=Iv[axis].getBound();

  // *** assume a,b,c are the same size ?? ****
  const int aDim0=a.getRawDataSize(0);
  const int aDim1=a.getRawDataSize(1);
  const int aDim2=a.getRawDataSize(2);
  const int aDim3=a.getRawDataSize(3);
  const int aDim4=a.getRawDataSize(4);
  const int stride = axis==0 ? aDim0*aDim1 : axis==1 ? aDim0*aDim1*aDim2 : aDim0*aDim1*aDim2*aDim3;
  const int offset = aDim0*aDim1*(i1+aDim2*(i2+aDim3*(i3)));

  // real *ap = a.Array_Descriptor.Array_View_Pointer2;
  // real *bp = b.Array_Descriptor.Array_View_Pointer2;
  // real *cp = c.Array_Descriptor.Array_View_Pointer2;

  // *wdh* 2014/08/06
  real *ap = a.Array_Descriptor.Array_View_Pointer4;
  real *bp = b.Array_Descriptor.Array_View_Pointer4;
  real *cp = c.Array_Descriptor.Array_View_Pointer4;

  ap+=offset;
  bp+=offset;
  cp+=offset;

  const int rDim0=r.getRawDataSize(0);
  const int rDim1=r.getRawDataSize(1);
  const int rDim2=r.getRawDataSize(2);
  const int rDim3=r.getRawDataSize(3);
  const int rStride = axis==0 ? rDim0 : axis==1 ? rDim0*rDim1 : rDim0*rDim1*rDim2;
  const int rOffset = rDim0*(i1+rDim1*(i2+rDim2*(i3)));

  // real *rp = r.Array_Descriptor.Array_View_Pointer2;
  real *rp = r.Array_Descriptor.Array_View_Pointer4; // *wdh* 2014/08/06
  rp+=rOffset;

  if( useOptimizedC && blockSize==2 )
  {

#define A(m,i) ap[m+stride*(i)]
#define B(m,i) bp[m+stride*(i)]
#define C(m,i) cp[m+stride*(i)]
#define R(m,i) rp[m+rStride*(i)]

    // forward elimination
    if( systemType==extended )
      r(N,bound  )-=matrixVectorMultiply(c,bound  ,r,bound-1);  // ******** fix this and below ***

    real a0,a1,a2,a3, b0,b1,b2,b3, d0,d1,d2,d3,  r0,r1;
    int i=1,j;
    const int ib=bound-base;
    for( i=1; i<=ib; i++ )
    {
      // A = [ a0  a2 ]  
      //     [ a1  a3 ]  

      j=i-1;
      a0=A(0,i); a1=A(1,i); a2=A(2,i); a3=A(3,i);
      r0=R(0,j); r1=R(1,j);
      // r(N,i1)-=matrixVectorMultiply(a,i1, r,i1-1);
      R(0,i) -= ( a0*r0+a2*r1 );
      R(1,i) -= ( a1*r0+a3*r1 );
    }
    // back substitution
    // r(N,bound)=matrixVectorMultiply(b,bound, r,bound);
    i=ib; 
    b0=B(0,i); b1=B(1,i); b2=B(2,i); b3=B(3,i);
    r0=R(0,i); r1=R(1,i);
    R(0,i) = ( b0*r0+b2*r1 );
    R(1,i) = ( b1*r0+b3*r1 );
    for( i=ib-1; i>=0; i-- )
    { 
      // r(N,i1)=matrixVectorMultiply(b,i1, evaluate(r(N,i1)-matrixVectorMultiply(c,i1, r,i1+1))); 
      j=i+1;
      a0=C(0,i); a1=C(1,i); a2=C(2,i); a3=C(3,i);
      r0=R(0,j); r1=R(1,j);
      d0 = R(0,i)-(a0*r0+a2*r1);
      d1 = R(1,i)-(a1*r0+a3*r1);
      b0=B(0,i); b1=B(1,i); b2=B(2,i); b3=B(3,i);
      R(0,i) = ( b0*d0+b2*d1 );
      R(1,i) = ( b1*d0+b3*d1 );
    }

    if( systemType==extended )
      r(N,base)-=matrixVectorMultiply(b,base, matrixVectorMultiply(a, base, r,base+2));

#undef A
#undef B
#undef C
#undef R
#undef W1
#undef W2
      
  }
  else if( useOptimizedC && blockSize==3 )
  {

#define A(m,i) ap[m+stride*(i)]
#define B(m,i) bp[m+stride*(i)]
#define C(m,i) cp[m+stride*(i)]
#define R(m,i) rp[m+rStride*(i)]

    real a00,a10,a20,a01,a11,a21,a02,a12,a22;
    real b00,b10,b20,b01,b11,b21,b02,b12,b22;
    real d00,d10,d20,d01,d11,d21,d02,d12,d22;
    real r0,r1,r2;
      
    // forward elimination
    if( systemType==extended )
      r(N,bound  )-=matrixVectorMultiply(c,bound  ,r,bound-1);  // ******** fix this and below ***

    int i=1,j;
    const int ib=bound-base;
    for( i=1; i<=ib; i++ )
    {
        //     [ a00 a01 a02 ]   [ a0 a3 a6 ]
        // A = [ a10 a11 a12 ] = [ a1 a4 a7 ]
        //     [ a20 a21 a22 ]   [ a2 a5 a8 ]

      j=i-1;
      a00=A(0,i), a10=A(1,i), a20=A(2,i); 
      a01=A(3,i), a11=A(4,i), a21=A(5,i); 
      a02=A(6,i), a12=A(7,i), a22=A(8,i); 
      r0=R(0,j); r1=R(1,j); r2=R(2,j);
      // r(N,i1)-=matrixVectorMultiply(a,i1, r,i1-1);
      R(0,i) -= (a00*r0+a01*r1+a02*r2);
      R(1,i) -= (a10*r0+a11*r1+a12*r2);
      R(2,i) -= (a20*r0+a21*r1+a22*r2);
    }
    // back substitution
    // r(N,bound)=matrixVectorMultiply(b,bound, r,bound);
    i=ib; 
    b00=B(0,i), b10=B(1,i), b20=B(2,i); 
    b01=B(3,i), b11=B(4,i), b21=B(5,i); 
    b02=B(6,i), b12=B(7,i), b22=B(8,i); 
    r0=R(0,i); r1=R(1,i); r2=R(2,i);
    R(0,i) = (b00*r0+b01*r1+b02*r2);
    R(1,i) = (b10*r0+b11*r1+b12*r2);
    R(2,i) = (b20*r0+b21*r1+b22*r2);

    for( i=ib-1; i>=0; i-- )
    { 
      // r(N,i1)=matrixVectorMultiply(b,i1, evaluate(r(N,i1)-matrixVectorMultiply(c,i1, r,i1+1))); 
      j=i+1;
      a00=C(0,i), a10=C(1,i), a20=C(2,i); 
      a01=C(3,i), a11=C(4,i), a21=C(5,i); 
      a02=C(6,i), a12=C(7,i), a22=C(8,i); 
      r0=R(0,j); r1=R(1,j); r2=R(2,j);
      d00 = R(0,i) - (a00*r0+a01*r1+a02*r2);
      d10 = R(1,i) - (a10*r0+a11*r1+a12*r2);
      d20 = R(2,i) - (a20*r0+a21*r1+a22*r2);
      b00=B(0,i), b10=B(1,i), b20=B(2,i); 
      b01=B(3,i), b11=B(4,i), b21=B(5,i); 
      b02=B(6,i), b12=B(7,i), b22=B(8,i); 
      R(0,i) = (b00*d00+b01*d10+b02*d20);
      R(1,i) = (b10*d00+b11*d10+b12*d20);
      R(2,i) = (b20*d00+b21*d10+b22*d20);

    }
    if( systemType==extended )
      r(N,base)-=matrixVectorMultiply(b,base, matrixVectorMultiply(a, base, r,base+2));

#undef A
#undef B
#undef C
#undef R

  }
  else
  { // general case

    // forward elimination
    int i1;
    if( systemType==extended )
      r(N,bound  )-=matrixVectorMultiply(c,bound  ,r,bound-1);

    for( i1=base+1; i1<=bound; i1++ )
      r(N,i1)-=matrixVectorMultiply(a,i1, r,i1-1);

    // back substitution
    r(N,bound)=matrixVectorMultiply(b,bound, r,bound);
    for( i1=bound-1; i1>=base; i1-- )
    { //  b^{-1}[ r_i - c_i*r_{i+1} ]
      r(N,i1)=matrixVectorMultiply(b,i1, evaluate(r(N,i1)-matrixVectorMultiply(c,i1, r,i1+1)));  
    }
    
    if( systemType==extended )
      r(N,base)-=matrixVectorMultiply(b,base, matrixVectorMultiply(a, base, r,base+2));
  }

  return 0;
}










int TridiagonalSolver::
scalarBlockFactorOld( int i1, int i2, int i3 )
// =========================================================================================
// /access: protected: 
//    Internal optimised solver. 
// =========================================================================================
{
  // block tridiagonal system
  Range N(0,blockSize-1);
  int base =Iv[axis].getBase();
  int bound=Iv[axis].getBound();

  // *** assume a,b,c are the same size ?? ****
  const int aDim0=a.getRawDataSize(0);
  const int aDim1=a.getRawDataSize(1);
  const int aDim2=a.getRawDataSize(2);
  const int aDim3=a.getRawDataSize(3);
  const int aDim4=a.getRawDataSize(4);
  const int stride = axis==0 ? aDim0*aDim1 : axis==1 ? aDim0*aDim1*aDim2 : aDim0*aDim1*aDim2*aDim3;
  const int offset = aDim0*aDim1*(i1+aDim2*(i2+aDim3*(i3)));

//   printf(" scalarBlockFactor: stride=%i, offset=%i a.getBase(2)=%i a.getDataBase(2)=%i  "
//          "a.getLength(2)=%i a.getLength(3)=%i,\n",
//            stride,offset,a.getBase(2),a.getDataBase(2),a.getLength(2),a.getLength(3) );
  
  real *ap = a.Array_Descriptor.Array_View_Pointer2;
  real *bp = b.Array_Descriptor.Array_View_Pointer2;
  real *cp = c.Array_Descriptor.Array_View_Pointer2;
  ap+=offset;
  bp+=offset;
  cp+=offset;

  if( systemType==normal )
  {
    if( useOptimizedC && blockSize==2 )
    {
      // printf("optimised scalar 2x2 blockFactor, base=%i\n",base);
      
#define A(m,i) ap[m+stride*(i)]
#define B(m,i) bp[m+stride*(i)]
#define C(m,i) cp[m+stride*(i)]

#define INVERT(B,i) \
      deti = 1./(B(0,i)*B(3,i)-B(2,i)*B(1,i)); \
      temp= B(0,i)*deti; \
      B(0,i)=B(3,i)*deti; \
      B(1,i)*=-deti; \
      B(2,i)*=-deti; \
      B(3,i)=temp; 

      real deti,temp,a0,a1,a2,a3, b0,b1,b2,b3, c0,c1,c2,c3;

      int i=0,j;
      // invert( b,base ); // invert b0
      INVERT(B,i);

      const int ib=bound-base;
      for( i=1; i<=ib; i++ )
      {
	// int i1=base+i;
	  
	j=i-1;
	a0=A(0,i); a1=A(1,i); a2=A(2,i); a3=A(3,i);
	b0=B(0,j); b1=B(1,j); b2=B(2,j); b3=B(3,j);
	// a(N,N,i) =multiply(a,i1, b,i-1); // save in a: a*b^{-1}
	A(0,i) = a0*B(0,j)+a1*B(2,j);
	A(1,i) = a0*B(1,j)+a1*B(3,j);
	A(2,i) = a2*B(0,j)+a3*B(2,j);
	A(3,i) = a2*B(1,j)+a3*B(3,j);

	a0=A(0,i); a1=A(1,i); a2=A(2,i); a3=A(3,i);
	c0=C(0,j); c1=C(1,j); c2=C(2,j); c3=C(3,j);
	// b(N,N,i)-=multiply(a,i1, c,i-1);
	B(0,i) -= a0*c0+a1*c2;
	B(1,i) -= a0*c1+a1*c3;
	B(2,i) -= a2*c0+a3*c2;
	B(3,i) -= a2*c1+a3*c3;
	// invert(b,i);
	INVERT(B,i);
      }

#undef A
#undef B
#undef C
#undef W1
#undef W2
#undef INVERT

    }
    else if( useOptimizedC && blockSize==3 )
    {
      // printf("optimised scalar 3x3 blockFactor, base=%i\n",base);
#define A(m,i) ap[m+stride*(i)]
#define B(m,i) bp[m+stride*(i)]
#define C(m,i) cp[m+stride*(i)]

      real deti;
      real a00,a10,a20,a01,a11,a21,a02,a12,a22;
      real b00,b10,b20,b01,b11,b21,b02,b12,b22;
      real c00,c10,c20,c01,c11,c21,c02,c12,c22;
      real d00,d10,d20,d01,d11,d21,d02,d12,d22;
      
      int i=0,j;
	// invert( b,base ); // invert b0

#define INVERT( B,i ) \
      b00=B(0,i), b10=B(1,i), b20=B(2,i);   \
      b01=B(3,i), b11=B(4,i), b21=B(5,i);   \
      b02=B(6,i), b12=B(7,i), b22=B(8,i);   \
      deti = 1./(b00*(b11*b22-b12*b21)+   \
		 b10*(b21*b02-b22*b01)+   \
		 b20*(b01*b12-b02*b11)  );   \
      d00= (b11*b22-b12*b21)*deti;   \
      d01= (b21*b02-b22*b01)*deti;   \
      d02= (b01*b12-b02*b11)*deti;   \
      d10= (b12*b20-b10*b22)*deti;   \
      d11= (b22*b00-b20*b02)*deti;   \
      d12= (b02*b10-b00*b12)*deti;   \
      d20= (b10*b21-b11*b20)*deti;   \
      d21= (b20*b01-b21*b00)*deti;   \
      d22= (b00*b11-b01*b10)*deti;   \
      B(0,i)=d00;   \
      B(1,i)=d10;   \
      B(2,i)=d20;   \
      B(3,i)=d01;   \
      B(4,i)=d11;   \
      B(5,i)=d21;   \
      B(6,i)=d02;   \
      B(7,i)=d12;   \
      B(8,i)=d22;

      INVERT(B,i);
      const int ib=bound-base;
      i=1;
      // for( int i1=base+1; i1<=bound; i1++,i++ )
      for( i=1; i<=ib; i++ )
      {
	j=i-1;
	a00=A(0,i), a10=A(1,i), a20=A(2,i); 
	a01=A(3,i), a11=A(4,i), a21=A(5,i); 
	a02=A(6,i), a12=A(7,i), a22=A(8,i); 

	b00=B(0,j), b10=B(1,j), b20=B(2,j); 
	b01=B(3,j), b11=B(4,j), b21=B(5,j); 
	b02=B(6,j), b12=B(7,j), b22=B(8,j); 

	// a(N,N,i) =multiply(a,i, b,i-1); // save in a: a*b^{-1}
	A(0,i) = a00*b00+a10*b01+a20*b02;
	A(1,i) = a00*b10+a10*b11+a20*b12;
	A(2,i) = a00*b20+a10*b21+a20*b22;
	A(3,i) = a01*b00+a11*b01+a21*b02;
	A(4,i) = a01*b10+a11*b11+a21*b12;
	A(5,i) = a01*b20+a11*b21+a21*b22;
	A(6,i) = a02*b00+a12*b01+a22*b02;
	A(7,i) = a02*b10+a12*b11+a22*b12;
	A(8,i) = a02*b20+a12*b21+a22*b22;

	a00=A(0,i), a10=A(1,i), a20=A(2,i); 
	a01=A(3,i), a11=A(4,i), a21=A(5,i); 
	a02=A(6,i), a12=A(7,i), a22=A(8,i); 

	c00=C(0,j), c10=C(1,j), c20=C(2,j); 
	c01=C(3,j), c11=C(4,j), c21=C(5,j); 
	c02=C(6,j), c12=C(7,j), c22=C(8,j); 

	// b(N,N,i)-=multiply(a,i1, c,i-1);
	B(0,i) -= ( a00*c00+a10*c01+a20*c02);
	B(1,i) -= ( a00*c10+a10*c11+a20*c12);
	B(2,i) -= ( a00*c20+a10*c21+a20*c22);
	B(3,i) -= ( a01*c00+a11*c01+a21*c02);
	B(4,i) -= ( a01*c10+a11*c11+a21*c12);
	B(5,i) -= ( a01*c20+a11*c21+a21*c22);
	B(6,i) -= ( a02*c00+a12*c01+a22*c02);
	B(7,i) -= ( a02*c10+a12*c11+a22*c12);
	B(8,i) -= ( a02*c20+a12*c21+a22*c22);

	// invert(b,i);
	INVERT(B,i);
      }

#undef A
#undef B
#undef C
#undef W1
#undef W2
#undef INVERT

    }
    else
    { // general case
      invert( b,base ); // invert b0
      for( int i1=base+1; i1<=bound; i1++ )
      {
	a(N,N,i1) =multiply(a,i1, b,i1-1); // save in a: a*b^{-1}
	b(N,N,i1)-=multiply(a,i1, c,i1-1);
	invert(b,i1);
      }
    }
  }
  else if( systemType==extended )
  {
    // ***** finish this case ******

    // eliminate c[n]
    RealArray aa(N,N);
    aa=a(N,N,bound-1);
    invert(aa,0);
    c(N,N,bound)=multiply(c,bound, aa,0);    // save in c : c*a^{-1}
      
    a(N,N,bound)-=multiply(c,bound, b,bound-1);
    b(N,N,bound)-=multiply(c,bound, c,bound-1);
  
    invert( b,base ); // invert b0
    // first case is special
    int i1=base+1;
    a(N,N,i1) =multiply(a,i1, b,i1-1); // save in a: a*b^{-1}
    b(N,N,i1)-=multiply(a,i1, c,i1-1);
    invert(b,i1);
    c(N,N,base+1)-=multiply(a,base+1, a,base); // adjust c[1]
    for( i1=base+2; i1<=bound; i1++ )
    {
      a(N,N,i1) =multiply(a,i1, b,i1-1); // save in a: a*b^{-1}
      b(N,N,i1)-=multiply(a,i1, c,i1-1);
      invert(b,i1);
    }
  }
  return 0;
}


int TridiagonalSolver::
scalarBlockSolveOld(RealArray & r, int i1, int i2, int i3)
// ============================================================================================
// ============================================================================================
{
  Range N(0,blockSize-1);
  int base =Iv[axis].getBase();
  int bound=Iv[axis].getBound();

  // *** assume a,b,c are the same size ?? ****
  const int aDim0=a.getRawDataSize(0);
  const int aDim1=a.getRawDataSize(1);
  const int aDim2=a.getRawDataSize(2);
  const int aDim3=a.getRawDataSize(3);
  const int aDim4=a.getRawDataSize(4);
  const int stride = axis==0 ? aDim0*aDim1 : axis==1 ? aDim0*aDim1*aDim2 : aDim0*aDim1*aDim2*aDim3;
  const int offset = aDim0*aDim1*(i1+aDim2*(i2+aDim3*(i3)));

  real *ap = a.Array_Descriptor.Array_View_Pointer2;
  real *bp = b.Array_Descriptor.Array_View_Pointer2;
  real *cp = c.Array_Descriptor.Array_View_Pointer2;
  ap+=offset;
  bp+=offset;
  cp+=offset;

  const int rDim0=r.getRawDataSize(0);
  const int rDim1=r.getRawDataSize(1);
  const int rDim2=r.getRawDataSize(2);
  const int rDim3=r.getRawDataSize(3);
  const int rStride = axis==0 ? rDim0 : axis==1 ? rDim0*rDim1 : rDim0*rDim1*rDim2;
  const int rOffset = rDim0*(i1+rDim1*(i2+rDim2*(i3)));

  real *rp = r.Array_Descriptor.Array_View_Pointer2;
  rp+=rOffset;

  if( useOptimizedC && blockSize==2 )
  {

#define A(m,i) ap[m+stride*(i)]
#define B(m,i) bp[m+stride*(i)]
#define C(m,i) cp[m+stride*(i)]
#define R(m,i) rp[m+rStride*(i)]

    // forward elimination
    if( systemType==extended )
      r(N,bound  )-=matrixVectorMultiply(c,bound  ,r,bound-1);  // ******** fix this and below ***

    real a0,a1,a2,a3, b0,b1,b2,b3, d0,d1,d2,d3,  r0,r1;
    int i=1,j;
    const int ib=bound-base;
    for( i=1; i<=ib; i++ )
    {
      j=i-1;
      a0=A(0,i); a1=A(1,i); a2=A(2,i); a3=A(3,i);
      r0=R(0,j); r1=R(1,j);
      // r(N,i1)-=matrixVectorMultiply(a,i1, r,i1-1);
      R(0,i) -= ( a0*r0+a1*r1 );
      R(1,i) -= ( a2*r0+a3*r1 );
    }
    // back substitution
    // r(N,bound)=matrixVectorMultiply(b,bound, r,bound);
    i=ib; 
    b0=B(0,i); b1=B(1,i); b2=B(2,i); b3=B(3,i);
    r0=R(0,i); r1=R(1,i);
    R(0,i) = ( b0*r0+b1*r1 );
    R(1,i) = ( b2*r0+b3*r1 );
    for( i=ib-1; i>=0; i-- )
    { 
      // r(N,i1)=matrixVectorMultiply(b,i1, evaluate(r(N,i1)-matrixVectorMultiply(c,i1, r,i1+1))); 
      j=i+1;
      a0=C(0,i); a1=C(1,i); a2=C(2,i); a3=C(3,i);
      r0=R(0,j); r1=R(1,j);
      d0 = R(0,i)-(a0*r0+a1*r1);
      d1 = R(1,i)-(a2*r0+a3*r1);
      b0=B(0,i); b1=B(1,i); b2=B(2,i); b3=B(3,i);
      R(0,i) = ( b0*d0+b1*d1 );
      R(1,i) = ( b2*d0+b3*d1 );
    }

    if( systemType==extended )
      r(N,base)-=matrixVectorMultiply(b,base, matrixVectorMultiply(a, base, r,base+2));

#undef A
#undef B
#undef C
#undef R
#undef W1
#undef W2
      
  }
  else if( useOptimizedC && blockSize==3 )
  {

#define A(m,i) ap[m+stride*(i)]
#define B(m,i) bp[m+stride*(i)]
#define C(m,i) cp[m+stride*(i)]
#define R(m,i) rp[m+rStride*(i)]

    real a00,a10,a20,a01,a11,a21,a02,a12,a22;
    real b00,b10,b20,b01,b11,b21,b02,b12,b22;
    real d00,d10,d20,d01,d11,d21,d02,d12,d22;
    real r0,r1,r2;
      
    // forward elimination
    if( systemType==extended )
      r(N,bound  )-=matrixVectorMultiply(c,bound  ,r,bound-1);  // ******** fix this and below ***

    int i=1,j;
    const int ib=bound-base;
    for( i=1; i<=ib; i++ )
    {
      j=i-1;
      a00=A(0,i), a10=A(1,i), a20=A(2,i); 
      a01=A(3,i), a11=A(4,i), a21=A(5,i); 
      a02=A(6,i), a12=A(7,i), a22=A(8,i); 
      r0=R(0,j); r1=R(1,j); r2=R(2,j);
      // r(N,i1)-=matrixVectorMultiply(a,i1, r,i1-1);
      R(0,i) -= (a00*r0+a10*r1+a20*r2);
      R(1,i) -= (a01*r0+a11*r1+a21*r2);
      R(2,i) -= (a02*r0+a12*r1+a22*r2);
    }
    // back substitution
    // r(N,bound)=matrixVectorMultiply(b,bound, r,bound);
    i=ib; 
    b00=B(0,i), b10=B(1,i), b20=B(2,i); 
    b01=B(3,i), b11=B(4,i), b21=B(5,i); 
    b02=B(6,i), b12=B(7,i), b22=B(8,i); 
    r0=R(0,i); r1=R(1,i); r2=R(2,i);
    R(0,i) = (b00*r0+b10*r1+b20*r2);
    R(1,i) = (b01*r0+b11*r1+b21*r2);
    R(2,i) = (b02*r0+b12*r1+b22*r2);

    for( i=ib-1; i>=0; i-- )
    { 
      // r(N,i1)=matrixVectorMultiply(b,i1, evaluate(r(N,i1)-matrixVectorMultiply(c,i1, r,i1+1))); 
      j=i+1;
      a00=C(0,i), a10=C(1,i), a20=C(2,i); 
      a01=C(3,i), a11=C(4,i), a21=C(5,i); 
      a02=C(6,i), a12=C(7,i), a22=C(8,i); 
      r0=R(0,j); r1=R(1,j); r2=R(2,j);
      d00 = R(0,i) - (a00*r0+a10*r1+a20*r2);
      d10 = R(1,i) - (a01*r0+a11*r1+a21*r2);
      d20 = R(2,i) - (a02*r0+a12*r1+a22*r2);
      b00=B(0,i), b10=B(1,i), b20=B(2,i); 
      b01=B(3,i), b11=B(4,i), b21=B(5,i); 
      b02=B(6,i), b12=B(7,i), b22=B(8,i); 
      R(0,i) = (b00*d00+b10*d10+b20*d20);
      R(1,i) = (b01*d00+b11*d10+b21*d20);
      R(2,i) = (b02*d00+b12*d10+b22*d20);

    }
    if( systemType==extended )
      r(N,base)-=matrixVectorMultiply(b,base, matrixVectorMultiply(a, base, r,base+2));

#undef A
#undef B
#undef C
#undef R

  }
  else
  { // general case

    // forward elimination
    int i1;
    if( systemType==extended )
      r(N,bound  )-=matrixVectorMultiply(c,bound  ,r,bound-1);

    for( i1=base+1; i1<=bound; i1++ )
      r(N,i1)-=matrixVectorMultiply(a,i1, r,i1-1);

    // back substitution
    r(N,bound)=matrixVectorMultiply(b,bound, r,bound);
    for( i1=bound-1; i1>=base; i1-- )
    { //  b^{-1}[ r_i - c_i*r_{i+1} ]
      r(N,i1)=matrixVectorMultiply(b,i1, evaluate(r(N,i1)-matrixVectorMultiply(c,i1, r,i1+1)));  
    }
    
    if( systemType==extended )
      r(N,base)-=matrixVectorMultiply(b,base, matrixVectorMultiply(a, base, r,base+2));
  }

  return 0;
}
