#
# A dropping sphere -- compare to the results from Yang and Stern 
# # Usage:
#    cgins [-noplot] sphereDrop -g=<name> -tp=<f> -tp=<f> -density=<f> ...
#        -sep=<f> -forceLimit=<f> -rampGravity=[0|1] -go[halt|go|og]
#		 -bcOption=wall|givenPressure|outflow
#  
#  -sep : separation distance for collisions
#
#  bc : all walls
#  
# Example: 
#  cgins sphereDrop -g= -tf=1. -tp=.05 -nu=.1 -density=1.25 -dtMax=.75e-3 -go=halt 
#
#AMP 
# cgins -noplot sphereDrop -g=*.hdf -tf=2. -tp=.1 -dtMax=0.01 -nu=.1 -ad2=0 -ts=im -density=1.15 -radius=.5 -move=1 -channelName=channel -gravity=-4 -cp0=1. -cpn=0. -project=0 -numberOfCorrections=2 -omega=.1 -addedMass=1 -useProvidedAcceleration=1 -addedDamping=1 -addedDampingCoeff=1. -addedDampingProjectVelocity=1 -scaleAddedDampingWithDt=1 -useTP=0 -debug=3 -solver=best -psolver=best -rtolp=2.5e-09 -atolp=1e-14 -rtol=2.5e-07 -atol=1e-14 -freqFullUpdate=1 -show=*.show -go=go > ! *.out 
#
#TP (no iteration)
#cgins -noplot sphereDrop -g=*.hdf -tf=2. -tp=.1 -dtMax=.01 -nu=.1 -ts=im -density=2. -inertia=1. -radius=.5  -channelName=channel -gravity=-4. -project=0 -numberOfCorrections=1 -omega=1. -addedMass=0  -useProvidedAcceleration=0 -useTP=0 -debug=3 -solver=mg -psolver=mg -show=*.show -go=go >! *.out 
#
#nohup cgins -noplot sphereDrop -g=*.hdf -tf=2. -tp=.1 -dtMax=.01 -recomputeDt=10 -ts=im -density=2. -radius=.5  -channelName=channel -project=0 -numberOfCorrections=1 -omega=1. -addedMass=0  -useProvidedAcceleration=0 -useTP=0 -debug=3 -psolver=mg -show=*.show -go=go >! *.out 
#
#TP (with iteration)
#nohup $cgins -noplot sphereDrop -g=*.hdf -tf=2. -tp=.1 -nu=.1 -ad2=0 -ts=im -density=2. -radius=.5  -channelName=channel -gravity=-4. -dtMax=.01 -project=0 -numberOfCorrections=100 -omega=.5 -rtolc=1.e-9 -atolc=1.e-12 -addedMass=0 -useProvidedAcceleration=0 -addedDamping=0 -addedDampingCoeff=1. -addedDampingProjectVelocity=0 -scaleAddedDampingWithDt=1 -useTP=1 -debug=3 -solver=mg -psolver=mg -rtolp=1.e-7 -atolp=1.e-12 -rtol=1.e-7 -atol=1.e-10 -freqFullUpdate=1 -flushFrequency=2 -show=*.show -go=go > ! *.out &
#
$model="ins"; $show=" "; $ts="pc"; $noplot=""; $move=1; 
$density=1120./970.; 
$inertia="-1"; # set this to over-ride computed inertia, -1=auto-compute 
$nu = 1/1.5; $dtMax=.05; 
$newts=0;
$tFinal=2.; $tPlot=.1; $cfl=.9; $debug=0; $go="halt"; $project=0;  
$recomputeDt=2; # 10 
$sep=3.;  
$fullImplicitSystem=0; # 1=do not use multiple scalar systems for implicit solves, even if possible
$restart=""; 
$slowstartFactor=0;
$slowstartTime=0;
#
$radius=.5; $dropName="sphere1-north-pole\n sphere1-south-pole"; $channelName="channel"; 
$gravity = "-101.9044321";   # cm/s^2   -9.81 acceleration due to gravity standard value: 9.80665 m/s^2.
$rampGravity=0; 
$bcOption="walls"; 
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
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"model=s"=>\$model,"move=i"=>\$move,\
"tp=f"=>\$tPlot,"solver=s"=>\$solver,"psolver=s"=>\$psolver,"show=s"=>\$show,"debug=i"=>\$debug, \
"ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl,"go=s"=>\$go,"newts=i"=> \$newts,\
"iluLevels=i"=>\$iluLevels,"ogesDebug=i"=>\$ogesDebug,\
"ssFactor=f"=>\$slowstartFactor,"ssTime=f"=>\$slowstartTime,\
"noplot=s"=>\$noplot,"project=i"=>\$project,"recomputeDt=i"=>\$recomputeDt,\
"dtMax=f"=>\$dtMax,"freqFullUpdate=i"=>\$freqFullUpdate,"density=f"=>\$density,\
"rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
"forceLimit=f"=>\$forceLimit,"sep=f"=>\$sep,"gravity=f"=>\$gravity,"bcOption=s"=>\$bcOption,\
"ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,\
"freqFullUpdate=i"=>\$freqFullUpdate,"radius=f"=>\$radius,"dropName=s"=>\$dropName,"channelName=s"=>\$channelName,\
"numberOfCorrections=i"=>\$numberOfCorrections,"omega=f"=>\$omega,"addedMass=f"=>\$addedMass,"useTP=i"=>\$useTP,\
"rtolc=f"=>\$rtolc,"atolc=f"=>\$atolc,"option=s"=>\$option,"useProvidedAcceleration=i"=>\$useProvidedAcceleration,\
"inertia=f"=>\$inertia,"flushFrequency=f"=>\$flushFrequency,"fullImplicitSystem=i"=>\$fullImplicitSystem,\
"addedDamping=f"=>\$addedDamping,"addedDampingCoeff=f"=>\$addedDampingCoeff,"rampGravity=i"=>\$rampGravity,\
"scaleAddedDampingWithDt=f"=>\$scaleAddedDampingWithDt,"addedDampingProjectVelocity=f"=>\$addedDampingProjectVelocity,\
"cp0=f"=>\$cp0,"cpn=f"=>\$cpn  );
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
	incompressible Navier Stokes
	exit
#
	show file options
	open
	 $show
	frequency to flush
	  $flushFrequency
	exit  
	turn off twilight zone
#************************************
	final time $tFinal
        if ($slowstartTime eq 0){$cmds="times to plot $tPlot";}else{$cmds="times to plot $slowstartTime";}
        $cmds
	cfl $cfl
        #my slow start -QT
        if( $slowstartFactor ne 0){$dtMaxs=$dtMax/$slowstartFactor; $cmds="dtMax $dtMaxs";}else{$cmds="dtMax $dtMax";}
        $cmds
	#
	recompute dt every $recomputeDt
	#
	plot and always wait
	#
#************************************
	$ts
	$newts
	use full implicit system $fullImplicitSystem
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
	# useNewImplicitMethod 
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
    #  turn on gravity
    $gravityVector="0. $gravity 0.";
    gravity
     $gravityVector
    $taGravity=0; $tbGravity=1.; 
    $rampOrder=3; 
    if ($rampGravity eq 1 ){ $cmd ="OBPDE:set gravity time dependence\n ramp end values: 0,1 (start,end)\n ramp times: $taGravity,$tbGravity (start,end)\n ramp order: $rampOrder\n ramp function\n   exit"; }else{ $cmd="#"; }
    $cmd
    #  turn on 2nd-order AD here:
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22 $ad21, $ad22
    OBPDE:fourth-order artificial diffusion $ad4
    OBPDE:ad41,ad42 $ad41, $ad42
    OBPDE:divergence damping  $cdv
    # OBPDE:check for inflow at outflow
    # OBPDE:expect inflow at outflow
    if( $bcOption eq "outflow" ||  $bcOption eq "outflowWall" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="#"; }
    $cmd
    done
# Gravity is done by adding a const forcing to the "v" equation 
# This is wrong in this case. should not be applied!
#	$pGrad=-$gravity;
#	user defined forcing
#		constant forcing
#		2	$pGrad
#		done
#	exit
#*************************************
	if( $move eq 1 ){ $cmd="turn on moving grids"; } else{ $cmd="#"; }
	$cmd
#*************************
	detect collisions 0
	minimum separation for collisions $sep
#************************
    specify grids to move
    # Moving grids debug:
    debug: $debug
    #
    #      improve quality of interpolation
    print moving body info 1
    # limit forces
    #  $forceLimit $forceLimit
    #
    rigid body
    $logFile="rigidbody.dt$dtMax.log";
     log file:$logFile 
     density
       $density
     $pi=4.*atan2(1.,1.);
     $volume=4./3.*$pi*$radius**3; $mass=$density*$volume;
     mass
	$mass
     $momentOfInertia=.4*$mass*$radius**2;
      moments of inertia
	    $momentOfInertia $momentOfInertia $momentOfInertia
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
    sphere1-north-pole
    sphere1-south-pole
  done
   # pause
  done
#
  boundary conditions
    all=noSlipWall
    # channel=slipWall
    $cmd="#";
    if( $bcOption eq "outflow"){$cmd="$channelName(1,1)=outflow, pressure(1.*p+0.*p.n=0.)\n";} 
    #outflowWall: bottom neumann outflow and top noslipwall
    if( $bcOption eq "outflowWall"){$cmd="$channelName(0,1)=outflow, pressure(1.*p+0.*p.n=0.)\n";} 
    #inflowOutflow: bottom inflowWithPressureAndTangentialVelocityGiven (note p=0) and top outflow (extrapolation?)
    if( $bcOption eq "inflowOutflow"){$cmd="$channelName(0,1)=inflowWithPressureAndTangentialVelocityGiven, uniform(u=0., w=0., p=0.)\n $channelName(1,1)=outflow\n";} 
    if( $bcOption eq "givenPressure" ){$cmd="$channelName(1,1)=inflowWithPressureAndTangentialVelocityGiven, uniform(u=0., w=0., p=1.)\n";}
    #"$channelName(1,1)=inflowWithPressureAndTangentialVelocityGiven, userDefinedBoundaryData\n";}
    $cmd
  done
#
  maximum number of iterations for implicit interpolation
     10
#***************************************************
#
echo to terminal 0
  pressure solver options
  #$ogesDebug=$debug; 
   $ogmgDebugcg=$ogesDebug; $ogmgDebug=$ogesDebug;
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels;  $ogesDtol=1e20; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
#   $ogesDebug=0; 
#   $ogmgDebugcg=0;
#   $ogmgDebug=0;
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
echo to terminal 1
#
#***************************************************
#
$project
# 
#
#this uniform pressure can be dangerous. It is a very wild initial guess, only works for some cases
if( $restart eq "" ){ $cmds = "uniform flow\n p=0, u=0, v=0, w=0"; }\
else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
 initial conditions
  $cmds
  exit
  debug
    $debug
  continue
#
  plot:v
#
  # my slow start (this happens between t=t0 to t=t0+tp) -QT
  if ($slowstartFactor ne 0){$cmds="continue\n plot:v\n dtMax $dtMax";}else{$cmds="#";}
  $cmds
  #if slowstartTime is defined, it needs get back to regular tPlot after slow start is finished
  if ($slowstartTime ne 0){$cmds="times to plot $tPlot";}else{$cmds="#";}
  $cmds
#
  $go
#
#



