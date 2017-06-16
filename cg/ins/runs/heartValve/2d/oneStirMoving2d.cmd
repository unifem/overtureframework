# =============================================================================================
# cgins: stirring stick example (rbins example)
# 
# Usage:
#   
#  cgins [-noplot] stirMoving -bcOption=<inflowOutflow|inflowGivenPressure> -go=[run/halt/og]
# 
#  -go : run, halt, og=open graphics
# 
# Examples: 
# 
#  cgins stirMoving -g=stir.hdf -nu=.05 -tf=1. -tp=.025 -go=halt 
#  cgins stirMoving -g=stir.hdf -nu=.05 -tf=1. -tp=.025 -go=halt -ts="adams PC"
#  cgins stirMoving -g=stir.hdf -nu=.05 -tf=1. -tp=.004 -rate=8. -go=halt 
#  cgins stirMoving -g=stir2.hdf -nu=.01 -tf=1. -tp=.002 -rate=8. -go=halt 
#
#
# =============================================================================================
# 
# --- set default values for parameters ---
$tFinal=.5; $tPlot=.025; $show = " "; 
$nu=.05; $cfl=.9;
$ts="implicit";
#
#modified from cylDrop.cmd
$move=1; 
$density=10; 
$inertia="-1"; # set this to over-ride computed inertia, -1=auto-compute 
$mass="-1"; # set this to over-ride computed mass, -1=auto-compute 
$dtMax=.1; $newts=0; 
$d=.2; # parabolic inflow distance 
$debug=0; $go="halt"; $project=0;  $refactorFrequency=100; 
$recomputeDt=2; 
$restart=""; 
$restartframe=-1; 
#
$dropName1="stir1"; $dropName2="stir2"; $channelName="backGround"; 
$gravity = 0; $rampGravity=0; 
#
$numberOfCorrections=1; 
$addedMass=0; $useTP=0;  $useProvidedAcceleration=1; 
$addedDamping=0;  $addedDampingCoeff=1.; $scaleAddedDampingWithDt=0; $addedDampingProjectVelocity=0; 
$bodyForce="";
#
$omega=.5; $rtolc=1.e-4; $atolc=1.e-7; 
# 
$cdv=1.; $ad2=1; $ad21=1; $ad22=1;  $ad4=0; $ad41=2.; $ad42=2.; 
$iluLevels=1; $ogesdebug=0; 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
$bcOption="inflowOutflow"; 
$inflowPressure=.1; # inflow pressure for $bcOption eq "rampedPressure"
$cp0=.1; $cpn=1.; # coefficients in pressure outflow BC
$option=""; 
$flushFrequency=5; 
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
#
$uIn=.0;
$ampSinusoidalPressure=1.; $freqSinusoidalPressure=1.; # for sinusoidal pressure option
$psolver=""; $solver="";
$ycenter="";
$pMax=10; $tMax=0.5;
#for cardiac cycle boundary
$pMin=-20; $tP1=1; $tP2=-0.5;
$theta10=-1; $theta20=0.05; 
$theta11=-0.05; $theta21=1; 
$deltaAngle1=1.e-2; $epsilonAngle1=1.e-3;
$deltaAngle2=1.e-2; $epsilonAngle2=1.e-3;
$damp1=1.0;
$damp2=1.0;
#
# 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"move=i"=>\$move,\
 "nu=f"=>\$nu,"cfl=f"=>\$cfl,"go=s"=>\$go,"show=s"=>\$show,\
 "tp=f"=>\$tPlot,"solver=s"=>\$solver,"psolver=s"=>\$psolver,"noplot=s"=>\$noplot,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"bcOption=s"=>\$bcOption,\
 "dtMax=f"=>\$dtMax,"freqFullUpdate=i"=>\$freqFullUpdate,"density=f"=>\$density,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
 "gravity=f"=>\$gravity,"uIn=f"=>\$uIn,"ycenter=f"=>\$ycenter,\
 "ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,\
 "freqFullUpdate=i"=>\$freqFullUpdate,"dropName1=s"=>\$dropName1,"dropName2=s"=>\$dropName2,"channelName=s"=>\$channelName,\
 "numberOfCorrections=i"=>\$numberOfCorrections,"omega=f"=>\$omega,"addedMass=f"=>\$addedMass,"useTP=i"=>\$useTP,"restartf=i"=>\$restartframe,\
 "bodyForce=s"=>\$bodyForce,"debug=i"=>\$debug,"ogesdebug=i"=>\$ogesdebug,\
 "delta1=f"=>\$deltaAngle1,"epsilon1=f"=>\$epsilonAngle1,\
 "delta2=f"=>\$deltaAngle2,"epsilon2=f"=>\$epsilonAngle2,\
 "damp1=f"=>\$damp1,"damp2=f"=>\$damp2,\
 "theta10=f"=>\$theta10,"theta20=f"=>\$theta20,"theta11=f"=>\$theta11,"theta21=f"=>\$theta21,\
 "rtolc=f"=>\$rtolc,"atolc=f"=>\$atolc,"option=s"=>\$option,"useProvidedAcceleration=i"=>\$useProvidedAcceleration,\
 "inertia=f"=>\$inertia,"mass=f"=>\$mass,"ampSinusoidalPressure=f"=>\$ampSinusoidalPressure,"inflowPressure=f"=>\$inflowPressure,\
 "freqSinusoidalPressure=f"=>\$freqSinusoidalPressure, "flushFrequency=f"=>\$flushFrequency,\
 "addedDamping=f"=>\$addedDamping,"addedDampingCoeff=f"=>\$addedDampingCoeff,"rampGravity=i"=>\$rampGravity,\
 "scaleAddedDampingWithDt=f"=>\$scaleAddedDampingWithDt,"addedDampingProjectVelocity=f"=>\$addedDampingProjectVelocity,\
 "cp0=f"=>\$cp0,"cpn=f"=>\$cpn, "tMax=f"=>\$tMax,"pMax=f"=>\$pMax,\
 "tP1=f"=>\$tP1,"tP2=f"=>\$tP2,"pMin=f"=>\$pMin);
* -------------------------------------------------------------------------------------------------
#
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
#
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "*"; }
if( $project eq "1" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
#
# 
$grid
* 
  incompressible Navier Stokes
  exit
  show file options
    open
     $show
    frequency to flush
      $flushFrequency
    exit
  turn off twilight zone
#************************************
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  dtMax $dtMax
#
  recompute dt every $recomputeDt
#
 plot and always wait
#************************************
  $ts
  $newts
  # -- for the AFS scheme:
  compact finite difference
  # -- convergence parameters for the af scheme
  max number of AF corrections $afit
  AF correction relative tol $aftol
  # optionally turn this on to improve stability of the high-order AF scheme by using 2nd-order dissipation at the boundary
  OBPDE:use boundary dissipation in AF scheme 1
# 
  number of PC corrections $numberOfCorrections
  # -- for added mass algorithm:
  use added mass algorithm $addedMass
  # for now we let the solver know that the added mass algorithm needed predicted values for the pressure:
  predicted pressure needed $addedMass
  # for added damping algorithm: 
  added damping coefficient: $addedDampingCoeff
  use added damping algorithm $addedDamping
  scale added damping with dt $scaleAddedDampingWithDt
  added damping project velocity $addedDampingProjectVelocity
  # For TP
  use moving grid sub-iterations $useTP
#   
# Use a single vector implicit system for the velocity components even if multiple scalar systems may be used: 
  $cmd="#";
  if( $bcOption eq "inflowGivenPressure" || $bcOption eq "inflowPulsePressure" || $bcOption eq "inflowCardiacCycle" ){ $cmd = "use vector implicit system 1"; }
  $cmd
#
  choose grids for implicit
  #need more tests here
     all=implicit
  done
# 
  frequency for full grid gen update $freqFullUpdate
# 
  pde parameters
   nu  $nu
   #   -- specify the fluid density
   fluid density
     1. 
   #  turn on gravity
   $gravityVector="$gravity 0. 0.";
   gravity
     $gravityVector
   #  turn on 2nd-order AD here:
   OBPDE:second-order artificial diffusion $ad2
   OBPDE:ad21,ad22 $ad21, $ad22
   OBPDE:fourth-order artificial diffusion $ad4
   OBPDE:ad41,ad42 $ad41, $ad42
   OBPDE:divergence damping  $cdv
   $outflowOption="neumann";
   if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
   $cmd
#
  done
#*************************************
  if( $move eq 1 ){ $cmd="turn on moving grids"; } else{ $cmd="#"; }
  $cmd
#************************
  specify grids to move
   # Moving grids debug:
   debug: $debug
   # improve quality of interpolation
   print moving body info 1
   # limit forces
   #  $forceLimit $forceLimit
 #
   rigid body
    log file: rigidBody1.log
    debug: $debug
     density
       $density
     if ($ycenter eq ""){$cmd="#";}else{$cmd="initial center of mass\n 0 $ycenter 0";}
     $cmd
     #      
     # mass=-1 : means auto compute inertia
     if( $mass eq "-1" ){ $cmd="#"; }else{ $cmd="mass\n $mass"; }
     $cmd
     #
     # inertia=-1 : means auto compute inertia
     if( $inertia eq "-1" ){ $cmd="#"; }else{ $cmd="moments of inertia\n $inertia"; }
     $cmd
    $theta1=$theta10; $theta2=$theta20;
    $cmd="#";
    if( $bodyForce eq "restrictAngle"){$cmd="restrict angle: $theta1 $deltaAngle1 $epsilonAngle1 $theta2 $deltaAngle2 $epsilonAngle2";} 
    if( $bodyForce eq "restrictAngleDamp")\
    {$cmd="restrict angle and damp: $theta1 $deltaAngle1 $epsilonAngle1 $damp1 $theta2 $deltaAngle2 $epsilonAngle2 $damp2";}
    $cmd
#
#    #old: if( $bodyForce eq "restrictAngle"){$cmd="restrict angle: $theta1 $theta2 $deltaAngle $epsilonAngle";}else{$cmd="#";} 
#    if( $bodyForce eq "restrictAngle"){$cmd="restrict angle: $theta1 $deltaAngle1 $epsilonAngle1 $theta2 $deltaAngle2 $epsilonAngle2";}else{$cmd="#";} 
#     $cmd
# 
    # relaxation is used for light bodies to stabilize the time stepping
    relax correction steps $useTP
    # -- indicate if we are using the direct projection AMP scheme ---
    direct projection added mass $addedMass
    use provided acceleration $useProvidedAcceleration
    # tolerences for TP-SI 
    force relaxation parameter: $omega
    force relative tol: $rtolc
    force absolute tol: $atolc
    $beta=$omega; 
    torque relaxation parameter: $beta
    torque relative tol: $rtolc
    torque absolute tol: $atolc
    done
    $dropName1
   done
#
  done
#
  boundary conditions
    all=noSlipWall
    $cmd="#";
    # inflow and outflow
    # inflow velocity: uIn
    if( $bcOption eq "inflowOutflow" ){ $cmd="bcNumber1=inflowWithVelocityGiven,  parabolic(d=$d,p=1,u=$uIn)\n bcNumber2=outflow, pressure(1.*p+0.*p.n=0.)"; }
    # inflow with Pressure given   
    #    Inflow pressure : $inflowPressure
    #    Outflow pressure = 0
    if( $bcOption eq "inflowGivenPressure" ){ $cmd="bcNumber1=inflowWithPressureAndTangentialVelocityGiven, uniform(v=0.,w=0.,p=$inflowPressure)\n bcNumber2=outflow, pressure(1.*p+0.*p.n=0.)";}
    if( $bcOption eq "inflowPulsePressure" ){ $cmd="bcNumber1=inflowWithPressureAndTangentialVelocityGiven, userDefinedBoundaryData\n pressure pulse \n $pMax $tMax \n done \n bcNumber2=outflow, pressure(1.*p+0.*p.n=0.)";}
    if( $bcOption eq "inflowCardiacCycle" ){ $cmd="bcNumber1=inflowWithPressureAndTangentialVelocityGiven, userDefinedBoundaryData\n cardiac cycle \n $pMax $tP1 $pMin $tP2 \n done \n bcNumber2=outflow, pressure(1.*p+0.*p.n=0.)";}
    $cmd 
    done
#
  maximum number of iterations for implicit interpolation
     10 
#***************************************************
   pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels;  $ogesDtol=1e20; 
   #$ogesDebug=$ogesdebug;
   include $ENV{CG}/ins/cmd/ogesOptions.h
   #debug
   # $ogesdebug
  exit
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; 
   #$ogesDebug=$ogesdebug;
   include $ENV{CG}/ins/cmd/ogesOptions.h
   debug
    $ogesdebug
  exit
#
$project
#
  if( $restart eq "" ){ $cmds = "uniform flow\n" . "p=0., u=0., v=0. \n"; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number $restartframe \n OBIC:assign solution from show file"; }
#restart -1 means it started from the last!
#
  initial conditions
  $cmds
  exit
#
  debug
    $debug
  continue
#
  plot:u
#
  $go
