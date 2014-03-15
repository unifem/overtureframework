* 
* cgmp example: solve advection-diffusion in two domains in the twilight-zone
*
* Usage:
*    cgmp [-noplot] aa -g=<name> -nu=<num> -kappa1=<num> -kappa2=<num> -ktc1=<> -ktc2=<> -tf=<tFinal> -tp=<tPlot> ...
*          -solver=<yale/best> -nc=<num> -degreex[1/2]=<num> -degreet[1/2]=<num> -tz=[poly/trig/none] ...
*           -bg=<backGroundGrid> -ts=<fe/be/im/pc> -coupled=[0|1]
* where
*  -kappa1, -kappa2 : thermal diffusivity ( T_t + .. = kappa*Delta(T) )
*  -ktc1, -ktc2 : thermal conductivity ( heat flux = - ktc1 grad(T)  kappa=k/(rho*C) )
*  -ts = time-stepping method, fe="forward Euler", be="backward Euler", mid="mid-point" im="implicit"
*  -nc : number of correction steps for implicit predictor-corrector
*  -coupled : 1=solve coupled interface equations, 0=solve decoupled 
* 
* Examples:
* 
*  cgmp aa -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.5 -ktc2=.5 -tf=.05 -tp=.01 
* 
*  cgmp aa -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.5 -ktc2=.5 -tf=.05 -tp=.01 -ts=pc
* 
* -- non-matching interfaces: (in progress...)
*  cgmp -noplot aa -g=twoSquaresInterface1to2.order2 -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.5 -tf=.01 -tp=.005 -iOmega=1. -coupled=0 -nc=3 -useNewInterfaceTransfer=1 -debug=3 >! junk
*  cgmp -noplot aa -g=twoSquaresInterfacee1.order2   -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.5 -tf=.01 -tp=.005 -iOmega=1. -coupled=0 -nc=3  -useNewInterfaceTransfer=1 [matching case for comparison
*   ..refinement patch: 
*  cgmp -noplot aa -g=twoSquaresInterface1RefineLeft.order2 -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.5 -tf=.01 -tp=.005 -iOmega=1. -coupled=0 -nc=3 -useNewInterfaceTransfer=1 -debug=3 
*  cgmp -noplot aa -g=twoSquaresInterface1Refine.order2 -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.5 -tf=.01 -tp=.005 -iOmega=1. -coupled=0 -nc=3 -useNewInterfaceTransfer=1 -debug=3  [refined both sides
* 
* -- pc: 
* cgmp aa -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.5 -tf=.05 -tp=.005 -ts=pc -nc=1
*
* -- explicit but solve interface equations decoupled: 
* cgmp aa -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.5 -tf=.05 -tp=.005 -iOmega=1. -coupled=0 -nc=3
* cgmp aa -g=twoSquaresInterfacee1.order2.hdf -kappa1=1. -ktc1=1. -kappa2=.1 -ktc2=.5 -tf=.05 -tp=.005 -iOmega=1. -coupled=0 -nc=3 -ts=pc
*
* implicit:
*  cgmp aa -g=twoSquaresInterfacee1.order2.hdf -kappa1=.9 -ktc1=.9 -kappa2=.1 -ktc2=.1 -tf=.5 -tp=.1 -ts=im -nc=6 -coupled=0 -debug=3
*  cgmp aa -g=twoSquaresInterfacee1.order2.hdf -kappa1=.1 -ktc1=.1 -kappa2=.9 -ktc2=.9 -tf=.5 -tp=.1 -ts=im -nc=6 -coupled=0 -debug=3
* 
*  cgmp noplot aa -g=twoSquaresInterfacee1.order2.hdf -kappa1=.9 -kappa2=.2 -tf=.02 -tp=.02 -ts=im -nc=5 >! junka
* 
* -- test implicit and under-relaxed: 
* cgmp noplot aa -g=twoSquaresInterfacee1.order2.hdf -kappa1=1.01 -ktc1=1.01 -kappa2=1. -ktc2=1. -tf=.2 -tp=.1 -ts=im -nc=10 -go=go -iOmega=.64 -coupled=0
* 
* parallel:
*  mpirun -np 1 $cgmpp aa.cmd
*  mpirun -np 1 -dbg=valgrindebug $cgmpp noplot aa.cmd
*
* --- set default values for parameters ---
* 
$grid="twoSquaresInterfacee1.order2.hdf";
$tFinal=10.; $tPlot=.1; $show = " "; $debug=0; $cfl=.9; $ghost=0; $show=""; 
$solver="yale";  $go="halt"; $dtMax=.1; $coupled=1; $tz="poly"; $uMin=2.; $uMax=4.;
* $ts="fe";            # mp solver
* $tsd="forward Euler"; $numberOfCorrections=0;  # domain solver
$ts="pc"; $numberOfCorrections=1;
$degreex1=2; $degreet1=2; $a1=0.; $b1=0.; 
$degreex2=2; $degreet2=2; $a2=0.; $b2=0.; 
$kappa1=.1; $kappa2=.1; 
$ktc1=-1.; $ktc2=-1.;   # by default set ktc equal to kappa
$iTol=1.e-3; $iOmega=1.; $useNewInterfaceTransfer=0; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal, "bg=s"=>\$backGround,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz,"degreex1=i"=>\$degreex1, "degreet1=i"=>\$degreet1,\
 "degreex2=i"=>\$degreex2, "degreet2=i"=>\$degreet2,"show=s"=>\$show,"ts=s"=>\$ts,"go=s"=>\$go,\
 "debug=i"=>\$debug,"nc=i"=> \$numberOfCorrections,"iOmega=f"=>\$iOmega,"noplot=s"=>\$noplot,\
 "kappa1=f"=>\$kappa1,"kappa2=f"=>\$kappa2,"ktc1=f"=>\$ktc1,"ktc2=f"=>\$ktc2,"coupled=i"=>\$coupled,\
 "useNewInterfaceTransfer=i"=>\$useNewInterfaceTransfer );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $ts eq "fe" ){ $ts="forward Euler";  $tsd="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; $tsd="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       $tsd="implicit";  }
if( $ts eq "pc" ){ $ts="adams PC";       $tsd="adams PC";  }
if( $ts eq "mid"){ $ts="midpoint";       $tsd="forward Euler"; }  
if( $ktc1 < 0. ){ $ktc1=$kappa1; }if( $ktc2 < 0. ){ $ktc2=$kappa2; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
* 
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $uMin=-1; $uMax=1.; }
* 
* $grid="twoSquaresInterface0e.hdf"; $debug=0; $kappa1=1.; $kappa2=.5; $tPlot=.001; $debug=0;
* $grid="twoSquaresInterfacee1.order2.hdf"; $debug=0; $kappa1=1.; $kappa2=.5; $tPlot=.01;
* $grid="twoBoxesInterface1.hdf"; $debug=3; $kappa1=1.; $kappa2=.5;
*
* $grid="twoBoxesInterfacee111.order2.hdf"; $debug=0; $kappa1=1.; $kappa2=.5; $tPlot=.01;
* $grid="twoBoxesInterface2.hdf"; 
* $grid="innerOuter2d.hdf"; $kappa1=1.; $kappa2=.5; $tFinal=.2; $tPlot=.05; $show="aa.show";
* $grid="innerOuter3d.hdf";  $kappa1=.1; $kappa2=.05; $tFinal=.2; $tPlot=.05; 
* $grid="innerOuter3d2.hdf";  $degreex1=0; $degreex2=0;
*
$grid
* 
* ------- start new domain ---------- 
$domainName=leftDomain; $solverName="solidA"; 
$bc = "all=dirichletBoundaryCondition\n bcNumber100=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
$kappa=$kappa1; $ktc=$ktc1; $degreeSpace=$degreex1; $degreeTime=$degreet1; $a=$a1; $b=$b1; 
include adDomain.h
* ------- start new domain ---------- 
$domainName=rightDomain; $solverName="solidB"; 
$bc = "all=dirichletBoundaryCondition\n bcNumber100=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
$kappa=$kappa2; $ktc=$ktc2; $degreeSpace=$degreex2; $degreeTime=$degreet2; $a=$a2; $b=$b2; 
include adDomain.h
* 
 continue
* -- set parameters for cgmp ---
*  midpoint
  $ts 
  number of PC corrections $numberOfCorrections
  OBPDE:interface tolerance $iTol
  OBPDE:interface omega $iOmega
  OBPDE:solve coupled interface equations $coupled
  OBPDE:use new interface transfer $useNewInterfaceTransfer
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
      1
    exit
  continue
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
