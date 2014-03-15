#ifndef LISTOFROTATEDBOX_H
#define LISTOFROTATEDBOX_H "ListOfRotatedBox.h"
class RotatedBox;
//
//			ListOfRotatedBox Class Template
//
//  This is the header file for a template class that stores pointers
//  of objects within an array.  The array is dynamic so that only 
//  memory and time limit the number of objects stored on the list.

//
//  Set this parameter to 1 if you find it necessary to track the lists
//  being created and destroyed.
//
#define DEBUGTEMPLATE 0


class ListOfRotatedBox{
  private:

#if DEBUGTEMPLATE
    int theID;		// Unique id for each list object created.
#endif			// the counter static int theIDCount is used
			// to assign a value.

    int listLength;	// Number of elements in the list.
    int memAlloc;	// Current amount of memory allocated for the list.
    RotatedBox **aList;		// Pointer to a list of pointers to object RotatedBox.
    
    void checkRange(int) const;	// Internal range check function.
    
  public:

#if DEBUGTEMPLATE		// Should be initialized for debugging.
    static int theIDCount;	// example:
#endif				//   int ListOfRotatedBox<stuff>::theIDCount = 0;

				// Constructors/Destructors
    ListOfRotatedBox();			//   Default constructor.
    ListOfRotatedBox(const ListOfRotatedBox&);		//   Copy constructor.
   ~ListOfRotatedBox();			//   Destructor.
    
    ListOfRotatedBox& operator=(const ListOfRotatedBox&);	// Equal operator (only pointers of
					// list objects copied).

//  void Iterator( void (RotatedBox::*Function)());  // Function iterator.


			// ListOfRotatedBox Management Functions

    void addElement(RotatedBox &X);		// Add an object to the list.

    void addElement(RotatedBox &X, int index);	// Add an object to the list at a
					//   given location.
    int getLength() const {return listLength;};// Get length of list.

    RotatedBox* getElementPtr(int index) const;	// Get the pointer of the object
					//   at a given location.
    RotatedBox& getElement(int index) const;		// Reference the object at a
    					//   a given location.
    RotatedBox& operator[](int index) const;		// Reference the object at a
    					//   given location.
    void deleteElement(RotatedBox &X);		// Find an element on the list
					//   and delete it.
    void deleteElement(int index);      // Delete the object at a given
					//   location in the list.
    void setElementPtr(RotatedBox *X, int i);    // Set the list element at i
    					//   to point to X.
    void swapElements(int i, int j);    // Swap two elements for sorting
    					//   among other things.
    void clean();			// Deallocate pointer list but
					//   not objects in the list.
    void deepClean();			// Deallocate pointer list and
					//   the pointers on the list.
};

#endif
