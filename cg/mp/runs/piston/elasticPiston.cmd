#
# cgmp: Elastic piston - compare to the exact solution
# 
# Usage:
#    cgmp [-noplot] elasticPiston -g=<name> -method=[ins|cns] -nu=<> -mu=<> -rhog=<num> -tf=<tFinal> -tp=<tPlot> ...
#           -solver=<yale/best> -ktcFluid=<> -ktcFluid=<> -tz=[poly/trig/none] -bg=<backGroundGrid> ...
#           -degreex=<num> -degreet=<num> -ts=[fe|be|im|pc] -nc=[] -d1=<> -d2=<> -smVariation=[nc|c|g|h] ...
#           -multiDomainAlgorithm=[0|1] -useExactInterface=[0|1] -useExactVelocity=[0|1] -pi=[0|1] -piOption=[0|1] -adCns=<> ...
#           -problem=[1|2] -angle[<degrees> -piGhostOption=[0|1]
# 
#  -rhog : fluid density
#  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep
#  -d1, -d2 : names for domains
#  -multiDomainAlgorithm =1: use new multi-domain algorithm.
#  -useExactInterface=1 : use exact interface positions
#  -useExactVelocity=1 : use exact velocity and acceleration in GridEvolution
#  -pi=1 : project interface values (impedance matching)
#  -piOption : 0=linear FSR, 1=nonlinear-FSR for the interface projection
#  -piGhostOption : =extrap ghost, 1=compatibility for ghost at interfaces
#  -problem : 0=TZ, 1=receding-piston, 2=shock
#  -angle : angle in degrees for the rotated piston
#  -godunovType : 0=linear, 2=SVK
# 
# Examples: 
# 
# cgmp elasticPiston -method=cns -cnsVariation=godunov -g="elasticPistonGrid8" -tp=.05 -tf=1. -smVariation=g -debug=0 
# 
# cgmp elasticPiston -method=cns -cnsVariation=godunov -g="planeInterfacenp16" -tp=.05 -tf=.5 -nc=1 -smVariation=g -debug=0 
#
#
# TZ testing (linear)
# cgmp elasticPiston -method=cns -cnsVariation=godunov -g="elasticPistonGrid4" -tp=.05 -tf=.05 -smVariation=g -godunovType=0 -problem=0 -tz=poly -debug=3 -go=halt -piGhostOption=0 -pi=0 
#
# TZ testing (SVK)
# cgmp noplot elasticPiston -method=cns -cnsVariation=godunov -g="elasticPistonGridfx2fy2.hdf" -tp=.2 -tf=.2 -smVariation=g -godunovType=2 -problem=0 -tz=poly -debug=3 -piGhostOption=0 -pi=1 -scf=.01 -go=go
# cgmp noplot elasticPiston -method=cns -cnsVariation=godunov -g="elasticPistonGridfx4fy4.hdf" -tp=.2 -tf=.2 -smVariation=g -godunovType=2 -problem=0 -tz=poly -debug=3 -piGhostOption=0 -pi=1 -scf=.01 -go=go
# cgmp noplot elasticPiston -method=cns -cnsVariation=godunov -g="elasticPistonGridfx8fy8.hdf" -tp=.2 -tf=.2 -smVariation=g -godunovType=2 -problem=0 -tz=poly -debug=3 -piGhostOption=0 -pi=1 -scf=.01 -go=go
#
# -- non-conservative: 
# cgmp elasticPiston -method=cns -cnsVariation=godunov -g="planeInterfacenp16" -tp=.05 -tf=.5 -nc=1 -smVariation=nc -debug=0 
# 
# -- TZ:
#  cgmp elasticPiston -method=cns -cnsVariation=godunov -g="elasticPistonGrid4" -tp=.05 -tf=1. -smVariation=g -problem=0 -tz=poly -debug=3 -go=halt
#
# --- set default values for parameters ---
# 
$grid="twoSquaresInterfacee1.order2.hdf"; $domain1="rightDomain"; $domain2="leftDomain";
$problem=1; $godunovType=0; 
$method="ins"; $probeFile=""; $multiDomainAlgorithm=1; $useExactInterface=0; $useExactVelocity=0; $pi=0; $piOption=0; $piGhostOption=1; 
$tFinal=20.; $tPlot=.1;  $cfl=.9; $show="";  $pdebug=0; $debug=0; $go="halt"; 
$muFluid=0.; $rhog=.1; $adCns=0.; 
$nu=.1; $rhoSolid=1.; $prandtl=.72; $cnsVariation="godunov"; $ktcFluid=-1.; $u0=0.; 
$scf=1.; # solidScaleFactor : scale rho,mu and lambda by this amount 
$dsf=1.; # displacement scale factor (for plotting displacement)
$thermalExpansivity=1.; $T0=1.; $Twall=1.;  $kappa=.01; $ktcSolid=-1.; $diss=.0;  $smVariation = "non-conservative";
$tz="none"; $degreex=2; $degreet=2; $fx=2.; $fy=2.; $fz=2.; $ft=2.; $tzType=0; $trigTzScaleFactor=1.; 
$gravity = "0 0. 0.";
$en="max"; # "max", "l1", "l2"
$fic = "uniform";  # fluid initial condition
$solver="best"; 
$backGround="outerSquare"; $deformingGrid="interface"; 
$ts="pc"; $numberOfCorrections=1;  # mp solver
$coupled=0; $iTol=1.e-3; $iOmega=1.; $useNewInterfaceTransfer=0; 
$vg0=0.; $vg1=0.; $vg2=0.;  # for the initial grid velocity
$bcOption=4; # cgcns slip wall BC:  0 or 4 normally. 
$reduceInterpWidth=3; # do not reduce interp width for cgcns
# we sometimes turn off application of the interface conditions when they are done by cgmp:
$applyInterfaceConditions=1;
$xShock=.25; $Mshock=2.; $angle=0;
# 
$solver="best"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
$pm="include plotElasticPiston.cmd"; # command to plot
#
$stressRelaxation=4; $relaxAlpha=.5; $relaxDelta=0.; $tangentialStressDissipation=.5;
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"nu=f"=>\$nu,"muFluid=f"=>\$muFluid,"kappa=f"=>\$kappa, "bg=s"=>\$backGround,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,\
 "show=s"=>\$show,"method=s"=>\$method,"ts=s"=>\$ts,"noplot=s"=>\$noplot,"ktcFluid=f"=>\$ktcFluid,\
  "ktcSolid=f"=>\$ktcSolid, "T0=f"=>\$T0,"Twall=f"=>\$Twall,"nc=i"=> \$numberOfCorrections,"coupled=i"=>\$coupled,\
   "d1=s"=>\$domain1,"d2=s"=>\$domain2,"dg=s"=>\$deformingGrid,"debug=i"=>\$debug,"rhog=f"=>\$rhog,\
   "cfl=f"=>\$cfl,"rhoSolid=f"=>\$rhoSolid,"cnsVariation=s"=>\$cnsVariation,"diss=f"=>\$diss,"fic=s"=>\$fic,\
   "degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"fx=f"=>\$fx,"fy=f"=>\$fy,"fz=f"=>\$fz,"ft=f"=>\$ft,"go=s"=>\$go,\
   "smVariation=s"=>\$smVariation,"scf=f"=>\$scf,"dsf=f"=>\$dsf,"probeFile=s"=>\$probeFile,"en=s"=>\$en,\
   "vg0=f"=>\$vg0,"vg1=f"=>\$vg1,"vg2=f"=>\$vg2,"useNewInterfaceTransfer=i"=>\$useNewInterfaceTransfer,\
   "bcOption=i"=>\$bcOption,"multiDomainAlgorithm=i"=>\$multiDomainAlgorithm,"adCns=f"=>\$adCns,"godunovType=i"=>\$godunovType,\
   "useExactInterface=i"=>\$useExactInterface,"useExactVelocity=i"=>\$useExactVelocity,"pi=i"=>\$pi,"piOption=i"=>\$piOption,\
   "problem=i"=>\$problem,"applyInterfaceConditions=i"=>\$applyInterfaceConditions,"Mshock=f"=>\$Mshock,"angle=f"=>\$angle, \
   "stressRelaxation=f"=>\$stressRelaxation,"relaxAlpha=f"=>\$relaxAlpha,"relaxDelta=f"=>\$relaxDelta, \
   "tangentialStressDissipation=f"=>\$tangentialStressDissipation,"piGhostOption=i"=>\$piGhostOption,\
   "trigTzScaleFactor=f"=>\$trigTzScaleFactor );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $ts eq "fe" ){ $ts="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.;  $tzType=1; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# 
if( $en eq "max" ){ $errorNorm="maximum norm"; }
if( $en eq "l1" ){ $errorNorm="l1 norm"; }
if( $en eq "l2" ){ $errorNorm="l2 norm"; }
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
# piston motion: F(t)=-(a/p)*t^p
$ap=1.; $pp=4.;  
$aplm=-$ap/$pp;   # linear motion is F(t) = a*t^p 
# Gas properties: 
$gamma=1.4; $pg=$rhog/$gamma; $Tg=$pg/$rhog; 
$ag0=0.; $ag1=0.; $ag2=0.; 
#
$rhoSolid=$rhoSolid*$scf; $lambdaSolid=1.*$scf; $muSolid=1.*$scf; 
# 
# ----------  define deforming bodies by a share flag of 100 ----
# ----------  NOTE: we parameterize the boundary by index so grid points match! ---
if( $useExactVelocity eq 1 ){ $exactVelocity="grid evolution parameters...\n linear motion\n $aplm $pp\n exit\n"; }else{ $exactVelocity=""; }
$moveCmds = \
  "turn on moving grids\n" . \
  "specify grids to move\n" . \
  "    deforming body\n" . \
  "      user defined deforming body\n" . \
  "        interface deform\n" . \
  "        boundary parameterization\n  1  \n $exactVelocity" . \
  "        debug\n $debug \n" . \
  "        initial velocity\n" . \
  "          $vg0 $vg1 $vg2\n" . \
  "        initial acceleration\n" . \
  "          $ag0 $ag1 $ag2\n" . \
  "        velocity order of accuracy\n" . \
  "          2  \n" . \
  "        acceleration order of accuracy\n" . \
  "          2  \n" . \
  "      done\n" . \
  "      choose grids by share flag\n" . \
  "         100 \n" . \
  "   done\n" . \
  "done";
# -- Commands to use the 'linear deform' option instead of the general 'interface deform'
$moveCmds2 = \
  "turn on moving grids\n" . \
  "specify grids to move\n" . \
  "    deforming body\n" . \
  "      user defined deforming body\n" . \
  "        linear deform\n" . \
  "          $aplm $pp\n" . \
  "        boundary parameterization\n  1  \n" . \
  "        grid evolution parameters...\n linear motion\n $aplm $pp\n exit\n" . \
  "        debug\n $debug \n" . \
  "        initial velocity\n" . \
  "          $vg0 $vg1 $vg2\n" . \
  "        initial acceleration\n" . \
  "          $ag0 $ag1 $ag2\n" . \
  "        velocity order of accuracy\n" . \
  "          2  \n" . \
  "        acceleration order of accuracy\n" . \
  "          2  \n" . \
  "      done\n" . \
  "      choose grids by share flag\n" . \
  "         100 \n" . \
  "   done\n" . \
  "done";
if( $useExactInterface eq 1 ){ $moveCmds=$moveCmds2; } 
# 
# ------- specify fluid domain ----------
$domainName=$domain1; $solverName="fluid"; 
#
#
#  Cgcns:
# 
$bc = "all=noSlipWall uniform(u=.0,T=$T0)\n bcNumber100=tractionInterface";
$bc = "all=symmetry\n bcNumber100=slipWall\n bcNumber100=tractionInterface";
# turn off the interface:
#- $bc = "all=symmetry\n bcNumber100=slipWall";
if( $tz ne "turn off twilight zone" ){ $bc = "all=dirichletBoundaryCondition\n bcNumber100=slipWall\n bcNumber100=tractionInterface"; }
if( $problem eq 0 ){ $bc="all=symmetry\n bcNumber2=dirichletBoundaryCondition\n bcNumber100=slipWall\n bcNumber100=tractionInterface"; }
$ic = "uniform flow\n r=$rhog T=$Tg"; 
# 
$app=-$ap/$pp; # fix me 
$extraCmds = "OBTZ:user defined known solution\n" \
   . "  specified piston motion\n" \
   . "    $app $pp\n" \
   . "    $rhog $pg $angle\n" \
   . " done"; 
# elastic shock tube: ***********************
if( $problem eq 2 ){\
$extraCmds = " OBTZ:user defined known solution\n" \
           . "   shock elastic piston\n" \
           . "  $xShock $Mshock $gamma $rhoSolid $lambdaSolid $muSolid\n" \
           . "   done"; }
# 
# *wdh* 2014/05/07 : specify lower bounds for fluid when using TZ
if( $tz ne "turn off twilight zone" ){ $slopeLimiter=0; } # turn off slope-limter for TZ
if( $problem eq 0 ){ $extraCmds = "#"; $densityLowerBound=1.e-16; $pressureLowerBound=1.e-16; $velocityLimiterEps=1.e-16; }
if( $probeFile ne "" ){ $probeFileName = $probeFile . "Fluid.dat"; \
$extraCmds .= \
    "\n frequency to save probes 1\n" . \
    "create a probe\n" . \
    "  file name $probeFileName\n" . \
    "  nearest grid point to 0. .5 0.\n" . \
    "  exit"; }else{ $extraCmds .="\n *"; }
# 
$extraCmds .= "\n boundary conditions...\n order of extrap for 2nd ghost line 3\n order of extrap for interp neighbours 3\n  done"; 
if( $tz ne "turn off twilight zone" ){ $ic = "*"; }
#
if( $tzType eq 1 && $godunovType eq 2 ){ $tzCmds = \
  "OBTZ:assign polynomial coefficients\n " . \
   "ct(0,0)=1.1e-2\n " . \
   "ct(1,0)=1.4e-2\n " . \
   "ct(2,0)=1.2e-2\n " . \
   "ct(0,1)=1.1e-2\n " . \
   "ct(1,1)=1.3e-2\n " . \
   "ct(2,1)=1.2e-2\n " . \
   "ct(0,2)=1.1e-2\n " . \
   "ct(1,2)=1.3e-2\n " . \
   "ct(2,2)=1.2e-2\n " . \
   "ct(0,3)=2.1e-1\n " . \
   "ct(1,3)=2.5e-1\n " . \
   "ct(2,3)=2.7e-1\n " . \
   "done"; }else{ $tzCmds ="*"; }
# 
if( $method eq "cns" ){ $mu=$muFluid; $kThermal=$muFluid/$prandtl; $cmd = "include $ENV{CG}/mp/cmd/cnsDomain.h"; }else{ $cmd ="*"; };
$cmd
# 
# ------- specify elastic solid domain ----------
# Cgsm: 
#
$domainName=$domain2; $solverName="solid"; 
$lambda=$lambdaSolid; $mu=$muSolid; 
$bcCommands="all=displacementBC\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
#- $bcCommands="all=dirichletBoundaryCondition\n bcNumber2=slipWall\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
# *wdh* 2014/05/06 -- use symmetry BC's for solid  (to fix blips in corners for stress)
if( $problem eq 0 ){ $bcCommands="all=symmetry\n bcNumber1=dirichletBoundaryCondition\n bcNumber2=slipWall\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; }
if( $problem eq 1 ){ $bcCommands="all=symmetry\n bcNumber1=dirichletBoundaryCondition\n bcNumber2=slipWall\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; }
# elastic shock tube: ************************
if( $problem eq 2 ){ $bcCommands="all=symmetry\n bcNumber1=displacementBC\n bcNumber2=slipWall\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; }
# turn off the interface:
#- $bcCommands="all=dirichletBoundaryCondition\n bcNumber2=slipWall\n bcNumber100=tractionBC"; 
# -- set initial conditions to special values that will result in a smooth elastic response with interface moving as F(t) 
$initialConditionCommands="specialInitialCondition\n" \
  . "Special initial condition option: pistonMotion\n" \
  . "$ap $pp \n" \
  . "$rhog $pg $gamma $angle\n"; 
# elastic shock tube: **********************
if( $problem eq 2 ){\
  $initialConditionCommands = "  OBTZ:user defined known solution\n" \
           . "   shock elastic piston\n" \
           . "  $xShock $Mshock $gamma $rhoSolid $lambdaSolid $muSolid\n" \
           . "   done\n" \
           . "   knownSolutionInitialCondition";}
if( $problem eq 0 ){ $initialConditionCommands="#"; }
$smCheckErrors=1; 
if( $probeFile ne "" ){ $probeFileName = $probeFile . "Solid.dat"; \
$extraCmds = \
    "frequency to save probes 1\n" . \
    "create a probe\n" . \
    "  file name $probeFileName\n" . \
    "  nearest grid point to 0. .5 0.\n" . \
    "  exit"; }else{ $extraCmds ="*"; }
if( $tzType eq 1 && $godunovType eq 2 ){ $tzCmds = \
  "OBTZ:assign polynomial coefficients\n " . \
   "ct(0,0)=1.1e-2\n " . \
   "ct(1,0)=3.2e-2\n " . \
   "ct(2,0)=2.4e-2\n " . \
   "ct(0,1)=1.7e-2\n " . \
   "ct(1,1)=2.2e-2\n " . \
   "ct(2,1)=2.3e-2\n " . \
   "ct(0,2)=1.8e-2\n " . \
   "ct(1,2)=3.5e-2\n " . \
   "ct(2,2)=1.3e-2\n " . \
   "ct(0,3)=2.4e-2\n " . \
   "ct(1,3)=3.6e-2\n " . \
   "ct(2,3)=1.9e-2\n " . \
   "ct(0,4)=2.1e-2\n " . \
   "ct(1,4)=3.2e-2\n " . \
   "ct(2,4)=1.5e-2\n " . \
   "ct(0,5)=1.4e-2\n " . \
   "ct(1,5)=2.6e-2\n " . \
   "ct(2,5)=2.3e-2\n " . \
   "ct(0,6)=2.4e-2\n " . \
   "ct(1,6)=3.8e-2\n " . \
   "ct(2,6)=3.6e-2\n " . \
   "ct(0,7)=1.5e-2\n " . \
   "ct(1,7)=3.3e-2\n " . \
   "ct(2,7)=1.4e-2\n " . \
   "done"; }else{ $tzCmds ="*"; }
# 
# FOR TZ we also need to tell the solid how to move the interface: 
if( $problem eq 0 ){ $tzCmds="SMPDE:TZ interface velocity $vg0 $vg1 $vg2\n SMPDE:TZ interface acceleration $ag0 $ag1 $ag2"; }
include $ENV{CG}/mp/cmd/smDomain.h
# 
continue
#
# -- set parameters for cgmp ---
# 
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  $ts
  number of PC corrections $numberOfCorrections
  OBPDE:interface tolerance $iTol
  OBPDE:interface omega $iOmega
  OBPDE:solve coupled interface equations $coupled
  OBPDE:use new interface transfer $useNewInterfaceTransfer
  OBPDE:project interface $pi
  OBPDE:use nonlinear interface projection $piOption
  $cmd="#"; 
  if( $piGhostOption == 0 ){ $cmd="OBPDE:interface ghost from extrapolation"; }
  if( $piGhostOption == 1 ){ $cmd="OBPDE:interface ghost from compatibility"; }else{ $cmd="OBPDE:interface ghost from exact"; }
  if( $piGhostOption == 2 ){ $cmd="OBPDE:interface ghost from exact"; }
  if( $piGhostOption == 3 ){ $cmd="OBPDE:interface ghost from domain solvers"; }
  $cmd
  # Choose the new multi-domain advance algorithm:
  if( $multiDomainAlgorithm eq 1 ){ $cmd="OBPDE:step all then match advance"; }else{ $cmd="#"; }
  $cmd 
  # 
 # -- for testing solve the domains in reverse order: 
  OBPDE:domain order 1 0
 #* OBPDE:domain order 0 1 
  $tz
  debug $debug
  show file options
    compressed
      open
       $show
    frequency to flush
      100
    exit
  continue
#
continue
# --
        erase
        plot domain: fluid
        contour
 # ghost lines 1
          plot:r
          ## wire frame
          exit
        plot domain: solid
if( $tz eq "turn off twilight zone" ){ $plotCmds="displacement\n displacement scale factor $dsf\n exit this menu";}else{ $plotCmds="contour\n exit"; }
 # $plotCmds
        contour
          if( $problem ne 0 ){ $plotCmds="adjust grid for displacement 1"; }else{ $plotCmds="#"; }
          $plotCmds
        exit
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


