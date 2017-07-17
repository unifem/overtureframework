#
# Heart Valve simulation in 3D
#
# Usage:
#    cgins [-noplot] heartValve -g=<name> -tp=<f> -tp=<f> ...
#        -go=[halt|go|og] -bcOption=wall|givenPressure|outflow
#
#  bc : all walls except xtop=outflow and xbottom=inflow/inflow+outflow
#  
# Examples: 
#  $cgins heartValveMoving3d -g=heartValveGride1.order2.hdf -tf=.1 -tp=.1 -dtMax=.025 -nu=.1 -mass=1e5 -inertia=.001 -xyinertia=1e5 -move=1 -bcOption=inflowGivenPressure -inflowPressure=10 -numberOfCorrections=2 -addedMass=1 -useProvidedAcceleration=1 -addedDamping=1 -addedDampingCoeff=1.0 -addedDampingProjectVelocity=1 -scaleAddedDampingWithDt=1 -solver=best -psolver=best -rtolp=1.e-5 -atolp=1.e-7 -rtol=1.e-7 -atol=1.e-9 -freqFullUpdate=1 -ycenter=0.15 -bodyForce=restrictAngleDamp -theta10=-1.05 -theta20=0.1 -delta1=0.1 -epsilon1=5.e-2 -delta2=0.1 -epsilon2=5.e-3 -damp1=20 -damp2=20 -go=og
#
$show=" "; $noplot=""; $move=1; 
$ts="implicit";
#
$density=1.; 
$xyinertia=1.e5;
$inertia=1.e-3;
$mass=1.e5;
#
$nu = 0.01; 
$dtMax=.05; 
$newts=0;
$tFinal=2.; $tPlot=.1; $cfl=.9; $debug=0; $go="halt"; $project=0;  
$recomputeDt=2; # 10 
$fullImplicit=0; 
$restart=""; 
$restartframe=-1; 
#
$bcOption="inflowGivenPressure"; 
$inflowPressure=1.;
#
$numberOfCorrections=1; 
$addedMass=0; $useTP=0;  $useProvidedAcceleration=1; 
$addedDamping=0;  $addedDampingCoeff=1.; $scaleAddedDampingWithDt=0; $addedDampingProjectVelocity=0; 
$outflowOption="";
$omega=.5; $rtolc=1.e-4; $atolc=1.e-7; 
# 
$cdv=1.; $ad2=1; $ad21=1; $ad22=1;  $ad4=0; $ad41=2.; $ad42=2.; 
$psolver="best"; $solver="best"; 
$iluLevels=1; $ogesDebug=0; 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
# -- for Kyle's AF scheme:
$afit = 20;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$option=""; 
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$flushFrequency=10; 
#
$cp0=.1; $cpn=1.; # coefficients in pressure outflow BC (not supported)
#
$ycenter=0.25;
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
$d=0.1;
$uIn=1.0;
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"move=i"=>\$move,\
"nu=f"=>\$nu,"cfl=f"=>\$cfl,"go=s"=>\$go,"iluLevels=i"=>\$iluLevels,\
"tp=f"=>\$tPlot,"solver=s"=>\$solver,"psolver=s"=>\$psolver,"show=s"=>\$show,"debug=i"=>\$debug, \
"noplot=s"=>\$noplot,"project=i"=>\$project,"recomputeDt=i"=>\$recomputeDt,\
"dtMax=f"=>\$dtMax,\
"rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
"bcOption=s"=>\$bcOption,"restartf=i"=>\$restartframe,\
"ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,\
"freqFullUpdate=i"=>\$freqFullUpdate,\
"numberOfCorrections=i"=>\$numberOfCorrections,"omega=f"=>\$omega,"addedMass=f"=>\$addedMass,"useTP=i"=>\$useTP,\
"rtolc=f"=>\$rtolc,"atolc=f"=>\$atolc,"option=s"=>\$option,"useProvidedAcceleration=i"=>\$useProvidedAcceleration,\
"flushFrequency=f"=>\$flushFrequency,"fullImplicit=i"=>\$fullImplicit,\
"addedDamping=f"=>\$addedDamping,"addedDampingCoeff=f"=>\$addedDampingCoeff,\
"scaleAddedDampingWithDt=f"=>\$scaleAddedDampingWithDt,"addedDampingProjectVelocity=f"=>\$addedDampingProjectVelocity,\
"ycenter=f"=>\$ycenter,"inflowPressure=f"=>\$inflowPressure,\
"bodyForce=s"=>\$bodyForce,"debug=i"=>\$debug,\
"delta1=f"=>\$deltaAngle1,"epsilon1=f"=>\$epsilonAngle1,\
"delta2=f"=>\$deltaAngle2,"epsilon2=f"=>\$epsilonAngle2,\
"damp1=f"=>\$damp1,"damp2=f"=>\$damp2,\
"d=f"=>\$d,"uIn=f"=>\$uIn,\
"theta10=f"=>\$theta10,"theta20=f"=>\$theta20,"theta11=f"=>\$theta11,"theta21=f"=>\$theta21,\
"inertia=f"=>\$inertia,"xyinertia=f"=>\$xyinertia,"mass=f"=>\$mass,\
"tMax=f"=>\$tMax,"pMax=f"=>\$pMax,"tP1=f"=>\$tP1,"tP2=f"=>\$tP2,"pMin=f"=>\$pMin,\
"cp0=f"=>\$cp0,"cpn=f"=>\$cpn  );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
#
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
if( $project eq "1" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
#
#
$grid
#
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
    #
#************************************
    $ts
    $newts
    #use full implicit system $fullImplicitSystem
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
    # TEMP FIX: this fix is not working now 06/17/2017 in 3D but it works without the fix
    # Use a single vector implicit system for the velocity components even if multiple scalar systems may be used: 
    $cmd="#";
    if( ($bcOption eq "inflowGivenPressure" || $bcOption eq "inflowPulsePressure" || $bcOption eq "inflowCardiacCycle" || $bcOption eq "reverseInflow" || $bcOptioin eq "reversePulse") && ($fullImplicit eq 1))\
    { $cmd = "use vector implicit system 1"; }
    $cmd
#
    choose grids for implicit
     all=implicit
     ## *** TURN OFF for TESTING
     ## channel=explicit
    done
#*************************************************
#
  frequency for full grid gen update $freqFullUpdate
# 
  pde parameters
    nu  $nu
    #   -- specify the fluid density
    fluid density
        1.
    #  turn off gravity
    #gravity
    #    0. 0. 0.
    #  turn on 2nd-order AD here:
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22 $ad21, $ad22
    OBPDE:fourth-order artificial diffusion $ad4
    OBPDE:ad41,ad42 $ad41, $ad42
    OBPDE:divergence damping  $cdv
    # OBPDE:check for inflow at outflow
    # OBPDE:expect inflow at outflow
    $outflowOption="neumann";
    if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
    $cmd
    done
#*************************************
    if( $move eq 1 ){ $cmd="turn on moving grids"; } else{ $cmd="#"; }
    $cmd
#*************************
#    detect collisions 0
#************************
    specify grids to move
    # Moving grids debug:
    #      improve quality of interpolation
    print moving body info 1
    #
#************************
#   upper leaflet
#************************
    rigid body
    log file: leafletUpper.log
    debug: $debug
    density
        $density
    mass
        $mass     
    moments of inertia
        $xyinertia $xyinertia $inertia
    initial center of mass
        0 $ycenter 0
#
#   body force (the setup is symmetric):
    $theta1=$theta10; $theta2=$theta20;
    $cmd="#";
    if( $bodyForce eq "restrictAngle"){$cmd="restrict angle: $theta1 $deltaAngle1 $epsilonAngle1 $theta2 $deltaAngle2 $epsilonAngle2";}
    if( $bodyForce eq "restrictAngleDamp")\
    {$cmd="restrict angle and damp: $theta1 $deltaAngle1 $epsilonAngle1 $damp1 $theta2 $deltaAngle2 $epsilonAngle2 $damp2";}    
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
    heartValveBottomUpper
    heartValveTopUpper
    heartValveEdgeUpper
  done
  # pause
#************************
#   lower leaflet
#************************
    rigid body
    log file: leafletLower.log
    debug: $debug
    density
        $density
    mass
        $mass     
    moments of inertia
        $xyinertia $xyinertia $inertia
     initial center of mass
     $ycenter=-$ycenter;
        0 $ycenter 0
#
#   body force (the setup is symmetric):
    $theta1=-$theta20; $theta2=-$theta10;
    $cmd="#";
    if( $bodyForce eq "restrictAngle"){$cmd="restrict angle: $theta1 $deltaAngle2 $epsilonAngle2 $theta2 $deltaAngle1 $epsilonAngle1";}
    if( $bodyForce eq "restrictAngleDamp")\
{$cmd="restrict angle and damp: $theta1 $deltaAngle2 $epsilonAngle2 $damp2 $theta2 $deltaAngle1 $epsilonAngle1 $damp1";}
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
    heartValveBottomLower
    heartValveTopLower
    heartValveEdgeLower
  done
   # pause
  done
#
  boundary conditions
    all=noSlipWall
    $cmd="#";
    # inflow with Pressure given   
    #    Inflow pressure : $inflowPressure
    #    Outflow pressure = 0
    #if( $bcOption eq "inflowGivenVelocity" ){ $cmd="inflowWithVelocityGiven, uniform(u=0.,v=0.)\n bcNumber5=outflow, pressure(1.*p+0.*p.n=0.)";}
    if( $bcOption eq "reverseInflow" ){ $cmd="bcNumber5=inflowWithPressureAndTangentialVelocityGiven, uniform(v=0.,w=0.,p=$inflowPressure)\n bcNumber4=outflow, pressure(1.*p+0.*p.n=0.)";}
    if( $bcOption eq "reversePulse" ){ $cmd="bcNumber5=inflowWithPressureAndTangentialVelocityGiven, userDefinedBoundaryData\n pressure pulse \n $pMax $tMax \n done \n bcNumber4=outflow, pressure(1.*p+0.*p.n=0.)";}
    if( $bcOption eq "inflowOutflow" ){ $cmd="bcNumber4=inflowWithVelocityGiven,  parabolic(d=$d,p=1,u=$uIn)\n bcNumber5=outflow, pressure(1.*p+0.*p.n=0.)"; }
    if( $bcOption eq "inflowGivenPressure" ){ $cmd="bcNumber4=inflowWithPressureAndTangentialVelocityGiven, uniform(v=0.,w=0.,p=$inflowPressure)\n bcNumber5=outflow, pressure(1.*p+0.*p.n=0.)";}
    #
    #old way (not woring througth share BC for userDefinedBC): 
    #if( $bcOption eq "inflowCardiacCycle" ){ $cmd="bcNumber4=inflowWithPressureAndTangentialVelocityGiven, userDefinedBoundaryData\n cardiac cycle \n $pMax $tP1 $pMin $tP2 \n done \n bcNumber5=outflow, pressure(1.*p+0.*p.n=0.)";}
    #if( $bcOption eq "inflowPulsePressure" ){ $cmd="bcNumber4=inflowWithPressureAndTangentialVelocityGiven, userDefinedBoundaryData\n pressure pulse \n $pMax $tMax \n done \n bcNumber5=outflow, pressure(1.*p+0.*p.n=0.)";}
    #new way
    if( $bcOption eq "inflowPulsePressure" ){ \
        $cmd="core(0,0)=inflowWithPressureAndTangentialVelocityGiven, userDefinedBoundaryData\n pressure pulse \n $pMax $tMax \n done \n". \
             "core(1,0)=outflow, pressure(1.*p+0.*p.n=0.)\n". \
             "pipe(0,1)=inflowWithPressureAndTangentialVelocityGiven, userDefinedBoundaryData\n pressure pulse \n $pMax $tMax \n done \n". \
             "pipe(1,1)=outflow, pressure(1.*p+0.*p.n=0.)";}
    if( $bcOption eq "inflowCardiacCycle" ){ \
        $cmd="core(0,0)=inflowWithPressureAndTangentialVelocityGiven, userDefinedBoundaryData\n cardiac cycle \n $pMax $tP1 $pMin $tP2 \n done \n". \
             "core(1,0)=outflow, pressure(1.*p+0.*p.n=0.)\n". \
             "pipe(0,1)=inflowWithPressureAndTangentialVelocityGiven, userDefinedBoundaryData\n cardiac cycle \n $pMax $tP1 $pMin $tP2 \n done \n". \
             "pipe(1,1)=outflow, pressure(1.*p+0.*p.n=0.)";}
    $cmd 
    done
#
  maximum number of iterations for implicit interpolation
     10
#***************************************************
#
  pressure solver options
#   $ogesDebug=$debug; $ogmgDebugcg=$debug; $ogmgDebug=$debug;
   $imDebug=0;
   $ogesDebug=$imDebug; $ogmgDebugcg=$imDebug; $ogmgDebug=$imDebug;
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels;  $ogesDtol=1e20; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   $imDebug=0;
   $ogesDebug=$imDebug; $ogmgDebugcg=$imDebug; $ogmgDebug=$imDebug;
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
#***************************************************
#
$project
#
if( $restart eq "" ){ $cmds = "uniform flow\n p=0, u=0, v=0, w=0"; }\
else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number $restartframe \n OBIC:assign solution from show file"; }
 initial conditions
  $cmds
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

