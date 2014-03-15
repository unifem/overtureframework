// ---- amrTest.C -------

#include "PlotStuff.h"
#include "Square.h"
#include "Annulus.h"
#include "BoxMapping.h" 
#include "HDF_DataBase.h"

#define GridCollection CompositeGrid
#define realGridCollectionFunction realCompositeGridFunction
int 
main(int argc, char* argv[]) 
{
  ios::sync_with_stdio();
  Index::setBoundsCheck(On);

  printf(" -------------------------------------------------------------------------- \n");
  printf(" Demonstrate the method for adding refinement levels to a grid collection   \n");
  printf(" Show how to plot the grid collection, refinement levels, or grid functions \n");
  printf("     usage: amrExample1 [square][annulus][box]                                \n");
  printf("     (default grid is `square')                                             \n");
  printf(" -------------------------------------------------------------------------- \n");


  PlotStuff ps(TRUE,"amrExample1");               // for plotting
  PlotStuffParameters psp;

  SquareMapping square(-1., 1., -1., 1.);         // Create a SquareMapping
  square.setGridDimensions(axis1,11); square.setGridDimensions(axis2,11);

  Mapping & mapping = (Mapping&)square;

  MappedGrid mg(mapping);      // grid for a mapping
  mg.update();
      
  //  Create a two-dimensional GridCollection with one grid.
  const int numberOfDimensions=2;
  GridCollection gc(numberOfDimensions,1);
  gc[0].reference(mg);   
  gc.updateReferences();

  psp.set(GI_TOP_LABEL,"initial grid");  // set title
  // PlotIt::plot(ps,gc,psp);                       // plot the grid

  // assign values to the grid collection function
  realGridCollectionFunction u(gc);
  Index I1,I2,I3;
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    getIndex(gc[grid].dimension(),I1,I2,I3);
    u[grid](I1,I2,I3)=sin(gc[grid].vertex()(I1,I2,I3,axis1)*Pi)*sin(gc[grid].vertex()(I1,I2,I3,axis2)*Pi);
    if( gc.numberOfDimensions()==3 )
      u[grid](I1,I2,I3)*=sin(gc[grid].vertex()(I1,I2,I3,axis3)*Pi);
  }
  psp.set(GI_TOP_LABEL,"u on initial grid"); 
//  PlotIt::contour(ps,u,psp);
    
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
  gc.update(GridCollection::THErefinementLevel); 

  gc.refinementFactor.display("gc.refinementFactor");
  gc.refinementLevel[0].refinementFactor.display("gc.refinementLevel[0].refinementFactor");
  gc.refinementLevel[1].refinementFactor.display("gc.refinementLevel[1].refinementFactor");
      
  gc.update(MappedGrid::THEvertex);
  
  psp.set(GI_TOP_LABEL,"refined grid");  
  // PlotIt::plot(ps,gc,psp);                        // plot the grid collection including refinements

  psp.set(GI_TOP_LABEL,"refinementLevel[0]");  
  // PlotIt::plot(ps,gc.refinementLevel[0],psp);     // plot refinement level 0 only

  u.updateToMatchGrid(gc);       // tell u that the gridCollection has been changed
  for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    getIndex(gc[grid].dimension(),I1,I2,I3);
    u[grid](I1,I2,I3)=sin(gc[grid].vertex()(I1,I2,I3,axis1)*Pi)*sin(gc[grid].vertex()(I1,I2,I3,axis2)*Pi);
  }
  psp.set(GI_TOP_LABEL,"u on refined grid"); 
  // PlotIt::contour(ps,u,psp);

  psp.set(GI_TOP_LABEL,"refinementLevel[1]");  
  // PlotIt::plot(ps,gc.refinementLevel[1],psp);     // plot refinement level 1 only

  // Now plot the refinementLevel's in the grid functions
  psp.set(GI_TOP_LABEL,"u.refinementLevel[0]"); 
  PlotIt::contour(ps,u.refinementLevel[0],psp);   // plot u on refinement level 0

  psp.set(GI_TOP_LABEL,"u.refinementLevel[1]"); 
  // PlotIt::contour(ps,u.refinementLevel[1],psp);    // plot u on refinement level 1


// *****

  GridCollection gc2(numberOfDimensions,1);
  gc2[0].reference(mg);   
  gc2.updateReferences();
  gc2.update();
  gc2.update(GridCollection::THErefinementLevel);  // indicate that we are want a refinement level

#if 0
  // Add a refinement, specify position in the coarse grid index space
  range(0,0) = 4; range(1,0) = 8;
  range(0,1) = 4; range(1,1) = 8;
  range(0,2) = 0; range(1,2) =  0;
  
  grid = 0;                              // refine this base grid
  gc2.addRefinement(range, factor, level, grid);    // add a refinement grid to level 1
  gc2.update(GridCollection::THErefinementLevel); 
#endif

  psp.set(GI_TOP_LABEL,"gc2 unrefined grid");  
  PlotIt::plot(ps,gc2,psp);                        // plot the grid collection including refinements

//  gc=gc2;
// ***  gc2.destroy();
  gc2 = gc;

// ***  gc2.update();
// ***  gc2.update(GridCollection::THErefinementLevel);  // indicate that we are want a refinement level

  gc2.deleteRefinement(1);
// **  gc2.update();
//   gc2.update(GridCollection::THErefinementLevel);  // indicate that we are want a refinement level

#if 1
  // Add a refinement, specify position in the coarse grid index space
  range(0,0) = 4; range(1,0) = 8;
  range(0,1) = 4; range(1,1) = 8;
  range(0,2) = 0; range(1,2) =  0;
  
  grid = 0;                              // refine this base grid
  gc2.addRefinement(range, factor, level, grid);    // add a refinement grid to level 1

  if( TRUE )
  {
    range(0,0) = 2; range(1,0) = 4;
    range(0,1) = 2; range(1,1) = 4;
    range(0,2) = 0; range(1,2) =  0;
  
    grid = 0;                              // refine this base grid
    gc2.addRefinement(range, factor, level, grid);    // add a refinement grid to level 1
  }

// **   gc2.update();
  gc2.update(GridCollection::THErefinementLevel); 

  psp.set(GI_TOP_LABEL,"gc2 after delete and add");  
  PlotIt::plot(ps,gc2,psp);
  psp.set(GI_TOP_LABEL,"gc2.refinementLevel[0] after delete and add");  
  PlotIt::plot(ps,gc2.refinementLevel[0],psp);
  psp.set(GI_TOP_LABEL,"gc2.refinementLevel[1] after delete and add");  
  PlotIt::plot(ps,gc2.refinementLevel[1],psp);


// Notice that we didn't call u.updateToMatchGrid(gc) after we deleted a grid
  u.updateToMatchGrid(gc2);       // tell u that the gridCollection has been changed  
#endif

  for( grid=0; grid<gc2.numberOfComponentGrids(); grid++ )
  {
    getIndex(gc2[grid].dimension(),I1,I2,I3);
    u[grid](I1,I2,I3)=sin(gc2[grid].vertex()(I1,I2,I3,axis1)*Pi)*sin(gc2[grid].vertex()(I1,I2,I3,axis2)*Pi);
    if( gc2.numberOfDimensions()==3 )
      u[grid](I1,I2,I3)*=sin(gc2[grid].vertex()(I1,I2,I3,axis3)*Pi);
  }
  psp.set(GI_TOP_LABEL,"u on refined grid gc2"); 
  PlotIt::contour(ps,u,psp);

  psp.set(GI_TOP_LABEL,"refinementLevel[1] on gc2");  
  PlotIt::plot(ps,gc.refinementLevel[1],psp);     // plot refinement level 1 only

  // Now plot the refinementLevel's in the grid functions
  psp.set(GI_TOP_LABEL,"u.refinementLevel[0] on gc2"); 
  PlotIt::contour(ps,u.refinementLevel[0],psp);   // plot u on refinement level 0

  psp.set(GI_TOP_LABEL,"u.refinementLevel[1] on gc2"); 
  PlotIt::contour(ps,u.refinementLevel[1],psp);    // plot u on refinement level 1

  return 0;
}
