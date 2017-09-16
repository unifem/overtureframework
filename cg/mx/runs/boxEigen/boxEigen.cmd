#================================================================================================
#  cgmx example:  Compute the eigenfunctions of a square or box.
#
# Usage:
#   
#  cgmx [-noplot] boxEigen -g=<name> -tf=<tFinal> -tp=<tPlot> -mx=<num> -my=<num> -mz=<num> ...
#                          -method=[nfdtd|Yee|sosup] -diss=<> -debug=<num> -cons=[0/1] ...
#                          -dm=[none|drude] -go=[run/halt/og]
# Arguments:
#  -mx= -my= -mz= : integer mode numbers defining the eigenfunction 
#
# Examples:
#   cgmx noplot boxEigen -g=box32.order4.hdf -go=go
#   cgmx boxEigen -g=box32.order4.hdf -mx=3 -my=3 -mz=3 
#   cgmx noplot boxEigen -g=box64.order4.hdf -go=go
#
#  -- square:
#   cgmx boxEigen -g=square40.order4.hdf -mx=3 -my=2  -tp=.1 -go=halt
#
#  -- sosup:
#   cgmx boxEigen -g=square32.order4.ng3 -mx=3 -my=2  -method=sosup -go=halt
#
# -- div clean:
#   cgmx boxEigen -g=square40.order4.hdf -mx=3 -my=2 -divClean=1 -divCleanCoeff=10. -go=halt
#   cgmx boxEigen -g=square128.order4.hdf -mx=3 -my=2 -divClean=1 -divCleanCoeff=10. -tp=.5 -tf=50. -go=halt
#   cgmx boxEigen -g=square256.order4.hdf -mx=3 -my=2 -divClean=1 -divCleanCoeff=20. -tp=.5 -tf=50. -go=halt
#     -- div decays over time to zero:
#   cgmx boxEigen -g=sise4.order4.hdf -mx=1 -my=2 -divClean=1 -divCleanCoeff=10. -x0=-1. -y0=-1. -tp=.5 -tf=10 -go=halt
#    -- project interpolation pts:
#   cgmx boxEigen -g=sise2.order2.hdf -mx=1 -my=2 -divClean=1 -divCleanCoeff=10. -x0=-1. -y0=-1. -tf=10. -tp=.1 -projectInterp=1 -go=halt
#      : non-matching grid: works too
#   cgmx boxEigen -g=sis2 -mx=1 -my=2 -divClean=1 -divCleanCoeff=10. -x0=-1. -y0=-1. -tf=10. -tp=.1 -projectInterp=1 -go=halt
#      : rotated works too:
#   cgmx boxEigen -g=rsise2.order2 -mx=1 -my=2 -divClean=1 -divCleanCoeff=10. -x0=-1. -y0=-1. -tf=10. -tp=.1 -projectInterp=1 -go=halt
#   cgmx boxEigen -g=rsise4.order2 -mx=1 -my=2 -divClean=1 -divCleanCoeff=10. -x0=-1. -y0=-1. -tf=10. -tp=.1 -projectInterp=1 -go=halt
#   cgmx boxEigen -g=rsise2.order4 -mx=1 -my=2 -divClean=1 -divCleanCoeff=10. -x0=-1. -y0=-1. -tf=10. -tp=.1 -projectInterp=1 -go=halt
# 
#   -- interp and filter generate div(E) but cleaning works away from boundaries 
#   cgmx boxEigen -g=rsise8.order2.hdf -mx=1 -my=2 -divClean=1 -divCleanCoeff=50. -diss=1. -filter=1 -x0=-1. -y0=-1. -tf=50 -tp=.5 -go=halt
# -- parallel: 
#   mpirun -np 2 $cgmxp boxEigen -g=box64.order4.hdf -mx=3 -my=3 -mz=3 
#
#================================================================================================
# 
$tFinal=1.; $tPlot=.1; $diss=.0; $cfl=.95; $dissOrder=-1; $filter=0; $divClean=0; $divCleanCoeff=1; $projectInterp=0;
$grid="box32.order4.hdf"; $method="NFDTD"; 
$cons=0; $go="halt"; $show=" ";
$mx=1; $my=1; $mz=1; $x0=0.; $y0=0.; $z0=0.;  # defines the eigenfunction 
$dm="none"; $gamma=0.; $omegap=0.;  # (gamma,omegap) for Drude model
# $alphaP=1.; $a0=1.; $a1=0.; $b0=0.; $b1=1.;  # GDM parameters
$npv=1; $alphaP=1.; $modeGDM=-1; 
@a0 = (); @a1=(); @b0=(); @b1=(); # these must be null for GetOptions to work, defaults are given below 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"show=s"=>\$show,\
  "dtMax=f"=>\$dtMax,"mx=f"=>\$mx,"my=f"=>\$my,"mz=f"=>\$mz, "cons=i"=>\$cons,"dissOrder=i"=>\$dissOrder,\
  "filter=i"=>\$filter,"divClean=i"=>\$divClean,"divCleanCoeff=f"=>\$divCleanCoeff,\
  "x0=f"=>\$x0,"y0=f"=>\$y0,"z0=f"=>\$z0,"projectInterp=i"=>\$projectInterp,"method=s"=>\$method,\
  "dm=s"=>\$dm,"gamma=f"=>\$gamma,"omegap=f"=>\$omegapn,"modeGDM=i"=>\$modeGDM,\
  "alphaP=f"=>\$alphaP,"a0=f{1,}"=>\@a0,"a1=f{1,}"=>\@a1,"b0=f{1,}"=>\@b0,"b1=f{1,}"=>\@b1,"npv=i"=>\$npv);
# -------------------------------------------------------------------------------------------------
#
if( $dm eq "none" ){ $dm="no dispersion"; }
if( $dm eq"drude" || $dm eq "Drude" ){ $dm="Drude"; }
if( $dm eq"gdm" ){ $dm="GDM"; }
# Give defaults here for array arguments: 
if( $a0[0] eq "" ){ @a0=(1,0,0,0); }
if( $a1[0] eq "" ){ @a1=(0,0,0,0); }
if( $b0[0] eq "" ){ @b0=(0,0,0,0); }
if( $b1[0] eq "" ){ @b1=(0,0,0,0); }
#
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
#
# $grid="nonBox64.order4.hdf"; $tPlot=1.; 
#
$grid
#
# modifiedEquationTimeStepping
#
$method
# dispersion model:
$dm
#
## Drude params $gamma $omegap all (gamma,omegap,domain-name)
## GDM params $a0 $a1 $b0 $b1 all (a0,a1,b0,b1,domain-name)
GDM mode: $modeGDM
$domain="all"; 
$cmd="#"; 
if( $npv == 1 ){ $cmd = "GDM params $a0[0] $a1[0] $b0[0] $b1[0] all (a0,a1,b0,b1,domain-name)"; }
if( $npv == 2 ){ \
   $cmd  = "GDM domain name: $domain\n"; \
   $cmd .= " number of polarization vectors: $npv\n"; \
   $cmd .= " GDM coeff: 0 $a0[0] $a1[0] $b0[0] $b1[0] (eqn, a0,a1,b0,b1)\n"; \
   $cmd .= " GDM coeff: 1 $a0[1] $a1[1] $b0[1] $b1[1] (eqn, a0,a1,b0,b1)"; \
      }
$cmd
#
#**
solve for magnetic field 0
#**
# 
bc: all=perfectElectricalConductor
#**
#****
squareEigenfunctionInitialCondition
 $mx $my $mz $x0 $y0 $z0
squareEigenfunctionKnownSolution
#**
#specify probes
#  .2 .3 0.
#  done
#****
#
tFinal $tFinal
tPlot  $tPlot
cfl  $cfl
use conservative divergence $cons 
#
dissipation $diss
apply filter $filter
order of dissipation $dissOrder
use divergence cleaning $divClean
div cleaning coefficient $divCleanCoeff
project interpolation points $projectInterp
#
#*********************************
show file options...
  MXSF:compressed
  MXSF:open
  $show
  MXSF:frequency to flush 20
exit
#**********************************
continue
$go
