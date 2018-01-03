// file:  PADRE_Representation.h

#ifndef PADRE_REPRESENTATION_H
#define PADRE_REPRESENTATION_H

/* include <PADRE_config.h> */
/* Define to enable PADRE code to use the MPI message passing interface */
#define PADRE_ENABLE_MP_INTERFACE_MPI 1

#include <PADRE_Distribution.h>

#ifndef NAMESPACE_IS_BROKEN
using namespace std;
#endif

// the template class argument UserCollection (aka `distributed data set'):
//   e.g. in P++ it might be an intarray or realarray (or the base class for the two of them)
//   since this can be only one class these must have a common base class
//
// the template class argument UserLocalDomain:
//    has a specific interface that tells what a UserCollection `looks like';
//    for now, indexing information--base, bound, stride (so implicitly
//    dimensionality, size of each dimension) via access functions
// the interator should proved a mechanism for accessing each of the
//    UserLocalDomain objects over all processors


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
class PADRE_Representation : 
public PADRE_CommonInterface
{

  /*
    Define shorthand for the long types.
    Distribution	-> Distr
    Representation	-> Repre
    Descriptor		-> Descr
  */
public:
  typedef PADRE_Descriptor<UserCollection,UserGlobalDomain,UserLocalDomain> Descr;
  typedef PADRE_Distribution<UserCollection,UserGlobalDomain,UserLocalDomain> Distr;
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


public:
  SublibraryNames::Container subLibrariesInUse;

// This domain object is translation invariant!  This is also the only class
// which contains an instance of this type (rather than a pointer to an instance)
// The lower level PARTI_Descriptor has a reference to this domain (since it has
// a pointer the the representation level object) along with the
// translation information for the translation specific information.  Member functions of
// the PARTI_Descriptor or the PADRE_Descriptor handle access to the translation specific information.
// Thus the descriptor level of the interface must have a common interface with the
// array domain object (a base class for the user to use in the derivation of the domain
// object would simplify the process later on).

// To permit any global domain to be built and used we make this a pointer and initialize it
// in the constructors.
  UserLocalDomain *globalDomain;

  // We need the distribution library specific classes
#if !defined(NO_Parti)
  PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> 
     *pPARTI_Representation;
#endif
#if !defined(NO_GlobalArrays)
  GlobalArrays_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> 
     *pGlobalArrays_Representation;
#endif
#if defined (USE_KELP)
  KELP_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> 
     *pKELP_Representation;
#endif

//??  DomainMap_Representation                                          
//     *pDomainLayout_Representation;
//??  PGSLib_Representation                                             
//     *pPGSLib_Representation;


  // I think we really need this and I'm confused why it was not already here!
  // But we also need a reference to the PADRE_Distribution object that is in use
  // This has to be a dynamic link (so that the association can be broken) so we
  // need a dynamic reference (i.e. a pointer rather than a C++ reference)
  Distr *distribution;

  // list of all UserCollection objects that share this representation
  list<UserCollection*> associatedUserCollections;

private:
  PADRE_Representation ( const PADRE_Representation & X );
  PADRE_Representation & operator= ( const PADRE_Representation & X );

public:
 ~PADRE_Representation();  
  PADRE_Representation();  // default is ??

  // These can take UserLocalDomain as a const & since we build a translation independent UserLocalDomain
  // internally (i.e we will not be referencing it so we don't have to pass it as a pointer)
  // If we just pass in a UserLocalDomain object then we assume that the defaults in the PADRE_Distribution
  // object will be used (else we should have specified a specific distribution object to guide the 
  // partitioning onto the multiprocessor environment.
  PADRE_Representation( const UserLocalDomain *inputGlobalDomain );

  // Here we have specified a UserLocalDomain object (required) and a PADRE_Distribution 
  // (optional, see previous constructor).  The input of a PADRE_Distribution object 
  // means that we want to use a specific PADRE_Distribution to control the distribution.
  PADRE_Representation( const UserLocalDomain *inputGlobalDomain,
		        const Distr *distribution );

  class iterator
  {
    // ??
  };

  // Specific of each library for the representation of the distribution of data
  // Member Data
  
  // Iterators for each processor's representation.  We have to hide the 
  // tabel based
  // vs. algorithm based distribution mechanisms here so it is not clear 
  // that iterators
  // should be here sense they tend to expose the interface details in terms 
  // of the elements.

  // iterator begin();
  // iterator end();

  Distr *getPADRE_Distribution()
     {
       return distribution;
     }

  UserLocalDomain & getGlobalDomain() const;
  UserLocalDomain * getGlobalDomainPointer() const;

  // These all come from PADRE_CommonInterface.h

  // add a distribution library to the head of the list
  SublibraryNames::Container &setSublibraryInUse( SublibraryNames::Name X );

  // get the sublibrary from the head of the list
  const SublibraryNames::Container &getSublibraryInUse() const;


  SublibraryNames::Name 
  subLibraryGreatestLowerBound( const SublibraryNames::Name & X, 
				const SublibraryNames::Name & Y );

  void getLocalSizes ( int* LocalSizeArray );

  void setGhostCellWidth( unsigned axis, unsigned width );

  int getGhostCellWidth( unsigned axis ) const;

  int getNumberOfAxesToDistribute() const;

  // void setDistributeAxis( int Axis );
  void DistributeAlongAxis ( int Axis, bool Partition_Axis, int GhostBoundaryWidth );

  // Determine the processor number for an array element defined by the indices indexVals.
  int findProcNum ( int* indexVals ) const;

  // void getProcessorSet(int *ProcessorArray, int & Number_Of_Processors) const;

  void swapDistribution( 
    const Distr & oldDistribution ,
    const Distr & newDistribution );

  void AssociateObjectWithDistribution( const UserCollection & X );

  void UnassociateObjectWithDistribution( const UserCollection & X );

  void testConsistency( const char *Label = "" ) const;
  static void displayDefaultValues( const char *Label = "" );
  void display( const char *Label = "" ) const;

  void displayReferenceCounts ( const char* label = "" ) const;

  void updateGhostBoundaries();


  // I/O operators have to go into the header files - I suspect this is
  // because the source for friend templated functions are not serached for and so
  // we can make sure the that template code is instanciated properly only if we have them
  // in the header file. ???

  friend ostream & operator<< ( ostream & os,
				const PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> & X )
  {
    os << "{PADRE_Represention<"
      // << UserCollection::typeName() << ","
      //      << UserLocalDomain::typeName()
       << ">" << endl;

    X.PADRE_CommonInterface::print();

    os << "Sublibraries in use:  ";
    os << X.subLibrariesInUse;
    os << endl;

    os << "(The Global Domain (Translation independent):\n";
    X.globalDomain->display("PADRE_Representation::operator<<");

    return os;
  }

friend istream & operator>> (istream & Input_PADRE_Stream, 
                             PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> & X)
   {
     cout << "PADRE_Representation::istream & operator>> not implemented\n";
     return Input_PADRE_Stream; 
   }

#if !defined(NO_Parti)
PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> *getPARTI_Representation () const
   {
  // PADRE_ASSERT (pPARTI_Representation != NULL);
     return pPARTI_Representation;
   }
#endif

#if !defined(NO_GlobalArrays)
GlobalArrays_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> *getGlobalArrays_Representation () const
   {
  // PADRE_ASSERT (pGlobalArrays_Representation != NULL);
     return pGlobalArrays_Representation;
   }
#endif

#if defined (USE_KELP)
KELP_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> *getKELP_Representation () const
   {
  // PADRE_ASSERT (pKELP_Representation != NULL);
     return pKELP_Representation;
   }
#endif

   static void freeMemoryInUse();

// private:

  void initialize();
};


// KCC and GNU g++ want to see the source code included 
// in the header files for template instantiation
// #ifdef HAVE_EXPLICIT_TEMPLATE_INSTANTIATION
// #include "PADRE_Representation.C"
// #endif

// PADRE_REPRESENTATION_H
#endif






