*
* cgmp: solve for INS and AD in two domains
*     : incompressible flow past a heated cylinder (2d)
* 
$tFinal=2.; $tPlot=.1; $degreeSpace=2; $degreeTime=2; $cfl=.9; $show=""; 
$nu=.1; $Prandtl=.72; $thermalExpansivity=1.; $Twall=100.;  $kappa=.01; $rtol=1.e-4; $atol=1.e-6; 
$tz=0; # turn on tz here
$gravity = "0 -10. 0.";
$solver="choose best iterative solver";
$backGround="outerSquare"; 
* -- set the solver:
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=1; 
* $ksp="bcgs"; $pc="hypre"; 
* 
$grid="innerOuter2d.hdf"; $backGround="outerSquare";  $tPlot=.1; $nu=.02; $kappa=.1; 
* $grid="innerOuter2d.hdf"; $degreeSpace=1; $degreeTime=0; $solver="yale"; $backGround="outerSquare";  $tPlot=.1;
* $grid="innerOuter3d.hdf"; $backGround="outerBox"; $tPlot=.01;
* $grid="innerOuter3d2.hdf"; $backGround="outerBox"; $tPlot=.01; $nu=.05; 
* $grid="diskArray2x2yi1.order2.hdf"; $solver="yale"; $nu=.05; $kappa=.1; $tPlot=.1; 
* $grid="diskArray2x2ye2.order2.hdf"; $solver="yale"; $tPlot=.1; $nu=.02; $kappa=.05; $solver="yale"; $show="diskArray.show";
* $grid="diskArray2x2ye4.order2.hdf"; $solver="yale"; $tPlot=.1; $nu=.005; $kappa=.0075; $solver="yale"; $show="diskArray.show";
* $grid="diskArray3x3yi1.order2.hdf"; $solver="yale"; $backGround="outerSquare"; $tPlot=.1; 
*
$grid
* 
$kThermal=$nu/$Prandtl; 
* 
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
   done
* 
    maximum number of iterations for implicit interpolation
      10
  pressure solver options
     $solver
     * yale
     * these tolerances are chosen for PETSc
     define petscOption -ksp_type $ksp
     define petscOption -pc_type $pc
     define petscOption -sub_ksp_type $subksp
     define petscOption -sub_pc_type $subpc
     define petscOption -pc_factor_levels $iluLevels
     define petscOption -sub_pc_factor_levels $iluLevels
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
    all=noSlipWall
    $backGround(0,1)=inflowWithVelocityGiven, uniform(p=1.,u=0.,v=.1)
    $backGround(1,1)=outflow
    bcNumber100=interfaceBoundaryCondition
    * bcNumber100=dirichletBoundaryCondition
  done
* 
  initial conditions
  if( $tz eq "0" ){ $commands="uniform flow\n" . "p=1., u=0.\n" . "continue"; }else{ $commands="continue";}
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
    all=dirichletBoundaryCondition, uniform(T=$Twall)
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
      $a=$Twall; $b=30.; $p=2.; $x0=0.; $y0=0.; 
      * add 1 source term
      1 
      0 0 $a $b $p $x0 $y0 0
     * 4 sources
*      4 
*       0 0 -$a $b $p -.5 -.5 0
*       1 0 -$a $b $p  .5 -.5 0
*       2 0  $a $b $p -.5  .5 0
*       3 0  $a $b $p  .5  .5 0
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
  show file options
    compressed
      open
       $show
    frequency to flush
      2
    exit
  continue
*
      plot:fluid : T
      plot:solid : T

movie mode
finish


      contour
        min max 0 $Twall
        exit
        min max 0 $Twall
        exit


  movie mode
  finish
