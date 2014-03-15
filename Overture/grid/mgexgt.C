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
  GenericGridCollection gc(2); // GenericGridCollectionwith 2 base grids

  printf("Initial:\n"); print(gc);
  
  int grid;
  for( grid=0; grid<gc.numberOfBaseGrids(); grid++ )
    gc.addRefinement(1,grid);
  
  printf("After add refinement:\n"); print(gc);
  
  int level;
  for( level=1; level<3; level++ )
    for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
      gc.addMultigridCoarsening(level,grid);
  
  printf("After add multigrid coarsenings:\n"); print(gc);

  
  gc.update(GenericGridCollection::THEbaseGrid); // Partition g according to base grid number. 
  GenericGridCollection & gc1=gc.baseGrid[1];    // Access base grid one and its refinements.

  gc1.update(GenericGridCollection::THEmultigridLevel); // Partition g1 according to multigrid level.

  GenericGridCollection & gc12=gc1.multigridLevel[2]; // Access base grid one and its refinements 
                                                      // at multigrid level two. 

  return 0;
}

