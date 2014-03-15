#include "MappedGrid.h"
#include "Annulus.h"
                                  
int
main()
{
  
  AnnulusMapping map;
  MappedGrid g = map;    // Construct MappedGrid from a  Mapping
  g.update(MappedGrid::THEvertex |              // Update the grid vertex coordinates, the                  
     MappedGrid::THEvertexDerivative |          // derivative of the mapping at the vertices,   
     MappedGrid::THEinverseVertexDerivative |   // the inverse derivative at the vertices, the
     MappedGrid::THEvertexJacobian |            // jacobian determinant of the derivative, the  
     MappedGrid::THEvertexBoundaryNormal |      // unit outward normal at boundary vertices,
     MappedGrid::THEboundingBox |               // the bounding box for grid vertices, and the    
     MappedGrid::THEminMaxEdgeLength);          // min.~and max.~distance between vertices.            

  RealMappedGridFunction & vertex = g.vertex(),    // Access the grid vertices and the                      
    & vertexDerivative = g.vertexDerivative();     // mapping derivative at the grid vertices.  
            
  Range d0 = g.numberOfDimensions(),                  // The indices of the vertices corresponding             
    I1(g.gridIndexRange(0,0),g.gridIndexRange(1,0)),  // to interior and boundaries of the grid are            
    I2(g.gridIndexRange(0,1),g.gridIndexRange(1,1)),  // given by g.gridIndexRange.  The      
    I3(g.gridIndexRange(0,2),g.gridIndexRange(1,2));  // fourth index selects the (x,y,z) component.           

  RealArray v;
  v = vertex(I1,I2,I3,d0);                  // Copy the grid vertices
  v.display("vertices");
  
  return 0;
}

