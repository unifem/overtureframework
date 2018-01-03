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

// file: PADRE_Kelp.h

//========================================================================

#ifndef PADRE_Kelp_Distributionh
#define PADRE_Kelp_Distributionh

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
class KELP_Distribution 
{
  private:
     friend class PADRE_Distribution
	<UserCollection, UserGlobalDomain, UserLocalDomain>;

  // In the case of P++ this is the list of array objects which use this 
  // partition object
  // This uses the STL list container class
     list<UserCollection*> *UserCollectionList;

     static KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> 
	*defaultDistribution;

  // Access functions to the default distribution
     static KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>
	& getDefaultDistribution();
     static KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>
	* getDefaultDistributionPointer();

     int referenceCount;

// ... WARNING: THIS SHOULD BE CHANGED FOR KELP ....
  // This is the maximum number of axes that this distribution object will use
  static int defaultDistributionDimension;
  int distributionDimension;


  // This describes a range of processors (both for the defaults and for 
  // each KELP_Distribution object). For a Kelp distribution some processors
  // in this range might not be used.  The start and end are mainly here
  // for compatibility with Parti.

  static int defaultStartingProcessor;
  static int defaultEndingProcessor;
  int startingProcessor, endingProcessor;

  //static int* defaultProcessorArray;
  static int defaultNumberOfProcessors;
  int* processorArray;
  int numberOfProcessors;


  // These are the widths used to setup the P++ arrays -- however the P++ arrays 
  // can reset their own ghost boundary widths and so these are not the default 
  // values for a specific instance of a partitioning object and not the same as 
  // the static DefaultGhostCellWidth for the partitioning_type class.
  
  static int defaultGhostCellWidth [KELP_MAX_ARRAY_DIMENSION];
  int LocalGhostCellWidth  [KELP_MAX_ARRAY_DIMENSION];
  

// ... WARNING: DOES CYCLIC EXIST IN KELP? ...
  // Specify block distribution (choice of "*" UNDISTRIBUTED "B" BLOCK "C" CYCLIC 
  // distribution)  
  // The length of the string is the number of dimensions that will be distributed
  // Specify block distribution (choice of "*"-Undistributed or 
  // "B"-Block or "C"-Cyclic distribution)

  static char defaultDistribution_String [KELP_MAX_ARRAY_DIMENSION];
  char Distribution_String [KELP_MAX_ARRAY_DIMENSION];


  // Default and user specified ArrayDimensionsToAlign
  
// ... WARNING: THIS SHOULD BE CHANGED FOR KELP ...
  static int defaultArrayDimensionsToAlign [KELP_MAX_ARRAY_DIMENSION];
  int ArrayDimensionsToAlign [KELP_MAX_ARRAY_DIMENSION];
  
// ... WARNING: THIS SHOULD BE CHANGED FOR KELP ...
  // KELP permits the user to specify an extended region around the perimeter of 
  // a distributed domain.  These values may be specified independently for the 
  // right/left, top/bottom, etc.  They are unused in P++ but we make them 
  // available here.  These might not be accessible from PADRE!
  
  static int defaultExternalGhostCellArrayLeft [KELP_MAX_ARRAY_DIMENSION];
  int ExternalGhostCellArrayLeft [KELP_MAX_ARRAY_DIMENSION];
  
  static int defaultExternalGhostCellArrayRight [KELP_MAX_ARRAY_DIMENSION];
  int ExternalGhostCellArrayRight [KELP_MAX_ARRAY_DIMENSION];
  

  // KELP permits some limited control over how each axis of the domain is 
  // distributed (see the KELP manual for more info on these).
  
  static int defaultPartitionControlFlags [KELP_MAX_ARRAY_DIMENSION];
  int PartitionControlFlags [KELP_MAX_ARRAY_DIMENSION];
  

  // See the KELP manual for more info!

  static int defaultDecomposition_Dimensions [KELP_MAX_ARRAY_DIMENSION];
  int Decomposition_Dimensions [KELP_MAX_ARRAY_DIMENSION];

  // ... SHOULD THIS BE PUBLIC? ...
  KELP_Distribution();

public:
  ~KELP_Distribution();
   KELP_Distribution( const KELP_Distribution & X );
   KELP_Distribution & operator= ( const KELP_Distribution & X );
   KELP_Distribution( int inputNumberOfProcessors );
   KELP_Distribution( int startingProcessor, int endingProcessor );
   KELP_Distribution( int* Processor_Array,  int Number_Of_Processors );

   static void freeMemoryInUse();
   void initialize ();
   static void staticInitialize ();

   void setProcessorRange( int startingProcessor, int endingProcessor );
   static void setDefaultProcessorRange
      ( int startingProcessor, int endingProcessor );

   void setProcessorArray( int* inputProcessorArray, int inputNumProcessors );
   static void setDefaultProcessorArray
      ( int* inputProcessorArray, int inputNumProcessors );

   void testConsistency( const char *label = "" ) const;
   static void displayDefaultValues ( const char *Label = "" );
   void display ( const char *label = "" ) const;

   void incrementReferenceCount () const
      { ((KELP_Distribution*) this)->referenceCount++; }
   void decrementReferenceCount () const
      { ((KELP_Distribution*) this)->referenceCount--; }
   int getReferenceCount () const
      { return referenceCount; }
   static int getReferenceCountBase ()
      { return 1; }

   list<UserCollection*> *getUserCollectionList() const
      {
        PADRE_ASSERT (UserCollectionList != NULL);
        return UserCollectionList;
      }

   void setUserCollectionList( list<UserCollection*> *List )
      {
        PADRE_ASSERT (List != NULL);
        UserCollectionList = List;
        PADRE_ASSERT (UserCollectionList != NULL);
      }

// Should we have default values of Partition_Axis = true and 
// GhostBoundaryWidth = 0?
   void DistributeAlongAxis 
      ( int Axis, bool Partition_Axis, int GhostBoundaryWidth )
      {
     // We use the bool variable becuase it make it easier to call
     // this function within a loop (typically over all the dimensions)
        if (Partition_Axis == true)
             Distribution_String [Axis] = 'B';
          else
             Distribution_String [Axis] = '*';

        LocalGhostCellWidth [Axis] = GhostBoundaryWidth;
      }

  friend ostream & operator<< ( ostream & os, const KELP_Distribution & X)
  {
    os << "{KELP_Distribution:  "
       << "Starting_Processor = " << X.startingProcessor
       << ", Ending_Processor = " << X.endingProcessor
       << ", more information as yet omitted"
       << "}" << endl;
    return os;
  }

  int getNumberOfAxesToDistribute() const
  { return distributionDimension; }

  int getInternalGhostCellWidth ( int axis ) const
  {
    return LocalGhostCellWidth[axis];
  }

  int getExternalGhostCellWidth ( int axis ) const
  {
    return ExternalGhostCellArrayLeft[axis];
  }

  void getInternalGhostCellWidthArray ( int* Values ) const
  {
    for (int i=0; i < KELP_MAX_ARRAY_DIMENSION; i++)
       {
         Values[i] = LocalGhostCellWidth[i];
       }
  }

  void getExternalGhostCellWidthArray ( int* Values ) const
  {
    for (int i=0; i < KELP_MAX_ARRAY_DIMENSION; i++)
       {
         PADRE_ASSERT (ExternalGhostCellArrayLeft[i] == 
		       ExternalGhostCellArrayRight[i]);
         Values[i] = ExternalGhostCellArrayLeft[i];
       }
  }

  char* getDistributionString () const
  { return (char*) Distribution_String; }

  int getStartingProcessor () const
  { return startingProcessor; }

  int getEndingProcessor () const
  { return endingProcessor; }

  int getDefaultStartingProcessor () const
  { return defaultStartingProcessor; }

  int getDefaultEndingProcessor () const
  { return defaultEndingProcessor; }

  int* getProcessorArray () const
  { return processorArray; }

  int getProcessorArrayLength () const
  { return numberOfProcessors; }

  int* getArrayDimensionsToAlign () const
  { return (int*) ArrayDimensionsToAlign; }

  int* getPartitionControlFlags () const
  { return (int*) PartitionControlFlags; }

  int* getDecomposition_Dimensions () const
  { return (int*) Decomposition_Dimensions; }


}; // end class KELP_Distribution

//========================================================================


#endif //PADRE_Kelp_Distributionh






