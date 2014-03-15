#ifndef OVERTURE_TYPES_H
#define OVERTURE_TYPES_H

// Define various things for Overture

#include "OvertureDefine.h"
#include <float.h>


/* *wdh* for pgi */
#ifdef __PGI
  #include "math.h"
  /* inline float abs(float x){ return fabs(x); }  */
  #include <cmath> 
#endif

// This next line was needed when compiling mogl.C with P++ version, BadValue appears in
// PADRE_Parti_Template.C and some motif or opengl header defines it I think
#undef BadValue
#include "A++.h"

// *wdh* 961005
#undef FALSE
#define FALSE 0

#  ifdef LONGINT
#    define INTEGER_MIN LONG_MIN
#    define INTEGER_MAX LONG_MAX
     typedef long int Integer;
     // typedef longIntSerialArray IntegerArray; // This does not exist, so we
     typedef intSerialArray IntegerArray;        // must use intSerialArray instead.
     typedef intArray     IntegerDistributedArray;
#  else
#    define INTEGER_MIN INT_MIN
#    define INTEGER_MAX INT_MAX
     typedef int Integer;
     typedef intSerialArray IntegerArray;
     typedef intArray     IntegerDistributedArray;
#  endif // LONGINT
  
typedef int Logical; // this must alway match LogicalArray !!
  
const Logical LogicalFalse=0, 
      LogicalTrue=!LogicalFalse;
  
typedef IntegerArray            LogicalArray; // There is no such thing as a boolArray!
typedef IntegerDistributedArray LogicalDistributedArray; 
typedef Integer                 LogicalAE;    // This might not be the same as Logical.

typedef int64_t doubleLengthInt; // this defines a double length int -- currently a 64 bit int

  
#  ifdef OV_USE_DOUBLE
#    ifndef USE_DOUBLE
#      define USE_DOUBLE
#    endif // USE_DOUBLE
#    ifdef USE_FLOAT
#      undef USE_FLOAT
#    endif // USE_FLOAT
#    ifndef   _REAL_H_
       // avoid conflict with boxlib  
       typedef double            Real;
#    endif
     typedef long double       DoubleReal;
     typedef doubleSerialArray RealArray;
     // typedef longDoubleSerialArray DoubleRealArray; // This does not exist, so
     typedef doubleSerialArray DoubleRealArray;        // we use doubleSerialArray.
     typedef doubleSerialArray RealSerialArray;   
     typedef doubleSerialArray realSerialArray;   
     typedef doubleArray       RealDistributedArray;
     typedef doubleArray       DoubleRealDistributedArray;
  
     #define MPI_Real MPI_DOUBLE

//     Define "real" versions of the constants in <float.h>
#    define REAL_RADIX    FLT_RADIX
#    define REAL_ROUNDS   FLT_ROUNDS 
#    define REAL_DIG      DBL_DIG
#    define REAL_EPSILON  DBL_EPSILON
#    define REAL_MANT_DIG DBL_MANT_DIG
#    define REAL_MAX      DBL_MAX
#    define REAL_MAX_EXP  DBL_MAX_EXP
#    define REAL_MIN      DBL_MIN
#    define REAL_MIN_EXP  DBL_MIN_EXP

#  else

#    ifdef USE_DOUBLE
#      undef USE_DOUBLE
#    endif // USE_DOUBLE
#    ifndef USE_FLOAT
#      define USE_FLOAT
#    endif // USE_FLOAT
#    ifndef   _REAL_H_
       typedef float             Real;
#    endif
     typedef double            DoubleReal;
     typedef floatSerialArray  RealArray;
     typedef floatSerialArray  RealSerialArray;
     typedef floatSerialArray  realSerialArray;

     typedef doubleSerialArray DoubleRealArray;
     typedef floatArray        RealDistributedArray;
     typedef doubleArray       DoubleRealDistributedArray;

     #define MPI_Real MPI_FLOAT

//     Define "real" versions of the constants in <float.h>
#    define REAL_RADIX    FLT_RADIX
#    define REAL_ROUNDS   FLT_ROUNDS 
#    define REAL_DIG      FLT_DIG
#    define REAL_EPSILON  FLT_EPSILON
#    define REAL_MANT_DIG FLT_MANT_DIG
#    define REAL_MAX      FLT_MAX
#    define REAL_MAX_EXP  FLT_MAX_EXP
#    define REAL_MIN      FLT_MIN
#    define REAL_MIN_EXP  FLT_MIN_EXP

#  endif // DOUBLE
  
typedef Real                 real;
typedef RealDistributedArray realArray;  
typedef RealDistributedArray realDistributedArray;
typedef intArray             intDistributedArray;
typedef floatArray           floatDistributedArray;
typedef doubleArray          doubleDistributedArray;


enum CopyType { DEEP, SHALLOW, NOCOPY };
  
// Constants for A++ array indexing.
/* *wdh* 030423 enum { START=0, END=1, X_AXIS=0, Y_AXIS=1, Z_AXIS=2 }; */
  
// Macros used by Boxlib.
#  undef  BL_SPACEDIM
#  define BL_SPACEDIM 3
#  undef  BL_ARCH_IEEE
#  define BL_ARCH_IEEE
  
#if ( defined(__alpha) || defined(__sgi) )
  inline ostream& operator<<(ostream& s, const long double& x)
    { s << (double)x; return s; }
#endif // ( defined(__alpha) || defined(__sgi) )

#if !defined(OV_BOOL_DEFINED) && !defined(USE_PPP)
  typedef int bool;    // this will be the new standard for Boolean
#endif

#ifdef OV_EXCEPTIONS_NOT_SUPPORTED
  #define throw exit(1); cout << 
#endif


#endif



