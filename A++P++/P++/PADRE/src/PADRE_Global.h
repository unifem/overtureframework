// file:  PADRE_Global.h

#ifndef PADRE_Global_H
#define PADRE_Global_H


/* include <PADRE_config.h> */
/* Define to enable PADRE code to use the MPI message passing interface */
#define PADRE_ENABLE_MP_INTERFACE_MPI 1

#include <PADRE_SublibraryNames.h>
#include <PADRE_macros.h>
// *wdh* #include <iostream.h>
#include <iostream>
using namespace std;
#if !defined(NO_Parti)
#include <PADRE_Parti.h>
#ifndef PARTI_MAX_ARRAY_DIMENSION
#error "no PARTI_MAX_ARRAY_DIMENSION"
#endif
#endif

// STL headers.
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

#ifdef PADRE_ENABLE_MP_INTERFACE_MPI
#include <mpi.h>
#endif

void PADRE_Assertion_Support( char* Source_File_With_Error, 
			      unsigned Line_Number_In_File );

void PADRE_Assertion_SupportM( char* message,
			       char* Source_File_With_Error, 
			       unsigned Line_Number_In_File );

void PADRE_Abortion_Support( char* Source_File_With_Error, 
			     unsigned Line_Number_In_File );

void PADRE_Abortion_Support();

#ifndef NAMESPACE_IS_BROKEN
using namespace std;
#endif



class PADRE {
  /*
    PADRE and PADRE::Initialization
    probably should not be classes but namespaces
    because there is no non-static member.  I will leave
    it as a class for now because namespace is not
    supported on some platforms.  BTNG.
   */

public: class Initialization {
private:
  static bool initialized;
  static void checkInitialization();

public:
 ~Initialization ();
  Initialization ();
  Initialization ( const Initialization & X );
  Initialization operator= ( const Initialization & X );

  // this is lame; just an indication that a solution is needed
#if defined(PADRE_ENABLE_MP_INTERFACE_MPI)
public: static void initialize( MPI_Comm specifedMPIComm, int numberOfProcessors );
#elif defined(PADRE_ENABLE_MP_INTERFACE_PVM)
public: static void initialize( int dummySomethings, int numberOfProcessors );
#else // no comm lib specified
public: static void initialize();
#endif

}; // class Initialization


static void setNumberOfProcessors( int numberOfProcessors );

  friend class Initialization;

private: static int runTimeDebugLevel;
private: static bool debugAllProcessors;
private: static bool isErrorChecking;
#if !defined(STL_STRING_IS_BROKEN)
private: static string rankString;
#else
private: static char rankString[1000];
#endif
  /*
    rankString is a string representing the rank,
    to be used in prints, etc.
    It makes more sense to make rankString an STL string type,
    but due to problems with STL with the SunPRO4.2 compiler,
    it is a vector.
  */

public:
 ~PADRE ();
  PADRE() {
    PADRE_Abortion_Support (__FILE__,__LINE__); // testing.
  }

#ifdef PADRE_ENABLE_MP_INTERFACE_MPI
private: static MPI_Comm MPI_Communicator;
private: static int MPIRank;
public: static MPI_Comm MPICommunicator () {
  return MPI_Communicator;
}
#endif
  //! Return the process number.
public: static int processNumber () {
#ifdef PADRE_ENABLE_MP_INTERFACE_MPI
  return MPIRank;
#else
  PADRE_Abortion_Support(__FILE__,__LINE__);	// At least one message passing library must be used.
  return 0; // required for compiler (this line is not reached)
#endif
}
  /*!
    Because processNumber() is used frequently and it is a long name,
    provide the shorthand, PN.
  */
public: static int PN () {
#ifdef PADRE_ENABLE_MP_INTERFACE_MPI
  return MPIRank;
#else
  PADRE_Abortion_Support(__FILE__,__LINE__);	// At least one message passing library must be used.
// required for compiler (this line is not reached)
  return 0;
#endif
}
  //! RS (rank string) is a shorthand for writing out the rank.
public: static const char *RS () {
#if !defined(STL_STRING_IS_BROKEN)
  return rankString.c_str();
#else
  return rankString;
#endif
}

  //! Set the run-time debug level.
  /*!
    Set the run-time debug level.  0 <= level <= 10.
  */
public: static void setDebugLevel( int level ) {
    if (level < 0 || level > 10) {
      cerr << "PADRE::setDebugLevel called with illegal level " << level << endl;
      PADRE_Abortion_Support(__FILE__,__LINE__);
    }
    runTimeDebugLevel = level;
  }

  //! Returns the run-time debug level.
public: static int debugLevel() { return runTimeDebugLevel; }

  //! Turn the error checking flag on and off.
public: static void setErrorChecking( bool on ) { isErrorChecking = on; }

  //! Return the error checking flag.
public: static bool errorChecking() { return isErrorChecking; }

  //! I do not know what this does.  BTNG.
public: static void setFortranOrderingFlag( int f ) {
#if !defined(NO_Parti)
    PARTI::setFortranOrderingFlag(f);
#endif
  }

  //! I do not know what this does.  BTNG.
public: static void cleanUpAfterPadre() {
#if !defined(NO_Parti)
  // Cleanup memory used withing PARTI (special parti function written by Dan Quinlan)
  cleanup_after_PARTI();
#endif
  }

  /*
    static stack<std::string> CallStack;
    This is currently removed (to be reinstated later) because it is
    too difficult to support it with the Sun 4.2 compiler.
   */
  /*
    The call stack is for debugging purposes.
    Functions should push their names onto the call stack upon entering
    and pup the name just before exciting.
    CallStack uses a vector base instead of the (default) deque because
    I ran into problems with deque on the SGI.  BTNG.
  */
public: void showCallStack( ostream &co ) const;

  //! Container for the sublibraries currently permitted (at run time).
  /*!
    The PermittedSublibrarySet contains the names of all the sublibraries that
    the user wants to permit at run time.  This is a subset of all the
    libraries that have been enabled when configuring PADRE.  Sublibraries
    used by individual objects will a subset of PermittedSublibrarySet.
    BTNG

    Here is the convention on whether a sublibrary is enabled, permitted
      and carried:
    Enable/Disable describes whether code is enabled at compile time.
    Permit/Prohibit describes whether the user wants the sublibrary
      used at run time.  The PermittedSublibrarySet is a static member.
    Carry/Drop describes whether individual objects carry around the
      the sublibrary's version of the data.
    A sublibrary must be enabled to be permitted.  It must be permitted
      to be carried.  This is per object member data.
    BTNG
   */
private: static SublibraryNames::Container PermittedSublibrarySet;
  //! Return the set of sublibraries being permitted at run time.
public: static const SublibraryNames::Container &permittedSubLibraries() {
  return PermittedSublibrarySet;
}
  //! Check whether a sublibrary is currently permitted.
public: static bool isSublibraryPermitted( SublibraryNames::Name l ) {
  return PermittedSublibrarySet.doesContain(l);
}
  //! Order that the given sublibrary is permitted.
public: static void permitSublibrary( SublibraryNames::Name l ) {
  PermittedSublibrarySet.insert(l);
  // If neccessary, this function should pass on information to objects.
}
  //! Order that the given sublibrary is prohibitted (not permitted).
public: static void prohibitSublibrary( SublibraryNames::Name l ) {
  PermittedSublibrarySet.erase(l);
  // If neccessary, this function should pass on information to objects.
}


  /*
    The following are interfaces to ask general questions about the
    capabilities of PADRE.  These capabilities vary depending on what
    sublibraries are permitted, so the return values must be determined
    at run time.
  */
  /*! Determines whether ghost boundary is supported. */
public: static bool isGhostBoundarySupported(void);
  /*! Determines whether type float is supported. */
public: static bool isFloatTypeSupported(void);


}; // class PADRE


#endif	// PADRE_Global_H
