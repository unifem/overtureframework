#include "GenericGraphicsInterface.h"
#include "Overture.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems

  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

// get a pointer to the graphics interface
  GenericGraphicsInterface * psPointer = Overture::getGraphicsInterface("plot", TRUE); 
  GenericGraphicsInterface & ps = *psPointer;

  int win0 = 0; // the default graphics window has number 0

  char buf[100];
  aString nameOfOGFile(80);
  ps.inputFileName(nameOfOGFile, ">> Enter the name of the (old) composite grid file:", "hdf");

  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);

  cg.update();                // update to create usual variables

  // set up a function for contour plotting:
  Range all;
  realCompositeGridFunction u(cg,all,all,all);

  u.setName("Velocity Stuff");

  Index I1,I2,I3;                                              // A++ Index object
  int i1,i2,i3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )  // loop over component grids
  {
    RealArray & coord = (bool)cg[grid].isAllVertexCentered() ? cg[grid].vertex() : cg[grid].center(); 

    getIndex(cg[grid].dimension(),I1,I2,I3);                   // assign I1,I2,I3 from indexRange
    if( cg.numberOfDimensions()==1 )
    {
      u[grid](I1,I2,I3)=sin(Pi*coord(I1,I2,I3,axis1));
    }
    else if( cg.numberOfDimensions()==2 )
    {
      u[grid](I1,I2,I3)=
         sin(Pi*coord(I1,I2,I3,axis1))   // assign all interior points on this
        *cos(Pi*coord(I1,I2,I3,axis2));   // component grid
    }
    else
    {
      u[grid](I1,I2,I3)=sin(.5*Pi*coord(I1,I2,I3,axis1))  
        *cos(.5*Pi*coord(I1,I2,I3,axis2))
        *cos(.5*Pi*coord(I1,I2,I3,axis3));
    }      
  }    
    
  GraphicsParameters gp;               // create an object that is used to pass parameters
  GUIState interface;                  // GUI object
  SelectionInfo select;                // the results from picking will be recorded here
  
  aString answer;

// define push buttons
  aString pbCommands[] = {"plot contour", "plot grid", ""};
  aString pbLabels[] = {"Contour...", "Grid...", ""};
  
  interface.setPushButtons( pbCommands, pbLabels, 1 ); // default is 2 rows
  interface.setWindowTitle("Small test program");
  interface.setExitCommand("exit", "Exit");

  ps.pushGUI(interface);
  
  for(;;)
  {
    ps.getAnswer(answer,"Ready to serve>", select);
  
    if (select.active == 1)
    {
      ps.outputString(sPrintF(buf,"Window coordinates: %e, %e", select.r[0], select.r[1]));
    }
    if( select.nSelect )
    {
      ps.outputString(sPrintF(buf,"World coordinates: %e, %e, %e", select.x[0], select.x[1], select.x[2]));
      ps.outputString(sPrintF(buf,"globalID=%i", select.globalID));
      ps.outputString(sPrintF(buf,"Selection:"));
      int i;
      for (i=0; i<select.nSelect; i++)
      {
 	ps.outputString(sPrintF(buf,"ID[%i]=%i, back-z=%i, front-z=%i",i,
  				select.selection(i,0),select.selection(i,1),select.selection(i,2)));
      }
    }
    
    if( answer == "plot grid" )
    {
      gp.set(GI_TOP_LABEL,"My Grid");  // set title
      gp.set(GI_PLOT_SHADED_SURFACE_GRIDS, TRUE);
      gp.set(GI_LABEL_GRIDS_AND_BOUNDARIES, TRUE);
      PlotIt::plot(ps, cg, gp);   // plot the composite grid
    }
    else if( answer == "plot contour" )
    {
      gp.set(GI_TOP_LABEL,"My Contour Plot");  // set title
      PlotIt::contour(ps, u, gp);  // contour/surface plots
    }
    else if( answer=="exit" )
    {
      break;
    }
  }
  ps.popGUI();
  
  Overture::finish();          
  return 0;
}
