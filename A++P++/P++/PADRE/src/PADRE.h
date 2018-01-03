// file: PADRE.h

#ifndef PADRE_H
#define PADRE_H 1

/* include <PADRE_config.h> */
/* Define to enable PADRE code to use the MPI message passing interface */
#define PADRE_ENABLE_MP_INTERFACE_MPI 1


/*
  This file used to also include certain macros.  I moved them to the file
  PADRE_macros.h because I needed them but I did not want all the headers included
  by this file.  To get the macros, this file includes PADRE_macros.h.  BTNG.
*/
#include <PADRE_macros.h>

/*
  This file is strictly for the benefit of P++, which does not want to be bothered
  with including any PADRE file except for PADRE.h (yes, this is a legacy).
  Therefore, PADRE.h should include everything.  This file should not be used by
  any PADRE file.  BTNG.
*/

// This file should include all of the relevant .h files according
// to compile-time flags.  We anticipate flags to dictate:
//    Debugging level
//    Underlying communication library (e.g. PVM, MPI, MPI2)
//    Direct specification of sublibrary
// This is actually a terrible way to structure C(++) code; it will hopefully
// be fixed later.


// Logic:  NDEBUG is C(++) intrinsic--if defined, the intrinsic assert(_)
// has vacuous semantics, and PADRE_ASSERT(_) should also have vacuous semantics.
// If NDEBUG is not defined, PADRE_NDEBUG may be defined, in which case
// PADRE_ASSERT(_) should be vacuous, otherwise it should behave much like assert(_).
//
// Independently, if PADRE_DEBUG_IS_ENABLED is defined, debug code should be compiled
// with the level of debugging governed by the integer value set and queried
// with the static functions PADRE::setDebugLevel and PADRE::debugLevel();
// the nominal range is 0 (for no debugging) to 10.
//
// Finally, the class PADRE defines a public function setErrorChecking( bool )
// which if called with true should cause fairly exhaustive run-time consistency
// checking.  These checks SHOULD NOT USE PADRE_ASSERT, but should use PADRE_ABORT.
// This is to prevent the condition when NDEBUG or PADRE_NDEBUG is defined and
// setErrorChecking is on resulting in error checking with errors found not being
// reported.
//
// [IM(mkd)HO having both PADRE_ASSERT and setErrorChecking is redundant, leading
// to e.g. the complication just described.]
//
// PADRE_debugLevel's
// 


// Use PARTI distributions. For now when this is turned on KELP is turned
// so that only one or the other can be turned on
// #define USE_PARTI
// #if !defined(USE_PARTI) 
// #   define USE_KELP
// #endif


#include <PADRE_forward-declarations.h>
// // Forward references to main templated classes in PADRE
// template<class UserCollection, class UserGlobalDomain, class UserLocalDomain> 
// class PADRE_Distribution;
// 
// template<class UserCollection, class UserGlobalDomain, class UserLocalDomain> 
// class PADRE_Representation;
// 
// template<class UserCollection, class UserGlobalDomain, class UserLocalDomain> 
// class PADRE_Descriptor;


// This seems obsolete.  SERIAL_PADRE is never defined.
// Therefore, I am replacing it with a hardcoded definition
// of PARALLEL_PADRE.  Eventually, these macros should be
// removed.  BTNG.  7Mar01
// #define PARALLEL_PADRE
// The rest of this paragraph is obsolete.
// // It is not clear when SERIAL_PADRE should be defined!
// // Perhaps in other software but A++ does not define it anywhere.
// #if !defined(SERIAL_PADRE) && !defined(PARALLEL_PADRE)
// #error "Error: neither SERIAL_PADRE and PARALLEL_PADRE defined"
// #endif
// //elsif defined(SERIAL_PADRE) && defined(PARALLEL_PADRE)
// #if defined(SERIAL_PADRE) && defined(PARALLEL_PADRE)
// #error "Error: both SERIAL_PADRE and PARALLEL_PADRE defined"
// #endif
// 



// *wdh* #include <iostream.h>
#include <iostream>
using namespace std;


#include "PADRE_Global.h"

// Specific PADRE header files for sublibraries.
#if !defined(NO_Parti)
#include "PADRE_Parti.h"
#endif
#if !defined(NO_GlobalArrays)
#include "PADRE_GlobalArrays.h"
#endif


// kelp is used instead
#if defined (USE_KELP)
#include "../KELP/kelp1.2/kelp/include/kelp.h"
#include "PADRE_Kelp.h"
#include "PADRE_Kelp_Distribution.h"
#include "PADRE_Kelp_Representation.h"
#include "PADRE_Kelp_Descriptor.h"
#endif

// dummy for GlobalPointer.h
class GlobalPointer { };

#include "PADRE_CommonInterface.h"
// #include "PADRE_Communication.h"
#include "PADRE_Distribution.h"
#include "PADRE_Representation.h"
#include "PADRE_Descriptor.h"

/*
  KCC and GNU g++ want to see the source code included 
  in the header files for template instantiation
  Note: This is strictly for the benefit of P++.
  It will go away once we implement the isolator file method.  BTNG
*/
#if defined(HAVE_EXPLICIT_TEMPLATE_INSTANTIATION)

#include <PADRE_Distribution.C>
#include <PADRE_Representation.C>
#include <PADRE_Descriptor.C>

#if !defined(NO_Parti)
#include "PADRE_Parti_Template.C"
#endif

#define NO_TEMPLATE_INDEPENDENT_DATA
#define NO_TEMPLATE_INDEPENDENT_FUNCTION
#if !defined(NO_GlobalArrays)
#include "PADRE_GlobalArrays.C"
#endif

#endif

// ifndef PADRE_H
#endif





