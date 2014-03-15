//kkc 081124 #include <iostream.h>
#include <iostream>

#include <stdlib.h>
#include "HDF_DataBase.h"
#include "ListOfHDF_DataBaseRCData.h"

using namespace std;

//		Default constructor

 ListOfHDF_DataBaseRCData::ListOfHDF_DataBaseRCData(){

#if DEBUGTEMPLATE
  theIDCount++;
  theID = theIDCount;
  cout << "ListOfHDF_DataBaseRCData default constructor called.  ";
  cout << "ListOfHDF_DataBaseRCData #" << theID << " created." << endl;
#endif
  
  listLength = 0;
  aList    = 0;
  memAlloc = 0;
}

//		Copy constructor

 ListOfHDF_DataBaseRCData::ListOfHDF_DataBaseRCData(const ListOfHDF_DataBaseRCData &X){

#if DEBUGTEMPLATE			
  theIDCount++;				// Assign a unique ID from the static counter.
  theID =theIDCount;
  cout << "ListOfHDF_DataBaseRCData copy constructor called.     ";
  cout << "ID " << X.theID <<" copied to " << theID << "." << endl;
#endif

  listLength = X.listLength;
  
  if(listLength < 1){			// Determine if any allocation is
    aList    = 0;			//   necessary.
    memAlloc = 0;
    return;
  }
  
  aList = new (HDF_DataBaseRCData* [listLength]);		// Allocate pointer space.
  memAlloc = listLength;
  
  for(int i=0; i<listLength; i++)		// Copy pointers.
    aList[i] = X.aList[i];
}

//		Destructor

 ListOfHDF_DataBaseRCData::~ListOfHDF_DataBaseRCData(){

#if DEBUGTEMPLATE
  cout << "ListOfHDF_DataBaseRCData destructor called.           ";
  cout << "ListOfHDF_DataBaseRCData #" << theID << " destroyed." << endl;
#endif
  
  delete[] aList;				// Delete the list of pointers (only).
}

//		Class equal operator

 ListOfHDF_DataBaseRCData& ListOfHDF_DataBaseRCData::operator=(const ListOfHDF_DataBaseRCData &X){

#if DEBUGTEMPLATE
  cout << "ListOfHDF_DataBaseRCData = operator called.           ";
  cout << "ID " << X.theID <<" copied to " << theID << "." << endl;
#endif

  listLength = X.listLength;		// Like the copy constructor
  delete[] aList;			//   allocate and copy pointer list.
  
  if(listLength < 1){
    aList    = 0;
    memAlloc = 0;
    return *this;
  }
  
  aList = new (HDF_DataBaseRCData* [listLength]);
  memAlloc = listLength;
  
  for(int i=0; i<listLength; i++)
    aList[i] = X.aList[i];

  return *this;
}

//		Function Iterator

/*
 void ListOfHDF_DataBaseRCData::Iterator(void (HDF_DataBaseRCData::*Function)()){

  for(int i=0; i<listLength; i++)  // works for nontemplate.
    (aList[i]->*Function)();
} 
*/

//		Add an object element to the list.

 void ListOfHDF_DataBaseRCData::addElement(HDF_DataBaseRCData &X){

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

  HDF_DataBaseRCData **aListTmp;

  aListTmp = new (HDF_DataBaseRCData* [memAlloc]);
  
  for(int i=0; i<listLength; i++){  	// Copy object pointers into new space.
    aListTmp[i] = aList[i];
  }
  
  delete[] aList;			// Delete old list and add the object.
  aList = aListTmp;
  
  aList[listLength++] = &X;

}

 void ListOfHDF_DataBaseRCData::addElement(HDF_DataBaseRCData &X, int index){

  if(listLength >= memAlloc){
  
    if(memAlloc ==0  )			// Like above increase array size
      memAlloc  = 2;			//   if necessary.
    else if(memAlloc < 100)
      memAlloc *= 2;
    else
      memAlloc += memAlloc/10;

    HDF_DataBaseRCData **aListTmp;

    aListTmp = new (HDF_DataBaseRCData* [memAlloc]);
    
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

 void ListOfHDF_DataBaseRCData::deleteElement(int index){

  checkRange(index);			// Check to make sure index is in the
					//   current range.
  for(int i=index; i<listLength-1; i++)
    aList[i] = aList[i+1];
  
  listLength--;
}

//		Delete an element with the same pointer

 void ListOfHDF_DataBaseRCData::deleteElement(HDF_DataBaseRCData &X){

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

 void ListOfHDF_DataBaseRCData::swapElements(int i, int j){

  checkRange(i);
  checkRange(j);
    
  HDF_DataBaseRCData *tmp;
  
  tmp      = aList[i];
  aList[i] = aList[j];
  aList[j] = tmp;
}

//		Set the list element at i to point to X.
					
 void ListOfHDF_DataBaseRCData::setElementPtr(HDF_DataBaseRCData *X, int index){
  checkRange(index);
  aList[index] = X;
}

//		Get an object element by reference (the cute way)

 HDF_DataBaseRCData& ListOfHDF_DataBaseRCData::operator[](int index) const{
  checkRange(index);  
  return *aList[index];
}
    
//		Get an object element by reference

 HDF_DataBaseRCData& ListOfHDF_DataBaseRCData::getElement(int index) const {
  checkRange(index);  
  return *aList[index];
}

//		Get an object element pointer

 HDF_DataBaseRCData *ListOfHDF_DataBaseRCData::getElementPtr(int index) const{
  
  checkRange(index);
  return aList[index];
}

//		Deallocate the list memory and set length to zero.

 void ListOfHDF_DataBaseRCData::clean(){
  listLength = 0;
}

//		Deallocate the list memory and delete the objects.

 void ListOfHDF_DataBaseRCData::deepClean(){
  for(int i=0; i<listLength; i++)
    delete aList[i];
  
  listLength = 0;
}

//		Internal range check routine

 void ListOfHDF_DataBaseRCData::
checkRange(int index) const{
  if(index < 0 || index > listLength - 1){
    cerr << "ListOfHDF_DataBaseRCData Index Out of Range!" << endl;
    cerr << "  Index Value: " << index << endl;
    cerr << "  Index Range: 0 - " << listLength-1 << endl;
    exit(-1);
  }  
}

