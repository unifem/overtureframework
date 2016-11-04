#
# A falling body in a channel
#
# Usage:
#    cgins [-noplot] fallingBody -g=<name> -tp=<f> -tp=<f> -density=<f> ...
#         -bcOption=[walls|inflowOutflow|pressure] -rampGravity=[0|1] -implicitFactor=<f>  ...
#        -sep=<f> -vIn=<f> -forceLimit=<f> -bodyForce=[x|y|wz|wzRamp|none] -go[halt|go|og]
#  
#  -sep : separation distance for collisions
#
# -bcOption : walls : noSlipWall's all around
#             inflowOutflow : inflow on bottom, outflow on top
#             pressure : time dependent pressure on bottom
#
#
$model="ins"; $solver = "best"; $show=" "; $ts="pc"; $noplot=""; $frequencyToFlush=5; 
$density=1.25; 
$inertia="-1"; # set this to over-ride computed inertia, -1=auto-compute 
$nu = .1; $dtMax=.05; $newts=0; $movingWall=0; 
# for nu=.005 the terminal velocity of one drop is about .9 -- for low Re the velocity is prop. to Re
$inflowVelocity=.9;
$tFinal=10.; $tPlot=.1; $cfl=.9; $debug=0; $go="halt"; $project=0; $refactorFrequency=100;
$detectCollisions=0; $sep=3.; $forceLimit=30.; $cdv=1.; $flush=5; $ad21=2.; $ad22=2.; 
$restart="";
$improveQuality=0; # 1=improve quality of interpolation
#
$radius=.125; $fallingBody="fallingBody";
$gravity = -1.; # acceleration due to gravity
$rampGravity=1; 
$bodyForce=""; 
$implicitFactor=.5; # for RB corrector. 1=BE, .5=TRap
#
$numberOfCorrections=1; 
$addedMass=0; $useTP=0;  $useProvidedAcceleration=1; 
$addedDamping=0;  $addedDampingCoeff=1.; $scaleAddedDampingWithDt=0; $addedDampingProjectVelocity=0; 
$omega=.5; $rtolc=1.e-4; $atolc=1.e-7; 
# 
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$cdv=1.; $cDt=.25; $ad2=1; $ad21=1; $ad22=1;  $ad4=0; $ad41=2.; $ad42=2.; 
$psolver="best"; $solver="best"; 
$iluLevels=1; $ogesDebug=0; 
$rtolp=1.e-4; $atolp=1.e-5;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
# -- for Kyle's AF scheme:
$afit = 20;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$vIn=.0;
$bcOption="walls"; 
$option=""; 
$freqFullUpdate=10; 
#
$ampSinusoidalPressure=1.; $freqSinusoidalPressure=1.; # for sinusoidal pressure option
#
$amp=1.; $freq=1.;   # for body force
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"model=s"=>\$model,"inflowVelocity=f"=>\$inflowVelocity,\
 "tp=f"=>\$tPlot,"solver=s"=>\$solver,"psolver=s"=>\$psolver,"show=s"=>\$show,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl,"go=s"=>\$go,"numDrops=i"=> \$numDrops,"newts=i"=> \$newts,\
 "noplot=s"=>\$noplot,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"bcOption=s"=>\$bcOption,\
 "dtMax=f"=>\$dtMax,"freqFullUpdate=i"=>\$freqFullUpdate,"density=f"=>\$density,"movingWall=i"=>\$movingWall,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
 "forceLimit=f"=>\$forceLimit,"sep=f"=>\$sep,"flush=i"=>\$flush,"gravity=f"=>\$gravity,"vIn=f"=>\$vIn,\
 "ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,\
 "freqFullUpdate=i"=>\$freqFullUpdate,"radius=f"=>\$radius,"fallingBody=s"=>\$fallingBody,"channelName=s"=>\$channelName,\
 "numberOfCorrections=i"=>\$numberOfCorrections,"omega=f"=>\$omega,"addedMass=f"=>\$addedMass,"useTP=i"=>\$useTP,\
 "rtolc=f"=>\$rtolc,"atolc=f"=>\$atolc,"option=s"=>\$option,"useProvidedAcceleration=i"=>\$useProvidedAcceleration,\
 "inertia=f"=>\$inertia,"amp=f"=>\$amp,"freq=f"=>\$freq,"addedDamping=f"=>\$addedDamping,\
 "rampGravity=i"=>\$rampGravity,"implicitFactor=f"=>\$implicitFactor,"improveQuality=i"=>\$improveQuality,\
 "ampSinusoidalPressure=f"=>\$ampSinusoidalPressure,"freqSinusoidalPressure=f"=>\$freqSinusoidalPressure,\
 "bodyForce=s"=>\$bodyForce,"cdv=f"=>\$cdv,"cDt=f"=>\$cDt,"addedDampingCoeff=f"=>\$addedDampingCoeff,\
 "scaleAddedDampingWithDt=f"=>\$scaleAddedDampingWithDt,"addedDampingProjectVelocity=f"=>\$addedDampingProjectVelocity,\
 "frequencyToFlush=i"=>\$frequencyToFlush,"iluLevels=i"=>\$iluLevels );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
# 
if( $ts eq "fe" ){ $ts="forward Euler";  }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       }
if( $ts eq "pc" ){ $ts="adams PC";       }
if( $ts eq "mid"){ $ts="midpoint";       }  
if( $ts eq "pc4" ){ $ts="adams PC order 4"; $useNewImp=0; } # NOTE: turn off new implicit for fourth order
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;  $useNewImp=0;}
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $project eq "1" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
#
$grid
#
  incompressible Navier Stokes
  exit
#
  show file options
    open
     $show
    frequency to flush
      $frequencyToFlush
  exit  
  turn off twilight zone
#************************************
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  implicit factor $implicitFactor 
  dtMax $dtMax 
#
  recompute dt every 10
#
  plot and always wait
#
#************************************
  $ts
  $newts
  ## first order predictor
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
  # -- CHECK ME: 
  use moving grid sub-iterations $useTP
  # TEMP FIX: 
  # Use a single vector implicit system for the velocity components even if multiple scalar systems may be used: 
  if( $bcOption eq "sinusoidalPressure" ){ $cmd = "use vector implicit system 1"; }else{ $cmd="#"; }
  $cmd
# 
  choose grids for implicit
     all=implicit
     ## *** TURN OFF for TESTING
     ## channel=explicit
    done
# 
  frequency for full grid gen update $freqFullUpdate
# 
  pde parameters
   nu  $nu
   #   -- specify the fluid density
   fluid density
     1.
   # turn on gravity
   $gravityVector="0. $gravity 0.";
   gravity
     $gravityVector
   # optionally ramp gravity 
   $taGravity=0; $tbGravity=1.; 
   if( $rampGravity eq 1 ){ $cmd ="OBPDE:set gravity time dependence\n ramp end values: 0,1 (start,end)\n ramp times: $taGravity,$tbGravity (start,end)\n ramp order: 3\n ramp function\n   exit"; }else{ $cmd="#"; }
   $cmd
   #  turn on 2nd-order AD here:
   OBPDE:second-order artificial diffusion $ad2
   OBPDE:ad21,ad22 $ad21, $ad22
   OBPDE:fourth-order artificial diffusion $ad4
   OBPDE:ad41,ad42 $ad41, $ad42
   OBPDE:divergence damping  $cdv
   OBPDE:cDt div damping $cDt   
   # OBPDE:check for inflow at outflow
   # OBPDE:expect inflow at outflow
  done
#*************************************
  turn on moving grids
#*************************
  detect collisions $detectCollisions
  minimum separation for collisions $sep
#************************
  specify grids to move
   # Moving grids debug:
   debug: $debug
   # try turning this on - July 8, 2016
   if( $improveQuality eq 1 ){ $cmd="improve quality of interpolation"; }else{ $cmd="#"; }
   $cmd
   #$quality=2.;  # default is 2
   #interpolation quality: $quality
   #
   print moving body info 1
   # limit forces
   #  $forceLimit $forceLimit
   #
 #
   rigid body
     log file: rigidBody.log
     #mass
     #  -.001
     density
       $density
     #
      # moment of inertia of a rectangle: (M/12)*( w^2 + d^2) = (rhos*w*d/12)*(  w^2 + d^2 )
      $width=1.; $depth=.5; # do this for now 
      if( $inertia eq "" ){ $inertia = ($density*$width*$depth/12.)*( $width*$width + $depth*$depth); }
      if( $inertia eq "-1" ){ $cmd="#"; }else{ $cmd="moments of inertia\n $inertia"; }
      $cmd
     # implicitFactor: .5=Trapezoidal, 1=BE for the corrector 
     implicitFactor: $implicitFactor
     # 
     # Add body force if any: 
     $a0=0.; $a1=1.; 
     $a0=10.; $a1=0.; 
     $cmd="#"; 
     if( $bodyForce eq "wz" ){ $cmd="time function\n  body torque z time function...\n linear function\n linear parameters: $a0,$a1 (a0,a1)\n  exit"; }
     if( $bodyForce eq "wzs" ){ $cmd="time function\n body torque z time function...\n sinusoidal function\n sinusoid parameters: $amp, $freq,0 (b0,f0,t0)\n  exit"; }
     # ramp: 
     if( $bodyForce eq "wzRamp" ){ $cmd="time function\n body torque z time function...\n ramp function\n ramp end values: 0,$amp (start,end)\n ramp times: 0,1 (start,end)\n  ramp order: 3\n exit"; }
     #
     $cmd
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
      $fallingBody
    done
   #
  done
#
  boundary conditions
    all=noSlipWall
   done
#
  maximum number of iterations for implicit interpolation
     10 
#***************************************************
#
  pressure solver options
   # $ogesDebug=$debug; 
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogesDtol=1e20; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; $ogesIluLevels=1; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
#***************************************************
#
$project
# 
 initial conditions
   uniform flow
    p=0., u=0., v=0.
 exit
#
  debug
    $debug
  continue
#
    plot:p
#
#
  $go



