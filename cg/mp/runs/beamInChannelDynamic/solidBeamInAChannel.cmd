#
# cgmp: INS or CNS flow pas a solid beam in a channel
# 
# Usage:
#    cgmp [-noplot] solidBeamInAChannel -g=<name> -method=[ins|cns] -nu=<> -mu=<> -kappa=<num> -tf=<tFinal> -tp=<tPlot> ...
#           -solver=<yale/best> -ktcFluid=<> -ktcFluid=<> -tz=[poly/trig/none] -bg=<backGroundGrid> ...
#           -degreex=<num> -degreet=<num> -ts=[fe|be|im|pc] -nc=[] -d1=<> -d2=<> -smVariation=[nc|c|g|h]
# 
#  -ktcFluid -ktcSolid : thermal conductivities 
#  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep
#  -d1, -d2 : names for domains
#  -godunovType : 0=linear, 1=LE(SVK-code) 2=SVK 4=neoHookean
#  -piGhostOption : =extrap ghost, 1=compatibility for ghost at interfaces, 2=use exact, 3=use-domain solvers
#  -piOption : 0=linear FSR, 1=nonlinear-FSR for the interface projection# 
#
# Examples:
# 
# 
# --- set default values for parameters ---
# 
$grid="twoSquaresInterfacee1.order2.hdf"; $domain1="rightDomain"; $domain2="leftDomain";
$method="ins"; $probeFile=""; $multiDomainAlgorithm=0;   $godunovType=0; 
$modelNameINS="#"; 
$projectInitialConditions="project initial conditions";
$projectInitialConditions="#";
$pi=0; $piOption=0; $piGhostOption=3; $bcOption=0; 
$tFinal=20.; $tPlot=.1;  $cfl=.9; $show="";  $pdebug=0; $debug=0; $go="halt"; 
$muFluid=0.; $rhoFluid=1.4; $pFluid=1.; $TFluid=$pFluid/$rhoFluid; $adCns=0.; 
$nu=.1; $rhoSolid=1.; $prandtl=.72; $cnsVariation="jameson"; $ktcFluid=-1.; $u0=1.; $xShock=-.5; $uShock=1.25; 
$cnsEOS="ideal"; 
$cnsGammaStiff=1.4; $cnsPStiff=0.;   # for stiffened EOS -- by default make it look like an ideal gas
$lambdaSolid=1.; $muSolid=1.;
$scf=1.; # solidScaleFactor : scale rho,mu and lambda by this amount 
$tangentialStressDissipation=.5; $tangentialStressDissipation1=.5; # new 
# $displacementDissipation=.5; $displacementDissipation1=.5; 
$displacementDissipation=.0; $displacementDissipation1=.0; 
$tangentialDissipationSolid=-1.; # if >0 use this value for above 4 values
#
$thermalExpansivity=1.; $T0=1.; $Twall=1.;  $kappa=.01; $ktcSolid=-1.; $diss=.1;  $smVariation = "g";
$tz="none"; $degreeSpace=1; $degreeTime=1;
$gravity = "0 0. 0."; $boundaryPressureOffset=0.; $cnsGodunovOrder=2; 
$fic = "uniform";  # fluid initial condition
$solver="best"; 
$backGround="backGroundFluid"; $deformingGrid="interface"; 
$ts="pc"; $numberOfCorrections=1;  # mp solver
$coupled=0; $iTol=1.e-3; $iOmega=1.; $flushFrequency=400; $useNewInterfaceTransfer=0; 
#
$stressRelaxation=4; $relaxAlpha=.5; $relaxDelta=.5; 
$godunovOrder=2; # order of godunov solver for solid 
$slopeLimiter=1;  # slope limiter for Godunov method
#
$rampedInflow=1; # set to 1 for ramped inflow, 0=shock
# 
$solver="best"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
#
#
# ---- Shock Jump Conditions: ----
#  (rho1,u1,T1) = state AHEAD of the shock
$shockSpeed=1.5;
$Mshock=-1.; # specify shock Mach number if >0 , otherwise use $shockSpeed
$gamma=1.4; $Rg=1.;
$a1=1.; $rho1=1.; $u1=0.;  
# For backward compatibility:
## if( $shockSpeed eq 0 ){ $rho1=2.6667; $u1=1.25; $e1=10.119; $rho2=1.; $u2=0.; $e2=1.786; }
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"nu=f"=>\$nu,"muFluid=f"=>\$muFluid,"kappa=f"=>\$kappa, "bg=s"=>\$backGround,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,\
 "show=s"=>\$show,"method=s"=>\$method,"ts=s"=>\$ts,"noplot=s"=>\$noplot,"ktcFluid=f"=>\$ktcFluid,\
  "ktcSolid=f"=>\$ktcSolid,"muSolid=f"=>\$muSolid,"lambdaSolid=f"=>\$lambdaSolid, "T0=f"=>\$T0,"Twall=f"=>\$Twall,\
  "nc=i"=> \$numberOfCorrections,"coupled=i"=>\$coupled,\
  "d1=s"=>\$domain1,"d2=s"=>\$domain2,"dg=s"=>\$deformingGrid,"debug=i"=>\$debug,"kThermalFluid=f"=>\$kThermalFluid,\
  "cfl=f"=>\$cfl,"rhoSolid=f"=>\$rhoSolid,"cnsVariation=s"=>\$cnsVariation,"diss=f"=>\$diss,"fic=s"=>\$fic,"go=s"=>\$go,\
   "smVariation=s"=>\$smVariation,"scf=f"=>\$scf,"probeFile=s"=>\$probeFile,"pOffset=f"=>\$boundaryPressureOffset,\
   "cnsGodunovOrder=f"=>\$cnsGodunovOrder,"flushFrequency=i"=>\$flushFrequency,\
   "cnsEOS=s"=>\$cnsEOS,"cnsGammaStiff=f"=>\$cnsGammaStiff,"cnsPStiff=f"=>\$cnsPStiff,"adCns"=>\$adCns,\
   "useNewInterfaceTransfer=i"=>\$useNewInterfaceTransfer,"multiDomainAlgorithm=i"=>\$multiDomainAlgorithm,\
   "pi=i"=>\$pi,"xShock=f"=>\$xShock,"Mshock=f"=>\$Mshock,"uShock=f"=>\$uShock,"godunovType=i"=>\$godunovType,\
   "shockSpeed=f"=>\$shockSpeed,"piOption=i"=>\$piOption,"piGhostOption=i"=>\$piGhostOption,"bcOption=i"=>\$bcOption,\
   "a1=f"=>\$a1, "tangentialDissipationSolid=f"=>\$tangentialDissipationSolid,"godunovOrder=i"=>\$godunovOrder,\
   "rampedInflow=i"=>\$rampedInflow,"relaxAlpha=f"=>\$relaxAlpha,"relaxDelta=f"=>\$relaxDelta,\
   "slopeLimiter=i"=>\$slopeLimiter );
# -------------------------------------------------------------------------------------------------
#  ---- Shock Jump Conditions: ----
#  (rho2,u2,T2) = state BEHIND the shock
if( $Mshock>0 ){ $shockSpeed=$Mshock*$a1; }else{ $Mshock=$shockSpeed/$a1; }
$T1=$a1*$a1/($gamma*$Rg); $p1=$rho1*$Rg*$T1;
$p2=$p1*( 1. +(2.*$gamma)/($gamma+1.)*( $Mshock*$Mshock -1. ));
$rho2=$rho1/( 1. - 2./($gamma+1.)*(1. - 1./($Mshock*$Mshock) ) );
$T2=$p2/($Rg*$rho2); 
$u2=( $shockSpeed*($rho2-$rho1) + $rho1*$u1 )/$rho2;
$pOffset=$p1; # NOTE
#
if( $tangentialDissipationSolid>0 ){ $tangentialStressDissipation=$tangentialDissipationSolid; $tangentialStressDissipation1=$tangentialDissipationSolid; $displacementDissipation=$tangentialDissipationSolid; $displacementDissipation1=$tangentialDissipationSolid; }
#
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $ts eq "fe" ){ $ts="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# 
if( $smVariation eq "nc" ){ $smVariation = "non-conservative"; }
if( $smVariation eq "c" ){ $smVariation = "conservative"; $cons=1; }
if( $smVariation eq "g" ){ $smVariation = "godunov"; }
if( $smVariation eq "h" ){ $smVariation = "hemp"; }
#
if( $method eq "ins" && $kThermalFluid eq "" ){ $kThermalFluid=$nu/$prandtl; }
if( $method eq "cns" && $kThermalFluid eq "" ){ $kThermalFluid=$muFluid/$prandtl; }
if( $ktcFluid < 0. ){ $ktcFluid=$kThermalFluid;} if( $ktcSolid < 0. ){ $ktcSolid=$kappa; }
# 
$grid
# ----------  define deforming bodies by a share flag of 100 ----
# ----------  NOTE: we parameterize the boundary by index so grid points match! ---
$moveCmds = \
  "turn on moving grids\n" . \
  "specify grids to move\n" . \
  "    deforming body\n" . \
  "      user defined deforming body\n" . \
  "        interface deform\n" . \
  "        boundary parameterization\n  1  \n" . \
  "        debug\n $debug \n" . \
  "      done\n" . \
  "      choose grids by share flag\n" . \
  "         100 \n" . \
  "   done\n" . \
  "done";
# 
#$probeFileName = $probeFile . "Fluid.dat";
#$extraCmds = \
#    "frequency to save probes 1\n" . \
#    "create a probe\n" . \
#    "  file name $probeFileName\n" . \
#    "  nearest grid point to 0. .5 0.\n" . \
#    "  exit";
# ------- specify fluid domain ----------
$domainName=$domain1; $solverName="fluid"; 
#
#  ******  Cgins ********
#
$ic = "uniform flow\n p=0., u=$u0";
$ic = "uniform flow\n p=0., u=0.";
$halfH=0.2; $cpn=1.;
$bc = "all=noSlipWall\n  bcNumber1=inflowWithVelocityGiven, parabolic(d=$halfH, p=1.,u=$u0,T=1.), ramp(ta=0.,tb=1.,ua=0,ub=$u0)\n bcNumber2=outflow, pressure(10.*p+$cpn*p.n=0.)\n bcNumber100=noSlipWall\n bcNumber100=tractionInterface";
## $bc = "all=noSlipWall\n  bcNumber1=inflowWithVelocityGiven, ramp(ta=1.,tb=2.,ua=0,ub=$u0)\n bcNumber2=outflow, pressure(10.*p+$cpn*p.n=0.)\n bcNumber100=noSlipWall\n bcNumber100=tractionInterface";
$ktc=$ktcFluid; $rtolp=1.e-4; $atolp=1.e-6; 
if( $method eq "ins" ){ $cmd = "include $ENV{CG}/mp/cmd/insDomain.h"; }else{ $cmd ="*"; };
$cmd
#
#  ******  Cgcns ********
#
# ------ SHOCK: 
$bcShock = "all=slipWall\n $backGround(1,0)=superSonicOutflow\n $backGround(0,0)=superSonicInflow uniform(r=$rho2,u=$u2,T=$T2)\n bcNumber100=slipWall\n bcNumber100=tractionInterface";
# -- ramped inflow ramp(ta=0.,tb=.5,ra=1.,rb=2.66667,ua=0.,ub=1.25,Ta=$Ta,Tb=$Tb)
# 
#---- RAMPED INFLOW: 
$bcRamped = "all=slipWall\n $backGround(1,0)=superSonicOutflow\n $backGround(0,0)=superSonicInflow uniform(r=$rho2,u=$u2,T=$T2) ramp(ta=0.,tb=1.,ra=$rho1,rb=$rho2,ua=$u1,ub=$u2,Ta=$T1,Tb=$T2)\n bcNumber100=slipWall\n bcNumber100=tractionInterface";
if( $rampedInflow eq 1 ){ $bc=$bcRamped; }else{ $bc=$bcShock; }
#
#
$ic="OBIC:step: a*x+b*y+c*z=d 1, 0, 0, $xShock, (a,b,c,d)\n OBIC:state behind r=$rho2 u=$u2 T=$T2\n OBIC:state ahead r=$rho1 u=$u1 T=$T1\n OBIC:assign step function\n"; 
#
if( $method eq "cns" ){ $mu=$muFluid; $kThermal=$muFluid/$prandtl; $cmd = "include $ENV{CG}/mp/cmd/cnsDomain.h"; }else{ $cmd ="*"; };
$cmd
# 
# ------- specify elastic solid domain ----------
$domainName=$domain2; $solverName="solid"; 
# $bcCommands="all=displacementBC\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
# $bcCommands="all=displacementBC\n bcNumber3=slipWall\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
$bcCommands="all=displacementBC\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
$exponent=10.; $x0=.5; $y0=.5; $z0=.5;  $rhoSolid=$rhoSolid*$scf; $lambda=$lambdaSolid*$scf; $mu=$muSolid*$scf; 
# $initialConditionCommands="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0)";
$initialConditionCommands="zeroInitialCondition";
if( $smVariation eq "hemp" ){ $initialConditionCommands="hempInitialCondition\n OBIC:Hemp initial condition option: default\n"; }
if( $smVariation eq "hemp" ){ $tsSM= "improvedEuler"; }
#
$probeFileName = $probeFile . "Solid.dat";
$probeCmds = \
    "frequency to save probes 10\n" . \
    "create a probe\n" . \
    "  file name $probeFileName\n" . \
    "  nearest grid point to 0. 1. 0.\n" . \
    "  exit";
# 
if( $probeFile ne "" ){ $extraCmds = $probeCmds; }else{ $extraCmds ="#"; }
#
include $ENV{CG}/mp/cmd/smDomain.h
# 
continue
#
# -- set parameters for cgmp ---
# 
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  dtMax .1
  $ts
  number of PC corrections $numberOfCorrections
  OBPDE:interface tolerance $iTol
  OBPDE:interface omega $iOmega
  OBPDE:solve coupled interface equations $coupled
  OBPDE:use new interface transfer $useNewInterfaceTransfer
 # -- for testing solve the domains in reverse order: 
  OBPDE:domain order 1 0
  OBPDE:project interface $pi
  OBPDE:use nonlinear interface projection $piOption
  $cmd="#"; 
  if( $piGhostOption == 0 ){ $cmd="OBPDE:interface ghost from extrapolation"; }
  if( $piGhostOption == 1 ){ $cmd="OBPDE:interface ghost from compatibility"; }else{ $cmd="OBPDE:interface ghost from exact"; }
  if( $piGhostOption == 2 ){ $cmd="OBPDE:interface ghost from exact"; }
  if( $piGhostOption == 3 ){ $cmd="OBPDE:interface ghost from domain solvers"; }
  $cmd
#
  if( $multiDomainAlgorithm eq 1 ){ $cmd="OBPDE:step all then match advance"; }else{ $cmd="#"; }
  $cmd 
  $tz
  debug $debug
  show file options
    compressed
      open
       $show
    frequency to flush
      $flushFrequency
    exit
  continue
#
continue
# --
        erase
        plot domain: fluid
        contour
 # ghost lines 1
          plot:p
          wire frame
          exit
        plot domain: solid
          contour
            adjust grid for displacement 1
            exit
        plot all
$go



        erase
        plot domain: fluid
        contour
 # ghost lines 1
          plot:p
          wire frame
          exit
        plot domain: solid
        displacement
          displacement scale factor 1
 # displacement scale factor 10
          exit this menu
        plot all
$go


   erase
   plot domain: fluid
   grid
     exit this menu
   plot domain: solid
   displacement
     exit this menu
   plot all




          OBIC:user defined...
            bubble with shock
            r=1 T=1 u=0 v=0
            .2 .5 0.
            r=2. T=2. 
            -5.
            r=2 T=2
            exit




