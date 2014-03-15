*
* cgmp:  acoustic lens
* 
* Usage:
*    cgmp [-noplot] lens -g=<name> -method=[ins|cns] -nu=<> -mu=<> -kappa=<num> -tf=<tFinal> -tp=<tPlot> ...
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
*  cgmp lens.cmd -method=cns -g="halfLensi1.hdf" -tp=.02 -nc=1 -scf=100. -cnsVariation=godunov -pOffset=.71440 -lambdaSolid=2. -muSolid=4.
*  cgmp lens.cmd -method=cns -g="halfLensi2.hdf" -tp=.05 -nc=1 -scf=100. -cnsVariation=godunov -pOffset=.71440 -lambdaSolid=2. -muSolid=4.
* 
*  cgmp lens.cmd -method=cns -g="lensi1.hdf" -tp=.01 -nc=1 -scf=100. -cnsVariation=godunov -pOffset=.71440 
*  cgmp noplot lens.cmd -method=cns -g="lense2.hdf" -tp=.1 -nc=1 -scf=2. -cnsVariation=godunov -lambdaSolid=2. -muSolid=4. -show="lens.show" -go=go >! lens2.out &
*  nohup $cgmp noplot lens.cmd -method=cns -g="lense4.hdf" -tp=.1 -nc=1 -scf=2. -cnsVariation=godunov -lambdaSolid=2. -muSolid=4. -show="lens4.show" -go=go >! lens4.out &
* 
* --- set default values for parameters ---
* 
$grid="twoSquaresInterfacee1.order2.hdf"; $domain1="fluidDomain"; $domain2="solidDomain";
$method="ins"; 
$tFinal=2.; $tPlot=.1;  $cfl=.9; $show="";  $pdebug=0; $debug=0; $go="halt"; 
$muFluid=0.;  $prandtl=.72; $diss=1.; $adCns=.0; 
$rhoSolid=1.; $muSolid=1.; $lambdaSolid=1.; 
$cnsVariation="jameson"; $ktcFluid=-1.; $u0=0.; $cnsGodunovOrder=2; 
$thermalExpansivity=1.; $T0=1.; $Twall=1.;  $kappa=.01; $ktcSolid=-1.; 
$scf=1.; # solidScaleFactor : scale rho,mu and lambda by this amount 
$tz="none"; $degreeSpace=1; $degreeTime=1;
$gravity = "0 0. 0.";
$solver="best"; 
$backGround="outerSquare"; $deformingGrid="interface"; 
$ts="pc"; $numberOfCorrections=1;  # mp solver
$coupled=0; $iTol=1.e-3; $iOmega=1.; 
# ---- Shock Jump Conditions: ----
$shockSpeed=1.1; 
$gamma=1.4; $Rg=1.;
$a1=1.; $rho1=1.; $u1=0.;  $T1=$a1*$a1/($gamma*$Rg); $p1=$rho1*$Rg*$T1;
$Mshock=$shockSpeed/$a1;
$p2=$p1*( 1. +(2.*$gamma)/($gamma+1.)*( $Mshock*$Mshock -1. ));
$rho2=$rho1/( 1. - 2./($gamma+1.)*(1. - 1./($Mshock*$Mshock) ) );
$T2=$p2/($Rg*$rho2); 
$u2=( $shockSpeed*($rho2-$rho1) + $rho1*$u1 )/$rho2;
# ------------------------------------
$boundaryPressureOffset=$p1;
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
   "pOffset=f"=>\$boundaryPressureOffset,"cnsGodunovOrder=f"=>\$cnsGodunovOrder,"muSolid=f"=>\$muSolid,\
   "lambdaSolid=f"=>\$lambdaSolid,"shockSpeed=f"=>\$shockSpeed,"go=s"=>\$go );
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
* ----------  define deforming bodies by a share flag of 100 or 101 ----
* ----------  NOTE: we parameterize the boundary by index so grid points match! ---
$moveCmds = \
  "turn on moving grids\n" . \
  "specify grids to move\n" . \
  "   deforming body\n" . \
  "     user defined deforming body\n" . \
  "       interface deform\n" . \
  "       boundary parameterization\n  1  \n" . \
  "       debug\n $debug \n" . \
  "     done\n" . \
  "     choose grids by share flag\n" . \
  "        100 \n" . \
  "   done\n" . \
  "   deforming body\n" . \
  "     user defined deforming body\n" . \
  "       interface deform\n" . \
  "       boundary parameterization\n  1  \n" . \
  "       debug\n $debug \n" . \
  "     done\n" . \
  "     choose grids by share flag\n" . \
  "        101 \n" . \
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
$bc = "all=slipWall\n bcNumber2=superSonicInflow uniform(r=$rho2,u=$u2,T=$T2)\n bcNumber100=slipWall\n bcNumber100=tractionInterface\n bcNumber101=slipWall\n bcNumber101=tractionInterface";
* $ic = "uniform flow\n r=1. T=$T0 "; 
$xStep="x=-1.25"; 
# $ic= "step function\n $xStep \n  r=2.6667 u=1.25 T=1.2053 \n  r=1. T=.7144 ";
$ic= "step function\n $xStep \n  r=$rho2 u=$u2 T=$T2 \n  r=$rho1 u=$u1 T=$T1";
if( $method eq "cns" ){ $mu=$muFluid; $kThermal=$muFluid/$prandtl; $cmd = "include cnsDomain.h"; }else{ $cmd ="*"; };
$cmd
* 
* ------- specify elastic solid domain ----------
$domainName=$domain2; $solverName="solid"; 
$bcCommands="all=slipWall\n bcNumber100=tractionBC\n bcNumber100=tractionInterface\n bcNumber101=tractionBC\n bcNumber101=tractionInterface"; 
$bcCommands="all=displacementBC\n bcNumber100=tractionBC\n bcNumber100=tractionInterface\n bcNumber101=tractionBC\n bcNumber101=tractionInterface"; 
$exponent=10.; $x0=.5; $y0=.5; $z0=.5; $cons=0;  $rhoSolid=$rhoSolid*$scf; $lambda=$lambdaSolid*$scf; $mu=$muSolid*$scf; 
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
  * -- solve the solid domain first: 
  OBPDE:domain order 1 0
* 
  $tz
  show file options
    compressed
      open
       $show
    frequency to flush
      40
    exit
  continue
*
continue
* --
$go


              plot domain: fluid
              contour
                wire frame (toggle)
                exit
              plot domain: solid
              displacement
                exit this menu
              plot all

              set view:0 0.193353 -0.326284 0 5.91525 1 0 0 0 1 0 0 0 1



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


