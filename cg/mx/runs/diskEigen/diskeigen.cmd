#======================================================================================
# cgmx example: compute eigenfunctions of a disk (and compare to the exact solution)
# 
#  Usage:
#       cgmx diskeigen -g=<gridName> -m=[1|2|3..] -n=[1|2|3...] -bcn=[p|d]
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
#  cgmx diskeigen -g=sice4.order6.ng4.hdf -method=sosup -m=2 -n=2 -bcn=d  [ FAKE DIRICHLET BC's - 
#
#  ogen -noplot sicArg -order=4 -numGhost=3 -interp=e -factor=4
#  cgmx diskeigen -g=sice4.order4.ng3 -method=sosup -m=2 -n=2 -cfl=1.
#
# -- parallel
#  mpirun -np 2 $cgmxp diskeigen
# =======================================================================================
$tFinal=5.; $tPlot=.25; $diss=.5; $cfl=.9; $bcn="p"
$grid="sice8.order4.hdf";
# 
$grid="sice4.order4.hdf"; $eigenvalue="2 3"; $show=" ";
$tFinal=1.; $tPlot=.1; $diss=.0; $cfl=.95; $dissOrder=-1; $filter=0; $divClean=0; $divCleanCoeff=1; $projectInterp=0;
$grid="box32.order4.hdf"; $method="NFDTD"; 
$cons=0; $go="halt"; 
$m=1; $n=1;  # defines the eigenfunction 
$dm="none"; $alphaP=1.; $a0=1.; $a1=0.; $b0=0.; $b1=1.;  # GDM parameters
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"bcn=s"=>\$bcn,\
  "dtMax=f"=>\$dtMax,"m=i"=>\$m,"n=i"=>\$n, "cons=i"=>\$cons,"dissOrder=i"=>\$dissOrder,\
  "filter=i"=>\$filter,"divClean=i"=>\$divClean,"divCleanCoeff=f"=>\$divCleanCoeff,\
  "x0=f"=>\$x0,"y0=f"=>\$y0,"z0=f"=>\$z0,"projectInterp=i"=>\$projectInterp,"method=s"=>\$method,\
   "dm=s"=>\$dm,"alphaP=f"=>\$alphaP,"a0=f"=>\$a0,"a1=f"=>\$a1,"b0=f"=>\$b0,"b1=f"=>\$b1 );
# -------------------------------------------------------------------------------------------------
if( $dm eq "none" ){ $dm="no dispersion"; }
if( $dm eq"gdm" ){ $dm="GDM"; }
# 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
$grid
#
modifiedEquationTimeStepping
#
$method
# dispersion model:
$dm
GDM params $a0 $a1 $b0 $b1 all (a0,a1,b0,b1,domain-name)
#
#***
if( $bcn eq "d" ){ $bcn = "bc: all=dirichlet"; }
if( $bcn eq "p" ){ $bcn = "bc: all=perfectElectricalConductor"; }
$bcn 
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
use conservative difference $cons
#*********************************
show file options...
  MXSF:compressed
  MXSF:open
  $show
  MXSF:frequency to flush 20
exit
#**********************************
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


