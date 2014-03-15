#===============================================================
# Cgins: flow over the site 300 terrain
#
# Grid script for ogen: site300Grid.cmd
#   
# Usage:
#     cgins site300 -g=<grid> -nu=<> -ts=[im|pc|afs] -project=[0|1] -flushFrequency=<> -aftol=<> -afit=<> ...
#          -solver=[best|mg] -psolver=[best|mg] -go=[go|halt]
#
# -afit : maximum number of AF iterations
# -aftol : convergence tol for AF iterations.
# 
# Examples: 
#
# cgins site300 -g=site300Gride1.order2 -nu=25. -ad22=1. -tp=.1 -debug=3 [OK
# cgins site300 -g=site300Gride2.order2 -nu=25. -ad22=1. -tp=.5 -debug=3 [
# cgins site300 -g=site300Gride2.order2.ml1 -nu=10. -ad22=1. -tp=1. -psolver=mg -solver=mg -debug=3 [OK 
# 
# --- Kyle's AFS scheme:
#   cgins site300 -g=site300Gride1.order2 -nu=25. -ad22=1. -ts=afs -cfl=4. -tp=5. -debug=2 [OK
# 
# -- parallel
# 
#===============================================================
#
# -- conversions:
#  1 mile/hour = .44704 m/s    (1 m/s = 2.2369 miles/hour)
$milesPerHourToMetersPerSecond=.44704; 
#
$grid="site300Gride1.order2"; $show = " "; $tFinal=500.; $tPlot=.1; $nu=.1; $cfl=.9;
$uInflow=10.;  # wind speed (m/s)
# 
$slowStartSteps=-1; $slowStartCFL=.5; $slowStartRecomputeDt=100; $slowStartTime=-1.; $recomputeDt=10000;
#
$implicitVariation="viscous"; $refactorFrequency=100; $impGrids="all=explicit"; $impFactor=.5; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; $newts=0; $recomputeDt=100;
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=5.; 
$order = 2; $fullSystem=0; $go="halt"; $move=1;  $moveOnly=0; $amp=.5; $freq=1.; 
$show=" "; $restart=""; $flushFrequency=2; 
$ts="im"; $outflowOption="neumann"; 
$cdv=1.; $ad2=1; $ad21=.5; $ad22=.5; $ad4=0; $ad41=.5; $ad42=.5;
$psolver="choose best iterative solver"; $solver="choose best iterative solver"; 
$ogmgAutoChoose=1; 
$iluLevels=1; $ogesDebug=0; $project=1; 
$rtolp=1.e-4; $atolp=1.e-5;  # tolerances for the pressure solve
$rtol=1.e-5; $atol=1.e-6;    # tolerances for the implicit solver
# -- for Kyle's AF scheme:
$afit = 20;
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2;
$cdv=1;  $cDt=.25;
# 
$pi = 4.*atan2(1.,1.);
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"imp=f"=>\$implicitFactor,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"move=i"=>\$move,"moveOnly=i"=>\$moveOnly,\
  "amp=f"=>\$amp,"freq=f"=>\$freq, "solver=s"=>\$solver, "psolver=s"=>\$psolver,"uInflow=f"=>\$uInflow,\
  "outflowOption=s"=>\$outflowOption,"newts=i"=>\$newts,"recomputeDt=i"=>\$recomputeDt,\
  "ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,\
  "flushFrequency=i"=>\$flushFrequency,"slowStartCFL=f"=>\$slowStartCFL, "slowStartTime=f"=>\$slowStartTime,\
  "recomputeDt=i"=>\$recomputeDt,"slowStartSteps=i"=>\$slowStartSteps,"slowStartRecomputeDt=i"=>\$slowStartRecomputeDt,\
  "ogmgAutoChooset=i"=>\$ogmgAutoChoose );
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
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
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
#
$grid
# 
  incompressible Navier Stokes
  $cmd="#";
  if( $moveOnly eq 1 ){ $cmd ="move and regenerate grids only"; }elsif( $moveOnly eq 2 ){ $cmd = "move grids only"; }
  $cmd
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
  #  OBPDE:use boundary dissipation in AF scheme 1
  ## apply filter $filter
  ## if( $filter eq 1 ){ $cmds = "filter order $filterOrder\n filter frequency $filterFrequency\n filter iterations 1\n filter coefficient 1. \n  filter stages $filterStages\n explicit filter\n  exit"; }else{ $cmds = "#"; }
  ## $cmds
# --------------------
#
  recompute dt every $recomputeDt
#
  choose grids for implicit
   all=implicit
   # slipWall and Neuamnn outflow BC's are broken for this with: 
   # backGround=explicit
  done
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
  pde parameters
    nu $nu
    #  OBPDE:check for inflow at outflow
    #  OBPDE:expect inflow at outflow
    #  turn on 2nd-order AD here:
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22 $ad21, $ad22
    if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
    $cmd
    # For AFS scheme:
    OBPDE:fourth-order artificial diffusion $ad4
    OBPDE:ad41,ad42 $ad41,$ad42   
    ## OBPDE:divergence damping  $cdv
    ## OBPDE:cDt div damping $cDt
    ## OBPDE:use boundary dissipation in AF scheme 1
    ## OBPDE:use new fourth order boundary conditions 1
    ## OBPDE:expect inflow at outflow
  done
#*
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogmgDebug=$ogesDebug; $ogmgCoarseGridSolver="best"; $ogmgRtolcg=$rtolp; $ogmgAtolcg=$atolp; $ogmgRtolcg=$rtolp; $ogmgAtolcg=$atolp;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   # $ogmgSsr=1;  $ogmgAutoChoose=1; 
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; $ogmgOpav=0; $ogmgRtolcg=1.e-6; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  boundary conditions
    # For implicit scheme use inflow, no-slip or outflow on side boundaries 
    #   (slipWall is currenltly bad to use -> full implicit  *fix me*)
    all=noSlipWall
    # Top z boundary is a slip wall
    bcNumber6=slipWall
    # bcNumber1=inflowWithVelocityGiven, uniform(u=$uInflow)
    $parabolicWidth=50.;   # =100.;
    bcNumber1=inflowWithVelocityGiven, parabolic(d=$parabolicWidth,p=1,u=$uInflow,v=0.,w=0.)
    # Make side walls "inflow" boundaries (with no incomping velocity):    
    bcNumber3=inflowWithVelocityGiven, parabolic(d=$parabolicWidth,p=1,u=$uInflow,v=0.,w=0.)
    bcNumber4=inflowWithVelocityGiven, parabolic(d=$parabolicWidth,p=1,u=$uInflow,v=0.,w=0.)
#
    # The outflow pressure BC scales with the size of the domain:  p.n ~ p.x ->  p.r/L   x=r*L 
    $domainLength=1400.;   #NOTE: change me if domain size changes
    $apn=.01*$domainLength; 
    bcNumber2=outflow , pressure(1.*p+$apn*p.n=0.)
#    $d=.5; 
#    backGround(0,0)=inflowWithVelocityGiven, parabolic(d=$d,p=1.,u=1.)
#    backGround(0,0)=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)
   done
#
  debug $debug
#
if( $restart eq "" ){ $cmds = "uniform flow\n u=$uInflow, v=0., p=1."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
#
  continue
#
 x-r 90
 set home
  grid
    toggle grid 0 0
    toggle grid 1 0
   #                s a g 
    toggle boundary 0 0 2 0
    toggle boundary 0 1 2 0
    toggle grid lines on boundary 0 0 2 0
    toggle grid lines on boundary 0 1 2 0
  exit this menu
# 
  contour
    delete contour plane 2
  exit
#
$go 




