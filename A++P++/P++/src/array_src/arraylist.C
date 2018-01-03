#define COMPILE_PPP

// GNU will build intances of all objects in the header file if this
// is not specified.  The result is very large object files (too many symbols)
// so we can significantly reduce the size of the object files which will
// build the library (factor of 5-10).
#ifdef GNU
#pragma implementation "arraylist.h"
#endif

// *********************************************************
// Include A++ header files
// *********************************************************
#include "A++.h"






#define LIST_OF_ARRAYS

/* EXPAND THE MACROS HERE! */

#define DOUBLEARRAY
//		Default constructor
List_Of_doubleArray::List_Of_doubleArray()
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

List_Of_doubleArray::List_Of_doubleArray(const List_Of_doubleArray &X)
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
  // aList = new (doubleArray* [listLength]);		// Allocate pointer space.
     aList = new doubleArray* [listLength];		// Allocate pointer space.
     memAlloc = listLength;
  
     for(int i=0; i<listLength; i++)		// Copy pointers.
          aList[i] = X.aList[i];
   }

// **************************
// *****   Destructor   *****
// **************************
List_Of_doubleArray::~List_Of_doubleArray()
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

List_Of_doubleArray& List_Of_doubleArray::operator= (const List_Of_doubleArray & X)
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
  // aList = new (doubleArray* [listLength]);
     aList = new doubleArray* [listLength];
     memAlloc = listLength;

     for(int i=0; i<listLength; i++)
          aList[i] = X.aList[i];

     return *this;
   }

//		Function Iterator

/*
void List_Of_doubleArray::Iterator(void (doubleArray::*Function)()){

  APP_ASSERT (aList != NULL);
  for(int i=0; i<listLength; i++)  // works for nontemplate.
    (aList[i]->*Function)();
} 
*/

//		Add an object element to the list.

void List_Of_doubleArray::addElement( const doubleArray & X )
   {
#if COMPILE_DEBUG_STATEMENTS
#if defined(LIST_OF_ARRAYS)
     if (APP_DEBUG > 1)
          printf ("Inside of List_Of_doubleArray::addElement( const doubleArray & X ) Array_ID = %d \n",X.Array_ID());
#endif
#endif
     if(listLength < memAlloc)
        {  // If there is enough memory just add it in!
          aList[listLength++] = &((doubleArray &) X);
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

     doubleArray **aListTmp;

  // (1/20/2002) IBM xlC complains that the expression in parenthesis is not constant
  // (as though it was a parameter instead of just unneccessary parenthesis).
     aListTmp = new doubleArray* [memAlloc];
  
     int i = 0;
     for(i=0; i < listLength; i++)  	// Copy object pointers into new space.
        {
          aListTmp[i] = aList[i];
        }

     delete[] aList;			// Delete old list and add the object.
     aList = aListTmp;

     APP_ASSERT (aList != NULL);
     aList[listLength++] = &((doubleArray &) X);

#if COMPILE_DEBUG_STATEMENTS
  // error checking for redundent entry
     for (i=0; i<listLength-1; i++)
        {
          if( &((doubleArray &) X) == aList[i] )
             {
               printf ("ERROR: in List_Of_doubleArray::addElement( const doubleArray & X ) -- Redundent entry in list! (listLength = %d  i = %d) \n",listLength,i);
            // if ( X.Array_ID() != aList[i]->Array_ID() )
            //    {
            //      printf ("ANother error: X.Array_ID() = %d  != aList[i]->Array_ID() = %d  \n",X.Array_ID() , aList[i]->Array_ID() );
            //    }
               APP_ABORT();
             }
        }
#endif
   }

void List_Of_doubleArray::addElement( const doubleArray & X , int index_i)
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
          doubleArray **aListTmp;
          aListTmp = new doubleArray* [memAlloc];
    
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
     aList[index_i] = &((doubleArray &) X);			// Put it at the desired location.
   }

//		Delete an element a location

void List_Of_doubleArray::deleteElement(int index_i)
   {
     checkRange(index_i); // Check to make sure index_i is in the current range.
     for(int i=index_i; i<listLength-1; i++)
          aList[i] = aList[i+1];
  
     listLength--;
   }

//		Delete an element with the same pointer
void List_Of_doubleArray::deleteElement( doubleArray & X)
   {
     int i = 0;
#if COMPILE_DEBUG_STATEMENTS
#if defined(LIST_OF_ARRAYS)
     if (APP_DEBUG > 1)
        {
          printf ("Inside of List_Of_doubleArray::deleteElement( const doubleArray & X ) Array_ID = %d \n",X.Array_ID());
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
     printf ("Inside of List_Of_doubleArray::deleteElement( const doubleArray & X ) Array_ID = %d \n",X.Array_ID());
     for(i=0; i<listLength; i++)
        { 
          printf ("Array ID at list position %d = %d \n",i,aList[i]->Array_ID());
        } 
#endif
     APP_ABORT();
   }

//		Swap two elements for sorting among other things.

void List_Of_doubleArray::swapElements(int i, int j)
   {
  checkRange(i);
  checkRange(j);
  doubleArray *tmp;
  
  APP_ASSERT (aList != NULL);
  tmp      = aList[i];
  aList[i] = aList[j];
  aList[j] = tmp;
}

//		Set the list element at i to point to X.
					
void List_Of_doubleArray::setElementPtr( doubleArray *X, int index_i)
   {
     checkRange(index_i);
     aList[index_i] = X;
   }

//		Get an object element by reference (the cute way)

doubleArray & List_Of_doubleArray::operator[] ( int index_i ) const 
   {
     checkRange(index_i);  
     APP_ASSERT (aList != NULL);
     return *aList[index_i];
   }
    
//		Get an object element by reference

doubleArray & List_Of_doubleArray::getElement ( int index_i ) const
   {
     checkRange(index_i);
     APP_ASSERT (aList != NULL);
     return *aList[index_i];
   }

//		Get an object element pointer
doubleArray *List_Of_doubleArray::getElementPtr ( int index_i )
   {
     checkRange(index_i);
     APP_ASSERT (aList != NULL);
     return aList[index_i];
   }

// Deallocate the list memory and set length to zero.
void List_Of_doubleArray::clean()
   {
     listLength = 0;
     delete[] aList;
     aList = NULL;
   }

// Deallocate the list memory and delete the objects.

void List_Of_doubleArray::deepClean()
   {
     for(int i=0; i<listLength; i++)
          delete aList[i];
     clean();
   }

//		Internal range check routine

void List_Of_doubleArray::checkRange ( int index_i ) const 
   {
     if (index_i < 0 || index_i > listLength - 1)
        {
          cerr << "List Index Out of Range!" << endl;
          cerr << "  Index Value: " << index_i << endl;
          cerr << "  Index Range: 0 - " << listLength-1 << endl;
          APP_ABORT();
        }  
   }

int List_Of_doubleArray::getIndex(const doubleArray & X)
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
List_Of_doubleArray::checkElement(const doubleArray & X)
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


#undef DOUBLEARRAY

#define FLOATARRAY
//		Default constructor
List_Of_floatArray::List_Of_floatArray()
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

List_Of_floatArray::List_Of_floatArray(const List_Of_floatArray &X)
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
  // aList = new (floatArray* [listLength]);		// Allocate pointer space.
     aList = new floatArray* [listLength];		// Allocate pointer space.
     memAlloc = listLength;
  
     for(int i=0; i<listLength; i++)		// Copy pointers.
          aList[i] = X.aList[i];
   }

// **************************
// *****   Destructor   *****
// **************************
List_Of_floatArray::~List_Of_floatArray()
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

List_Of_floatArray& List_Of_floatArray::operator= (const List_Of_floatArray & X)
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
  // aList = new (floatArray* [listLength]);
     aList = new floatArray* [listLength];
     memAlloc = listLength;

     for(int i=0; i<listLength; i++)
          aList[i] = X.aList[i];

     return *this;
   }

//		Function Iterator

/*
void List_Of_floatArray::Iterator(void (floatArray::*Function)()){

  APP_ASSERT (aList != NULL);
  for(int i=0; i<listLength; i++)  // works for nontemplate.
    (aList[i]->*Function)();
} 
*/

//		Add an object element to the list.

void List_Of_floatArray::addElement( const floatArray & X )
   {
#if COMPILE_DEBUG_STATEMENTS
#if defined(LIST_OF_ARRAYS)
     if (APP_DEBUG > 1)
          printf ("Inside of List_Of_floatArray::addElement( const floatArray & X ) Array_ID = %d \n",X.Array_ID());
#endif
#endif
     if(listLength < memAlloc)
        {  // If there is enough memory just add it in!
          aList[listLength++] = &((floatArray &) X);
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

     floatArray **aListTmp;

  // (1/20/2002) IBM xlC complains that the expression in parenthesis is not constant
  // (as though it was a parameter instead of just unneccessary parenthesis).
     aListTmp = new floatArray* [memAlloc];
  
     int i = 0;
     for(i=0; i < listLength; i++)  	// Copy object pointers into new space.
        {
          aListTmp[i] = aList[i];
        }

     delete[] aList;			// Delete old list and add the object.
     aList = aListTmp;

     APP_ASSERT (aList != NULL);
     aList[listLength++] = &((floatArray &) X);

#if COMPILE_DEBUG_STATEMENTS
  // error checking for redundent entry
     for (i=0; i<listLength-1; i++)
        {
          if( &((floatArray &) X) == aList[i] )
             {
               printf ("ERROR: in List_Of_floatArray::addElement( const floatArray & X ) -- Redundent entry in list! (listLength = %d  i = %d) \n",listLength,i);
            // if ( X.Array_ID() != aList[i]->Array_ID() )
            //    {
            //      printf ("ANother error: X.Array_ID() = %d  != aList[i]->Array_ID() = %d  \n",X.Array_ID() , aList[i]->Array_ID() );
            //    }
               APP_ABORT();
             }
        }
#endif
   }

void List_Of_floatArray::addElement( const floatArray & X , int index_i)
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
          floatArray **aListTmp;
          aListTmp = new floatArray* [memAlloc];
    
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
     aList[index_i] = &((floatArray &) X);			// Put it at the desired location.
   }

//		Delete an element a location

void List_Of_floatArray::deleteElement(int index_i)
   {
     checkRange(index_i); // Check to make sure index_i is in the current range.
     for(int i=index_i; i<listLength-1; i++)
          aList[i] = aList[i+1];
  
     listLength--;
   }

//		Delete an element with the same pointer
void List_Of_floatArray::deleteElement( floatArray & X)
   {
     int i = 0;
#if COMPILE_DEBUG_STATEMENTS
#if defined(LIST_OF_ARRAYS)
     if (APP_DEBUG > 1)
        {
          printf ("Inside of List_Of_floatArray::deleteElement( const floatArray & X ) Array_ID = %d \n",X.Array_ID());
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
     printf ("Inside of List_Of_floatArray::deleteElement( const floatArray & X ) Array_ID = %d \n",X.Array_ID());
     for(i=0; i<listLength; i++)
        { 
          printf ("Array ID at list position %d = %d \n",i,aList[i]->Array_ID());
        } 
#endif
     APP_ABORT();
   }

//		Swap two elements for sorting among other things.

void List_Of_floatArray::swapElements(int i, int j)
   {
  checkRange(i);
  checkRange(j);
  floatArray *tmp;
  
  APP_ASSERT (aList != NULL);
  tmp      = aList[i];
  aList[i] = aList[j];
  aList[j] = tmp;
}

//		Set the list element at i to point to X.
					
void List_Of_floatArray::setElementPtr( floatArray *X, int index_i)
   {
     checkRange(index_i);
     aList[index_i] = X;
   }

//		Get an object element by reference (the cute way)

floatArray & List_Of_floatArray::operator[] ( int index_i ) const 
   {
     checkRange(index_i);  
     APP_ASSERT (aList != NULL);
     return *aList[index_i];
   }
    
//		Get an object element by reference

floatArray & List_Of_floatArray::getElement ( int index_i ) const
   {
     checkRange(index_i);
     APP_ASSERT (aList != NULL);
     return *aList[index_i];
   }

//		Get an object element pointer
floatArray *List_Of_floatArray::getElementPtr ( int index_i )
   {
     checkRange(index_i);
     APP_ASSERT (aList != NULL);
     return aList[index_i];
   }

// Deallocate the list memory and set length to zero.
void List_Of_floatArray::clean()
   {
     listLength = 0;
     delete[] aList;
     aList = NULL;
   }

// Deallocate the list memory and delete the objects.

void List_Of_floatArray::deepClean()
   {
     for(int i=0; i<listLength; i++)
          delete aList[i];
     clean();
   }

//		Internal range check routine

void List_Of_floatArray::checkRange ( int index_i ) const 
   {
     if (index_i < 0 || index_i > listLength - 1)
        {
          cerr << "List Index Out of Range!" << endl;
          cerr << "  Index Value: " << index_i << endl;
          cerr << "  Index Range: 0 - " << listLength-1 << endl;
          APP_ABORT();
        }  
   }

int List_Of_floatArray::getIndex(const floatArray & X)
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
List_Of_floatArray::checkElement(const floatArray & X)
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


#undef FLOATARRAY

#define INTARRAY
//		Default constructor
List_Of_intArray::List_Of_intArray()
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

List_Of_intArray::List_Of_intArray(const List_Of_intArray &X)
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
  // aList = new (intArray* [listLength]);		// Allocate pointer space.
     aList = new intArray* [listLength];		// Allocate pointer space.
     memAlloc = listLength;
  
     for(int i=0; i<listLength; i++)		// Copy pointers.
          aList[i] = X.aList[i];
   }

// **************************
// *****   Destructor   *****
// **************************
List_Of_intArray::~List_Of_intArray()
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

List_Of_intArray& List_Of_intArray::operator= (const List_Of_intArray & X)
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
  // aList = new (intArray* [listLength]);
     aList = new intArray* [listLength];
     memAlloc = listLength;

     for(int i=0; i<listLength; i++)
          aList[i] = X.aList[i];

     return *this;
   }

//		Function Iterator

/*
void List_Of_intArray::Iterator(void (intArray::*Function)()){

  APP_ASSERT (aList != NULL);
  for(int i=0; i<listLength; i++)  // works for nontemplate.
    (aList[i]->*Function)();
} 
*/

//		Add an object element to the list.

void List_Of_intArray::addElement( const intArray & X )
   {
#if COMPILE_DEBUG_STATEMENTS
#if defined(LIST_OF_ARRAYS)
     if (APP_DEBUG > 1)
          printf ("Inside of List_Of_intArray::addElement( const intArray & X ) Array_ID = %d \n",X.Array_ID());
#endif
#endif
     if(listLength < memAlloc)
        {  // If there is enough memory just add it in!
          aList[listLength++] = &((intArray &) X);
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

     intArray **aListTmp;

  // (1/20/2002) IBM xlC complains that the expression in parenthesis is not constant
  // (as though it was a parameter instead of just unneccessary parenthesis).
     aListTmp = new intArray* [memAlloc];
  
     int i = 0;
     for(i=0; i < listLength; i++)  	// Copy object pointers into new space.
        {
          aListTmp[i] = aList[i];
        }

     delete[] aList;			// Delete old list and add the object.
     aList = aListTmp;

     APP_ASSERT (aList != NULL);
     aList[listLength++] = &((intArray &) X);

#if COMPILE_DEBUG_STATEMENTS
  // error checking for redundent entry
     for (i=0; i<listLength-1; i++)
        {
          if( &((intArray &) X) == aList[i] )
             {
               printf ("ERROR: in List_Of_intArray::addElement( const intArray & X ) -- Redundent entry in list! (listLength = %d  i = %d) \n",listLength,i);
            // if ( X.Array_ID() != aList[i]->Array_ID() )
            //    {
            //      printf ("ANother error: X.Array_ID() = %d  != aList[i]->Array_ID() = %d  \n",X.Array_ID() , aList[i]->Array_ID() );
            //    }
               APP_ABORT();
             }
        }
#endif
   }

void List_Of_intArray::addElement( const intArray & X , int index_i)
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
          intArray **aListTmp;
          aListTmp = new intArray* [memAlloc];
    
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
     aList[index_i] = &((intArray &) X);			// Put it at the desired location.
   }

//		Delete an element a location

void List_Of_intArray::deleteElement(int index_i)
   {
     checkRange(index_i); // Check to make sure index_i is in the current range.
     for(int i=index_i; i<listLength-1; i++)
          aList[i] = aList[i+1];
  
     listLength--;
   }

//		Delete an element with the same pointer
void List_Of_intArray::deleteElement( intArray & X)
   {
     int i = 0;
#if COMPILE_DEBUG_STATEMENTS
#if defined(LIST_OF_ARRAYS)
     if (APP_DEBUG > 1)
        {
          printf ("Inside of List_Of_intArray::deleteElement( const intArray & X ) Array_ID = %d \n",X.Array_ID());
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
     printf ("Inside of List_Of_intArray::deleteElement( const intArray & X ) Array_ID = %d \n",X.Array_ID());
     for(i=0; i<listLength; i++)
        { 
          printf ("Array ID at list position %d = %d \n",i,aList[i]->Array_ID());
        } 
#endif
     APP_ABORT();
   }

//		Swap two elements for sorting among other things.

void List_Of_intArray::swapElements(int i, int j)
   {
  checkRange(i);
  checkRange(j);
  intArray *tmp;
  
  APP_ASSERT (aList != NULL);
  tmp      = aList[i];
  aList[i] = aList[j];
  aList[j] = tmp;
}

//		Set the list element at i to point to X.
					
void List_Of_intArray::setElementPtr( intArray *X, int index_i)
   {
     checkRange(index_i);
     aList[index_i] = X;
   }

//		Get an object element by reference (the cute way)

intArray & List_Of_intArray::operator[] ( int index_i ) const 
   {
     checkRange(index_i);  
     APP_ASSERT (aList != NULL);
     return *aList[index_i];
   }
    
//		Get an object element by reference

intArray & List_Of_intArray::getElement ( int index_i ) const
   {
     checkRange(index_i);
     APP_ASSERT (aList != NULL);
     return *aList[index_i];
   }

//		Get an object element pointer
intArray *List_Of_intArray::getElementPtr ( int index_i )
   {
     checkRange(index_i);
     APP_ASSERT (aList != NULL);
     return aList[index_i];
   }

// Deallocate the list memory and set length to zero.
void List_Of_intArray::clean()
   {
     listLength = 0;
     delete[] aList;
     aList = NULL;
   }

// Deallocate the list memory and delete the objects.

void List_Of_intArray::deepClean()
   {
     for(int i=0; i<listLength; i++)
          delete aList[i];
     clean();
   }

//		Internal range check routine

void List_Of_intArray::checkRange ( int index_i ) const 
   {
     if (index_i < 0 || index_i > listLength - 1)
        {
          cerr << "List Index Out of Range!" << endl;
          cerr << "  Index Value: " << index_i << endl;
          cerr << "  Index Range: 0 - " << listLength-1 << endl;
          APP_ABORT();
        }  
   }

int List_Of_intArray::getIndex(const intArray & X)
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
List_Of_intArray::checkElement(const intArray & X)
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


#undef INTARRAY








