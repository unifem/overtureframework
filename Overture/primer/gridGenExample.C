#include "Overture.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "Ogen.h"
#include "PlotStuff.h"
#include "StretchTransform.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" -------------------------------------------------------------------------- \n");
  printf(" This example shows how to build an overlapping grid directly in a program, \n");
  printf(" rather than interactively.                                                 \n");
  printf(" -------------------------------------------------------------------------- \n");

  int plotOption=true; // set to false for no plotting
  PlotStuff gi(plotOption,"GridGenExample");
  PlotStuffParameters gip;

  // By default start saving the command file called "motion.cmd"
  aString logFile="gridGen.cmd";
  gi.saveCommandFile(logFile);
  cout << "User commands are being saved in the file `" << (const char *)logFile << "'\n";

  // First build mappings
  SquareMapping map1(-1., 1., -1., 1.);        // Create a square
  map1.setGridDimensions(axis1,21); map1.setGridDimensions(axis2,21);

  AnnulusMapping map2;            // Create an Annulus
  map2.setRadii(.3,.6);
  map2.setGridDimensions(axis1,41); map2.setGridDimensions(axis2,9);
  // set outer boundary to be interpolation:
  map2.setBoundaryCondition(End,axis2,0);  

  MappingInformation mapInfo;
  mapInfo.graphXInterface=&gi;

  // stretch the annulus
  StretchTransform map3;
  map3.setMapping(map2);

  // Here we show how to use interactive commands to set the properties of the
  // stretching (rather than calling member functions to set the properties).

  // Make a list of interactive commands: 
  aString cmds[5] = {"Stretch r2:itanh",
		     "STP:stretch r2 itanh: position and min dx 0 0.02",
		     "stretch grid",
		     "exit",
		     "" }; // N.B. string should be null terminated
  // Tell the graphics interface to use these commands when requested
  gi.readCommandsFromStrings(cmds);
  // "interactively" update the StretchTransform (which will read the command strings)
  map3.update(mapInfo);

  // map3.update(mapInfo);
  

  // Put the mappings into a list
  mapInfo.mappingList.addElement(map1);
  mapInfo.mappingList.addElement(map3);

  // Indicate which mappings should be used in the overlapping grid
  const int numberOfDimensions=2, numberOfGrids=2;
  IntegerArray mapList(numberOfGrids);
  mapList(0)=0;
  mapList(1)=1;

  // build an empty overlapping grid
  CompositeGrid cg;

  // Create an overlapping grid generator
  Ogen ogen(gi);

  // Put the mappings into the CompositeGrid
  ogen.buildACompositeGrid(cg,mapInfo,mapList);

  //  ogen.debug=3; // turn this on to show intermediate results

  // ** generate the overlapping grid**
  ogen.updateOverlap(cg);
  
  // Plot the overlapping grid.
  gi.erase();
  gip.set(GI_TOP_LABEL,"Grid after Ogen");  // set title
  PlotIt::plot(gi,cg,gip);                       // plot the grid


  Overture::finish();          
  return 0;
}
