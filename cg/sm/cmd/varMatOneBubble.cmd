#
#  cgsm: Example using variable material parameters.
#
# Usage: (not all options implemented yet)
#   
#  cgsm [-noplot] varMat -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -dissOrder=<> -order=<2/4> -debug=<num> ...
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
#  SOS
#    cgsm varMatOneBubble -show=oneBubbleSOS.show -g=square320 -pv=c  -tp=.01
#
#  FOS
#    cgsm varMatOneBubble -show=oneBubbleFOS.show -g=square320 -pv=g  -tp=.01
#    cgsm varMatOneBubble -show=oneBubbleFOS.show -g=square320 -pv=g  -tp=.01 -ad=1.
#
#
# --- set default values for parameters ---
#
$tFinal=.4; $tPlot=.02; $backGround="square"; $cfl=.9; $bc="sf"; $ic="zero"; $ts="me"; 
$exponent=10.; $dsf=.4; $show="";
$x0=.2; $y0=.5; $z0=.0; $specialOption="travelingWave"; 
$noplot=""; $grid="rectangle80.ar10"; $mu=1.; $lambda=1.;
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2; $godunovOrder=2; $cons=1; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx;
$order = 2; $go="run"; 
$Rg=8.314/27.; $yield=1.e10; $basePress=0.0; $c0=2.0; $cl=1.0; $hgFlag=2; $hgVisc=1.0e-2;
#$Rg=8.314/27.; $yield=1.e10; $basePress=0.0; $c0=0.0; $cl=0.0; $hgFlag=0; $hgVisc=1.0e-2;
$ad=0.; $ad4=0.;  # art. diss for Godunov
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "dissOrder=i"=>\$dissOrder,"tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bc=s"=>\$bc,"ic=s"=>\$ic,"go=s"=>\$go,"noplot=s"=>\$noplot,"pv=s"=>\$pv,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"dsf=f"=>\$dsf,"x0=f"=>\$x0,"y0=f"=>\$y0,\
  "ts=s"=>\$ts,"godunovOrder=f"=>\$godunovOrder,"ad=f"=>\$ad,"ad4=f"=>\$ad4 );
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
# Turn on variable material properties:
variable material properties 1
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
SMPDE:Godunov order of accuracy $godunovOrder
SMPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad
SMPDE:fourth-order artificial diffusion $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 
#
# ----- define variable material properties ---
#
$numBubbles=1;
$rho0=1.0; $mu0=1.0; $lambda0=1.0;
$rho1=1.0; $mu1=10.0; $lambda1=10.0;
$rb1=.2; $xb1=.5; $yb1=.5;
#
#   forcing options...
# Add new user defined options in userDefinedMaterialProperties.C 
  user defined material properties...
    bubbles
     $numBubbles
  # backGround values:
     $rho0
     $mu0
     $lambda0
  # bubble 1: (radius, x0,y0,z0)
     $rb1 $xb1 $yb1 0.
     $rho1
     $mu1
     $lambda1
    exit
# ---------------------------------------------
# 
boundary conditions
 # apply exact solution on all boundaries
 all=dirichletBoundaryCondition
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
# 
initial conditions options...
  specialInitialCondition  
  Special initial condition option: $specialOption
  # 1 p-wave:
  $np=1; $ns=0; 
  $np $ns
  $ap=1.; $k1=1.; $k2=0.; $k3=0.; $xa=$x0; $ya=$y0; $za=0.; # pwave 1 
  $ap $k1 $k2 $k3 $xa $ya $za
  # 
  specialInitialCondition
close initial conditions options
#
debug $debug
#
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



erase
displacement
exit
