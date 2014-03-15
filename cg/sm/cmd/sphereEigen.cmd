#
# cgsm -- compute eigenfunctions of a sphere
#
# Usage:
#   
#  cgsm [-noplot] sphereEigen -g=<name> -vClass=[1\2] -nMode=[1|2|...] -mMode=[0|1|...] -tf=<tFinal> -tp=<tPlot> ...
#                    -bcn=[d|sf|mixed] -diss=<> -order=<2/4> -debug=<num> -bg=<backGround> -cons=[0/1] ...
#                    -pv=[nc|c|g|h] -godunovOrder=[1|2] -mu=<> -lambda=<> -rho=<> -go=[run/halt/og]
# 
#  -vClass : 1 or 2 specifies the class of vibration (see docs)
#  -nMode, -mNode : define the vibration mode. 
# 
#  -diss : coeff of artificial diffusion 
#  -bcn : d=dirichlet, sf=stress-free
#  -go : run, halt, og=open graphics
#  -cons : 1= conservative difference 
#  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
# 
# Examples:
# 
#  -- vibration-class =1 
#  cgsm sphereEigen -g=spheree1.order2 -diss=0.5 -tp=.05 -vClass=1 -nMode=1 -mMode=1 -go=halt -dsf=.5
#  cgsm sphereEigen -g=spheree2.order2 -diss=0.5 -tp=.05 -vClass=1 -nMode=1 -mMode=1 -go=halt -dsf=.5
#  cgsm sphereEigen -g=spheree2.order2 -diss=0.5 -tp=.05 -vClass=1 -nMode=1 -mMode=1 -go=halt -dsf=.5 -pv=c
#  cgsm sphereEigen -g=spheree4.order2 -diss=0.5 -tp=.05 -vClass=1 -nMode=1 -mMode=1 -go=halt -dsf=.5
#
#  cgsm sphereEigen -g=spheree2.order2 -diss=0.5 -tp=.05 -vClass=1 -nMode=2 -mMode=1 -go=halt -dsf=.5 
#  cgsm sphereEigen -g=spheree4.order2 -diss=0.5 -tp=.05 -vClass=1 -nMode=2 -mMode=1 -go=halt -dsf=.5 -tf=1. -show="sphereEigen4c1n2m0.show"
# 
# -- vibration-class 2
#    n=0 : radial vibrations:
#  cgsm sphereEigen -g=spheree2.order2 -diss=0.5 -tp=.05 -vClass=2 -nMode=0 -mMode=1 -go=halt -dsf=.25
#  cgsm sphereEigen -g=spheree4.order2 -diss=0.5 -tp=.05 -vClass=2 -nMode=0 -mMode=1 -go=halt -dsf=.25
#     -- godunov
#  cgsm sphereEigen -g=spheree2.order2 -pv=g -diss=0. -tp=.02 -vClass=2 -nMode=0 -mMode=1 -go=halt -dsf=.25
# 
#    n=2 : spheroidal vibrations:     
#     -- m=1 : 
#  cgsm sphereEigen -g=spheree2.order2 -diss=0.5 -tp=.05 -vClass=2 -nMode=2 -mMode=1 -go=halt -dsf=.25
#  cgsm sphereEigen -g=spheree2.order2 -diss=0. -filter=1 -filterFrequency=4 -tp=.05 -vClass=2 -nMode=2 -mMode=1 -go=halt -dsf=.25
#  cgsm sphereEigen -g=spheree4.order2 -diss=0.5 -tp=.05 -vClass=2 -nMode=2 -mMode=1 -go=halt -dsf=.25
#     -- godunov
#  cgsm sphereEigen -g=spheree2.order2 -pv=g -diss=0. -tp=.02 -vClass=2 -nMode=2 -mMode=1 -go=halt -dsf=.25
#     -- m=2: 
#  cgsm sphereEigen -g=spheree2.order2 -diss=0.5 -tp=.05 -vClass=2 -nMode=2 -mMode=2 -go=halt -dsf=.1
#  cgsm sphereEigen -g=spheree2.order2 -diss=0. -filter=1 -tp=.05 -vClass=2 -nMode=2 -mMode=2 -go=halt -dsf=.1
#  cgsm sphereEigen -g=spheree2.order2 -pv=g -diss=0. -tp=.02 -vClass=2 -nMode=2 -mMode=2 -go=halt -dsf=.1
#  cgsm sphereEigen -g=spheree4.order2 -pv=g -diss=0. -tp=.05 -vClass=2 -nMode=2 -mMode=2 -go=halt -dsf=.1
#
# 
# --- set default values for parameters ---
# 
$noplot=""; $backGround="box"; $grid="spheree1.order2"; $mu=1.; $lambda=1.; $pv="nc"; $ts="me"; 
$ic = "specialInitialCondition"; $specialOption="sphereEigenmode";
$Rg=8.314/27.; $yield=1.e10; $basePress=0.0; $c0=2.0; $cl=1.0; $hgFlag=2; $hgVisc=4.e-2; $rho=1.;
# turn off Q:
$Rg=8.314/27.; $yield=1.e10; $basePress=0.0; $c0=0.0; $cl=0.0; $hgFlag=0; $hgVisc=4.e-2;
$apr=0.0; $bpr=0.0; $cpr=0.0; $dpr=0.4;
$debug = 0;  $diss=.5;  $dissOrder=2; $bcn="sf"; $cons=0; $godunovOrder=2; 
$filter=0;   $filterOrder=6; $filterStages=2; $filterFrequency=1;
$tz = "poly"; $degreex=2; $degreet=2; $fx=2.; $fy=$fx; $fz=$fx; $ft=$fx;
$order = 2; $go="run"; 
$tFinal=5.; $tPlot=.05; $cfl=.9; $dsf=.5; $p0=2.; $p1=1.; $modem=1; $moden=0; $checkErrors=1; $plotVelocity=1; 
$ad=0.; # art. diss for Godunov
$vClass=1; # vibration class
$nMode=1; $mMode=0; 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"rho=f"=>\$rho,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"pv=s"=>\$pv,\
  "godunovOrder=f"=>\$godunovOrder,"p0=f"=>\$p0,"p1=f"=>\$p1,"modem=i"=>\$modem,"ts=s"=>\$ts,\
  "c0=f"=>\$c0,"cl=f"=>\$cl,"dsf=f"=>\$dsf,"vClass=i"=>\$vClass,"nMode=i"=>\$nMode,"mMode=i"=>\$mMode,\
  "dissOrder=i"=>\$dissOrder,"filter=i"=>\$filter,"filterOrder=i"=>\$filterOrder,"filterStages=i"=>\$filterStages,\
  "filterFrequency=i"=>\$filterFrequency,"ad=f"=>\$ad,"checkErrors=i"=>\$checkErrors,\
  "plotVelocity=i"=>\$plotVelocity  );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }else{ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
# 
if( $pv eq "nc" ){ $pv = "non-conservative"; }
if( $pv eq "c" ){ $pv = "conservative"; $cons=1; }
if( $pv eq "g" ){ $pv = "godunov"; }
if( $pv eq "h" ){ $pv = "hemp"; }
#
if( $ts eq "me" ){ $ts = "modifiedEquationTimeStepping"; }
if( $ts eq "fe" ){ $ts = "forwardEuler"; }
if( $ts eq "ie" ){ $ts = "improvedEuler"; }
if( $ts eq "ab" ){ $ts = "adamsBashforth2"; }
# 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
$grid
# -new: set-up stage: 
linear elasticity
$pv
 continue
# 
# -- set the time-stepping method:
$ts
# 
apply filter $filter
if( $filter eq 1 ){ $cmds = "filter order $filterOrder\n filter frequency $filterFrequency\n filter iterations 1\n filter coefficient 1. \n  filter stages $filterStages \n explicit filter\n  exit"; }else{ $cmds = "#"; }
$cmds
#
initial conditions options...
$rad=1.; # hard code for now -- radius of the sphere
Special initial condition option: $specialOption
  $vClass
  $nMode $mMode $rad
# 
specialInitialCondition
$ic 
close initial conditions options
# 
# -- reduce interpolation width for godunov --
SMPDE:rho $rho
SMPDE:Godunov order of accuracy $godunovOrder
SMPDE:lambda $lambda
SMPDE:mu $mu 
SMPDE:stressRelaxation 0
SMPDE:relaxAlpha 0.1
SMPDE:relaxDelta 0.1
if( $pv eq "godunov" ){ $cmds = "reduce interpolation width\n 2"; }else{ $cmds="#"; }
# $cmds
# --- start hemp parameters ---
SMPDE:Rg $Rg
SMPDE:yield stress $yield
SMPDE:base pressure $basePress
SMPDE:c0 viscosity $c0
SMPDE:cl viscosity $cl
SMPDE:hg viscosity $hgVisc
SMPDE:EOS polynomial $apr $bpr $cpr $dpr
SMPDE:hourglass control $hgFlag
# --- end hemp parameters ---
# 
OBTZ:$tz
OBTZ:twilight zone flow 0 
OBTZ:degree in space $degreex
OBTZ:degree in time $degreet
OBTZ:frequencies (x,y,z,t) $fx $fy $fz $ft
# l2 norm
# 
final time $tFinal
times to plot $tPlot
dissipation $diss
order of dissipation $dissOrder
SMPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad 
cfl $cfl
use conservative difference $cons
#
if( $bcn eq "d" ){ $bcmd = "bc: all=dirichlet"; }else{ $bcmd = "*"; };
if( $bcn eq "sf" ){$bcmd = "bc: all=tractionBC"; }
$bcmd
done
#
displacement scale factor $dsf
debug $debug
check errors $checkErrors
plot errors $checkErrors
# plot vorticity 1 
# plot divergence 1 
# For displacement solvers plot velocity and stress: 
if( $pv eq "non-conservative" || $pv eq "conservative" ){ $plotCommands = "plot velocity $plotVelocity\n plot stress $plotVelocity"; }else{ $plotCommands="*"; }
$plotCommands
# 
#*********************************
show file options...
  OBPSF:compressed
  OBPSF:open
    $show
 # OBPSF:frequency to save 
  OBPSF:frequency to flush 10
exit
#**********************************
continue
erase
displacement
  displacement scale factor 0.05
  plot block boundaries 0
  exit this menu
# 
x+r 20
y+r 20
$go


  erase
  displacement
  exit this menu

