#include "Overture.h"
#include "Square.h"
#include "Annulus.h"
#include "HDF_DataBase.h"
#include "PlotStuff.h"

void
build(realMappedGridFunction *&ww)
{
  ww = new realMappedGridFunction(); // calls A++ new
}

void
destroy(realMappedGridFunction *ww)
{
  delete ww;  // does not call A++ delete
}


int 
main(int argc, char *argv[])
{
  Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON );

  Overture::start(argc,argv);  // initialize Overture

  printf(" -------------------------------------------------------------------------- \n");
  printf(" Demonstrate the method for adding refinement levels to a grid collection   \n");
  printf(" Show how to plot the grid collection, refinement levels, or grid functions \n");
  printf(" -------------------------------------------------------------------------- \n");


  realMappedGridFunction *ww = new realMappedGridFunction(); // calls A++ new
  delete ww;  // calls A++ delete
  ww=NULL;
  
  build(ww);
  assert( ww!=NULL );
  destroy(ww);


  SquareMapping mapping(-1., 1., -1., 1.);            // Create a SquareMapping
  mapping.setGridDimensions(axis1,11); mapping.setGridDimensions(axis2,11);
//AnnulusMapping mapping;            // Create an Annulus
//mapping.setGridDimensions(axis1,21); mapping.setGridDimensions(axis2,11);

  MappedGrid mg(mapping);      // grid for a mapping
  mg.update();
      
  //  Create a two-dimensional GridCollection with one grid.
  GridCollection gc(2,1);
  gc[0].reference(mg);   
  gc.updateReferences();

  // assign values to the grid collection function
  realGridCollectionFunction u(gc);
  Index I1,I2,I3;
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    getIndex(gc[grid].dimension(),I1,I2,I3);
    u[grid](I1,I2,I3)=sin(gc[grid].vertex()(I1,I2,I3,axis1)*Pi)*sin(gc[grid].vertex()(I1,I2,I3,axis2)*Pi);
  }
    
  gc.update(GridCollection::THErefinementLevel);  // indicate that we are want a refinement level

  // Add a refinement, specify position in the coarse grid index space
  IntegerArray range(2,3), factor(3);
  range(0,0) = 2; range(1,0) = 6;
  range(0,1) = 2; range(1,1) = 6;
  range(0,2) = 0; range(1,2) =  0;
  factor = 4;                            // refinement factor = 4
  Integer level = 1;
  grid = 0;                              // refine this base grid
  gc.addRefinement(range, factor, level, grid);    // add a refinement grid to level 1

  range(0,0) = 4; range(1,0) = 8;
  range(0,1) = 4; range(1,1) = 8;
  range(0,2) = 0; range(1,2) = 0;
  gc.addRefinement(range, factor, level, grid);   // add another refinement grid to level 1

  gc.update(GridCollection::THErefinementLevel); 

  gc.refinementFactor.display("gc.refinementFactor");
  gc.refinementLevel[0].refinementFactor.display("gc.refinementLevel[0].refinementFactor");
  gc.refinementLevel[1].refinementFactor.display("gc.refinementLevel[1].refinementFactor");
      
  gc.update(MappedGrid::THEvertex);

  u.updateToMatchGrid(gc);       // tell u that the gridCollection has been changed
  for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    getIndex(gc[grid].dimension(),I1,I2,I3);
    u[grid](I1,I2,I3)=sin(gc[grid].vertex()(I1,I2,I3,axis1)*Pi)*sin(gc[grid].vertex()(I1,I2,I3,axis2)*Pi);
  }

  Range all;
  realGridCollectionFunction q(gc,all,all,all,3);
  
  q=1.;

  Overture::finish();          
  return 0;
}
