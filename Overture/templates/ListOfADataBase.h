#ifndef LISTOFADATABASE_H
#define LISTOFADATABASE_H "ListOfADataBase.h"
class ADataBase;
//
//			ListOfADataBase Class Template
//
//  This is the header file for a template class that stores pointers
//  of objects within an array.  The array is dynamic so that only 
//  memory and time limit the number of objects stored on the list.

//
//  Set this parameter to 1 if you find it necessary to track the lists
//  being created and destroyed.
//
#define DEBUGTEMPLATE 0


class ListOfADataBase{
  private:

#if DEBUGTEMPLATE
    int theID;		// Unique id for each list object created.
#endif			// the counter static int theIDCount is used
			// to assign a value.

    int listLength;	// Number of elements in the list.
    int memAlloc;	// Current amount of memory allocated for the list.
    ADataBase **aList;		// Pointer to a list of pointers to object ADataBase.
    
    void checkRange(int) const;	// Internal range check function.
    
  public:

#if DEBUGTEMPLATE		// Should be initialized for debugging.
    static int theIDCount;	// example:
#endif				//   int ListOfADataBase<stuff>::theIDCount = 0;

				// Constructors/Destructors
    ListOfADataBase();			//   Default constructor.
    ListOfADataBase(const ListOfADataBase&);		//   Copy constructor.
   ~ListOfADataBase();			//   Destructor.
    
    ListOfADataBase& operator=(const ListOfADataBase&);	// Equal operator (only pointers of
					// list objects copied).

//  void Iterator( void (ADataBase::*Function)());  // Function iterator.


			// ListOfADataBase Management Functions

    void addElement(ADataBase &X);		// Add an object to the list.

    void addElement(ADataBase &X, int index);	// Add an object to the list at a
					//   given location.
    int getLength() const {return listLength;};// Get length of list.

    ADataBase* getElementPtr(int index) const;	// Get the pointer of the object
					//   at a given location.
    ADataBase& getElement(int index) const;		// Reference the object at a
    					//   a given location.
    ADataBase& operator[](int index) const;		// Reference the object at a
    					//   given location.
    void deleteElement(ADataBase &X);		// Find an element on the list
					//   and delete it.
    void deleteElement(int index);      // Delete the object at a given
					//   location in the list.
    void setElementPtr(ADataBase *X, int i);    // Set the list element at i
    					//   to point to X.
    void swapElements(int i, int j);    // Swap two elements for sorting
    					//   among other things.
    void clean();			// Deallocate pointer list but
					//   not objects in the list.
    void deepClean();			// Deallocate pointer list and
					//   the pointers on the list.
};

#endif
