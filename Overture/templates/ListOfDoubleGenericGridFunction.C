#include "ListOfDoubleGenericGridFunction.h"
#include "wdhdefs.h"
#include <assert.h>
#include "OvertureInit.h"
#include "doubleGenericGridFunction.h"
//  ---Default constructor---
 
ListOfDoubleGenericGridFunction::
ListOfDoubleGenericGridFunction()
{
  initialize();  
}

// ---Create a list with a given number of elements---
 
ListOfDoubleGenericGridFunction::
ListOfDoubleGenericGridFunction( const int numberOfElements )
{
  initialize();  
  for( int i=0; i<numberOfElements; i++ )
    addElement();
  
}

// ---Copy constructor :  deep copy by default---
 
ListOfDoubleGenericGridFunction::
ListOfDoubleGenericGridFunction( const ListOfDoubleGenericGridFunction & X, const CopyType copyType )
{
  switch (copyType)
  {
  case DEEP:
    initialize();
    (*this)=X;
    break;
  case SHALLOW:
    rcData=X.rcData;   // before we call reference we must make rcData point to something
    rcData->incrementReferenceCount();
    reference( X ); 
    break;
  case NOCOPY:
    initialize();
    break;
  }
}


void ListOfDoubleGenericGridFunction::
initialize()
{
  rcData = ::new RCData;
  rcData->incrementReferenceCount();
  rcData->listLength = 0;
  rcData->memAlloc = 0;
  rcData->aList    = NULL;
}


// --- Destructor---
 
ListOfDoubleGenericGridFunction::
~ListOfDoubleGenericGridFunction()
{
//  cout << "destructor called for ListOfReferenceCountedData "<<
//    " referenceCount = " << rcData->getReferenceCount()-1 << endl;
  
  if( rcData->decrementReferenceCount() == 0 )
    deleteStuff();
}



void ListOfDoubleGenericGridFunction::
deleteStuff()
{
  for( int i=0; i<listLength(); i++)
    ::delete rcData->aList[i];      // first delete the objects
  
  if( rcData->memAlloc > 0 )
    ::delete [] rcData->aList;       // delete array of pointers
  ::delete rcData;
}


// ---Assignment with = is a deep copy ---
 
ListOfDoubleGenericGridFunction & ListOfDoubleGenericGridFunction::
operator=(const ListOfDoubleGenericGridFunction &X)
{

  // make the list the correct size
  int i;
  int length=listLength();
  if( length <  X.listLength() )
  {
    for( int i=length; i<X.listLength(); i++ )
      addElement();
  }
  else if( length > X.listLength() )
  {
    for( i=length-1; i >= X.getLength(); i-- )
      deleteElement();
  }
  if( rcData->listLength != X.listLength() )
  {
    cout << "ListOfDoubleGenericGridFunction:ERROR in operator=" << endl;
    cout << "         listLength's are not equal " << endl;
  }    

  for( i=0; i<listLength(); i++)
    (*this)[i] = X[i];  // this is assumed to be a deep copy

  return *this;
}

// ---Reference this list to another list---

void ListOfDoubleGenericGridFunction::
reference( const ListOfDoubleGenericGridFunction & list )
{
  if( rcData->decrementReferenceCount() == 0 )
    deleteStuff();  
  rcData=list.rcData;
  rcData->incrementReferenceCount();

}

// ---break the reference of this list (if any)---

void ListOfDoubleGenericGridFunction::
breakReference()
{
  // if there is only 1 reference, there is no need to make a new copy  
  if( rcData->getReferenceCount() != 1 )
  {
    ListOfDoubleGenericGridFunction list = *this;  // makes a deep copy
    reference(list);   // make a reference to this new copy
  }
}


void ListOfDoubleGenericGridFunction::
destroy()
// =======================================================================
// /Description:
//    Destroy the list. 
// =======================================================================
{
  if( rcData->decrementReferenceCount() == 0 )
  {
    // delete all the data if no-one is referencing it.
    deleteStuff();
  }
  // make this a valid null list
  initialize();
}

 
void ListOfDoubleGenericGridFunction::
openAPositionForAnElement( const int index )
// =================================================================================================
// /Description: (private routine)
//   Adjust the list (if necessary) so that a new Element can be created at the "index" position
//   Whoever calls this function is responsible for "newing" a doubleGenericGridFunction and adding it to the list
// =================================================================================================
{

  // If there is enough memory just add it in!
  if( index==listLength() && listLength() < rcData->memAlloc )
  {  
    rcData->listLength++;
//    rcData->aList[rcData->listLength++]= ::new doubleGenericGridFunction;
//    assert(rcData->aList[rcData->listLength-1]!=NULL);
    return;
  }

  if(listLength() >= rcData->memAlloc)
  {
    if(rcData->memAlloc ==0  )			// Double the memory if it less than
      rcData->memAlloc  = 2;			// 100, otherwise increase by 10 percent
    else if(rcData->memAlloc < 100)
      rcData->memAlloc *= 2;
    else
      rcData->memAlloc += rcData->memAlloc/10;

    doubleGenericGridFunction **aListTmp;

    aListTmp = ::new doubleGenericGridFunction* [rcData->memAlloc]; // create an array of pointers
    assert(aListTmp!=NULL);
    
    for(int i=0; i<listLength(); i++)
      aListTmp[i] = rcData->aList[i];

    ::delete [] rcData->aList;                // delete old list
    rcData->aList = aListTmp;
  }
  
  rcData->listLength++;				// Add in the object by ...
  checkRange(index);
    
  for(int i=listLength()-1; i>index; i--)   // Displacing  elements.
    rcData->aList[i] = rcData->aList[i-1];

//  rcData->aList[index] = ::new doubleGenericGridFunction;			// Put it at the desired location.
//  assert(rcData->aList[index]!=NULL);
}



// ---Add a new object at position index---
// note: addElement creates a new object
 
void ListOfDoubleGenericGridFunction::
addElement( const int index )
{
  openAPositionForAnElement( index );     
  rcData->aList[index]= ::new doubleGenericGridFunction;
  assert(rcData->aList[index]!=NULL);

}

//--- Add a new object to the end of the list---
 
void ListOfDoubleGenericGridFunction::
addElement( )
{
  addElement( listLength() );
}


// ---Add a new object at position index and reference to Object t---
 
void ListOfDoubleGenericGridFunction::
addElement( const doubleGenericGridFunction & t, const int index )
{
//  addElement( index );
//  (*this)[index].reference(t);

  openAPositionForAnElement( index );     
  // do a shallow copy as this will be faster
// *wdh   rcData->aList[index]= ::new doubleGenericGridFunction(t,SHALLOW);  // trouble with A++ arrays
// *****
  rcData->aList[index]= ::new doubleGenericGridFunction;
  rcData->aList[index]->reference(t);
// *****
  assert(rcData->aList[index]!=NULL);
}  

// ---Add a new object at the end of the list and reference to Object t---
 
void ListOfDoubleGenericGridFunction::
addElement( const doubleGenericGridFunction & t )
{
  addElement( t,listLength() );
}

//  ---Delete a given element ---
 
void ListOfDoubleGenericGridFunction::
deleteElement(const int index)
{
  checkRange(index);  // Check to make sure index is in the current range.
  
  ::delete rcData->aList[index];				    
  for(int i=index; i<listLength()-1; i++)
    rcData->aList[i] = rcData->aList[i+1];
  
  rcData->listLength--;
}

// ---Delete the last element ---
 
void ListOfDoubleGenericGridFunction::
deleteElement()
{
  deleteElement( listLength()-1 );
}

//		Delete an element with the same pointer
 
void ListOfDoubleGenericGridFunction::
deleteElement( const doubleGenericGridFunction &X)
{
  for(int i=0; i<listLength(); i++)
  {	
    if(&X == rcData->aList[i])
    {
      deleteElement(i);
      return;
    }
  }
					// Not there!
  cerr << "ListOfDoubleGenericGridFunction: Object not found in list for deletion!" << endl;
  cerr << "Proceed with caution..." << endl; 
}

// Swap two elements for sorting among other things.
 void 
ListOfDoubleGenericGridFunction::
swapElements( const int i, const int j)
{
  checkRange(i);
  checkRange(j);
    
  doubleGenericGridFunction *tmp;
  tmp = rcData->aList[i];
  rcData->aList[i] = rcData->aList[j];
  rcData->aList[j] = tmp;
}


//  Get an object element by reference
 
doubleGenericGridFunction& ListOfDoubleGenericGridFunction::
operator[]( const int index ) const 
{
  checkRange(index);  
  return *rcData->aList[index];
}
    
#undef getIndex
// Return the index of an object, return -1 if not found
 
int ListOfDoubleGenericGridFunction::
getIndex(const doubleGenericGridFunction & X) const
{
  for(int i=0; i<listLength(); i++)
  {	
    if(&X == rcData->aList[i])
    {
      return i;
    }
  }
  return -1;  // not found
}


//		Internal range check routine
 
void ListOfDoubleGenericGridFunction::
checkRange( const int index) const 
{
  if(index < 0 || index > listLength() - 1)
  {
    cerr << "ListOfDoubleGenericGridFunction: List Index Out of Range!" << endl;
    cerr << "  trying to reference a list with index=" << index << endl;
    cerr << "  but the list only has indicies in the range: [0," << listLength()-1 << "]" << endl;
    cerr << "  length of List = " << getLength() << endl;
    Overture::abort("ListOfDoubleGenericGridFunction: List Index Out of Range!");
  }  
}

