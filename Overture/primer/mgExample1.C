#include "Overture.h"
#include "PlotStuff.h"

int 
main(int argc, char* argv[]) 
{
  ios::sync_with_stdio();
  Index::setBoundsCheck(On);

  printf(" -------------------------------------------------------------------------- \n");
  printf("     Demonstrate how to use multigrid levels with an overlapping grid.      \n");
  printf(" The overlapping grid should be created with more than 1 multigrid level,   \n");
  printf(" see the cicmg.cmd command file as an example.                              \n");
  printf(" -------------------------------------------------------------------------- \n");

  aString nameOfOGFile;
  cout << "mgExample1>> Enter the name of the (old) overlapping grid file: (cicmg for example)" << endl;
  cin >> nameOfOGFile;

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  PlotStuff ps;               // for plotting
  PlotStuffParameters psp;
  char buff[80];              // buffer for sPrintF

  // plot each multigrid level (the grid plotter also knows how to plot multigrid levels,
  //   if we were to use: PlotIt::plot(ps,cg); and choose the menu option "plot a multigrid level" )
  int grid,level;
  for( level=0; level<cg.numberOfMultigridLevels(); level++ )
  {
    psp.set(GI_TOP_LABEL,sPrintF(buff,"Multigrid level %i",level));  // set title
    PlotIt::plot(ps,cg.multigridLevel[level],psp);    // plot the CompositeGrid for a given level
  }
  
  // create a grid function on the multigrid-overlapping grid and assign values to it
  realCompositeGridFunction u(cg);
  Index I1,I2,I3;
  for( level=0; level<cg.numberOfMultigridLevels(); level++ )
  {
    CompositeGrid & cgLevel = cg.multigridLevel[level];            // make a reference to a given level
    realCompositeGridFunction & uLevel = u.multigridLevel[level];
    for( grid=0; grid<cgLevel.numberOfComponentGrids(); grid++ )
    {
      getIndex(cgLevel[grid].dimension(),I1,I2,I3);
      uLevel[grid](I1,I2,I3)=sin(cgLevel[grid].vertex()(I1,I2,I3,axis1)*Pi)    // u = sin(x*Pi)*sin(y*Pi)
                            *sin(cgLevel[grid].vertex()(I1,I2,I3,axis2)*Pi);
    }
    psp.set(GI_TOP_LABEL,sPrintF(buff,"u on multigrid level %i",level));  // set title
    PlotIt::contour(ps,uLevel,psp);
  }

  return 0;
}



