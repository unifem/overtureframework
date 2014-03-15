
#ifndef __FORTTYPE

/* wdh */
#ifdef DOUBLE
#undef D_PRECISION
#define D_PRECISION
#endif

#if ( CRAY1 || CRAY2 )		/*  CRAY Unicos */

#define __REAL	REAL
#define __float	float
#define __NCHPWD	8

#define __INTEGER	INTEGER
#define __int	int
#define __NCHINT	8

#define __POINTER	INTEGER
#define __NCHPTR	8

#define __FORTTYPE
#endif


#if ( ( sgi && mips ) || DEC_ALPHA )	/*  IRIS 4D(mips1-4), DEC Alpha  */

#ifdef D_PRECISION
#define __REAL	REAL*8
#define __float	double
#define __NCHPWD	8

#else
#define __REAL	REAL
#define __float	float
#define __NCHPWD	4
#endif

#define __INTEGER	INTEGER
#define __int	int
#define __NCHINT	4

#if ( MIPS4 || _MIPS_SZPTR==64 || DEC_ALPHA )
#define __POINTER	INTEGER*8
#define __NCHPTR	8

#else
#define __POINTER	INTEGER
#define __NCHPTR	4
#endif

#define __FORTTYPE
#endif


#ifndef __FORTTYPE		/*  Default (seems most prevalent)  */

#ifdef D_PRECISION
#define __REAL	REAL*8
#define __float	double
#define __NCHPWD	8

#else
#define __REAL	REAL
#define __float	float
#define __NCHPWD	4
#endif

#define __INTEGER	INTEGER
#define __int	int
#define __NCHINT	4

#define __POINTER	INTEGER
#define __NCHPTR	4

#define __FORTTYPE
#endif

#endif

