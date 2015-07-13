#================================================================================================
#  cgmx example:  scattering from a dielectric cylinder or sphere (and compare to the exact solution)
#
# Usage:
#   
#  cgmx [-noplot] dielectricCyl  -g=<name> -tf=<tFinal> -tp=<tPlot> -kx=<num> -ky=<num> -kz=<num> -show=<name> ...
#                                -eps1=<> -eps2=<> -interit=<> -diss=<> -filter=[0|1] -dissc=<> -debug=<num> ...
#                                -cons=[0/1] -plotIntensity=[0|1] ...
#                                -method=[nfdtd|Yee|sosup] -errorNorm=[0|1|2] -go=[run/halt/og]
# Arguments:
#  -kx= -ky= -kz= : integer wave numbers of the incident wave
#  -interit : number of iterations to solve the interface equations 
#  -errorNorm:  set to 1 or 2 to show L1 and L2 norm errors
#  -diss, -dissc : coefficients of art. dissipation. If dissc>=0 then use this for curvilinear grids. 
#
# Examples: (see Makefile to build grids)
#  -- NEW 8th order filter: smaller errors and more stable for fewer -interit 
#   cgmx dielectricCyl -g=innerOutere8.order4 -kx=2 -eps1=.25 -eps2=1. -go=halt -filter=1 -tp=.5 -tf=10 -interit=1
#   cgmx dielectricCyl -g=innerOutere4.order2 -kx=2 -eps1=.25 -eps2=1. -go=halt -dissOrder=4 -tp=.5 -tf=10 
#   cgmx dielectricCyl -g=innerOutere4.order4 -kx=1.25 -eps1=2.25 -eps2=1. -go=halt -diss=0. -filter=1 -tp=.1 -tf=10 -interit=1
#
#  -- broken:
#   cgmx dielectricCyl -g=innerOutere2.order4.hdf -kx=2 -eps1=.25 -eps2=1. -go=halt -tp=.01
# 
#  --NEW: build grids with domains
#   cgmx dielectricCyl -g=innerOutere2.order4.hdf -kx=2 -eps1=.25 -eps2=1. -go=halt
#   cgmx dielectricCyl -g=innerOutere4.order4.hdf -kx=2 -eps1=.25 -eps2=1. -go=halt
#   cgmx dielectricCyl -g=innerOutere8.order4.hdf -kx=2 -eps1=.25 -eps2=1. -go=halt
# 
#   cgmx dielectricCyl -g=innerOutere4.order4.hdf -kx=2 -eps1=2.25 -eps2=1. -go=halt
# 
#   mpirun -np 2 $cgmxp dielectricCyl -g=innerOutere2.order2.hdf -kx=2 -eps1=.25 -eps2=1. -useNewInterface=1 -go=halt
#   mpirun -np 2 $cgmxp dielectricCyl -g=$ov/ogen.p/innerOuter4.order2parallel.hdf -kx=2 -eps1=.25 -eps2=1. -useNewInterface=1 -go=halt
# 
#   srun -N1 -n1 -ppdebug $cgmxp noplot dielectricCyl -g=innerOutere2.order2.hdf -kx=2 -eps1=.25 -eps2=1. -tf=.1 -useNewInterface=1 -go=go
# 
# -- Yee scheme : dielectric cylinder:
#   cgmx dielectricCyl -g=bigSquareSize1f4.hdf -kx=2 -eps1=.25 -eps2=1. -method=Yee -errorNorm=2 -tp=.01 -go=halt
#   cgmx dielectricCyl -g=bigSquareSize1f8.hdf -kx=2 -eps1=.25 -eps2=1. -method=Yee -errorNorm=2 -go=halt
#   cgmx dielectricCyl -g=bigSquareSize1f16.hdf -kx=2 -eps1=.25 -eps2=1. -method=Yee -errorNorm=2 -go=halt
#   cgmx dielectricCyl -g=bigSquareSize1f32.hdf -kx=2 -eps1=.25 -eps2=1. -method=Yee -errorNorm=2 -go=halt
# -- Yee scheme : dielectric sphere
#   cgmx dielectricCyl -cyl=0 -g=bigBox1.order2 -kx=1 -eps1=.25 -eps2=1. -method=Yee -errorNorm=2 -tp=.01 -go=halt
#   cgmx dielectricCyl -cyl=0 -g=bigBox2.order2 -kx=1 -eps1=.25 -eps2=1. -method=Yee -errorNorm=2 -tp=.01 -go=halt
#   cgmx dielectricCyl -cyl=0 -g=bigBox8.order2 -kx=1 -eps1=.25 -eps2=1. -method=Yee -errorNorm=2 -cfl=.5 -tp=.01 -go=halt
#
# -- sosup
#   cgmx dielectricCyl -g=innerOutere4.order4.hdf -kx=2 -eps1=.25 -eps2=1. -method=sosup -go=halt
#
# -- OLD: 
#   cgmx dielectricCyl -g=innerOuter4.order2.hdf -kx=2 -eps1=.25 -eps2=1. -go=halt
#   cgmx dielectricCyl -g=innerOuter4.order4.hdf -kx=2 -eps1=.25 -eps2=1. -go=halt
#   cgmx dielectricCyl -g=innerOuter8.order4.hdf -kx=2 -eps1=2.  -eps2=1. -go=halt
#   cgmx dielectricCyl -g=innerOuter16.order4.hdf -kx=2 -eps1=2. -eps2=1. -go=halt
#
#  -- dielectric sphere: eps1=inside eps2=outside (should be 1)
#   cgmx dielectricCyl -cyl=0 -g=solidSphereInABoxi1.order2 -kx=1 -eps1=.25 -eps2=1. -go=halt
#   cgmx dielectricCyl -cyl=0 -g=solidSphereInABoxi2.order2 -kx=1 -eps1=.25 -eps2=1. -go=halt
#   cgmx dielectricCyl -cyl=0 -g=solidSphereInABoxi1.order2 -kx=1 -eps1=1. -eps2=1. -go=halt -tp=.01
# 
#   cgmx dielectricCyl -cyl=0 -g=solidSphereInABoxe2.order2 -kx=1 -eps1=.25 -eps2=1. -go=halt 
#   cgmx dielectricCyl -cyl=0 -g=solidSphereInABoxe4.order2 -kx=1 -eps1=.25 -eps2=1. -go=halt
# 
# -- fourth order:
#   cgmx dielectricCyl -cyl=0 -g=solidSphereInABoxi2.order4 -kx=1 -eps1=.25 -eps2=1. -go=halt  [ok]
#   cgmx noplot dielectricCyl -cyl=0 -g=solidSphereInABoxi4.order4 -kx=1 -eps1=.25 -eps2=1. -tf=.2 -go=go 
#  -- fix grid dimensions for convergence tests:
#   cgmx noplot dielectricCyl -cyl=0 -g=solidSphereInABoxFixedi1.order2 -kx=1 -eps1=.25 -eps2=1. -go=go -tf=.5
#   cgmx noplot dielectricCyl -cyl=0 -g=solidSphereInABoxFixedi2.order2 -kx=1 -eps1=.25 -eps2=1. -go=go -tf=.5
# --- new sphere grids
#   cgmx noplot dielectricCyl -cyl=0 -g=solidSphereInABoxNewi1.order4 -kx=1 -eps1=.25 -eps2=1. -go=og
#   cgmx dielectricCyl -cyl=0 -g=solidSphereInABoxe2.order4 -kx=1 -eps1=.25 -eps2=1. -go=halt -tp=.01
#   cgmx dielectricCyl -cyl=0 -g=solidSphereInABoxe4.order4 -kx=1 -eps1=.25 -eps2=1. -go=halt -tp=.01 
# 
# parallel: 
#  srun -N1 -n1 -ppdebug $cgmxp noplot dielectricCyl -cyl=0 -g=solidSphereInABoxe2.order2 -kx=1 -eps1=.25 -eps2=1. -tp=.1 -tf=.2 -go=go
#  srun -N1 -n1 -ppdebug $cgmxp noplot dielectricCyl -cyl=0 -g=solidSphereInABoxe2.order4 -kx=1 -eps1=.25 -eps2=1. -tp=.1 -tf=.2 -go=go
#  totalview srun -a -N1 -n1 -ppdebug $cgmxp noplot ...
#  srun -ppdebug -N2 -n2 memcheck_all $cgmxp noplot ...
# diss=.5 not enough for:
#  srun -N1 -n1 -ppdebug $cgmxp noplot dielectricCyl -cyl=0 -g=solidSphereInABoxe8.order4 -kx=1 -eps1=.25 -eps2=1. -tp=.1 -tf=.5 -diss=2. -go=go
#================================================================================================
# 
$tFinal=2.; $tPlot=.1;  $show=" "; $method="NFDTD";
$cfl = .8; $diss=.5; $dissOrder=-1; $filter=0; $dissc=-1.; $debug=0;  $plotIntensity=0;
$cyl=1;   # set to 0 for a sphere 
$kx=2; $ky=0; $kz=0;
$eps1=.25; $mu1=1.; # inner
$eps2=1.;  $mu2=1.; # outer 
$show=" "; $backGround="backGround"; $useNewInterface=1; 
$interfaceIterations=3;
$grid="innerOuter4.order4.hdf";
$cons=0; $go="halt";  $errorNorm=0;
# 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"dissc=f"=>\$dissc,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"plotIntensity=i"=>\$plotIntensity,\
 "interit=i"=>\$interfaceIterations,"cyl=i"=>\$cyl,"useNewInterface=i"=>\$useNewInterface,"errorNorm=i"=>\$errorNorm,\
 "dtMax=f"=>\$dtMax,"kx=f"=>\$kx,"ky=f"=>\$ky,"kz=f"=>\$kz,"eps1=f"=>\$eps1,"eps2=f"=>\$eps2, "cons=i"=>\$cons,\
 "method=s"=>\$method,"dissOrder=i"=>\$dissOrder,"filter=i"=>\$filter );
# -------------------------------------------------------------------------------------------------
if( $method eq "sosup" ){ $diss=0.; }
if( $method eq "fd" ){ $method="nfdtd"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
# 
#
$grid
#
$method
#* planeWaveInitialCondition
# ++ zeroInitialCondition
# ==== in 3d solve for the full field ===
$ic = $cyl ? "planeWaveScatteredFieldInitialCondition" : "planeWaveInitialCondition";
# $ic
planeWaveScatteredFieldInitialCondition
$known = $cyl ? "scatteringFromADielectricDiskKnownSolution" : "scatteringFromADielectricSphereKnownSolution";
$known
# *****************
# for Yee we define the cylinder as a masked stair step region
if( $cyl eq 1 ){ $rad=.4; }else{ $rad=1.; }
$x0=0.; $y0=0; $z0=0;  
$cmds="#";
if( $cyl eq 1 && $method eq "Yee" ){ $cmds = "define embedded bodies\n dielectric cylinder\n $rad $x0 $y0 $z0\n $eps1 $mu1 0. 0. \nexit"; }
if( $cyl eq 0 && $method eq "Yee" ){ $cmds = "define embedded bodies\n dielectric sphere\n $rad $x0 $y0 $z0\n $eps1 $mu1 0. 0. \nexit"; }
$cmds 
# ****************
# ====
# twilightZone
#  degreeSpace, degreeTime  1 1
#
kx,ky,kz $kx $ky $kz
# 
use new interface routines $useNewInterface
#
bc: all=dirichlet
# --
#bc: all=abcEM2
#bc: outerBox(0,0)=planeWaveBoundaryCondition
# --
# ++ bc: all=perfectElectricalConductor
# ++ bc: outerSquare(0,0)=planeWaveBoundaryCondition
# 
#      innerAnnulus
#      innerSquare
#      outerAnnulus
#      outerSquare
# NOTE: material interfaces have share>=100
$cmd="#";
if( $cyl eq 1 && $method ne "Yee" ){ $cmd = \
  "coefficients $eps1 1. innerAnnulus* (eps,mu,grid-name)\n" .\
  "coefficients $eps1 1. innerSquare (eps,mu,grid-name)\n" .\
  "coefficients $eps2 1. outerAnnulus* (eps,mu,grid-name)\n" .\
  "coefficients $eps2 1. outerSquare (eps,mu,grid-name)\n"; }
if( $cyl eq 0 && $method ne "Yee" ){ $cmd = \
  "coefficients $eps1 1. inner* (eps,mu,grid-name)\n" .\
  "coefficients $eps2 1. outer* (eps,mu,grid-name)\n";}
$cmd
#
interface BC iterations $interfaceIterations
#
# bc: Annulus=perfectElectricalConductor
tFinal $tFinal
tPlot  $tPlot
#
# this is broken: order of dissipation 6
order of dissipation $dissOrder
apply filter $filter
dissipation $diss
dissipation (curvilinear) $dissc
#
# use conservative difference $cons 
debug $debug
#
cfl $cfl 
plot errors 1
check errors 1
error norm $errorNorm
plot intensity $plotIntensity
# 
#*********************************
show file options...
  MXSF:compressed
  MXSF:open
    $show
  MXSF:frequency to flush 10
exit
#**********************************
continue
#
plot:Ey
# 
$go
