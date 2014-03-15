#
#  cgsm: elastic vibrations for a plate with holes
#
# Usage: (not all options implemented yet)
#   
#  cgsm [-noplot] pulse -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -dissOrder=<> -order=<2/4> -debug=<num> ...
#                       -bc=[d|sf|slip] -bg=<backGround> -pv=[nc|c|g|h] -godunovOrder=[1|2] -go=[run/halt/og]
# 
#  -bc : boundary conditions: -bc=d : displacement, -bc=sf :stress-free, 
#        -bc=ellipseDeform : specified motion of the boundary
#  -ic : initial conditions, ic=gaussianPulse, ic=zero, ic=special
#  -diss : coeff of artificial diffusion 
#  -go : run, halt, og=open graphics
#  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
#  -option : 3D or 2D 
# 
# Examples:
#     cgsm plateWithHoles -g=plateWith12Holese2.order2 -pv=c -tp=.1 -tf=10. -x0=.0 -y0=.0 -option=2D
#     cgsm plateWithHoles -g=plateWith24Holese4.order2 -pv=c -tp=.1 -tf=10. -x0=.0 -y0=.0 -option=2D
#
#  -- 3D
#     cgsm plateWithHoles -g=plate3dWith24Holese2.order2 -pv=c -tp=.1 -tf=10. -x0=.0 -y0=.0 
#     cgsm plateWithHoles -g=plate3dWith24Holese2.order2 -pv=g -tp=.1 -tf=10. -x0=.0 -y0=.0 
# 
# --- set default values for parameters ---
# 
$tFinal=10.; $tPlot=.05; $backGround="backGround"; $cfl=.9; $bc="sf"; $pv="nc";
$ic="gaussianPulse";  $exponent=10.; $x0=.0; $y0=.0; $z0=.0; $specialOption="default"; 
$ic="zero";
$noplot=""; $grid="rectangle80.ar10"; $mu=1.; $lambda=1.; $godunovOrder=2; $iw=3; 
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2; $cons=1; $dsf=1.; 
$filter=1; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx;
$order = 2; $go="halt"; $show=" "; $flushFrequency=10; 
$ad=0.; $ad4=0.;  # art. diss for Godunov
$option="3D"; 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "dissOrder=i"=>\$dissOrder,"tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bc=s"=>\$bc,"ic=s"=>\$ic,"go=s"=>\$go,"noplot=s"=>\$noplot,"iw=i"=>\$iw,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"x0=f"=>\$x0,"y0=f"=>\$y0,"z0=f"=>\$z0,\
  "pv=s"=>\$pv,"exponent=f"=>\$exponent,"godunovOrder=f"=>\$godunovOrder,"specialOption=s"=>\$specialOption,\
  "dsf=f"=>\$dsf,"filter=i"=>\$filter,"filterFrequency=i"=>\$filterFrequency,"filterOrder=i"=>\$filterOrder,\
  "filterStages=i"=>\$filterStages,"flushFrequency=i"=>\$flushFrequency,"ad=f"=>\$ad,"ad4=f"=>\$ad4,"option=s"=>\$option );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }else{ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $pv eq "nc" ){ $pv = "non-conservative"; $cons=0; }
if( $pv eq "c" ){ $pv = "conservative"; $cons=1; }
if( $pv eq "g" ){ $pv = "godunov"; }
if( $pv eq "h" ){ $pv = "hemp"; }
# 
if( $bc eq "d" ){ $bc = "all=displacementBC"; }
if( $bc eq "sf" ){ $bc = "all=tractionBC"; }
if( $bc eq "m" ){ $bc = "all=displacementBC\n Annulus(0,1)=tractionBC"; }
if( $bc eq "slip" ){ $bc = "all=slipWall"; }
if( $bc eq "ellipseDeform" ){ $bc = "all=displacementBC , userDefinedBoundaryData\n ellipse deform\n .25 1.\n done"; }
if( $ic eq "gaussianPulse" ){ $ic="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 \n"; }
if( $ic eq "zero" ){ $ic = "zeroInitialCondition"; }; 
if( $ic eq "special" ){ $ic = "specialInitialCondition"; $dsf=.1; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
# 
# $tFinal=10.; $tPlot=.05; $backGround="rectangle"; 
# $diss=0.; $cfl=.9;
# 
# $grid = "rectangle80.ar10"; $diss=10.; $tPlot=.2; $cfl=.5; 
#
# 
# Note: artificial dissipation is scaled by c^2
#
$grid
# 
# -new: set-up stage: 
linear elasticity
$pv
 continue
# 
modifiedEquationTimeStepping
# ----- trig IC's ----
# twilightZoneInitialCondition
# trigonometric
# TZ omega: 2 2 2 2 (fx,fy,fz,ft)
# -----------------------------
close forcing options
# 
# 
apply filter $filter
if( $filter eq 1 ){ $cmds = "filter order $filterOrder\n filter frequency $filterFrequency\n filter iterations 1\n filter coefficient 1. \n  filter stages $filterStages\n explicit filter\n  exit"; }else{ $cmds = "#"; }
$cmds
# 
final time $tFinal
times to plot $tPlot
# 
SMPDE:lambda $lambda
SMPDE:mu $mu 
SMPDE:Godunov order of accuracy $godunovOrder
#
# -- reduce interpolation width for godunov --
if( $pv eq "godunov" && $iw eq 2 ){ $cmds = "reduce interpolation width\n $iw"; }else{ $cmds="#"; }
$cmds
#
boundary conditions
  $bc
# Here is a boundary forcing on the centre top: (see userDefinedBoundaryValues.C)
# amp, alpha x0 y0 z0 t0
if( $option eq "3D" ){ $cmd = "$backGround(1,2)=tractionBC, userDefinedBoundaryData\n Gaussian forcing\n -1. 20. 0. .0 0.25 1. 3.\n done"; }else{ $cmd="#";} 
$cmd
 # --- 
done  
#
displacement scale factor $dsf
dissipation $diss
order of dissipation $dissOrder
cfl $cfl
use conservative difference $cons
SMPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad
SMPDE:fourth-order artificial diffusion $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 
# 
plot divergence 1
plot vorticity 1
initial conditions options...
Special initial condition option: $specialOption
$ic 
if( $option eq "2D" ){ $cmd="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0)"; }else{ $cmd="#";}
$cmd
close initial conditions options
#
$checkErrors=0;
if( $ic eq "specialInitialCondition" ){ $checkErrors=1; }
check errors $checkErrors
plot errors $checkErrors
#
debug $debug
#
# For displacement solvers plot velocity and stress: 
if( $pv eq "non-conservative" || $pv eq "conservative" ){ $plotCommands = "plot velocity 0\n plot stress 0"; }else{ $plotCommands="*"; }
$plotCommands
#*********************************
show file options...
  OBPSF:compressed
  * specify the max number of parallel hdf sub-files: 
  OBPSF:maximum number of parallel sub-files 8
  OBPSF:open
    $show
 # OBPSF:frequency to save 
  OBPSF:frequency to flush $flushFrequency
exit
#**********************************
continue
# 
erase
displacement
exit
$go
