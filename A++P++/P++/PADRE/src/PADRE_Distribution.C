
#ifndef PADRE_DISTRIBUTION_C
#define PADRE_DISTRIBUTION_C

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <PADRE_Global.h>
#include <PADRE_Distribution.h>


#if 1
// Declaration of static variables (defined in the header file)
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
Default_GhostBoundary_Width [6];

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>* 
     PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
     defaultDistribution = NULL;
#endif

// Memory Management
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::freeMemoryInUse ()
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Distribution::freeMemoryInUse()", "void(void)", TAU_PADRE_MEMORY_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PADRE_Distribution::freeMemoryInUse (possible memory leak) \n");
#endif

     if (defaultDistribution != NULL)
        {
          printf ("In PADRE_Distribution::freeMemoryInUse(): defaultDistribution->getReferenceCount() = %d \n",
               defaultDistribution->getReferenceCount());
          defaultDistribution->decrementReferenceCount();
          if (defaultDistribution->getReferenceCount() < defaultDistribution->getReferenceCountBase())
             {
            // printf ("Calling delete defaultDistribution in PADRE_Distribution::freeMemoryInUse()! \n");
               delete defaultDistribution;
             }
            else
             {
            // I'm not clear if this is an error or not (since there could be a good reason whey there are outstanding references)
            // since there are outstanding array objects (I think).
               printf ("WARNING: In PADRE_Distribution::freeMemoryInUse(): defaultDistribution not deleted as part of memory cleanup! \n");
            // PADRE_ABORT();
             }
          defaultDistribution = NULL;
        }

  // printf ("In PARTI_Distribution::freeMemoryInUse() -- calling PARTI_Distribution::freeMemoryInUse() \n");
#if !defined(NO_Parti)
        PARTI_Distribution  <UserCollection, UserGlobalDomain, UserLocalDomain>::
	   freeMemoryInUse ();
#endif
#if defined (USE_KELP)
        KELP_Distribution  <UserCollection, UserGlobalDomain, UserLocalDomain>::
	   freeMemoryInUse ();
#endif
#if !defined(NO_GlobalArrays)
        GlobalArrays_Distribution  <UserCollection, UserGlobalDomain, UserLocalDomain>::
	   freeMemoryInUse ();
#endif
  // printf ("At BASE of PARTI_Distribution::freeMemoryInUse() \n");
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & 
   PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::getDefaultDistribution()
   {
     if (defaultDistribution == NULL)
        {
       // We call the default constructor to build a PADRE_Distribution object with default values!
          defaultDistribution = new PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>();
        }
     PADRE_ASSERT (defaultDistribution != NULL);
     return *defaultDistribution;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> *
   PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::getDefaultDistributionPointer()
   {
     if (defaultDistribution == NULL)
        {
       // We call the default constructor to build a PADRE_Distribution object with default values!
          defaultDistribution = new PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>();
        }
     PADRE_ASSERT (defaultDistribution != NULL);
     return defaultDistribution;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
setDefaultProcessorRange( int Start, int End )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Distribution ::"
               << " setDefaultProcessorRange(int,int)"
               << " id " << "(STATIC OBJECT)" << "\n";
#endif

  // This is a specific mechanism (i.e. taking a base a bound) and it is ok to 
  // make this specific requirement because it maps to the equivalant limitation within PARTI
  // but this function (PADRE_Distribution::setDefaultProcessorRange) really should have
  // a more general interface.

#if !defined(NO_Parti)
       PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::setDefaultProcessorRange(Start,End);
#endif
#if !defined(NO_GlobalArrays)
       GlobalArrays_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::setDefaultProcessorRange(Start,End);
#endif
  // if (getpPARTI_DistributionPointer() != NULL)
  // else
  //      if (getpKELP_DistributionPointer() != NULL)
  //           KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::setDefaultProcessorRange(Start,End);
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
~PADRE_Distribution()
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PADRE_Distribution::~PADRE_Distribution(): referenceCount = %d \n",getReferenceCount());
#endif

     PADRE_ASSERT (getReferenceCount() == getReferenceCountBase()-1);

  // printf ("delete pPARTI_Distribution \n");
#if !defined(NO_Parti)
     if (pPARTI_Distribution != NULL)
        {
          pPARTI_Distribution->decrementReferenceCount();
          if (pPARTI_Distribution->getReferenceCount() < pPARTI_Distribution->getReferenceCountBase())
             {
            // printf ("call delete pPARTI_Distribution \n");
               delete pPARTI_Distribution;
             }
          pPARTI_Distribution = NULL;
        }
#endif

#if defined (USE_KELP)
     if (pKELP_Distribution != NULL)
        {
          pKELP_Distribution->decrementReferenceCount();
          if (pKELP_Distribution->getReferenceCount() < 
	      pKELP_Distribution->getReferenceCountBase())
             {
               delete pKELP_Distribution;
             }
          pKELP_Distribution = NULL;
        }
#endif

#if !defined(NO_GlobalArrays)
     if (pGlobalArrays_Distribution != NULL)
        {
          pGlobalArrays_Distribution->decrementReferenceCount();
          if (pGlobalArrays_Distribution->getReferenceCount() < 
	      pGlobalArrays_Distribution->getReferenceCountBase())
             {
               delete pGlobalArrays_Distribution;
             }
          pGlobalArrays_Distribution = NULL;
        }
#endif

  // printf ("delete UserCollectionList \n");
     if (UserCollectionList != NULL)
        {
       // printf ("In ~PADRE_Distribution(): commented out -- delete UserCollectionList \n");
       // printf ("UserCollectionList->size() = %d \n",UserCollectionList->size());
       // int Size = UserCollectionList->size();
       // for (int i=0 i < Size; i++)

          // wdh* for (list<UserCollection*>::iterator i = UserCollectionList->begin(); i != UserCollectionList->end(); i++)
          typedef typename list<UserCollection*>::iterator UserCollectionList_Iterator;
          for (UserCollectionList_Iterator i = UserCollectionList->begin(); i != UserCollectionList->end(); i++)
             {
            // printf ("Commented out deletion of UserCollection in ~PADRE_Distribution \n");
            // delete (*i);
            // *i = NULL;
            // UserCollectionList->pop_front();
            // printf ("Inside loop (at bottom): *i is %s pointer: UserCollectionList->size() = %d \n",
            //      (*i == NULL) ? "NULL" : "VALID",UserCollectionList->size());
             }
          delete UserCollectionList;
          UserCollectionList = NULL;
        }

  // STL class (need to delete its member elements)
  // associatedRepresentations.cleanup();
   }

// Make this a private member function and only call it to build the default
// distribution object (with default data values)
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
PADRE_Distribution()
   {
// This specifies the default distribution mechanism within PADRE (normally this is a PARTI distribution
// but it could be either PARTI, KELP or GlobalArrays.  We can change this arbitrarily to allow us
// to test the use of PADRE with different distribution mechanisms.

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Distribution ::"
               << " PADRE_Distribution()"
               << " id " << getId() << "\n";
#endif

     initialize();

// Select a distribution to use (internally)
#if !defined(NO_GlobalArrays)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::GlobalArrays ) ) {
       pGlobalArrays_Distribution = 
	 new GlobalArrays_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ();
       PADRE_ASSERT (pGlobalArrays_Distribution != NULL);
       setSublibraryInUse( SublibraryNames::GlobalArrays );
       pGlobalArrays_Distribution->setUserCollectionList(UserCollectionList);
     }
#endif

#if !defined(NO_Parti)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::PARTI ) ) {
       pPARTI_Distribution = 
	 new PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ();
       PADRE_ASSERT (pPARTI_Distribution != NULL);
       setSublibraryInUse( SublibraryNames::PARTI );
       pPARTI_Distribution->setUserCollectionList(UserCollectionList);
     }
     PADRE_ASSERT (pPARTI_Distribution != NULL);
#endif

#if defined (USE_KELP)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::KELP ) ) {
       pKELP_Distribution = 
	 new KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ();
       PADRE_ASSERT (pKELP_Distribution != NULL);
       setSublibraryInUse( SublibraryNames::KELP );
       pKELP_Distribution->setUserCollectionList(UserCollectionList);
     }
     PADRE_ASSERT (pKELP_Distribution != NULL);
#endif

     testConsistency ("Called from PADRE_Distribution() constructor! \n");
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
PADRE_Distribution( const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Distribution ::"
               << " PADRE_Distribution( const PADRE_Distribution & X )"
               << " id " << getId() << "\n";
#endif

     if (defaultDistribution == NULL)
        {
       // We call the default constructor to build a PADRE_Distribution object with default values!
          defaultDistribution = new PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>();
        }

     initialize();

#if !defined(NO_GlobalArrays)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::GlobalArrays ) ) {
       pGlobalArrays_Distribution = 
	 new GlobalArrays_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> 
	 (*X.pGlobalArrays_Distribution);
       PADRE_ASSERT (pGlobalArrays_Distribution != NULL);
       setSublibraryInUse( SublibraryNames::GlobalArrays );
       pGlobalArrays_Distribution->setUserCollectionList(UserCollectionList);
     }
#endif

#if !defined(NO_Parti)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::PARTI ) ) {
       pPARTI_Distribution = 
	 new PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> 
	 (*X.pPARTI_Distribution);
       PADRE_ASSERT (pPARTI_Distribution != NULL);
       setSublibraryInUse( SublibraryNames::PARTI );
       pPARTI_Distribution->setUserCollectionList(UserCollectionList);
     }
#endif

#if defined (USE_KELP)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::KELP ) ) {
       pKELP_Distribution = 
	 new KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> 
	 (*X.pKELP_Distribution);
       PADRE_ASSERT (pKELP_Distribution != NULL);
       setSublibraryInUse( SublibraryNames::KELP );
       pKELP_Distribution->setUserCollectionList(UserCollectionList);
     }
#endif

     testConsistency ("Called from PADRE_Distribution(PADRE_Distribution) constructor! \n");
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
PADRE_Distribution( int Number_Of_Processors )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Distribution ::"
               << " PADRE_Distribution( int Number_Of_Processors )"
               << " id " << getId() << "\n";
#endif

     if (defaultDistribution == NULL)
        {
       // We call the default constructor to build a PADRE_Distribution object with default values!
          defaultDistribution = new PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>();
        }

     initialize();

#if !defined(NO_Parti)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::PARTI ) ) {
#if defined(PADRE_DEBUG_IS_ENABLED)
       printf("Setting pPARTI_Distribution at line %d in file %s\n", __LINE__, __FILE__);
#endif
       pPARTI_Distribution = new PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ();
       PADRE_ASSERT (pPARTI_Distribution != NULL);
       pPARTI_Distribution->setProcessorRange(0,Number_Of_Processors-1);
     PADRE_ASSERT (pPARTI_Distribution != NULL);
       setSublibraryInUse( SublibraryNames::PARTI );
     PADRE_ASSERT (pPARTI_Distribution != NULL);
       pPARTI_Distribution->setUserCollectionList(UserCollectionList);
     PADRE_ASSERT (pPARTI_Distribution != NULL);
     }
#endif

#if !defined(NO_GlobalArrays)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::GlobalArrays ) ) {
       pGlobalArrays_Distribution = 
	 new GlobalArrays_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ();
       PADRE_ASSERT (pGlobalArrays_Distribution != NULL);
       pGlobalArrays_Distribution->setProcessorRange(0,Number_Of_Processors-1);
       setSublibraryInUse( SublibraryNames::GlobalArrays );
       pGlobalArrays_Distribution->setUserCollectionList(UserCollectionList);
     }
#endif

#if defined (USE_KELP)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::KELP ) ) {
	 pKELP_Distribution = 
	   new KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ();
	 PADRE_ASSERT (pKELP_Distribution != NULL);
	 pKELP_Distribution->setProcessorRange(0,Number_Of_Processors-1);
	 setSublibraryInUse( SublibraryNames::KELP );
	 pKELP_Distribution->setUserCollectionList(UserCollectionList);
     }
#endif


     testConsistency ("Called from PADRE_Distribution(Number_Of_Processors) constructor! \n");
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
PADRE_Distribution( int startingProcessor, int endingProcessor )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Distribution ::"
               << " PADRE_Distribution(int StartingProcessor " << startingProcessor
               << ", int EndingProcessor " << endingProcessor << ")"
               << " id " << getId() << "\n";
#endif

     if (defaultDistribution == NULL)
        {
       // We call the default constructor to build a PADRE_Distribution object with default values!
          defaultDistribution = new PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>();
        }

     initialize();

  // use of this constructor implies use of PARTI

  // pDomainMap_Distribution = NULL;
  // pPGSLib_Distribution = NULL;

#if !defined(NO_Parti)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::PARTI ) ) {
       pPARTI_Distribution = 
	 new PARTI_Distribution<UserCollection,UserGlobalDomain,UserLocalDomain>
	 (startingProcessor, endingProcessor);
       PADRE_ASSERT (pPARTI_Distribution != NULL);
       // pPARTI_Distribution->setProcessorRange(startingProcessor, endingProcessor);
       setSublibraryInUse( SublibraryNames::PARTI );
       pPARTI_Distribution->setUserCollectionList(UserCollectionList);
     }
#endif
#if !defined(NO_GlobalArrays)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::GlobalArrays ) ) {
	 pGlobalArrays_Distribution = 
	   new GlobalArrays_Distribution<UserCollection,UserGlobalDomain,UserLocalDomain>
	   (startingProcessor, endingProcessor);
	 PADRE_ASSERT (pGlobalArrays_Distribution != NULL);
	 // pGlobalArrays_Distribution->setProcessorRange(startingProcessor, endingProcessor);
	 setSublibraryInUse( SublibraryNames::GlobalArrays );
	 pGlobalArrays_Distribution->setUserCollectionList(UserCollectionList);
       }
#endif
#if defined (USE_KELP)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::KELP ) ) {
	 pKELP_Distribution = 
	   new KELP_Distribution<UserCollection,UserGlobalDomain,UserLocalDomain>
	   (startingProcessor, endingProcessor);
	 PADRE_ASSERT (pKELP_Distribution != NULL);
	 setSublibraryInUse( SublibraryNames::KELP );
	 pKELP_Distribution->setUserCollectionList(UserCollectionList);
       }
#endif

     testConsistency ("Called from PADRE_Distribution(startingProcessor,endingProcessor) constructor! \n");
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
PADRE_Distribution( const int* Processor_Array, int NumberOfProcessors )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Distribution ::"
               << " PADRE_Distribution(const int* Processor_Array, int NumberOfProcessors )"
               << " id " << getId() << "\n";
#endif

     printf ("PADRE_Distribution (int*,int) not implemented \n");
     PADRE_ABORT();

     if (defaultDistribution == NULL)
        {
       // We call the default constructor to build a PADRE_Distribution object with default values!
          defaultDistribution = new PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>();
        }

     initialize();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> &
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
operator= ( const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Distribution<UC,ULD,UD> & operator="
               << " ( const PADRE_Distribution<UC,ULD,UD> & X )"
               << " id " << getId() << "\n";
#endif

     printf ("The PADRE_Distribution::operator= should not be called! \n");
     PADRE_ABORT();

  // We need to think about if this should do a deep copy or a shallow copy
  // I would suggest a secondary input parameter do permit the copy constructor
  // to be use for deep OR shallow copies.
  // ... note: one of these should be null ...
#if !defined(NO_Parti)
     PADRE_ASSERT ( X.pPARTI_Distribution == NULL);
#else
#if !defined(NO_GlobalArrays)
     PADRE_ASSERT ( X.pPARTI_Distribution == NULL || X.pGlobalArrays_Distribution == NULL);
#endif
#if defined (USE_KELP)
     PADRE_ASSERT ( X.pPARTI_Distribution == NULL || X.pKELP_Distribution == NULL);
#endif
#endif

     *pPARTI_Distribution = *X.pPARTI_Distribution;
#if defined (USE_KELP)
     *pKELP_Distribution = *X.pKELP_Distribution;
#endif
#if !defined(NO_GlobalArrays)
     *pGlobalArrays_Distribution = *X.pGlobalArrays_Distribution;
#endif

  // How should we copy the UserCollectionList?
  // UserCollectionList = NULL;
     UserCollectionList  = new list<UserCollection*>;
     PADRE_ASSERT (UserCollectionList != NULL);

#if 1
     subLibrariesInUse = X.subLibrariesInUse;
#else
  // WHEN we have multiple libraries we would want to handle the 
  // setting of the subLibrariesInUse differently
     if (pPARTI_Distribution != NULL)
        setSublibraryInUse( SublibraryNames::PARTI );
#if !defined(NO_GlobalArrays)
     else if (pGlobalArrays_Distribution != NULL)
        setSublibraryInUse( SublibraryNames::GlobalArrays );
#endif
#if defined (USE_KELP)
     else if (pKELP_Distribution != NULL)
        setSublibraryInUse( SublibraryNames::KELP );
#endif
#endif

     testConsistency ("Called from PADRE_Distribution::operator=(PADRE_Distribution) constructor! \n");
     return *this;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
initialize()
   {
#if !defined(NO_Parti)
     pPARTI_Distribution = NULL;
#endif
#if !defined(NO_GlobalArrays)
     pGlobalArrays_Distribution  = NULL;
#endif
#if defined (USE_KELP)
     pKELP_Distribution  = NULL;
#endif

     UserCollectionList  = new list<UserCollection*>;
     PADRE_ASSERT (UserCollectionList != NULL);

     PADRE_ASSERT (getReferenceCount() > 0);

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PADRE_Distribution<UC,ULD,UD>::initialize() \n");
#endif

  // If this is to be static then it has to forma an upper bound
  // and it seems it has to be static since we don't have an instance of
  // a UserLocalDomain object at this point.
  // Distribution_Dimension = UserLocalDomain::dimension();
     Distribution_Dimension = UserLocalDomain::maxNumberOfDimensions();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
SublibraryNames::Container&
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
setSublibraryInUse( SublibraryNames::Name X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Distribution<UC,ULD,UD> ::"
               << " setSublibraryInUse(" << X << ")\n";
#endif

     // cerr << "subLibrariesInUse: " << subLibrariesInUse << endl;
     if ( ! PADRE::isSublibraryPermitted(X) ) {
       // Given sublibrary is not on, according to the common interface.
       cerr << "Error: object setting a sublibrary that is not on in the global scope." << endl;
       PADRE_ABORT();
     }
     // PADRE_ASSERT( subLibrariesInUse.empty() ); // Currently cannot handle multiples.
     subLibrariesInUse.insert( X );
     return subLibrariesInUse;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
const SublibraryNames::Container&
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
getSublibraryInUse() const
   {
     PADRE_ASSERT( !subLibrariesInUse.empty() );
     PADRE_ASSERT( subLibrariesInUse.size() == 1 ); // Currently cannot handle multiples.
     // return *subLibrariesInUse.begin();
     return subLibrariesInUse;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
SublibraryNames::Name
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
subLibraryGreatestLowerBound( const SublibraryNames::Name & X, 
			      const SublibraryNames::Name & Y )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "SublibraryNames::Name"
               << "PADRE_Distribution<UC,ULD,UD> ::"
               << " subLibraryGreatestLowerBound(" << X << "," << Y << ")"
               << " not implemented, returning LibNONE\n";
#endif
     return SublibraryNames::NONE;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
setGhostCellWidth( unsigned axis, unsigned width )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Distribution ::"
               << " setGhostCellWidth("
               << "unsigned axis = " << axis << ",unsigned width = " << width << ")"
               << " id " << getId()
               << " not implemented\n";
#endif

  // This warning might apply to a function that does not yet exist in PADRE which would
  // permit the arrays associated with an existing distribution to vary their ghost boundary widths
  // printf ("Warning: this function does not change the widths of existing arrays using this distribution (feature is not implemented yet)! \n");

#if !defined(NO_Parti)
     PADRE_ASSERT (pPARTI_Distribution != NULL );
     if (pPARTI_Distribution != NULL)
        pPARTI_Distribution->LocalGhostCellWidth[axis] = width;
#endif

#if !defined(NO_GlobalArrays)
     PADRE_ASSERT (pGlobalArrays_Distribution != NULL);
     if (pGlobalArrays_Distribution != NULL)
        pGlobalArrays_Distribution->LocalGhostCellWidth[axis] = width;
#endif

#if defined (USE_KELP)
     PADRE_ASSERT (pKELP_Distribution != NULL);
     if (pKELP_Distribution != NULL)
        pKELP_Distribution->LocalGhostCellWidth[axis] = width;
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
getGhostCellWidth( unsigned axis ) const
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "int"
               << " PADRE_Distribution ::"
               << " int getGhostCellWidth("
               << "unsigned axis = " << axis << ")"
               << " id " << getId()
               << " not implemented, returning 0\n";
#endif

#if !defined(NO_Parti)
     PADRE_ASSERT (pPARTI_Distribution != NULL );
  // if ( pPARTI_Distribution != NULL)
  //      return pPARTI_Distribution->LocalGhostCellWidth[axis];
     return pPARTI_Distribution->LocalGhostCellWidth[axis];
#endif

#if 0
#if !defined(NO_GlobalArrays)
     PADRE_ASSERT (pPARTI_Distribution != NULL || pGlobalArrays_Distribution != NULL);
     if ( pPARTI_Distribution != NULL)
        return pPARTI_Distribution->LocalGhostCellWidth[axis];
     else if ( pGlobalArrays_Distribution != NULL)
        return pGlobalArrays_Distribution->LocalGhostCellWidth[axis];
#endif

#if defined (USE_KELP)
     PADRE_ASSERT (pPARTI_Distribution != NULL || pKELP_Distribution != NULL);
     if ( pPARTI_Distribution != NULL)
        return pPARTI_Distribution->LocalGhostCellWidth[axis];
     else if ( pKELP_Distribution != NULL)
        return pKELP_Distribution->LocalGhostCellWidth[axis];
#endif
#endif
     return 0;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
getNumberOfAxesToDistribute() const
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "int"
               << " PADRE_Distribution ::"
               << " getNumberOfAxesToDistribute()"
               << " id " << getId()
               << " implemented, returning " 
               << Distribution_Dimension
               << "\n";
#endif

     return Distribution_Dimension; 
   }

#if 0
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
setDistributeAxis( int Axis )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Distribution ::"
               << " setDistributeAxis("
               << "int Axis = " << Axis << ")"
               << " id " << getId()
               << " not implemented\n";
#endif
   }
#else
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
DistributeAlongAxis ( int Axis, bool Partition_Axis, int GhostBoundaryWidth )
   {
#if !defined(NO_Parti)
     if (getpPARTI_DistributionPointer() != NULL)
        getpPARTI_DistributionPointer()->DistributeAlongAxis (Axis,Partition_Axis,GhostBoundaryWidth);
#endif

#if !defined(NO_GlobalArrays)
     if (getpGlobalArrays_DistributionPointer() != NULL)
        getpGlobalArrays_DistributionPointer()->DistributeAlongAxis (Axis,Partition_Axis,GhostBoundaryWidth);
#endif

#if defined (USE_KELP)
     if (getpKELP_DistributionPointer() != NULL)
        getpKELP_DistributionPointer()->DistributeAlongAxis (Axis,Partition_Axis,GhostBoundaryWidth);
#endif
   }
#endif

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
getProcessorSet( PADRE_CommonInterface::ProcessorSet &processorSet ) const {
#if defined(PADRE_DEBUG_IS_ENABLED)
  if (PADRE::debugLevel() > 0)
    cout << "void"
	 << " PADRE_Distribution ::"
	 << " getProcessorSet(int *ProcessorArray, int & Number_Of_Processors)"
	 << " id " << getId()
	 << " not implemented\n";
#endif
  
  printf ("PADRE_Distribution::getProcessorSet(int*,int) const: not implemented! \n");
  PADRE_ABORT();
}

#if 0
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
swapDistribution( const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> 
		     & oldDistribution,
		   const PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> 
		     & newDistribution )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Distribution ::"
               << " swapDistribution("
               << " const PADRE_Distribution & oldDistribution id " << oldDistribution.getId() << ","
               << " const PADRE_Distribution & newDistribution id " << newDistribution.getId() << ")"
               << " id " << getId()
               << " not implemented\n";
#endif

     getpPARTI_Distribution().swapDistribution ( oldDistribution, newDistribution );
   }
#endif

// *************************************************************************
// *************************************************************************
// ***********  USERCOLLECTION ASSOCIATION TO PADRE_DISTRIBUTION ***********
// *************************************************************************
// *************************************************************************
// These functions provide the mechanism that permits changes to a distribution 
// to propagate back to objects that are distributed.  These do not act as
// references to the PADRE_Distribution object and so they do not effect the
// reference counts.  UserCollection objects are placed into a list of
// UserCollection objects within each PADRE_Distributin object.  Modifications
// to the PADRE_Distribution then force the PADRE_Distribution to be
// reapplied (using teh swap distribution member function) to each 
// UserCollection object.  In P++ the UserCollection objects
// are the base class for all the array objects (thus they are not
// typed by the type of element that the UserCollection represents).
// This is an issue of how P++ uses PADRE, not now PADRE is designed.
// The effect is that the same PADRE_Distribution object can be used to
// distribute any P++ array (i.e. of any type).  This simplifies
// alignment processes within numberical algorithms and higher level
// object-oriented libraries (such as AMR++ which must align the distribution
// of many P++ array obejcts or different types).

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
AssociateObjectWithDefaultDistribution( const UserCollection & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("In PADRE_Distribution::AssociateObjectWithDefaultDistribution -- getDefaultUserCollectionList()->size() = %d \n",
               getDefaultUserCollectionList()->size());
#endif

  // printf ("Exiting in PADRE_Distribution::AssociateObjectWithDefaultDistribution \n");
  // PADRE_ABORT();

  // Before we add this to this list we want to make sure
  // that it does not already exist in the list.  We have to check the Array_ID's
  // because there could be multiple references to a specific array and it must have
  // only a single representation by a Distribution object.
     int errorStatus = false;
     // *wdh* for (list<UserCollection*>::iterator i =
     typedef typename list<UserCollection*>::iterator UserCollectionList_Iterator;
     for (UserCollectionList_Iterator i =
          getDefaultUserCollectionList()->begin();
          i != getDefaultUserCollectionList()->end(); i++)
        {
#if 0
          if ( (*i)->Array_ID() == X.Array_ID() )
             {
               errorStatus = true;
             }
#endif
       // PADRE_ASSERT ( (*i)->Array_ID() != X.Array_ID() );
        }

     if (errorStatus == true)
        {
       // print out all the Array_IDs in the list!
          // *wdh* for (list<UserCollection*>::iterator i =
          for (UserCollectionList_Iterator i =
               getDefaultUserCollectionList()->begin();
               i != getDefaultUserCollectionList()->end(); i++)
             {
            // printf ("Array_ID = %d \n",(*i)->Array_ID());
             }
       // printf ("Exiting in PADRE_Distribution::AssociateObjectWithDefaultDistribution() \n");
       // PADRE_ABORT();
        }
       else
        {
          getDefaultUserCollectionList()->push_back(&(UserCollection &) X);
        }
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
AssociateObjectWithDistribution( const UserCollection & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("In PADRE_Distribution::AssociateObjectWithDistribution -- getUserCollectionList()->size() = %d \n",
               getUserCollectionList()->size());
#endif

  // printf ("Exiting in PADRE_Distribution::AssociateObjectWithDistribution \n");
  // PADRE_ABORT();

  // Before we add this to this list we want to make sure
  // that it does not already exist in the list.  We have to check the Array_ID's
  // because there could be multiple references to a specific array and it must have
  // only a single representation by a Distribution object.
     int errorStatus = false;
     // *wdh* 
     typedef typename list<UserCollection*>::iterator UserCollectionList_Iterator;
     // *wdh* for (list<UserCollection*>::iterator i =
     for (UserCollectionList_Iterator i =
          getUserCollectionList()->begin();
          i != getUserCollectionList()->end(); i++)
        {
#if 0
          if ( (*i)->Array_ID() == X.Array_ID() )
             {
               errorStatus = true;
             }
#endif
       // PADRE_ASSERT ( (*i)->Array_ID() != X.Array_ID() );
        }

     if (errorStatus == true)
        {
       // print out all the Array_IDs in the list!
          // *wdh* for (list<UserCollection*>::iterator i =
          for (UserCollectionList_Iterator i =
               getDefaultUserCollectionList()->begin();
               i != getDefaultUserCollectionList()->end(); i++)
             {
            // printf ("Array_ID = %d \n",(*i)->Array_ID());
             }
       // printf ("Exiting in PADRE_Distribution::AssociateObjectWithDistribution() \n");
       // PADRE_ABORT();
        }
       else
        {
          getUserCollectionList()->push_back(&(UserCollection &) X);
          PADRE_ASSERT (getUserCollectionList()->size() > 0);
        }

   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
UnassociateObjectWithDefaultDistribution( const UserCollection & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("In PADRE_Distribution::UnassociateObjectWithDefaultDistribution -- getDefaultUserCollectionList()->size() = %d \n",
               getDefaultUserCollectionList()->size());
#endif

  // printf ("Exiting in PADRE_Distribution::UnassociateObjectWithDefaultDistribution \n");
  // PADRE_ABORT();

     int SizeBeforeRemove = 0;
     int SizeAfterRemove  = 0;
     if (getDefaultUserCollectionList()->size() > 0)
        {
          PADRE_ASSERT (getDefaultUserCollectionList()->size() > 0);
          int SizeBeforeRemove = getDefaultUserCollectionList()->size();
          getDefaultUserCollectionList()->remove(&(UserCollection &) X);
          int SizeAfterRemove  = getDefaultUserCollectionList()->size();
#if 0
       // At a later stage in development we will uncomment this part!
       // Make sure that something (only one thing) really was removed!
          PADRE_ASSERT (SizeBeforeRemove - SizeAfterRemove == 1);
#endif
        }
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
UnassociateObjectWithDistribution( const UserCollection & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("In PADRE_Distribution::UnassociateObjectWithDistribution -- getUserCollectionList()->size() = %d \n",
               getUserCollectionList()->size());
#endif

  // printf ("Exiting in PADRE_Distribution::UnassociateObjectWithDistribution \n");
  // PADRE_ABORT();

#if 1
  // Comment this for now!
     PADRE_ASSERT (UserCollectionList != NULL);
     PADRE_ASSERT (getUserCollectionList() != NULL);
  // printf ("WARNING: getUserCollectionList()->size() == %d \n",
  //      getUserCollectionList()->size());
  // PADRE_ASSERT (getUserCollectionList()->size() > 0);
     int SizeBeforeRemove = getUserCollectionList()->size();
     getUserCollectionList()->remove(&(UserCollection &) X);
     int SizeAfterRemove  = getUserCollectionList()->size();
#else
     printf ("In PADRE_Distribution::UnassociateObjectWithDistribution: commented out function body! \n");
#endif

#if 0
  // At a later stage in development we will uncomment this part!
  // Make sure that something (only one thing) really was removed!
     PADRE_ASSERT (SizeBeforeRemove - SizeAfterRemove == 1);
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
testConsistency( const char *Label ) const
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Distribution ::"
               << " testConsistency( const char *Label = \"" << Label << "\" )"
               << " id " << getId()
               << " partially implemented\n";
#endif

  // Until we can use multiple distribution libraries this must be non null
#if 0
#if !defined(NO_Parti)
     PADRE_ASSERT (pPARTI_Distribution != NULL);
#endif

#if !defined(NO_GlobalArrays)
     PADRE_ASSERT (pGlobalArrays_Distribution != NULL);
#endif

#if defined (USE_KELP)
     PADRE_ASSERT (pKELP_Distribution != NULL);
#endif
#endif

  // Later when we have more distribution libraries in use we will call the
  // testConsistency() functions for each of then which is in use!
#if !defined(NO_Parti)
     if (pPARTI_Distribution != NULL)
        {
       // We need to make sure that the list of distribution libraries in use 
       // includes PARTI
          pPARTI_Distribution->testConsistency(Label);
        }
#endif

#if !defined(NO_GlobalArrays)
     if (pGlobalArrays_Distribution != NULL)
     {
       // We need to make sure that the list of distribution libraries in use 
       // includes KELP
          pGlobalArrays_Distribution->testConsistency(Label);
     }
#endif

#if defined (USE_KELP)
     if (pKELP_Distribution != NULL)
     {
       // We need to make sure that the list of distribution libraries in use
       // includes KELP
          pKELP_Distribution->testConsistency(Label);
     }
#endif

     PADRE_ASSERT (UserCollectionList != NULL);
   }

#if 0
// With more sophisticated use of templates (one which would not be portable)
// we can use a template parameter to replace the float type.  However, to
// be portable we have to have multiple functions for each type.  This 
// restricts the supported types to double float and int but this is only
// temporary.
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
regularSectionTransfer ( const UserLocalDomain & LhsDomain,
                         const float *LhsData,
                         const UserLocalDomain & RhsDomain,
                         const float *RhsData )
   {
  // We need to make sure that the list of distribution libraries in use includes PARTI
     pPARTI_Distribution->regularSectionTransfer(LhsDomain,LhsData,RhsDomain,RhsData);
   }
#endif

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
updateGhostBoundaries()
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Distribution ::"
               << " updateGhostBoundaries()"
               << " id " << getId()
               << " not implemented\n";
#endif
  // Loop through all the PADRE_Representations and call their updateGhostBoundaries() member function!

     printf ("PADRE_Distribution::updateGhostBoundaries() not implemented \n");
     PADRE_ABORT();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
displayDefaultValues ( const char *label )
   {
     int i=0;
     printf ("(static values) PADRE_Distribution::Default_GhostBoundary_Width [%d] = \n",i);
     for (i=0; i < PADRE_MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Default_GhostBoundary_Width[i]);

  // We should consider a design that would permit us to use the following code
  // this would require a common base class to exist for all of the interface classes
  // that would link PADRE to each of the distribution libraries.  This would be a better design.
  // getSublibraryInUse() -> displayDefaultValues(label);

  // Number of UserCollection ojects associated with this PADRE_Distribution
  // printf ("Number of UserCollection objects in the default PADRE_Distribution = %d \n",
  //      getDefaultUserCollectionList()->size());

  // This is a static function so we have to refer to the member data objects differently 
  // (i.e. we have to call static member functions of the member objects).
  // PADRE_ASSERT (pPARTI_Distribution != NULL);
#if !defined(NO_Parti)
     //pPARTI_Distribution->displayDefaultValues(label);
     PARTI_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
        displayDefaultValues(label);
#endif
#if !defined(NO_GlobalArrays)
     GlobalArrays_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
	displayDefaultValues(label);
#endif
#if defined (USE_KELP)
     KELP_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
	displayDefaultValues(label);
#endif
   }


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
display ( const char *label ) const
   {
     int i=0;
     printf ("(static values) PADRE_Distribution::Default_GhostBoundary_Width [%d] = \n",i);
     for (i=0; i < PADRE_MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Default_GhostBoundary_Width[i]);

     printf ("PADRE_Distribution::Distribution_Dimension = %d \n",Distribution_Dimension);

  // Number of UserCollection ojects associated with this PADRE_Distribution
     printf ("Number of UserCollection objects in this PADRE_Distribution = %d \n",
          getUserCollectionList()->size());

     PADRE_CommonInterface::display("PADRE_CommonInterface");
     
#if !defined(NO_Parti)
     PADRE_ASSERT (pPARTI_Distribution != NULL );
     if (pPARTI_Distribution != NULL)
       pPARTI_Distribution->display(label);
#endif
#if !defined(NO_GlobalArrays)
     PADRE_ASSERT (pGlobalArrays_Distribution != NULL);
     if (pGlobalArrays_Distribution != NULL)
       pGlobalArrays_Distribution->display(label);
#endif
#if defined (USE_KELP)
     PADRE_ASSERT (pKELP_Distribution != NULL);
     if (pKELP_Distribution != NULL)
       pKELP_Distribution->display(label);
#endif
   }

// ********* Reference Count Information 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
   displayReferenceCounts ( const char* label ) const
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Distribution::displayReferenceCounts()", "void(void)",
		  TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

     printf ("     PADRE_Distribution<...>::getDefaultDistributionPointer()");
     printf ("->getReferenceCount() = %d \n",
          PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>
               ::getDefaultDistributionPointer()->getReferenceCount());
#if !defined(NO_Parti)
     if (PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
	 getDefaultDistributionPointer()->getpPARTI_DistributionPointer() != NULL)
        {
          printf ("     PADRE_Distribution<...>::getDefaultDistributionPointer()");
          printf ("->getpPARTI_DistributionPointer()->getReferenceCount() = %d \n",
               PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>
               ::getDefaultDistributionPointer()->getpPARTI_DistributionPointer()->getReferenceCount());
        }
#endif
#if !defined(NO_GlobalArrays)
     if (PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
	 getDefaultDistributionPointer()->getpGlobalArrays_DistributionPointer() != NULL)
        {
          printf ("     PADRE_Distribution<...>::getDefaultDistributionPointer()");
          printf ("->getpGlobalArrays_DistributionPointer()->getReferenceCount() = %d \n",
               PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>
               ::getDefaultDistributionPointer()->getpGlobalArrays_DistributionPointer()->getReferenceCount());
        }
#endif
#if defined (USE_KELP)
     if (PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>::
	 getDefaultDistributionPointer()->getpKELP_DistributionPointer() != NULL)
        {
          printf ("     PADRE_Distribution<...>::getDefaultDistributionPointer()");
          printf ("->getpKELP_DistributionPointer()->getReferenceCount() = %d \n",
               PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>
               ::getDefaultDistributionPointer()->getpKELP_DistributionPointer()->getReferenceCount());
        }
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
bool PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
Has_Same_Ghost_Boundary_Widths ( const UserGlobalDomain & Lhs_Domain,
                                 const UserGlobalDomain & Rhs_Domain )
   {
     bool Return_Has_Same_Ghost_Boundary_Widths = true;
     int temp;
     for ( temp=0; temp < PADRE_MAX_ARRAY_DIMENSION; temp++ )
        {
       // This works (using the &= operator) and is cute but not as clear as an explicit conditional
       // Return_Has_Same_Ghost_Boundary_Widths &= (Lhs_Domain.InternalGhostCellWidth[temp] == Rhs_Domain.InternalGhostCellWidth[temp]);
          if (Lhs_Domain.InternalGhostCellWidth[temp] != Rhs_Domain.InternalGhostCellWidth[temp])
               Return_Has_Same_Ghost_Boundary_Widths = false;
        }

     return Return_Has_Same_Ghost_Boundary_Widths;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
bool PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain> ::
Has_Same_Ghost_Boundary_Widths ( const UserGlobalDomain & This_Domain,
                                 const UserGlobalDomain & Lhs_Domain,
                                 const UserGlobalDomain & Rhs_Domain )
   {
     bool Temp_1_Has_Same_Ghost_Boundary_Widths = true;
     bool Temp_2_Has_Same_Ghost_Boundary_Widths = true;
     int temp;
     for ( temp=0; temp < PADRE_MAX_ARRAY_DIMENSION; temp++ )
        {
       // This works (using the &= operator) and is cute but not as clear as an explicit conditional
       // Temp_1_Has_Same_Ghost_Boundary_Widths &= (This_Domain.InternalGhostCellWidth[temp] == Rhs_Domain.InternalGhostCellWidth[temp]);
       // Temp_2_Has_Same_Ghost_Boundary_Widths &= (Lhs_Domain.InternalGhostCellWidth [temp] == Rhs_Domain.InternalGhostCellWidth[temp]);
          if (This_Domain.InternalGhostCellWidth[temp] != Rhs_Domain.InternalGhostCellWidth[temp])
               Temp_1_Has_Same_Ghost_Boundary_Widths = false;
          if (Lhs_Domain.InternalGhostCellWidth[temp] != Rhs_Domain.InternalGhostCellWidth[temp])
               Temp_2_Has_Same_Ghost_Boundary_Widths = false;
        }

     return Temp_1_Has_Same_Ghost_Boundary_Widths && Temp_2_Has_Same_Ghost_Boundary_Widths;
   }


// PADRE_DISTRIBUTION_C
#endif
