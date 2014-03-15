#include <iostream>
#include <stdlib.h>
#include "BoundingBox.h"
#include "ListOfBoundingBox.h"

using namespace std;

//		Default constructor

 ListOfBoundingBox::ListOfBoundingBox(){

#if DEBUGTEMPLATE
  theIDCount++;
  theID = theIDCount;
  cout << "ListOfBoundingBox default constructor called.  ";
  cout << "ListOfBoundingBox #" << theID << " created." << endl;
#endif
  
  listLength = 0;
  aList    = 0;
  memAlloc = 0;
}

//		Copy constructor

 ListOfBoundingBox::ListOfBoundingBox(const ListOfBoundingBox &X){

#if DEBUGTEMPLATE			// Assign a unique ID from the
  theIDCount++;				// static counter.
  theID =theIDCount;
  cout << "ListOfBoundingBox copy constructor called.     ";
  cout << "ID " << X.theID <<" copied to " << theID << "." << endl;
#endif

  listLength = X.listLength;
  
  if(listLength < 1){			// Determine if any allocation is
    aList    = 0;			//   necessary.
    memAlloc = 0;
    return;
  }
  
  aList = new BoundingBox* [listLength];		// Allocate pointer space.
  memAlloc = listLength;
  
  for(int i=0; i<listLength; i++)		// Copy pointers.
    aList[i] = X.aList[i];
}

//		Destructor

 ListOfBoundingBox::~ListOfBoundingBox(){

#if DEBUGTEMPLATE
  cout << "ListOfBoundingBox destructor called.           ";
  cout << "ListOfBoundingBox #" << theID << " destroyed." << endl;
#endif
  
  delete[] aList;				// Delete the list of pointers (only).
}

//		Class equal operator

 ListOfBoundingBox& ListOfBoundingBox::operator=(const ListOfBoundingBox &X){

#if DEBUGTEMPLATE
  cout << "ListOfBoundingBox = operator called.           ";
  cout << "ID " << X.theID <<" copied to " << theID << "." << endl;
#endif

  listLength = X.listLength;		// Like the copy constructor
  delete[] aList;			//   allocate and copy pointer list.
  
  if(listLength < 1){
    aList    = 0;
    memAlloc = 0;
    return *this;
  }
  
  aList = new BoundingBox* [listLength];
  memAlloc = listLength;
  
  for(int i=0; i<listLength; i++)
    aList[i] = X.aList[i];

  return *this;
}

//		Function Iterator

/*
 void ListOfBoundingBox::Iterator(void (BoundingBox::*Function)()){

  for(int i=0; i<listLength; i++)  // works for nontemplate.
    (aList[i]->*Function)();
} 
*/

//		Add an object element to the list.

 void ListOfBoundingBox::addElement(BoundingBox &X){

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

  BoundingBox **aListTmp;

  aListTmp = new BoundingBox* [memAlloc];
  
  for(int i=0; i<listLength; i++){  	// Copy object pointers into new space.
    aListTmp[i] = aList[i];
  }
  
  delete[] aList;			// Delete old list and add the object.
  aList = aListTmp;
  
  aList[listLength++] = &X;

}

 void ListOfBoundingBox::addElement(BoundingBox &X, int index){

  if(listLength >= memAlloc){
  
    if(memAlloc ==0  )			// Like above increase array size
      memAlloc  = 2;			//   if necessary.
    else if(memAlloc < 100)
      memAlloc *= 2;
    else
      memAlloc += memAlloc/10;

    BoundingBox **aListTmp;

    aListTmp = new BoundingBox* [memAlloc];
    
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

 void ListOfBoundingBox::deleteElement(int index){

  checkRange(index);			// Check to make sure index is in the
					//   current range.
  for(int i=index; i<listLength-1; i++)
    aList[i] = aList[i+1];
  
  listLength--;
}

//		Delete an element with the same pointer

 void ListOfBoundingBox::deleteElement(BoundingBox &X){

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

 void ListOfBoundingBox::swapElements(int i, int j){

  checkRange(i);
  checkRange(j);
    
  BoundingBox *tmp;
  
  tmp      = aList[i];
  aList[i] = aList[j];
  aList[j] = tmp;
}

//		Set the list element at i to point to X.
					
 void ListOfBoundingBox::setElementPtr(BoundingBox *X, int index){
  checkRange(index);
  aList[index] = X;
}

//		Get an object element by reference (the cute way)

 BoundingBox& ListOfBoundingBox::operator[](int index) const{
  checkRange(index);  
  return *aList[index];
}
    
//		Get an object element by reference

 BoundingBox& ListOfBoundingBox::getElement(int index) const {
  checkRange(index);  
  return *aList[index];
}

//		Get an object element pointer

 BoundingBox *ListOfBoundingBox::getElementPtr(int index) const{
  
  checkRange(index);
  return aList[index];
}

//		Deallocate the list memory and set length to zero.

 void ListOfBoundingBox::clean(){
  listLength = 0;
}

//		Deallocate the list memory and delete the objects.

 void ListOfBoundingBox::deepClean(){
  for(int i=0; i<listLength; i++)
    delete aList[i];
  
  listLength = 0;
}

//		Internal range check routine

 void ListOfBoundingBox::
checkRange(int index) const{
  if(index < 0 || index > listLength - 1){
    cerr << "ListOfBoundingBox Index Out of Range!" << endl;
    cerr << "  Index Value: " << index << endl;
    cerr << "  Index Range: 0 - " << listLength-1 << endl;
    exit(-1);
  }  
}

