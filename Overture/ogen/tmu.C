// 
//  Test the movingUpdate function for moving overlapping grids 
//


#include "PlotStuff.h"
#include "Ogen.h"
#include "HDF_DataBase.h"
#include "display.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  
  char buff[80];
  aString commandFileName="";
  if( argc>1 )
  {
    commandFileName=argv[1];
  }

  const bool plot=true;
  PlotStuff ps(plot,"tmu");               // for plotting
  GraphicsParameters psp;

  aString logFile="tmu.cmd";
  ps.saveCommandFile(logFile);
  cout << "User commands are being saved in the file `" << (const char *)logFile << "'\n";
  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);


  aString menu[]=
  {
    "moving update",
    "full update",
    "plot old grid",
    "plot new grid",
    "debug",
    "erase",
    "exit",
    ""
  };
  aString answer,answer2;
  
  CompositeGrid cgOld, cgNew;
  
  getFromADataBase(cgOld,"movingGridOld.hdf");
  getFromADataBase(cgNew,"movingGridNew.hdf");

  LogicalArray hasMoved(cgNew.numberOfComponentGrids());
  hasMoved=false;
  // do this for now
  for( int grid=1; grid<cgNew.numberOfComponentGrids(); grid++ )
  {
    hasMoved(grid)=true;
  }
  
  Ogen ogen(ps);
  ogen.debug=0;
    
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  psp.set(GI_PLOT_INTERPOLATION_POINTS,true);
  psp.set(GI_COLOUR_INTERPOLATION_POINTS,true);

  bool plotGrid=true;
  
  for( ;; )
  {
    if( plotGrid )
    {
      ps.erase();
      PlotIt::plot(ps,cgNew,psp);
    }
    
    ps.getMenuItem(menu,answer,"choose");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="moving update" )
    {
      Ogen::MovingGridOption option = Ogen::useOptimalAlgorithm;
      ogen.updateOverlap(cgNew,cgOld,hasMoved,option );
    }
    else if( answer=="full update" )
    {
      Ogen::MovingGridOption option = Ogen::useFullAlgorithm;
      ogen.updateOverlap(cgNew,cgOld,hasMoved,option );
    }
    else if( answer=="plot old grid" )
    {
      ps.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      PlotIt::plot(ps,cgOld,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="plot new grid" )
    {
      ps.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      PlotIt::plot(ps,cgNew,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="erase" )
    {
      ps.erase();
      plotGrid=false;
    }
    else if( answer=="debug" )
    {
      ps.inputString(answer,"Enter debug");
      sScanF(answer,"%i",&ogen.debug);
      printf(" ogen.debug = %i\n",ogen.debug);
    }
    else
    {
      printf("Unknown response: [%s] \n",(const char*)answer);
      ps.stopReadingCommandFile();
    }
  }
  

  Overture::finish();          
  return 0;
}
