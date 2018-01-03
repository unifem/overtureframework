#define COMPILE_PPP

// *********************************************************
// Include A++ header files
// *********************************************************
#include "A++.h"






/* EXPAND THE MACROS HERE! */

//		Default constructor
List_Of_Partitioning_Type::List_Of_Partitioning_Type()
   {
  // Set referenceCount to zero for new object
     referenceCount = 0;

#if DEBUGTEMPLATE
     theIDCount++;
     theID = theIDCount;
     printf ("List default constructor called.  \n");
     printf ("List #%d  created. \n",theID);
#endif
  
     listLength = 0;
     aList      = NULL;
     memAlloc   = 0;
   }

//		Copy constructor

List_Of_Partitioning_Type::List_Of_Partitioning_Type(const List_Of_Partitioning_Type &X)
   {
  // Set referenceCount to zero for new object
     referenceCount = 0;

#if DEBUGTEMPLATE
  // Assign a unique ID from the static counter.
     theIDCount++;
     theID =theIDCount;
     printf ("List copy constructor called. \n");
     printf ("ID = %d  copied to %d \n",X.theID,theID);
#endif

     listLength = X.listLength;
  
     if(listLength < 1)
        {   // Determine if any allocation is necessary
          aList    = NULL;
          memAlloc = 0;
          return;
        }

  // (1/20/2002) IBM xlC complains that the expression in parenthesis is not constant
  // (as though it was a parameter instead of just unneccessary parenthesis).
  // aList = new (Partitioning_Type* [listLength]);		// Allocate pointer space.
     aList = new Partitioning_Type* [listLength];		// Allocate pointer space.
     memAlloc = listLength;
  
     for(int i=0; i<listLength; i++)		// Copy pointers.
          aList[i] = X.aList[i];
   }

// **************************
// *****   Destructor   *****
// **************************
List_Of_Partitioning_Type::~List_Of_Partitioning_Type()
   {
#if DEBUGTEMPLATE
     printf ("List destructor called.    \n");
     printf ("List #%d  destroyed. \n",theID);
#endif
  
  // printf ("DELETING LIST IN LIST CLASS! \n");
     delete[] aList;				// Delete the list of pointers (only).
     aList = NULL;
   }

//		Class equal operator

List_Of_Partitioning_Type& List_Of_Partitioning_Type::operator= (const List_Of_Partitioning_Type & X)
   {
#if DEBUGTEMPLATE
     printf ("List = operator called.           \n");
     printf ("ID = %d  copied to %d \n",X.theID,theID);
#endif

  // DO NOT MODIFY THE REFERENCE COUNT
  // referenceCount = 0;
     listLength = X.listLength;		// Like the copy constructor
     delete[] aList;			//   allocate and copy pointer list.
     aList = NULL;
  
     APP_ASSERT (listLength >= 0);
     if(listLength < 1)
        {
          aList    = NULL;
          memAlloc = 0;
          return *this;
        }
  
  // (1/20/2002) IBM xlC complains that the expression in parenthesis is not constant
  // (as though it was a parameter instead of just unneccessary parenthesis).
  // aList = new (Partitioning_Type* [listLength]);
     aList = new Partitioning_Type* [listLength];
     memAlloc = listLength;

     for(int i=0; i<listLength; i++)
          aList[i] = X.aList[i];

     return *this;
   }

//		Function Iterator

/*
void List_Of_Partitioning_Type::Iterator(void (Partitioning_Type::*Function)()){

  APP_ASSERT (aList != NULL);
  for(int i=0; i<listLength; i++)  // works for nontemplate.
    (aList[i]->*Function)();
} 
*/

//		Add an object element to the list.

void List_Of_Partitioning_Type::addElement( const Partitioning_Type & X )
   {
#if COMPILE_DEBUG_STATEMENTS
#if defined(LIST_OF_ARRAYS)
     if (APP_DEBUG > 1)
          printf ("Inside of List_Of_Partitioning_Type::addElement( const Partitioning_Type & X ) Array_ID = %d \n",X.Array_ID());
#endif
#endif
     if(listLength < memAlloc)
        {  // If there is enough memory just add it in!
          aList[listLength++] = &((Partitioning_Type &) X);
          return;
        }
  
     if(memAlloc ==0  )		// Double the memory size if it is less then
          memAlloc  = 2;		//   100 otherwise increase by 10 percent.
       else 
        {
          if(memAlloc < 100)
               memAlloc *= 2;
            else
               memAlloc += memAlloc/10;
        }

     Partitioning_Type **aListTmp;

  // (1/20/2002) IBM xlC complains that the expression in parenthesis is not constant
  // (as though it was a parameter instead of just unneccessary parenthesis).
     aListTmp = new Partitioning_Type* [memAlloc];
  
     int i = 0;
     for(i=0; i < listLength; i++)  	// Copy object pointers into new space.
        {
          aListTmp[i] = aList[i];
        }

     delete[] aList;			// Delete old list and add the object.
     aList = aListTmp;

     APP_ASSERT (aList != NULL);
     aList[listLength++] = &((Partitioning_Type &) X);

#if COMPILE_DEBUG_STATEMENTS
  // error checking for redundent entry
     for (i=0; i<listLength-1; i++)
        {
          if( &((Partitioning_Type &) X) == aList[i] )
             {
               printf ("ERROR: in List_Of_Partitioning_Type::addElement( const Partitioning_Type & X ) -- Redundent entry in list! (listLength = %d  i = %d) \n",listLength,i);
            // if ( X.Array_ID() != aList[i]->Array_ID() )
            //    {
            //      printf ("ANother error: X.Array_ID() = %d  != aList[i]->Array_ID() = %d  \n",X.Array_ID() , aList[i]->Array_ID() );
            //    }
               APP_ABORT();
             }
        }
#endif
   }

void List_Of_Partitioning_Type::addElement( const Partitioning_Type & X , int index_i)
   {
     if(listLength >= memAlloc)
        {
          if(memAlloc ==0  )			// Like above increase array size
               memAlloc  = 2;		//   if necessary.
            else 
             {
               if(memAlloc < 100)
                    memAlloc *= 2;
                 else
                    memAlloc += memAlloc/10;
             }

       // (1/20/2002) IBM xlC complains that the expression in parenthesis is not constant
       // (as though it was a parameter instead of just unneccessary parenthesis).
          Partitioning_Type **aListTmp;
          aListTmp = new Partitioning_Type* [memAlloc];
    
          for(int i=0; i<listLength; i++)
             {
               aListTmp[i] = aList[i];
             }
    
          delete[] aList;
          aList = aListTmp;
        }
  
     listLength++;				// Add in the object by ...
     checkRange(index_i);
    
     for(int i=listLength-1; i>index_i; i--)      // Displacing  elements.
        { 
          aList[i] = aList[i-1];
        }
  
     APP_ASSERT (aList != NULL);
     aList[index_i] = &((Partitioning_Type &) X);			// Put it at the desired location.
   }

//		Delete an element a location

void List_Of_Partitioning_Type::deleteElement(int index_i)
   {
     checkRange(index_i); // Check to make sure index_i is in the current range.
     for(int i=index_i; i<listLength-1; i++)
          aList[i] = aList[i+1];
  
     listLength--;
   }

//		Delete an element with the same pointer
void List_Of_Partitioning_Type::deleteElement( Partitioning_Type & X)
   {
     int i = 0;
#if COMPILE_DEBUG_STATEMENTS
#if defined(LIST_OF_ARRAYS)
     if (APP_DEBUG > 1)
        {
          printf ("Inside of List_Of_Partitioning_Type::deleteElement( const Partitioning_Type & X ) Array_ID = %d \n",X.Array_ID());
          for(i=0; i<listLength; i++)
             { 
               printf ("Array ID at list position %d = %d \n",i,aList[i]->Array_ID());
             } 
        }
#endif
#endif
     APP_ASSERT (aList != NULL);
     for(i=0; i<listLength; i++)
        {  // Loop until you find it.
          if(&X == aList[i])
             {
               deleteElement(i);
               return;
             }
        }
  // Not there!
     printf ("Object not found in list for deletion! \n");
     printf ("Proceed with caution... \n");
     printf ("Exiting as part of debugging addition/deletion from list ... \n");
#if defined(LIST_OF_ARRAYS)
     printf ("Inside of List_Of_Partitioning_Type::deleteElement( const Partitioning_Type & X ) Array_ID = %d \n",X.Array_ID());
     for(i=0; i<listLength; i++)
        { 
          printf ("Array ID at list position %d = %d \n",i,aList[i]->Array_ID());
        } 
#endif
     APP_ABORT();
   }

//		Swap two elements for sorting among other things.

void List_Of_Partitioning_Type::swapElements(int i, int j)
   {
  checkRange(i);
  checkRange(j);
  Partitioning_Type *tmp;
  
  APP_ASSERT (aList != NULL);
  tmp      = aList[i];
  aList[i] = aList[j];
  aList[j] = tmp;
}

//		Set the list element at i to point to X.
					
void List_Of_Partitioning_Type::setElementPtr( Partitioning_Type *X, int index_i)
   {
     checkRange(index_i);
     aList[index_i] = X;
   }

//		Get an object element by reference (the cute way)

Partitioning_Type & List_Of_Partitioning_Type::operator[] ( int index_i ) const 
   {
     checkRange(index_i);  
     APP_ASSERT (aList != NULL);
     return *aList[index_i];
   }
    
//		Get an object element by reference

Partitioning_Type & List_Of_Partitioning_Type::getElement ( int index_i ) const
   {
     checkRange(index_i);
     APP_ASSERT (aList != NULL);
     return *aList[index_i];
   }

//		Get an object element pointer
Partitioning_Type *List_Of_Partitioning_Type::getElementPtr ( int index_i )
   {
     checkRange(index_i);
     APP_ASSERT (aList != NULL);
     return aList[index_i];
   }

// Deallocate the list memory and set length to zero.
void List_Of_Partitioning_Type::clean()
   {
     listLength = 0;
     delete[] aList;
     aList = NULL;
   }

// Deallocate the list memory and delete the objects.

void List_Of_Partitioning_Type::deepClean()
   {
     for(int i=0; i<listLength; i++)
          delete aList[i];
     clean();
   }

//		Internal range check routine

void List_Of_Partitioning_Type::checkRange ( int index_i ) const 
   {
     if (index_i < 0 || index_i > listLength - 1)
        {
          cerr << "List Index Out of Range!" << endl;
          cerr << "  Index Value: " << index_i << endl;
          cerr << "  Index Range: 0 - " << listLength-1 << endl;
          APP_ABORT();
        }  
   }

int List_Of_Partitioning_Type::getIndex(const Partitioning_Type & X)
   {
     for (int i=0; i < listLength; i++)
        {
          APP_ASSERT (aList != NULL);
          if(aList[i] == &X)
               return(i); 
        }

     printf("ERROR: element not found, could return -1 but this is a bad practice \n");
     APP_ABORT();
     return (-1);
   }

int
List_Of_Partitioning_Type::checkElement(const Partitioning_Type & X)
   {
  // Check to see if element is already in the list 
     int returnValue = FALSE;
     if (aList != NULL)
        {
          APP_ASSERT (aList != NULL);
          int i = 0;
       // for (int i=0; i < listLength; i++)
          while ( (returnValue == FALSE) && (i < listLength) )
             {
               if(aList[i] == &X)
                    returnValue = TRUE;
               i++;
             }
        }

     return returnValue;
   }










