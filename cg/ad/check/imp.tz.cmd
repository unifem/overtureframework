*
* Check the implicit time stepping for a box.
* This should be exact for degreeX=2 and degreeT=1
*
$kappa=.1;
$grid = "box10";
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
* -- time-stepping method --
  implicit
  implicit factor .5 (1=BE,0=FE)
* 
  OBTZ:polynomial
  OBTZ:twilight zone flow 1
  OBTZ:degree in space 2
  OBTZ:degree in time 1
  close twilight zone options
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

