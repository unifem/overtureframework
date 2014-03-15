*
* cgmp: co-centric cylinders heat exchanger example
* 
* Usage:
*   cgmp [-noplot] cocentricCylinders -g=<name> -nu=<num> -kFluid=<num> -kappaFluid=<> -kSolid=<> -kappaSolid=<> ...
*        -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -tz=[poly/trig/none] -ts=[fe|be|im|pc] -nc=<> -coupled=[0|1]-go=[run/halt/og]
* 
*  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep
* 
* Examples: 
* 
*  cgmp cocentricCylinders.cmd -g="cocentricCylindersi2p.order2" -solver=best -tp=.01 -coupled=1 -nu=.05 -kappaSolid=1. -kFluid=.2 -kappaFluid=.2 
*
* -- implicit:
*    CR=.48 -> iOmega=1/(1+CR) = .67 
*  cgmp cocentricCylinders.cmd -g="cocentricCylindersi2p.order2" -tp=.1 -ts=im -nu=.05 -kSolid=1. -kappaSolid=1. -kFluid=.2 -kappaFluid=.2 -solver=best -coupled=0 -nc=10 -iTol=1.e-3 -iOmega=.67 -debug=3 -go=halt 
*
* Runs for paper:
*
* cgmp noplot cocentricCylinders.cmd -g="cocentricCylindersi1p.order2" -tf=10. -tp=1. -ts=im -nu=.05 -kSolid=1. -kappaSolid=1. -kFluid=.2 -kappaFluid=.2 -solver=best -coupled=0 -nc=10 -iTol=1.e-3 -iOmega=.67 -debug=3 -go=halt -show=cocentric1.show >! cocentric1.out &
* cgmp noplot cocentricCylinders.cmd -g="cocentricCylindersi2p.order2" -tf=10. -tp=1. -ts=im -nu=.05 -kSolid=1. -kappaSolid=1. -kFluid=.2 -kappaFluid=.2 -solver=best -coupled=0 -nc=10 -iTol=1.e-3 -iOmega=.67 -debug=3 -go=halt -show=cocentric2.show >! cocentric2.out &
* reduce iTol from 1e-3 to 1e-4 : 
* nohup $mp/bin/cgmp noplot cocentricCylinders.cmd -g="cocentricCylindersi4p.order2" -tf=10. -tp=1. -ts=im -nu=.05 -kSolid=1. -kappaSolid=1. -kFluid=.2 -kappaFluid=.2 -solver=best -coupled=0 -nc=10 -iTol=1.e-4 -rtolp=1.e-6 -rtoli=1.e-6 -iOmega=.67 -debug=3 -go=go -show=cocentric4a.show >! cocentric4a.out &
* nohup $mp/bin/cgmp noplot cocentricCylinders.cmd -g="cocentricCylindersi8p.order2" -tf=10. -tp=1. -ts=im -nu=.05 -kSolid=1. -kappaSolid=1. -kFluid=.2 -kappaFluid=.2 -solver=best -coupled=0 -nc=10 -iTol=1.e-5 -rtolp=1.e-9 -rtoli=1.e-9 -iOmega=.67 -debug=3 -go=go -show=cocentric8.show >! cocentric8.out &
*
* --- full implicit 
* nohup $mp/bin/cgmp noplot cocentricCylinders.cmd -g="cocentricCylindersi1p.order2" -tf=10. -tp=1. -ts=im -nu=.05 -kSolid=1. -kappaSolid=1. -kFluid=.2 -kappaFluid=.2 -solver=best -ts=im -iv=full -dtMax=.2 -coupled=0 -nc=10 iTol=1.e-4 -rtolp=1.e-6 -rtoli=1.e-6 -iOmega=.67 -debug=3 -go=halt -show=cocentric1.show >! cocentric1.out &
* nohup $mp/bin/cgmp noplot cocentricCylinders.cmd -g="cocentricCylindersi2p.order2" -tf=10. -tp=1. -ts=im -nu=.05 -kSolid=1. -kappaSolid=1. -kFluid=.2 -kappaFluid=.2 -solver=best -ts=im -iv=full -dtMax=.1 -coupled=0 -nc=10 iTol=1.e-4 -rtolp=1.e-6 -rtoli=1.e-6 -iOmega=.67 -debug=3 -go=halt -show=cocentric2f.show >! cocentric2f.out &
* nohup $mp/bin/cgmp noplot cocentricCylinders.cmd -g="cocentricCylindersi4p.order2" -tf=10. -tp=1. -ts=im -nu=.05 -kSolid=1. -kappaSolid=1. -kFluid=.2 -kappaFluid=.2 -solver=best -ts=im -iv=full -dtMax=.2 -coupled=0 -nc=10 iTol=1.e-5 -rtolp=1.e-8 -rtoli=1.e-8 -iOmega=.67 -debug=3 -go=halt -show=cocentric4.show >! cocentric4.out &
* -- too much memory: 
* nohup $mp/bin/cgmp noplot cocentricCylinders.cmd -g="cocentricCylindersi8p.order2" -tf=10. -tp=1. -ts=im -nu=.05 -kSolid=1. -kappaSolid=1. -kFluid=.2 -kappaFluid=.2 -solver=best -ts=im -iv=full -dtMax=.2 -coupled=0 -nc=10 iTol=1.e-5 -rtolp=1.e-9 -rtoli=1.e-9 -iOmega=.67 -debug=3 -go=halt -show=cocentric8.show >! cocentric8.out &
* 
* zeus:
* srun -N1 -n4 -ppdebug $cgmpp noplot cocentricCylinders.cmd -g="cocentricCylindersi2p.order2" -tf=10. -tp=1. -ts=im -nu=.05 -kSolid=1. -kappaSolid=1. -kFluid=.2 -kappaFluid=.2 -solver=best -ts=im -iv=full -dtMax=.1 -coupled=0 -nc=10 iTol=1.e-4 -rtolp=1.e-6 -rtoli=1.e-6 -iOmega=.67 -debug=3 -go=halt -show=cocentric2.show >! cocentric2.out &
* 
* srun -N2 -n8 -ppdebug $cgmpp noplot cocentricCylinders.cmd -g="cocentricCylindersi8p.order2" -tf=10. -tp=1. -ts=im -nu=.05 -kSolid=1. -kappaSolid=1. -kFluid=.2 -kappaFluid=.2 -solver=best -ts=im -iv=full -dtMax=.2 -coupled=0 -nc=10 iTol=1.e-5 -rtolp=1.e-9 -rtoli=1.e-9 -iOmega=.67 -debug=3 -go=halt -show=cocentric8.show >! cocentric8.out &
* 
* --- set default values for parameters ---
* 
$grid="curvedPipei1.order2.hdf";  $backGround="solid"; 
$tFinal=2.; $tPlot=.02; $degreeSpace=2; $degreeTime=2; $cfl=.9; $show=""; $debug=1; 
$ts="pc";  $numberOfCorrections=1; $coupled=1; $iOmega=1.; $iTol=1.e-3; $dtMax=.1;
$nu=.05; $kFluid=.2; $kappaFluid=.2;  $thermalExpansivity=1.; $pdebug=0; 
$Ta=5.; $Tb=1.;   # T0 = initial T in the solid 
$tz="none";
$gravity = "0 -1. 0.";
$kSolid="1."; $kappaSolid=1.; 
$solver="best"; $go="halt"; $extrapolateInitialInterfaceValues="1"; $implicitFactor=1.; 
$atolp=1.e-7; $rtolp=1.e-5; 
* 
$bcInflow=10;       # bc for inflow boundaries on the tube
$bcOutflow=11;      # bc for outflow boundaries on the tube
* 
$solver="best"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
*
* ----------------------------- get command line arguments ---------------------------------------
*  -- first get any commonly used options: (requires the env variable CG to be set)
$getCommonOptions = "$ENV{'CG'}/mp/cmd/getCommonOptions.h";
include $getCommonOptions
GetOptions( "degreex=i"=>\$degreex, "degreet=i"=>\$degreet,\
 "kSolid=f"=>\$kSolid,"kappaSolid=f"=>\$kappaSolid, "kFluid=f"=>\$kFluid, "kappaFluid=f"=>\$kappaFluid,\
  );
* -------------------------------------------------------------------------------------------------
* $kThermal=$nu/$prandtl;
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
* 
* ------- Assign domains ----------
$domainName="innerDomain"; $solverName="solid"; $ktc=$ktcSolid;
$TSolid=.5*($Ta+$Tb); 
if( $tz eq "turn off twilight zone" ){$ic = "uniform flow\n T=$TSolid";}
if( $tz eq "turn off twilight zone" ){ $bc = "all=dirichletBoundaryCondition, uniform(T=$Ta)\n $siface100" }else{ $bc = "all=dirichletBoundaryCondition\n $siface100"; }
* $bc = "all=dirichletBoundaryCondition"; 
$kappa=$kappaSolid; $ktc=$kSolid; 
$fx=.5; $fy=$fx; $fz=$fx; $ft=$fx;
include adDomain.h
*
$domainName="outerDomain"; $solverName="fluid"; $Twall=0.; $ktc=$kFluid; $kThermal=$kappaFluid; 
* $bc = "bcNumber$bcInflow=inflowWithVelocityGiven, parabolic(d=.2,p=1.,w=-1.,T=0.)\n bcNumber$bcOutflow=outflow\n $iface100"; 
if( $tz eq "turn off twilight zone" ){ $bc = "all=noSlipWall uniform(T=$Tb)\n bcNumber$bcInflow=inflowWithVelocityGiven, parabolic(d=.2,p=1.,w=-1.,T=0.)\n bcNumber$bcOutflow=outflow\n $iface100"; } else{ $bc = "all=dirichletBoundaryCondition\n $iface100"; }
* $bc = "all=dirichletBoundaryCondition"; 
$ts0=$ts; 
$TFluid=.5*($Ta+$Tb);
if( $tz eq "turn off twilight zone" ){$ic = "uniform flow\n u=0., v=0., w=0., p=0., T=$TFluid"; }
$fx=1.; $fy=$fx; $fz=$fx; $ft=$fx;
include insDomain.h
$ts=$ts0; 
* 
continue
* 
* -- set parameters for cgmp ---
* 
  final time $tFinal
  times to plot $tPlot
  debug $debug
  $ts 
* 
  number of PC corrections $numberOfCorrections
  OBPDE:interface tolerance $iTol
  OBPDE:interface omega $iOmega
  OBPDE:solve coupled interface equations $coupled
  OBPDE:extrapolate initial interface values $extrapolateInitialInterfaceValues
* 
  cfl $cfl
  $tz
* 
  show file options
    compressed
      open
       $show
    frequency to flush
      10
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


