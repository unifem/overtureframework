#include "Overture.h"
#include "Ogshow.h"  
#include "CompositeGridOperators.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ---------------------------------------------------------------------------- \n");
  printf("Solve: u.t + a*u.x + b*u.y = viscosity*( u.xx + u.yy ) on an Overlapping grid \n");
  printf("Save results in a show file, use plotStuff to view this file                  \n");
  printf(" ---------------------------------------------------------------------------- \n");

  aString nameOfOGFile="cice.hdf", nameOfShowFile="example6.show";
  #ifndef USE_PPP
   // prompt for name changes in serial, for parallel just use default
   cout << "example6>> Enter the name of the (old) overlapping grid file:" << endl;
   cin >> nameOfOGFile;
   cout << "example6>> Enter the name of the (new) show file (blank for none):" << endl;
   cin >> nameOfShowFile;
  #endif

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  // Interpolant interpolant(cg);                                 // Make an interpolant
  Interpolant & interpolant = *new Interpolant(cg);               // do this instead for now. 

  Ogshow show( nameOfShowFile );                               // create a show file
  show.saveGeneralComment("Convection Diffusion Equation");    // save a general comment in the show file
//  show.setFlushFrequency(10);                                  // flush file every 10 frames
    
  CompositeGridOperators operators(cg);                        // operators for a CompositeGrid
  // operators.setOrderOfAccuracy(4);                          // for fourth order

  Range all;
  realCompositeGridFunction u(cg,all,all,all,1);               // create a grid function 
  u.setOperators(operators);                                 
  u.setName("u");                                              // name the grid function

  u=1.;                                                        // initial condition
  real t=0, dt=.001;                                           // initialize time and time step
  real a=1., b=1., viscosity=.1;                               // initialize parameters
    
  char buffer[80];                                             // buffer for sprintf
  int numberOfTimeSteps=200;
  for( int i=0; i<numberOfTimeSteps; i++ )                    // take some time steps
  {
    if( i % 40 == 0 )  // save solution every 40 steps
    {
      show.startFrame();                                         // start a new frame
      show.saveComment(0,sPrintF(buffer,"Here is solution %i",i));   // comment 0 (shown on plot)
      show.saveComment(1,sPrintF(buffer,"  t=%e ",t));               // comment 1 (shown on plot)
      show.saveSolution( u );                                        // save the current grid function
    }
    u+=dt*( -a*u.x() - b*u.y() + viscosity*(u.xx() + u.yy())); // take a time step with Euler's method
    t+=dt;
    u.interpolate();                                           // interpolate
    // apply a dirichlet BC on all boundaries:
    u.applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::allBoundaries,0.);
    // u.applyBoundaryCondition(0,BCTypes::extrapolate,BCTypes::allBoundaries,0.); // for 4th order
    u.finishBoundaryConditions();
  }

  Overture::finish();          
  return 0;
    
}
