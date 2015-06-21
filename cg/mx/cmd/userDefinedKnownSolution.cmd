#================================================================================================
#  cgmx example: Test a user defined known solution
#
# Usage:
#   
#  cgmx [-noplot] userDefinedKnownSolution -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -debug=<num> -cons=[0/1] ...
#                     -rbc=[abcEM2|rbcNonLocal|abcPML|perfectElectricalConductor|symmetry|dirichlet] ...
#                     -pmlWidth=<> -pmlStrength=<> -pmlPower=<> -go=[run/halt/og]
#
# -pmlWidth, -pmlStrength, -pmlPower: The pml damping function sigma(s) = (pmlStrength)*(s)^(pmlPower) where 0 <= s <= 1
# 
#  Examples: 
# -- square: 
#   cgmx userDefinedKnownSolution -g=square64.order4 -beta=40 -rbc=abcPML -pmlWidth=21 -pmlStrength=50. -diss=1. -tf=100 -tp=.1 -go=halt
#
$tFinal=10.; $tPlot=.1; $diss=.1; $cfl=.9;  $kx=1; $ky=0; $kz=0.; $plotIntensity=0; 
$x0=.5; $y0=.5; $z0=.5; $beta=40.; 
$grid="sib1.order4.hdf"; $ic="gs"; $ks="none";
$cons=0; $go="halt"; $rbc="abcEM2"; $bcn="debug $debug"; 
$pmlWidth=11;  $pmlStrength=50.; $pmlPower=4.; 
$useNewForcingMethod=0; 
$cons=1; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"ic=s"=>\$ic,"bc=s"=>\$bc,\
  "dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"kx=i"=>\$kx,"ky=i"=>\$ky,"kz=i"=>\$kz,"plotIntensity=i"=>\$plotIntensity,\
  "ks=s"=>\$ks,"rbc=s"=>\$rbc,"pmlWidth=f"=>\$pmlWidth,"pmlStrength=f"=>\$pmlStrength,"pmlPower=f"=>\$pmlPower,\
  "x0=f"=>\$x0,"y0=f"=>\$y0,"z0=f"=>\$z0,"beta=f"=>\$beta,"cons=i"=>\$cons,\
  "useNewForcingMethod=i"=>\$useNewForcingMethod );
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
$grid
# 
NFDTD
#
user defined known solution
  manufactured pulse
     # amp beta x0 y0 z0 cx cy cz 
     1. $beta $x0 $y0 $z0 1. 1. 1.
done
# 
userDefinedKnownSolutionInitialCondition
#  
userDefinedForcing
  manufactured pulse
exit
#
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
#
use new forcing method $useNewForcingMethod
# use conservative divergence $cons 
use conservative difference $cons
#*********************************
show file options...
  MXSF:compressed
  MXSF:open
    $show
 # MXSF:frequency to save 
  MXSF:frequency to flush 10
exit
#**********************************
## plot intensity $plotIntensity
# -- do this for now:
# check errors 0
# plot errors 0
# 
continue
# plot:Hz
$go

