#include "Overture.h"

int main()
{
  ios::sync_with_stdio();  
  Index::setBoundsCheck(on);

  aString nameOfOGFile;
  cout << "bugtest>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;
  
  // create and read in a CompositeGrid
  int grid,side,axis,comp;
  Range all;
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();
  cg.update(GridCollection::THEcenter|
            GridCollection::THEcenterDerivative|
            GridCollection::THEinverseCenterDerivative|
            GridCollection::THEcenterBoundaryNormal|
            GridCollection::THEcenterJacobian|
            GridCollection::THEinverseVertexDerivative|
            GridCollection::THEminMaxEdgeLength);
  
  for(grid=0;grid<cg.numberOfComponentGrids();grid++){
    
    realArray minEdge = cg[grid].minimumEdgeLength();
    realArray maxEdge = cg[grid].maximumEdgeLength();

    minEdge.display("min edge length");
    maxEdge.display("max edge length");
  }
  return 1;
}
