#
# airfoil.cmd : cgcns command file for flow past an airoil
#
# Usage:
#   cgcns [-noplot] airfoil -g=<grid name> -Mach=<machNumber> -tp=<> -tf=<> -show=<name> ...
#          -cnsVariation=[goudnov|jameson] -restart=<restart-file> ...
#          -amr=[0|1] -tol=<>
#
#  Inviscid or viscous flow past an airfoil
#   o Build the grids using ogen and Overture/sampleGrids/airfoil.cmd
#   o For viscous computations remember to set the Temperature on the noSlipWall
#
# Examples:
#      - NOTE: Use sampleGrids/airfoil.cmd to make airfoile1.order2
#
#   cgcns airfoil -g=airfoile1.order2 -tf=1. -amr=0 -show=airfoil1.show -go=halt
# - AMR: 
#   cgcns airfoil -g=airfoile1.order2 -tf=1. -amr=1 -show=airfoil1a.show -go=halt
#
#  - restart AMR:
#   cgcns airfoil -g=airfoile1.order2 -tf=1. -amr=1 -show=airfoil1b.show -restart=airfoil1a.show -go=halt
#  - restart on a finer grid: (NOTE: set useGridFromShowfile=0 so we don't use the grid in the show file)
#   cgcns airfoil -g=airfoile2.order2 -tf=2. -amr=0 -show=airfoil2.show -restart=airfoil1.show -useGridFromShowfile=0 -go=halt
#  - restart on a finer grid and turn on amr:
#   cgcns airfoil -g=airfoile2.order2 -tf=2. -amr=1 -show=airfoil2.show -restart=airfoil1.show -useGridFromShowfile=0 -tol=.05 -go=halt
#
# - parallel
#  mpirun -np 2 $cgcnsp airfoil -g=airfoile2.order2 -tf=1. -amr=0 -show=airfoil1.show -go=halt
#  mpirun -np 2 $cgcnsp airfoil -g=airfoile2.order2 -tf=2. -amr=0 -show=airfoil2.show -restart=airfoil1.show -go=halt
# 
$cfl=.95; $tPlot=.1; $tFinal=1.; $show=" "; $debug=0; $mu=0.; $restart=""; $go="halt"; 
$gamma=1.4;  $Mach=.8; # M = Mach number of incoming flow
$cnsVariation="godunov"; 
$slip="all=slipWall"; $noSlip="all=noSlipWall uniform(T=$T1)"; $bc=$slip;
$backGround="backGround";
$useGridFromShowfile=1; 
# 
#   -- AMR parameters --
$amr=0; $tol=.1; $ratio=2; $nrl=2; $nbz=1; $regrid=100; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"l=i"=> \$nrl,"r=i"=> \$ratio, "tf=f"=>\$tFinal,"debug=i"=> \$debug, \
            "tp=f"=>\$tPlot, "xStep=s"=>\$xStep, "bg=s"=>\$backGround,"show=s"=>\$show,"go=s"=>\$go,\
            "cnsVariation=s"=>\$cnsVariation,"Mach=f"=>\$Mach,"restart=s"=>\$restart,"amr=i"=>\$amr,\
            "tol=f"=>\$tol,"useGridFromShowfile=i"=>\$useGridFromShowfile );
# -------------------------------------------------------------------------------------------------
# ---------------------------------------------------
# Define the inflow state for a given Mach number:
#  M = Mach number of incoming flow
$r1=$gamma; $u1=$Mach; $p1=1.; $T1=$p1/$r1; $a1=1.;
# ---------------------------------------------------
if( $cnsVariation eq "godunov" ){ $pdeVariation="compressible Navier Stokes (Godunov)"; }
if( $cnsVariation eq "jameson" ){ $pdeVariation="compressible Navier Stokes (Jameson)"; }   #
if( $cnsVariation eq "nonconservative" ){ $pdeVariation="compressible Navier Stokes (non-conservative)";}  #
if( $amr eq 1 ){ $amr="turn on adaptive grids"; }else{ $amr="turn off adaptive grids"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# Here is the overlapping grid to use:
$grid 
# 
# +++OLD WAY:
# --------- Here are different cases: ---------------
#    -- inviscid : use godunov or jameson --
#* $grid="airfoil1.hdf";  $tPlot=.1; $tFinal=5.; $show="airfoil1.show"; 
# $grid="airfoil2.hdf";  $tPlot=1.; $tFinal=10.; $show="airfoil2.show"; 
# $grid="airfoilWithCamber1.hdf";  $tPlot=.5; $tFinal=5.; $show="airfoil1.show"; 
# restart with AMR turned on:
# $grid="airfoil2.hdf";  $tPlot=.25; $nrl=2; $ratio=4; $tFinal=12.; $amr=$amrOn; $show="airfoil2r.show"; $restart="airfoil2.show";
# -- viscous computations : use Jameson solver ---
# $grid="cice.hdf";  $debug=0; $tPlot=.1; $tFinal=5.; $bc=$noSlip; $solverType=$jameson; $mu=1.e-2; $backGround="square";
# $grid="airfoil1.hdf";  $tPlot=1.; $tFinal=10.; $bc=$noSlip; $solverType=$jameson; $mu=1.e-4; $show="airfoil1j.show";
# $grid="airfoil2.hdf";  $tPlot=1.; $tFinal=10.; $bc=$noSlip; $solverType=$jameson; $mu=1.e-5; $show="airfoil2j.show"; 
#
#  --------------------------------------------------
# 
  $pdeVariation
#
  exit
  turn off twilight
#
#  do not use iterative implicit interpolation
#
  final time (tf=)
    $tFinal
  times to plot (tp=)
    $tPlot
# no plotting
  plot and always wait
  show file options
    compressed
    open
     $show
    frequency to flush 1
    frequency to save sequences 100
    exit
# 
 # no plotting
  reduce interpolation width
    2
  boundary conditions
    $bc
    $backGround(0,0)=superSonicInflow uniform(r=$r1,u=$u1,T=$T1)
    $backGround(1,0)=superSonicOutflow
    $backGround(0,1)=slipWall
    $backGround(1,1)=slipWall
    done
#
  pde parameters
    mu
      $mu
    kThermal
      $prandtl=.72; $kThermal=$mu/$prandtl;
      $kThermal
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
  cfl $cfl
  debug
   $debug
#
#
  $amr
# 
  order of AMR interpolation
      2
  error threshold
     $tol 
  regrid frequency
 #  $regrid=$nbz*$ratio;
     $regrid
  change error estimator parameters
    set scale factors
      1 1 1 1 1 1 1 
    done
    weight for first difference
    1.
    weight for second difference
    1.
    exit
    truncation error coefficient
    1.
#   show amr error function
  change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $nrl
    number of buffer zones
      $nbz
    grid efficiency
      .7
  exit
#
  if( $restart eq "" ){ $cmds = "uniform flow\n" . " r=$r1 u=$u1 T=$T1\n"; }\
  else{ $cmds = "OBIC:show file name $restart\n use grid from show file $useGridFromShowfile\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
#
  initial conditions
    $cmds
  exit
  continue
#
$go


