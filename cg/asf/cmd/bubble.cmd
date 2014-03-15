*
* define a bubble of heavy gas in a gravitational field
*
$tFinal=10.; $tPlot=.2; $Mach=.1; $Re=1000.; $nuRho=.05; $anu=.1; $debug=1; 
$tol=1.e-12;
$method ="linearized all speed implicit"; 
* $method="all speed implicit";
$grid="square5.hdf"; $Mach=.1; $Re=10.; $tPlot=.01; #$debug=63;
 $grid="square20.hdf";
* $grid="square80.hdf"; $Re=2000.; $Mach=.01; 
* $grid="square256.hdf"; $Re=5000.; 
*
  $grid
* 
  all speed Navier Stokes
  exit
*
  $method
*
  turn off twilight zone 
* 
  final time $tFinal
  times to plot $tPlot
  plot and always wait
  debug $debug 
* 
  OBPDE:densityFromGasLawAlgorithm
  * OBPDE:defaultAlgorithm
* 
  pde parameters
*    conservative with artificial dissipation
    Mach number
     $Mach
    Reynolds number
     $Re
    OBPDE:nuRho $nuRho
    OBPDE:anu $anu
    gravity
      0. -1. 0.
    done
  boundary conditions
    all=noSlipWall,  uniform(T=1.)
  done
  debug $debug
  initial conditions
    user defined
      * note that p = r R T = constant
      * p given below is the perturbation pressure
      bubbles
       1
      r=1. p=0. T=1.
       .175 .5 .7
      r=2. p=0. T=.5
    exit
   exit
*
   implicit time step solver options
     * choose best iterative solver
     * these tolerances are chosen for PETSc
     choose best iterative solver
     * PETScNew     
     relative tolerance
       1.e-4 $tol
     absolute tolerance
       1.e-5 1.e-15
     debug 
      1 7 0 1 7 3
    exit
  continue

