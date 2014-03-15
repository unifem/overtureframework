#
# cgins: Purdue's Living Lab VAV room
#       
#  cgins [-noplot] vavWithDiffusers -g=<name> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -ts=[im|pc|afs] -debug=<num> ..,
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
#     - 1 m/s = 2.24 mph
#     
# Examples:
#    cgins vavWithDiffusers -g=vavWithDiffusersGride1.order2.ml1.hdf -nu=.01 -tf=10. -tp=.01 -ts=pc -debug=3 -project=0 -go=halt [OK
#    cgins vavWithDiffusers -g=vavWithDiffusersGride2.order2.hdf -nu=.01 -tf=10. -tp=.01 -ts=pc -debug=3 -go=halt [OK
# 
# --- set default values for parameters ---
# 
$f2m = .3048; # feet to meters conversion (exact)
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
$gravity = "0 0. -$accelerationDueToGravity"; $cdv=1.; $cDt=.25; $project=1; $restart="";  $restartSolution=-1;
# $solver="choose best iterative solver";
$solver="best";  $rtoli=1.e-3; $atoli=1.e-4; $idebug=0; 
$psolver="best"; $rtolp=1.e-3; $atolp=1.e-4; $pdebug=0;
$pc="ilu"; $refactorFrequency=500; 
# -- for Kyle's AF scheme:
$afit = 10;
$aftol=1e-2;
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
 "frequencyToFlush=i"=>\$frequencyToFlush,"newts=i"=>\$newts,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,\
 "restartSolution=i"=>\$restartSolution );
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
  body forcing...
    # -- Occupants --
    choose region...
       $xWidtho=2.25*$f2m;  $yWidtho=2.25*$f2m; $zHeighto=3.*$f2m; 
       $p12a_x = 14.50 * $f2m;   $p12a_y = 12.50 * $f2m;
       $p12b_x = 16.75 * $f2m;   $p12b_y = 14.75 * $f2m;
       $p01a_z =  1.50 * $f2m;   $p01b_z =  4.50 * $f2m;
#
    $n=0; # number of occupants 
    # lower left corner of occupants (in feet!)
#
    $ox[$n]= 1.75; $oy[$n]=5.5;   $n=$n+1;
    $ox[$n]= 1.75; $oy[$n]=10.0;  $n=$n+1;
    $ox[$n]= 1.75; $oy[$n]=20.0;  $n=$n+1;
    $ox[$n]= 1.75; $oy[$n]=24.5;  $n=$n+1;
#
    $ox[$n]= 7.0; $oy[$n]=4.5;    $n=$n+1;
    $ox[$n]= 7.0; $oy[$n]=9.0;    $n=$n+1;
    $ox[$n]= 7.0; $oy[$n]=13.75;  $n=$n+1;
    $ox[$n]= 7.0; $oy[$n]=18.5;   $n=$n+1;
    $ox[$n]= 7.0; $oy[$n]=23.;    $n=$n+1;
#
    $ox[$n]=14.5; $oy[$n]=3.25;   $n=$n+1;
    $ox[$n]=14.5; $oy[$n]=7.75;   $n=$n+1;
    $ox[$n]=14.5; $oy[$n]=12.5;   $n=$n+1;
    $ox[$n]=14.5; $oy[$n]=17.25;  $n=$n+1;
    $ox[$n]=14.5; $oy[$n]=21.75;  $n=$n+1;
#
    $ox[$n]=19.75; $oy[$n]=4.5;   $n=$n+1;
    $ox[$n]=19.75; $oy[$n]=9.0;   $n=$n+1;
    $ox[$n]=19.75; $oy[$n]=13.75; $n=$n+1;
    $ox[$n]=19.75; $oy[$n]=18.5;  $n=$n+1;
    $ox[$n]=19.75; $oy[$n]=23.;   $n=$n+1;
#
    $ox[$n]=27.25; $oy[$n]=3.25;   $n=$n+1;
    $ox[$n]=27.25; $oy[$n]=7.75;   $n=$n+1;
    $ox[$n]=27.25; $oy[$n]=12.5;   $n=$n+1;
    $ox[$n]=27.25; $oy[$n]=17.25;  $n=$n+1;
    $ox[$n]=27.25; $oy[$n]=21.75;  $n=$n+1;
#
    $occupantHeatSource=2.; 
    $cmd=""; 
    for( $m=0; $m<$n; $m++ ){ \
      $xa=$ox[$m]*$f2m; $xb=$xa+$xWidtho; $ya=$oy[$m]*$f2m; $yb=$ya+$yWidtho; $za=$p01a_z; $zb=$p01b_z; \
      $cmd .= "box: $xa $xb $ya $yb $za $zb (xa,xb, ya,yb, za,zb)\n"; \
      $cmd .= "region lines: 6 6 9\n"; \
      $cmd .= "heat coefficient: $occupantHeatSource\n "; \
      $cmd .= "add immersed boundary\n "; \
      $cmd .= "add heat source\n "; \
    }
    $cmd .= "#";
    $cmd
#
    # -- clouds ---
    # Clouds should be adiabatic : do NOT force T=T_0
    $cmd = "";
    $xc=2.*$f2m; $yc=2.*$f2m;  # lower left corner of first cloud
    ## wdh $cloud_thickness = 0.5 * $f2m;
    $cloud_thickness = 1.0 * $f2m;  # make clouds thicker on coarse grids for immersed
    $xWidthc=8.*$f2m; $yWidthc=8.*$f2m;   # width of clouds
    for( $i=0; $i<3; $i++ ){ for( $j=0; $j<3; $j++ ){\
      $xa = $xc + $i*10.*$f2m; $xb = $xa + $xWidthc; \
      $ya = $yc + $j*10.*$f2m; $yb = $ya + $yWidthc; \
      $za = 10.*$f2m; $zb=$za+$cloud_thickness; \
      $cmd .= "box: $xa $xb $ya $yb $za $zb (xa,xb, ya,yb, za,zb)\n"; \
      $cmd .= "region lines: 11 11 3\n"; \
      $cmd .= "add immersed boundary\n "; \
    }}
    $cmd .= "#";
    $cmd  
#
#   # --- cloud diffusers: set 
#
#
    $xWidthd=2.*$f2m;  $yWidthd=2.*$f2m;  # width of diffusers
    $zd = 10.*$f2m;  # diffuser z height
    $ds=.05;
    $za = $zd - 2.*$ds; $zb = $zd + 2.*$ds;   # make diffuser a thin box
    $ni=0; # number of diffusers
    # lower left corner of diffusers (in feet!)
#    -- locations of the diffusers (inlets)
    $ix[$ni]= 7.75; $iy[$ni]=6.0;   $ni=$ni+1;
    $ix[$ni]= 7.75; $iy[$ni]=12.25; $ni=$ni+1;
    $ix[$ni]= 7.75; $iy[$ni]=17.75; $ni=$ni+1;
    $ix[$ni]= 7.75; $iy[$ni]=24.0;  $ni=$ni+1;
#
    $ix[$ni]= 22.25; $iy[$ni]=6.0;   $ni=$ni+1;
    $ix[$ni]= 22.25; $iy[$ni]=12.25; $ni=$ni+1;
    $ix[$ni]= 22.25; $iy[$ni]=17.75; $ni=$ni+1;
    $ix[$ni]= 22.25; $iy[$ni]=24.0;  $ni=$ni+1;
#
    $cmd=""; 
    for( $m=0; $m<$ni; $m++ ){ \
      $xa=$ix[$m]*$f2m; $xb=$xa+$xWidthd; $ya=$iy[$m]*$f2m; $yb=$ya+$yWidthd; \
      $cmd .= "box: $xa $xb $ya $yb $za $zb (xa,xb, ya,yb, za,zb)\n"; \
      $cmd .= "heat coefficient: -1\n "; \
      $cmd .= "add drag force\n "; \
      $cmd .= "add heat source\n "; \
    }
    $cmd .= "#";
    ## turn off $cmd
#
  exit
  boundary conditions
    # by default all walls are isothermal no-slip:
    all=noSlipWall
    # Here is an adiabatic (heat flux) wall:
    ## all=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    #- all=noSlipWall , mixedDerivative(1.*t+1.*t.n=0.)
    # bcNumber11=inflowWithVelocityGiven, uniform(p=1.,v=$vIn,T=$Tin)
    # bcNumber11=inflowWithVelocityGiven, parabolic(p=1.,v=$vIn,T=$Tin)
    # The outflow pressure BC scales with the size of the domain:  p.n ~ p.x ->  p.r/L   x=r*L 
    $domainLength=32.*$f2m; $apn=$domainLength; 
    bcNumber12=outflow , pressure(1.*p+$apn*p.n=0.)
    # 
    # --- Add local inflows on some walls: 
    #
    $inflowWidth=2.*$f2m; $inflowHeight=3.; # width and height of inflow boxes
    $parabolicWidth=.25*$f2m;   # width of parabolic inflow profile
    # ----------------------------
    # -- Inflow on east face x=0
    # ----------------------------
    $uInflow=$inflowVelocity; $vInflow=0.; $wInflow=0; $TInflow=10.; 
#+    bcNumber1=noSlipWall variableBoundaryData
#+      box
#+       $xai=0.-.01; $xbi=0.+.01; $yai=2.*$f2m; $ybi=$yai+$inflowWidth; $zai=2*$f2m.; $zbi=$zai+$inflowHeight; 
#+       box: $xai $xbi $yai $ybi $zai $zbi (xa,xb, ya,yb, za,zb)
#+       velocity forcing: $uInflow $vInflow $wInflow
#+       # define a parabolic inflow profile
#+       parabolic forcing profile
#+       parabolic depth: $parabolicWidth
#+       add velocity forcing
#+       # set the temperature at inflow: 
#+       temperature forcing: $TInflow
#+       add temperature forcing
#+     exit
    # 
    # --- Set local temperature regions on the floor: (isothermal wall) ---
    #
    bcNumber5=noSlipWall , variableBoundaryData
#
     # Smooth out the transition region where the forcing turns on using a tanh profile:
      tanh forcing profile
      # Note: this exponent has units of 1/Length and thus should be smaller for larger domains 
      $tanhExponent = 40./$domainLength; 
      tanh exponent: $tanhExponent
     #
      # -- wall patches $tw[$m] = wall temperature (fixed for now)
      $n=0; 
      # bottom:
      $xaw[$n]= 0.; $xbw[$n]=16.; $yaw[$n]= 0.; $ybw[$n]=10.; $zaw[$n]=-$ds; $zbw[$n]=$ds; $tw[$n]=.1; $n++;
      $xaw[$n]= 0.; $xbw[$n]=16.; $yaw[$n]=10.; $ybw[$n]=20.; $zaw[$n]=-$ds; $zbw[$n]=$ds; $tw[$n]=.2; $n++;
      $xaw[$n]= 0.; $xbw[$n]=16.; $yaw[$n]=20.; $ybw[$n]=32.; $zaw[$n]=-$ds; $zbw[$n]=$ds; $tw[$n]=.3; $n++;
      #
      $xaw[$n]=16.; $xbw[$n]=32.; $yaw[$n]= 0.; $ybw[$n]=10.; $zaw[$n]=-$ds; $zbw[$n]=$ds; $tw[$n]=.4; $n++;
      $xaw[$n]=16.; $xbw[$n]=32.; $yaw[$n]=10.; $ybw[$n]=20.; $zaw[$n]=-$ds; $zbw[$n]=$ds; $tw[$n]=.5; $n++;
      $xaw[$n]=16.; $xbw[$n]=32.; $yaw[$n]=20.; $ybw[$n]=32.; $zaw[$n]=-$ds; $zbw[$n]=$ds; $tw[$n]=.6; $n++;
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
#    -- west wall --
#
    bcNumber1=noSlipWall , variableBoundaryData
     $n=0; 
      # west
      $xaw[$n]=-$ds; $xbw[$n]=$ds; $yaw[$n]= 0.; $ybw[$n]=32.; $zaw[$n]=0.0; $zbw[$n]=5.0; $tw[$n]=.7; $n++;
      $xaw[$n]=-$ds; $xbw[$n]=$ds; $yaw[$n]= 0.; $ybw[$n]=32.; $zaw[$n]=5.0; $zbw[$n]=10.; $tw[$n]=.8; $n++;
      $xaw[$n]=-$ds; $xbw[$n]=$ds; $yaw[$n]= 0.; $ybw[$n]=32.; $zaw[$n]=10.; $zbw[$n]=14.5;$tw[$n]=.9; $n++;
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
#    -- east wall --
#
    bcNumber2=noSlipWall , variableBoundaryData
     $n=0; 
      # east
      $xaw[$n]=32.-$ds; $xbw[$n]=32.+$ds; $yaw[$n]= 0.; $ybw[$n]=32.; $zaw[$n]=0.0; $zbw[$n]=5.0; $tw[$n]=.7; $n++;
      $xaw[$n]=32.-$ds; $xbw[$n]=32.+$ds; $yaw[$n]= 0.; $ybw[$n]=32.; $zaw[$n]=5.0; $zbw[$n]=10.; $tw[$n]=.8; $n++;
      $xaw[$n]=32.-$ds; $xbw[$n]=32.+$ds; $yaw[$n]= 0.; $ybw[$n]=32.; $zaw[$n]=10.; $zbw[$n]=14.5;$tw[$n]=.9; $n++;
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
#    -- south wall --
#
    bcNumber3=noSlipWall , variableBoundaryData
     $n=0; 
      # south
      $xaw[$n]=0.; $xbw[$n]=32.; $yaw[$n]=-$ds; $ybw[$n]=$ds; $zaw[$n]=0.0; $zbw[$n]=6.5; $tw[$n]=.7; $n++;
      $xaw[$n]=0.; $xbw[$n]=32.; $yaw[$n]=-$ds; $ybw[$n]=$ds; $zaw[$n]=6.5; $zbw[$n]=10.; $tw[$n]=.8; $n++;
      $xaw[$n]=0.; $xbw[$n]=32.; $yaw[$n]=-$ds; $ybw[$n]=$ds; $zaw[$n]=10.; $zbw[$n]=14.5;$tw[$n]=.9; $n++;
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
#    -- north wall --
#
    bcNumber4=noSlipWall , variableBoundaryData
     $n=0; 
      # north
      $xaw[$n]=0.; $xbw[$n]=32.; $yaw[$n]=32.-$ds; $ybw[$n]=32.+$ds; $zaw[$n]=0.0; $zbw[$n]=5.0; $tw[$n]=.7; $n++;
      $xaw[$n]=0.; $xbw[$n]=32.; $yaw[$n]=32.-$ds; $ybw[$n]=32.+$ds; $zaw[$n]=5.0; $zbw[$n]=10.; $tw[$n]=.8; $n++;
      $xaw[$n]=0.; $xbw[$n]=32.; $yaw[$n]=32.-$ds; $ybw[$n]=32.+$ds; $zaw[$n]=10.; $zbw[$n]=14.5;$tw[$n]=.9; $n++;
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
#   Define inflow on the lower half of the diffusers (spheres)
#
    $diffuserInflowVelocity=-1.; 
    $diffuserInflowT=-1.; 
    $za=$zd-.5*$xWidthd-.1; $zb=$zd;
    $cmd=""; 
    for( $m=0; $m<$ni; $m++ ){ \
      $xa=$ix[$m]*$f2m-.1; $xb=$xa+$xWidthd+.2; $ya=$iy[$m]*$f2m-.1; $yb=$ya+$yWidthd+.2; \
      $bc=20+$m; $bcn="bcNumber$bc"; \
      $cmd .= "$bcn=noSlipWall, variableBoundaryData\n"; \
      $cmd .= " box: $xa $xb $ya $yb $za $zb (xa,xb, ya,yb, za,zb)\n"; \
      $cmd .= " velocity forcing: 0. 0. $diffuserInflowVelocity\n"; \
      $cmd .= " add velocity forcing\n "; \
      $cmd .= " temperature forcing: $diffuserInflowT\n "; \
      $cmd .= " add temperature forcing\n "; \
      $cmd .= "exit\n "; \
      $cmd .= " box: $xa $xb $ya $yb $za $zb (xa,xb, ya,yb, za,zb)\n"; \
      $cmd .= " velocity forcing: 0. 0. $diffuserInflowVelocity\n"; \
      $cmd .= " add velocity forcing\n "; \
      $cmd .= " temperature forcing: $diffuserInflowT\n "; \
      $cmd .= " add temperature forcing\n "; \
      $cmd .= "exit\n "; \
    }
    $cmd .= "#";
    $cmd
#
  done
# 
  if( $restart eq "" ){ $cmds = "uniform flow\n" . "p=1., u=0., v=$v0, T=$T0\n"; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number $restartSolution \n OBIC:assign solution from show file"; }
#
  initial conditions
    $cmds
  exit
# 
  $project
  continue
#  plot:T
  plot:u
#
  grid
    plot shaded surfaces (3D) 0
    plot grid lines 0
    # turn off spheres:
    plot block boundaries 0
  exit this menu
  contour
    delete contour plane 2
    delete contour plane 1
    delete contour plane 0
    # x-contour plane through outflow 
    #$x1=9.;  $y1=0.; $z1=0.; 
    #add contour plane  1.00000e+00  0.00000e+00  0.00000e+00  $x1 $y1 $z1
    # z-plane 4 ft off the floor:
    #$x1=0.;  $y1=0.; $z1=4.; 
    #add contour plane  0.00000e+00  0.00000e+00  1.00000e+00  $x1 $y1 $z1
    # --
    pick to add contour plane y
    # y-plane through south diffusers
    add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  2.37831e+00  2.10470e+00  3.14570e+00 
    # y plane through north diffusers
    add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  2.66330e+00  7.61377e+00  3.40867e+00 
    # y-planes through middle diffusers
    # add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  2.62341e+00  5.64870e+00  3.40867e+00 
    # add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  2.66330e+00  3.99914e+00  3.40867e+00 
    # x-plane through outlet 
    add contour plane  1.00000e+00  0.00000e+00  0.00000e+00  4.33468e+00  9.72224e+00  3.61646e+00 
    # x-plane through east (right) diffusers
    add contour plane  1.00000e+00  0.00000e+00  0.00000e+00  7.09030e+00  4.02573e+00  3.40867e+00 
    plot:T
    base min/max on contour plane values
    # nice colours for T: 
    min max -1.4 1.
    contour lines 0
  exit
#
  DISPLAY SQUARES:0 0
  x-r 90
  set home
#
  DISPLAY COLOUR BAR:0 0
  DISPLAY AXES:0 0
  forcing regions plot options
  body force shaded surfaces 1
  body force grid lines 1
  exit
  # view from west above:
  ##- set view:0 0.00906344 -0.0271903 0 1.04088 0.766044 0.219846 -0.604023 -0.642788 0.262003 -0.719846 3.93594e-17 0.939693 0.34202#
  # view from west below:
  set view:0 0.00906344 -0.0271903 0 1.05361 0.766044 -0.111619 -0.633022 -0.642788 -0.133022 -0.754407 3.93594e-17 0.984808 -0.173648
  # DISPLAY LABELS:0 0
$go




#
  contour
    pick to delete contour planes
    delete contour plane 2
    add contour plane  0.00000e+00  0.00000e+00  1.00000e+00  3.36925e-01  2.75221e+00  4.99791e-01 
    add contour plane  1.00000e+00  0.00000e+00  0.00000e+00  1.98829e+00  2.75066e+00  1.63576e+00 
  exit
  y+r 20
  x+r 20
# 
  $go
  
