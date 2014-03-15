#
#  cgsm: rotating disk example
#
# Usage: (not all options implemented yet)
#   
#  cgsm [-noplot] rotatingDisk -g=<name> -omega=<> -ra=<> -rb=<> -godunovType=[0|1|2] ...
#                       -tf=<tFinal> -tp=<tPlot> -diss=<> -dissOrder=<> -order=<2/4> -debug=<num> ...
#                       -bc=[d|sf|slip|dirichlet] -bg=<backGround> -pv=[nc|c|g|h] -godunovOrder=[1|2] -go=[run/halt/og]
# 
#  -godunovType : 0=linear, 2=SVK
#  -bc : boundary conditions: -bc=dirichlet, -bc=d : displacement, -bc=sf :stress-free
#  -ic : initial conditions, ic=gaussianPulse, ic=zero, ic=special
#  -diss : coeff of artificial diffusion 
#  -go : run, halt, og=open graphics
#  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
#  -en : error norm, "max", "l1" or "l2"
# 
# Examples:
# 
# -- godunov:
#   cgsm rotatingDisk -g=annulus20 -tp=.5 -tf=.5 -pv=g -godunovType=2 -omega=.5 -ra=.5 -rb=1. -bc=sf -cfl=.8 -relaxAlpha=0. -go=halt
#   cgsm rotatingDisk -g=sicFixede4.order2 -tp=.1 -tf=.5 -pv=g -godunovType=2 -omega=.5 -ra=.0 -rb=1. -bc=sf -cfl=.8 -relaxAlpha=0. -go=halt
# 
* --- set default values for parameters ---
*
$tFinal=10.; $tPlot=.05; $backGround="square"; $cfl=.5; $bc="sf"; $pv="nc";
$rate=.1; $dsf=1.; $xc=.5; $yc=.5; $specialOption="default"; 
$noplot=""; $grid="rectangle80.ar10"; $mu=1.; $lambda=1.; $godunovOrder=2;
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2; $bc="d"; $cons=1; $dsf=0.4; 
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx;
$ad=0.; # art. diss for Godunov
$order = 2; $go="halt"; $show=" "; $model="linear elasticity";
$en="max";
$omega=.5; $ra=.5; $rb=1.;
$stressRelaxation=2; $relaxAlpha=.5; $relaxDelta=0.; $godunovType=0;
$tangentialStressDissipation=.5;
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "dissOrder=i"=>\$dissOrder,"tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bc=s"=>\$bc,"ic=s"=>\$ic,"go=s"=>\$go,"noplot=s"=>\$noplot,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"xc=f"=>\$xc,"yc=f"=>\$yc,\
  "pv=s"=>\$pv,"godunovOrder=f"=>\$godunovOrder,"specialOption=s"=>\$specialOption,\
  "dsf=f"=>\$dsf,"filter=i"=>\$filter,"filterFrequency=i"=>\$filterFrequency,"filterOrder=i"=>\$filterOrder,\
  "en=s"=>\$en,"omega=f"=>\$omega,"ra=f"=>\$ra,"rb=f"=>\$rb,\
  "filterStages=i"=>\$filterStages,"ad=f"=>\$ad,"model=s"=>\$model,"godunovType=i"=>\$godunovType,\
  "stressRelaxation=f"=>\$stressRelaxation,"relaxAlpha=f"=>\$relaxAlpha,"relaxDelta=f"=>\$relaxDelta,\
  "tangentialStressDissipation=f"=>\$tangentialStressDissipation,"rate=f"=>\$rate,"dsf=f"=>\$dsf  );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }else{ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $pv eq "nc" ){ $pv = "non-conservative"; $cons=0; }
if( $pv eq "c" ){ $pv = "conservative"; $cons=1; }
if( $pv eq "g" ){ $pv = "godunov"; }
if( $pv eq "h" ){ $pv = "hemp"; }
#if( $en eq "max" ){ $errorNorm="maximum norm"; }
#if( $en eq "l1" ){ $errorNorm="l1 norm"; }
#if( $en eq "l2" ){ $errorNorm="l2 norm"; }
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
* -new: set-up stage:
$model
# linear elasticity
$pv
 continue
# 
# -- set the time-stepping method:
# $ts
# 
# $errorNorm
# -----------------------------
close forcing options
# 
final time $tFinal
times to plot $tPlot
# 
SMPDE:lambda $lambda
SMPDE:mu $mu
SMPDE:rho $rho
SMPDE:Godunov order of accuracy $godunovOrder
SMPDE:PDE type for Godunov $godunovType
SMPDE:slope limiting for Godunov 0
SMPDE:slope upwinding for Godunov 0
SMPDE:stressRelaxation $stressRelaxation
SMPDE:relaxAlpha $relaxAlpha
SMPDE:relaxDelta $relaxDelta
SMPDE:tangential stress dissipation $tangentialStressDissipation
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
# --- Choose the rotating disk known solution:
#
OBTZ:user defined known solution
  rotating disk
  #  n   omega ra rb 
  #  4001 $omega $ra $rb
  2001 $omega $ra $rb
  done
initial conditions options...
  knownSolutionInitialCondition
#
$checkErrors=1;
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
$go
