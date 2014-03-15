#============================================================================
# Cgins: - Rayleigh Benard problem 
#
# Usage:
#     cgins benard -g=<grid> -nu=<> -ts=[pc|im|afs] -project=[0|1]
#
# Examples: 
#     cgins benard -g=benardGride4.order2 -ts=im -nu=.001 -tp=2. -tf=100.
# 
# NOTES:
#     - Scaling is MKS -- meters, Kilogram, s, (K)
#     - Coefficient of thermal expansion is 1/T (1/K) = 3.4e-3 (1/K) at 70F, 21C 
#     - nu = 1.5e-5 m^2/s 
#     - Pr =.713 = nu/kappa
#     - Thermal conductivity = .026 (at 20C)
#     - Cp = 1000 J/Kg-K
#     - rho = 1.21 Kg/m^3 at 20C
#     - kappa = k/(rho*Cp) = 2.6e-5 
# 
#=============================================================================
#
#
$grid="benardGride4.order2"; $show = " "; $tFinal=500.; $tPlot=.5; $nu=.1; $cfl=.9;
$Tbottom=10.; $flushFrequency=10; 
$adcBoussinesq=.5; # artificial dissipation coefficient for T
$Prandtl=.72; $thermalExpansivity=3.4e-3; 
$kThermal=-1;   # this is set below based o nu and Prandtl
$thermalConductivity=.026; # air at 20C 
$accelerationDueToGravity=9.81; 
$gravity = "0 -$accelerationDueToGravity 0."; 
# 
$slowStartSteps=-1; $slowStartCFL=.5; $slowStartRecomputeDt=100; $slowStartTime=-1.; $recomputeDt=10000;
# 
$implicitVariation="viscous"; $refactorFrequency=100; $impGrids="all=explicit"; $impFactor=.5; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; $newts=0; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.2; 
$order = 2; $fullSystem=0; $go="halt"; $amp=.5; $freq=1.; 
$show=" "; $restart="";
$ts="im"; $outflowOption="neumann"; 
$cdv=1.; $ad2=1; $ad21=.5; $ad22=.5;  $ad4=0; $ad41=.5; $ad42=.5; 
# $psolver="choose best iterative solver"; $solver="choose best iterative solver"; 
$psolver="yale"; $solver="yale"; 
$iluLevels=1; $ogesDebug=0; $project=0; 
$rtolp=1.e-4; $atolp=1.e-5;  # tolerances for the pressure solve
$rtol=1.e-7; $atol=1.e-8;    # tolerances for the implicit solver
# -- for Kyle's AF scheme:
$afit = 20;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$cdv=1;  $cDt=.25;
$ogmgAutoChoose=1; 
# 
$pi = 4.*atan2(1.,1.);
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,\
 "imp=f"=>\$implicitFactor,"adcBoussinesq=f"=>\$adcBoussinesq,"Tbottom=f"=>\$Tbottom,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"move=i"=>\$move,"moveOnly=i"=>\$moveOnly,\
  "amp=f"=>\$amp,"freq=f"=>\$freq, "solver=s"=>\$solver, "psolver=s"=>\$psolver,"uInflow=f"=>\$uInflow,\
  "outflowOption=s"=>\$outflowOption,"newts=i"=>\$newts,"ad4=i"=>\$ad4,"ad42=f"=>\$ad42,"ad41=f"=>\$ad41,\
  "slowStartCFL=f"=>\$slowStartCFL, "slowStartTime=f"=>\$slowStartTime,"recomputeDt=i"=>\$recomputeDt,\
  "slowStartSteps=i"=>\$slowStartSteps,"slowStartRecomputeDt=i"=>\$slowStartRecomputeDt,\
  "ogmgAutoChoose=i"=>\$ogmgAutoChoose,"flushFrequency=i"=>\$flushFrequency );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
#
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
# 
if( $ts eq "fe" ){ $ts="forward Euler";  }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       }
if( $ts eq "pc" ){ $ts="adams PC";       }
if( $ts eq "mid"){ $ts="midpoint";       }  
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;}
# 
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $project eq "1" && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
$kThermal=$nu/$Prandtl;
#
$grid
# 
  incompressible Navier Stokes
  Boussinesq model
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq $adcBoussinesq
  exit
#
  show file options
   compressed
     OBPSF:maximum number of parallel sub-files 8
   open
     $show
    frequency to flush
      $flushFrequency
  exit  
#
  turn off twilight zone
# 
  final time $tFinal
  times to plot $tPlot
  plot and always wait
# 
##  plot residuals 1
#
  $project
#
#*  useNewImplicitMethod
#  implicitFullLinearized
  implicit factor $impFactor
  dtMax $dtMax
# 
# use full implicit system 1
# use implicit time stepping
  $ts
  if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "*"; }
  $newts
  # -- for the AFS scheme:
  compact finite difference
  # -- convergence parameters for the af scheme
  max number of AF corrections $afit
  AF correction relative tol $aftol
  # optionally turn this on to improve stability of the high-order AF scheme by using 2nd-order dissipation at the boundary
  OBPDE:use boundary dissipation in AF scheme 1
#
  choose grids for implicit
   all=implicit
  # slipWall and Neuamnn outflow BC's are broken for this with: 
  ## backGround=explicit
  done
#
  cfl $cfl
# 
# 
  slow start cfl $slowStartCFL
  slow start steps $slowStartSteps
  slow start recompute dt $slowStartRecomputeDt
  slow start $slowStartTime   # (seconds)
#
  recompute dt every $recomputeDt
#
  pde parameters
    nu $nu
    kThermal $kThermal
    thermal conductivity $thermalConductivity
    gravity
      $gravity
    #  turn on 2nd-order AD here:
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22 $ad22, $ad22
    OBPDE:fourth-order artificial diffusion $ad4
    OBPDE:ad41,ad42 $ad41, $ad42
    if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
    $cmd
    OBPDE:expect inflow at outflow
  done
#*
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogmgDebug=$ogesDebug; $ogmgCoarseGridSolver="best"; $ogmgRtolcg=$rtolp; $ogmgAtolcg=$atolp; $ogmgRtolcg=$rtolp; $ogmgAtolcg=$atolp;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   # $ogmgSsr=1;  
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; $ogmgOpav=0; $ogmgRtolcg=1.e-6; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  boundary conditions
    all=slipWall
    backGround(0,1)=noSlipWall , uniform(T=$Tbottom)
    backGround(1,1)=noSlipWall , uniform(T=0.)
    square(0,1)=noSlipWall , uniform(T=$Tbottom)
    square(1,1)=noSlipWall , uniform(T=0.)
   done
#
  debug $debug
#
if( $restart eq "" ){ $cmds = "uniform flow\n u=0., v=0., p=1."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
#
  continue
#
$go 















*
* cgins - Rayleigh Benard problem 
*
$tFinal=1.; $tPlot=.1; $nu=.1; $kThermal=.1; $thermalExpansivity=1.; $Tbottom=10.; 
* 
* $grid = "square5.hdf"; $tFinal=1.; $tPlot=.01; 
* $grid = "benard1.hdf"; $tFinal=1.; $tPlot=.1; 
* $grid = "benard2.hdf"; $tFinal=1.; $tPlot=.1; 
$grid = "benard4.hdf"; $tFinal=1000.; $tPlot=1.; $nu=.005; $kThermal=.005;
* $grid = "benard6.hdf"; $tFinal=1000.; $tPlot=.5; $nu=.0025; $kThermal=.0025;
* $grid = "benard8.hdf"; $tFinal=1000.; $tPlot=2; $nu=.001; $kThermal=.001;
*
$grid
*
  incompressible Navier Stokes
  Boussinesq model
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
  exit
* 
  turn off twilight zone 
  final time $tFinal
  times to plot $tPlot
  * plot and always wait
  no plotting
  pde parameters
    nu  $nu
    kThermal $kThermal
    gravity
      0. -1. 0. 
   done
* 
  boundary conditions
    all=slipWall
    backGround(0,1)=noSlipWall , uniform(T=$Tbottom)
    backGround(1,1)=noSlipWall , uniform(T=0.)
    square(0,1)=noSlipWall , uniform(T=$Tbottom)
    square(1,1)=noSlipWall , uniform(T=0.)
   done
* ----------
  initial conditions
  uniform flow
    p=1., u=0., v=0., T=0. 
  exit
*   project initial conditions
* ----------
* 
  debug
    1 31
*   check error on ghost
*     1
 continue


 movie mode 
 finish
