// file:  PADRE_CommonInterface.h

#ifndef PADRE_CommonInterface_h
#define PADRE_CommonInterface_h

/* include <PADRE_config.h> */
/* Define to enable PADRE code to use the MPI message passing interface */
#define PADRE_ENABLE_MP_INTERFACE_MPI 1

#include <PADRE_macros.h>
#include <PADRE_SublibraryNames.h>
#include <PADRE_Global.h>

// *wdh* #include <iostream.h>
// *wdh* #include <iomanip.h>
#include <iostream>
#include <iomanip>
using namespace std;

// STL headers.
#ifndef STL_VECTOR_HEADER_FILE
#define STL_VECTOR_HEADER_FILE <vector.h>
#endif
#include STL_VECTOR_HEADER_FILE

#ifndef NAMESPACE_IS_BROKEN
using namespace std;
#endif

///////////////////////// PADRE_CommonInterface

//\begin{>>PADRE_CommonInterface.h.tex}{\subsection{Common Interface}}
class PADRE_CommonInterface
//===========================================================================
// /Purpose:
//   This class provides public interfaces across all PADRE classes.
// /Author: DQ
//\end{PADRE_CommonInterface.h.tex} 
//===========================================================================
{
private:

  static int idCount;

  // What is the purpose of this data member variable?
  static bool initialized;

  int id;                   // object assignment should not change id, equality should ignore id,
			    // identity is id.

private: void constructorInitialize();        // every constructor must call initialize()
public: static void staticInitialize();        // Initialize static members.

private: int referenceCount;



private: SublibraryNames::Container CarriedSublibrarySet;
  /*
    The CarriedSublibrarySet contains the names of the sublibraries
    whose data is being carried around by the object.  This data is
    redundant with the values of the pointers pointing to the distribution's
    version of the object.  They should agree at all times.  At this time,
    this is not implemented because I'm not sure how to coordinate it
    with the PADRE_Distribution, PADRE_Representation and PADRE_Descriptor
    classes, which keep pointers to the sublibrary-specific versions.
    BTNG
  */
public: bool sublibraryIsCarried( SublibraryNames::Name l ) const {
  return CarriedSublibrarySet.doesContain(l);
}
public: void carrySublibrary( SublibraryNames::Name l ) {
  CarriedSublibrarySet.insert(l);
  // If neccessary, this function should pass on information to objects.
}
public: void dropSublibrary( SublibraryNames::Name l ) {
  CarriedSublibrarySet.erase(l);
  // If neccessary, this function should pass on information to objects.
}




public:

  virtual ~PADRE_CommonInterface ();

  PADRE_CommonInterface ();
  PADRE_CommonInterface ( const PADRE_CommonInterface & X );

  PADRE_CommonInterface & operator= ( const PADRE_CommonInterface & X );

  friend ostream & operator<< ( ostream & os, const PADRE_CommonInterface & X )
  {
    os << "{PADRE_CommonInterface<"
      //   << UserCollection::typeName() << ","
      // << UserLocalDomain::typeName() 
       << ">"
       << " id " << X.getId()
       << " refcount: " << X.getReferenceCount()
       << "}" << endl;
    return os;
  }

  static int newId();       // returns id unique PADRE-wide

  int getId () const { return id; }     // returns the unique integer id of this object

  // adds a sublibrary to the head of the list
  static void setSublibraryInUse( SublibraryNames::Name X );

  // get the sublibrary from the head of the list
  static SublibraryNames::Name getSublibraryInUse();

  void print() const { cout << *this; }
  static void displayDefaultValues( const char *Label = "" );
  void display( const char *Label = "" ) const;

  // reference counting mechanism, value of 1 implies a single reference,
  // the self reference.  0, the initial value given by the compiler, will
  // indicate a failure of a constructor to initialize properly.

  // incrementReferenceCount and decrementReferenceCount are declared const
  // as they may operate on logically const objects

  void incrementReferenceCount () const;
  void decrementReferenceCount () const;
  int getReferenceCount () const;
  static int getReferenceCountBase ()
     { return 1; }

  // operator new and delete operators for memory pools (performance)
  // free all internal memory in use (simplifies use with Purify)
  //??    void *operator new    (size_t);
  //??    void operator  delete (void*);
  //?? static void freeMemoryInUse() = 0;
  
  
  // Set width of ghost cells
  virtual void setGhostCellWidth( unsigned axis, unsigned width ) = 0;
  virtual int getGhostCellWidth( unsigned axis ) const = 0;
  
  virtual int getNumberOfAxesToDistribute() const = 0;
  // virtual void setDistributeAxis( int Axis ) = 0;
  virtual void DistributeAlongAxis ( int Axis, bool Partition_Axis, int GhostBoundaryWidth ) = 0;
  
  // get list of processors used in distribution
  /*
    Define STL-dependent types.
    Do this because of bug in SUN-5.0 compiler not treating
    default template parameters consistently between declaration
    and definitions.  It also organizes the code better.
  */
#ifdef SUNPRO_CC
  typedef vector<int,allocator<int> > ProcessorSet;
#else
  typedef vector<int> ProcessorSet;
#endif
  virtual void getProcessorSet( ProcessorSet &processorSet ) const {
    // This is a virtual function and should be overriden, not used as is.
#if defined(PADRE_DEBUG_IS_ENABLED)
    if (PADRE::debugLevel() > 0)
      cerr << "void"
	   << " PADRE_CommonInterface ::"
	   << " getProcessorSet( vector<int> &processorSet )"
	   << " id " << getId()
	   << " not implemented"
	   << endl;
#endif
    cerr << "PADRE_CommonInterface::getProcessorSet(vector <int>&) const: not implemented!"
	 << endl;
    PADRE_ABORT();
  }
  
// This is the only reason that PADRE_CommonInterface would need to be templated!
//  Reassociate the list of associated distributed object with a new distribution
//  virtual void 
//  swapDistribution( const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> 
//  		      & oldDistribution ,
//		    const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> 
//		      & newDistribution ) = 0;
//  virtual void AssociateObjectWithDistribution   ( const UserCollection & X ) = 0;
//  virtual void UnassociateObjectWithDistribution ( const UserCollection & X ) = 0;

  // Test for internal correctness of data structures
  virtual void testConsistency( const char *Label = "" ) const = 0;

  // Update GhostBoundaries of all associated objects
  virtual void updateGhostBoundaries() = 0;



};



#endif // PADRE_CommonInterface_h
