#include "Overture.h"
#include "CompositeGridOperators.h"
#include "PlotStuff.h"

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  printf(" ------------------------------------------------------------ \n");
  printf(" Solve a convection-diffusion equation on an overlapping grid \n");
  printf(" Plot the solution in `movie' mode                            \n");
  printf(" ------------------------------------------------------------ \n");

  aString nameOfOGFile;
  cout << ">> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  // Interpolant interpolant(cg);                                 // Make an interpolant
  Interpolant & interpolant = *new Interpolant(cg);               // do this instead for now. 

  CompositeGridOperators operators(cg);                        // operators for a CompositeGrid
  Range all;
  realCompositeGridFunction u(cg,all,all,all,1);               // create a grid function 
  u.setOperators(operators);                                 
  u.setName("u");                                              // name the grid function
  u=1.;                                                        // initial condition
  real t=0, dt=.01;                                            // initialize time and time step
  real a=1., b=1., viscosity=.05;                              // initialize parameters
    
  PlotStuff ps;                                                // For plotting stuff.
  PlotStuffParameters psp;                                     // plotting parameters
  
  char buffer[80];                                             // buffer for sPrintF
  int numberOfTimeSteps=1000;
  for( int step=0; step<numberOfTimeSteps; step++ )                    // take some time steps
  {
    if( step % 5 == 0 )
    {
      psp.set(GI_TOP_LABEL,sPrintF(buffer,"Solution at time t=%e",t));  // set title
      ps.erase();
      PlotIt::contour(ps, u,psp );
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);     // set this to run in "movie" mode (after first plot)
      ps.redraw(TRUE);
    }
    u+=dt*( -a*u.x() - b*u.y() + viscosity*(u.xx() + u.yy())); // take a time step with Euler's method
    t+=dt;
    u.interpolate();                                           // interpolate
    // apply a dirichlet BC on all boundaries:
    u.applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::allBoundaries,0.);
    u.finishBoundaryConditions();
  }

  return 0;
    
}
