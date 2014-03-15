*
* cgmp: solve for INS and AD in two domains
*     : incompressible flow past a heated cylinder (3d)
* 
$tFinal=1.; $tPlot=.1; $degreeSpace=2; $degreeTime=2; $cfl=.9; $show=""; 
$nu=.1; $Prandtl=.72; $ktcFluid=.1; 
$kappa=.01; $ktcSolid=.05; $thermalExpansivity=1.; $Twall=10.;   
$w0=-1.; # background flow
$tz=0; # turn on tz here
* $gravity = "0 -1. 0.";
$gravity = "0. 0. 0.";
$solver="choose best iterative solver"; $rtol=1.e-3; $atol=1.e-4; 
* $solver="yale";
$backGround="outerSquare"; 
* 
$grid="fuelAssembly1pinsi1.order2.hdf"; $nu=.05*$Prandtl; $kappa=.05; $tFinal=1.; $tPlot=.05; $show="fuelPins.show"; 
* 
* $grid="fuelAssembly1pins2.order2.hdf"; $nu=.05; $kappa=.05; $tFinal=.1; $tPlot=.02; $show=" "; 
* $grid="fuelAssembly3pins1.order2.hdf"; $nu=.05; $kappa=.05; $tFinal=.1; $tPlot=.02; $show=" "; 
* $grid="fuelAssembly3pins2.order2.hdf"; $nu=.05; $kappa=.05; $tFinal=.1; $tPlot=.02; $show=" "; 
* $grid="fuelAssembly5pins1.order2.hdf"; $nu=.05; $kappa=.05; $tPlot=.05; $show=" "; 
* $grid="fuelAssembly9pins1.order2.hdf"; $nu=.05; $kappa=.05; $tPlot=.05; $show=" "; 
* $grid="fuelAssembly11pins1.order2.hdf"; $nu=.05; $kappa=.05; $tPlot=.05; $show=" "; 
* $grid="fuelAssembly17pins1.order2.hdf"; $nu=.05; $kappa=.05; $tFinal=.5; $tPlot=.05; $show="fuelPins17n.show"; 
* ogen noplot rodArray -factor=1 -nCylx=2 -nCyly=2 -interp=e : 
$grid="rodArray2x2ye1.order2.hdf"; $nu=.05; $kappa=.025; $tFinal=.5; $tPlot=.1; $rtol=1.e-3; $atol=1.e-5; $show="tube2x2e1.show"; 
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
    thermal conductivity $ktcFluid
    gravity
      $gravity
     turn on second order artificial diffusion
     OBPDE:ad21,ad22 2,2
     * turn on fourth order artificial diffusion
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
    * $backGround=slipWall
    * bcNumber4=inflowWithVelocityGiven, parabolic(d=.2,p=1.,w=$w0)
    *   bcNumber4=inflowWithVelocityGiven, uniform(d=.2,p=1.,w=$w0)
    * bcNumber3=outflow
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
    thermal conductivity $ktcSolid
  done
* 
  forward Euler
  OBTZ:twilight zone flow $tz
  boundary conditions
    * all=dirichletBoundaryCondition, uniform(T=0.)
    all=neumannBoundaryCondition
    * bcNumber3=neumannBoundaryCondition
    * $backGround=neumannBoundaryCondition
    bcNumber100=interfaceBoundaryCondition
    * bcNumber100=dirichletBoundaryCondition
    done
* 
    maximum number of iterations for implicit interpolation
      10
*
  degree in space $degreeSpace
  degree in time $degreeTime
  initial conditions
  if( $tz eq "0" ){ $commands="uniform flow\n" . "T=$Twall\n" . "continue"; }else{ $commands="continue";}
    $commands
* 
  continue
done 
* 
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
continue


movie mode
finish


      contour
        min max 0 $Twall
        exit
        min max 0 $Twall
        exit


  movie mode
  finish
