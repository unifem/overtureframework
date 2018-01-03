
// file PADRE_Distribution.h

#ifndef PADRE_DISTRIBUTION_H
#define PADRE_DISTRIBUTION_H

/* include <PADRE_config.h> */
/* Define to enable PADRE code to use the MPI message passing interface */
#define PADRE_ENABLE_MP_INTERFACE_MPI 1

#include <PADRE_CommonInterface.h>
#include <PADRE_forward-declarations.h>

#if !defined(NO_Parti)
#include <PADRE_Parti.h>
#endif
#if !defined(NO_GlobalArrays)
#include <PADRE_GlobalArrays.h>
#endif

// STL headers.
#ifndef STL_VECTOR_HEADER_FILE
#define STL_VECTOR_HEADER_FILE <vector.h>
#endif
#include STL_VECTOR_HEADER_FILE

#ifndef NAMESPACE_IS_BROKEN
using namespace std;
#endif

// We have to template the PADRE_Distribution class on the user's
// class UserCollection which will represent his/her distributed object.
// Nothing is known about UserCollection here.

// ALSO, as a point of the desing of the implementation of PADRE we should
// likely move all the member functions that have been define in the header file
// to the source file to make the implementation cleaner (and easier to work with).

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
class PADRE_Distribution : 
public PADRE_CommonInterface {


  /*
    Define shorthand for the long types.
    Distribution	-> Distr
    Representation	-> Repre
    Descriptor		-> Descr
  */
  typedef PADRE_Representation<UserCollection,UserGlobalDomain,UserLocalDomain> Repre;
#if !defined(NO_Parti)
  typedef PARTI_Distribution<UserCollection,UserGlobalDomain,UserLocalDomain> PARTI_Distr;
  typedef PARTI_Representation<UserCollection,UserGlobalDomain,UserLocalDomain> PARTI_Repre;
  typedef PARTI_Descriptor<UserCollection,UserGlobalDomain,UserLocalDomain> PARTI_Descr;
#endif
#if !defined(NO_GlobalArrays)
  typedef GlobalArrays_Distribution<UserCollection,UserGlobalDomain,UserLocalDomain> GlobalArrays_Distr;
  typedef GlobalArrays_Representation<UserCollection,UserGlobalDomain,UserLocalDomain> GlobalArrays_Repre;
  typedef GlobalArrays_Descriptor<UserCollection,UserGlobalDomain,UserLocalDomain> GlobalArrays_Descr;
#endif

  /*
    Define STL-dependent types.
    Do this because of bug in SUN-5.0 compiler not treating
    default template parameters consistently between declaration
    and definitions.  It also organizes the code better.
  */
#ifdef SUNPRO_CC
  typedef vector<Repre*,allocator<Repre> > AssocRepre;
#else
  typedef vector<Repre*> AssocRepre;
#endif

     private:
        void initialize();

        SublibraryNames::Container subLibrariesInUse;

     // Every sublibary must have an abstract concept of Distribution
     // if this this is added to, the constructors must set these pointers to NULL
#if !defined(NO_Parti)
        PARTI_Distr *pPARTI_Distribution;
#endif
#if !defined(NO_GlobalArrays)
        GlobalArrays_Distr *pGlobalArrays_Distribution;
#endif
#if defined (USE_KELP)
        KELP_Distr *pKELP_Distribution;
#endif

     // DomainMap_Distribution                                            
     //    *pDomainMap_Distribution;
     // PGS_Distribution                                                  
     //   *pPGS_Distribution;

     // list of all Representations that have this Distribution
     // I think this is unused at the moment.
        AssocRepre associatedRepresentations;

        int Distribution_Dimension;

     // Need to make this dependent upon Distribution_Dimension (but that is not a const)
        static int Default_GhostBoundary_Width [6];

     // Make this private and make sure that only the defaultDistribution object can be initialized by it.
        PADRE_Distribution();  // default is a distribution over all processors

     // In the case of P++ this is the list of array objects which use this partition object
     // This uses the STL list container class
        list<UserCollection*> *UserCollectionList;

        static PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>* defaultDistribution;

     public:
        static PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & getDefaultDistribution();
        static PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> * getDefaultDistributionPointer();

        list<UserCollection*> *getUserCollectionList() const
           {
             PADRE_ASSERT (UserCollectionList != NULL);
             return UserCollectionList;
           }

        static list<UserCollection*> *getDefaultUserCollectionList()
           {
             return getDefaultDistribution().getUserCollectionList();
           }

     // PADRE_Distribution Constructors
     // the choice of constructor dictates the distribution library

       ~PADRE_Distribution();  
        PADRE_Distribution( const PADRE_Distribution & X );

     // Use PARTI
        PADRE_Distribution( int Number_Of_Processors );

  // Use PARTI
  PADRE_Distribution( int startingProcessor, int endingProcessor );


  PADRE_Distribution( const int* Processor_Array, int NumberOfProcessors );

  PADRE_Distribution & operator= ( const PADRE_Distribution & X );

  // We recall that the relation between P_Distributions and P_Representations is
  // one-many.  Explicitly setting the sublibrary in use of a P_Distribution has
  // the same effect on the P_Representations; however, this may be overridden on
  // a per-P_Representation basis.  If the sublibrary of a P_Distribution is changed
  // after a sublibrary is specified for a P_Representation, the sublibrary is
  // effectively _added_.
  // Implemenentation:  both P_Representation's and P_Distribution's maintain a
  // a list of sublibraries in use.  For a P_Representation the sublibraries in use
  // are the union of its list and its P_Distribution's list.

  // add a distribution library to the head of the list
  SublibraryNames::Container &setSublibraryInUse( SublibraryNames::Name X );

  // get the sublibrary from the head of the list
  const SublibraryNames::Container &getSublibraryInUse() const;

  // This is the translation mechanism and defines the sublibrary representation
  // that would be used within operations that would mix sublibrary distributions
  // -representations-
  // We need a better name for this than what we have specified below!

  SublibraryNames::Name 
  subLibraryGreatestLowerBound( const SublibraryNames::Name & X, 
				const SublibraryNames::Name & Y );

#if !defined(NO_Parti)
  PARTI_Distr *getpPARTI_DistributionPointer() const
  {
    return pPARTI_Distribution;
  }
#endif

#if !defined(NO_GlobalArrays)
  GlobalArrays_Distr * getpGlobalArrays_DistributionPointer() const
  {
    return pGlobalArrays_Distribution;
  }
#endif

#if defined (USE_KELP)
  KELP_Distr * getpKELP_DistributionPointer() const
  {
    return pKELP_Distribution;
  }
#endif

#if !defined(NO_Parti)
  PARTI_Distr & getpPARTI_Distribution() const
  {
    PADRE_ASSERT(pPARTI_Distribution != NULL);
    return *pPARTI_Distribution;
  }
#endif

#if !defined(NO_GlobalArrays)
  GlobalArrays_Distr & getpGlobalArrays_Distribution() const
  {
    PADRE_ASSERT(pGlobalArrays_Distribution != NULL);
    return *pGlobalArrays_Distribution;
  }
#endif

#if defined (USE_KELP)
  KELP_Distr & getpKELP_Distribution() const
  {
    PADRE_ASSERT(pKELP_Distribution != NULL);
    return *pKELP_Distribution;
  }
#endif

  static int getDefaultDistributionReferenceCount ()
     {
       int returnValue = -1000;
       if (defaultDistribution != NULL)
            returnValue = defaultDistribution->getReferenceCount();
       return returnValue;
     }

  // free all internal memory in use (simplifies use with Purify)

  //?? static void freeMemoryInUse() = 0;

  // These are the pure virtual functions from PADRE_CommonInterface.h

  void setGhostCellWidth ( unsigned axis, unsigned width );
  static void setDefaultGhostCellWidth ( unsigned axis, unsigned width )
     { Default_GhostBoundary_Width [axis] = width; }

  int getGhostCellWidth ( unsigned axis ) const;
  static int getDefaultGhostCellWidth ( unsigned axis )
     { 
    // This can only be called before distribution objects are setup!
    // We test this be checking that the pPARTI_Distribution is still NULL
    // this is an OK testing mechanism as long as we only use PARTI but
    // when we use other distribution libraries we will require a more 
    // robust test.
    // This test does not work because this is a static member function :-(.
    // PADRE_ASSERT(pPARTI_Distribution == NULL);
       return Default_GhostBoundary_Width [axis]; 
     }

  int getNumberOfAxesToDistribute () const;

  // void setDistributeAxis ( int Axis );
  void DistributeAlongAxis ( int Axis, bool Partition_Axis, int GhostBoundaryWidth );

  static void setDefaultProcessorRange ( int Start, int End );

  virtual /* from CommonInterface */ void getProcessorSet ( PADRE_CommonInterface::ProcessorSet &processorSet ) const;

#if 0
  void swapDistribution ( const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> 
			    & oldDistribution ,
			  const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> 
			    & newDistribution );
#endif

//static void AssociateObjectWithDefaultDistribution( const UserCollection & X );
//static void UnassociateObjectWithDefaultDistribution( const UserCollection & X );

#if 1
  static void AssociateObjectWithDefaultDistribution( const UserCollection & X );
  void AssociateObjectWithDistribution( const UserCollection & X );
  static void UnassociateObjectWithDefaultDistribution( const UserCollection & X );
  void UnassociateObjectWithDistribution( const UserCollection & X );
#endif

  void testConsistency( const char *Label = "" ) const;

  void updateGhostBoundaries();
#if 0
  void regularSectionTransfer ( const UserLocalDomain & LhsDomain,
                                const float *LhsData,
                                const UserLocalDomain & RhsDomain,
                                const float *RhsData );
#endif

   static void displayDefaultValues ( const char *Label = "" );
   void display( const char *Label = "" ) const;

   void displayReferenceCounts ( const char* label = "" ) const;

// I/O operators have to go into the header files - I suspect this is
// because the source for friend templated functions are not serached for and so
// we can make sure the that template code is instanciated properly only if we have them
// in the header file.

  friend istream & operator>> ( istream & is, PADRE_Distribution & X ) {
    cout << "istream &"
	 << " PADRE_Distribution ::"
	 << " operator>> ( istream & is, PADRE_Distribution & X )"
	 << " not implemented\n";
    return is;
  }


  friend ostream & operator<< ( ostream & os, const PADRE_Distribution & X ) {
    os << "{PADRE_Distribution<" 
      // << UserCollection::typeName() << ","
      //      << UserLocalDomain::typeName() 
       << ">" << endl;

    X.PADRE_CommonInterface::print();

    os << "Sublibraries in use:  " << X.subLibrariesInUse << endl;

    // print out the current UserCollection objects using this distribution in use
    os << "Associated representations:  [";
    int j = X.associatedRepresentations.size(); // iterator+n seems to be broken
    if (j>0) {
      // wdh* AssocRepre::const_iterator i;
      typedef typename AssocRepre::const_iterator AssocRepre_Iterator;
      AssocRepre_Iterator i;
      // *wdh*
      for (i = X.associatedRepresentations.begin(); j!=1; i++,j--)
	os << (*i)->getId() << ",";
      os << (*i)->getId();
    }
    os << "]" << endl;

// #if defined(PARALLEL_PADRE)
#if !defined(NO_Parti)
    if (X.pPARTI_Distribution != NULL) {
      X.pPARTI_Distribution->show(os);
      /* Equvalent to
	 os << *X.pPARTI_Distribution << endl;
	 but using the latter sometimes gives g++-2.91.66 problems
	 finding instantiation.  BTNG.
      */
    }
    else
      {
	// Later we delete this since it is acceptable to not have
	// a PARTI sublibrar in use if another one is in use, but for the moment
	// there can be no other sublibraries in use.
	PADRE_ASSERT(X.pPARTI_Distribution != NULL);
      }
#endif

#if !defined(NO_GlobalArrays)
    if (X.pGlobalArrays_Distribution != NULL) {
      X.pGlobalArrays_Distribution->show(os);
      /* Equvalent to
	 os << (*X.pGlobalArrays_Distribution);
	 but using the latter sometimes gives g++-2.91.66 problems
	 finding instantiation.  BTNG.
      */
      os << endl;
    }
    else
      {
	// Later we delete this since it is acceptable to not have
	// a PARTI sublibrar in use if another one is in use, but for the moment
	// there can be no other sublibraries in use.
	PADRE_ASSERT(X.pGlobalArrays_Distribution != NULL);
      }
#endif

#if defined (USE_KELP)
    if (X.pKELP_Distribution != NULL)
      os << *X.pKELP_Distribution << endl;
    else
      {
	// Later we delete this since it is acceptable to not have
	// a PARTI sublibrar in use if another one is in use, but for the moment
	// there can be no other sublibraries in use.
	PADRE_ASSERT(X.pKELP_Distribution != NULL);
      }
#endif
// #endif
    
    os << "}" << endl;
    return os;
  }

  void print() const { cout << *this; }

  static bool Has_Same_Ghost_Boundary_Widths ( const UserGlobalDomain & Lhs_Domain,
                                               const UserGlobalDomain & Rhs_Domain );
  static bool Has_Same_Ghost_Boundary_Widths ( const UserGlobalDomain & This_Domain,
                                               const UserGlobalDomain & Lhs_Domain,
                                               const UserGlobalDomain & Rhs_Domain );

   static void freeMemoryInUse();
};




// KCC and GNU g++ want to see the source code included 
// in the header files for template instantiation
// #ifdef HAVE_EXPLICIT_TEMPLATE_INSTANTIATION
// #include "PADRE_Distribution.C"
// #endif

// PADRE_DISTRIBUTION_H
#endif
