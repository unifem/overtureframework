*
* cgmp: solve for INS and AD in multiple domains
* 
* Usage:
*    cgmp [-noplot] multiDomain -g=<name> -nu=<num> -kappa=<num> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> ...
*                -nc=[] -tz=[poly/trig/none] -bg=<backGroundGrid> -degreex=<num> -degreet=<num> ...
*                -ts=[fe|be|im|pc] -debug=[]
* 
*  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep, pc=predictor-corrector
* 
* Examples: 
* 
*    cgmp multiDomain.cmd -g="multiDomaini1.order2.hdf" -solver=yale -tz=poly -tp=.02 
*    cgmp multiDomain.cmd -g="multiDomaini2.order2.hdf" -solver=yale -tz=poly -tp=.02
*
* Real run: 
*    cgmp multiDomain.cmd -g="multiDomaini1.order2.hdf" -solver=yale
*    cgmp multiDomain.cmd -g="multiDomaini2.order2.hdf" -solver=yale -tf=1. -tp=.1 -nu=.025 -show=multiDomain.show 
*    cgmp noplot multiDomain.cmd -g="multiDomaini4.order2.hdf" -solver=yale -tf=1. -tp=.1 -nu=.01 -show=multiDomain4.show  >! md4.out & 
*    cgmp noplot multiDomain.cmd -g="multiDomaini8.order2.hdf" -solver=yale -tf=1. -tp=.1 -nu=.005 -show=multiDomain8.show  >! md8.out & 
* 
* try this: 
*    cgmp noplot multiDomain.cmd -g="multiDomaini16.order2.hdf" -ts=im -iTol=1.e-3 -nc=15 -coupled=0 -dtMax=.01 -tf=1. -tp=.1 -nu=.002 -show=multiDomain16.show  >! md16.out & 
*  -- implicit:   
*    cgmp multiDomain.cmd -g="multiDomaini1.order2.hdf" -solver=yale -ts=im -iTol=1.e-3 -nc=15 -coupled=0 -tp=.01 -debug=3
* -- trouble with implicit and inner streamlines: 
*    cgmp multiDomain.cmd -g="multiDomaini2.order2.hdf" -solver=yale -ts=im -iTol=1.e-3 -nc=15 -coupled=0 -tf=1. -tp=.05 -dtMax=.005 -show=multiDomain.show 
*
* --- set default values for parameters ---
* 
$grid="multiDomain.hdf";
$tFinal=2.; $tPlot=.1; $degreeSpace=2; $degreeTime=2; $cfl=.9; $show=""; $dtMax=.1;
$ts="pc"; $numberOfCorrections=1; $implicitVariation="viscous";
$nu=.1; $prandtl=.72;  $thermalExpansivity=1.;  $kappa=.1; $ktc=.2; $pdebug=0; $debug=0; 
$T0=10.; $Twall=10.; # T0 = initial T in the solid 
$tz="none"; 
$gravity = "0 -10. 0.";  $ad2=1; 
$solver="best"; 
$backGround="outerSquare"; 
$coupled=1; $iTol=1.e-3; $iOmega=1.; 
* 
$solver="best"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"nu=f"=>\$nu,"kappa=f"=>\$kappa,"ktc=f"=>\$ktc, "bg=s"=>\$backGround,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,\
 "show=s"=>\$show,"ts=s"=>\$ts,"coupled=i"=>\$coupled,"nc=i"=> \$numberOfCorrections,"iOmega=f"=>\$iOmega,\
 "iTol=f"=>\$iTol,"dtMax=f"=>\$dtMax,"debug=i"=>\$debug,"noplot=s"=>\$noplot,"cDt=f"=>\$cDt,"iv=s"=>\$implicitVariation );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $ts eq "fe" ){ $ts="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC";       $tsd="adams PC";  }
* 
if( $implicitVariation eq "viscous" ){ $implicitVariation = "useNewImplicitMethod\n implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "useNewImplicitMethod\n implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "useNewImplicitMethod\n implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
*
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; }
* 
$kThermal=$nu/$prandtl;
* 
$grid
* 
$domain1="ellipseInnerDomain"; $domain2="ellipseCenterDomain"; $domain3="ellipseOuterDomain"; 
$domain4="annulus1Domain"; $domain5="annulus2Domain"; $domain6="mainDomain"; 
* 
* -- interface BCs for the fluid: 
$iface100="bcNumber100=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
$iface101="bcNumber101=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber101=heatFluxInterface";
$iface102="bcNumber102=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber102=heatFluxInterface";
$iface103="bcNumber103=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber103=heatFluxInterface";
$iface104="bcNumber104=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber104=heatFluxInterface";
* -- turn off interfaces: 
* $iface101="bcNumber101=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)";
* $iface102="bcNumber102=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)";
* $iface103="bcNumber103=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)";
* $iface104="bcNumber104=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)";
* -- interface BCs for the solid: 
$siface100="bcNumber100=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
$siface101="bcNumber101=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber101=heatFluxInterface";
$siface102="bcNumber102=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber102=heatFluxInterface";
$siface103="bcNumber103=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber103=heatFluxInterface";
$siface104="bcNumber104=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber104=heatFluxInterface";
* -- turn off interfaces: 
* $siface101="bcNumber101=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)";
* $siface102="bcNumber102=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)";
* $siface103="bcNumber103=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)";
* $siface104="bcNumber104=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)";
* 
* ------- Assign domains ----------
*  fluidA: inside ellipse
$domainName=$domain1; $solverName="fluidA"; $bc = "all=noSlipWall\n$iface100"; $u0=0.; $T0=0.; $ic=""; 
if( $tz eq "turn off twilight zone" ){$ic = "uniform flow\n p=1., u=$u0, T=$T0";}
include insDomain.h
*  solidA: surrounds fluidA ellipse
$domainName=$domain2; $solverName="solidA"; $Twall=10.; $T0=10.; $ic=""; $ktc=.05; 
$bc = "all=dirichletBoundaryCondition, uniform(T=$Twall)\n$siface100\n$siface101";
include adDomain.h
*  solidB: surrounds solidA
$domainName=$domain3; $solverName="solidB"; $Twall=0.; $T0=5.; $ic=""; $ktc=.01; 
$bc = "all=dirichletBoundaryCondition, uniform(T=$Twall)\n $siface101\n $siface102"; 
include adDomain.h
*  solidC: upper right disk
$domainName=$domain4; $solverName="solidC"; $Twall=10.; $T0=10.; $ic=""; $ktc=.01; 
$bc = "all=dirichletBoundaryCondition, uniform(T=$Twall)\n $siface103"; 
include adDomain.h
*  solidD: lower right disk
$domainName=$domain5; $solverName="solidD"; $T0=10.;  $ic=""; $ktc=.01; 
$bc = "all=dirichletBoundaryCondition, uniform(T=$Twall)\n $siface104"; 
include adDomain.h
*
$domainName=$domain6; $solverName="fluidB"; $Twall=0.; $T0=0.; $ic=""; $ktc=.2; 
if( $tz eq "turn off twilight zone" ){$ic = "uniform flow\n p=1., u=$u0, T=$T0";}
$bc = "all=noSlipWall\n $iface102\n $iface103\n $iface104"; 
include insDomain.h
* 
continue
* 
* -- set parameters for cgmp ---
* 
  $tz
  final time $tFinal
  times to plot $tPlot
  $ts 
  cfl $cfl
  debug $debug 
* 
  number of PC corrections $numberOfCorrections
  OBPDE:interface tolerance $iTol
  OBPDE:interface omega $iOmega
  OBPDE:solve coupled interface equations $coupled
* 
  show file options
    compressed
      open
       $show
    frequency to flush
      $numDomains=5; 
      $freq=$numDomains*4; 
      $freq
    exit
  continue
*
continue
* 
   plot domain: fluidA
   erase
   streamlines
     exit
   plot domain: fluidB
   streamlines
     exit


   $Tmax=5.; 
if( $tz eq "turn off twilight zone" ){ $cmds = "contour\n plot:T\n min max 0 $Tmax\n exit\n min max 0 $Tmax\n exit\n min max 0 $Tmax\n\
 exit\n min max 0 $Tmax\n exit\n min max 0 $Tmax\n exit\n plot:T\n min max 0 $Tmax\n exit"; }else{ $cmds="*"; }
$cmds


   plot:fluidA : T
   plot:solidA : T
   plot:solidB : T
   plot:solidC : T
   plot:solidD : T
   plot:fluidB : T


