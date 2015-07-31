#
# cgcns: Heated cylinder in a gravitational field 
#       
#  cgcns [-noplot] heatedCyl -g=<name> -tf=<tFinal> -tp=<tPlot> -debug=<num> -model=[Jameson|Godunov]..,
#                            -mu=<num> -Prandtl=<num> -bg=<backGround> -show=<name> 
# 
# Examples:
#    cgcns heatedCyl -g="cice2.order2.hdf" -mu=.02 -tf=3. -tp=.5
#    cgcns heatedCyl -g="cice4.order2.hdf" -mu=.01 -tf=10. -tp=.1 
#    srun -N1 -n1 -ppdebug $cgcnsp heatedCyl -g="cice2.order2.hdf" -mu=.05 -tf=10. -tp=1. -ts=implicit -solver=best
#   
# --- set default values for parameters ---
# 
$grid="cic.hdf"; $noplot=""; $backGround="square"; $model="Jameson"; $go="halt"; 
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
    all=noSlipWall, uniform(T=$T0)
    Annulus(0,1)=noSlipWall, uniform(T=$Twall)
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



