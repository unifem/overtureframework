#ifndef _Types
#define _Types

//
// Who to blame:  Geoff Chesshire
//

#include "A++.h"

#ifdef LONGINT
typedef long int    Integer;
// typedef longIntArray intArray; // Unfortunately this does not exist,
typedef intArray    intArray;     // so we must use intArray instead.
#else
typedef int         Integer;
typedef intArray    intArray;
#endif // LONGINT

#ifdef DOUBLE
#ifndef USE_DOUBLE
#define USE_DOUBLE
#endif // USE_DOUBLE
#ifdef USE_FLOAT
#undef USE_FLOAT
#endif // USE_FLOAT
typedef double      Real;
typedef long double DoubleReal;
typedef doubleArray realArray;
// typedef longDoubleArray DoubleRealArray; // Unfortunately this does not
typedef doubleArray DoubleRealArray;        // exist, so we use doubleArray.

#else
#ifdef USE_DOUBLE
#undef USE_DOUBLE
#endif // USE_DOUBLE
#ifndef USE_FLOAT
#define USE_FLOAT
#endif // USE_FLOAT
typedef float       Real;
typedef double      DoubleReal;
typedef floatArray  realArray;
typedef doubleArray DoubleRealArray;
#endif // DOUBLE

typedef Integer                     Logical;
typedef Integer                     Pointer;
typedef struct { Real       x, y; } Complex;
typedef struct { DoubleReal x, y; } DoubleComplex;
typedef intArray                LogicalArray;
typedef intArray                PointerArray;

// These typedefs will go away.  Use the recommended replacements instead.
typedef Integer         Int;         // Use Integer.
typedef intArray    IntArray;    // Use intArray.
typedef Real            Float;       // Use Real.
typedef DoubleReal      Double;      // Use DoubleReal.
typedef realArray       FloatArray;  // Use realArray.
typedef DoubleRealArray DoubleArray; // Use DoubleRealArray.

const Logical LogicalFalse=0, LogicalTrue=-1;

enum CopyType { DEEP, SHALLOW, NOCOPY };

// Constants for A++ array indexing.
enum { START=0, END=1, X_AXIS=0, Y_AXIS=1, Z_AXIS=2 };

// Macros used by Boxlib.
#undef  SPACEDIM
#define SPACEDIM 3
#undef  ARCH_IEEE
#define ARCH_IEEE

#endif // _Types
