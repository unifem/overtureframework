* 
* cgmp: examples with two domains
*
* Usage:
*    cgmp [-noplot] twoDomain -g=<name> -nu=<num> -kappa1=<num> -kappa2=<num> -ktc1=<> -ktc2=<> -tf=<tFinal> -tp=<tPlot> ...
*          -solver=<yale/best> -nc=<num> -degreex[1/2]=<num> -degreet[1/2]=<num> -tz=[poly/trig/none] ...
*           -bg=<backGroundGrid> -ts=<fe/be/im/pc> -coupled=[0|1] -fx1=[] -fy1=[] -fz=1[] -ft1[] -imp=<> ...
*          -method1=[ad|ins|cns] -method2=[ad|ins|cns] -mixedInterface=[0|1] -iv=[viscous|full]
* where
*  -kappa1, -kappa2 : thermal diffusivity ( T_t + .. = kappa*Delta(T) )
*  -ktc1, -ktc2 : thermal conductivity ( heat flux = - ktc1 grad(T)  kappa=k/(rho*C) )
*  -ts = time-stepping method, fe="forward Euler", be="backward Euler", mid="mid-point" im="implicit"
*  -nc : number of correction steps for implicit predictor-corrector
*  -coupled : 1=solve coupled interface equations, 0=solve decoupled 
*  -imp : .5=CN, 1.=BE, 0.=FE
* 
* Examples:
* 
*  cgmp twoDomain -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.5 -ktc2=.5 -tf=.05 -tp=.01 
* 
*  cgmp twoDomain -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.05 -tp=.01 -ts=pc
*   -- restart: 
*  cgmp twoDomain -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.5 -ktc2=.5 -tf=.05 -tp=.01 -restart="twoDomain.show"
* 
* -- pc: 
* cgmp twoDomain -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.5 -tf=.05 -tp=.005 -ts=pc -nc=1
* ---curved pipe---
* cgmp twoDomain -g=curvedPipe22e1.order2 -kappa1=1. -ktc1=1. -kappa2=.5 -ktc2=.1 -tf=.05 -tp=.01 -domain1=solidDomain -domain2=fluidDomain
* ---solid sphere in a box ---
* cgmp twoDomain -g=solidSphereInABoxi1.order2 -kappa1=1. -ktc1=1. -kappa2=.5 -ktc2=.1 -tf=.05 -tp=.01 -domain1=innerDomain -domain2=outerDomain
* cgmp twoDomain -g=solidSphereInABoxi2.order2 -kappa1=1. -ktc1=1. -kappa2=.5 -ktc2=.1 -tf=.005 -tp=.0002 -domain1=innerDomain -domain2=outerDomain
*  -- ok now: 
* cgmp twoDomain -g=solidSphereInABoxe2.order2 -kappa1=.1 -ktc1=.1 -kappa2=.05 -ktc2=.05 -tf=.5 -tp=.01 -domain1=innerDomain -domain2=outerDomain 
* --- solid and fluid -- explicit and implicit: 
* cgmp twoDomain -g=twoSquaresInterfacee2.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.5 -tp=.01 -dtMax=.01 -ts=pc -coupled=1 -tz=poly -degreet1=2 -degreet2=2 -debug=1 -method2=ins
* cgmp twoDomain -g=twoSquaresInterfacee2.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.5 -tp=.01 -dtMax=.01 -ts=im -nc=15 -coupled=0 -tz=poly -degreet1=2 -degreet2=2 -iTol=1.e-9 -mixedInterface=1 -debug=3 -method2=ins
* cgmp twoDomain -g=solidSphereInABoxe2.order2 -kappa1=.1 -ktc1=.1 -kappa2=.05 -ktc2=.05 -tf=.5 -tp=.01 -domain1=innerDomain -method2=ins -domain2=outerDomain -nu=.1 -tz=trig -fx2=.5 -fy2=.5 -fz2=.5 -ft2=.5 -solver=best
*
* -- explicit but solve interface equations decoupled: 
* cgmp twoDomain -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.5 -tf=.05 -tp=.005 -iOmega=1. -coupled=0 -nc=3
* cgmp twoDomain -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.5 -tf=.05 -tp=.005 -iOmega=1. -coupled=0 -nc=3 -ts=pc
*
* implicit:
*  cgmp twoDomain -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.5 -tp=.1 -ts=im -nc=9 -coupled=0 -debug=3
* 
*  cgmp twoDomain -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.5 -tp=.1 -ts=im -nc=9 -coupled=0 -tz=trig -fy=0. -debug=3 
* 
* -- to see analytic interface convergence rate: 
*  cgmp twoDomain -g=twoSquaresInterfacee32.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.5 -tp=.01 -dtMax=.01 -ts=im -nc=9 -coupled=0 -tz=trig -fy=0. -iTol=1.e-7 -debug=3 
*  cgmp twoDomain -g=twoSquaresInterfacee32.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.5 -tp=.01 -dtMax=.01 -ts=im -nc=9 -coupled=0 -tz=poly -degreex1=4 -degreex2=4 -iTol=1.e-10 -debug=3
* 
**** cgmp twoDomain -g=twoSquaresInterfacee32.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=5. -tp=.1 -dtMax=.1 -ts=im -nc=9 -coupled=0 -tz=trig -iTol=1.e-7 -mixedInterface=1 -debug=3
**** cgmp twoDomain -g=twoSquaresInterfacee32.order2.hdf -kappa1=1. -ktc1=1. -kappa2=1. -ktc2=1. -tf=.5 -tp=.01 -dtMax=.01 -ts=im -nc=9 -coupled=0 -tz=trig -fx1=3. -fx2=1.5 -fy1=2. -fy2=3 -iTol=1.e-10 -mixedInterface=1 -debug=3
*** cgmp twoDomain -g=twoSquaresInterfacee8.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.5 -tp=.01 -dtMax=.01 -ts=im -nc=15 -coupled=0 -tz=poly -degreet1=1 -degreet2=1 -iTol=1.e-7 -mixedInterface=1 -debug=3
*  
** cgmp twoDomain -g=innerOuter2d -kappa1=1. -ktc1=1. -kappa2=1. -ktc2=1. -tf=1. -tp=.1 -domain1=innerDomain -domain2=outerDomain -ts=im -solver=yale -nc=10 -coupled=0 -mixedInterface=1 -iTol=1.e-10 -tz=trig -fx1=2. -fy2=1.5 -debug=3 
** cgmp twoDomain -g=innerOuter2d -kappa1=1. -ktc1=1. -kappa2=1. -ktc2=.1 -tf=1. -tp=.1 -domain1=innerDomain -domain2=outerDomain -ts=im -solver=yale -nc=10 -coupled=0 -mixedInterface=1 -iTol=1.e-10 -tz=trig -fx1=2. -fy2=1.5 -debug=3 
*
* ==== test extrapolation of the first iteration of the interface equations: 
* cgmp twoDomain -g=twoSquaresInterfacee32.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=5. -tp=.1 -dtMax=.1 -ts=im -nc=20 -coupled=0 -tz=poly -degreet1=1 -degreet2=1 -iTol=1.e-10 -mixedInterface=0 -debug=3
*
*  cgmp twoDomain -g=twoSquaresInterfacee32.order2.hdf -kappa1=.1 -ktc1=1. -kappa2=1. -ktc2=.1 -tf=.5 -tp=.01 -dtMax=.01 -ts=im -nc=9 -coupled=0 -tz=poly -degreex1=4 -degreex2=4 -debug=3
* 
* === test changing the time step with the mixed interface BC
* cgmp twoDomain -g=twoSquaresInterfacee4.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.5 -tp=.1 -dtMax=.1 -ts=im -nc=15 -coupled=0 -tz=trig -iTol=1.e-8 -mixedInterface=1 -debug=3 
* cgmp twoDomain -g=twoSquaresInterfacee4.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.5 -tp=.1 -dtMax=.1 -ts=im -nc=15 -coupled=0 -tz=poly -degreet1=1 -degreet2=1 -iTol=1.e-12 -mixedInterface=1 -debug=3 
* 
* cgmp twoDomain -g=twoSquaresInterfacee4.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.5 -tp=.01 -dtMax=.01 -ts=im -nc=15 -coupled=0 -tz=poly -degreet1=1 -degreet2=1 -iTol=1.e-9 -mixedInterface=1 -debug=3 -method2=ins
*
* +++++++++
* cgmp noplot twoDomain -g=twoSquaresInterfacee0.order2 -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.01 -tp=.01 -dtMax=.01 -ts=im -nc=15 -coupled=0 -tz=poly -degreet1=1 -degreet2=1 -degreex1=0 -degreex2=0 -iTol=1.e-9 -mixedInterface=1 -debug=15 -method2=ins -ktcFluid=1. > ! junk
* cgmp noplot twoDomain -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.01 -tp=.01 -dtMax=.01 -ts=im -nc=15 -coupled=0 -tz=poly -degreet1=1 -degreet2=1 -degreex1=0 -degreex2=0 -iTol=1.e-9 -mixedInterface=1 -debug=15 -method2=ins -ktcFluid=1. > ! junk
* ++++++++
* 
* cgmp twoDomain -g=innerOuter2d -kappa1=1. -ktc1=1. -kappa2=1. -ktc2=1. -tf=5. -tp=.05 -dtMax=.05 -domain1=outerDomain -domain2=innerDomain -ts=im -solver=yale -nc=10 -coupled=0 -mixedInterface=1 -debug=3 
* 
**** TROUBLE: mixed-interface plus dt changing  (cartesian case is ok!) --> BUG found : neumann.C !
* cgmp twoDomain -g=twoNonSquaresInterfacee1.order2 -kappa1=1. -ktc1=1. -kappa2=1. -ktc2=1. -kThermal=1. -ktcFluid=1. -tf=5. -tp=.01 -dtMax=.01 -ts=im -solver=yale -nc=10 -coupled=0 -iTol=1.e-10 -mixedInterface=1 -debug=3 -method2=ins 
**** TROUBLE: mixed-interface plus dt changing 
* cgmp twoDomain -g=innerOuter2d -kappa1=1. -ktc1=1. -kappa2=1. -ktc2=1. -kThermal=1. -ktcFluid=1. -tf=5. -tp=.01 -dtMax=.01 -domain1=outerDomain -domain2=innerDomain -ts=im -solver=yale -nc=10 -coupled=0 -iTol=1.e-7 -mixedInterface=1 -debug=3 -method1=ins  
********
* 
* TROUBLE near corners with mixedInterface 080830 
* cgmp twoDomain -g=twoSquaresInterfacee2.order2.hdf -kappa1=1. -ktc1=1. -kappa2=1. -ktc2=1. -tf=.01 -tp=.01 -dtMax=.01 -ts=im -nc=10 -coupled=0 -tz=poly -iTol=1.e-8 -mixedInterface=1 -debug=3 
* cgmp twoDomain -g=twoSquaresInterfacee16.order2.hdf -kappa1=1. -ktc1=1. -kappa2=1. -ktc2=1. -tf=.01 -tp=.01 -dtMax=.01 -ts=im -nc=10 -coupled=0 -tz=poly -iTol=1.e-8 -mixedInterface=1 -debug=3 
* cgmp twoDomain -g=twoSquaresInterfacee8.order2.hdf -kappa1=1. -ktc1=1. -kappa2=1. -ktc2=1. -tf=.01 -tp=.01 -dtMax=.01 -ts=im -nc=10 -coupled=0 -tz=poly -iTol=1.e-8 -mixedInterface=1 -debug=3 
* cgmp noplot twoDomain -g=twoSquaresInterfacee32.order2.hdf -kappa1=1. -ktc1=1. -kappa2=1. -ktc2=1. -tf=.01 -tp=.01 -dtMax=.01 -ts=im -nc=10 -coupled=0 -tz=poly -iTol=1.e-8 -mixedInterface=1 -debug=3 -go=og
* 
* cgmp twoDomain -g=solidSphereInABoxi1.order2 -kappa1=.1 -ktc1=.1 -kappa2=.05 -ktc2=.05 -tf=.5 -tp=.01 -domain1=innerDomain -domain2=outerDomain -ts=im -solver=best -nc=10 -coupled=0 -debug=3 
* cgmp twoDomain -g=solidSphereInABoxi2.order2 -kappa1=.1 -ktc1=.1 -kappa2=.05 -ktc2=.05 -tf=.5 -tp=.01 -domain1=innerDomain -domain2=outerDomain -ts=im -solver=best -nc=10 -coupled=0 -debug=3 
* 
*  -- implicit and periodic
*  cgmp twoDomain -g=twoSquaresInterfacenp32.hdf -kappa1=1. -ktc1=1. -kappa2=.99 -ktc2=.99 -tf=.2 -tp=.05 -ts=im -nc=10 -go=halt -iOmega=.5 -coupled=0 -debug=3 -tz=trig -fx1=2 -fy1=2 
*  cgmp twoDomain -g=twoSquaresInterfacenp32.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.2 -tp=.05 -ts=im -nc=10 -go=halt -iOmega=.5 -coupled=0 -debug=3 -tz=trig -fx1=2 -fy1=2 
*
*  cgmp noplot twoDomain -g=twoSquaresInterfacee1.order2.hdf -kappa1=.9 -kappa2=.2 -tf=.02 -tp=.02 -ts=im -nc=5 >! junka
* 
* -- test implicit and under-relaxed: 
* cgmp noplot twoDomain -g=twoSquaresInterfacee1.order2.hdf -kappa1=1.01 -ktc1=1.01 -kappa2=1. -ktc2=1. -tf=.2 -tp=.1 -ts=im -nc=10 -go=go -iOmega=.64 -coupled=0
* 
* parallel:
*  mpirun -np 1 $cgmpp twoDomain.cmd
*  mpirun -np 1 -dbg=valgrindebug $cgmpp noplot twoDomain.cmd
* 
*  mpirun -np 1 $cgmpp twoDomain -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.5 -ktc2=.5 -tf=.05 -tp=.01 
*  mpirun -np 1 $cgmpp twoDomain -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.5 -tp=.1 -ts=im -nc=9 -coupled=0 -debug=3 -solver=best
*  mpirun -np 1 $cgmpp twoDomain -g=innerOuter2d -kappa1=1. -ktc1=1. -kappa2=1. -ktc2=1. -tf=1. -tp=.1 -domain1=innerDomain -domain2=outerDomain -ts=im -solver=best -nc=10 -coupled=0 -mixedInterface=1 -iTol=1.e-3 -tz=trig -fx1=2. -fy2=1.5 -debug=3
*
* ======= new parallel tests
* mpirun -np 2 $cgmpp noplot twoDomain -g=twoSquaresInterfacee32.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.1 -tf=.5 -tp=.1 -dtMax=.05 -ts=im -nc=9 -coupled=0 -solver=best -tz=poly -degreex1=2 -degreex2=2 -iTol=1.e-10 -rToli=1.e-10 -debug=3 -method1=ins >! junk
* 
*  srun -N1 -n1 -ppdebug $cgmpp twoDomain -g=twoSquaresInterfacee2.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.5 -ktc2=.5 -tf=.05 -tp=.01 -tz=poly -degreet1=1 -degreet2=1 -ts=im -nc=9 -coupled=0 -debug=3 -solver=best -iTol=1.e-7 -rtoli=1.e-10
*  srun -N1 -n4 -ppdebug $cgmpp twoDomain -g=twoSquaresInterfacee4.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.5 -ktc2=.5 -tf=1. -tp=.1 -tz=poly -degreet1=1 -degreet2=1 -ts=im -nc=9 -coupled=0 -debug=3 -solver=best -iTol=1.e-7 -rtoli=1.e-10
*  srun -N1 -n1 -ppdebug $cgmpp twoDomain -g=twoSquaresInterfacee4.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.5 -ktc2=.5 -tf=1. -tp=.1 -tz=poly -degreet1=1 -degreet2=1 -ts=im -nc=9 -coupled=0 -debug=3 -solver=best -iTol=1.e-7 -rtoli=1.e-10 -method1=ins
*  totalview srun -a -N1 -n4 -ppdebug $cgmpp noplot twoDomain -g=twoBoxesInterfacee444.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.5 -ktc2=.5 -tf=.04 -tp=.02 -tz=poly -degreet1=1 -degreet2=1 -ts=im -nc=9 -coupled=0 -debug=3 -solver=best -iTol=1.e-7 -rtoli=1.e-10
* 
* --- set default values for parameters ---
* 
$grid="twoSquaresInterfacee1.order2.hdf";
$method1="ad"; $method2="ad"; $bct="d"; 
$ghost=0; $tz="poly"; $uMin=2.; $uMax=4.;
$degreex1=2; $degreet1=2; $a1=0.; $b1=0.; $domain1="leftDomain";
$degreex2=2; $degreet2=2; $a2=0.; $b2=0.; $domain2="rightDomain";
$fx1=1.; $fy1=$fx1; $fz1=$fx1; $ft1=$fx1; $fx2=1.; $fy2=$fx2; $fz2=$fx2; $ft2=$fx2; 
$kappa1=.1; $kappa2=.1; 
$ktc1=-1.; $ktc2=-1.;   # by default set ktc equal to kappa
$u0=0.; $T0=100.; $Twall=100.;
$ad2=0; $ad21=0; $ad22=0.; $adcBoussinesq=0.; 
*
* ----------------------------- get command line arguments ---------------------------------------
*  -- first get any commonly used options: (requires the env variable CG to be set)
$getCommonOptions = "$ENV{'CG'}/mp/cmd/getCommonOptions.h";
include $getCommonOptions
*  -- now get additional options: 
GetOptions("bg=s"=>\$backGround,"degreex1=i"=>\$degreex1, "degreet1=i"=>\$degreet1,\
 "degreex2=i"=>\$degreex2, "degreet2=i"=>\$degreet2,"kappa1=f"=>\$kappa1,"kappa2=f"=>\$kappa2,\
  "ktc1=f"=>\$ktc1,"ktc2=f"=>\$ktc2,"domain1=s"=>\$domain1,"domain2=s"=>\$domain2,\
  "fx1=f"=>\$fx1,"fy1=f"=>\$fy1,"fz1=f"=>\$fz1,"ft1=f"=>\$ft1,\
  "fx2=f"=>\$fx2,"fy2=f"=>\$fy2,"fz2=f"=>\$fz2,"ft2=f"=>\$ft2,\
  "method1=s"=>\$method1,"method2=s"=>\$method2,"restart=s"=>\$restart,"bct=s"=>\$bct);
* -------------------------------------------------------------------------------------------------
if( $ktc1 < 0. ){ $ktc1=$kappa1; }  if( $ktc2 < 0. ){ $ktc2=$kappa2; }
* 
* 
$grid
* 
* -- interface BCs for the fluid: 
$iface100="bcNumber100=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
$iface101="bcNumber101=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber101=heatFluxInterface";
* -- interface BCs for the solid: 
$siface100="bcNumber100=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
$siface101="bcNumber101=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber101=heatFluxInterface";
*
* $iface100="*"; $iface101="*"; $siface100="*"; $siface101="*"; 
* ------- start new domain ---------- 
$fx=$fx1; $fy=$fy1; $fz=$fz1; $ft=$ft1;
$domainName=$domain1; $solverName="solidA"; 
$bc = "all=dirichletBoundaryCondition\n $siface100\n $siface101";
if( $bct eq "n" ){ $bc = "all=neumannBoundaryCondition\n $siface100\n $siface101";}
$kappa=$kappa1; $ktc=$ktc1; $degreeSpace=$degreex1; $degreeTime=$degreet1; $a=$a1; $b=$b1; 
*
if( $restart ne "" ){ $ic="use grid from show file 0\n read from a show file\n $restart\n  -1 \n";} 
if( $method1 eq "ad" ){ $cmd = "include adDomain.h"; }else{ $cmd="*"; }
$cmd
*  --- commands for domain1=INS: 
$domainName=$domain1; $solverName="fluidA"; $bc = "all=dirichletBoundaryCondition\n$iface100\n$iface101"; $u0=0.; $T0=0.; $ic=""; 
if( $tz eq "turn off twilight zone" ){$ic = "uniform flow\n p=1., u=$u0, T=$T0";}
if( $method1 eq "ins" ){ $cmd = "include insDomain.h"; }else{ $cmd="*"; }
$cmd
* ------- start new domain ---------- 
$fx=$fx2; $fy=$fy2; $fz=$fz2; $ft=$ft2;
*  --- commands for domain2=AD: 
$domainName=$domain2; $solverName="solidB"; 
$bc = "all=dirichletBoundaryCondition\n $siface100\n $siface101";
if( $bct eq "n" ){ $bc = "all=neumannBoundaryCondition\n $siface100\n $siface101"; }
$kappa=$kappa2; $ktc=$ktc2; $degreeSpace=$degreex2; $degreeTime=$degreet2; $a=$a2; $b=$b2; 
if( $restart ne "" ){ $ic="use grid from show file 0\n read from a show file\n $restart\n  -1 \n";} 
if( $method2 eq "ad" ){ $cmd = "include adDomain.h"; }else{ $cmd="*"; }
$cmd
*  --- commands for domain2=INS: 
* $domainName=$domain2; $solverName="fluidB"; $bc = "all=noSlipWall\n$iface100\n$iface101"; $u0=0.; $T0=0.; $ic=""; 
$domainName=$domain2; $solverName="fluidB"; $bc = "all=dirichletBoundaryCondition\n$iface100\n$iface101"; $u0=0.; $T0=0.; $ic=""; 
$ktc=$ktcFluid;
if( $tz eq "turn off twilight zone" ){$ic = "uniform flow\n p=1., u=$u0, T=$T0";}
if( $restart ne "" ){ $ic="use grid from show file 0\n read from a show file\n $restart\n  -1 \n";} 
if( $method2 eq "ins" ){ $cmd = "include insDomain.h"; }else{ $cmd="*"; }
$cmd
* 
 continue
* -- set parameters for cgmp ---
*  midpoint
  $ts 
  number of PC corrections $numberOfCorrections
  OBPDE:interface tolerance $iTol
  OBPDE:interface omega $iOmega
  OBPDE:solve coupled interface equations $coupled
  OBPDE:use mixed interface conditions $mixedInterface
  * -- for testing solve the domains in reverse order: 
  OBPDE:domain order 1 0 
*
   $tz
* --
* 
  final time $tFinal
  times to plot $tPlot
  debug $debug
*  Here is the show file that saves the solutions for both domains
  show file options
    compressed
      open
       $show
    frequency to flush
      10
    exit
  continue
$go

*
    contour
      min max $uMin $uMax
      exit
      min max $uMin $uMax
      exit
continue
$go

 movie mode
 finish
