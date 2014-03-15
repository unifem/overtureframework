#include "MappedGrid.h"
#include "Annulus.h"

int
main()
{
  AnnulusMapping map;
  MappedGrid g = map;

  printf("discretizationWidth=[%i,%i,%i]\n",       // Print out the interior discretization width
	 g.discretizationWidth(axis1),g.discretizationWidth(axis2),g.discretizationWidth(axis3));


  printf("boundaryDiscretizationWidth=[%i,%i]x[%i,%i]x[%i,%i]\n", // Print boundary discretization width
	 g.boundaryDiscretizationWidth(0,0),g.boundaryDiscretizationWidth(1,0),
	 g.boundaryDiscretizationWidth(0,1),g.boundaryDiscretizationWidth(1,1),
	 g.boundaryDiscretizationWidth(0,2),g.boundaryDiscretizationWidth(1,2));

  printf("gridSpacing=[%f,%f,%f]\n",  // Print out the unit square spacing in each direction
	 g.gridSpacing(axis1),g.gridSpacing(axis2),g.gridSpacing(axis3));

  printf("isCellCentered=[%i,%i,%i]\n",       // Print te cell-centering in each direction.
	 g.isCellCentered(axis1),g.isCellCentered(axis2),g.isCellCentered(axis3));


  printf("isAllCellCentered=%i\n",g.isAllCellCentered());     // Print out the cell-centering of the grid
  printf("isAllVertexCentered=%i\n",g.isAllVertexCentered()); 

  int axis;
  for( axis=0; axis<g.numberOfDimensions(); axis++ )
    g.setIsCellCentered(axis,LogicalTrue); // Change the grid to cell-centered

  g.update(MappedGrid::THEvertex);
  return 0;
}
