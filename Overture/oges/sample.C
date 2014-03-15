#include "Oges.h"
  
int 
main()
{
  MultigridCompositeGrid mgcg("square.dat",".");      // make a grid from a data base file
  CompositeGrid & cg=mgcg[0];                         // use multigrid level 0
  cg.update();                                        // update the grid
  createInverseVertexDerivative(cg);                  // this should go away!

  Oges solver(cg);                                    // create an Oges solver 
  solver.setEquationType( Oges::LaplaceDirichlet );   // Use one of the predefined equations 
  solver.setSolverType( Oges::yale );                 // Use Yale solver

  solver.initialize();                                // init solver

  // -- make some grid functions for the solution and rhs
  Range all;
  realCompositeGridFunction u(cg,all,all,all), f(cg,all,all,all);

  // assign the rhs: u_{xx}+u_{yy}=1  in the interior
  //                             u=0  on the boundary
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    f[grid]=0.;
    where( solver.classify[grid]>0 )
      f[grid]=1.;   // rhs for Laplace operator
    where( solver.classify[grid]==Oges::boundary )
      f[grid]=0.;   // boundary condition
  }    
  solver.solve( u,f );                                 // solve the equations
  u.display("Here is the solution");
}

