#================================================================================================
#  cgmx example:  Scattering of a plane wave from an embedded body in a dielectric
#
# Usage:
#   
#  cgmx [-noplot] embeddedBody -g=<name> -tf=<tFinal> -tp=<tPlot>  -method=[nfdtd|Yee|sosup] 
#                      -theta=<degrees> -diss=<> -rbc=[abcEM2|abcPML] -debug=<num> -cons=[0/1] ...
#                      -plotIntensity=[0|1] -sidebc=[symmety|abcEM2] ...
#                      -upperbc=[planeWaveBoundaryCondition|abcEM2|abcPML] -go=[run/halt/og]
# Arguments:
#  -angle : angle of incidence of plane wave from the vertical
#      Incident wave is u(x,y,t) = F(2*pi*[ c*t - x*sin(theta) - y*cos(theta) ])
# 
# Examples: 
#   cgmx embeddedBody -g=embeddedBodyGride2.order2 
#
#================================================================================================
# 
$tFinal=10.; $tPlot=.1; $diss=1.; $cfl=.9; $plotIntensity=0;  $method="NFDTD"; 
$sidebc="abcEM2"; 
$projectInitialConditions=0; $projectFields=0; $projectionFrequency=1;  $projectInterp=0;
$theta=60; # angle of incidence of plane wave from the vertical
$grid="embeddedBodyGride2.order2.hdf"; $backGround="square"; 
$cons=0; $go="halt"; 
#
$planeWaveInitialCondition=0; # 1=use plane wave initial condition (inside bounding box)
$planeWaveBoundaryForcing=0; # 1=compute scattered field directly by forcing PEC BC
#$upperbc="planeWaveBoundaryCondition"; 
$upperbc="abcEM2"; 
$ax=0.; $ay=0.; $az=0.; # plane wave coeffs. all zero -> use default
$epsUpper=1.; $muUpper=1.;
$epsLower=6.; $muLower=1.;
$useNewInterface=1; $interfaceIterations=3;
$xa=1.; $xb=100.; $ya=1.; $yb=100.; $za=-100.; $zb=100.;  # initial condition bounding box
$xa=-100.; $xb=100.; $ya=1.001; $yb=100.; $za=-100.; $zb=100.;  # initial condition bounding box
$rbc="abcEM2"; # radiation BC 
$pmlLines=11; $pmlPower=6; $pmlStrength=50.; # pml parameters
# chirp parameters
$ta=.5; $tb=20.; $bandWidth=2.; $beta=10.; $amp=1.; $x0=0.; $y0=0.; $z0=0.; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,"method=s"=>\$method,\
  "dtMax=f"=>\$dtMax,"theta=f"=>\$theta,"plotIntensity=i"=>\$plotIntensity,"cons=i"=>\$cons,\
  "rbc=s"=>\$rbc,"pmlLines=i"=>\$pmlLines,"pmlPower=i"=>\$pmlPower,"pmlStrength=f"=>\$pmlStrength,\
  "xa=f"=>\$xa,"ya=f"=>\$ya,"projectFields=i"=>\$projectFields,"projectionFrequency=i"=>\$projectionFrequency,\
  "projectInterp=i"=>\$projectInterp,"sidebc=s"=>\$sidebc,"upperbc=s"=>\$upperbc,\
  "planeWaveInitialCondition=i"=>\$planeWaveInitialCondition,"planeWaveBoundaryForcing=i"=>\$planeWaveBoundaryForcing,\
  "projectInitialConditions=i"=>\$projectInitialConditions  );
# -------------------------------------------------------------------------------------------------
if( $method eq "sosup" ){ $diss=0.; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
$pi=4.*atan2(1.,1.);
$kx = -sqrt(10)*sin($theta*$pi/180.); 
$ky = -sqrt(10)*cos($theta*$pi/180.); 
$kz=0.; 
#
$grid
#
$method
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
# -- if planeWaveInitialCondition==1 :
#     - use a plane wave initial condition
#     - set top BC to an ABC
#     - adjust ABC boundaries for incident fields (plane wave) so abc applies to scattered field only
#
if( $planeWaveInitialCondition eq 1 ){ $cmd="planeWaveInitialCondition"; }else{ $cmd="zeroInitialCondition"; }
$cmd
#
# planeWaveBoundaryForce: computed scattered field directly, PEC BC is inhomogeneous
#
if( $planeWaveBoundaryForcing eq 1 ){ $cmd="planeWaveBoundaryForcing"; }else{ $cmd="#"; }
$cmd
#
$kya= abs($ky);
# $ya = $yb - int( ($yb-$ya)*$kya +.5 )/$kya;   # we need to clip the plane wave on a period
initial condition bounding box $xa $xb $ya $yb $za $zb
#  damp initial conditions on the bottom of the box: 
bounding box decay face 0 1 
$beta=5.; # exponent in tanh function for smooth transition to zero outside the bounding box
bounding box decay exponent $beta
# 
plane wave coefficients $ax $ay $az $epsUpper $muUpper
#
kx,ky,kz $kx $ky $kz 
#
# -- boundary conditions:
#
bc: all=$sidebc
bc: upperHalfSpace(1,1)=$upperbc
bc: upperHalfSpaceCoarse(1,1)=$upperbc
## bc: upperHalfSpaceCoarse(1,0)=planeWaveBoundaryCondition
bc: bodySquare=perfectElectricalConductor
#
#
use new interface routines $useNewInterface
# NOTE: material interfaces have share>=100
# coefficients $epsUpper $muUpper upperHalfSpace* (eps,mu,grid-name)
# coefficients $epsLower $muLower lowerHalfSpace* (eps,mu,grid-name)
# coefficients $epsLower $muLower body* (eps,mu,grid-name)
coefficients $epsUpper $muUpper upperDomain (eps,mu,grid-name)
coefficients $epsLower $muLower lowerDomain (eps,mu,grid-name)
#
interface BC iterations $interfaceIterations
#
# -- pml parameters:
pml width,strength,power $pmlLines $pmlStrength $pmlPower
#
tFinal $tFinal
tPlot  $tPlot
# 
# -- we need to subtract out the incident field on the "inflow" boundary before
#    applying the radiation boundary condition: 
if( $planeWaveInitialCondition eq 1 ){ $adjustFields=1; }else{ $adjustFields=0;  }
adjust boundaries for incident field 0 all
adjust boundaries for incident field $adjustFields upperHalfSpace
adjust boundaries for incident field $adjustFields upperHalfSpaceCoarse
# 
dissipation  $diss
#
# Optional project the divergence
project initial conditions $projectInitialConditions
project fields $projectFields
projection frequency $projectionFrequency
project interpolation points $projectInterp
#***********************
#* slow start interval 1.
slow start interval -1.
#***********************
divergence damping $divDamping
#***********************
cfl $cfl
#
use conservative divergence $cons 
# plot scattered field 1
plot total field 0
plot errors 0
check errors 0
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
plot:Ex
contour
plot contour lines (toggle)
min max -0.25 0.25
vertical scale factor 0.
exit 
$go 

movie and save
images/ebExChirpBig


