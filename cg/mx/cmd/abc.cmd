*================================================================================================
*  cgmx example:  test the absorbing/non-reflecting/radiation 
*
* Usage:
*   
*  cgmx [-noplot] abc -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -debug=<num> -cons=[0/1] ...
*                     -rbc=[abcEM2|rbcNonLocal|abcPML] -ic=[gs|pw|tz|gpw] -ks=[pw|dd|gpw|none] ...
*                     -pmlLines=<> -pmlPower=<> -pmlStrength=<>  -go=[run/halt/og]
*
*  -ic : initial condition: gs=Gausian-source, pw=plane-wave, tz=TZ, gpw=Gaussian-plane-wave
*  -ks : known solution: pw=plane-wave, dd=scattering-from-a-disk, gpw=Gaussian-plane-wave
* 
* Examples:
*   cgmx abc -g=square128.order2 -go=halt
*   cgmx abc -g=square128.order4 -go=halt
* 
*   cgmx abc -g=nonSquare5 -go=halt -debug=1 
*   cgmx abc -g=nonBox5 -go=halt -debug=1
*   cgmx abc -g=nonSquare128 -go=halt
*   cgmx abc -g=nonSquare128.order4 -go=halt
*   cgmx abc -g=rotatedSquare40 -go=halt
*   cgmx abc -g=rotatedSquare16.order4 -y0=.707107 -go=halt
*   cgmx abc -g=rotatedSquare64.order4 -x0=0. -y0=.707107 -go=halt
* 
*   cgmx abc -g=box64.order4 -x0=.5 -y0=.5 -z0=.5 -go=halt
*   cgmx abc -g=rotatedBox4.order2 -x0=.5 -y0=.5 -z0=.5 -go=halt
*   cgmx abc -g=rotatedBox2.order4 -x0=.5 -y0=.5 -z0=.5 -go=halt
*   cgmx abc -g=nonBox16.order4 -x0=.5 -y0=.5 -z0=.5 -go=halt
*   cgmx abc -g=nonBox64.order4 -x0=.5 -y0=.5 -z0=.5 -go=halt
*
* PML: 
*   cgmx abc -g=square128.order2 -rbc=abcPML -ic=gs -go=halt -pmlLines=11
*   cgmx abc -g=square128.order4 -rbc=abcPML -ic=pw -ks=pw -go=halt -pmlLines=11 -kx=1 -ky=1
*   cgmx abc -g=square128np.order4 -rbc=abcPML -ic=pw -ks=pw -go=halt -pmlLines=11 -kx=1 -ky=1
*   cgmx abc -g=square32.order4 -rbc=abcPML -ic=pw -ks=pw -go=halt -pmlLines=5 -tp=.01
* Gaussian plane wave: 
*   cgmx abc -g=square64np.order4 -rbc=abcPML -ic=gpw -ks=gpw -go=halt -pmlLines=11 -tp=.2 -x0=.0 
*   cgmx abc -g=square64np.order4 -rbc=rbcNonLocal -ic=gpw -ks=gpw -go=halt -tp=.2 -x0=.0 
*   cgmx abc -g=square64np.order4 -rbc=abcEM2 -ic=gpw -ks=gpw -go=halt -tp=.2 -x0=.0 
* 
*   cgmx abc -g=square8 -rbc=abcPML -ic=pw -ks=pw -go=halt -tp=.01 -debug=15 -pmlLines=2
* rbcNonLocal:
*   cgmx abc -g=square128np.order4 -rbc=rbcNonLocal -ic=pw -ks=pw -go=halt -kx=1 -ky=1
* 
*   cgmx abc -g="sice4.order4.hdf" -x0=0. -y0=0. -ic=gs -go=halt
* 
* -- test abc's with incident field 
*   cgmx abc -g=square5 -ic=pw -go=halt -debug=1
*   cgmx abc -g=square128 -ic=pw -ks=pw -go=halt
* 
* -- test scattering from a 2d cylinder
*   cgmx abc -g=cice3.order4.hdf -diss=1. -tp=.5 -ic=pw -bcn="bc: Annulus=perfectElectricalConductor" -ks=dd
*   cgmx abc -g=cice6.order4.hdf -diss=1. -kx=2 -tp=.5 -ic=pw -bcn="bc: Annulus=perfectElectricalConductor" -ks=dd
* PML: 
*   cgmx abc -g=cice6.order4.hdf -diss=1. -kx=1 -tp=.1 -ic=pw -rbc=abcPML -pmlLines=21 -bcn="bc: Annulus=perfectElectricalConductor" -ks=dd
*   cgmx abc -g=cice12.order4.hdf -diss=1. -kx=1 -tp=.1 -ic=pw -rbc=abcPML -pmlLines=41 -bcn="bc: Annulus=perfectElectricalConductor" -ks=dd
* 
*   cgmx abc -g=aiae2.order4.hdf -diss=1. -tp=.5 -ic=pw -bcn="bc: Annulus=perfectElectricalConductor" -ks=dd
* 
* test "exact" non-local radiation BC: 
*   cgmx abc -g=aiae4.order4.hdf -diss=2. -tp=.5 -ic=pw -rbc=rbcNonLocal -bcn="bc: Annulus=perfectElectricalConductor" -ks=dd
*   cgmx abc -g=aiae8.order4.hdf -diss=2. -tp=.5 -ic=pw -rbc=rbcNonLocal -bcn="bc: Annulus=perfectElectricalConductor" -ks=dd
* 
* parallel: 
*   mpirun -np 2 $cgmxp abc square40    *** does this work ?? 
*
*================================================================================================
* 
$tFinal=10.; $tPlot=.1; $diss=.0; $cfl=.9; $x0=0.5; $y0=0.5; $z0=0.; $kx=1; $ky=0; $kz=0.; 
$grid="sib1.order4.hdf"; $ic="gs"; $ks="none"; $pmlLines=11; $pmlPower=6; $pmlStrength=50.; 
$cons=0; $go="halt"; $rbc="abcEM2"; $bcn="debug $debug"; 
$beta=100.; $omega=5.; # Gaussian souce
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"ic=s"=>\$ic,"bc=s"=>\$bc,\
  "dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"x0=f"=>\$x0,"y0=f"=>\$y0,"z0=f"=>\$z0,"kx=i"=>\$kx,"ky=i"=>\$ky,"kz=i"=>\$kz,\
   "ks=s"=>\$ks,"rbc=s"=>\$rbc,"pmlLines=i"=>\$pmlLines,"pmlPower=i"=>\$pmlPower,"pmlStrength=f"=>\$pmlStrength,\
   "beta=f"=>\$beta,"omega=f"=>\$omega );
* -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $ic eq "gs" ){ $ic="gaussianSource"; }\
elsif( $ic eq "pw" ){ $ic="planeWaveInitialCondition"; }\
elsif( $ic eq "gpw" ){ $ic="gaussianPlaneWave"; }\
else{ $ic="twilightZone"; }
if( $ks eq "pw" ){ $ks="planeWaveKnownSolution"; }\
elsif( $ks eq "gpw" ){ $ks="gaussianPlaneWaveKnownSolution"; }\
elsif( $ks eq "dd" ){ $ks="scatteringFromADielectricDiskKnownSolution"; }\
else{ $ks="noKnownSolution"; }
*
*
$grid
NFDTD
* --- initial condition: 
$ic 
** planeWaveBoundaryForcing
** planeWaveScatteredFieldInitialCondition
* 
* -- specify known solution here:
$ks 
* 
* gaussianSource
* twilightZone
* turn on the plane wave for testing 
* planeWaveInitialCondition
kx,ky,kz $kx $ky $kz
* gaussianPlaneWave
* nx,ny 41 41
Gaussian source: $beta $omega $x0 $y0 $z0
Gaussian plane wave: 30. $x0 $y0 $z0
tFinal $tFinal
tPlot $tPlot
****
bc: all=$rbc
* bc: all=perfectElectricalConductor
* bc: square(0,1)=symmetry
* bc: square=dirichlet
* bc: square(0,1)=dirichlet
* bc: square(1,1)=dirichlet
$bcn
pml width,strength,power $pmlLines $pmlStrength $pmlPower
****
debug $debug
***
dissipation $diss
* order of dissipation 2
cfl $cfl
* pause
**********************************
show file options...
MXSF:compressed
* MXSF:open
* /home/henshaw/res/maxwell/mx.show
* solution with -size=2
* square2Source.show
exit
***********************************
*compare to reference show file 1
*plot errors 1
**********************************
continue
plot:Hz
$go

