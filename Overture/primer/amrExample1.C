#include "Overture.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "HDF_DataBase.h"
#include "PlotStuff.h"
#include "display.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" -------------------------------------------------------------------------- \n");
  printf(" Demonstrate the method for adding refinement levels to a grid collection   \n");
  printf(" Show how to plot the grid collection, refinement levels, or grid functions \n");
  printf(" -------------------------------------------------------------------------- \n");


  PlotStuff ps;               // for plotting
  PlotStuffParameters psp;

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
  gc.update(MappedGrid::THEvertex);

  psp.set(GI_TOP_LABEL,"initial grid");  // set title
  PlotIt::plot(ps,gc,psp);                       // plot the grid

  // assign values to the grid collection function
  realGridCollectionFunction u(gc);
  Index I1,I2,I3;
  int grid;
  for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    getIndex(gc[grid].dimension(),I1,I2,I3);
    u[grid](I1,I2,I3)=sin(gc[grid].vertex()(I1,I2,I3,axis1)*Pi)*sin(gc[grid].vertex()(I1,I2,I3,axis2)*Pi);
  }
  ps.erase();
  psp.set(GI_TOP_LABEL,"u on initial grid"); 
  PlotIt::contour(ps,u,psp);
    
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
      
  ps.erase();
  psp.set(GI_TOP_LABEL,"refined grid");  
  PlotIt::plot(ps,gc,psp);                        // plot the grid collection including refinements

  ps.erase();
  psp.set(GI_TOP_LABEL,"refinementLevel[0]");  
  PlotIt::plot(ps,gc.refinementLevel[0],psp);     // plot refinement level 0 only

  u.updateToMatchGrid(gc);       // tell u that the gridCollection has been changed

  gc.update(MappedGrid::THEvertex);

  gc.setMaskAtRefinements();
  
  for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    printf(" grid=%i, level=%i, refinementFactor=%i\n",grid,gc.refinementLevelNumber(grid),
             gc.refinementFactor(axis1,grid));
    

    getIndex(gc[grid].dimension(),I1,I2,I3);
    u[grid](I1,I2,I3)=sin(gc[grid].vertex()(I1,I2,I3,axis1)*Pi)*sin(gc[grid].vertex()(I1,I2,I3,axis2)*Pi);

  }
  ps.erase();
  psp.set(GI_TOP_LABEL,"u on refined grid"); 
  PlotIt::contour(ps,u,psp);

  ps.erase();
  psp.set(GI_TOP_LABEL,"refinementLevel[1]");  
  PlotIt::plot(ps,gc.refinementLevel[1],psp);     // plot refinement level 1 only

  // Now plot the refinementLevel's in the grid functions
  ps.erase();
  psp.set(GI_TOP_LABEL,"u.refinementLevel[0]"); 
  PlotIt::contour(ps,u.refinementLevel[0],psp);   // plot u on refinement level 0

  ps.erase();
  psp.set(GI_TOP_LABEL,"u.refinementLevel[1]"); 
  PlotIt::contour(ps,u.refinementLevel[1],psp);    // plot u on refinement level 1

  Overture::finish();          
  return 0;
}
