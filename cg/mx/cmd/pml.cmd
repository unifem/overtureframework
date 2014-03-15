#================================================================================================
#  cgmx example: Test the PML boundary conditions
#
# Usage:
#   
#  cgmx [-noplot] pml -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -debug=<num> -cons=[0/1] ...
#                     -rbc=[abcEM2|rbcNonLocal|abcPML|perfectElectricalConductor|symmetry] ...
#                     -pmlWidth=<> -pmlStrength=<> -pmlPower=<> -go=[run/halt/og]
#
# -pmlWidth, -pmlStrength, -pmlPower: The pml damping function sigma(s) = (pmlStrength)*(s)^(pmlPower) where 0 <= s <= 1
# 
#  Examples: 
#  -- square: Grids: 
#       ogen -noplot squareArg -periodic=np -order=4 -nx=128
#       ogen -noplot squareArg -periodic=pn -order=4 -nx=128
#   -- wave to right - OK
#   cgmx pml -g=square128np.order4 -rbc=abcPML -pmlWidth=64 -pmlStrength=50. -pmlPower=4 -kx=8 -xb=.25 -go=halt  
# 
#   -- wave to left - OK
#   cgmx pml -g=square128np.order4 -rbc=abcPML -pmlWidth=64 -pmlStrength=50. -pmlPower=4 -kx=-8 -xa=.75 -go=halt  
# 
#   -- wave upward: OK now 
#   cgmx pml -g=square128pn.order4 -rbc=abcPML -pmlWidth=64 -pmlStrength=50. -pmlPower=4 -kx=0 -ky=8 -yb=.25 -go=halt  
# 
#   -- wave downward: *OK*
#   cgmx pml -g=square128pn.order4 -rbc=abcPML -pmlWidth=64 -pmlStrength=50. -pmlPower=4 -kx=0 -ky=-8 -ya=.75 -go=halt  
# 
#
# -- 3D box: 
#   Grids:
#   ogen noplot boxArg -order=4 -periodic=npp -factor=5   [ 50^3 grid 
#   ogen noplot boxArg -order=4 -periodic=pnp -factor=5   [ 50^3 grid 
#   ogen noplot boxArg -order=4 -periodic=ppn -factor=5   [ 50^3 grid 
# 
#  -- wave to right (x+): OK
#   cgmx pml -g=box5npp.order4  -bg=box -rbc=abcPML -pmlWidth=25 -pmlStrength=50. -pmlPower=4 -kx=4 -xb=.25 -go=halt  
#
#  -- wave to left (x-):
#   cgmx pml -g=box5npp.order4  -bg=box -rbc=abcPML -pmlWidth=25 -pmlStrength=50. -pmlPower=4 -kx=-4 -xa=.75 -go=halt  
#
#  -- wave going in y+ direction: [OK
#   cgmx pml -g=box5pnp.order4  -bg=box -rbc=abcPML -pmlWidth=25 -pmlStrength=50. -pmlPower=4 -kx=0 -ky=4 -yb=.25 -go=halt  
#
#  -- wave going in y- direction: [OK
#   cgmx pml -g=box5pnp.order4  -bg=box -rbc=abcPML -pmlWidth=25 -pmlStrength=50. -pmlPower=4 -kx=0 -ky=-4 -ya=.75 -go=halt  
#
#  -- wave going in z+ direction: [OK
#   cgmx pml -g=box5ppn.order4  -bg=box -rbc=abcPML -pmlWidth=25 -pmlStrength=50. -pmlPower=4 -kx=0 -kz=4 -zb=.25 -go=halt  
#
#  -- wave going in z- direction: [OK
#   cgmx pml -g=box5ppn.order4  -bg=box -rbc=abcPML -pmlWidth=25 -pmlStrength=50. -pmlPower=4 -kx=0 -kz=-4 -za=.75 -go=halt  
#
$tFinal=5.; $tPlot=.1; $diss=.0; $cfl=.9; $x0=0.0; $y0=0.; $z0=0.; $kx=2; $ky=0; $kz=0.; $eps=1.; $mu=1.; 
$grid="sib1.order4.hdf"; $ic="gs"; $ks="none"; 
$backGround="square"; # grid name 
$cons=0; $go="halt"; $rbc="abcEM2"; $bcn="debug $debug"; 
$pmlWidth=11; $pmlStrength=50.; $pmlPower=6.; 
$ax=0.; $ay=0.; $az=0.; # plane wave coeffs. all zero -> use default
$xa=-100.; $xb=100.; $ya=-100.; $yb=100.; $za=-100.; $zb=100.;  # initial condition bounding box
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"ic=s"=>\$ic,"bc=s"=>\$bc,\
  "dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"za=f"=>\$za,"zb=f"=>\$zb,\
  "kx=i"=>\$kx,"ky=i"=>\$ky,"kz=i"=>\$kz,\
  "ks=s"=>\$ks,"rbc=s"=>\$rbc,"pmlWidth=f"=>\$pmlWidth,"pmlStrength=f"=>\$pmlStrength,"pmlPower=f"=>\$pmlPower );
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
$grid
#
NFDTD
# gaussianSource
#
planeWaveInitialCondition
planeWaveKnownSolution
#
kx,ky,kz $kx $ky $kz
#
$kxa = abs($kx);
$kya = abs($ky);
$kza = abs($kz);
# we need to clip the plane wave on a period
if( $kx > 0 ){ $xb = $xa + int( ($xb-$xa)*$kxa +.5 )/$kxa;  }  
if( $kx < 0 ){ $xa = $xb - int( ($xb-$xa)*$kxa +.5 )/$kxa;  }
if( $ky > 0 ){ $yb = $ya + int( ($yb-$ya)*$kya +.5 )/$kya;  }
if( $ky < 0 ){ $ya = $yb - int( ($yb-$ya)*$kya +.5 )/$kya;  }
if( $kz > 0 ){ $zb = $za + int( ($zb-$za)*$kza +.5 )/$kza;  }
if( $kz < 0 ){ $za = $zb - int( ($zb-$za)*$kza +.5 )/$kza;  }
initial condition bounding box $xa $xb $ya $yb $za $zb
$beta=20.; # exponent in tanh function for smooth transition to zero outside the bounding box
bounding box decay exponent $beta
plane wave coefficients $ax $ay $az $eps $mu
#
bc: all=planeWaveBoundaryCondition
if( $kx > 0 ){ $cmd="bc: $backGround(1,0)=abcPML"; }\
 elsif( $kx < 0 ){ $cmd="bc: $backGround(0,0)=abcPML"; }\
 elsif( $ky > 0 ){ $cmd="bc: $backGround(1,1)=abcPML"; }\
 elsif( $ky < 0 ){ $cmd="bc: $backGround(0,1)=abcPML"; }\
 elsif( $kz > 0 ){ $cmd="bc: $backGround(1,2)=abcPML"; }\
 else{ $cmd="bc: $backGround(0,2)=abcPML"; }
$cmd
# All boundaries get the far field BC: 
# bc: all=$rbc
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
plot errors 1
check errors 1
continue
plot:Ey
$go

