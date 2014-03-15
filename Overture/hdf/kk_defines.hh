#ifndef __KK_DEFINES_HH__
#define __KK_DEFINES_HH__

/** \file kk_defines.hh
 *  \brief usefull stuff used by K.K. Chand
 */

#include <ctime>
#include <cstdlib>
#include <limits.h>
#include <float.h>
#include <exception>
#include <string>

/** defining KK_DEBUG on the command line activates debugging
 *  information, including range checking 
 */
#undef RANGE_CHK
#undef KK_DBG_EXEC
#ifdef KK_DEBUG

/// this macro throws an exception if \a x does not lie between \a base and \a bound
#define RANGE_CHK(x,base,bound) if ( x<base || x>bound ) throw KK::RangeErr();
#define KK_DBG_EXEC(x) x;

#else

/// optimized version of the RANGE_CHK macro, eliminates all the checks
#define RANGE_CHK(x,base,bound) 
#define KK_DBG_EXEC(x) ;

#endif

/// I like the word NULL, so make sure it exists
#ifndef NULL
const int NULL = 0;
#endif

/// some compilers do not know about explicit yet, this macro adjusts for that
#ifndef EXPLICIT
#define EXPLICIT explicit
#endif

/// some compilers do not know about the iso C99 restrict keyword
#ifndef RESTRICT
#define RESTRICT __restrict__
#endif

/// use the following macro to allow restricted this pointers (as in g++ ).
#ifndef RESTRICT_THIS
#define RESTRICT_THIS RESTRICT
#endif

/** The KK namespace contains usefull stuff used frequently by 
 *  K.K. Chand.
 */
/// usefull utility classes and functions
namespace KK {

#ifndef real
  /// change the value of this typedef to get single/double precision
  typedef double real;
#endif

#ifndef REAL_MAX
  /// maximum real value
  const real REAL_MAX = DBL_MAX;
#endif
#ifndef REAL_MIN
  /// minimum positive, nonzero real value
  const real REAL_MIN = DBL_MIN;
#endif
#ifndef REAL_EPSILON
  /// 1+REAL_EPSILON = 1 on a computer
  const real REAL_EPSILON = DBL_EPSILON;
#endif

  /// standard exception class
  class Err : public std::exception 
  {  
    
  public:
    /// default constructor, initializes the message to nothing
    Err() : msg("") { }
    /// provide a message to the error class
    Err(std::string s) : msg(s) { }

    /// override default std::exception::~exception
    virtual ~Err() throw() {}
    /// report the error string
    virtual std::string repr() const { return msg ; }
    /// direct access to the error string
    std::string msg;

  };

  /** an assertion that can be used with derivations of the Err class.
   * similar to Stroustroup "The C++ Programming Language, 3rd Ed., SS 24.3.7.2
   * \param assertion the boolean assertion 
   * \param s a string that holds information in case of failure (added to the exception)
   * note that the assertions only do something when KK_DEBUG is defined
   */
  template < class E, class B >
  inline
  void 
  Assert ( B assertion, std::string s = "" )
  {
#ifdef KK_DEBUG
    if ( ! assertion ) 
      {
	E e;
	e.msg += s;
#ifdef KK_ABORTASSERT
	abort();
#else
	throw e;
#endif
      }
#endif
  }
  
  /** an assertion that can be used with derivations of the Err class.
   * similar to Stroustroup "The C++ Programming Language, 3rd Ed., SS 24.3.7.2
   * \param assertion the boolean assertion 
   * \param exception the exception to be thrown in the case of failure.
   * note that the assertions only do something when KK_DEBUG is defined
   */
  /// simple assertion throwing a user defined exception instance
  template < class E, class B >
  inline
  void 
  Assert ( B assertion, E exception)
  {
#ifdef KK_DEBUG
#ifdef KK_ABORTASSERT
    if ( !assertion ) abort();
#else
    if ( ! assertion ) throw exception;
#endif
#endif
  }

  /** an assertion that can be used with derivations of the Err class.
   * similar to Stroustroup "The C++ Programming Language, 3rd Ed., SS 24.3.7.2
   * \param assertion the boolean assertion 
   * \param s a string that holds information in case of failure (added to the exception)
   * note that the assertions always do something, even when KK_DEBUG is not defined
   */
  template < class E, class B >
  inline
  void 
  AssertAlways ( B assertion, std::string s = "" )
  {
    if ( ! assertion ) 
      {
	E e;
	e.msg += s;
#ifdef KK_ABORTASSERT
	abort();
#else
	throw e;
#endif
      }
  }

  /** an assertion that can be used with derivations of the Err class.
   * similar to Stroustroup "The C++ Programming Language, 3rd Ed., SS 24.3.7.2
   * \param assertion the boolean assertion 
   * \param exception the exception to be thrown in the case of failure.
   * note that this assertion is ALWAYS on, even if KK_DEBUG is defined!
   */
  /// simple assertion throwing a user defined exception instance
  template < class E, class B >
  inline
  void 
  AssertAlways ( B assertion, E exception)
  {
#ifdef KK_ABORTASSERT
    if ( !assertion ) abort();
#else
    if ( ! assertion ) throw exception;
#endif
  }

  /// exception class for range errors
  class RangeErr : public Err
  {
  public:
    RangeErr( std::string s="" ) : Err(s) { }
  };

  /// get the current cpu time in seconds
  inline real getcpu() { return real(std::clock())/real(CLOCKS_PER_SEC); }

  /// get a random number in [0,1] 
  inline real getrandom() 
  { 
    return real((double(RAND_MAX)-std::rand())/double(RAND_MAX));
  }

  /// round a real to the nearest integer
  inline int round(const real r)
  { return int( r > 0. ? int(r+.5) : int(r-.5) ); }

}

#endif
