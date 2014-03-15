*
* cgmp: solve for INS/CNS and AD in two or more domains
*     : flow around a heated cylinder (2d or 3d)
* 
* Usage:
*    cgmp [-noplot] io -g=<name> -method=[ins|cns] -nu=<> -mu=<> -kappa=<num> -tf=<tFinal> -tp=<tPlot> ...
*           -solver=<yale/best> -ktcFluid=<> -ktcFluid=<> -tz=[poly/trig/none] -bg=<backGroundGrid> ...
*           -nc=<num> degreex=<num> -degreet=<num> -ts=[fe|be|im|pc] -coupled=[0|1]  -mixedInterface=[0|1] ...
*           -go=[run/halt/og]
* 
*  -ktcFluid -ktcSolid : thermal conductivities 
*  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep
*  -nc : number of correction steps for implicit predictor-corrector
* 
* Examples:   (grid innerOuter2d built with Overture/sampleGrids/io.cmd)
* 
*  cgmp io.cmd -g="innerOuter2d.hdf" -nu=.1 -kappa=.05 -tf=1. 
*  cgmp io.cmd -g="innerOuter2d2.hdf" -nu=.05 -kThermal=.04 -ktcFluid=.03 -kappa=.02 -ktcSolid=.025
*  cgmp io.cmd -g="innerOuter2d.hdf" -tz=trig -solver=yale 
*  cgmp io.cmd -g="diskArray2x2ye1.order2.hdf" -nu=.05 -kappa=.01 -tp=.05 -solver=yale
*  cgmp io.cmd -g="diskArray2x2ye2.order2.hdf" -nu=.02 -kappa=.005 -tp=.05 -solver=yale
*
* -- remember: don't make dt too big, set -rf 
* cgmp io.cmd -g="innerOuter2d.hdf" -solver="yale" -nu=.1 -kappa=.05 -ktcSolid=.5 -tp=.05 -ts=imp -iv=full -coupled=0 -nc=20 -iTol=1.e-3 -debug=3 -dtMax=.01 -rf=5
*  --- 3D ---
*  cgmp io.cmd -g="innerOuter3d.hdf" -nu=.1 -kappa=.025 -tp=.05 
*  cgmp io.cmd -g="innerOuter3d.hdf" -solver="best" -nu=.1 -kappa=.025 -ktcSolid=.5 -tp=.05 -dtMax=.01  -ts=imp -iv=full -coupled=0 -nc=10 -iTol=1.e-3 -rf=5 -debug=3 
*
*  --- parallel examples --
*  srun -N1 -n1 -ppdebug $cgmpp io.cmd -g="innerOuter2d.hdf" -show="io.show" 
*  srun -N1 -n1 -ppdebug $cgmpp io.cmd -g="diskArray2x2ye2.order2.hdf" -nu=.02 -kappa=.005 -tp=.05 -show="diskArray.show" 
*  srun -N1 -n4 -ppdebug $cgmpp io.cmd -g="innerOuter3d.hdf" -nu=.1 -kappa=.025 -tp=.05 -bg=outerBox -show="io3d.show" 
*
* -- implicit
*   cgmp io.cmd -g="innerOuter2d.hdf" -nu=.1 -kappa=.05 -tf=1. -ts=im -coupled=0 -iTol=1.e-3 -nc=30 -debug=3 -iOmega=.85 -tp=.01 
*   cgmp io.cmd -g="innerOuter2d4.hdf" -nu=.01 -kappa=.1 -tf=1. -ts=im -coupled=0 -iTol=1.e-3 -nc=30 -debug=3 -tp=.01 
*   cgmp noplot io.cmd -g="innerOuter2d8.hdf" -nu=.005 -ktcFluid=.01 -kappa=.1 -tf=1. -ts=im -coupled=0 -iTol=1.e-3 -nc=10 -debug=3 -tp=.05 -tf=.5 -dtMax=.01 -ad2=1 -solver=yale -show="io8.show" >! io8.out 
*   cgmp io.cmd -g="innerOuter2d.hdf" -tf=1. -ts=im -coupled=1 -nc=4
*   cgmp io.cmd -g="diskArray2x2ye1.order2.hdf" -nu=.05 -kappa=.01 -tp=.05 -solver=yale -ts=im -coupled=0 -nc=6 -debug=3
*   cgmp io.cmd -g="innerOuter2d.hdf" -tf=1. -ts=im -coupled=0 -nc=8 -tp=.01 -dtMax=.01 -tz=trig -kThermal=1. -ktcFluid=1. -kappa=1. -ktcSolid=1. -solver=yale -debug=3 -iTol=1.e-6 -mixedInterface=1
* -- convergence rates of the interface iterations:
*    cgmp io.cmd -g="innerOuter2d4.hdf" -tf=1. -ts=im -coupled=0 -nc=8 -tp=.01 -dtMax=.01 -tz=trig -debug=3 
*    cgmp io.cmd -g="innerOuter2d8.hdf" -tf=1. -ts=im -coupled=0 -nc=8 -tp=.01 -dtMax=.01 -tz=trig -kThermal=1. -ktcFluid=1. -kappa=.99 -ktcSolid=.99 -solver=best -debug=3 -iTol=1.e-6 -mixedInterface=1
* 
* -- compressible examples
*  cgmp io.cmd -g="innerOuter2d2.hdf" -method=cns -mu=.05 -T0=300 -Twall=400
*  cgmp io.cmd -g="innerOuter2d2.hdf" -method=cns -mu=.05 -T0=100 -Twall=200 -tp=.05 
*
**** TROUBLE: mixed-BC plus changing dt
* cgmp io.cmd -g="innerOuter2d.hdf" -tf=1. -ts=im -coupled=0 -nc=8 -tp=.05 -dtMax=.05 -tz=trig -kThermal=1. -ktcFluid=1. -kappa=1. -ktcSolid=.1 -solver=yale -debug=3 -iTol=1.e-6 -mixedInterface=1 
* 
* --- set default values for parameters ---
* 
$grid="innerOuter2d.hdf"; $method="ins"; $go="halt"; 
$ts="pc";  $numberOfCorrections=1; $coupled=1; $iOmega=1.; $iTol=1.e-3; $dtMax=.1;
$degreeSpace=2; $degreeTime=2;  $u0=0.; $T0=10.; $Twall=10.; 
$nu=.025; $kThermal=-1.; $ktcFluid=.05; $kappa=.04; $ktcSolid=.5; $ad2=0; 
$gravity = "0 -10. 0."; $solver="best"; $dtMax=.05; 
$rtolp=1.e-5; $atolp=1.e-7; $rtoli=1.e-7; $atoli=1.e-9; 
$fx1=1.; $fx2=2.; 
* 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
*
* ----------------------------- get command line arguments ---------------------------------------
*  -- first get any commonly used options: (requires the env variable CG to be set)
$getCommonOptions = "$ENV{'CG'}/mp/cmd/getCommonOptions.h";
include $getCommonOptions
*  -- now get additional options: 
GetOptions( "degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"fx1=f"=>\$fx1,"fx2=f"=>\$fx2,"T0=f"=>\$T0,"Twall=f"=>\$Twall,\
            "ad2=i"=>\$ad2 );
* -------------------------------------------------------------------------------------------------
if( $kThermal < 0 && $method eq "ins" ){ $kThermal=$nu/$prandtl; }; 
if( $kThermal < 0 && $method eq "cns" ){ $kThermal=$mu/$prandtl; }; 
* 
$grid
* 
* ------- specify fluid domain ----------
$domainName=outerDomain; $solverName="fluid"; 
$bc = "all=noSlipWall\n bcNumber100=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
* $bc = "all=slipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
if( $tz eq "turn off twilight zone" ){$ic = "uniform flow\n p=1., u=$u0";}
$ktc=$ktcFluid; 
$fx=$fx1; $fy=$fx1; $fz=$fx1; $ft=$fx1;
if( $method eq "ins" ){ $cmd = "include $ENV{CG}/mp/cmd/insDomain.h"; }else{ $cmd ="*"; };
$cmd
*
*  Cgcns:
$bc = "all=noSlipWall uniform(u=.0,T=$T0)\n bcNumber100=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
* gravitationally stratified : rho(y) = rho0*exp( gravity[1]/(Rg*T0) ( y - y0 ))
$rho0=1.; $y0=0.; 
if( $tz eq "turn off twilight zone" ){ $ic = "OBIC:user defined...\n gravitationally stratified\n $rho0 $y0\n r=$rho0 u=0. v=0. T=$T0\n exit";}
if( $method eq "cns" ){ $cmd = "include cnsDomain.h"; }else{ $cmd ="*"; };
$cmd
* 
* ------- specify solid domain ----------
$domainName=innerDomain; $solverName="solid"; 
* $bc = "all=dirichletBoundaryCondition, uniform(T=$Twall)\n bcNumber100=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
$bc = "all=neumannBoundaryCondition\n bcNumber100=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber100=heatFluxInterface";
if( $tz eq "turn off twilight zone" ){ $ic = "uniform flow\n" . "T=$Twall\n";}
$ktc = $ktcSolid;
$fx=$fx2; $fy=$fx2; $fz=$fx2; $ft=$fx2;
include $ENV{CG}/mp/cmd/adDomain.h
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
  OBPDE:use mixed interface conditions $mixedInterface
* 
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
plot:fluid : T
$go



 plot:solid : T


      contour
        *min max 0 $Twall
        exit
        *min max 0 $Twall
        exit


  movie mode
  finish
