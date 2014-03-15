#include "Overture.h"  
#include "Square.h"

int 
main(int argc, char** argv)
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  SquareMapping square(0.,1.,0.,1.);                   // Make a mapping, unit square
  square.setGridDimensions(axis1,11);                  // axis1==0, set no. of grid points
  square.setGridDimensions(axis2,11);                  // axis2==1, set no. of grid points
  MappedGrid mg(square);                               // MappedGrid for a square
  mg.update();                                         // create default variables

  GridCollection gc(mg.numberOfDimensions,1);  // make a GridCollection with 1 component grid
  gc[0].reference(mg);

  gc.updateReferences();
  
  gc.update();

  return 0;
}

