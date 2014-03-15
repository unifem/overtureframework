*================================================================================================
*  cgmx example:  Scattering of a plane wave from different bodies
*
* Usage:
*   
*  cgmx [-noplot] scat -g=<name> -tf=<tFinal> -tp=<tPlot> -kx=<num> -ky=<num> -kz=<num> -diss=<> ...
*                      -bg=<back-ground> -debug=<num> -cons=[0/1] -plotIntensity=[0|1] -go=[run/halt/og]
* Arguments:
*  -kx= -ky= -kz= : integer wave numbers of the incident wave
*  -bg= : name of the background grid
* 
* Examples: (see Makefile to construct grids)
*   cgmx scat -g=cice3.order4.hdf 
*   cgmx scat -g=trianglee4.order4.hdf -bg=backGround           (scattering from a rounded triangle)
*   cgmx scat -g=trianglee8.order4.hdf -bg=backGround -plotIntensity=1
*   cgmx scat -g=crve4.order4.hdf -bg=backGround                (scattering from a space reentry vehicle )
* 
* parallel: 
*   mpirun -np 2 $cgmxp scat -g=trianglee8.order4.hdf -bg=backGround
*
*================================================================================================
* 
$tFinal=10.; $tPlot=.1; $diss=1.; $cfl=.9; $plotIntensity=0;
$kx=1; $ky=0; $kz=0;
$grid="sib1.order4.hdf"; $backGround="square"; 
$cons=0; $go="halt"; 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,\
  "dtMax=f"=>\$dtMax,"kx=i"=>\$kx,"ky=i"=>\$ky,"kz=i"=>\$kz,"plotIntensity=i"=>\$plotIntensity, "cons=i"=>\$cons );
* -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
*
$grid
*
NFDTD
*
* Adjust PEC boundaries to account for a plane wave being subtracted out
planeWaveBoundaryForcing
* ====
*  Initial condition:
zeroInitialCondition
* ====
*
kx,ky,kz $kx $ky $kz 
* Gaussian plane wave: 100. -1.5 0. 0.
** bc: square=perfectElectricalConductor
* bc: square=abcEM2
* bc: square=abcPML
* pml width,strength,power 25 30. 4
bc: all=perfectElectricalConductor
bc: $backGround=abcEM2
*bc: square(0,0)=planeWaveBoundaryCondition
*bc: square(1,0)=planeWaveBoundaryCondition
tFinal $tFinal
tPlot  $tPlot
* 
dissipation  $diss
************************
** slow start interval 1.
slow start interval -1.
************************
* divergence damping  0.0025   .02  1. .005 .001 .0025 .01
************************
cfl $cfl
*
use conservative divergence $cons 
* plot scattered field 1
plot total field 1
plot errors 0
check errors 0
plot intensity $plotIntensity
# $omega= sqrt( $kx*$kx + $ky*$ky + $kz*$kz );
# time harmonic omega $omega (omega/(2pi), normally c*|k|^2
*
continue
plot:Ey
$go 

