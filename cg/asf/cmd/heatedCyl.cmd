*
* All-speed flow: - Heated cylinder in a gravitational field
* 
*    This problem may require a bit of a slow start until the initial pressure field is formed
*
$show =" "; $Mach=.1; $Reynolds=10.; $tFinal=5.; $tPlot=.1; $debug=0; $cfl=.9; 
$tol=1.e-10; $anu=0.; $nuRho=1.; $debug=1; 
$method ="linearized all speed implicit"; 
* $method="all speed implicit"; 
* 
$T0=2.; 
* $grid="cic.hdf"; $backGround="square"; $Mach=.01; $tPlot=.1; $Reynolds=50.; 
* $grid="cic4.hdf"; $backGround="square"; $Mach=.1; $tFinal=10.; $tPlot=.25; $Reynolds=500.; 
$grid="cice2.order2.hdf"; $backGround="square"; $Mach=.1; $tFinal=10.; $tPlot=.1; $Reynolds=500.; 
* $grid="cice4.order2.hdf"; $backGround="square"; $Mach=.1; $tFinal=10.; $tPlot=.1; $Reynolds=1000.; 
*
$grid
*
  all speed Navier Stokes
  exit
*
  OBPDE:densityFromGasLawAlgorithm
  * OBPDE:defaultAlgorithm
  $method
* 
  turn off twilight zone 
* 
  final time $tFinal
  cfl $cfl 
  times to plot $tPlot
* 
* Next specify the file to save the results in. 
* This file can be viewed with Overture/bin/plotStuff.
  show file options
    * compressed
    open
     $show
    frequency to flush
      5
    exit
*
  plot and always wait
  * no plotting
  debug $debug 
* 
  pde parameters
    Mach number
     $Mach
**   OBPDE:pressureLevel 10.
    Reynolds number
      $Reynolds
    OBPDE:nuRho $nuRho
    OBPDE:anu $anu
    gravity
      0. -1. 0.
    done
* 
  boundary conditions
    all=slipWall  uniform(T=1.)
    Annulus(0,1)=noSlipWall uniform(T=$T0)
    done
  initial conditions
    uniform flow
      r=1., u=0, T=1., p=0.
  exit
  debug $debug
* 
**  project initial conditions
*
   implicit time step solver options
     * choose best iterative solver
     * these tolerances are chosen for PETSc
     choose best iterative solver
     * PETScNew     
     relative tolerance
       $tol
     absolute tolerance
       1.e-15
     debug 
      1 7 0 1 7 3
    exit
  continue




