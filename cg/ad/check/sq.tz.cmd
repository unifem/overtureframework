$kappa=.1;
$grid = "square20";
*
$grid
* 
  convection diffusion
* 
  continue
*
  final time (tf=)
  .5
  times to plot (tp=)
  .1
  OBTZ:polynomial
  OBTZ:twilight zone flow 1
  close twilight zone options
*
  adams order 2
* 
  pde parameters
    kappa $kappa
    a 1.0
    b 1.0
    c  .0
  done
  boundary conditions
    all=dirichletBoundaryCondition
  done
 done
  continue
  movie mode 
  finish

