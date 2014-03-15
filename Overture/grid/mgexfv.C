#include "MappedGrid.h"
#include "Annulus.h"
                                  
int
main()
{
  AnnulusMapping map;
  MappedGrid g = map;           // Construct MappedGrid from a  Mapping
  g.changeToAllCellCentered();  // Make this a cell-centered grid. 
  g.update(MappedGrid::THEcorner       |   // Update the discretization cell corners,
	   MappedGrid::THEfaceNormal   |   // the cell face normal vectors,
	   MappedGrid::THEfaceArea     |   // the cell face areas, 
	   MappedGrid::THEcellVolume   |   // the cell volumes,
	   MappedGrid::THEcenterNormal |   // the cell-centered normal vectors,
	   MappedGrid::THEcenterArea   |   // the cell-centered face areas,  
	   MappedGrid::THEcenterBoundaryNormal |  // the unit normals to the grid boundaries, and 
	   MappedGrid::THEcenterBoundaryTangent | // the unit tangents to the grid boundaries.
	   MappedGrid::USEdifferenceApproximation);  // Compute the the cell corners from the mapping,
                                                     // and compute everything else from the corners by
                                                     // finite differences and averaging of corner data.
  RealMappedGridFunction & corner = g.corner(); // Access the discretization points
  corner.display("corner");
  return 0;
}
