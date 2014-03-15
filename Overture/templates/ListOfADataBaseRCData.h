#ifndef LISTOFADATABASERCDATA_H
#define LISTOFADATABASERCDATA_H "ListOfADataBaseRCData.h"
class ADataBaseRCData;
//
//			ListOfADataBaseRCData Class Template
//
//  This is the header file for a template class that stores pointers
//  of objects within an array.  The array is dynamic so that only 
//  memory and time limit the number of objects stored on the list.

//
//  Set this parameter to 1 if you find it necessary to track the lists
//  being created and destroyed.
//
#define DEBUGTEMPLATE 0


class ListOfADataBaseRCData{
  private:

#if DEBUGTEMPLATE
    int theID;		// Unique id for each list object created.
#endif			// the counter static int theIDCount is used
			// to assign a value.

    int listLength;	// Number of elements in the list.
    int memAlloc;	// Current amount of memory allocated for the list.
    ADataBaseRCData **aList;		// Pointer to a list of pointers to object ADataBaseRCData.
    
    void checkRange(int) const;	// Internal range check function.
    
  public:

#if DEBUGTEMPLATE		// Should be initialized for debugging.
    static int theIDCount;	// example:
#endif				//   int ListOfADataBaseRCData<stuff>::theIDCount = 0;

				// Constructors/Destructors
    ListOfADataBaseRCData();			//   Default constructor.
    ListOfADataBaseRCData(const ListOfADataBaseRCData&);		//   Copy constructor.
   ~ListOfADataBaseRCData();			//   Destructor.
    
    ListOfADataBaseRCData& operator=(const ListOfADataBaseRCData&);	// Equal operator (only pointers of
					// list objects copied).

//  void Iterator( void (ADataBaseRCData::*Function)());  // Function iterator.


			// ListOfADataBaseRCData Management Functions

    void addElement(ADataBaseRCData &X);		// Add an object to the list.

    void addElement(ADataBaseRCData &X, int index);	// Add an object to the list at a
					//   given location.
    int getLength() const {return listLength;};// Get length of list.

    ADataBaseRCData* getElementPtr(int index) const;	// Get the pointer of the object
					//   at a given location.
    ADataBaseRCData& getElement(int index) const;		// Reference the object at a
    					//   a given location.
    ADataBaseRCData& operator[](int index) const;		// Reference the object at a
    					//   given location.
    void deleteElement(ADataBaseRCData &X);		// Find an element on the list
					//   and delete it.
    void deleteElement(int index);      // Delete the object at a given
					//   location in the list.
    void setElementPtr(ADataBaseRCData *X, int i);    // Set the list element at i
    					//   to point to X.
    void swapElements(int i, int j);    // Swap two elements for sorting
    					//   among other things.
    void clean();			// Deallocate pointer list but
					//   not objects in the list.
    void deepClean();			// Deallocate pointer list and
					//   the pointers on the list.
};

#endif
