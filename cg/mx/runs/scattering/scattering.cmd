#================================================================================================
#  cgmx example:  Scattering of a plane wave from different bodies
#
# Usage:
#   
#  cgmx [-noplot] scattering -g=<name> -tf=<tFinal> -tp=<tPlot> -boundaryForing=[0|1]
#                      -kx=<num> -ky=<num> -kz=<num> -diss=<> ...
#                      -bg=<back-ground> -rbc=[abcEM2|abcPML] -debug=<num> -cons=[0/1] ...
#                      -plotIntensity=[0|1] -go=[run/halt/og]
# Arguments:
#  -boundaryForcing=1 : perform scattering directly by subtracting out the plane wave solution; this results
#                       in a inhomogenoeous PEC boundary condition.
#  -boundaryForcing=0 :  incident wave front hits the body: 
#  -kx= -ky= -kz= : integer wave numbers of the incident wave
#  -bg= : name of the background grid
# 
# Examples: 
#   cgmx scattering -g=cice3.order4.hdf -bg=square
#   cgmx scattering -g=trianglee4.order4.hdf -bg=backGround           (scattering from a rounded triangle)
#   cgmx scattering -g=trianglee8.order4.hdf -bg=backGround -plotIntensity=1
#   cgmx scattering -g=crve4.order4.hdf -bg=backGround                (scattering from a space reentry vehicle )
#
# -- PML with boundaryForcing
#    cgmx scattering -g=cice4.order4.hdf  -bg=square -rbc=abcPML -boundaryForcing=1
# 
# parallel: 
#   mpirun -np 2 $cgmxp scattering -g=trianglee8.order4.hdf -bg=backGround
#
#================================================================================================
# 
$tFinal=10.; $tPlot=.1; $diss=1.; $cfl=.9; $plotIntensity=0; $boundaryForcing=0; 
$kx=1; $ky=0; $kz=0;
$grid="sib1.order4.hdf"; $backGround="square"; 
$cons=0; $go="halt"; 
$ax=0.; $ay=0.; $az=0.; # plane wave coeffs. all zero -> use default
$eps=1.; $mu=1.;
$xa=-100.; $xb=-1.; $ya=-100.; $yb=100.; $za=-100.; $zb=100.;  # initial condition bounding box
$rbc="abcEM2"; # radiation BC 
$pmlLines=11; $pmlPower=6; $pmlStrength=50.; # pml parameters
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,\
  "dtMax=f"=>\$dtMax,"kx=i"=>\$kx,"ky=i"=>\$ky,"kz=i"=>\$kz,"plotIntensity=i"=>\$plotIntensity, "cons=i"=>\$cons,\
  "rbc=s"=>\$rbc,"pmlLines=i"=>\$pmlLines,"pmlPower=i"=>\$pmlPower,"pmlStrength=f"=>\$pmlStrength,\
 "xb=f"=>\$xb,"boundaryForcing=i"=>\$boundaryForcing  );
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
#
$grid
#
NFDTD
#  boundaryForcing=1 :  Adjust PEC boundaries to account for a plane wave being subtracted out
if( $boundaryForcing eq 0 ){ $cmd="planeWaveInitialCondition"; }else{ $cmd="planeWaveBoundaryForcing\n zeroInitialCondition"; }
$cmd
$kxa= abs($kx);
$xb = $xa + int( ($xb-$xa)*$kxa +.5 )/$kxa;   # we need to clip the plane wave on a period
if( $boundaryForcing eq 0 ){ $cmd="initial condition bounding box $xa $xb $ya $yb $za $zb"; }else{ $cmd="#"; }
$cmd
$beta=10.; # exponent in tanh function for smooth transition to zero outside the bounding box
bounding box decay exponent $beta
# 
plane wave coefficients $ax $ay $az $eps $mu
# ====
#  Initial condition:
## zeroInitialCondition
# ====
#
kx,ky,kz $kx $ky $kz 
#
# -- boundary conditions:
#
bc: all=perfectElectricalConductor
bc: $backGround=$rbc
if( $boundaryForcing eq 0 ){ $cmd="bc: $backGround(0,0)=planeWaveBoundaryCondition"; }else{ $cmd="#"; }
$cmd 
# bc: $backGround(0,1)=symmetry
# bc: $backGround(1,1)=symmetry
#
# -- pml parameters:
pml width,strength,power $pmlLines $pmlStrength $pmlPower
#
tFinal $tFinal
tPlot  $tPlot
# 
# -- we need to subtract out the incident field on the "inflow" boundary before
#    applying the radiation boundary condition: 
if( $boundaryForcing eq 1 ){ $adjustFields=0; }else{ $adjustFields=1; }
adjust boundaries for incident field 0 all
adjust boundaries for incident field $adjustFields $backGround
# 
dissipation  $diss
#***********************
#* slow start interval 1.
slow start interval -1.
#***********************
# divergence damping  0.0025   .02  1. .005 .001 .0025 .01
#***********************
cfl $cfl
#
use conservative divergence $cons 
# plot scattered field 1
if( $boundaryForcing eq 1 ){ $plotTotalField=1; }else{ $plotTotalField=0; }
plot total field $plotTotalField
plot errors 0
check errors 0
plot intensity $plotIntensity
#*********************************
show file options...
  MXSF:compressed
  MXSF:open
    $show
  MXSF:frequency to flush 2
exit
#**********************************
# $omega= sqrt( $kx*$kx + $ky*$ky + $kz*$kz );
# time harmonic omega $omega (omega/(2pi), normally c*|k|^2
#
continue
plot:Ey
contour
plot contour lines (toggle)
exit
$go 

