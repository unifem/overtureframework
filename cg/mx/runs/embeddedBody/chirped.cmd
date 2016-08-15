#================================================================================================
#  cgmx example:  Scattering of a chirped  plane wave from different bodies
#
# Usage:
#   
#  cgmx [-noplot] chirped -g=<name> -tf=<tFinal> -tp=<tPlot> -boundaryForing=[0|1] -option=[|cyl]
#                      -kx=<num> -ky=<num> -kz=<num> -diss=<>  -method=[nfdtd|Yee|sosup] ...
#                      -bg=<back-ground> -rbc=[abcEM2|abcPML] -debug=<num> -cons=[0/1] ...
#                      -plotIntensity=[0|1] -go=[run/halt/og]
# Arguments:
#  -boundaryForcing=1 : perform scattering directly by subtracting out the plane wave solution; this results
#                       in a inhomogenoeous PEC boundary condition.
#  -boundaryForcing=0 :  incident wave front hits the body: 
#  -kx= -ky= -kz= : integer wave numbers of the incident wave
#  -bg= : name of the background grid
# 
# Examples: 
#   cgmx scattering -g=cice3.order4.hdf -bg=square
#   cgmx scattering -g=trianglee4.order4.hdf -bg=backGround           (scattering from a rounded triangle)
#   cgmx scattering -g=trianglee8.order4.hdf -bg=backGround -plotIntensity=1
#   cgmx scattering -g=crve4.order4.hdf -bg=backGround                (scattering from a space reentry vehicle )
#
# -- computed scattered field directly using boundaryForcing=1: --------
#    -- scattering from a cylinder
#    ogen -noplot cicArg -order=4 -interp=e -factor=4 
#    cgmx scattering -g=cice4.order4.hdf  -bg=square -rbc=abcPML -boundaryForcing=1
# 
#     -- scattering from a sphere with zero initial conditions and PML far-field
#    ogen -noplot sphereInABox -order=4 -factor=2 
#    cgmx scattering -g=sphereInABoxe2.order4 -bg=backGround -rbc=abcPML -boundaryForcing=1
# 
# parallel: 
#   mpirun -np 2 $cgmxp scattering -g=trianglee8.order4.hdf -bg=backGround
#
#================================================================================================
# 
$tFinal=4.; $tPlot=.1; $diss=0.; $cfl=.9; $plotIntensity=0; $boundaryForcing=0; $method="NFDTD";
$kx=1; $ky=0; $kz=0;
$grid="sib1.order4.hdf"; $backGround="square"; $option=""; 
$cons=0; $go="halt"; 
$ax=0.; $ay=0.; $az=0.; # plane wave coeffs. all zero -> use default
$eps=1.; $mu=1.;
$xa=-100.; $xb=-1.; $ya=-100.; $yb=100.; $za=-100.; $zb=100.;  # initial condition bounding box
$rbc="abcEM2"; # radiation BC 
$pmlLines=11; $pmlPower=6; $pmlStrength=50.; # pml parameters
# chirp parameters
$ta=.5; $tb=2.; $bandWidth=2.; $beta=10.; $amp=1.; $x0=0.; $y0=0.; $z0=0.; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"method=s"=>\$method,\
  "dtMax=f"=>\$dtMax,"kx=i"=>\$kx,"ky=i"=>\$ky,"kz=i"=>\$kz,"plotIntensity=i"=>\$plotIntensity, "cons=i"=>\$cons,\
  "rbc=s"=>\$rbc,"pmlLines=i"=>\$pmlLines,"pmlPower=i"=>\$pmlPower,"pmlStrength=f"=>\$pmlStrength,\
 "xb=f"=>\$xb,"boundaryForcing=i"=>\$boundaryForcing,"option=s"=>\$option,\
  "bandWidth=f"=>\$bandWidth,"beta=f"=>\$beta,"ta=f"=>\$ta,"tb=f"=>\$tb,"x0=f"=>\$x0,"y0=f"=>\$y0,"z0=f"=>\$z0 );
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
#
$grid
#
$method
#  boundaryForcing=1 :  Adjust PEC boundaries to account for a plane wave being subtracted out
if( $boundaryForcing eq 0 ){ $cmd="planeWaveInitialCondition"; }else{ $cmd="planeWaveBoundaryForcing\n zeroInitialCondition"; }
# $cmd
# *********************
zeroInitialCondition
#
chirpedPlaneWaveBoundaryForcing
#
# phi = omega0*t + alpha*t^2,   Chi = tanh(beta*(t-ta)) - ...
$alpha=$bandWidth/(2.*($tb-$ta)); 
chirp parameters $ta $tb $alpha $beta $amp $x0 $y0 $z0
#
$kxa= abs($kx);
$xb = $xa + int( ($xb-$xa)*$kxa +.5 )/$kxa;   # we need to clip the plane wave on a period
if( $boundaryForcing eq 0 ){ $cmd="initial condition bounding box $xa $xb $ya $yb $za $zb"; }else{ $cmd="#"; }
$cmd
$beta=10.; # exponent in tanh function for smooth transition to zero outside the bounding box
bounding box decay exponent $beta
# 
plane wave coefficients $ax $ay $az $eps $mu
# ====
#  Initial condition:
## zeroInitialCondition
# ====
user defined known solution
  chirped plane wave
done
#
kx,ky,kz $kx $ky $kz 
#
# -- boundary conditions:
#
bc: all=perfectElectricalConductor
## bc: $backGround=$rbc
# if( $boundaryForcing eq 0 ){ $cmd="bc: $backGround(0,0)=planeWaveBoundaryCondition"; }else{ $cmd="#"; }
# $cmd 
bc: $backGround(1,0)=abcEM2
bc: $backGround(0,1)=symmetry
bc: $backGround(1,1)=symmetry
if( $option eq "cyl" ){ $cmd="bc: $backGround=$rbc"; }else{ $cmd="#"; }
$cmd 
#
# -- pml parameters:
pml width,strength,power $pmlLines $pmlStrength $pmlPower
#
tFinal $tFinal
tPlot  $tPlot
# 
# -- we need to subtract out the incident field on the "inflow" boundary before
#    applying the radiation boundary condition: 
#**if( $boundaryForcing eq 1 ){ $adjustFields=0; }else{ $adjustFields=1; }
#** adjust boundaries for incident field 0 all
#** adjust boundaries for incident field $adjustFields $backGround
# 
dissipation  $diss
#***********************
#* slow start interval 1.
slow start interval -1.
#***********************
# divergence damping  0.0025   .02  1. .005 .001 .0025 .01
#***********************
cfl $cfl
#
use conservative divergence $cons 
# plot scattered field 1
if( $boundaryForcing eq 1 ){ $plotTotalField=1; }else{ $plotTotalField=0; }
plot total field $plotTotalField
plot errors 1
check errors 1
plot intensity $plotIntensity
#*********************************
show file options...
  MXSF:compressed
  MXSF:open
    $show
  MXSF:frequency to flush 2
exit
#**********************************
# $omega= sqrt( $kx*$kx + $ky*$ky + $kz*$kz );
# time harmonic omega $omega (omega/(2pi), normally c*|k|^2
#
continue
plot:Ey
contour
plot contour lines (toggle)
exit
$go 

