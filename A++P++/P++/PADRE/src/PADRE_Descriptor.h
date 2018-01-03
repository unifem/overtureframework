// file:  PADRE_Descriptor.h


#ifndef PADRE_Descriptor_h
#define PADRE_Descriptor_h

/* include <PADRE_config.h> */
/* Define to enable PADRE code to use the MPI message passing interface */
#define PADRE_ENABLE_MP_INTERFACE_MPI 1

#include <PADRE_Representation.h>

#if !defined(NO_Parti)
#include <PADRE_Parti.h>
#endif

#if !defined(NO_GlobalArrays)
#include <PADRE_GlobalArrays.h>
#endif

#if !defined(NO_Kelp)
#include <PADRE_Kelp.h>
#endif


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
class PADRE_Descriptor : 
public PADRE_CommonInterface
{

  /*
    Define shorthand for the long types.
    Distribution	-> Distr
    Representation	-> Repre
    Descriptor		-> Descr
  */
public:
  typedef PADRE_Representation<UserCollection,UserGlobalDomain,UserLocalDomain> Repre;
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
#if !defined(NO_Kelp)
  typedef KELP_Distribution<UserCollection,UserGlobalDomain,UserLocalDomain> Kelp_Distr;
  typedef KELP_Representation<UserCollection,UserGlobalDomain,UserLocalDomain> Kelp_Repre;
  typedef KELP_Descriptor<UserCollection,UserGlobalDomain,UserLocalDomain> Kelp_Descr;
#endif


  /*
    Create a function, which does nothing when called, but forces the members
    of this class to be instantiated at compile time.
  */
public: static void instantiateMe() {
  PADRE_Descriptor x;
  int i;
  if ( &x == NULL /* never true */ ) {
    PADRE_ABORT();	// Precautionary measure to make sure this if block is not executed.
    freeMemoryInUse();
     x.allocateData(i, (double**)NULL);
     x.allocateData(i, (float**)NULL);
     x.allocateData(i, (int**)NULL);
  }
}


public:

// We want the PADRE_Representation objects to be translation independent so
// we have to store the specific domain in the PADRE_Descriptor object
// this allows multiple PADRE_Descriptor objects to use the same 
// PADRE_Representation object with different bases for the data

  UserLocalDomain *globalDomain;
  /* Yes, globalDomain IS of type UserLocalDomain.  Not an error!
     Because the padre representation's domain is translation-independent,
     we have to store a translation-dependent version locally here in the
     padre descriptor.
  */
  UserLocalDomain *localDomain;

  // pointer to parent PADRE_Representation
  Repre *representation;

  // Pointer to the local descriptor which would contain a pointer to the data 
  // many details of the local descriptor are accessible through access functions

  UserGlobalDomain *GlobalDescriptor; // This is the P++ parallel domain.

  // This is the base used by default for all array objects.  It could be in the localDescriptor that might be better!
  static int PADRE_Global_Array_Base;
  
  // Null Descriptor Constructor (no data, default distribution, 
  // representation pointer is NULL)

private:
  PADRE_Descriptor ( const PADRE_Descriptor & X );
  PADRE_Descriptor & operator= ( const PADRE_Descriptor & X );

  // Null Descriptor Constructor (i.e. no data, but a specific distribution, 
  // representation pointer is NULL)

public:
 ~PADRE_Descriptor();
  PADRE_Descriptor();

// For now we make these private so we can test PADRE with P++!
private:
#if 1
  PADRE_Descriptor ( const Distr *User_Distribution );

  // Null Descriptor Constructor (i.e. no data, but a specific distribution and 
  // representation) This might not make so much sense because no data is 
  // associated with the representation.

  PADRE_Descriptor
     ( const UserLocalDomain *inputGlobalDomain,
       const Repre *inputRepresentation );

  // Valid Descriptor Constructor with specific size of data and distribution

  PADRE_Descriptor
     ( const UserLocalDomain *inputGlobalDomain,
       const Distr *User_Distribution );

  // Valid Constructor (builds descriptor for specific size of data and using 
  // default distribution) 
  /* I think this function is not used.  BTNG.
  PADRE_Descriptor( const UserLocalDomain *inputGlobalDomain );
  */
#endif

public:
  // This is the constructor called in P++ (in the Array_Domain_Type class)
     PADRE_Descriptor( const UserGlobalDomain *inputLocalDescriptor );
     PADRE_Descriptor
	( const UserGlobalDomain *inputLocalDescriptor,
          const Distr * inputDistribution );

  // These constructors are used in the implementation of the P++ specific constructor
     PADRE_Descriptor ( 
          const UserLocalDomain *inputGlobalDomain,
          const UserLocalDomain *inputLocalDomain,
          const UserGlobalDomain *inputLocalDescriptor );
     PADRE_Descriptor (
	  const UserLocalDomain *inputGlobalDomain,
          const UserLocalDomain *inputLocalDomain,
          const Repre *inputRepresentation,
          const UserGlobalDomain *inputLocalDescriptor );
     PADRE_Descriptor (
		       const PADRE_Descriptor *inputPADRE_Descriptor,
	  const UserLocalDomain *inputGlobalDomain,
          const UserLocalDomain *inputLocalDomain,
          const Repre *inputRepresentation,
          const UserGlobalDomain *inputLocalDescriptor );

     void intializeConstructor (
          const UserGlobalDomain *inputLocalDescriptor );
     void intializeConstructor (
          const UserGlobalDomain *inputLocalDescriptor,
          const Distr * inputDistribution );
     void intializeConstructor (
          const UserLocalDomain *inputGlobalDomain,
          const UserLocalDomain *inputLocalDomain,
          const UserGlobalDomain *inputLocalDescriptor );
     void intializeConstructor (
          const UserLocalDomain *inputGlobalDomain,
          const UserLocalDomain *inputLocalDomain,
          const Repre *inputRepresentation,
          const UserGlobalDomain *inputLocalDescriptor );
     void intializeConstructor (
		       const PADRE_Descriptor *inputPADRE_Descriptor,
          const UserLocalDomain *inputGlobalDomain,
          const UserLocalDomain *inputLocalDomain,
          const Repre *inputRepresentation,
          const UserGlobalDomain *inputLocalDescriptor );

  // These member functions will return a pointer to the data if the
  // distribution library needs to allocate the memory itsel.

     void allocateData (int& preallocated_data, double** local_data);
     void allocateData (int& preallocated_data, float**  local_data);
     void allocateData (int& preallocated_data, int**    local_data);

  // Member Functions (Put interface here, as it is defined)
  /* BTNG: This seems obsolete:
  const UserGlobalDomain*  getUserLocalDomainPointer() const 
     { return GlobalDescriptor; }
  const UserGlobalDomain & getUserLocalDomain()        const 
     { return *GlobalDescriptor; }
  */
//PADRE_CommunicationSchedule buildCommunicationSchedule
//   ( const PADRE_Descriptor & X );

  // I/O operators have to go into the header files - I suspect this is
  // because the source for friend templated functions are not searched for and so
  // we can make sure the that template code is instanciated properly only if we 
  // have them // in the header file.


  // These are the pure virtual functions from PADRE_CommonInterface.h
  Distr *getPADRE_Distribution() {
       PADRE_ASSERT (representation != NULL);
       return representation->getPADRE_Distribution();
     }

  Repre *getPADRE_Representation() {
       PADRE_ASSERT (representation != NULL);
       return representation;
     }

  UserLocalDomain *getLocalDomain()
     { return localDomain; }

  UserLocalDomain *getGlobalDomain()
     { return globalDomain; }

  void setLocalDomain( UserLocalDomain *inputLocalDomain );

  /* BTNG: This seems obsolete:
  UserGlobalDomain *getLocalDescriptor()
     { return GlobalDescriptor; }
  */

  void displayReferenceCounts ( const char* label = "" ) const;

// These are used to initialize the PADRE_Descriptor objects after they are built
// this is required because often the data is unavailable at the point when the
// PADRE_Descriptor is constructed.  This is the case in P++ which uses
// references to P++ objects for within PADRE (this make the implementation 
// as efficient as possible).
  void setLocalDescriptor( UserGlobalDomain *inputLocalDescriptor );
  void getLocalSizes ( int* LocalSizeArray );

  void setGhostCellWidth( unsigned axis, unsigned width );

  int getGhostCellWidth( unsigned axis ) const;

  int getNumberOfAxesToDistribute() const;

  // void setDistributeAxis( int Axis );
  void DistributeAlongAxis 
     ( int Axis, bool Partition_Axis, int GhostBoundaryWidth );

  // void getProcessorSet(int *ProcessorArray, int & Number_Of_Processors) const;

  static void swapDistribution( 
    const Distr & oldDistribution ,
    const Distr & newDistribution );

  void AssociateObjectWithDistribution( const UserCollection & X );
  void AssociateObjectWithDefaultDistribution( const UserCollection & X );
  void UnassociateObjectWithDistribution( const UserCollection & X );
  void UnassociateObjectWithDefaultDistribution( const UserCollection & X );

  void testConsistency( const char *Label = "" ) const;
  static void displayDefaultValues( const char *Label = "" );
  void display( const char *Label = "" ) const;

  void updateGhostBoundaries();

  // void initializeDistributionLibrary ();
    // ( UserLocalDomain & globalDomain,
    //   const PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> 
    //   & representation );

     void InitializeLocalDescriptor();
     void InitializeLocalDescriptor ( UserGlobalDomain & inputLocalDescriptor, 
                                      UserLocalDomain & inputLocalDomain );
  // void InitializeLocalDescriptor
  //      ( UserLocalDomain & globalDomain,
  //        const PADRE_Representation
  //        <UserCollection, UserGlobalDomain, UserLocalDomain> & representation,
  //        UserGlobalDomain & localDescriptor );

#if 0
  void InitializeLocalDescriptor
       ( UserLocalDomain & globalDomain,
         const Repre & representation, UserGlobalDomain & localDescriptor );
#endif

// Later we want to make this a parameterized type in place of double float and int!
   static void transferData
       ( const UserGlobalDomain & receiveDomain,
         const UserGlobalDomain & sendDomain,
         const double *sourceDataPointer,
         const double *destinationDataPointer );

   static void transferData
       ( const UserGlobalDomain & receiveDomain,
         const UserGlobalDomain & sendDomain,
         const float *sourceDataPointer,
         const float *destinationDataPointer );

   static void transferData
       ( const UserGlobalDomain & receiveDomain,
         const UserGlobalDomain & sendDomain,
         const int *sourceDataPointer,
         const int *destinationDataPointer );

#if 0
   static void transferData
       ( const XArray4<Grid4<double>>& sendArray,
         XArray4<Grid4<double>>& receiveArray);

   static void transferData
       ( const XArray4<Grid4<float>>& sendArray,
         XArray4<Grid4<float>>& receiveArray);

   static void transferData
       ( const XArray4<Grid4<int>>& sendArray,
         XArray4<Grid4<int>>& receiveArray);

   static void transferData
       ( const 
	 Kelp_Descr & sendDescriptor,
	 Kelp_Descr & receiveDescriptor);
#endif

/* The functions is...Partition should probably return map of booleans
   because there may be more than one active.  But for now, this is
   quick and dirty contraption just returns the first one active.  BTNG.
*/
   bool isLeftPartition   (int axis) const
      { 
        PADRE_ASSERT (representation != NULL);

#if !defined(NO_Parti)
	if (pPARTI_Descriptor != NULL)
	  return pPARTI_Descriptor->isLeftPartition(axis);
#endif
#if !defined(NO_GlobalArrays)
	if (pGlobalArrays_Descriptor != NULL)
          return pGlobalArrays_Descriptor->representation->isLeftPartition(axis);
#endif
#if !defined(NO_Kelp)
	if (pKELP_Descriptor != NULL)
          return pKELP_Descriptor->isLeftPartition(axis);
#endif
	PADRE_ABORT();	// No distribution sublibrary specified.
      }

   bool isMiddlePartition (int axis) const
      { 
        PADRE_ASSERT (representation != NULL);

#if !defined(NO_Parti)
	if (pPARTI_Descriptor != NULL)
	  return pPARTI_Descriptor->isMiddlePartition(axis);
#endif
#if !defined(NO_GlobalArrays)
	if (pGlobalArrays_Descriptor != NULL)
          return pGlobalArrays_Descriptor->representation->isMiddlePartition(axis);
#endif
#if !defined(NO_Kelp)
	if (pKELP_Descriptor != NULL)
          return pKELP_Descriptor->isMiddlePartition(axis);
#endif
	PADRE_ABORT();	// No distribution sublibrary specified.
      }

   bool isRightPartition  (int axis) const
      { 
        PADRE_ASSERT (representation != NULL);

#if !defined(NO_Parti)
	if (pPARTI_Descriptor != NULL)
	  return pPARTI_Descriptor->isRightPartition(axis);
#endif
#if !defined(NO_GlobalArrays)
	if (pGlobalArrays_Descriptor != NULL)
          return pGlobalArrays_Descriptor->representation->isRightPartition(axis);
#endif
#if !defined(NO_Kelp)
	if (pKELP_Descriptor != NULL)
          return pKELP_Descriptor->isRightPartition(axis);
#endif
	PADRE_ABORT();	// No distribution sublibrary specified.
      }

   bool isNonPartition  (int axis) const
      {
        PADRE_ASSERT (representation != NULL);

#if !defined(NO_Parti)
	if (pPARTI_Descriptor != NULL)
	  return pPARTI_Descriptor->isNonPartition(axis);
#endif
#if !defined(NO_GlobalArrays)
	if (pGlobalArrays_Descriptor != NULL)
          return pGlobalArrays_Descriptor->isNonPartition(axis);
#endif
#if !defined(NO_Kelp)
	if (pKELP_Descriptor != NULL)
          return pKELP_Descriptor->isNonPartition(axis);
#endif
	PADRE_ABORT();	// No distribution sublibrary specified.
      }

// Used in allocate.C to determin position of null array objects in distributions
   int isLeftNullArray ( UserLocalDomain & serialArrayDomain , int Axis ) const;
   int isRightNullArray( UserLocalDomain & serialArrayDomain , int Axis ) const;

   void updateGhostBoundaries ( double *dataPointer );
   void updateGhostBoundaries ( float  *dataPointer );
   void updateGhostBoundaries ( int    *dataPointer );

// I/O operators have to go into the header files - I suspect this is
// because the source for friend templated functions are not serached for and so
// we can make sure the that template code is instanciated properly only if we have them
// in the header file.

  friend istream & operator>> ( istream & is, PADRE_Descriptor & X ) {
    cout << "istream &"
	 << " PADRE_Descriptor ::"
	 << " operator>> ( istream & is, PADRE_Descriptor & X )"
	 << " not implemented\n";
    return is;
  }

  friend ostream & operator<< (ostream & os, const PADRE_Descriptor & X ) {
    PADRE_ASSERT(X.representation != NULL);
    PADRE_ASSERT(X.GlobalDescriptor != NULL);
    os << '('
       << "The Global Domain:\n" << X.globalDomain << '\n'
       << "The Representation:\n" << *X.representation << '\n'
    // << "The GlobalDescriptor:\n" << *X.GlobalDescriptor << '\n'
       << ')' ;
    return os;
  }

  static void freeMemoryInUse();


// These are the pointers to the sublibrary specific objects
// wrapped in objects to provide a standard interface.
#if !defined(NO_Parti)
private: PARTI_Descr *pPARTI_Descriptor;
#endif
#if !defined(NO_GlobalArrays)
private: GlobalArrays_Descr *pGlobalArrays_Descriptor;
#endif
#if !defined(NO_Kelp)
private: KELP_Descr *pKELP_Descriptor;
#endif

  //  DomainMap_Descriptor *pDomainLayout_Descriptor;
  //  PGSLib_Descriptor    *pPGSLib_Descriptor;

  /*
    Read access to sublibrary descriptors.
    Public interface should not allow modifying the pointer values.
  */
public:
#if !defined(NO_Parti)
  PARTI_Descr &getPartiDescriptor (){ return *pPARTI_Descriptor; }
#endif
#if !defined(NO_GlobalArrays)
  GlobalArrays_Descr &getGlobalArraysDescriptor () const { return *pGlobalArrays_Descriptor; }
#endif
#if !defined(NO_Kelp)
  KELP_Descr &getKelpDescriptor (){ return *pKELP_Descriptor; }
#endif

#if !defined(NO_Parti)
public: DECOMP *getBlockPartiArrayDecomposition() const { 
  DECOMP* ReturnPointer = NULL;
  PADRE_ASSERT (representation != NULL);
  if (representation->pPARTI_Representation != NULL) {
    PADRE_ASSERT 
      (representation->pPARTI_Representation->BlockPartiArrayDecomposition != NULL);
    ReturnPointer =representation->
      pPARTI_Representation->BlockPartiArrayDecomposition; 
  }
  return ReturnPointer;
}

public: DARRAY *getBlockPartiArrayDomain() const { 
  DARRAY* ReturnPointer = NULL;
  PADRE_ASSERT (representation != NULL);
  if (representation->pPARTI_Representation != NULL) {
    PADRE_ASSERT 
      (representation->pPARTI_Representation->BlockPartiArrayDescriptor != NULL);
    ReturnPointer =representation->
      pPARTI_Representation->BlockPartiArrayDescriptor; 
  }
  return ReturnPointer;
}
#endif

public: int nDims () const {
#if !defined(NO_Parti)
  return pPARTI_Descriptor->nDims();
#elif !defined(NO_GlobalArrays)
  return pGlobalArrays_Descriptor->nDims();
#elif !defined(NO_Kelp)
  return pKELP_Descriptor->nDims();
#else
#error "No sublibrary is defined in PADRE."
  PADRE_ABORT();
#endif
}

};


// KCC and GNU g++ want to see the source code included 
// in the header files for template instantiation
// #ifdef HAVE_EXPLICIT_TEMPLATE_INSTANTIATION
// #include "PADRE_Descriptor.C"
// #endif

#endif	// PADRE_Descriptor_h
