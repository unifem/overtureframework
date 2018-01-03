/*

This header file declares types PADRE types that interface with GlobalArrays.

Author: BTNG

*/

/*

Design notes:

GA uses global C variables to track data.  The primary data which describes
all arrays is the global variable GA.  Each array corresponds to one location
in GA.  GA contains data that describes all the information PADRE would need
from the GA arrays.

As of 16Oct2000, GA is described in global.armci.c as:
typedef struct {
       int  ndim;               number of dimensions
       int  dims[MAXDIM];       global array dimensions
       int  chunk[MAXDIM];      chunking (dimension of partition, I think.  BTNG)
       int  nblock[MAXDIM];     number of blocks in each dimension
       double scale[MAXDIM];    nblock/dim (precomputed)
       char **ptr;              arrays of pointers to remote data
       int  *mapc;              block distribution map
       Integer type;            type of array
       int  actv;               activity status
       Integer lo[MAXDIM];      top/left corner in local patch (first indices of partition, I think. BTNG)
       Integer size;            size of local data in bytes
       int elemsize;            sizeof(datatype)
       long lock;               lock
       long id;			ID of shmem region / MA handle
       char name[FNAM+1];       array name
} global_array_t;

*/

#ifndef PADRE_GlobalArrays_H
#define PADRE_GlobalArrays_H


#include <PADRE_config.h>
#include <PADRE_macros.h>
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
class PADRE_Descriptor; 


// GlobalArrays header files.
#include <global.h>
#include <macommon.h>

// *wdh*#include <iostream.h>
#include <iostream>
using namespace std;

// Resolve STL header file names.
#ifndef STL_LIST_HEADER_FILE
#define STL_LIST_HEADER_FILE <list.h>
#endif
#ifndef STL_VECTOR_HEADER_FILE
#define STL_VECTOR_HEADER_FILE <vector.h>
#endif
// STL headers.
#include STL_LIST_HEADER_FILE
#include STL_VECTOR_HEADER_FILE

// Forward references to main templated classes in PADRE
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain> 
class PADRE_Distribution;


#define GlobalArrays_MAX_ARRAY_DIMENSION 6
#if GlobalArrays_MAX_ARRAY_DIMENSION == 4
#define REPEAT_MAX_ARRAY_DIMENSION_TIMES(A) A,A,A,A
#define IOTA_MAX_ARRAY_DIMENSION_TIMES 0,1,2,3
#define FILL_MACRO(A,B) { A[0]=B; A[1]=B; A[2]=B; A[3]=B; }
#elif GlobalArrays_MAX_ARRAY_DIMENSION == 6
#define REPEAT_MAX_ARRAY_DIMENSION_TIMES(A) A,A,A,A,A,A
#define IOTA_MAX_ARRAY_DIMENSION_TIMES 0,1,2,3,4,5
#define FILL_MACRO(A,B) { A[0]=B; A[1]=B; A[2]=B; A[3]=B; A[4]=B; A[5]=B; }
#else
#error Current code in PADRE_GlobalArrays.h can only handle GlobalArrays_MAX_ARRAY_DIMENSION of 4 or 6
#endif


  //! Class for static members supporting GlobalArrays.  Not to be instantiated!
class GlobalArrays
{
private:
  GlobalArrays ( const GlobalArrays & X );
  GlobalArrays & operator= ( const GlobalArrays & X );
public:
 ~GlobalArrays ();
  GlobalArrays ();
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: static void freeMemoryInUse ();
  //! Function to initialize GlobalArrays library.  Should be called by PADRE's initialization.
public: static void SublibraryInitialization();
  //! Specify whether the GlobalArrays library has been initialized.
private: static bool SublibraryInitialized;
  //! The process number.
public: static int My_Process_Number;             // my process number
  //! Number of processes at run time.
public: static int numberOfProcessors;
  //! For representing an invalid GlobalArrays handle.
public: enum HandleValues {
  InvalidHandle=-1 /* Make sure that -1 really is an invalid handle value in the GA code. */
};

}; // end class GlobalArrays





  //! GlobalArrays distribution.
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
class GlobalArrays_Distribution 
{
public: ~GlobalArrays_Distribution() {
  /*
    As of 20Apr01, BTNG is not aware of anything that should be done at
    at descruction time, nothing that should not destruct itself.  I could
    be wrong though.
  */
}
public: GlobalArrays_Distribution();
  //! This constructor has not been written because it is not currently required.  BTNG.
public: GlobalArrays_Distribution( const GlobalArrays_Distribution & X ) {
  cerr << "Using unfinished code at " << __LINE__ << " in " << __FILE__ << endl;
  PADRE_ABORT();
}
  //! This constructor has not been written because it is not currently required.  BTNG.
public: GlobalArrays_Distribution & operator= ( const GlobalArrays_Distribution & X ) {
  cerr << "Using unfinished code at " << __LINE__ << " in " << __FILE__ << endl;
}
  //! This constructor has not been written because it is not currently required.  BTNG.
public: GlobalArrays_Distribution( int inputNumberOfProcessors ) {
  cerr << "Using unfinished code at " << __LINE__ << " in " << __FILE__ << endl;
  PADRE_ABORT();
}
public: GlobalArrays_Distribution( int startingProcessor, int endingProcessor );
  //! Perform common initialization tasks for all constructors.
private: void constructorInitialize();
  //! Perform initialization on all static variables of this class.
public: static void staticInitialize ();


  public: ostream &show( ostream &os ) const {
    os << *this; return os;
    /* This is just an interface to the friend operator<<,
       but g++-2.91.66 sometimes have problems instantiating
       the latter when directly used.  BTNG.
     */
  }
  friend ostream & operator<< ( ostream & os, const GlobalArrays_Distribution & X)
  {
    os << "{GlobalArrays_Distribution:  "
       << "This is an imcomplete function at " << __LINE__ << " in " << __FILE__
       << "}" << endl;
    return os;
  }


  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: int distributionDimension;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: static int defaultDistributionDimension;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: static GlobalArrays_Distribution *defaultDistribution;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: static int defaultStartingProcessor;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: static int defaultEndingProcessor;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: static char defaultDistribution_String		[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: char Distribution_String			[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: static int defaultGhostCellWidth		[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: static int defaultArrayDimensionsToAlign	[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: int ArrayDimensionsToAlign			[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: static int defaultExternalGhostCellArrayLeft	[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: int ExternalGhostCellArrayLeft			[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: static int defaultExternalGhostCellArrayRight	[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: int ExternalGhostCellArrayRight		[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: static int defaultPartitionControlFlags	[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: int PartitionControlFlags			[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: static int defaultDecomposition_Dimensions	[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: int Decomposition_Dimensions			[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: int startingProcessor, endingProcessor;
  //! Specifies whether the array should be partitioned along each of its axes.
public: bool distributeAlongAxis			[GlobalArrays_MAX_ARRAY_DIMENSION];


  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: void setProcessorRange( int inputStartingProcessor, int inputEndingProcessor );

  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: static void setDefaultProcessorRange( int startingProcessor, int endingProcessor );

  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: int getNumberOfAxesToDistribute() const {
  return distributionDimension;
}


  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: void getInternalGhostCellWidthArray ( int* Values ) const {
  // All ghost boundary widths are zero because GA does not support ghost boundaries.
  fill( Values, Values+GlobalArrays_MAX_ARRAY_DIMENSION, 0 );
}




  /*
    The following is simply analogous to Parti example.
    I don't necessarily know what it does.  BTNG
  */
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: list<UserCollection*> *UserCollectionList;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: void setUserCollectionList( list<UserCollection*> *List )
  {
    PADRE_ASSERT (List != NULL);
    UserCollectionList = List;
    PADRE_ASSERT (UserCollectionList != NULL);
  }
  //! Test the consistency of the object state.  Currently there is no test.
public: void testConsistency( const char *label = "" ) const {};
  //! Reference count.
private: int referenceCount;
  //! Increment reference count, to be called whenever newly referencing this object.
public: void incrementReferenceCount () const {
  ((GlobalArrays_Distribution*) this)->referenceCount++;
}
  //! Decrement reference count, to be called when no longer referencing this object.
public: void decrementReferenceCount () const {
  ((GlobalArrays_Distribution*) this)->referenceCount--;
}
  //! Returns the reference count for the object.
public: int getReferenceCount () const {
  return referenceCount;
}
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: static int getReferenceCountBase () {
  return 1;
}
  //! Display the object's reference count.
public: void displayReferenceCounts ( const char *label ) const;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: int LocalGhostCellWidth[GlobalArrays_MAX_ARRAY_DIMENSION];
  //! Specify whether an axis should be partitioned.
public: void DistributeAlongAxis ( int Axis, bool Partition_Axis, int GhostBoundaryWidth );
  //! Display the object.
public: void display ( const char *label = "" ) const;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: static void freeMemoryInUse ();
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: static void displayDefaultValues ( const char *Label = "" );




}; // end class GlobalArrays_Distribution








  //! GlobalArrays representation.
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
class GlobalArrays_Representation
{
public: typedef GlobalArrays_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> Distr;
  /*
    Constructors:
   */
public: GlobalArrays_Representation( const GlobalArrays_Representation & X );
public: GlobalArrays_Representation( const UserLocalDomain *inputGlobalDomain );
public: GlobalArrays_Representation(
  const Distr & distribution
, const UserLocalDomain *inputGlobalDomain
);
public: GlobalArrays_Representation(
  const PADRE_Distribution<UserCollection,UserGlobalDomain,UserLocalDomain> & inputPADRE_Distribution
, const UserLocalDomain *inputGlobalDomain
);
private: void constructorInitialize();
  /*
    Destructors:
   */
public: ~GlobalArrays_Representation();
  /*
    Operators.
  */
public: GlobalArrays_Representation & operator= ( const GlobalArrays_Representation & X );

  /*
    Access.
  */
public: Distr & getGlobalArrays_Distribution() const {
  return *(((GlobalArrays_Representation *)this)->Distribution);
}
  //! Reference count.
private: int referenceCount;
  //! Increment reference count, to be called whenever newly referencing this object.
public: void incrementReferenceCount () const {
  ((GlobalArrays_Representation*) this)->referenceCount++;
}
  //! Decrement reference count, to be called when no longer referencing this object.
public: void decrementReferenceCount () const {
  ((GlobalArrays_Representation*) this)->referenceCount--;
}
  //! Returns the reference count for the object.
public: int getReferenceCount () const {
  return referenceCount;
}
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: static int getReferenceCountBase () {
  return 1;
}
  //! Display the object's reference count.
public: void displayReferenceCounts ( const char *label ) const;
  //! Test consistency of internal state.
public: void testConsistency( const char *label = "" ) const {};

  /*
    External data.
  */
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: UserLocalDomain *globalDomain;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: UserLocalDomain* getGlobalDomainPointer()
  { 
    PADRE_ASSERT (globalDomain != NULL);
    return globalDomain; 
  }

  //! Return zero because GlobalArrays does not support ghost boundaries.
public: int getInternalGhostCellWidth ( int axis ) const {
  /* Ghost cells are not (currently) supported in GA. */
  return 0;
  }
private:
  vector<int> arrayDims //! Dimensions of the array
  , loIndex //! Low indices  of local partition
  , hiIndex //! High indices  of local partition
  ;
  //! Set some overhead data regarding the array.
public: void setGaOverheadData( const vector<int> &dims, const vector<int> &lo, const vector<int> &hi ) {
  /*
    After a GlobalArrays_Descriptor allocates data, it should call this function
    to inform the representation of the array sizes.
    The data here duplicates those kept by GlobalArrays.
    This class cannot access data kept by GlobalArrays because it does not have
    the GlobalArrays handle for the array.
   */
  PADRE_ASSERT( globalDomain->numberOfDimensions() == dims.size() );
  PADRE_ASSERT( globalDomain->numberOfDimensions() == lo.size() );
  PADRE_ASSERT( globalDomain->numberOfDimensions() == hi.size() );
  arrayDims = dims;
  loIndex = lo;
  hiIndex = hi;
}
  //! Set LocalSizeArray to arrayDims for significant dimensions. Higher dimensions are set to 1.
public: void getLocalSizes ( int* LocalSizeArray ) {
  fill_n( LocalSizeArray, PADRE_MAX_ARRAY_DIMENSION, 1 );
  copy( arrayDims.begin(), arrayDims.end(), LocalSizeArray );
#if 0
  int i;
  for ( i=0; i<loIndex.size(); i++ ) {
    //wrong: LocalSizeArray[i] = hiIndex[i] - loIndex[i];
  }
#endif
};
  //! Display the object.
public: void display ( const char *label = "" ) const {
  cout << PADRE::RS() << label << endl;
  cout << PADRE::RS() << "nblock: " << flush;
  int i;
  for ( i=0; i<PADRE_MAX_ARRAY_DIMENSION; i++ ) cout << ' ' << ga_nblock[i];
  cout << endl;
  cout << PADRE::RS() << "mapc: " << flush;
  for ( i=0; i<ga_mapc.size(); i++ ) cout << ' ' << ga_mapc[i];
  cout << endl;
}
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: static void freeMemoryInUse ();


  //! The distribution used by this object.
  Distr *Distribution;



  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: UserGlobalDomain *localDescriptor;
  //! States whether the local partition is a left partition.
public: int isLeftPartition   (int Axis) const ;
  //! States whether the local partition is a right partition.
public: int isMiddlePartition (int Axis) const ;
  //! States whether the local partition is a middle partition.
public: int isRightPartition  (int Axis) const ;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: int isNonPartition    (int Axis) const ;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
private: bool isSomePartitionPosition(int Axis, char position) const;

  //! Shorthand for a vector of integers.
private: typedef vector<int> vi;
  //! ga_mapc is the array specifying how an array is partitioned.
  /*!
    It is equivalent to the field mapc in the GlobalArrays global
    variable GA.  Its length is the sum of nblock (number of
    partitions) over all dimensions.  Its data is the start of
    each partition in each dimension.
  */
private: vi ga_mapc;
  //! nblock is the number of blocks each dimension is divided into.
  /*!
    if nblock[i] == 0, no number of block has been specified for dimension i.
  */
private: int ga_nblock[PADRE_MAX_ARRAY_DIMENSION];

  //! Return the GlobalArrays variable nblock corresponding to the array.
public: const int* nblock() const {
  return ga_nblock;
}
  //! Return the GlobalArrays variable mapc corresponding to the array.
public: const int* mapc() const {
  return ga_mapc.begin();
}
  //! This function computes the variables ga_mapc using the user-specifiable ga_nblock data.
public: int computeMapc( const vi &dims);

  /*!
    This function is a nice interface to GA's NGA_Locate_region function.
    Given the inclusive index ranges in the vectors lo and hi (inclusive),
    this function fills region_location with the ranges for each processor
    that holds any part of the region.
   */
private: int get_GA_region_location( vector<vector<int> > &region_location, const vector<int> &lo, const vector<int> &hi );


};	// Ends GlobalArrays_Representation.








  //! GlobalArrays descriptor.
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
class GlobalArrays_Descriptor
{
  //! Shorthand for GlobalArrays_Representation.
public: typedef GlobalArrays_Representation<UserCollection,UserGlobalDomain,UserLocalDomain> Repre;

public: ~GlobalArrays_Descriptor ();
public: GlobalArrays_Descriptor 
( const PADRE_Descriptor<UserCollection,UserGlobalDomain,UserLocalDomain> *inputPADRE_Descriptor,
  const UserLocalDomain *inputGlobalDomain,
  const UserLocalDomain *inputLocalDomain,
  const Repre *inputRepresentation, 
  const UserGlobalDomain *inputLocalDescriptor );


private: GlobalArrays_Descriptor ();
private: GlobalArrays_Descriptor ( const GlobalArrays_Descriptor & X );
private: GlobalArrays_Descriptor &operator=( const GlobalArrays_Descriptor & X );

  //! The representation used by this descriptor.
public: Repre *representation;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: UserGlobalDomain *localDescriptor;


  //! The identifying handle for the GlobalArrays array.
  /*!
    handle: the GA handle for a particular array.  This is (directly related to)
    the index of the array in the global list GA.
  */
private: int handle;
  //! Return the GlobalArrays handle.
public: int Handle() const {
  return handle;
}
  //! GlobalArrays type index of primitive data.
private: size_t data_ga_index;
  //! Return the GlobalArrays type index of primitive data.
public: int dataGAIndex() const {
  return data_ga_index;
}
  //! Size of primitive data.
private: size_t data_size;
  //! Return the size of primitive data.
public: int dataSize() const {
  return data_size;
}
  //! Address of data allocated by GlobalArrays.
private: void *data_address;
  //! Return the address of GlobalArrays-allocated data.
public: void *dataAddress() const {
  return data_address;
}
  // private: const global_array_t *gat;
/*
  gat: pointer to the structure (part of the GlobalArrays GA list) that describes
  the array.
*/
  //! Return the dimension of the array.
public: int nDims() { return GA_Ndim(handle); }


  //! Reference count.
private: int referenceCount;
  //! Increment reference count, to be called whenever newly referencing this object.
public: void testConsistency( const char *label = "" ) const;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: void incrementReferenceCount () const {
  ((GlobalArrays_Descriptor*) this)->referenceCount++;
}
  //! Decrement reference count, to be called when no longer referencing this object.
public: void decrementReferenceCount () const {
  ((GlobalArrays_Descriptor*) this)->referenceCount--;
}
  //! Returns the reference count for the object.
public: int getReferenceCount () const {
  return referenceCount;
}
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: static int getReferenceCountBase () {
  return 1;
}
  //! Display the object's reference count.
public: void displayReferenceCounts ( const char *label ) const;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: UserLocalDomain *localDomain;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: UserLocalDomain *globalDomain;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: UserLocalDomain* getGlobalDomainPointer()
  { 
    PADRE_ASSERT (globalDomain != NULL);
    return globalDomain; 
  }
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: void InitializeLocalDescriptor ( UserGlobalDomain & inputLocalDescriptor, UserLocalDomain & inputLocalDomain );
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: void InitializeLocalDescriptor ();
  //! Allocate the array of doubles.
  /*!
    Functions allocateData should call on the distribution to allocate the
    distributed object. The local_data_ptr variable will be set to point to
    the location where the local data of the distributed object resides.
   */
public: void allocateData ( double** local_data_ptr ) {
  allocateData(MT_F_DBL, (void**)local_data_ptr);
  data_ga_index = MT_F_DBL;
  data_size = sizeof(**local_data_ptr);
  PADRE_ASSERT( data_size != 0 );
#ifdef PADRE_DEBUG_IS_ENABLED
  cerr << PADRE::RS() << "Allocated double array with handle " << handle << endl;
#endif
}
  //! Allocate the array of floats.
public: void allocateData ( float** local_data_ptr ) {
  /*
    When floats are finally supported by GA, change MT_C_FLOAT to MT_F_FLOAT.
    In the mean time, this will compile but will not work and MT_F_FLOAT
    will not compile.  BTNG.
  */
  allocateData(MT_C_FLOAT, (void**)local_data_ptr);
  data_ga_index = MT_C_FLOAT;
  data_size = sizeof(**local_data_ptr);
  PADRE_ASSERT( data_size != 0 );
#ifdef PADRE_DEBUG_IS_ENABLED
  cerr << PADRE::RS() << "Allocated float array with handle " << handle << endl;
#endif
}
  //! Allocate the array of ints.
public: void allocateData ( int** local_data_ptr ) {
  allocateData(MT_F_INT, (void**)local_data_ptr );
  data_ga_index = MT_F_INT;
  data_size = sizeof(**local_data_ptr);
  PADRE_ASSERT( data_size != 0 );
#ifdef PADRE_DEBUG_IS_ENABLED
  cerr << PADRE::RS() << "Allocated int array with handle " << handle << endl;
#endif
}
  //! Allocate an array of type specified by the type_specifier.
private: void allocateData ( int type_specifier, void **local_data_ptr );

  //! Transfer integer data from the source to the destination.
public: static void transferData 
( const UserGlobalDomain & receiveDomain,
  const UserGlobalDomain & sendDomain,
  const int *sourceDataPointer,
  const int *destinationDataPointer ) {
#if 1
  // Make sure that descriptors are associated with the correct type.
  PADRE_ASSERT( receiveDomain.parallelPADRE_DescriptorPointer->getGlobalArraysDescriptor().dataGAIndex()
		== MT_F_INT );
  PADRE_ASSERT( sendDomain.parallelPADRE_DescriptorPointer->getGlobalArraysDescriptor().dataGAIndex()
		== MT_F_INT );
  PADRE_ASSERT( receiveDomain.parallelPADRE_DescriptorPointer->getGlobalArraysDescriptor().dataSize()
		== sizeof(*destinationDataPointer) );
  PADRE_ASSERT( sendDomain.parallelPADRE_DescriptorPointer->getGlobalArraysDescriptor().dataSize()
		== sizeof(*sourceDataPointer) );
#endif
  transferData( receiveDomain, sendDomain, (void*)sourceDataPointer, (void*)destinationDataPointer, MT_F_INT );
}


  //! Transfer float type data from the source to the destination.
public: static void transferData 
( const UserGlobalDomain & receiveDomain,
  const UserGlobalDomain & sendDomain,
  const float *sourceDataPointer,
  const float *destinationDataPointer ) {
#if 1
  // Make sure that descriptors are associated with the correct type.
  PADRE_ASSERT( receiveDomain.parallelPADRE_DescriptorPointer->getGlobalArraysDescriptor().dataGAIndex()
		== MT_C_FLOAT );
  PADRE_ASSERT( sendDomain.parallelPADRE_DescriptorPointer->getGlobalArraysDescriptor().dataGAIndex()
		== MT_C_FLOAT );
  PADRE_ASSERT( receiveDomain.parallelPADRE_DescriptorPointer->getGlobalArraysDescriptor().dataSize()
		== sizeof(*destinationDataPointer) );
  PADRE_ASSERT( sendDomain.parallelPADRE_DescriptorPointer->getGlobalArraysDescriptor().dataSize()
		== sizeof(*sourceDataPointer) );
#endif
  transferData( receiveDomain, sendDomain, (void*)sourceDataPointer, (void*)destinationDataPointer, MT_C_FLOAT );
}

  //! Transfer double type data from the source to the destination.
public: static void transferData 
( const UserGlobalDomain & receiveDomain,
  const UserGlobalDomain & sendDomain,
  const double *sourceDataPointer,
  const double *destinationDataPointer ) {
#if 1
  // Make sure that descriptors are associated with the correct type.
  PADRE_ASSERT( receiveDomain.parallelPADRE_DescriptorPointer->getGlobalArraysDescriptor().dataGAIndex()
		== MT_F_DBL );
  PADRE_ASSERT( sendDomain.parallelPADRE_DescriptorPointer->getGlobalArraysDescriptor().dataGAIndex()
		== MT_F_DBL );
  PADRE_ASSERT( receiveDomain.parallelPADRE_DescriptorPointer->getGlobalArraysDescriptor().dataSize()
		== sizeof(*destinationDataPointer) );
  PADRE_ASSERT( sendDomain.parallelPADRE_DescriptorPointer->getGlobalArraysDescriptor().dataSize()
		== sizeof(*sourceDataPointer) );
#endif
  transferData( receiveDomain, sendDomain, (void*)sourceDataPointer, (void*)destinationDataPointer, MT_F_DBL );
}

  //! Transfer data of type specified by ga_data_type from the source to the destination.
private: static void transferData 
( const UserGlobalDomain & receiveDomain,
  const UserGlobalDomain & sendDomain,
  const void *sourceDataPointer,
  const void *destinationDataPointer,
  int ga_data_type
);

  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: void updateGhostBoundaries ( double *dataPointer ) {
  /* Do nothing because GA does not support ghost boundaries. */
  return;
}
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: void updateGhostBoundaries ( float *dataPointer ) {
  /* Do nothing because GA does not support ghost boundaries. */
  return;
}
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: void updateGhostBoundaries ( int *dataPointer ) {
  /* Do nothing because GA does not support ghost boundaries. */
  return;
}
  //! Display the object.
public: void display ( const char *label = "" ) const;
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: static void freeMemoryInUse ();
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: static void displayDefaultValues ( const char *Label = "" ) {
  cerr << "Non-critical use of unimplemented code at " << __LINE__ << " in " << __FILE__ << endl;
}
  //! This member was copied from corresponding PARTI class.  Not sure exactly how it is used.  BTNG.
public: static void swapDistribution 
( const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & oldDistribution ,
  const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & newDistribution );


};




// PADRE_GlobalArrays_H
#endif







