#include "Overture.h"  
#include "PlotStuff.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ------------------------------------------------------------------- \n");
  printf("Demonstrate interactive plotting using the PlotStuff class           \n");
  printf("Make a menu and selectively plot the grid or contours or streamlines.\n");
  printf(" ------------------------------------------------------------------- \n");

  aString nameOfOGFile;
  cout << "example>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  Range all;
  realCompositeGridFunction u(cg,all,all,all,2);           // create a grid function with 2 components
  u.setName("Velocity Stuff");                             // give names to grid function ...
  u.setName("u Stuff",0);                                  // ...and components
  u.setName("v Stuff",1);
  Index I1,I2,I3;                                            
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )  // loop over component grids
  {
    getIndex(cg[grid].dimension(),I1,I2,I3);                        // assign I1,I2,I3 from dimension
    u[grid](I1,I2,I3,0)=sin(Pi*cg[grid].center()(I1,I2,I3,axis1))   // component 0 : sin(x)*cos(y)
                       *cos(Pi*cg[grid].center()(I1,I2,I3,axis2));  
    u[grid](I1,I2,I3,1)=cos(Pi*cg[grid].center()(I1,I2,I3,axis1))   // component 1 : cos(x)*sin(y)
                       *sin(Pi*cg[grid].center()(I1,I2,I3,axis2));
  }    
    
  bool openGraphicsWindow=TRUE;
  PlotStuff ps(openGraphicsWindow,"example8");  // create a PlotStuff object
  PlotStuffParameters psp;                      // This object is used to change plotting parameters
    
  aString answer;
  aString menu[] = { 
                    "!example8",      
                    "contour",                  // Make some menu items
		    "stream lines",
		    "grid",
		    "read command file",
		    "save command file",
		    "erase",
		    "exit",
                    "" };                       // empty string denotes the end of the menu
  for(;;)
  {
    ps.getMenuItem(menu,answer);                // put up a menu and wait for a response
    if( answer=="contour" )
    {
      psp.set(GI_TOP_LABEL,"My Contour Plot");  // set title
      PlotIt::contour(ps,u,psp);                        // contour/surface plots
    }
    else if( answer=="grid" )
    {
      PlotIt::plot(ps,cg);                              // plot the composite grid
    }
    else if( answer=="stream lines" )
    {
      PlotIt::streamLines(ps,u);                        // streamlines
    }
    else if( answer=="read command file" )
    {
      ps.readCommandFile(); 
    }
    else if( answer=="save command file" )
    {
      ps.saveCommandFile(); 
    }
    else if( answer=="erase" )
    {
      ps.erase();
    }
    else if( answer=="exit" )
    {
      break;
    }
  }

  Overture::finish();          
  return 0;
}

