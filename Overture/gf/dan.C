#include "Overture.h"
#include "LineMapping.h"

int main(int argc, char** argv )
   {
     ios::sync_with_stdio();     // Syncs C++ and C I/O subsystems!
     Index::setBoundsCheck(On);

  // ******************************
  // * Initialize Virtual Machine *
  // ******************************
     int numberOfProcessors = 1;
     Optimization_Manager::Initialize_Virtual_Machine ("", numberOfProcessors, argc, argv);

  // Create a mapping  (no data on a mapping yet)
#if 1
     LineMapping square(0.0, 1.0);
     square.setGridDimensions(axis1,10);
#else
     SquareMapping square(0.0, 1.0, 0.0, 1.0);
     square.setGridDimensions(axis1,10);
     square.setGridDimensions(axis2,10);
#endif
     Mapping::staticMapList().add(&square);

  // Create a two-dimensional GridCollection with one square grid.

  // Build a grid from the mapping just built above
     MappedGrid mapped_grid(square);
     mapped_grid.update();

     cout << "In MAIN: mapped_grid = " << mapped_grid << endl;

     assert (mapped_grid.isAllCellCentered() == FALSE);
     assert (mapped_grid.isAllVertexCentered());

     floatMappedGridFunction u(mapped_grid);

     printf ("In MAIN: u.getIsFaceCentered(0) = %i \n",u.getIsFaceCentered(0));

     u     = 1.0;

     printf ("Program Terminated Normally! \n");
     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Virtual Machine exited! \n");
   }


