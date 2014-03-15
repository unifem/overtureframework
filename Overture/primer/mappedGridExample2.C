#include "Overture.h"  
#include "PlotStuff.h"
#include "AnnulusMapping.h"
#include "MappedGridOperators.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ------------------------------------------------------------------- \n");
  printf(" Solve a convection-diffusion equation on an annulus                 \n");
  printf(" Use operators to compute derivatives and apply boundary conditions  \n");
  printf(" Interactively plot results                                          \n");
  printf(" ------------------------------------------------------------------- \n");

  AnnulusMapping annulus; 
  annulus.setGridDimensions(axis1,41);               // axis1==0, set no. of grid points
  annulus.setGridDimensions(axis2,13);               // axis2==1, set no. of grid points
  MappedGrid mg(annulus);                            // MappedGrid for a square
  mg.update();                                       // create default variables

  Range all;
  realMappedGridFunction u(mg);
  u.setName("Solution");                          // give names to grid function ...
  u.setName("u",0);                               // ...and components

  Index I1,I2,I3;                                            
  // The A++ array mg.dimension()(2,3) holds index bounds on all points on the grid, including ghost-points
  getIndex(mg.dimension(),I1,I2,I3);               // assign I1,I2,I3 from dimension
  u(I1,I2,I3)=1.;                                // initial conditions
    
  MappedGridOperators op(mg);                    // operators 
  u.setOperators(op);                            // associate with a grid function

  PlotStuff ps(TRUE,"mappedGridExample2");      // create a PlotStuff object
  PlotStuffParameters psp;         // This object is used to change plotting parameters
  char buffer[80];

  real t=0, dt=.005, a=1., b=1., nu=.1; 
  for( int step=0; step<100; step++ )
  {
    if( step % 10 == 0 )
    { // plot contours every 10 steps
      ps.erase();
      psp.set(GI_TOP_LABEL,sPrintF(buffer,"Solution at time t=%e",t));  // set title
      PlotIt::contour(ps, u,psp );
    }

    u+=dt*( (-a)*u.x()+(-b)*u.y()+nu*(u.xx()+u.yy()) ); // ****** forward Euler time step *****
    t+=dt;
    // apply Boundary conditions
    int component=0;
    u.applyBoundaryCondition(component,BCTypes::dirichlet,BCTypes::allBoundaries,0.);    // set u=0.
    // fix up corners, periodic update:
    u.finishBoundaryConditions();                                      
  }
  
  Overture::finish();          
  return 0;
}

