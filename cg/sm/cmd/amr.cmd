#
#  cgsm: amr examples
#
# Usage: (not all options implemented yet)
#   
#  cgsm [-noplot] amr -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -dissOrder=<> -order=<2/4> -debug=<num> ...
#                       -bc=[d|sf|slip|dirichlet] -bg=<backGround> -pv=[nc|c|g|h] -godunovOrder=[1|2] ...
#                     -nrl=<> -ratio=<> -go=[run/halt/og]
# 
#  -bc : boundary conditions: -bc=d : displacement, -bc=sf :stress-free, 
#        -bc=ellipseDeform : specified motion of the boundary
#  -ic : initial conditions, ic=gaussianPulse, ic=zero, ic=special
#  -diss : coeff of artificial diffusion 
#  -go : run, halt, og=open graphics
#  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
# 
# Examples:
#   cgsm amr -g=square20 -pv=nc -diss=1. -tp=.05 -tf=10. -tol=.001 -ic=special -bc=dirichlet -debug=3 -show="amr20l2r2.show"
#   cgsm amr -g=square40 -pv=nc -diss=1. -tp=.05 -tf=10. -tol=.0001 -ic=special -bc=dirichlet -debug=3 -nrl=3 -ratio=2
#   cgsm amr -g=square80 -pv=nc -diss=1. -tp=.05 -tf=10. -tol=.0001 -ic=special -bc=dirichlet -debug=3 -nrl=3 -ratio=2 -show="amr80l3r2.show"
# 
#   cgsm amr -g=cice2.order2 -pv=nc -diss=1. -tp=.05 -tf=10. -tol=.001 -ic=special -bc=dirichlet -debug=3 
# -- 3 levels:
#   cgsm amr -g=square40 -pv=nc -diss=2. -tp=.01 -tf=10. -tol=.0001 -ic=special -bc=dirichlet -debug=3 -ratio=4 -nrl=3
# -- conservative
#   cgsm amr -g=square40 -pv=c -diss=1. -tp=.05 -tf=10. -tol=.0001 -ic=special -bc=dirichlet -debug=3 -nrl=3 -ratio=2
# -- godunov:
#   cgsm amr -g=square20 -pv=g -diss=.5 -tp=.05 -tf=10. -tol=.001 -ic=special -bc=dirichlet -debug=3 
#   cgsm amr -g=square40 -pv=g -diss=.5 -tp=.05 -tf=10. -tol=.001 -ic=special -bc=dirichlet -debug=3 -nrl=3 -ratio=4
#   cgsm amr -g=sise1.order2 -pv=g -diss=0. -tz=poly -degreex=1 -degreet=0 -xTopHat=-.5 -yTopHat=-.5 -useTopHat=1 -bc=d -tp=.01
#   cgsm amr -g=sise2.order2 -pv=g -diss=0. -tz=trig -fx=1. -fy=1. -ft=1. -xTopHat=-.5 -yTopHat=-.5 -useTopHat=1 -bc=d -tp=.05
# -- hemp : this doesn't work yet: 
#   cgsm amr -g=square20 -pv=h -ts=ie -diss=.5 -tp=.05 -tf=10. -tol=.001 -ic=special -bc=dirichlet -debug=3 
# 
# -- pulse TZ:
#   cgsm amr -g=square40 -pv=nc -diss=1. -tp=.05 -tf=10. -tol=.001 -tz=pulse -x0=.25 -y0=.25 -bc=dirichlet 
#   cgsm amr -g=sise3.order2 -pv=nc -diss=1. -tp=.05 -tf=10. -tol=.01 -tz=pulse  -x0=-.5 -y0=-.5 -bc=dirichlet 
#   cgsm amr -g=rsise4.order2 -pv=nc -diss=1. -tp=.05 -tf=10. -tol=.01 -tz=pulse  -x0=-.5 -y0=-.5 -bc=dirichlet 
#   cgsm amr -g=square40 -pv=nc -diss=0. -tp=.05 -tf=10. -tz=poly -degreex=2 -degreet=2 -useTopHat=1 -bc=d [exact]
#   cgsm amr -g=square80 -pv=nc -diss=0. -tp=.05 -tf=10. -tz=poly -degreex=2 -degreet=2 -useTopHat=1 -bc=d [exact]
#     -- not exact: fix me: 
#   cgsm amr -g=sise2.order2 -pv=nc -diss=0. -tz=poly -degreex=2 -degreet=2 -xTopHat=-.5 -yTopHat=-.5 -useTopHat=1 -bc=d 
#   cgsm amr -g=sise1.order2 -pv=nc -diss=0. -tz=poly -degreex=1 -degreet=0 -xTopHat=-.5 -yTopHat=-.5 -useTopHat=1 -bc=d -tp=.01 [exact]
#   cgsm amr -g=sise1.order2 -pv=nc -diss=0. -tz=poly -degreex=2 -degreet=0 -xTopHat=-.5 -yTopHat=-.5 -useTopHat=1 -bc=d -tp=.01 [not exact]
#   cgsm amr -g=sise2.order2 -pv=nc -diss=0. -tz=trig -fx=1. -fy=1. -ft=1. -xTopHat=-.5 -yTopHat=-.5 -useTopHat=1 -bc=d -tp=.05
# 
#   cgsm amr -g=square10 -pv=nc -diss=0. -tp=.05 -tf=10. -tol=.001 -tz=poly -amr=0 -bc=dirichlet 
# 
# --- set default values for parameters ---
# 
$tFinal=10.; $tPlot=.05; $backGround="square"; $cfl=.9; $bc="sf"; $pv="nc"; $ts="me"; 
$ic="special";  $exponent=10.; $x0=.5; $y0=.5; $z0=.5; $specialOption="travelingWave"; 
$noplot=""; $grid="square20"; $mu=1.; $lambda=1.; $godunovOrder=2; 
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2; $bc="sf"; $cons=1; $dsf=0.4;
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=$fx; $fz=$fx; $ft=$fx;
$amr=1; $useTopHat=0; $xTopHat=.25; $yTopHat=.25; $zTopHat=0.; 
$ratio=2;  $nrl=2;  # refinement ratio and number of refinement levels
$tol=.001;  $nbz=2;   # amr tol and number-of-buffer-zones
$order = 2; $go="halt"; 
$filter=0; $filterOrder=6; $filterStages=2; 
# -- hemp parameters: 
$Rg=8.314/27.; $yield=1.e10; $basePress=0.0; $c0=2.0; $cl=1.0; $hgFlag=2; $hgVisc=4.e-2; $rho=1.;
# turn off Q:
$Rg=8.314/27.; $yield=1.e10; $basePress=0.0; $c0=0.0; $cl=0.0; $hgFlag=0; $hgVisc=4.e-2;
$apr=0.0; $bpr=0.0; $cpr=0.0; $dpr=0.4;
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "dissOrder=i"=>\$dissOrder,"tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bc=s"=>\$bc,"ic=s"=>\$ic,"go=s"=>\$go,"noplot=s"=>\$noplot,"tol=f"=>\$tol,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"x0=f"=>\$x0,"y0=f"=>\$y0,"dsf=f"=>\$dsf,\
  "pv=s"=>\$pv,"exponent=f"=>\$exponent,"godunovOrder=f"=>\$godunovOrder,"specialOption=s"=>\$specialOption,\
   "nrl=i"=>\$nrl,"nbz=i"=>\$nbz,"ratio=i"=>\$ratio,"ts=s"=>\$ts,"amr=i"=>\$amr, "useTopHat=i"=>\$useTopHat,\
   "xTopHat=f"=>\$xTopHat,"yTopHat=f"=>\$yTopHat,"zTopHat=f"=>\$zTopHat,\
   "fx=f"=>\$fx,"fy=f"=>\$fy,"fz=f"=>\$fz,"ft=f"=>\$ft,"filter=i"=>\$filter,"filterOrder=i"=>\$filterOrder,\
   "filterStages=i"=>\$filterStages );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }
if( $tz eq "trig" ){ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
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
# 
if( $bc eq "d" ){ $bc = "all=displacementBC"; }
if( $bc eq "sf" ){ $bc = "all=tractionBC"; }
if( $bc eq "slip" ){ $bc = "all=slipWall"; }
if( $bc eq "dirichlet" ){ $bc = "all=dirichletBoundaryCondition"; }
if( $bc eq "ellipseDeform" ){ $bc = "all=displacementBC , userDefinedBoundaryData\n ellipse deform\n .25 1.\n done"; }
if( $ic eq "gaussianPulse" ){ $ic="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 \n"; }
if( $ic eq "zero" ){ $ic = "zeroInitialCondition"; }; 
if( $ic eq "special" ){ $ic = "specialInitialCondition"; }
if( $tz ne "none" ){ $ic = "#"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
# 
#
$grid
# 
# -new: set-up stage: 
linear elasticity
$pv
 continue
# -- set the time-stepping method:
$ts
apply filter $filter
if( $filter eq 1 ){ $cmds = "filter order $filterOrder\n filter frequency 1\n filter iterations 1\n filter coefficient 1. \n  filter stages $filterStages \n explicit filter\n  exit"; }else{ $cmds = "#"; }
$cmds
# 
# modifiedEquationTimeStepping
# ----- Twilight zone
if( $tz ne "none" ){ $cmds = "OBTZ:$tz\n OBTZ:twilight zone flow 1"; }else{ $cmds = "OBTZ:twilight zone flow 0"; }
$cmds
OBTZ:degree in space $degreex
OBTZ:degree in time $degreet
OBTZ:frequencies (x,y,z,t) $fx $fy $fz $ft
$pulseAmp=1.; $pulsePower=1; $pulseExponent=30.; 
OBTZ:pulse amplitude, exponent, power $pulseAmp $pulseExponent $pulsePower
OBTZ:pulse center $x0 $y0 $z0
OBTZ:pulse velocity 1 1 1
# -----------------------------
close forcing options
# 
final time $tFinal
times to plot $tPlot
# 
SMPDE:lambda $lambda
SMPDE:mu $mu 
SMPDE:Godunov order of accuracy $godunovOrder
#
# --- start hemp parameters ---
SMPDE:Rg $Rg
SMPDE:yield stress $yield
SMPDE:base pressure $basePress
SMPDE:c0 viscosity $c0
SMPDE:cl viscosity $cl
SMPDE:hg viscosity $hgVisc
SMPDE:lambda $lambda
SMPDE:mu $mu 
SMPDE:EOS polynomial $apr $bpr $cpr $dpr
SMPDE:rho $rho
SMPDE:Godunov order of accuracy $godunovOrder
SMPDE:hourglass control $hgFlag
# --- end hemp parameters ---
# 
boundary conditions
  $bc
 # ---
#!  all=displacementBC , userDefinedBoundaryData
#!    ellipse deform
#!    .25 1.
#!  done
 # --- 
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
initial conditions options...
if( $tz eq "none" ){ $cmds = "specialInitialCondition\nSpecial initial condition option: $specialOption"; }else{ $cmds="#"; }
$cmds
# 1 p-wave:
$np=1; $ns=0; 
$ap=1.; $k1=1.; $k2=0.; $k3=0.; $xa=-1.; $ya=.0; $za=0.; # pwave 1 
if( $tz eq "none" ){ $cmds = "$np $ns\n $ap $k1 $k2 $k3 $xa $ya $za\n"; }else{ $cmds="#"; }
$cmds
# --- superseismic soln from Don ---
# alpha1=2.934768e-01  va=(8.332125e-02,-1.323250e-01) kappa=(5.328216e-01,-8.462276e-01)
# alpha2=-2.214236e-01  vb=(2.609750e-02,-1.482750e-01) kappa=(2.683919e-01,-9.633098e-01)
# lambda=1.398307e-01, mu=7.203351e-02, cp=5.328205e-01, cs=2.683906e-01
#* $np=1; $ns=1; 
#* $ap=2.934768e-01; $k1=5.328216e-01; $k2=-8.462276e-01; $k3=0.; $xa=.5; $ya=.5; $za=0.; # pwave 1 
#* if( $tz eq "none" ){ $cmds = "$np $ns\n $ap $k1 $k2 $k3 $xa $ya $za\n"; }else{ $cmds="#"; }
#* $cmds
#* $ap=-2.214236e-01; $k1=2.683919e-01; $k2=-9.633098e-01; $k3=0.; $xa=.5; $ya=.5; $za=0.; # swave1
#* if( $tz eq "none" ){ $cmds = " $ap $k1 $k2 $k3 $xa $ya $za\n"; }else{ $cmds="#"; }
#* $cmds
$ic 
# gaussianPulseInitialCondition
# Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0)
close initial conditions options
#
# ---------------------------AMR------------------------------
if( $amr eq "1" ){ $cmd =" turn on adaptive grids"; }else{ $cmd =" turn off adaptive grids"; }
$cmd
#   save error function to the show file
  show amr error function 0
#
if( $useTopHat eq 1 ){ $cmds = "use top-hat for error function"; }else{ $cmds="#"; }
$cmds
#
  top hat parameters
    $xTopHat $yTopHat $zTopHat
    .125
    1. 1. 0.
#
  $amrInterpOrder=3; # =2; 
  order of AMR interpolation $amrInterpOrder
  error threshold
     $tol 
  regrid frequency
    $regrid=$nbz*$ratio;
    $regrid
  change error estimator parameters
    weight for first difference
      0.
    weight for second difference
      1.
    default number of smooths
      1
    set scale factors     
      1 1 1 1 1 1 1 1 1 1 1
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
      .7 
  exit
# --------------------------------------------------------------
#
$checkErrors=0;
if( tz ne "none" || $ic eq "specialInitialCondition" ){ $checkErrors=1; }
check errors $checkErrors
plot errors $checkErrors
check error on ghost
   1
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


erase
displacement
exit
