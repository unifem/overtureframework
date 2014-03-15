*
* define a bubble of heavy gas in a gravitational field
*
$tFinal=10.; $tPlot=.2; $Mach=.1; $Re=1000.; $nuRho=.05; $anu=.1; $debug=1; 
$av4=1.; $av2=0.;
$tol=1.e-12;
*$method ="linearized all speed implicit"; 
* $method="all speed implicit";
$method = "compressible Navier Stokes (implicit)";
$grid="square20.hdf"; $Mach=.1; $Re=10.; $tPlot=.01; 
* $grid="square20.hdf";
* $grid="square80.hdf"; $Re=2000.; $Mach=.01; 
* $grid="square256.hdf"; $Re=5000.; 
*
  $grid
* 
$method
  exit
*
*
 turn off twilight zone 
* 
  final time $tFinal
  times to plot $tPlot
  plot and always wait
  debug 
$debug 
* 
*  OBPDE:densityFromGasLawAlgorithm
  * OBPDE:defaultAlgorithm
* 
  pde parameters
*    conservative with artificial dissipation
    Mach number
     $Mach
    Reynolds number
     $Re
    gravity
      0. -1. 0.
    OBPDE:av2,av4 $av2, $av4
    done
  boundary conditions
    square(0,1)=noSlipWall, userDefinedBoundaryData
      linear ramp in x
      3 1. 0. 0.
    done
    done
    square(0,0)=noSlipWall, userDefinedBoundaryData
      linear ramp in x
      3 1. 0. 0.
    done
    done
    square(1,1)=noSlipWall, userDefinedBoundaryData
      linear ramp in x
      3 1. 0. 0.
    done
    done
    square(1,0)=noSlipWall, userDefinedBoundaryData
      linear ramp in x
      3 1. 0. 0.
    done
    done
  done
  debug $debug
  initial conditions
    user defined
      * note that p = r R T = constant
      * p given below is the perturbation pressure
      bubbles
       1
      r=1. T=1.
       .175 .5 .7
      r=2. T=.5
    exit
 turn off twilight zone 
   exit
*
   implicit factor .5
   implicit time step solver options
     * choose best iterative solver
     * these tolerances are chosen for PETSc
     choose best iterative solver
     * PETScNew     
     relative tolerance
       $tol
     absolute tolerance
       1.e-8 
     debug 
      1 7 0 1 7 3
    exit
  continue

