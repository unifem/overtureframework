#================================================================================================
#
#  cgmx example:  scattering from 2d dielectric rod
#
# Usage:
#   
#  cgmx [-noplot] rod2d -g=<name> -tf=<tFinal> -tp=<tPlot> -kx=<num> -ky=<num> -kz=<num> ...
#                 -plotIntensity=[0|1] -diss=<>  -filter=[0|1] -debug=<num> -cons=[0/1] -varDiss=<0|1> ...
#                 -rbc=[abcEM2|rbcNonLocal|abcPML] -leftBC=[rbc|planeWave] -method=[fd|Yee|sosup] 
#                 -probeFileName=<s> -xLeftProbe=<f> -xRightProbe=<f> ...
#                 -useSosupDissipation=[0|1] -dm=[none|gdm] -go=[run/halt/og]
#
# Arguments:
#  -kx= -ky= -kz= : integer wave number of the incident wave
#  -varDiss :  if 1, use variable dissipation (only add dissipation near interpolation points)
#  -dm : dispersion model
# 
# Notes: 
# refractive index from vacuum to glass is 1.5
#       $epsGlass = 1.5^2 = 2.25
# 
#   pts/wave-length = (1/ds)*(1/ky) = factor*100/ky 
# 
# NOTE: diss=.5 was too small for fine grid runs;  diss=5. works but a smaller value may also be ok. 
# 
#     glass  n=1.5 -> eps1= (1.5)^2 
#     silicon  n=3.45 -> eps1= (3.45)^2 =11.9025
# Examples:
#  ---- ROD ----
#   Grid:  ogen -noplot rodGrid2d -order=2 -interp=e -factor=8 
#   Run: cgmx rod2d -g=rodGride8.order2.hdf -eps1=2.25 -eps2=1. -kx=5 -diss=1. -tp=.1 -tf=10 -go=halt
#   cgmx rod2d -g=rodGride8.order2.hdf -eps1=11.9025 -eps2=1. -kx=2 -diss=1. -tp=.1 -tf=10 -go=halt
# 
# ---- DISK -----
# GRID: ogen -noplot io -prefix=diskInBox -order=4 -interp=e -xa=-2. -xb=2. -ya=-.75 -yb=.75 -outerBC=yPeriodic -factor=4
# RUN: cgmx dielectricBodies -g=diskInBoxYpe4.order4.hdf -backGround=outerSquare -kx=4 -eps1=11.9025 -eps2=1. -go=halt
#================================================================================================
# 
$tFinal=5; $tPlot=.1; $diss=1.; $filter=0; $dissOrder=-1; $cfl=.9; $varDiss=0; $varDissSmooths=20; $sidebc="symmetry"; 
$kx=1; $ky=0; $kz=0; $plotIntensity=0; $intensityOption=1; $checkErrors=0; $method="NFDTD"; $dm="none"; 
$ax=0.; $ay=0.; $az=0.; # plane wave coeffs. all zero -> use default
$numBlocks=0; # 0 = default case of scattering from a "innerDomain" 
$eps0=1.; $mu0=1.; # outer domain 
$eps1=1.; $mu1=1.; # block 1 
$eps2=1.; $mu2=1.; # block 2 
$eps3=1.; $mu3=1.; # block 3 
$eps4=1.; $mu4=1.; # block 4 
$show=" "; $backGround="backGround";
$interfaceEquationOption=1; $interfaceIterations=5;  $interfaceOmega=.5; $useNewInterface=1; 
$grid="afm2.order4.hdf";
$cons=1; $go="halt"; 
$xa=-100.; $xb=-1.5; $ya=-100.; $yb=100.; $za=-100.; $zb=100.;  # initial condition bounding box
$leftBC="rbc"; $bcBody=""; 
$probeFileName="probeFile"; $xLeftProbe=-1.5; $xRightProbe=1.5; $yLeftProbe=-.2; $yRightProbe=.2; 
$xar=-2.; $xbr=-1.; # reflection probe x-bounds
$xat= 1.; $xbt= 2.; # transmission probe x-bounds 
$rbc="abcEM2"; $pmlLines=11; $pmlPower=6; $pmlStrength=50.; 
$useSosupDissipation=0;  $sosupDissipationOption=0;
$stageOption="";
$alphaP=1.; $a0=1.; $a1=0.; $b0=0.; $b1=1.;  # GDM parameters
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"diss=f"=>\$diss,"tp=f"=>\$tPlot,"show=s"=>\$show,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,\
   "plotIntensity=i"=>\$plotIntensity,"ax=f"=>\$ax,"ay=f"=>\$ay,"az=f"=>\$az,"intensityOption=i"=>\$intensityOption,\
  "dtMax=f"=>\$dtMax,"kx=f"=>\$kx,"ky=f"=>\$ky,"kz=f"=>\$kz, "numBlocks=i"=>\$numBlocks,\
   "ii=i"=>\$interfaceIterations,"varDiss=i"=>\$varDiss ,"varDissSmooths=i"=>\$varDissSmooths,\
   "xb=f"=>\$xb,"yb=f"=>\$yb,"stageOption=s"=>\$stageOption,"cons=i"=>\$cons,\
   "probeFileName=s"=>\$probeFileName,"xLeftProbe=f"=>\$xLeftProbe,"xRightProbe=f"=>\$xRightProbe,\
   "yLeftProbe=f"=>\$yLeftProbe,"yRightProbe=f"=>\$yRightProbe,\
   "checkErrors=i"=>\$checkErrors,"sidebc=s"=>\$sidebc,"dissOrder=i"=>\$dissOrder,"method=s"=>\$method,\
   "filter=i"=>\$filter, "backGround=s"=>\$backGround,"rbc=s"=>\$rbc,"pmlLines=i"=>\$pmlLines,\
   "pmlPower=i"=>\$pmlPower,"pmlStrength=f"=>\$pmlStrength,"leftBC=s"=>\$leftBC,"bcBody=s"=>\$bcBody,\
   "useSosupDissipation=i"=>\$useSosupDissipation,"sosupDissipationOption=i"=>\$sosupDissipationOption,\
   "eps0=f"=>\$eps0,"eps1=f"=>\$eps1,"eps2=f"=>\$eps2,"eps3=f"=>\$eps3,"eps4=f"=>\$eps4,\
   "xar=f"=>\$xar,"xbr=f"=>\$xbr,"xat=f"=>\$xat,"xbt=f"=>\$xbt,"dm=s"=>\$dmn,\
   "alphaP=f"=>\$alphaP,"a0=f"=>\$a0,"a1=f"=>\$a1,"b0=f"=>\$b0,"b1=f"=>\$b1   );
# -------------------------------------------------------------------------------------------------
if( $dm eq "none" ){ $dm="no dispersion"; }
if( $dm eq"drude" || $dm eq "Drude" ){ $dm="Drude"; }
if( $dm eq"gdm" ){ $dm="GDM"; }
#
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
# dispersion model:
$dm
# 
# Drude params 1 1 all (gamma,omegap,domain-name)
if( $numBlocks eq 0 ){ $cmd="GDM params $a0 $a1 $b0 $b1 innerDomain (a0,a1,b0,b1,domain-name)\n"; }\
else{\
  $cmd="#"; \
  for( $i=0; $i<$numBlocks; $i++ ){ $cmd .= "\n GDM params $a0 $a1 $b0 $b1 blockDomain$i (a0,a1,b0,b1,domain-name)"; }\
}
$cmd 
#
# Set the stage option for sosup dissipation: 
$stages="#"; 
if( $useSosupDissipation ne 0 && $stageOption eq "" ){ $stageOption="D-IB"; } # default for sosup
# --- Define multi-stage time-step: 
if( $stageOption eq "IDB" ){ $stages="updateInterior,addDissipation,applyBC"; }
if( $stageOption eq "D-IB" ){ $stages="addDissipation\n updateInterior,applyBC"; }
if( $stageOption eq "DB-IB" ){ $stages="addDissipation,applyBC\n updateInterior,applyBC"; }
if( $stageOption eq "IB-DB" ){ $stages="updateInterior,applyBC\n addDissipation,applyBC"; }
if( $stageOption eq "IB-D" ){ $stages="updateInterior,applyBC\n addDissipation"; }
# -- options to precompute V=uDot used in the dissipation
if( $stageOption eq "IVDB" ){ $stages="updateInterior,computeUt,addDissipation,applyBC"; }
if( $stageOption eq "VD-IB" ){ $stages="computeUt,addDissipation\n updateInterior,applyBC"; }
if( $stageOption eq "VDB-IB" ){ $stages="computeUt,addDissipation,applyBC\n updateInterior,applyBC"; }
if( $stageOption eq "IB-VDB" ){ $stages="updateInterior,applyBC\n computeUt,addDissipation,applyBC"; }
if( $stageOption eq "IB-VD" ){ $stages="updateInterior,applyBC\n computeUt,addDissipation"; }
if( $stages ne "#" ){ $cmd="set stages...\n $stages\n done"; }else{ $cmd="#"; }
$cmd
#
# planeWaveInitialCondition
if( $leftBC eq "rbc" ){ $cmd = "planeWaveInitialCondition"; }else{ $cmd="zeroInitialCondition"; }
$cmd 
if( $checkErrors ){ $known="planeWaveKnownSolution"; }else{ $known="#"; }
$known
$kxa= abs($kx);
if( $kxa > 1. ){ $xb = int( $xb*$kxa +.5 )/$kxa; }  # we need to clip the plane wave on a period
if( $kx < 0 ){ $xa=$xb; $xb=100.; }
if( $leftBC eq "rbc" ){ $cmd="initial condition bounding box $xa $xb $ya $yb $za $zb"; }else{ $cmd="#"; }
$cmd
# initial condition bounding box $xa $xb $ya $yb $za $zb
#  damp initial conditions at face (side,axis)=(0,1) of the box
bounding box decay face 1 0 
$beta=5.; # exponent in tanh function for smooth transition to zero outside the bounding box
bounding box decay exponent $beta
# 
$epsPW=$eps0; $muPW=$mu0; # parameters for the incident plane wave
plane wave coefficients $ax $ay $az $epsPW $muPW
#
use new interface routines $useNewInterface
# zeroInitialCondition
# ====
# planeWaveScatteredFieldInitialCondition
# ====
# twilightZone
#  degreeSpace, degreeTime  1 1
#
kx,ky,kz $kx $ky $kz
#
# bc: all=dirichlet
# bc: all=perfectElectricalConductor
bc: all=$rbc
#
if( $leftBC eq "planeWave" ){ $cmd="bc: $backGround(0,0)=planeWaveBoundaryCondition"; }else{ $cmd="#"; }
$cmd 
if( $bcBody eq "pec" ){ $cmd="bc: annulus=perfectElectricalConductor"; }else{ $cmd="#"; }
$cmd
#
pml width,strength,power $pmlLines $pmlStrength $pmlPower
# 
# -- we need to subtract out the incident field on the "inflow" boundary before
#    applying the radiation boundary condition: 
if( $leftBC eq "planeWave" ){ $adjustFields=0; }else{ $adjustFields=1; }
adjust boundaries for incident field $adjustFields all
adjust boundaries for incident field $adjustFields $backGround
# 
# NOTE: material interfaces have share>=100
#
#  -- Assign material parameters ---
#  For multi-block grids we assume domain names of blockDomain0, blockDomain1, etc. 
#
@epsv = ( $eps1, $eps2, $eps3, $eps4 );
@muv = ( $mu1, $mu2, $mu3, $mu4 );
if( $numBlocks eq 0 ){ $cmd="coefficients $eps1 $mu1 innerDomain   (eps,mu,grid/domain-name)\n"; }\
else{\
  $cmd="#"; \
  for( $i=0; $i<$numBlocks; $i++ ){ $cmd .= "\n coefficients $epsv[$i] $muv[$i] blockDomain$i   (eps,mu,grid/domain-name)"; }\
}
$cmd 
coefficients $eps0 $mu0 outerDomain   (eps,mu,grid/domain-name)
#
# *****************
# for Yee we define the material regions
# if( $method eq "Yee" ){ $cmds = "define embedded bodies\n PEC cylinder\n $rad $x0 $y0 $z0\n exit"; }else{ $cmds="#"; }
if( $method eq "Yee" ){ $cmds = "define embedded bodies\n plane material interface\n 1 0 0 0 0 0\n $eps1 $mu1 0 0\n exit"; }else{ $cmds="#"; }
$cmds 
# ****************
#
interface BC iterations $interfaceIterations
# interfaceEquationsOption=0 : use extrap for 2nd ghost, 1=use eqns
interface equations option $interfaceEquationOption
omega for interface iterations $interfaceOmega
#
# bc: Annulus=perfectElectricalConductor
tFinal $tFinal
tPlot  $tPlot
#
apply filter $filter
order of dissipation $dissOrder
dissipation $diss
#
use sosup dissipation $useSosupDissipation
sosup dissipation option $sosupDissipationOption
#*********************************
show file options...
  MXSF:compressed
  MXSF:open
    $show
 # MXSF:frequency to save 
  MXSF:frequency to flush 101 
exit
#**********************************
#
use variable dissipation $varDiss
number of variable dissipation smooths $varDissSmooths
use conservative difference $cons
# order of dissipation 4
debug 0
#
cfl $cfl 
plot errors $checkErrors
check errors $checkErrors
plot intensity $plotIntensity
intensity option $intensityOption
$intensityAveragingInterval=1.;
intensity averaging interval $intensityAveragingInterval
# $c1 = 1./sqrt($eps1*$mu1); $omega= $c1*sqrt( $kx*$kx + $ky*$ky + $kz*$kz );
# time harmonic omega $omega (omega/(2pi), normally c*|k|
#
# output probes every time step: 
probe frequency 1
#
# --- *NEW* USER DEFINED PROBE
create user defined probe...
  $probeFile ="reflection$probeFileName.dat"; 
  file name $probeFile
  probe box $xar $xbr -2 2 -1 1 (xa,xb, ya,yb, za,zb)
  probe name reflectionProbe
  $L=-.25; # reflection coefficient is centered here 
  R/T offset $L
  incident amplitude 1.
  incident phase 0.
  reflection probe
exit
# 
create user defined probe...
  $probeFile ="transmission$probeFileName.dat"; 
  file name $probeFile
  probe box $xat $xbt -2 2 -1 1 (xa,xb, ya,yb, za,zb)
  probe name reflectionProbe
  $L=.25; # reflection coefficient is centered here 
  R/T offset $L
  transmission probe
exit
# 
#- # box probe
#- create a probe... 
#- open graphics
#- 
#- region probe
#- 
#- bounding box...
#-   bounding box grid 0
#-   bounding box 10 15 0 40 (i1a,i1b, i2a,i2b, i3a,i3b)
#-   number of layers 2
#- exit
#- probe name boxProbe
#- file name boxProbeFile.dat
#- sum
#- exit
# 
# Point probe: 
create a probe...
  $leftProbeFileName="left$probeFileName.dat"; 
  file name $leftProbeFileName
  probe name leftProbe
  nearest grid point to $xLeftProbe $yLeftProbe 0
exit
create a probe...
  $rightProbeFileName="right$probeFileName.dat"; 
  file name $rightProbeFileName
  probe name rightProbe
  nearest grid point to $xRightProbe $yRightProbe 0
exit
# OLD: 
# probe file: probeFile.dat
# specify probes
#   -1.5 0 0 
#    1.5 .0 0
# done
#
continue
#
#
plot:Ey
contour
  plot contour lines (toggle)
  # vertical scale factor 0.2
  # min max -1.1 1.1
exit
$go


plot:intensity
contour
  plot contour lines (toggle)
 # vertical scale factor 0.
 # min max .9 1.1
exit
$go


#
plot:Ex
contour
plot contour lines (toggle)
vertical scale factor 0.
# min max -1.1 1.1
exit
$go
