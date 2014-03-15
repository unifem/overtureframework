#include "Overture.h"
#include "Square.h"

MemoryManagerType memoryManager;  // This will delete A++ allocated memory at the end

void initOvertureGlobalVariables();
void initializeMappingList();   // this allows Mappings to be made with "make"
void destructMappingList();


//================================================================================
//  Test the gridclasses
//
//================================================================================



int main()
{
//  MemoryManagerType memoryManager;  // This will delete allocated memory at the end

  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  Range all;                  // a null Range is used when constructing grid functions, it indicates
                              // the positions of the coordinate axes

  Index I1,I2,I3;
  int axis;

  cout << "initialize mapping list \n";
  initializeMappingList();

  aString nameOfOGFile = "/n/c3servet/henshaw/cgap/cguser/square5.dat";
  cout << "Enter the name of the composite grid file (in the cgsh directory)" << endl;
  cin >> nameOfOGFile;
  if( nameOfOGFile[0]!='.' )
    nameOfOGFile="/n/c3servet/henshaw/res/cgsh/" + nameOfOGFile;
  cout << "\n Create an Overlapping Grid, mount file " << nameOfOGFile<< endl;

  CompositeGrid og;
  getFromADataBase(og,nameOfOGFile);
  og.update();

  CompositeGrid cg;
  cout << "cg.reference(og) \n";

  cg.reference(og);

/* ---
  for( int grid=0; grid<og.numberOfGrids; grid++ )
  {
    cout << " **** grid = " << grid << endl;
    og[grid].vertex.display("Here is og[grid].vertex");
    cg[grid].vertex.display("Here is cg[grid].vertex");
  }
---- */

  SquareMapping square;
  cg[0].vertex.display("Before reference, cg[0].vertex");
  cg[0].reference(square);
  cg[0].vertex.display("After reference, cg[0].vertex");
  cg[0].update();
  cg[0].vertex.display("After update, cg[0].vertex");



  destructMappingList();
  printf ("Program Terminated Normally! \n");
  return 0;
}
