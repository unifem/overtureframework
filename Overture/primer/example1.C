#include "Overture.h"
  
int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

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
    cg[grid].boundaryCondition().display("Here are the boundary conditions");
    cg[grid].vertex().display("Here are the vertex coordinates");

    // A Composite grid is a list of MappedGrid's. To save typing we can 
    // make a reference (alias):       
    MappedGrid & mg = cg[grid];                                       // make a reference to the MappedGrid
    mg.boundaryCondition().display("Here is boundaryCondition again");  // same result as above

    
    realArray & ci = cg.interpolationCoordinates[grid];
    intArray & il = cg.interpoleeLocation[grid];
    intArray & donor = cg.interpoleeGrid[grid];
    
    for( int i=0; i<cg.numberOfInterpolationPoints(grid); i++  )
    {
      printF("%i %i %26.18e %26.18e %i %i %i\n",grid,i,ci(i,0),ci(i,1),il(i,0),il(i,1),donor(i));
    }
    
  }    

  Overture::finish();          
  return 0;
}

