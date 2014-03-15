*
* cgmp: flow past solid fuel rods in an hexagonal container
* 
* Usage:
*   cgmp [-noplot] solidFuelAssembly -g=<name> -nu=<num> -kFluid=<num> -kappaFluid=<> -kSolid=<> -kappaSolid=<> ...
*        -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -tz=[poly/trig/none] -ts=[fe|be|im|pc] -nc=<> ...
*        -iv=[viscous|full] -coupled=[0|1]-go=[run/halt/og]
* 
*  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep
* 
* Examples: 
* 
*  cgmp noplot solidFuelAssembly.cmd -g="solidFuelAssembly1pinsi1.order2" -solver=best -ts=imp -coupled=0 -nc=8 -dtMax=.05 -tp=.05  -tf=.2 -show="sfa.show"
*  cgmp solidFuelAssembly.cmd -g="solidFuelAssembly3pinsi1.order2" -solver=best -ts=imp -coupled=0 -nc=8 -dtMax=.05 -tp=.05  
*
* parallel: 
*  mpirun -np 1 $cgmpp solidFuelAssembly.cmd -g="solidFuelAssembly1pinse1.5.order2" -solver=best -ts=imp -coupled=0 -nc=8 -dtMax=.05 -tp=.05 -tf=.2 
*  srun -ppdebug -N2 -n2 $cgmpp noplot solidFuelAssembly.cmd -g="solidFuelAssembly1pinse1.5.order2" -solver=best -ts=imp -coupled=0 -nc=8 -dtMax=.05 -tp=.05 -tf=.2 -show="sfa.show" >! sfa1.out &
*  srun -ppdebug -N2 -n2 $cgmpp noplot solidFuelAssembly.cmd -g="solidFuelAssembly3pinse1.order2" -solver=best -ts=imp -coupled=0 -nc=8 -dtMax=.05 -tp=.05 -tf=.2 -show="sfa3.show" >! sfa3.out &
*  totalview srun -a -N1 -n2 -ppdebug $cgmpp noplot solidFuelAssembly.cmd -g="solidFuelAssembly1pinse1.5.order2" -solver=best -ts=imp -coupled=0 -nc=8 -dtMax=.05 -tp=.05 -tf=.2 -show="sfa.show" 
* 
* srun -ppdebug -N2 -n4 $cgmpp noplot solidFuelAssembly.cmd -g="solidFuelAssembly3pinsl5e1" -solver=best -ts=imp -coupled=0 -nc=8 -dtMax=.05 -tp=.05 -tf=.2 -show="sfa3l5.show" >! sfa3l5.out &
* 
* --- set default values for parameters ---
* 
$grid="curvedPipei1.order2.hdf";  $backGround="solid"; 
$tFinal=2.; $tPlot=.02; $degreeSpace=2; $degreeTime=2; $cfl=.9; $show=""; $debug=1; 
$ts="pc";  $numberOfCorrections=1; $coupled=1; $iOmega=1.; $iTol=1.e-3; $dtMax=.1;
$nu=.05; $prandtl=.72; $kappaFluid=$nu/$prandtl; $kFluid=.05;  $thermalExpansivity=.1; $pdebug=0; 
$mixedInterface=0; $implicitFactor=1.; $implicitVariation="viscous"; $ad2=0; 
$tz="none";
$gravity = "0 0. -1.";
$kRods=.5; $kappaRods=.1; $heatSourceRods=1.; 
$kContainer=1.; $kappaContainer=.1; 
$solver="best"; $go="halt"; 
$atolp=1.e-7; $rtolp=1.e-5; $projectInitialConditions="project initial conditions"; 
* 
* --- coversions (from the "units" program)
*   MeV = 1.6021765e-13 J,   J = 6.2415097e+12 MeV
* -- from Kyle: (rfa_array_vulcan.cmd)
* UO2 material information
*
$U02_density = 10.5;
$U02_specific_heat = (281.*6.2415097e+9); # MeV/(g K) ;
$U02_heat_transfer_coeff = (5.*6.2415097e+10); # MeV/(s cm K) ;
$U02_kappa = $U02_heat_transfer_coeff/($U02_density * $U02_specific_heat); # cm^2/s ;
*
* ZR material information
*
$ZR_density = 6.52;
$ZR_specific_heat = (3.8896.*6.2415097e+12); # MeV/(g K) ;
$ZR_heat_transfer_coeff = (22.6*6.2415097e+10); # MeV/(s cm K) ;
$ZR_kappa = $ZR_heat_transfer_coeff/($ZR_density * $ZR_specific_heat); # cm^2/s ;
*
* H2O material information
*
$H2O_density = .998; # g/(cm^3);
#$H2O_density = .650; # g/(cm^3);
$H2O_specific_heat = (6.8*6.2415097e+12); # MeV/( g K ) ;
*wdh $H2O_viscosity = (10.)*(2.414e-5)*pow(10,247.8/(333-140)); # g/(cm s) ;
$H2O_viscosity = (10.)*(2.414e-5)*(10**(247.8/(333-140))); # g/(cm s) ;
$H2O_kinematic_visc = $H2O_viscosity/$H2O_density; # cm^2/s ;
$H2O_heat_transfer_coeff = (.58*6.2415097e+10); # MeV/(s cm K) ;
$H2O_kappa = $H2O_heat_transfer_coeff/($H2O_density * $H2O_specific_heat); # cm^2/s ;
$H2O_thermal_expansivity = 0;#.4e-3; # /K
* 
*
*
$bcWall=2;         # solid walls
$bcInflow=3;       # bc for inflow boundaries 
$bcOutflow=4;      # bc for outflow boundaries
$bcContainer=5;    # bc at outer wall of the container 
* 
$solver="best"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
*
* ----------------------------- get command line arguments ---------------------------------------
*  -- first get any commonly used options: (requires the env variable CG to be set)
$getCommonOptions = "$ENV{'CG'}/mp/cmd/getCommonOptions.h";
include $getCommonOptions
*  -- now get additional options: 
GetOptions( "kappa=f"=>\$kappa, "bg=s"=>\$backGround,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,\
 "kSolid=f"=>\$kSolid,"kappaSolid=f"=>\$kappaSolid );
* -------------------------------------------------------------------------------------------------
*
$kThermal=$nu/$prandtl;
* 
$grid
* 
* -- interface BCs for the fluid: 
$ifaceWall="bcNumber$bcWall=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber$bcWall=heatFluxInterface";
* -- interface BCs for the solid: 
$sifaceWall="bcNumber$bcWall=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n bcNumber$bcWall=heatFluxInterface";
*
** turn off interfaces: 
** $ifaceWall="*";
** $sifaceWall="*"; 
*
$TFluid=0;
$TRods=0.; 
$TContainer=0.;
$w0=1.; 
* 
* ------- fluid domain ----------
* 
$domainName="fluidDomain"; $solverName="fluid"; $Twall=0.; $ktc=$kFluid; $kThermal=$kappaFluid; 
$bc = "all=noSlipWall uniform(T=$TFluid)\n bcNumber$bcInflow=inflowWithVelocityGiven, parabolic(d=.2,p=1.,w=$w0,T=$TFluid)\n bcNumber$bcOutflow=outflow\n $ifaceWall"; 
* $bc = "all=dirichletBoundaryCondition"; 
$ic = "uniform flow\n u=0., v=0., w=$w0, p=0., T=$TFluid";
$ktc=$kFluid;
* $commands="check for floating point errors 1"; 
include insDomain.h
* 
*   --- rod domain ---
$domainName="rodDomain"; $solverName="rods"; 
$ic = "uniform flow\n T=$TRods";
* $bc = "all=dirichletBoundaryCondition, uniform(T=$TRods)\n $sifaceWall";
$bc = "all=neumannBoundaryCondition, uniform(T=$TRods)\n $sifaceWall";
$kappa=$kappaRods; $ktc=$kRods; 
*  Here is the source term
$a=$heatSourceRods; $b=30.; $p=2.; $x0=0.; $y0=0.; $z0=.25;
$gaussianSource =\
  "user defined forcing\n" . \
  "  gaussian forcing\n" . \
  "    * add 1 source term\n" . \
  "     1 \n" . \
  "     0 0 $a $b $p $x0 $y0 $z0\n" . \
  "  done\n" . \
  "exit"; 
$uniformSource =\
  "user defined forcing\n" . \
  "  constant forcing\n" . \
  "     0 $a \n" . \
  "  done\n" . \
  "exit"; 
* $commands=$gaussianSource;
$commands=$uniformSource;
* $commands .="\ncheck for floating point errors 1"; 
include adDomain.h
$commands=""; 
*
*   --- container domain ---
$domainName="hexagonalContainer"; $solverName="container"; 
$ic = "uniform flow\n T=$TContainer";
** $bc = "all=neumannBoundaryCondition\n bcNumber$bcContainer=dirichletBoundaryCondition, uniform(T=$TContainer)\n $sifaceWall";
$bc = "all=neumannBoundaryCondition\n $sifaceWall";
* $bc = "all=dirichletBoundaryCondition"; 
$kappa=$kappaContainer; $ktc=$kContainer; 
include adDomain.h
*
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
    * specify the max number of parallel hdf sub-files: 
      OBPSF:maximum number of parallel sub-files 8
      open
       $show
    frequency to flush
      * note: currently each domain is counted separately:
      8
    exit
  continue
*
continue
set view:0 -0.0675823 0.00985752 0 1.31263 0.939693 -0.116978 0.321394 0.34202 0.321394 -0.883022 -2.84789e-17 0.939693 0.34202
plot:fluid : w
$go




   plot:fluidA : T
   plot:solidA : T
   plot:solidB : T
   plot:solidC : T
   plot:solidD : T
   plot:fluidB : T


