// file:  PADRE_Descriptor.C

#ifndef PADRE_DESCRIPTOR_C
#define PADRE_DESCRIPTOR_C

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <PADRE_Global.h>
#include <PADRE_Descriptor.h>

// *****************************************************************************
//                     STATIC VARIABLE INITIALIZATION
// *****************************************************************************

#if 1
// Indexing for all arrays starts a zero (or what ever this is set to be!)
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::PADRE_Global_Array_Base = 0;
#endif

// *****************************************************************************
//                     DEFINITION OF MEMBER FUNCTIONS 
// *****************************************************************************

// Memory Management
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::freeMemoryInUse ()
   {
     puts("asdf");
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::freeMemoryInUse()", "void(void)", 
		 TAU_PADRE_MEMORY_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
       printf 
	 ("Inside of PADRE_Descriptor::freeMemoryInUse (possible memory leak) \n");
#endif

  // Call to clean up function for the sublibraries
#if !defined(NO_Parti)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::PARTI ) ) {
       PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
	 freeMemoryInUse (); 
     }
#endif
#if !defined(NO_GlobalArrays)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::GlobalArrays ) ) {
       GlobalArrays_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
	 freeMemoryInUse (); 
       }
#endif
#if defined (USE_KELP)
     KELP_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
	freeMemoryInUse (); 
#endif

  // Call the clean up functions for the other PADRE classes
  // We need ot make sure that these are not called explicitly by the user!
     Repre::freeMemoryInUse (); 
     Distr::freeMemoryInUse (); 
  // printf ("At BASE of PADRE_Descriptor::freeMemoryInUse() \n");
   }

// ********* Reference Count Information 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
   displayReferenceCounts ( const char* label ) const
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::displayReferenceCounts()", "void(void)", 
		  TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > -1)
          printf ("Inside of PADRE_Descriptor displayReferenceCounts(%s) = %d \n",label,getReferenceCount());
#endif

     printf ("Reference Counts of PADRE objects: \n");
     if (Distr::getDefaultDistributionPointer() != NULL)
        {
          Distr::
               getDefaultDistributionPointer()->displayReferenceCounts(label);
#if 0
          printf ("     PADRE_Distribution<...>::getDefaultDistributionPointer()");
	  printf ("->getReferenceCount() = %d \n",
               Distr::getDefaultDistributionPointer()->getReferenceCount());
          if (Distr::getDefaultDistributionPointer()->getpPARTI_DistributionPointer() 
		  != NULL)
            printf ("     PADRE_Distribution<...>::getDefaultDistributionPointer()");
	    printf ("->getpPARTI_DistributionPointer()->getReferenceCount() = %d \n",
               Distr::getDefaultDistributionPointer()->getpPARTI_DistributionPointer()
	       ->getReferenceCount());
#endif
        }
     if (globalDomain != NULL)
          printf ("     globalDomain->getReferenceCount() = %d \n",
	     globalDomain->getReferenceCount());
     if (localDomain != NULL)
          printf ("     localDomain->getReferenceCount() = %d \n",
	     localDomain->getReferenceCount());
     if (GlobalDescriptor != NULL)
          printf ("     GlobalDescriptor->getReferenceCount() = %d \n",
	     GlobalDescriptor->getReferenceCount());
     if (representation != NULL)
        {
          representation->displayReferenceCounts(label);
#if 0
          printf ("     representation->getReferenceCount() = %d \n",
	     representation->getReferenceCount());
          if (representation->pPARTI_Representation != NULL)
             {
               printf ("     representation->pPARTI_Representation->");
	       printf ("getReferenceCount() = %d \n",
                       representation->pPARTI_Representation->getReferenceCount());
               if (representation->pPARTI_Representation->globalDomain != NULL)
	       {
                    printf ("     representation->pPARTI_Representation->");
		    printf ("globalDomain->getReferenceCount() = %d \n",
                            representation->pPARTI_Representation->globalDomain->
			    getReferenceCount());
	       }
               if (representation->pPARTI_Representation->Distribution != NULL)
                  {
                    printf ("     representation->pPARTI_Representation->");
		    printf ("Distribution->getReferenceCount() = %d \n",
                            representation->pPARTI_Representation->Distribution->
			    getReferenceCount());
                  }
               if (representation->pPARTI_Representation->BlockPartiArrayDescriptor 
		  != NULL)
               {
                  printf ("     representation->pPARTI_Representation->");
		  printf ("BlockPartiArrayDescriptor->referenceCount = %d \n",
                          representation->pPARTI_Representation->
			  BlockPartiArrayDescriptor->referenceCount);
               }
               if (representation->pPARTI_Representation->
		  BlockPartiArrayDecomposition != NULL)
               {
                    printf ("     representation->pPARTI_Representation->");
		    printf ("BlockPartiArrayDecomposition->referenceCount = %d \n",
                            representation->pPARTI_Representation->
			    BlockPartiArrayDecomposition->referenceCount);
               }
             }
          if (representation->Distribution != NULL)
          {
             printf ("     representation->Distribution->getReferenceCount() = %d \n",
                     representation->Distribution->getReferenceCount());
             if (representation->Distribution->getpPARTI_DistributionPointer() 
		!= NULL)
             {
                  printf ("     representation->Distribution->");
		  printf ("getpPARTI_Distribution().getReferenceCount() = %d \n",
                          representation->Distribution->getpPARTI_Distribution().
			  getReferenceCount());
             }
          }
#endif
        }

#if !defined(NO_Parti)
     if (pPARTI_Descriptor != NULL)
        {
          pPARTI_Descriptor->displayReferenceCounts(label);
#if 0
          printf ("     pPARTI_Descriptor->getReferenceCount() = %d \n",
		  pPARTI_Descriptor->getReferenceCount());
          if (pPARTI_Descriptor->globalDomain != NULL)
               printf ("     pPARTI_Descriptor->globalDomain->");
	       printf ("getReferenceCount() = %d \n",
                        pPARTI_Descriptor->globalDomain->getReferenceCount());
          if (pPARTI_Descriptor->localDomain != NULL)
               printf 
		  ("    pPARTI_Descriptor->localDomain->getReferenceCount() = %d\n",
                   pPARTI_Descriptor->localDomain->getReferenceCount());
          if (pPARTI_Descriptor->localDescriptor != NULL)
               printf 
		  ("    pPARTI_Descriptor->localDescriptor->getReferenceCount()=%d\n",
                    pPARTI_Descriptor->localDescriptor->getReferenceCount());
          if (pPARTI_Descriptor->representation != NULL)
             {
               printf ("    pPARTI_Descriptor->representation->getReferenceCount() = %d \n",
                   pPARTI_Descriptor->representation->getReferenceCount());
               if (pPARTI_Descriptor->representation->Distribution != NULL)
                  {
                    printf ("     pPARTI_Descriptor->representation->");
		    printf ("Distribution->getReferenceCount() = %d \n",
                            pPARTI_Descriptor->representation->Distribution->
			    getReferenceCount());
                  }

               if (pPARTI_Descriptor->representation->BlockPartiArrayDecomposition != NULL)
                  {
                    printf ("     pPARTI_Descriptor->representation->");
		    printf ("BlockPartiArrayDecomposition->referenceCount = %d \n",
                            pPARTI_Descriptor->representation->
			    BlockPartiArrayDecomposition->referenceCount);
                  }
               if (pPARTI_Descriptor->representation->BlockPartiArrayDescriptor != NULL)
                  {
                    printf ("     pPARTI_Descriptor->representation->");
		    printf ("BlockPartiArrayDescriptor->referenceCount = %d \n",
                            pPARTI_Descriptor->representation->
			    BlockPartiArrayDescriptor->referenceCount);
                  }
             }
#endif
        }
#endif

#if !defined(NO_GlobalArrays)
     if (pGlobalArrays_Descriptor != NULL)
        {
          pGlobalArrays_Descriptor->displayReferenceCounts(label);
        }
#endif

#if defined (USE_KELP)
     if (pKELP_Descriptor != NULL)
        {
          pKELP_Descriptor->displayReferenceCounts(label);
        }
#endif
   }

// ********* Default Destructor 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
~PADRE_Descriptor ()
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
     TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of PADRE_Descriptor destructor referenceCount = %d \n",
		  getReferenceCount());
#endif

     PADRE_ASSERT (getReferenceCount() == getReferenceCountBase()-1);
  // displayReferenceCounts("Inside of ~PADRE_Descriptor()");

  // printf ("Inside of ~PADRE_Descriptor: delete the globalDomain \n");
     if (globalDomain != NULL)
        {
       // printf ("globalDomain->getReferenceCount() = %d \n",
       //         globalDomain->getReferenceCount());
          globalDomain->decrementReferenceCount();
          if (globalDomain->getReferenceCount() < 
	      globalDomain->getReferenceCountBase())
               delete globalDomain;
          globalDomain = NULL;
        }

  // printf ("Inside of ~PADRE_Descriptor: delete the localDomain \n");
     if (localDomain != NULL)
        {
       // printf ("localDomain->getReferenceCount() = %d \n",
       //          localDomain->getReferenceCount());
          localDomain->decrementReferenceCount();
          if (localDomain->getReferenceCount() < 
	     localDomain->getReferenceCountBase())
               delete localDomain;
          localDomain = NULL;
        }

  // printf ("Inside of ~PADRE_Descriptor: delete the GlobalDescriptor \n");
     if (GlobalDescriptor != NULL)
        {
       // printf ("GlobalDescriptor->getReferenceCount() = %d \n",
       //          GlobalDescriptor->getReferenceCount());
          GlobalDescriptor->decrementReferenceCount();
          if (GlobalDescriptor->getReferenceCount() < 
	     GlobalDescriptor->getReferenceCountBase())
               delete GlobalDescriptor;
          GlobalDescriptor = NULL;
        }

  // printf ("Inside of ~PADRE_Descriptor: delete the representation \n");
     if (representation != NULL)
        {
       // printf ("representation->getReferenceCount() = %d \n",
       //          representation->getReferenceCount());
          representation->decrementReferenceCount();
          if (representation->getReferenceCount() < 
	      representation->getReferenceCountBase())
               delete representation;
          representation = NULL;
        }

#if !defined(NO_Parti)
  // printf ("Inside of ~PADRE_Descriptor: delete the pPARTI_Descriptor \n");
     if (pPARTI_Descriptor != NULL)
        {
       // printf ("pPARTI_Descriptor->getReferenceCount() = %d \n",
       //          pPARTI_Descriptor->getReferenceCount());
          pPARTI_Descriptor->decrementReferenceCount();
          if (pPARTI_Descriptor->getReferenceCount() < 
	      pPARTI_Descriptor->getReferenceCountBase())
               delete pPARTI_Descriptor;
          pPARTI_Descriptor = NULL;
        }
#endif

#if !defined(NO_GlobalArrays)
  // printf ("Inside of ~PADRE_Descriptor: delete the pGlobalArrays_Descriptor \n");
     if (pGlobalArrays_Descriptor != NULL)
        {
       // printf ("pGlobalArrays_Descriptor->getReferenceCount() = %d \n",
       //          pGlobalArrays_Descriptor->getReferenceCount());
          pGlobalArrays_Descriptor->decrementReferenceCount();
          if (pGlobalArrays_Descriptor->getReferenceCount() < 
	      pGlobalArrays_Descriptor->getReferenceCountBase())
               delete pGlobalArrays_Descriptor;
          pGlobalArrays_Descriptor = NULL;
        }
#endif

#if defined (USE_KELP)
  // ... delete the pKELP_Descriptor ... 
     if (pKELP_Descriptor != NULL)
     {
	// .. note: no reference counting for kelp ...
        //pKELP_Descriptor->decrementReferenceCount();
        //if (pKELP_Descriptor->getReferenceCount() < 
	//    pKELP_Descriptor->getReferenceCountBase())
               delete pKELP_Descriptor;
        pKELP_Descriptor = NULL;
     }
#endif

  // globalDomain      = NULL;
  // localDomain       = NULL;
  // GlobalDescriptor   = NULL;
  // pPARTI_Descriptor = NULL;
   }

// ********* Default Constructor 
// Default PADRE_Descriptor does not make any sence (but it is decalred private in the 
// interface to trigger a compiler error if the user tries to use it).
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>:: 
PADRE_Descriptor ()
   {
  // Note: in the case of P++ this generates recursive calls 
  // because the Array_Domain_Type contains a PADRE_Descriptor object!
  // This is the principle reason why we implement this class with pointers.

#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		  TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0) {
          printf ("Inside of top of PADRE_Descriptor(void) constructor body ");
	  printf ("(this = %p) \n",this);
     }
#endif

     globalDomain      = NULL;
     localDomain       = NULL;
     GlobalDescriptor   = NULL;
     representation    = NULL;
#if !defined(NO_Parti)
     pPARTI_Descriptor = NULL;
#endif
#if !defined(NO_GlobalArrays)
     pGlobalArrays_Descriptor  = NULL;
#endif
#if defined (USE_KELP)
     pKELP_Descriptor  = NULL;
#endif
   }

// ********* Constructor 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>:: PADRE_Descriptor 
     ( const UserGlobalDomain *inputGlobalDescriptor )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of top of PADRE_Descriptor(UserGlobalDomain*) constructor body (this = %p) \n",this);
#endif

     intializeConstructor (inputGlobalDescriptor);

  // printf ("PADRE: Leaving PADRE_Descriptor (UserGlobalDomain*) constructor \n");
   }

// ********* Constructor 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>:: PADRE_Descriptor 
     ( const UserGlobalDomain *inputGlobalDescriptor,
       const Distr *inputDistribution )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		  TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of top of PADRE_Descriptor(UserGlobalDomain*) constructor body (this = %p) \n",this);
#endif

     intializeConstructor (inputGlobalDescriptor,inputDistribution);

  // printf ("PADRE: Leaving PADRE_Descriptor (UserGlobalDomain*,inputDistribution*) constructor \n");
   }

// ********* Constructor 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>:: PADRE_Descriptor 
   ( const UserLocalDomain *inputGlobalDomain,
     const UserLocalDomain *inputLocalDomain,
     const UserGlobalDomain *inputGlobalDescriptor )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		  TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("Inside of top of PADRE_Descriptor(UserLocalDomain*,UserLocalDomain*,UserGlobalDomain*) constructor body (this = %p) \n",this);
#endif

     intializeConstructor (inputGlobalDomain,inputLocalDomain,inputGlobalDescriptor);

  // printf ("PADRE: Leaving PADRE_Descriptor (UserGlobalDomain*,UserLocalDomain*) constructor \n");
  // printf ("Exiting at base of PADRE_Descriptor (UserGlobalDomain*,UserLocalDomain*) constructor \n");
  // PADRE_ABORT();
   }

// ********* Constructor 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
PADRE_Descriptor( const UserLocalDomain *inputGlobalDomain,
                  const UserLocalDomain *inputLocalDomain,
                  const Repre *inputRepresentation,
                  const UserGlobalDomain *inputGlobalDescriptor )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "entering:"
               << " PADRE_Descriptor(UserLocalDomain*,UserLocalDomain*,"
	       << "PADRE_Representation<*,*,*>,UserGlobalDomain*)"
               << endl;
#endif

     intializeConstructor 
(inputGlobalDomain,inputLocalDomain,inputRepresentation,inputGlobalDescriptor);
   }

// ********* Constructor 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
PADRE_Descriptor( const PADRE_Descriptor *inputPADRE_Descriptor,
		  const UserLocalDomain *inputGlobalDomain,
                  const UserLocalDomain *inputLocalDomain,
                  const Repre *inputRepresentation,
                  const UserGlobalDomain *inputGlobalDescriptor )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "entering:"
               << " PADRE_Descriptor(UserLocalDomain*,UserLocalDomain*,"
	       << "PADRE_Representation<*,*,*>,UserGlobalDomain*)"
               << endl;
#endif

     intializeConstructor 
(inputPADRE_Descriptor,inputGlobalDomain,inputLocalDomain,inputRepresentation,inputGlobalDescriptor);
   }


// ********* Constructor 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
PADRE_Descriptor( const PADRE_Descriptor & X )
   {
     printf ("PADRE_Descriptor copy constructor not implemented! \n");
     PADRE_ABORT();
   }

// ********* operator= 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain> & 
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
operator= ( const PADRE_Descriptor & X )
   {
     printf ("PADRE_Descriptor::operator=  not implemented! \n");
     PADRE_ABORT();

     return *this;
   }

// ********* Member function for initialization of constructors 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
intializeConstructor ( const UserGlobalDomain *inputGlobalDescriptor )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "entering:" 
	       << " PADRE_Descriptor::intializeConstructor(UserGlobalDomain*)" 
	       << endl;
#endif

     PADRE_ASSERT (inputGlobalDescriptor != NULL);

     UserLocalDomain* inputGlobalDomain = new UserLocalDomain(*inputGlobalDescriptor);
     PADRE_ASSERT (inputGlobalDomain != NULL);

     UserLocalDomain* inputLocalDomain  = NULL;
     Repre *inputRepresentation = new Repre (inputGlobalDomain);
     PADRE_ASSERT (inputRepresentation != NULL);

  // Now call the main intializeConstructor member function!
     intializeConstructor
       (inputGlobalDomain,inputLocalDomain,inputRepresentation,inputGlobalDescriptor);

  // The intializeConstructor will increment the reference count but we lose the 
  // reference from inputRepresentation to we ave to decrement the reference count 
  // on inputRepresentation.
     inputGlobalDomain->decrementReferenceCount();
     inputRepresentation->decrementReferenceCount();
   }

// ********* Member function for initialization of constructors
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
intializeConstructor 
   ( const UserGlobalDomain *inputGlobalDescriptor,
     const Distr *inputDistribution )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "entering:" 
	       << " PADRE_Descriptor::intializeConstructor(UserGlobalDomain*)" 
	       << endl;
#endif

     PADRE_ASSERT (inputGlobalDescriptor != NULL);

     UserLocalDomain* inputGlobalDomain = new UserLocalDomain(*inputGlobalDescriptor);

     UserLocalDomain* inputLocalDomain  = NULL;
     Repre *inputRepresentation =
          new Repre( inputGlobalDomain,inputDistribution );
     PADRE_ASSERT (inputRepresentation != NULL);

  // Now call the main intializeConstructor member function!
     intializeConstructor
       (inputGlobalDomain,inputLocalDomain,inputRepresentation,inputGlobalDescriptor);

  // The intializeConstructor will increment the reference count but we lose the 
  // reference from inputRepresentation to we ave to decrement the reference count 
  // on inputRepresentation.
     inputGlobalDomain->decrementReferenceCount();
     inputRepresentation->decrementReferenceCount();
   }

// ********* Member function for initialization of constructors 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
intializeConstructor ( 
     const UserLocalDomain *inputGlobalDomain,
     const UserLocalDomain *inputLocalDomain,
     const UserGlobalDomain *inputGlobalDescriptor )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "entering:"
               << " PADRE_Descriptor(UserLocalDomain*,UserLocalDomain*,UserGlobalDomain*)"
               << endl;
#endif

     Repre * inputRepresentation =
          new Repre(inputGlobalDomain);
     PADRE_ASSERT (inputRepresentation != NULL);

     printf ("Exiting in PADRE_Descriptor::intializeConstructor(UserLocalDomain*,UserLocalDomain*,UserGlobalDomain*) \n");
     PADRE_ABORT();

  // Now call the main intializeConstructor member function!
     intializeConstructor 
       (inputGlobalDomain,inputLocalDomain,inputRepresentation,inputGlobalDescriptor);
   }

// ********* Member function for initialization of constructors 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
intializeConstructor ( 
     const UserLocalDomain *inputGlobalDomain,
     const UserLocalDomain *inputLocalDomain,
     const Repre *inputRepresentation,
     const UserGlobalDomain *inputGlobalDescriptor )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "entering:" 
               << " PADRE_Descriptor::intializeConstructor"
	       << "(UserLocalDomain*,UserLocalDomain*,PADRE_Representation<*,*,*>*,"
	       << "UserGlobalDomain*)" 
               << endl;
#endif

  // Call the base class initialize member function (this initializes the id 
  // number and reference count)
  // This is called by the PADRE_CommonInterface default constructor.
  // initialize();

     PADRE_ASSERT (inputRepresentation != NULL);
  // if (inputRepresentation != NULL)
     inputRepresentation->incrementReferenceCount();
  // Cast away const here to avoid compiler warning
     representation = (Repre *) inputRepresentation;
     PADRE_ASSERT(representation != NULL);

     if (inputGlobalDescriptor != NULL)
          inputGlobalDescriptor->incrementReferenceCount();
     GlobalDescriptor = (UserGlobalDomain*) inputGlobalDescriptor;
     PADRE_ASSERT(GlobalDescriptor != NULL);

  // We don't want to get the globalDomain from the inputRepresentation because then
  // it would have a translation invariant base (i.e. zero base).
  // The alternative is to use the globalDomain from the PADRE_Representation
  // and keep track of the translation of the base.
  // globalDomain = inputRepresentation->getGlobalDomainPointer();
     if (inputGlobalDomain != NULL)
          inputGlobalDomain->incrementReferenceCount();
     globalDomain = (UserLocalDomain*) inputGlobalDomain;

#if 1
  // These should be the same!
     PADRE_ASSERT (globalDomain == inputGlobalDomain);
     PADRE_ASSERT (globalDomain == representation->getGlobalDomainPointer());
     //PADRE_ASSERT (globalDomain == representation->getPARTI_Representation()->
     //		   getGlobalDomainPointer());
#endif

  // globalDomain = (UserLocalDomain*) inputGlobalDomain;
    
  // During initial testing we will assume that global base is zero
  // that is a simplification.
  // Store the bases for each dimension
  // for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
  //      TranslationBase [i] = 0;

     if (inputLocalDomain != NULL)
          inputLocalDomain->incrementReferenceCount();
     localDomain = (UserLocalDomain*) inputLocalDomain;
  // This should fail!!!
  // PADRE_ASSERT(localDomain != NULL);
    

  // Initialize so garbage values in pointers won't cause problems.

#if !defined(NO_Parti)
     pPARTI_Descriptor = NULL;
#endif
#if !defined(NO_GlobalArrays)
     pGlobalArrays_Descriptor  = NULL;
#endif
#if defined (USE_KELP)
     pKELP_Descriptor  = NULL;
#endif

  // allocate the corresponding distributionLibrary object

     {
       const SublibraryNames::Container &sublibs=representation->getSublibraryInUse();
       SublibraryNames::Container::const_iterator sublib;
       for ( sublib=sublibs.begin(); sublib!=sublibs.end(); sublib++ ) {
	 switch (*sublib) { 
          case SublibraryNames::NONE:
	       cout << "Error in:"
	            << " PADRE_Descriptor( const PADRE_Representation<*,*,*> & "
		    << "inputRepresentation ) switch case LibNONE"
	            << endl;
	       PADRE_ABORT();
	       break;

          case SublibraryNames::Trivial:
	       cout << "Error in:"
	            << " PADRE_Descriptor( const PADRE_Representation<*,*,*> & "
		    << "inputRepresentation ) switch case LibTrivial"
	            << endl;
	       PADRE_ABORT();
	       break;

#if !defined(NO_Parti)
          case SublibraryNames::PARTI: 
	       pPARTI_Descriptor = 
	         new PARTI_Descriptor<UserCollection,UserGlobalDomain,UserLocalDomain>
                      (globalDomain,localDomain,representation->
		      getPARTI_Representation(),GlobalDescriptor);
	       break;
#endif

#if !defined(NO_GlobalArrays)
          case SublibraryNames::GlobalArrays: 
	    // PADRE_ABORT(); // GlobalArrays needs the PADRE_Descriptor not available here.
	       pGlobalArrays_Descriptor = 
	         new GlobalArrays_Descriptor<UserCollection,UserGlobalDomain,UserLocalDomain>
                      (NULL,globalDomain,localDomain,representation->
		      getGlobalArrays_Representation(),GlobalDescriptor);
	       break;
#endif

#if defined (USE_KELP)
          case SublibraryNames::KELP: 
	       pKELP_Descriptor = 
	         new KELP_Descriptor<UserCollection,UserGlobalDomain,UserLocalDomain>
                      (globalDomain,localDomain,representation->
		      getKELP_Representation(),GlobalDescriptor);
	       break;
#endif

          case SublibraryNames::PGS:
	       cout << "Error in:"
	            << " PADRE_Descriptor( const PADRE_Representation<*,*,*> & "
		    << "inputRepresentation ) switch case PGS"
	            << endl;
	       PADRE_ABORT();
	       break;

          case SublibraryNames::DomainLayout:
	       cout << "Error in:"
	            << " PADRE_Descriptor( const PADRE_Representation<*,*,*> & "
		    << "inputRepresentation ) switch case DomainLayout"
	            << endl;
	       PADRE_ABORT();
	       break;

          default:
	       cout << "Error in:"
	            << " PADRE_Descriptor( const PADRE_Representation<*,*,*> & "
		    << "inputRepresentation ) switch default"
	            << endl;
	       PADRE_ABORT();
        }
       }
     }

  // cout << "exiting:"
  //      << " PADRE_Descriptor::intializeConstructor"
  //      << "(UserLocalDomain*,UserLocalDomain*,PADRE_Representation<*,*,*>*,"
  //      << "UserGlobalDomain*)" 
  //      << endl;

#if !defined(NO_Parti)
     if ( pPARTI_Descriptor )
       PADRE_ASSERT (globalDomain == pPARTI_Descriptor->getGlobalDomainPointer());
#endif

#if !defined(NO_GlobalArrays)
     if ( pGlobalArrays_Descriptor )
       PADRE_ASSERT (globalDomain == pGlobalArrays_Descriptor->getGlobalDomainPointer());
#endif

#if defined (USE_KELP)
     if ( pKELP_Descriptor )
       PADRE_ASSERT (globalDomain == pKELP_Descriptor->getGlobalDomainPointer());
#endif

     testConsistency("PADRE_Descriptor::intializeConstructor(UserLocalDomain*,UserLocalDomain*,PADRE_Representation<*,*,*>*,UserGlobalDomain*)");
   }


// ********* Member function for initialization of constructors 
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
intializeConstructor ( 
     const PADRE_Descriptor *inputPADRE_Descriptor,
     const UserLocalDomain *inputGlobalDomain,
     const UserLocalDomain *inputLocalDomain,
     const Repre *inputRepresentation,
     const UserGlobalDomain *inputGlobalDescriptor )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "entering:" 
               << " PADRE_Descriptor::intializeConstructor"
	       << "(UserLocalDomain*,UserLocalDomain*,PADRE_Representation<*,*,*>*,"
	       << "UserGlobalDomain*)" 
               << endl;
#endif

  // Call the base class initialize member function (this initializes the id 
  // number and reference count)
  // This is called by the PADRE_CommonInterface default constructor.
  // initialize();

     PADRE_ASSERT (inputRepresentation != NULL);
  // if (inputRepresentation != NULL)
     inputRepresentation->incrementReferenceCount();
  // Cast away const here to avoid compiler warning
     representation = (Repre *) inputRepresentation;
     PADRE_ASSERT(representation != NULL);

     if (inputGlobalDescriptor != NULL)
          inputGlobalDescriptor->incrementReferenceCount();
     GlobalDescriptor = (UserGlobalDomain*) inputGlobalDescriptor;
     PADRE_ASSERT(GlobalDescriptor != NULL);

  // We don't want to get the globalDomain from the inputRepresentation because then
  // it would have a translation invariant base (i.e. zero base).
  // The alternative is to use the globalDomain from the PADRE_Representation
  // and keep track of the translation of the base.
  // globalDomain = inputRepresentation->getGlobalDomainPointer();
     if (inputGlobalDomain != NULL)
          inputGlobalDomain->incrementReferenceCount();
     globalDomain = (UserLocalDomain*) inputGlobalDomain;

#if 1
  // These should be the same!
     PADRE_ASSERT (globalDomain == inputGlobalDomain);
     PADRE_ASSERT (globalDomain == representation->getGlobalDomainPointer());
     //PADRE_ASSERT (globalDomain == representation->getPARTI_Representation()->
     //		   getGlobalDomainPointer());
#endif

  // globalDomain = (UserLocalDomain*) inputGlobalDomain;
    
  // During initial testing we will assume that global base is zero
  // that is a simplification.
  // Store the bases for each dimension
  // for (i=0; i < PARTI_MAX_ARRAY_DIMENSION; i++)
  //      TranslationBase [i] = 0;

     if (inputLocalDomain != NULL)
          inputLocalDomain->incrementReferenceCount();
     localDomain = (UserLocalDomain*) inputLocalDomain;
  // This should fail!!!
  // PADRE_ASSERT(localDomain != NULL);
    

  // Initialize so garbage values in pointers won't cause problems.

#if !defined(NO_Parti)
     pPARTI_Descriptor = NULL;
#endif
#if !defined(NO_GlobalArrays)
     pGlobalArrays_Descriptor  = NULL;
#endif
#if defined (USE_KELP)
     pKELP_Descriptor  = NULL;
#endif

  // allocate the corresponding distributionLibrary object

     {
       const SublibraryNames::Container &sublibs=representation->getSublibraryInUse();
       SublibraryNames::Container::const_iterator sublib;
       for ( sublib=sublibs.begin(); sublib!=sublibs.end(); sublib++ ) {
	 switch (*sublib) { 
          case SublibraryNames::NONE:
	       cout << "Error in:"
	            << " PADRE_Descriptor( const PADRE_Representation<*,*,*> & "
		    << "inputRepresentation ) switch case LibNONE"
	            << endl;
	       PADRE_ABORT();
	       break;

          case SublibraryNames::Trivial:
	       cout << "Error in:"
	            << " PADRE_Descriptor( const PADRE_Representation<*,*,*> & "
		    << "inputRepresentation ) switch case LibTrivial"
	            << endl;
	       PADRE_ABORT();
	       break;

#if !defined(NO_Parti)
          case SublibraryNames::PARTI: 
	       pPARTI_Descriptor = 
	         new PARTI_Descriptor<UserCollection,UserGlobalDomain,UserLocalDomain>
                      (globalDomain,localDomain,representation->
		      getPARTI_Representation(),GlobalDescriptor);
	       break;
#endif

#if !defined(NO_GlobalArrays)
          case SublibraryNames::GlobalArrays: 
	       pGlobalArrays_Descriptor = 
	         new GlobalArrays_Descriptor<UserCollection,UserGlobalDomain,UserLocalDomain>
                      (inputPADRE_Descriptor,globalDomain,localDomain,representation->
		      getGlobalArrays_Representation(),GlobalDescriptor);
	       break;
#endif

#if defined (USE_KELP)
          case SublibraryNames::KELP: 
	       pKELP_Descriptor = 
	         new KELP_Descriptor<UserCollection,UserGlobalDomain,UserLocalDomain>
                      (globalDomain,localDomain,representation->
		      getKELP_Representation(),GlobalDescriptor);
	       break;
#endif

          case SublibraryNames::PGS:
	       cout << "Error in:"
	            << " PADRE_Descriptor( const PADRE_Representation<*,*,*> & "
		    << "inputRepresentation ) switch case PGS"
	            << endl;
	       PADRE_ABORT();
	       break;

          case SublibraryNames::DomainLayout:
	       cout << "Error in:"
	            << " PADRE_Descriptor( const PADRE_Representation<*,*,*> & "
		    << "inputRepresentation ) switch case DomainLayout"
	            << endl;
	       PADRE_ABORT();
	       break;

          default:
	       cout << "Error in:"
	            << " PADRE_Descriptor( const PADRE_Representation<*,*,*> & "
		    << "inputRepresentation ) switch default"
	            << endl;
	       PADRE_ABORT();
        }
       }
     }

  // cout << "exiting:"
  //      << " PADRE_Descriptor::intializeConstructor"
  //      << "(UserLocalDomain*,UserLocalDomain*,PADRE_Representation<*,*,*>*,"
  //      << "UserGlobalDomain*)" 
  //      << endl;

#if !defined(NO_Parti)
     if ( pPARTI_Descriptor )
       PADRE_ASSERT (globalDomain == pPARTI_Descriptor->getGlobalDomainPointer());
#endif

#if !defined(NO_GlobalArrays)
     if ( pGlobalArrays_Descriptor )
       PADRE_ASSERT (globalDomain == pGlobalArrays_Descriptor->getGlobalDomainPointer());
#endif

#if defined (USE_KELP)
     if ( pKELP_Descriptor )
       PADRE_ASSERT (globalDomain == pKELP_Descriptor->getGlobalDomainPointer());
#endif

     testConsistency("PADRE_Descriptor::intializeConstructor(UserLocalDomain*,UserLocalDomain*,PADRE_Representation<*,*,*>*,UserGlobalDomain*)");
   }


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
InitializeLocalDescriptor ()
  // ( UserLocalDomain & inputGlobalDomain,
  //   const PADRE_Representation<UserCollection, UserGlobalDomain, UserLocalDomain> 
  //   & inputRepresentation,
  //   UserGlobalDomain & inputGlobalDescriptor )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "entering:"
               << " InitializeLocalDescriptor ( const UserLocalDomain &, "
	       << "const PADRE_Representation<*,*,*> &, const UserGlobalDomain & )"
               << endl;
#endif

 
  // Build a local descriptor
  // PADRE_ASSERT (localDescriptor != NULL);
  // localDescriptor = new UserGlobalDomain();

  // allocate the corresponding distributionLibrary object

     {
       const SublibraryNames::Container &sublibs=representation->getSublibraryInUse();
       SublibraryNames::Container::const_iterator sublib;
       for ( sublib=sublibs.begin(); sublib!=sublibs.end(); sublib++ ) {
	 switch (*sublib) { 
          case SublibraryNames::NONE:
	       cout << "Error in:"
	            << " InitializeLocalDescriptor( ... )"
	            << " switch case NONE"
	            << endl;
	       PADRE_ABORT();
	       break;

          case SublibraryNames::Trivial:
	       cout << "Error in:"
	            << " InitializeLocalDescriptor( ... )"
	            << " switch case Trivial"
	            << endl;
	       PADRE_ABORT();
	       break;

#if !defined(NO_Parti)
          case SublibraryNames::PARTI: 
	       PADRE_ASSERT (pPARTI_Descriptor != NULL);
            // pPARTI_Descriptor->InitializeLocalDescriptor
	    //   (inputGlobalDomain,inputRepresentation,inputGlobalDescriptor);
               pPARTI_Descriptor->InitializeLocalDescriptor();
	       break;
#endif

#if !defined(NO_GlobalArrays)
          case SublibraryNames::GlobalArrays: 
	       PADRE_ASSERT (pGlobalArrays_Descriptor != NULL);
            // pGlobalArrays_Descriptor->InitializeLocalDescriptor
	    //   (inputGlobalDomain,inputRepresentation,inputGlobalDescriptor);
               pGlobalArrays_Descriptor->InitializeLocalDescriptor();
	       break;
#endif

#if defined (USE_KELP)
          case SublibraryNames::KELP: 
	       PADRE_ASSERT (pKELP_Descriptor != NULL);
            // pKELP_Descriptor->InitializeLocalDescriptor
	    //   (inputGlobalDomain,inputRepresentation,inputGlobalDescriptor);
               pKELP_Descriptor->InitializeLocalDescriptor();
	       break;
#endif

          case SublibraryNames::PGS:
	       cout << "Error in:"
	            << " InitializeLocalDescriptor( ... )"
	            << " switch case PGS"
	            << endl;
	       PADRE_ABORT();
	       break;

          case SublibraryNames::DomainLayout:
	       cout << "Error in:"
	            << " InitializeLocalDescriptor( ... )"
	            << " switch case DomainLayout"
	            << endl;
	       PADRE_ABORT();
	       break;

          default:
	       cout << "Error in:"
	            << " InitializeLocalDescriptor( ... )"
	            << " switch default"
	            << endl;
	       PADRE_ABORT();
	 }
       }
     }

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("At base of PADRE_Descriptor::InitializeLocalDescriptor() \n");
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
InitializeLocalDescriptor 
   ( UserGlobalDomain & inputGlobalDescriptor,
     UserLocalDomain          & inputLocalDomain)
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "entering: "
               << "InitializeLocalDescriptor ( UserGlobalDomain&, UserLocalDomain& )"
               << endl;
#endif


  // Build a local descriptor
  // PADRE_ASSERT (localDescriptor != NULL);
  // localDescriptor = new UserGlobalDomain();

  // allocate the corresponding distributionLibrary object

     {
       const SublibraryNames::Container &sublibs=representation->getSublibraryInUse();
       SublibraryNames::Container::const_iterator sublib;
       for ( sublib=sublibs.begin(); sublib!=sublibs.end(); sublib++ ) {
	 switch (*sublib) { 
          case SublibraryNames::NONE:
               cout << "Error in:"
                    << " InitializeLocalDescriptor( ... )"
                    << " switch case NONE"
                    << endl;
               PADRE_ABORT();
               break;

          case SublibraryNames::Trivial:
               cout << "Error in:"
                    << " InitializeLocalDescriptor( ... )"
                    << " switch case Trivial"
                    << endl;
               PADRE_ABORT();
               break;

#if !defined(NO_Parti)
          case SublibraryNames::PARTI:
               PADRE_ASSERT (pPARTI_Descriptor != NULL);
               pPARTI_Descriptor->InitializeLocalDescriptor
		 (inputGlobalDescriptor,inputLocalDomain);
               break;
#endif

#if !defined(NO_GlobalArrays)
          case SublibraryNames::GlobalArrays:
               PADRE_ASSERT (pGlobalArrays_Descriptor != NULL);
               pGlobalArrays_Descriptor->InitializeLocalDescriptor (inputGlobalDescriptor,inputLocalDomain);
               break;
#endif

#if defined (USE_KELP)
          case SublibraryNames::KELP:
               PADRE_ASSERT (pKELP_Descriptor != NULL);
               pKELP_Descriptor->InitializeLocalDescriptor
		 (inputGlobalDescriptor,inputLocalDomain);
               break;
#endif

          case SublibraryNames::PGS:
               cout << "Error in:"
                    << " InitializeLocalDescriptor( ... )"
                    << " switch case PGS"
                    << endl;
               PADRE_ABORT();
               break;

          case SublibraryNames::DomainLayout:
               cout << "Error in:"
                    << " InitializeLocalDescriptor( ... )"
                    << " switch case DomainLayout"
                    << endl;
               PADRE_ABORT();
               break;

          default:
               cout << "Error in:"
                    << " InitializeLocalDescriptor( ... )"
                    << " switch default"
                    << endl;
               PADRE_ABORT();
        }

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          printf ("At base of PADRE_Descriptor::InitializeLocalDescriptor(...) \n");
#endif
       }
     }
   }

//==========================================================================

// ... KELP must allocate its own data ...

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
allocateData (int& preallocateData, double** local_data_ptr) {
#if !defined(NO_Parti)
  if (pPARTI_Descriptor != NULL) {
    // Parti does not allocate its own memory for data.
    preallocateData = false;
    *local_data_ptr = NULL;
  }
#endif
#if !defined(NO_GlobalArrays)
  if (pGlobalArrays_Descriptor != NULL) {
    preallocateData = true;
    pGlobalArrays_Descriptor->allocateData(local_data_ptr);
  }
#endif
#if defined (USE_KELP)
  if (pKELP_Descriptor != NULL) {
    preallocateData = true;
    pKELP_Descriptor->allocateData(local_data_ptr);
  }
#endif
}

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
allocateData (int& preallocateData, float** local_data_ptr) {
#if !defined(NO_Parti)
  if (pPARTI_Descriptor != NULL) {
    preallocateData = false;
    *local_data_ptr = NULL;
  }
#endif
#if !defined(NO_GlobalArrays)
  if (pGlobalArrays_Descriptor != NULL) {
    preallocateData = true;
    pGlobalArrays_Descriptor->allocateData(local_data_ptr);
  }
#endif
#if defined (USE_KELP)
  if (pKELP_Descriptor != NULL) {
    preallocateData = true;
    pKELP_Descriptor->allocateData(local_data_ptr);
  }
#endif
}

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
allocateData (int& preallocateData, int** local_data_ptr) {
#if !defined(NO_Parti)
  if (pPARTI_Descriptor != NULL) {
    preallocateData = false;
    *local_data_ptr = NULL;
  }
#endif
#if !defined(NO_GlobalArrays)
  if (pGlobalArrays_Descriptor != NULL) {
    preallocateData = true;
    pGlobalArrays_Descriptor->allocateData(local_data_ptr);
  }
#endif
#if defined (USE_KELP)
  if (pKELP_Descriptor != NULL) {
    preallocateData = true;
    pKELP_Descriptor->allocateData(local_data_ptr);
  }
#endif
}

//==========================================================================

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
setGhostCellWidth( unsigned axis, unsigned width )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
            << " PADRE_Descriptor ::"
            << " setGhostCellWidth("
            << "unsigned axis = " << axis << ",unsigned width = " << width << ")"
            << " not implemented\n";
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
getGhostCellWidth( unsigned axis ) const
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "int"
               << " PADRE_Descriptor ::"
               << " int getGhostCellWidth("
               << "unsigned axis = " << axis << ")"
               << " not implemented, returning 0\n";
#endif
     return 0; 
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
getNumberOfAxesToDistribute() const
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "int"
               << " PADRE_Descriptor ::"
               << " getNumberOfAxesToDistribute()"
               << " not implemented, returning 0\n";
#endif
     return 0; 
   }

#if 0
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
setDistributeAxis( int Axis )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Descriptor ::"
               << " setDistributeAxis("
               << "int Axis = " << Axis << ")"
               << " not implemented\n";
#endif
   }
#else
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
DistributeAlongAxis ( int Axis, bool Partition_Axis, int GhostBoundaryWidth )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Descriptor ::"
               << " DistributeAlongAxis("
               << "int Axis = " << Axis << ")"
               << " not implemented\n";
#endif
     
  // We have to build a new Representation (and maybe a distribution) inorder to
  // change the distribution using this function.  This implementation is not 
  // complete!
  // representation->DistributeAlongAxis(Axis,Partition_Axis,GhostBoundaryWidth);
     PADRE_ABORT();
   }
#endif

#if 0
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
getProcessorSet( int *ProcessorArray, int & Number_Of_Processors ) const
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Descriptor ::"
               << " getProcessorSet(int *ProcessorArray, int & Number_Of_Processors)"
               << " not implemented\n";
#endif
   }
#endif

#if 1
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
swapDistribution ( const Distr & oldDistribution,
    const Distr & newDistribution ) {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Descriptor ::"
               << " swapDistribution("
               << " const PADRE_Distribution & oldDistribution,"
               << " const PADRE_Distribution & newDistribution ) \n";
#endif

#if 0
     printf ("########################################################## \n");
     printf ("##########  PADRE_Descriptor::swapDistribution  ########## \n");
     printf ("########################################################## \n");
#endif

#if !defined(NO_Parti)
     PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
	swapDistribution ( oldDistribution, newDistribution );
#endif

#if !defined(NO_GlobalArrays)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::GlobalArrays ) ) {
     GlobalArrays_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
	swapDistribution ( oldDistribution, newDistribution );
       }
#endif

#if defined (USE_KELP)
     KELP_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
	swapDistribution ( oldDistribution, newDistribution );
#endif

  // printf ("Exiting AFTER PARTI_Descriptor::swapDistribution () \n");
  // PADRE_ABORT();
   }
#endif

#if 1
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
AssociateObjectWithDistribution( const UserCollection & X )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

  // This function puts the array object (for example) into a list at the 
  // Distribution level of the heirarchy. The purpose is to permit modifications 
  // of the distribution object to propogate their effect to the array objects 
  // that were previously constucted with the previously unmodified distribution 
  // object.

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Descriptor ::"
               << " AssociateObjectWithDistribution( const UserCollection & X )"
               << " not implemented\n";
#endif

     PADRE_ASSERT (representation != NULL);
     PADRE_ASSERT (representation->Distribution != NULL);
#if 0
     PADRE_ASSERT (representation->Distribution->getUserCollectionList() != NULL);

  // Before we add this to this list we want to make sure 
  // that it does not already exist in the list.  We have to check the Array_ID's
  // because there could be multiple references to a specific array and it must have 
  // only a single representation by a Distribution object.
     for (list<UserCollection*>::iterator i = 
          representation->Distribution->getUserCollectionList()->begin();
          i != representation->Distribution->getUserCollectionList()->end(); i++)
        {
          PADRE_ASSERT ( (*i)->Array_ID() != X.Array_ID() );
        }

     representation->Distribution->getUserCollectionList()->push_back(&(UserCollection &) X);
#else
     representation->Distribution->AssociateObjectWithDistribution(X);
#endif
   }
#endif

#if 1
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
AssociateObjectWithDefaultDistribution( const UserCollection & X )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

  // This function puts the array object (for example) into a list at the 
  // Distribution level of the heirarchy. The purpose is to permit modifications 
  // of the distribution object to propogate their effect to the array objects 
  // that were previously constucted with the previously unmodified distribution 
  // object.

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Descriptor ::"
               << " AssociateObjectWithDistribution( const UserCollection & X )"
               << " not implemented\n";
#endif

     printf ("Default Distribution case not implemented! \n");
     PADRE_ABORT();

     PADRE_ASSERT (representation != NULL);
     PADRE_ASSERT (representation->Distribution != NULL);
#if 0
     PADRE_ASSERT (representation->Distribution->getUserCollectionList() != NULL);

  // Before we add this to this list we want to make sure 
  // that it does not already exist in the list.  We have to check the Array_ID's
  // because there could be multiple references to a specific array and it must have 
  // only a single representation by a Distribution object.
     for (list<UserCollection*>::iterator i = 
          representation->Distribution->getUserCollectionList()->begin();
          i != representation->Distribution->getUserCollectionList()->end(); i++)
        {
          PADRE_ASSERT ( (*i)->Array_ID() != X.Array_ID() );
        }

     representation->Distribution->getUserCollectionList()->push_back(&(UserCollection &) X);
#else
     representation->Distribution->AssociateObjectWithDefaultDistribution(X);
#endif
   }
#endif

#if 1
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
UnassociateObjectWithDistribution( const UserCollection & X )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Descriptor ::"
               << " UnassociateObjectWithDistribution( const UserCollection & X )"
               << " not implemented\n";
#endif

     PADRE_ASSERT (representation != NULL);
     PADRE_ASSERT (representation->Distribution != NULL);
#if 0
     PADRE_ASSERT (representation->Distribution->getUserCollectionList() != NULL);
     PADRE_ASSERT (representation->Distribution->getUserCollectionList()->size() > 0);
     int SizeBeforeRemove = representation->Distribution->getUserCollectionList()->size();
     representation->Distribution->getUserCollectionList()->remove(&(UserCollection &) X);
     int SizeAfterRemove  = representation->Distribution->getUserCollectionList()->size();

  // Make sure that something (only one thing) really was removed!
     PADRE_ASSERT (SizeBeforeRemove - SizeAfterRemove == 1);
#else
     representation->Distribution->UnassociateObjectWithDistribution(X);
#endif
   }
#endif

#if 1
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
UnassociateObjectWithDefaultDistribution( const UserCollection & X )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Descriptor ::"
               << " UnassociateObjectWithDistribution( const UserCollection & X )"
               << " not implemented\n";
#endif

     printf ("Default Distribution case not implemented! \n");
     PADRE_ABORT();

     PADRE_ASSERT (representation != NULL);
     PADRE_ASSERT (representation->Distribution != NULL);
#if 0
     PADRE_ASSERT (representation->Distribution->getUserCollectionList() != NULL);
     PADRE_ASSERT (representation->Distribution->getUserCollectionList()->size() > 0);
     int SizeBeforeRemove = representation->Distribution->getUserCollectionList()->
	size();
     representation->Distribution->getUserCollectionList()->
	remove(&(UserCollection &) X);
     int SizeAfterRemove  = representation->Distribution->getUserCollectionList()->
	size();

  // Make sure that something (only one thing) really was removed!
     PADRE_ASSERT (SizeBeforeRemove - SizeAfterRemove == 1);
#else
     representation->Distribution->UnassociateObjectWithDefaultDistribution(X);
#endif
   }
#endif

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
getLocalSizes ( int* LocalSizeArray )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Descriptor ::"
               << " getLocalSizes ( int* LocalSizeArray ) \n";
#endif

     PADRE_ASSERT (representation != NULL);
     representation->getLocalSizes(LocalSizeArray);
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
testConsistency( const char *Label ) const {
  /*
    This function makes some checks at this level then calls on each of the
    PADRE descriptors for the sublibraries to run their testConsistency functions.
  */
#if defined(USE_TAU)
  TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
	      TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
  if (PADRE::debugLevel() > 0)
    printf ("Inside of PADRE_Descriptor::testConsistency(%s) \n",Label);
#endif

  // PADRE_ASSERT (localDomain       != NULL);
  PADRE_ASSERT (GlobalDescriptor   != NULL);
  PADRE_ASSERT (representation    != NULL);

  PADRE_ASSERT (globalDomain      != NULL);
  PADRE_ASSERT (globalDomain      == representation->getGlobalDomainPointer());

  int num_dimensions = globalDomain->numberOfDimensions();

#if !defined(NO_Parti)
  /*
    Consistency checks for Parti.
  */
  if (representation->getPARTI_Representation() != NULL)
    PADRE_ASSERT (globalDomain == representation->getPARTI_Representation()->
		  getGlobalDomainPointer());
  
  PADRE_ASSERT( pPARTI_Descriptor != NULL );
  PADRE_ASSERT( pPARTI_Descriptor->getGlobalDomainPointer() == globalDomain );
  PADRE_ASSERT( pPARTI_Descriptor->getBlockPartiArrayDescriptor()->nDims == num_dimensions );
  pPARTI_Descriptor->testConsistency( "Called from PADRE_Descriptor::testConsistency.\n" );
#endif

#if !defined(NO_GlobalArrays)
  /*
    Consistency checks for GlobalArrays.
  */
  if (representation->getGlobalArrays_Representation() != NULL)
    PADRE_ASSERT (globalDomain == representation->getGlobalArrays_Representation()->getGlobalDomainPointer());
  
  PADRE_ASSERT (pGlobalArrays_Descriptor != NULL );
  if (pGlobalArrays_Descriptor != NULL)
    PADRE_ASSERT (globalDomain == pGlobalArrays_Descriptor->getGlobalDomainPointer());
  pGlobalArrays_Descriptor->testConsistency( "Called from PADRE_Descriptor::testConsistency.\n" );
#endif

#if defined (USE_KELP)
  /*
    Consistency checks for KELP.
  */
  if (representation->getKELP_Representation() != NULL)
    PADRE_ASSERT (globalDomain == representation->getKELP_Representation()->getGlobalDomainPointer());
  
  PADRE_ASSERT (pKELP_Descriptor != NULL );
  if (pKELP_Descriptor != NULL )
    PADRE_ASSERT (globalDomain == pKELP_Descriptor->getGlobalDomainPointer());
  pKELP_Descriptor->testConsistency( "Called from PADRE_Descriptor::testConsistency.\n" );
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
  if (PADRE::debugLevel() > 0)
    printf ("End of PADRE_Descriptor::testConsistency() \n");
#endif
}

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
updateGhostBoundaries()
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

#if defined(PADRE_DEBUG_IS_ENABLED)
     if (PADRE::debugLevel() > 0)
          cout << "void"
               << " PADRE_Descriptor ::"
               << " updateGhostBoundaries()"
               << " not implemented\n";
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
displayDefaultValues( const char *Label )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

     printf ("Inside of PADRE_Descriptor::displayDefaultValues (%s) \n",Label);
     PADRE_CommonInterface::displayDefaultValues(Label);
     printf ("PADRE_Global_Array_Base = %d \n",PADRE_Global_Array_Base);
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
display( const char *Label ) const
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::PADRE_Descriptor()", "void(void)", 
		 TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

     printf ("Inside of PADRE_Descriptor::display (%s) \n",Label);
     PADRE_CommonInterface::display(Label);

     if (globalDomain != NULL)
        {
          PADRE_ASSERT (globalDomain != NULL);
          printf ("In PADRE_Descriptor::display(%s): globalDomain->display() commented out! \n",Label);
       // globalDomain->display(Label);
        }
       else
        {
          printf ("In PADRE_Descriptor::display(%s) globalDomain == NULL pointer \n",Label);
        }

     if (localDomain != NULL)
        {
          PADRE_ASSERT (localDomain != NULL);
          printf ("In PADRE_Descriptor::display(%s): localDomain->display() commented out! \n",Label);
       // localDomain->display(Label);
        }
       else
        {
          printf ("In PADRE_Descriptor::display(%s) localDomain == NULL pointer \n",Label);
        }

     if (GlobalDescriptor != NULL)
        {
          PADRE_ASSERT (GlobalDescriptor != NULL);
       // We have to avoid a recursive call here!
       // GlobalDescriptor->display(Label);
        }
       else
        {
          printf ("In PADRE_Descriptor::display(%s) GlobalDescriptor == NULL pointer \n",Label);
        }

     if (representation != NULL)
        {
          PADRE_ASSERT (representation != NULL);
          representation->display(Label);
        }
       else
        {
          printf ("In PADRE_Descriptor::display(%s) representation == NULL pointer \n",Label);
        }

#if !defined(NO_Parti)
     if (pPARTI_Descriptor != NULL) pPARTI_Descriptor->display("");
#endif

#if !defined(NO_GlobalArrays)
     if (pGlobalArrays_Descriptor != NULL) pGlobalArrays_Descriptor->display("");
#endif

#if defined (USE_KELP)
     if (pKelp_Descriptor != NULL) pKelp_Descriptor->display("");
#endif

     printf ("At BASE of PADRE_Descriptor::display(); \n");
   }


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
transferData ( const UserGlobalDomain & receiveDomain,
               const UserGlobalDomain & sendDomain,
               const double *sourceDataPointer,
               const double *destinationDataPointer )

   {
#if defined(USE_TAU)
     TAU_PROFILE ("PADRE_Descriptor::transferData(UserGlobalDomain&,UserGlobalDomain&,double*)", 
                 "void(void)", TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

  // This function is a shortcut to the communication mechanism in PADRE.
  // It assues a schedule will be computed and used and doe not permit 
  // the communication schedule to be extracted separately.
  // Other functionality (some of which will be provided later)
  // allows the construction of communication schedules and there
  // separate execution (under control of the runtime system for example).

  // We need a switch statement here to permit the selection of the 
  // correct distribution library for now we can just to this.  
  // The mechanism for the selection of the correct library could 
  // likely be improved (I hope) to avoid the use of a switch statement.
#if !defined(NO_Parti)
     PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
       transferData 
        ( receiveDomain, sendDomain, sourceDataPointer, destinationDataPointer );
#endif

#if !defined(NO_GlobalArrays)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::GlobalArrays ) ) {
     GlobalArrays_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::transferData 
          ( receiveDomain, sendDomain, sourceDataPointer, destinationDataPointer );
	//( *sendDomain.parallelPADRE_DescriptorPointer->getKelpDescriptor(),
	//  *receiveDomain.parallelPADRE_DescriptorPointer->getKelpDescriptor());
       }
#endif

#if defined (USE_KELP)
     KELP_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::transferData
        ( receiveDomain, sendDomain, sourceDataPointer, destinationDataPointer );
	//( *sendDomain.parallelPADRE_DescriptorPointer->getKelpDescriptor(),
	//  *receiveDomain.parallelPADRE_DescriptorPointer->getKelpDescriptor());
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
transferData ( const UserGlobalDomain & receiveDomain,
               const UserGlobalDomain & sendDomain,
               const float *sourceDataPointer,
               const float *destinationDataPointer )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::transferData(UserGlobalDomain&,UserGlobalDomain&,float*)",
                 "void(void)", TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif

  // See function description in similar function above.
#if !defined(NO_Parti)
     PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::transferData 
         ( receiveDomain, sendDomain, sourceDataPointer, destinationDataPointer);
#endif

#if !defined(NO_GlobalArrays)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::GlobalArrays ) ) {
     GlobalArrays_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::transferData 
          ( receiveDomain, sendDomain, sourceDataPointer, destinationDataPointer);
	//( *sendDomain.parallelPADRE_DescriptorPointer->getKelpDescriptor(),
	//  *receiveDomain.parallelPADRE_DescriptorPointer->getKelpDescriptor());
       }
#endif

#if defined (USE_KELP)
     KELP_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
       transferData 
         ( receiveDomain, sendDomain, sourceDataPointer, destinationDataPointer);
	//( *sendDomain.parallelPADRE_DescriptorPointer->getKelpDescriptor(),
	//  *receiveDomain.parallelPADRE_DescriptorPointer->getKelpDescriptor());
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
transferData ( const UserGlobalDomain & receiveDomain,
               const UserGlobalDomain & sendDomain,
               const int *sourceDataPointer,
               const int *destinationDataPointer )
   {
#if defined(USE_TAU)
     TAU_PROFILE("PADRE_Descriptor::transferData(UserGlobalDomain&,UserGlobalDomain&,int*)", 
                 "void(void)", TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif
  // See function description in similar function above.
#if !defined(NO_Parti)
       PARTI_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::transferData 
          ( receiveDomain, sendDomain, sourceDataPointer, destinationDataPointer );
#endif

#if !defined(NO_GlobalArrays)
     if ( PADRE::isSublibraryPermitted( SublibraryNames::GlobalArrays ) ) {
	 GlobalArrays_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::transferData 
	   ( receiveDomain, sendDomain, sourceDataPointer, destinationDataPointer );
	 //( *sendDomain.parallelPADRE_DescriptorPointer->getKelpDescriptor(),
	 //  *receiveDomain.parallelPADRE_DescriptorPointer->getKelpDescriptor());
       }
#endif

#if defined (USE_KELP)
     KELP_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::transferData 
          ( receiveDomain, sendDomain, sourceDataPointer, destinationDataPointer );
	//( *sendDomain.parallelPADRE_DescriptorPointer->getKelpDescriptor(),
	//  *receiveDomain.parallelPADRE_DescriptorPointer->getKelpDescriptor());
#endif
   }

#if 0
template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
transferData 
  ( const KELP_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>& 
       sendDescriptor,
    KELP_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>& 
       receiveDescriptor)
   {
#if defined(USE_TAU)
     TAU_PROFILE
	("PADRE_Descriptor::transferData(KELP_Descriptor,KELP_Descriptor)", 
                 "void(void)", TAU_PADRE_CONSTRUCTOR_OVERHEAD_MANAGEMENT);
#endif
  // See function description in similar function above.
     KELP_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::transferData 
        ( sendDescriptor,receiveDescriptor);
   }
#endif

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
updateGhostBoundaries ( double *dataPointer )
   {
#if !defined(NO_Parti)
     PADRE_ASSERT (pPARTI_Descriptor != NULL );
     if (pPARTI_Descriptor != NULL)
        pPARTI_Descriptor->updateGhostBoundaries(dataPointer);
#endif

#if !defined(NO_GlobalArrays)
     PADRE_ASSERT (pGlobalArrays_Descriptor != NULL);
     if (pGlobalArrays_Descriptor != NULL)
        pGlobalArrays_Descriptor->updateGhostBoundaries(dataPointer);
#endif

#if defined (USE_KELP)
     PADRE_ASSERT (pKELP_Descriptor != NULL);
     if (pKELP_Descriptor != NULL)
        pKELP_Descriptor->updateGhostBoundaries(dataPointer);
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
updateGhostBoundaries ( float *dataPointer )
   {
#if !defined(NO_Parti)
     PADRE_ASSERT (pPARTI_Descriptor != NULL );
     if (pPARTI_Descriptor != NULL)
        pPARTI_Descriptor->updateGhostBoundaries(dataPointer);
#endif

#if !defined(NO_GlobalArrays)
     PADRE_ASSERT (pGlobalArrays_Descriptor != NULL);
     if (pGlobalArrays_Descriptor != NULL)
        pGlobalArrays_Descriptor->updateGhostBoundaries(dataPointer);
#endif

#if defined (USE_KELP)
     PADRE_ASSERT (pKELP_Descriptor != NULL);
     if (pKELP_Descriptor != NULL)
        pKELP_Descriptor->updateGhostBoundaries(dataPointer);
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
updateGhostBoundaries ( int *dataPointer )
   {
#if !defined(NO_Parti)
     PADRE_ASSERT (pPARTI_Descriptor != NULL );
     if (pPARTI_Descriptor != NULL)
        pPARTI_Descriptor->updateGhostBoundaries(dataPointer);
#endif

#if !defined(NO_GlobalArrays)
     PADRE_ASSERT (pGlobalArrays_Descriptor != NULL);
     if (pGlobalArrays_Descriptor != NULL)
        pGlobalArrays_Descriptor->updateGhostBoundaries(dataPointer);
#endif

#if defined (USE_KELP)
     PADRE_ASSERT (pKELP_Descriptor != NULL);
     if (pKELP_Descriptor != NULL)
        pKELP_Descriptor->updateGhostBoundaries(dataPointer);
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
setLocalDomain( UserLocalDomain *inputLocalDomain )
     {
    // We want to accept NULL as a pointer value since we will use this as
    // a way to free the memory associated with PADRE objects.
    // This is a mechanism used in P++ to reduce the reference count of
    // P++ objects used as template parameters in PADRE (like the
    // SerialArray_Domain)

    // PADRE_ASSERT (inputLocalDomain != NULL);
       if (inputLocalDomain != NULL)
            inputLocalDomain->incrementReferenceCount();
       if (localDomain != NULL)
          {
            localDomain->decrementReferenceCount();
            if (localDomain->getReferenceCount() <
                localDomain->getReferenceCountBase())
                 delete localDomain;
            localDomain = NULL;
          }
       localDomain = inputLocalDomain;

    // We need a function in the PADRE_PARTI interface which will
    // abstract these details.  then we need to call all of the
    // distribution libraries and call this function!
    // Setup PARTI's descriptor's localDescriptor pointer
#if 0
    // Why is this here???
       printf ("NOTE: inputLocalDomain->incrementReferenceCount() called three time \n");
       if (inputLocalDomain != NULL)
            inputLocalDomain->incrementReferenceCount();
#endif

    // WHY IS THIS ONE HERE ALSO?
       if (inputLocalDomain != NULL)
            inputLocalDomain->incrementReferenceCount();

#if !defined(NO_Parti)
       PADRE_ASSERT (pPARTI_Descriptor != NULL );
#endif

#if !defined(NO_GlobalArrays)
       PADRE_ASSERT (pGlobalArrays_Descriptor != NULL);
#endif

#if defined (USE_KELP)
       PADRE_ASSERT (pKELP_Descriptor != NULL);
#endif

#if !defined(NO_Parti)
       if (pPARTI_Descriptor != NULL)
       {
          if (pPARTI_Descriptor->localDomain != NULL)
          {
            pPARTI_Descriptor->localDomain->decrementReferenceCount();
            if (pPARTI_Descriptor->localDomain->getReferenceCount() <
                pPARTI_Descriptor->localDomain->getReferenceCountBase())
                 delete pPARTI_Descriptor->localDomain;
            pPARTI_Descriptor->localDomain = NULL;
          }
          pPARTI_Descriptor->localDomain = inputLocalDomain;
       }
#endif

#if !defined(NO_GlobalArrays)
       if (pGlobalArrays_Descriptor != NULL)
       {
          if (pGlobalArrays_Descriptor->localDomain != NULL)
          {
            pGlobalArrays_Descriptor->localDomain->decrementReferenceCount();
            if (pGlobalArrays_Descriptor->localDomain->getReferenceCount() <
                pGlobalArrays_Descriptor->localDomain->getReferenceCountBase())
                 delete pGlobalArrays_Descriptor->localDomain;
            pGlobalArrays_Descriptor->localDomain = NULL;
          }
          pGlobalArrays_Descriptor->localDomain = inputLocalDomain;
       }
#endif

#if defined (USE_KELP)
       if (pKELP_Descriptor != NULL)
       {
          if (pKELP_Descriptor->localDomain != NULL)
          {
            pKELP_Descriptor->localDomain->decrementReferenceCount();
            if (pKELP_Descriptor->localDomain->getReferenceCount() <
                pKELP_Descriptor->localDomain->getReferenceCountBase())
                 delete pKELP_Descriptor->localDomain;
            pKELP_Descriptor->localDomain = NULL;
          }
          pKELP_Descriptor->localDomain = inputLocalDomain;
       }
#endif

#if 0
  // Double check that the PADRE_PARTI pointers are valid!
     PADRE_ASSERT (globalDomain    != NULL);
     PADRE_ASSERT (localDomain     != NULL);
     PADRE_ASSERT (GlobalDescriptor != NULL);
     PADRE_ASSERT (representation  != NULL);

  // Double check that the PADRE_PARTI pointers are valid!
     if (pPARTI_Descriptor != NULL);
        {
          PADRE_ASSERT (pPARTI_Descriptor->localDescriptor != NULL);
          PADRE_ASSERT (pPARTI_Descriptor->localDomain     != NULL);
          PADRE_ASSERT (pPARTI_Descriptor->globalDomain    != NULL);
          PADRE_ASSERT (pPARTI_Descriptor->representation  != NULL);
        }
#if !defined(NO_GlobalArrays)
       else if (pGlobalArrays_Descriptor != NULL);
        {
          PADRE_ASSERT (pGlobalArrays_Descriptor->localDescriptor != NULL);
          PADRE_ASSERT (pGlobalArrays_Descriptor->localDomain     != NULL);
          PADRE_ASSERT (pGlobalArrays_Descriptor->globalDomain    != NULL);
          PADRE_ASSERT (pGlobalArrays_Descriptor->representation  != NULL);
        }
#endif
#if defined (USE_KELP)
       else if (pKELP_Descriptor != NULL);
        {
          PADRE_ASSERT (pKELP_Descriptor->localDescriptor != NULL);
          PADRE_ASSERT (pKELP_Descriptor->localDomain     != NULL);
          PADRE_ASSERT (pKELP_Descriptor->globalDomain    != NULL);
          PADRE_ASSERT (pKELP_Descriptor->representation  != NULL);
        }
#endif
#endif
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
void
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
setLocalDescriptor( UserGlobalDomain *inputGlobalDescriptor )
     {
    // We want to accept NULL as a pointer value since we will use this as
    // a way to free the memory associated with PADRE objects.
    // This is a mechanism used in P++ to reduce the reference count of
    // P++ objects used as template parameters in PADRE (like the
    // SerialArray_Domain)

    // PADRE_ASSERT (inputGlobalDescriptor != NULL);
       if (inputGlobalDescriptor != NULL)
            inputGlobalDescriptor->incrementReferenceCount();
       if (GlobalDescriptor != NULL)
          {
            GlobalDescriptor->decrementReferenceCount();
            if (GlobalDescriptor->getReferenceCount() <
                GlobalDescriptor->getReferenceCountBase())
                 delete GlobalDescriptor;
            GlobalDescriptor = NULL;
          }
       GlobalDescriptor = inputGlobalDescriptor;

    // We need a function in the PADRE_PARTI interface which will
    // abstract these details.  then we need to call all of the
    // distribution libraries and call this function!
    // Setup PARTI's descriptor's localDescriptor pointer
       if (inputGlobalDescriptor != NULL)
            inputGlobalDescriptor->incrementReferenceCount();
#if !defined(NO_Parti)
       PADRE_ASSERT (pPARTI_Descriptor != NULL );
#endif
#if !defined(NO_GlobalArrays)
       PADRE_ASSERT (pPARTI_Descriptor != NULL || pGlobalArrays_Descriptor != NULL);
#endif
#if defined (USE_KELP)
       PADRE_ASSERT (pPARTI_Descriptor != NULL || pKELP_Descriptor != NULL);
#endif

       if (pPARTI_Descriptor != NULL)
       {
          if (pPARTI_Descriptor->localDescriptor != NULL)
          {
            pPARTI_Descriptor->localDescriptor->decrementReferenceCount();
            if (pPARTI_Descriptor->localDescriptor->getReferenceCount() <
                pPARTI_Descriptor->localDescriptor->getReferenceCountBase())
                 delete pPARTI_Descriptor->localDescriptor;
            pPARTI_Descriptor->localDescriptor = NULL;
          }
          pPARTI_Descriptor->localDescriptor = inputGlobalDescriptor;
       }
#if !defined(NO_GlobalArrays)
       else if (pGlobalArrays_Descriptor != NULL)
       {
          if (pGlobalArrays_Descriptor->localDescriptor != NULL)
          {
            pGlobalArrays_Descriptor->localDescriptor->decrementReferenceCount();
            if (pGlobalArrays_Descriptor->localDescriptor->getReferenceCount() <
                pGlobalArrays_Descriptor->localDescriptor->getReferenceCountBase())
                 delete pGlobalArrays_Descriptor->localDescriptor;
            pGlobalArrays_Descriptor->localDescriptor = NULL;
          }
       }
#endif
#if defined (USE_KELP)
       else if (pKELP_Descriptor != NULL)
       {
          if (pKELP_Descriptor->localDescriptor != NULL)
          {
            pKELP_Descriptor->localDescriptor->decrementReferenceCount();
            if (pKELP_Descriptor->localDescriptor->getReferenceCount() <
                pKELP_Descriptor->localDescriptor->getReferenceCountBase())
                 delete pKELP_Descriptor->localDescriptor;
            pKELP_Descriptor->localDescriptor = NULL;
          }
       }
#endif

#if 0
    // Double check that the PADRE_PARTI pointers are valid!
       PADRE_ASSERT (globalDomain    != NULL);
       PADRE_ASSERT (localDomain     != NULL);
       PADRE_ASSERT (GlobalDescriptor != NULL);
       PADRE_ASSERT (representation  != NULL);

    // Double check that the PADRE_PARTI pointers are valid!
       if (pPARTI_Descriptor != NULL)
       {
          PADRE_ASSERT (pPARTI_Descriptor->localDescriptor != NULL);
          PADRE_ASSERT (pPARTI_Descriptor->localDomain     != NULL);
          PADRE_ASSERT (pPARTI_Descriptor->globalDomain    != NULL);
          PADRE_ASSERT (pPARTI_Descriptor->representation  != NULL);
       }
#if !defined(NO_GlobalArrays)
       else if (pGlobalArrays_Descriptor != NULL)
       {
          PADRE_ASSERT (pGlobalArrays_Descriptor->localDescriptor != NULL);
          PADRE_ASSERT (pGlobalArrays_Descriptor->localDomain     != NULL);
          PADRE_ASSERT (pGlobalArrays_Descriptor->globalDomain    != NULL);
          PADRE_ASSERT (pGlobalArrays_Descriptor->representation  != NULL);
       }
#endif
#if defined (USE_KELP)
       else if (pKELP_Descriptor != NULL)
       {
          PADRE_ASSERT (pKELP_Descriptor->localDescriptor != NULL);
          PADRE_ASSERT (pKELP_Descriptor->localDomain     != NULL);
          PADRE_ASSERT (pKELP_Descriptor->globalDomain    != NULL);
          PADRE_ASSERT (pKELP_Descriptor->representation  != NULL);
       }
#endif
#endif
     }


template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
isLeftNullArray ( UserLocalDomain & serialArrayDomain , int axis ) const
   {

     int result = false;

#if !defined(NO_Parti)
     PADRE_ASSERT (pPARTI_Descriptor != NULL );
     if (pPARTI_Descriptor != NULL)
          result = pPARTI_Descriptor->isLeftNullArray(serialArrayDomain,axis);
#endif

#if !defined(NO_GlobalArrays)
     PADRE_ASSERT (pGlobalArrays_Descriptor != NULL);
#endif

#if defined (USE_KELP)
     PADRE_ASSERT (pKELP_Descriptor != NULL);
#endif

     return result;
   }

template<class UserCollection, class UserGlobalDomain, class UserLocalDomain>
int
PADRE_Descriptor<UserCollection, UserGlobalDomain, UserLocalDomain>::
isRightNullArray ( UserLocalDomain & serialArrayDomain , int axis ) const
   {

     int result = false;

#if !defined(NO_Parti)
     PADRE_ASSERT (pPARTI_Descriptor != NULL );
     if (pPARTI_Descriptor != NULL)
          result = pPARTI_Descriptor->isRightNullArray(serialArrayDomain,axis);
#endif

#if !defined(NO_GlobalArrays)
     PADRE_ASSERT (pGlobalArrays_Descriptor != NULL);
#endif

#if defined (USE_KELP)
     PADRE_ASSERT (pKELP_Descriptor != NULL);
#endif

     return result;
   }


// PADRE_DESCRIPTOR_C
#endif
