// short version for Dan Zwillinger
#include "Overture.h"  
#include "PlotStuff.h"
#include "Annulus.h"
#include "MappedGridOperators.h"

int 
main()
{
  real r0=.5, r1=1., x0=0., y0=0., theta0=0., theta1=1.; // Annulus parameters
  AnnulusMapping annulus(r0,r1,x0,y0,theta0,theta1); // Mapping for an annulus 
  MappedGrid mg(annulus);                            // MappedGrid for an annulus
  mg.update();                                       // create default variables
  realMappedGridFunction u(mg);                      // declare a grid function on the grid
  u=1.;                                              // initial conditions
  MappedGridOperators op(mg);                        // difference operators and boundary conditions
  u.setOperators(op);                                // associate with a grid function
  real t=0, dt=.005, a=1., b=1., nu=.1;
  for( int step=0; step<100; step++ )
  {
    u.display("solution");                              // print out the solution
    u+=dt*( (-a)*u.x()+(-b)*u.y()+nu*(u.xx()+u.yy()) ); // forward Euler time step *****
    t+=dt;
    // apply Boundary condition u=0.
    u.applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::allBoundaries,0.);
    u.finishBoundaryConditions();   // fix up corners, periodic update                                   
  }
  
  return 0;
}

