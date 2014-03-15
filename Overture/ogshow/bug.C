//===============================================================================
//  Test the Overlapping Grid Show file class Ogshow
//==============================================================================
#include "Overture.h"
#include "HDF_DataBase.h"  

int
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  aString nameOfOGFile(80), nameOfShowFile(80), nameOfDirectory(80);
  
  cout << "togshow>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  nameOfShowFile="bug.hdf";

  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);          // read from a data base file
  cg.update();
//  cg.update(CompositeGrid::THEvertexBoundaryNormal | CompositeGrid::THEinverseVertexDerivative ); // seg fault
//  cg.update(CompositeGrid::THEvertexBoundaryNormal );   // ok

  cg.update(CompositeGrid::THEinverseVertexDerivative );  // seg fault
  // realMappedGridFunction & rx = cg[0].inverseVertexDerivative();
  // rx.display("Here is the inverseVertexDeriavtive");

//  cg.update(CompositeGrid::THEcenterDerivative );         // ok
//  cg.update(CompositeGrid::THEinverseCenterDerivative );         // seg fault

//  cg.update(CompositeGrid::THEinverseCenterDerivative );      // seg fault 
//  cg.destroy(CompositeGrid::THEinverseCenterDerivative);
    
//  cg.update(CompositeGrid::THEvertexJacobian );                  // seg fault


  HDF_DataBase dataFile;
  dataFile.mount(nameOfShowFile,"I");
  cout << "save the cg in a file...\n";
    
  cg.put(dataFile,"cg");
  dataFile.unmount();

  return 0;
}
