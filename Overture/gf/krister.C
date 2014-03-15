#include "Overture.h"  
#include "CompositeGridOperators.h"

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  aString nameOfOGFile="square5";
  if( nameOfOGFile[0]!='.' )
    nameOfOGFile="/home/henshaw/res/ogen/" + nameOfOGFile;
  aString nameOfDirectory = ".";
  cout << "\n Create an Overlapping Grid, mount file " << nameOfOGFile<< endl;
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  CompositeGridOperators op(cg);                            // create some differential operators

  realCompositeGridFunction u(cg),ux; // (cg);
  u.setOperators(op);

  u=1.;
  ux.destroy();
  
  Range all;
  // ux.updateToMatchGridFunction(u,all,all,all,all);
    
  const real ad=2.;
//  ux=u.x();  // ok
  ux=u.x()+u.y(); 
  ux=u.x()*ad; 

  ux=ad*u.x(); 

}

