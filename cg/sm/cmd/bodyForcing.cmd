#
#  cgsm: Example showing the application of a user defined body force
#
# Usage: (not all options implemented yet)
#   
#  cgsm [-noplot] bodyForcing -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -dissOrder=<> -order=<2/4> -debug=<num> ...
#                    -pv=[nc|c|g|h]  -bg=<backGround> -cons=[0/1] -dsf=<> -go=[run/halt/og]
# 
#  -bc : boundary conditions: -bc=d : displacement, -bc=sf :stress-free, 
#        -bc=ellipseDeform : specified motion of the boundary
#  -ic : initial conditions, ic=gaussianPulseForcing, ic=zero
#  -diss : coeff of artificial diffusion 
#  -go : run, halt, og=open graphics
#  -cons : 1= conservative difference 
#  -dsf : displacement scale factor
#  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
# 
# Examples:
#    cgsm bodyForcing -g=square10 -cons=0 -diss=1. -tp=.05 -tf=10.
# 
# -- godunov
#    cgsm bodyForcing -g=square20 -pv=g -tp=.1 -tf=10.
#
# -- hemp
#    cgsm bodyForcing -g=square20 -pv=h -ts=ie -tp=.1 -tf=10.
#
# --- set default values for parameters ---
# 
$Rg=8.314/27.; $yield=1.e10; $basePress=0.0; $c0=2.0; $cl=1.0; $hgFlag=2; $hgVisc=1.0e-2;
#$Rg=8.314/27.; $yield=1.e10; $basePress=0.0; $c0=0.0; $cl=0.0; $hgFlag=0; $hgVisc=1.0e-2;
$tFinal=10.; $tPlot=.05; $backGround="square"; $cfl=.9; $bc="sf"; $ic="zero"; $ts="me"; 
$exponent=10.; $x0=.5; $y0=.5; $z0=.5; $dsf=.4; 
$noplot=""; $grid="rectangle80.ar10"; $mu=1.; $lambda=1.;
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2; $cons=1; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx;
$order = 2; $go="run"; 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "dissOrder=i"=>\$dissOrder,"tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bc=s"=>\$bc,"ic=s"=>\$ic,"go=s"=>\$go,"noplot=s"=>\$noplot,"pv=s"=>\$pv,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"dsf=f"=>\$dsf,"x0=f"=>\$x0,"y0=f"=>\$y0,\
  "ts=s"=>\$ts );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }else{ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
# 
if( $pv eq "nc" ){ $pv = "non-conservative"; $cons=0; }
if( $pv eq "c" ){ $pv = "conservative"; $cons=1; }
if( $pv eq "g" ){ $pv = "godunov"; }
if( $pv eq "h" ){ $pv = "hemp"; }
# 
if( $ts eq "me" ){ $ts = "modifiedEquationTimeStepping"; }
if( $ts eq "fe" ){ $ts = "forwardEuler"; }
if( $ts eq "ie" ){ $ts = "improvedEuler"; }
if( $ts eq "ab" ){ $ts = "adamsBashforth2"; }
# 
if( $bc eq "d" ){ $bc = "all=displacementBC"; }
if( $bc eq "sf" ){ $bc = "all=tractionBC"; }
if( $bc eq "ellipseDeform" ){ $bc = "all=displacementBC , userDefinedBoundaryData\n ellipse deform\n .25 1.\n done"; }
if( $ic eq "gaussianPulse" ){ $ic="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 \n"; }
if( $ic eq "zero" ){ $ic = "zeroInitialCondition"; }; 
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
# set-up stage: 
linear elasticity
$pv
 continue
# 
$ts
# 
# ----- trig IC's ----
# twilightZoneInitialCondition
# trigonometric
# TZ omega: 2 2 2 2 (fx,fy,fz,ft)
# -----------------------------
close forcing options
# 
final time $tFinal
times to plot $tPlot
# 
SMPDE:Rg $Rg
SMPDE:yield stress $yield
SMPDE:base pressure $basePress
SMPDE:c0 viscosity $c0
SMPDE:cl viscosity $cl
SMPDE:hg viscosity $hgVisc
SMPDE:lambda $lambda
SMPDE:mu $mu 
#
# ----- define the body force ---
 userDefinedForcing
  constant forcing
   0 1.
   1 1.
   done
exit
# 
boundary conditions
 $bc
done  
#
displacement scale factor $dsf
dissipation $diss
order of dissipation $dissOrder
cfl $cfl
use conservative difference $cons
# 
plot divergence 1
plot vorticity 1
initial conditions options...
if( $pv eq "hemp" ){ $ic="hempInitialCondition\n OBIC:Hemp initial condition option: default\n"; }
$ic 
# gaussianPulseInitialCondition
# Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0)
close initial conditions options
#
debug $debug
#
continue
# 
erase
displacement
exit
