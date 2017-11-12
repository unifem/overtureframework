# 
# cgmx: test for material interfaces
#
# Usage:
#    cgmx [-noplot] interface -g=<name> -tf=<tFinal> -tp=<tPlot> -cfl=<> -diss=<> -eps1=<> -eps2=<> -ic=<> ...
#                   -bc=[abcEM2|rbcNonLocal|abcPML|perfectElectricalConductor|symmetry] ...
#                   -useNewInterface=[0|1] -method=[nfdtd|Yee] -errorNorm=[0|1|2] -interfaceIts=<i> ...
#                   -dm=[none|gdm] -npv=i i 
# 
# Examples:
# 
# -- 2nd order --
#  cgmx interface -g=twoSquaresInterfacee16.order2 -eps2=.25 -kx=4 -tp=.1 -ic=pmic -bc=d
#  cgmx noplot interface -g=twoSquaresInterfacee1.order2 -eps2=.25 -kx=4 -tf=.01 -tp=.01 -ic=pmic -bc=d -debug=3 -useNewInterface=1 >! junk
#  srun -ppdebug -N1 -n1 $cgmxp noplot interface -g=twoSquaresInterfacee4.order2 -eps2=.25 -kx=2 -tf=.01 -tp=.01 -ic=pmic -bc=d -debug=3 -useNewInterface=1 
#  -- problem here: err(Ex)!=0 for n>1 : 
# srun -ppdebug -N1 -n2 $cgmxp noplot interface -g=twoSquaresInterfacee1.order2 -eps2=.25 -kx=2 -tf=.02 -tp=.01 -ic=pmic -bc=d -useNewInterface=1 -debug=7
# -- 4th order --
#  cgmx interface -g=twoSquaresInterface8.order4 -eps2=.25 -kx=4 -tp=.1 -ic=pmic -bc=d -tf=.5 -ax=0. -ay=1. -go=halt
#  cgmx interface -g=twoSquaresInterface8.order4 -eps2=.25 -kx=4 -tp=.1 -ic=pmic -bc=d
#  cgmx noplot interface -g=twoSquaresInterfacee1.order4 -eps2=.25 -kx=4 -tf=.01 -tp=.01 -ic=pmic -bc=d -debug=3 -interfaceIts=20 -useNewInterface=0 >! junk0
# 
# --- Yee .hdf
#  cgmx interface -g=square64  -method=Yee -eps2=.25 -kx=4 -tp=.1 -ic=pmic -bc=d -ip=".5 0. 0."
#  cgmx interface -g=square128 -method=Yee -eps2=.25 -kx=4 -tp=.1 -ic=pmic -bc=d -ip=".5 0. 0."
#  cgmx interface -g=square128 -method=Yee -eps2=2.25 -kx=4 -tp=.1 -ic=pmic -bc=d -ip=".5 .5 0." -in="1. -.2 0." -errorNorm=1
#  cgmx interface -g=square512 -method=Yee -eps2=2.25 -kx=4 -tp=.1 -ic=pmic -bc=d -ip=".5 .5 0." -in="1. -.2 0." -errorNorm=1
#
#  cgmx interface -g=bigSquareSize1f4 -method=Yee -eps2=2.25 -kx=4 -tp=.1 -ic=pmic -bc=d -ip=".0 .0 0." -in="1. -1. 0." -errorNorm=1
#  cgmx interface -g=box40 -method=Yee -eps2=2.25 -kx=4 -tp=.1 -ic=pmic -bc=d -ip=".5 .5 .5" -in="1. 0. 0." -errorNorm=1
# 
# -- 3d 2nd-order --
#   cgmx interface -g=twoBoxesInterfacee111.order2 -eps2=1. -kx=1 -ic=pmic -bc=d -tp=.01 -tf=.01 -left=leftBox -right=rightBox -debug=1
#   cgmx noplot interface -g=twoBoxesInterfacee111.order2 -eps2=1. -kx=1 -ic=pmic -bc=d -tp=.01 -tf=.01 -left=leftBox -right=rightBox -debug=3 >! junk
#   cgmx noplot interface -g=twoBoxesInterfacee111.order2 -eps2=.25 -kx=1 -ic=pmic -bc=d -tp=.01 -tf=1. -left=leftBox -right=rightBox -debug=3 >! junk
#   srun -ppdebug -N1 -n1 $cgmxp noplot interface -g=twoBoxesInterfacee444.order2 -eps2=.25 -kx=1 -tp=.1 -ic=pmic -bc=d -tp=.01 -tf=.1 -left=leftBox -right=rightBox 
#   -- 3D order=4
#      ... test new:
#      cgmx interface -g=twoBoxesInterfacee222.order4 -eps2=.25 -kx=1 -tf=1. -tp=.01 -ic=pmic -bc=d -diss=0. -left=leftBox -right=rightBox -debug=7 -interfaceEquationOption=1 -interfaceIts=5 -interfaceOmega=1.
#      cgmx interface -g=twoBoxesInterfacee222.order4p -eps2=1. -tf=1. -tp=.001 -ic=pmic -bc=d -diss=0. -left=leftBox -right=rightBox -debug=7 -interfaceEquationOption=1 -interfaceIts=5 -interfaceOmega=.5
#
#  ++ compare to 2d cgmx interface -g=twoSquaresInterface2.order4 -eps2=.25 -kx=1 -tp=.01 -ic=pmic -bc=d -tf=.5 -useNewInterface=1 -debug=7 -interfaceIts=5 -go=halt
#   
#    cgmx  interface -g=twoBoxesInterfacee111.order4 -eps2=.25 -tf=1. -tp=.1 -ic=pmic -bc=d -diss=0. -left=leftBox -right=rightBox -debug=1 
#    cgmx  interface -g=twoBoxesInterfacee222.order4 -eps2=.25 -tf=1. -tp=.1 -ic=pmic -bc=d -diss=0. -left=leftBox -right=rightBox -debug=1 -interfaceEquationOption=0
#    cgmx  interface -g=twoBoxesInterfacee444.order4 -eps2=.25 -tf=1. -tp=.1 -ic=pmic -bc=d -diss=0. -left=leftBox -right=rightBox -go=halt
#    cgmx noplot interface -g=twoBoxesInterfacee222.order4 -eps2=.25 -kx=2 -ax=0. -ay=1 -tf=.5 -tp=.1 -ic=pmic -bc=d -diss=0. -left=leftBox -right=rightBox -go=go > twoBoxesInterfacee222.out
#    cgmx noplot interface -g=twoBoxesInterfacee444.order4 -eps2=.25 -kx=2 -ax=0. -ay=1 -tf=.5 -tp=.1 -ic=pmic -bc=d -diss=0. -left=leftBox -right=rightBox -go=go > twoBoxesInterfacee444.out
#   -- more general incident wave:
#    cgmx noplot interface -g=twoBoxesInterfacee222.order4 -eps2=2.25 -kx=1 -ky=1 -kz=0 -ax=1. -ay=-1 -az=1. -tf=.5 -tp=.1 -ic=pmic -bc=d -diss=0. -left=leftBox -right=rightBox -go=go
#    cgmx noplot interface -g=twoBoxesInterfacee444.order4 -eps2=2.25 -kx=1 -ky=1 -kz=0 -ax=1. -ay=-1 -az=1. -tf=.5 -tp=.1 -ic=pmic -bc=d -diss=0. -left=leftBox -right=rightBox -go=go
#  srun -ppdebug -N1 -n2 $cgmxp interface -g=twoBoxesInterfacee222.order4 -eps2=2.25 -kx=1 -ky=1 -kz=0 -ax=1. -ay=-1 -az=1. -tf=.5 -tp=.1 -ic=pmic -bc=d -diss=0. -left=leftBox -right=rightBox -go=halt
#
# --- TZ:
# srun -ppdebug -N1 -n1 $cgmxp noplot interface -g=twoSquaresInterface0 -eps2=.25 -tf=.02 -tp=.01 -ic=tz -bc=d -diss=0. -useNewInterface=1 -debug=1 -degreex=2 -degreet=2 -debug=7 >! junk
# cgmx noplot interface -g=twoSquaresInterface0 -eps2=1. -tf=.02 -tp=.01 -ic=tz -bc=d -diss=0. -useNewInterface=1 -debug=1 -degreex=1 -degreet=0 -debug=7 >! junk
#  srun -ppdebug -N1 -n1 $cgmxp noplot interface -g=twoSquaresInterfacee1.order2 -eps2=.25 -tf=.02 -tp=.01 -ic=tz -bc=d -diss=0. -useNewInterface=1 -debug=1 -degreex=2 -degreet=2  [exact]
#  cgmx noplot interface -g=twoSquaresInterfacee1.order2 -eps2=.25 -tf=.2 -tp=.1 -ic=tz -bc=d -diss=0. -useNewInterface=1 -debug=1 -degreex=2 -degreet=2  [exact]
# srun -ppdebug -N1 -n2 $cgmxp noplot interface -g=twoSquaresInterfacee4.order2 -eps2=1. -tf=.2 -tp=.1 -ic=tz -bc=d -diss=0. -useNewInterface=1 -degreex=2 -degreet=2 -debug=1
#   srun -ppdebug -N1 -n1 $cgmxp noplot interface -g=twoBoxesInterfacee111.order2 -eps2=.25 -tf=.2 -tp=.1 -ic=tz -bc=d -diss=0. -left=leftBox -right=rightBox -debug=1 -degreex=2 -degreet=2   [exact]
#   srun -ppdebug -N1 -n1 $cgmxp noplot interface -g=twoBoxesInterfacee444.order2 -eps2=.25 -tf=.2 -tp=.1 -ic=tz -bc=d -diss=0. -left=leftBox -right=rightBox -debug=1 -degreex=2 -degreet=2   [exact]
#
#  -- 3D order=4 TZ
#    cgmx  interface -g=twoBoxesInterfacee111.order4 -eps2=.25 -tf=.5 -tp=.1 -ic=tz -bc=d -diss=0. -left=leftBox -right=rightBox -debug=1 -degreex=4 -degreet=4  [ exact]
#      **new: 
#    cgmx interface -g=twoBoxesInterfacee111.order4 -eps2=.25 -tf=.5 -tp=.01 -ic=tz -bc=d -diss=0. -left=leftBox -right=rightBox -debug=1 -degreex=4 -degreet=4
# 
#* mpirun -np 1 $cgmxp noplot interface -g=twoSquaresInterfacee1.order2 -eps2=.25 -tf=.2 -tp=.1 -ic=tz -bc=d -diss=0. -useNewInterface=1 -debug=1 -degreex=2 -degreet=2
#
#  cgmx interface -g=twoSquaresInterfacee1.order4 -eps2=.25 -tf=.2 -tp=.05 -ic=tz -bc=d -diss=0. -useNewInterface=1 -debug=1 -degreex=4 -degreet=4  [exact]
# srun -ppdebug -N1 -n2 $cgmxp noplot interface -g=twoSquaresInterfacee1.order2 -eps2=.25 -tf=.2 -tp=.1 -ic=tz -bc=d -diss=0. -useNewInterface=1 -debug=1 -degreex=2 -degreet=2
# 
#  -- TZ and rotated:
#  cgmx interface -g=twoSquaresInterfaceRotated1.order2 -eps2=.25 -tp=.01 -ic=tz -bc=d -diss=0. -useNewInterface=1 -debug=1 -degreex=2 -degreet=2  [exact]
#  cgmx interface -g=twoSquaresInterfaceRotated1.order4 -eps2=.25 -tp=.01 -ic=tz -bc=d -diss=0. -useNewInterface=1 -debug=1 -degreex=4 -degreet=4  [exact]
#  
#  cgmx interface -g=twoBoxesInterfaceRotated1.order2.hdf -eps2=.25 -tp=.1 -ic=tz -bc=d -diss=0. -left=leftBox -right=rightBox -debug=1 -degreex=2 -degreet=2   [exact]
#  
# -- set default values for parameters ---
$kx=2; $ky=0; $kz=0; $left="leftSquare*"; $right="rightSquare*"; $degreex=2; $degreet=2; $method="NFDTD";
$tFinal=5.; $tPlot=.2; $cfl=.9; $show=" "; $interfaceIts=3; $debug=0; $diss=.1; $dissOrder=-1;
$useNewInterface=1; $errorNorm=0; $interfaceEquationOption=1; $interfaceOmega=.7; $setDivergenceAtInterfaces=0; 
$useImpedanceInterfaceProjection=1; 
$eps1=1.; $mu1=1.;
$eps2=1.; $mu2=1.;
$ax=0.; $ay=0.; $az=0.; # plane wave coeffs. all zero -> use default
$ic = "zeroInitialCondition"; $bc1=""; $bc2=""; $bc3=""; $bc4=""; $bc5=""; $bc6=""; $bc7=""; $bc8=""; 
$pmic = "planeMaterialInterfaceInitialCondition";
$bc = "perfectElectricalConductor";
$interfaceNormal = "1. 0. 0.";
$interfacePoint = ".0 0. 0.";
$tz = "#"; $go="halt";
$dm="none"; @npv=();  $modeGDM=-1; 
$alphaP = 1.; # there is only one alphaP 
@a01 = (); @a11=(); @b01=(); @b11=(); # these must be null for GetOptions to work, defaults are given below
@a02 = (); @a12=(); @b02=(); @b12=(); 
# ----------------------------- get command line arguments ---------------------------------------
#  -- first get any commonly used options: (requires the env variable CG to be set)
# $getCommonOptions = "$ENV{'CG'}/mp/cmd/getCommonOptions.h";
# include $getCommonOptions
#  -- now get additional options: 
GetOptions("bc=s"=>\$bc,"cfl=f"=>\$cfl,"debug=i"=>\$debug,"diss=f"=>\$diss,"eps1=f"=>\$eps1,"eps2=f"=>\$eps2,\
           "go=s"=>\$go,"g=s"=>\$grid,"ic=s"=>\$ic,"kx=i"=>\$kx,"ky=i"=>\$ky,"kz=i"=>\$kz,"degreex=f"=>\$degreex,\
           "degreet=f"=>\$degreet,"method=s"=>\$method,"errorNorm=i"=>\$errorNorm,"dissOrder=i"=>\$dissOrder,\
           "ip=s"=>\$interfacePoint,"in=s"=>\$interfaceNormal,"ax=f"=>\$ax,"ay=f"=>\$ay,"az=f"=>\$az,\
           "left=s"=>\$left,"right=s"=>\$right,"restart=s"=>\$restart,"interfaceIts=i"=>\$interfaceIts,\
           "show=s"=>\$show,"tf=f"=>\$tFinal,"tp=f"=>\$tPlot,"tz=s"=>\$tz,"useNewInterface=i"=>\$useNewInterface,\
           "interfaceEquationOption=i"=>\$interfaceEquationOption,"interfaceOmega=f"=>\$interfaceOmega,"tz=s"=>\$tz,\
           "bc1=s"=>\$bc1,"bc2=s"=>\$bc2,"bc3=s"=>\$bc3,"bc4=s"=>\$bc4,"bc5=s"=>\$bc5,"bc6=s"=>\$bc6,\
           "bc7=s"=>\$bc7,"bc8=s"=>\$bc8,"setDivergenceAtInterfaces=s"=>\$setDivergenceAtInterfaces,\
           "useImpedanceInterfaceProjection=s"=>\$useImpedanceInterfaceProjection,"modeGDM=i"=>\$modeGDM,\
           "dm=s"=>\$dm,"npv=i{1,}"=>\@npv,"alphaP=f"=>\$alphaP,\
           "a01=f{1,}"=>\@a01,"a11=f{1,}"=>\@a11,"b01=f{1,}"=>\@b01,"b11=f{1,}"=>\@b11,\
           "a02=f{1,}"=>\@a02,"a12=f{1,}"=>\@a12,"b02=f{1,}"=>\@b02,"b12=f{1,}"=>\@b12);
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $ic eq "pmic" ){ $ic="planeMaterialInterfaceInitialCondition"; }
if( $ic eq "tz" ){ $ic="twilightZone"; }
if( $bc eq "d" ){ $bc="dirichlet"; }
if( $tz eq "poly" ){ $tz="polynomial"; }
if( $tz eq "trig" ){ $tz="trigonometric"; }
if( $method eq "sosup" ){ $diss=0.; }
# 
#
if( $dm eq "none" ){ $dm="no dispersion"; }
if( $dm eq"gdm" ){ $dm="GDM"; }
# Give defaults here for array arguments: 
if( $npv[0] eq "" ){ @npv=(0,0); }
if( $a01[0] eq "" ){ @a01=(1,0,0,0); }
if( $a11[0] eq "" ){ @a11=(0,0,0,0); }
if( $b01[0] eq "" ){ @b01=(0,0,0,0); }
if( $b11[0] eq "" ){ @b11=(0,0,0,0); }
#
if( $a02[0] eq "" ){ @a02=(1,0,0,0); }
if( $a12[0] eq "" ){ @a12=(0,0,0,0); }
if( $b02[0] eq "" ){ @b02=(0,0,0,0); }
if( $b12[0] eq "" ){ @b12=(0,0,0,0); }
#
$grid
#
$method
# dispersion model:
$dm
# printf(" dm=$dm\n");
#
GDM mode: $modeGDM
$domain="all"; 
GDM alphaP: $alphaP
# ------------ Set GDM parameters on the left domain -----------
GDM domain name: leftDomain
  number of polarization vectors: $npv[0]
$cmd="#"; 
if( $npv[0] == 1 ){ \
   $cmd = " GDM coeff: 0 $a01[0] $a11[0] $b01[0] $b11[0] (eqn, a0,a1,b0,b1)\n"; \
 }
if( $npv[0] == 2 ){ \
   $cmd  = " GDM coeff: 0 $a01[0] $a11[0] $b01[0] $b11[0] (eqn, a0,a1,b0,b1)\n"; \
   $cmd .= " GDM coeff: 1 $a01[1] $a11[1] $b01[1] $b11[1] (eqn, a0,a1,b0,b1)"; \
      }
$cmd
# ------------ Set GDM parameters on the right domain -----------
GDM domain name: rightDomain
  number of polarization vectors: $npv[1]
  GDM alphaP: $alphaP[1]
$cmd="#"; 
if( $npv[1] == 1 ){ \
   $cmd = " GDM coeff: 0 $a02[0] $a12[0] $b02[0] $b12[0] (eqn, a0,a1,b0,b1)\n"; \
 }
if( $npv[1] == 2 ){ \
   $cmd  = " GDM coeff: 0 $a02[0] $a12[0] $b02[0] $b12[0] (eqn, a0,a1,b0,b1)\n"; \
   $cmd .= " GDM coeff: 1 $a02[1] $a12[1] $b02[1] $b12[1] (eqn, a0,a1,b0,b1)"; \
      }
$cmd
# 
#  The dispersive case is handled by a user defined known solution
if( $dm ne "no dispersion" && $tz eq "#" ){\
 $ic = "user defined known solution\n  dispersive plane wave interface\n done\n userDefinedKnownSolutionInitialCondition"; }
# 
$ic
$tz 
** trigonometric
 degreeSpace, degreeTime  $degreex $degreet
#
kx,ky,kz $kx $ky $kz
plane wave coefficients $ax $ay $az $eps1 $mu1
#
use new interface routines $useNewInterface
set divergence at interfaces $setDivergenceAtInterfaces
use impedance interface projection $useImpedanceInterfaceProjection
# These next parameters define the exact solution for a material interface: 
material interface point $interfacePoint
material interface normal $interfaceNormal
#
# *****************
# for Yee we define the interface here:
$cmds="#";
if( $method eq "Yee" ){ $cmds = "define embedded bodies\n plane material interface\n $interfaceNormal $interfacePoint\n $eps2 $mu2 0. 0. \nexit"; }
$cmds 
# ****************
bc: all=$bc
#** bc: all=dirichlet
#* bc: leftSquare(0,0)=planeWaveBoundaryCondition
# ****************
if( $bc1 ne "" ){ $cmd="bc: bcNumber1=$bc1"; }else{ $cmd="#"; }
$cmd
if( $bc2 ne "" ){ $cmd="bc: bcNumber2=$bc2"; }else{ $cmd="#"; }
$cmd
if( $bc3 ne "" ){ $cmd="bc: bcNumber3=$bc3"; }else{ $cmd="#"; }
$cmd
if( $bc4 ne "" ){ $cmd="bc: bcNumber4=$bc4"; }else{ $cmd="#"; }
$cmd
if( $bc5 ne "" ){ $cmd="bc: bcNumber5=$bc5"; }else{ $cmd="#"; }
$cmd
if( $bc6 ne "" ){ $cmd="bc: bcNumber6=$bc6"; }else{ $cmd="#"; }
$cmd
if( $bc7 ne "" ){ $cmd="bc: bcNumber7=$bc7"; }else{ $cmd="#"; }
$cmd
if( $bc8 ne "" ){ $cmd="bc: bcNumber8=$bc8"; }else{ $cmd="#"; }
$cmd
# 
$cmd="#";
if( $method ne "Yee" ){ $cmd = \
  "coefficients $eps1 1. leftDomain (eps,mu,grid-name)\n" . \
  "coefficients $eps2 1. rightDomain (eps,mu,grid-name)\n"; }
$cmd 
#
# option: 1=extrapolate as initial guess for material interface ghost values
interface option 1
# interfaceEquationsOption=0 : use extrap for 2nd ghost, 1=use eqns
interface equations option $interfaceEquationOption
omega for interface iterations $interfaceOmega
#
interface BC iterations $interfaceIts
#
#
# bc: Annulus=perfectElectricalConductor
tFinal $tFinal
tPlot $tPlot
#
order of dissipation $dissOrder
dissipation $diss
#
# use conservative difference 0
debug $debug
#
cfl $cfl 
plot errors 1
check errors 1
error norm $errorNorm
#*********************************
show file options...
MXSF:compressed
MXSF:open
  $show
# MXSF:frequency to save 1
MXSF:frequency to flush 1
exit
#**********************************
continue
#
plot:Ey
# 
$go


movie mode
finish
