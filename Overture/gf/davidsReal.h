/*  -*-Mode: c++; -*-  */
#ifndef _DavidsReal_H_
#define _DavidsReal_H_  "REAL.h"

#undef USE_FLOAT
#define USE_FLOAT // *** THis is a test only
#if USE_FLOAT
#     define REAL float
#     define REALArray floatArray
#     define REALStreamArray floatStreamArray
#     define ListOfREALArray ListOffloatArray
#     define REALCompositeGridFunction floatCompositeGridFunction
#     define REALMappedGridFunction floatMappedGridFunction
#elif OV_USE_USE_DOUBLE
#     define REAL double
#     define REALArray double
#     define REALStreamArray doubleStreamArray
#     define ListOfREALArray ListOfdoubleArray
#     define REALCompositeGridFunction doubleCompositeGridFunction
#     define REALMappedGridFunction doubleMappedGridFunction
//#else
//ERROR:: REAL.H must define USE_FLOAT or USE_DOUBLE
#endif

#endif
