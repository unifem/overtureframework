#
#  cgsm: large translation-rotation solutions
#
# Usage: (not all options implemented yet)
#   
#  cgsm [-noplot] translationRotation -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -dissOrder=<> -order=<2/4> -debug=<num> ...
#                       -bc=[d|sf|slip|dirichlet] -bg=<backGround> -pv=[nc|c|g|h] -godunovOrder=[1|2] -go=[run/halt/og]
# 
#  -bc : boundary conditions: -bc=dirichlet, -bc=d : displacement, -bc=sf :stress-free
#  -ic : initial conditions, ic=gaussianPulse, ic=zero, ic=special
#  -diss : coeff of artificial diffusion 
#  -go : run, halt, og=open graphics
#  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
#  -en : error norm, "max", "l1" or "l2"
# 
# Examples:
#    - rotation: 
#   cgsm translationRotation -g=square20 -cons=0 -diss=1. -tp=.05 -tf=10. -omega=1 -x0=.5 -x1=.5 -bc=dirichlet
#    - translation:
#   cgsm translationRotation -g=square20 -cons=0 -diss=1. -tp=.05 -tf=10. -omega=0 -v0=1 -bc=dirichlet
# 
# -- godunov:
#   cgsm translationRotation -g=square20 -tp=.05 -tf=.1 -pv=g  -omega=1 -x0=.0 -x1=.0 -bc=dirichlet
# 
# --- set default values for parameters ---
# 
$tFinal=10.; $tPlot=.05; $backGround="square"; $cfl=.9; $bc="dirichlet"; $pv="nc"; $ts="me"; 
$ic="special";  $exponent=10.; $x0=.5; $y0=.5; $z0=.5; $specialOption="translationAndRotation"; 
$noplot=""; $grid="rectangle80.ar10"; $mu=1.; $lambda=1.; $godunovOrder=2;
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2; $bc="sf"; $cons=1; $dsf=0.4;
$omega=1.; $x0=0.; $x1=0.; $x2=0.; $v0=0.; $v1=0.; $v2=0.;
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx;
$order = 2; $go="run"; 
$en="max";
# Hemp parameters: (turn off Q:)
$Rg=8.314/27.; $yield=1.e10; $basePress=0.0; $c0=0.0; $cl=0.0; $hgFlag=0; $hgVisc=4.e-2;
$apr=0.0; $bpr=0.0; $cpr=0.0; $dpr=0.4;
# 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "dissOrder=i"=>\$dissOrder,"tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bc=s"=>\$bc,"ic=s"=>\$ic,"go=s"=>\$go,"noplot=s"=>\$noplot,"ts=s"=>\$ts,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"dsf=f"=>\$dsf,\
  "pv=s"=>\$pv,"exponent=f"=>\$exponent,"godunovOrder=f"=>\$godunovOrder,"specialOption=s"=>\$specialOption,\
  "en=s"=>\$en,"omega=f"=>\$omega,"x0=f"=>\$x0,"x1=f"=>\$x1,"x2=f"=>\$x2,"v0=f"=>\$v0,"v1=f"=>\$v1,"v2=f"=>\$v2 );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }else{ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $pv eq "nc" ){ $pv = "non-conservative"; $cons=0; }
if( $pv eq "c" ){ $pv = "conservative"; $cons=1; }
if( $pv eq "g" ){ $pv = "godunov"; }
if( $pv eq "h" ){ $pv = "hemp"; }
if( $en eq "max" ){ $errorNorm="maximum norm"; }
if( $en eq "l1" ){ $errorNorm="l1 norm"; }
if( $en eq "l2" ){ $errorNorm="l2 norm"; }
#
if( $ts eq "me" ){ $ts = "modifiedEquationTimeStepping"; }
if( $ts eq "fe" ){ $ts = "forwardEuler"; }
if( $ts eq "ie" ){ $ts = "improvedEuler"; }
if( $ts eq "ab" ){ $ts = "adamsBashforth2"; }
# 
if( $bc eq "d" ){ $bc = "all=displacementBC"; }
if( $bc eq "sf" ){ $bc = "all=tractionBC"; }
if( $bc eq "slip" ){ $bc = "all=slipWall"; }
if( $bc eq "dirichlet" ){ $bc = "all=dirichletBoundaryCondition"; }
if( $ic eq "zero" ){ $ic = "zeroInitialCondition"; }; 
if( $ic eq "special" ){ $ic = "specialInitialCondition"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
# 
#
$grid
# 
# set-up stage: 
linear elasticity
$pv
 continue
# 
# -- set the time-stepping method:
$ts
# 
$errorNorm
# -----------------------------
close forcing options
# 
final time $tFinal
times to plot $tPlot
# 
SMPDE:lambda $lambda
SMPDE:mu $mu 
SMPDE:Godunov order of accuracy $godunovOrder
# -new- 
SMPDE:slope limiting for Godunov 0
SMPDE:slope upwinding for Godunov 0
# 0=linear-elasticity, 2=SVK
SMPDE:PDE type for Godunov 2
#
boundary conditions
  $bc
done  
#
debug $debug
#
displacement scale factor $dsf
dissipation $diss
order of dissipation $dissOrder
cfl $cfl
use conservative difference $cons
# 
plot divergence 1
plot vorticity 1
#
# --- here is where we set the parameters for the translation rotation example ---
#
initial conditions options...
specialInitialCondition
Special initial condition option: $specialOption
$omega $x0 $x1 $x2 $v0 $v1 $v2
# 
$ic 
close initial conditions options
# forcing:
 userDefinedForcing
   translation and rotation forcing
   $omega $x0 $x1 $x2 $v0 $v1 $v2
 exit
#
$checkErrors=0;
if( $ic eq "specialInitialCondition" ){ $checkErrors=1; }
check errors $checkErrors
plot errors $checkErrors
#
displacement scale factor 1
# 
# For displacement solvers plot velocity and stress: 
if( $pv eq "non-conservative" || $pv eq "conservative" ){ $plotCommands = "plot velocity 1\n plot stress 1"; }else{ $plotCommands="*"; }
$plotCommands
#*********************************
show file options...
  OBPSF:compressed
  OBPSF:open
    $show
 # OBPSF:frequency to save 
  OBPSF:frequency to flush 50
exit
#**********************************
continue
# 
  contour
    ghost lines 1
    plot:u-error
    adjust grid for displacement 1
    exit


erase
displacement
exit
