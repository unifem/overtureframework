#include "Overture.h"
  
int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  printf(" ------------------------------------------------------------ \n");
  printf(" Read an overlapping grid from a data base file               \n");
  printf(" Loop over the component grids and display the boundary       \n");
  printf("   conditions and the grid points (vertex array)              \n");
  printf(" ------------------------------------------------------------ \n");

  aString nameOfOGFile;
  cout << "Enter the name of the overlapping grid data base file " << endl;
  cin >> nameOfOGFile;
  
  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )   // loop over component grids
  {
    MappedGrid & c = cg[grid];
    RealArray & x = c.vertex();
    
    // c.gridIndexRange(0:1,0:2)  
    getIndex(c.gridIndexRange(),I1,I2,I3);
    // x = x(I1,I2,I3,axis1)  
   
    // c.isPeriodic(axis) 

    // nI = cg.numberOfInterpolationPoints(grid)
    // cg.interpolationPoint(i,0:2)  i=0,...,nI-1 : index coordinates of the interp.
    // cg.interpoleeGrid(i)          i=0,...,nI-1
    // cg.interpolationCoordinates(i,0:2)         : unit square coordinates 
    // cg.interpolationLocation(i,0:3) : index coord's of the lower left point in the interpolation stencil 

    cg[grid].boundaryCondition().display("Here are the boundary conditions");
    cg[grid].vertex().display("Here are the vertex coordinates");

    

    // A Composite grid is a list of MappedGrid's. To save typing we can 
    // make a reference (alias):       
    MappedGrid & mg = cg[grid];                                       // make a reference to the MappedGrid
    mg.boundaryCondition().display("Here is boundaryCondition again");  // same result as above
  }    

  return 0;
}
