// file:  PADRE_Representation.C

#ifndef PADRE_REPRESENTATION_C
#define PADRE_REPRESENTATION_C

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <PADRE_Global.h>
#include <PADRE_Distribution.h>
#include <PADRE_Representation.h>

// Memory Management
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>::freeMemoryInUse ()
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Representation::freeMemoryInUse()", "void(void)", TAU_PADRE_MEMORY_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PADRE_Representation::freeMemoryInUse (possible memory leak) \n");
#endif

#if !defined(NO_Parti)
        PARTI_Representation  <UserCollection, UserGlobalDomain, UserLocalDomain>::
	   freeMemoryInUse ();
#endif
#if !defined(NO_GlobalArrays)
        GlobalArrays_Representation <UserCollection, UserGlobalDomain, UserLocalDomain>::
	   freeMemoryInUse ();
#endif
#if defined (USE_KELP)
        KELP_Representation  <UserCollection, UserGlobalDomain, UserLocalDomain>::
	   freeMemoryInUse ();
#endif

  // printf ("At BASE of PADRE_Representation::freeMemoryInUse \n");
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
~PADRE_Representation()
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PADRE_Representation ::~PADRE_Representation(): referenceCount = %d \n",getReferenceCount());
#endif

     PADRE_ASSERT (getReferenceCount() == getReferenceCountBase()-1);
  // Initialize to default values
  // delete globalDomain;
  // globalDomain = NULL;

  // delete pPARTI_Representation;
  // pPARTI_Representation = NULL;
  // printf ("In ~PADRE_Representation(): delete pPARTI_Representation \n");
#if !defined(NO_Parti)
     if (pPARTI_Representation != NULL)
        {
          pPARTI_Representation->decrementReferenceCount();
          if (pPARTI_Representation->getReferenceCount() < pPARTI_Representation->getReferenceCountBase())
               delete pPARTI_Representation;
          pPARTI_Representation = NULL;
        }
#endif

#if !defined(NO_GlobalArrays)
     if (pGlobalArrays_Representation != NULL)
        {
          pGlobalArrays_Representation->decrementReferenceCount();
          if (pGlobalArrays_Representation->getReferenceCount() < 
	      pGlobalArrays_Representation->getReferenceCountBase())
               delete pGlobalArrays_Representation;
          pGlobalArrays_Representation = NULL;
        }
#endif

#if defined (USE_KELP)
     if (pKELP_Representation != NULL)
        {
          pKELP_Representation->decrementReferenceCount();
          if (pKELP_Representation->getReferenceCount() < 
	      pKELP_Representation->getReferenceCountBase())
               delete pKELP_Representation;
          pKELP_Representation = NULL;
        }
#endif

  // delete Distribution;
  // Distr = NULL;
  // printf ("In ~PADRE_Representation(): delete Distr \n");
     if (distribution != NULL)
        {
          distribution->decrementReferenceCount();
          if (distribution->getReferenceCount() < distribution->getReferenceCountBase())
               delete distribution;
          distribution = NULL;
        }

  // printf ("In ~PADRE_Representation(): delete globalDomain \n");
     if (globalDomain != NULL)
        {
          globalDomain->decrementReferenceCount();
          if (globalDomain->getReferenceCount() < globalDomain->getReferenceCountBase())
               delete globalDomain;
          globalDomain = NULL;
        }

  // if (associatedUserCollections != NULL)
  //    {
  //      delete associatedUserCollections;
  //      associatedUserCollections = NULL;
  //    }

   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PADRE_Representation()
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Representation ::"
               << " PADRE_Representation()"
               << " id " << getId() << "\n";
#endif

  // We have no concept of a default globalDoamin.  The globalDomain must be
  // provided by the user.  So the default constructor for a representation makes
  // no sense.  Still we must define one so that the C++ compiler will not build it for us.
     printf ("The default constructor makes no sense because we can have no default globalDomain \n");
     PADRE_ABORT();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PADRE_Representation( const UserLocalDomain *inputGlobalDomain )
   {
  // A PADRE_Representation object must specify a domain since
  // It is generally most useful to build a PADRE_Representation and specify the 
  // PADRE_Distribution within the constructor. This avoids the redundent
  // construction of PADRE_Distribution objects and more readily permits them
  // to be shared between different PADRE_Representation.  It is expected that
  // most applications would want to have as few PADRE_Distribution objects
  // as possible since this simplifies the dynamic control of data within an 
  // application. But we can't enforce this so we provide this constructor.

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Representation ::"
               << " PADRE_Representation(UserLocalDomain*)"
               << " id " << getId() << "\n";
#endif

     PADRE_ASSERT (inputGlobalDomain != NULL);
     inputGlobalDomain->incrementReferenceCount();
     globalDomain = (UserLocalDomain *) inputGlobalDomain;
  // Initialize the translation independent domain of the PADRE_Representation
     globalDomain->normalize();

  // globalDomain->display("globalDomain in PADRE_Representation(UserLocalDomain*) constructor");

  // use the PADRE_Distribution default constructor
  // This should generate a default behavior
  // The PADRE_Distribution object now contains a static pointer to a PADRE_Distribution
  // object which is used as a default.  This is the design I always wanted to have but it
  // was problematic in previous implementations of this idea (in P++).  The difference
  // here is that there is an extra level of abstraction to hide this detail in PADRE.
  // Distribution = new PADRE_Distribution<UserCollection, UserGlobalDomain, UserLocalDomain>();
     distribution = Distr::getDefaultDistributionPointer();
     distribution->incrementReferenceCount();

  // use the PARTI_Representation default constructor
  // This should generate a default behavior
     PADRE_ASSERT (distribution != NULL);

  // Initialize pointers to NULL so garbage values don't cause problems.

#if !defined(NO_Parti)
     pPARTI_Representation = NULL; 
#endif
#if !defined(NO_GlobalArrays)
     pGlobalArrays_Representation  = NULL; 
#endif
#if defined (USE_KELP)
     pKELP_Representation  = NULL; 
#endif

#if !defined(NO_Parti)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::PARTI ) ) {
     pPARTI_Representation = 
       new PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>
          (distribution->getpPARTI_Distribution(),globalDomain);
     }
#endif

#if defined (USE_KELP)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::KELP ) ) {
     pKELP_Representation = 
       new KELP_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>
          (distribution->getpKELP_Distribution(),globalDomain);
     }
#endif

#if !defined(NO_GlobalArrays)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::GlobalArrays ) ) {
     pGlobalArrays_Representation = 
       new GlobalArrays_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>
          (distribution->getpGlobalArrays_Distribution(),globalDomain);
     }
#endif

     initialize();
     testConsistency ("Called from PADRE_Representation() constructor! \n");
   }


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PADRE_Representation( const PADRE_Representation & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Represention ::"
               << " PADRE_Represention( const PADRE_Represention & X )"
               << " id " << getId() << "\n";
#endif

     operator=(X);
     testConsistency ("Called from PADRE_Representation(PADRE_Representation) constructor! \n");
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> & 
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
operator= ( const PADRE_Representation & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Represention ::"
               << " PADRE_Represention & PADRE_Represention::operator= ( const PADRE_Represention & X )"
               << " id " << getId() << "\n";
#endif

     printf ("Inside of PADRE_Represention::operator= \n");

     if (X.globalDomain != NULL)
          X.globalDomain->incrementReferenceCount();
     globalDomain = X.globalDomain;
  // Should we share the distribution?  I think not since this would restrict the usefulness of the operator=
     distribution = new Distr(*X.distribution);

  // Initialize pointers to NULL so garbage values don't cause problems.

     pPARTI_Representation = NULL; 

#if !defined(NO_Parti)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::PARTI ) ) {
     pPARTI_Representation = 
       new PARTI_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> 
          (*X.getPARTI_Representation());
     }
#endif

#if !defined(NO_GlobalArrays)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::GlobalArrays ) ) {
     pGlobalArrays_Representation  = NULL; 
     pGlobalArrays_Representation = 
       new GlobalArrays_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> 
          (*X.getGlobalArrays_Representation());
     }
#endif

#if defined (USE_KELP)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::KELP ) ) {
     pKELP_Representation = 
       new KELP_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> 
          (*X.getKELP_Representation());
     }
#endif

     initialize();
     testConsistency ("Called from PADRE_Representation::operator=(PADRE_Representation) constructor! \n");
     return *this;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
PADRE_Representation( const UserLocalDomain *inputGlobalDomain, 
                      const Distr *inputDistribution )
   {
  // This is the most useful of the constructors defined within this class.

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "Entering:"
               << " PADRE_Represention( const UserLocalDomain & inputGlobalDomain," 
               << " PADRE_Distribution<UC,ULD,UD> *inputDistribution )"
               << " id " << getId() << "\n";
#endif

     PADRE_ASSERT (inputDistribution != NULL);
     if (inputDistribution != NULL);
          inputDistribution->incrementReferenceCount();
     distribution = (Distr *) inputDistribution;

  // initialize the global domain (thought we want to 
  // enforce a default base to permit invariance to translation)
  // ?? globalDomain.translateBase( 0 );
     if (inputGlobalDomain != NULL)
          inputGlobalDomain->incrementReferenceCount();
     globalDomain = (UserLocalDomain*) inputGlobalDomain;
     globalDomain->normalize();


  // Initialize pointers to NULL so garbage values don't cause problems.

#if !defined(NO_Parti)
     pPARTI_Representation = NULL; 
#endif
#if !defined(NO_GlobalArrays)
     pGlobalArrays_Representation  = NULL; 
#endif
#if defined (USE_KELP)
     pKELP_Representation  = NULL; 
#endif

  // We have a switch statement within this constructor.
  // All the constructors should have this, but it has not been implemented yet
  // since at present we are using only a single distribution library
     {
       const SublibraryNames::Container &sublibs=distribution->getSublibraryInUse();
       SublibraryNames::Container::const_iterator sublib;
       for ( sublib=sublibs.begin(); sublib!=sublibs.end(); sublib++ ) {
	 switch (*sublib) { 
          case SublibraryNames::NONE:
               cout << "ERROR: distribution->getSublibraryInUse() id " << distribution->getId()
	            << " is libNONE\n";
               PADRE_ABORT();
               break;

#if !defined(NO_Parti)
          case SublibraryNames::PARTI: 
               pPARTI_Representation = 
	         new PARTI_Representation
		   <UserCollection, UserGlobalDomain, UserLocalDomain>
	           (distribution->getpPARTI_Distribution(), globalDomain );
               PADRE_ASSERT( pPARTI_Representation != NULL );
               break;
#endif

#if !defined(NO_GlobalArrays)
          case SublibraryNames::GlobalArrays: 
               pGlobalArrays_Representation = 
	         new GlobalArrays_Representation
		   <UserCollection, UserGlobalDomain, UserLocalDomain>
	           (distribution->getpGlobalArrays_Distribution(), globalDomain );
               PADRE_ASSERT( pGlobalArrays_Representation != NULL );
               break;
#endif
#if defined (USE_KELP)
          case SublibraryNames::KELP: 
               pKELP_Representation = 
	         new KELP_Representation
		   <UserCollection, UserGlobalDomain, UserLocalDomain>
	           (distribution->getpKELP_Distribution(), globalDomain );
               PADRE_ASSERT( pKELP_Representation != NULL );
               break;
#endif

          case SublibraryNames::DomainLayout:
#if 0      
               DomainMap_RepresentationPointer = 
	         new DOMAIN_MAP_Representation( distribution->DOMAIN_MAP_Distribution, globalDomain );
               PADRE_ASSERT( DomainMap_Representation != NULL );
#else
               PADRE_ABORT();
#endif
               break;

          case SublibraryNames::ANY:
               cout << "ERROR: distribution->getSublibraryInUse() id " << distribution->getId()
	            << " is libANY\n";
               PADRE_ABORT();
               break;

          default: 
               cout << "ERROR: distribution->getSublibraryInUse() id " << distribution->getId()
	            << " is default\n";
               cout << distribution;
               PADRE_ABORT();
	 }
       }
     }

     initialize();

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << " PADRE_Represention( const UserLocalDomain & inputGlobalDomain," 
               << " PADRE_Distribution<UC,ULD,UD> distribution )"
               << " id " << getId() << "\n";
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>::
initialize()
   {
  // The PADRE_Representation class has so few data members that this function is not very useful
  // pPARTI_Representation = NULL;
  // pDomainLayout_Representation = NULL;
  // pPGSLib_Representation = NULL;

#if 1
     if ( PADRE::isSublibraryPermitted( SublibraryNames::PARTI ) ) {
          setSublibraryInUse( SublibraryNames::PARTI );
     }
#if !defined(NO_GlobalArrays)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::GlobalArrays ) ) {
          setSublibraryInUse( SublibraryNames::GlobalArrays );
     }
#endif
#if defined (USE_KELP)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::KELP ) ) {
          setSublibraryInUse( SublibraryNames::KELP );
     }
#endif
#else
     if (pPARTI_Representation != NULL)
          setSublibraryInUse( SublibraryNames::PARTI );
#if !defined(NO_GlobalArrays)
     else if (pGlobalArrays_Representation != NULL)
          setSublibraryInUse( SublibraryNames::GlobalArrays );
#endif
#if defined (USE_KELP)
     else if (pKELP_Representation != NULL)
          setSublibraryInUse( SublibraryNames::KELP );
#endif
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
SublibraryNames::Container&
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
setSublibraryInUse( SublibraryNames::Name X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Distribution<UC,ULD,UD> ::"
               << " setSublibraryInUse(" << X << ")\n";
#endif

     subLibrariesInUse.insert( X );
     // PADRE_ASSERT( subLibrariesInUse.size() == 1 ); // Currently cannot handle multiples.
     return subLibrariesInUse;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
const SublibraryNames::Container&
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
getSublibraryInUse() const
   {
     PADRE_ASSERT( !subLibrariesInUse.empty() );
     // PADRE_ASSERT( subLibrariesInUse.size() == 1 ); // Currently cannot handle multiples.
     return subLibrariesInUse;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
SublibraryNames::Name
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> :: 
subLibraryGreatestLowerBound( const SublibraryNames::Name & X, 
			      const SublibraryNames::Name & Y )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "SublibraryNames::Name"
               << "PADRE_Distribution<UC,ULD,UD> ::"
               << " subLibraryGreatestLowerBound(" << X << "," << Y << ")"
               << " not implemented, returning NONE\n";
#endif
     return SublibraryNames::NONE;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
UserLocalDomain &
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
getGlobalDomain() const
   {
  // This function casts away cost (we might want to implement it better at some later point)
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_Representation<...>::getGlobalDomain() "
               << endl;
#endif

  // PADRE_ABORT();
     return *(((PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>*) this)->globalDomain);
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
UserLocalDomain *
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
getGlobalDomainPointer() const
   {
  // This function casts away cost (we might want to implement it better at some later point)
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PADRE_Representation::getGlobalDomainPointer()   PADRE::debugLevel() = %d \n",PADRE::debugLevel());
#endif

  // PADRE_ABORT();
  // return &((UserLocalDomain) globalDomain);
     return ((PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>*) this)->globalDomain;
   }

// following are the definitions of the pure virtual functions declared in PADRE_CommonInterface.h

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
setGhostCellWidth( unsigned axis, unsigned width )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Representation ::"
               << " setGhostCellWidth("
               << "unsigned axis = " << axis << ",unsigned width = " << width << ")"
               << " not implemented\n";
#endif

     PADRE_ABORT();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
getGhostCellWidth( unsigned axis ) const
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "int"
               << " PADRE_Representation ::"
               << " int getGhostCellWidth("
               << "unsigned axis = " << axis << ")"
               << " not implemented, returning 0\n";
#endif

     PADRE_ABORT();
     return 0; 
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
getNumberOfAxesToDistribute() const
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "int"
               << " PADRE_Representation ::"
               << " getNumberOfAxesToDistribute()"
               << " not implemented, returning 0\n";
#endif

     PADRE_ABORT();
     return 0; 
   }

#if 0
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
setDistributeAxis( int Axis )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Representation ::"
               << " setDistributeAxis("
               << "int Axis = " << Axis << ")"
               << " not implemented\n";
#endif

     PADRE_ABORT();
   }
#else
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
DistributeAlongAxis ( int Axis, bool Partition_Axis, int GhostBoundaryWidth )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Representation ::"
               << " DistributeAlongAxis("
               << "int Axis = " << Axis << ")"
               << " not implemented\n";
#endif

     PADRE_ABORT();
   }
#endif

#if 0
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
getProcessorSet( int *ProcessorArray, int & Number_Of_Processors ) const
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Representation ::"
               << " getProcessorSet( int *ProcessorArray, int & Number_Of_Processors )"
               << " not implemented\n";
#endif

     PADRE_ABORT();
   }
#endif

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
swapDistribution( const Distr 
		     & oldDistribution,
		   const Distr 
		     & newDistribution )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Representation ::"
               << " swapDistribution("
               << " const PADRE_Distribution & oldDistribution,"
               << " const PADRE_Distribution & newDistribution )"
               << " not implemented\n";
#endif

     PADRE_ABORT();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
AssociateObjectWithDistribution( const UserCollection & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Representation ::"
               << " AssociateObjectWithDistribution( const UserCollection & X )"
               << " not implemented\n";
#endif

     PADRE_ABORT();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
UnassociateObjectWithDistribution( const UserCollection & X )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Representation ::"
               << " UnassociateObjectWithDistribution( const UserCollection & X )"
               << " not implemented\n";
#endif

     PADRE_ABORT();
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
testConsistency( const char *Label ) const
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Representation ::"
               << " testConsistency( const char *Label = \"" << Label << "\" )"
               << " not implemented\n";
#endif

  // More tests than this will be required!
     PADRE_ASSERT (distribution != NULL);
#if !defined(NO_Parti)
     PADRE_ASSERT (pPARTI_Representation != NULL );
#endif

#if !defined(NO_GlobalArrays)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::GlobalArrays ) ) {
       PADRE_ASSERT (pGlobalArrays_Representation != NULL);
     }
#endif

#if defined (USE_KELP)
     PADRE_ASSERT (pKELP_Representation != NULL);
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
getLocalSizes ( int* LocalSizeArray )
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Representation ::"
               << " getLocalSizes(int*) \n";
#endif

#if !defined(NO_Parti)
     PADRE_ASSERT (pPARTI_Representation != NULL );
#endif
#if !defined(NO_GlobalArrays)
     PADRE_ASSERT (pGlobalArrays_Representation != NULL);
#endif
#if defined (USE_KELP)
     PADRE_ASSERT (pKELP_Representation != NULL);
#endif
#if !defined(NO_Parti)
     if (pPARTI_Representation != NULL )
        pPARTI_Representation->getLocalSizes(LocalSizeArray);
#endif
#if !defined(NO_GlobalArrays)
     if ( pGlobalArrays_Representation != NULL)
        pGlobalArrays_Representation->getLocalSizes(LocalSizeArray);
#endif
#if defined (USE_KELP)
     if ( pKELP_Representation != NULL)
        pKELP_Representation->getLocalSizes(LocalSizeArray);
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
updateGhostBoundaries()
   {
#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Representation ::"
               << " updateGhostBoundaries()"
               << " not implemented\n";
#endif

     PADRE_ABORT();
   }

// findProcNum written by BTNG.
// Find number of processor where indexVals lives
// The indices in indexVals must have NO offset.
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>::findProcNum( int* indexVals ) const {
  const SublibraryNames::Container &sublibs=distribution->getSublibraryInUse();
  SublibraryNames::Container::const_iterator sublib;
  for ( sublib=sublibs.begin(); sublib!=sublibs.end(); sublib++ ) {
    switch (*sublib) { 
#if !defined(NO_Parti)
    case SublibraryNames::PARTI:
      // Use the get_proc_num function defined by PARTI.
      return get_proc_num( pPARTI_Representation->BlockPartiArrayDescriptor,indexVals );
#endif
    default:
      cerr << "Using PADRE sublibrary for which findProcNum has not been written" << endl;
      PADRE_ABORT();
    }
  }
  return -1;
}

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
displayDefaultValues ( const char *Label ) {
  // This is a static function so we have to refer to the member data objects differently
  // (i.e. we have to call static member functions of the member objects).
  // PADRE_ASSERT (pPARTI_Distribution != NULL);
  // pPARTI_Distribution->displayDefaultValues(Label);
     //if (pPARTI_Representation != NULL )
#if !defined(NO_Parti)
  PARTI_Representation<UserCollection,UserGlobalDomain,UserLocalDomain>::
    displayDefaultValues(Label);
#endif
     //else if ( pKELP_Representation != NULL)
#if !defined(NO_GlobalArrays)
  GlobalArrays_Representation<UserCollection,UserGlobalDomain,UserLocalDomain>::
    displayDefaultValues(Label);
#endif
#if defined (USE_KELP)
  KELP_Representation<UserCollection,UserGlobalDomain,UserLocalDomain>::
    displayDefaultValues(Label);
#endif
}


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> ::
display ( const char *Label ) const 
   {
  // This is a static function so we have to refer to the member data objects differently 
  // (i.e. we have to call static member functions of the member objects).

     PADRE_ASSERT (globalDomain != NULL);
  // globalDomain->display(Label);
     if (globalDomain != NULL)
        {
          PADRE_ASSERT (globalDomain != NULL);
          printf ("In PADRE_Representation::display(%s): globalDomain->display() commented out! \n",Label);
       // globalDomain->display(Label);
        }
       else
        {
          printf ("In PADRE_Representation::display(%s) globalDomain == NULL pointer \n",Label);
        }

     PADRE_ASSERT (distribution != NULL);
     distribution->display(Label);

#if !defined(NO_Parti)
     PADRE_ASSERT (pPARTI_Representation != NULL );
     pPARTI_Representation->display(Label);
#endif
#if !defined(NO_GlobalArrays)
     PADRE_ASSERT (pGlobalArrays_Representation != NULL);
     pGlobalArrays_Representation->display(Label);
#endif
#if defined (USE_KELP)
     PADRE_ASSERT (pKELP_Representation != NULL);
     pKELP_Representation->display(Label);
#endif

   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain>::
   displayReferenceCounts ( const char* label ) const
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Representation::displayReferenceCounts()", "void(void)",
		  TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

// Comment this out for now!
#if 0
     printf ("     PADRE_Representation<...>::getDefaultRepresentationPointer()");
     printf ("->getReferenceCount() = %d \n",
          Distr
               ::getDefaultDistributionPointer()->getReferenceCount());
    if (Distr::
	 getDefaultDistributionPointer()->getpPARTI_DistributionPointer() != NULL)
        {
          printf ("     PADRE_Distribution<...>::getDefaultDistributionPointer()");
          printf ("->getpPARTI_DistributionPointer()->getReferenceCount() = %d \n",
               Distr
               ::getDefaultDistributionPointer()->getpPARTI_DistributionPointer()->getReferenceCount());
#if !defined(NO_GlobalArrays)
          printf ("     PADRE_Distribution<...>::getDefaultDistributionPointer()");
          printf ("->getpGlobalArrays_DistributionPointer()->getReferenceCount() = %d \n",
               Distr
               ::getDefaultDistributionPointer()->getpGlobalArrays_DistributionPointer()->getReferenceCount());
#endif
#if defined (USE_KELP)
          printf ("     PADRE_Distribution<...>::getDefaultDistributionPointer()");
          printf ("->getpKELP_DistributionPointer()->getReferenceCount() = %d \n",
               Distr
               ::getDefaultDistributionPointer()->getpKELP_DistributionPointer()->getReferenceCount());
#endif
        }
#endif
   }


// PADRE_REPRESENTATION_C
#endif
