*
* cgmp: curved pipe example: fluid in a curved pipe
* 
* Usage:
*   cgmp [-noplot] curvedPipe -g=<name> -nu=<num> -kappa=<num> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> ...
*             -tz=[poly/trig/none] -bg=<backGroundGrid> -ts=[fe|be|im|pc] -nc=<> -coupled=[0|1]-go=[run/halt/og]
* 
*  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep
* 
* Examples: 
* 
*  cgmp curvedPipe.cmd -g="curvedPipeFixed90e1.order2.hdf" -solver=best -tp=.01 -show="curvedPipe.show" 
*  cgmp curvedPipe.cmd -g="curvedPipeFixed90e1.order2.hdf" -solver=best -tf=.1 -tp=.05 -show="curvedPipe.show" -gravity="0 0 -10" 
*  cgmp curvedPipe.cmd -g="curvedPipeFixed90e1.order2.hdf" -solver=best -tz=trig -tp=.01
* 
* 
*  cgmp curvedPipe.cmd -g="curvedPipei1.order2.hdf" -solver=best -tz=trig
*  cgmp curvedPipe.cmd -g="curvedPipei2.order2.hdf" -solver=best -tz=trig
*
* -- implicit:
*  cgmp curvedPipe.cmd -g="curvedPipeFixed90e1.order2.hdf" -solver=best -tp=.1 -ts=imp -iv=viscous -coupled=0 -nc=5 -debug=3 -iTol=1.e-2 -mixedInterface=1
* 
*  cgmp curvedPipe.cmd -g="curvedPipeFixed90e1.order2.hdf" -tp=.01 -ts=im -nu=.1 -kappa=.025 -solver=best -coupled=0 -nc=15 -debug=3 -go=halt
*
*  cgmp noplot curvedPipe.cmd -g="curvedPipee2.order2.hdf" -solver=best -show="he.show" -tp=.1 -tf=.1 -go=go
*
* mpirun -np 1 $cgmpp curvedPipe.cmd -g="curvedPipeFixed90e1.order2.hdf" -solver=best -tz=trig -tp=.01 -tf=.01
* mpirun -np 2 $cgmpp curvedPipe.cmd -g="curvedPipee2.order2.hdf" -solver=best -tz=trig
* 
* srun -N1 -n2 -ppdebug $cgmpp curvedPipe.cmd -g="curvedPipee1.order2.hdf" -solver=best -tz=trig
* 
* --- set default values for parameters ---
* 
$grid="curvedPipei1.order2.hdf";  $backGround="solidPipe"; 
$tFinal=2.; $tPlot=.02; $degreeSpace=2; $degreeTime=2; $cfl=.9; $show=""; $debug=1; 
$ts="pc";  $numberOfCorrections=1; $coupled=1; $iOmega=1.; $iTol=1.e-3; $dtMax=.1;
$nu=.025; $prandtl=.72;  $thermalExpansivity=1.;  $kappa=.05; $pdebug=0; 
$ktcFluid=.1; $ktcSolid=1.; 
$T0=10.; $Twall=10.;   # T0 = initial T in the solid 
$tz="none"; $fx1=.5; $fx2=1.; 
* $gravity = "0 -10. 0.";
$gravity = "0  0. 0.";
$ktcSolid=""; $ktcFluid=""; 
$solver="best"; $go="halt"; 
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
GetOptions( "kappa=f"=>\$kappa, "bg=s"=>\$backGround,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,\
            "fx1=f"=>\$fx1,"fx2=f"=>\$fx2 );
* -------------------------------------------------------------------------------------------------
*
$kThermal=$nu/$prandtl;
if( $ktcSolid eq "" ){ $ktcSolid=$kThermal; }       # Solid thermal conductivity -- do this for now 
if( $ktcFluid eq "" ){ $ktcFluid=$kappa; }          # Fluid thermal conductivity -- do this for now 
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
$domainName="solidDomain"; $solverName="solid"; $ktc=$ktcSolid;
* $bc = "all=dirichletBoundaryCondition, uniform(T=$Twall)\n $iface100";
if( $tz eq "turn off twilight zone" ){$ic = "uniform flow\n T=$Twall";}
if( $tz eq "turn off twilight zone" ){ $bc = "all=neumannBoundaryCondition\n $backGround(1,1)=dirichletBoundaryCondition, uniform(T=$Twall)\n $siface100" }else{ $bc = "all=dirichletBoundaryCondition\n $siface100"; }
* $bc = "all=dirichletBoundaryCondition"; 
$fx=$fx1; $fy=$fx; $fz=$fx; $ft=$fx;
include adDomain.h
*
$domainName="fluidDomain"; $solverName="fluid"; $Twall=0.; $ktc=$ktcFluid;
* $bc = "bcNumber$bcInflow=inflowWithVelocityGiven, parabolic(d=.2,p=1.,w=-1.,T=0.)\n bcNumber$bcOutflow=outflow\n $iface100"; 
if( $tz eq "turn off twilight zone" ){ $bc = "all=noSlipWall\n bcNumber$bcInflow=inflowWithVelocityGiven, parabolic(d=.2,p=1.,w=-1.,T=0.)\n bcNumber$bcOutflow=outflow\n $iface100"; } else{ $bc = "all=dirichletBoundaryCondition\n $iface100"; }
* $bc = "all=dirichletBoundaryCondition"; 
$ts0=$ts; 
if( $tz eq "turn off twilight zone" ){$ic = "uniform flow\n p=1., w=0., T=$Twall"; $ts=$ts . "\n project initial conditions"; }
$fx=$fx2; $fy=$fx; $fz=$fx; $ft=$fx;
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
  OBPDE:use mixed interface conditions $mixedInterface
* 
  cfl $cfl
  $tz
* 
  show file options
    compressed
      open
       $show
    frequency to flush
      8 
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


