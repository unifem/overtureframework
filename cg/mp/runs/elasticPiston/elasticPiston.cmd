#
# cgmp:   INS + Elasticity: elastic piston with exact solution
# 
# Usage:
#    cgmp [-noplot] elasticPiston -g=<name> -method=[ins|cns] -nu=<> -mu=<> -kappa=<num> -tf=<tFinal> -tp=<tPlot> ...
#           -solver=[yale|best] -psolver=[yale|best] -ktcFluid=<> -ktcFluid=<> -tz=[poly/trig/none] -bg=<backGroundGrid> ...
#           -degreex=<num> -degreet=<num> -ts=[fe|be|im|pc] -nc=[] -d1=<> -d2=<> -smVariation=[nc|c|g|h] ...
#           -sideBC=[noSlipWall|slipWall|dirichlet]
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
$method="ins"; $probeFile="probeFile"; $multiDomainAlgorithm=1;  $pi=0; $pOffset=0.; 
$tFinal=20.; $tPlot=.1;  $cfl=.9; $show="";  $pdebug=0; $debug=0; $go="halt"; $cdv=""; 
$muFluid=0.; $rhoFluid=1.4; $pFluid=1.; $TFluid=$pFluid/$rhoFluid; 
$nu=.1; $rhoSolid=1.; $prandtl=.72; $cnsVariation="jameson"; $ktcFluid=-1.; $u0=0.; $xShock=-1.5; $uShock=1.25; 
$p0=1.; 
$cnsEOS="ideal"; 
$cnsGammaStiff=1.4; $cnsPStiff=0.;   # for stiffened EOS -- by default make it look like an ideal gas
$lambdaSolid=1.; $muSolid=1.;
## $stressRelaxation=1; $relaxAlpha=0.1; $relaxDelta=0.1; 
$stressRelaxation=4; $relaxAlpha=.5; $relaxDelta=.5; 
$scf=1.; # solidScaleFactor : scale rho,mu and lambda by this amount 
$thermalExpansivity=1.; $T0=1.; $Twall=1.;  $kappa=.01; $ktcSolid=-1.; 
$diss=.2;   # 2nd-order linear dissipation for cgsm --> increase from .1 to .2 : July 2, 2017
$smVariation = "g"; 
$tsSM="modifiedEquationTimeStepping";
$tz="none"; $degreeSpace=1; $degreeTime=1;
$gravity = "0 0. 0."; $boundaryPressureOffset=0.; $cnsGodunovOrder=2; 
$fic = "uniform";  # fluid initial condition
$backGround="outerSquare"; $deformingGrid="interface"; 
#
$ts="pc";   # MP solver
$tsINS="pc"; # INS time-stepping method 
$numberOfCorrections=1;  # cgmp and cgins 
$coupled=0; $iTol=1.e-3; $iOmega=1.; $flushFrequency=10; $useNewInterfaceTransfer=0; 
$useTP=0; # 1=use traditional partitioned scheme
$projectMultiDomainInitialConditions=0; 
$useNewTimeSteppingStartup=1;  # *NEW* July 1, 2017
$freqFullUpdate=1; # frequency for using full ogen update in moving grids 
#
$smoothInterface=0;  # smooth the interface (in DeformingBodyMotion.C )
$numberOfInterfaceSmooths=4; 
#
# $option="beamUnderPressure"; # this currently means ramp the inflow
$option="bulkSolidPiston"; # define pressure BC from known solution
#
$sideBC="slipWall"; 
#
$bcOption=4;   # does this do anything ? I thibnk this is for cgcns
$orderOfExtrapForOutflow=3; $orderOfExtrapForGhost2=2; $orderOfExtrapForInterpNeighbours=2; 
$projectInitialConditions=0; # for INS
# 
$psolver="yale"; 
$solver="yale"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
# -- p-wave strength: don't make too big or else solid may become inverted in the deformed space
$append=0; 
# ------------------------- turn on added mass here ----------------
$addedMass=0; 
# ---- piston parameters:  choose t0=1/(4*k) to make yI(0)=0 
$Pi=4.*atan2(1.,1.);
$amp=.1; $k=.5; $t0=1./(4*$k);  $H=1.; $Hbar=.5; $rho=1.; 
$rampOrder=2;  # number of zero derivatives at start and end of the ramp
$ra=-10.; $rb=-9.; # ramp interval -- actual interval shifted by Hbar/cp 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"nu=f"=>\$nu,"muFluid=f"=>\$muFluid,"kappa=f"=>\$kappa, "bg=s"=>\$backGround,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "psolver=s"=>\$psolver,"useTP=i"=> \$useTP,\
 "tz=s"=>\$tz,"degreeSpace=i"=>\$degreeSpace, "degreeTime=i"=>\$degreeTime,\
 "show=s"=>\$show,"method=s"=>\$method,"ts=s"=>\$ts,"tsSM=s"=>\$tsSM,"noplot=s"=>\$noplot,"ktcFluid=f"=>\$ktcFluid,\
  "ktcSolid=f"=>\$ktcSolid,"muSolid=f"=>\$muSolid,"lambdaSolid=f"=>\$lambdaSolid, "T0=f"=>\$T0,"Twall=f"=>\$Twall,\
  "nc=i"=> \$numberOfCorrections, "numberOfCorrections=i"=> \$numberOfCorrections,"coupled=i"=>\$coupled,\
  "d1=s"=>\$domain1,"d2=s"=>\$domain2,"dg=s"=>\$deformingGrid,"debug=i"=>\$debug,"kThermalFluid=f"=>\$kThermalFluid,\
  "cfl=f"=>\$cfl,"rhoSolid=f"=>\$rhoSolid,"cnsVariation=s"=>\$cnsVariation,"diss=f"=>\$diss,"fic=s"=>\$fic,"go=s"=>\$go,\
   "smVariation=s"=>\$smVariation,"scf=f"=>\$scf,"probeFile=s"=>\$probeFile,"pOffset=f"=>\$boundaryPressureOffset,\
   "cnsGodunovOrder=f"=>\$cnsGodunovOrder,"flushFrequency=i"=>\$flushFrequency,\
   "cnsEOS=s"=>\$cnsEOS,"cnsGammaStiff=f"=>\$cnsGammaStiff,"cnsPStiff=f"=>\$cnsPStiff,"u0=f"=>\$u0,\
   "useNewInterfaceTransfer=i"=>\$useNewInterfaceTransfer,"multiDomainAlgorithm=i"=>\$multiDomainAlgorithm,\
   "pi=i"=>\$pi,"xShock=f"=>\$xShock,"uShock=f"=>\$uShock,"bcOption=i"=>\$bcOption,"option=s"=>\$option,\
   "stressRelaxation=f"=>\$stressRelaxation,"relaxAlpha=f"=>\$relaxAlpha,"relaxDelta=f"=>\$relaxDelta,\
   "p0=f"=>\$p0,"sideBC=s"=>\$sideBC,"iOmega=f"=>\$iOmega,"iTol=f"=>\$iTol,"addedMass=f"=>\$addedMass,\
   "projectInitialConditions=f"=>\$projectInitialConditions,"restart=s"=>\$restart,"append=i"=>\$append,\
   "projectMultiDomainInitialConditions=f"=>\$projectMultiDomainInitialConditions,\
   "amp=f"=>\$amp,"rampOrder=i"=>\$rampOrder,"ra=f"=>\$ra,"rb=f"=>\$rb,"cdv=f"=>\$cdv,\
   "useNewTimeSteppingStartup=i"=> \$useNewTimeSteppingStartup,"tsINS=s"=>\$tsINS,\
   "freqFullUpdate=i"=>\$freqFullUpdate,"smoothInterface=i"=>\$smoothInterface,\
   "numberOfInterfaceSmooths=i"=>\$numberOfInterfaceSmooths );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $ts eq "fe" ){ $ts="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
#
if( $tsINS eq "fe" ){ $tsINS="forward Euler";}
if( $tsINS eq "be" ){ $tsINS="backward Euler"; }
if( $tsINS eq "im" ){ $tsINS="implicit"; }
if( $tsINS eq "bdf" ){ $tsINS="implicit BDF"; }
if( $tsINS eq "imex" ){ $tsINS="implicit explicit multistep"; }
if( $tsINS eq "pc" ){ $tsINS="adams PC"; }
if( $tsINS eq "pc4" ){ $tsINS="adams PC order 4"; $useNewImp=0; } # NOTE: turn off new implicit for fourth order
if( $tsINS eq "mid"){ $tsINS="midpoint"; }  
if( $tsINS eq "afs"){ $tsINS="approximate factorization"; $newts=1;  $implicitVariation="full"; }
#
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
# **** NEW WAY TO SPECIFY DEFORMING BODY FOR A BULK SOLID 
# $vInitial=-.54414;
$numberOfPastTimeLevels=3; 
$gridEvolutionVelocityAccuracy=3; 
$gridEvolutionAccelerationAccuracy=2; 
if( $tz eq "turn off twilight zone" ){ $useKnown=1; }else{ $useKnown=0; }
$moveCmds = \
  "turn on moving grids\n" . \
  "specify grids to move\n" . \
  "    deforming body\n" . \
  "      bulk solid\n" . \
  "        debug\n $debug \n" . \
  "      velocity order of accuracy\n $gridEvolutionVelocityAccuracy\n" . \
  "      acceleration order of accuracy\n $gridEvolutionAccelerationAccuracy\n" . \
  "      generate past history 1\n" . \
  "      use known solution for initial conditions $useKnown\n" . \
  "      number of past time levels: $numberOfPastTimeLevels\n" . \
  "      smooth surface $smoothInterface \n" . \
  "      number of surface smooths: $numberOfInterfaceSmooths \n" . \
  "     done\n" . \
  "     choose grids by share flag\n" . \
  "        100 \n" . \
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
# -- RAMP PRESSURE BC: 
if( $sideBC eq "dirichlet" ){ $sideBC = "dirichletBoundaryCondition"; }
$bc = "all=$sideBC\n bcNumber100=noSlipWall uniform(u=.0,T=$T0)\n bcNumber100=tractionInterface";
    #
    # **** ramp the pressure on the top ****
    $cmdRamp="bcNumber4=outflow, pressure(1.*p+0.*p.n=$p0), userDefinedBoundaryData\n" . \
    " pause\n" . \
    " time function option\n" . \
    "   ramp function\n" .\
    "   ramp end values: 0,1 (start,end)\n" .\
    "   ramp times: 0,1 (start,end)\n" .\
    "   ramp order: 3\n" .\
    " exit \n" .\
    "done";
    # pressure at top from the known bulk solid piston solution
    $cmdKnown="bcNumber4=outflow, pressure(1.*p+0.*p.n=$p0), userDefinedBoundaryData\n" . \
    " known solution\n" . \
    " exit \n" .\
    "done";
# if( $option ne "beamUnderPressure" ){ $cmdRamp = "bcNumber3=outflow, pressure(1.*p+0.*p.n=$p0)"; }
# $bc = $bc . "\n" . $cmdRamp;
if( $tz eq "turn off twilight zone" ){ $bc = $bc . "\n" . $cmdKnown; }
#
$ic="uniform flow\n" . "p=0., u=$u0, T=$T0";
$rhoBar=$rhoSolid*$scf; $lambdaBar=$lambdaSolid*$scf; $muBar=$muSolid*$scf;
$ic="OBTZ:user defined known solution\n" .\
    "choose a common known solution\n" .\
    " bulk solid piston\n" .\
    "  $amp,$k,$t0,$H,$Hbar,$rho,$rhoBar,$lambdaBar,$muBar\n" .\
    "  $rampOrder $ra $rb\n" .\
    " done\n" .\
    "done"; 
if( $tz ne "turn off twilight zone" ){ $ic="#"; }
#
echo to terminal 0
include $ENV{CG}/mp/cmd/insDomain.h
$extraCmds="#"; 
echo to terminal 1
# 
# ------- specify elastic solid domain ----------
$domainName=$domain2; $solverName="solid"; 
# $bcCommands="all=displacementBC\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
# $bcCommands="all=displacementBC\n bcNumber2=slipWall\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
$bcCommands="all=tractionBC\n bcNumber1=displacementBC\n bcNumber2=displacementBC\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
# -- slipWall on sides and displacement on bottom:
if( $sideBC eq "dirichlet" ){ $sideBC = "dirichletBoundaryCondition"; }
$bcCommands="all=displacementBC\n bcNumber1=$sideBC\n bcNumber2=$sideBC\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
#  -- for noSlipWall's we use displacement on sides of solid
if( $sideBC eq "noSlipWall" ){ $bcCommands="all=displacementBC\n  bcNumber100=tractionBC\n bcNumber100=tractionInterface"; }
$exponent=10.; $x0=.5; $y0=.5; $z0=.5;  $rhoSolid=$rhoSolid*$scf; $lambda=$lambdaSolid*$scf; $mu=$muSolid*$scf; 
# $initialConditionCommands="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0)";
$initialConditionCommands="zeroInitialCondition";
$initialConditionCommands=\
    "OBTZ:user defined known solution\n" .\
    "choose a common known solution\n" .\
    " bulk solid piston\n" .\
    "  $amp,$k,$t0,$H,$Hbar,$rho,$rhoBar,$lambdaBar,$muBar\n" .\
    "  $rampOrder $ra $rb\n" .\
    " done\n" .\
  "done \n" .\
  "knownSolutionInitialCondition";
if( $tz ne "turn off twilight zone" ){ $initialConditionCommands="#"; }
#
$smCheckErrors=1;
# 
echo to terminal 0
include $ENV{CG}/mp/cmd/smDomain.h
echo to terminal 1
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
  #
  $tz
  # DEFINE THE MULTI_STAGE ALGORITHM --
  OBPDE:multi-stage
  actions=takeStep classNames=Cgsm
  actions=takeStep,applyBC classNames=Cgins
  actions=applyBC classNames=Cgsm
  #actions=takeStep classNames=Cgsm,Cgins
  #actions=takeStep,applyBC classNames=Cgins
  #actions=takeStep,applyBC domainNames=fluidDomain
  #actions=takeStep,applyBC domainNames=fluidDomain,solidDomain
  done
#
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
          vertical scale factor 0.
           # ghost lines 1
           plot:p
           ##  wire frame
          exit
        plot domain: solid
        contour
          vertical scale factor 0.
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


