* 
* cgmp example: solve the axisymmetric incompressible N-S equations in two domains 
*
* Usage:
*    cgmp [-noplot] ii -g=<name> -nu1=<> -nu2=<> -kThermal1=<> -kThermal2=<> -tf=<tFinal> -tp=<tPlot> ...
*             -ktc1=<> -ktc2=<> -solver=<yale/best> -domain1=<name> -domain2=<name> ...
*             -nc=<num> -degreex[1/2]=<num> -degreet[1/2]=<num> -tz=[0/1] -bg=<backGroundGrid> -ts=<fe/be/im>
* where
*  -ts = time-stepping method, fe="forward Euler", be="backward Euler", mid="mid-point" im="implicit"
*  -nc : number of correction steps for implicit predictor-corrector
* 
* Examples:  (use the ogen script hio.cmd to build hioi2.order2.hdf)
* 
*  cgmp axisymSphere -g=hioi2.order2.hdf -nu=.2 -kThermal1=.2 -nu2=.1 -kThermal2=.1 -tf=1. -tp=.05 -go=halt
*
* --- set default values for parameters ---
* 
$tFinal=2.; $tPlot=.1; $debug=0; $cfl=.9; $dtMax=.1; $go="og"; 
$grid="hioi2.order2.hdf";
$domain1="outerDomain"; $domain2="innerDomain";
$show = " "; $ghost=0; $show=""; $numberOfCorrections=1; 
$gravity = "-10 0. 0."; # gravity must be in the x-direction for axisymetric flow
$solver="yale";  $rtol=1.e-10; $atol=1.e-12;  
* -- set the solver:
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=1; 
$ts="forward Euler";  # mp solver
$tsd="forward Euler"; # domain solver
$degreex1=2; $degreet1=1; $a1=0.; $b1=0.; 
$degreex2=2; $degreet2=1; $a2=0.; $b2=0.; 
$nu1=.1; $nu2=.2; $kThermal1=.1; $kThermal2=.2; $thermalExpansivity=.1; $T1=0.; $T2=1.; 
$ktc1=-1.; $ktc2=-1.;   # by default set ktc equal to kThermal
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"kThermal1=f"=>\$kThermal1,"kThermal2=f"=>\$kThermal2, "bg=s"=>\$backGround,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz,"degreex1=i"=>\$degreex1, "degreet1=i"=>\$degreet1,\
 "degreex2=i"=>\$degreex2, "degreet2=i"=>\$degreet2,"show=s"=>\$show,"ts=s"=>\$ts,"go=s"=>\$go,\
 "debug=i"=>\$debug,"nc=i"=> \$numberOfCorrections,"noplot=s"=>\$noplot,\
 "domain1=s"=>\$domain1,"domain2=s"=>\$domain2,"ktc1=f"=>\$ktc1,"ktc2=f"=>\$ktc2 );
* -------------------------------------------------------------------------------------------------
if( $ktc1 < 0. ){ $ktc1=$kThermal1; }if( $ktc2 < 0. ){ $ktc2=$kThermal2; }
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $ts eq "fe" ){ $ts="forward Euler";  $tsd="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; $tsd="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       $tsd="implicit";  }
if( $ts eq "mid"){ $ts="midpoint";       $tsd="forward Euler"; }  # the midpoint rule uses forward-euler on each domain
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
* 
* $ksp="bcgs"; $pc="hypre"; 
* 
*
* $grid="twoSquaresInterface1e.hdf"; $degreeX=1; $degreeT=1; $debug=0;
* $grid="innerOuter2d.hdf"; $degreeX=1; $degreeT=0; $debug=0; $tPlot=.01; $domain1="outerDomain"; $domain2="innerDomain";
* $grid="innerOuter3d.hdf"; $degreeX=1; $degreeT=1; $debug=0; $tPlot=.01; 
*
$grid
* 
*  where should we put cgmp parameters ??
  $iTol=1.e-3; $iOmega=.7; 
  define real parameter interfaceTolerance $iTol
  define real parameter interfaceOmega $iOmega
* ------- start new domain ----------
setup $domain1
 set solver Cgins
 solver name fluidA
 solver parameters
*
  incompressible Navier Stokes
  Boussinesq model
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
  continue
* 
  $tsd
  dtMax $dtMax
*
  turn on axisymmetric flow
* 
  OBTZ:polynomial
  OBTZ:twilight zone flow 0
  OBTZ:degree in space $degreex1
  OBTZ:degree in time $degreet1
  boundary conditions
    all=noSlipWall
    bcNumber100=interfaceBoundaryCondition
    bcNumber3=axisymmetric
  done
  pde parameters
    nu $nu1
    kThermal $kThermal1
    thermal conductivity $ktc1
    gravity
      $gravity
  done
  pressure solver options
     $solver
     * yale
     * these tolerances are chosen for PETSc
     * these tolerances are chosen for PETSc
     define petscOption -ksp_type $ksp
     define petscOption -pc_type $pc
     define petscOption -sub_ksp_type $subksp
     define petscOption -sub_pc_type $subpc
     define petscOption -pc_factor_levels $iluLevels
     define petscOption -sub_pc_factor_levels $iluLevels
     relative tolerance
       $rtol
     absolute tolerance
       $atol
     debug 
       0
    exit
  initial conditions
   uniform flow
     p=1., u=1. T=$T1
  done
  debug $debug
  continue
done
* 
* ------- start new domain ----------
setup $domain2
 set solver Cgins
 solver name fluidB
 solver parameters
* 
  incompressible Navier Stokes
  Boussinesq model
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
  continue
* 
* 
  $tsd
  dtMax $dtMax
*
  turn on axisymmetric flow
* 
  OBTZ:polynomial
  OBTZ:twilight zone flow 0 
  OBTZ:degree in space $degreex2
  OBTZ:degree in time $degreet2
  boundary conditions
    all=noSlipWall
    bcNumber100=interfaceBoundaryCondition
    bcNumber3=axisymmetric
  done
  pde parameters
    nu $nu2
    kThermal $kThermal2
    thermal conductivity $ktc2
    gravity
      $gravity
  done
  pressure solver options
     $solver
     * yale
     * these tolerances are chosen for PETSc
     * these tolerances are chosen for PETSc
     define petscOption -ksp_type $ksp
     define petscOption -pc_type $pc
     define petscOption -sub_ksp_type $subksp
     define petscOption -sub_pc_type $subpc
     define petscOption -pc_factor_levels $iluLevels
     define petscOption -sub_pc_factor_levels $iluLevels
     relative tolerance
       $rtol
     absolute tolerance
       $atol
     debug 
       0
    exit
  initial conditions
   uniform flow
     p=1., u=1. T=$T2
  done
  debug $debug
  continue
done
*
continue
*
* -- set parameters for cgmp ---
* 
  final time $tFinal
  times to plot $tPlot
* 
  $ts 
  OBTZ:twilight zone flow 0 
  number of PC corrections $numberOfCorrections
* 
* 
  continue
continue
plot:fluidA : T
plot:fluidB : T
$go





*
* cgins example: axisymmetric flow past a half annulus (ie. flow past a sphere)
*
* specify the overlapping grid to use:
halfCylinder.hdf
* Specify the equations we solve:
  incompressible Navier Stokes
  done
  turn off twilight zone 
* choose implicit time stepping:
*   implicit
* but integrate the square explicitly:
  choose grids for implicit
    all=implicit
    square=explicit
    done
  final time (tf=)
    10.
  times to plot (tp=)
    .1
  plot and always wait
  * no plotting
  pde parameters
    nu
      .05
    turn on second order artificial diffusion
*    divergence damping
*     .5
  done
    turn on axisymmetric flow
*   cfl
*    .2
  boundary conditions
    all=slipWall
    square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.)
    square(1,0)=outflow
    square(0,1)=axisymmetric
    annulus(0,0)=axisymmetric
    annulus(1,0)=axisymmetric
    annulus(0,1)=noSlipWall
    done
  initial conditions
   uniform flow
     p=1., u=1.
  done
  project initial conditions
  exit
