#==============================================================
# cgcns example: flow past a wind turbine and tower
#
# Usage:
#       cgcns turbineAndTower -g=<grid-name> -amr=[0|1]  -moveOnly=[0|1|2]  -memoryCheck=[0|1] -freqFullUpdate=[0|1]
#
# -moveOnly : 
#   0 = solve and move grids
#   1 = move and regenerate grids only
#   2 = move grids only
#
# Examples: NOTE: use grid with less stretching 
#     cgcns turbineAndTower -g=turbineAndTower1Towerse2.order2.s2 -tp=.01 -tf=5. -moveOnly=1 [Ok
#  -- non-moving: 
#     cgcns turbineAndTower -g=turbineAndTower1Towerse2.order2.s1 -tp=.01 -tf=5. -move=0 
#     mpirun -np 4 $cgcnsp turbineAndTower -g=turbineAndTower1Towerse2.order2.s2 -tp=.01 -tf=5. -move=0 
#     mpirun -np 4 $cgcnsp turbineAndTower -g=turbineAndTower1Towerse2.order2.ml1 -tp=.01 -tf=5. -move=0 
#   
#    
#=============================================================
#
# --- set default values for parameters ---
# 
$cnsVariation="godunov"; $ts="pc"; $show=" ";
$grid="turbineAndTower1Towerse2.order2.ml1"; $backGround="backGround"; $bcn="slipWall";  $vIn=1.;
$mg="square"; $mt="shift"; $vg0=0.; $vg1=0.; $vg2=0.; $move=1; $moveOnly=0;
$tFinal=10.; $tPlot=.1; $cfl=.9; $mu=.0; $Prandtl=.72; $thermalExpansivity=.1; 
$noplot=""; $debug = 0;  
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5;  $freq=.1; 
$order = 2; $fullSystem=0; $go="halt"; 
$ad2=0; $ad22=2.; $interpWidth=2; $memoryCheck=0;
$amr=0;
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$pi=4.*atan2(1.,1.);
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "cnsVariation=s"=>\$cnsVariation,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug,"memoryCheck=i"=>\$memoryCheck, \
 "nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "go=s"=>\$go,"freqFullUpdate=i"=>\$freqFullUpdate,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"bcn=s"=>\$bcn,"vg0=f"=>\$vg0,"vg1=f"=>\$vg1,"vg2=f"=>\$vg2,\
 "dtMax=f"=>\$dtMax,"bc=s"=>\$bc,"mg=s"=>\$mg,"mt=s"=>\$mt,"ts=s"=>\$ts,"amr=i"=>\$amr,\
 "interpWidth=i"=>\$interpWidth,"freq=f"=>\$freq,"moveOnly=i"=>\$moveOnly,"vIn=f"=>\$vIn,"move=i"=>\$move );
# -------------------------------------------------------------------------------------------------
$kThermal=$mu/$Prandtl;   # check this 
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $amr eq "0" ){ $amr="turn off adaptive grids"; }else{ $amr="turn on adaptive grids"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $cnsVariation eq "godunov" ){ $cnsVariation="compressible Navier Stokes (Godunov)"; $ts="fe"; }
if( $cnsVariation eq "jameson" ){ $cnsVariation="compressible Navier Stokes (Jameson)"; }  
if( $cnsVariation eq "nonconservative" ){ $cnsVariation="compressible Navier Stokes (non-conservative)"; } 
if( $ts eq "fe" ){ $ts="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $moveOnly eq 0 ){ $moveOnly="solve and move grids"; }elsif( $moveOnly eq 1 ){ $moveOnly="move and regenerate grids only"; }else{ $moveOnly="move grids only"; }
# 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# specify the overlapping grid to use:
$grid
  $cnsVariation
  $moveOnly
  done
if( $memoryCheck ne 0 ){ $cmd="turn on memory checking"; }else{ $cmd="#"; }
$cmd
# -- time stepping method:
  $ts
# 
  turn off twilight
#
#  do not use iterative implicit interpolation
#
  final time $tFinal
  times to plot $tPlot
  dtMax $dtMax
# 
 no plotting
 plot and always wait
#
  show file options
    compressed
     open
       $show
    frequency to flush
      2
    exit
 # no plotting
#****************************
 # There can be trouble if the grid moves too fast
 if( $move eq 1 ){ $cmd="turn on moving grids"; }else{ $cmd="#"; }
 $cmd
# 
  frequency for full grid gen update $freqFullUpdate
#**********
  specify grids to move
    matrix motion
        rotate around a line
        tangent to line: 0 1 0
        # point on line: 0 0 4.
        # The blade is .25 below the top of the tower:
        point on line: 0 0 3.75
        edit time function
          linear function
          $a1 = $freq*2.*$pi; 
          linear parameters: 0,$a1 (a0,a1)
          exit
        exit
#   -- choose which grids to move by the share value 
    choose grids by share flag
      7
   done
# 
  done
#**************
#**************************
if( $interpWidth eq 2 ){ $cmds = "reduce interpolation width\n 2"; }else{ $cmds ="#"; }
$cmds
#****
*****  Here we optionally turn on AMR *******
  $amr
  order of AMR interpolation
      2
  error threshold
      .0005
  regrid frequency
     8 4  8
  change error estimator parameters
    set scale factors
      1 10000 10000 10000 10000
    done
    weight for first difference
    0.
    weight for second difference
    .03
    exit
    truncation error coefficient
    1.
    show amr error function
  change adaptive grid parameters
    refinement ratio
      2 
    default number of refinement levels
      2 
    number of buffer zones
      2
    grid efficiency
      .7
  exit
*****
  boundary conditions
    all=slipWall 
    backGround(0,1)=superSonicInflow uniform(r=1.,v=$vIn,e=1.786,s=0)
    backGround(1,1)=outflow
#
    done
#
  pde parameters
    mu
     0.0
    kThermal
     0.0
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
#**************
  cfl
   $cfl
#*****************
 debug
   $debug
#
  initial conditions
#*****************************************************
 uniform flow
    r=1. v=$vIn e=1.786 s=0.
#*****************************************************
#  OBIC:read from a show file
#    turbineAndTower.show
#    -1
#*****************************************************
  continue
continue
#
  x-r 90
  set home
  grid
   toggle grid 0 0
  exit this menu
#
$go 

