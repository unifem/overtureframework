#
# cgmp: incompressible flow through a deforming channel
# 
# Usage:
#    cgmp [-noplot] elasticCyl -g=<name> -method=[ins|cns] -nu=<> -mu=<> -kappa=<num> -tf=<tFinal> -tp=<tPlot> ...
#           -solver=[yale|best] -psolver=[yale|best] -ktcFluid=<> -ktcFluid=<> -tz=[poly/trig/none] -bg=<backGroundGrid> ...
#           -degreex=<num> -degreet=<num> -ts=[fe|be|im|pc] -nc=[] -d1=<> -d2=<> -smVariation=[nc|c|g|h] ...
#           -sideBC=[noSlipWall|slipWall]
# 
#  -ktcFluid -ktcSolid : thermal conductivities 
#  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep
#  -d1, -d2 : names for domains
# 
# Examples:
# 
# --- set default values for parameters ---
# 
$grid="deformingChannelGrid4.order2"; $domain1="fluidDomain"; $domain2="solidDomain";
$method="ins"; $probeFile="probeFile"; $multiDomainAlgorithm=0;  $pi=0; $pOffset=0.; 
$tFinal=20.; $tPlot=.1;  $cfl=.9; $show="";  $pdebug=0; $debug=0; $go="halt"; 
$muFluid=0.; $rhoFluid=1.4; $pFluid=1.; $TFluid=$pFluid/$rhoFluid; 
$nu=.1; $rhoSolid=1.; $prandtl=.72; $cnsVariation="jameson"; $ktcFluid=-1.; $u0=0.; $xShock=-1.5; $uShock=1.25; 
$p0=1.; 
$cnsEOS="ideal"; 
$cnsGammaStiff=1.4; $cnsPStiff=0.;   # for stiffened EOS -- by default make it look like an ideal gas
$lambdaSolid=1.; $muSolid=1.;
## $stressRelaxation=1; $relaxAlpha=0.1; $relaxDelta=0.1; 
$stressRelaxation=4; $relaxAlpha=.5; $relaxDelta=.5; 
$scf=1.; # solidScaleFactor : scale rho,mu and lambda by this amount 
$thermalExpansivity=1.; $T0=1.; $Twall=1.;  $kappa=.01; $ktcSolid=-1.; $diss=.1;  
$smVariation = "g"; 
$tsSM="modifiedEquationTimeStepping";
$tz="none"; $degreeSpace=1; $degreeTime=1;
$gravity = "0 0. 0."; $boundaryPressureOffset=0.; $cnsGodunovOrder=2; 
$fic = "uniform";  # fluid initial condition
$backGround="outerSquare"; $deformingGrid="interface"; 
#
$ts="pc";   # MP solver
$numberOfCorrections=1;  # cgmp and cgins 
$coupled=0; $iTol=1.e-3; $iOmega=1.; $flushFrequency=10; $useNewInterfaceTransfer=0; 
$useTP=0; # 1=use traditional partitioned scheme
$projectMultiDomainInitialConditions=1;
#
$option="beamUnderPressure"; # this currently means ramp the inflow
#
$sideBC="noSlipWall"; # = "slipWall"; 
#
$bcOption=0; 
$orderOfExtrapForOutflow=2; $orderOfExtrapForGhost2=2; $orderOfExtrapForInterpNeighbours=2; 
$projectInitialConditions=1; # for INS
# 
$psolver="best"; 
$solver="best"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
# -- p-wave strength: don't make too big or else solid may become inverted in the deformed space
$ap=.01; 
$append=0; 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"nu=f"=>\$nu,"muFluid=f"=>\$muFluid,"kappa=f"=>\$kappa, "bg=s"=>\$backGround,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "psolver=s"=>\$psolver,"useTP=i"=> \$useTP,\
 "tz=s"=>\$tz,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,\
 "show=s"=>\$show,"method=s"=>\$method,"ts=s"=>\$ts,"tsSM=s"=>\$tsSM,"noplot=s"=>\$noplot,"ktcFluid=f"=>\$ktcFluid,\
  "ktcSolid=f"=>\$ktcSolid,"muSolid=f"=>\$muSolid,"lambdaSolid=f"=>\$lambdaSolid, "T0=f"=>\$T0,"Twall=f"=>\$Twall,\
  "nc=i"=> \$numberOfCorrections, "numberOfCorrections=i"=> \$numberOfCorrections,"coupled=i"=>\$coupled,\
  "d1=s"=>\$domain1,"d2=s"=>\$domain2,"dg=s"=>\$deformingGrid,"debug=i"=>\$debug,"kThermalFluid=f"=>\$kThermalFluid,\
  "cfl=f"=>\$cfl,"rhoSolid=f"=>\$rhoSolid,"cnsVariation=s"=>\$cnsVariation,"diss=f"=>\$diss,"fic=s"=>\$fic,"go=s"=>\$go,\
   "smVariation=s"=>\$smVariation,"scf=f"=>\$scf,"probeFile=s"=>\$probeFile,"pOffset=f"=>\$boundaryPressureOffset,\
   "cnsGodunovOrder=f"=>\$cnsGodunovOrder,"flushFrequency=i"=>\$flushFrequency,\
   "cnsEOS=s"=>\$cnsEOS,"cnsGammaStiff=f"=>\$cnsGammaStiff,"cnsPStiff=f"=>\$cnsPStiff,"u0=f"=>\$u0,\
   "useNewInterfaceTransfer=i"=>\$useNewInterfaceTransfer,"multiDomainAlgorithm=i"=>\$multiDomainAlgorithm,\
   "pi=i"=>\$pi,"xShock=f"=>\$xShock,"uShock=f"=>\$uShock,"ap=f"=>\$ap,"bcOption=i"=>\$bcOption,"option=s"=>\$option,\
   "stressRelaxation=f"=>\$stressRelaxation,"relaxAlpha=f"=>\$relaxAlpha,"relaxDelta=f"=>\$relaxDelta,\
   "p0=f"=>\$p0,"sideBC=s"=>\$sideBC,"iOmega=f"=>\$iOmega,"iTol=f"=>\$iTol,\
   "projectInitialConditions=f"=>\$projectInitialConditions,"restart=s"=>\$restart,"append=i"=>\$append,\
   "projectMultiDomainInitialConditions=f"=>\$projectMultiDomainInitialConditions );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
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
if( $projectInitialConditions eq "1" ){ $projectInitialConditions = "project initial conditions"; }else{ $projectInitialConditions = "do not project initial conditions"; }
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
#  Cgins:
$domainName=$domain1; $solverName="fluid"; 
$modelNameINS="none"; 
#
$T0=0.; 
## $bc = "all=noSlipWall uniform(u=.0,T=$T0)\n bcNumber3=slipWall\n bcNumber4=slipWall\n bcNumber1=inflowWithVelocityGiven, uniform(u=$u0,T=0.)\n bcNumber2=outflow, pressure(1.*p+.1*p.n=0.)\n bcNumber100=tractionInterface";
## $bc = "all=noSlipWall uniform(u=.0,T=$T0)\n bcNumber3=slipWall\n bcNumber4=slipWall\n bcNumber1=inflowWithVelocityGiven, parabolic(d=.1,u=$u0,T=0.)\n bcNumber2=outflow, pressure(1.*p+.1*p.n=0.)\n bcNumber100=tractionInterface";
### $bc = "all=noSlipWall uniform(u=.0,T=$T0)\n bcNumber3=slipWall\n bcNumber4=slipWall\n bcNumber1=inflowWithPressureAndTangentialVelocityGiven uniform(p=1.,v=0.T=0.)\n bcNumber2=outflow, pressure(1.*p+0.*p.n=0.)\n bcNumber100=tractionInterface";
# -- beam under pressure: 
$bc = "all=$sideBC\n bcNumber100=noSlipWall uniform(u=.0,T=$T0)\n bcNumber100=tractionInterface";
    #
    # **** ramp the pressure on the bottom ****
    $cmdRamp="bcNumber3=outflow, pressure(1.*p+0.*p.n=$p0), userDefinedBoundaryData\n" . \
    " time function option\n" . \
    "   ramp function\n" .\
    "   ramp end values: 0,1 (start,end)\n" .\
    "   ramp times: 0,1 (start,end)\n" .\
    "   ramp order: 3\n" .\
    " exit \n" .\
    "done";
if( $option ne "beamUnderPressure" ){ $cmdRamp = "bcNumber3=outflow, pressure(1.*p+0.*p.n=$p0)"; }
$bc = $bc . "\n" . $cmdRamp;
#
$ic="uniform flow\n" . "p=1., u=$u0, T=$T0";
#
include $ENV{CG}/mp/cmd/insDomain.h
$extraCmds="#"; 
# 
# ------- specify elastic solid domain ----------
$domainName=$domain2; $solverName="solid"; 
# $bcCommands="all=displacementBC\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
# $bcCommands="all=displacementBC\n bcNumber2=slipWall\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
$bcCommands="all=tractionBC\n bcNumber1=displacementBC\n bcNumber2=displacementBC\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
# -- slipWall on sides and displacement on top:
$bcCommands="all=displacementBC\n bcNumber1=slipWall\n bcNumber2=slipWall\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
#  -- for noSlipWall's we use displacement on sides of solid
if( $sideBC eq "noSlipWall" ){ $bcCommands="all=displacementBC\n  bcNumber100=tractionBC\n bcNumber100=tractionInterface"; }
$exponent=10.; $x0=.5; $y0=.5; $z0=.5;  $rhoSolid=$rhoSolid*$scf; $lambda=$lambdaSolid*$scf; $mu=$muSolid*$scf; 
# $initialConditionCommands="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0)";
$initialConditionCommands="zeroInitialCondition";
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
  # project multi-domain initial condtions
  OBPDE:project initial conditions $projectMultiDomainInitialConditions
  OBPDE:use new interface transfer $useNewInterfaceTransfer
  # relax correction steps for TP scheme: 
  OBPDE:relax correction steps $useTP
 # -- for testing solve the domains in reverse order: 
 # OBPDE:domain order 1 0
  OBPDE:project interface $pi
  if( $multiDomainAlgorithm eq 1 ){ $cmd="OBPDE:step all then match advance"; }else{ $cmd="#"; }
  $cmd 
  $tz
  debug $debug
  show file options
    if( $append eq 0 ){ $cmd="OBPSF:create new show file"; }else{ $cmd="OBPSF:append to old show file"; }
    $cmd
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
         ##  wire frame
          exit
        plot domain: solid
        contour
          adjust grid for displacement 1
        exit
        plot:solid : v2
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


