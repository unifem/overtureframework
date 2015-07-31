#
# cgcns: Heated box in a gravitational field 
#       
#  cgcns [-noplot] heatedBox -g=<name> -tf=<tFinal> -tp=<tPlot> -debug=<num> -model=[Jameson|Godunov]..,
#                            -mu=<num> -Prandtl=<num> -bg=<backGround> -show=<name> 
# 
# Examples:
#    cgcns heatedBox -g=square64.order2.hdf -mu=.02 -tf=3. -tp=.1 -go=halt
#    cgcns heatedBox -g=square128.order2.hdf -mu=.01 -tf=10. -tp=.1 -go=halt
#   
# --- set default values for parameters ---
# 
$grid="square64.order2"; $noplot=""; $backGround="square"; $model="Jameson"; $go="halt"; 
$tFinal=20.; $tPlot=.1; $dtMax=.1; $degreeSpace=2; $degreeTime=2; $show=" "; $debug=1; 
$mu=.05; $Prandtl=.72; $T0=300.; $Twall=400.;
$tz=0; # turn on tz here
$gravity = "0 -10. 0.";
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"implicitFactor=f"=>\$implicitFactor, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"debug=i"=>\$debug,"go=s"=>\$go, \
 "mu=f"=>\$mu,"Prandtl=f"=>\$Prandtl,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "noplot=s"=>\$noplot );
# -------------------------------------------------------------------------------------------------
if( $model eq "Jameson" ){ $model = "compressible Navier Stokes (Jameson)"; }else\
                     { $model = "compressible Navier Stokes (Godunov)"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
$kThermal=$mu/$Prandtl;
#
# specify the grid: 
$grid
#
  $model
  exit
  turn off twilight
#
#
  final time $tFinal
  times to plot $tPlot
# 
  show file options
    compressed
      open
      $show
    frequency to flush
     4
    exit
#**
  reduce interpolation width
    2
# **************
  pde parameters
    mu
     $mu
    kThermal
     $kThermal
    gravity
      $gravity
  done
#
  boundary conditions
    all=slipWall
    # Set T on the top
    bcNumber4=noSlipWall, uniform(T=$T0)
    # Heat the bottom
    bcNumber3=noSlipWall, uniform(T=$Twall)
    done
#  debug
#    1
#
#  --- to avoid large initial sound waves, initialize the density to be the steady
#      stratified profile. 
  initial conditions 
    OBIC:user defined...
      gravitationally stratified
 # rho(y) = rho0*exp( gravity[1]/(Rg*T0) ( y - y0 ))
        $rho0=1.; $y0=0.; 
         $rho0 $y0 
      r=1. u=0. v=0. T=$T0
     exit
   exit
  continue
# 
$go







#
#  cgcns command file: heated box (or square)
# 
# Usage:
#    cgcns [-noplot] heatedBox.cmd -g=<grid> -tf=<final time> -tp=<tPlot> -show=<show file>  ...
#                                  -method=[jameson|godunov]
#
# Examples:
#   cgcns heatedBox -g=square128.order2 -tf=10. -tp=.1 -go=halt
#   cgcns heatedBox -g=box2.order2    -tf=1. -tp=.05 -go=halt
# 
# --- set default values for parameters ---
$grid="square20.order2.hdf"; $show = " "; $method="jameson"; 
$tFinal=1.; $tPlot=.1; $cfl=.9; $debug=1; $backGround="square"; 
$mu=.01; $kThermal=.14; 
$gg=10.; $gravity = "0 $gg 0.";
$T0=10.; $Twall=$T0+1.; 
$go="halt"; 
# 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"debug=i"=>\$debug,"tp=f"=>\$tPlot,"show=s"=>\$show,"go=s"=>\$go,\
            "method=s"=>\$method );
# -------------------------------------------------------------------------------------------------
if( $method eq "godunov" ){ $pdeVariation="compressible Navier Stokes (Godunov)"; }
if( $method eq "jameson" ){ $pdeVariation="compressible Navier Stokes (Jameson)"; }   #
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# Here is the overlapping grid to use:
#
# Heated box in a gravitational field
#
#- $tFinal=10.; $tPlot=.1; $show=" "; $backGround="square"; 
#- $mu=.01; $kThermal=.14; 
#- $gg=10.; $gravity = "0 $gg 0.";
#- $T0=300.; $Twall=$T0+10.; 
# 
# $grid="square5.hdf"; $tPlot=.01;
# $grid="square20.hdf"; $tPlot=.5; $show="heatedBox.show"; $tFinal=5.;
# $grid="nonSquare20.hdf"; $tPlot=.1;
# $grid="rotatedSquare20.hdf"; $tPlot=.1; $gg=$gg/sqrt(2.); $gravity="-$gg $gg 0."; 
# $grid="square40.hdf"; $tPlot=.5;
# $grid="square40.hdf"; $mu=.01; $kThermal=.014; $tPlot=.5;
#
## $grid="box20.hdf"; $backGround="box"; $tPlot=.05; 
# 
$grid
#
  $pdeVariation
  #   compressible Navier Stokes (Godunov)  
  #  compressible Navier Stokes (Jameson)
  exit
  turn off twilight
#
#
  final time $tFinal 
  times to plot $tPlot
#*
  plot and always wait
 # no plotting
#**
  show file options
    compressed
      open
      $show
    frequency to flush
     4
    exit
#**
  reduce interpolation width
    2
# **************
  pde parameters
    mu
     $mu
    kThermal
     $kThermal
    gravity
      $gravity
  done
#
  boundary conditions
    # Set T on left and right wall
    all=slipWall
    # heat the bottom:
    bcNumber3=noSlipWall, uniform(T=$Twall)
    bcNumber4=noSlipWall, uniform(T=$T0)
    #
    # adiabatic walls: T.n =0
#+    bcNumber3=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#+    bcNumber4=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#+    bcNumber5=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#+    bcNumber6=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#-     # Set T on left and right wall
#-     $backGround(0,0)=noSlipWall, uniform(T=$Twall)
#-     $backGround(1,0)=noSlipWall, uniform(T=$T0)
#-     #
#-     # adiabatic walls: T.n =0
#-     $backGround(0,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#-     $backGround(1,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
##    $backGround(0,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
##    $backGround(1,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#
#    $backGround(0,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#    $backGround(1,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
# 
#   $backGround(0,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#   $backGround(1,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#    $backGround(0,1)=slipWall 
#    $backGround(1,1)=slipWall 
    done
#  debug
#    1
  initial conditions
    uniform flow
      r=1. u=0. T=$T0
  exit
  continue
plot:T




#
# Heated box in a gravitational field
#
$tFinal=10.; $tPlot=.1; $show=" "; $backGround="square"; 
$mu=.01; $kThermal=.14; 
# $gg=10.; $gravity = "0 $gg 0.";
$gg=0.; $gravity = "0 $gg 0.";
$T0=300.; $Twall=$T0+10.; 
# 
# $grid="square5.hdf"; $tPlot=.01;
## $grid="square20.hdf"; $tPlot=.5; $show="heatedBox.show"; $tFinal=5.;
# $grid="nonSquare20.hdf"; $tPlot=.1;
# $grid="rotatedSquare20.hdf"; $tPlot=.1; $gg=$gg/sqrt(2.); $gravity="-$gg $gg 0."; 
# $grid="square40.hdf"; $tPlot=.5;
# $grid="square40.hdf"; $mu=.01; $kThermal=.014; $tPlot=.5;
#
$grid="box20.hdf"; $backGround="box"; $tPlot=.05; 
# 
$grid
#
#   compressible Navier Stokes (Godunov)  
  compressible Navier Stokes (Jameson)
  exit
  turn off twilight
#
#
  final time $tFinal 
  times to plot $tPlot
#*
  plot and always wait
 # no plotting
#**
  show file options
    compressed
      open
      $show
    frequency to flush
     4
    exit
#**
  reduce interpolation width
    2
# **************
  pde parameters
    mu
     $mu
    kThermal
     $kThermal
    gravity
      $gravity
  done
#
  boundary conditions
    # Set T on left and right wall
    $backGround(0,0)=noSlipWall, uniform(T=$Twall)
    $backGround(1,0)=noSlipWall, uniform(T=$T0)
    #
    # adiabatic walls: T.n =0
    $backGround(0,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    $backGround(1,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    $backGround(0,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    $backGround(1,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#
#    $backGround(0,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#    $backGround(1,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
# 
#   $backGround(0,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#   $backGround(1,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#    $backGround(0,1)=slipWall 
#    $backGround(1,1)=slipWall 
    done
#  debug
#    1
  initial conditions
    uniform flow
      r=1. u=0. T=$T0
  exit
  continue
# 
  contour
    ghost lines 1
    exit




#
#  cgcns command file: heated box (or square)
# 
# Usage:
#    cgcns [-noplot] heatedBox.cmd -g=<grid> -tf=<final time> -tp=<tPlot> -show=<show file>  ...
#                                  -method=[jameson|godunov]
#
# Examples:
#   cgcns heatedBox -g=square20.order2 -tf=10. -tp=.1 -go=halt
#   cgcns heatedBox -g=box2.order2    -tf=1. -tp=.05 -go=halt
# 
# --- set default values for parameters ---
$grid="square20.order2.hdf"; $show = " "; $method="jameson"; 
$tFinal=1.; $tPlot=.1; $cfl=.9; $debug=1; $backGround="square"; 
$mu=.01; $kThermal=.14; 
$gg=10.; $gravity = "0 $gg 0.";
$T0=300.; $Twall=$T0+10.; 
$go="halt"; 
# 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"debug=i"=>\$debug,"tp=f"=>\$tPlot,"show=s"=>\$show,"go=s"=>\$go,\
            "method=s"=>\$method );
# -------------------------------------------------------------------------------------------------
if( $method eq "godunov" ){ $pdeVariation="compressible Navier Stokes (Godunov)"; }
if( $method eq "jameson" ){ $pdeVariation="compressible Navier Stokes (Jameson)"; }   #
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# Here is the overlapping grid to use:
#
# Heated box in a gravitational field
#
#- $tFinal=10.; $tPlot=.1; $show=" "; $backGround="square"; 
#- $mu=.01; $kThermal=.14; 
#- $gg=10.; $gravity = "0 $gg 0.";
#- $T0=300.; $Twall=$T0+10.; 
# 
# $grid="square5.hdf"; $tPlot=.01;
# $grid="square20.hdf"; $tPlot=.5; $show="heatedBox.show"; $tFinal=5.;
# $grid="nonSquare20.hdf"; $tPlot=.1;
# $grid="rotatedSquare20.hdf"; $tPlot=.1; $gg=$gg/sqrt(2.); $gravity="-$gg $gg 0."; 
# $grid="square40.hdf"; $tPlot=.5;
# $grid="square40.hdf"; $mu=.01; $kThermal=.014; $tPlot=.5;
#
## $grid="box20.hdf"; $backGround="box"; $tPlot=.05; 
# 
$grid
#
  $pdeVariation
  #   compressible Navier Stokes (Godunov)  
  #  compressible Navier Stokes (Jameson)
  exit
  turn off twilight
#
#
  final time $tFinal 
  times to plot $tPlot
#*
  plot and always wait
 # no plotting
#**
  show file options
    compressed
      open
      $show
    frequency to flush
     4
    exit
#**
  reduce interpolation width
    2
# **************
  pde parameters
    mu
     $mu
    kThermal
     $kThermal
    gravity
      $gravity
  done
#
  boundary conditions
    # Set T on left and right wall
    all=slipWall
    bcNumber1=noSlipWall, uniform(T=$Twall)
    bcNumber2=noSlipWall, uniform(T=$T0)
    #
    # adiabatic walls: T.n =0
#+    bcNumber3=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#+    bcNumber4=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#+    bcNumber5=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#+    bcNumber6=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#-     # Set T on left and right wall
#-     $backGround(0,0)=noSlipWall, uniform(T=$Twall)
#-     $backGround(1,0)=noSlipWall, uniform(T=$T0)
#-     #
#-     # adiabatic walls: T.n =0
#-     $backGround(0,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#-     $backGround(1,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
##    $backGround(0,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
##    $backGround(1,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#
#    $backGround(0,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#    $backGround(1,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
# 
#   $backGround(0,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#   $backGround(1,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#    $backGround(0,1)=slipWall 
#    $backGround(1,1)=slipWall 
    done
#  debug
#    1
  initial conditions
    uniform flow
      r=1. u=0. T=$T0
  exit
  continue
# 
  contour
    ghost lines 1
    exit




#
#  cgcns command file: heated box (or square)
# 
# Usage:
#    cgcns [-noplot] heatedBox.cmd -g=<grid> -tf=<final time> -tp=<tPlot> -show=<show file>  ...
#                                  -method=[jameson|godunov]
#
# Examples:
#   cgcns heatedBox -g=square20.order2 -tf=1. -tp=.05 -go=halt
#   cgcns heatedBox -g=box4.order2    -tf=1. -tp=.05 -go=halt
# 
# --- set default values for parameters ---
$grid="square20.order2.hdf"; $show = " "; $method="jameson"; 
$tFinal=1.; $tPlot=.1; $cfl=.9; $debug=1;
$mu=.01; $kThermal=.14; 
$gg=10.; $gravity = "0 $gg 0.";
$T0=300.; $Twall=$T0+10.; 
$go="halt"; 
# 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"debug=i"=>\$debug,"tp=f"=>\$tPlot,"show=s"=>\$show,"go=s"=>\$go,\
            "method=s"=>\$method );
# -------------------------------------------------------------------------------------------------
if( $method eq "godunov" ){ $pdeVariation="compressible Navier Stokes (Godunov)"; }
if( $method eq "jameson" ){ $pdeVariation="compressible Navier Stokes (Jameson)"; }   #
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# Here is the overlapping grid to use:
$grid
  # Choose Jameson or Godunov method:
  $pdeVariation
  #   compressible Navier Stokes (Godunov)  
  #  compressible Navier Stokes (Jameson)
  exit
  turn off twilight
#
  final time $tFinal 
  times to plot $tPlot
#
  show file options
    compressed
      open
      $show
    frequency to flush
     4
    exit
#
  reduce interpolation width
    2
# 
  pde parameters
    mu
     $mu
    kThermal
     $kThermal
    gravity
      $gravity
  done
#
  boundary conditions
    # Set T on left and right wall
    bcNumber1=noSlipWall, uniform(T=$Twall)
    bcNumber2=noSlipWall, uniform(T=$T0)
    #
    # adiabatic walls: T.n =0
    bcNumber3=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    bcNumber4=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    bcNumber5=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    bcNumber6=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#
    done
  debug
     $debug
  initial conditions
    uniform flow
      r=1. u=0. T=$T0
  exit
  continue
# 
$go



#
# Heated box in a gravitational field
#
$tFinal=10.; $tPlot=.1; $show=" "; $backGround="square"; 
$mu=.01; $kThermal=.14; 
$gg=10.; $gravity = "0 $gg 0.";
$T0=300.; $Twall=$T0+10.; 
# 
# $grid="square5.hdf"; $tPlot=.01;
## $grid="square20.hdf"; $tPlot=.5; $show="heatedBox.show"; $tFinal=5.;
# $grid="nonSquare20.hdf"; $tPlot=.1;
# $grid="rotatedSquare20.hdf"; $tPlot=.1; $gg=$gg/sqrt(2.); $gravity="-$gg $gg 0."; 
# $grid="square40.hdf"; $tPlot=.5;
# $grid="square40.hdf"; $mu=.01; $kThermal=.014; $tPlot=.5;
#
$grid="box20.hdf"; $backGround="box"; $tPlot=.05; 
# 
$grid
#
#   compressible Navier Stokes (Godunov)  
  compressible Navier Stokes (Jameson)
  exit
  turn off twilight
#
#
  final time $tFinal 
  times to plot $tPlot
#*
  plot and always wait
 # no plotting
#**
  show file options
    compressed
      open
      $show
    frequency to flush
     4
    exit
#**
  reduce interpolation width
    2
# **************
  pde parameters
    mu
     $mu
    kThermal
     $kThermal
    gravity
      $gravity
  done
#
  boundary conditions
    # Set T on left and right wall
    $backGround(0,0)=noSlipWall, uniform(T=$Twall)
    $backGround(1,0)=noSlipWall, uniform(T=$T0)
    #
    # adiabatic walls: T.n =0
    $backGround(0,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    $backGround(1,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    $backGround(0,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    $backGround(1,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#
#    $backGround(0,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#    $backGround(1,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
# 
#   $backGround(0,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#   $backGround(1,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#    $backGround(0,1)=slipWall 
#    $backGround(1,1)=slipWall 
    done
#  debug
#    1
  initial conditions
    uniform flow
      r=1. u=0. T=$T0
  exit
  continue
# 
  contour
    ghost lines 1
    exit


