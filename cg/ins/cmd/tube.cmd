*
* cgins: flow along an array of tubes in a box (Boussinesq)
*       
* 
$tFinal=20.; $tPlot=.1; $degreeSpace=2; $degreeTime=2; $show=" "; 
$nu=.1; $kThermal=.1; $thermalExpansivity=.1; $Twall=1.;  $kappa=.1; $Prandtl=.72; $rtol=1.e-4; $atol=1.e-5; 
$tz=0; # turn on tz here
$gravity = "0  0. 10.";
$solver="choose best iterative solver";
* $solver="yale";
* 
* $grid="cylBox2eZ2.hdf"; $tPlot=.1; $nu=.05; $backGround="box"; 
$grid="cylArray2x2ye1.order2.hdf"; $tPlot=.1; $nu=.05; $backGround="backGround"; 
$grid="cylArray3x3ye1.order2.hdf"; $tPlot=.1; $nu=.05; $backGround="backGround"; 
* 
*
$kThermal=$nu/$Prandtl;
*
$grid
* 
  incompressible Navier Stokes
  Boussinesq model
  define real parameter kappa $kThermal
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
  continue
* 
  OBTZ:polynomial
  OBTZ:twilight zone flow $tz
  degree in space $degreeSpace
  degree in time $degreeTime
* 
  forward Euler
* 
*
  final time $tFinal
  times to plot $tPlot
  pde parameters
    nu  $nu
    kThermal $kThermal
    gravity
      $gravity
   done
* 
* 
  show file options
    compressed
      open
      $show
    frequency to flush
     4
    exit
***
* choose implicit time stepping:
  implicit
* but integrate the square explicitly:
  choose grids for implicit
    all=implicit
    $backGround=explicit
    done
***
  pressure solver options
     $solver
     * yale
     * these tolerances are chosen for PETSc
     relative tolerance
       $rtol 
     absolute tolerance
       $atol
    exit
  implicit time step solver options
     choose best iterative solver
     * PETSc
     * these tolerances are chosen for PETSc
     relative tolerance
       1.e-5
     absolute tolerance
       1.e-7
    exit
* 
  boundary conditions
    $backGround=slipWall
    all=noSlipWall, uniform(T=0.)
    bcNumber4=inflowWithVelocityGiven, parabolic(d=.2,p=1.,w=-1.)
*     bcNumber4=inflowWithVelocityGiven, uniform(d=.2,p=1.,w=-1.)
    bcNumber3=outflow
    bcNumber5=noSlipWall, uniform(T=$Twall)
  done
* 
  initial conditions
  if( $tz eq "0" ){ $commands="uniform flow\n" . "p=1., w=-1. T=0.\n" . "continue"; }else{ $commands="continue";}
    $commands
* 
  continue
