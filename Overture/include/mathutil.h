// Math utilities
#ifndef MATHUTIL_H
#define MATHUTIL_H "Mathutil.h"

#define SQR(x) ((x)*(x))
//#define SQRT(x) pow((x),.5)  // *wdh* 061013
#define SQRT(x) sqrt((x))

#ifndef NO_APP
#include "OvertureTypes.h"  // define GUITypes::real to be float or double
#endif

inline int max(int x1,int x2 ){ return x1>x2 ? x1 : x2; }
inline int max(int x1, unsigned int x2) { return (unsigned int)x1>x2 ? x1 : x2; }
inline int max(unsigned int x1, int x2) { return x1>(unsigned int)x2 ? x1 : x2; }

// these next are needed on the dec machines
inline int max(unsigned long x1, int x2) { return x1>x2 ? x1 : x2; }
inline int max(int x1, unsigned long  x2) { return x1>x2 ? x1 : x2; }

inline double max(int x1,double x2 ){ return x1>x2 ? x1 : x2; }
inline double max(double x1,int x2 ){ return x1>x2 ? x1 : x2; }
inline float max(int x1,float x2 ){ return x1>x2 ? x1 : x2; }
inline float max(float x1,int x2 ){ return x1>x2 ? x1 : x2; }

inline float max(float x1,float x2 ){ return x1>x2 ? x1 : x2; }

inline double max(double x1,double x2 ){ return x1>x2 ? x1 : x2; }

inline double max(float x1,double x2 ){ return x1>x2 ? double(x1) : x2; }

inline double max(double x1,float x2 ){ return x1>x2 ? x1 : double(x2); }

// ifndef USE_PPP *no longer needed* 010117
inline int min(int x1,int x2 ){ return x1<x2 ? x1 : x2; }
// endif

inline float min(float x1,float x2 ){ return x1<x2 ? x1 : x2; }

inline double min(double x1,double x2 ){ return x1<x2 ? x1 : x2; }

inline double min(float x1,double x2 ){ return x1<x2 ? double(x1) : x2; }

inline double min(double x1,float x2 ){ return x1<x2 ? x1 : double(x2); }

// ************************************************
// **** here are max/min for multiple arguments ***
// ************************************************
inline int max(int x1,int x2,int x3 ){ return max(x1,max(x2,x3)); }
inline int max(int x1,int x2,int x3,int x4 ){ return max(x1,max(x2,x3,x4));}
inline int max(int x1,int x2,int x3,int x4,int x5 )
   { return max(x1,max(x2,x3,x4,x5));}

inline float max(float x1,float x2,float x3 ){ return max(x1,max(x2,x3)); }
inline float max(float x1,float x2,float x3,float x4 )
 { return max(x1,max(x2,x3,x4));}
inline float max(float x1,float x2,float x3,float x4,float x5 )
 { return max(x1,max(x2,x3,x4,x5));}

inline double max(double x1,double x2,double x3 ){ return max(x1,max(x2,x3)); }
inline double max(double x1,double x2,double x3,double x4 )
 { return max(x1,max(x2,x3,x4));}
inline double max(double x1,double x2,double x3,double x4,double x5 )
 { return max(x1,max(x2,x3,x4,x5));}

inline int min(int x1,int x2,int x3 ){ return min(x1,min(x2,x3)); }
inline int min(int x1,int x2,int x3,int x4 ){ return min(x1,min(x2,x3,x4));}
inline int min(int x1,int x2,int x3,int x4,int x5 )
   { return min(x1,min(x2,x3,x4,x5));}

inline float min(float x1,float x2,float x3 ){ return min(x1,min(x2,x3)); }
inline float min(float x1,float x2,float x3,float x4 )
 { return min(x1,min(x2,x3,x4));}
inline float min(float x1,float x2,float x3,float x4,float x5 )
 { return min(x1,min(x2,x3,x4,x5));}

inline double min(double x1,double x2,double x3 ){ return min(x1,min(x2,x3)); }
inline double min(double x1,double x2,double x3,double x4 )
 { return min(x1,min(x2,x3,x4));}
inline double min(double x1,double x2,double x3,double x4,double x5 )
 { return min(x1,min(x2,x3,x4,x5));}


// new gcc also defines round
inline int rounder(double x ){ return x>0 ? int(x+.5) : int(x-.5); } //  round to nearest integer

// these are needed by the dec compiler

#if 0

#ifdef __GNUC__
inline float pow(float x1,int x2 ){ return x2==0 ? 1 : pow(double(x1),double(x2)); }
inline double pow(double x1,int x2 ){ return x2==0 ? 1 : pow(x1,double(x2)); }
#endif

#ifndef __GNUC__

#ifdef OV_USE_DOUBLE
inline float pow(float &x1,int x2 ){ return x2==0 ? 1 : pow(double(x1),double(x2)); }
#else
inline double pow(double &x1,int x2 ){ return x2==0 ? 1 : pow(x1,double(x2)); }
#endif

inline double pow(real &x1,int x2) { return x2==0 ? 1 : pow(x1,real(x2)); }

inline float pow(const float &x1,int x2 ){ return x2==0 ? 1 : pow(double(x1),double(x2)); }
inline double pow(const double &x1,int x2 ){ return x2==0 ? 1 : pow(x1,double(x2)); }
#ifndef __hpux
#endif
inline double pow(float x1,double x2 ){ return pow(double(x1),x2); }
inline double pow(double x1,float x2 ){ return pow(x1,double(x2)); }
inline float pow(float x1,float x2 ){ return pow(double(x1),double(x2)); }
inline double pow(double x1,int x2 ){ return x2==0 ? 1 : pow(x1,double(x2)); }

inline double pow(int x1,int x2 ){ return x2>0 ? rounder(pow(double(x1),double(x2))) : pow(double(x1),double(x2)); } 
#else
inline double pow(int x1,int x2 ){ return x2>0 ? rounder(pow(double(x1),double(x2))) : pow(double(x1),double(x2)); } 
#endif
#endif

#include <cmath> 
using std::pow; 

inline double pow(int x1,int x2 ){ return x2>0 ? rounder(pow(double(x1),double(x2))) : pow(double(x1),double(x2)); } 

#ifdef __GNUC__
inline double sqrt(int x) { return sqrt(double(x)); }
#endif

#ifndef __KCC
inline float atan2( float  x1, float  x2 ){ return atan2(double(x1),double(x2)); } 
#endif
inline float atan2(double x1, float  x2 ){ return atan2(double(x1),double(x2)); } 
inline float atan2( float  x1,double x2 ){ return atan2(double(x1),double(x2)); } 
inline float fmod( float  x1, float  x2 ){ return fmod(double(x1),double(x2)); } 
inline float fmod(double x1, float  x2 ){ return fmod(double(x1),double(x2)); } 
inline float fmod( float  x1,double x2 ){ return fmod(double(x1),double(x2)); } 
  
inline int sign(int    x ) { return x>0 ? 1 : ( x<0 ? -1 : 0);}
inline int sign(float  x ) { return x>0 ? 1 : ( x<0 ? -1 : 0);}
inline int sign(double x ) { return x>0 ? 1 : ( x<0 ? -1 : 0);}

//  020410 : wdh these don't seem to be needed with g++
#ifndef __GNUC__
inline double log10( int x ){ return log10((double)x); }
inline double log( int x ){ return log((double)x); }
#endif

inline double fabs( int x ){ return fabs((double)x); }

int inline 
floorDiv(int numer, int denom )
//  return the floor( "numer/denom" ) (ie. always chop to the left).
//  Assumes denom>0
//  
//  floorDiv(  3,2)  = 1  same as 3/2
//  floorDiv( -3,2 ) =-2  **note** not the same as (-3)/2 = -1
{
  if( numer>0 )
    return numer/denom;
  else
    return (numer-denom+1)/denom;
}
   
// =================================================================
/// \brief Overture modulus function: returns mod(a,b) with the 
///   result between 0 and b-1 (i.e. always positive unlike a % b)
/// \details This function assumes b>0 
// ================================================================
inline int ovmod (int a, int b)
{
   int ret = a % b;
   if(ret < 0)
     ret+=b;
   return ret;
}



#ifndef NO_APP
#define ARRAY_GET_DATA_POINTER(type,aType) \
inline type* getDataPointer(const aType ## Array & u)  \
{  \
 return u.Array_Descriptor.Array_View_Pointer3+  \
          u.getBase(0)+u.getRawDataSize(0)*(  \
          u.getBase(1)+u.getRawDataSize(1)*(  \
          u.getBase(2)+u.getRawDataSize(2)*(u.getBase(3))));  \
}

// return the data pointer to an array for apasing to Fortran (works for views too)
// Only works for up to 4D arrays.
ARRAY_GET_DATA_POINTER(int,int)
ARRAY_GET_DATA_POINTER(float,float)
ARRAY_GET_DATA_POINTER(double,double)

#ifdef USE_PPP
ARRAY_GET_DATA_POINTER(int,intSerial)
ARRAY_GET_DATA_POINTER(float,floatSerial)
ARRAY_GET_DATA_POINTER(double,doubleSerial)
#endif

#undef ARRAY_GET_DATA_POINTER
#endif // ifndef NO_APP

#endif // MATHUTIL_H
