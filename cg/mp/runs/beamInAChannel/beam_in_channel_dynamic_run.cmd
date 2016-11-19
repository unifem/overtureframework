*
*  cgmp command file: shock hitting a flexible beam
*     Solve the Euler equations with Godunov's method and linear elasticity
* 
* Usage:
*    cgmp [-noplot] beam_in_channel_dynamic_run.cmd -g=<grid> -tf=<final time> ...
*          -tp=<tPlot> -xs=<xstep> -show=<show file> -thick=<beam_thickness_computational>
*          -cfl=<cfl>
*
* Examples:
*     cgmp beam_in_channel_dynamic_run.cmd noplot -g="beam_in_channel_dynamice4.hdf" -method=cns -cnsVariation=godunov  -tp=5e-3 -tf=3. -smVariation=g -debug=0 -pi=1 -cnsGodunovOrder=1 -cfl=.9 
*
* --- set default values for parameters ---
* -method=cns -cnsVariation=godunov
$compT=0.005;
$thick=0.001;
$cnsGodunovOrder=2;
$grid="../grid/beam_in_channel.hdf";
$tFinal=3.; $tPlot=.1; $cfl=1.; $debug=1; $tol=.2; $x0=.5; $dtMax=1.e10; $nbz=2; 
$xStep="x=0.02"; $go="halt"; 
$grid="beam_in_channel_dynamic.hdf"; $domain1="fluidDomain"; $domain2="solidDomain";
$problem=1; $godunovType=0; 
$method="ins"; $multiDomainAlgorithm=1; $useExactInterface=0; $useExactVelocity=0; $pi=0; $piOption=0; 
$tFinal=20.; $tPlot=.1;  $cfl=.9; $show="beam_in_channel_dynamic.show";  $pdebug=0; $debug=0; $go="halt"; 
$muFluid=0.; $rhog=.1; $adCns=0.; 
$nu=.1;  $prandtl=.72; $cnsVariation="godunov"; $ktcFluid=-1.; $u0=0.; 
$thermalExpansivity=1.0; $T0=1.; $Twall=1.;  $kappa=.01; $ktcSolid=-1.; 
$diss=2.;  $smVariation = "non-conservative";
$tz="none"; $degreex=2; $degreet=2; $fx=2.; $fy=2.; $fz=2.; $ft=2.;
$gravity = "0 0. 0.";
$en="max"; # "max", "l1", "l2"
$fic = "uniform";  # fluid initial condition
$solver="best"; 
$ts="pc"; $numberOfCorrections=1;  # mp solver
$coupled=0; $iTol=1.e-3; $iOmega=1.; $useNewInterfaceTransfer=0; 
$bcOption=4; # cgcns slip wall BC:  0 or 4 normally. 
$reduceInterpWidth=3; # do not reduce interp width for cgcns
# we sometimes turn off application of the interface conditions when they are done by cgmp:
$applyInterfaceConditions=1;
$stressRelaxation=4; $relaxAlpha=0.1; $relaxDelta=0.1;
$orderOfExtrapForGhost2=2; $orderOfExtrapForInterpNeighbours=2;
* 
$solver="best"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
$interfaceGhostOption="compatibility"; # compatbility or extrapolation
*
* 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"l=i"=> \$nrl,"r=i"=> \$ratio, "tf=f"=>\$tFinal,"debug=i"=> \$debug, \
            "tp=f"=>\$tPlot, "bg=s"=>\$backGround,"show=s"=>\$show,"go=s"=>\$go,\
            "cnsVariation=s"=>\$cnsVariation, "cnsGodunovOrder=i"=>\$cnsGodunovOrder,              \
	    "smVariation=s"=>\$smVariation, "cfl=f"=>\$cfl,"thick=f"=>\$compT, \
            "diss=f"=>\$diss  );
* -------------------------------------------------------------------------------------------------
*
$trat=$thick/$compT;
*$trat=1.0;
$Esolid=220e4;
$Esolid=$Esolid*$trat*$trat*$trat;
$rhoSolid=7600.;
$rhoSolid=$rhoSolid*$trat;
*$rhoSolid=1e11;
$poissonSolid=0.3;
$lambdaSolid=$Esolid*$poissonSolid/((1.+$poissonSolid)*(1.-2.*$poissonSolid));
$muSolid=$Esolid/(2.0*(1.+$poissonSolid));
*
if( $cnsVariation eq "godunov" ){ $pdeVariation="compressible Navier Stokes (Godunov)"; }
if( $cnsVariation eq "jameson" ){ $pdeVariation="compressible Navier Stokes (Jameson)"; }   #
if( $cnsVariation eq "nonconservative" ){ $pdeVariation="compressible Navier Stokes (non-conservative)";}  #
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
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
*
# 
if( $en eq "max" ){ $errorNorm="maximum norm"; }
if( $en eq "l1" ){ $errorNorm="l1 norm"; }
if( $en eq "l2" ){ $errorNorm="l2 norm"; }
* 
if( $smVariation eq "nc" ){ $smVariation = "non-conservative"; }
if( $smVariation eq "c" ){ $smVariation = "conservative"; $cons=1; }
if( $smVariation eq "g" ){ $smVariation = "godunov"; }
if( $smVariation eq "h" ){ $smVariation = "hemp"; }
*
if( $method eq "ins" && $kThermalFluid eq "" ){ $kThermalFluid=$nu/$prandtl; }
if( $method eq "cns" && $kThermalFluid eq "" ){ $kThermalFluid=$muFluid/$prandtl; }
if( $ktcFluid < 0. ){ $ktcFluid=$kThermalFluid;} if( $ktcSolid < 0. ){ $ktcSolid=$kappa; }
*
* Here is the overlapping grid to use:
$grid 
*
$moveCmds = "turn on moving grids\n". \
  "specify grids to move\n". \
  "    deforming body\n" . \
  "      user defined deforming body\n" . \
  "        interface deform\n" . \
  "        boundary parameterization\n  1  \n" . \
  "        debug\n $debug \n" . \
  "      done\n" . \
  "      choose grids by share flag\n" . \
  "         1 \n" . \
  "   done\n" . \
  "done";
* ------- specify fluid domain ----------
$domainName=$domain1; $solverName="fluid"; 
*
*
*  Cgcns:
* 
reduce interpolation width
  2
*
$bc = "bcNumber3=superSonicOutflow uniform(r=1.6158,u=0.3468,T=0.9540)\n" . \
      "bcNumber4=slipWall\n" . \
      "bcNumber2=slipWall\n" . \
      "bcNumber1=slipWall\n" . \
      "bcNumber1=tractionInterface";
*e is p/(gamma-1) ????
$ic = "step function\n" . \
      "$xStep\n" . \
      "r=1.6158 u=0.3468 T=0.9540\n" . \
      "r=1.189 u=0 T=0.8410";
$boundaryPressureOffset=1.0;
# try this  
$freqFullUpdate=1;
$extraCmds = "#";
$extraCmds .="\n *";
$extraCmds .= "\n frequency for full grid gen update $freqFullUpdate\n boundary conditions...\n order of extrap for 2nd ghost line 3\n order of extrap for interp neighbours 3\n  done"; 
$mu=$muFluid; $kThermal=$muFluid/$prandtl;
include $ENV{CG}/mp/cmd/cnsDomain.h
* 
* ------- specify elastic solid domain ----------
* Cgsm: 
*
$domainName=$domain2; $solverName="solid"; 
$lambda=$lambdaSolid; $mu=$muSolid; 
$bcCommands="all=symmetry\n bcNumber2=displacementBC\n bcNumber1=tractionBC\n bcNumber1=tractionInterface";
$initialConditionCommands="zeroInitialCondition";
$extraCmds = "frequency to save probes 1\n" . \
    "create a probe\n" . \
    "  file name tip.txt\n" . \
    "  nearest grid point to 0.05 0.038 0.\n" . \
    "  exit";
include $ENV{CG}/mp/cmd/smDomain.h
*
continue
*
* -- set parameters for cgmp ---
* 
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
#
  OBPDE:use nonlinear interface projection $piOption
  # interfaceGhostOption : compatibility or extrapolation
#  OBPDE:interface ghost from $interfaceGhostOption
#
  # Choose the new multi-domain advance algorithm:
  if( $multiDomainAlgorithm eq 1 ){ $cmd="OBPDE:step all then match advance"; }else{ $cmd="#"; }
  $cmd 
  # 
  * -- for testing solve the domains in reverse order: 
  OBPDE:domain order 1 0
  ** OBPDE:domain order 0 1 
  *$tz
  debug $debug
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
*
*erase
plot all
    plot domain: solid
    contour
      adjust grid for displacement 1
      exit
$go
