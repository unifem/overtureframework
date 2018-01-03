#ifndef PADRE_SublibraryNames_h
#define PADRE_SublibraryNames_h

/* include <PADRE_config.h> */
/* Define to enable PADRE code to use the MPI message passing interface */
#define PADRE_ENABLE_MP_INTERFACE_MPI 1

/*
  This file defines the SublibraryNames class, which helps in maintaining
  the identities of the distribution sublibraries.
*/

#define PADRE_DO_NOT_USE_SET_FOR_SUBLIBRARY_NAME_CONTAINER


// *wdh* #include <iostream.h>
// *wdh* #include <iomanip.h>

#include <iostream>
#include <iomanip>
using namespace std;

/* Make boolean syntax work even when boolean is broken. */
#ifdef BOOL_IS_BROKEN
#ifndef BOOL_IS_TYPEDEFED
#define BOOL_IS_TYPEDEFED 1
typedef int bool;
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
#endif

// STL headers.
#ifndef STL_VECTOR_HEADER_FILE
#define STL_VECTOR_HEADER_FILE <vector.h>
#endif
#ifndef STL_ALGO_HEADER_FILE
#define STL_ALGO_HEADER_FILE <algo.h>
#endif
#include STL_VECTOR_HEADER_FILE
#include STL_ALGO_HEADER_FILE
#ifndef PADRE_DO_NOT_USE_SET_FOR_SUBLIBRARY_NAME_CONTAINER
#include STL_SET_HEADER_FILE
#endif


#ifndef NAMESPACE_IS_BROKEN
using namespace std;
#endif

//! Class repesenting the name of the sublibraries.
/*!
  This class specifies unique names given to the sublibraries PADRE may use.
  These names give run-time identification of the libraries and the objects
  they support.
*/
class SublibraryNames
{
  //! Enumeration of possible sublibrary names.
  /*!
    PADRE needs a way to refer to and identify underlying distribution
    libraries.  Such identifiers are defined by this enumeration.
  */
public: enum Name
  { NONE		/*!< No library				*/
  , Trivial		/*!< A trivial library			*/
  , PARTI		/*!< Parti library from UMD		*/
  , GlobalArrays	/*!< GlobalArrays library from PNL	*/
  , KELP		/*!< KeLP library from UCSD		*/
  , DomainLayout	/*!< DomainLayout library		*/
  , PGS			/*!< PGSLIB library			*/
  , ANY			/*!< Any library			*/
};

  //! Name of one sublibrary.
  Name name;
  //! Default constructor.
  /*! Because PARTI is always available, default name is PARTI. */
  SublibraryNames() : name(PARTI) {}

  //! Insert sublibrary name into a stream.
  friend ostream & operator << (ostream & os, Name lib) {
    switch (lib) {
    case NONE:		return os << '[' << (int)lib << "]NONE";
    case Trivial:	return os << '[' << (int)lib << "]Trivial";
    case PARTI:		return os << '[' << (int)lib << "]PARTI";
    case GlobalArrays:	return os << '[' << (int)lib << "]GlobalArrays";
    case KELP:		return os << '[' << (int)lib << "]KELP";
    case DomainLayout:	return os << '[' << (int)lib << "]DomainLayout";
    case PGS:		return os << '[' << (int)lib << "]PGS";
    default:
      return os << "Error: ostream & operator << (ostream & os, Name lib)"
		<< " lib (" << (int)lib << ") not valid!";
    }
  }


  /*
    Define a sublibrary name container (STL set) with some higher-level member
    functions specifically for treating sublibrary names.
    The ideal STL container for this is set, but because the SunPRO4.2 compiler
    does not (correctly) support STL, we have to kludge together something
    that will work with it.  So far, this is STL vector.
    This temporary fix (until set works again on SunPRO) should not harm
    performance because the vector length is extremely small right now.
    I designed the classes in this file such that all changes due to the
    macro PADRE_DO_NOT_USE_SET_FOR_SUBLIBRARY_NAME_CONTAINER is contained
    only here.
    BTNG.
  */
#if !defined(PADRE_DO_NOT_USE_SET_FOR_SUBLIBRARY_NAME_CONTAINER)
#ifdef STL_SET_NEEDS_EXPLICIT_LESS
  typedef set<Name,less<Name> > SublibraryNameSTLContainer;
#else
  typedef set<Name> SublibraryNameSTLContainer;
#endif
#else
  //! A set of sublibrary names.
  /*!
    PADRE will often need to refer to a set of sublibraries.
    This type represents such a set.
  */
  typedef vector<Name> SublibraryNameSTLContainer;
#endif

  //! Class for containing a set of sublibrary names.
  /*!
    This is basically SublibraryNamesSTLContainer, a set of sublibrary
    names.  In addition, it provides higher level interfaces for PADRE
    to use the set, so PADRE does not have to make low-level set
    function calls, especially when the the actual type of the set
    varies due to configuration.

    @see SublibraryNamesSTLContainer
  */
public: class Container : public SublibraryNameSTLContainer {

#if !defined(PADRE_DO_NOT_USE_SET_FOR_SUBLIBRARY_NAME_CONTAINER)
  //! Check whether the given sublibrary name exists in the sublibrary container.
public: bool doesContain( key_type k ) const {
  return find(k) != end();
}
#else
  //! Check whether the given sublibrary name exists in the sublibrary container.
public: bool doesContain( Name k ) const {
  return find( begin(), end(), k ) != end();
}
#endif

#if !defined(PADRE_DO_NOT_USE_SET_FOR_SUBLIBRARY_NAME_CONTAINER)
  //! Insert the given sublibrary name into the sublibrary name container.
public: void insert( SublibraryNames::Container::key_type l ) {
  SublibraryNameSTLContainer::insert(l);
  // If neccessary, this function should pass on information to objects.
}
  //! Remove the given sublibrary name into the sublibrary name container.
public: void erase( SublibraryNames::Container::key_type l ) {
  SublibraryNameSTLContainer::erase(l);
  // If neccessary, this function should pass on information to objects.
}
#else
  //! Insert the given sublibrary name into the sublibrary name container.
public: void insert( SublibraryNames::Name l ) {
  SublibraryNameSTLContainer::insert(begin(), l);
  // If neccessary, this function should pass on information to objects.
}
  //! Remove the given sublibrary name into the sublibrary name container.
public: void erase( SublibraryNames::Name l ) {
  SublibraryNameSTLContainer::erase(
    (SublibraryNameSTLContainer::iterator)	/* Type cast required with
						   SunPro4.2 CC using the
						   package-provided STL */
    find( begin(), end(), l )
    );
  // If neccessary, this function should pass on information to objects.
}
#endif


  //! Insert a sublibrary name into a stream.
  friend ostream & operator << (ostream & os, const Container &lib) {
    Container::const_iterator i;
    for ( i=lib.begin(); i!=lib.end(); i++ ) {
      os << *i << ',';
    }
    // copy( lib.begin(), lib.end(), ostream_iterator<Container::value_type>(os,",") );
    return os;
  }

};	// End Container class.

};	// End SublibraryNames class.


#endif	 // PADRE_SublibraryNames_h
