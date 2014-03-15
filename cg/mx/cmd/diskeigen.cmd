#======================================================================================
# cgmx example: compute eigenfunctions of a disk (and compare to the exact solution)
# 
#  Usage:
#       cgmx diskeigen -g=<gridName> -m=[1|2|3..] -n=[1|2|3...]
#
#      Jm cos(n*theta) : give m, n
# 
# Examples:
#
#  cgmx diskeigen -g=sice4.order4.hdf -m=1 -n=1
#  cgmx diskeigen -g=sice4.order4.hdf -m=2 -n=2
#
# -- SOSUP:
#  cgmx diskeigen -g=sice4.order2.hdf -method=sosup -m=2 -n=2             [OK
#  cgmx diskeigen -g=sice4.order4.ng3.hdf -method=sosup -m=2 -n=2         [OK
#  cgmx diskeigen -g=sice4.order6.ng4.hdf -method=sosup -m=2 -n=2         [Not yet
#
#  ogen -noplot sicArg -order=4 -numGhost=3 -interp=e -factor=4
#  cgmx diskeigen -g=sice4.order4.ng3 -method=sosup -m=2 -n=2 -cfl=1.
#
# -- parallel
#  mpirun -np 2 $cgmxp diskeigen
# =======================================================================================
$tFinal=5.; $tPlot=.25; $diss=.5; $cfl=.9; 
$grid="sice8.order4.hdf";
# 
$grid="sice4.order4.hdf"; $eigenvalue="2 3";
$tFinal=1.; $tPlot=.1; $diss=.0; $cfl=.95; $dissOrder=-1; $filter=0; $divClean=0; $divCleanCoeff=1; $projectInterp=0;
$grid="box32.order4.hdf"; $method="NFDTD"; 
$cons=0; $go="halt"; 
$m=1; $n=1;  # defines the eigenfunction 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,\
  "dtMax=f"=>\$dtMax,"m=i"=>\$m,"n=i"=>\$n, "cons=i"=>\$cons,"dissOrder=i"=>\$dissOrder,\
  "filter=i"=>\$filter,"divClean=i"=>\$divClean,"divCleanCoeff=f"=>\$divCleanCoeff,\
  "x0=f"=>\$x0,"y0=f"=>\$y0,"z0=f"=>\$z0,"projectInterp=i"=>\$projectInterp,"method=s"=>\$method );
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
$grid
#
modifiedEquationTimeStepping
#
$method
#
#***
bc: all=perfectElectricalConductor
#**
#****
annulusEigenfunctionInitialCondition
 $m $n 
# 
annulusEigenfunctionKnownSolution
#**
specify probes
  .2 .3 0.
  .4 .6 0.
done
#****
#
tFinal $tFinal
tPlot  $tPlot
dissipation $diss
order of dissipation 4
cfl  $cfl
#
use conservative difference 0
continue
#
$go


movie mode
finish


plot:Hz
erase
contour
ghost lines 1
exit
continue

#
movie mode
finish


continue

finish


