* 
* cgmp example: solve the incompressible N-S equations in two domains in the twilight-zone
*
* Usage:
*    cgmp [-noplot] ii -g=<name> -nu1=<> -nu2=<> -kThermal1=<> -kThermal2=<> -ktc1=<> -ktc2=<> ...
*           -tf=<tFinal> -tp=<tPlot>  -solver=<yale/best> -domain1=<name> -domain2=<name> coupled=[0|1] ...
*             -nc=<num> -degreex[1/2]=<num> -degreet[1/2]=<num> -tz=[poly/trig/none] -bg=<backGroundGrid> -ts=<fe/be/im>
* where
*  -ts = time-stepping method, fe="forward Euler", be="backward Euler", mid="mid-point" im="implicit"
*  -nc : number of correction steps for implicit predictor-corrector
*  -kThermal1, -kThermal2 : thermal diffusivity ( T_t + .. = kThermal*Delta(T) )
*  -ktc1, -ktc2 : thermal conductivity ( heat flux = - ktc1 grad(T)  kThermal=k/(rho*C) )
*  -coupled : 1=solve coupled interface equations, 0=solve decoupled 
* 
* Examples:
* 
*  cgmp ii -g=twoSquaresInterfacee1.order2.hdf -kThermal1=1. -kThermal2=.1 -tf=.05 -tp=.01 -coupled=1 -go=halt
*  cgmp ii -g=twoSquaresInterfacee1.order2.hdf -kThermal1=1. -kThermal2=.1 -tf=.05 -tp=.01 -coupled=0 -nc=5 -go=halt
* 
*  cgmp ii -g=innerOuter2d.hdf -domain1=outerDomain -domain2=innerDomain -kThermal1=1. -kThermal2=.1 -go=halt
*
* implicit:
*  cgmp ii -g=twoSquaresInterfacee1.order2.hdf -kThermal1=.9 -kThermal2=.1 -tf=.5 -tp=.02 -ts=im -nc=3 -coupled=0 -go=halt
*  cgmp noplot ii -g=twoSquaresInterfacee1.order2.hdf -kThermal1=.9 -kThermal2=.2 -tf=.02 -tp=.02 -ts=im -degreex1=1 -degreex2=1 -degreet1=0 -degreet2=0 -nc=3 -go=go >! junki
* 
* parallel:
*  mpirun -np 1 $cgmpp ii.cmd
*  mpirun -np 1 -dbg=valgrindebug $cgmpp noplot ii.cmd
*
* --- set default values for parameters ---
* 
$tFinal=2.; $tPlot=.1; $debug=0; $cfl=.9; $dtMax=.1; $go="halt"; 
$grid="twoSquaresInterfacee1.order2.hdf"; $coupled=1; 
$domain1="leftDomain"; $domain2="rightDomain";
$show = " "; $ghost=0; $numberOfCorrections=1; 
$gravity = "0. -1. 0.";
$gravity = "0. 0. 0.";
$solver="yale";  $rtol=1.e-10; $atol=1.e-12;  
* -- set the solver:
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=1; 
$ts="pc";  # mp solver
$tsd="pc"; # domain solver
$tz="poly";
$degreex1=2; $degreet1=1; $a1=0.; $b1=0.; 
$degreex2=2; $degreet2=1; $a2=0.; $b2=0.; 
$nu1=.1; $nu2=.1; $kThermal1=.1; $kThermal2=.1; $ktc1=-1.; $ktc2=-1; $thermalExpansivity=.1; 
$iTol=1.e-3; $iOmega=1.; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"kThermal1=f"=>\$kThermal1,"kThermal2=f"=>\$kThermal2, "bg=s"=>\$backGround,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz,"degreex1=i"=>\$degreex1, "degreet1=i"=>\$degreet1,\
 "degreex2=i"=>\$degreex2, "degreet2=i"=>\$degreet2,"show=s"=>\$show,"ts=s"=>\$ts,"go=s"=>\$go,\
 "debug=i"=>\$debug,"nc=i"=> \$numberOfCorrections,"noplot=s"=>\$noplot,\
 "domain1=s"=>\$domain1,"domain2=s"=>\$domain2,"ktc1=f"=>\$ktc1,"ktc2=f"=>\$ktc2,"coupled=i"=>\$coupled );
* -------------------------------------------------------------------------------------------------
if( $ktc1 < 0. ){ $ktc1=$kThermal1; }if( $ktc2 < 0. ){ $ktc2=$kThermal2; }
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $ts eq "fe" ){ $ts="forward Euler";  $tsd="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; $tsd="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       $tsd="implicit";  }
if( $ts eq "mid"){ $ts="midpoint";       $tsd="forward Euler"; }  # the midpoint rule uses forward-euler on each domain
if( $ts eq "pc" ){ $ts="adams PC";       $tsd="adams PC";  }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
* 
* $ksp="bcgs"; $pc="hypre"; 
* 
*
* $grid="twoSquaresInterface1e.hdf"; $degreeX=1; $degreeT=1; $debug=0;
* $grid="innerOuter2d.hdf"; $degreeX=1; $degreeT=0; $debug=0; $tPlot=.01; $domain1="outerDomain"; $domain2="innerDomain";
* $grid="innerOuter3d.hdf"; $degreeX=1; $degreeT=1; $debug=0; $tPlot=.01; 
*
$grid
* 
*   define real parameter interfaceTolerance $iTol
*   define real parameter interfaceOmega $iOmega
* ------- start new domain ----------
$domainName=$domain1; $solverName="fluidA"; 
$bc = "all=dirichletBoundaryCondition\n bcNumber100=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
$kThermal=$kThermal1; $ktc=$ktc1; $degreeSpace=$degreex1; $degreeTime=$degreet1;
include insDomain.h
*
* ------- start new domain ----------
$domainName=$domain2; $solverName="fluidB"; 
$bc = "all=dirichletBoundaryCondition\n bcNumber100=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
$kThermal=$kThermal2; ktc=$ktc2; $degreeSpace=$degreex2; $degreeTime=$degreet2;
include insDomain.h
* 
continue
*
* -- set parameters for cgmp ---
* 
  final time $tFinal
  times to plot $tPlot
* 
  $ts 
  number of PC corrections $numberOfCorrections
  OBPDE:interface tolerance $iTol
  OBPDE:interface omega $iOmega
  OBPDE:solve coupled interface equations $coupled
* 
* 
  show file options
   * compressed
      open
       $show
    frequency to flush
      10
    exit
  continue
* 
  continue
plot:fluidA : T
plot:fluidB : T
    contour
      min max 0 1.5
      exit
      min max 0 1.5
      exit
continue
$go

