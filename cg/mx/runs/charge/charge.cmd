#================================================================================================
#  cgmx example: Moving Gaussian charge source
#
# Usage:
#   
#  cgmx [-noplot] charge -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -debug=<num>  ...
#                     -xp0=<> -xp1=<> -xp2=<> -vp0=<> -vp1=<> -vp2=<> -projectFreq=<>
#                     -rbc=[abcEM2|rbcNonLocal|abcPML|perfectElectricalConductor|symmetry] ...
#                     -pmlWidth=<> -pmlStrength=<> -pmlPower=<> -go=[run/halt/og]
#
# (xp0,xp1,xp2): initial location of the pulse 
# (vp0,vp1,vp2): velocity of the pulse
# -projectFreq  : project fields every this many steps 
# -width : width of the pulse 
# -pmlWidth, -pmlStrength, -pmlPower: The pml damping function sigma(s) = (pmlStrength)*(s)^(pmlPower) where 0 <= s <= 1
# 
#  Examples: 
#  -- square: 
#   ogen -noplot squareArg -order=4 -nx=128
#   cgmx charge -g=square128.order2.hdf -xp0=.25 -yp0=.5 -width=.025 -go=halt 
#   ogen -noplot squareArg -order=4 -nx=128
#   cgmx charge -g=square128.order4.hdf -xp0=.25 -yp0=.5 -width=.025 -go=halt 
#
$tFinal=.5; $tPlot=.1; $project=1;  $cfl=.95; $diss=.5; $projectFreq=1; $debug=1; 
$width=.05; $amp=10.; $beta=1./$width; $p=2.; $rtol=1.e-5;
#
$solver="yale"; $best="choose best iterative solver";
$tzFreq=" 1. 1. 1. 1."; 
$xp0=.25; $xp1=.5; $xp2=0.; # initial position of the pulse
$vp0=1.; $vp1=.0; $vp2=0.;  # velocity of the pulse 
# 
$grid="square64.order2.hdf"; $ic="gs"; $ks="none";
$cons=0; $go="halt"; $rbc="perfectElectricalConductor"; 
$pmlWidth=11; $pmlStrength=50.; $pmlPower=4.; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"go=s"=>\$go,"noplot=s"=>\$noplot,"ic=s"=>\$ic,"bc=s"=>\$bc,\
  "dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"width=f"=>\$width,"projectFreq=i"=>\$projectFreq,\
  "xp0=f"=>\$xp0,"xp1=f"=>\$xp1,"xp2=f"=>\$xp2,"vp0=f"=>\$vp0,"vp1=f"=>\$vp1,"vp2=f"=>\$vp2,\
  "ks=s"=>\$ks,"rbc=s"=>\$rbc,"pmlWidth=f"=>\$pmlWidth,"pmlStrength=f"=>\$pmlStrength,"pmlPower=f"=>\$pmlPower );
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
$grid
# ************ Old: 
# $grid="square8"; $tPlot=.1;
# $grid="square16"; $tPlot=.1;
# $grid="nonSquare20"; $tPlot=.05;
# $grid="square64.order2"; $tPlot=.1; $solver="yale"; $project=1; $tFinal=.1; $rtol=1.e-2;
# $grid="square128"; $tFinal=.5; $tPlot=.1; 
# $grid="square256"; $tFinal=.5; $tPlot=.1; 
# $grid="square64.order4"; $tPlot=.05; 
# $grid="square128.order4"; $tPlot=.05; 
# $grid="nonSquare128.order4"; $tPlot=.05; 
# $grid="bigSquare100"; $tPlot=.05; $xp0=.25; $xp1=1.; $xp2=0.; 
# $grid="nonSquare128"; $tPlot=.05; 
# annulus3.hdf
# $grid="cic3.hdf"; $tzFreq=" .5 .5 .5 1."; 
# $grid="quarterAnnulus2.hdf"; $tzFreq=" .5 .5 .5 1."; $project=0; 
# $grid="annulus1.hdf"; $tzFreq=" .5 .5 .5 1."; $project=1;  $tPlot=.01; 
# cic2.order4.hdf
# $grid="sis4.hdf"; $tPlot=.01; $xp0=-.5; $xp1=.0; $xp2=0.
# $grid="sis2.hdf"; $tPlot=.01; $xp0=-.5; $xp1=.0; $xp2=0.
# sis2.order4.hdf
# $grid="box32"; $tPlot=.05; $width=.1; $xp2=.5; $solver=$best;
# $grid="box64"; $tPlot=.05; $width=.1; $xp2=.5; $solver=$best;
# $grid="box32.order4"; $tPlot=.05; $width=.1; $xp2=.5; $rtol=1.e-5; $solver=$best; 
# $grid="box64.order4"; $tPlot=.05; $width=.1; $xp2=.5; $rtol=1.e-5; $solver=$best; 
# $grid="box96.order4"; $tPlot=.05; $width=.1; $xp2=.5; $rtol=1.e-5; $solver=$best; 
# $grid="box128.order4"; $tPlot=.05; $width=.1; $xp2=.5; $rtol=1.e-5; $solver=$best; 
# $grid="sib2.hdf"; $tPlot=.1; $tzFreq=" .5 .5 .5 1."; 
#
NFDTD
# 
gaussianChargeSource
Gaussian charge source: $amp $beta $p $xp0 $xp1 $xp2  $vp0 $vp1 $vp2 (amp,beta,p,x0,y0,z0,v0,v1,v2)
# 
bc: all=perfectElectricalConductor
#**
#* twilightZone
TZ omega: $tzFreq
trigonometric
# degreeSpace, degreeTime 2 2
# degreeSpace, degreeTime 1 1
#**
#
tFinal $tFinal
tPlot  $tPlot
dissipation $diss
cfl $cfl
# 
debug $debug
project initial conditions $project
project fields $project
projection frequency $projectFreq
#
check errors 0
plot errors 0
plot rho 1
use charge density 1
# 
use conservative divergence 1
# 
projection solver parameters...
   $solver
 # PETSc
 # these tolerances are chosen for PETSc
 # this is a less expensive PC:
 # sor preconditioner
   relative tolerance
     $rtol
   absolute tolerance
     $atol=$rtol; 
     $atol
  exit
continue

continue


movie mode
finish

