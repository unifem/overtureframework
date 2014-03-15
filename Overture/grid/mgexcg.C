#include "MappedGrid.h"
#include "Annulus.h"
                                  
int
main()
{
  AnnulusMapping map;
  MappedGrid g = map;    // Construct MappedGrid from a  Mapping
  g.update(MappedGrid::THEcenter |              // Update the discretization points, the                  
     MappedGrid::THEcenterDerivative |          // derivative of the mapping,   
     MappedGrid::THEinverseCenterDerivative |   // the inverse derivative, and the 
     MappedGrid::THEcenterJacobian );           // jacobian determinant of the derivative

  RealMappedGridFunction & center = g.center(); // Access the discretization points
  center.display("center");
  return 0;
}
