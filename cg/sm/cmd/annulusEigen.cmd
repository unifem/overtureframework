#
# cgsm -- eigenfunctions of an annulus, compare to an exact solution
#
# Usage:
#   
#  cgsm [-noplot] annulusEigen -g=<name> -tz=<poly/trig> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
#                    -bcn=[d|sf|mixed] -diss=<> -order=<2/4> -debug=<num> -bg=<backGround> -cons=[0/1] ...
#                    -pv=[nc|c|g|h] -godunovOrder=[1|2] -mu=<> -lambda=<> -rho=<> -go=[run/halt/og]
# 
#  -mode : 0=steady, 1=time-harmonic
#  -diss : coeff of artificial diffusion 
#  -bcn : d=dirichlet, sf=stress-free
#  -go : run, halt, og=open graphics
#  -cons : 1= conservative difference 
#  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
#  -en : error norm, "max", "l1" or "l2"
# 
# Examples:
# 
# -- traction bc: 
#  cgsm annulusEigen -g=annulus2.order2.hdf -tp=.1 -p0=1. -p1=2. -modem=0 -bcn=sf -go=halt   [steady state]
#  cgsm annulusEigen -g=annulus2.order2.hdf -tp=.1 -p0=1. -p1=2. -modem=1 -bcn=sf -go=halt -show=annulusEigen.show   [eigen-mode 1]
#  cgsm annulusEigen -g=annulus2.order2.hdf -tp=.02 -p0=0. -p1=0. -modem=2 -bcn=sf -go=halt   [eigen-mode 2]
#  - lambda=100 case: (nc with diss=0 goes bad early)
#  cgsm annulusEigen -g=annulus4.order2.hdf -pv=c -diss=0. -tp=.1 -p0=1. -p1=2. -lambda=100. -modem=1 -bcn=sf -go=halt
# 
#  -- displacement bc: 
#  cgsm annulusEigen -g=annulus2.order2.hdf -tp=.02 -tf=2. -modem=1 -bcn=d -go=halt  [eigen-mode 1]
# 
# -- test split annulus:
#   cgsm annulusEigen -g=annulusSplit -tp=.02 -tf=2. -modem=1 -bcn=d -pv=c -go=halt
#     trouble at boundary+interp: 
#   cgsm annulusEigen -g=annulusSplit1 -tp=.02 -tf=2. -modem=1 -bcn=d -pv=c -go=halt
#     -- errors along interpolation boundary with godunov and iw=2: 
#   cgsm annulusEigen -g=annulusSplit -tp=.02 -tf=2. -modem=1 -bcn=d -pv=g -iw=2 -go=halt -ad=1. -ad4=.5
# 
# -- godunov traction bc: 
#  cgsm annulusEigen -g=annulus2.order2.hdf -tp=.1 -p0=1. -p1=2. -pv=g -modem=0 -bcn=sf -go=halt   [steady state]
#  cgsm annulusEigen -g=annulus2.order2.hdf -tp=.1 -p0=1. -p1=2. -pv=g -modem=1 -bcn=sf -go=halt  [eigen-mode 1]
#  cgsm annulusEigen -g=annulus4.order2.hdf -tp=.02 -p0=1. -p1=2. -pv=g -modem=2 -bcn=sf -go=halt  [eigen-mode 2]
# 
#  cgsm annulusEigen -g=annulusSplit -tp=.1 -p0=1. -p1=2. -pv=g -modem=1 -bcn=sf -go=halt  [eigen-mode 1]
# 
# -- godunov displacement bc: 
#  cgsm annulusEigen -g=annulus1.order2.hdf -tp=.02 -pv=g -modem=1 -bcn=d -go=halt [eigen-mode 1]
#  cgsm annulusEigen -g=annulus2.order2.hdf -tp=.02 -pv=g -modem=1 -bcn=d -go=halt [eigen-mode 1]
#  cgsm annulusEigen -g=annulus8.order2.hdf -tf=.5 -tp=.05 -pv=g -modem=1 -bcn=d -lambda=100 -go=halt
#
# -- hemp
#  cgsm annulusEigen -g=annulus1.order2.hdf -tp=.1 -p0=1. -p1=2. -ts=ie -pv=h -modem=0 -bcn=sf -go=halt
#  cgsm annulusEigen -g=annulus1.order2.hdf -tp=.1 -p0=.5 -p1=1. -ts=ie -pv=h -modem=0 -bcn=sf -go=halt
#  cgsm annulusEigen -g=annulus2.order2.hdf -tp=.1 -p0=1.e-6 -p1=2.e-6 -ts=ie -pv=h -modem=0 -bcn=sf -cfl=.1 -go=halt
#  cgsm annulusEigen -g=annulus1.order2.hdf -tp=.1 -p0=1. -p1=2. -ts=ie -pv=h -modem=1 -bcn=sf -go=halt
# dirichlet: **broken**
#  cgsm annulusEigen -g=annulus1.order2.hdf -tp=.1 -p0=1. -p1=2. -ts=ie -pv=h -modem=1 -bcn=d -go=halt
# 
# --- set default values for parameters ---
# 
$noplot=""; $backGround="square"; $grid="square10"; $mu=1.; $lambda=1.; $pv="nc"; $ts="me"; 
$Rg=8.314/27.; $yield=1.e10; $basePress=0.0; $c0=2.0; $cl=1.0; $hgFlag=2; $hgVisc=4.e-2; $rho=1.;
# turn off Q:
$Rg=8.314/27.; $yield=1.e10; $basePress=0.0; $c0=0.0; $cl=0.0; $hgFlag=0; $hgVisc=4.e-2;
$apr=0.0; $bpr=0.0; $cpr=0.0; $dpr=0.4;
$debug = 0;  $tPlot=.1; $bcn="sf"; $cons=0; $godunovOrder=2; $iw=2; 
$diss=.0; $dissOrder=2;  $filter=1; $filterOrder=6; $filterStages=2;
$tz = "poly"; $degreex=2; $degreet=2; $fx=2.; $fy=$fx; $fz=$fx; $ft=$fx;
$order = 2; $go="run"; 
$tFinal=5.; $cfl=.9; $dsf=.2; $p0=2.; $p1=1.; $modem=1; $moden=0; 
$en="max";
$ad=0.; $ad4=0.;  # art. diss for Godunov
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug,"ad=f"=>\$ad,"ad4=f"=>\$ad4, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"iw=i"=>\$iw,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"rho=f"=>\$rho,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"pv=s"=>\$pv,\
  "godunovOrder=f"=>\$godunovOrder,"p0=f"=>\$p0,"p1=f"=>\$p1,"modem=i"=>\$modem,"ts=s"=>\$ts,"en=s"=>\$en,\
  "c0=f"=>\$c0,"cl=f"=>\$cl,"filter=i"=>\$filter,"filterOrder=i"=>\$filterOrder,"filterStages=i"=>\$filterStages );
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
if( $en eq "max" ){ $errorNorm="maximum norm"; }
if( $en eq "l1" ){ $errorNorm="l1 norm"; }
if( $en eq "l2" ){ $errorNorm="l2 norm"; }
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
if( $filter eq 1 ){ $cmds = "filter order $filterOrder\n filter frequency 1\n filter iterations 1\n filter coefficient 1. \n  filter stages $filterStages \n explicit filter\n  exit"; }else{ $cmds = "#"; }
$cmds
# 
#  NOTE: assign lambda,mu BEFORE setting IC's since stress depends on lambda
SMPDE:lambda $lambda
SMPDE:mu $mu 
# --- start hemp parameters ---
SMPDE:Rg $Rg
SMPDE:yield stress $yield
SMPDE:base pressure $basePress
SMPDE:c0 viscosity $c0
SMPDE:cl viscosity $cl
SMPDE:hg viscosity $hgVisc
SMPDE:EOS polynomial $apr $bpr $cpr $dpr
SMPDE:rho $rho
SMPDE:Godunov order of accuracy $godunovOrder
SMPDE:hourglass control $hgFlag
SMPDE:stressRelaxation 0
SMPDE:relaxAlpha 0.1
SMPDE:relaxDelta 0.1
# --- end hemp parameters ---
#
if( $pv eq "godunov" && $iw eq 2 ){ $cmds = "reduce interpolation width\n $iw"; }else{ $cmds="#"; }
$cmds
# 
annulusEigenfunctionInitialCondition
 if( $bcn eq "d" ){ $eigopt =0; }else{ $eigopt=1; }
 $eigopt $modem $moden $p0 $p1
# 
# annulusEigenfunctionKnownSolution
# 
OBTZ:$tz
OBTZ:twilight zone flow 0 
OBTZ:degree in space $degreex
OBTZ:degree in time $degreet
OBTZ:frequencies (x,y,z,t) $fx $fy $fz $ft
# error norm: 
$errorNorm
# 
final time $tFinal
times to plot $tPlot
dissipation $diss
order of dissipation $dissOrder
cfl $cfl
use conservative difference $cons
SMPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad
SMPDE:fourth-order artificial diffusion $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 
#
if( $bcn eq "d" ){ $bcmd = "all=displacementBC"; }else{ $bcmd = "*"; };
if( $bcn eq "sf" ){\
 $bcmd  = "annulus(0,1)=tractionBC, userDefinedBoundaryData\n pressure force\n  $p0 \n  done\n"; \
 $bcmd .= "annulus(1,1)=tractionBC, userDefinedBoundaryData\n pressure force\n  $p1 \n done";}
if( $grid =~ "annulusSplit" && $bcn eq "sf" ){\
 $bcmd  = "left(0,1)=tractionBC, userDefinedBoundaryData\n pressure force\n  $p0 \n  done\n"; \
 $bcmd .= "left(1,1)=tractionBC, userDefinedBoundaryData\n pressure force\n  $p1 \n done\n"; \
 $bcmd .= "right(0,1)=tractionBC, userDefinedBoundaryData\n pressure force\n  $p0 \n  done\n"; \
 $bcmd .= "right(1,1)=tractionBC, userDefinedBoundaryData\n pressure force\n  $p1 \n done";}
boundary conditions
  $bcmd
  # all=tractionBC
  # all=displacementBC
done
#
displacement scale factor $dsf
debug $debug
check errors 1
plot errors 1
# plot vorticity 1 
# plot divergence 1 
# For displacement solvers plot velocity and stress: 
if( $pv eq "non-conservative" || $pv eq "conservative" ){ $plotCommands = "plot velocity 1\n plot stress 1"; }else{ $plotCommands="*"; }
$plotCommands
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
$go


  erase
  displacement
  exit this menu

