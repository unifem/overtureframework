*
* cgins example: two-phase flow example: light or heavy bubble in a gravitation field
*
* Usage:
*   
*  cgins [-noplot] bubble -g=[name] -tz=[poly/trig/none] -degreex=[] -degreet=[] -tf=[tFinal] -tp=[tPlot] ...
*        -bc=[a|d|r]  -solver=[yale/best] -order=[2/4] -model=[ins/boussinesq] -ts=[implicit] -debug=[num] ..,
*        -ad2=[0|1] -bg=[backGround] -project=[0/1] -iv=[viscous/adv/full] -imp=[val] -rf=[val] ...
*        -tpo=[0|1] -av1=[] -av2=[] -go=[run/halt/og]
* 
*  -iv : implicit variation : viscous=viscous terms implicit, adv=viscous + advection, full=full linearized version
*  -imp : .5=CN, 1.=BE, 0.=FE
*  -rf : refactor frequency
*  -go : run, halt, og=open graphics
*  -bc : d=dirichlet, a=dirichlet+bubblemetric, r=real BC's  
*  -av1: artificial dissipation coeff for psi
*  -av2: artificial dissipation coeff for phi 
*  -ad2 : turn on or off the 2nd order artificial dissipation 
*  -rho1, -rho2 : specify rho in the two phases
*  -tpo= : (two-phase option) : 0=rho is smoothed out over time,  1=use Heaviside definition of rho
* 
* Examples: (offsetSquare grids are made with rectangle.cmd)
* 
*  cgins bubble -g=square40.hdf -bg=square -nu=.1 -tf=.2 -tp=.01 -av1=.01 -av2=.01 -go=halt -cfl=.75 
*  cgins bubble -g=square20.hdf -bg=square -nu=.1 -tf=.2 -tp=.01 -av1=.01 -av2=.01 -go=halt -cfl=.75 -rho2=1.
* 
* implicit: 
*  cgins bubble -g=square40.hdf -bg=square -nu=.1 -tf=2. -tp=.1 -av1=.01 -av2=.01 -go=halt  -ts=implicit -dtMax=.05 -rf=5
*  cgins bubble -g=square80.hdf -bg=square -nu=.1 -tf=2. -tp=.05 -av1=.005 -av2=.005 -go=halt -ts=implicit -dtMax=.05 -rf=5 -tpo=1
* 
* --- set default values for parameters ---
* 
$grid="square20.hdf"; $backGround="square"; 
$tFinal=1.; $tPlot=.1; $cfl=.9; $nu=.05; $Prandtl=.72; $thermalExpansivity=.1; 
$gravity = "0. -10. 0."; $rho1=1.; $rho2=2.; $mu1=.1; $mu2=.1; $av1=.1; $av2=.1; $rhot=0
$model="ins"; $ts="adams PC"; $noplot=""; $implicitVariation="full"; $refactorFrequency=100; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; $show=" ";
$solver="yale"; $ogesDebug=0; $project=0; $cdv=1.; $ad2=0; $ad22=2.; 
$twoPhaseOption=0;   # 0= rho=smooth, 1= rho=jump 
$bc="a"; 
* 
$psolver="best"; $solver="best"; $rtol=1.e-4; $atol=.1e-6; $iluLevels=1;
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
  "bc=s"=>\$bc,"gravity=s"=>\$gravity,"rho1=f"=>\$rho1,"rho2=f"=>\$rho2,\
  "mu1=f"=>\$mu1,"mu2=f"=>\$mu2,"av1=f"=>\$av1,"av2=f"=>\$av2,"tpo=i"=>\$twoPhaseOption );
* -------------------------------------------------------------------------------------------------
$kThermal=$nu/$Prandtl; 
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
if( $model eq "boussinesq" ){ $model = "incompressible Navier Stokes\n Boussinesq model"; }
if( $model eq "tp" ){ $model = "incompressible Navier Stokes\n two-phase flow model"; }
* 
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $project eq "1" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
* specify the overlapping grid to use:
$grid
* Specify the equations we solve:
* 
  incompressible Navier Stokes
  two-phase flow model
  define real parameter twoPhaseRho1 $rho1
  define real parameter twoPhaseRho2 $rho2
  define real parameter twoPhaseMu1  $mu1
  define real parameter twoPhaseMu2  $mu2
  define real parameter twoPhaseArtDisPsi $av1
  define real parameter twoPhaseArtDisPhi $av2
  define real parameter twoPhaseOption $twoPhaseOption
* 
  done
* 
  show file options
     compressed
      open
       $show
    frequency to flush
      100
    exit
* -- twilightzone options:
  $tz
  degree in space $degreex
  degree in time $degreet
  frequencies (x,y,z,t)   $fx $fy $fz $ft
* 
* choose the time stepping:
  $ts
* 
  useNewImplicitMethod
  $implicitVariation
  refactor frequency $refactorFrequency
  choose grids for implicit
    all=implicit
*     square=explicit
    done
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  dtMax $dtMax
* 
* 
  plot and always wait
  * no plotting
  pde parameters
    nu $nu
    kThermal $kThermal
    gravity
      $gravity
* 
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22  $ad22, $ad22
    OBPDE:divergence damping  $cdv 
  done
* 
  boundary conditions
    all=noSlipWall
*    square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.)
*    square(1,0)=outflow
  done 
**
  pressure solver options
     $psolver
     * yale
     * these tolerances are chosen for PETSc
     number of incomplete LU levels
       $iluLevels
     * do these next two work?
     define petscOption -pc_factor_levels $iluLevels
     define petscOption -sub_pc_factor_levels $iluLevels
     relative tolerance
       $rtol 
     absolute tolerance
       $atol
    exit
  implicit time step solver options
     * choose best iterative solver
     $solver
     * 
     number of incomplete LU levels
       $iluLevels
     define petscOption -pc_factor_levels $iluLevels
     define petscOption -sub_pc_factor_levels $iluLevels
     * these tolerances are chosen for PETSc
     relative tolerance
       $rtol
     absolute tolerance
       $atol
     * debug 
     *   3
    exit
*
* 
  initial conditions
    user defined
      * note that p = r R T = constant
      * p given below is the perturbation pressure
      bubbles
       1
      u=0. v=0. psi=0.
      * radius, center: 
       .175 .5 .5
      u=0 v=0. psi=1.
    exit
*   if( $tz eq "turn off twilight zone" ){ $ic = "uniform flow\n p=1., u=1., T=0. \n done";}else{ $ic = "done"; }
*    $ic 
  done
  debug $debug
  $project
  exit
  $go
