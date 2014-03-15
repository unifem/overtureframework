#include "Overture.h"

// Test the memory leak
//================================================================================


int main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  Range all;                  // a null Range is used when constructing grid functions, it indicates
                              // the positions of the coordinate axes

  Index I1,I2,I3;

  aString nameOfOGFile = "square5.dat";
  // cout << "Enter the name of the composite grid file (in the cguser directory)" << endl;
  // cin >> nameOfOGFile;
  nameOfOGFile="/usr/snurp/henshaw/cgap/cguser/" + nameOfOGFile;

  aString nameOfDirectory = ".";
  cout << "\n Create an Overlapping Grid, mount file " << nameOfOGFile<< endl;

  MultigridCompositeGrid mgcog(nameOfOGFile,nameOfDirectory);
  CompositeGrid & og=mgcog[0];  // use multigrid level 0
  MappedGrid & mg = og[0];
  

  realCompositeGridFunction q(og,all,all,3),q1,q2;
  q=1.;
  // This next loop makes sure we don't have a memory leak
  q1=q;
  q2=q;

  cout << "Loop to check memory leak... \n";
  for(int iter=0; iter<400; iter++)
  {
    // no leaks with : q=q1;
//    realCompositeGridFunction q4(og,all,all,3);
//    q=q1+q2;

    ListOfFloatMappedGridFunction list;
    list.addElement();
    list[0].updateToMatchGrid(mg);

//    realMappedGridFunction *u = ::new realMappedGridFunction(mg);
//    ::delete u;
  }

  printf ("Program Terminated Normally! \n");
  return 0;
}
