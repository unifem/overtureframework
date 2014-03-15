#================================================================================================
#  cgmx example:  scattering from 3d curved interface between glass and a vacuum
#
# Usage:
#   
#  cgmx [-noplot] bump3d  -g=<name> -tf=<tFinal> -tp=<tPlot> -kx=<num> -ky=<num> -kz=<num> ...
#                          -diss=<> -debug=<num> -cons=[0/1]  -filter=[0|1] -varDiss=<0|1> -go=[run/halt/og]
# Arguments:
#  -kx= -ky= -kz= : integer wave number of the incident wave
#  -varDiss :  if 1, use variable dissipation (only add dissipation near interpolation points)
# 
# Notes: 
# refractive index from vacuum to glass is 1.5
#       $epsGlass = 1.5^2 = 2.25
# 
#   pts/wave-length = (1/ds)*(1/ky) = factor*20/kz   ds=.05=1/20
# 
# NOTE: diss=.5 was too small for fine grid runs;  diss=5. works but a smaller value may also be ok. 
# 
# Examples:
#          I = .5*sqrt(eps/mu) a^2  ppl=2*20/5=8 
#   cgmx bump3d -g=interfaceBump3d1bumpe2.order2 -eps1=2.25 -eps2=1. -kz=5 -ax=.81649658 -ay=.81649658 -zb=-.2 -diss=1. -plotIntensity=1 -go=halt
#   cgmx bump3d -g=interfaceBump3d1bumpe2.order4 -eps1=1. -eps2=1. -kz=5 -ax=1. -ay=1. -zb=-.2 -diss=1. -plotIntensity=1 -go=halt
#              ppl=16 
#   cgmx noplot bump3d -g=interfaceBump3d1bumpe4.order4 -eps1=1. -eps2=1. -kz=5 -ax=1. -ay=1. -zb=-.2 -diss=1. -plotIntensity=1 -show="bump4order4.show" -go=go  >! bump4order4.out
# 
#   mpirun -np 2 $cgmxp bump3d -g=interfaceBump3d1bumpe2.order2 -eps1=2.25 -eps2=1. -kz=5 -ax=.81649658 -ay=.81649658 -zb=-.1 -diss=2. -go=halt
#   totalview srun -a -N1 -n2 -ppdebug $cgmxp noplot bump3d -g=interfaceBump3d1bumpe2.order2 -eps1=2.25 -eps2=1. -ax=.81649658 -ay=.81649658 -kz=5  -diss=2. -go=halt
#
#================================================================================================
# 
$tFinal=1.5; $tPlot=.1; $diss=5.; $filter=0;  $cfl=.9; $varDiss=0; $varDissSmooths=20; $plotIntensity=0;
$kx=0; $ky=0; $kz=0; 
$ax=0.; $ay=0.; $az=0.; # plane wave coeffs. all zero -> use default
$eps1=1.; $mu1=1.; # inner
$eps2=1.; $mu2=1. # outer 
$show=" "; $backGround="backGround"; $flush=5; 
$interfaceIterations=3;
$grid="interfaceBump3de1.order2";
$cons=0; $go="halt"; 
$xa=-100.; $xb=100.; $ya=-100.; $yb=100.; $za=-100.; $zb=100.;  # initial condition bounding box
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
  "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"filter=i"=>\$filter,\
  "dtMax=f"=>\$dtMax,"kx=f"=>\$kx,"ky=f"=>\$ky,"kz=f"=>\$kz,"ax=f"=>\$ax,"ay=f"=>\$ay,"az=f"=>\$az,"flush=i"=>\$flush,\
  "eps1=f"=>\$eps1,"eps2=f"=>\$eps2, "cons=i"=>\$cons,"yb=f"=>\$yb,"zb=f"=>\$zb,"plotIntensity=i"=>\$plotIntensity,\
  "ii=i"=>\$interfaceIterations,"varDiss=i"=>\$varDiss ,"varDissSmooths=i"=>\$varDissSmooths,"yb=f"=>\$yb);
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# 
$grid
#
NFDTD
planeWaveInitialCondition
# we need to clip the plane wave on a period to avoid oscillations at the front
if( $kz ne 0 ){ $kza= abs($kz); $zb = int( $zb*$kza -.9999 )/$kza; if( $kz < 0 ){ $za=$zb; $zb=1e9; } }
initial condition bounding box $xa $xb $ya $yb $za $zb
if( $kz > 0 || $ky > 0 ){ $epsPW=$eps1; $muPW=$mu1; }else{ $epsPW=$eps2; $muPW=$mu2; }
#
plane wave coefficients $ax $ay $az $epsPW $muPW
#
kx,ky,kz $kx $ky $kz
#
# bc: all=dirichlet
# bc: all=perfectElectricalConductor
# bc: all=perfectElectricalConductor
bc: all=abcEM2
# if( $ky > 0 ){ $inbc = "lower(0,2)"; }else{ $inbc = "upper(1,2)"; }
# bc: lower(0,2)=planeWaveBoundaryCondition
# no: bc: $inbc=planeWaveBoundaryCondition
# 
bc: upper(0,0)=symmetry
bc: upper(1,0)=symmetry
bc: upper(0,1)=symmetry
bc: upper(1,1)=symmetry
# 
bc: upperInterface(0,0)=symmetry
bc: upperInterface(1,0)=symmetry
bc: upperInterface(0,1)=symmetry
bc: upperInterface(1,1)=symmetry
# 
bc: lower(0,0)=symmetry
bc: lower(1,0)=symmetry
bc: lower(0,1)=symmetry
bc: lower(1,1)=symmetry
# 
bc: lowerInterface(0,0)=symmetry
bc: lowerInterface(1,0)=symmetry
bc: lowerInterface(0,1)=symmetry
bc: lowerInterface(1,1)=symmetry
# 
# -- we need to subtract out the incident field on the "inflow" boundary before
#    applying the radiation boundary condition: 
adjust boundaries for incident field 0 all
if( $kz > 0 || $ky > 0 ){ $inflow="lower"; }else{ $inflow="upper"; }
adjust boundaries for incident field 1 $inflow
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
apply filter $filter
#*********************************
maximum number of parallel sub-files 8
show file options...
  MXSF:compressed
  MXSF:open
    $show
 # MXSF:frequency to save 
  MXSF:frequency to flush $flush 
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
# plot energy density 1
plot intensity $plotIntensity
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
x-r 90
set home
plot:Ex
contour
plot contour lines (toggle)
# vertical scale factor 0.
# min max -1.1 1.1
exit
$go
