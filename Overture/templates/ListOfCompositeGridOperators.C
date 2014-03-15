#include <iostream>
#include <stdlib.h>
#include "CompositeGridOperators.h"
#include "ListOfCompositeGridOperators.h"

using namespace std;

//		Default constructor

 ListOfCompositeGridOperators::ListOfCompositeGridOperators(){

#if DEBUGTEMPLATE
  theIDCount++;
  theID = theIDCount;
  cout << "ListOfCompositeGridOperators default constructor called.  ";
  cout << "ListOfCompositeGridOperators #" << theID << " created." << endl;
#endif
  
  listLength = 0;
  aList    = 0;
  memAlloc = 0;
}

//		Copy constructor

 ListOfCompositeGridOperators::ListOfCompositeGridOperators(const ListOfCompositeGridOperators &X){

#if DEBUGTEMPLATE			// Assign a unique ID from the
  theIDCount++;				// static counter.
  theID =theIDCount;
  cout << "ListOfCompositeGridOperators copy constructor called.     ";
  cout << "ID " << X.theID <<" copied to " << theID << "." << endl;
#endif

  listLength = X.listLength;
  
  if(listLength < 1){			// Determine if any allocation is
    aList    = 0;			//   necessary.
    memAlloc = 0;
    return;
  }
  
  aList = new CompositeGridOperators* [listLength];		// Allocate pointer space.
  memAlloc = listLength;
  
  for(int i=0; i<listLength; i++)		// Copy pointers.
    aList[i] = X.aList[i];
}

//		Destructor

 ListOfCompositeGridOperators::~ListOfCompositeGridOperators(){

#if DEBUGTEMPLATE
  cout << "ListOfCompositeGridOperators destructor called.           ";
  cout << "ListOfCompositeGridOperators #" << theID << " destroyed." << endl;
#endif
  
  delete[] aList;				// Delete the list of pointers (only).
}

//		Class equal operator

 ListOfCompositeGridOperators& ListOfCompositeGridOperators::operator=(const ListOfCompositeGridOperators &X){

#if DEBUGTEMPLATE
  cout << "ListOfCompositeGridOperators = operator called.           ";
  cout << "ID " << X.theID <<" copied to " << theID << "." << endl;
#endif

  listLength = X.listLength;		// Like the copy constructor
  delete[] aList;			//   allocate and copy pointer list.
  
  if(listLength < 1){
    aList    = 0;
    memAlloc = 0;
    return *this;
  }
  
  aList = new CompositeGridOperators* [listLength];
  memAlloc = listLength;
  
  for(int i=0; i<listLength; i++)
    aList[i] = X.aList[i];

  return *this;
}

//		Function Iterator

/*
 void ListOfCompositeGridOperators::Iterator(void (CompositeGridOperators::*Function)()){

  for(int i=0; i<listLength; i++)  // works for nontemplate.
    (aList[i]->*Function)();
} 
*/

//		Add an object element to the list.

 void ListOfCompositeGridOperators::addElement(CompositeGridOperators &X){

  if(listLength < memAlloc){	// If there is enough memory just add it in!
    aList[listLength++] = &X;
    return;
  }
  
  if(memAlloc ==0  )		// Double the memory size if it is less then
    memAlloc  = 2;		//   100 otherwise increase by 10 percent.
  else if(memAlloc < 100)
    memAlloc *= 2;
  else
    memAlloc += memAlloc/10;

  CompositeGridOperators **aListTmp;

  aListTmp = new CompositeGridOperators* [memAlloc];
  
  for(int i=0; i<listLength; i++){  	// Copy object pointers into new space.
    aListTmp[i] = aList[i];
  }
  
  delete[] aList;			// Delete old list and add the object.
  aList = aListTmp;
  
  aList[listLength++] = &X;

}

 void ListOfCompositeGridOperators::addElement(CompositeGridOperators &X, int index){

  if(listLength >= memAlloc){
  
    if(memAlloc ==0  )			// Like above increase array size
      memAlloc  = 2;			//   if necessary.
    else if(memAlloc < 100)
      memAlloc *= 2;
    else
      memAlloc += memAlloc/10;

    CompositeGridOperators **aListTmp;

    aListTmp = new CompositeGridOperators* [memAlloc];
    
    for(int i=0; i<listLength; i++){
      aListTmp[i] = aList[i];
    }
    
    delete[] aList;
    aList = aListTmp;
  }
  
  listLength++;				// Add in the object by ...
  checkRange(index);
    
  for(int i=listLength-1; i>index; i--){  // Displacing  elements.
    aList[i] = aList[i-1];
  }
  
  aList[index] = &X;			// Put it at the desired location.
}

//		Delete an element a location

 void ListOfCompositeGridOperators::deleteElement(int index){

  checkRange(index);			// Check to make sure index is in the
					//   current range.
  for(int i=index; i<listLength-1; i++)
    aList[i] = aList[i+1];
  
  listLength--;
}

//		Delete an element with the same pointer

 void ListOfCompositeGridOperators::deleteElement(CompositeGridOperators &X){

  for(int i=0; i<listLength; i++){	// Loop until you find it.
    if(&X == aList[i]){
      deleteElement(i);
      return;
    }
  }
					// Not there!

  cerr << "Object not found in list for deletion!" << endl;
  cerr << "Proceed with caution..." << endl; 
}

//		Swap two elements for sorting among other things.

 void ListOfCompositeGridOperators::swapElements(int i, int j){

  checkRange(i);
  checkRange(j);
    
  CompositeGridOperators *tmp;
  
  tmp      = aList[i];
  aList[i] = aList[j];
  aList[j] = tmp;
}

//		Set the list element at i to point to X.
					
 void ListOfCompositeGridOperators::setElementPtr(CompositeGridOperators *X, int index){
  checkRange(index);
  aList[index] = X;
}

//		Get an object element by reference (the cute way)

 CompositeGridOperators& ListOfCompositeGridOperators::operator[](int index) const{
  checkRange(index);  
  return *aList[index];
}
    
//		Get an object element by reference

 CompositeGridOperators& ListOfCompositeGridOperators::getElement(int index) const {
  checkRange(index);  
  return *aList[index];
}

//		Get an object element pointer

 CompositeGridOperators *ListOfCompositeGridOperators::getElementPtr(int index) const{
  
  checkRange(index);
  return aList[index];
}

//		Deallocate the list memory and set length to zero.

 void ListOfCompositeGridOperators::clean(){
  listLength = 0;
}

//		Deallocate the list memory and delete the objects.

 void ListOfCompositeGridOperators::deepClean(){
  for(int i=0; i<listLength; i++)
    delete aList[i];
  
  listLength = 0;
}

//		Internal range check routine

 void ListOfCompositeGridOperators::
checkRange(int index) const{
  if(index < 0 || index > listLength - 1){
    cerr << "ListOfCompositeGridOperators Index Out of Range!" << endl;
    cerr << "  Index Value: " << index << endl;
    cerr << "  Index Range: 0 - " << listLength-1 << endl;
    exit(-1);
  }  
}

