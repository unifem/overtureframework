# *******************************************************
# cgins command file: 
#    Test various exact solutions: 
#       - Hagen-Poiseuille pipe flow, 
#       - Couette flow (flow between rotating cylinders), 
#       - Taylor-Green vortex
#
# Usage:
#    cgins [-noplot] exact -g=<gridName> -option=[pipe|rotating|tg] -ts=[pc|im] -tf=<> -tp=<> -nu=<> ...
#            -solver=[best|yale|mg] -psolver=[best|yale|mg] -model=[ins|boussinesq] -tm=[les] ...
#            -move=[0|rotate|shift] -pipeAxis=[0|1|2] -axialAxis=[0|1|2]
# 
# Parameters:  
#  -option : "pipe" = Poiseuille flow (2d) or Hagen-Poiseuille flow (3d)
#         : "rotating" = Rotating Couette flow (flow between rotating cylinders, Taylor-Couette)
#         : "tg" = Taylor-Green vortex
#
#  -nu : viscosity
#  -tf : final time
#  -tp : time to plot
#  -ts : time-stepping method, pc=explicit predictor-corrector, im=implicit predictor-corrector
#  -pipeAxis  : axis for pipe flow
#  -axialAxis : axis for Couette flow in a 3d cylinder.
#
# Examples:
#  -- 2D channel: 
#   cgins exact -g=channel1.order2.ml2 -ts=pc -nu=.1 -ad2=0 -tf=20. -tp=.1  -go=halt  [exact 
#   cgins exact -g=channel1.order4.ml2 -ts=pc -nu=.1 -ad2=0 -tf=20. -tp=.1  -go=halt  [small errors 1e-6
#
#  -- 3D pipe:
#   cgins exact -g=pipee2.order2.hdf -ts=pc -nu=.1 -ad2=0 -tf=20. -tp=.1  -go=halt 
#
#  --- Taylor-Green vortex
#   cgins exact -g=square32p.order2 -option=tg -ts=pc -nu=.01 -tf=1.5 -tp=.1 -cfl=1. -psolver=yale -go=halt
#   cgins exact -g=square32p.order4 -option=tg -ts=pc -nu=.01 -tf=1.5 -tp=.1 -cfl=1. -psolver=yale -go=halt
#
# -- assign default values for all parameters: 
$option="pipe"; $nd=2; # set nd=3 for 3D
$tFinal=30.; $tPlot=.1; $nu=.1; $show=" "; $debug=0; $ogesDebug=0; $debugmg=0; $dtMax=.02; $order=2; $newts=0;
$restart=""; $restartSolution=-1; $outflowOption="neumann";
# 
$slowStartSteps=-1; $slowStartCFL=.5; $slowStartRecomputeDt=100; $slowStartTime=-1.; $recomputeDt=10000;
#
$apn=0.; # coeff in outflow BC : p + apn*p.n = 0 
$rtol=1.e-4; $atol=1.e-4; $solver="best"; 
$rtolp=1.e-5; $atolp=1.e-6; $psolver="best"; $iluLevels=3; 
$project=0; $ts="pc"; $go="halt";
$ad2=0; $ad21=1.; $ad22=1.;   # for 2nd-order artificial dissipation, ad2=0 means no dissipation
$ad4=0; $ad41=1.; $ad42=1.;   # for 4th-order artificial dissipation, ad4=0 means no dissipation
$model="#";  $gravity = "0. 0. 0."; $thermalExpansivity=.1;
$tm = "#"; 
$lesOption=0; $lesPar1=.01;
$move=0; 
$pipeAxis=0; # for pipe flow 
# -- rotating Couette: 
$rInner=.5; $rOuter=1.; $omegaInner=0.; $omegaOuter=1.; $axialAxis=2; 
# 
$kp=1.; # Taylor Green wave number 
# 
# -- for Kyle's AF scheme:
$afit = 20;
$aftol=1e-5;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2;
$cdv=1;  $cDt=.25;
$ogmgAutoChoose=2; $ogmgCoarseGridSolver="best";
$ogmgCoarseGridMaxIterations=1000; 
$ogmgSaveGrid=""; $ogmgReadGrid=""; # save or read a MG grid with levels built
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"tp=f"=>\$tPlot,"u0=f"=>\$u0,"u1=f"=>\$u1,"u2=f"=>\$u2,"ax=f"=>\$ax, "ay=f"=>\$ay,\
 "axp=f"=>\$axp,"pgf=f"=>\$pgf, "model=s"=>\$model,"tm=s"=>\$tm,"apn=f"=>\$apn,"option=s"=>\$option,\
 "solver=s"=>\$solver, "psolver=s"=>\$psolver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"gravity=s"=>\$gravity,\
  "ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,"restartSolution=i"=>\$restartSolution,\
 "imp=f"=>\$implicitFactor,"rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,\
  "restart=s"=>\$restart,"move=s"=>\$move,"debugmg=i"=>\$debugmg,"nullVector=s"=>\$nullVector,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"outflowOption=s"=>\$outflowOption,\
  "bg=s"=>\$bg,"gridToMove=s"=>\$gridToMove,"bcTop=s"=>\$bcTop,"ogesDebug=i"=>\$ogesDebug,"rate=f"=>\$rate,\
  "slowStartCFL=f"=>\$slowStartCFL,"axialAxis=i"=>\$axialAxis,"pipeAxis=i"=>\$pipeAxis,\
  "slowStartTime=f"=>\$slowStartTime,"ogmgCoarseGridSolver=s"=>\$ogmgCoarseGridSolver,"aftol=f"=>\$aftol,\
  "recomputeDt=i"=>\$recomputeDt,"slowStartSteps=i"=>\$slowStartSteps,"slowStartRecomputeDt=i"=>\$slowStartRecomputeDt,\
  "ogmgSaveGrid=s"=>\$ogmgSaveGrid,"ogmgReadGrid=s"=>\$ogmgReadGrid,"nd=i"=>\$nd,"cdv=f"=>\$cdv,"kp=f"=>\$kp );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
# 
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }\
elsif( $model eq "boussinesq" ){ $model = "incompressible Navier Stokes\n Boussinesq model"; }\
elsif( $model eq "tp" ){ $model = "incompressible Navier Stokes\n two-phase flow model"; $twoPhaseFlow=1; }\
elsif( $model eq "bp" ){ $model = "incompressible Navier Stokes\n Boussinesq model\n passive scalar advection"; }
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
  OBPDE:use boundary dissipation in AF scheme 0
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
   OBPDE:divergence damping  $cdv
   #  OBPDE:check for inflow at outflow
   # This next option will use Neumann BC's on the velocity at outflow
   if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="OBPDE:expect inflow at outflow\n use extrapolate BC at outflow"; }
   $cmd
  done
# 
# 
  dtMax $dtMax
#
#  ------------ MOVING GRIDS --------
   $pi=4.*atan2(1.,1.);
   $rate=$omegaOuter/(2.*$pi); 
   $cmd="#";
  if( $move eq "rotate" && $nd eq 2 ){ $cmd="turn on moving grids\n" . \
   "specify grids to move\n" . \
   " rotate\n" . \
   "   0. 0. 0 \n" . \
   " $rate 0.\n" . \
   " outerAnnulus\n" . \
   " done\n" . \
   "done"; }
# -- define the vector in the axial direction: 
  $d0=0.; $d1=0.; $d2=1.; 
  if( $axialAxis eq "0" ){ $d0=1.; $d1=0.; $d2=0;} 
  if( $axialAxis eq "1" ){ $d0=0.; $d1=1.; $d2=0;} 
# 
  if( $move eq "rotate" && $nd eq 3 ){ $cmd="turn on moving grids\n specify grids to move\n rotate\n 0. 0. 0.\n  $d0 $d1 $d2 \n   $rate -1.\n outerCylinder\n done\n done"; }
# 
$shiftVelocity=1.; 
if( $move eq "shift" ){ $cmd="turn on moving grids\n" . \
  "specify grids to move\n" . \
  " translate\n" . \
  "   1. 0. 0 \n" . \
  "   $shiftVelocity\n" . \
  " upperChannel\n" . \
  " done\n" . \
  "done"; }
$cmd
#  -----------------------------------
# 
#  -- DEFINE EXACT SOLUTION --
# 
  OBTZ:user defined known solution
    # -- pipe: 
    # Enter radius,pInflow,pOutflow,x0,length for
    $radius=1.;  $pInflow=1.; $pOutflow=0.; $x0=0.; $length=2.; $ua=0.; $ub=1.; 
    if( $option eq "pipe" ){ $cmd = "pipe flow\n $radius $pInflow $pOutflow $x0 $length $ua $ub $pipeAxis"; }
    # 
    # -- rotating Couette: 
    # Enter rInner, rOuter, omegaInner, omegaOuter, axialAxis
    if( $option eq "rotating" ){ $cmd = "rotating Couette flow\n $rInner $rOuter $omegaInner $omegaOuter $axialAxis"; }
    # Taylor Green vortex
    if( $option eq "tg" ){ $cmd = "Taylor Green vortex\n $kp $axialAxis"; }
    $cmd    
    done
  done
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
  all=dirichletBoundaryCondition
  all=noSlipWall
#  $cmd="#"; 
#  if( $option eq "rotating" ){ $cmd="all=dirichletBoundaryCondition\n bcNumber1=dirichletBoundaryCondition\n bcNumber2=dirichletBoundaryCondition"; }
#
#  $cmd
  #
  # center of the cylinder is (x0,x1,x2)+ s*(d0,d1,d2)
  $vr=0.; $vTheta=0.; $vPhi=0.; $tb=0.; $x0=0.; $x1=0.; $x2=0.; 
  $bcInner=1; $bcOuter=2; 
  if( $nd eq "3" ){ $bcInner=3; $bcOuter=4; }
  $bcCmd1 = "bcNumber$bcInner=noSlipWall, userDefinedBoundaryData\n" .\
     "cylindrical velocity\n" .\
     " $vr $vTheta $vPhi $tb\n" .\
     " $x0 $x1 $x2 $d0 $d1 $d2\n" .\
     "done\n";
  $vTheta=1.; 
  $bcCmd2 = "bcNumber$bcOuter=noSlipWall, userDefinedBoundaryData\n" .\
    "cylindrical velocity\n" .\
    " $vr $vTheta $vPhi $tb\n" .\
    " $x0 $x1 $x2 $d0 $d1 $d2\n" .\
    "done";
  if( $move ne "0" ){ $bcCmd2 = "bcNumber$bcOuter=noSlipWall"; } # for moving annulus use normal no-slip wall
  if( $option ne "tg" ){ $cmd=$bcCmd1 . $bcCmd2; }else{ $cmd="#"; }
  # Flow in a pipe BC: 
  if( $option eq "pipe" ){ $cmd="all=noSlipWall\n bcNumber1=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)\n bcNumber2=outflow,  pressure(1.*p+$apn*p.n=0.)"; }
  $cmd
  # 2D pipe in a channel: add shear 
  if( $option eq "pipe" && $nd eq "2" ){ $cmd="bcNumber4=noSlipWall, uniform(u=$ub)"; }else{ $cmd="#"; }
  $cmd
#
    done
# 
# initial conditions: uniform flow or restart from a solution in a show file 
if( $restart eq "" ){ $cmds = "uniform flow\n u=1., v=0., p=1."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number $restartSolution \n OBIC:assign solution from show file"; }
# 
  initial conditions
#    $cmds
  exit
  if( $project eq "1"  && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
  $project
  debug $debug
  continue
# 
# 
  $go

