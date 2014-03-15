*
* cgmp: solve for INS and AD in two domains
*     : incompressible flow past a heated cylinder (3d)
* 
*  srun -N1 -n4 -ppdebug $cgmpp tube
*
$tFinal=2.; $tPlot=.1; $degreeSpace=2; $degreeTime=2; $cfl=.9; $show=""; 
$nu=.1; $Prandtl=.72; $thermalExpansivity=1.; $Twall=100.;  $kappa=.01; $rtol=1.e-4; $atol=1.e-6; 
$w0=-1.; # background flow
$tz=0; # turn on tz here
$gravity = "0 0 -10.";
$solver="choose best iterative solver";
$backGround="outerSquare"; 
* 
* $grid="rodArray1x1ye1.order2.hdf"; $nu=.05; $kappa=.05; $tFinal=.1; $tPlot=.02; $show="tube.show"; 
$grid="rodArray2x2ye1.order2.hdf"; $nu=.05; $kappa=.025; $tFinal=.5; $tPlot=.1; $rtol=1.e-3; $atol=1.e-5;
* $grid="rodArray2x2ye2.order2.hdf"; $nu=.025; $kappa=.025; $tPlot=.01; $rtol=1.e-3; $atol=1.e-5; 
* 6.6M pts:
* $grid="rodArray2x2ye4.order2.hdf"; $nu=.005; $kappa=.005; $tPlot=.1; $rtol=1.e-3; $atol=1.e-5; $show="tube2x2y4.show"; 
* $grid="rodArray3x3ye1.order2.hdf"; $nu=.005; $kappa=.005; $tFinal=.2; $tPlot=.05; $rtol=1.e-3; $atol=1.e-5; 
*  $grid="rodArray3x3ye2.order2.hdf"; $nu=.01; $kappa=.01; $tFinal=5.; $tPlot=.2; $rtol=1.e-3; $atol=1.e-5; $show="tube3x3e2.show"; 
* $grid="rodArray3x3ye4.order2.hdf"; $nu=.005; $kappa=.005; $tPlot=.1; $rtol=1.e-3; $atol=1.e-5; $show="tube3x3y4.show"; 
* 
*
$grid
* 
$kThermal=$nu/$Prandtl; 
* ------- start new domain ----------
setup outerDomain
 set solver Cgins
 solver name fluid
 solver parameters
* 
  incompressible Navier Stokes
  Boussinesq model
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
  pde parameters
    nu  $nu
    kThermal $kThermal
    thermal conductivity $kThermal
    gravity
      $gravity
     turn on second order artificial diffusion
     turn on fourth order artificial diffusion
   done
* 
    maximum number of iterations for implicit interpolation
      10
  pressure solver options
     $solver
     * yale
     * these tolerances are chosen for PETSc
     relative tolerance
       $rtol 
     absolute tolerance
       $atol
     debug 
       0
    exit
* 
  boundary conditions
    all=noSlipWall, uniform(T=0.)
    $backGround=slipWall
    * bcNumber4=inflowWithVelocityGiven, parabolic(d=.2,p=1.,w=$w0)
    bcNumber4=inflowWithVelocityGiven, uniform(d=.2,p=1.,w=$w0)
    bcNumber3=outflow
    * bcNumber5=noSlipWall, uniform(T=$Twall)
    bcNumber100=interfaceBoundaryCondition
    * bcNumber100=dirichletBoundaryCondition
  done
* 
  initial conditions
  if( $tz eq "0" ){ $commands="uniform flow\n" . "p=1., u=0., w=$w0\n" . "continue"; }else{ $commands="continue";}
    $commands
* 
  continue
done
* 
* ------- start new domain ----------
setup innerDomain
 set solver Cgad
 solver name solid
 solver parameters
* 
  convection diffusion
  continue
* 
  pde parameters
    kappa $kappa
    thermal conductivity $kappa
  done
* 
  forward Euler
  boundary conditions
    all=dirichletBoundaryCondition, uniform(T=0.)
    bcNumber3=neumannBoundaryCondition
    $backGround=neumannBoundaryCondition
    bcNumber100=interfaceBoundaryCondition
    * bcNumber100=dirichletBoundaryCondition
    done
*
  OBTZ:twilight zone flow $tz
  degree in space $degreeSpace
  degree in time $degreeTime
  initial conditions
  if( $tz eq "0" ){ $commands="uniform flow\n" . "T=0.\n" . "continue"; }else{ $commands="continue";}
    $commands
* 
*  Here is the source term
  user defined forcing
    gaussian forcing
      $a=$Twall; $b=30.; $p=2.; $x0=0.; $y0=0.; $z0=.5; 
      * add 1 source term
*       1 
*       0 0 $a $b $p $x0 $y0 $z0
     * 4 sources
       4 
       0 0 -$a $b $p -.5 -.5 $z0
       1 0 -$a $b $p  .5 -.5 $z0
       2 0  $a $b $p -.5  .5 $z0
       3 0  $a $b $p  .5  .5 $z0
     * 9 sources
*        $z0=1.5; 
*        9 
*        0 0  $a $b $p -1.0 -1.0 $z0
*        1 0  $a $b $p   .0 -1.0 $z0
*        2 0  $a $b $p  1.0 -1.0 $z0
*        3 0  $a $b $p -1.0   .0 $z0
*        4 0  $a $b $p   .0   .0 $z0
*        5 0  $a $b $p  1.0   .0 $z0
*        6 0  $a $b $p -1.0  1.0 $z0
*        7 0  $a $b $p   .0  1.0 $z0
*        8 0  $a $b $p  1.0  1.0 $z0
    done
  exit
  continue
done
continue
*
* -- set parameters for cgmp ---
* 
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  forward Euler
  OBTZ:twilight zone flow $tz
* 
  show file options
    compressed
      open
       $show
    frequency to flush
      * there are 2 frames per solution for 2 domains
      2
    exit
  continue
*
      plot:fluid : T
      plot:solid : T
* 

movie mode
finish


      contour
        min max 0 $Twall
        exit
        min max 0 $Twall
        exit


  movie mode
  finish
