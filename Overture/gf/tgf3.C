#include "Overture.h"
#include "Square.h"

//================================================================================
//  Test the gridFunction classes
//
//================================================================================



int 
main()
{

  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  Range all;                  // a null Range is used when constructing grid functions, it indicates
                              // the positions of the coordinate axes
  

  SquareMapping square(0.,1.,0.,1.);                   // Make a mapping, unit square
  square.setGridDimensions(axis1,5);                  // axis1==0, set no. of grid points
  square.setGridDimensions(axis2,5);                  // axis2==1, set no. of grid points
  MappedGrid mg(square);                               // MappedGrid for a square
  mg.update();


  CompositeGrid cg;

  cg.setNumberOfDimensionsAndGrids(mg.numberOfDimensions(), 1);
  cout << "cg.numberOfGrids = " << cg.numberOfGrids() << endl;
  cout << "cg.numberOfComponentGrids = " << cg.numberOfComponentGrids() << endl;
  
  cg[0].reference(mg);
  cg.updateReferences();
  cg.update();
  cout << "cg.numberOfComponentGrids = " << cg.numberOfComponentGrids() << endl;

  cg.numberOfComponentGrids()=1;
  
  cg.update(MappedGrid::THEinverseVertexDerivative);
  cg[0].inverseVertexDerivative.display("here is inverseVertexDerivative");

  cg.update(MappedGrid::THEinverseCenterDerivative);
  cg[0].inverseCenterDerivative.display("here is inverseCenterDerivative");

  cg.update(MappedGrid::THEcenterBoundaryTangent);
  cg[0].centerBoundaryTangent[0][0].display("Here is the centerBoundaryTangent");

  return 0;
}
