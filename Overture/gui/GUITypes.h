#ifndef GUI_TYPES_H
#define GUI_TYPES_H

// Define various things for Overture

#ifdef NO_APP

#include "GUIDefine.h"
#include <float.h>

#ifndef NO_APP
#include "A++.h"
#endif

// *wdh* 961005
#undef FALSE
#define FALSE 0

#ifdef NO_APP
#undef TRUE
#define TRUE 1

//#include "ArraySimple.h"
#include "kk_Array.hh"

#include <math.h>
#endif

namespace GUITypes {

#  ifdef LONGINT
#    define INTEGER_MIN LONG_MIN
#    define INTEGER_MAX LONG_MAX
     typedef long int Integer;
     // typedef longIntSerialArray IntegerArray; // This does not exist, so we must use intSerialArray instead.
#  else
#    define INTEGER_MIN INT_MIN
#    define INTEGER_MAX INT_MAX
     typedef int Integer;
#  endif // LONGINT

#ifndef NO_APP
typedef intSerialArray IntegerArray;        
typedef intArray       IntegerDistributedArray;
#else
typedef KK::Array<int> IntegerArray;
typedef KK::Array<int> IntegerDistributedArray;
#endif
  
typedef int Logical; // this must alway match LogicalArray !!
  
const Logical LogicalFalse=0, 
      LogicalTrue=!LogicalFalse;
  
typedef IntegerArray            LogicalArray; // There is no such thing as a boolArray!
typedef IntegerDistributedArray LogicalDistributedArray; 
typedef Integer                 LogicalAE;    // This might not be the same as Logical.

  
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
#ifndef NO_APP
     typedef doubleSerialArray RealArray;
     // typedef longDoubleSerialArray DoubleRealArray; // This does not exist, so
     typedef doubleSerialArray DoubleRealArray;        // we use doubleSerialArray.
     typedef doubleSerialArray RealSerialArray;   
     typedef doubleSerialArray realSerialArray;   
     typedef doubleArray       RealDistributedArray;
     typedef doubleArray       DoubleRealDistributedArray;
#else
     typedef KK::Array<double> RealArray;
     typedef KK::Array<double> DoubleRealArray;
     typedef KK::Array<double> RealSerialArray;   
     typedef KK::Array<double> realSerialArray;   
     typedef KK::Array<double> RealDistributedArray;
     typedef KK::Array<double> DoubleRealDistributedArray;
#endif
  
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
#ifndef NO_APP
     typedef floatSerialArray  RealArray;
     typedef floatSerialArray  RealSerialArray;
     typedef floatSerialArray  realSerialArray;

     typedef doubleSerialArray DoubleRealArray;
     typedef floatArray        RealDistributedArray;
     typedef doubleArray       DoubleRealDistributedArray;
#else
     typedef KK::Array<float> RealArray;
     typedef KK::Array<float> RealSerialArray;
     typedef KK::Array<float> realSerialArray;

     typedef KK::Array<double> DoubleRealArray;
     typedef KK::Array<float>  RealDistributedArray;
     typedef KK::Array<double> DoubleRealDistributedArray;
#endif

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
#ifndef NO_APP
typedef intArray             intDistributedArray;
typedef floatArray           floatDistributedArray;
typedef doubleArray          doubleDistributedArray;
#else
typedef KK::Array<int>     intDistributedArray;
typedef KK::Array<float>   floatDistributedArray;
typedef KK::Array<double>  doubleDistributedArray;

typedef KK::Array<int>     intSerialArray;
typedef KK::Array<float>   floatSerialArray;
typedef KK::Array<double>  doubleSerialArray;
#endif

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

}

using GUITypes::RealArray;
using GUITypes::IntegerArray;
using GUITypes::realArray;
using GUITypes::CopyType;
using GUITypes::intSerialArray;
using GUITypes::floatSerialArray;
using GUITypes::doubleSerialArray;
//using GUITypes::intArray;

inline std::string
substring(const std::string & s, const int startPosition, const int endPosition )
// ====================================================================================
//   Return a substring -- replacement for aString(i1,i2)
// ====================================================================================
{
  return s.substr(startPosition,endPosition-startPosition+1);
}    


inline int
matches(const std::string & s,  const char *name ) 
// ================================================================================
// If all the characters of name match the characters of this string then
// return the number of characters of name, otherwise return zero.
// ==================================================================================
{
  int lenName=strlen(name);
  if( s.substr(0,lenName)==name )
    return lenName;
  else
    return 0;
}

#define getCPU KK::getcpu
#else

#include "OvertureTypes.h"
#include <string>

namespace GUITypes {

  typedef ::real real;

}
#endif

inline unsigned int str_matches(std::string s, std::string m)
{ // *wdh* 051012 return s.find(m)!=std::string::npos ? m.length() : 0; 
  return s.find(m)==0 ? m.length() : 0; // *wdh* The string must match from the start
}

#endif



