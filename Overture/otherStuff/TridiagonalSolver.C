#include "TridiagonalSolver.h"
#include "OvertureInit.h"
#include "mathutil.h"

//\begin{>TridiagonalSolverInclude.tex}{\subsection{constructor}}
TridiagonalSolver::
TridiagonalSolver() 
  : I1(Iv[0]), I2(Iv[1]), I3(Iv[2])   // define references
// ============================================================================
// /Description:
//   Use this class to solve a tridiagonal system or a pentadiagonal system. 
//  The system may be block tridiagonal. There may
//  be multiple independent tridiagonal (pentadiagonal) systems to be solved.
//  The basic tridiagonal system is ({\tt type=normal})
// \begin{verbatim}
//            | b[0] c[0]                     |
//            | a[1] b[1] c[1]                |
//        A = |      a[2] b[2] c[2]           |
//            |            .    .    .        |
//            |                a[.] b[.] c[.] |
//            |                     a[n] b[n] |
//
// \end{verbatim}
// We can also solve the {\tt type=periodic}
// \begin{verbatim}
//            | b[0] c[0]                a[0] |
//            | a[1] b[1] c[1]                |
//        A = |      a[2] b[2] c[2]           |
//            |            .    .    .        |
//            |                a[.] b[.] c[.] |
//            | c[n]                a[n] b[n] |
//
// \end{verbatim}
// and the {\tt type=extended}
// \begin{verbatim}
//            | b[0] c[0] a[0]                |
//            | a[1] b[1] c[1]                |
//        A = |      a[2] b[2] c[2]           |
//            |            .    .    .        |
//            |                a[.] b[.] c[.] |
//            |                c[n] a[n] b[n] |
//
// \end{verbatim}
// which may occur with certain boundary conditions.
//
// This class expects the matrices a,b,c to be passed separately and to be of the 
// form
// \begin{itemize}
//  \item a(I1,I2,I3), b(I1,I2,I3), c(I1,I2,I3) : if the block size is 1.
//  \item a(b,b,I1,I2) : if the block size is b$>1$.
// \end{itemize}
//  The `{\tt axis}' argument to the member functions indicates which of I1,I2 or I3
//  represents the axis along which the tridiagonal matrix extends. The other axes
//  can be used to hold independent tridiagonal systems. Thus if {\tt axis=0} then
//  {\tt a(i1,i2,i3) i1=0,1,2,...,n} are the entries in the tridiagonal matrix for
//  each fixed i2 and i3. If {\tt axis=1} then {\tt a(i1,i2,i3) i2=0,1,2,...,n} 
// are the entries in the tridiagonal matrix for fixed i1 and i3.
//\end{TridiagonalSolverInclude.tex} 
// ================================================================================
{
#ifndef USE_PPP
  useOptimizedC=true;
#else
  useOptimizedC=false;
#endif

  bandWidth=3;
  
}

TridiagonalSolver::
~TridiagonalSolver()
{
}

#define pentaFactor EXTERN_C_NAME(pentafactor)
#define pentaSolve  EXTERN_C_NAME(pentasolve)
#define triFactor EXTERN_C_NAME(trifactor)
#define triSolve  EXTERN_C_NAME(trisolve)
extern "C"
{
  void pentaFactor(const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,const int &nd3a,const int &nd3b,\
		   const int &ipar, real&a, real&b, real&c, real&d, real&e,
                                    real&w1,real&w2,real&w3,real&w4 );

  void pentaSolve(const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,const int &nd3a,const int &nd3b,\
	    const int &ndf1a,const int &ndf1b,const int &ndf2a,const int &ndf2b,const int &ndf3a,const int &ndf3b,\
                  const int &ipar, real&a, real&b, real&c, real&d, real&e, real&f,
                                    real&w1,real&w2,real&w3,real&w4 );

//    void triFactor(const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,const int &nd3a,const int &nd3b,\
//  		   const int &ipar, real&a, real&b, real&c,
//                                      real&w1,real&w2 );

//    void triSolve(const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,const int &nd3a,const int &nd3b,\
//  		   const int &ipar, real&a, real&b, real&c, real&d, 
//                                      real&w1,real&w2 );
}



//\begin{>>TridiagonalSolverInclude.tex}{\subsection{factor}}
int TridiagonalSolver::
factor(RealArray & a_, 
       RealArray & b_, 
       RealArray & c_, 
       const SystemType & type_ /* =normal */,
       const int & axis_ /* =0 */,
       const int & block /* =1 */ )
// ========================================================================================
// /Description:
//   Factor the tri-diagonal (block) matrix defined by (a,b,c).
//   NOTE: This routine keeps a reference to (a,b,c) and factors in place. 
// /a,b,c (input/output) : on input the 3 diagonals, on output the LU factorization
// /type (input) : One of {\tt normal}, {\tt periodic} or {\tt extended}.
// /axis (input) : 0, 1, or 2. See the comments below.
// /block (input) : block size. If block=2 or 3 then the matrix is block tridiagonal.
// /Notes:
// This class expects the matrices a,b,c to be of the form
// \begin{itemize}
//  \item a(I1,I2,I3), b(I1,I2,I3), c(I1,I2,I3) : if the block size is 1.
//  \item a(b,b,I1,I2), b(b,b,I1,I2), c(b,b,I1,I2) : if the block size is b$>1$.
// \end{itemize}
//  The `{\tt axis}' argument to the member functions indicates which of I1,I2 or I3
//  represents the axis along which the tridiagonal matrix extends. The other axes
//  can be used to hold independent tridiagonal systems. Thus if {\tt axis=0} then
//  {\tt a(i1,i2,i3) i1=0,1,2,...,n} are the entries in the tridiagonal matrix for
//  each fixed i2 and i3. If {\tt axis=1} then {\tt a(i1,i2,i3) i2=0,1,2,...,n} 
// are the entries in the tridiagonal matrix for fixed i1 and i3.
//\end{TridiagonalSolverInclude.tex} 
// ========================================================================================
{
  blockSize=block;
  bandWidth=3;     // this means we are solving a tri-diagonal system
  
  a.reference(a_);
  b.reference(b_);
  c.reference(c_);
  d.redim(0);
  e.redim(0);
  
  systemType=type_;
  axis=axis_;


  const int na = blockSize==1 ? 0 : 2;
  I1=a.dimension(0+na);
  I2=a.dimension(1+na);
  I3=a.dimension(2+na);
  if( systemType==extended && Iv[axis].getLength()<6 )
  {
    printf(" TridiagonalSolver::factor:ERROR: extended system not implemented for less than 6 points\n");
    Overture::abort("error");
  }
  scalarSystem = axis==0 && I2.getLength()==1 && I3.getLength()==1;
  // if( scalarSystem )
  //  printf("*** TridiagonalSolver: solving a scalar system\n");
  
  if( systemType==periodic )
  {
    w1.redim(a);  // work space
    w2.redim(a);
    periodicTridiagonalFactor();
  }
  else
  {
//      if( true && blockSize==1 && systemType!=periodic )
//      {
//        // use opt fortran version

//    int ipar[]={I1.getBase(),
//                I1.getBound(),
//                I1.getStride(),
//                I2.getBase(),
//                I2.getBound(),
//                I2.getStride(),
//                I3.getBase(),
//                I3.getBound(),
//                I3.getStride(),
//                (int)systemType,
//                axis};  //
//        assert( a.dimension(0)==b.dimension(0) && a.dimension(1)==b.dimension(1) && a.dimension(2)==b.dimension(2) );
//        assert( a.dimension(0)==c.dimension(0) && a.dimension(1)==c.dimension(1) && a.dimension(2)==c.dimension(2) );
//        assert( a.dimension(0)==c.dimension(0) && a.dimension(1)==c.dimension(1) && a.dimension(2)==c.dimension(2) );

//        int ne=0;
//        w1.redim(1);
      
//        // what if a,b,c,.. are views??
//        triFactor(a.getBase(0),a.getBound(0),a.getBase(1),a.getBound(1),a.getBase(2),a.getBound(2),
//  		ipar[0],
//  		*getDataPointer(a), 
//  		*getDataPointer(b), 
//  		*getDataPointer(c), 
//  		w1(0),w1(ne));
//      }
//      else
//      {
      tridiagonalFactor();
//    }
  }
  
  return 0;
}



//\begin{>>TridiagonalSolverInclude.tex}{\subsection{factor}}
int TridiagonalSolver::
factor(RealArray & a_, 
       RealArray & b_, 
       RealArray & c_, 
       RealArray & d_, 
       RealArray & e_, 
       const SystemType & type_ /* =normal */,
       const int & axis_ /* =0 */,
       const int & block /* =1 */ )
// ========================================================================================
// /Description:
//   Factor the penta-diagonal (block) matrix defined by (a,b,c,d,e).
//   NOTE: This routine keeps a reference to (a,b,c,d,e) and factors in place. 
// /a,b,c,d,e (input/output) : on input the 5 diagonals, on output the LU factorization
// /type (input) : One of {\tt normal}, {\tt periodic} or {\tt extended}.
// /axis (input) : 0, 1, or 2. See the comments below.
// /block (input) : block size. If block=2 or 3 then the matrix is block tridiagonal.
// /Notes:
// This class expects the matrices a,b,c to be of the form
// \begin{itemize}
//  \item a(I1,I2,I3), b(I1,I2,I3), c(I1,I2,I3), d(I1,I2,I3), e(I1,I2,I3) : if the block size is 1.
//  \item a(b,b,I1,I2), b(b,b,I1,I2), c(b,b,I1,I2) d(b,b,I1,I2), e(b,b,I1,I2) : if the block size is b$>1$.
// \end{itemize}
//  The `{\tt axis}' argument to the member functions indicates which of I1,I2 or I3
//  represents the axis along which the tridiagonal matrix extends. The other axes
//  can be used to hold independent tridiagonal systems. Thus if {\tt axis=0} then
//  {\tt a(i1,i2,i3) i1=0,1,2,...,n} are the entries in the tridiagonal matrix for
//  each fixed i2 and i3. If {\tt axis=1} then {\tt a(i1,i2,i3) i2=0,1,2,...,n} 
// are the entries in the tridiagonal matrix for fixed i1 and i3.
//\end{TridiagonalSolverInclude.tex} 
// ========================================================================================
{
  blockSize=block;
  bandWidth=5;             // this means we are solving a penta-diagonal system
  
  a.reference(a_);
  b.reference(b_);
  c.reference(c_);
  d.reference(d_);
  e.reference(e_);
  systemType=type_;
  axis=axis_;


  const int na = blockSize==1 ? 0 : 2;
  I1=a.dimension(0+na);
  I2=a.dimension(1+na);
  I3=a.dimension(2+na);
  if( systemType==extended && Iv[axis].getLength()<6 )
  {
    printf(" TridiagonalSolver::factor:ERROR: extended system not implemented for less than 6 points\n");
    Overture::abort("error");
  }
  scalarSystem = axis==0 && I2.getLength()==1 && I3.getLength()==1;
  // if( scalarSystem )
  //  printf("*** TridiagonalSolver: solving a scalar system\n");
  
  int ne=0; 
  if( systemType==periodic )
  {
    ne=a.elementCount();
    w1.redim(ne*4);  // work space
  }
  else
  {
    w1.redim(1);
    w2.redim(0);
  }

  // printf("pentadiagonalFactor...\n");

  int ipar[]={I1.getBase(),
              I1.getBound(),
              I1.getStride(),
              I2.getBase(),
              I2.getBound(),
              I2.getStride(),
              I3.getBase(),
              I3.getBound(),
              I3.getStride(),
              (int)systemType,
              axis};  //

  assert( a.dimension(0)==b.dimension(0) && a.dimension(1)==b.dimension(1) && a.dimension(2)==b.dimension(2) );
  assert( a.dimension(0)==c.dimension(0) && a.dimension(1)==c.dimension(1) && a.dimension(2)==c.dimension(2) );
  assert( a.dimension(0)==c.dimension(0) && a.dimension(1)==c.dimension(1) && a.dimension(2)==c.dimension(2) );
  assert( a.dimension(0)==d.dimension(0) && a.dimension(1)==d.dimension(1) && a.dimension(2)==d.dimension(2) );
  assert( a.dimension(0)==e.dimension(0) && a.dimension(1)==e.dimension(1) && a.dimension(2)==e.dimension(2) );

  // what if a,b,c,.. are views??
  pentaFactor(a.getBase(0),a.getBound(0),a.getBase(1),a.getBound(1),a.getBase(2),a.getBound(2),
              ipar[0],
              *getDataPointer(a), 
              *getDataPointer(b), 
              *getDataPointer(c), 
              *getDataPointer(d), 
              *getDataPointer(e),
              w1(0),w1(ne),w1(2*ne),w1(3*ne));

  return 0;
}

//\begin{>>TridiagonalSolverInclude.tex}{\subsection{solve}}
real TridiagonalSolver::
sizeOf( FILE *file /* =NULL */ ) const 
//===================================================================================
// /Description: 
//   Return number of bytes allocated by Oges; optionally print detailed info to a file
//
// /file (input) : optinally supply a file to write detailed info to. Choose file=stdout to
// write to standard output.
// /Return value: the number of bytes.
//\end{TridiagonalSolverInclude.tex} 
//===================================================================================
{
  real size=0.;

  real sizeOfThis=sizeof(*this);
  size+=sizeOfThis;
  size+=(a.elementCount()+b.elementCount()+c.elementCount()+d.elementCount()+e.elementCount()+
         w1.elementCount()+w2.elementCount())*sizeof(real);

  return size;
}

//\begin{>>TridiagonalSolverInclude.tex}{\subsection{solve}}
int TridiagonalSolver::
solve(const RealArray & r_,  // this is not really const
      const Range & R1 /* =nullRange */, 
      const Range & R2 /* =nullRange */, 
      const Range & R3 /* =nullRange */ )
// ========================================================================================
// /Description:
//    Solve a set of tri-diagonal systems (or penta-diagonal systems). 
// /r\_ (input/output) : rhs vector on input, solution on output. This is declared const to avoid compiler
//    warnings.
// /R1,R2,R3: Specifies which systems to solve. By default all the systems are solved.
//  These Ranges must be a subset of the collection of
//  systems that are found in the matrices passed to the {\tt factor} function.
//  One of these is arguments is ignored, the one corresponding to the axis along which the
//  tridiagonal system extends. 
//\end{TridiagonalSolverInclude.tex} 
// ========================================================================================
{
  RealArray & r = (RealArray &)r_;  // cast away const
  
  const int na = blockSize==1 ? 0 : 2;
  I1= (axis==axis1 || R1==nullRange) ? a.dimension(na  ) : R1;
  I2= (axis==axis2 || R2==nullRange) ? a.dimension(na+1) : R2;
  I3= (axis==axis3 || R3==nullRange) ? a.dimension(na+2) : R3;

  
  if( bandWidth==3 )
  {
//      if( true && blockSize==1 && systemType!=periodic )
//      {
//        // use opt fortran version
//    int ipar[]={I1.getBase(),
//                I1.getBound(),
//                I1.getStride(),
//                I2.getBase(),
//                I2.getBound(),
//                I2.getStride(),
//                I3.getBase(),
//                I3.getBound(),
//                I3.getStride(),
//                (int)systemType,
//                axis};  //

//        assert( a.dimension(0)==b.dimension(0) && a.dimension(1)==b.dimension(1) && a.dimension(2)==b.dimension(2) );
//        assert( a.dimension(0)==c.dimension(0) && a.dimension(1)==c.dimension(1) && a.dimension(2)==c.dimension(2) );
//        assert( a.dimension(0)==c.dimension(0) && a.dimension(1)==c.dimension(1) && a.dimension(2)==c.dimension(2) );

//      // what if a,b,c,.. are views??
//        const int ne= systemType==periodic ? a.elementCount() : 0;
//        triSolve(a.getBase(0),a.getBound(0),a.getBase(1),a.getBound(1),a.getBase(2),a.getBound(2),
//  	       ipar[0],
//  	       *getDataPointer(a), 
//  	       *getDataPointer(b), 
//  	       *getDataPointer(c), 
//  	       *getDataPointer(r),
//  	       w1(0),w1(ne) );

//        return 0;

//      }
//      else
//      {
      if( systemType==periodic )
	return periodicTridiagonalSolve(r);
      else
	return tridiagonalSolve(r);
//    }
    
  }
  else if( bandWidth==5 )
  {
    // **** penta-diagonal solve ****
    // printf("pentadiagonalSolve...\n");

  int ipar[]={I1.getBase(),
              I1.getBound(),
              I1.getStride(),
              I2.getBase(),
              I2.getBound(),
              I2.getStride(),
              I3.getBase(),
              I3.getBound(),
              I3.getStride(),
              (int)systemType,
              axis};  //

    assert( a.dimension(0)==b.dimension(0) && a.dimension(1)==b.dimension(1) && a.dimension(2)==b.dimension(2) );
    assert( a.dimension(0)==c.dimension(0) && a.dimension(1)==c.dimension(1) && a.dimension(2)==c.dimension(2) );
    assert( a.dimension(0)==c.dimension(0) && a.dimension(1)==c.dimension(1) && a.dimension(2)==c.dimension(2) );
    assert( a.dimension(0)==d.dimension(0) && a.dimension(1)==d.dimension(1) && a.dimension(2)==d.dimension(2) );
    assert( a.dimension(0)==e.dimension(0) && a.dimension(1)==e.dimension(1) && a.dimension(2)==e.dimension(2) );

    // what if a,b,c,.. are views??

    const int ne= systemType==periodic ? a.elementCount() : 0;
    pentaSolve(a.getBase(0),a.getBound(0),a.getBase(1),a.getBound(1),a.getBase(2),a.getBound(2),
               r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),r.getBase(2),r.getBound(2),
	       ipar[0],
	       *getDataPointer(a), 
	       *getDataPointer(b), 
	       *getDataPointer(c), 
	       *getDataPointer(d), 
	       *getDataPointer(e), 
	       *getDataPointer(r),
              w1(0),w1(ne),w1(2*ne),w1(3*ne) );

    return 0;
  }
  else
  {
    printf("TridiagonalSolver::solve:ERROR: unknown bandWidth=%i\n",bandWidth);
    Overture::abort();
    
    return 1;
  }
  
  
}

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

// int TridiagonalSolver::
// tridiagonalFactor()
// // ----------------------------------------------------
// // Factor the tridiagonal system:
// //
// //            | b[0] c[0] a[0]                |
// //            | a[1] b[1] c[1]                |
// //        A = |      a[2] b[2] c[2]           |
// //            |            .    .    .        |
// //            |                a[.] b[.] c[.] |
// //            |                c[n] a[n] b[n] |
// //
// // Input:  a, b, c : arrays denoting the 3 diagonals. 
// //  extended system: a[0]!=0, c[n]!=0
// //
// // ------------------------------------------------------
// { 
//   if( blockSize==1 )
//   {
//     if( axis==axis1 )
//     {
//       int base =I1.getBase();  
//       int bound=I1.getBound();  
//       if( false && I2.getLength()==1 && I3.getLength()==1 )  // this needs to be tested.
//       {
//         printf("use opt tridiagonal solver\n");
	
// 	real *ap=a.getDataPointer();
// 	real *bp=b.getDataPointer();
// 	real *cp=c.getDataPointer();
// 	const int num=bound-base+1;
// 	if( systemType!=extended )
// 	{
// 	  for( int i=1; i<num; i++ )  
// 	  {  
// 	    ap[i]/=bp[i-1];  
// 	    bp[i]-=ap[i]*cp[i-1];  
// 	  }  
// 	}
// 	else
// 	{
// 	  for( int i=1; i<num; i++ )  
// 	  {  
// 	    ap[i]/=bp[i-1];  
// 	    bp[i]-=ap[i]*cp[i-1];  
// 	    if( i==base+1 )  
// 	      cp[base+1]-=ap[base+1]*ap[base]; /* adjust c[1] */  
// 	    if( i==bound-1 )  
// 	    {  /* adjust row n at step n-1 */  
// 	      cp[bound]/=bp[i-1];                   /* save the factor here */ 
// 	      ap[bound]-=cp[bound]*cp[i-1];      
// 	    }  
// 	  }  
// 	}
//       }
//       else
//       {
// #undef LI
// #define LI
// #undef RI
// #define RI ,I2,I3
// 	FACTOR(I1);
//       }
//     }
//     else if( axis==axis2 )
//     {
// #undef LI
// #define LI I1,
// #undef RI
// #define RI ,I3
//       FACTOR(I2)
// 	}
//     else if( axis==axis3 )
//     {
// #undef LI
// #define LI I1,I2,
// #undef RI
// #define RI 
//       FACTOR(I3)
// 	}
//     else
//     {
//       cout << "tridiagonalFactor::ERROR: invalid value for axis = " << axis << endl;
//       throw "error";
//     }
//   }
//   else
//   {
//     // block tridiagonal system
//     blockFactor();
//   }
  
//   //a.display("a, After factor");
//   //b.display("b, After factor");
//   //c.display("c, After factor");

//   return 0;
// }

#undef FACTOR

int TridiagonalSolver::
invert(RealArray & d, const int & i1 )
{
  if( blockSize==2 )
  {
    const real & deti = 1./(d(0,0,i1)*d(1,1,i1)-d(0,1,i1)*d(1,0,i1));
    real d00= d(1,1,i1)*deti;
    real d10=-d(1,0,i1)*deti;
    real d01=-d(0,1,i1)*deti;
    real d11= d(0,0,i1)*deti;
    d(0,0,i1)=d00;
    d(1,0,i1)=d10;
    d(0,1,i1)=d01;
    d(1,1,i1)=d11;
  }
  else if( blockSize==3 )
  {
    const real & deti = 1./(
      d(0,0,i1)*(d(1,1,i1)*d(2,2,i1)-d(1,2,i1)*d(2,1,i1))+
      d(1,0,i1)*(d(2,1,i1)*d(0,2,i1)-d(2,2,i1)*d(0,1,i1))+
      d(2,0,i1)*(d(0,1,i1)*d(1,2,i1)-d(0,2,i1)*d(1,1,i1))  );
    real d00= (d(1,1,i1)*d(2,2,i1)-d(1,2,i1)*d(2,1,i1))*deti;
    real d01= (d(2,1,i1)*d(0,2,i1)-d(2,2,i1)*d(0,1,i1))*deti;
    real d02= (d(0,1,i1)*d(1,2,i1)-d(0,2,i1)*d(1,1,i1))*deti;

    real d10= (d(1,2,i1)*d(2,0,i1)-d(1,0,i1)*d(2,2,i1))*deti;
    real d11= (d(2,2,i1)*d(0,0,i1)-d(2,0,i1)*d(0,2,i1))*deti;
    real d12= (d(0,2,i1)*d(1,0,i1)-d(0,0,i1)*d(1,2,i1))*deti;

    real d20= (d(1,0,i1)*d(2,1,i1)-d(1,1,i1)*d(2,0,i1))*deti;
    real d21= (d(2,0,i1)*d(0,1,i1)-d(2,1,i1)*d(0,0,i1))*deti;
    real d22= (d(0,0,i1)*d(1,1,i1)-d(0,1,i1)*d(1,0,i1))*deti;

    d(0,0,i1)=d00;
    d(1,0,i1)=d10;
    d(2,0,i1)=d20;

    d(0,1,i1)=d01;
    d(1,1,i1)=d11;
    d(2,1,i1)=d21;

    d(0,2,i1)=d02;
    d(1,2,i1)=d12;
    d(2,2,i1)=d22;

  }
  else
  {
    printf("TridiagonalSolver::invert:fatal error. This should not occur.\n");
    Overture::abort("error");
  }
  return 0;
}

int TridiagonalSolver::
invert(RealArray & d, const Index & K1, const Index & K2, const Index & K3 )
{
  if( blockSize==2 )
  {
    const RealArray & deti = evaluate(1./(d(0,0,K1,K2,K3)*d(1,1,K1,K2,K3)-d(0,1,K1,K2,K3)*d(1,0,K1,K2,K3)));

    const RealArray & d00=evaluate( d(1,1,K1,K2,K3)*deti);
    const RealArray & d10=evaluate(-d(1,0,K1,K2,K3)*deti);
    const RealArray & d01=evaluate(-d(0,1,K1,K2,K3)*deti);
    const RealArray & d11=evaluate( d(0,0,K1,K2,K3)*deti);
    d(0,0,K1,K2,K3)=d00;
    d(1,0,K1,K2,K3)=d10;
    d(0,1,K1,K2,K3)=d01;
    d(1,1,K1,K2,K3)=d11;
  }
  else if( blockSize==3 )
  {
    const RealArray & deti = evaluate(1./(
      d(0,0,K1,K2,K3)*(d(1,1,K1,K2,K3)*d(2,2,K1,K2,K3)-d(1,2,K1,K2,K3)*d(2,1,K1,K2,K3))+
      d(1,0,K1,K2,K3)*(d(2,1,K1,K2,K3)*d(0,2,K1,K2,K3)-d(2,2,K1,K2,K3)*d(0,1,K1,K2,K3))+
      d(2,0,K1,K2,K3)*(d(0,1,K1,K2,K3)*d(1,2,K1,K2,K3)-d(0,2,K1,K2,K3)*d(1,1,K1,K2,K3))  ));
    const RealArray & d00=evaluate( (d(1,1,K1,K2,K3)*d(2,2,K1,K2,K3)-d(1,2,K1,K2,K3)*d(2,1,K1,K2,K3))*deti);
    const RealArray & d01=evaluate( (d(2,1,K1,K2,K3)*d(0,2,K1,K2,K3)-d(2,2,K1,K2,K3)*d(0,1,K1,K2,K3))*deti);
    const RealArray & d02=evaluate( (d(0,1,K1,K2,K3)*d(1,2,K1,K2,K3)-d(0,2,K1,K2,K3)*d(1,1,K1,K2,K3))*deti);

    const RealArray & d10=evaluate( (d(1,2,K1,K2,K3)*d(2,0,K1,K2,K3)-d(1,0,K1,K2,K3)*d(2,2,K1,K2,K3))*deti);
    const RealArray & d11=evaluate( (d(2,2,K1,K2,K3)*d(0,0,K1,K2,K3)-d(2,0,K1,K2,K3)*d(0,2,K1,K2,K3))*deti);
    const RealArray & d12=evaluate( (d(0,2,K1,K2,K3)*d(1,0,K1,K2,K3)-d(0,0,K1,K2,K3)*d(1,2,K1,K2,K3))*deti);

    const RealArray & d20=evaluate( (d(1,0,K1,K2,K3)*d(2,1,K1,K2,K3)-d(1,1,K1,K2,K3)*d(2,0,K1,K2,K3))*deti);
    const RealArray & d21=evaluate( (d(2,0,K1,K2,K3)*d(0,1,K1,K2,K3)-d(2,1,K1,K2,K3)*d(0,0,K1,K2,K3))*deti);
    const RealArray & d22=evaluate( (d(0,0,K1,K2,K3)*d(1,1,K1,K2,K3)-d(0,1,K1,K2,K3)*d(1,0,K1,K2,K3))*deti);

    d(0,0,K1,K2,K3)=d00;
    d(1,0,K1,K2,K3)=d10;
    d(2,0,K1,K2,K3)=d20;

    d(0,1,K1,K2,K3)=d01;
    d(1,1,K1,K2,K3)=d11;
    d(2,1,K1,K2,K3)=d21;

    d(0,2,K1,K2,K3)=d02;
    d(1,2,K1,K2,K3)=d12;
    d(2,2,K1,K2,K3)=d22;

  }
  else
  {
    printf("TridiagonalSolver::invert:fatal error. This should not occur.\n");
    Overture::abort("error");
  }
  
  return 0;
}

RealArray TridiagonalSolver::
multiply( const RealArray & d, const int & i1, const RealArray & e, const int & j1)
{
  RealArray r(blockSize,blockSize);
  if( blockSize==2 )
  {
    r(0,0) = d(0,0,i1)*e(0,0,j1)+d(1,0,i1)*e(0,1,j1);
    r(1,0) = d(0,0,i1)*e(1,0,j1)+d(1,0,i1)*e(1,1,j1);
    r(0,1) = d(0,1,i1)*e(0,0,j1)+d(1,1,i1)*e(0,1,j1);
    r(1,1) = d(0,1,i1)*e(1,0,j1)+d(1,1,i1)*e(1,1,j1);
  }
  else if( blockSize==3 )
  {
    r(0,0) = d(0,0,i1)*e(0,0,j1)+d(1,0,i1)*e(0,1,j1)+d(2,0,i1)*e(0,2,j1);
    r(1,0) = d(0,0,i1)*e(1,0,j1)+d(1,0,i1)*e(1,1,j1)+d(2,0,i1)*e(1,2,j1);
    r(2,0) = d(0,0,i1)*e(2,0,j1)+d(1,0,i1)*e(2,1,j1)+d(2,0,i1)*e(2,2,j1);

    r(0,1) = d(0,1,i1)*e(0,0,j1)+d(1,1,i1)*e(0,1,j1)+d(2,1,i1)*e(0,2,j1);
    r(1,1) = d(0,1,i1)*e(1,0,j1)+d(1,1,i1)*e(1,1,j1)+d(2,1,i1)*e(1,2,j1);
    r(2,1) = d(0,1,i1)*e(2,0,j1)+d(1,1,i1)*e(2,1,j1)+d(2,1,i1)*e(2,2,j1);

    r(0,2) = d(0,2,i1)*e(0,0,j1)+d(1,2,i1)*e(0,1,j1)+d(2,2,i1)*e(0,2,j1);
    r(1,2) = d(0,2,i1)*e(1,0,j1)+d(1,2,i1)*e(1,1,j1)+d(2,2,i1)*e(1,2,j1);
    r(2,2) = d(0,2,i1)*e(2,0,j1)+d(1,2,i1)*e(2,1,j1)+d(2,2,i1)*e(2,2,j1);

  }
  else
  {
    Overture::abort("error");
  }
  return r;
}


RealArray TridiagonalSolver::
matrixVectorMultiply( const RealArray & d, const int & i1, const RealArray & e, const int & j1)
{
  RealArray r(blockSize);
  if( blockSize==2 )
  {
    r(0) = d(0,0,i1)*e(0,j1)+d(1,0,i1)*e(1,j1);
    r(1) = d(0,1,i1)*e(0,j1)+d(1,1,i1)*e(1,j1);
  } 
  else if( blockSize==3 )
  {
    r(0) = d(0,0,i1)*e(0,j1)+d(1,0,i1)*e(1,j1)+d(2,0,i1)*e(2,j1);
    r(1) = d(0,1,i1)*e(0,j1)+d(1,1,i1)*e(1,j1)+d(2,1,i1)*e(2,j1);
    r(2) = d(0,2,i1)*e(0,j1)+d(1,2,i1)*e(1,j1)+d(2,2,i1)*e(2,j1);
  }
  else
  {
    Overture::abort("error");
  }
  return r;
}

RealArray TridiagonalSolver::
matrixVectorMultiply( const RealArray & d, const int & i1, const RealArray & e)
{
  RealArray r(blockSize);
  if( blockSize==2 )
  {
    r(0) = d(0,0,i1)*e(0)+d(1,0,i1)*e(1);
    r(1) = d(0,1,i1)*e(0)+d(1,1,i1)*e(1);
  }
  else if( blockSize==3 )
  {
    r(0) = d(0,0,i1)*e(0)+d(1,0,i1)*e(1)+d(2,0,i1)*e(2);
    r(1) = d(0,1,i1)*e(0)+d(1,1,i1)*e(1)+d(2,1,i1)*e(2);
    r(2) = d(0,2,i1)*e(0)+d(1,2,i1)*e(1)+d(2,2,i1)*e(2);
  }
  else
  {
    Overture::abort("error");
  }
  return r;
}

RealArray TridiagonalSolver::
multiply(const RealArray & d, const Index & K1, const Index & K2, const Index & K3, 
         const RealArray & e, const Index & J1, const Index & J2, const Index & J3)
{
  RealArray r(blockSize,blockSize,K1,K2,K3);
  if( blockSize==2 )
  {
    r(0,0,K1,K2,K3) = d(0,0,K1,K2,K3)*e(0,0,J1,J2,J3)+d(1,0,K1,K2,K3)*e(0,1,J1,J2,J3);
    r(1,0,K1,K2,K3) = d(0,0,K1,K2,K3)*e(1,0,J1,J2,J3)+d(1,0,K1,K2,K3)*e(1,1,J1,J2,J3);
    r(0,1,K1,K2,K3) = d(0,1,K1,K2,K3)*e(0,0,J1,J2,J3)+d(1,1,K1,K2,K3)*e(0,1,J1,J2,J3);
    r(1,1,K1,K2,K3) = d(0,1,K1,K2,K3)*e(1,0,J1,J2,J3)+d(1,1,K1,K2,K3)*e(1,1,J1,J2,J3);
  }
  else if( blockSize==3 )
  {
    r(0,0,K1,K2,K3) = d(0,0,K1,K2,K3)*e(0,0,J1,J2,J3)+d(1,0,K1,K2,K3)*e(0,1,J1,J2,J3)+d(2,0,K1,K2,K3)*e(0,2,J1,J2,J3);
    r(1,0,K1,K2,K3) = d(0,0,K1,K2,K3)*e(1,0,J1,J2,J3)+d(1,0,K1,K2,K3)*e(1,1,J1,J2,J3)+d(2,0,K1,K2,K3)*e(1,2,J1,J2,J3);
    r(2,0,K1,K2,K3) = d(0,0,K1,K2,K3)*e(2,0,J1,J2,J3)+d(1,0,K1,K2,K3)*e(2,1,J1,J2,J3)+d(2,0,K1,K2,K3)*e(2,2,J1,J2,J3);

    r(0,1,K1,K2,K3) = d(0,1,K1,K2,K3)*e(0,0,J1,J2,J3)+d(1,1,K1,K2,K3)*e(0,1,J1,J2,J3)+d(2,1,K1,K2,K3)*e(0,2,J1,J2,J3);
    r(1,1,K1,K2,K3) = d(0,1,K1,K2,K3)*e(1,0,J1,J2,J3)+d(1,1,K1,K2,K3)*e(1,1,J1,J2,J3)+d(2,1,K1,K2,K3)*e(1,2,J1,J2,J3);
    r(2,1,K1,K2,K3) = d(0,1,K1,K2,K3)*e(2,0,J1,J2,J3)+d(1,1,K1,K2,K3)*e(2,1,J1,J2,J3)+d(2,1,K1,K2,K3)*e(2,2,J1,J2,J3);

    r(0,2,K1,K2,K3) = d(0,2,K1,K2,K3)*e(0,0,J1,J2,J3)+d(1,2,K1,K2,K3)*e(0,1,J1,J2,J3)+d(2,2,K1,K2,K3)*e(0,2,J1,J2,J3);
    r(1,2,K1,K2,K3) = d(0,2,K1,K2,K3)*e(1,0,J1,J2,J3)+d(1,2,K1,K2,K3)*e(1,1,J1,J2,J3)+d(2,2,K1,K2,K3)*e(1,2,J1,J2,J3);
    r(2,2,K1,K2,K3) = d(0,2,K1,K2,K3)*e(2,0,J1,J2,J3)+d(1,2,K1,K2,K3)*e(2,1,J1,J2,J3)+d(2,2,K1,K2,K3)*e(2,2,J1,J2,J3);

  }
  else
  {
    Overture::abort("error");
  }
  return r;
}


RealArray TridiagonalSolver::
matrixVectorMultiply(const RealArray & d, const Index & K1, const Index & K2, const Index & K3, 
                     RealArray & e, const Index & J1, const Index & J2, const Index & J3)
{
  RealArray r(blockSize,1,K1,K2,K3);
  e.reshape(e.dimension(0),1,e.dimension(1),e.dimension(2),e.dimension(3));
  if( blockSize==2 )
  {
    r(0,0,K1,K2,K3) = d(0,0,K1,K2,K3)*e(0,0,J1,J2,J3)+d(1,0,K1,K2,K3)*e(1,0,J1,J2,J3);
    r(1,0,K1,K2,K3) = d(0,1,K1,K2,K3)*e(0,0,J1,J2,J3)+d(1,1,K1,K2,K3)*e(1,0,J1,J2,J3);
  } 
  else if( blockSize==3 )
  {
    r(0,0,K1,K2,K3) = d(0,0,K1,K2,K3)*e(0,0,J1,J2,J3)+d(1,0,K1,K2,K3)*e(1,0,J1,J2,J3)+d(2,0,K1,K2,K3)*e(2,0,J1,J2,J3);
    r(1,0,K1,K2,K3) = d(0,1,K1,K2,K3)*e(0,0,J1,J2,J3)+d(1,1,K1,K2,K3)*e(1,0,J1,J2,J3)+d(2,1,K1,K2,K3)*e(2,0,J1,J2,J3);
    r(2,0,K1,K2,K3) = d(0,2,K1,K2,K3)*e(0,0,J1,J2,J3)+d(1,2,K1,K2,K3)*e(1,0,J1,J2,J3)+d(2,2,K1,K2,K3)*e(2,0,J1,J2,J3);
  }
  else
  {
    Overture::abort("error");
  }
  r.reshape(blockSize,K1,K2,K3);
  e.reshape(e.dimension(0),e.dimension(2),e.dimension(3),e.dimension(4));
  return r;
}

int TridiagonalSolver::
blockFactor()
// ===============================================================================
// block tridiagonal system
// ===============================================================================
{
  Range N(0,blockSize-1);
  int base =Iv[axis].getBase();
  int bound=Iv[axis].getBound();
  if( scalarSystem )
  {
    int i1Base=I1.getBase();
    int i2Base=I2.getBase();
    int i3Base=I3.getBase();
    return scalarBlockFactor( i1Base,i2Base,i3Base );
  }
  else if( axis==axis1 )
  {
    if(  true && systemType!=extended  )
    {
      int i1Base=I1.getBase();
      int i2Base=I2.getBase(), i2Bound=I2.getBound();
      int i3Base=I3.getBase(), i3Bound=I3.getBound();
      for( int i3=i3Base; i3<=i3Bound; i3++ )
      {
	for( int i2=i2Base; i2<=i2Bound; i2++ )
	{
	  scalarBlockFactor( i1Base,i2,i3 );
	}
      }
    }
    else
    {
      int i1;
      if( systemType==normal )
      {
	invert( b,base,I2,I3 ); // invert b0
	for( i1=base+1; i1<=bound; i1++ )
	{
	  a(N,N,i1,I2,I3) =multiply(a,i1,I2,I3, b,i1-1,I2,I3); // save in a: a*b^{-1}
	  b(N,N,i1,I2,I3)-=multiply(a,i1,I2,I3, c,i1-1,I2,I3);
	  invert(b,i1,I2,I3);
	}
      }
      else if( systemType==extended )
      {
	// eliminate c[n]
	RealArray aa(N,N,1,I2,I3);
	aa=a(N,N,bound-1,I2,I3);
	invert(aa,0,I2,I3);
	c(N,N,bound,I2,I3)=multiply(c,bound,I2,I3, aa,0,I2,I3);    // save in c : c*a^{-1}
      
	a(N,N,bound,I2,I3)-=multiply(c,bound,I2,I3, b,bound-1,I2,I3);
	b(N,N,bound,I2,I3)-=multiply(c,bound,I2,I3, c,bound-1,I2,I3);
  
	invert( b,base,I2,I3 ); // invert b0
	// first case is special
	i1=base+1;
	a(N,N,i1,I2,I3) =multiply(a,i1,I2,I3, b,i1-1,I2,I3); // save in a: a*b^{-1}
	b(N,N,i1,I2,I3)-=multiply(a,i1,I2,I3, c,i1-1,I2,I3);
	invert(b,i1,I2,I3);
	c(N,N,base+1,I2,I3)-=multiply(a,base+1,I2,I3, a,base,I2,I3); // adjust c[1]
	for( i1=base+2; i1<=bound; i1++ )
	{
	  a(N,N,i1,I2,I3) =multiply(a,i1,I2,I3, b,i1-1,I2,I3); // save in a: a*b^{-1}
	  b(N,N,i1,I2,I3)-=multiply(a,i1,I2,I3, c,i1-1,I2,I3);
	  invert(b,i1,I2,I3);
	}
      }
    }
  }
  else if( axis==axis2 )
  {
    if(  true && systemType!=extended  )
    {
      int i1Base=I1.getBase(), i1Bound=I1.getBound();
      int i2Base=I2.getBase();
      int i3Base=I3.getBase(), i3Bound=I3.getBound();
      for( int i3=i3Base; i3<=i3Bound; i3++ )
      {
	for( int i1=i1Base; i1<=i1Bound; i1++ )
	{
	  scalarBlockFactor( i1,i2Base,i3 );
	}
      }
    }
    else
    {
      int i2;
      if( systemType==normal )
      {
	invert( b,I1,base,I3 ); // invert b0
	for( i2=base+1; i2<=bound; i2++ )
	{
	  a(N,N,I1,i2,I3) =multiply(a,I1,i2,I3, b,I1,i2-1,I3); // save in a: a*b^{-1}
	  b(N,N,I1,i2,I3)-=multiply(a,I1,i2,I3, c,I1,i2-1,I3);
	  invert(b,I1,i2,I3);
	}
      }
      else if( systemType==extended )
      {
	// eliminate c[n]
	RealArray aa(N,N,I1,1,I3);
	aa=a(N,N,I1,bound-1,I3);
	invert(aa,I1,0,I3);
	c(N,N,I1,bound,I3)=multiply(c,I1,bound,I3, aa,I1,0,I3);    // save in c : c*a^{-1}
      
	a(N,N,I1,bound,I3)-=multiply(c,I1,bound,I3, b,I1,bound-1,I3);
	b(N,N,I1,bound,I3)-=multiply(c,I1,bound,I3, c,I1,bound-1,I3);
  
	invert( b,I1,base,I3 ); // invert b0
	// first case is special
	i2=base+1;
	a(N,N,I1,i2,I3) =multiply(a,I1,i2,I3, b,I1,i2-1,I3); // save in a: a*b^{-1}
	b(N,N,I1,i2,I3)-=multiply(a,I1,i2,I3, c,I1,i2-1,I3);
	invert(b,I1,i2,I3);
	c(N,N,I1,base+1,I3)-=multiply(a,I1,base+1,I3, a,I1,base,I3); // adjust c[1]
	for( i2=base+2; i2<=bound; i2++ )
	{
	  a(N,N,I1,i2,I3) =multiply(a,I1,i2,I3, b,I1,i2-1,I3); // save in a: a*b^{-1}
	  b(N,N,I1,i2,I3)-=multiply(a,I1,i2,I3, c,I1,i2-1,I3);
	  invert(b,I1,i2,I3);
	}
      }
    }
  }
  else if( axis==axis3 )
  {
    if(  true && systemType!=extended  )
    {
      int i1Base=I1.getBase(), i1Bound=I1.getBound();
      int i2Base=I2.getBase(), i2Bound=I2.getBound();
      int i3Base=I3.getBase();
      for( int i2=i2Base; i2<=i2Bound; i2++ )
      {
	for( int i1=i1Base; i1<=i1Bound; i1++ )
	{
	  scalarBlockFactor( i1,i2,i3Base );
	}
      }
    }
    else
    {
      int i3;
      if( systemType==normal )
      {
	invert( b,I1,I2,base ); // invert b0
	for( i3=base+1; i3<=bound; i3++ )
	{
	  a(N,N,I1,I2,i3) =multiply(a,I1,I2,i3, b,I1,I2,i3-1); // save in a: a*b^{-1}
	  b(N,N,I1,I2,i3)-=multiply(a,I1,I2,i3, c,I1,I2,i3-1);
	  invert(b,I1,I2,i3);
	}
      }
      else if( systemType==extended )
      {
	// eliminate c[n]
	RealArray aa(N,N,I1,I2,1);
	aa=a(N,N,I1,I2,bound-1);
	invert(aa,I1,I2,0);
	c(N,N,I1,I2,bound)=multiply(c,I1,I2,bound, aa,I1,I2,0);    // save in c : c*a^{-1}
      
	a(N,N,I1,I2,bound)-=multiply(c,I1,I2,bound, b,I1,I2,bound-1);
	b(N,N,I1,I2,bound)-=multiply(c,I1,I2,bound, c,I1,I2,bound-1);
  
	invert( b,I1,I2,base ); // invert b0
	// first case is special
	i3=base+1;
	a(N,N,I1,I2,i3) =multiply(a,I1,I2,i3, b,I1,I2,i3-1); // save in a: a*b^{-1}
	b(N,N,I1,I2,i3)-=multiply(a,I1,I2,i3, c,I1,I2,i3-1);
	invert(b,I1,I2,i3);
	c(N,N,I1,I2,base+1)-=multiply(a,I1,I2,base+1, a,I1,I2,base); // adjust c[1]
	for( i3=base+2; i3<=bound; i3++ )
	{
	  a(N,N,I1,I2,i3) =multiply(a,I1,I2,i3, b,I1,I2,i3-1); // save in a: a*b^{-1}
	  b(N,N,I1,I2,i3)-=multiply(a,I1,I2,i3, c,I1,I2,i3-1);
	  invert(b,I1,I2,i3);
	}
      }
    }
    
  }
  else
    Overture::abort("error");
  
  return 0;
}




int TridiagonalSolver::
blockSolve(RealArray & r)
// ============================================================================================
// ============================================================================================
{
  Range N(0,blockSize-1);
  int base =Iv[axis].getBase();
  int bound=Iv[axis].getBound();
  int i1;
  if( scalarSystem )
  {
    int i1Base=I1.getBase();
    int i2Base=I2.getBase();
    int i3Base=I3.getBase();
    return scalarBlockSolve(r,i1Base,i2Base,i3Base);
  }
  else if( axis==axis1 )
  {
    if(  true && systemType!=extended  )
    {
      int i1Base=I1.getBase();
      int i2Base=I2.getBase(), i2Bound=I2.getBound();
      int i3Base=I3.getBase(), i3Bound=I3.getBound();

      const int i2Stride=I2.getStride();
      const int i3Stride=I3.getStride();
      
      for( int i3=i3Base; i3<=i3Bound; i3+=i3Stride )
      {
	for( int i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	{
	  scalarBlockSolve( r,i1Base,i2,i3 );
	}
      }
    }
    else
    {
      printf("***TridiagonalSolver::need to finish optimization of blockSolve, systemType==extended ****\n");

      // forward elimination
      if( systemType==extended )
	r(N,bound,I2,I3)-=matrixVectorMultiply(c,bound,I2,I3  ,r,bound-1,I2,I3);

      for( i1=base+1; i1<=bound; i1++ )
	r(N,i1,I2,I3)-=matrixVectorMultiply(a,i1,I2,I3, r,i1-1,I2,I3);

    // back substitution
      r(N,bound,I2,I3)=matrixVectorMultiply(b,bound,I2,I3, r,bound,I2,I3);
      RealArray t(N,1,I2,I3);
      for( i1=bound-1; i1>=base; i1-- )
      { //  b^{-1}[ r_i - c_i*r_{i+1} ]
	t=r(N,i1,I2,I3)-matrixVectorMultiply(c,i1,I2,I3, r,i1+1,I2,I3);
	r(N,i1,I2,I3)=matrixVectorMultiply(b,i1,I2,I3, t,0,I2,I3);  
      }
    
      if( systemType==extended )
      {
	t=matrixVectorMultiply(a, base,I2,I3, r,base+2,I2,I3);
	r(N,base,I2,I3)-=matrixVectorMultiply(b,base,I2,I3, t,0,I2,I3);
      }
    }
    
  }
  else if( axis==axis2 )
  {
    if(  true && systemType!=extended  )
    {
      int i1Base=I1.getBase(), i1Bound=I1.getBound();
      int i2Base=I2.getBase();
      int i3Base=I3.getBase(), i3Bound=I3.getBound();
      const int i1Stride=I1.getStride();
      const int i3Stride=I3.getStride();
      for( int i3=i3Base; i3<=i3Bound; i3+=i3Stride )
      {
	for( int i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	{
	  scalarBlockSolve( r,i1,i2Base,i3 );
	}
      }
    }
    else
    {
      printf("***TridiagonalSolver::need to finish optimization of blockSolve, systemType==extended ****\n");

      // forward elimination
      int i2;
      if( systemType==extended )
	r(N,I1,bound,I3)-=matrixVectorMultiply(c,I1,bound,I3  ,r,I1,bound-1,I3);

      for( i2=base+1; i2<=bound; i2++ )
	r(N,I1,i2,I3)-=matrixVectorMultiply(a,I1,i2,I3, r,I1,i2-1,I3);

    // back substitution
      r(N,I1,bound,I3)=matrixVectorMultiply(b,I1,bound,I3, r,I1,bound,I3);
      RealArray t(N,I1,1,I3);
      for( i2=bound-1; i2>=base; i2-- )
      { //  b^{-1}[ r_i - c_i*r_{i+1} ]
	t=r(N,I1,i2,I3)-matrixVectorMultiply(c,I1,i2,I3, r,I1,i2+1,I3);
	r(N,I1,i2,I3)=matrixVectorMultiply(b,I1,i2,I3, t,I1,0,I3);  
      }
    
      if( systemType==extended )
      {
	t=matrixVectorMultiply(a,I1, base,I3, r,I1,base+2,I3);
	r(N,I1,base,I3)-=matrixVectorMultiply(b,I1,base,I3, t,I1,0,I3);
      }
    }
  }
  else if( axis==axis3 )
  {
    // forward elimination
    if(  true && systemType!=extended  )
    {
      int i1Base=I1.getBase(), i1Bound=I1.getBound();
      int i2Base=I2.getBase(), i2Bound=I2.getBound();
      int i3Base=I3.getBase();
      const int i1Stride=I1.getStride();
      const int i2Stride=I2.getStride();
      for( int i2=i2Base; i2<=i2Bound; i2+=i2Stride )
      {
	for( int i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	{
	  scalarBlockSolve( r,i1,i2,i3Base );
	}
      }
    }
    else
    {
      printf("***TridiagonalSolver::need to finish optimization of blockSolve, systemType==extended ****\n");

      int i3;
      if( systemType==extended )
	r(N,I1,I2,bound)-=matrixVectorMultiply(c,I1,I2,bound  ,r,I1,I2,bound-1);

      for( i3=base+1; i3<=bound; i3++ )
	r(N,I1,I2,i3)-=matrixVectorMultiply(a,I1,I2,i3, r,I1,I2,i3-1);

    // back substitution
      r(N,I1,I2,bound)=matrixVectorMultiply(b,I1,I2,bound, r,I1,I2,bound);
      RealArray t(N,I1,I2,1);
      for( i3=bound-1; i3>=base; i3-- )
      { //  b^{-1}[ r_i - c_i*r_{i+1} ]
	t=r(N,I1,I2,i3)-matrixVectorMultiply(c,I1,I2,i3, r,I1,I2,i3+1);
	r(N,I1,I2,i3)=matrixVectorMultiply(b,I1,I2,i3, t,I1,I2,0);  
      }
    
      if( systemType==extended )
      {
	t=matrixVectorMultiply(a,I1,I2, base, r,I1,I2,base+2);
	r(N,I1,I2,base)-=matrixVectorMultiply(b,I1,I2,base, t,I1,I2,0);
      }
    }
  }
  return 0;
}




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


// int TridiagonalSolver::
// tridiagonalSolve( RealArray & r )
// // -----------------------------------------------------------------------------
// // Solve the tridiagonal system Ax=r (A should be first factored by tridiagonalFactor)
// // Input: 
// //   n,a[n],b[n],c[n] : arrays created by calling tridiagonal Factor (once)
// //   r[n] : right hand side (this will be over-written)
// // Output: 
// //   r[n] : The solution (over-writes the input values)
// // -----------------------------------------------------------------------------
// {
// /* -----
//   if( axis==axis1 )
//   {
//     int base =I1.getBase();
//     int bound=I1.getBound();
//     // forward elimination
//     if( systemType==extended )
//       r(bound,I2,I3)-=c(bound,I2,I3)*r(bound-1,I2,I3);

//     for( int i1=base+1; i1<=bound; i1++ )
//       r(i1,I2,I3)-=a(i1,I2,I3)*r(i1-1,I2,I3);

//     // back substitution
//     r(bound,I2,I3)/=b(bound,I2,I3);
//     for( i1=bound-1; i1>=base; i1-- )
//       r(i1,I2,I3)=(r(i1,I2,I3)-c(i1,I2,I3)*r(i1+1,I2,I3))/b(i1,I2,I3);


//     if( systemType==extended )
//       r(base,I2,I3)-=a(base,I2,I3)*r(base+2,I2,I3)/b(base,I2,I3);
//     return 0;
//   }
// ------ */

//   int i;
//   if( blockSize==1 )
//   {
//     if( axis==axis1 )
//     {
// #undef LI
// #define LI
// #undef RI
// #define RI ,I2,I3
//       SOLVE(I1)
// 	}
//     else if( axis==axis2 )
//     {
// #undef LI
// #define LI I1,
// #undef RI
// #define RI ,I3
//       SOLVE(I2)
// 	}
//     else if( axis==axis3 )
//     {
// #undef LI
// #define LI I1,I2,
// #undef RI
// #define RI 
//       SOLVE(I3)
// 	}
//     else
//     {
//       cout << "tridiagonalSolve::ERROR: invalid value for axis = " << axis << endl;
//       throw "error";
//     }
//   }
//   else
//   {
//     blockSolve(r);
//   }
//   return 0;
// }
#undef SOLVE

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


// int TridiagonalSolver::
// periodicTridiagonalFactor()
// //====================================================================
// // Solve the "Periodic" type tridiagonal system  ax=r where
// //
// //            | b[0] c[0]            a[0] |
// //            | a[1] b[1] c[1]            |
// //        a = |      a[2] b[2] c[2]       |      n > 2
// //            |            .    .    .    |
// //            |c[n-1]        a[n-1] b[n-1]|
// //
// // Input:  a[n], b[b], c[n] : arrays denoting the 3 diagonals. 
// //         w1[2],w2[2] : two work arrays of size n
// //
// //====================================================================
// {
// /* ---
//   if( axis==axis1 )
//   {
//     int base =I1.getBase();
//     int bound=I1.getBound();

//     if( bound-base+1<3 )
//     {
//       cout << "periodicTridiagonalFactor:ERROR bound-base+1<3 \n";
//       throw "error";
//     }

//     w2(base,I2,I3)=a(base,I2,I3);
//     for( int i1=base+1; i1<=bound-1; i1++ )
//     {
//       a(i1,I2,I3)/=b(i1-1,I2,I3);
//       b(i1,I2,I3)-=a(i1,I2,I3)*c(i1-1);
//       w2(i1,I2,I3)=-a(i1,I2,I3)*w2(i1-1,I2,I3);
//       w1(i1,I2,I3)=c(bound,I2,I3)/b(i1-1,I2,I3);
//       c(bound,I2,I3)=-w1(i1,I2,I3)*c(i1-1,I2,I3);
//       b(bound,I2,I3)-=w1(i1,I2,I3)*w2(i1-1,I2,I3);
//     }
  
//     w2(bound-1,I2,I3)+=c(bound-1,I2,I3);
//     a(bound,I2,I3)+=c(bound,I2,I3);
//     a(bound,I2,I3)/=b(bound-1,I2,I3);
//     b(bound,I2,I3)-=a(bound,I2,I3)*w2(bound-1,I2,I3);
//   }
// --- */
//   if( blockSize==1 )
//   {
//     if( axis==axis1 )
//     {
// /* ---      
//    a.display("periodicTridiagonalFactor (before):a");
//    b.display("periodicTridiagonalFactor (before):b");
//    c.display("periodicTridiagonalFactor (before):c");
// ----- */

// #undef LI
// #define LI
// #undef RI
// #define RI ,I2,I3
//       FACTOR(I1)

// /* ----
//    a.display("periodicTridiagonalFactor (before):a");
//    b.display("periodicTridiagonalFactor (before):b");
//    c.display("periodicTridiagonalFactor (before):c");
//    w1.display("periodicTridiagonalFactor:w1");
//    w2.display("periodicTridiagonalFactor:w2");
//    ----- */
// 	}
//     else if( axis==axis2 )
//     {
// #undef LI
// #define LI I1,
// #undef RI
// #define RI ,I3
//       FACTOR(I2)
// 	}
//     else if( axis==axis3 )
//     {
// #undef LI
// #define LI I1,I2,
// #undef RI
// #define RI 
//       FACTOR(I3)
// 	}
//     else
//     {
//       cout << "tridiagonalFactor::ERROR: invalid value for axis = " << axis << endl;
//       throw "error";
//     }
//   }
//   else
//   {
//     // block tridiagonal system
//     blockPeriodicFactor();
//   }
//   return 0;
// }
#undef FACTOR

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


// int TridiagonalSolver::
// periodicTridiagonalSolve( RealArray & r )
// //====================================================================
// // Solve the perioidc tridiagonal system Ax=r (A should be first factored by 
// // periodicTridiagonalFactor)
// // Input: 
// //   n,a[n],b[n],c[n],w1[n],w2[n] : arrays created by calling the 
// //        periodic tridiagonal Factor (once)
// //   r[n] : right hand side (this will be over-written)
// // Output: 
// //   r[n] : The solution (over-writes the input values)
// //====================================================================
// {

// /* ----
//   if( axis==axis1 )
//   {
//     int base =I1.getBase();
//     int bound=I1.getBound();

//     if( bound-base+1<3 )
//     {
//       cout << "periodicTridiagonalFactor:ERROR bound-base+1<3 \n";
//       throw "error";
//     }

//     for( int i1=base+1; i1<bound; i1++ )
//     {
//       r(i1,I2,I3)-=a(i1,I2,I3)*r(i1-1,I2,I3);
//       r(bound,I2,I3)-=w1(i1,I2,I3)*r(i1-1,I2,I3);
//     }
//     r(bound,I2,I3)=(r(bound,I2,I3)-a(bound,I2,I3)*r(bound-1,I2,I3))/b(bound,I2,I3);

//     i1=bound-1;
//     r(i1,I2,I3)=(r(i1,I2,I3)-w2(i1,I2,I3)*r(bound,I2,I3))/b(i1,I2,I3);
//     for( i1=bound-2; i1>=base; i1-- )
//       r(i1,I2,I3)=(r(i1,I2,I3)-c(i1,I2,I3)*r(i1+1,I2,I3)-w2(i1,I2,I3)*r(bound,I2,I3))/b(i1,I2,I3);
//   }
// ----- */

//   int i;
//   if( blockSize==1 )
//   {
//     if( axis==axis1 )
//     {
// /* ----
//    a.display("periodicTridiagonalSolve: a");
//    b.display("periodicTridiagonalSolve:b");
//    c.display("periodicTridiagonalSolve:c");
//    w1.display("periodicTridiagonalSolve:w1");
//    w2.display("periodicTridiagonalSolve:w2");
//    r.display("periodicTridiagonalSolve:r before");
//    ----- */
// #undef LI
// #define LI
// #undef RI
// #define RI ,I2,I3
//       SOLVE(I1)

// 	// r.display("periodicTridiagonalSolve:r after");
// 	}
//     else if( axis==axis2 )
//     {
// #undef LI
// #define LI I1,
// #undef RI
// #define RI ,I3
//       SOLVE(I2)
// 	}
//     else if( axis==axis3 )
//     {
// #undef LI
// #define LI I1,I2,
// #undef RI
// #define RI 
//       SOLVE(I3)
// 	}
//     else
//     {
//       cout << "tridiagonalSolve::ERROR: invalid value for axis = " << axis << endl;
//       throw "error";
//     }
//   }
//   else
//   {
//     // block tridiagonal system
//     blockPeriodicSolve(r);
//   }
//   return 0;
// }
#undef SOLVE


int TridiagonalSolver::
blockPeriodicFactor()
// ======================================================================================================
// 
// ======================================================================================================
{
  // block tridiagonal system
  Range N(0,blockSize-1);
  int base =Iv[axis].getBase();
  int bound=Iv[axis].getBound();

  if( scalarSystem ) // only one block tridiangonal system to solve
  {
    int i1Base=I1.getBase();
    int i2Base=I2.getBase();
    int i3Base=I3.getBase();
    return scalarBlockPeriodicFactor( i1Base,i2Base,i3Base );
  }
  else if( axis==axis1 )
  {
    if( true )
    {
      int i1Base=I1.getBase();
      int i2Base=I2.getBase(), i2Bound=I2.getBound();
      int i3Base=I3.getBase(), i3Bound=I3.getBound();
      for( int i3=i3Base; i3<=i3Bound; i3++ )
      {
	for( int i2=i2Base; i2<=i2Bound; i2++ )
	{
	  scalarBlockPeriodicFactor( i1Base,i2,i3 );
	}
      }
    }
    else
    {
      w2(N,N,base,I2,I3)=a(N,N,base,I2,I3);
      invert( b,base,I2,I3 ); // invert b0
      for( int i1=base+1; i1<=bound-1; i1++ )
      {
	a(N,N,i1,I2,I3) =multiply(a,i1,I2,I3, b,i1-1,I2,I3); // save in a: a*b^{-1}
	b(N,N,i1,I2,I3)-=multiply(a,i1,I2,I3, c,i1-1,I2,I3);
	w2(N,N,i1,I2,I3)=-multiply(a,i1,I2,I3, w2,i1-1,I2,I3);
	w1(N,N,i1,I2,I3)= multiply(c,bound,I2,I3, b,i1-1,I2,I3); // save c*b^{-1}
	c(N,N,bound,I2,I3)=-multiply(w1,i1,I2,I3, c,i1-1,I2,I3);
	b(N,N,bound,I2,I3)-=multiply(w1,i1,I2,I3, w2,i1-1,I2,I3);

	invert(b,i1,I2,I3);
      }
      w2(N,N,bound-1,I2,I3)+=c(N,N,bound-1,I2,I3);
      a(N,N,bound,I2,I3)+=c(N,N,bound,I2,I3);
      a(N,N,bound,I2,I3)=multiply(a,bound,I2,I3, b,bound-1,I2,I3);
      b(N,N,bound,I2,I3)-=multiply(a,bound,I2,I3, w2,bound-1,I2,I3);
      invert(b,bound,I2,I3);
    }
  }
  else if( axis==axis2 )
  {
    if( true )
    {
      int i1Base=I1.getBase(), i1Bound=I1.getBound();
      int i2Base=I2.getBase();
      int i3Base=I3.getBase(), i3Bound=I3.getBound();
      for( int i3=i3Base; i3<=i3Bound; i3++ )
      {
	for( int i1=i1Base; i1<=i1Bound; i1++ )
	{
	  scalarBlockPeriodicFactor( i1,i2Base,i3 );
	}
      }
    }
    else
    {
      int i2;
      w2(N,N,I1,base,I3)=a(N,N,I1,base,I3);
      invert( b,I1,base,I3 ); // invert b0
      for( i2=base+1; i2<=bound-1; i2++ )
      {
	a(N,N,I1,i2,I3) =multiply(a,I1,i2,I3, b,I1,i2-1,I3); // save in a: a*b^{-1}
	b(N,N,I1,i2,I3)-=multiply(a,I1,i2,I3, c,I1,i2-1,I3);
	w2(N,N,I1,i2,I3)=-multiply(a,I1,i2,I3, w2,I1,i2-1,I3);
	w1(N,N,I1,i2,I3)= multiply(c,I1,bound,I3, b,I1,i2-1,I3); // save c*b^{-1}
	c(N,N,I1,bound,I3)=-multiply(w1,I1,i2,I3, c,I1,i2-1,I3);
	b(N,N,I1,bound,I3)-=multiply(w1,I1,i2,I3, w2,I1,i2-1,I3);

	invert(b,I1,i2,I3);
      }
      w2(N,N,I1,bound-1,I3)+=c(N,N,I1,bound-1,I3);
      a(N,N,I1,bound,I3)+=c(N,N,I1,bound,I3);
      a(N,N,I1,bound,I3)=multiply(a,I1,bound,I3, b,I1,bound-1,I3);
      b(N,N,I1,bound,I3)-=multiply(a,I1,bound,I3, w2,I1,bound-1,I3);
      invert(b,I1,bound,I3);
    }
  }
  else if( axis==axis3 )
  {
    if( true  )
    {
      int i1Base=I1.getBase(), i1Bound=I1.getBound();
      int i2Base=I2.getBase(), i2Bound=I2.getBound();
      int i3Base=I3.getBase();
      for( int i2=i2Base; i2<=i2Bound; i2++ )
      {
	for( int i1=i1Base; i1<=i1Bound; i1++ )
	{
	  scalarBlockPeriodicFactor( i1,i2,i3Base );
	}
      }
    }
    else
    {
      int i3;
      w2(N,N,I1,I2,base)=a(N,N,I1,I2,base);
      invert( b,I1,I2,base ); // invert b0
      for( i3=base+1; i3<=bound-1; i3++ )
      {
	a(N,N,I1,I2,i3) =multiply(a,I1,I2,i3, b,I1,I2,i3-1); // save in a: a*b^{-1}
	b(N,N,I1,I2,i3)-=multiply(a,I1,I2,i3, c,I1,I2,i3-1);
	w2(N,N,I1,I2,i3)=-multiply(a,I1,I2,i3, w2,I1,I2,i3-1);
	w1(N,N,I1,I2,i3)= multiply(c,I1,I2,bound, b,I1,I2,i3-1); // save c*b^{-1}
	c(N,N,I1,I2,bound)=-multiply(w1,I1,I2,i3, c,I1,I2,i3-1);
	b(N,N,I1,I2,bound)-=multiply(w1,I1,I2,i3, w2,I1,I2,i3-1);

	invert(b,I1,I2,i3);
      }
      w2(N,N,I1,I2,bound-1)+=c(N,N,I1,I2,bound-1);
      a(N,N,I1,I2,bound)+=c(N,N,I1,I2,bound);
      a(N,N,I1,I2,bound)=multiply(a,I1,I2,bound, b,I1,I2,bound-1);
      b(N,N,I1,I2,bound)-=multiply(a,I1,I2,bound, w2,I1,I2,bound-1);
      invert(b,I1,I2,bound);
    }
  }
  
  return 0;
}

int TridiagonalSolver::
scalarBlockPeriodicFactor(int i1, int i2, int i3)
// ======================================================================================================
// 
// ======================================================================================================
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

  const int wDim0=w1.getRawDataSize(0);
  const int wDim1=w1.getRawDataSize(1);
  const int wDim2=w1.getRawDataSize(2);
  const int wDim3=w1.getRawDataSize(3);
  const int wDim4=w1.getRawDataSize(4);
  const int wStride = axis==0 ? wDim0*wDim1 : axis==1 ? wDim0*wDim1*wDim2 : wDim0*wDim1*wDim2*wDim3;
  const int wOffset = wDim0*wDim1*(i1+wDim2*(i2+wDim3*(i3)));

  real *ap = a.Array_Descriptor.Array_View_Pointer1;  
  real *bp = b.Array_Descriptor.Array_View_Pointer1;
  real *cp = c.Array_Descriptor.Array_View_Pointer1;
  real *w1p= w1.Array_Descriptor.Array_View_Pointer1;
  real *w2p= w2.Array_Descriptor.Array_View_Pointer1;

  ap+=offset;
  bp+=offset;
  cp+=offset;
  w1p+=wOffset;
  w2p+=wOffset;
  
  if( useOptimizedC && blockSize==2 )
  {
    // printf("optimised scalar blockPeriodicFactor, base=%i\n",base);
      
#define A(m,i) ap[m+stride*(i)]
#define B(m,i) bp[m+stride*(i)]
#define C(m,i) cp[m+stride*(i)]
#define W1(m,i) w1p[m+wStride*(i)]
#define W2(m,i) w2p[m+wStride*(i)]

#define INVERT(B,i) \
      deti = 1./(B(0,i)*B(3,i)-B(2,i)*B(1,i)); \
      temp= B(0,i)*deti; \
      B(0,i)=B(3,i)*deti; \
      B(1,i)*=-deti; \
      B(2,i)*=-deti; \
      B(3,i)=temp; 

    real deti,temp,a0,a1,a2,a3, b0,b1,b2,b3, c0,c1,c2,c3, d0,d1,d2,d3, e0,e1,e2,e3;

    // w2(N,N,base)=a(N,N,base);
    int i=0,j;
    W2(0,i)=A(0,i); W2(1,i)=A(1,i); W2(2,i)=A(2,i); W2(3,i)=A(3,i); 
    //  invert( b,base ); // invert b0

    INVERT(B,i);

    const int ib=bound-base;

    for( i=1; i<=ib-1; i++ )
    {
      j=i-1;
      a0=A(0,i); a1=A(1,i); a2=A(2,i); a3=A(3,i);
      b0=B(0,j); b1=B(1,j); b2=B(2,j); b3=B(3,j);

	// a(N,N,i1) =multiply(a,i1, b,i1-1); // save in a: a*b^{-1}
      A(0,i) = a0*B(0,j)+a1*B(2,j);
      A(1,i) = a0*B(1,j)+a1*B(3,j);
      A(2,i) = a2*B(0,j)+a3*B(2,j);
      A(3,i) = a2*B(1,j)+a3*B(3,j);

      a0=A(0,i); a1=A(1,i); a2=A(2,i); a3=A(3,i);
      c0=C(0,j); c1=C(1,j); c2=C(2,j); c3=C(3,j);
      // b(N,N,i1)-=multiply(a,i1, c,i1-1);
      B(0,i) -= a0*c0+a1*c2;
      B(1,i) -= a0*c1+a1*c3;
      B(2,i) -= a2*c0+a3*c2;
      B(3,i) -= a2*c1+a3*c3;

      d0=W2(0,j); d1=W2(1,j); d2=W2(2,j); d3=W2(3,j);
      // w2(N,N,i1)=-multiply(a,i1, w2,i1-1);
      W2(0,i) =- (a0*d0+a1*d2);
      W2(1,i) =- (a0*d1+a1*d3);
      W2(2,i) =- (a2*d0+a3*d2);
      W2(3,i) =- (a2*d1+a3*d3);

      // w1(N,N,i1)= multiply(c,bound, b,i1-1); // save c*b^{-1}
      e0=C(0,ib); e1=C(1,ib); e2=C(2,ib); e3=C(3,ib);
      W1(0,i) = (e0*b0+e1*b2);
      W1(1,i) = (e0*b1+e1*b3);
      W1(2,i) = (e2*b0+e3*b2);
      W1(3,i) = (e2*b1+e3*b3);

      // c(N,N,bound)=-multiply(w1,i1, c,i1-1);
      e0=W1(0,i); e1=W1(1,i); e2=W1(2,i); e3=W1(3,i);
      C(0,ib) =- (e0*c0+e1*c2);
      C(1,ib) =- (e0*c1+e1*c3);
      C(2,ib) =- (e2*c0+e3*c2);
      C(3,ib) =- (e2*c1+e3*c3);

      // b(N,N,bound)-=multiply(w1,i1, w2,i1-1);
      B(0,ib) -= e0*d0+e1*d2;
      B(1,ib) -= e0*d1+e1*d3;
      B(2,ib) -= e2*d0+e3*d2;
      B(3,ib) -= e2*d1+e3*d3;

	// invert(b,i1);
      INVERT(B,i);
    }
    // w2(N,N,bound-1)+=c(N,N,bound-1);
    W2(0,ib-1)+=C(0,ib-1), W2(1,ib-1)+=C(1,ib-1), W2(2,ib-1)+=C(2,ib-1), W2(3,ib-1)+=C(3,ib-1);
    // a(N,N,bound)+=c(N,N,bound);
    A(0,ib)+=C(0,ib), A(1,ib)+=C(1,ib), A(2,ib)+=C(2,ib), A(3,ib)+=C(3,ib);

    // a(N,N,bound)=multiply(a,bound, b,bound-1);
    i=ib, j=ib-1;
    a0=A(0,i); a1=A(1,i); a2=A(2,i); a3=A(3,i);
    b0=B(0,j); b1=B(1,j); b2=B(2,j); b3=B(3,j);
    A(0,i) = a0*b0+a1*b2;
    A(1,i) = a0*b1+a1*b3;
    A(2,i) = a2*b0+a3*b2;
    A(3,i) = a2*b1+a3*b3;

    // b(N,N,bound)-=multiply(a,bound, w2,bound-1);
    a0=A(0,i); a1=A(1,i); a2=A(2,i); a3=A(3,i);
    d0=W2(0,j); d1=W2(1,j); d2=W2(2,j); d3=W2(3,j);
    B(0,ib) -= a0*d0+a1*d2;
    B(1,ib) -= a0*d1+a1*d3;
    B(2,ib) -= a2*d0+a3*d2;
    B(3,ib) -= a2*d1+a3*d3;

      // invert(b,bound);
    INVERT(B,ib);

#undef INVERT

  }
  else if( useOptimizedC && blockSize==3 )
  {
    // printf("optimised scalar 3x3 blockPeriodicFactor, base=%i\n",base);
    real deti;
    real a00,a10,a20,a01,a11,a21,a02,a12,a22;
    real b00,b10,b20,b01,b11,b21,b02,b12,b22;
    real c00,c10,c20,c01,c11,c21,c02,c12,c22;
    real d00,d10,d20,d01,d11,d21,d02,d12,d22;
    real e00,e10,e20,e01,e11,e21,e02,e12,e22;
      
    // w2(N,N,base)=a(N,N,base);
    int i=0,j;
      
    int k;
    for( k=0; k<9; k++ )
      W2(k,i)=A(k,i); 

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
//    for( int i1=base+1; i1<=bound-1; i1++,i++ )
    for( i=1; i<=ib-1; i++ )
    {
      j=i-1;
      a00=A(0,i), a10=A(1,i), a20=A(2,i); 
      a01=A(3,i), a11=A(4,i), a21=A(5,i); 
      a02=A(6,i), a12=A(7,i), a22=A(8,i); 

      b00=B(0,j), b10=B(1,j), b20=B(2,j); 
      b01=B(3,j), b11=B(4,j), b21=B(5,j); 
      b02=B(6,j), b12=B(7,j), b22=B(8,j); 

	// a(N,N,i1) =multiply(a,i1, b,i1-1); // save in a: a*b^{-1}
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

	// b(N,N,i1)-=multiply(a,i1, c,i1-1);
      B(0,i) -= ( a00*c00+a10*c01+a20*c02);
      B(1,i) -= ( a00*c10+a10*c11+a20*c12);
      B(2,i) -= ( a00*c20+a10*c21+a20*c22);
      B(3,i) -= ( a01*c00+a11*c01+a21*c02);
      B(4,i) -= ( a01*c10+a11*c11+a21*c12);
      B(5,i) -= ( a01*c20+a11*c21+a21*c22);
      B(6,i) -= ( a02*c00+a12*c01+a22*c02);
      B(7,i) -= ( a02*c10+a12*c11+a22*c12);
      B(8,i) -= ( a02*c20+a12*c21+a22*c22);

      d00=W2(0,j), d10=W2(1,j), d20=W2(2,j); 
      d01=W2(3,j), d11=W2(4,j), d21=W2(5,j); 
      d02=W2(6,j), d12=W2(7,j), d22=W2(8,j); 

	// w2(N,N,i1)=-multiply(a,i1, w2,i1-1);
      W2(0,i) =-( a00*d00+a10*d01+a20*d02);
      W2(1,i) =-( a00*d10+a10*d11+a20*d12);
      W2(2,i) =-( a00*d20+a10*d21+a20*d22);
      W2(3,i) =-( a01*d00+a11*d01+a21*d02);
      W2(4,i) =-( a01*d10+a11*d11+a21*d12);
      W2(5,i) =-( a01*d20+a11*d21+a21*d22);
      W2(6,i) =-( a02*d00+a12*d01+a22*d02);
      W2(7,i) =-( a02*d10+a12*d11+a22*d12);
      W2(8,i) =-( a02*d20+a12*d21+a22*d22);

      e00=C(0,ib), e10=C(1,ib), e20=C(2,ib); 
      e01=C(3,ib), e11=C(4,ib), e21=C(5,ib); 
      e02=C(6,ib), e12=C(7,ib), e22=C(8,ib); 

//	w1(N,N,i1)= multiply(c,bound, b,i1-1); // save c*b^{-1}
      W1(0,i) =( e00*b00+e10*b01+e20*b02);
      W1(1,i) =( e00*b10+e10*b11+e20*b12);
      W1(2,i) =( e00*b20+e10*b21+e20*b22);
      W1(3,i) =( e01*b00+e11*b01+e21*b02);
      W1(4,i) =( e01*b10+e11*b11+e21*b12);
      W1(5,i) =( e01*b20+e11*b21+e21*b22);
      W1(6,i) =( e02*b00+e12*b01+e22*b02);
      W1(7,i) =( e02*b10+e12*b11+e22*b12);
      W1(8,i) =( e02*b20+e12*b21+e22*b22);

      e00=W1(0,i), e10=W1(1,i), e20=W1(2,i); 
      e01=W1(3,i), e11=W1(4,i), e21=W1(5,i); 
      e02=W1(6,i), e12=W1(7,i), e22=W1(8,i); 

//	c(N,N,bound)=-multiply(w1,i1, c,i1-1);
      C(0,ib) =-( e00*c00+e10*c01+e20*c02);
      C(1,ib) =-( e00*c10+e10*c11+e20*c12);
      C(2,ib) =-( e00*c20+e10*c21+e20*c22);
      C(3,ib) =-( e01*c00+e11*c01+e21*c02);
      C(4,ib) =-( e01*c10+e11*c11+e21*c12);
      C(5,ib) =-( e01*c20+e11*c21+e21*c22);
      C(6,ib) =-( e02*c00+e12*c01+e22*c02);
      C(7,ib) =-( e02*c10+e12*c11+e22*c12);
      C(8,ib) =-( e02*c20+e12*c21+e22*c22);

//	b(N,N,bound)-=multiply(w1,i1, w2,i1-1);
      B(0,ib) -=( e00*d00+e10*d01+e20*d02);
      B(1,ib) -=( e00*d10+e10*d11+e20*d12);
      B(2,ib) -=( e00*d20+e10*d21+e20*d22);
      B(3,ib) -=( e01*d00+e11*d01+e21*d02);
      B(4,ib) -=( e01*d10+e11*d11+e21*d12);
      B(5,ib) -=( e01*d20+e11*d21+e21*d22);
      B(6,ib) -=( e02*d00+e12*d01+e22*d02);
      B(7,ib) -=( e02*d10+e12*d11+e22*d12);
      B(8,ib) -=( e02*d20+e12*d21+e22*d22);

      // invert(b,i1);
      INVERT(B,i);
    }
    i=ib, j=ib-1;
    // w2(N,N,bound-1)+=c(N,N,bound-1);
    // a(N,N,bound)+=c(N,N,bound);
    for( k=0; k<9; k++ )
    {
      W2(k,j)+=C(k,j);
      A(k,i)+=C(k,i);
    }

    a00=A(0,i), a10=A(1,i), a20=A(2,i); 
    a01=A(3,i), a11=A(4,i), a21=A(5,i); 
    a02=A(6,i), a12=A(7,i), a22=A(8,i); 
    b00=B(0,j), b10=B(1,j), b20=B(2,j); 
    b01=B(3,j), b11=B(4,j), b21=B(5,j); 
    b02=B(6,j), b12=B(7,j), b22=B(8,j); 

    // a(N,N,bound)=multiply(a,bound, b,bound-1);

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
    d00=W2(0,j), d10=W2(1,j), d20=W2(2,j); 
    d01=W2(3,j), d11=W2(4,j), d21=W2(5,j); 
    d02=W2(6,j), d12=W2(7,j), d22=W2(8,j); 

    // b(N,N,bound)-=multiply(a,bound, w2,bound-1);

    B(0,i) -=( a00*d00+a10*d01+a20*d02);
    B(1,i) -=( a00*d10+a10*d11+a20*d12);
    B(2,i) -=( a00*d20+a10*d21+a20*d22);
    B(3,i) -=( a01*d00+a11*d01+a21*d02);
    B(4,i) -=( a01*d10+a11*d11+a21*d12);
    B(5,i) -=( a01*d20+a11*d21+a21*d22);
    B(6,i) -=( a02*d00+a12*d01+a22*d02);
    B(7,i) -=( a02*d10+a12*d11+a22*d12);
    B(8,i) -=( a02*d20+a12*d21+a22*d22);

    // invert(b,bound);
    INVERT(B,ib);

#undef A
#undef B
#undef C
#undef W1
#undef W2
#undef INVERT

  }
  else
  { // general case
    w2(N,N,base)=a(N,N,base);
    invert( b,base ); // invert b0
    for( int i1=base+1; i1<=bound-1; i1++ )
    {
      a(N,N,i1) =multiply(a,i1, b,i1-1); // save in a: a*b^{-1}
      b(N,N,i1)-=multiply(a,i1, c,i1-1);
      w2(N,N,i1)=-multiply(a,i1, w2,i1-1);
      w1(N,N,i1)= multiply(c,bound, b,i1-1); // save c*b^{-1}
      c(N,N,bound)=-multiply(w1,i1, c,i1-1);
      b(N,N,bound)-=multiply(w1,i1, w2,i1-1);

      invert(b,i1);
    }
    w2(N,N,bound-1)+=c(N,N,bound-1);
    a(N,N,bound)+=c(N,N,bound);
    a(N,N,bound)=multiply(a,bound, b,bound-1);
    b(N,N,bound)-=multiply(a,bound, w2,bound-1);
    invert(b,bound);
  }
  return 0;
}


int TridiagonalSolver::
blockPeriodicSolve(RealArray & r)
// =====================================================================================================
// =====================================================================================================
{

  Range N(0,blockSize-1);
  int base =Iv[axis].getBase();
  int bound=Iv[axis].getBound();
  int i1;
  if( scalarSystem )
  {
    // forward elimination
    int i1Base=I1.getBase();
    int i2Base=I2.getBase();
    int i3Base=I3.getBase();
    return scalarBlockPeriodicSolve(r,i1Base,i2Base,i3Base);
  }
  else if( axis==axis1 )
  {
    if( true )
    {
      int i1Base=I1.getBase();
      int i2Base=I2.getBase(), i2Bound=I2.getBound();
      int i3Base=I3.getBase(), i3Bound=I3.getBound();
      const int i3Stride=I3.getStride();
      const int i2Stride=I2.getStride();
      for( int i3=i3Base; i3<=i3Bound; i3+=i3Stride )
      {
	for( int i2=i2Base; i2<=i2Bound; i2+=i2Stride )
	{
	  scalarBlockPeriodicSolve( r,i1Base,i2,i3 );
	}
      }
    }
    else
    {
      // forward elimination
      for( i1=base+1; i1<bound; i1++ )
      {
	r(N,i1,I2,I3)-=matrixVectorMultiply(a,i1,I2,I3, r,i1-1,I2,I3);
	r(N,bound,I2,I3)-=matrixVectorMultiply(w1,i1,I2,I3, r,i1-1,I2,I3);
      }
      RealArray t(N,1,I2,I3);
      t=r(N,bound,I2,I3)-matrixVectorMultiply(a,bound,I2,I3, r,bound-1,I2,I3);
      r(N,bound,I2,I3)=matrixVectorMultiply(b,bound,I2,I3, t,0,I2,I3);
  

      i1=bound-1;
      t=r(N,i1,I2,I3)-matrixVectorMultiply(w2,i1,I2,I3, r,bound,I2,I3);
      r(N,i1,I2,I3)=matrixVectorMultiply(b,i1,I2,I3, t,0,I2,I3);
      for( i1=bound-2; i1>=base; i1-- )
      { 
	t=r(N,i1,I2,I3)-
	  matrixVectorMultiply(c,i1,I2,I3, r,i1+1,I2,I3)-
	  matrixVectorMultiply(w2,i1,I2,I3, r,bound,I2,I3);
      
	r(N,i1,I2,I3)=matrixVectorMultiply(b,i1,I2,I3, t,0,I2,I3);  
      }
    }
  }
  else if( axis==axis2 )
  {
    if( true )
    {
      int i1Base=I1.getBase(), i1Bound=I1.getBound();
      int i2Base=I2.getBase();
      int i3Base=I3.getBase(), i3Bound=I3.getBound();
      const int i1Stride=I1.getStride();
      const int i3Stride=I3.getStride();
      for( int i3=i3Base; i3<=i3Bound; i3+=i3Stride )
      {
	for( int i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	{
	  scalarBlockPeriodicSolve( r,i1,i2Base,i3 );
	}
      }
    }
    else
    {
      int i2;
      // forward elimination
      for( i2=base+1; i2<bound; i2++ )
      {
	r(N,I1,i2,I3)-=matrixVectorMultiply(a,I1,i2,I3, r,I1,i2-1,I3);
	r(N,I1,bound,I3)-=matrixVectorMultiply(w1,I1,i2,I3, r,I1,i2-1,I3);
      }
      RealArray t(N,I1,1,I3);
      t=r(N,I1,bound,I3)-matrixVectorMultiply(a,I1,bound,I3, r,I1,bound-1,I3);
      r(N,I1,bound,I3)=matrixVectorMultiply(b,I1,bound,I3, t,I1,0,I3);
  

      i2=bound-1;
      t=r(N,I1,i2,I3)-matrixVectorMultiply(w2,I1,i2,I3, r,I1,bound,I3);
      r(N,I1,i2,I3)=matrixVectorMultiply(b,I1,i2,I3, t,I1,0,I3);
      for( i2=bound-2; i2>=base; i2-- )
      { 
	t=r(N,I1,i2,I3)-
	  matrixVectorMultiply(c,I1,i2,I3, r,I1,i2+1,I3)-
	  matrixVectorMultiply(w2,I1,i2,I3, r,I1,bound,I3);
      
	r(N,I1,i2,I3)=matrixVectorMultiply(b,I1,i2,I3, t,I1,0,I3);  
      }
    }
  }
  else if( axis==axis3 )
  {
    if(  true )
    {
      int i1Base=I1.getBase(), i1Bound=I1.getBound();
      int i2Base=I2.getBase(), i2Bound=I2.getBound();
      int i3Base=I3.getBase();
      const int i1Stride=I1.getStride();
      const int i2Stride=I2.getStride();
      for( int i2=i2Base; i2<=i2Bound; i2+=i2Stride )
      {
	for( int i1=i1Base; i1<=i1Bound; i1+=i1Stride )
	{
	  scalarBlockPeriodicSolve( r,i1,i2,i3Base );
	}
      }
    }
    else
    {
      int i3;
      // forward elimination
      for( i3=base+1; i3<bound; i3++ )
      {
	r(N,I1,I2,i3)-=matrixVectorMultiply(a,I1,I2,i3, r,I1,I2,i3-1);
	r(N,I1,I2,bound)-=matrixVectorMultiply(w1,I1,I2,i3, r,I1,I2,i3-1);
      }
      RealArray t(N,I1,I2,1);
      t=r(N,I1,I2,bound)-matrixVectorMultiply(a,I1,I2,bound, r,I1,I2,bound-1);
      r(N,I1,I2,bound)=matrixVectorMultiply(b,I1,I2,bound, t,I1,I2,0);
  

      i3=bound-1;
      t=r(N,I1,I2,i3)-matrixVectorMultiply(w2,I1,I2,i3, r,I1,I2,bound);
      r(N,I1,I2,i3)=matrixVectorMultiply(b,I1,I2,i3, t,I1,I2,0);
      for( i3=bound-2; i3>=base; i3-- )
      { 
	t=r(N,I1,I2,i3)-
	  matrixVectorMultiply(c,I1,I2,i3, r,I1,I2,i3+1)-
	  matrixVectorMultiply(w2,I1,I2,i3, r,I1,I2,bound);
      
	r(N,I1,I2,i3)=matrixVectorMultiply(b,I1,I2,i3, t,I1,I2,0);  
      }
    }
  }
  return 0;
}
#undef A
#undef B
#undef C
#undef R
#undef W1
#undef W2


int TridiagonalSolver::
scalarBlockPeriodicSolve(RealArray & r, int i1, int i2, int i3)
// =====================================================================================================
// =====================================================================================================
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

  const int wDim0=w1.getRawDataSize(0);
  const int wDim1=w1.getRawDataSize(1);
  const int wDim2=w1.getRawDataSize(2);
  const int wDim3=w1.getRawDataSize(3);
  const int wDim4=w1.getRawDataSize(4);
  const int wStride = axis==0 ? wDim0*wDim1 : axis==1 ? wDim0*wDim1*wDim2 : wDim0*wDim1*wDim2*wDim3;
  const int wOffset = wDim0*wDim1*(i1+wDim2*(i2+wDim3*(i3)));

  real *ap = a.Array_Descriptor.Array_View_Pointer1;
  real *bp = b.Array_Descriptor.Array_View_Pointer1;
  real *cp = c.Array_Descriptor.Array_View_Pointer1;
  real *w1p= w1.Array_Descriptor.Array_View_Pointer1;
  real *w2p= w2.Array_Descriptor.Array_View_Pointer1;
  ap+=offset;
  bp+=offset;
  cp+=offset;
  w1p+=wOffset;
  w2p+=wOffset;
  
  const int rDim0=r.getRawDataSize(0);
  const int rDim1=r.getRawDataSize(1);
  const int rDim2=r.getRawDataSize(2);
  const int rDim3=r.getRawDataSize(3);
  const int rStride = axis==0 ? rDim0 : axis==1 ? rDim0*rDim1 : rDim0*rDim1*rDim2;
  const int rOffset = rDim0*(i1+rDim1*(i2+rDim2*(i3)));

  real *rp = r.Array_Descriptor.Array_View_Pointer1;
  rp+=rOffset;

  // forward elimination
  if( useOptimizedC && blockSize==2 )
  {
    // printf("optimised scalar blockPeriodicFactor, base=%i\n",base);

#define A(m,i) ap[m+stride*(i)]
#define B(m,i) bp[m+stride*(i)]
#define C(m,i) cp[m+stride*(i)]
#define R(m,i) rp[m+rStride*(i)]
#define W1(m,i) w1p[m+wStride*(i)]
#define W2(m,i) w2p[m+wStride*(i)]


    real a0,a1,a2,a3, d0,d1,d2,d3,  r0,r1;
    int i=1,j;
    const int ib=bound-base;

    for( i=1; i<ib; i++ )
    {
      j=i-1;
      a0=A(0,i); a1=A(1,i); a2=A(2,i); a3=A(3,i);
      r0=R(0,j); r1=R(1,j);
      // r(N,i)-=matrixVectorMultiply(a,i, r,i-1);
      R(0,i) -= ( a0*r0+a1*r1 );
      R(1,i) -= ( a2*r0+a3*r1 );

      // r(N,bound)-=matrixVectorMultiply(w1,i, r,i-1);
      d0=W1(0,i); d1=W1(1,i); d2=W1(2,i); d3=W1(3,i);
      R(0,ib) -= ( d0*r0+d1*r1 );
      R(1,ib) -= ( d2*r0+d3*r1 );
       
    }
    // r(N,bound)=matrixVectorMultiply(b,bound, evaluate(r(N,bound)-matrixVectorMultiply(a,bound, r,bound-1)));
    i=ib, j=ib-1;
    a0=A(0,i); a1=A(1,i); a2=A(2,i); a3=A(3,i);
    real b0=B(0,i), b1=B(1,i), b2=B(2,i), b3=B(3,i);

    r0=R(0,j); r1=R(1,j);
    d0=R(0,i) - ( a0*r0+a1*r1 );
    d1=R(1,i) - ( a2*r0+a3*r1 );

    R(0,i) = ( b0*d0+b1*d1 );
    R(1,i) = ( b2*d0+b3*d1 );

      // i1=bound-1;
      // r(N,i1)=matrixVectorMultiply(b,i1, evaluate(r(N,i1)-matrixVectorMultiply(w2,i1, r,bound)));
    i=ib-1;
    a0=W2(0,i); a1=W2(1,i); a2=W2(2,i); a3=W2(3,i);
    b0=B(0,i); b1=B(1,i); b2=B(2,i); b3=B(3,i);
    r0=R(0,ib); r1=R(1,ib);
    d0=R(0,i) - ( a0*r0+a1*r1 );
    d1=R(1,i) - ( a2*r0+a3*r1 );

    R(0,i) = ( b0*d0+b1*d1 );
    R(1,i) = ( b2*d0+b3*d1 );

    i=ib-2;
    for( i=ib-2; i>=0; i-- )
    { 
      j=i+1;
// 	r(N,i1)=matrixVectorMultiply(b,i1,evaluate(r(N,i1)
// 		  			   -matrixVectorMultiply(c,i1, r,i1+1)-matrixVectorMultiply(w2,i1, r,bound)));  
      a0=W2(0,i); a1=W2(1,i); a2=W2(2,i); a3=W2(3,i);

      d0=( a0*r0+a1*r1 );
      d1=( a2*r0+a3*r1 );
          
      a0=C(0,i); a1=C(1,i); a2=C(2,i); a3=C(3,i);
      d2 = a0*R(0,j)+a1*R(1,j);
      d3 = a2*R(0,j)+a3*R(1,j);
	
      d0 = R(0,i)-d2-d0;
      d1 = R(1,i)-d3-d1;

      b0=B(0,i); b1=B(1,i); b2=B(2,i); b3=B(3,i);
	
      R(0,i) = ( b0*d0+b1*d1 );
      R(1,i) = ( b2*d0+b3*d1 );


    }
      
  }
  else if( useOptimizedC && blockSize==3 )
  {
    // printf("optimised scalar blockPeriodicFactor, base=%i\n",base);
    real a00,a10,a20,a01,a11,a21,a02,a12,a22;
    real b00,b10,b20,b01,b11,b21,b02,b12,b22;
    real c00,c10,c20,c01,c11,c21,c02,c12,c22;
    real d00,d10,d20,d01,d11,d21,d02,d12,d22;
    real e00,e10,e20,e01,e11,e21,e02,e12,e22;
    real r0,r1,r2;
      
    int i=1,j;
    const int ib=bound-base;

    for( i=1; i<ib; i++ )
    {
      j=i-1;
      a00=A(0,i), a10=A(1,i), a20=A(2,i); 
      a01=A(3,i), a11=A(4,i), a21=A(5,i); 
      a02=A(6,i), a12=A(7,i), a22=A(8,i); 

      r0=R(0,j); r1=R(1,j); r2=R(2,j);
	
      // r(N,i1)-=matrixVectorMultiply(a,i1, r,i1-1);
      R(0,i) -= a00*r0+a10*r1+a20*r2;
      R(1,i) -= a01*r0+a11*r1+a21*r2;
      R(2,i) -= a02*r0+a12*r1+a22*r2;

      // r(N,bound)-=matrixVectorMultiply(w1,i1, r,i1-1);
      d00=W1(0,i), d10=W1(1,i), d20=W1(2,i); 
      d01=W1(3,i), d11=W1(4,i), d21=W1(5,i); 
      d02=W1(6,i), d12=W1(7,i), d22=W1(8,i); 

      R(0,ib) -= d00*r0+d10*r1+d20*r2;
      R(1,ib) -= d01*r0+d11*r1+d21*r2;
      R(2,ib) -= d02*r0+d12*r1+d22*r2;

    }
    i=ib, j=ib-1;
    // r(N,bound)=matrixVectorMultiply(b,bound, evaluate(r(N,bound)-matrixVectorMultiply(a,bound, r,bound-1)));
    a00=A(0,i), a10=A(1,i), a20=A(2,i); 
    a01=A(3,i), a11=A(4,i), a21=A(5,i); 
    a02=A(6,i), a12=A(7,i), a22=A(8,i); 
    b00=B(0,i), b10=B(1,i), b20=B(2,i); 
    b01=B(3,i), b11=B(4,i), b21=B(5,i); 
    b02=B(6,i), b12=B(7,i), b22=B(8,i); 
    r0=R(0,j); r1=R(1,j); r2=R(2,j);
    d00 = R(0,i) - (a00*r0+a10*r1+a20*r2);
    d10 = R(1,i) - (a01*r0+a11*r1+a21*r2);
    d20 = R(2,i) - (a02*r0+a12*r1+a22*r2);

    R(0,i) = b00*d00+b10*d10+b20*d20;
    R(1,i) = b01*d00+b11*d10+b21*d20;
    R(2,i) = b02*d00+b12*d10+b22*d20;


    // i1=bound-1;
    // r(N,i1)=matrixVectorMultiply(b,i1, evaluate(r(N,i1)-matrixVectorMultiply(w2,i1, r,bound)));

    i=ib-1; j=ib;
    a00=W2(0,i), a10=W2(1,i), a20=W2(2,i); 
    a01=W2(3,i), a11=W2(4,i), a21=W2(5,i); 
    a02=W2(6,i), a12=W2(7,i), a22=W2(8,i); 
    b00=B(0,i), b10=B(1,i), b20=B(2,i); 
    b01=B(3,i), b11=B(4,i), b21=B(5,i); 
    b02=B(6,i), b12=B(7,i), b22=B(8,i); 
    r0=R(0,j); r1=R(1,j); r2=R(2,j);
    d00 = R(0,i) - (a00*r0+a10*r1+a20*r2);
    d10 = R(1,i) - (a01*r0+a11*r1+a21*r2);
    d20 = R(2,i) - (a02*r0+a12*r1+a22*r2);

    R(0,i) = b00*d00+b10*d10+b20*d20;
    R(1,i) = b01*d00+b11*d10+b21*d20;
    R(2,i) = b02*d00+b12*d10+b22*d20;

    for( i=ib-2; i>=0; i-- )
    { 
      // r(N,i1)=matrixVectorMultiply(b,i1,evaluate(r(N,i1)
      //  			   -matrixVectorMultiply(c,i1, r,i1+1)-matrixVectorMultiply(w2,i1, r,bound)));  
      j=i+1;
	
      a00=W2(0,i), a10=W2(1,i), a20=W2(2,i); 
      a01=W2(3,i), a11=W2(4,i), a21=W2(5,i); 
      a02=W2(6,i), a12=W2(7,i), a22=W2(8,i); 
      d00 = (a00*r0+a10*r1+a20*r2);
      d10 = (a01*r0+a11*r1+a21*r2);
      d20 = (a02*r0+a12*r1+a22*r2);
      a00=C(0,i), a10=C(1,i), a20=C(2,i); 
      a01=C(3,i), a11=C(4,i), a21=C(5,i); 
      a02=C(6,i), a12=C(7,i), a22=C(8,i); 
      d01 = (a00*R(0,j)+a10*R(1,j)+a20*R(2,j));
      d11 = (a01*R(0,j)+a11*R(1,j)+a21*R(2,j));
      d21 = (a02*R(0,j)+a12*R(1,j)+a22*R(2,j));

      d00 = R(0,i)-d01-d00;
      d10 = R(1,i)-d11-d10;
      d20 = R(2,i)-d21-d20;
      b00=B(0,i), b10=B(1,i), b20=B(2,i); 
      b01=B(3,i), b11=B(4,i), b21=B(5,i); 
      b02=B(6,i), b12=B(7,i), b22=B(8,i); 
      R(0,i) = b00*d00+b10*d10+b20*d20;
      R(1,i) = b01*d00+b11*d10+b21*d20;
      R(2,i) = b02*d00+b12*d10+b22*d20;

    }

  }
  else
  { // general case
    int i1;
    for( i1=base+1; i1<bound; i1++ )
    {
      r(N,i1)-=matrixVectorMultiply(a,i1, r,i1-1);
      r(N,bound)-=matrixVectorMultiply(w1,i1, r,i1-1);
    }
    r(N,bound)=matrixVectorMultiply(b,bound, evaluate(r(N,bound)-matrixVectorMultiply(a,bound, r,bound-1)));
  

    i1=bound-1;
    r(N,i1)=matrixVectorMultiply(b,i1, evaluate(r(N,i1)-matrixVectorMultiply(w2,i1, r,bound)));
    for( i1=bound-2; i1>=base; i1-- )
    { 
      r(N,i1)=matrixVectorMultiply(b,i1,evaluate(r(N,i1)
			     -matrixVectorMultiply(c,i1, r,i1+1)-matrixVectorMultiply(w2,i1, r,bound)));  
    }
  }
  return 0;
}


#undef A
#undef B
#undef C
#undef R
#undef W1
#undef W2
