#
#  cgcns command file: blast wave around obstacles.
#     Solve the Euler equations with Godunov's method and AMR
# 
# Usage:
#    cgcns [-noplot] blast.cmd -g=<grid> -amr=[0|1] -l=<levels> -r=<ratio> -tf=<final time> ...
#          -tp=<tPlot> -rad=<> -x0=<> -y0=<> -z0=<> -show=<show file> 
#
# Examples:
#   cgcns blast.cmd -g=shapese2.order2.hdf -rad=.15 -x0=.2 -y0=.5 -amr=0 -tf=1. -tp=.05 
#   cgcns blast.cmd -g=shapese2.order2.hdf -rad=.15 -x0=.2 -y0=.5 -amr=0 -tf=1. -tp=.05 -cnsVariation=jameson
#   cgcns blast.cmd -g=shapese4.order2.hdf -rad=.15 -x0=.2 -y0=.5 -amr=1 -l=2 -r=2 -tf=1. -tp=.05 
# 
#   cgcns blast.cmd -g=building3.hdf -rad=.25 -x0=.0 -y0=1.75 -z0=1. -amr=0 -l=2 -r=2 -tf=1. -tp=.05 -show="blast.show"
#
# --- set default values for parameters ---
$grid="shapes.hdf"; $show = " "; $backGround="square"; $cnsVariation="godunov"; 
$ratio=2;  $nrl=2;  # refinement ratio and number of refinement levels
$tFinal=1.; $tPlot=.1; $cfl=.9; $debug=1; $tol=.2;  $dtMax=1.e10; $nbz=2; 
$rad=.2; $x0=.0; $y0=0.; $z0=0.;
$xStep="x=-1.5"; $go="halt"; 
$amr=0; 
# 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"amr=i"=>\$amr,"l=i"=>\$nrl,"r=i"=>\$ratio,"tf=f"=>\$tFinal,"debug=i"=>\$debug, \
            "tp=f"=>\$tPlot, "xStep=s"=>\$xStep, "bg=s"=>\$backGround,"show=s"=>\$show,"go=s"=>\$go,\
            "cnsVariation=s"=>\$cnsVariation,"rad=f"=>\$rad,"x0=f"=>\$x0,"y0=f"=>\$y0,"z0=f"=>\$z0 );
# -------------------------------------------------------------------------------------------------
if( $amr eq "0" ){ $amr="turn off adaptive grids"; }else{ $amr="turn on adaptive grids"; }
if( $cnsVariation eq "godunov" ){ $pdeVariation="compressible Navier Stokes (Godunov)"; }
if( $cnsVariation eq "jameson" ){ $pdeVariation="compressible Navier Stokes (Jameson)"; }   #
if( $cnsVariation eq "nonconservative" ){ $pdeVariation="compressible Navier Stokes (non-conservative)";}  #
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# Here is the overlapping grid to use:
$grid 
#
$pdeVariation
  exit
  turn off twilight
#
#  do not use iterative implicit interpolation
#
  final time $tFinal
  times to plot $tPlot
# no plotting
  plot and always wait
#
  show file options
    compressed
 # specify the max number of parallel hdf sub-files: 
    OBPSF:maximum number of parallel sub-files 2
    open
     $show
    frequency to flush
      5
    exit
 # no plotting
  reduce interpolation width
    2
  boundary conditions
    # all=dirichletBoundaryCondition
    # * all=noSlipWall uniform(T=.3572)
    all=slipWall 
    # $backGround(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119,s=0)
    # $backGround(1,0)=superSonicOutflow
    # $backGround(0,1)=slipWall
    # $backGround(1,1)=slipWall
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
# 
  cfl $cfl
# 
  debug $debug
#***
#
  $amr
#*  turn off adaptive grids
#   save error function to the show file
#*  show amr error function 1
  order of AMR interpolation
      2
  error threshold
     $tol 
  regrid frequency
    $regrid=$nbz*$ratio;
    $regrid
  change error estimator parameters
    default number of smooths
      1
    set scale factors     
      2 1 1 1 1 
    done
    exit
  change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $nrl
    number of buffer zones
      $nbz
#    width of proper nesting 
#      1
    grid efficiency
      .7 .5 
  exit
#  Add a probe
  output options...
  frequency to save probes 2
  create a probe
    file name probeFile.dat
    nearest grid point to -1. 0. 0.
    exit
  close output options
#
    initial conditions options...
    OBIC:user defined...
      bubbles
      # number of bubbles
      # back-ground state
      # radius, center of the bubble
      # inner bubble state
      1
      r=1. e=1.786
       $rad $x0 $y0 $z0 
      r=2.6667 u=0. e=10.119 
      exit
continue
#
$go
