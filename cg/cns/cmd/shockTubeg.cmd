#
# cgcns: command file for a shock-tube
#   cgcns [-noplot] shockTubeg.cmd -g=<gridName> -amr=[0|1] -r=[2|4] -l=[1|2|3...] -tf=<f> -tp=<f> ...
#              -bf=[none|gaussian]
#
#  -l= number of refinement levels
#  -r= refinement ratio
#  -bf= body forcing  
#
# Examples:
#    cgcns shockTubeg.cmd -g=square40.order2 -bg=square -l=3 -r=2 -tf=2. -tp=.02
#
#  -- test body forcing:
#    cgcns shockTubeg.cmd -g=square40.order2 -bg=square -l=3 -r=2 -tf=2. -tp=.02 -bf=1
#
# -- defaults:
$tFinal=1.; $tPlot=.05; $xStep="x=.25"; $show=" "; 
$cnsVariation="godunov"; 
$grid="square40.order2.hdf";
$show = " ";  $format="%18.12e";  $debug=0; 
$nrl=2;  $ratio=2; $errTol=.1;  $nbz=2; $numberOfSmooths=1; 
$amr=1;
$x0=.25; $y0=.5; $z0=0.; $amp=10.; $beta=200.; # for body forcing
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"amr=i"=>\$amr,"l=i"=>\$nrl,"r=i"=>\$ratio,"tf=f"=>\$tFinal,"debug=i"=>\$debug, \
            "tp=f"=>\$tPlot, "xStep=s"=>\$xStep, "show=s"=>\$show,"bf=s"=>\$bf,"go=s"=>\$go,\
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
# --- OLD commands: ---
# $grid="channelShort"; $debug=3; $numberOfLevels=3;
# $grid="channelShort"; $debug=1; $numberOfLevels=2; $tFinal=.2; $tPlot=.01; $format="%21.15e"; $regrid=2; $show="shockTube.show"; $xStep="x=.3";
# $grid="channelShort1"; $debug=3; $numberOfLevels=2; $tFinal=.01; $tPlot=.01; $format="%10.4e"; $regrid=2; $xStep="x=.3"; $numberOfSmooths=1; 
# $grid="channelShort"; $debug=3; $numberOfLevels=2; $tFinal=.2; $amr=$amrOff; $format="%20.14e";
#
# $grid="square10"; $debug=3; $numberOfLevels=2; $tFinal=.3; $tPlot=.01; $format="%21.15e"; $regrid=2; $backGround="square"; $xStep="x=.5"; $show="shockTube.show"; 
# $grid="square20"; $debug=3; $numberOfLevels=2; $tFinal=.3; $tPlot=.1; $format="%21.15e"; $regrid=2; $backGround="square"; $xStep="x=-.5";
# $grid="sise"; $debug=3; $numberOfLevels=2; $tFinal=.3; $tPlot=.1; $format="%21.15e"; $regrid=2; $backGround="outer-square"; $xStep="x=-.5";
# 
# channelFine
#  channel.hdf
#   channelShort
# 
  $grid
  $pdeVariation
  exit
# 
  turn off twilight
  final time $tFinal
  times to plot $tPlot
  plot and always wait
 # no plotting
  show file options
    compressed
    open
      $show 
    frequency to flush
      2
    exit
#
 reduce interpolation width
   2
#
#
  pde parameters
    mu
     0.
    kThermal
     0.
#    conservative Godunov
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
#
#  Optionally add a user defined forcing:
# forcing options...
$component=0; #  forcing is added to the equations in conservative variables
if( $bf eq 1 ){ $cmd="user defined forcing\n gaussian forcing\n 1\n  0, $component, $amp,$beta,2,  $x0,$y0,$z0\n done\n exit"; }else{ $cmd="#"; }
  $cmd
#
  output format $format
#
  boundary conditions
    all=slipWall
    bcNumber1=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
    bcNumber2=superSonicOutflow
    done
# 
 $amr
#
 turn on user defined error estimator
#
  order of AMR interpolation
      2
  error threshold
     $errTol
  regrid frequency
    $regrid=$nbz*$ratio;
    $regrid
  change error estimator parameters
    default number of smooths
      $numberOfSmooths
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
  exit
#
#  allow user defined output 1
#
  initial conditions
 # x=.5
    step function
      $xStep 
      r=2.6667 u=1.25 e=10.119
      r=1. e=1.786
    continue
#**************
  debug
    $debug
#**************
   continue
# 

movie mode

finish


  contour
   wire frame (toggle)
  exit this menu





  change the grid
    add a refinement
    rectangle
    set bounds
      .05 .45 0. 1.
    done
    done
   erase and exit
   erase and exit




