#include "Oges.h"
#include "PlotStuff.h"

int 
main(int argc, char* argv[]) 
{
  ios::sync_with_stdio();
  Index::setBoundsCheck(On);

  printf(" -------------------------------------------------------------------------- \n");
  printf(" Demonstrate the solution of a system of equations on a subset of grids.    \n");
  printf(" -------------------------------------------------------------------------- \n");


  PlotStuff ps;               // for plotting
  PlotStuffParameters psp;

  // create and read in a CompositeGrid
  aString nameOfOGFile="cic.hdf";
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();


  //  Create a two-dimensional GridCollection with one grid.
  GridCollection gc(2,1);
  gc[0].reference(cg[0]);   
  gc.updateReferences();

  psp.set(GI_TOP_LABEL,"subset grid");  // set title
  ps.plot(gc,psp);                       // plot the grid

  return 0;
}
