*
* cgad: stirring stick example (rotating stick)
*
$kappa=.1; $a=1.; $b=1.; $c=0.; 
$tFinal=.5; $tPlot=.025; $show = " "; 
$grid="stir.hdf"; $show = "stir.show"; 
*
$grid
* 
  convection diffusion
*
  exit
  show file options
    * compressed
    open
     $show
    frequency to flush
      5
    exit
**  turn off twilight zone
**  project initial conditions
*
  final time $tFinal
  times to plot $tPlot
  plot and always wait
  * no plotting
* 
  pde parameters
    kappa $kappa
    a $a
    b $b
    c $c
  done
* 
  turn on moving grids
* 
  specify grids to move
    rotate
      0. 0. 0.
*    specify rate and rampInterval (rampInterval=0. => impulsive start, .5=slow start)
      .5 .0
    stir
    done
  done
  * use implicit time stepping
  implicit
  choose grids for implicit
     all=explicit
     stir=implicit
    done
* 
  boundary conditions
    all=dirichletBoundaryCondition
    done
*   initial conditions
*     uniform flow
*      p=1.
*   exit
* 
  continue
