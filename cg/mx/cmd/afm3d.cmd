#================================================================================================
#  cgmx example:  scattering from 3d AFM curved interface between glass and a vacuum
#
# Usage:
#   
#  cgmx [-noplot] afm3d  -g=<name> -tf=<tFinal> -tp=<tPlot> -kx=<num> -ky=<num> -kz=<num> ...
#                          -diss=<> -debug=<num> -cons=[0/1] -filter=[0|1] -varDiss=<0|1> ...
#                           -plotHarmonicComponents=[0|1] -go=[run/halt/og]
# Arguments:
#  -kx= -ky= -kz= : integer wave number of the incident wave
#  -varDiss :  if 1, use variable dissipation (only add dissipation near interpolation points)
#  -filter=1 : add the high order filter
#  -zb : upper limit of initial condition for the plane wave.
#  -plotHarmonicComponents : plot the harmonic components of the E field
#  -dissc : artificial dissipation on curvlinear grids (by default equal to -diss)
# 
# Notes: 
# refractive index from vacuum to glass is 1.5
#       $epsGlass = 1.5^2 = 2.25
# 
#   pts/wave-length = (1/ds)*(1/kz) = factor*20/kz  (ds=.05)
# 
# choose -ax=sqrt(2/3) -ay=sqrt(2/3) so I=1. in glass sqrt(2/3)=.81649658
# 
# Examples:
#
#   cgmx afm3d -g=interfaceBump3dafm3d2i4.order2 -eps1=2.25 -eps2=1. -kz=5 -ax=1. -zb=-.01 -diss=2. -plotIntensity=1 -go=halt
#   cgmx afm3d -g=interfaceBump3dafm3d2e8.order2 -eps1=2.25 -eps2=1. -kz=20 -ax=1. -zb=-.01 -diss=2. -plotIntensity=1 -go=halt
#   -- order 4: (may have to run at lower cfl with current implementation of fourth order interface)
#   cgmx afm3d -g=interfaceBump3dafm3d2i4.order4 -eps1=2.25 -eps2=1. -kz=5 -ax=.81649658 -ay=.81649658 -zb=-.02 -diss=1. -filter=1 -tf=2. -tp=.01 -plotIntensity=1
#
#   srun -N1 -n2 -ppdebug $cgmxp -g=interfaceBump3dafm3d2e8.order2 -eps1=2.25 -eps2=1. -kz=20 -ax=1. -zb=-.01 -diss=2. -plotIntensity=1 -go=halt
#   nohup $cgmx noplot afm3d -g=interfaceBump3dafm3d2i4.order2 -eps1=2.25 -eps2=1. -kz=5 -ax=1. -zb=-.01 -diss=2. -plotIntensity=1 -tp=.1 -tf=1.5 -show="afm3d4.show" -go=go > afm4.out &
#
# -- flat interface:  I=1. in glass: -ax=sqrt(4/3) = 1.1547
#  cgmx afm3d -g=interfaceBump3dFlate2.order2.hdf -eps1=2.25 -eps2=1. -kz=5 -ax=.81649658 -ay=.81649658 -zb=-.01 -diss=2. -plotIntensity=1 -go=halt
#  cgmx afm3d -g=interfaceBump3dFlate2.order2.hdf -eps1=2.25 -eps2=1. -kz=5 -ax=1.1547 -zb=-.01 -diss=2. -plotIntensity=1 -go=halt
#  cgmx afm3d -g=interfaceBump3dFlate8.order2.hdf -eps1=1. -eps2=1. -kz=5 -ax=1.414 -zb=-.01 -diss=2. -plotIntensity=1 -go=halt
#================================================================================================ 
# 
$tFinal=.6; $tPlot=.1; $diss=2.; $dissc=-1; $cfl=.9; $varDiss=0; $varDissSmooths=20; $plotIntensity=0;
$kx=0; $ky=0; $kz=0; 
$ax=0.; $ay=0.; $az=0.; # plane wave coeffs. all zero -> use default
$eps1=1.; $mu1=1.; # lower
$eps2=1.; $mu2=1.; # upper
$filter=0; $filterOrder=4; $filterFrequency=1; 
$show=" "; $backGround="backGround";
$interfaceIterations=3;
$grid="interfaceBump3dafm3d2i4.order2";
$cons=0; $go="halt";  $plotHarmonicComponents=0; 
$xa=-100.; $xb=100.; $ya=-100.; $yb=100.; $za=-100.; $zb=100.;  # initial condition bounding box
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
  "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"filter=i"=>\$filter,\
  "dtMax=f"=>\$dtMax,"kx=f"=>\$kx,"ky=f"=>\$ky,"kz=f"=>\$kz,"ax=f"=>\$ax,"ay=f"=>\$ay,"az=f"=>\$az,\
  "eps1=f"=>\$eps1,"eps2=f"=>\$eps2, "cons=i"=>\$cons,"yb=f"=>\$yb,"zb=f"=>\$zb,"plotIntensity=i"=>\$plotIntensity,\
  "ii=i"=>\$interfaceIterations,"varDiss=i"=>\$varDiss ,"varDissSmooths=i"=>\$varDissSmooths,"yb=f"=>\$yb,\
  "filterOrder=i"=>\$filterOrder,"$filterFrequency=i"=>\$filterFrequency,\
  "plotHarmonicComponents=i"=>\$plotHarmonicComponents,"dissc=f"=>\$dissc );
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $dissc < 0 ){ $dissc=$diss; }
#
# 
$grid
#
NFDTD
planeWaveInitialCondition
# we need to clip the plane wave on a period to avoid oscillations at the front
if( $ky ne 0 ){ $kya= abs($ky); $yb = int( $yb*$kya -.9999 )/$kya; if( $ky < 0 ){ $ya=$yb; $yb=1e9; } }
if( $kz ne 0 ){ $kza= abs($kz); $zb = int( $zb*$kza -.9999 )/$kza; if( $kz < 0 ){ $za=$zb; $zb=1e9; } }
initial condition bounding box $xa $xb $ya $yb $za $zb
if( $kz > 0 ){ $epsPW=$eps1; $muPW=$mu1; }else{ $epsPW=$eps2; $muPW=$mu2; }
plane wave coefficients $ax $ay $az $epsPW $muPW
# zeroInitialCondition
# ====
# planeWaveScatteredFieldInitialCondition
# ====
# twilightZone
#  degreeSpace, degreeTime  1 1
#
kx,ky,kz $kx $ky $kz
#
# -- top boundary is an absorbing BC: 
bc: all=abcEM2
if( $kz > 0 ){ $inbc = "lower(0,2)"; }else{ $inbc = "upper(1,2)"; }
# no: bc: $inbc=planeWaveBoundaryCondition
# 
$sideBC="symmetry";
# $sideBC="abcEM2";
# $sideBC="dirichlet";
$sideBCd="dirichlet";
bc: upper(0,0)=$sideBC
bc: upper(1,0)=$sideBC
bc: upper(0,1)=$sideBC
bc: upper(1,1)=$sideBC
# 
bc: upperInterface(0,0)=$sideBC
bc: upperInterface(1,0)=$sideBC
bc: upperInterface(0,1)=$sideBC
bc: upperInterface(1,1)=$sideBC
# 
bc: lower(0,0)=$sideBC
bc: lower(1,0)=$sideBC
bc: lower(0,1)=$sideBC
bc: lower(1,1)=$sideBC
# 
bc: lowerInterface(0,0)=$sideBC
bc: lowerInterface(1,0)=$sideBC
bc: lowerInterface(0,1)=$sideBC
bc: lowerInterface(1,1)=$sideBC
# 
# -- we need to subtract out the incident field on the "inflow" boundary before
#    applying the radiation boundary condition: 
adjust boundaries for incident field 0 all
adjust boundaries for incident field 1 lower
# 
#      innerAnnulus
#      innerSquare
#      outerAnnulus
#      outerSquare
# NOTE: material interfaces have share>=100
#* coefficients $eps1 1. all (eps,mu,grid-name)
coefficients $eps1 1. lower          (eps,mu,grid-name)
coefficients $eps1 1. lowerInterface (eps,mu,grid-name)
coefficients $eps2 1. upper          (eps,mu,grid-name)
coefficients $eps2 1. upperInterface (eps,mu,grid-name)
#
interface BC iterations $interfaceIterations
#
# bc: Annulus=perfectElectricalConductor
tFinal $tFinal
tPlot  $tPlot
#
dissipation $diss
dissipation (curvilinear) $dissc
apply filter $filter
filter order $filterOrder
filter frequency $filterFrequency
#*********************************
maximum number of parallel sub-files 8
show file options...
  MXSF:compressed
  MXSF:open
    $show
 # MXSF:frequency to save 
  MXSF:frequency to flush 2
exit
#**********************************
#
use variable dissipation $varDiss
number of variable dissipation smooths $varDissSmooths
use conservative difference $cons
debug 0
#
cfl $cfl 
plot errors 0
check errors 0
plot intensity $plotIntensity
plot harmonic E field $plotHarmonicComponents
#$c1 = 1./sqrt($eps1*$mu1); $omega= $c1*sqrt( $kx*$kx + $ky*$ky + $kz*$kz );
#time harmonic omega $omega (omega/(2pi), normally c*|k|
#
#specify probes
#  -1.544e-01 2.334e-02 0.000e+00
#  -1.926e-02 2.234e-02 0.000e+00
# done
#
continue
#
plot:Ex
contour
plot contour lines (toggle)
# vertical scale factor 0.
# min max -1.1 1.1
exit
x-r 90
set home # make this view the default home view
$go
