*
* cgmp:  examples with three domains
*
* Usage:
*    cgmp [-noplot] threeDomains -g=<name> -method=[ins|cns] -nu=<> -mu=<> -kappa=<num> -tf=<tFinal> -tp=<tPlot> ...
*           -solver=<yale/best> -ktcFluid=<> -ktcFluid=<> -tz=[poly/trig/none] -bg=<backGroundGrid> ...
*           -nc=<num> degreex=<num> -degreet=<num> -ts=[fe|be|im|pc] -coupled=[0|1] -iOmega=[] ...
*           -debug=<> -go=[run/halt/og]
* 
*  -ktcFluid -ktcSolid : thermal conductivities 
*  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep
*  -nc : number of correction steps for implicit predictor-corrector
* 
* Examples: 
* 
*   cgmp threeDomains -g="cylShell.hdf" -tf=1. -tp=0.05  
*   cgmp threeDomains -g="cylShell2.hdf" -tf=1.         
* 
* implicit:
*   cgmp threeDomains -g="cylShell.hdf" -tf=1. -ts=im -nc=5 -coupled=0 -dtMax=.02 -tp=.02
*   cgmp threeDomains -g="cylShell.hdf" -tf=1. -ts=im -nc=15 -coupled=0 -dtMax=.05 -tp=.05 -debug=3 -iTol=1.e-3 -iOmega=.9
* 
* -- three squares
*   cgmp threeDomains -g=threeSquaresInterfacei1.order2.hdf -d1=leftDomain -d2=middleDomain -d3=rightDomain -tf=1. -tz=poly
*  - implicit: 
*   cgmp threeDomains -g=threeSquaresInterfacei1.order2.hdf -d1=leftDomain -d2=middleDomain -d3=rightDomain -tf=1. -tz=trig -ts=im -nc=6 -coupled=0 -tp=.01 -debug=3 
* 
* 
* --- set default values for parameters ---
* 
$grid="innerOuter2d.hdf"; $method="ins"; $go="halt"; 
$tFinal=2.; $tPlot=.1; $degreeSpace=2; $degreeTime=2; $cfl=.9; $show=""; $pdebug=0; $debug=0; 
$ts="pc";  $numberOfCorrections=1;
$nu=.1; $prandtl=.72; $ktcFluid=-1.; $u0=0.; 
$thermalExpansivity=1.; $T0=100.; $Twall=100.;  $kappa=.01; $ktcSolid=-1.; 
$tz="none";
$gravity = "0 -10. 0.";
$solver="best"; 
$backGround="outerSquare"; 
$iTol=1.e-3; $iOmega=1.; $coupled=1; $dtMax=.05; 
$domain1="outerDomain"; $domain2="shellDomain"; $domain3="innerDomain"; 
* 
$solver="best"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"nu=f"=>\$nu,"mu=f"=>\$mu,"kappa=f"=>\$kappa, "bg=s"=>\$backGround,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,\
 "show=s"=>\$show,"method=s"=>\$method,"ts=s"=>\$ts,"noplot=s"=>\$noplot,"ktcFluid=f"=>\$ktcFluid,\
  "ktcSolid=f"=>\$ktcSolid, "T0=f"=>\$T0,"Twall=f"=>\$Twall,"coupled=i"=>\$coupled,"nc=i"=> \$numberOfCorrections,\
  "dtMax=f"=>\$dtMax,"iOmega=f"=>\$iOmega,"iTol=f"=>\$iTol,"d1=s"=>\$domain1,"d2=s"=>\$domain2,"d3=s"=>\$domain3,\
  "dtMax=f"=>\$dtMax,"debug=i"=>\$debug,"go=s"=>\$go );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $ts eq "fe" ){ $ts="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC";  }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
*
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
* 
$kThermal=$nu/$prandtl;
if( $ktcFluid < 0. ){ $ktcFluid=$kThermal;} if( $ktcSolid < 0. ){ $ktcSolid=$kappa; }
*
* -- interface BCs for the fluid: 
$iface100="bcNumber100=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
$iface101="bcNumber101=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber101=heatFluxInterface";
* -- interface BCs for the solid: 
$siface100="bcNumber100=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
$siface101="bcNumber101=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber101=heatFluxInterface";
*
* $iface100="*"; $siface100="*"; # for testing turn on interface 100
* $iface101="*"; $siface101="*"; # for testing turn on interface 101
* 
$grid
* 
* ------- specify fluid domain A ----------
$domainName=$domain1; $solverName="fluidA"; 
$bc = "all=noSlipWall\n $iface100";
if( $tz eq "turn off twilight zone" ){$ic = "uniform flow\n p=1., u=$u0";}
$ktc=$ktcFluid; $rtolp=1.e-4; $atolp=1.e-6; 
include insDomain.h
*
* ------- specify solid domain ----------
$domainName=$domain2; $solverName="solid"; 
$bc = "all=dirichletBoundaryCondition, uniform(T=$Twall)\n $siface100\n $siface101";
if( $tz eq "turn off twilight zone" ){ $ic = "uniform flow\n" . "T=$Twall\n";}
$ktc = $ktcSolid;
$fx=2.; $fy=2.; $fz=2.; $ft=2.;
include adDomain.h
* 
* ------- specify fluid domain B ----------
$domainName=$domain3; $solverName="fluidB"; 
$bc = "all=noSlipWall\n $iface100\n $iface101";
if( $tz eq "turn off twilight zone" ){$ic = "uniform flow\n p=1., u=$u0";}
$ktc=$ktcFluid; $rtolp=1.e-4; $atolp=1.e-6; 
$fx=1.; $fy=1.; $fz=1.; $ft=1.;
include insDomain.h
*
* 
continue
*
* -- set parameters for cgmp ---
* 
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  $ts
  $tz
  debug $debug
  number of PC corrections $numberOfCorrections
  OBPDE:interface tolerance $iTol
  OBPDE:interface omega $iOmega
  OBPDE:solve coupled interface equations $coupled
  show file options
    compressed
      open
       $show
    frequency to flush
      2
    exit
  continue
continue
*
plot:fluidA : T
plot:fluidB : T
$go



