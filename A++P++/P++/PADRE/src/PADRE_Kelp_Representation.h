// *****************************************************************
// BUG ALERT --- BUG ALERT --- BUG ALERT --- BUG ALERT --- BUG ALERT
// *****************************************************************
// BUG DISCRIPTION: A bug in the Sun C++ compiler was found which
// is particularly difficult to figure out. ALL template class member
// function not defined in the header file MUST be declared before any
// of the member function defined in the header file!!!!!  If this is not
// done then the compiler will not search the *.C file and will not instantiate
// the template function.  The result is that templated member function will not
// be found at link time of any application requiring the templated class's member
// function.
// *****************************************************************
// BUG ALERT --- BUG ALERT --- BUG ALERT --- BUG ALERT --- BUG ALERT
// *****************************************************************

// file: PADRE_Kelp_Representation.h

//========================================================================

#ifndef PADRE_Kelp_Representationh
#define PADRE_Kelp_Representationh

// Forward class declaration 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain> 
class KELP_Descriptor;


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
class KELP_Representation
{
// At some point we should move these data to be private and provide access 
// functions

public:

  // Contains KELP specific parts of P++ descriptor
  // Pointer to a P++ partitioning object
  
  KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> *Distribution;

// This should be a global domain which is translation independent
// It will likely share a pointer with the PADRE_Representation object's 
// globalDomain
  UserLocalDomain *globalDomain;

  FLOORPLAN *KelpFloorPlan;

  
public:

  ~KELP_Representation();

  KELP_Representation();
  KELP_Representation( const KELP_Representation & X );
  KELP_Representation( const UserLocalDomain *inputGlobalDomain );
  KELP_Representation
     ( const KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> 
       & distribution, const UserLocalDomain *inputGlobalDomain );
  KELP_Representation
     ( const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> 
       & distribution, const UserLocalDomain *inputGlobalDomain );
  
  KELP_Representation & operator= ( const KELP_Representation & X );

  KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & 
     getKELP_Distribution() const
     { return 
	 *(((KELP_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>*)
	 this)->Distribution); }

  static void freeMemoryInUse();

   int referenceCount;
   void incrementReferenceCount () const
      { ((KELP_Representation*) this)->referenceCount++; }
   void decrementReferenceCount () const
      { ((KELP_Representation*) this)->referenceCount--; }
   int getReferenceCount () const
      { return referenceCount; }
   static int getReferenceCountBase ()
      { return 1; }

  UserLocalDomain* getGlobalDomainPointer()
     { 
       PADRE_ASSERT (globalDomain != NULL);
       return globalDomain; 
     }

  int getInternalGhostCellWidth ( int axis ) const
  {
    return getKELP_Distribution().getInternalGhostCellWidth(axis);
  }

  // Tests the internal consistency of the object
  void testConsistency ( const char *Label = "" ) const;

  void getLocalSizes ( int* LocalSizeArray );

  static void displayDefaultValues ( const char *Label = "" );
  void display ( const char *Label = "" ) const;

  // ... WARNING: REPLACE THIS? ...
  // We need to get this function from P++ where it resides currently
  // void testConsistency ( const DARRAY* BlockPartiArrayDescriptor, 
  //			    const char *label = "" ) const {};

  //static void SpecifyDecompositionEmbeddingIntoVirtualProcessorSpace (
  //            DECOMP* BlockParti_Decomposition_Pointer ,
  //            int StartingProcessor , int EndingProcessor );

#if 0
  DECOMP* Build_BlockPartiDecompostion(
    const KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> 
       & distribution, 
    int *Array_Sizes );

  DARRAY* Build_BlockPartiDescriptor (
    const KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & 
      distribution,
    DECOMP* BlockPartiArrayDecomposition,
    int *Array_Sizes ,
    int* InternalGhostCellWidth, 
    int* ExternalGhostCellWidth );
#endif

  FLOORPLAN* Build_KelpFloorPlan
    ( const KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & 
      distribution,
      int *Array_Sizes ,
      int* InternalGhostCellWidth, 
      int* ExternalGhostCellWidth );

  static void make_parti_distribution 
    ( int numdims, int* arrayDims, 
      int *Array_Sizes ,
      int* intGCWidth, int* extGCWidthL, int* extGCWidthR,
      int* extra_flag,int* decompDims, char* dimDist,
      FLOORPLAN* Return_KelpFloorPlan);

  friend ostream & operator<< (ostream & os, const KELP_Representation & X)
  {
    os << "{KELP_Represention:  "
       << ", more information as yet omitted"
       << "}" << endl;
    return os;
  }

  //static void displaySCHED  ( SCHED*  X );
  static void displayFloorPlan ( FLOORPLAN* X );

private:
  void initialize ();
};

//========================================================================


#endif //PADRE_Kelp_Representationh






