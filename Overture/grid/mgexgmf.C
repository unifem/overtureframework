#include "Square.h"
#include "GridCollection.h"

int
main()
{
  GridCollection g(2,1);                          // Start with one two-dimensional grid. 
  // GridCollection g; g.initialize(2,1);         // This would have the same effect. 
  int base = 0, ratio = 2, level = 1, refinement; // The base grid is grid zero. 

  SquareMapping square(0.,1.,0.,1.);         // A Mapping for the square [0,1]x[0,1]. 
  g[base].reference(square);                 // Make the base grid use the square mapping. 
  g.update(GridCollection::THEboundingBox);  // Update and print out the bounding box

  printf("boundingBox=[%f,%f]x[%f,%f]x[%f,%f]\n",  
	 g.boundingBox(0,0),g.boundingBox(1,0),
	 g.boundingBox(0,1),g.boundingBox(1,1),
	 g.boundingBox(0,2),g.boundingBox(1,2));

  g.changeToAllVertexCentered();                        // Make the base grid vertex-centered. 
  IntegerArray range = g[base].indexRange();                 // Find the index range of discretization points. 
  range(0,0) = range(0,1) = 1; range(1,0) = range(1,1) = 3;  // Refine the index range [1:3,1:3]. 
  refinement = g.addRefinement(range, ratio, level, base);   // Add a level-one refinement grid.     
  g.deleteRefinement(refinement);                            // Delete the refinement grid.            

  g.changeToAllCellCentered();                   // Make the base grid cell-centered. 
  range = g[base].indexRange();                  // Find the index range of discretization points. 
  range(0,0) = range(0,1) = 1; range(1,0) = range(1,1) = 2; // Refine the index range [1:2,1:2]. 
  refinement = g.addRefinement(range, ratio, level, base);  // Add a level-one refinement grid.     
  g.deleteRefinement(refinement);                           // Delete the refinement grid.            

  int component=0,coarsening;
  coarsening = g.addMultigridCoarsening(ratio, level, component); // Add a level-one multigrid coarsening.  
  g.deleteMultigridCoarsening(coarsening);   // Delete the multigrid coarsening.

  int grid;
  for( grid=0; grid<g.numberOfGrids(); grid++ )
  {
    printf("refinementFactor=[%i,%i,%i]\n",                // Print the refinement factors 
	   g.refinementFactor(axis1),g.refinementFactor(axis2),g.refinementFactor(axis3));
    printf("multigridCoarseningFactor=[%i,%i,%i]\n",       // Print the multigrid coarsening factors 
	   g.multigridCoarseningFactor(axis1),g.multigridCoarseningFactor(axis2),
           g.multigridCoarseningFactor(axis3));
    printf("refinementFactor=[%i,%i,%i]\n",                // Print the multigrid refinement factors 
	   g.refinementFactor(axis1),g.refinementFactor(axis2),g.refinementFactor(axis3));
    	   
  }
  return 0;
}
