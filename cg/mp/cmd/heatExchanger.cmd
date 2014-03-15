*
* cgmp: heat exchanger example: fluid in a tube that winds through a solid.
* 
* Usage:
*   cgmp [-noplot] heatExchanger -g=<name> -nu=<num> -kappa=<num> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> ...
*                      -tz=[0/1] -bg=<backGroundGrid> -ts=<fe/be/im> -ktcSolid=<> -ktcFluid=<> -go=[run/halt/og]
* 
*  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep
* 
* Examples: 
* 
*  cgmp heatExchanger.cmd -g="heatExchangeri1.order2.hdf" -solver=best
*  cgmp noplot heatExchanger.cmd -g="heatExchangere2.order2.hdf" -solver=best -show="he.show" -tp=.1 -tf=.1 -go=go
*
* -- implicit
*  cgmp heatExchanger.cmd -g="heatExchangeri1.order2.hdf" -solver=best -ts=im -nc=20 -dtMax=.05 -tp=.05 -coupled=0 -iOmega=.65 -debug=3 
*
*  cgmp heatExchanger.cmd -g="heatExchangere2.order2.hdf" -solver=best -ts=im -nc=20 -dtMax=.05 -tp=.05 -coupled=0 -iOmega=1.  -debug=3 
*
* --- set default values for parameters ---
* 
$grid="heatExchangeri1.order2.hdf";  $backGround="solid"; 
$tFinal=2.; $tPlot=.02; $degreeSpace=2; $degreeTime=2; $cfl=.9; $show=""; 
$ts="pc"; $numberOfCorrections=1; $implicitFactor=.5; $iTol=1.e-3; $iOmega=1.; $dtMax=1.; 
$nu=.025; $prandtl=.72;  $ktcFluid=""; $thermalExpansivity=1.;  $kappa=.1; $ktcSolid=""; $pdebug=0; 
$T0=10.; $Twall=10.;   # T0 = initial T in the solid 
$tz=none; # turn on tz here
* $gravity = "0 -10. 0.";
$gravity = "0  0. 0."; $adcBoussinesq=1.;
$solver="best"; $go="halt"; 
* 
$bcInflow=10;       # bc for inflow boundaries on the tube
$bcOutflow=11;      # bc for outflow boundaries on the tube
* 
$solver="best"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"nu=f"=>\$nu,"kappa=f"=>\$kappa, "bg=s"=>\$backGround,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,\
 "show=s"=>\$show,"ts=s"=>\$ts,"ktcSolid=f"=>\$ktcSolid,"ktcFluid=f"=>\$ktcFluid,"coupled=i"=>\$coupled,\
 "nc=i"=> \$numberOfCorrections,"iOmega=f"=>\$iOmega,"iTol=f"=>\$iTol,"dtMax=f"=>\$dtMax,"debug=i"=>\$debug,\
 "noplot=s"=>\$noplot,"adcBoussinesq=f"=>\$adcBoussinesq );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $ts eq "fe" ){ $ts="forward Euler";  }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       }
if( $ts eq "pc" ){ $ts="adams PC";       }
if( $ts eq "mid"){ $ts="midpoint";       }  
* 
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $uMin=-1; $uMax=1.; }
* 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
$kThermal=$nu/$prandtl;
if( $ktcSolid eq "" ){ $ktcSolid=$kappa;}  # Solid thermal conductivity 
if( $ktcFluid eq "" ){ $ktcFluid=$kThermal;} # Fluid thermal conductivity 
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
* ------- Assign domains ----------
$domainName="solidDomain"; $solverName="solid"; $ktc=$ktcSolid;
* $bc = "all=dirichletBoundaryCondition, uniform(T=$Twall)\n $backGround(0,2)=dirichletBoundaryCondition, uniform(T=$Twall)\n $siface100";
$bc = "all=neumannBoundaryCondition\n $backGround(0,2)=dirichletBoundaryCondition, uniform(T=$Twall)\n $siface100";
include adDomain.h
*
$domainName="tubeDomain"; $solverName="fluid"; $Twall=0.; $ktc=$ktcFluid;
$bc = "bcNumber$bcInflow=inflowWithVelocityGiven, parabolic(d=.2,p=1.,v=1.,T=0.)\n bcNumber$bcOutflow=outflow\n $iface100"; 
* $bc = "all=noSlipWall\n bcNumber$bcInflow=inflowWithVelocityGiven, parabolic(d=.2,p=1.,v=1.,T=0.)\n bcNumber$bcOutflow=outflow\n"; 
$ic="uniform flow\n" . "p=1., u=$u0, T=0.";
$ts0=$ts; $ts=$ts . "\n project initial conditions";
include insDomain.h
$ts=$ts0; 
* 
continue
* 
* -- set parameters for cgmp ---
* 
  final time $tFinal
  times to plot $tPlot
  $ts 
* 
  number of PC corrections $numberOfCorrections
  OBPDE:interface tolerance $iTol
  OBPDE:interface omega $iOmega
  OBPDE:solve coupled interface equations $coupled
* 
  cfl $cfl
  debug $debug
  $tz
* 
  show file options
    compressed
      open
       $show
    frequency to flush
      2
    exit
  continue
*
continue
$go




   plot:fluidA : T
   plot:solidA : T
   plot:solidB : T
   plot:solidC : T
   plot:solidD : T
   plot:fluidB : T


