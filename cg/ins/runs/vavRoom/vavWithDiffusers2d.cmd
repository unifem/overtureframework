#
# cgins: Purdue's Living Lab VAV room ** 2D cross section **
#       
#  cgins [-noplot] vav2D -g=<name> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -ts=[im|pc|afs] -debug=<num> ..,
#         -nu=<num> -Prandtl=<num> -bg=<backGround> -show=<name> -implicitFactor=<num> ...,
#         -solver=[best|yale|mg] -psolver=[best|yale|mg] -pc=[ilu|lu] -adcBoussinesq=<>
# Options:
#  -adcBoussinesq : artificial dissipation for the T equations.
#  -project : 1=project initial conditions
#  -solver : implicit solver
#  -psolver : pressure solver
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
# Examples:
#
#  cgins vavWithDiffuser2d -g=vavGrid2De1.order2.ml1.hdf -nu=.05 -tf=10. -tp=.5 -ts=im -debug=1 -project=1 -go=halt
#
# 
# --- set default values for parameters ---
# 
$f2m = .3048; # feet to meters conversion (exact)
# $m2f = 1./.3048; # meters to feet
## $f2m = 1.; # use units of feet
$grid="room3de4.order2"; $ts="adams PC"; $noplot=""; $backGround="square"; $frequencyToFlush=2; 
$vIn=-.5; $v0=0.; $T0=0.; $cfl=.9; $useNewImp=1; $newts=0;
$inflowVelocity=5.; 
$tFinal=600.; $tPlot=.1; $dtMax=.2; $degreeSpace=2; $degreeTime=2; $show=" "; $debug=1; $go="halt";
$nu=.1; $Prandtl=.72; $thermalExpansivity=3.4e-3; $Tin=-10.;  $implicitFactor=.5; $implicitVariation="viscous"; 
$tz=0; # turn on tz here
$ad2=1; $ad21=2.; $ad22=2.; 
$adcBoussinesq=.5;  
$thermalConductivity=.026; # air at 20C  
$outflowOption="neumann";
$accelerationDueToGravity=9.81; 
$gravity = "0 -$accelerationDueToGravity 0."; $cdv=1.; $cDt=.25; $project=1; $restart=""; 
# $solver="choose best iterative solver";
$solver="yale";  $rtoli=1.e-2; $atoli=1.e-3; $idebug=0; 
$psolver="yale"; $rtolp=1.e-2; $atolp=1.e-3; $pdebug=0;
$pc="ilu"; $refactorFrequency=500; 
# -- for Kyle's AF scheme:
$afit = 10;
$aftol=1e-3;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; $ad4=4;
$cdv=1;  $cDt=.25;
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"implicitFactor=f"=>\$implicitFactor, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"refactorFrequency=i"=>\$refactorFrequency, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"Prandtl=f"=>\$Prandtl,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "noplot=s"=>\$noplot,\
 "go=s"=>\$go,"dtMax=f"=>\$dtMax,"cDt=f"=>\$cDt,"iv=s"=>\$implicitVariation,"Tin=f"=>\$Tin,"ad2=i"=>\$ad2,\
 "solver=s"=>\$solver,"psolver=s"=>\$psolver,"pc=s"=>\$pc,"outflowOption=s"=>\$outflowOption,\
 "debug=i"=>\$debug,"pdebug=i"=>\$pdebug,"idebug=i"=>\$idebug,"project=i"=>\$project,"cfl=f"=>\$cfl,\
 "restart=s"=>\$restart,"useNewImp=i"=>\$useNewImp,"adcBoussinesq=f"=>\$adcBoussinesq,"thermalConductivity=f"=>\$thermalConductivity,\
 "frequencyToFlush=i"=>\$frequencyToFlush,"newts=i"=>\$newts,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22 );
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
    # kThermal = thermal diffusivity 
    kThermal $kThermal
    thermal conductivity $thermalConductivity
    gravity
      $gravity
    OBPDE:divergence damping  $cdv 
    OBPDE:cDt div damping $cDt
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22 $ad21, $ad22
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
    $cmd=""; 
    for( $m=0; $m<$n; $m++ ){ \
      $xa=$ox[$m]*$f2m; $xb=$xa+$xWidtho; $ya=1.5*$f2m; $yb=$ya+$yWidtho; \
      $cmd .= "box: $xa $xb $ya $yb -.01 .01 (xa,xb, ya,yb, za,zb)\n"; \
      $cmd .= "heat coefficient: 1.\n "; \
      $cmd .= "add immersed boundary\n "; \
      $cmd .= "add heat source\n "; \
    }
    $cmd .= "#";
    $cmd
#
# -- clouds ---
    # Clouds should be adiabatic : do NOT force T=T_0
    $cmd = "";
    $xc=2.*$f2m; $yc=10.*$f2m;  # lower left corner of first cloud
    # $cloud_thickness = 0.5 * $f2m;
    $cloud_thickness = 1.5 * $f2m;  # make clouds thicker on coarse grids
    $xWidthc=8.*$f2m; # $yWidthc=8.*$f2m;   # width of clouds
    for( $i=0; $i<3; $i++ ){\
      $xa = $xc + $i*10.*$f2m; $xb = $xa + $xWidthc; \
      $ya = 10.*$f2m; $yb=$ya+$cloud_thickness; \
      $cmd .= "box: $xa $xb $ya $yb -.01 .01 (xa,xb, ya,yb, za,zb)\n"; \
      $cmd .= "add immersed boundary\n "; \
    }
    $cmd .= "#";
    $cmd  
#   # --- cloud diffusers: set 
     $xWidthd=2.*$f2m;  $yWidthd=2.*$f2m;  # width of diffusers
#
#+    # diffuser 1: **This doesn't work -- We need a div source !
#+      ## $xa=8.*$f2m; $xb=$xa+$xWidthd; $ya=9.*$f2m; $yb=10.25*$f2m; 
#+      $xa=8.*$f2m; $xb=$xa+$xWidthd; $ya=9.*$f2m; $yb=9.5*$f2m; 
#+      box: $xa $xb $ya $yb -.01 .01 (xa,xb, ya,yb, za,zb)
#+      body temperature: -5.
#+      body velocity: 0. -1.5 0.
#+      # We specify the temperature: 
#+      isothermal body
#+      add immersed boundary
#+      # add heat source
#+    #
#+    # diffuser 1:
#+      $xa=$xa+14.*$f2m; $xb=$xa+$xWidthd;
#+      box: $xa $xb $ya $yb -.01 .01 (xa,xb, ya,yb, za,zb)
#+      body temperature: -5.
#+      body velocity: 0. -1.5 0.
#+      # We specify the temperature: 
#+      isothermal body
#+      add immersed boundary
#+      # add heat source
#
  exit
  boundary conditions
    # by default all walls are isothermal no-slip:
    all=noSlipWall
    $domainLength=32.*$f2m; $apn=$domainLength; 
    bcNumber12=outflow , pressure(1.*p+$apn*p.n=0.)
    # 
    # --- Add local inflows on some walls: 
    #
#    $inflowWidth=4.*$f2m; $inflowHeight=3.; # width and height of inflow boxes
    $parabolicWidth=.25*$f2m;   # width of parabolic inflow profile
    # ----------------------------
    # -- Inflow on ceiling y=14.5*.3048
    # ----------------------------
    $uInflow=0.; $vInflow=-$inflowVelocity; $wInflow=0; $TInflow=-5.; 
    bcNumber4=noSlipWall 
#-    bcNumber4=noSlipWall variableBoundaryData
#-      box
#-       ## $xai=0.-.01; $xbi=0.+.01; $yai=2.*$f2m; $ybi=$yai+$inflowWidth; $zai=2*$f2m.; $zbi=$zai+$inflowHeight; 
#-       $xai=4.*$f2m; $xbi=8.*$f2m; $yai=14.5*$f2m-.01; $ybi=$yai+.02; $zai=0.-.01; $zbi=0.+.01; 
#-       box: $xai $xbi $yai $ybi $zai $zbi (xa,xb, ya,yb, za,zb)
#-       velocity forcing: $uInflow $vInflow $wInflow
#-       # define a parabolic inflow profile
#-       parabolic forcing profile
#-       parabolic depth: $parabolicWidth
#-       add velocity forcing
#-       # set the temperature at inflow: 
#-       temperature forcing: $TInflow
#-       add temperature forcing
#-     exit
    # 
    # --- Set local temperature regions on the floor: (isothermal wall) ---
    #
    bcNumber3=noSlipWall , variableBoundaryData
#
     $floorPatch1Temperature=2.; 
     $floorPatch2Temperature=1.;  
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
     add temperature forcing
     # Define a time varying temperature: 
      define temperature time variation...
        mapping function
          6
        0. 0. 
        .5 1.
        1. 2.
        2. 2.
        3. 2.
       100. 2.
        edit mapping function
          # turn on shape preserving spline -- this will be monotone 
          shape preserving (toggle)
        exit
      exit
       # floor patch2 : [16,32]x[0,0]x[0,0]
     $xa=16.*$f2m; $xb=32.0*$f2m; $ya=-.01; $yb=.01; $za=-.01; $zb=.01; 
     box 
     box: $xa $xb $ya $yb $za $zb (xa,xb, ya,yb, za,zb)
     temperature forcing: $floorPatch2Temperature
     add temperature forcing
     exit
#
    # 
    # --- Set local temperature regions on the left wall: (isothermal wall) ---
	#
    bcNumber1=noSlipWall , variableBoundaryData
      # -- wall patches $tw[$m] = wall temperature (fixed for now)
      $n=0; 
      # left
      $xaw[$n]=-.01; $xbw[$n]=.01; $yaw[$n]= 0.; $ybw[$n]=7.25; $zaw[$n]=-.01; $zbw[$n]=.01; $tw[$n]=.75; $n++;
      $xaw[$n]=-.01; $xbw[$n]=.01; $yaw[$n]= 7.5; $ybw[$n]=14.5; $zaw[$n]=-.01; $zbw[$n]=.01; $tw[$n]=.5; $n++;
 #
    $cmd=""; 
    for( $m=0; $m<$n; $m++ ){ \
      $xa=$xaw[$m]*$f2m; $xb=$xbw[$m]*$f2m; \
      $ya=$yaw[$m]*$f2m; $yb=$ybw[$m]*$f2m; \
      $za=$zaw[$m]*$f2m; $zb=$zbw[$m]*$f2m; \
      $cmd .= "box\n"; \
      $cmd .= "box: $xa $xb $ya $yb $za $zb (xa,xb, ya,yb, za,zb)\n"; \
      $cmd .= "temperature forcing: $tw[$m]\n "; \
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
      $xaw[$n]=32.-.01; $xbw[$n]=32.+.01; $yaw[$n]= 0.; $ybw[$n]=7.25; $zaw[$n]=-.01; $zbw[$n]=.01; $tw[$n]=.2; $n++;
      $xaw[$n]=32.-.01; $xbw[$n]=32.+.01; $yaw[$n]= 7.25; $ybw[$n]=14.5; $zaw[$n]=-.01; $zbw[$n]=.01; $tw[$n]=1.; $n++;
 #
    $cmd=""; 
    for( $m=0; $m<$n; $m++ ){ \
      $xa=$xaw[$m]*$f2m; $xb=$xbw[$m]*$f2m; \
      $ya=$yaw[$m]*$f2m; $yb=$ybw[$m]*$f2m; \
      $za=$zaw[$m]*$f2m; $zb=$zbw[$m]*$f2m; \
      $cmd .= "box\n"; \
      $cmd .= "box: $xa $xb $ya $yb $za $zb (xa,xb, ya,yb, za,zb)\n"; \
      $cmd .= "temperature forcing: $tw[$m]\n "; \
      $cmd .= "add temperature forcing\n "; \
    }
    $cmd .= "#";
    $cmd    
    exit
    #
    #     diffuser 1 : inflow on lower half of the annulus
    #
    bcNumber10=noSlipWall, variableBoundaryData
      box
       $xa=8.*$f2m; $xb=$xa+$xWidthd; $ya=10.*$f2m; 
       $innerRad=$xWidthd*.5;
       $cx = .5*($xa+$xb); $cy=$ya+$xWidthd*.5; 
       # 
       $xai=$xa-.1; $xbi=$xb+.1; $yai=$ya-$innerRad-.1; $ybi=$ya; $zai=0.-.01; $zbi=0.+.01; 
       box: $xai $xbi $yai $ybi $zai $zbi (xa,xb, ya,yb, za,zb)
       velocity forcing: 0. -1. 0. 
       # define a parabolic inflow profile
       # parabolic forcing profile
       # parabolic depth: $parabolicWidth
       add velocity forcing
       # set the temperature at inflow: 
       temperature forcing: -1.
       add temperature forcing
     exit 
    #
    #     diffuser 2 : inflow on lower half of the annulus
    #
    bcNumber11=noSlipWall, variableBoundaryData
      box
       $xa=$xa+14.*$f2m; $xb=$xb+14.*$f2m;
       # 
       $xai=$xa-.1; $xbi=$xb+.1; $yai=$ya-$innerRad-.1; $ybi=$ya; $zai=0.-.01; $zbi=0.+.01; 
       box: $xai $xbi $yai $ybi $zai $zbi (xa,xb, ya,yb, za,zb)
       velocity forcing: 0. -1. 0. 
       # define a parabolic inflow profile
       # parabolic forcing profile
       # parabolic depth: $parabolicWidth
       add velocity forcing
       # set the temperature at inflow: 
       temperature forcing: -1.
       add temperature forcing
     exit 
#
  done
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
  exit
$go 


  
