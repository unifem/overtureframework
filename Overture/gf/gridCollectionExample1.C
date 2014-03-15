#include "PlotStuff.h"
#include "Square.h"

void
showGrid(GridCollection & gc, PlotStuff & ps, const aString & title )
{
  cout << title << endl;
  printf("  numberOfComponentGrids   = %i \n"
         "  numberOfBaseGrids        = %i \n"
         "  numberOfGrids            = %i \n"
         "  numberOfRefinementLevels = %i \n"
         "  numberOfMultigridLevels  = %i \n",
         gc.numberOfComponentGrids(),gc.numberOfBaseGrids(),gc.numberOfGrids(),gc.numberOfRefinementLevels(),
         gc.numberOfMultigridLevels());
  PlotStuffParameters psp;
  psp.set(GI_TOP_LABEL,title);  // set title
  PlotIt::plot(ps,gc,psp);                       // plot the grid
}


int 
main(int argc, char* argv[]) 
{
  ios::sync_with_stdio();
  Index::setBoundsCheck(On);

  printf(" -------------------------------------------------------------------------- \n");
  printf(" Demonstrate the properties of a GridCollection                             \n");
  printf(" -------------------------------------------------------------------------- \n");

  bool plot=TRUE;
  if( argc>1 )
  {
    aString arg = argv[1];
    if( arg=="noplot" )
      plot=FALSE;
  }

  PlotStuff ps(plot,"gridCollectionExample1");               // for plotting
  char buff[100];

  const int numberOfBaseGrids=3;
  Mapping *map[numberOfBaseGrids];

  map[0] = new SquareMapping(-2.25,-.25, -2.25,-.25);         // Create a SquareMapping
  map[1] = new SquareMapping(-1.00, 1.0, -1., 1.);        
  map[2] = new SquareMapping(  .25,2.25,   .25,2.25);        


  MappedGrid *mg[numberOfBaseGrids];
  int grid;
  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    mg[grid] = new MappedGrid(*map[grid]);
    mg[grid]->update();
  }
      
  const int numberOfDimensions= mg[0]->numberOfDimensions();

  //  Create a two-dimensional GridCollection
  GridCollection gc(numberOfDimensions,numberOfBaseGrids);
  for( grid=0; grid<numberOfBaseGrids; grid++ )
    gc[grid].reference(*mg[grid]);   
  gc.updateReferences();

  showGrid(gc,ps,"Initial grid");

  gc.update(GridCollection::THErefinementLevel);  // indicate that we are want a refinement level

  // Add a refinement, specify position in the coarse grid index space
  IntegerArray range(2,3), factor(3);
  range(0,0) = 2; range(1,0) = 6;
  range(0,1) = 2; range(1,1) = 6;
  range(0,2) = 0; range(1,2) =  0;
  
  factor = 2;                            // refinement factor 
  Integer level = 1;
  grid = 0;                              // refine this base grid
  gc.addRefinement(range, factor, level, grid);    // add a refinement grid to level 1

  range(0,0) = 4; range(1,0) = 8;
  range(0,1) = 4; range(1,1) = 8;
  range(0,2) = 0; range(1,2) =  0;
  grid=2;
  gc.addRefinement(range, factor, level, grid);   // add another refinement grid to level 1

  gc.update(GridCollection::THErefinementLevel); 

  showGrid(gc,ps,"Grid with two refinements");

  showGrid(gc.refinementLevel[0],ps,"gc.refinementLevel[0]");
  showGrid(gc.refinementLevel[1],ps,"gc.refinementLevel[1]");

  // now add some multigrid coarsenings
  level=1;
  grid=0;
  factor=2;
  gc.addMultigridCoarsening(factor,level,grid);
  grid=1;
  gc.addMultigridCoarsening(factor,level,grid);

  level=2;
  grid=1;
  gc.addMultigridCoarsening(factor,level,grid);

  gc.update(GridCollection::THErefinementLevel | GridCollection::THEmultigridLevel);


  showGrid(gc,ps,"gc after addMultiGridCoarsening");

  for( level=0; level<gc.numberOfRefinementLevels(); level++ )
    showGrid(gc.refinementLevel[level],ps,sPrintF(buff,"gc.refinementLevel[%i]",level));

  for( level=0; level<gc.numberOfMultigridLevels(); level++ )
    showGrid(gc.multigridLevel[level],ps,sPrintF(buff,"gc.multigridLevel[%i]",level));

  showGrid(gc.multigridLevel[1].masterGridCollection(),ps,"gc.multigridLevel[1].masterGridCollection");

  grid=3;
  gc.deleteRefinement(grid);
  gc.update(GridCollection::THErefinementLevel | GridCollection::THEmultigridLevel); 

  showGrid(gc,ps,"gc after deleteRefinement(3)");
  for( level=0; level<gc.numberOfRefinementLevels(); level++ )
    showGrid(gc.refinementLevel[level],ps,sPrintF(buff,"gc.refinementLevel[%i] after deleteRefinement(3)",level));

  for( level=0; level<gc.numberOfMultigridLevels(); level++ )
    showGrid(gc.multigridLevel[level],ps,sPrintF(buff,"gc.multigridLevel[%i] after deleteRefinement(3)",level));


  gc.deleteRefinementLevels(0);
  gc.update(GridCollection::THErefinementLevel | GridCollection::THEmultigridLevel); 

  showGrid(gc,ps,"gc after deleteRefinementLevels(0)");
  for( level=0; level<gc.numberOfRefinementLevels(); level++ )
    showGrid(gc.refinementLevel[level],ps,sPrintF(buff,"gc.refinementLevel[%i]",level));

  for( level=0; level<gc.numberOfMultigridLevels(); level++ )
    showGrid(gc.multigridLevel[level],ps,sPrintF(buff,"gc.multigridLevel[%i]",level));

  return 0;
}
