#include "Overture.h"  
#include "PlotStuff.h"
#include "SquareMapping.h"
#include "MappedGridOperators.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ------------------------------------------------------------------ \n");
  printf(" Demonstrate mappings, grids, gridfunctions, operators and plotting \n");
  printf(" ------------------------------------------------------------------ \n");

  SquareMapping square(0.,1.,0.,1.);                   // Make a mapping, unit square
  square.setGridDimensions(axis1,11);                  // axis1==0, set no. of grid points
  square.setGridDimensions(axis2,11);                  // axis2==1, set no. of grid points
  MappedGrid mg(square);                               // MappedGrid for a square
  // * mg.changeToAllCellCentered();                        // make a cell centered grid
  mg.update();                                       // This will generate default geometry arrays (e.g. vertex)
  
  Range all;                                   // a null Range is used as a place-holder below for the coordinates
  realMappedGridFunction u(mg,all,all,all,2);  // create a grid function with 2 components: u(0:10,0:10,0:0,0:1)
  u.setName("Velocity Stuff");                 // give names to grid function ...
  u.setName("u Stuff",0);                      // ...and components
  u.setName("v Stuff",1);
  Index I1,I2,I3;                                            

  // mg.dimension()(2,3) : start/end index values for all points on the grid, including ghost-points
  getIndex(mg.dimension(),I1,I2,I3);                        // assign I1,I2,I3 from dimension
  u(I1,I2,I3,0)=sin(Pi*mg.vertex()(I1,I2,I3,axis1))         // component 0 : sin(pi*x)*cos(pi*y)
               *cos(Pi*mg.vertex()(I1,I2,I3,axis2));        
  u(I1,I2,I3,1)=cos(Pi*mg.vertex()(I1,I2,I3,axis1))         // component 1 : cos(pi*x)*sin(pi*y)
               *sin(Pi*mg.vertex()(I1,I2,I3,axis2));
    
  MappedGridOperators op(mg);                             // operators 
  u.setOperators(op);                                     // associate with a grid function
  u.x().display("here is u.x");                           // x derivative
  u.x(all,all,all,0).display("here is u.x(all,all,all,0)");  // x derivative of component 0
  u.x(all,all,all,1).display("here is u.x(all,all,all,1)");  // x derivative of component 1

  getIndex(mg.gridIndexRange(),I1,I2,I3);                   // interior and boundary points
  // compute the error in component 0 of u.x, the notation u.x(I1,I2,I3,0) means only evaluate
  // the derivative for component 0 and at the points (I1,I2,I3) (done for efficiency only)
  real error = max(abs( u.x(I1,I2,I3,0)(I1,I2,I3,0)-
            Pi*cos(Pi*mg.vertex()(I1,I2,I3,axis1))*cos(Pi*mg.vertex()(I1,I2,I3,axis2)) ));
  cout << "Maximum error in component 0 of u.x = " << error << endl;

  bool openGraphicsWindow=TRUE;
  PlotStuff ps(openGraphicsWindow,"mappedGridExample1");      // create a PlotStuff object
  PlotStuffParameters psp;                      // This object is used to change plotting parameters
    
  aString answer;
  aString menu[] = {                             // Make some menu items
                    "!mappedGridExample1",      // title
                    "contour",                  
		    "stream lines",
		    "grid",
		    "read command file",
		    "save command file",
		    "erase",
                    "help",
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
      PlotIt::plot(ps,mg,psp);                          // plot the grid
    else if( answer=="stream lines" )
      PlotIt::streamLines(ps,u);                        // streamlines
    else if( answer=="read command file" )
      ps.readCommandFile(); 
    else if( answer=="save command file" )
      ps.saveCommandFile(); 
    else if( answer=="erase" )
      ps.erase();
    else if( answer=="help" )
      helpOverture("PR","mappedGridExample1"); // open web page at documentation 
    else if( answer=="exit" )
      break;
  }

  Overture::finish();          
  return 0;
}

