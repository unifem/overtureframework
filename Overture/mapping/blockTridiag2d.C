#include "blockTridiag2d.h"

//\begin{>blockTridiag.tex}{\subsection{Constructor}}
blockTridiag2d::
  blockTridiag2d()
//=========================================================================
// /Description: blockTridiag2d is to solve a $2\times 2$ block tridiagonal system. 
//  Here are the public member functions:\\
//  {\em factor}: Computes the LU factorization of the matrix coefficients.\\
//  {\em solve}: Computes the solution using the gotten LU decomposition.\\
// \\
//  The function {\em factor} has to be submitted before {\em solve}
//\end{blockTridiag.tex}
//========================================================================= 
{
}

blockTridiag2d::
  ~blockTridiag2d() {
 }

int blockTridiag2d::
  checkDimension(realArray A, realArray B, realArray C){
  int n11, n12, n21, n22, n31, n32;

  n11=(A.getBound(0)-A.getBase(0)), n12=(A.getBound(1)-A.getBase(1));
  n21=(B.getBound(0)-B.getBase(0)), n22=(B.getBound(1)-B.getBase(1));
  n31=(C.getBound(0)-C.getBase(0)), n32=(C.getBound(1)-C.getBase(1));

  if ((n11 != n12)||(n11 != n21)||(n11 != n22)||(n11 != n31)||
      (n11 != n32)||
      (A.getFullRange(2) != B.getFullRange(2))||
      (A.getFullRange(2) != C.getFullRange(2))||
      (A.getFullRange(3) != B.getFullRange(3))||
      (A.getFullRange(3) != C.getFullRange(3))||
      (A.getFullRange(4) != B.getFullRange(4))||
      (A.getFullRange(4) != C.getFullRange(4))){
     printf("!!! \t Size Conflict \t !!!\n"
	    "A, B, C should be of size (2,2,I1,I2,I3)\n");
     return(1);
  }
  else return(0);
}

int blockTridiag2d::
  checkDimension(realArray D){
  int n11, n12;

  n11=(a.getBound(0)-a.getBase(0));
  n12=(D.getBound(0)-D.getBase(0));

  if ((n11 != n12)||
      (a.getFullRange(2) != D.getFullRange(1))||
      (a.getFullRange(3) != D.getFullRange(2))||
      (a.getFullRange(4) != D.getFullRange(3))){
     printf("!!! \t Size Conflict \t !!!\n"
	    "D should be of size (2, I1,I2,I3) when \n"
	    "A, B, C are of size (2,2,I1,I2,I3)\n");
     return(1);
  }
  else return(0);
}

//    Let's define a macro that does the factorization of the 
//    matrix M according to the direction of the sweep
//    The macro LI and RI need to have been defined before
//    FACTOR can be used.
//    The routine overwrites the matrices A,B,C. The factorization 
//    is such that on there are 1's on the upper triangular factor
//

#undef FACTOR
#define FACTOR(I) \
   int istart = I.getBase();                                        \
   int istop = I.getBound();                                        \
			                                            \
    /* Do the first block line  */                                  \
    b(0,1,LI istart RI) /= b(0,0,LI istart RI);                     \
    b(1,1,LI istart RI) -= b(1,0,LI istart RI)*b(0,1,LI istart RI); \
    c(0,0,LI istart RI) /= b(0,0,LI istart RI);                     \
    c(0,1,LI istart RI) /= b(0,0,LI istart RI);                     \
    c(1,0,LI istart RI) -= c(0,0,LI istart RI)*b(1,0,LI istart RI); \
    c(1,0,LI istart RI) /= b(1,1,LI istart RI);                     \
    c(1,1,LI istart RI) -= c(0,1,LI istart RI)*b(1,0,LI istart RI); \
    c(1,1,LI istart RI) /= b(1,1,LI istart RI);                     \
								    \
    /* Now the remaining block; */                                  \
    for (int i=istart+1; i<=istop; i++){                            \
      a(0,1,LI i RI) -= a(0,0,LI i RI)*b(0,1,LI i-1 RI);            \
      a(1,1,LI i RI) -= a(1,0,LI i RI)*b(0,1,LI i-1 RI);            \
      b(0,0,LI i RI) -= (a(0,0,LI i RI)*c(0,0,LI i-1 RI)+           \
			 a(0,1,LI i RI)*c(1,0,LI i-1 RI));          \
      b(1,0,LI i RI) -= (a(1,0,LI i RI)*c(0,0,LI i-1 RI)+           \
			 a(1,1,LI i RI)*c(1,0,LI i-1 RI));          \
      b(0,1,LI i RI) -= (a(0,0,LI i RI)*c(0,1,LI i-1 RI)+           \
			 a(0,1,LI i RI)*c(1,1,LI i-1 RI));          \
      b(0,1,LI i RI) /= b(0,0,LI i RI);                             \
      b(1,1,LI i RI) -= (a(1,0,LI i RI)*c(0,1,LI i-1 RI)+           \
			 a(1,1,LI i RI)*c(1,1,LI i-1 RI)+           \
			 b(1,0,LI i RI)*b(0,1,LI i RI));            \
      if (i != istop){                                              \
	c(0,0,LI i RI) /= b(0,0,LI i RI);                           \
	c(0,1,LI i RI) /= b(0,0,LI i RI);                           \
	c(1,0,LI i RI) -= c(0,0,LI i RI)*b(1,0,LI i RI);            \
	c(1,0,LI i RI) /= b(1,1,LI i RI);                           \
	c(1,1,LI i RI) -= c(0,1,LI i RI)*b(1,0,LI i RI);            \
	c(1,1,LI i RI) /= b(1,1,LI i RI);                           \
      }                                                             \
    }

//\begin{>>blockTridiag.tex}{\subsection{factor}}
int blockTridiag2d::
  factor(realArray &A, realArray &B, realArray &C, 
	 systemType type, int axis)
//===================================================
// /Purpose:
//    Factors the $2\times 2$ block tridiagonal matrix defined by (A,B,C).
// /A, B, C: input/output; on input the three block tridiagonal, on output the LU factorization
// /type: input; normal or periodic (the system type).
// /axis: input; $0$ or $1$ (the direction of the sweep).
//
// /Return value: int 0 when succesfull otherwise throws an error.
//\end{blockTridiag.tex}
//==================================================
 { 
  a.reference(A);
  b.reference(B);
  c.reference(C);

  iaxis=axis;
  I1=a.dimension(2);
  I2=a.dimension(3);
  I3=a.dimension(4);

  if (checkDimension(a,b,c)==1) exit(1);
  if (type==normal) factorRegular();
  else if (type==periodic) factorPeriodic();
  else {
    printf("The type of system %d is not defined\n",type);
    {throw "error";}
  }

  return 0;
}

int blockTridiag2d::
  factorRegular(void){
   // Factor the 2D Block tridiagonal
   // 
   //         |B[0] C[0]                                |
   //         |A[1] B[1] C[1]                           |
   //         |     A[2] B[2] C[2]                      |
   //   M =   |          .... .... ....                 |
   //         |              ....  ....    ....         |
   //         |                    A[n-1] B[n-1] C[n-1] |
   //         |                           A[n]   B[n]   |
   //
   //
   //
   //  where A[i], B[i], C[i] are two by two matrices.
   //  For this case A[0]=0 and C[n]=0
   //  No pivoting is used for this case

  if (iaxis==axis1){
#undef LI
#define LI
#undef RI
#define RI ,I2,I3
 FACTOR(I1)
  }
  else if (iaxis==axis2){
#undef LI
#define LI I1,
#undef RI
#define RI ,I3
  FACTOR(I2)
  }
  else if (iaxis==axis3){
#undef LI
#define LI I1,I2,
#undef RI
#define RI
  FACTOR(I3)
  }
  else {
   printf("Invalid value for iaxis in FACTOR.   Exiting\n");
   {throw "error";}
  }

  return(0);
}
#undef FACTOR

#undef INITIALIZEP
#define INITIALIZEP(I)                              \
   /* This is to initialize       */                \
   /* the arrays v and w for the  */                \
   /* periodic 2X2 block solver   */                \
				                    \
  int istart = I.getBase();                         \
  int istop = I.getBound()-1;                       \
				                    \
  v(0,0,LI istart RI)=a(0,0,LI istart RI);          \
  v(0,1,LI istart RI)=a(0,1,LI istart RI);          \
  v(1,0,LI istart RI)=a(1,0,LI istart RI);          \
  v(1,1,LI istart RI)=a(1,1,LI istart RI);          \
                                                    \
  w(0,0,LI istart RI)=c(0,0,LI istop RI);           \
  w(0,1,LI istart RI)=c(0,1,LI istop RI);           \
  w(1,0,LI istart RI)=c(1,0,LI istop RI);           \
  w(1,1,LI istart RI)=c(1,1,LI istop RI);           \
                                                    \
  v(0,0,LI istop-1 RI)=c(0,0,LI istop-1 RI);        \
  v(0,1,LI istop-1 RI)=c(0,1,LI istop-1 RI);        \
  v(1,0,LI istop-1 RI)=c(1,0,LI istop-1 RI);        \
  v(1,1,LI istop-1 RI)=c(1,1,LI istop-1 RI);        \
                                                    \
  w(0,0,LI istop-1 RI)=a(0,0,LI istop RI);          \
  w(0,1,LI istop-1 RI)=a(0,1,LI istop RI);          \
  w(1,0,LI istop-1 RI)=a(1,0,LI istop RI);          \
  w(1,1,LI istop-1 RI)=a(1,1,LI istop RI);          \
 

  // The macro for a 2X2 block tridiagonal in the
  // periodic case

#undef FACTORP
#define FACTORP(I)                                                    \
  int istart=I.getBase();                                             \
  int istop = I.getBound()-1;                                         \
								      \
  /*Do the first blocks    */                                         \
								      \
  b(0,1,LI istart RI) /= b(0,0,LI istart RI);                         \
  b(1,1,LI istart RI) -= b(0,1,LI istart RI)*b(1,0,LI istart RI);     \
  c(0,0,LI istart RI) /= b(0,0,LI istart RI);                         \
  c(0,1,LI istart RI) /= b(0,0,LI istart RI);                         \
  v(0,0,LI istart RI) /= b(0,0,LI istart RI);                         \
  v(0,1,LI istart RI) /= b(0,0,LI istart RI);                         \
  c(1,0,LI istart RI) = (c(1,0,LI istart RI)-b(1,0,LI istart RI)*     \
			 c(0,0,LI istart RI))/b(1,1,LI istart RI);    \
  c(1,1,LI istart RI) = (c(1,1,LI istart RI)-b(1,0,LI istart RI)*     \
			 c(0,1,LI istart RI))/b(1,1,LI istart RI);    \
  v(1,0,LI istart RI) = (v(1,0,LI istart RI)-b(1,0,LI istart RI)*     \
			 v(0,0,LI istart RI))/b(1,1,LI istart RI);    \
  v(1,1,LI istart RI) = (v(1,1,LI istart RI)-b(1,0,LI istart RI)*     \
			 v(0,1,LI istart RI))/b(1,1,LI istart RI);    \
  w(0,1,LI istart RI) -= b(0,1,LI istart RI)*w(0,0,LI istart RI);     \
  w(1,1,LI istart RI) -= b(0,1,LI istart RI)*w(1,0,LI istart RI);     \
								      \
  /* The interior points     */                                       \
								      \
  for (int i=istart+1; i<istop; i++){                                 \
    a(0,1,LI i RI) -= a(0,0,LI i RI)*b(0,1,LI i-1 RI);                \
    a(1,1,LI i RI) -= a(1,0,LI i RI)*b(0,1,LI i-1 RI);                \
    b(0,0,LI i RI) -= (a(0,0,LI i RI)*c(0,0,LI i-1 RI)+               \
		       a(0,1,LI i RI)*c(1,0,LI i-1 RI));              \
    b(1,0,LI i RI) -= (a(1,0,LI i RI)*c(0,0,LI i-1 RI)+               \
		       a(1,1,LI i RI)*c(1,0,LI i-1 RI));              \
    b(0,1,LI i RI) = (b(0,1,LI i RI)-a(0,0,LI i RI)*                  \
		      c(0,1,LI i-1 RI)-a(0,1,LI i RI)*                \
		      c(1,1,LI i-1 RI))/b(0,0,LI i RI);               \
    b(1,1,LI i RI) -= (a(1,0,LI i RI)*c(0,1,LI i-1 RI)+               \
		       a(1,1,LI i RI)*c(1,1,LI i-1 RI)+               \
		       b(1,0,LI i RI)*b(0,1,LI i RI));                \
    c(0,0,LI i RI) /= b(0,0,LI i RI);                                 \
    c(0,1,LI i RI) /= b(0,0,LI i RI);                                 \
    c(1,0,LI i RI) = (c(1,0,LI i RI)-b(1,0,LI i RI)*                  \
		      c(0,0,LI i RI))/b(1,1,LI i RI);                 \
    c(1,1,LI i RI) = (c(1,1,LI i RI)-b(1,0,LI i RI)*                  \
		      c(0,1,LI i RI))/b(1,1,LI i RI);                 \
    v(0,0,LI i RI) = (v(0,0,LI i RI)-a(0,0,LI i RI)*                  \
		      v(0,0,LI i-1 RI)-a(0,1,LI i RI)*                \
		      v(1,0,LI i-1 RI))/b(0,0,LI i RI);               \
    v(0,1,LI i RI) = (v(0,1,LI i RI)-a(0,0,LI i RI)*                  \
		      v(0,1,LI i-1 RI)-a(0,1,LI i RI)*                \
                      v(1,1,LI i-1 RI))/b(0,0,LI i RI);               \
    v(1,0,LI i RI) = (v(1,0,LI i RI)-a(1,0,LI i RI)*                  \
		      v(0,0,LI i-1 RI)-a(1,1,LI i RI)*                \
		      v(1,0,LI i-1 RI)-b(1,0,LI i RI)*                \
		      v(0,0,LI i RI))/b(1,1,LI i RI);                 \
    v(1,1,LI i RI) = (v(1,1,LI i RI)-a(1,0,LI i RI)*                  \
		      v(0,1,LI i-1 RI)-a(1,1,LI i RI)*                \
		      v(1,1,LI i-1 RI)-b(1,0,LI i RI)*                \
		      v(0,1,LI i RI))/b(1,1,LI i RI);                 \
    w(0,0,LI i RI) -= (c(0,0,LI i-1 RI)*w(0,0,LI i-1 RI)+             \
		       c(1,0,LI i-1 RI)*w(0,1,LI i-1 RI));            \
    w(1,0,LI i RI) -= (c(0,0,LI i-1 RI)*w(1,0,LI i-1 RI)+             \
		       c(1,0,LI i-1 RI)*w(1,1,LI i-1 RI));            \
    w(0,1,LI i RI) -= (c(0,1,LI i-1 RI)*w(0,0,LI i-1 RI)+             \
		       c(1,1,LI i-1 RI)*w(0,1,LI i-1 RI)+             \
		       b(0,1,LI i RI)*w(0,0,LI i RI));                \
    w(1,1,LI i RI) -= (c(0,1,LI i-1 RI)*w(1,0,LI i-1 RI)+             \
		       c(1,1,LI i-1 RI)*w(1,1,LI i-1 RI)+             \
		       b(0,1,LI i RI)*w(1,0,LI i RI));                \
  }                                                                   \
								      \
     /* The last point */                                             \
								      \
  for (i=istart;i<istop;i++){                                         \
    b(0,0,LI istop RI) -= (w(0,0,LI i RI)*v(0,0,LI i RI)+             \
			   w(0,1,LI i RI)*v(1,0,LI i RI));            \
    b(1,0,LI istop RI) -= (w(1,0,LI i RI)*v(0,0,LI i RI)+             \
			   w(1,1,LI i RI)*v(1,0,LI i RI));            \
    b(0,1,LI istop RI) -= (w(0,0,LI i RI)*v(0,1,LI i RI)+             \
			   w(0,1,LI i RI)*v(1,1,LI i RI));            \
    b(1,1,LI istop RI) -= (w(1,0,LI i RI)*v(0,1,LI i RI)+             \
			   w(1,1,LI i RI)*v(1,1,LI i RI));            \
  }                                                                   \
								      \
  b(0,1,LI istop RI) /= b(0,0,LI istop RI);                           \
  b(1,1,LI istop RI) -= b(1,0,LI istop RI)*b(0,1,LI istop RI);        \
								      

int blockTridiag2d::
  factorPeriodic(void){
   // Factor the 2D Block tridiagonal
   // 
   //         |B[0] C[0]                         A[0]   |
   //         |A[1] B[1] C[1]                           |
   //         |     A[2] B[2] C[2]                      |
   //   M =   |          .... .... ....                 |
   //         |              ....  ....    ....         |
   //         |                    A[n-1] B[n-1] C[n-1] |
   //         |C[n]                       A[n]   B[n]   |
   //
   //
   //
   //  where A[i], B[i], C[i] are two by two matrices.
   //  No pivoting is used for this case

  // v contains the last column and w the last row.

  v.redim(a);
  w.redim(a);
  v=0.0, w=0.0;

  if (iaxis==axis1){
#undef LI
#define LI
#undef RI
#define RI ,I2,I3
 INITIALIZEP(I1)
  }
  else if (iaxis==axis2){
#undef LI
#define LI I1,
#undef RI
#define RI ,I3
  INITIALIZEP(I2)
  }
  else if (iaxis==axis3){
#undef LI
#define LI I1,I2,
#undef RI
#define RI
  INITIALIZEP(I3)
  }
  else {
   printf("Invalid value for iaxis in INITIALIZEP.   Exiting\n");
   {throw "error";}
  }
#undef INITIALIZEP

  if (iaxis==axis1){
#undef LI
#define LI
#undef RI
#define RI ,I2,I3
 FACTORP(I1)
  }
  else if (iaxis==axis2){
#undef LI
#define LI I1,
#undef RI
#define RI ,I3
  FACTORP(I2)
  }
  else if (iaxis==axis3){
#undef LI
#define LI I1,I2,
#undef RI
#define RI
  FACTORP(I3)
  }
  else {
   printf("Invalid value for iaxis in FACTORP.   Exiting\n");
   {throw "error";}
  }

  return(0);
}
#undef FACTORP

#undef SOLVE
#define SOLVE(I) \
  int istart=I.getBase();                                          \
  int istop=I.getBound();                                          \
								   \
  /* Forward Elimination (Solve LZ=D) */                           \
  /* start with the first point        */                          \
  d.reshape(1,d.dimension(0),d.dimension(1),d.dimension(2),        \
	    d.dimension(3));                                       \
								   \
  d(0,0,LI istart RI) /= b(0,0,LI istart RI);                      \
  d(0,1,LI istart RI) = (d(0,1,LI istart RI)-b(1,0,LI istart RI)*  \
		       d(0,0,LI istart RI))/b(1,1,LI istart RI);   \
								   \
  /* The remaining points            */                            \
								   \
  for (int i=istart+1;i<=istop;i++){                               \
    d(0,0,LI i RI)=(d(0,0,LI i RI)-a(0,0,LI i RI)*d(0,0,LI i-1 RI)-\
		    a(0,1,LI i RI)*d(0,1,LI i-1 RI))/              \
		    b(0,0,LI i RI);                                \
    d(0,1,LI i RI)=(d(0,1,LI i RI)-a(1,0,LI i RI)*d(0,0,LI i-1 RI)-\
		    a(1,1,LI i RI)*d(0,1,LI i-1 RI)-               \
		    b(1,0,LI i RI)*d(0,0,LI i RI))/b(1,1,LI i RI); \
  }                                                                \
                                                                   \
								   \
  /* Now Backward Substitution        */                           \
  /* Start with the last point        */                           \
								   \
  /* Since the upper triangular decomposition has 1. on the        \
     diagonal, the last d contains the last solution x     */      \
								   \
  d(0,0,LI istop RI) -= b(0,1,LI istop RI)*d(0,1,LI istop RI);     \
								   \
  /* The remaining points  */                                      \
								   \
  for (i=istop-1;i>=istart;i--){                                   \
    d(0,1,LI i RI) -= (c(1,0,LI i RI)*d(0,0,LI i+1 RI)+            \
		     c(1,1,LI i RI)*d(0,1,LI i+1 RI));             \
    d(0,0,LI i RI) -= (b(0,1,LI i RI)*d(0,1,LI i RI)+              \
		     c(0,0,LI i RI)*d(0,0,LI i+1 RI)+              \
		     c(0,1,LI i RI)*d(0,1,LI i+1 RI));             \
  }                                                                \
  d.reshape(d.dimension(1),d.dimension(2),d.dimension(3),          \
	    d.dimension(4));                                       \

int blockTridiag2d::
  solve(realArray &D,systemType type){
  d.reference(D);
  if (checkDimension(d)==1) exit(1);
  if (type==normal) solveRegular();
  else if (type==periodic) solvePeriodic();
  else {
    printf("!! Unknown type of system !!\n");
    {throw "error";}
  }
  return 0;
}

int blockTridiag2d::
  solveRegular(){
  if (iaxis==axis1){
#undef LI
#define LI
#undef RI
#define RI ,I2,I3
 SOLVE(I1)
  }
  else if (iaxis==axis2){
#undef LI
#define LI I1,
#undef RI
#define RI ,I3
  SOLVE(I2)
  }
  else if (iaxis==axis3){
#undef LI
#define LI I1,I2,
#undef RI
#define RI
  SOLVE(I3)
  }
  else {
   printf("Invalid value for iaxis in SOLVE.   Exiting\n");
   {throw "error";}
  }

  return(0);
}
#undef SOLVE
//
//
//
#undef SOLVEP
#define SOLVEP(I)                                                  \
  int istart=I.getBase();                                          \
  int istop=I.getBound()-1;                                        \
								   \
  /* Forward Elimination (Solve LZ=D) */                           \
  /* start with the first point        */                          \
  d.reshape(1,d.dimension(0),d.dimension(1),d.dimension(2),        \
	    d.dimension(3));                                       \
								   \
  d(0,0,LI istart RI) /= b(0,0,LI istart RI);                      \
  d(0,1,LI istart RI) = (d(0,1,LI istart RI)-b(1,0,LI istart RI)*  \
		       d(0,0,LI istart RI))/b(1,1,LI istart RI);   \
								   \
  /* The interior points            */                             \
								   \
  for (int i=istart+1;i<istop;i++){                                \
    d(0,0,LI i RI)=(d(0,0,LI i RI)-a(0,0,LI i RI)*d(0,0,LI i-1 RI)-\
		    a(0,1,LI i RI)*d(0,1,LI i-1 RI))/              \
		    b(0,0,LI i RI);                                \
    d(0,1,LI i RI)=(d(0,1,LI i RI)-a(1,0,LI i RI)*d(0,0,LI i-1 RI)-\
		    a(1,1,LI i RI)*d(0,1,LI i-1 RI)-               \
		    b(1,0,LI i RI)*d(0,0,LI i RI))/b(1,1,LI i RI); \
  }                                                                \
								   \
  /* The last point */                                             \
								   \
  for (i=istart;i<istop;i++){                                      \
    d(0,0,LI istop RI) -= (w(0,0,LI i RI)*d(0,0,LI i RI)+          \
			 w(0,1,LI i RI)*d(0,1,LI i RI));           \
    d(0,1,LI istop RI) -= (w(1,0,LI i RI)*d(0,0,LI i RI)+          \
			 w(1,1,LI i RI)*d(0,1,LI i RI));           \
  }                                                                \
                                                                   \
  d(0,0,LI istop RI) /= b(0,0,LI istop RI);                        \
  d(0,1,LI istop RI) -= b(1,0,LI istop RI)*d(0,0,LI istop RI);     \
  d(0,1,LI istop RI) /= b(1,1,LI istop RI);                        \
								   \
  /* Now Backward Substitution        */                           \
  /* Start with the last point        */                           \
								   \
  /* Since the upper triangular decomposition has 1. on the */     \
  /* diagonal, the last d contains the last solution x     */      \
								   \
  d(0,0,LI istop RI) -= b(0,1,LI istop RI)*d(0,1,LI istop RI);     \
								   \
  /* The point at istop-1   */                                     \
								   \
  d(0,1,LI istop-1 RI)-=(v(1,0,LI istop-1 RI)*d(0,0,LI istop RI)+  \
			 v(1,1,LI istop-1 RI)*d(0,1,LI istop RI)); \
  d(0,0,LI istop-1 RI)-=(b(0,1,LI istop-1 RI)*d(0,1,LI istop-1 RI)+\
			 v(0,0,LI istop-1 RI)*d(0,0,LI istop RI)+  \
			 v(0,1,LI istop-1 RI)*d(0,1,LI istop RI)); \
  /* The remaining points  */                                      \
								   \
  for (i=istop-2;i>=istart;i--){                                   \
    d(0,1,LI i RI) -= (c(1,0,LI i RI)*d(0,0,LI i+1 RI)+            \
		     c(1,1,LI i RI)*d(0,1,LI i+1 RI)+              \
		     v(1,0,LI i RI)*d(0,0,LI istop RI)+            \
		     v(1,1,LI i RI)*d(0,1,LI istop RI));           \
    d(0,0,LI i RI) -= (b(0,1,LI i RI)*d(0,1,LI i RI)+              \
		     c(0,0,LI i RI)*d(0,0,LI i+1 RI)+              \
		     c(0,1,LI i RI)*d(0,1,LI i+1 RI)+              \
		     v(0,0,LI i RI)*d(0,0,LI istop RI)+            \
		     v(0,1,LI i RI)*d(0,1,LI istop RI));           \
  }                                                                \
  d.reshape(d.dimension(1),d.dimension(2),d.dimension(3),          \
	    d.dimension(4));                                       \
  /* The periodic points */                                        \
  d(0,LI istop+1 RI) = d(0,LI istart RI);                          \
  d(1,LI istop+1 RI) = d(1,LI istart RI);                          \

int blockTridiag2d::
  solvePeriodic(){
  if (iaxis==axis1){
#undef LI
#define LI
#undef RI
#define RI ,I2,I3
 SOLVEP(I1)
  }
  else if (iaxis==axis2){
#undef LI
#define LI I1,
#undef RI
#define RI ,I3
  SOLVEP(I2)
  }
  else if (iaxis==axis3){
#undef LI
#define LI I1,I2,
#undef RI
#define RI
  SOLVEP(I3)
  }
  else {
   printf("Invalid value for iaxis in SOLVEP.   Exiting\n");
   {throw "error";}
  }

  return(0);
}
#undef SOLVEP
