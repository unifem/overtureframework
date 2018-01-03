// file: PADRE_macros.h

#ifndef PADRE_macros_h
#define PADRE_macros_h 1


/*
  The following declarations are from PADRE_Global.h,
  but do not include PADRE_Global.h here because it
  leads to cicular inclusions.
  BTNG.
*/
void PADRE_Assertion_Support( char* Source_File_With_Error, 
			      unsigned Line_Number_In_File );
void PADRE_Assertion_SupportM( char* message,
			       char* Source_File_With_Error, 
			       unsigned Line_Number_In_File );
void PADRE_Abortion_Support( char* Source_File_With_Error, 
			     unsigned Line_Number_In_File );





#if defined(PADRE_NDEBUG) || defined(NDEBUG)
// Define a version of assert for PADRE (case of NDEBUG defined)

#define PADRE_ASSERT(f) assert(f)
#else
  // Define a version of assert for PADRE (case of NDEBUG NOT defined)
  // PADRE requires it's own version of assert since special termination
  // procedures are required for the parallel environment.
#define PADRE_ASSERT(f) \
     if(f)            \
          (void) NULL;\
     else             \
          PADRE_Assertion_Support (__FILE__,__LINE__)
#endif

#define PADRE_ABORT() PADRE_Abortion_Support (__FILE__,__LINE__)


#define PADRE_MAX_ARRAY_DIMENSION 6

// Previously this limit was set at 128
// Within P++ this is set in the A++_headers.h file, but 
// within an arbitrary use of PADRE it might not be set
#ifndef MAX_PROCESSORS
/* #define MAX_PROCESSORS 1024 */
/* use new include file to specify the number of processors. wdh and jwb 100924. */
#include "../include/maxProcessors.h"
#endif
#ifndef MAX_PROCESSORS
#error "MAX_PROCESSORS is not defined"
#endif
#define MAIN_PROCESSOR_GROUP_NAME "MainProcessorGroup"


// ifndef PADRE_macros_h
#endif





