#================================================================================================
#  cgmx example:  Scattering of a plane wave from a cylinder (compute the errors)
#
#   cgmx cic.planeWaveBC
# 
# Usage:
#   
#  cgmx [-noplot] cic.planeWaveBC -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -dissOrder=<> ...
#        -debug=<num> -cons=[0/1] -method=[nfdtd|Yee|sosup] -errorNorm=[0|1|2]  -plotIntensity=[0|1]...
#        -plotHarmonicComponents=[0|1] -rbc=[dirichlet|abcEM2|abcPML] -ic=[exact|zero] ...
#        -go=[run/halt/og]
#
# -ic : initial condition
# -rbc : radiation (far field) boundary condition (dirichlet=use exact soln)
# -errorNorm:  set to 1 or 2 to show L1 and L2 norm errors
#  -plotHarmonicComponents : plot the harmonic components of the E field
# 
# Examples:
#   cgmx cic.planeWaveBC -g=cice2.order4.hdf 
#   cgmx cic.planeWaveBC -g=cice3.order4.hdf
#   cgmx cic.planeWaveBC -g=cice16.order4.hdf
#    -- new 8th order dissipation ok: (diss=0 goes unstable about t=10)
#   cgmx cic.planeWaveBC -g=cice4.order4.hdf -dissOrder=8 -tf=100 -tp=1
#   cgmx cic.planeWaveBC -g=cice8.order4.hdf -dissOrder=8 -tf=100 -tp=1
# -- yee:
#   cgmx cic.planeWaveBC -g=bigSquare2.order2.hdf -errorNorm=2 -method=Yee
#   cgmx cic.planeWaveBC -g=bigSquare4.order2.hdf -errorNorm=2 -method=Yee
#
# -- sosup
#   cgmx cic.planeWaveBC -g=cice2.order2.hdf -method=sosup -tp=.1  [ OK
#   cgmx cic.planeWaveBC -g=cice2.order4.ng3.hdf -method=sosup -tp=.1 [ OK
# 
# -- test far field (radiation) BC's:
#   ogen -noplot cicArg -order=4 -interp=e -factor=4
#   cgmx cic.planeWaveBC -g=cice4.order4.hdf -rbc=abcEM2 
#   -- for pml we need to start with a zero field in the pml region I think:
#   cgmx cic.planeWaveBC -g=cice4.order4.hdf -diss=.5 -ic=zero -rbc=abcPML -pmlWidth=21 -pmlStrength=50. 
# 
# parallel: 
#   mpirun -np 2 $cgmxp cic.planeWaveBC -g=cice3.order4.hdf
#   mpirun -np 4 $cgmxp cic.planeWaveBC -g=cice16.order4.hdf
#   mpirun -np 2 $cgmxp cic.planeWaveBC -g=$ov/ogen.p/cice3.order4.hdf
#   srun -N1 -n1 -ppdebug $cgmxp noplot cic.planeWaveBC -g=cice2.order2.hdf -tf=.1
#   srun -N1 -n1 -ppdebug $cgmxp cic.planeWaveBC -g=cice2.order4.hdf
# 
#   srun -N1 -n4 -ppdebug $cgmxp noplot cic.planeWaveBC -g=cice64.order4.hdf -tf=.1
#
# NOTES:
#   (1) In this example we solve for the scattered field. 
# 
#*** NOTE: when using the scattered field as an initial condition and BC use the options
#           (1) planeWaveBoundaryForcing : to assign the plane wave forcing on the boundary
#           (2) planeWaveScatteredFieldInitialCondition
#           (3) square=dirichlet 
#           (4) slow start interval -1.
#================================================================================================
# 
$tFinal=10.; $tPlot=.1; $diss=.1; $dissOrder=-1; $cfl=.9; $kx=1.; $ky=0.; $kz=0.; $method="NFDTD";
$plotIntensity=0;  # =1 : plot intensity of the scattered field (not total field)
$grid="sib1.order4.hdf"; $show=" "; 
$cons=0; $go="halt"; $errorNorm=0;  $plotHarmonicComponents=0; $rbc="dirichlet"; $ic="exact"; 
$pmlWidth=11; $pmlStrength=50.; $pmlPower=4.; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"errorNorm=i"=>\$errorNorm,\
  "dtMax=f"=>\$dtMax,"cons=i"=>\$cons,"plotIntensity=i"=>\$plotIntensity,"kx=f"=>\$kx,"method=s"=>\$method,\
  "dissOrder=i"=>\$dissOrder,"plotHarmonicComponents=i"=>\$plotHarmonicComponents,"rbc=s"=>\$rbc,\
  "pmlWidth=f"=>\$pmlWidth,"pmlStrength=f"=>\$pmlStrength,"pmlPower=f"=>\$pmlPower,"ic=s"=>\$ic );
# -------------------------------------------------------------------------------------------------
if( $method eq "sosup" ){ $diss=0.; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
#
$grid
#
$method
#
planeWaveBoundaryForcing
# ====
#  Initial condition:
if( $ic eq "exact" ){ $cmd = "planeWaveScatteredFieldInitialCondition"; }else{ $cmd="zeroInitialCondition"; }
$cmd
# -- specify the exact solution:
scatteringFromADiskKnownSolution
# ====
# *****************
# for Yee we define the cylinder as a masked stair step region
$rad=.5; $x0=0.; $y0=0; $z0=0;  
if( $method eq "Yee" ){ $cmds = "define embedded bodies\n PEC cylinder\n $rad $x0 $y0 $z0\n exit"; }else{ $cmds="#"; }
$cmds 
# ****************
#
kx,ky,kz $kx $ky $kz
# Gaussian plane wave: 100. -1.5 0. 0.
#* bc: square=perfectElectricalConductor
# bc: square=abcEM2
# =====================================
# radius for checking errors 2.
# =====================================
bc: square=$rbc
# pml parameters: 
pml width,strength,power $pmlWidth $pmlStrength $pmlPower
#******
#bc: square(0,0)=planeWaveBoundaryCondition
#bc: square(1,0)=planeWaveBoundaryCondition
if( $method eq "NFDTD" || $method eq "sosup" ){ $cmd="bc: Annulus=perfectElectricalConductor"; }else{ $cmd="#"; }
$cmd
tFinal $tFinal
tPlot  $tPlot
# 
order of dissipation $dissOrder
dissipation  $diss
#***********************
# slow start interval 1.
slow start interval -1.
#***********************
# divergence damping  0.0025   .02  1. .005 .001 .0025 .01
#***********************
cfl $cfl
#
#*********************************
show file options...
  MXSF:compressed
  MXSF:open
    $show
  MXSF:frequency to flush 10
exit
#**********************************
error norm $errorNorm
use conservative divergence $cons 
# plot scattered field 1
plot errors 1
check errors 1
plot intensity $plotIntensity
plot harmonic E field $plotHarmonicComponents
#
continue
$go 

