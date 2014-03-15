#================================================================================================
#  cgmx example: Test user defined initial conditions
#
# Usage:
#   
#  cgmx [-noplot] userDefinedInitialConditions -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -debug=<num> -cons=[0/1] ...
#                     -rbc=[abcEM2|rbcNonLocal|abcPML|perfectElectricalConductor|symmetry] ...
#                     -pmlWidth=<> -pmlStrength=<> -pmlPower=<> -go=[run/halt/og]
#
# -pmlWidth, -pmlStrength, -pmlPower: The pml damping function sigma(s) = (pmlStrength)*(s)^(pmlPower) where 0 <= s <= 1
# 
#  Examples: 
# -- square: 
#   ogen -noplot squareArg -order=4 -nx=128
#   cgmx userDefinedInitialConditions -g=square128.order4.hdf -rbc=abcEM2 -go=halt 
#   cgmx userDefinedInitialConditions -g=square128.order4.hdf -rbc=perfectElectricalConductor -go=halt
#   cgmx userDefinedInitialConditions -g=square128.order4.hdf -rbc=abcPML -pmlWidth=21 -pmlStrength=50. -go=halt 
# 
# 
# -- bigger square:
#   ogen -noplot bigSquare -order=4 -factor=8 -xa=-2. -xb=2. -ya=-2. -yb=2. -name="bigSquareX2Y2f8.order4.hdf"
#   cgmx userDefinedInitialConditions -g=bigSquareX2Y2f8.order4 -x1=-.5 -y1=-.5 -x2=.5 -y2=.5 -rbc=abcEM2 -go=halt
# 
# -- 3D box:
#   ogen -noplot boxArg -order=4 -xa=-1. -xb=1. -ya=-1. -yb=1. -za=-1. -zb=1. -factor=4 -name="boxLx2Ly2Lz2Factor4.order4.hdf"
#   cgmx userDefinedInitialConditions -g=boxLx2Ly2Lz2Factor4.order4.hdf -rbc=abcEM2 -x1=-.3 -y1=-.3 -z1=0 -x2=.3 -y2=.3 -z2=0 -go=halt    
#   cgmx userDefinedInitialConditions -g=boxLx2Ly2Lz2Factor4.order4.hdf -rbc=abcPML -x1=-.3 -y1=-.3 -z1=0 -x2=.3 -y2=.3 -z2=0 -pmlWidth=11 -go=halt 
#   cgmx userDefinedInitialConditions -g=boxLx2Ly2Lz2Factor4.order4.hdf -rbc=perfectElectricalConductor -x1=-.3 -y1=-.3 -z1=0 -x2=.3 -y2=.3 -go=halt
#
$tFinal=10.; $tPlot=.1; $diss=.1; $cfl=.9;  $kx=1; $ky=0; $kz=0.; $plotIntensity=0; 
$x1=.3; $y1=.3; $z1=0.;
$x2=.7; $y2=.7; $z2=0.;
$grid="sib1.order4.hdf"; $ic="gs"; $ks="none";
$cons=0; $go="halt"; $rbc="abcEM2"; $bcn="debug $debug"; 
$pmlWidth=11; 
$pmlStrength=50.;
$pmlPower=4.; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"ic=s"=>\$ic,"bc=s"=>\$bc,\
  "dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"kx=i"=>\$kx,"ky=i"=>\$ky,"kz=i"=>\$kz,"plotIntensity=i"=>\$plotIntensity,\
  "ks=s"=>\$ks,"rbc=s"=>\$rbc,"pmlWidth=f"=>\$pmlWidth,"pmlStrength=f"=>\$pmlStrength,"pmlPower=f"=>\$pmlPower,\
  "x1=f"=>\$x1,"y1=f"=>\$y1,"z1=f"=>\$z1,"x2=f"=>\$x2,"y2=f"=>\$y2,"z2=f"=>\$z2 );
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
$grid
#
# -- user defined initial conditions
userDefinedInitialConditions
gaussian pulses
  2
  # g(x,y,z,t) = a*cos(2*pi*omega*(t-t0) )*exp( -beta*[ (x-x0)^2 + (y-y0)^2 + (z-z0)^2 ]^p )
  # a beta omega p x0 y0 z0 t0
  10. 200. 4. 1. $x1 $y1 $z1 0.
  10. 200. 2. 1. $x2 $y2 $z2 0.
exit
NFDTD
#
# All boundaries get the far field BC: 
bc: all=$rbc
#  sigma(s) = (pmlStrength)*(s)^(pmlPower)
pml width,strength,power $pmlWidth $pmlStrength $pmlPower
# 
#
tFinal $tFinal
tPlot $tPlot
dissipation $diss
# order of dissipation 4
cfl $cfl
#*********************************
show file options...
  MXSF:compressed
  MXSF:open
    $show
 # MXSF:frequency to save 
  MXSF:frequency to flush 10
exit
#**********************************
plot intensity $plotIntensity
continue
# plot:Hz
$go

