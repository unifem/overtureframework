*
*   cgad example: 
* 
$kappa=.1;
$grid = "cic";
*
$grid
* 
  convection diffusion
  continue
* 
  turn off twilight zone 
  * turn on trig
  final time (tf=)
  .5
  times to plot (tp=)
  .1
  plot and always wait
  * no plotting
  pde parameters
    kappa $kappa
    a 1.0
    b 1.0
    c  .0
  done
  boundary conditions
    all=dirichletBoundaryCondition
    Annulus(0,1)=dirichletBoundaryCondition, uniform(T=1.)
   done
*  debug
*    7
  initial conditions
    OBIC:uniform state T=0. 
    OBIC:assign uniform state
  continue
continue
movie mode
finish
