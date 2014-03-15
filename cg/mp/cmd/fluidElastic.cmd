*
* cgmp: fluid (cgcns or cgins) interacting with an elastic solid 
* 
* Usage:
*    cgmp [-noplot] fluidElastic -g=<name> -method=[ins|cns] -nu=<> -mu=<> -kappa=<num> -tf=<tFinal> -tp=<tPlot> ...
*           -solver=<yale/best> -ktcFluid=<> -ktcFluid=<> -tz=[poly/trig/none] -bg=<backGroundGrid> ...
*           -degreex=<num> -degreet=<num> -ts=[fe|be|im|pc] -nc=[] -d1=<> -d2=<> -smVariation=[nc|c|g|h] ...
#           -multiDomainAlgorithm=[0|1] -pi=[0|1]
* 
*  -ktcFluid -ktcSolid : thermal conductivities 
*  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep
*  -d1, -d2 : names for domains
*  -multiDomainAlgorithm =1: use new multi-domain algorithm.
*  -pi=1 : project interface values (impedance matching)
* 
* Examples: 
* 
*  cgmp fluidElastic.cmd -method=ins -g="twoSquaresInterfacee4.order2.hdf" 
*
*  cgmp fluidElastic.cmd -method=cns -g="diskDeforme1.hdf" -d1="outerDomain" -d2="innerDomain" -dg="outerInterface" -tp=.01 -nc=1 -rhoSolid=5. -T0=1.
*  cgmp fluidElastic.cmd -method=cns -g="diskDeforme1.hdf" -d1="innerDomain" -d2="outerDomain" -dg="innerInterface" -tp=.01 -nc=1 -rhoSolid=2. -T0=.5
*  cgmp fluidElastic.cmd -method=cns -g="diskDeforme1.hdf" -d1="innerDomain" -d2="outerDomain" -dg="innerInterface" -tp=.01 -nc=1 -rhoSolid=2. -T0=.5 -cnsVariation=godunov
* 
* cgmp noplot fluidElastic.cmd -method=cns -g="diskDeforme1.hdf" -d1="innerDomain" -d2="outerDomain" -dg="innerInterface" -tp=.01 -nc=1 -tf=8. -show=diskDeform.show > ! fe.out &
*
* --- NOT periodic in y 
*     cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterface1" -tp=.01 -tf=.5 -nc=1 -debug=7
* 
* --- periodic in y: 
*    cgmp fluidElastic.cmd -method=ins -g="twoSquaresInterfacenp2.hdf" -tp=.01 -nc=1
* 
*    cgmp fluidElastic.cmd -method=cns -g="twoSquaresInterfacenp2.hdf" -tp=.01 -nc=1
* 
* -- the planeInterface grid has a grid near the interface that can move
*    cgmp fluidElastic.cmd -method=cns -g="planeInterfacenp2.hdf" -tp=.01 -nc=1 -debug=3
*      -- fluid-on-left, solid-on-right:
*    cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfaceLeftnp1.hdf" -d1="leftDomain" -d2="rightDomain" -tp=.05 -tf=.5 -nc=1 -debug=15
*    --- solid-on-left, fluid-on-right
*    cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfacenp1" -tp=.05 -tf=.5 -nc=1 -debug=15
*    cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfacenp2" -tp=.05 -tf=.5 -nc=1 -debug=3 -show=fluidSolidPlaneInterface2.show
*    cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfacenp4" -tp=.05 -tf=.5 -nc=1 -debug=3
*    cgmp noplot fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfacenp8" -tp=.1  -tf=.5 -nc=1 -diss=5. -debug=3 -show=fluidSolidPlaneInterface8.show -probeFile="planeInterfaceProbe8"  >! junk8
*    cgmp noplot fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfacenp16" -tp=.1  -tf=.5 -nc=1 -debug=3 -show=fluidSolidPlaneInterface16.show -probeFile="planeInterfaceProbe16" >! junk16
*  - need to fix symmetry corner condition for moving grids: 
*    cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfacee2.order2" -mu=0. -tp=.01 -nc=1 -debug=3
* 
*  -- godunov for elastic:
*    cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfacenp1" -tp=.05 -tf=.5 -nc=1 -smVariation=g -debug=15
*    cgmp noplot fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfacenp8" -tp=.05 -tf=.5 -nc=1 -smVariation=g -debug=3 -show=fluidSolidPlaneInterface8g.show -probeFile="planeInterfaceProbe8g" >! junk8g
*    cgmp noplot fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfacenp16" -tp=.05 -tf=.5 -nc=1 -smVariation=g -debug=3 -show=fluidSolidPlaneInterface16g.show -probeFile="planeInterfaceProbe16g" >! junk16g
* 
*  -- hemp for elastic:
*    cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfacenp1" -tp=.05 -tf=.5 -nc=1 -smVariation=h -debug=15
*    cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfacenp4" -tp=.05 -tf=.5 -nc=1 -smVariation=h -cfl=.5   
*    cgmp noplot fluidElastic.cmd -method=cns -cnsVariation=godunov -g=planeInterfacenp8 -tp=.05 -tf=.5 -nc=1 -smVariation=h -cfl=.5 -show=fluidSolidPlaneInterface8h.show -probeFile=planeInterfaceProbe8h >! junk8h  
*    cgmp noplot fluidElastic.cmd -method=cns -cnsVariation=godunov -g=planeInterfacenp16 -tp=.05 -tf=.5 -nc=1 -smVariation=h -cfl=.5 -show=fluidSolidPlaneInterface16h.show -probeFile=planeInterfaceProbe16h >! junk16h  
* 
*  -- fluid-solid Riemann problem (horizontal shock)
*     cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfaceX2Y2f1" -scf=100. -tp=.01 -tf=2. -nc=1 -fic=shock -debug=1
*     cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfaceX2Y2f4" -scf=100. -tp=.01 -tf=2. -nc=1 -fic=shock -debug=1
*     cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfaceX2Y2f8" -scf=100. -tp=.01 -tf=2. -nc=1 -fic=shock -debug=1
*
* ==== TZ examples ===
*  cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfacenp2" -tz=trig -tp=.01 -tf=.5 -debug=3
*  cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterfacenp4" -tz=trig -tp=.01 -tf=.5 -debug=1
* cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="diskDeforme1.hdf" -d1="innerDomain" -d2="outerDomain" -dg="innerInterface" -tp=.01 -nc=1 -rhoSolid=2. -T0=.5 -tz=poly -dsf=.05
*  Troubles here with jameson and PC time-stepping: 
* cgmp fluidElastic.cmd -method=cns -cnsVariation=jameson -g="diskDeformi.5.hdf" -d1="innerDomain" -d2="outerDomain" -dg="innerInterface" -tp=.001 -nc=1 -rhoSolid=2. -T0=.5 -tz=poly -degreex=1 -degreet=0 -debug=7
* -- not periodic in y : 
*   cgmp fluidElastic.cmd -method=cns -cnsVariation=godunov -g="planeInterface2" -tz=trig -tp=.01 -tf=.5 -debug=3
*   cgmp fluidElastic.cmd -method=cns -cnsVariation=nonconservative -g="planeInterface1" -tz=poly -degreex=1 -degreet=1 -tp=.01 -tf=.5 -debug=3
*    -- test for exact answers with TZ and degree=(1,1) *note* set initial grid velocity
*   cgmp noplot fluidElastic.cmd -method=cns -cnsVariation=nonconservative -g="planeInterface0" -vg0=1 -tz=poly -degreex=1 -degreet=1 -tp=.005 -tf=.01 -debug=15 >! junk
* 
* --- set default values for parameters ---
* 
$grid="twoSquaresInterfacee1.order2.hdf"; $domain1="rightDomain"; $domain2="leftDomain";
$method="ins"; $probeFile=""; 
$tFinal=20.; $tPlot=.1;  $cfl=.9; $show="";  $pdebug=0; $debug=0; $go="halt"; 
$muFluid=0.; $rhoFluid=1.4; $pFluid=1.; $TFluid=$pFluid/$rhoFluid; 
$nu=.1; $rhoSolid=1.; $prandtl=.72; $cnsVariation="godunov"; $ktcFluid=-1.; $u0=0.; 
$scf=1.; # solidScaleFactor : scale rho,mu and lambda by this amount 
$dsf=1.; # displacement scale factor (for plotting displacement)
$thermalExpansivity=1.; $T0=1.; $Twall=1.;  $kappa=.01; $ktcSolid=-1.; $diss=.0;  $smVariation = "non-conservative";
$tz="none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.;
$gravity = "0 0. 0.";
$fic = "uniform";  # fluid initial condition
$solver="best"; 
$backGround="outerSquare"; $deformingGrid="interface"; 
$ts="pc"; $numberOfCorrections=1;  # mp solver
$coupled=0; $iTol=1.e-3; $iOmega=1.; $useNewInterfaceTransfer=0; $multiDomainAlgorithm=0; $pi=0;
$vg0=0.; $vg1=0.; $vg2=0.;  # for the initial grid velocity
$slopeLimiter=1;  # 1=use slope limiter, 0=do not 
* 
$bcOption=0; # cgcns slip wall BC:  0 or 4 normally. 
$orderOfExtrapForOutflow=2; $orderOfExtrapForGhost2=2; $orderOfExtrapForInterpNeighbours=2;
$solver="best"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"nu=f"=>\$nu,"muFluid=f"=>\$muFluid,"kappa=f"=>\$kappa, "bg=s"=>\$backGround,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,\
 "show=s"=>\$show,"method=s"=>\$method,"ts=s"=>\$ts,"noplot=s"=>\$noplot,"ktcFluid=f"=>\$ktcFluid,\
  "ktcSolid=f"=>\$ktcSolid, "T0=f"=>\$T0,"Twall=f"=>\$Twall,"nc=i"=> \$numberOfCorrections,"coupled=i"=>\$coupled,\
   "d1=s"=>\$domain1,"d2=s"=>\$domain2,"dg=s"=>\$deformingGrid,"debug=i"=>\$debug,"kThermalFluid=f"=>\$kThermalFluid,\
   "cfl=f"=>\$cfl,"rhoSolid=f"=>\$rhoSolid,"cnsVariation=s"=>\$cnsVariation,"diss=f"=>\$diss,"fic=s"=>\$fic,\
   "degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"fx=f"=>\$fx,"fy=f"=>\$fy,"fz=f"=>\$fz,"ft=f"=>\$ft,"go=s"=>\$go,\
   "smVariation=s"=>\$smVariation,"scf=f"=>\$scf,"dsf=f"=>\$dsf,"probeFile=s"=>\$probeFile,"bcOption=i"=>\$bcOption,\
   "vg0=f"=>\$vg0,"vg1=f"=>\$vg1,"vg2=f"=>\$vg2,"useNewInterfaceTransfer=i"=>\$useNewInterfaceTransfer,\
   "multiDomainAlgorithm=i"=>\$multiDomainAlgorithm,"pi=i"=>\$pi,"slopeLimiter=i"=>\$slopeLimiter );
* -------------------------------------------------------------------------------------------------
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
$grid
* ----------  define deforming bodies by a share flag of 100 ----
* ----------  NOTE: we parameterize the boundary by index so grid points match! ---
$moveCmds = \
  "turn on moving grids\n" . \
  "specify grids to move\n" . \
  "    deforming body\n" . \
  "      user defined deforming body\n" . \
  "        interface deform\n" . \
  "        boundary parameterization\n  1  \n" . \
  "        debug\n $debug \n" . \
  "        initial velocity\n" . \
  "          $vg0 $vg1 $vg2\n" . \
  "      done\n" . \
  "      choose grids by share flag\n" . \
  "         100 \n" . \
  "   done\n" . \
  "done";
* 
if( $probeFile ne "" ){ $probeFileName = $probeFile . "Fluid.dat"; \
$extraCmds = \
    "frequency to save probes 1\n" . \
    "create a probe\n" . \
    "  file name $probeFileName\n" . \
    "  nearest grid point to 0. .5 0.\n" . \
    "  exit"; }else{ $extraCmds ="*"; }
* ------- specify fluid domain ----------
$domainName=$domain1; $solverName="fluid"; 
$ic = "uniform flow\n p=1., u=$u0";
$bc = "all=noSlipWall";
$bc = "all=noSlipWall\n bcNumber100=noSlipWall\n bcNumber100=tractionInterface";
$ktc=$ktcFluid; $rtolp=1.e-4; $atolp=1.e-6; 
if( $method eq "ins" ){ $cmd = "include insDomain.h"; }else{ $cmd ="*"; };
$cmd
*
*  Cgcns:
$adCns=$diss; 
$bc = "all=noSlipWall uniform(u=.0,T=$T0)";
$bc = "all=noSlipWall uniform(u=.0,T=$T0)\n bcNumber100=tractionInterface";
$bc = "all=slipWall\n bcNumber100=slipWall\n bcNumber100=tractionInterface";
if( $tz ne "turn off twilight zone" ){ $bc = "all=dirichletBoundaryCondition\n bcNumber100=slipWall\n bcNumber100=tractionInterface"; }
* gravitationally stratified : rho(y) = rho0*exp( gravity[1]/(Rg*T0) ( y - y0 ))
* $rho0=1.; $y0=0.; 
* $ic = "OBIC:user defined...\n gravitationally stratified\n $rho0 $y0\n r=$rho0 u=0. v=0. T=$T0\n exit";
$ic = "uniform flow\n r=$rhoFluid T=$TFluid "; 
* ---- horizontal step function:
if( $fic eq "shock" ){ $ic="OBIC:step: a*x+b*y+c*z=d 0, 1, 0, 0, (a,b,c,d)\n OBIC:state behind r=1, u=0, v=0, T=1\n OBIC:state ahead r=2., u=0, v=0, T=2.\n OBIC:assign step function\n"; }
if( $tz ne "turn off twilight zone" ){ $ic = "*"; }
if( $method eq "cns" ){ $mu=$muFluid; $kThermal=$muFluid/$prandtl; $cmd = "include cnsDomain.h"; }else{ $cmd ="*"; };
$cmd
* 
* ------- specify elastic solid domain ----------
$domainName=$domain2; $solverName="solid"; 
$bcCommands="all=displacementBC\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
$bcCommands="all=displacementBC\n bcNumber2=slipWall\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
$exponent=10.; $x0=.5; $y0=.5; $z0=.5;  $rhoSolid=$rhoSolid*$scf; $lambda=1.*$scf; $mu=1.*$scf; 
* $initialConditionCommands="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0)";
$initialConditionCommands="zeroInitialCondition";
if( $smVariation eq "hemp" ){ $initialConditionCommands="hempInitialCondition\n OBIC:Hemp initial condition option: default\n"; }
if( $smVariation eq "hemp" ){ $tsSM= "improvedEuler"; }
if( $tz ne "turn off twilight zone" ){$initialConditionCommands="*"; }
if( $probeFile ne "" ){ $probeFileName = $probeFile . "Solid.dat"; \
$extraCmds = \
    "frequency to save probes 1\n" . \
    "create a probe\n" . \
    "  file name $probeFileName\n" . \
    "  nearest grid point to 0. .5 0.\n" . \
    "  exit"; }else{ $extraCmds ="*"; }
* 
include smDomain.h
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
  * -- for testing solve the domains in reverse order: 
  OBPDE:project interface $pi
  # Choose the new multi-domain advance algorithm:
  if( $multiDomainAlgorithm eq 1 ){ $cmd="OBPDE:step all then match advance"; }else{ $cmd="#"; }
  $cmd 
**  OBPDE:domain order 1 0
**  OBPDE:domain order 0 1 
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
*
continue
* --
        erase
        plot domain: fluid
        contour
          * ghost lines 1
          plot:r
          # wire frame
          exit
        plot domain: solid
if( $tz eq "turn off twilight zone" ){ $plotCmds="displacement\n displacement scale factor $dsf\n exit this menu";}else{ $plotCmds="contour\n  displacement scale factor $dsf\n exit"; }
        $plotCmds
        contour
          adjust grid for displacement 1
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


