#
# cgins: Purdue's Living Lab VAV room ** 2D cross section **
#       
#  cgins [-noplot] vavWithClouds2d -g=<name> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -ts=[im|pc|afs] ...
#         -debug=<num> -probeFile=<name> ...
#         -nu=<num> -Prandtl=<num> -bg=<backGround> -show=<name> -implicitFactor=<num> ...,
#         -solver=[best|yale|mg] -psolver=[best|yale|mg] -pc=[ilu|lu] -adcBoussinesq=<>
# Options:
#  -adcBoussinesq : artificial dissipation for the T equations.
#  -project : 1=project initial conditions
#  -solver : implicit solver
#  -psolver : pressure solver
# 
# NOTES:
#     - Variables are scaled by U=1f/s, L=1f, T=1K
#     - Coefficient of thermal expansion is 1/T (1/K) = 3.4e-3 (1/K) at 70F, 21C 
#     - nu = 1.5e-5 m^2/s = ?? f^2/s 
#     - Pr =.713
#     
# Examples:
#
# - Serial
#
# --- Factor 2
#
#	cgins vavWithClouds2d -g=vavWithCloudsGrid2de2.order2.ml1 -nu=.05 -tf=25. -tp=.1 -ts=im -debug=1 -project=0 -go=halt -show=Cloud_factor2_im.show
#
# --- Factor 4
#
#	cgins vavWithClouds2d -g=vavWithCloudsGrid2de4.order2.ml2 -nu=.00001 -tf=100. -tp=.1 -ts=im -project=0 -go=halt
#
# --- Factor 8
#
#	cgins vavWithClouds2d -g=vavWithCloudsGrid2de8.order2.ml2 -nu=.05 -tf=10. -tp=1. -ts=im -project=0 -go=halt
#
#	cgins vavWithClouds2d -g=vavWithCloudsGrid2de8.order2.ml2 -nu=.001 -tf=15. -tp=.5 -cfl=3. -ad21=1. -ad2=1. -ts=afs -project=1 -go=og 
#
# --- Factor 16
#
#   	cgins vavWithClouds2d -g=vavWithCloudsGrid2de16.order2.ml3 -nu=.000001 -tf=400. -tp=.25 -ts=im -project=0 -go=halt
#
#   	cgins vavWithClouds2d -g=vavWithCloudsGrid2de16.order2.ml3 -nu=.0001 -tf=25. -tp=.25 -ts=afs -cfl=2. -ad21=1. -ad2=1. -project=0 -go=halt
#
#
#
# - Parallel
#
#	mpirun -np 1 $cginsp vavWithClouds2d -g=vavWithCloudsGrid2de2.order2.ml1 -nu=.001 -tf=400. -tp=.1 -ts=im -project=0 -go=halt -solver=best -psolver=best -inflowVelocity=0. -Tin=0.
#
#
#   	mpirun -np 1 $cginsp vavWithClouds2d -g=vavWithCloudsGrid2de8.order2.ml2 -nu=.00001 -tf=400. -tp=.1 -ts=im -project=0 -go=halt -solver=best -psolver=best -inflowVelocity=0. -Tin=0.
#
# --- Factor 16
#
#   	mpirun -np 4 $cginsp -noplot vavWithClouds2d -g=vavWithCloudsGrid2de16.order2.ml3 -nu=.000001 -tf=400. -tp=.1 -ts=im -project=0 -go=run -solver=best -psolver=best -inflowVelocity=0. -Tin=0. -show=Cloud_factor16_im.show -debug=0
#
#   	mpirun -np 6 $cginsp vavWithClouds2d -g=vavWithCloudsGrid2de16.order2.ml3 -nu=.0001 -tf=10. -tp=.1 -ts=afs -cfl=2. -ad21=1. -ad2=1. -project=0 -go=halt -solver=best -psolver=best -numParallelGhost=4 -show=Cloud_factor16_afs.show
#
#
#  -Floor Heat run
#
#	mpirun -np 8 $cginsp -noplot vavWithClouds2d -g=vavWithCloudsGrid2de16.order2.ml3 -nu=.000001 -tf=400. -tp=.25 -ts=im -project=0 -go=run -solver=best -psolver=best -inflowVelocity=0. -Tin=0. -show=Cloud_factor16_im.show -debug=0
#
#
#
#  - Floor Heat comparison
#
# 	cgins vavWithClouds2d -g=vavWithCloudsGrid2de4.order2.ml2 -nu=.00001 -tf=100. -tp=.1 -ts=im -project=0 -go=run -solver=best -psolver=best -inflowVelocity=0. -Tin=0. -show=Cloud_factor4_serial.show -debug=0
#
#
# 	mpirun -np 4 $cginsp vavWithClouds2d -g=vavWithCloudsGrid2de4.order2.ml2 -nu=.00001 -tf=100. -tp=.1 -ts=im -project=0 -go=run -solver=best -psolver=best -inflowVelocity=0. -Tin=0. -show=Cloud_factor4_parallel.show -debug=0
#
#
#
#
#
# 
# --- set default values for parameters ---
# 
# -- we use MKS here 
$f2m = .3048; # feet to meters conversion (exact)
# $m2f = 1./.3048; # meters to feet
## $f2m = 1.; # use units of feet
$grid="vavWithCloudsGrid2de2.order2.ml1"; $ts="im"; $noplot=""; $backGround="square"; $frequencyToFlush=3; 
$vIn=0.; $v0=0.; $T0=0.; $cfl=.9; $useNewImp=1; $newts=0;
$inflowVelocity=2.; $tFinal=600.; $tPlot=.1; $dtMax=.01; $degreeSpace=2; $degreeTime=2; $show="vavWithClouds2d.show"; $debug=0; $go="halt";
$nu=.1; $Prandtl=.72; $thermalExpansivity=3.4e-3; $Tin=-5.;  $implicitFactor=.5; $implicitVariation="viscous"; 
$lowerLeftWallT=0.; $upperLeftWallT=0.;
$tz=0; # turn on tz here
$ad2=1; $ad21=2.; $ad22=2.; 
$ad4=0; $ad41=.5; $ad42=.5; 
$thermalConductivity=.026; # air at 20C  
$adcBoussinesq=.5;  
$outflowOption="neumann";
$accelerationDueToGravity=9.81;
$gravity = "0 -$accelerationDueToGravity 0."; $cdv=1.; $cDt=.25; $project=1; $restart=""; 
# $solver="choose best iterative solver";
$solver="yale";  $rtoli=1.e-3; $atoli=1.e-3; $idebug=0; 
$psolver="yale"; $rtolp=1.e-3; $atolp=1.e-3; $pdebug=0;
$pc="ilu"; $refactorFrequency=500; 
$probeFile="vav2dProbe";
# -- for Kyle's AF scheme:
$afit = 10;
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2;
$cdv=1;  $cDt=.25;
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"implicitFactor=f"=>\$implicitFactor, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"refactorFrequency=i"=>\$refactorFrequency, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"Prandtl=f"=>\$Prandtl,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "noplot=s"=>\$noplot,\
 "go=s"=>\$go,"dtMax=f"=>\$dtMax,"cDt=f"=>\$cDt,"iv=s"=>\$implicitVariation,\
 "inflowVelocity=f"=>\$inflowVelocity,"Tin=f"=>\$Tin,\
 "solver=s"=>\$solver,"psolver=s"=>\$psolver,"pc=s"=>\$pc,"outflowOption=s"=>\$outflowOption,\
 "debug=i"=>\$debug,"pdebug=i"=>\$pdebug,"idebug=i"=>\$idebug,"project=i"=>\$project,"cfl=f"=>\$cfl,\
 "restart=s"=>\$restart,"useNewImp=i"=>\$useNewImp,"adcBoussinesq=f"=>\$adcBoussinesq,\
 "frequencyToFlush=i"=>\$frequencyToFlush,"newts=i"=>\$newts,"thermalConductivity=f"=>\$thermalConductivity,\
  "ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,\
  "lowerLeftWallT=f"=>\$lowerLeftWallT,"upperLeftWallT=f"=>\$upperLeftWallT,"probeFile=s"=>\$probeFile );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $pc eq "ilu" ){ $pc = "incomplete LU preconditioner"; }elsif( $pc eq "lu" ){ $pc = "lu preconditioner"; }else{ $pc="#"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
if( $useNewImp eq 1 ){ $useNewImp ="useNewImplicitMethod"; }else{ $useNewImp="#"; }
if( $implicitVariation eq "viscous" ){ $implicitVariation = "$useNewImp\n implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "$useNewImp\n implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "$useNewImp\n implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $ts eq "fe" ){ $ts="forward Euler";}
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "pc4" ){ $ts="adams PC order 4"; }
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;}
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
if( $project eq "1" && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
#
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
$kThermal=$nu/$Prandtl;
#
# specify the grid: 
$grid
# 
  incompressible Navier Stokes
  Boussinesq model
#   define real parameter kappa $kThermal
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq $adcBoussinesq
  continue
# 
  OBTZ:twilight zone flow $tz
  OBTZ:polynomial
  degree in space $degreeSpace
  degree in time $degreeTime
# 
# ------------------------------------------
# choose time stepping method:
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
# -----------------------------------------
#   
 # number of PC corrections 5
  $implicitVariation
  implicit factor $implicitFactor 
  refactor frequency $refactorFrequency
# 
  choose grids for implicit
    all=implicit
   done
# 
#
  final time $tFinal
  times to plot $tPlot
  dtMax $dtMax
  pde parameters
    nu  $nu
    kThermal $kThermal
    thermal conductivity $thermalConductivity
    gravity
      $gravity
#
    OBPDE:divergence damping  $cdv 
    OBPDE:cDt div damping $cDt
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22 $ad21, $ad22
    OBPDE:fourth-order artificial diffusion $ad4
    OBPDE:ad41,ad42 $ad41, $ad42
    # MG solver currently wants a Neumann BC at outflow
    if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
      $cmd
   done
# 
  debug $debug
#**
  show file options
    compressed
     OBPSF:maximum number of parallel sub-files 8
      open
      $show
    frequency to flush
      $frequencyToFlush
    exit
#**
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesPC=$pc; $ogesDebug=$pdebug; $ogmgDebug=$pdebug;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtoli; $ogesAtol=$atoli; $ogesPC=$pc; $ogesDebug=$idebug; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
 cfl $cfl
# 
#
#
#
  body forcing...
    # -- Occupants --
    choose region...
       $xWidtho=2.25*$f2m;  $yWidtho=3.*$f2m;  
#       $p12a_x = 14.50 * $f2m;   $p12a_y = 12.50 * $f2m;
#       $p12b_x = 16.75 * $f2m;   $p12b_y = 14.75 * $f2m;
#       $p01a_z =  1.50 * $f2m;   $p01b_z =  4.50 * $f2m;
#
    $n=0; # number of occupants 
    # lower left corner of occupants (in feet!)
#
    $ox[$n]= 1.75;  $n=$n+1;
    $ox[$n]= 7.0;   $n=$n+1;
    $ox[$n]= 14.5;  $n=$n+1;
    $ox[$n]= 19.75; $n=$n+1;
    $ox[$n]= 27.25; $n=$n+1;
#
    $occupantHeatSource=0.; # fix me -- what should this be ??
    $cmd=""; 
    for( $m=0; $m<$n; $m++ ){ \
      $xa=$ox[$m]*$f2m; $xb=$xa+$xWidtho; $ya=1.5*$f2m; $yb=$ya+$yWidtho; \
      $cmd .= "box: $xa $xb $ya $yb -.01 .01 (xa,xb, ya,yb, za,zb)\n"; \
      $cmd .= "heat coefficient: $occupantHeatSource\n "; \
      $cmd .= "add immersed boundary\n "; \
      $cmd .= "add heat source\n "; \
    }
    $cmd .= "#";
    $cmd
#
  exit
  boundary conditions
    # by default all walls are isothermal no-slip:
    all=noSlipWall
    $domainLength=32.*$f2m; $apn=$domainLength; 
    bcNumber12=outflow , pressure(1.*p+$apn*p.n=0.)
    ##  ** bcNumber12=outflow , pressure(1.*p+0*p.n=0.)
    # 
    # --- Add local inflows on some walls: 
    #
#    $inflowWidth=4.*$f2m; $inflowHeight=3.; # width and height of inflow boxes
    $parabolicWidth=.25*$f2m;   # width of parabolic inflow profile
#
    # ----------------------------
    # -- Inflow on Diffusers
    # ----------------------------
    $uInflow=0.; $vInflow=-$inflowVelocity; $wInflow=0; $TInflow=$Tin; 
    bcNumber4=noSlipWall 
#
    #
    # -- clouds: adiabtaic:
    #
    bcNumber7=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
         # -- This local inflow didn't work for some reason (??)
     # cloud 2
     bcNumber8=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
     # cloud 3 
     bcNumber9=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    #
    # diffuser1
    $d=.1; # parabolic inflow 
    bcNumber10=inflowWithVelocityGiven, parabolic(d=$d,p=1.,v=$vInflow,T=$TInflow)
    #
    # diffuser2
    $d=.1; # parabolic inflow 
    bcNumber11=inflowWithVelocityGiven, parabolic(d=$d,p=1.,v=$vInflow,T=$TInflow)
    # 
    # --- Set local temperature regions on the floor: (isothermal wall) ---
    #
    bcNumber3=noSlipWall , variableBoundaryData
#
     $floorPatch1Temperature=5.; 
     $floorPatch2Temperature=0;  
       # Smooth out the transition region where the forcing turns on using a tanh profile:
     tanh forcing profile
       # Note: this exponent has units of 1/Length and thus should be smaller for larger domains 
     $tanhExponent = 40./$domainLength; 
     tanh exponent: $tanhExponent
#
       # floor patch1 : [0,16]x[0,0]x[0,0]
     $xa=0; $xb=16.0*$f2m; $ya=-.01; $yb=.01; $za=-.01; $zb=.01; 
     box 
     box: $xa $xb $ya $yb $za $zb (xa,xb, ya,yb, za,zb)
     temperature forcing: $floorPatch1Temperature
     boundary forcing name: floorPatch1
     add temperature forcing
#
       # floor patch2 : [16,32]x[0,0]x[0,0]
     $xa=16.*$f2m; $xb=32.0*$f2m; $ya=-.01; $yb=.01; $za=-.01; $zb=.01; 
     box 
     box: $xa $xb $ya $yb $za $zb (xa,xb, ya,yb, za,zb)
     temperature forcing: $floorPatch2Temperature
     boundary forcing name: floorPatch2
     add temperature forcing
     exit
#
    # 
    # --- Set local temperature regions on the left wall: (isothermal wall) ---
	#
    bcNumber1=noSlipWall , variableBoundaryData
#
      # -- wall patches $tw[$m] = wall temperature (fixed for now)
      $n=0; 
      # left
      $xaw[$n]=-.01; $xbw[$n]=.01; $yaw[$n]= 0.; $ybw[$n]=7.25; $zaw[$n]=-.01; $zbw[$n]=.01; $tw[$n]=$lowerLeftWallT;  $wallForce[$n]="lowerLeftWall"; $n++;
      $xaw[$n]=-.01; $xbw[$n]=.01; $yaw[$n]= 7.5; $ybw[$n]=14.5; $zaw[$n]=-.01; $zbw[$n]=.01; $tw[$n]=$upperLeftWallT; $wallForce[$n]="upperLeftWall"; $n++;
 #
    $cmd=""; 
    for( $m=0; $m<$n; $m++ ){ \
      $xa=$xaw[$m]*$f2m; $xb=$xbw[$m]*$f2m; \
      $ya=$yaw[$m]*$f2m; $yb=$ybw[$m]*$f2m; \
      $za=$zaw[$m]*$f2m; $zb=$zbw[$m]*$f2m; \
      $cmd .= "box\n"; \
      $cmd .= "box: $xa $xb $ya $yb $za $zb (xa,xb, ya,yb, za,zb)\n"; \
      $cmd .= "temperature forcing: $tw[$m]\n "; \
      $cmd .= "boundary forcing name: $wallForce[$m]\n "; \
      $cmd .= "add temperature forcing\n "; \
    }
    $cmd .= "#";
    $cmd    
    exit
#
    # 
    # --- Set local temperature regions on the right wall: (isothermal wall) ---
	#
    bcNumber2=noSlipWall , variableBoundaryData
      # -- wall patches $tw[$m] = wall temperature (fixed for now)
      $n=0; 
      # right
      $xaw[$n]=32.-.01; $xbw[$n]=32.+.01; $yaw[$n]= 0.; $ybw[$n]=7.25; $zaw[$n]=-.01; $zbw[$n]=.01; $tw[$n]=0.; $wallForce[$n]="lowerRightWall"; $n++;
      $xaw[$n]=32.-.01; $xbw[$n]=32.+.01; $yaw[$n]= 7.25; $ybw[$n]=14.5; $zaw[$n]=-.01; $zbw[$n]=.01; $tw[$n]=0.; $wallForce[$n]="upperRightWall"; $n++;
 #
    $cmd=""; 
    for( $m=0; $m<$n; $m++ ){ \
      $xa=$xaw[$m]*$f2m; $xb=$xbw[$m]*$f2m; \
      $ya=$yaw[$m]*$f2m; $yb=$ybw[$m]*$f2m; \
      $za=$zaw[$m]*$f2m; $zb=$zbw[$m]*$f2m; \
      $cmd .= "box\n"; \
      $cmd .= "box: $xa $xb $ya $yb $za $zb (xa,xb, ya,yb, za,zb)\n"; \
      $cmd .= "temperature forcing: $tw[$m]\n "; \
      $cmd .= "boundary forcing name: $wallForce[$m]\n "; \
      $cmd .= "add temperature forcing\n "; \
    }
    $cmd .= "#";
    $cmd    
    exit
#
  done
#
#  ===== Create probes =====
#
#   save probes every this many time steps:
    frequency to save probes 100
#
#   -- here is a probe BOUNDARY region (save results in the first file)
  create a probe 
    probe name floorPatch1HeatFlux
    $probeFileName= $probeFile . "HeatFlux.dat"; 
    file name $probeFileName
    boundary forcing region: floorPatch1
    heat flux
    total
    volume weighted sum
  exit
  create a probe 
    probe name floorPatch2HeatFlux
    file name $probeFileName
    boundary forcing region: floorPatch2
    heat flux
    total
    volume weighted sum
  exit
  create a probe 
    probe name lowerLeftWallHeatFlux
    file name $probeFileName
    boundary forcing region: lowerLeftWall
    heat flux
    total
    volume weighted sum
  exit
  create a probe 
    probe name upperLeftWallHeatFlux
    file name $probeFileName
    boundary forcing region: upperLeftWall
    heat flux
    total 
    volume weighted sum
  exit
#   -- here is a location probe:
  create a probe
    probe name temperatureProbe1
    $probeFileName= $probeFile . "TemperatureProbe1.dat"; 
    file name $probeFileName
    temperature
    location 3.5 .5 0.
  exit
# 
  if( $restart eq "" ){ $cmds = "uniform flow\n" . "p=1., u=0., v=$v0, T=$T0\n"; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
#
  initial conditions
    $cmds
  exit
# 
  $project
  continue
  plot:T
#  plot:u
#
  contour
    vertical scale factor 0.
    # min max -6 2
  exit
$go 


  
