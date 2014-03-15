*
*   cgad example: flow in a cylinder with a source term 
* 
$tFinal=5.; $tPlot=.1; $kappa=.1; $numberOfExtraVariables=0;
* $grid = "sic";
* $grid = "sic3e"; $kappa=.05; $numberOfExtraVariables=2;
$grid = "sice5.order2.hdf"; $kappa=.05; $a=1.; $b=1.; $c=0.; 
*
$grid
* 
  convection diffusion
* 
  add extra variables
    $numberOfExtraVariables
  continue
* 
  turn off twilight zone 
  * turn on trig
  final time $tFinal
  times to plot $tPlot
  plot and always wait
  * no plotting
  pde parameters
    kappa $kappa
    a $a
    b $b
    c $c
  done
  boundary conditions
    all=dirichletBoundaryCondition
   done
*  debug
*    7
  initial conditions
    OBIC:uniform state T=0. 
    OBIC:assign uniform state
  continue
*  Here is the source term
  user defined forcing
    gaussian forcing
      * add 2 source terms
      2
      $a=1.; $b=30.; $p=2.; 
      0 0 $a $b $p -.6 -.2 0
      1 0 $a $b $p -.2 -.6 0
    done
  exit
* 
 continue
