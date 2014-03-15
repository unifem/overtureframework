# 
# cgmx: test for material interfaces
#
# Usage:
#    cgmx [-noplot] interface -g=<name> -tf=<tFinal> -tp=<tPlot> -cfl=<> -diss=<> -eps1=<> -eps2=<> -ic=<> ...
#                   -bc=<> -useNewInterface=[0|1] -method=[nfdtd|Yee] -errorNorm=[0|1|2] 
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
$kx=2; $ky=0; $kz=0; $left="leftSquare"; $right="rightSquare"; $degreex=2; $degreet=2; $method="NFDTD"; $tz="poly";
$tFinal=5.; $tPlot=.2; $cfl=.9; $show=" "; $interfaceIts=30; $debug=0; $diss=.1; $dissOrder=-1;
$useNewInterface=0; $errorNorm=0; $interfaceEquationOption=1; $interfaceOmega=1.;
$eps1=1.; $mu1=1.;
$eps2=1.; $mu2=1.;
$ax=0.; $ay=0.; $az=0.; # plane wave coeffs. all zero -> use default
$ic = "zeroInitialCondition";
$pmic = "planeMaterialInterfaceInitialCondition";
$bc = "all=perfectElectricalConductor";
$interfaceNormal = "1. 0. 0.";
$interfacePoint = ".0 0. 0.";
$tz = "*"; $go="halt";
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
           "interfaceEquationOption=i"=>\$interfaceEquationOption,"interfaceOmega=f"=>\$interfaceOmega,"tz=s"=>\$tz);
# -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $ic eq "pmic" ){ $ic="planeMaterialInterfaceInitialCondition"; }
if( $ic eq "tz" ){ $ic="twilightZone"; }
if( $bc eq "d" ){ $bc="all=dirichlet"; }
if( $tz eq "poly" ){ $tz="polynomial"; }
if( $tz eq "trig" ){ $tz="trigonometric"; }
# 
#
# **** sixth order ***
#  $ts16  ="twoSquaresInterface1.order6.hdf"; 
#  $ts26  ="twoSquaresInterface2.order6.hdf"; 
#  $ts46  ="twoSquaresInterface4.order6.hdf"; 
#  $ts26s ="twoSquaresInterface2s.order6.hdf"; 
#  $ts46s ="twoSquaresInterface4s.order6.hdf"; 
#  $ts86s ="twoSquaresInterface8s.order6.hdf"; 
#
# $gridName=$ts16; $tz="twilightZone"; $tPlot=.01; $bc="all=dirichlet"; $eps2=1.; $debug=1; $diss=0.
# $gridName=$ts26; $kx=1.; $ky=0.; $tPlot=.01; $eps2=4.; $ic=$pmic; $bc="all=dirichlet"; $debug=0; $diss=0.
# $gridName=$ts46; $kx=1.; $ky=0.; $tPlot=.01; $eps2=4.; $ic=$pmic; $bc="all=dirichlet"; $debug=1; $diss=.2;
# $gridName=$ts26s; $kx=1.; $ky=0.; $tPlot=.01; $eps2=4.; $ic=$pmic; $bc="all=dirichlet"; $debug=1; $diss=.2;
# $gridName=$ts46s; $kx=1.; $ky=0.; $tPlot=.01; $eps2=4.; $ic=$pmic; $bc="all=dirichlet"; $debug=1; $diss=.2;
# $gridName=$ts86s; $kx=1.; $ky=1.; $tPlot=.01; $eps2=4.; $ic=$pmic; $bc="all=dirichlet"; $debug=0; $diss=.2;
#
# square aspect ratio:
#
#*****
# $gridName="twoSquaresInterface4s.hdf"; $kx=1.; $ky=0.; $debug=0; $tPlot=.01; $eps2=4.; $ic=$pmic; $bc="all=dirichlet";
# $gridName="twoSquaresInterface8s.hdf"; $kx=1.; $ky=1.; $tPlot=.1; $eps2=4.; $ic=$pmic;  $bc="all=dirichlet";
# $gridName="twoSquaresInterface16s.hdf"; $kx=1.; $ky=1.; $tPlot=.5; $eps2=4.; $ic=$pmic;  $bc="all=dirichlet";
#
# $gridName="twoSquaresInterface2s.order4.hdf"; $kx=4.; $ky=4.; $tPlot=.5;
# $gridName="twoSquaresInterface2s.order4.hdf"; $kx=1.; $ky=1.; $debug=1; $tPlot=.01; $eps2=4.; $ic=$pmic; $bc="all=dirichlet";
#* $gridName="twoSquaresInterface4s.order4.hdf"; $kx=1.; $ky=1.; $debug=1; $tPlot=.01; $eps2=4.; $ic=$pmic; $bc="all=dirichlet";
# $gridName="twoSquaresInterface8sp.order4.hdf"; $kx=2.; $ky=2.; $tFinal=500.; $tPlot=2.; $eps2=4.; $ic=$pmic; $bc="all=dirichlet"; $show=" "; 
#* $gridName="twoSquaresInterface8sp.order4.hdf"; $kx=2.; $ky=2.; $tFinal=500.; $tPlot=50.; $eps2=4.; $ic=$pmic; $bc="all=dirichlet"; $show="twoSquaresInterface8sp.order4.show"; 
#* $gridName="twoSquaresInterface8sp.order4.hdf"; $kx=2.; $ky=2.; $tFinal=500.; $tPlot=50.; $eps2=16.; $ic=$pmic; $bc="all=dirichlet"; $show="twoSquaresInterface8spEps16.order4.show"; 
# $gridName="twoSquaresInterface8s.order4.hdf"; $kx=4.; $ky=4.; $tPlot=.5;
#
# $gridName="twoSquaresInterface4s.hdf"; $kx=1.; $ky=0.; $tPlot=.01; $debug=1; $eps2=1.; $ic=$pmic; $bc="all=dirichlet";
# --- rotated
# $gridName="twoSquaresInterface4s45.order4.hdf"; $kx=1.; $ky=1.; $debug=0; $tPlot=.5; $eps2=4.; $ic=$pmic; $bc="all=dirichlet"; $interfaceNormal = "1. 1. 0."; $interfacePoint="0. .5 0."; $diss=0.; 
#   periodic BC's:
# $gridName="twoSquaresInterface4s45p.order4.hdf"; $kx=1.; $ky=1.; $debug=0; $tFinal=500.; $tPlot=100.; $eps2=16.; $ic=$pmic; $bc="all=dirichlet"; $interfaceNormal = "1. 1. 0."; $interfacePoint="0. .5 0."; $diss=0.; $show="twoSquaresInterface4s45p.order4.show"; 
#* $gridName="twoSquaresInterface8s45p.order4.hdf"; $kx=1.; $ky=1.; $debug=0; $tFinal=500.; $tPlot=10.; $eps2=16.; $ic=$pmic; $bc="all=dirichlet"; $interfaceNormal = "1. 1. 0."; $interfacePoint="0. .5 0."; $diss=0.; $show="twoSquaresInterface8s45p.order4.show"; $interfaceIts=60;
# $gridName="twoSquaresInterface8s45p.order4.hdf"; $kx=1.; $ky=1.; $debug=0; $tFinal=500.; $tPlot=10.; $eps2=16.; $ic=$pmic; $bc="all=dirichlet"; $interfaceNormal = "1. 1. 0."; $interfacePoint="0. .5 0."; $diss=2.; $show="twoSquaresInterface8s45pDiss0p5.order4.show"; $interfaceIts=5;
# $gridName="twoSquaresInterface4s90.order4.hdf"; $kx=1.; $ky=1.; $debug=0; $tPlot=.5; $eps2=4.; $ic=$pmic; $bc="all=dirichlet"; $interfaceNormal = "0. 1. 0."; $interfacePoint="0. .5 0.";
#
$grid
#
$method
$ic
$tz 
** trigonometric
 degreeSpace, degreeTime  $degreex $degreet
#
kx,ky,kz $kx $ky $kz
plane wave coefficients $ax $ay $az $eps1 $mu1
#
use new interface routines $useNewInterface
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
bc: $bc
#** bc: all=dirichlet
#* bc: leftSquare(0,0)=planeWaveBoundaryCondition
# 
$cmd="#";
if( $method ne "Yee" ){ $cmd = \
  "coefficients $eps1 1. $left (eps,mu,grid-name)\n" . \
  "coefficients $eps2 1. $right (eps,mu,grid-name)\n"; }
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
