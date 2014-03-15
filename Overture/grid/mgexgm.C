#include "GenericGridCollection.h"

void
print(GenericGridCollection & gc)
{
  printf("numberOfGrids=%i, numberOfBaseGrids=%i, numberOfComponentGrids=%i\n"
         "numberOfRefinementLevels=%i, numberOfMultigridLevels=%i\n",
	 gc.numberOfGrids(),gc.numberOfBaseGrids(),gc.numberOfComponentGrids(),
	 gc.numberOfRefinementLevels(),gc.numberOfMultigridLevels());
}

int
main()
{
  GenericGridCollection gc1(2), gc2; 

  int grid,level;
  for( level=1; level<3; level++ )
    for( grid=0; grid<gc1.numberOfBaseGrids(); grid++ )
      gc1.addRefinement(level,grid);   // Add level-one and level-two refinements of each base grid.  

  for( level=1; level<3; level++ )
    for( grid=0; grid<gc1.numberOfComponentGrids(); grid++ ) // Add level-one and level-two multigrid 
      gc1.addMultigridCoarsening(level,grid);                // coarsenings of each component grid. 

  printf("gc1 after adding grids:\n"); print(gc1);

  gc2.referenceRefinementLevels(gc1, 1);  // Share grids with gc1 that have  refinement level at most one. 

  printf("gc2 after reference:\n"); print(gc2);

  gc1.deleteRefinementLevels(1); // Delete refinements with refinement levels higher than one. 
  gc1.deleteMultigridLevels(1);  // Delete coarsenings with multigrid levels higher than one.  

  printf("gc1 after deleting grids:\n"); print(gc1);

  GenericGrid & g1 = gc1[0]; assert(gc1.getIndex(g1) == 0);  // Check that g1 is in gc1 at index zero.   
  GenericGrid g2; assert(gc1.getIndex(g2) < 0);  // Check that g2 is not in gc1.

  return 0;
}

