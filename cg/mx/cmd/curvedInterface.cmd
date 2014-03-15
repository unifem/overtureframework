*
*     mx cmd/curvedInterface
*
*  == run conv/convergence.p (see conv/memo) tzType=material for convergence rates ==
*
*  plotCurvedInterface.cmd
* 
$kx=4.; $ky=0; $kz=0;
* $kx=2.; 
$eps1=1.; # inner
$eps2=1.; # outer 
$cfl = .8;
$diss=.5;
$tFinal=.1; $tPlot=.05; $show=" "; $go="go"; $divClean=0; $divCleanCoeff=0.;
$interfaceIterations=3;
* 
* $gridName = "innerOuter.hdf"; $eps1=.25; $tPlot=.01;
* 
* $gridName = "innerOuter2.hdf"; $eps1=1.;
* $gridName = "innerOuter2.hdf"; $eps1=.25; $tPlot=.001;
* $gridName = "innerOuter4.hdf"; $eps1=1.; $diss=.5;
* $gridName = "innerOuter4.hdf"; $eps1=.25; $cfl=.8; $tFinal=1.; $diss=.5; 
* $gridName = "innerOuter8.hdf"; $eps1=.25; $tFinal=1.; $tPlot=.5; $diss=1.; $interfaceIterations=3; $show=" ";
* $gridName = "innerOuter8.hdf"; $eps1=1.; $diss=1.;
* $gridName = "innerOuter8.hdf"; $eps1=1.; $diss=1.; $eps1=.25; 
*
**** this coarse grid needed more dissipation
* $gridName = "innerOuter2.order4.hdf";  $eps1=.25; $diss=.5; $interfaceIterations=3;
$gridName = "innerOuter4.order4.hdf";  $eps1=.25; $diss=.25; $interfaceIterations=3;
* $gridName = "innerOuter4.order4.hdf"; $eps1=1.;
* $gridName = "innerOuter4.order4.hdf"; $eps1=.25; $tFinal=1.; $tPlot=1.; $diss=.5; $interfaceIterations=3; $show=" "; 
* $gridName = "innerOuter4.order4.hdf"; $eps1=.25; $tFinal=2.; $tPlot=1.; $diss=.5; $interfaceIterations=3; $show=" "; 
* $gridName = "innerOuter4.order4.hdf"; $eps1=.25; $tFinal=1.; $diss=.5; $interfaceIterations=1; $show="innerOuter4.order4.show"; 
* $gridName = "innerOuter8.order4.hdf"; $eps1=.25; $tFinal=1.; $tPlot=.01; $diss=.5; $interfaceIterations=3; $show=" "; 
* $gridName = "innerOuter8.order4.hdf"; $eps1=.25; $tFinal=1.; $diss=.5; $interfaceIterations=1; $show="innerOuter8.order4.show"; $tPlot=.2;
* $gridName = "innerOuter8.order4.hdf"; $eps1=1.; $cfl=.85; $diss=.5; 
* $gridName = "innerOuter8.order4.hdf"; $eps1=.25; $cfl=.8; $diss=.5; $tFinal=1.; $interfaceIterations=1;
* $gridName = "innerOuter8.order4.hdf"; $eps1=2.; $cfl=.8; $diss=.5; $tFinal=1.; 
* 
* $gridName = "innerOuter16.order4.hdf"; $eps1=.25; $cfl=.8; $diss=.5; $tPlot=.5; $tFinal=1.; $interfaceIterations=3;
* innerOuter.hdf
* innerOuter2.hdf
* innerOuter4.hdf
* innerOuter8.hdf
*
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,\
  "dtMax=f"=>\$dtMax,"mx=f"=>\$mx,"my=f"=>\$my,"mz=f"=>\$mz, "cons=i"=>\$cons,"dissOrder=i"=>\$dissOrder,\
  "filter=i"=>\$filter,"divClean=i"=>\$divClean,"divCleanCoeff=f"=>\$divCleanCoeff,\
  "x0=f"=>\$x0,"y0=f"=>\$y0,"z0=f"=>\$z0 );
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
# 
$gridName 
*
NFDTD
** planeWaveInitialCondition
* ++ zeroInitialCondition
* ====
planeWaveScatteredFieldInitialCondition
scatteringFromADielectricDiskKnownSolution
* ====
* twilightZone
*  degreeSpace, degreeTime  1 1
*
kx,ky,kz $kx $ky $kz
*
bc: all=dirichlet
* ++ bc: all=perfectElectricalConductor
* ++ bc: outerSquare(0,0)=planeWaveBoundaryCondition
* 
*      innerAnnulus
*      innerSquare
*      outerAnnulus
*      outerSquare
* NOTE: material interfaces have share>=100
coefficients $eps1 1. innerAnnulus (eps,mu,grid-name)
coefficients $eps1 1. innerSquare (eps,mu,grid-name)
coefficients $eps2 1. outerAnnulus (eps,mu,grid-name)
coefficients $eps2 1. outerSquare (eps,mu,grid-name)
*
interface BC iterations $interfaceIterations
*
* bc: Annulus=perfectElectricalConductor
tFinal $tFinal
tPlot  $tPlot
*
dissipation $diss
#
use divergence cleaning $divClean
div cleaning coefficient $divCleanCoeff
#
*
* use conservative difference 0
debug 0
*
cfl $cfl 
plot errors 1
check errors 1
* 
continue
*
$go
