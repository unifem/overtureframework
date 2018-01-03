// file:  PADRE_Global.C

#ifndef PADRE_Global_C
#define PADRE_Global_C


#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

/* include <PADRE_config.h> */
/* Define to enable PADRE code to use the MPI message passing interface */
#define PADRE_ENABLE_MP_INTERFACE_MPI 1

#include "PADRE_Global.h"

#if !defined(NO_Parti)
#include "PADRE_Parti.h"
#endif

#if !defined(NO_GlobalArrays)
#include "PADRE_GlobalArrays.h"
#endif

#if !defined(STL_VECTOR_HEADER_FILE)
#define STL_VECTOR_HEADER_FILE <vector.h>
#endif
#include STL_VECTOR_HEADER_FILE

#if !defined(STL_STRING_IS_BROKEN)
#if !defined(STL_STRING_HEADER_FILE)
#define STL_STRING_HEADER_FILE <string.h>
#endif
#include STL_STRING_HEADER_FILE
#endif


SublibraryNames::Container PADRE::PermittedSublibrarySet;

/*
  stack<std::string> PADRE::CallStack;
  This is currently removed (to be reinstated later) because it is
  too difficult to support it with the Sun 4.2 compiler.
*/

#ifdef PADRE_ENABLE_MP_INTERFACE_MPI
int PADRE::MPIRank;
MPI_Comm PADRE::MPI_Communicator;
#endif

#if !defined(STL_STRING_IS_BROKEN)
std::string PADRE::rankString;
#else
char PADRE::rankString[1000];
#endif


/*
  Call stack functions.
*/
void PADRE::showCallStack( ostream &co ) const {
  /*
    Because stack does not support random access reads,
    it is duplicated in a temporary variable.  The temporary
    variable is popped to get get access to the whole stack.
  */
  co << "Call stack:" << endl;
  /*
  stack<std::string> temp(CallStack);
  while ( ! temp.empty() ) {
    co << CallStack.top() << '\n';
    CallStack.pop();
  }
  */
}

// ******************************************************
// Abort function used univerally within PADRE
// ******************************************************

void PADRE_Abortion_Support ( char* Source_File_With_Error, unsigned Line_Number_In_File )
   {
     // #if defined(PARALLEL_PADRE)
     cerr << "Exiting PARALLEL PADRE program from inside of "
          << Source_File_With_Error
          << " on line "
          << Line_Number_In_File
          << "\n"
          << " (Calling system abort function ...) \n";
     cerr.flush();
  // PARTI::Exit_Virtual_Machine ();
  // printf ("Exiting PADRE Virtual Machine! \n");
  // This pointer is not accessible (maybe we should make it a 
  // static pointer in one of the PADRE class?)
  // delete Communication_Manager_Pointer;
  // Communication_Manager_Pointer = NULL;
     cerr << "Virtual Machine exited!\n";
     cerr.flush();
     cout.flush();
     fflush(stdout);
     fflush(stderr);
     abort();
     // #else
          // cerr << "Exiting SERIAL_PADRE program from inside of "
               // << Source_File_With_Error
               // << " on line "
               // << Line_Number_In_File
               // << "\n"
               // << " (Calling system abort function ...) \n";
          // cerr.flush();
          // abort();
     // #endif
   }

void PADRE_Abortion_Support () {
  cerr << "PADRE aborting.\n";
  cerr.flush();
  cout.flush();
  fflush(stdout);
  fflush(stderr);
  abort();
}


// ******************************************************
// Assertion support function used univerally within PADRE (used in PADRE_ASSERT)
// ******************************************************
void PADRE_Assertion_Support ( char* Source_File_With_Error, unsigned Line_Number_In_File )
   {
// #if defined(PARALLEL_PADRE)
  cerr << "\n\nParallel Assertion failed on processor " 
       << PADRE::processNumber()
       << " of file "
       << Source_File_With_Error
       << " on line "
       << Line_Number_In_File
       << "\n";
// #else
  // cerr << "\n\nSerial Assertion failed " 
       // << "in file "
       // << Source_File_With_Error
       // << " on line "
       // << Line_Number_In_File
       // << "\n";
// #endif
  cerr.flush();
  // PADRE_ABORT();
  PADRE_Abortion_Support ();
}

bool PADRE::Initialization::initialized = false;

void PADRE::Initialization::checkInitialization() {
  if (PADRE::Initialization::initialized) {
    cerr << "PADRE::Initialization :: checkInitialization(): already initialized." << endl;
    // PADRE_ABORT();
    PADRE_Abortion_Support (__FILE__,__LINE__);
  }
  /* Permit all enabled distribution libraries as part of the initialization. */
#if !defined(NO_Parti)
  PADRE::permitSublibrary( SublibraryNames::PARTI );
#endif
#if !defined(NO_GlobalArrays)
  PADRE::permitSublibrary( SublibraryNames::GlobalArrays );
#endif
  cout << "PADRE initially permitting these distribution libraries: "
       << PermittedSublibrarySet << endl;
  PADRE::Initialization::initialized = true;
}


void PADRE::setNumberOfProcessors( int numberOfProcessors ) {
#if !defined(NO_Parti)
  Global_PARTI_PADRE_Interface_Number_Of_Processors = numberOfProcessors;
#endif
}




#if defined(PADRE_ENABLE_MP_INTERFACE_MPI)
#if 0
void PADRE::Initialization::initialize( void (*mpiComm)(MPI_Comm *), int numberOfProcessors ) {
  // We need to explain why this function takes a pointer to a function as a parameter
     checkInitialization();
     PADRE_Communication :: setNumberOfProcessors (numberOfProcessors );
     PADRE_Communication :: setIntercommunicatorFun( mpiComm );
     // Set PADRE MPI communicator.
     mpiComm( &PADRE::MPICommunicator );
#if !defined(NO_Parti)
     // Set PARTI MPI communicator.
     mpiComm( &Global_PARTI_PADRE_Interface_PADRE_Comm_World );
#endif
     MPI_Comm_rank( PADRE::MPICommunicator, &PADRE::MPIRank );
     {	// Initialize rankString, (the string representing MPIRank).
#if !defined(STL_STRING_IS_BROKEN)
       char tstr[1000];
       sprintf( tstr, "[%d]", PADRE::MPIRank );
       rankString = tstr;
#else
       sprintf( rankString, "[%d]", PADRE::MPIRank );
#endif
     }
     /*
       Call the static initialization functions of the sublibrary classes.
       Start out permitting all enabled distribution libraries.
       The user can prohibit these manually afterwards.
     */
#if !defined(NO_Parti)
     PARTI::SublibraryInitialization ();
     PADRE::permitSublibrary( SublibraryNames::PARTI );
#endif
#if !defined(NO_GlobalArrays)
     GlobalArrays::SublibraryInitialization ();
     PADRE::permitSublibrary( SublibraryNames::GlobalArrays );
#endif
#if defined(USE_KELP)
     PADRE::permitSublibrary( SublibraryNames::KELP );
#endif
#if defined(USE_PGSLIB)
     PADRE::permitSublibrary( SublibraryNames::PGSLIB );
#endif
   }
#else
void PADRE::Initialization::initialize( MPI_Comm specifiedMPIComm, int numberOfProcessors ) {
  // We need to explain why this function takes a pointer to a function as a parameter
     checkInitialization();
     PADRE :: setNumberOfProcessors (numberOfProcessors );
     // Set PADRE MPI communicator.
     MPI_Comm_dup( specifiedMPIComm, &PADRE::MPI_Communicator );
     MPI_Comm_rank( PADRE::MPI_Communicator, &PADRE::MPIRank );
     {	// Initialize rankString, (the string representing MPIRank).
#if !defined(STL_STRING_IS_BROKEN)
       char tstr[1000];
       sprintf( tstr, "[%d]", PADRE::MPIRank );
       rankString = tstr;
#else
       sprintf( rankString, "[%d]", PADRE::MPIRank );
#endif
     }
     /*
       Call the static initialization functions of the sublibrary classes.
       Start out permitting all enabled distribution libraries.
       The user can prohibit these manually afterwards.
     */
#if !defined(NO_Parti)
     PARTI::SublibraryInitialization ();
     PADRE::permitSublibrary( SublibraryNames::PARTI );
#endif
#if !defined(NO_GlobalArrays)
     GlobalArrays::SublibraryInitialization ();
     PADRE::permitSublibrary( SublibraryNames::GlobalArrays );
#endif
#if defined(USE_KELP)
     PADRE::permitSublibrary( SublibraryNames::KELP );
#endif
#if defined(USE_PGSLIB)
     PADRE::permitSublibrary( SublibraryNames::PGSLIB );
#endif
   }
#endif
#elif defined(PVM)
void PADRE::Initialization::initialize( int dummySomethings,
	    int numberOfProcessors ) {
  checkInitialization();
  cerr << "PADRE_Global.h, PADRE::Initialization::initialize(): "
       << "PVM not supported." << endl;
  // PADRE_ABORT();
  PADRE_Abortion_Support (__FILE__,__LINE__);
}
#else // no comm lib specified
void PADRE::Initialization::initialize() {
  cerr << "PADRE_Global.h, PADRE::Initialization::initialize(): "
       << "no comm lib #defined" << endl;
  // PADRE_ABORT();
  PADRE_Abortion_Support (__FILE__,__LINE__);
}
#endif

  /*! Determines whether ghost boundary is supported.  This depends on
    whether it is supported by the sublibraries currently permitted.
    The following sublibraries do not support ghost boundaries:
    GlobalArrays.
  */
bool PADRE::isGhostBoundarySupported(void) {
  if ( isSublibraryPermitted( SublibraryNames::GlobalArrays ) )  return false;
  return true;
}
  /*! Determines whether type float is supported.  This depends on
    whether it is supported by the sublibraries currently permitted.
    The following sublibraries do not support type float:
    GlobalArrays.
  */
bool PADRE::isFloatTypeSupported(void) {
  if ( isSublibraryPermitted( SublibraryNames::GlobalArrays ) )  return false;
  return true;
}

int PADRE::runTimeDebugLevel = 0;
bool debugAllProcessors = true;
bool PADRE::isErrorChecking = false;



#endif	// PADRE_Global_C
