#include "MappedGrid.h"
#include "Annulus.h"

int
main()
{
  AnnulusMapping map;
  MappedGrid g = map;

  printf("isPeriodic=[%i,%i,%i]\n",       // Print out the periodicity of each grid coordinate
	 g.isPeriodic(axis1),g.isPeriodic(axis2),g.isPeriodic(axis3));

  printf("boundaryCondition=[%i,%i]x[%i,%i]x[%i,%i]\n",    // Print out the boundary condition for each side.
	 g.boundaryCondition(0,0),g.boundaryCondition(1,0),
	 g.boundaryCondition(0,1),g.boundaryCondition(1,1),
	 g.boundaryCondition(0,2),g.boundaryCondition(1,2));

  printf("numberOfGhostPoints=[%i,%i]x[%i,%i]x[%i,%i]\n",     // Print out the shared boundary flag.
	 g.sharedBoundaryFlag(0,0),g.sharedBoundaryFlag(1,0),
	 g.sharedBoundaryFlag(0,1),g.sharedBoundaryFlag(1,1),
	 g.sharedBoundaryFlag(0,2),g.sharedBoundaryFlag(1,2));

  printf("sharedBoundaryTolerance=[%f,%f]x[%f,%f]x[%f,%f]\n",  // Print out the shared boundary tolerance
	 g.sharedBoundaryTolerance(0,0),g.sharedBoundaryTolerance(1,0),
	 g.sharedBoundaryTolerance(0,1),g.sharedBoundaryTolerance(1,1),
	 g.sharedBoundaryTolerance(0,2),g.sharedBoundaryTolerance(1,2));

  g.setIsPeriodic(axis1,Mapping::notPeriodic);  // Make the grid not periodic
  g.setBoundaryCondition(0,axis1, 2);           // assign boundary condition values.
  g.setBoundaryCondition(1,axis1, 3); 

  g.update(MappedGrid::THEvertex);
  return 0;
}
