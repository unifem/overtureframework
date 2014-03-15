*
* cgmp:  deforming cavity (for Veronica's shock tube)
* 
* Usage:
*    cgmp [-noplot] cavityDeform -g=<name> -method=[ins|cns] -nu=<> -mu=<> -kappa=<num> -tf=<tFinal> -tp=<tPlot> ...
*           -solver=<yale/best> -ktcFluid=<> -ktcFluid=<> -tz=[poly/trig/none] -bg=<backGroundGrid> ...
*           -degreex=<num> -degreet=<num> -ts=[fe|be|im|pc] -nc=[] -d1=<> -d2=<> -scf=<>
* 
*  -ktcFluid -ktcSolid : thermal conductivities 
*  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep
*  -scf : solid scale factor: scale rho,mu and lambda by this amount (make solid heavier but with same sound speeds)
*  -d1, -d2 : names for domains
* 
* Examples: 
* 
*  cgmp cavityDeform.cmd -method=cns -g="cavityDeformi1.hdf" -d1="innerDomain" -d2="outerDomain" -dg="innerInterface" -tp=.01 -nc=1 -scf=100. -cnsVariation=godunov -cnsGodunovOrder=1 -pOffset=.71440 -show="cavityDeform.show"
* 
*  cgmp cavityDeform.cmd -method=cns -g="cavityDeformi2.hdf" -d1="innerDomain" -d2="outerDomain" -dg="innerInterface" -tp=.01 -tf=1. -nc=1 -scf=100. -cnsVariation=godunov -cnsGodunovOrder=1 -show="cavityDeform.show"
* 
* -- stiffened gas examples:
*  cgmp cavityDeform.cmd -method=cns -cnsEOS=stiffened -g="cavityDeformi2.hdf" -d1="innerDomain" -d2="outerDomain" -dg="innerInterface" -tp=.01 -nc=1 -scf=100. -cnsVariation=godunov -pOffset=.71440
*  cgmp cavityDeform.cmd -method=cns -cnsEOS=stiffened -cnsGammaStiff=7.15 -cnsPStiff=.11111 -g="cavityDeformi2.hdf" -d1="innerDomain" -d2="outerDomain" -dg="innerInterface" -tp=.01 -nc=1 -scf=100. -cnsVariation=godunov -pOffset=.71440
* --- set default values for parameters ---
* 
$grid="twoSquaresInterfacee1.order2.hdf"; $domain1="leftDomain"; $domain2="rightDomain";
$method="ins"; 
$tFinal=1.5; $tPlot=.1;  $cfl=.9; $show="";  $pdebug=0; $debug=0; $go="halt"; 
$muFluid=0.; $rhoSolid=1.; $prandtl=.72; $diss=1.; $adCns=.0; $boundaryPressureOffset=0.; 
$cnsVariation="jameson"; $ktcFluid=-1.; $u0=0.; $cnsGodunovOrder=2; 
$cnsEOS="ideal"; 
$cnsGammaStiff=1.4; $cnsPStiff=0.;   # for stiffened EOS -- by default make it look like an ideal gas
$thermalExpansivity=1.; $T0=1.; $Twall=1.;  $kappa=.01; $ktcSolid=-1.; 
$scf=1.; # solidScaleFactor : scale rho,mu and lambda by this amount 
$tz="none"; $degreeSpace=1; $degreeTime=1;
$gravity = "0 0. 0.";
$solver="best"; 
$backGround="outerSquare"; $deformingGrid="interface"; 
$ts="pc"; $numberOfCorrections=1;  # mp solver
$coupled=0; $iTol=1.e-3; $iOmega=1.; 
* 
$solver="best"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"nu=f"=>\$nu,"mu=f"=>\$mu,"kappa=f"=>\$kappa, "bg=s"=>\$backGround,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,\
 "show=s"=>\$show,"method=s"=>\$method,"ts=s"=>\$ts,"noplot=s"=>\$noplot,"ktcFluid=f"=>\$ktcFluid,"diss=f"=>\$diss,\
  "ktcSolid=f"=>\$ktcSolid, "T0=f"=>\$T0,"Twall=f"=>\$Twall,"nc=i"=> \$numberOfCorrections,"coupled=i"=>\$coupled,\
   "d1=s"=>\$domain1,"d2=s"=>\$domain2,"dg=s"=>\$deformingGrid,"debug=i"=>\$debug,"kThermal=f"=>\$kThermal,\
   "cfl=f"=>\$cfl,"rhoSolid=f"=>\$rhoSolid,"cnsVariation=s"=>\$cnsVariation,"scf=f"=>\$scf,"adCns=f"=>\$adCns,\
   "pOffset=f"=>\$boundaryPressureOffset,"cnsGodunovOrder=f"=>\$cnsGodunovOrder,"go=s"=>\$go,\
   "cnsEOS=s"=>\$cnsEOS,"cnsGammaStiff=f"=>\$cnsGammaStiff,"cnsPStiff=f"=>\$cnsPStiff  );
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
if( $method eq "ins" && $kThermal eq "" ){ $kThermal=$nu/$prandtl; }
if( $method eq "cns" && $kThermal eq "" ){ $kThermal=$mu/$prandtl; }
if( $ktcFluid < 0. ){ $ktcFluid=$kThermal;} if( $ktcSolid < 0. ){ $ktcSolid=$kappa; }
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
  "      done\n" . \
  "      choose grids by share flag\n" . \
  "         100 \n" . \
  "   done\n" . \
  "done";
* 
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
* ---- shock: use T instead of e for non-ideal EOS
# $bc = "all=slipWall\n bcNumber2=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)\n bcNumber100=slipWall\n bcNumber100=tractionInterface";
$bc = "all=slipWall\n bcNumber2=superSonicInflow uniform(r=2.6667,u=1.25,T=1.205331)\n bcNumber100=slipWall\n bcNumber100=tractionInterface";
* $ic = "uniform flow\n r=1. T=$T0 "; 
$xStep="x=.05"; 
# $ic= "step function\n $xStep \n  r=2.6667 u=1.25 e=10.119 \n  r=1. e=1.786 ";
$ic= "step function\n $xStep \n  r=2.6667 u=1.25 T=1.205331 \n  r=1. T=.7144 ";
if( $method eq "cns" ){ $mu=$muFluid; $kThermal=$muFluid/$prandtl; $cmd = "include cnsDomain.h"; }else{ $cmd ="*"; };
$cmd
* 
* ------- specify elastic solid domain ----------
$domainName=$domain2; $solverName="solid"; 
* $bcCommands="all=displacementBC\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
$bcCommands="all=slipWall\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
$exponent=10.; $x0=.5; $y0=.5; $z0=.5; $cons=0;  $rhoSolid=$rhoSolid*$scf; $lambda=1.*$scf; $mu=1.*$scf; 
* $initialConditionCommands="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0)";
$initialConditionCommands="zeroInitialCondition";
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
  $tz
  show file options
    compressed
      open
       $show
    frequency to flush
      20
    exit
  continue
*
continue
* --
        erase
        plot domain: fluid
        contour
          plot:p
          wire frame
          exit
        plot domain: solid
        displacement
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


