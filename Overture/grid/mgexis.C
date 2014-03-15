#include "MappedGrid.h"
#include "Annulus.h"

int
main()
{
  AnnulusMapping map;
  MappedGrid g = map;

  printf("gridIndexRange=[%i,%i]x[%i,%i]x[%i,%i]\n",       // Print out the range of indices for grid vertices.
	 g.gridIndexRange(0,0),g.gridIndexRange(1,0),
	 g.gridIndexRange(0,1),g.gridIndexRange(1,1),
	 g.gridIndexRange(0,2),g.gridIndexRange(1,2));

  printf("indexRange=[%i,%i]x[%i,%i]x[%i,%i]\n",           // Print out the range for discretization points.
	 g.indexRange(0,0),g.indexRange(1,0),
	 g.indexRange(0,1),g.indexRange(1,1),
	 g.indexRange(0,2),g.indexRange(1,2));

  printf("numberOfGhostPoints=[%i,%i]x[%i,%i]x[%i,%i]\n",   // Print out the number of ghost points
	 g.numberOfGhostPoints(0,0),g.numberOfGhostPoints(1,0),
	 g.numberOfGhostPoints(0,1),g.numberOfGhostPoints(1,1),
	 g.numberOfGhostPoints(0,2),g.numberOfGhostPoints(1,2));

  printf("dimension=[%i,%i]x[%i,%i]x[%i,%i]\n",            // Print out the grid dimensions 
	 g.dimension(0,0),g.dimension(1,0),
	 g.dimension(0,1),g.dimension(1,1),
	 g.dimension(0,2),g.dimension(1,2));

  int axis;
  for( axis=0; axis<g.numberOfDimensions(); axis++ )
  {
    g.setIsCellCentered(axis,LogicalTrue); // Change the grid to cell-centered
    g.setNumberOfGhostPoints(0,axis, 2);  // Change the number of ghost points on the left side
    g.setNumberOfGhostPoints(1,axis, 2); // Change the number of ghost points on the right side
  }

  g.update(MappedGrid::THEvertex);  // Compute the geometry; 

  return 0;
}
