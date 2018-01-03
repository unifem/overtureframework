#ifndef PADRE_CommonInterface_C
#define PADRE_CommonInterface_C

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <PADRE_CommonInterface.h>
#include <PADRE_Global.h>

#if !defined NO_Parti
#include <PADRE_Parti.h>
#include <bsparti.h>
#endif

#include <iostream.h>

int PADRE_CommonInterface :: idCount = 0;

bool PADRE_CommonInterface :: initialized = false;


/*
  Function to initialize static members.
*/
void PADRE_CommonInterface::staticInitialize() {
  PADRE_CommonInterface::initialized = true;
  /*
    This class happens to not need a lot of static initializaion.
  */
}


/*
  The object's initialize function (includes a one-time initialization
  of PADRE_CommonInterface's static variables.
*/
void PADRE_CommonInterface::constructorInitialize()
   {
     // Perform a one-time initialization of PADRE_CommonInterface's static variables if not done.
     if (!PADRE_CommonInterface::initialized) {
       staticInitialize();
     }
     id = PADRE_CommonInterface::newId();
     referenceCount = getReferenceCountBase();
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_CommonInterface<UC,UD> :: initialize()"
               << " id " << getId() << "\n";
#if !defined(NO_Parti)
     /* the PARTI macro must agree with the PADRE macro */
     if ( MAX_DIM != PARTI_MAX_ARRAY_DIMENSION ) {
       cout << "ERROR: MAX_DIM (" << MAX_DIM << ") does not equal PARTI_MAX_ARRAY_DIMENSION ("
	    << PARTI_MAX_ARRAY_DIMENSION << ")" << endl;
     }
     PADRE_ASSERT( MAX_DIM == PARTI_MAX_ARRAY_DIMENSION );
#endif

   }

int PADRE_CommonInterface::newId()
   { 
     PADRE_ASSERT(++PADRE_CommonInterface::idCount != 0);  //wrapped!
     return PADRE_CommonInterface::idCount;
   }

PADRE_CommonInterface ::
~PADRE_CommonInterface()
   {
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_CommonInterface<UC,UD> ::"
               << " ~PADRE_CommonInterface()"
               << " id " << getId() << "\n";
   }

PADRE_CommonInterface ::
PADRE_CommonInterface()
   {
  // printf ("Inside of TOP of PADRE_CommonInterface constructor body \n");

#if 1
     constructorInitialize();
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_CommonInterface<UC,UD> ::"
               << " PADRE_CommonInterface()"
               << " id " << getId() << "\n";

#if 0
     if (getId() > 3)
        {
          printf ("Exiting at base of PADRE_CommonInterface default constructor \n");
          PADRE_ABORT();
        }
#endif
#endif

  // printf ("Inside of BASE of PADRE_CommonInterface constructor body \n");
   }

PADRE_CommonInterface ::
PADRE_CommonInterface( const PADRE_CommonInterface & X )
   {
     constructorInitialize();
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_CommonInterface<UC,UD> ::"
               << " PADRE_CommonInterface(const PADRE_CommonInterface & X )"
               << " id " << getId() << "\n";
   }

PADRE_CommonInterface & 
PADRE_CommonInterface ::
operator= ( const PADRE_CommonInterface & X )
   {
     if (PADRE::debugLevel() > 0)
          cout << "PADRE_CommonInterface<UC,UD> ::"
               << " PADRE_CommonInterface<UC,UD> & operator="
               << " ( const PADRE_Distribution<UC,UD> & X )"
               << " id " << getId() << "\n";

     cerr << "PADRE_CommonInterface::operator= not implemented!" << endl;
     PADRE_ABORT();
     return *this;
   }

void
PADRE_CommonInterface ::
incrementReferenceCount () const
   {
     PADRE_ASSERT(referenceCount > 0);
  // printf ("In PADRE_CommonInterface::incrementReferenceCount: referenceCount = %d \n",referenceCount);
     ((PADRE_CommonInterface*) this)->referenceCount++;  // cast away the const
   }

void
PADRE_CommonInterface ::
decrementReferenceCount () const
   {
  // PADRE_ASSERT(referenceCount > 1);
  // printf ("In PADRE_CommonInterface::decrementReferenceCount: referenceCount = %d \n",referenceCount);
     ((PADRE_CommonInterface*) this)->referenceCount--;  // cast away the const
   }

int
PADRE_CommonInterface ::
getReferenceCount () const
   {
  // PADRE_ASSERT(referenceCount > 0);
  // printf ("In PADRE_CommonInterface::getReferenceCount: referenceCount = %d \n",referenceCount);
     return referenceCount;
   }

void
PADRE_CommonInterface::
displayDefaultValues( const char *Label )
   {
  // This function prints the static variables of the class
     cout <<"Inside of PADRE_CommonInterface::displayDefaultValues (" << Label << ")\n"
	  << "idCount     = " << idCount << "\n"
	  << "initialized = " << initialized << "\n"
       ;
   }

void
PADRE_CommonInterface::
display( const char *Label ) const
   {
  // This function prints the non-static variables of the class
     cout <<"Inside of PADRE_CommonInterface::display (" << Label << ")\n"
	  << "id	     = " << id << "\n"
	  << "referenceCount = " << referenceCount << "\n"
       ;
   }




#endif	// PADRE_CommonInterface_C
