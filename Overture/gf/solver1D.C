//===============================================================================
//  Primer: Here we solve a simple 1D PDE
//==============================================================================
#include "Overture.h"  
#include "PlotStuff.h"
#include "MappedGridOperators.h"
#include "LineMapping.h"

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  int numberOfGridPoints=11;
  LineMapping line(0.,1.,numberOfGridPoints);   // mapping for unit interval, [0,1] with 11 grid points
  MappedGrid mappedGrid(line);   // a grid
  mappedGrid.update();           // compute usual geometry arrays

  mappedGrid.vertex.display("Here is the vertex array");
  mappedGrid.mask.display("Here is the mask array");

  mappedGrid.mask=1; // **************************************

  Range all;
  realMappedGridFunction u(mappedGrid,all,2);           // create a grid function with 2 components
  u.setName("Velocity Stuff");                // give names to grid function ...
  u.setName("u",0);                           // ...and components
  u.setName("v",1);

  MappedGridOperators op(mappedGrid);
  u.setOperators(op);

  // assign initial conditions
  int i0=mappedGrid.indexRange(Start,0);  // get index values for x=0 and x=1
  int i1=mappedGrid.indexRange(End,0); 
  Index I1(i0,i1);
  u(I1,0)=sin(twoPi*mappedGrid.center(I1));   // component 0 : sin(x)
  u(I1,1)=cos(twoPi*mappedGrid.center(I1));   // component 1 : cos(x)
    
/* ----
  real dt=.5/numberOfGridPoints;
  for( int step=0; step<10; step++ )
  {
    u+=dt*u.x();
    u(i0)=0.;                                               // give u=0 at x=0
    u(i1)=u(i1-1);       // extrapolate u at x=1
  }
---- */

  // Now do some plotting...

  PlotStuff ps;                                 // create a PlotStuff object
  PlotStuffParameters psp;                      // This object is used to change plotting parameters
    
  aString answer;
  aString menu[] = { "contour",                  // Make some menu items
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
      PlotIt::plot(ps,mappedGrid);                              // plot the composite grid
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

  return 0;
}
