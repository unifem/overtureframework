# *******************************************************
# cgins command file: 
#     Flow over a non-uniform surface 
#
# Usage:
#    cgins [-noplot] surfaceFlow -g=<gridName> -ts=[pc|im] -tf=<> -tp=<> -nu=<> -solver=[best|yale|mg] ...
#                    -psolver=[best|yale|mg] -model=[b|vp] -tm=[les] -gravity=<> -surfaceTemp=<>
# 
# Parameters:  
#  nu : viscosity
#  tf : final time
#  tp : time to plot
#  ts : time-stepping method, pc=explicit predictor-corrector, im=implicit predictor-corrector
#  gravity : e.g. set to "0 -1. 0" to turn on 
#  surfaceTemp : surface temperature
#  tm : turbulence model (les)
#
# Examples:
#   cgins surfaceFlow -g=channelWithBumpe5.order2.ml2 -ts=pc -nu=1.e-3 -ad2=1 -tf=20. -tp=.1 -show="channelWithBump5.show" -go=halt
#   cgins surfaceFlow -g=channelWithBumpe5.order2.ml2 -ts=im -nu=1.e-3 -ad2=1 -tf=20. -tp=.1 -show="channelWithBump5.show" -go=halt
#   cgins surfaceFlow -g=channelWithBumpe10.order2.ml3 -nu=.5e-3 -ad2=1 -tf=20. -tp=1. -show="channelWithBumpe10.show" -go=halt
# 
# -- MG
#  cgins surfaceFlow -g=channelWithBump5.order2.ml2 -order=2 -nu=1.e-3 -tf=20. -tp=.05 -psolver=mg -rtolp=1.e-5 -debug=3  -go=halt
#  cgins surfaceFlow -g=channelWithBump10.order2.ml3 -order=2 -nu=.5e-3 -tf=20. -tp=.05 -psolver=mg -rtolp=1.e-5 -debug=3  -go=halt
#    -- implicit with MG -- ok with Neumann BC at outflow (fix line solver extrap BC)
#  cgins surfaceFlow -g=channelWithBumpe10.order2.ml3 -order=2 -nu=.5e-3 -tf=20. -tp=.05 -psolver=mg -rtolp=1.e-5 -ts=im -solver=mg -rtol=1.e-5 -debug=3  -go=halt
#  cgins surfaceFlow -g=channelWithBump20.order2.ml3 -order=2 -nu=1.e-4 -ad2=1 -tf=20. -tp=.05 -psolver=mg -rtolp=1.e-6 -debug=3  -go=halt
#   Bump40: 2.3M explicit  hmin=6.3e-4 dt=4.e-4 -> 3.e-4
#  cgins surfaceFlow -g=channelWithBump40.order2.ml4 -order=2 -nu=.5e-4 -ad2=1 -tf=20. -tp=.05 -psolver=mg -rtolp=1.e-7 -debug=3  -go=halt
# 
# -- order=4 -- trouble at inflow bottom corner (?)
#    cgins surfaceFlow -g=channelWithBump5.order4.ml2 -order=4 -nu=1.e-3 -tf=20. -tp=.001 -debug=3 -project=1 -go=halt
# 
# -- parallel 
#   mpirun -np 4 $cginsp surfaceFlow -g=channelWithBumpe20.order2.ml3 -nu=.5e-3 -tf=20. -tp=.01 -psolver=mg -rtolp=1.e-5 -go=halt
# -- restart
#   cgins surfaceFlow -g=channelWithBumpe40.order2.ml4 -nu=1.e-4 -ad2=1 -tf=20. -tp=.001 -psolver=mg -rtolp=1.e-6 -show="channelWithBump40b.show" -restart="channelWithBump40.show" -restartSolution=24 -debug=3 -ogesDebug=3 -go=halt
# 
# - LES model:
#  cgins surfaceFlow -g=channelWithBumpe5.order2.ml2 -tm=les -lesOption=1 -lesPar1=.001 -ts=pc -nu=1.e-3 -ad2=0 -tf=20. -tp=.1 -go=halt
# 
# - Boussinesq:
#  cgins surfaceFlow -g=channelWithBumpe5.order2.ml2 -model=b -ts=pc -nu=1.e-3 -ad2=0 -tf=20. -tp=.1 -gravity="0. -1. 0." -surfaceTemp=1. -go=halt
#    -- LES + Boussinesq
#  cgins surfaceFlow -g=channelWithBumpe5.order2.ml2 -model=b -tm=les -lesOption=0 -ts=pc -nu=1.e-3 -ad2=0 -tf=20. -tp=.01 -gravity="0. -1. 0." -surfaceTemp=1. -go=halt
#  cgins surfaceFlow -g=channelWithBumpe5.order2.ml2 -model=b -tm=les -lesOption=1 -lesPar1=1.e-5 -ts=pc -nu=1.e-3 -ad2=0 -tf=20. -tp=.01 -gravity="0. -1. 0." -surfaceTemp=1. -go=halt
#
# -- assign default values for all parameters: 
$tFinal=30.; $tPlot=.1; $nu=.1; $show=" "; $debug=0; $ogesDebug=0; $debugmg=0; $dtMax=.02; $order=2; $newts=0;
$restart=""; $restartSolution=-1; $outflowOption="neumann";
# 
$slowStartSteps=-1; $slowStartCFL=.5; $slowStartRecomputeDt=100; $slowStartTime=-1.; $recomputeDt=10000;
#
$apn=1.; # coeff in outflow BC : p + apn*p.n = 0 
$rtol=1.e-4; $atol=1.e-4; $solver="best"; 
$rtolp=1.e-3; $atolp=1.e-4; $psolver="best"; $iluLevels=3; 
$project=1; $ts="pc"; $go="halt";
$ad2=0; $ad21=1.; $ad22=1.;   # for 2nd-order artificial dissipation, ad2=0 means no dissipation
$ad4=0; $ad41=1.; $ad42=1.;   # for 4th-order artificial dissipation, ad4=0 means no dissipation
$model="#";  $gravity = "0. 0. 0."; $thermalExpansivity=.1;
$tm = "#"; 
$surfaceTemp=0.; # surface temperature
$lesOption=0; $lesPar1=.01; 
# -- for Kyle's AF scheme:
$afit = 20;
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2;
$cdv=1;  $cDt=.25;
$ogmgAutoChoose=2; $ogmgCoarseGridSolver="best";
$ogmgCoarseGridMaxIterations=1000; 
$ogmgSaveGrid=""; $ogmgReadGrid=""; # save or read a MG grid with levels built
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"tp=f"=>\$tPlot,"u0=f"=>\$u0,"u1=f"=>\$u1,"u2=f"=>\$u2,"ax=f"=>\$ax, "ay=f"=>\$ay,\
 "axp=f"=>\$axp,"pgf=f"=>\$pgf, "model=s"=>\$model,"tm=s"=>\$tm,"apn=f"=>\$apn,\
 "solver=s"=>\$solver, "psolver=s"=>\$psolver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"gravity=s"=>\$gravity,\
  "ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,"restartSolution=i"=>\$restartSolution,\
 "imp=f"=>\$implicitFactor,"rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,\
  "restart=s"=>\$restart,"move=s"=>\$move,"debugmg=i"=>\$debugmg,"nullVector=s"=>\$nullVector,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"outflowOption=s"=>\$outflowOption,\
  "bg=s"=>\$bg,"gridToMove=s"=>\$gridToMove,"bcTop=s"=>\$bcTop,"ogesDebug=i"=>\$ogesDebug,"rate=f"=>\$rate,\
  "surfaceTemp=f"=>\$surfaceTemp,"lesOption=i"=>\$lesOption,"lesPar1=f"=>\$lesPar1,"slowStartCFL=f"=>\$slowStartCFL,\
  "slowStartTime=f"=>\$slowStartTime,"ogmgCoarseGridSolver=s"=>\$ogmgCoarseGridSolver,"aftol=f"=>\$aftol,\
  "recomputeDt=i"=>\$recomputeDt,"slowStartSteps=i"=>\$slowStartSteps,"slowStartRecomputeDt=i"=>\$slowStartRecomputeDt,\
  "ogmgSaveGrid=s"=>\$ogmgSaveGrid,"ogmgReadGrid=s"=>\$ogmgReadGrid );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
# 
if( $model eq "vp" ){ $model ="visco-plastic model"; }
if( $model eq "b" ){ $model ="Boussinesq model"; }
if( $tm eq "les" ){ $tm ="LargeEddySimulation"; }
if( $ts eq "fe" ){ $ts="forward Euler";  }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       }
if( $ts eq "pc" ){ $ts="adams PC";       }
if( $ts eq "mid"){ $ts="midpoint";       }  
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;}
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $order eq "2" ){ $order = "second order accurate"; }\
elsif( $order eq "4" ){ $order = "fourth order accurate"; }\
elsif( $order eq "6" ){ $order = "sixth order accurate";}\
else { $order = "eighth order accurate";}
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# -- here is the grid we use: 
$grid
#
#
  incompressible Navier Stokes
  # Choose Boussineq: 
  $model
  # Choose turbulence model: 
  $tm 
# 
  $nuVP=.0; $etaVP=$nu; $yieldStressVP=0.; $exponentVP=1.; $kThermal=$nu; $epsVP=1.e-3; 
  define real parameter nuViscoPlastic $nuVP
  define real parameter etaViscoPlastic $etaVP
  define real parameter yieldStressViscoPlastic $yieldStressVP
  define real parameter exponentViscoPlastic $exponentVP 
  define real parameter epsViscoPlastic $epsVP
  define real parameter thermalExpansivity $thermalExpansivity
  # Define LES parameters that are accessed by getLargeEddySimulationViscosity.bf 
  define integer parameter lesOption $lesOption
  define real parameter lesPar1 $lesPar1
  exit
  turn off twilight zone 
# -- order of accuracy: 
##$order 
#
# ** Warning: with multigrid one must take all grids to be implicit
#* implicit
  choose grids for implicit
   all=implicit
  done
#
  final time $tFinal
  times to plot $tPlot 
#
  show file options
    compressed
    OBPSF:maximum number of parallel sub-files 8
    open
      $show
    frequency to flush
      10
    exit
#
  no plotting
  plot and always wait
# ------------
# choose the time stepping:
  $ts
  $newts
  dtMax $dtMax
  compact finite difference
  # -- convergence parameters for the af scheme
  max number of AF corrections $afit
  AF correction relative tol $aftol
  # optionally turn this on to improve stability of the high-order AF scheme by using 2nd-order dissipation at the boundary
  OBPDE:use boundary dissipation in AF scheme 1
  ## apply filter $filter
  ## if( $filter eq 1 ){ $cmds = "filter order $filterOrder\n filter frequency $filterFrequency\n filter iterations 1\n filter coefficient 1. \n  filter stages $filterStages\n explicit filter\n  exit"; }else{ $cmds = "#"; }
  ## $cmds
# --------------------
  $ts
# 
  cfl $cfl
# 
  slow start cfl $slowStartCFL
  slow start steps $slowStartSteps
  slow start recompute dt $slowStartRecomputeDt
  slow start $slowStartTime   # (seconds)
#
  recompute dt every $recomputeDt
#
  choose grids for implicit
    all=implicit
    done
#
    maximum number of iterations for implicit interpolation
      10
#
  pde parameters
   nu $nu 
   kThermal $kThermal
   gravity
     $gravity
   #  turn on 2nd-order AD here:
   OBPDE:second-order artificial diffusion $ad2
   OBPDE:ad21,ad22 $ad21 , $ad22
   OBPDE:fourth-order artificial diffusion $ad4
   OBPDE:ad41,ad42 $ad41 , $ad42
   #  OBPDE:check for inflow at outflow
   # This next option will use Neumann BC's on the velocity at outflow
   if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="OBPDE:expect inflow at outflow\n use extrapolate BC at outflow"; }
   $cmd
  done
# 
  dtMax $dtMax
# 
#************************************
#
  pressure solver options
   # $ogmgAutoChoose=1;
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogmgDebug=$ogesDebug;  $ogmgRtolcg=$rtolp; $ogmgAtolcg=$atolp;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; $ogmgOpav=0; $ogmgRtolcg=1.e-6;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  boundary conditions
#
    bcNumber1=noSlipWall, uniform(T=$surfaceTemp)
    bcNumber2=slipWall
    bcNumber2=outflow,  pressure(1.*p+$apn*p.n=0.)
#    bcNumber3=inflowWithVelocityGiven, uniform(p=1.,u=1.)
#    bcNumber3=inflowWithVelocityGiven, parabolic(d=.15,p=1,u=1.)
    bcNumber3=inflowWithVelocityGiven, parabolic(d=.10,p=1,u=1.,T=1.)
    bcNumber4=outflow,  pressure(1.*p+$apn*p.n=0.)
#
    done
# 
# initial conditions: uniform flow or restart from a solution in a show file 
if( $restart eq "" ){ $cmds = "uniform flow\n u=1., v=0., p=1."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number $restartSolution \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
  if( $project eq "1"  && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
  $project
  debug $debug
  continue
# 
# 
set view:0 -0.501718 -0.0103093 0 3.42353 1 0 0 0 1 0 0 0 1
DISPLAY COLOUR BAR:0 0
  $go
