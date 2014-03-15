#include "Overture.h"


int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  Range all;                  // a null Range is used when constructing grid functions, it indicates
                              // the positions of the coordinate axes
  int errorCount=0;

  aString nameOfOGFile;
  nameOfOGFile="/users/henshaw/res/cgsh/square5";

  cout << "\n Create an Overlapping Grid, mount file " << nameOfOGFile<< endl;

  CompositeGrid og;
  getFromADataBase(og,nameOfOGFile);
  og.update();

//  realCompositeGridFunction u15(og,GridFunctionParameters::cellCentered,2), u16(og), u15a, u15b;
  realCompositeGridFunction u15(og,all,all,all,2), u16(og), u15a, u15b;
  u15=0.;
  u15a.link(u15,Range(0,0));
  u15b.link(u15,Range(1,1));
  u16=1.;
  u15a=u16;
//  u15b=2.; // 2.*u16;
  u15b=2.*u16;
  u15b.display("Here is u15b, should be 2");
  u15.display("Here is u15, should be 1,2");
  
  return 0;
}
