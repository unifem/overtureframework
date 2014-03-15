#ifndef __ARRAY_SIMPLE_COMMON_H__
#define __ARRAY_SIMPLE_COMMON_H__

// *wdh* 010730 : The SGI compiler wants to see iostream.h even though the new headers are supported for <string> etc.
//                I hope I haven't broken anything here.

//kkc 040415 #include <iostream.h>
#include "OvertureDefine.h"
#include OV_STD_INCLUDE(iostream)

#ifndef OV_USE_OLD_STL_HEADERS
#include <string>
OV_USINGNAMESPACE(std);
#else
#include <string.h>
#endif

/// assume explicit is available unless told otherwise at compile time
#ifndef EXPLICIT
#define EXPLICIT explicit
#endif

/** \def OV_DEBUG
 *  \brief define this macro to turn assertions and range checking on
 */

/** \def RANGE_CHK
 *  \brief turns range checking on/off
 *  if OV_DEBUG is not defined, turn range checking off by setting it to true always
 */

#ifdef OV_DEBUG
#ifndef assert
#define assert(x) if ( !(x) ) throw "assertion failed"
#endif
#undef RANGE_CHK
#define RANGE_CHK(x) x
#else
#undef RANGE_CHK
#define RANGE_CHK(x) true
#ifndef assert
#define assert(x) 
#endif
#endif

// // //
/// the largest possible rank/dimension of the array class
/**  if you change this parameter, you must also do the following :
 *    -# Adjust ArraySimpleFixed
 *    -# Add an appropropriate constructor for ArraySimple
 *    -# Add new copy constructors for ArraySimple
 *    -# Add a new ArraySimple::operator() 
 *    -# Adjust/add the appropriate ostream & operator<<
 */
const int MAXRANK = 5;
// // // 


#endif
