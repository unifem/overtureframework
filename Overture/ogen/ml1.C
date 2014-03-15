//
// Code to test for memory leaks -- version 1
//
// 960215:
//  no deep copy of the CG:
//   MIU = 3276 bytes (all from sync io)
//   MLK = 0
// 960215:
//  with deep copy:
//   0bytes MIU, MLK with system stuff turned off in .purify
#include "Cgsh.h"
// include "Square.h"
// include "PlotStuff.h"
// include "mogl.h"
// include "MatrixTransform.h"
// include "OGPolyFunction.h"
// include "Oges.h"
// include "Ogshow.h"

// MemoryManagerType memoryManager;  // This will delete A++ allocated memory at the end

void initializeMappingList();   // this allows Mappings to be made with "make"
void destructMappingList();
    

int 
main() 
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  aString nameOfOGFile, nameOfDirectory=".";
  
  cout << "Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  cout << "Enter Mapping::debug\n" ;
  cin >> Mapping::debug;   
  // This next call will allow the Mappings to be read in from the data-base file
  initializeMappingList();

  cout << "Create a CompositeGrid..." << endl;
  MultigridCompositeGrid m0(nameOfOGFile,nameOfDirectory);  // keep a copy of the original grid
  CompositeGrid & cg0 = m0[0];

  cg0.update(); // m0.update

  CompositeGrid cg2;
  cg2=cg0;

  destructMappingList();
  cout << "Done! ...\n";
  return 0;
}
