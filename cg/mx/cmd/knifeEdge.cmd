#================================================================================================
#  cgmx example:  plane wave past a knife edge
#
# Usage:
#   
#  cgmx [-noplot] knifeEdge  -g=<name> -tf=<tFinal> -tp=<tPlot> -kx=<num> -ky=<num> -kz=<num> -show=<name> ...
#                            -plotIntensity=[0|1] -diss=<> -debug=<num> -cons=[0/1] -go=[run/halt/og]
# Arguments:
#  -kx= -ky= -kz= : integer wave numbers of the incident wave
#
# Examples: 
#   cgmx knifeEdge -g=knifeSlit2.order4 -kx=8 -tp=.05 -tf=2. -plotIntensity=1 -go=halt 
# 
#================================================================================================
# 
$tFinal=2.; $tPlot=.1;  $show=" ";
$cfl = .9; $diss=.5; $debug=0; $divDamping=.0; $plotIntensity=0;
$kx=2; $ky=0; $kz=0; 
$ax=0.; $ay=0.; $az=0.; # plane wave coeffs. all zero -> use default
$omega=-1.; # time harmonic omega for intensity computations 
$eps=1.; $mu=1.;  
$show=" "; 
$grid="knifeSlit2.order4.hdf";
$cons=0; $go="halt"; 
$xa=-100.; $xb=-.5; $ya=-100.; $yb=100.; $za=-100.; $zb=100.;  # initial condition bounding box
# 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
    "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,\
    "divDamping=f"=>\$divDamping,"plotIntensity=i"=>\$plotIntensity,"intensityOption=i"=>\$intensityOption,\
    "interit=i"=>\$interfaceIterations,"cyl=i"=>\$cyl,"useNewInterface=i"=>\$useNewInterface,\
    "dtMax=f"=>\$dtMax,"kx=i"=>\$kx,"ky=i"=>\$ky,"kz=i"=>\$kz,"eps1=f"=>\$eps1,"eps2=f"=>\$eps2, "cons=i"=>\$cons );
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
# 
#
$grid
*
*   mx cmd/knifeEdge.cmd
*
* $tFinal=2.; $tPrint=.05; 
* $kx=1; $ky=0; $kz=0; 
*
* $gridName="knifeEdge2.order4.hdf"; $kx=8; $show="knifeEdge2-k8.show";
* $gridName="knifeEdge4.order4.hdf"; $kx=16; $show="knifeEdge4-k16.show";
* $gridName="knifeEdge8.order4.hdf"; $kx=64; $show="knifeEdge8-k64.show";
*
* The knifeSlit has a narrow gap at the top
* $gridName="knifeSlit2.order4.hdf"; $kx=8; $tFinal=3.; $show="knifeSlit-k8.show";
* $gridName="knifeSlit4.order4.hdf"; $kx=16; $show="knifeSlit4-k16.show";
*
* $gridName
*
NFDTD
planeWaveInitialCondition
$kxa= abs($kx);
$xb = $xa + int( ($xb-$xa)*$kxa +.5 )/$kxa;   # we need to clip the plane wave on a period
initial condition bounding box $xa $xb $ya $yb $za $zb
$beta=10.; # exponent in tanh function for smooth transition to zero outside the bounding box
bounding box decay exponent $beta
plane wave coefficients $ax $ay $az $eps $mu
*
*
kx,ky,kz $kx $ky $kz
*
bc: all=symmetry
bc: backGround(0,0)=abcEM2
bc: backGround(1,0)=abcEM2
bc: knife(0,1)=perfectElectricalConductor
# bc: backGround=perfectElectricalConductor
* bc: backGround=abcEM2
* bc: square=abcPML
* pml width,strength,power 25 30. 4
* pml width,strength,power 35 30. 4
* pml width,strength,power 45 30. 4
* pml width,strength,power 55 30. 4
* =====================================
* radius for checking errors 2.
* =====================================
* bc: square=dirichlet
*******
# bc: backGround(0,0)=planeWaveBoundaryCondition
*bc: square(1,0)=planeWaveBoundaryCondition
* bc: Annulus=perfectElectricalConductor
# bc: backGround(1,0)=abcEM2
* 
* -- we need to subtract out the incident field on the "inflow" boundary before
*    applying the radiation boundary condition: 
adjust boundaries for incident field 0 all
adjust boundaries for incident field 1 backGround
* 
tFinal $tFinal
tPlot  $tPlot
*
dissipation $diss
************************
cfl $cfl
*
plot scattered field 0
* plot errors 1
check errors 0
plot intensity $plotIntensity
# $omega= sqrt( $kx*$kx + $ky*$ky + $kz*$kz );
# time harmonic omega $omega (omega/(2pi), normally c*|k|^2
*
**********************************
show file options...
  MXSF:compressed
  MXSF:open
    $show
  * MXSF:frequency to save 
  MXSF:frequency to flush 5
exit
***********************************
continue
plot:Ey
contour
plot contour lines (toggle)
exit
*
$go



movie mode
finish
