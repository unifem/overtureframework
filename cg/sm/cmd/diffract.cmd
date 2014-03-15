#
#  cgsm: diffraction examples
#
# Usage: (not all options implemented yet)
#   
#  cgsm [-noplot] diffract -g=<name> -tf=<tFinal> -tp=<tPlot> -amr=[0|1] -diss=<> -dissOrder=<> -order=<2/4> -debug=<num> ...
#                       -bc=[d|sf|slip|dirichlet] -bg=<backGround> -pv=[nc|c|g|h] -godunovOrder=[1|2] ...
#                     -nrl=<> -ratio=<> -go=[run/halt/og]
# 
#  -amr : 1=turn on amr. 
#  -bc : boundary conditions: -bc=d : displacement, -bc=sf :stress-free, 
#        -bc=ellipseDeform : specified motion of the boundary
#  -ic : initial conditions, ic=gaussianPulse, ic=zero, ic=special
#  -diss : coeff of artificial diffusion 
#  -go : run, halt, og=open graphics
#  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
# 
# Examples:
# 
#  -- diffraction of a traveling wave by a cylinder -- use filter -- better
#   cgsm diffract -g=cice2.order2 -pv=nc -tp=.1  -tol=.001 -ic=special -bc=d -x0=-1.25
#   cgsm diffract -g=cice2.order2 -pv=c  -tp=.1  -tol=.001 -ic=special -bc=d -x0=-1.25
#   cgsm diffract -g=cice2.order2 -pv=c -tp=.1  -tol=.001 -diss=1. -dissOrder=4 -ic=special -bc=sf -x0=-1.25 [traction BC
#   cgsm diffract -g=cice4.order2 -pv=c -diss=0. -tp=.1  -tol=.001 -ic=special -bc=d  -x0=-1.25
#   cgsm diffract -g=cice8.order2 -pv=c -diss=20. -tp=.1  -tol=1.e-4 -ic=special -bc=sf -x0=-1.25
#  -- godunov:
#   cgsm diffract -g=cice2.order2 -pv=g -filter=0 -tp=.1  -tol=.001 -ic=special -bc=d -x0=-1.25
#   cgsm diffract -g=cice4.order2 -pv=g -filter=0 -tp=.1  -tol=.005 -ic=special -bc=d -x0=-1.25
#     -- ok: (no amr)
#   cgsm diffract -g=cice8.order2 -pv=nc -diss=4. -tp=.1  -tol=.001 -ic=special -bc=sf -amr=0 -x0=-1.25
#   cgsm diffract -g=cice16.order2 -pv=nc -diss=8. -tp=.1  -tol=.001 -ic=special -bc=sf -amr=0 -x0=-1.25
# 
# --- set default values for parameters ---
# 
$tFinal=1.2; $tPlot=.05; $backGround="square"; $cfl=.9; $bc="sf"; $pv="nc"; $ts="me"; 
$ic="special";  $exponent=10.; $x0=.5; $y0=.5; $z0=.5; $specialOption="travelingWave"; 
$noplot=""; $grid="square20"; $mu=1.; $lambda=1.; $godunovOrder=2; $godunovLimiter=0; 
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2;  $filter=1; $filterOrder=6; $filterStages=2; $bc="sf"; $cons=1; $dsf=0.4;
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx; $restart=""; 
$amr=1; $ratio=2;  $nrl=2;  # refinement ratio and number of refinement levels
$tol=.001;  $nbz=2;  $nbza=2;  # amr tol and number-of-buffer-zones (nbz+ value used to compute regrid, nbza=actual)
$order = 2; $go="halt"; 
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
  "pv=s"=>\$pv,"exponent=f"=>\$exponent,"godunovOrder=i"=>\$godunovOrder,"specialOption=s"=>\$specialOption,\
   "amr=i"=>\$amr,"nrl=i"=>\$nrl,"nbz=i"=>\$nbz,"nbza=i"=>\$nbza,"ratio=i"=>\$ratio,"ts=s"=>\$ts,"filter=i"=>\$filter,\
   "filterOrder=i"=>\$filterOrder,"filterStages=i"=>\$filterStages,"godunovLimiter=i"=>\$godunovLimiter,\
   "restart=s"=>\$restart);
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }else{ $tz="trigonometric"; }
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
if( $bc eq "d" ){ $bc = "Annulus=displacementBC"; }
if( $bc eq "sf" ){ $bc = "Annulus=tractionBC"; }
if( $bc eq "slip" ){ $bc = "all=slipWall"; }
if( $bc eq "dirichlet" ){ $bc = "all=dirichletBoundaryCondition"; }
if( $bc eq "ellipseDeform" ){ $bc = "all=displacementBC , userDefinedBoundaryData\n ellipse deform\n .25 1.\n done"; }
if( $ic eq "gaussianPulse" ){ $ic="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 \n"; }
if( $ic eq "zero" ){ $ic = "zeroInitialCondition"; }; 
if( $ic eq "special" ){ $ic = "specialInitialCondition"; }
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
# -- set the time-stepping method:
$ts
# 
apply filter $filter
if( $filter eq 1 ){ $cmds = "filter order $filterOrder\n filter frequency 1\n filter iterations 1\n filter coefficient 1. \n  filter stages $filterStages\n explicit filter\n  exit"; }else{ $cmds = "#"; }
$cmds
# 
# modifiedEquationTimeStepping
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
SMPDE:lambda $lambda
SMPDE:mu $mu 
SMPDE:Godunov order of accuracy $godunovOrder
SMPDE:stressRelaxation 0
SMPDE:relaxAlpha 0.1
SMPDE:relaxDelta 0.0
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
SMPDE:slope limiting for Godunov $godunovLimiter
SMPDE:hourglass control $hgFlag
# --- end hemp parameters ---
# 
boundary conditions
#  $bc
 all=dirichletBoundaryCondition
#  -- we need a slipWall or symmetry BC for SOS-C
square(0,0)=dirichletBoundaryCondition
square(1,0)=displacementBC
# -- for now SOS-C has no slipWall
$bcSlip="slipWall"; 
# if( $pv ne "conservative" ){ $bcSlip="slipWall"; }else{ $bcSlip="dirichletBoundaryCondition"; }
square(0,1)=$bcSlip
square(1,1)=$bcSlip
 $bc
# Annulus=displacementBC
# Annulus=tractionBC
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
specialInitialCondition
Special initial condition option: $specialOption
# 1 p-wave:
$np=1; $ns=0; 
$np $ns
$ap=1.; $k1=1.; $k2=0.; $k3=0.; $xa=$x0; $ya=$y0; $za=0.; # pwave 1 
$ap $k1 $k2 $k3 $xa $ya $za
# 2 pwaves:
# $np=2; $ns=0; 
# $np $ns
# $ap=1.; $k1=.5; $k2=.5; $k3=0.; $xa=.25; $ya=.5; $za=0.; # pwave 1 
# $ap $k1 $k2 $k3 $xa $ya $za
# $ap=1.; $k1=.5; $k2=-.5; $k3=0.; $xa=.25; $ya=.5; $za=0.; # pwave 2
# $ap $k1 $k2 $k3 $xa $ya $za
#  s-wave: 
# $np=0; $ns=1; 
# $np $ns
# $ap=1.; $k1=1.; $k2=0.; $k3=0.; $xa=.25; $ya=.5; $za=0.; # swave 1 
# $ap $k1 $k2 $k3 $xa $ya $za
#
# --- superseismic soln from Don ---
# alpha1=2.934768e-01  va=(8.332125e-02,-1.323250e-01) kappa=(5.328216e-01,-8.462276e-01)
# alpha2=-2.214236e-01  vb=(2.609750e-02,-1.482750e-01) kappa=(2.683919e-01,-9.633098e-01)
# lambda=1.398307e-01, mu=7.203351e-02, cp=5.328205e-01, cs=2.683906e-01
# $np=1; $ns=1; 
# $np $ns
# $ap=2.934768e-01; $k1=5.328216e-01; $k2=-8.462276e-01; $k3=0.; $xa=.5; $ya=.5; $za=0.; # pwave 1 
# $ap $k1 $k2 $k3 $xa $ya $za
# $ap=-2.214236e-01; $k1=2.683919e-01; $k2=-9.633098e-01; $k3=0.; $xa=.5; $ya=.5; $za=0.; # swave1
# $ap $k1 $k2 $k3 $xa $ya $za
# 
if( $restart eq "" ){ $icCmds = $ic; }\
 else{ $icCmds = "use grid from show file 1\n always interpolate from show file 1\n read from a show file\n $restart\n -1"; }
$icCmds
# gaussianPulseInitialCondition
# Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0)
close initial conditions options
#
# ---------------------------------------------------------
if( $amr eq "1" ){ $cmd =" turn on adaptive grids"; }else{ $cmd =" turn off adaptive grids"; }
$cmd
#   save error function to the show file
#  show amr error function 1
  order of AMR interpolation
      2
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
      $nbza
#    width of proper nesting 
#      1
    grid efficiency
      .7 
  exit
# --------------------------------------------------------------
#
$checkErrors=0;
# if( $ic eq "specialInitialCondition" ){ $checkErrors=1; }
check errors $checkErrors
plot errors $checkErrors
#
# For displacement solvers plot velocity and stress: 
if( $pv eq "non-conservative" || $pv eq "conservative" ){ $plotCommands = "plot velocity 1\n plot stress 1"; }else{ $plotCommands="*"; }
$plotCommands
#*********************************
show file options...
  OBPSF:compressed
  * specify the max number of parallel hdf sub-files: 
  OBPSF:maximum number of parallel sub-files 4
  OBPSF:open
    $show
 # OBPSF:frequency to save 
  OBPSF:frequency to flush 2
exit
#**********************************
continue
# 


erase
displacement
exit
