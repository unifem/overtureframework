//================================================================================
// Demonstrate how to use grid function that are defined on the edge of grid
//===============================================================================
#include "Overture.h"
#include "CompositeGridOperators.h"


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  cout << "Starting test...\n";
  
  aString nameOfOGFile;
  cout << " >>>>>Testing the Edge GridFunctions " << endl;

  cout << "Enter the name of the composite grid file (in the ogen directory)" << endl;
  cin >> nameOfOGFile;   nameOfOGFile="/users/henshaw/res/cgsh/" + nameOfOGFile;

  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  MappedGrid & mg = cg[0];

  Range all;

   // first define a some local variables so we can shorten the names
  realMappedGridFunction::edgeGridFunctionValues startIndex =realMappedGridFunction::startingGridIndex;
  realMappedGridFunction::edgeGridFunctionValues endIndex   =realMappedGridFunction::endingGridIndex;
    
  //
  // define a grid function that lives on the face: (side,axis)=(0,0)
  //
  Range S(startIndex,startIndex);
  realMappedGridFunction u(mg,S,all,all);
  u=1.;
  u.display("Here is u(mg,S,all,all)");
  u.periodicUpdate();

  //
  // change the grid function to live on the face (side,axis)=(0,1) and include neighbouring grid lines
  //
  u.updateToMatchGrid(mg,all,Range(startIndex-1,startIndex+1));
  u=2.;
  u.display("u.updateToMatchGrid(mg,all,Range(startIndex-1,startIndex+1))");
  u.periodicUpdate();

  //
  // define another grid function to live on the face (side,axis)=(1,0)
  Range E(endIndex,endIndex);
  realMappedGridFunction v(mg,E,all,all);
  v=3.;
  v.display("Here is v(mg,E,all,all)");
  v.periodicUpdate();

  v=3.*v;
  v.display("Here is v=3*v =9?");
  evaluate(3.*v).display(" evaluate(3.*v) =27?");

  //
  // Now make the grid function live on a corner (or edge in 3d)
  //
  v.updateToMatchGrid(mg,S,E,all);
  v=4.;
  v.display("Here is uLeftRight(mg,S,E,all)");
  v.periodicUpdate();

  Overture::finish();          
  return 0;

}  
