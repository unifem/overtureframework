/*
 * fia.h - fortran interface
 */


#ifndef __FIA_H__
#define __FIA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* From scstd.h - an attempt to expand the interface to support f90 modules.
 *  Failed - sun mangles module procdure names as procedure.in.module_
 *  The .'s cause the C compiler grief.
 *
 * also simplified by using cpp token cating.
 * (i.e the trailing underscore is not explicit in the macro)
 */
/*--------------------------------------------------------------------------*/

/* F90_ID - attempt a uniform naming of FORTRAN 90 functions which 
 *        - gets around loader naming conventions
 *        -
 *        - F90_ID(foo_, foo, FOO)(x, y, z)
 */

#if 0
/* this is from F77_ID in score */
#ifdef ANSI_F90
# define F90_ID(x, X)  X
#endif

#ifndef __GNUC__

# ifdef AIX
#  define F90_ID(x, X)  x
# endif

# ifdef HPUX
#  define F90_ID(x, X)  x
# endif

#endif

#ifndef F90_ID
# define F90_ID(x, X)  x ## _
#endif
#endif

/*--------------------------------------------------------------------------*/

#ifdef AIX
# define F90_ID(x, X)  x
# define FCDTOCP(x)      ((char *) x)
# define CPTOFCD(x, len) (x)
# define FCDLENARG(var)  ,FIXNUM var
# define FCDLENDEF(var, x)
# define CPLENARG(var) ,(var)

typedef float F90_REAL4;
#define F90_REAL4_S "float"

typedef double F90_REAL8;
#define F90_REAL8_S "double"

#define F90_REAL16_S "unknown"

typedef float F90_REAL;
#define F90_REAL_S "float"

typedef signed char F90_INT1;
#define F90_INT1_S "signed char"

typedef short F90_INT2;
#define F90_INT2_S "short"

typedef int F90_INT4;
#define F90_INT4_S "int"

#define F90_INT8_S "unknown"

typedef int F90_INTEGER;
#define F90_INTEGER_S "int"

typedef unsigned int F90_LOGICAL;
#define F90_LOGICAL_S "int"

typedef struct _s_f90_complex4 {
  float real;
  float imag;
} F90_COMPLEX4;

typedef struct _s_f90_complex4 F90_COMPLEX;
/*
typedef struct _s_f90_complex4 F90_COMPLEX4;
*/

typedef struct _s_f90_complex8 {
  double real;
  double imag;
} F90_COMPLEX8;

/*
typedef struct _s_f90_complex8 F90_COMPLEX8;
*/
#endif

/*--------------------------------------------------------------------------*/

#ifdef __alpha
# define F90_ID(x, X)  x ## _
# define FCDTOCP(x)      ((char *) x)
# define CPTOFCD(x, len) (x)
# define FCDLENARG(var)  ,FIXNUM var
# define FCDLENDEF(var, x)
# define CPLENARG(var) ,(var)

typedef float F90_REAL4;
#define F90_REAL4_S "float"

typedef double F90_REAL8;
#define F90_REAL8_S "double"

#define F90_REAL16_S "unknown"

typedef float F90_REAL;
#define F90_REAL_S "float"

typedef signed char F90_INT1;
#define F90_INT1_S "signed char"

typedef short F90_INT2;
#define F90_INT2_S "short"

typedef int F90_INT4;
#define F90_INT4_S "int"

typedef long F90_INT8;
#define F90_INT8_S "long"

typedef int F90_INTEGER;
#define F90_INTEGER_S "int"

typedef unsigned F90_LOGICAL;
#define F90_LOGICAL_S "int"

typedef struct _s_f90_complex4 {
  float real;
  float imag;
} F90_COMPLEX4;

typedef struct _s_f90_complex4 F90_COMPLEX;
/*
typedef struct _s_f90_complex4 F90_COMPLEX4;
*/

typedef struct _s_f90_complex8 {
  double real;
  double imag;
} F90_COMPLEX8;

/*
typedef struct _s_f90_complex8 F90_COMPLEX8;
*/

#endif


/*--------------------------------------------------------------------------*/

#if defined(__linux) || defined(__APPLE__)
# define F90_ID(x, X)  x ## _
# define FCDTOCP(x)      ((char *) x)
# define CPTOFCD(x, len) (x)
# define FCDLENARG(var)  ,FIXNUM var
# define FCDLENDEF(var, x)
# define CPLENARG(var) ,(var)

typedef float F90_REAL4;
#define F90_REAL4_S "float"

typedef double F90_REAL8;
#define F90_REAL8_S "double"

#define F90_REAL16_S "unknown"

typedef float F90_REAL;
#define F90_REAL_S "float"

typedef signed char F90_INT1;
#define F90_INT1_S "signed char"

typedef short F90_INT2;
#define F90_INT2_S "short"

typedef int F90_INT4;
#define F90_INT4_S "int"

typedef long F90_INT8;
#define F90_INT8_S "long"

typedef int F90_INTEGER;
#define F90_INTEGER_S "int"

typedef unsigned int F90_LOGICAL;
#define F90_LOGICAL_S "int"

#endif

/*--------------------------------------------------------------------------*/

#ifdef __sun
# define F90_ID(x, X)  x ## _
# define FCDTOCP(x)      ((char *) x)
# define CPTOFCD(x, len) (x)
# define FCDLENARG(var)  ,FIXNUM var
# define FCDLENDEF(var, x)
# define CPLENARG(var) ,(var)
#endif

/*--------------------------------------------------------------------------*/

#ifdef UNICOS
# define F90_ID(x, X)  X
# define FCDTOCP(x)      (_fcdtocp(x))
# define CPTOFCD(x, len) (_cptofcd(x, len))
# define FCDLENARG(var)  
# define FCDLENDEF(var, x)  FIXNUM var = _fcdlen(x);
# define CPLENARG(var)
#endif

/*--------------------------------------------------------------------------*/
#if 0
/*--------------------------------------------------------------------------*/

/* F90_ID - attempt a uniform naming of FORTRAN 90 functions which 
 *        - gets around loader naming conventions
 *        -
 *        - F90_ID(procedure, PROCEDURE)(x, y, z)
 *        - F90_MOD(module, MODULE, procedure, PROCEDURE)(x, y, z)
 */

#ifdef __sun

#define F90_ID(x, X)  x ## _
#define F90_MOD(m, M, x, X)  x ## in ## m ## _

#endif

/*--------------------------------------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif

#endif /* __FIA__ */
