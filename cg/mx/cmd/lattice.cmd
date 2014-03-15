#================================================================================================
#  cgmx example:  scattering from an array of dielectric disks (e.g. a photonic band gap simulation)
#
# Usage:
#   
#  cgmx [-noplot] lattice  -g=<name> -tf=<tFinal> -tp=<tPlot> -kx=<num> -ky=<num> -kz=<num> ...
#                           -plotIntensity=[0|1] -diss=<> -debug=<num> -cons=[0/1] -go=[run/halt/og]
# Arguments:
#  -kx= -ky= -kz= : integer wave number of the incident wave
#  -xb = right most position of the plane wave initial condition
#
# Examples:
#   cgmx lattice -g=lattice2x2yFactor2.order4 -eps1=.25 -eps2=1. -kx=2 -plotIntensity=1 -xb=-1.5 -go=halt
#
#   cgmx lattice -g=lattice3x3yFactor2.order4 -eps1=.25 -eps2=1. -kx=2 -plotIntensity=1 -xb=-2. -go=halt
#   cgmx lattice -g=lattice3x3yFactor2.order4 -eps1=2.25 -eps2=1. -kx=2 -plotIntensity=1 -xb=-2. -go=halt
#
#   cgmx lattice -g=lattice3x3yFactor4.order4 -eps1=.25 -eps2=1. -kx=4 -plotIntensity=1 -xb=-2. -go=halt -show=lattice33.show
#
# parallel: 
#
#================================================================================================
# 
$tFinal=10.; $tPlot=.1; $diss=.5; $cfl=.8; $plotIntensity=0; 
$kx=1; $ky=0; $kz=0;
$eps1=1.; $mu1=1.; # inner
$eps2=1.; $mu2=1.; # outer 
$show=" "; $backGround="backGround";
$interfaceIterations=3;
$grid="lattice2.order4.hdf";
$cons=0; $go="halt"; 
$ax=0.; $ay=0.; $az=0.; # plane wave coeffs. all zero -> use default
$xa=-100.; $xb=-.5; $ya=-100.; $yb=100.; $za=-100.; $zb=100.;  # initial condition bounding box
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"plotIntensity=i"=>\$plotIntensity,\
  "dtMax=f"=>\$dtMax,"kx=i"=>\$kx,"ky=i"=>\$ky,"kz=i"=>\$kz,"eps1=f"=>\$eps1,"eps2=f"=>\$eps2, "cons=i"=>\$cons,\
   "xa=f"=>\$xa,"xb=f"=>\$xb );
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# 
# $gridName = "lattice.hdf"; $eps1=.5; $interfaceIterations=1; $tPlot=.01;
# $gridName = "lattice2.hdf"; $eps1=1.;
# $gridName = "lattice2.hdf"; $eps1=.5; $interfaceIterations=1; $tPlot=.01;
# $gridName = "lattice2.hdf"; $eps1=.5; $interfaceIterations=1;
#
# $gridName = "lattice2.order4.hdf"; $eps1=1.; $interfaceIterations=1;
# $gridName = "lattice2.order4.hdf"; $eps1=.25; $interfaceIterations=1;
# $gridName = "lattice4.order4.hdf"; $eps1=.25; $interfaceIterations=1;
# $gridName = "innerOuter2.order4.hdf"; $eps1=.25; $cfl=.8; $diss=.5; $tFinal=1.; $interfaceIterations=1; $backGround="outerSquare";
# $gridName = "innerOuter8.order4.hdf"; $eps1=.25; $cfl=.8; $diss=.5; $tFinal=1.; $interfaceIterations=1;
#
$grid
#
NFDTD
planeWaveInitialCondition
$kxa= abs($kx);
$xb = $xa + int( ($xb-$xa)*$kxa +.5 )/$kxa;   # we need to clip the plane wave on a period
initial condition bounding box $xa $xb $ya $yb $za $zb
$beta=20.; # exponent in tanh function for smooth transition to zero outside the bounding box
bounding box decay exponent $beta
plane wave coefficients $ax $ay $az $eps2 $mu2
# ++ zeroInitialCondition
# ====
# planeWaveScatteredFieldInitialCondition
# ====
# twilightZone
#  degreeSpace, degreeTime  1 1
#
kx,ky,kz $kx $ky $kz
#
# NOTE: we set the left and right boundaries of the background grid to be far-field
#       and ALSO adjust boundaries for incident field (below) -- this is like setting
#       a planeWaveBoundaryCondition + abcEM2 on the same boundary
bc: $backGround(0,0)=abcEM2
# bc: $backGround(0,0)=planeWaveBoundaryCondition
bc: $backGround(1,0)=abcEM2
bc: $backGround(0,1)=abcEM2
bc: $backGround(1,1)=abcEM2
# bc: $backGround(1,0)=perfectElectricalConductor
# 
#      innerAnnulus
#      innerSquare
#      outerAnnulus
#      outerSquare
# NOTE: material interfaces have share>=100
#* coefficients $eps1 1. all (eps,mu,grid-name)
coefficients $eps1 1. innerAnnulus* (eps,mu,grid-name)
coefficients $eps1 1. innerSquare* (eps,mu,grid-name)
coefficients $eps2 1. outerAnnulus* (eps,mu,grid-name)
coefficients $eps2 1. outerSquare* (eps,mu,grid-name)
#
interface BC iterations $interfaceIterations
#
# -- we need to subtract out the incident field on the "inflow" boundary before
#    applying the radiation boundary condition: 
adjust boundaries for incident field 0 all
adjust boundaries for incident field 1 backGround
# bc: Annulus=perfectElectricalConductor
tFinal $tFinal
tPlot  $tPlot
#
dissipation $diss
#*********************************
show file options...
  MXSF:compressed
  MXSF:open
    $show
 # MXSF:frequency to save 
  MXSF:frequency to flush 2
exit
#**********************************
#
# use conservative difference 0
debug 0
#
cfl $cfl 
plot errors 0
check errors 0
plot intensity $plotIntensity
continue
#
plot:Ey
$go
