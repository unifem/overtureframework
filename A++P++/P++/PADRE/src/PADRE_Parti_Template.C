// file:  PADRE_Parti_Template.C

#ifndef PADRE_PARTI_TEMPLATE_C
#define PADRE_PARTI_TEMPLATE_C

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

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

#if !defined(NO_Parti)

#include <PADRE_Parti.h>
#include <PADRE_Global.h>



// *****************************************************************************
//                     GLOBAL VARIABLE INITIALIZATION
// *****************************************************************************

// This is defined in P++ so we don't want to define it again here!
extern MPI_Comm Global_PARTI_P_plus_plus_Interface_PPP_Comm_World;

// int   Global_PARTI_PADRE_Interface_Number_Of_Processors         = 0;
extern char* Global_PARTI_PADRE_Interface_Name_Of_Main_Processor_Group; // = MAIN_PROCESSOR_GROUP_NAME;
   

#if 1
// template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
// int PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
// defaultGhostCellWidth = 0;

// Decarataion of space for static member variables
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
defaultDistributionDimension = PARTI_MAX_ARRAY_DIMENSION;

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
defaultStartingProcessor = 0;

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
defaultEndingProcessor = 0;

// template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
// int PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
// defaultGhostCellWidth [PARTI_MAX_ARRAY_DIMENSION];

// Specify block distribution (choice of "*"-Undistributed or "B"-Block or "C"-Cyclic distribution)
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
char PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
defaultDistribution_String [PARTI_MAX_ARRAY_DIMENSION] = { REPEAT_MAX_ARRAY_DIMENSION_TIMES('B') };

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
defaultArrayDimensionsToAlign [PARTI_MAX_ARRAY_DIMENSION] = { IOTA_MAX_ARRAY_DIMENSION_TIMES };

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
defaultGhostCellWidth [PARTI_MAX_ARRAY_DIMENSION] = { REPEAT_MAX_ARRAY_DIMENSION_TIMES(0) };

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
defaultExternalGhostCellArrayLeft [PARTI_MAX_ARRAY_DIMENSION] = { REPEAT_MAX_ARRAY_DIMENSION_TIMES(0) };

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
defaultExternalGhostCellArrayRight [PARTI_MAX_ARRAY_DIMENSION] = { REPEAT_MAX_ARRAY_DIMENSION_TIMES(0) };

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
defaultPartitionControlFlags [PARTI_MAX_ARRAY_DIMENSION] = { REPEAT_MAX_ARRAY_DIMENSION_TIMES(0) };

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
defaultDecomposition_Dimensions [PARTI_MAX_ARRAY_DIMENSION] = { IOTA_MAX_ARRAY_DIMENSION_TIMES };

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>*
     PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
     defaultDistribution = NULL;

// This variable provides a means of communicating the Number of processors and a common
// name to use in referencing the PVM group of processors to the lower level parallel 
// library.

#endif

// *****************************************************************************
//                     DEFINITION OF MEMBER FUNCTIONS
// *****************************************************************************

//Distribution////////////////////////////////////////////////////////////////////

// Memory Management
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
freeMemoryInUse () {
#if defined(USE_TAU)
     TAU_PROFILE("PARTI_Distribution::freeMemoryInUse()", "void(void)", TAU_PADRE_MEMORY_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PARTI_Distribution::freeMemoryInUse (possible memory leak) \n");
#endif

  // printf ("In PARTI_Distribution::freeMemoryInUse(): delete defaultDistribution \n");
     if (defaultDistribution != NULL)
        {
       // printf ("defaultDistribution->getReferenceCount() = %d \n",
       //      defaultDistribution->getReferenceCount());
          defaultDistribution->decrementReferenceCount();
          if (defaultDistribution->getReferenceCount() < defaultDistribution->getReferenceCountBase())
               delete defaultDistribution;
          defaultDistribution = NULL;
        }

     PARTI::freeMemoryInUse();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> *
   PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
getDefaultDistributionPointer() {
     if (defaultDistribution == NULL)
        {
       // We call the default constructor to build a PARTI_Distribution object with default values!
       // printf ("############################################################################ \n");
       // printf ("###############   BUILDING THE DEFAULT DISTRIBUTION OBJECT   ############### \n");
       // printf ("############################################################################ \n");
          defaultDistribution = new PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>();
       // printf ("############################################################################ \n");
        }
     PADRE_ASSERT (defaultDistribution != NULL);

  // Bugfix (DQ 3/3/2000): previously return *defaultDistribution it seems that all but the SGI C++ compiler
  // ignore this error and even permit "***** *defaultDistribution" I don't know why -- this function is used
  // so it should be instantiated everywhere.
  // *return defaultDistribution;
     return defaultDistribution;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> &
   PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
getDefaultDistribution()
   {
  // Implement this function in terms of the other function (much simpler)
     return *(getDefaultDistributionPointer());
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
~PARTI_Distribution ()
   {
  // There are no pointers to delete so nothing is required here!
  // cout << " PARTI_Distribution<UC,UD> ::"
  //      << "  ~PARTI_Distribution() "
  //      << " not implemented\n";

  // printf ("Inside of PARTI_Distribution::~PARTI_Distribution() referenceCount = %d \n",referenceCount);

     const int BadValue = -99;
     distributionDimension = BadValue;
     startingProcessor = BadValue;
     endingProcessor = BadValue;

  // This is deleted in the PADRE object not in the distribution library objects
  // delete UserCollectionList;
  // if (UserCollectionList != NULL)
  //    {
  //      delete UserCollectionList;
  //    }
     UserCollectionList = NULL;

     for (int i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          LocalGhostCellWidth[i]         = BadValue;
          Distribution_String[i]         = 'X';
          ArrayDimensionsToAlign[i]      = BadValue;
          ExternalGhostCellArrayLeft[i]  = BadValue;
          ExternalGhostCellArrayRight[i] = BadValue;
          PartitionControlFlags[i]       = BadValue;
          Decomposition_Dimensions[i]    = BadValue;
        }
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PARTI_Distribution ()
   {
  // This default constructor is only used for constructing a PARTI_Distribution
  // with default values.
  // This constructor does not have any data that need be set
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PARTI_Distribution<UC,UD> ::"
               << " PARTI_Distribution()"
               << " not implemented\n";
#endif

     initialize();
     testConsistency ("Called from constructor PARTI_Distribution()");
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PARTI_Distribution ( int inputStartingProcessor, int inputEndingProcessor )
   {
     // cerr << "Inside PARTI_Distribution ( int inputStartingProcessor, int inputEndingProcessor )"
	  // << endl;
     // PADRE_ABORT();
  // This constructor does not have any data that need be set
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PARTI_Distribution<UC,UD> ::"
               << " PARTI_Distribution(inputStartingProcessor,inputEndingProcessor)"
               << " partially implemented\n";
#endif

     if (defaultDistribution == NULL)
        {
       // We call the default constructor to build a PARTI_Distribution object with default values!
       // printf ("############################################################################ \n");
       // printf ("###############   BUILDING THE DEFAULT DISTRIBUTION OBJECT   ############### \n");
       // printf ("############################################################################ \n");
          defaultDistribution = new PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>();
       // printf ("############################################################################ \n");
        }
  // initialize();
  // The setProcessorRange() function first calls the initialize function!
     setProcessorRange (inputStartingProcessor,inputEndingProcessor);
     testConsistency ("Called from constructor PARTI_Distribution()");
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PARTI_Distribution ( int inputNumberOfProcessors )
// : PARTI_Distribution(0,inputNumberOfProcessors-1)
   {
  // This constructor calls an alternative constructor
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PARTI_Distribution<UC,UD> ::"
               << " PARTI_Distribution(inputNumberOfProcessors)"
               << " partially implemented\n";
#endif

     if (defaultDistribution == NULL)
        {
       // We call the default constructor to build a PARTI_Distribution object with default values!
       // printf ("############################################################################ \n");
       // printf ("###############   BUILDING THE DEFAULT DISTRIBUTION OBJECT   ############### \n");
       // printf ("############################################################################ \n");
          defaultDistribution = new PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>();
       // printf ("############################################################################ \n");
        }
  // The setProcessorRange() function first calls the initialize function!
     setProcessorRange (0,inputNumberOfProcessors-1);
     testConsistency ("Called from constructor PARTI_Distribution()");
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PARTI_Distribution ( const PARTI_Distribution & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PARTI_Distribution<UC,ULD,UD> ::"
               << " PARTI_Distribution( const PARTI_Distribution & X )"
               << " not implemented\n";
#endif

     PADRE_ASSERT (defaultDistribution != NULL);

  // we simplify this copy constructor by calling the operator=
     operator= (X);
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> &
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
operator= ( const PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PARTI_Distribution<UC,ULD,UD> & operator="
               << " ( const PARTI_Distribution<UC,UD> & X ) \n";
#endif

     startingProcessor = X.startingProcessor;
     endingProcessor   = X.endingProcessor;

     UserCollectionList = NULL;

     int temp = 0;
     for (temp=0; temp < PARTI_MAX_ARRAY_DIMENSION; temp++)
        {
       // We need to look up the correct settings for these in the P++ source!
          LocalGhostCellWidth         [temp] = X.LocalGhostCellWidth         [temp];
          Distribution_String         [temp] = X.Distribution_String         [temp];
          ArrayDimensionsToAlign      [temp] = X.ArrayDimensionsToAlign      [temp];
          ExternalGhostCellArrayLeft  [temp] = X.ExternalGhostCellArrayLeft  [temp];
          ExternalGhostCellArrayRight [temp] = X.ExternalGhostCellArrayRight [temp];
          PartitionControlFlags       [temp] = X.PartitionControlFlags       [temp];
          Decomposition_Dimensions    [temp] = X.Decomposition_Dimensions    [temp];
        }
  
     return *this;
   }

#if 1
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void 
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
setDefaultProcessorRange( int inputStartingProcessor, int inputEndingProcessor )
   {
     PADRE_ASSERT( inputStartingProcessor <= inputEndingProcessor );
  // initialize();
     defaultStartingProcessor = inputStartingProcessor;
     defaultEndingProcessor   = inputEndingProcessor;
   }
#endif
  
#if 1
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void 
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
setProcessorRange( int inputStartingProcessor, int inputEndingProcessor )
   {
     PADRE_ASSERT( inputStartingProcessor <= inputEndingProcessor );
     initialize();
     startingProcessor = inputStartingProcessor;
     endingProcessor   = inputEndingProcessor;
   }
#endif

#if 1
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
testConsistency ( const char *Label ) const
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "Inside of PARTI_Distribution::testConsistency (" << Label << ") \n";
#endif

     PADRE_ASSERT (startingProcessor >= 0);
     if (endingProcessor   >= PARTI::numberOfProcessors)
        {
          printf ("ERROR: endingProcessor = %d  PARTI::numberOfProcessors = %d \n",
               endingProcessor,PARTI::numberOfProcessors);
        }
     PADRE_ASSERT (endingProcessor   < PARTI::numberOfProcessors);

  // PADRE_ASSERT (UserCollectionList != NULL);

  // printf ("WARNING: We still need to check all the values and see what values we should check against! \n");

     int temp = 0;
     for (temp=0; temp < PARTI_MAX_ARRAY_DIMENSION; temp++)
        {
          PADRE_ASSERT (LocalGhostCellWidth[temp] >= 0);
        }
   }
#endif

#if 1
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
staticInitialize()
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PARTI_Distribution<UC,UD> ::"
               << " staticInitialize() \n";
#endif

  // printf ("Exiting at TOP of PARTI_Distribution::staticInitialize() \n");
  // PADRE_ABORT();

  // This is not the best way to handle this.  Here we initialize
  // the static member data (but for another function).
     PARTI::SublibraryInitialization ();

  // Set the default to be the maximum number of dimensions
  // This is adjusted lower if the data to be distributed is lower dimensional
     defaultDistributionDimension = PARTI_MAX_ARRAY_DIMENSION;

  // Initialize the processor space to be the whole machine
     defaultStartingProcessor = 0;
     defaultEndingProcessor   = PARTI::numberOfProcessors-1;

  // This is a scalar quanity (not an array of scalars)
  // defaultGhostCellWidth = 0;

  // printf ("WARNING: We must look up the correct settings for these in the P++ source! \n");

     int temp = 0;
     for (temp=0; temp < PARTI_MAX_ARRAY_DIMENSION; temp++)
        {
#if 0
       // We need to look up the correct settings for these in the P++ source!
          LocalGhostCellWidth         [temp] = defaultGhostCellWidth;
          Distribution_String         [temp] = 'B';
          ArrayDimensionsToAlign      [temp] =  0;
          ExternalGhostCellArrayLeft  [temp] =  0;
          ExternalGhostCellArrayRight [temp] =  0;
          PartitionControlFlags       [temp] =  0;
          Decomposition_Dimensions    [temp] =  0;
#else
          defaultDistribution_String         [temp] = 'B';
       // defaultLocalGhostCellWidth         [temp] =  0;
       // defaultGhostCellWidth              [temp] =  0;
          defaultGhostCellWidth              [temp] =  
               PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
                  getDefaultGhostCellWidth(temp);
          defaultArrayDimensionsToAlign      [temp] =  temp;
          defaultExternalGhostCellArrayLeft  [temp] =  0;
          defaultExternalGhostCellArrayRight [temp] =  0;
          defaultPartitionControlFlags       [temp] =  0;
          defaultDecomposition_Dimensions    [temp] =  temp;
#endif
        }

     if (defaultDistribution == NULL)
        {
       // We call the default constructor to build a PARTI_Distribution object with default values!
       // printf ("############################################################################ \n");
       // printf ("###############   BUILDING THE DEFAULT DISTRIBUTION OBJECT   ############### \n");
       // printf ("############################################################################ \n");
          defaultDistribution = new PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>();
       // printf ("############################################################################ \n");
        }

  // testConsistency ("Called from constructor PARTI_Distribution() point A");
   }
#endif

#if 1
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
initialize()
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PARTI_Distribution<UC,UD> ::"
               << " initialize()"
               << " not implemented\n";
#endif

     static int initialized = 0;

     if (initialized == 0)
       {
      // reset to avoid calling this again
         initialized = 1;
      // initialize static data in this class
         staticInitialize();
       }
     startingProcessor = 0;
     endingProcessor   = PARTI::numberOfProcessors-1;

     referenceCount     = getReferenceCountBase();
     UserCollectionList = NULL;

     distributionDimension = defaultDistributionDimension;

  // printf ("WARNING: We must look up the correct settings for these in the P++ source! \n");

     int temp = 0;
     for (temp=0; temp < PARTI_MAX_ARRAY_DIMENSION; temp++)
        {
#if 0
       // We need to look up the correct settings for these in the P++ source!
          LocalGhostCellWidth         [temp] = defaultGhostCellWidth;
          Distribution_String         [temp] = 'B';
          ArrayDimensionsToAlign      [temp] =  0;
          ExternalGhostCellArrayLeft  [temp] =  0;
          ExternalGhostCellArrayRight [temp] =  0;
          PartitionControlFlags       [temp] =  0;
          Decomposition_Dimensions    [temp] =  0;
#else
          Distribution_String         [temp] = defaultDistribution_String         [temp];
       // LocalGhostCellWidth         [temp] = defaultLocalGhostCellWidth         [temp];
       // LocalGhostCellWidth         [temp] = defaultGhostCellWidth;
          LocalGhostCellWidth         [temp] = defaultGhostCellWidth              [temp];
       // GhostCellWidth              [temp] = 0;
          ArrayDimensionsToAlign      [temp] = defaultArrayDimensionsToAlign      [temp];
          ExternalGhostCellArrayLeft  [temp] = defaultExternalGhostCellArrayLeft  [temp];
          ExternalGhostCellArrayRight [temp] = defaultExternalGhostCellArrayRight [temp];
          PartitionControlFlags       [temp] = defaultPartitionControlFlags       [temp];
          Decomposition_Dimensions    [temp] = defaultDecomposition_Dimensions    [temp];
#endif
        }

  // display("this is a test");
   }
#endif


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
display ( const char *label ) const
   {
     printf ("Inside of PARTI_Distribution::display (%s) \n",label);
     printf ("referenceCount                 = %d \n",getReferenceCount());
     printf ("distributionDimension          = %d \n",distributionDimension);
     printf ("startingProcessor              = %d \n",startingProcessor);
     printf ("endingProcessor                = %d \n",endingProcessor);

     int i = 0;
     printf ("LocalGhostCellWidth ([0-%d]):        ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %d",LocalGhostCellWidth[i]);
        }
     printf ("\n");

     printf ("Distribution_String ([0-%d]):        ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %c",Distribution_String[i]);
        }
     printf ("\n");

     printf ("ArrayDimensionsToAlign ([0-%d]):     ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %d",ArrayDimensionsToAlign[i]);
        }
     printf ("\n");

     printf ("ExternalGhostCellArrayLeft ([0-%d]): ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %d",ExternalGhostCellArrayLeft[i]);
        }
     printf ("\n");

     printf ("ExternalGhostCellArrayRight ([0-%d]):",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %d",ExternalGhostCellArrayRight[i]);
        }
     printf ("\n");

     printf ("PartitionControlFlags ([0-%d]):      ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %d",PartitionControlFlags[i]);
        }
     printf ("\n");

     printf ("Decomposition_Dimensions ([0-%d]):   ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %d",Decomposition_Dimensions[i]);
        }
     printf ("\n");
   }

//Representation///////////////////////////////////////////////////////////////

// Memory Management
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>::freeMemoryInUse ()
   {
#if defined(USE_TAU)
     TAU_PROFILE("PARTI_Representation::freeMemoryInUse()", "void(void)", TAU_PADRE_MEMORY_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PARTI_Representation::freeMemoryInUse (possible memory leak) \n");
#endif

   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
~PARTI_Representation()
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PARTI_Representation::~PARTI_Representation() referenceCount = %d \n",referenceCount);
#endif

     referenceCount = 0;

  // Pointer to Block-Parti parallel decomposition (object)
  // PADRE_ASSERT(BlockPartiArrayDecomposition != NULL);
  // printf ("In ~PARTI_Representation(): delete BlockPartiArrayDecomposition \n");
     if (BlockPartiArrayDecomposition != NULL)
        {
#if defined(PADRE_DEBUG_IS_ENABLED)
          if (PADRE::debugLevel() > 0)
               printf ("PARTI_Representation(): deleting BlockPartiArrayDecomposition (reference count = %d) \n",
                    BlockPartiArrayDecomposition->referenceCount);
#endif
       // We must handle the reference counting manually since these are BLOCK PARTI objects (in C)!
       // printf ("Inside of ~PARTI_Representation: BlockPartiArrayDecomposition->referenceCount = %d \n",
       //      BlockPartiArrayDecomposition->referenceCount);

       // Note that the decrement of the reference count is a part of the delete_DECOMP function!
          PADRE_ASSERT( BlockPartiArrayDecomposition->referenceCount >= 0 );
          delete_DECOMP ( BlockPartiArrayDecomposition );
          BlockPartiArrayDecomposition = NULL;
        }

  // PADRE_ASSERT(BlockPartiArrayDescriptor != NULL);
  // printf ("PARTI_Representation(): delete BlockPartiArrayDescriptor \n");
     if (BlockPartiArrayDescriptor != NULL)
        {
#if defined(PADRE_DEBUG_IS_ENABLED)
          if (PADRE::debugLevel() > 0)
               printf ("PARTI_Representation(): deleting BlockPartiArrayDescriptor (reference count = %d) \n",
                    BlockPartiArrayDescriptor->referenceCount);
#endif

       // We must handle the reference counting manually since these are BLOCK PARTI objects (in C)!
       // printf ("Inside of ~PARTI_Representation: BlockPartiArrayDescriptor->referenceCount = %d \n",
       //    BlockPartiArrayDescriptor->referenceCount);
       // Note that the decrement of the reference count is a part of the delete_DARRAY function!

          PADRE_ASSERT( BlockPartiArrayDescriptor->referenceCount >= 0 );
          delete_DARRAY ( BlockPartiArrayDescriptor );
          BlockPartiArrayDescriptor = NULL;
        }

  // printf ("PARTI_Representation(): delete Distribution \n");
     if (Distribution != NULL)
        {
       // printf ("PARTI_Representation(): Distribution->getReferenceCount() = %d \n",Distribution->getReferenceCount());
          Distribution->decrementReferenceCount();
          if (Distribution->getReferenceCount() < Distribution->getReferenceCountBase())
               delete Distribution;
          Distribution = NULL;
        }

  // printf ("PARTI_Representation(): delete globalDomain \n");
     if (globalDomain != NULL)
        {
       // printf ("PARTI_Representation(): globalDomain->getReferenceCount() = %d \n",globalDomain->getReferenceCount());
          globalDomain->decrementReferenceCount();
          if (globalDomain->getReferenceCount() < globalDomain->getReferenceCountBase())
               delete globalDomain;
          globalDomain = NULL;
        }
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PARTI_Representation()
   {
  // This constructor is only useful if the data is filled in afterward
  // however since this is not the programming model that we want 
  // We make calling this constructor an error for now!
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PARTI_Representation<UC,UD> ::"
               << " PARTI_Representation()"
               << " not implemented \n";
#endif

     referenceCount = getReferenceCountBase();
     Distribution = NULL;
     globalDomain = NULL;
     BlockPartiArrayDecomposition = NULL;
     BlockPartiArrayDescriptor    = NULL;

     printf ("Calling the PARTI_Representation default constructor is an error! \n");
     PADRE_ABORT();
     testConsistency ("Called in PARTI_Representation() \n");
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PARTI_Representation( const UserLocalDomain *inputGlobalDomain )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel())
        {
       // printf ("Inside of PARTI_Representation (UserLocalDomain) \n");
          cout << "PARTI_Representation<UC,UD> ::"
               << " PARTI_Representation(UserLocalDomain)"
               << " partially implemented \n";
        }
#endif

     referenceCount = getReferenceCountBase();

  // This makes a copy of the UserLocalDomain object
  // we require a copy here since the UserDoamin object in the PARTI_Representation
  // object is ment to be translation independent (but do we want it here or in the 
  // PADRE_Representation object?????).  Maybe this should really just be a reference
  // (through a pointer mechanism perhaps) to the UserLocalDomain in the PADRE_Representation
  // object.  That might simplify the design.
     if (inputGlobalDomain != NULL)
          inputGlobalDomain->incrementReferenceCount();
     globalDomain = (UserLocalDomain*) inputGlobalDomain;

  // Use the default PARTI_Distribution object in this default constructor!
  // Distribution = new PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ();
     Distribution = 
          PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
               getDefaultDistributionPointer();
     Distribution->incrementReferenceCount();
     BlockPartiArrayDecomposition = NULL;
     BlockPartiArrayDescriptor    = NULL;

  // initialize();
     testConsistency ("Called in PARTI_Representation(const UserLocalDomain & X) \n");
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PARTI_Representation(const PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> & X)
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PARTI_Representation<UC,UD> ::"
               << " PARTI_Representation(const PARTI_Representation<UC,UD> & X)"
               << " not implemented \n";
#endif
   
     referenceCount = getReferenceCountBase();

     printf ("Exit at end of PARTI_Representation (PARTI_Representation,UserLocalDomain) constructor \n");
     PADRE_ABORT();
  // initialize();
     testConsistency ("Called in PARTI_Representation(const PARTI_Representation & X) \n");
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PARTI_Representation ( const PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & inputDistribution, 
		       const UserLocalDomain *inputGlobalDomain )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PARTI_Representation<UC,UD> ::"
               << " PARTI_Representation("
               << " const PARTI_Distribution<UC,UD> & distribution,"
               << " const UserLocalDomain & inputGlobalDomain)"
               << " entering \n";
#endif

     referenceCount = getReferenceCountBase();

#if 1
   //?? to make it compile...
     if (inputGlobalDomain != NULL)
          inputGlobalDomain->incrementReferenceCount();
     globalDomain = (UserLocalDomain*) inputGlobalDomain;

     inputDistribution.incrementReferenceCount();
     Distribution = &((PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>&) inputDistribution);

     PADRE_ASSERT (globalDomain != NULL);
     PADRE_ASSERT (Distribution != NULL);

  // Distribution->display("Distribution in PARTI_Representation(PARTI_Distribution,UserLocalDomain)");

  // We don't have a "min" function available to us
  // int Number_Of_Dimensions_To_Partition 
  // = min (distribution->Descriptor_Dimension,globalDomain.dimension());
     int Number_Of_Dimensions_To_Partition =
       (Distribution->getNumberOfAxesToDistribute() < globalDomain->numberOfDimensions()) ?
        Distribution->getNumberOfAxesToDistribute() : globalDomain->numberOfDimensions();

     int Array_Sizes[PARTI_MAX_ARRAY_DIMENSION];
     globalDomain->getRawDataSize(Array_Sizes);
     BlockPartiArrayDecomposition = Build_BlockPartiDecomposition ( inputDistribution , Array_Sizes );
  // PADRE_ASSERT (BlockPartiArrayDecomposition != NULL);
     PADRE_ASSERT (BlockPartiArrayDecomposition != NULL || globalDomain->numberOfDimensions() == 0);

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          displayDECOMP (BlockPartiArrayDecomposition);
#endif

     int tempInternalGhostCellWidth [PARTI_MAX_ARRAY_DIMENSION];
     int tempExternalGhostCellWidth [PARTI_MAX_ARRAY_DIMENSION];
     Distribution->getInternalGhostCellWidthArray(tempInternalGhostCellWidth);
     Distribution->getExternalGhostCellWidthArray(tempExternalGhostCellWidth);

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
        {
          int i=0;
          for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
             {
               printf ("Array_Sizes[%d] = %d \n",i,Array_Sizes[i]);
               printf ("tempInternalGhostCellWidth[%d] = %d \n",i,tempInternalGhostCellWidth[i]);
               printf ("tempExternalGhostCellWidth[%d] = %d \n",i,tempExternalGhostCellWidth[i]);
             }
        }
#endif

     BlockPartiArrayDescriptor = Build_BlockPartiDescriptor (
                         *Distribution,
                         BlockPartiArrayDecomposition , Array_Sizes ,
                         tempInternalGhostCellWidth , 
                         tempExternalGhostCellWidth );
     
  // PADRE_ASSERT(BlockPartiArrayDescriptor != NULL);
     PADRE_ASSERT(BlockPartiArrayDescriptor != NULL || globalDomain->numberOfDimensions() == 0);

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          displayDARRAY (BlockPartiArrayDescriptor);
#endif

     testConsistency ("Called in PARTI_Representation() \n");

  // printf ("Exiting at BASE of PARTI_Representation constructor! \n");
  // PADRE_ABORT();
#endif
   }


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void 
PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
testConsistency( const char *label ) const
   {
  // The purpose of this function is to fill in the 
  // local descriptor but not allocate the local data

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PARTI_Representation<UC,ULD,UD> ::"
               << " testConsistency"
               << " ( const char *label = "" )"
               << " not implemented \n";
#endif

     PADRE_ASSERT (referenceCount >= getReferenceCountBase());

  // These are not initialized until the localDescriptor is initialized (this is a confusing point!)
  // PADRE_ASSERT (BlockPartiArrayDecomposition != NULL);
  // PADRE_ASSERT (BlockPartiArrayDescriptor    != NULL);
     PADRE_ASSERT (globalDomain != NULL);
     PADRE_ASSERT (Distribution != NULL);
  
     if (globalDomain->isNull())
        {
          PADRE_ASSERT (BlockPartiArrayDecomposition == NULL);
          PADRE_ASSERT (BlockPartiArrayDescriptor    == NULL);
        }
       else
        {
       // This will not handle the case were the local processors has no part of the
       // nonzero size global array.  We WILL need a test for that case!
       // But this will serve well as a test for now! 
          PADRE_ASSERT (BlockPartiArrayDecomposition != NULL);
          PADRE_ASSERT (BlockPartiArrayDescriptor    != NULL);
        }
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
SpecifyDecompositionEmbeddingIntoVirtualProcessorSpace
     ( DECOMP* BlockParti_Decomposition_Pointer , int Input_Starting_Processor , int Input_Ending_Processor )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of Internal_Partitioning_Type::SpecifyDecompositionEmbeddingIntoVirtualProcessorSpace (%d,%d) \n",
               Input_Starting_Processor,Input_Ending_Processor);
#endif

     // #if defined(PARALLEL_PADRE)
  // Starting_Processor = Input_Starting_Processor;
  // Ending_Processor   = Input_Ending_Processor;

#if 0
     // #if defined(PARALLEL_PADRE)
     FFlag = 1;                    // so PARTI treats all arrays as FORTRAN ordered arrays

#if defined(PVM)
     myproc(My_Process_Number);  // initialize the PARTI library (we call the global myprog)
#endif

     PARTI::My_Process_Number  = PARTI_myproc();
     PARTI::numberOfProcessors = PARTI_MPI_numprocs();
     printf ("This processor = %d numberOfProcessors = %d \n",
          PARTI::My_Process_Number,PARTI::numberOfProcessors);

  // Bugfix (6/11/95) fixed bug in translation from old version of comm_man.C to
  // new version of comm_man.C (with ability to switch communication libraries PVM and MPI)
     Global_PARTI_PADRE_Interface_Number_Of_Processors = PARTI::numberOfProcessors;

     PADRE_ASSERT(Global_PARTI_PADRE_Interface_Number_Of_Processors == PARTI::numberOfProcessors);
     PADRE_ASSERT(Global_PARTI_PADRE_Interface_Number_Of_Processors > 0);
     PADRE_ASSERT(Global_PARTI_PADRE_Interface_Name_Of_Main_Processor_Group != NULL);
     // #endif

     PADRE_ASSERT (PARTI::numberOfProcessors > 0);
     PADRE_ASSERT (PARTI::numberOfProcessors <= MAX_PROCESSORS);

  // Make sure that Virtual Processor Spaces is defined
     if (PARTI::VirtualProcessorSpace == NULL) 
        {

#if defined(PADRE_DEBUG_IS_ENABLED)
          if (PADRE::debugLevel())
               printf ("Build virtual processor space! \n");
#endif

       // Block Parti only implemented with 1D virtual processor spaces
          int Sizes[1];
          Sizes[0] = PARTI::numberOfProcessors;
          PARTI::VirtualProcessorSpace = vProc(1,Sizes);
        } 
       else 
        {
#if defined(PADRE_DEBUG_IS_ENABLED)
    if (PADRE::debugLevel())
      printf ("Virtual Processor Space ALREADY BUILT! \n");
#endif
        } // if

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel())
          printf ("(Virtual processor Space size) - numberOfProcessors = %d \n", PARTI::numberOfProcessors);
#endif
#endif

     PADRE_ASSERT(PARTI::VirtualProcessorSpace != NULL);

     PARTI::isPARTIInitialized();

  // error checking
     PADRE_ASSERT( BlockParti_Decomposition_Pointer != NULL );
     PADRE_ASSERT( PARTI::VirtualProcessorSpace != NULL );
     PADRE_ASSERT( Input_Starting_Processor >= 0 );
     PADRE_ASSERT( Input_Ending_Processor   >= 0 );
     PADRE_ASSERT( Input_Ending_Processor   < MAX_PROCESSORS );
     PADRE_ASSERT( Input_Starting_Processor <= Input_Ending_Processor );

  // printf ("BEFORE: Embed function! \n");
  // displayDECOMP (BlockParti_Decomposition_Pointer);

     embed ( BlockParti_Decomposition_Pointer ,
             PARTI::VirtualProcessorSpace ,
             Input_Starting_Processor , Input_Ending_Processor );

  // printf ("AFTER: Embed function! \n");
  // displayDECOMP (BlockParti_Decomposition_Pointer);
     // #endif
   }

// Alternatively we have to be able to take a PADRE Distribution as well
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>::
PARTI_Representation( 
     const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & inputDistribution, 
     const UserLocalDomain *inputGlobalDomain )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PARTI_Representation<UC,UD> ::"
               << " PARTI_Representation"
               << "(const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & inputDistribution,"
               << " const UserLocalDomain & inputGlobalDomain)"
               << " not implemented \n";
#endif

     printf ("PARTI_Representation constructor taking PADRE_Distribution object not implemented \n");
     PADRE_ABORT();
   
  // Distribution = &((PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>&) inputDistribution);
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> &
PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
operator= ( const PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PARTI_Representation<UC,UD> & operator="
               << " ( const PARTI_Representation<UC,UD> & X ) not implemented\n";
#endif
     return *this;
   }


// This functions does not appear to be used anywhere!
// inline bool Is_Virtual_Processor_Space_Too_Large ()
//    {
//   // for the moment we return false to simplify initial debugging
//      return true;
//    }


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
DECOMP* 
PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
Build_BlockPartiDecomposition ( const PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & distribution,
			       int *Array_Sizes )
// Need to get these from the distribution object
// char* Local_Distribution_String, int Local_Starting_Processor, int Local_Ending_Processor
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>::Build_BlockPartiDecomposition( PARTI_Distribution,int*)! \n");
#endif

  // PARTI can't represent a NULL array so we represent this abstraction in PADRE
  // (outside of PARTI)
     if (globalDomain->isNull())
        {
          return NULL;
        }

  // Resize the selected subset of the virtual processor space in the case where
  // the number of processors is greater than the number of elements in the array.
  // Steps:
  //     1. figure out the size of the largest dimension
  //     2. and the size of the prime factors of the Number of processors

  // int Number_Of_Dimensions_To_Partition = UserLocalDomain::dimension ( Array_Sizes );
  // int Number_Of_Dimensions_To_Partition 
  //    = (distribution.getNumberOfAxesToDistribute() < globalDomain.dimension()) ?
  //      distribution.getNumberOfAxesToDistribute() : globalDomain.dimension();
     int Number_Of_Dimensions_To_Partition =
       (distribution.getNumberOfAxesToDistribute() < globalDomain->numberOfDimensions()) ?
        distribution.getNumberOfAxesToDistribute() : globalDomain->numberOfDimensions();

     char* Local_Distribution_String = distribution.getDistributionString();
     int Local_Starting_Processor = distribution.getStartingProcessor();
     int Local_Ending_Processor   = distribution.getEndingProcessor();
     bool Subset_Of_Virtual_Processor_Space_Too_Large = false;
     DECOMP* localBlockPartiDecomposition = NULL;
 
#if 1
     do {
       // do ... while not generating a case with fewer grid points along each
       // array dimension than the prime factors of the value representing the
       // number of processors.  Each iteration will reduce the range of the
       // virtual processor space.

#if defined(PADRE_DEBUG_IS_ENABLED)
       if (PADRE::debugLevel() > 0)
         {
	   printf ("Number_Of_Dimensions_To_Partition = %d \n",Number_Of_Dimensions_To_Partition);
	   printf ("Subset_Of_Virtual_Processor_Space_Too_Large = %s \n",
		(Subset_Of_Virtual_Processor_Space_Too_Large) ? "true" : "false");
	   for (int i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
	     printf ("Array_Sizes [%d] = %d \n",i,Array_Sizes[i]);
           printf ("PARTI_numprocs() = %d \n",PARTI_numprocs());
         }
#endif

    // Bugfix (9/17/95) for memory leak in case of more than a single pass through the while loop!
    // printf ("if delete localBlockPartiDecomposition \n");
       if (localBlockPartiDecomposition != NULL)
         {
        // printf ("delete_DECOMP (commented out for now!) \n");
           delete_DECOMP (localBlockPartiDecomposition);
	   localBlockPartiDecomposition = NULL;
        // printf ("DONE: delete_DECOMP \n");
         }

    // printf ("Call create_decomp in PARTI_Representation::Build_BlockPartiDecomposition() \n");
       localBlockPartiDecomposition  = create_decomp ( Number_Of_Dimensions_To_Partition, Array_Sizes );
    // printf ("DONE: Call create_decomp in PARTI_Representation::Build_BlockPartiDecomposition() \n");
       PADRE_ASSERT ( localBlockPartiDecomposition != NULL);

#if defined(PADRE_DEBUG_IS_ENABLED)
       if (PADRE::debugLevel())
         {
	   printf ("Call embedding function using the partitioining objects Starting_Processor and Ending_Processor \n");
	   // displayDECOMP (localBlockPartiDecomposition);
         }
#endif
    // Call embedding function using the partitioining objects Starting_Processor and Ending_Processor
       SpecifyDecompositionEmbeddingIntoVirtualProcessorSpace (
         localBlockPartiDecomposition , Local_Starting_Processor , Local_Ending_Processor );

    // Must be called before the align function!
       distribute( localBlockPartiDecomposition, Local_Distribution_String );

#if defined(PADRE_DEBUG_IS_ENABLED)
       if (PADRE::debugLevel())
         {
	   printf ("After call to SpecifyDecompositionEmbeddingIntoVirtualProcessorSpace AND distribute \n");
	// displayDECOMP (localBlockPartiDecomposition);
         }
#endif

       Subset_Of_Virtual_Processor_Space_Too_Large = false;
       for (int i=0; i < localBlockPartiDecomposition->nDims; i++)
         {
#if defined(PADRE_DEBUG_IS_ENABLED)
	   if (PADRE::debugLevel())
              {
	        printf ("Axis = %d \n",i);
                printf ("Array_Sizes[%d] = %d \n",i,Array_Sizes[i]);
                printf ("localBlockPartiDecomposition->dimProc[%d] = %d \n",
                     i,localBlockPartiDecomposition->dimProc[i]);
              }
#endif
	   if ( (Array_Sizes[i] < localBlockPartiDecomposition->dimProc[i]) &&
	        !Subset_Of_Virtual_Processor_Space_Too_Large )
	     {
	       Subset_Of_Virtual_Processor_Space_Too_Large = true;
	       Local_Ending_Processor--;
	       if (Local_Ending_Processor < Local_Starting_Processor)
	         {
		   printf ("ERROR: Local_Ending_Processor = %d < Local_Starting_Processor = %d \n",
                        Local_Ending_Processor,Local_Starting_Processor);
		   PADRE_ABORT();
	         }
	     }
	   // ... (1/6/96,kdb) add a test to make sure this dimension
	   //  isn't partitioned because of too many processors ...
	   if ( (localBlockPartiDecomposition->dimProc[i]>1) &&
	        (localBlockPartiDecomposition->dimDist[i] == '*'))
	     {
	       Subset_Of_Virtual_Processor_Space_Too_Large = true;
	       Local_Ending_Processor--;
	       if (Local_Ending_Processor < Local_Starting_Processor)
	         {
		   printf ("ERROR: Local_Ending_Processor = %d < Local_Starting_Processor = %d \n",
                        Local_Ending_Processor,Local_Starting_Processor);
		   PADRE_ABORT();
	         }
	     }
         }
     }
     while (Subset_Of_Virtual_Processor_Space_Too_Large);
  
  // Error checking (better checking required)
  // Error_Checking_For_Fewer_Array_Elements_Than_Processors ( Array_Sizes , Number_Of_Dimensions_To_Partition ,
  //                                                           Local_Starting_Processor , Local_Ending_Processor );
  
  // printf ("In Build_BlockPartiDecomposition -- Local_Starting_Processor = %d   Local_Ending_Processor = %d \n",
  //      Local_Starting_Processor , Local_Ending_Processor );
  
     PADRE_ASSERT (localBlockPartiDecomposition != NULL);
#endif
     return localBlockPartiDecomposition;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
DARRAY* PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>::Build_BlockPartiDescriptor ( 
     const PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & inputDistribution, 
     DECOMP* localBlockPartiDecomposition , int *Array_Sizes ,
     int* InternalGhostCellWidth , int* ExternalGhostCellWidth )
   {
  // The interface to this functions should be cleaned up to remove redundently
  // defined structures (defined in the function parameter list and as data members 
  // in the PARTI_Representation class).  For now we keep the interface of the
  // function the same as that of those in the P++ implementation.

  // We include the InternalGhostCellWidth and ExternalGhostCellWidth because a representation should
  // be able to be built using the given distribution but with different ghost cell widths.
  // This allows (later) a two PADRE_Descriptors to share the same distribution while permitting
  // each to have more specialized ghost boundary widths that are different. 
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PARTI_Representation::Build_BlockPartiDescriptor(DECOMP*,int*,int*,int*)! \n");
#endif

  // PARTI does not permit the use of its data structures to 
  // define a NULL object!  So PADRE must encapsulate the abstraction.
     if (globalDomain->isNull())
        {
          return NULL;
        }

     // #if defined(PARALLEL_PADRE)
     PADRE_ASSERT (PARTI::VirtualProcessorSpace != NULL );
     PADRE_ASSERT (localBlockPartiDecomposition != NULL);
     PADRE_ASSERT (localBlockPartiDecomposition == BlockPartiArrayDecomposition);
     PADRE_ASSERT (Distribution != NULL);

  // display("*this localBlockPartiDecomposition");
  // int Array_Dimension = globalDomain::getSize ( Array_Sizes );

  // I'm not sure if this is correct (need to recheck -- think about the next line)
     PADRE_ASSERT (Distribution != NULL);
     PADRE_ASSERT (globalDomain != NULL);
     int Array_Dimension  =
       (Distribution->getNumberOfAxesToDistribute() < globalDomain->numberOfDimensions()) ?
        Distribution->getNumberOfAxesToDistribute() : globalDomain->numberOfDimensions();

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
        {
          printf ("Array_Dimension                             = %d \n",Array_Dimension);
          printf ("globalDomain->numberOfDimensions()          = %d \n",globalDomain->numberOfDimensions());
          printf ("Distribution->getNumberOfAxesToDistribute() = %d \n",Distribution->getNumberOfAxesToDistribute());
       // globalDomain->display("globalDomain");
        }
#endif


     if (Array_Dimension <= 0)
        {
          globalDomain->display("globalDomain");
          printf ("Exiting in PARTI_Representation::Build_BlockPartiDescriptor(DECOMP*,int*,int*,int*)! \n");
          PADRE_ABORT();
        }

     PADRE_ASSERT (Array_Dimension > 0);
     PADRE_ASSERT (Array_Dimension <= PARTI_MAX_ARRAY_DIMENSION);

  // Make the Temp_Decomposition_Dimensions a permutation of the member 
  // discriptor.Decomposition_Dimensions and the Input_Decomposition_Dimensions.
     DARRAY* Return_BlockPartiArrayDescriptor = align ( localBlockPartiDecomposition ,
                                                        Array_Dimension ,
                                                        Distribution->getArrayDimensionsToAlign() ,
                                                        Array_Sizes ,
                                                        InternalGhostCellWidth ,
                                                        ExternalGhostCellWidth ,
                                                        ExternalGhostCellWidth ,
                                                        Distribution->getPartitionControlFlags() ,
                                                        Distribution->getDecomposition_Dimensions() );

     PADRE_ASSERT(Return_BlockPartiArrayDescriptor != NULL);

  // displayDECOMP (localBlockPartiDecomposition);
  // displayDARRAY (Return_BlockPartiArrayDescriptor);

     return Return_BlockPartiArrayDescriptor;
     // #else // !defined(PARALLEL_PADRE)
       // // The function must return a value to be properly defined outside of P++
          // return new DARRAY;
     // #endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
getLocalSizes ( int* LocalSizeArray )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PARTI_Representation ::"
               << " getLocalSizes(int*) \n";
#endif

  // We have to handle the case of a NULL array object (which PARTI can't cope with)!
     if (BlockPartiArrayDescriptor == NULL)
        {
          LocalSizeArray[0] = LocalSizeArray[1] = LocalSizeArray[2] = LocalSizeArray[3] = 0;
	  FILL_MACRO(LocalSizeArray,0);
        }
       else
        {
          PADRE_ASSERT (BlockPartiArrayDescriptor != NULL);
       // printf ("Call displayDARRAY in PADRE \n");
       // displayDARRAY(BlockPartiArrayDescriptor);
          laSizes(BlockPartiArrayDescriptor,LocalSizeArray);
        }
   }

//Descriptor///////////////////////////////////////////////////////////////////

// Memory Management
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::freeMemoryInUse ()
   {
#if defined(USE_TAU)
     TAU_PROFILE("PARTI_Descriptor::freeMemoryInUse()", "void(void)", TAU_PADRE_MEMORY_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PARTI_Descriptor::freeMemoryInUse (possible memory leak) \n");
#endif

   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
~PARTI_Descriptor()
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PARTI_Descriptor::~PARTI_Descriptor() referenceCount = %d \n",referenceCount);
#endif

  // printf ("Inside of PADRE: PARTI_Descriptor<UC,ULD,UD>::~PARTI_Descriptor() \n");
  // printf ("In ~PARTI_Descriptor(): delete globalDomain \n");
     if (globalDomain != NULL)
        {
       // printf ("globalDomain->getReferenceCount() = %d \n",globalDomain->getReferenceCount());
          globalDomain->decrementReferenceCount();
          if (globalDomain->getReferenceCount() < globalDomain->getReferenceCountBase())
               delete globalDomain;
          globalDomain = NULL;
        }

  // printf ("In ~PARTI_Descriptor(): delete localDomain \n");
     if (localDomain != NULL)
        {
       // printf ("localDomain->getReferenceCount() = %d \n",localDomain->getReferenceCount());
          localDomain->decrementReferenceCount();
          if (localDomain->getReferenceCount() < localDomain->getReferenceCountBase())
               delete localDomain;
          localDomain = NULL;
        }

  // printf ("In ~PARTI_Descriptor(): delete localDescriptor \n");
     if (localDescriptor != NULL)
        {
       // printf ("localDescriptor->getReferenceCount() = %d \n",localDescriptor->getReferenceCount());
          localDescriptor->decrementReferenceCount();
          if (localDescriptor->getReferenceCount() < localDescriptor->getReferenceCountBase())
               delete localDescriptor;
          localDescriptor = NULL;
        }

  // printf ("In ~PARTI_Descriptor(): delete representation \n");
     if (representation != NULL)
        {
       // printf ("representation->getReferenceCount() = %d \n",representation->getReferenceCount());
          representation->decrementReferenceCount();
          if (representation->getReferenceCount() < representation->getReferenceCountBase())
               delete representation;
          representation = NULL;
        }

  // delete representation;
  // globalDomain    = NULL;
  // localDomain     = NULL;
  // representation  = NULL;
  // localDescriptor = NULL;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PARTI_Descriptor ( const UserLocalDomain *inputGlobalDomain
                   , const UserLocalDomain *inputLocalDomain
                   , const PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> *inputRepresentation
                   , const UserGlobalDomain *inputLocalDescriptor
		   ) {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE: PARTI_Descriptor<UC,ULD,UD> ::"
               << " PARTI_Descriptor(UserLocalDomain*,PARTI_Representation*) \n";
#endif

     referenceCount  = getReferenceCountBase();
     if (inputGlobalDomain != NULL)
          inputGlobalDomain->incrementReferenceCount();
     globalDomain    = (UserLocalDomain*) inputGlobalDomain;
     if (inputLocalDomain != NULL)
          inputLocalDomain->incrementReferenceCount();
     localDomain     = (UserLocalDomain*) inputLocalDomain;

  // The only remaining reference to this object is through the descriptor so 
  // we would have to decrement the reference count (instead the just don't increment it)!
  // I think there should be two references to this object (i.e. from the PADRE_Representation as well)
     if (inputRepresentation != NULL)
          inputRepresentation->incrementReferenceCount();
     representation  = (PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>*) inputRepresentation;
     if (inputLocalDescriptor != NULL)
          inputLocalDescriptor->incrementReferenceCount();
     localDescriptor = (UserGlobalDomain*) inputLocalDescriptor;

  // printf ("PADRE: Leaving PARTI_Descriptor(UserLocalDomain*,PARTI_Representation*) \n");
     // testConsistency ("At BASE of PARTI_Descriptor(UserLocalDomain*,PARTI_Representation*)" point B);
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PARTI_Descriptor()
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE: PARTI_Descriptor<UC,ULD,UD> ::"
               << " PARTI_Descriptor() "
               << " not implemented\n";
#endif

     printf ("This default constructor should not have been called! \n");
     PADRE_ABORT();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PARTI_Descriptor( const PARTI_Descriptor & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE: PARTI_Descriptor<UC,ULD,UD> ::"
               << " PARTI_Descriptor( const PARTI_Descriptor & X ) "
               << " not implemented\n";
#endif

     printf ("The copy constructor should not be called! \n");
     PADRE_ABORT();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> &
PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
operator= ( const PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain > & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE: PARTI_Descriptor<UC,ULD,UD> & operator="
               << " ( const PARTI_Descriptor<UC,ULD,UD> & X ) not implemented\n";
#endif

     PADRE_ABORT();
     return *this;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void 
PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
testConsistency( const char *label ) const
   {
  // The purpose of this function is to fill in the 
  // local descriptor but not allocate the local data

     PADRE_ASSERT (referenceCount > 0);
     PADRE_ASSERT (representation != NULL);
     PADRE_ASSERT (globalDomain != NULL);
  // PADRE_ASSERT (localDomain != NULL);
     PADRE_ASSERT (localDescriptor != NULL);
     // testConsistency2( "testConsistency2 called from testConsistency" );
   }




template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
testConsistency2( const char *label ) const {
// *wdh* 050620 -- this function fails to compile with gcc 3.4.3 
//    It uses MAX_ARRAY_DIMENSION etc. that are not defined.
//    I don't think this function is every used?
// *wdh*  
// *wdh* #if defined(PADRE_DEBUG_IS_ENABLED)
// *wdh*   if (PADRE::debugLevel() > 0)
// *wdh*     cout << "PADRE: PARTI_Descriptor<UC,ULD,UD> ::"
// *wdh* 	 << " testConsistency2"
// *wdh* 	 << "Label: " << label
// *wdh* 	 << endl
// *wdh*       ;
// *wdh* #endif
// *wdh*   int i;
// *wdh*   /*
// *wdh*     The next block is the block copied over from P++.
// *wdh*   */
// *wdh*   {
// *wdh*     for (i=0; i < representation->BlockPartiArrayDescriptor->nDims; i++) {
// *wdh*       // Skip error checking in case of gLBnd(Array_Descriptor->BlockPartiArrayDomain,i) < 0 
// *wdh*       // -1 is the error code returned by the PARTI library (meaning that the dimension was out of range)
// *wdh*       // (Actually meaning that the data is not owned by this processor).
// *wdh*       int PARTI_Parallel_Descriptor_Lower_Bound = gLBnd(representation->BlockPartiArrayDescriptor,i);
// *wdh*       // Trap out error return value for PARTI gLBnd
// *wdh*       if ( PARTI_Parallel_Descriptor_Lower_Bound == -1 ) {
// *wdh* 	printf ("No array data located on this processor! \n");
// *wdh* 	printf ("Axis = %d  gLBnd(Array_Descriptor.Array_Domain.getBlockPartiArrayDomain(),i) = %d \n",
// *wdh* 		i,gLBnd(representation->BlockPartiArrayDescriptor,i));
// *wdh*       }
// *wdh*       // PADRE_ASSERT (PARTI_Parallel_Descriptor_Lower_Bound != -1);
// *wdh*       if ( PARTI_Parallel_Descriptor_Lower_Bound != -1 ) {
// *wdh* 	if ( isLeftPartition(i) ) {
// *wdh* 	  if ( localDescriptor->Left_Number_Of_Points [i] != 0 ) {
// *wdh* 	    cout << PADRE::RS() << "localDescriptor->Left_Number_Of_Points[" << i << "] = " << localDescriptor->Left_Number_Of_Points[i] << '\n'
// *wdh* 		 << PADRE::RS() << "PARTI_Parallel_Descriptor_Lower_Bound  (for Axis = " << i << ") = " << PARTI_Parallel_Descriptor_Lower_Bound << '\n'
// *wdh* 		 << PADRE::RS() << "localDescriptor->Base[" << i << "] = " << localDescriptor->Base[i] << '\n'
// *wdh* 		 << PADRE::RS() << "localDescriptor->Data_Base[" << i << "] = " << localDescriptor->Data_Base[i] << '\n'
// *wdh* 		 << endl;
// *wdh* #if 0
// *wdh* 	    printf ("localDescriptor->Left_Number_Of_Points[%d] = %d \n",i,localDescriptor->Left_Number_Of_Points[i]);
// *wdh* 	    printf ("PARTI_Parallel_Descriptor_Lower_Bound  (for Axis = %d) = %d \n",i,PARTI_Parallel_Descriptor_Lower_Bound );
// *wdh* 	    printf ("localDescriptor->Base[%d] = %d \n",i,localDescriptor->Base[i]);
// *wdh* 	    printf ("localDescriptor->Data_Base[%d] = %d \n",i,localDescriptor->Data_Base[i]);
// *wdh* #endif
// *wdh* 	    /* view is a P++ function.
// *wdh* 	       view ("ERROR: localDescriptor->Left_Number_Of_Points [i] == PARTI_Parallel_Descriptor_Lower_Bound - Array_Descriptor.Array_Domain.Base[i]");
// *wdh* 	    */
// *wdh* 	  }
// *wdh* 	  PADRE_ASSERT( localDescriptor->Left_Number_Of_Points [i] == 0 );
// *wdh* 	}
// *wdh* 	else {
// *wdh* 	  // When the right or middle processor is close enough to the left edge of the left most processor
// *wdh* 	  // (as in when the number of array elements is approximately the number of processors).
// *wdh* 	  // int Computed_Left_Number_Of_Points = PARTI_Parallel_Descriptor_Lower_Bound - 
// *wdh* 	  //      (Array_Descriptor->Base[i] + Array_Descriptor.Array_Domain.InternalGhostCellWidth[i]);
// *wdh* 	  // Divide the ghost boudary width by the Strid to compute it's contribution to the Left_Number_Of_Points
// *wdh* 	  // int Computed_Left_Number_Of_Points = PARTI_Parallel_Descriptor_Lower_Bound - 
// *wdh* 	  //      (Array_Descriptor->Base[i] + (Array_Descriptor.Array_Domain.InternalGhostCellWidth[i] / Array_Descriptor->Stride[i]));
// *wdh* 
// *wdh* 	  // The following logic supports the error checking of the Left_Number_Of_Points and is complex
// *wdh* 	  // because it accounts for the interdependence of the partitioning and the stride of access.
// *wdh* 	  // Basically if the partitioning is such that the added ghost boundary width is greater
// *wdh* 	  // than or equal to the stride then the Left_Number_Of_Points is decremented.  This relationship
// *wdh* 	  // is very simple if the stride is 1 but this code must handle the more general cases.
// *wdh* #if 1	// GIVE_UP_ON_ERROR_CHECKING
// *wdh* 	  // This force the error checking to workwhile we test another idea!
// *wdh* 	  // printf ("TEMPORARY CODE IN ERROR CHECKING! \n");
// *wdh* 	  int Computed_Left_Number_Of_Points = localDescriptor->Left_Number_Of_Points [i];
// *wdh* #else
// *wdh* 	  // OK - I give up - this is the simple way to do this (though it does less checking)
// *wdh* 	  int Computed_Left_Number_Of_Points = 
// *wdh* 	    localDescriptor->Local_Mask_Index [i].getBase() - 
// *wdh* 	    localDescriptor->Global_Index [i].getBase();
// *wdh* 	  // Computed_Left_Number_Of_Points -= 
// *wdh* 	  //      (Array_Descriptor->Array_Conformability_Info == NULL) ? 0 :
// *wdh* 	  //      Array_Descriptor->Array_Conformability_Info->Lhs_Left_Number_Of_Points_Truncated;
// *wdh* #endif
// *wdh* 	  if ( localDescriptor->Left_Number_Of_Points [i] != Computed_Left_Number_Of_Points ) {
// *wdh* 	    printf ("Computed_Left_Number_Of_Points = %d \n",Computed_Left_Number_Of_Points);
// *wdh*                                 // printf ("Offset_For_Stride = %d \n",Offset_For_Stride);
// *wdh* 	    printf ("localDescriptor->Left_Number_Of_Points[%d] = %d \n",i,localDescriptor->Left_Number_Of_Points[i]);
// *wdh* 	    printf ("PARTI_Parallel_Descriptor_Lower_Bound  (for Axis = %d) = %d \n",i,PARTI_Parallel_Descriptor_Lower_Bound );
// *wdh* 	    printf ("localDescriptor->Base[%d] = %d \n",i,localDescriptor->Base[i]);
// *wdh* 	    printf ("localDescriptor->Data_Base[%d] = %d \n",i,localDescriptor->Data_Base[i]);
// *wdh* 	    /*
// *wdh* 	      view is a P++ function
// *wdh* 	      view ("ERROR: localDescriptor->Left_Number_Of_Points [i] == PARTI_Parallel_Descriptor_Lower_Bound - (localDescriptor->Base[i] + localDescriptor->InternalGhostCellWidth[i])");
// *wdh* 	    */
// *wdh* 	  }
// *wdh* 	  PADRE_ASSERT( localDescriptor->Left_Number_Of_Points [i] == Computed_Left_Number_Of_Points );
// *wdh* 	}
// *wdh*       }
// *wdh*     }
// *wdh* 
// *wdh*     // I think that the use of the Bound[?] below does not get computed correctly
// *wdh*     // if we have a view of a view (so the wrong case would be processed)  We can
// *wdh*     // debug this case at a later date!   ?????
// *wdh*     // PADRE_ASSERT (Array_Descriptor->isView() == FALSE);
// *wdh* 
// *wdh* #if 0	// This seems to do nothing!  BTNG
// *wdh*     // These are the bounds of the actual data (with assumed base of ZERO)
// *wdh*     // if we just use the Array_Descriptor->Bound[?] then we get the bound of the view
// *wdh*     // (if the array object was a view).  We could use the RawArraySize member function.
// *wdh*     int Bounds[MAX_ARRAY_DIMENSION];
// *wdh*     Bounds [0] = localDescriptor->Size[0] - 1;
// *wdh*     for ( i=1; i<MAX_ARRAY_DIMENSION; i++ ) {
// *wdh*       Bounds [i] = (localDescriptor->Size[i] / localDescriptor->Size[i-1]) - 1;
// *wdh*     }
// *wdh* #endif
// *wdh* 
// *wdh*     int SerialArray_Sizes [MAX_ARRAY_DIMENSION];
// *wdh*     // ??????????????????
// *wdh*     // Array_Descriptor.SerialArray->Array_Descriptor.getRawDataSize(SerialArray_Sizes); 
// *wdh*     globalDomain->getRawDataSize(SerialArray_Sizes);
// *wdh* 
// *wdh*     for (i=0; i < globalDomain->Domain_Dimension; i++) {
// *wdh*       // Skip error checking in case of gLBnd(Array_Descriptor->BlockPartiArrayDomain,i) < 0
// *wdh*       // -1 is the error code returned by the PARTI library (meaning that the dimension was out of range)
// *wdh*       int PARTI_Parallel_Descriptor_Upper_Bound = gUBnd(representation->BlockPartiArrayDescriptor,i);
// *wdh*       // Trap out error return value for PARTI gUBnd
// *wdh*       if ( PARTI_Parallel_Descriptor_Upper_Bound == -1 ) {
// *wdh* 	printf ("PARTI_Parallel_Descriptor_Upper_Bound = %d \n",PARTI_Parallel_Descriptor_Upper_Bound);
// *wdh* 	printf ("SerialArray_Sizes [0-%d] = ", MAX_ARRAY_DIMENSION-1);
// *wdh* 	printf (IO_CONTROL_STRING_MACRO_INTEGER,ARRAY_TO_LIST_MACRO(SerialArray_Sizes));
// *wdh* 	printf ("\n");
// *wdh* 	printf ("globalDomain->Bound[%d] = %d \n",
// *wdh* 		i,globalDomain->Bound[i]);
// *wdh* 	printf ("Axis = %d  gUBnd(globalDomain->getBlockPartiArrayDomain(),i) = %d \n",
// *wdh* 		i,gUBnd(representation->BlockPartiArrayDescriptor,i));
// *wdh*       }
// *wdh* 	 // PADRE_ASSERT (PARTI_Parallel_Descriptor_Upper_Bound != -1);
// *wdh*       if ( PARTI_Parallel_Descriptor_Upper_Bound != -1 ) {
// *wdh* 	if (isRightPartition(i)) {
// *wdh* 	  if ( localDescriptor->Right_Number_Of_Points[i] != 0 ) {
// *wdh* 	    printf ("localDescriptor->Right_Number_Of_Points[%d] = %d \n",i,localDescriptor->Right_Number_Of_Points[i]);
// *wdh* 	  }
// *wdh* 	  PADRE_ASSERT( localDescriptor->Right_Number_Of_Points[i] == 0 );
// *wdh* 	}
// *wdh* 	else {
// *wdh* 	  // The following logic supports the error checking of the Left_Number_Of_Points and is complex
// *wdh* 	  // because it accounts for the interdependence of the partitioning and the stride of access.
// *wdh* 	  // Basically if the partitioning is such that the added ghost boundary width is greater
// *wdh* 	  // than or equal to the stride then the Left_Number_Of_Points is decremented.  This relationship
// *wdh* 	  // is very simple if the stride is 1 but this code must handle the more general cases.
// *wdh* #if 1	// GIVE_UP_ON_ERROR_CHECKING
// *wdh* 	  // This force the error checking to workwhile we test another idea!
// *wdh* 	  // printf ("TEMPORARY CODE IN ERROR CHECKING! \n");
// *wdh* 	  int Computed_Right_Number_Of_Points = localDescriptor->Right_Number_Of_Points [i];
// *wdh* #else
// *wdh* 	  // OK - I give up - this is the simple way to do this (though it does less checking)
// *wdh* 	  int Computed_Right_Number_Of_Points = 
// *wdh* 	    localDescriptor->Global_Index [i].getRawBound() - 
// *wdh* 	    localDescriptor->Local_Mask_Index [i].getBound();
// *wdh* #endif
// *wdh* 	  if ( localDescriptor->Right_Number_Of_Points[i] != Computed_Right_Number_Of_Points ) {
// *wdh* 	    printf ("Computed_Right_Number_Of_Points = %d \n",Computed_Right_Number_Of_Points);
// *wdh*                                 // printf ("Offset_For_Stride = %d \n",Offset_For_Stride);
// *wdh* 	    printf ("localDescriptor->Right_Number_Of_Points[%d] = %d \n",i,localDescriptor->Right_Number_Of_Points[i]);
// *wdh* 	    printf ("PARTI_Parallel_Descriptor_Upper_Bound (for Axis = %d) = %d \n",i,PARTI_Parallel_Descriptor_Upper_Bound);
// *wdh* 	    printf ("localDescriptor->Bound[%d] = %d \n",i,localDescriptor->Bound[i]);
// *wdh* 	    /*
// *wdh* 	      view is a P++ function
// *wdh* 	      view ("ERROR: Array_Descriptor.Array_Domain.Right_Number_Of_Points[i] != Computed_Right_Number_Of_Points");
// *wdh* 	    */
// *wdh* 	  }
// *wdh* 	  PADRE_ASSERT( localDescriptor->Right_Number_Of_Points[i] == Computed_Right_Number_Of_Points );
// *wdh* 	}
// *wdh*       }
// *wdh*     }
// *wdh*   }  // end of block from P++ test code.
}





// *************************************************************************
// *************************************************************************
// ********     PARTI_Descriptor::InitializeLocalDescriptor  ***************
// ******** This function is the heart of the use of PARTI in PADRE ********
// *************************************************************************
// *************************************************************************

// Notes about this function:
//     I worry that references to globalDomain should be referencing the localDescriptor
//     object because the globalDomain is nomalized with respect to translation.
//     Currently the use of a localDescriptor with nozero base generates
//     a localDescriptor that is a NULL array (in at least one case from testppp.C)

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
InitializeLocalDescriptor ( UserGlobalDomain & inputLocalDescriptor, UserLocalDomain & inputLocalDomain )
   {
  // Since the InitializeLocalDescriptor() takes no parameters and I wish
  // to avoid building a special version of that member function
  // we take advantage of how it modifies the existing LocalDescriptor and
  // localDomain and we save these values -- modify them -- call the 
  // InitializeLocalDescriptor function and then restore them.
  // This is not a thread safe operation and it will have to be made 
  // thread save fairly shortly.  
     UserGlobalDomain *initialLocalDescriptor = localDescriptor;
     UserLocalDomain          *initialLocalDomain     = localDomain;

  // Fill in the temporary values
     localDescriptor = &inputLocalDescriptor;
     localDomain     = &inputLocalDomain;

     InitializeLocalDescriptor();

  // Reset the to use the initial values!
     localDescriptor = initialLocalDescriptor;
     localDomain     = initialLocalDomain;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
InitializeLocalDescriptor ()
   {
     int i;  // local index variable

  // The purpose of this function is to fill in the 
  // local descriptor but not allocate the local data
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("PARTI_Descriptor<UC,ULD,UD>::InitializeLocalDescriptor() \n");
#endif

  // localDomain->display("At TOP of PARTI_Descriptor::InitializeLocalDescriptor()");

  // We skip the construction of a Parti descriptor for the case of a Null array because
  // it is not required and because Parti could not build a descriptor for an array of ZERO size.
  // But P++ must provide a valid pointer to a valid serial Null array.
     if (localDescriptor->isNull())
        {
          int i;
// *wdh* 050620          for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
             {
               localDescriptor->setLocalMaskIndex (0,-1,1,i);
             }
          return;
        }

     PADRE_ASSERT (representation != NULL);
     PADRE_ASSERT (representation->BlockPartiArrayDescriptor    != NULL);
     PADRE_ASSERT (representation->BlockPartiArrayDecomposition != NULL);

     int Bases[PARTI_MAX_ARRAY_DIMENSION];
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
          Bases[i] = 0;

     bool Generated_A_Null_Array              = false;
     int SerialArray_Cannot_Have_Contiguous_Data = false;

  // Need to set the upper bound on the iteration to PARTI_MAX_ARRAY_DIMENSION
  // since all the variables for each dimension must be initialized.
     int j;
     for (j=0; j < PARTI_MAX_ARRAY_DIMENSION; j++)
        {
          if (j < localDescriptor->getNumberOfDimensions())
             {
               Bases[j] = gLBnd(representation->BlockPartiArrayDescriptor,j) -
                                representation->getInternalGhostCellWidth(j);
             }
            else
             {
                Bases[j] = PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::PADRE_Global_Array_Base;
             }

       // Set the base to coorespond to the global address space -- but also
       // set the Data_Base so it can be consistant with the P++ descriptor
       // This could be implemented without the function call overhead
       // represented here but this simplifies the initial implementation for now

          localDomain->setBase(Bases[j]+localDescriptor->getDataBaseVariable(j),j);

        }

#if 1
  // Call common support function for initialization of the parallel and serial domain objects
     localDescriptor->postAllocationSupport(*localDomain);
#else
  // Old code which is now superceeded by the call to localDescriptor->postAllocationSupport(j)

     for (j=0; j < PARTI_MAX_ARRAY_DIMENSION; j++)
        {
          if ( (localDescriptor->getStrideVariable(j) > 1) && (localDomain->isNullArray() == false) )
             {
               localDomain->adjustSerialDomainForStridedAllocation(j,*localDescriptor);
             }

       // This could be implemented without the function call overhead
       // represented here but this simplifies the initial implementation for
       // now
          int Left_Size_Size  = localDomain->getBase(j) - localDescriptor->getBase(j);
          int Right_Size_Size = localDescriptor->getBound(j) - localDomain->getBound(j);

          localDescriptor->setLeftNumberOfPoints ( (Left_Size_Size  >= 0) ? Left_Size_Size  : 0, j);
          localDescriptor->setRightNumberOfPoints( (Right_Size_Size >= 0) ? Right_Size_Size : 0, j);

       // localDescriptor->setGlobalIndex(localDescriptor->getBase(j),localDescriptor->getBound(j),1,j);
       // Array_Domain.Global_Index [j] = Range (getRawBase(j),getRawBound(j),getRawStride(j));
          localDescriptor->setGlobalIndex(localDescriptor->getRawBase(j),localDescriptor->getRawBound(j),localDescriptor->getRawStride(j),j);

       // Array_Descriptor->Left_Number_Of_Points [j] =
       //    (Left_Size_Size  >= 0) ? Left_Size_Size  : 0;
       // Array_Descriptor->Right_Number_Of_Points[j] =
       //    (Right_Size_Size >= 0) ? Right_Size_Size : 0;
          localDescriptor->setLeftNumberOfPoints(
             (Left_Size_Size  >= 0) ? Left_Size_Size  : 0,j);
          localDescriptor->setRightNumberOfPoints(
             (Right_Size_Size >= 0) ? Right_Size_Size : 0,j);

       // Array_Descriptor->Global_Index [j] =
       //    Index (getBase(j),(getBound(j)-getBase(j))+1,1);
       // localDescriptor->setGlobalIndex(globalDomain->getBase(j),globalDomain->getBound(j),1,j);
       // localDescriptor->setGlobalIndex(localDescriptor->getBase(j),localDescriptor->getBound(j),1,j);
          localDescriptor->setGlobalIndex(localDescriptor->getRawBase(j),localDescriptor->getRawBound(j),localDescriptor->getRawStride(j),j);

       // int ghostBoundaryWidth = Array_Descriptor->InternalGhostCellWidth[j];
          int ghostBoundaryWidth = representation->getInternalGhostCellWidth(j);
          PADRE_ASSERT (ghostBoundaryWidth >= 0);

       // SerialArray->Array_Descriptor.Array_Domain.Is_Contiguous_Data = Array_Domain.Is_Contiguous_Data;
          localDomain->setContiguousDataVariable (localDescriptor->IsContiguousDataVariable());

          if (representation->getInternalGhostCellWidth(j) > 0)
             SerialArray_Cannot_Have_Contiguous_Data = true;

          if (SerialArray_Cannot_Have_Contiguous_Data == true)
             {
            // Note: we are changing the parallel array descriptor (not done much in this function)
               localDescriptor->setContiguousDataVariable(false);
               localDomain->setContiguousDataVariable (false);
             }

	  
          int Local_Stride = localDomain->getStrideVariable(j);

          if (j < localDescriptor->getNumberOfDimensions())
             {
            // Bugfix (17/10/96)
            // The Local_Mask_Index is set to NULL in the case of repartitioning
            // an array distributed first across one processor and then across 2 processors.
            // The second processor is a NullArray and then it has a valid local array (non nullarray).
            // But the isLeftPartition isRightPartition isMiddlePartition call the isNonPartition
            // member functions and the isNonPartition relies upon the Local_Mask_Index[0] to be properly
            // setup.  The problem is that it is either not setup or sometimes setup incorrectly.
            // So we have to set it to a non-nullArray as a default.
               if (localDescriptor->getLocalMaskBase(j) > localDescriptor->getLocalMaskBound(j))
                  {
                 // This resets the Local_Mask_Index to be consistant with what it usually is going into
                 // this function. This provides a definition of Local_Mask_Index consistant with
                 // the global range of the array. We cannot let the Local_Mask_Index be a Null_Index
                 // since it would not allow the isLeftPartition isRightPartition isMiddlePartition call the isNonPartition
                 // member functions and the isNonPartition relies upon the Local_Mask_Index[0] to be properly setup.
                 // int tempStride = globalDomain->getStride(j);
                    int tempStride = 1;
                    localDescriptor->setLocalMaskIndex(localDescriptor->getBaseVariable(j),
                                                       localDescriptor->getBoundVariable(j),
                                                       tempStride, j );
                  }

            // PADRE_ASSERT (Array_Descriptor->Local_Mask_Index[j].getMode() != Null_Index);
               PADRE_ASSERT (localDescriptor->getLocalMaskBase(j) <= localDescriptor->getLocalMaskBound(j));

               bool Is_A_Left_Partition  = isLeftPartition   (j);
               bool Is_A_Right_Partition = isRightPartition  (j);
               bool Is_A_Middle_Partition= isMiddlePartition (j);

#if defined(PADRE_DEBUG_IS_ENABLED)
               if (PADRE::debugLevel() > 0)
                  {
                    printf ("Is_A_NonPartition     = %s \n",(isNonPartition(j)     == true) ? "true" : "false");
                    printf ("Is_A_Left_Partition   = %s \n",(Is_A_Left_Partition   == true) ? "true" : "false");
                    printf ("Is_A_Right_Partition  = %s \n",(Is_A_Right_Partition  == true) ? "true" : "false");
                    printf ("Is_A_Middle_Partition = %s \n",(Is_A_Middle_Partition == true) ? "true" : "false");
                  }
#endif

            // Error checking for same strides in parallel and serial array
            // PADRE_ASSERT ( localDescriptor->getStrideVariable(j) == localDomain->getStrideVariable(j) );

            // int Local_Stride = localDomain->getStrideVariable(j);

            // If the parallel array descriptor is a view then we have to
            // adjust the view we build on the local partition!

               int base_offset  = localDescriptor->getBase(j)- localDomain->getBase(j);
               int bound_offset = localDomain->getBound(j) - localDescriptor->getBound(j);

               int Original_Base_Offset_Of_View  = (localDescriptor->isAView()) ? (base_offset>0 ? base_offset : 0) : 0;
               int Original_Bound_Offset_Of_View = (localDescriptor->isAView()) ? (bound_offset>0 ? bound_offset : 0) : 0;

            // int Count = ((getLocalBound(j)-getLocalBase(j))+1)-(Original_Base_Offset_Of_View+Original_Bound_Offset_Of_View);
               int Count = ((localDomain->getBound(j)-localDomain->getBase(j))+1)-
                  (Original_Base_Offset_Of_View+Original_Bound_Offset_Of_View);


            /*
            // ... bug fix, 4/8/96, kdb, at the end the local base and
            // bound won't include ghost cells on the left and right
            // processors respectively so adjust Count ...
            */

               int Start_Offset = 0;

               if (!localDescriptor->isView())
                  {
                 /*
                 // if this is a view, the ghost boundary cells are
                 // already removed by Original_Base_Offset_Of_View and
                 // Original_Bound_Offset_Of_View
                 */
                    if (Is_A_Left_Partition)
                       {
                         Count        -= ghostBoundaryWidth;
                         Start_Offset += ghostBoundaryWidth;
                       }
                    if (Is_A_Right_Partition)
                         Count -= ghostBoundaryWidth;
                  }
               if (Count > 0)
                  {
                    int tempLocalBase  = localDomain->getBase(j) + Original_Base_Offset_Of_View + Start_Offset;
                    int tempLocalBound = tempLocalBase + ((Count - 1) * Local_Stride);
                 // Array_Domain.Local_Mask_Index[j] = 
                 //      Index(getLocalBase(j) + Original_Base_Offset_Of_View + Start_Offset, Count, Local_Stride);
                    localDescriptor->setLocalMaskIndex (tempLocalBase,tempLocalBound, Local_Stride,j);

                 // Make the SerialArray a view of the valid portion of the local partition
                    localDomain->setViewVariable(true);

                 // ... (bug fix, 4/8/96, kdb) move bases and bounds in
                 // on edge partitions so ghost cells aren't included ...

                 /*
                 // ... (bug fix, 5/21/96,kdb) bases and bounds need to
                 // be adjusted by Original_Base_Offset_Of_View and
                 // Original_Bound_Offset_Of_View no matter what processor ...
                 */

                    localDomain->setBaseVariable (localDomain->getBaseVariable (j) + Original_Base_Offset_Of_View, j);
                    localDomain->setBoundVariable(localDomain->getBoundVariable(j) - Original_Bound_Offset_Of_View,j);
                 // ... bug fix (8/26/96, kdb) User_Base must reflect the view ...
                    localDomain->setUserBaseVariable ( localDomain->getUserBaseVariable (j) + Original_Base_Offset_Of_View, j );

                    if (!localDescriptor->isView())
                       {
                         if (Is_A_Left_Partition)
                            {
                              localDomain->setBaseVariable(localDomain->getBaseVariable (j)+ghostBoundaryWidth,j);
                           // ... bug fix (8/26/96, kdb) User_Base must reflect the ghost cell ...
                              localDomain->setUserBaseVariable 
                                 ( localDomain->getUserBaseVariable(j) + ghostBoundaryWidth, j );
                            }
                         if (Is_A_Right_Partition)
                            {
                              localDomain->setBoundVariable(localDomain->getBoundVariable(j) - ghostBoundaryWidth,j);
                            }
                       }

                 // Bugfix (10/19/95) if we modify the base and bound in the
                 // descriptor then the data is no longer contiguous
                 // meaning that it is no longer binary conformable
                    if (ghostBoundaryWidth > 0)
                       {
                         localDomain->setContiguousDataVariable(false);
                       }
                  }
                 else
                  {
                    Generated_A_Null_Array = true;
                  }
             }
            else // j >= Array_Descriptor->Descriptor_Dimension
             {
               int Count = (localDomain->getBound(j) - localDomain->getBase(j)) + 1;
               if (Count > 0)
                     localDescriptor->setLocalMaskIndex(localDomain->getBase(j),localDomain->getBound(j),Local_Stride, j);
                 else
                     localDescriptor->setLocalMaskIndex(0,-1,1, j);

               if (PARTI::numberOfProcessors == 1)
                  {
                 // Check that the Index objects are the same length()
                    PADRE_ASSERT (localDescriptor->getLocalMaskLength(j) == localDescriptor->getGlobalMaskLength(j));
                  }
             }

       // ... add correct view offset and Scalar Offset ...
          if (j==0)
             {
#if 0
               localDomain->setViewOffsetVariable   ( localDomain->getBaseVariable(0) * localDomain->getStrideVariable(0));
               localDomain->setScalarOffsetVariable (-localDomain->getUserBaseVariable(0) * localDomain->getStrideVariable(0),0);
#else
               localDomain->setViewOffsetVariable   ( localDomain->getBaseVariable(0) );
               localDomain->setScalarOffsetVariable (-localDomain->getUserBaseVariable(0) * localDomain->getStrideVariable(0),0);
#endif
             }
            else
             {
#if 0
               localDomain->setViewOffsetVariable 
                   (localDomain->getViewOffsetVariable() + localDomain->getBaseVariable(j) *
                    localDomain->getStrideVariable(j)    * localDomain->getSizeVariable(j-1));
               localDomain->setScalarOffsetVariable(localDomain->getScalarOffsetVariable(j-1) -
                    localDomain->getUserBaseVariable(j) * localDomain->getStrideVariable(j) * 
                    localDomain->getSizeVariable(j-1), j);
#else
               localDomain->setViewOffsetVariable ( localDomain->getViewOffsetVariable() + 
                    localDomain->getBaseVariable(j) * localDomain->getSizeVariable(j-1));
               localDomain->setScalarOffsetVariable(localDomain->getScalarOffsetVariable(j-1) -
                    localDomain->getUserBaseVariable(j) * localDomain->getStrideVariable(j) * 
                    localDomain->getSizeVariable(j-1), j);
#endif
             }

       // Error checking for same strides in parallel and serial array
       // PADRE_ASSERT ( localDescriptor->getStrideVariable(j) == localDomain->getStrideVariable(j) );
          PADRE_ASSERT ( localDomain->isNullArray() ||
                         (localDescriptor->getStrideVariable(j) == localDomain->getStrideVariable(j)) );
        } // end of j loop

  // printf ("localDomain->getViewOffsetVariable() = %d \n",localDomain->setViewOffsetVariable());

  // ... add View_Offset to Scalar_Offset (we can't do this inside the
  // loop above because View_Offset is a sum over all dimensions). Also
  // set View_Pointers now. ...
     for (j=0; j < PARTI_MAX_ARRAY_DIMENSION; j++)
        {
          localDomain->setScalarOffsetVariable( localDomain->getScalarOffsetVariable(j) + localDomain->getViewOffsetVariable(), j);
       // printf ("localDomain->getScalarOffsetVariable(%d) = %d \n",j,localDomain->getScalarOffsetVariable());
        }

  // This is called by the application using PADRE (we might want to change that)
  // SERIAL_POINTER_LIST_INITIALIZATION_MACRO;
  // printf ("Need to call SERIAL_POINTER_LIST_INITIALIZATION_MACRO \n");

  // if we have generated a null view of a valid serial array then we have
  // to make the descriptor conform to some specific rules
  // 1. A Null_Array has to have the Is_Contiguous_Data flag false
  // 2. A Null_Array has to have the Base and Bound 0 and -1 (repectively)
  //    for ALL dimensions
  // 3. The Local_Mask_Index in the Parallel Descriptor must be a Null Index

     if (Generated_A_Null_Array == true)
        {
          localDomain->setContiguousDataVariable (false);
          localDomain->setViewVariable(true);
          localDomain->setNull();
          int j;
          for (j=0; j < PARTI_MAX_ARRAY_DIMENSION; j++)
             {
               localDescriptor->setLocalMaskIndex (0,-1,1,j);
               localDomain->setBaseVariable (0,j);
               localDomain->setBoundVariable (-1,j);
               localDomain->setUserBaseVariable(0,j);
             }
        }

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 4)
        {
          printf ("At base of PARTI_Descriptor::InitializeLocalDescriptor \n");
          localDomain->display("In PARTI_Descriptor::InitializeLocalDescriptor -- localDomain");
        }
#endif

  // This error checking only works for the single processor case!
     PADRE_ASSERT (PARTI::numberOfProcessors >= 0);
     if (PARTI::numberOfProcessors == 1)
        {
          int k;
          for (k=0; k < PARTI_MAX_ARRAY_DIMENSION; k++)
          if (localDomain->getRawBase(k) != localDescriptor->getRawBase(k))
             {
               printf ("ERROR: localDomain->getRawBase(%d) = %d != localDescriptor->getRawBase(%d) = %d \n",
                    k,localDomain->getRawBase(k),k,localDescriptor->getRawBase(k));
               localDomain->display("ERROR in PARTI_Representation::InitializeLocalDescriptor -- localDomain");
               localDescriptor->display("ERROR in PARTI_Representation::InitializeLocalDescriptor -- localDescriptor");
               PADRE_ABORT();
             }
        }
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     testConsistency ("Called from base of PARTI_Representation::InitializeLocalDescriptor");
#endif
   }


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
displayDARRAY ( DARRAY* X )
   {
  // This function prints out the information hiding in the parti DARRAY structure

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PARTI_Representation::displayDARRAY() \n");
#endif

     if (X == NULL)
        {
          printf ("Case of Null input DARRAY* in PARTI_Representation::displayDARRAY(DARRAY*) \n");
        }
       else
        {
          PADRE_ASSERT (X != NULL);

#if defined(PARALLEL_PADRE)
          int i = 0;
          printf ("Number of Dimensions (nDims) = %d \n",X->nDims);
          printf ("number of internal ghost cells in each dim (ghostCells[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %d",X->ghostCells[i]);
             }
          printf ("\n");

          printf ("total size of each dim (dimVecG[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %d",X->dimVecG[i]);
             }
          printf ("\n");

          printf ("local size of each dim for central pieces (dimVecL[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %d",X->dimVecL[i]);
             }
          printf ("\n");

          printf ("local size of each dim for left most piece (dimVecL_L[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %d",X->dimVecL_L[i]);
             }
          printf ("\n");

          printf ("local size of each dim for right most piece (dimVecL_R[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %d",X->dimVecL_R[i]);
             }
          printf ("\n");

          printf ("lower global index on my processor (g_index_low[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %d",X->g_index_low[i]);
             }
          printf ("\n");

          printf ("upper global index on my processor (g_index_hi[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %d",X->g_index_hi[i]);
             }
          printf ("\n");

          printf ("Local size on my processor (local_size[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %d",X->local_size[i]);
             }
          printf ("\n");

          printf ("dim of decomp to which each dim aligned defines how array aligned to decomp \n");
          printf ("used with decomp to inialize decompPosn and dimDist -- (decompDim[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %d",X->decompDim[i]);
             }
          printf ("\n");

          printf ("coordinate position of processor in the decomposition to which it's bound \n");
          printf ("in the multi-dimensional decomposition space -- (decompPosn[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %d",X->decompPosn[i]);
             }
          printf ("\n");

          printf ("type of distribution in each dim (dimDist[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %c",X->dimDist[i]);
             }
          printf ("\n");

          printf ("DECOMP structure inside of DARRAY \n");
          PADRE_ASSERT(X->decomp != NULL);
       // displayDECOMP ( X->decomp );
#endif
        }
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
displaySCHED ( SCHED* X )
   {
  // this function prints out the information hiding in the parti SCHED structure
  // which is generated to describe the communication to be done by the <type>DataMove function
  // in the parti library.

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PARTI_Representation::displaySCHED() \n");
#endif

     PADRE_ASSERT (X != NULL);

#if defined(PARALLEL_PADRE)
     int i = 0;
     printf ("rMsgSz[0-%d]:",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          printf (" %d",X->rMsgSz[i]);
        }
     printf ("\n");

     printf ("sMsgSz[0-%d]:",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          printf (" %d",X->sMsgSz[i]);
        }
     printf ("\n");

     if (X->type == 1)
        {
          printf ("Hash Type (type) == 1 meaning is an 'exch_sched' \n");
        }
       else
        {
          if (X->type == 2)
             {
               printf ("Hash Type (type) == 2 meaning is an 'subarray_sched' \n");
             }
            else
             {
               printf ("Hash Type (type) == %d : AN INVALID TYPE \n",X->type);
             }
        }

     printf ("Hash Bucket (hash) = %d \n",X->hash);

     printf ("rData[0-%d] (+ = VALID POINTER, * = NULL POINTER):",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          if (X->rData[i] != NULL)
               printf ("+");
            else
               printf ("*");
        }
     printf ("\n");

     printf ("rData[0-%d]->proc:",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          if (X->rData[i] != NULL)
               printf (" %d",X->rData[i]->proc);
        }
     printf ("\n");

     printf ("rData[0-%d]->numDims:",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          if (X->rData[i] != NULL)
               printf (" %d",X->rData[i]->numDims);
        }
     printf ("\n");

     printf ("rData[0-%d]->startPosn:",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          if (X->rData[i] != NULL)
               printf (" %d",X->rData[i]->startPosn);
        }
     printf ("\n");

     printf ("rData[0-%d]->numelem[0-3]:",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          if (X->rData[i] != NULL)
               printf ("(%d,%d,%d,%d)",X->rData[i]->numelem[0],X->rData[i]->numelem[1],
                                       X->rData[i]->numelem[2],X->rData[i]->numelem[3]);
        }
     printf ("\n");

     printf ("rData[0-%d]->str[0-3]:",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          if (X->rData[i] != NULL)
               printf ("(%d,%d,%d,%d)",X->rData[i]->str[0],X->rData[i]->str[1],
                                       X->rData[i]->str[2],X->rData[i]->str[3]);
        }
     printf ("\n");

     printf ("sData[0-%d] (+ = VALID POINTER, * = NULL POINTER):",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          if (X->sData[i] != NULL)
               printf ("+");
            else
               printf ("*");
        }
     printf ("\n");

     printf ("sData[0-%d]->proc:",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          if (X->sData[i] != NULL)
               printf (" %d",X->sData[i]->proc);
        }
     printf ("\n");

     printf ("sData[0-%d]->numDims:",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          if (X->sData[i] != NULL)
               printf (" %d",X->sData[i]->numDims);
        }
     printf ("\n");

     printf ("sData[0-%d]->startPosn:",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          if (X->sData[i] != NULL)
               printf (" %d",X->sData[i]->startPosn);
        }
     printf ("\n");

     printf ("sData[0-%d]->numelem[0-3]:",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          if (X->sData[i] != NULL)
               printf ("(%d,%d,%d,%d)",X->sData[i]->numelem[0],X->sData[i]->numelem[1],
                                       X->sData[i]->numelem[2],X->sData[i]->numelem[3]);
        }
     printf ("\n");

     printf ("sData[0-%d]->str[0-3]:",PARTI_numprocs()-1);
     for (i=0; i < PARTI_numprocs(); i++)
        {
          if (X->sData[i] != NULL)
               printf ("(%d,%d,%d,%d)",X->sData[i]->str[0],X->sData[i]->str[1],
                                       X->sData[i]->str[2],X->sData[i]->str[3]);
        }
     printf ("\n");

     printf ("\n");
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
displayDECOMP ( DECOMP* X )
   {
  // This function prints out the information hiding in the parti DARRAY structure

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PARTI_Representation::displayDECOMP() \n");
#endif

  // Sometimes this is NULL (as for a Null array object)
     if (X == NULL)
        {
          printf ("Case of Null input DECOMP* in PARTI_Representation::displayDECOMP(DECOMP*) \n");
        }
       else
        {
          PADRE_ASSERT (X != NULL);

#if defined(PARALLEL_PADRE)
          int i = 0;
          printf ("Number Of Dimensions (nDims)  = %d \n",X->nDims);
          printf ("Number Of Processors (nProcs) = %d \n",X->nProcs);
          printf ("Base Processor (baseProc)     = %d \n",X->baseProc);
     
          printf ("size of decomposition in each dim (dimVec[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %d",X->dimVec[i]);
             }
          printf ("\n");

          printf ("num processors allocated to each dim (dimProc[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %d",X->dimProc[i]);
             }
          printf ("\n");

          printf ("type of distribution in each dim (dimDist[0-%d]):",X->nDims-1);
          for (i=0; i < X->nDims; i++)
             {
               printf (" %c",X->dimDist[i]);
             }
          printf ("\n");
          printf ("\n");
#endif
        }
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
displayDefaultValues ( const char *label )
   {
  // We can get more detailed code directly from the P++ implementation
     printf ("Inside of PARTI_Distribution::displayDefaultValues (%s) \n",label);

     printf ("defaultStartingProcessor = %d \n",defaultStartingProcessor);
     printf ("defaultEndingProcessor   = %d \n",defaultEndingProcessor);

     printf ("defaultDistributionDimension = %d \n",defaultDistributionDimension);
  // printf ("defaultGhostCellWidth = %d \n",defaultGhostCellWidth);

     int i = 0;
     printf ("defaultDistribution_String ([0-%d]):",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %c",defaultDistribution_String[i]);
        }
     printf ("\n");

     printf ("defaultArrayDimensionsToAlign ([0-%d]):",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %d",defaultArrayDimensionsToAlign[i]);
        }
     printf ("\n");

     printf ("defaultGhostCellWidth ([0-%d]):",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %d",defaultGhostCellWidth[i]);
        }
     printf ("\n");

     printf ("defaultExternalGhostCellArrayLeft ([0-%d]):",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %d",defaultExternalGhostCellArrayLeft[i]);
        }
     printf ("\n");

     printf ("defaultExternalGhostCellArrayRight ([0-%d]):",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %d",defaultExternalGhostCellArrayRight[i]);
        }
     printf ("\n");

     printf ("defaultPartitionControlFlags ([0-%d]):",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %d",defaultPartitionControlFlags[i]);
        }
     printf ("\n");

     printf ("defaultDecomposition_Dimensions ([0-%d]):",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
          printf (" %d",defaultDecomposition_Dimensions[i]);
        }
     printf ("\n");
   }

// These may no longer be required since we now represent default values 
// of PARTI_Representation objects a little differently???
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
displayDefaultValues ( const char *label )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PARTI_Representation::displayDefaultValues (%s) \n",label);
#endif

     printf ("PARTI_Representation::displayDefaultValues (label) --- not implemented \n");
     PADRE_ABORT();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
displayDefaultValues ( const char *label )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PARTI_Descriptor::displayDefaultValues (label) \n");
#endif

     printf ("PARTI_Descriptor::displayDefaultValues (label) --- not implemented \n");
     PADRE_ABORT();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
display ( const char *label ) const
   {
     printf ("Inside of PARTI_Descriptor::display (%s) \n",label);
     PADRE_ASSERT (globalDomain != NULL);
     printf ("In PARTI_Descriptor::display() -- globalDomain->display() commented out! \n");
  // globalDomain->display(label);
     PADRE_ASSERT (representation != NULL);
  // printf ("In PARTI_Descriptor::display() -- representation->display() commented out! \n");
     representation->display(label);

     PADRE_ASSERT (representation != NULL);
     if (representation->BlockPartiArrayDescriptor != NULL)
        {
#if 1
       // These require the BlockPartiArrayDescriptor internally to determin the partition position
       // isNonPartition requires the Local_Mask to be properly set
          printf ("Is NOT PARTITIONED ==========(requires Local_Mask to be properly set)==================== ");
          int i=0;
          int Domain_Dimension = globalDomain->numberOfDimensions();
          for (i=0; i < Domain_Dimension; i++)
               printf (" %s \n",(isNonPartition(i))   ? "true" : "false");
          printf ("\n");

          printf ("Is a LEFT PARTITION =========(uses BlockPartiArrayDescriptor data)======================= ");
          for (i=0; i < Domain_Dimension; i++)
               printf (" %s \n",(isLeftPartition(i))   ? "true" : "false");
          printf ("\n");

          printf ("Is a MIDDLE PARTITION =======(uses BlockPartiArrayDescriptor data)======================= ");
          for (i=0; i < Domain_Dimension; i++)
               printf (" %s \n",(isMiddlePartition(i))   ? "true" : "false");
          printf ("\n");

          printf ("Is a RIGHT PARTITION ========(uses BlockPartiArrayDescriptor data)======================= ");
          for (i=0; i < Domain_Dimension; i++)
               printf (" %s \n",(isRightPartition(i))   ? "true" : "false");
          printf ("\n");
#endif
        }
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
displayReferenceCounts ( const char *label ) const
   {
     printf ("     pPARTI_Descriptor->getReferenceCount() = %d \n",getReferenceCount());
     if (globalDomain != NULL)
        {
          printf ("     globalDomain->getReferenceCount() = %d \n",globalDomain->getReferenceCount());
        }

     if (localDomain != NULL)
          printf ("    localDomain->getReferenceCount() = %d\n",localDomain->getReferenceCount());
     if (localDescriptor != NULL)
          printf ("    localDescriptor->getReferenceCount()=%d\n",localDescriptor->getReferenceCount());
     if (representation != NULL)
        {
          representation->displayReferenceCounts(label);

        }
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
display ( const char *label ) const
   {
     printf ("Inside of PARTI_Representation::display (label) \n");

     PADRE_ASSERT (globalDomain != NULL);
     printf ("In PARTI_Representation::display -- globalDomain->display() commented out! \n");
  // globalDomain->display(label);
     PADRE_ASSERT (Distribution != NULL);
  // printf ("In PARTI_Representation::display -- Distribution->display() commented out! \n");
     Distribution->display(label);

  // Pointer to Block-Parti parallel decomposition (object)
     printf ("BlockPartiArrayDecomposition is %s \n",(BlockPartiArrayDecomposition == NULL) ? "NULL" : "VALID POINTER");
  // Pointer to Block-Parti parallel distributed array descriptor
     printf ("BlockPartiArrayDescriptor is %s \n",(BlockPartiArrayDescriptor == NULL) ? "NULL" : "VALID POINTER");

     if (BlockPartiArrayDescriptor != NULL)
        {
          displayDECOMP(BlockPartiArrayDecomposition);
        }

     if (BlockPartiArrayDescriptor != NULL)
        {
          displayDARRAY(BlockPartiArrayDescriptor);
        }

     if (BlockPartiArrayDescriptor != NULL)
        {
          int i=0;
          int Domain_Dimension = globalDomain->numberOfDimensions();
          printf ("PARTI -- ((LOCAL) START INDEX of the range) lalbnd ====================================== ");
          for (i=0; i < Domain_Dimension; i++)
               printf (" %d ",lalbnd(BlockPartiArrayDescriptor,i,globalDomain->getBaseVariable(i),1));
          printf ("\n");

          printf ("PARTI -- ((LOCAL) LAST INDEX of the range) laubnd  ====================================== ");
          for (i=0; i < Domain_Dimension; i++)
               printf (" %d ",laubnd(BlockPartiArrayDescriptor,i,globalDomain->getBoundVariable(i),1));
          printf ("\n");

          printf ("PARTI -- (Local sizes) laSizes  ========================================================= ");
// *wdh* 050620           int Sizes[MAX_ARRAY_DIMENSION];
          int Sizes[PARTI_MAX_ARRAY_DIMENSION];
          laSizes(BlockPartiArrayDescriptor,Sizes);
          for (i=0; i < Domain_Dimension; i++)
               printf (" %d ",globalDomain->getSizeVariable(i));
          printf ("\n");

          printf ("PARTI -- (LOWER bound of the range of global indices stored locally) gLBnd ============== ");
          for (i=0; i < Domain_Dimension; i++)
               printf (" %d ",gLBnd(BlockPartiArrayDescriptor,i));
          printf ("\n");

          printf ("PARTI -- (UPPER bound of the range of global indices stored locally) gUBnd ============== ");
          for (i=0; i < Domain_Dimension; i++)
               printf (" %d ",gUBnd(BlockPartiArrayDescriptor,i));
          printf ("\n");

#if 1
          printf ("PARTI -- (LOCAL INDEX OF THE GLOBAL INDEX (or -1 if not owned)) globalToLocal =========== \n");
          for (i=0; i < Domain_Dimension; i++)
             {
               int Start = globalDomain->getDataBaseVariable(i)+globalDomain->getBaseVariable(i);
               int End   = globalDomain->getDataBaseVariable(i)+globalDomain->getBoundVariable(i);
               printf ("          Dimension %d (Global Range: %d-%d): ",i,Start,End);
               for (int j=Start; j <= End; j++)
                    printf ("%d ",globalToLocal(BlockPartiArrayDescriptor,j,i));
               printf ("\n");
             }
#endif /* end of if 0 else */

#if 1
          printf ("PARTI -- (LOCAL INDEX OF THE GLOBAL INDEX (or -1 if not owned)) globalToLocalWithGhost == \n");
          for (i=0; i < Domain_Dimension; i++)
             {
               int Start = (globalDomain->getDataBaseVariable(i)+globalDomain->getBaseVariable(i) )-
                            getPARTI_Distribution().getInternalGhostCellWidth(i);
               int End   = (globalDomain->getDataBaseVariable(i)+globalDomain->getBoundVariable(i))+
                            getPARTI_Distribution().getInternalGhostCellWidth(i);
               printf ("          Dimension %d (Global Range: %d-%d): ",i,Start,End);
               for (int j=Start; j <= End; j++)
                    printf ("%d ",globalToLocalWithGhost(BlockPartiArrayDescriptor,j,i));
               printf ("\n");
             }
#endif /* end of if 0 else */

#if 1
          printf ("PARTI -- (GLOBAL INDEX OF THE LOCAL INDEX (or -1 if not owned)) localToGlobal == \n");
          for (i=0; i < Domain_Dimension; i++)
             {
               int Start = lalbnd(BlockPartiArrayDescriptor,i,globalDomain->getBaseVariable(i),1);
               int End   = laubnd(BlockPartiArrayDescriptor,i,globalDomain->getBoundVariable(i),1);
               printf ("          Dimension %d (Local Range: %d-%d): ",i,Start,End);
               for (int j=Start; j <= End; j++)
                    printf ("%d ",localToGlobal(BlockPartiArrayDescriptor,j,i));
               printf ("\n");
             }
#endif /* end of if 0 else */

#if 1
          printf ("PARTI -- (GLOBAL INDEX OF THE LOCAL INDEX (or -1 if not owned)) localToGlobalWithGhost == \n");
          for (i=0; i < Domain_Dimension; i++)
             {
               int Start = lalbnd(BlockPartiArrayDescriptor,i,globalDomain->getBaseVariable(i),1) - 
                           getPARTI_Distribution().getInternalGhostCellWidth(i);
               int End   = laubnd(BlockPartiArrayDescriptor,i,globalDomain->getBoundVariable(i),1) + 
                           getPARTI_Distribution().getInternalGhostCellWidth(i);
               printf ("          Dimension %d (Local Range: %d-%d): ",i,Start,End);
               for (int j=Start; j <= End; j++)
                    printf ("%d ",localToGlobalWithGhost(BlockPartiArrayDescriptor,j,i));
               printf ("\n");
             }
#endif /* end of if 0 else */
        }
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
displayReferenceCounts ( const char *label ) const
   {
     printf ("     pPARTI_Representation->getReferenceCount() = %d \n",getReferenceCount());

     printf ("    Distribution->getReferenceCount() = %d \n",Distribution->getReferenceCount());
     if (Distribution != NULL)
        {
          printf ("Distribution->getReferenceCount() = %d \n",
               Distribution->getReferenceCount());
        }

     if (BlockPartiArrayDecomposition != NULL)
        {
          printf ("BlockPartiArrayDecomposition->referenceCount = %d \n",
               BlockPartiArrayDecomposition->referenceCount);
        }
     if (BlockPartiArrayDescriptor != NULL)
        {
          printf ("BlockPartiArrayDescriptor->referenceCount = %d \n",
               BlockPartiArrayDescriptor->referenceCount);
        }
   }


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
isLeftPartition(int Axis) const
   {
  // Left_Number_Of_Points and Right_Number_Of_Points start counting at the left and right most
  // ghost boundaries which on small arrays (with the adjacent processor having a number of
  // elements in the partition equal to the ghost boundary width) can mean that a partition might
  // mistakenly be considered to be the leftmost processor.  We use the PARTI descriptor to
  // resolve this since it assumes the world starts at zero (P++ makes no such assumption).

     bool Result = false;

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 6)
          printf ("Inside of PARTI_Representation::isLeftPartition(int i) const \n");
#endif

  // printf ("Axis = %d \n",Axis);
     if (!isNonPartition(Axis))
        {
          int Domain_Dimension = globalDomain->numberOfDimensions();
          if ( Axis < Domain_Dimension)
             {
               PADRE_ASSERT(representation != NULL);
               PADRE_ASSERT(representation->BlockPartiArrayDescriptor != NULL);
               Result = (gLBnd(representation->BlockPartiArrayDescriptor,Axis) == 0);
             }
            else
             {
            // Along higher axes we want to force isLeftPartition == true
               Result = true;
             }
        }

     return Result;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
isMiddlePartition(int Axis) const
   {
  // Left_Number_Of_Points and Right_Number_Of_Points start counting at the left and right most
  // ghost boundaries which on small arrays (with the adjacent processor having a number of
  // elements in the partition equal to the ghost boundary width) can mean that a partition might
  // mistakenly be considered to be the leftmost processor.  We use the PARTI descriptor to
  // resolve this since it assumes the world starts at zero (P++ makes no such assumption).

     bool Result = false;

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 6)
          printf ("Inside of PARTI_Representation::isMiddlePartition(int i) const \n");
#endif

  // printf ("Axis = %d \n",Axis);
     if (!isNonPartition(Axis))
        {
          int Domain_Dimension = globalDomain->numberOfDimensions();
          if ( Axis < Domain_Dimension)
             {
               Result = ( (isLeftPartition(Axis) || isRightPartition(Axis)) ) ? false : true;

            // if (Result && Is_A_Null_Array)
            //    {
            //      printf ("Inside of $2Array_Domain_Type::isMiddlePartition -- Middle_Processor is a NULL_ARRAY \n");
            //    }
             }
            else
             {
            // Along higher axes we want to force isMiddlePartition == true
               Result = false;
             }
        }

     return Result;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
isRightPartition(int Axis) const
   {
  // Left_Number_Of_Points and Right_Number_Of_Points start counting at the left and right most
  // ghost boundaries which on small arrays (with the adjacent processor having a number of
  // elements in the partition equal to the ghost boundary width) can mean that a partition might
  // mistakenly be considered to be the leftmost processor.  We use the PARTI descriptor to
  // resolve this since it assumes the world starts at zero (P++ makes no such assumption).

     bool Result = false;

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 6)
          printf ("Inside of PARTI_Representation::isRightPartition(int i) const \n");
#endif

  // printf ("Axis = %d \n",Axis);
     if (!isNonPartition(Axis))
        {
          int Domain_Dimension = globalDomain->numberOfDimensions();
          if ( Axis < Domain_Dimension)
             {
               PADRE_ASSERT(representation != NULL);
               PADRE_ASSERT(representation->BlockPartiArrayDescriptor != NULL);
               int Array_Size_Data [PARTI_MAX_ARRAY_DIMENSION];
               globalDomain->getRawDataSize (Array_Size_Data);
               Result = (gUBnd(representation->BlockPartiArrayDescriptor,Axis) == Array_Size_Data[Axis]-1);
             }
            else
             {
            // Along higher axes we want to force isRightPartition == true
               Result = true;
             }
        }

     return Result;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
isNonPartition(int Axis) const
   {
  // Check to see if local processor has a part of the P++ array object
  // since not all processors have to own a part of every array object.

  // (bug fix 3/5/96 kdb) BlockPartiArrayDomain doesn't
  // reflect views so use Local_Mask_Index instead if any are
  // a Null_Index all should be so check axis 0

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 3)
          printf ("Inside of PARTI_Representation::isNonPartition(int %d) const \n",Axis);
#endif

  // bool Result = (Local_Mask_Index[0].getlength() == 0);
     PADRE_ASSERT (localDescriptor != NULL);
  // We don't have access to the localDescriptor so we have to implement this differently!
  // I don't know if this implementation is sufficient.
     bool Result = (localDescriptor->getLocalMaskLength(0) == 0);
  // bool Result = true;
  // if (gLBnd(BlockPartiArrayDescriptor,0) != -1)
  //      Result = (gLBnd(BlockPartiArrayDescriptor,0) < gUBnd(BlockPartiArrayDescriptor,0));
      
     return Result;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
swapDistribution 
   ( const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & oldDistribution ,
     const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & newDistribution )
   {

#if 1
  // for now lets assume these are of length zero (but this is not always true)
  // the header file contains a description of the purpose of these variables
  // PADRE_ASSERT (Internal_Partitioning_Type::DefaultPartitioningObjectList.getLength() == 0);
  // PADRE_ASSERT (oldDistribution.PartitioningObjectList.getLength() == 0);
  // PADRE_ASSERT (newDistribution.PartitioningObjectList.getLength() == 0);

  // Used in the assertion at the base of this function!
     int OldReferenceCountAtStart = oldDistribution.getReferenceCount();

  // We don't want to touch the default lists since this is where the array objects
  // are stored which have a default distribution.  The default distributions are not
  // dynamic in the same way.  Specifically an array not built using a partitioning object is
  // assigned a defualt distribution.  The array objects "partition()" member function can be used
  // to assign a new partition object to the array but the default partition object can not
  // be changed once arrays objects are associated with it.  If an array object was build using
  // a partition object (or explicitly associated with a specific partition object at a later point)
  // then the partition object can be dynamically changed and the associated array objects will
  // be repartitioned automatically.

     PADRE_ASSERT (oldDistribution.getUserCollectionList() != NULL);
     PADRE_ASSERT (newDistribution.getUserCollectionList() != NULL);

     int LengthOfUserCollectionList = oldDistribution.getUserCollectionList()->size();
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("LengthOfUserCollectionList = %d \n",LengthOfUserCollectionList);
#endif

  // At a later time we want to have representations be shared rather than forcing
  //new ones to be built for each distribution object that chages its distribution.
#if 1
  // We have to make a copy ofthe list of UserCollection objects since we want to know
  // the UserCollection object which will be associated with new distributions
  // however to change the distribution we call the UserCollection's partition
  // member function and htis function will remove the UserCollection from the list internally.
  // If we don't make a new list then the partition member function will modify the list
  // as we are iterating through it and this will creat an infinite loop starting at the 
  // LAST element in the list (as it is removed and the "i != end()" becomes true!).

  // Make a copy of the STL list
     list<UserCollection*> tempList (*oldDistribution.getUserCollectionList());

     PADRE_ASSERT (tempList.size() == oldDistribution.getUserCollectionList()->size());

  // int Counter = 0;
     // *wdh* for (list<UserCollection*>::iterator i = tempList.begin(); i != tempList.end(); i++)
     typedef typename list<UserCollection*>::iterator UserCollectionList_Iterator;
     for (UserCollectionList_Iterator i = tempList.begin(); i != tempList.end(); i++)
        {
       // if (PADRE::debugLevel() > 0)
       //      printf ("Changing the distribution of UserCollection (Array_ID=%d) (%d of %d in list) \n",
       //           (*i)->Array_ID(), Counter, tempList.size() );

       // printf ("BEFORE PARTITION CALL newDistribution.getReferenceCount() = %d \n",
       //      newDistribution.getReferenceCount());
       // printf ("(*i)->partition commented out: In PADRE_Distribution::swapDistribution()! \n");
          (*i)->partition ( &((PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> &) 
               newDistribution) );
       // printf ("AFTER PARTITION CALL  newDistribution.getReferenceCount() = %d \n",
       //      newDistribution.getReferenceCount());
       // Counter++;
        }

  // printf ("Exiting in PARTI_Descriptor::swapDistribution() -- AFTER (*i)->partition() \n");
#else
  // swapping a Distribution for another should not effect everything using that
  // distribution, but it should effect everything using the representation but at
  // this level we don't have a record of that!
     int Counter = 0;
     for (list<UserCollection*>::iterator i = oldDistribution.getUserCollectionList()->begin();
          i != oldDistribution.getUserCollectionList()->end(); i++)
        {
       // if (PADRE::debugLevel() > 0)
       //      printf ("Changing the distribution of UserCollection (Array_ID=%d) (%d of %d in list) \n",
       //           (*i)->Array_ID(), Counter, 
       //           oldDistribution.getUserCollectionList()->size() );

          printf ("BEFORE PARTITION CALL newDistribution.getReferenceCount() = %d \n",
               newDistribution.getReferenceCount());
          printf ("(*i)->partition (PADRE_Distribution) commented out! \n");
          (*i)->partition ( &((PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> &) 
               newDistribution) );
          printf ("AFTER PARTITION CALL  newDistribution.getReferenceCount() = %d \n",
               newDistribution.getReferenceCount());
          Counter++;
        }
#endif

  // printf ("AFTER LOOP CALL  newDistribution.getReferenceCount() = %d \n",
  //      newDistribution.getReferenceCount());
     
  // The reference count on the PADRE_Discriptor is not associated with the number of
  // array objects in the list!  So this code is not the correct sort of error checking.
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
        {
          printf ("OldReferenceCountAtStart = %d \n",OldReferenceCountAtStart);
          printf ("oldDistribution.getReferenceCount() = %d \n",oldDistribution.getReferenceCount());
          printf ("newDistribution.getReferenceCount() = %d \n",newDistribution.getReferenceCount());
        }
#endif

#endif
   }


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
SCHED *PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
BuildCommunicationScheduleRegularSectionTransfer ( 
     const UserGlobalDomain & Lhs,
     const UserGlobalDomain & Rhs )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PARTI_Descriptor::BuildCommunicationScheduleRegularSectionTransfer()", 
                 "void(void)", TAU_PADRE_COMMUNICATION_OVERHEAD_MANAGEMENT);
#endif

// ****************************************************************************************************
// These functions move a rectangular part of the Rhs (which might be distributed over many processors)
// to the processors owning the specified rectangular part of the Lhs (the operation would typically
// be between unaligned arrays and thus is a basis for the unaligned array operations).
// In cases of no ghost boundaries (width zero) this is an expensive substitute for the
// use of the ghost boundary update.
// ****************************************************************************************************

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of BuildCommunicationScheduleRegularSectionTransfer(Array_Domain_Type,Array_Domain_Type) \n");
#endif

#if defined(PARALLEL_PADRE)
     int Max_Dimension = (Lhs.Domain_Dimension > Rhs.Domain_Dimension) ?
                          Lhs.Domain_Dimension : Rhs.Domain_Dimension;

     int Lhs_Dimension_Array [PARTI_MAX_ARRAY_DIMENSION];
     int Rhs_Dimension_Array [PARTI_MAX_ARRAY_DIMENSION];
     int Lhs_Base_Array      [PARTI_MAX_ARRAY_DIMENSION];
     int Rhs_Base_Array      [PARTI_MAX_ARRAY_DIMENSION];
     int Lhs_Bound_Array     [PARTI_MAX_ARRAY_DIMENSION];
     int Rhs_Bound_Array     [PARTI_MAX_ARRAY_DIMENSION];
     int Lhs_Stride_Array    [PARTI_MAX_ARRAY_DIMENSION];
     int Rhs_Stride_Array    [PARTI_MAX_ARRAY_DIMENSION];

     Lhs.getRawDataSize (Lhs_Dimension_Array);
     Rhs.getRawDataSize (Rhs_Dimension_Array);

  // This would be more efficient but for now we should initialize the whole array
  // for (int i=0; i < Max_Dimension; i++)
     for (int i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
        {
       // Parti requires that the unused dimensions should have -1 as an entry
          if ( i > Lhs.Domain_Dimension-1 )
               Lhs_Dimension_Array [i] = -1;
            else
               Lhs_Dimension_Array [i] = i;

          if ( i > Rhs.Domain_Dimension-1 )
               Rhs_Dimension_Array [i] = -1;
            else
               Rhs_Dimension_Array [i] = i;

       // We could make this more efficient by just providing pointers to
       // the Array_Descriptor Base Bound and Stride arrays directly.  But for now we will
       // sperate the two to avoid any potential problems
          Lhs_Base_Array [i] = Lhs.Base[i];
          Rhs_Base_Array [i] = Rhs.Base[i];

          Lhs_Bound_Array [i] = Lhs.Bound[i];
          Rhs_Bound_Array [i] = Rhs.Bound[i];

          Lhs_Stride_Array [i] = Lhs.Stride[i];
          Rhs_Stride_Array [i] = Rhs.Stride[i];
        }

  // PADRE_ASSERT( Lhs.BlockPartiArrayDomain != NULL );
  // PADRE_ASSERT( Rhs.BlockPartiArrayDomain != NULL );

#if 0
     printf ("In PADRE: Printout input data to subArraySched \n");

     printf ("Lhs.Domain_Dimension = %d \n",Lhs.Domain_Dimension);
     printf ("Rhs.Domain_Dimension = %d \n",Rhs.Domain_Dimension);

     printf ("Max_Dimension = %d \n",Max_Dimension);

     printf ("Lhs_Dimension_Array [0-%d] = ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
          printf (" %d",Lhs_Dimension_Array [i]);
     printf ("\n");

     printf ("Rhs_Dimension_Array [0-%d] = ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
          printf (" %d",Rhs_Dimension_Array [i]);
     printf ("\n");

     printf ("Lhs_Base_Array [0-%d] = ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
          printf (" %d",Lhs_Base_Array [i]);
     printf ("\n");

     printf ("Lhs_Bound_Array [0-%d] = ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
          printf (" %d",Lhs_Bound_Array [i]);
     printf ("\n");

     printf ("Lhs_Stride_Array [0-%d] = ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
          printf (" %d",Lhs_Stride_Array [i]);
     printf ("\n");

     printf ("Rhs_Base_Array [0-%d] = ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
          printf (" %d",Rhs_Base_Array [i]);
     printf ("\n");

     printf ("Rhs_Bound_Array [0-%d] = ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
          printf (" %d",Rhs_Bound_Array [i]);
     printf ("\n");

     printf ("Rhs_Stride_Array [0-%d] = ",PARTI_MAX_ARRAY_DIMENSION-1);
     for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
          printf (" %d",Rhs_Stride_Array [i]);
     printf ("\n");

  // displayDARRAY (Lhs.BlockPartiArrayDomain);
  // displayDARRAY (Rhs.BlockPartiArrayDomain);
#endif

  // The Rhs is the source and the Lhs is the destination
  // printf ("Building a Schedule by calling PARTI subArraySched \n");
     // DARRAY *RhsBlockPartiArrayDescriptor = Rhs.getBlockPartiArrayDomain();
     DARRAY *RhsBlockPartiArrayDescriptor = Rhs.parallelPADRE_DescriptorPointer
       ? Rhs.parallelPADRE_DescriptorPointer->getBlockPartiArrayDomain()
       : NULL;
     DARRAY *LhsBlockPartiArrayDescriptor = Lhs.parallelPADRE_DescriptorPointer
       ? Lhs.parallelPADRE_DescriptorPointer->getBlockPartiArrayDomain()
       : NULL;
     PADRE_ASSERT (RhsBlockPartiArrayDescriptor != NULL);
     PADRE_ASSERT (LhsBlockPartiArrayDescriptor != NULL);
     SCHED  *Temp_Schedule = subArraySched ( RhsBlockPartiArrayDescriptor,
                                             LhsBlockPartiArrayDescriptor,
                                             Max_Dimension,
                                             Rhs_Dimension_Array, Rhs_Base_Array, 
                                             Rhs_Bound_Array, Rhs_Stride_Array,
                                             Lhs_Dimension_Array, Lhs_Base_Array, 
                                             Lhs_Bound_Array, Lhs_Stride_Array );
  // printf ("DONE: Building a Schedule by calling PARTI subArraySched \n");
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          if (Temp_Schedule == NULL)
               printf ("WARNING: Temp_Schedule == NULL (no data to communicate: this is rare but OK) \n");
#endif

#if 0
  // displayDARRAY (Lhs.BlockPartiArrayDomain);
  // displayDARRAY (Rhs.BlockPartiArrayDomain);
     displaySCHED (Temp_Schedule);
#endif

     return Temp_Schedule;
#else
     return NULL;
#endif
   }


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
transferData ( const UserGlobalDomain & receiveDomain,
               const UserGlobalDomain & sendDomain,
               const double *sourceDataPointer,
               const double *destinationDataPointer )
   {

#if defined(USE_TAU)
     TAU_PROFILE("PARTI_Descriptor::transferData()", "void(void)", TAU_PADRE_COMMUNICATION_OVERHEAD_MANAGEMENT);
#endif

  // printf ("Inside of PARTI_Descriptor::transferData(UserGlobalDomain,UserGlobalDomain,double,double) \n");
  // PADRE_ABORT();

#if 1
       // Bugfix (4/11/96) PARTI returns the wrong schedule because it does not invalidate
       // the cached schedule from before the distribution was changed.  Thus it returns
       // the wrong schedule (or something like that).  The call to remove_subarray_scheds
       // is a PARTI function which clears the internal cache to force all subarray schedules
       // to be rebuilt from scratch.  When this bug in PARTI is fixed we can remove this function
       // call.
          remove_subarray_scheds();

       // These don't have to be valid points in the case where the communication
       // schedule will not force the generation of any communication
       // PADRE_ASSERT(sourceDataPointer != NULL);
       // PADRE_ASSERT(destinationDataPointer != NULL);
          SCHED* Temp_Schedule = BuildCommunicationScheduleRegularSectionTransfer
                                    ( receiveDomain, sendDomain );

#if defined(PADRE_DEBUG_IS_ENABLED)
          bool Receving_Data = false;
          bool Sending_Data  = false;
       // See if we have to send or recieve anything so we can check the pointers using asserts.
          if (Temp_Schedule != NULL)
             {
               for (int i=0; i < PARTI_numprocs(); i++)
                  {
                    if (Temp_Schedule->rMsgSz[i] > 0)
                         Receving_Data = true;
                    if (Temp_Schedule->sMsgSz[i] > 0)
                         Sending_Data  = true;
                  }
             }
          if (Receving_Data)
             {
               PADRE_ASSERT(destinationDataPointer != NULL);
             }
          if (Sending_Data)
             {
               PADRE_ASSERT(sourceDataPointer != NULL);
             }
#endif

          if (Temp_Schedule != NULL)
             {
            // printf ("@@@@@@@ VALID SCHED OBJECT GENERATE IN PARTI_Descriptor::transferData() \n");
            // displaySCHED (Temp_Schedule);

              dDataMove( (double*) sourceDataPointer, Temp_Schedule, (double*) destinationDataPointer);

            // Delete the communication schedule that was just used
            // Note that a copy was saved as a part of PARTI's processing so that the
            // schedule can be reused later if required -- without it's recreation.
               delete_SCHED (Temp_Schedule);
               Temp_Schedule = NULL;
             }
            else
             {
            // printf ("$$$$$ VOID SCHED OBJECT GENERATE IN PARTI_Descriptor::transferData() \n");
             }
#endif

  // printf ("At BASE of PARTI_Descriptor::transferData(UserGlobalDomain,UserGlobalDomain,double,double) \n");
  // PADRE_ABORT();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
transferData ( const UserGlobalDomain & receiveDomain,
               const UserGlobalDomain & sendDomain,
               const float *sourceDataPointer,
               const float *destinationDataPointer )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PARTI_Descriptor::transferData()", "void(void)", TAU_PADRE_COMMUNICATION_OVERHEAD_MANAGEMENT);
#endif

#if 1
       // Bugfix (4/11/96) PARTI returns the wrong schedule because it does not invalidate
       // the cached schedule from before the distribution was changed.  Thus it returns
       // the wrong schedule (or something like that).  The call to remove_subarray_scheds
       // is a PARTI function which clears the internal cache to force all subarray schedules
       // to be rebuilt from scratch.  When this bug in PARTI is fixed we can remove this function
       // call.
          remove_subarray_scheds();

       // PADRE_ASSERT(sourceDataPointer != NULL);
       // PADRE_ASSERT(destinationDataPointer != NULL);
          SCHED* Temp_Schedule = BuildCommunicationScheduleRegularSectionTransfer
                                    ( receiveDomain, sendDomain );

#if defined(PADRE_DEBUG_IS_ENABLED)
          bool Receving_Data = false;
          bool Sending_Data  = false;
       // See if we have to send or recieve anything so we can check the pointers using asserts.
          if (Temp_Schedule != NULL)
             {
               for (int i=0; i < PARTI_numprocs(); i++)
                  {
                    if (Temp_Schedule->rMsgSz[i] > 0)
                         Receving_Data = true;
                    if (Temp_Schedule->sMsgSz[i] > 0)
                         Sending_Data  = true;
                  }
             }
          if (Receving_Data)
             {
               PADRE_ASSERT(destinationDataPointer != NULL);
             }
          if (Sending_Data)
             {
               PADRE_ASSERT(sourceDataPointer != NULL);
             }
#endif

          if (Temp_Schedule != NULL)
             {
            // printf ("@@@@@@@ VALID SCHED OBJECT GENERATE IN PARTI_Descriptor::transferData() \n");
            // displaySCHED (Temp_Schedule);
              fDataMove( (float*) sourceDataPointer, Temp_Schedule, (float*) destinationDataPointer);

            // Delete the communication schedule that was just used
            // Note that a copy was saved as a part of PARTI's processing so that the
            // schedule can be reused later if required -- without it's recreation.
               delete_SCHED (Temp_Schedule);
               Temp_Schedule = NULL;
             }
            else
             {
            // printf ("$$$$$ VOID SCHED OBJECT GENERATE IN PARTI_Descriptor::transferData() \n");
             }
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
transferData ( const UserGlobalDomain & receiveDomain,
               const UserGlobalDomain & sendDomain,
               const int *sourceDataPointer,
               const int *destinationDataPointer )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PARTI_Descriptor::transferData()", "void(void)", TAU_PADRE_COMMUNICATION_OVERHEAD_MANAGEMENT);
#endif

#if 1
       // Bugfix (4/11/96) PARTI returns the wrong schedule because it does not invalidate
       // the cached schedule from before the distribution was changed.  Thus it returns
       // the wrong schedule (or something like that).  The call to remove_subarray_scheds
       // is a PARTI function which clears the internal cache to force all subarray schedules
       // to be rebuilt from scratch.  When this bug in PARTI is fixed we can remove this function
       // call.
          remove_subarray_scheds();

       // PADRE_ASSERT(sourceDataPointer != NULL);
       // PADRE_ASSERT(destinationDataPointer != NULL);
          SCHED* Temp_Schedule = BuildCommunicationScheduleRegularSectionTransfer
                                    ( receiveDomain, sendDomain );

#if defined(PADRE_DEBUG_IS_ENABLED)
          bool Receving_Data = false;
          bool Sending_Data  = false;
       // See if we have to send or recieve anything so we can check the pointers using asserts.
          if (Temp_Schedule != NULL)
             {
               for (int i=0; i < PARTI_numprocs(); i++)
                  {
                    if (Temp_Schedule->rMsgSz[i] > 0)
                         Receving_Data = true;
                    if (Temp_Schedule->sMsgSz[i] > 0)
                         Sending_Data  = true;
                  }
             }
          if (Receving_Data)
             {
               PADRE_ASSERT(destinationDataPointer != NULL);
             }
          if (Sending_Data)
             {
               PADRE_ASSERT(sourceDataPointer != NULL);
             }
#endif

          if (Temp_Schedule != NULL)
             {
            // printf ("@@@@@@@ VALID SCHED OBJECT GENERATE IN PARTI_Descriptor::transferData() \n");
            // displaySCHED (Temp_Schedule);
              iDataMove( (int*) sourceDataPointer, Temp_Schedule, (int*) destinationDataPointer);

            // Delete the communication schedule that was just used
            // Note that a copy was saved as a part of PARTI's processing so that the
            // schedule can be reused later if required -- without it's recreation.
               delete_SCHED (Temp_Schedule);
               Temp_Schedule = NULL;
             }
            else
             {
            // printf ("$$$$$ VOID SCHED OBJECT GENERATE IN PARTI_Descriptor::transferData() \n");
             }
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
updateGhostBoundaries ( double *dataPointer )
   {
  // temporarily remove exch schedules to force a new one to
  // be built because somehow the incorrect one is found by block parti ...
  // PARTI has some internal problems in its caching of communication schedules
     remove_exch_scheds();

  // Temp_Schedule will be NULL if no data is present on the 
  // local processor (so no ghost boundaries exist to update)
     SCHED* Temp_Schedule = BuildCommunicationScheduleUpdateAllGhostBoundaries ();

  // Make call to datmove function!
     if (Temp_Schedule != NULL)
        {
          dDataMove(dataPointer, Temp_Schedule, dataPointer);
          delete_SCHED (Temp_Schedule);
        }
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
updateGhostBoundaries ( float *dataPointer )
   {
  // temporarily remove exch schedules to force a new one to
  // be built because somehow the incorrect one is found by block parti ...
  // PARTI has some internal problems in its caching of communication schedules
     remove_exch_scheds();

  // Temp_Schedule will be NULL if no data is present on the 
  // local processor (so no ghost boundaries exist to update)
     SCHED* Temp_Schedule = BuildCommunicationScheduleUpdateAllGhostBoundaries ();

  // Make call to datmove function!
     if (Temp_Schedule != NULL)
        {
          fDataMove(dataPointer, Temp_Schedule, dataPointer);
          delete_SCHED (Temp_Schedule);
        }
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
updateGhostBoundaries ( int *dataPointer )
   {
  // temporarily remove exch schedules to force a new one to
  // be built because somehow the incorrect one is found by block parti ...
  // PARTI has some internal problems in its caching of communication schedules
     remove_exch_scheds();

  // Temp_Schedule will be NULL if no data is present on the 
  // local processor (so no ghost boundaries exist to update)
     SCHED* Temp_Schedule = BuildCommunicationScheduleUpdateAllGhostBoundaries ();

  // Make call to datmove function!
     if (Temp_Schedule != NULL)
        {
       // We want to implement this later within PADRE
       // Optimization_Manager::Number_Of_Messages_Sent++;
       // Optimization_Manager::Number_Of_Messages_Recieved++;
       // iDataMove(X_SerialArray.Array_Descriptor.Array_Data,
       //           Temp_Schedule,X_SerialArray.Array_Descriptor.Array_Data);
          iDataMove(dataPointer, Temp_Schedule, dataPointer);
          delete_SCHED (Temp_Schedule);
        }
   }


// ************************************************************************************
// ************************************************************************************
// As in the initializeLocalDescriptor member function we should likely change the
// references to localDescriptor to globalDomain once P++ is organized a little differently.
// ************************************************************************************
// ************************************************************************************
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
SCHED* PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
BuildCommunicationScheduleUpdateAllGhostBoundaries ()
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PARTI_Descriptor::BuildCommunicationScheduleUpdateAllGhostBoundaries! \n");
#endif

  // PARTI can't handle a Null array
     if (localDescriptor->isNull() == true) 
          return NULL;

  // int Number_Of_Dimensions = X.Domain_Dimension;
     PADRE_ASSERT (localDescriptor->numberOfDimensions() == 
                   UserGlobalDomain::computeArrayDimension (*localDescriptor));

  // ... (12/11/96,kdb) if X.Array_Conformability_Info isn't null use the
  // update info to only update the needed ghost cell boundaries ...
     SCHED* Update_All_Ghost_Boundaries = NULL;

  // ... (12/12/96,kdb) block parti doesn't allow only one side of ghost
  // boundaries to be updated so turn this off for now and put temporary fix
  // in cases.c that forces an ghost cell update every where if the
  // parallel array size is different or the partitioning is different ...
  // ... the Array_Conformability_Info is null so update all ghost
  // boundaries ...

  // Parti name used here for consistency with the documentation
  // int fillVec[MAX_ARRAY_DIMENSION];
  // for (int Axis=0; Axis < MAX_ARRAY_DIMENSION; Axis++)
  //    {
  //      fillVec[Axis] = (X.Partitioning_Object_Pointer == NULL) ?
  //                       Internal_Partitioning_Type::DefaultGhostCellWidth[Axis] :
  //                       X.InternalGhostCellWidth[Axis];
  //      printf ("fillVec[%d] = %d \n",Axis,fillVec[Axis]);
  //    }
  // PADRE_ASSERT (X.BlockPartiArrayDomain != NULL);
  // displayDARRAY ( X.BlockPartiArrayDomain );

     PADRE_ASSERT ( representation != NULL );
     PADRE_ASSERT ( representation->BlockPartiArrayDescriptor != NULL );
     Update_All_Ghost_Boundaries = ghostFillAllSched( representation->BlockPartiArrayDescriptor );

  // see note about the problem with block parti ...
     return Update_All_Ghost_Boundaries;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int
PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
isLeftNullArray (UserLocalDomain & serialArrayDomain , int Axis) const
   {
  // A Null array can be part of the left edge or the right edge of a distributed axis
  // This function is helpful in determining which it is.

     int result = false;

  // printf ("In isLeftNullArray(): Axis = %d \n",Axis);

     if ( serialArrayDomain.isNullArray() == true )
        {
       // printf ("In ARRAY_DESCRIPTOR_TYPE($1,$2)::isLeftNullArray(axis): Domain_Dimension = %d \n",Domain_Dimension);
          PADRE_ASSERT (localDescriptor != NULL);

          if ( Axis < localDescriptor->Domain_Dimension)
             {
               PADRE_ASSERT(representation != NULL);
               PADRE_ASSERT(representation->BlockPartiArrayDescriptor != NULL);
            // printf ("     gUBnd(representation->BlockPartiArrayDescriptor,Axis) = %d \n",gUBnd(representation->BlockPartiArrayDescriptor,Axis));
            // printf ("     lalbnd(BlockPartiArrayDomain,Axis,Base[Axis],1) = %d \n",lalbnd(BlockPartiArrayDomain,Axis,localDescriptor->Base[Axis],1));

               result = (gUBnd(representation->BlockPartiArrayDescriptor,Axis) < lalbnd(representation->BlockPartiArrayDescriptor,Axis,localDescriptor->Base[Axis],1));
             }
            else
             {
            // Along higher axes we want to force isLeftPartition == true
               result = true;
             }
        }

     return result;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int
PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> ::
isRightNullArray (UserLocalDomain & serialArrayDomain , int Axis) const
   {
  // A Null array can be part of the left edge or the right edge of a distributed axis
  // This function is helpful in determining which it is.

     int result = false;

  // printf ("In isRightNullArray(): Axis = %d \n",Axis);
     if ( serialArrayDomain.isNullArray() == true )
        {
          PADRE_ASSERT (localDescriptor != NULL);
          if ( Axis < localDescriptor->Domain_Dimension)
             {
               result = !isLeftNullArray(serialArrayDomain,Axis);
             }
            else
             {
            // Along higher axes we want to force isRightPartition == true
               result = true;
             }
        }

     return result;
   }

#endif	// !defined(NO_Parti)

#endif	// PADRE_PARTI_TEMPLATE_C


