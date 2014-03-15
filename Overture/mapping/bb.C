//
// Test of Bounding Boxes
//

#include "A++.h"
// #include "Dsk.h"
#include "Square.h"     // Define a Square
#include "BoundingBox.h"


void main()
{
  MemoryManagerType memoryManager;  // This will delete allocated memory at the end

  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems

//  Mapping::debug=15;         // set the debug parameter for mappings
//  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking


  cout << "====== Test of the BoundingBox Class =====" << endl;

  BoundingBox boundingBox[2][3];  // root of tree of boxes for each side

  int side=0, axis=0;
  boundingBox[side][axis].addChildren();

  BoundingBox box1,box2,box3,box4;

  boundingBox[side][axis].child1->domainBound=1;
  boundingBox[side][axis].child1->rangeBound=1; 
  boundingBox[side][axis].child2->domainBound=2;
  boundingBox[side][axis].child2->rangeBound=2.;

  BoundingBoxStack boxStack;
  boxStack.push( *boundingBox[side][axis].child1 );
  boxStack.push( *boundingBox[side][axis].child2 );
    
  box1=boxStack.pop(); box1.domainBound.display("Here is box1.domainBound");
  if( boxStack.isEmpty() ) cout << "Stack is empty";
  box2=boxStack.pop(); box2.rangeBound.display("Here is box2.rangeBound");
  if( boxStack.isEmpty() ) cout << "Stack is empty\n";

}
