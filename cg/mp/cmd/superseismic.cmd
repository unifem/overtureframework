*
* cgmp:  superseismic shock (compare to the exact solution)
* 
* Usage:
*    cgmp [-noplot] superseismic -g=<name> -method=[ins|cns] -nu=<> -mu=<> -kappa=<num> -tf=<tFinal> -tp=<tPlot> ...
*           -solver=<yale/best> -ktcFluid=<> -ktcFluid=<> -tz=[poly/trig/none] -bg=<backGroundGrid> ...
*           -degreex=<num> -degreet=<num> -ts=[fe|be|im|pc] -nc=[] -d1=<> -d2=<> -scf=<>
* 
*  -ktcFluid -ktcSolid : thermal conductivities 
*  -ts = time-stepping-method, be=backward-Euler, fe=forward-Euler, im=implicit-multistep
*  -scf : solid scale factor: scale rho,mu and lambda by this amount (make solid heavier but with same sound speeds)
*  -d1, -d2 : names for domains
*  -past : this file has grids at a time t<0 -- used to compute the grid velocity at t=0
*  -smVariation : nc, c, g, h  : solid algorithm 
* 
* Examples: 
* 
*  cgmp superseismic.cmd -method=cns -g="superseismicGrid2.order2" -dg="innerInterface" -tp=.01 -nc=1 -cnsVariation=godunov -pOffset=1.045151e-02 -cnsGodunovOrder=1
*  cgmp superseismic.cmd -method=cns -g="superseismicGrid4.order2" -dg="innerInterface" -tp=.01 -nc=1 -cnsVariation=godunov -pOffset=1.045151e-02-cnsGodunovOrder=1  
*  cgmp superseismic.cmd -method=cns -g="superseismicGrid8.order2" -dg="innerInterface" -tp=.05 -tf=.5 -nc=1 -cnsVariation=godunov -pOffset=1.045151e-02 -cnsGodunovOrder=1 -show="superseismic.show"
* 
* --- set default values for parameters ---
* 
$grid="superseismicGrid2.order2"; $domain1="fluidDomain"; $domain2="solidDomain"; 
$past="superseismicGrid2tmp01.hdf"; $past2=""; 
$obliqueShow=""; # profile a show file with the oblique shock initial condition
$method="ins"; 
$tFinal=1.5; $tPlot=.1;  $cfl=.9; $show="";  $pdebug=0; $debug=0; $go="halt"; $dtMax=1.; 
$muFluid=0.; $rhoSolid=1.; $prandtl=.72; $diss=1.; $adCns=.0; $boundaryPressureOffset=0.; 
$cnsVariation="jameson"; $ktcFluid=-1.; $u0=0.; $cnsGodunovOrder=2; $smVariation="nc"; 
$normalTol=.001; # ogen shared boundary normal tol 
$errorNorm="l2 norm"; # "maximum norm", "l1 norm", "l2 norm"
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
   "cnsEOS=s"=>\$cnsEOS,"cnsGammaStiff=f"=>\$cnsGammaStiff,"cnsPStiff=f"=>\$cnsPStiff,\
   "past=s"=>\$past,"past2=s"=>\$past2,"dtMax=f"=>\$dtMax,"smVariation=s"=>\$smVariation,"obliqueShow=s"=>\$obliqueShow );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $ts eq "fe" ){ $ts="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $smVariation eq "nc" ){ $smVariation = "non-conservative"; $cons=0; }
if( $smVariation eq "c" ){ $smVariation = "conservative"; $cons=1; }
if( $smVariation eq "g" ){ $smVariation = "godunov"; }
if( $smVariation eq "h" ){ $smVariation = "hemp"; }
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
  "    default shared boundary normal tolerance\n" . \
  "     $normalTol\n" . \
  "    deforming body\n" . \
  "      user defined deforming body\n" . \
  "        interface deform\n" . \
  "        boundary parameterization\n  1  \n" . \
  "        debug\n $debug \n" . \
  "        provide past history\n" . \
  "      done\n" . \
  "      choose grids by share flag\n" . \
  "         100 \n" . \
  "   done\n" . \
  "   deforming grid history\n" . \
  "    $past\n" . \
  "     -.01\n" . \
  "    $past2\n" . \
  "     -.02\n" . \
  "     done\n" . \
  "   done\n" . \
  "done";
* 
* ------- specify fluid domain ----------
$domainName=$domain1; $solverName="fluid"; 
*
*  Cgcns:
$bc = "all=dirichletBoundaryCondition\n bcNumber100=slipWall\n bcNumber100=tractionInterface";
#   --- moving shock ---
# xi=6.047047e-02, theta=1.551374e-01, beta=1.510326e+00, normalOption=1 (0=deformed, 1=un-deformed)
$xInterface=0.; 
$ic="OBTZ:user defined known solution\n"\
  .  "oblique shock flow\n"\
  .  "1.114827e-01 0.000000e+00 0.000000e+00 0.000000e+00 9.375000e-02\n"\
  .  "4.032762e-01 7.209148e-01 -4.364727e-02 0.000000e+00 2.252081e-01\n"\
  .  "9.981722e-01 -6.043363e-02 0.000000e+00\n"\
  .  "$xInterface .0 0.\n"\
  .  "9.981722e-01\n done";
# -- read initial conditions from a show file
if( $obliqueShow ne "" ){ $ic.="\n use grid from show file 0\n read from a show file\n oblique16.show\n -1\n initial time 0."; }
if( $method eq "cns" ){ $mu=$muFluid; $kThermal=$muFluid/$prandtl; $cmd = "include cnsDomain.h"; }else{ $cmd ="*"; };
$cmd
* 
* ------- specify elastic solid domain ----------
$domainName=$domain2; $solverName="solid"; 
$bcCommands="all=dirichletBoundaryCondition\n bcNumber100=tractionBC\n bcNumber100=tractionInterface"; 
$cons=0;  $rhoSolid=1.; $lambda=1.398307e-01; $mu=7.203351e-02; 
# These next values all use the real normal to the solid interface
$np=1; $ns=1;
# real normal: normalOption=0
# xi=5.882442e-02 (3.370391e+00 degrees)
# theta=1.510901e-01 (8.656828e+00 degrees)
# p-wave: alphap=2.934954e-01 normal: [n1,n2] = [5.328205e-01,-8.462283e-01]
# s-wave: alphas=-2.213346e-01 normal: [n1,n2] = [2.683906e-01,-9.633102e-01]
$app=2.934768e-01; $k1p=5.328216e-01; $k2p=-8.462276e-01; $k3p=0.; $xa=$xInterface; $ya=.0; $za=0.;
$aps=-2.214236e-01; $k1s=2.683919e-01; $k2s=-9.633098e-01; $k3s=0.; $xa=$xInterface; $ya=.0; $za=0.;
# undeformed normal : (normalOption=1)
# xi=6.047047e-02 (3.464703e+00 degrees)
# theta=1.551374e-01 (8.888718e+00 degrees) --normals should be the same, these have more correct digits: 
#  p-wave: alphap=3.079172e-01 normal: [n1,n2] = [5.328205e-01,-8.462283e-01]
#  s-wave: alphas=-1.229946e-01 normal: [n1,n2] = [2.683906e-01,-9.633102e-01]
$app=3.079172e-01; $k1p=5.328205e-01; $k2p=-8.462283e-01; $k3p=0.; $xa=$xInterface; $ya=.0; $za=0.;
$aps=-1.229946e-01; $k1s=2.683906e-01; $k2s=-9.633102e-01; $k3s=0.; $xa=$xInterface; $ya=.0; $za=0.;
$initialConditionCommands="Special initial condition option: travelingWave\n"\
 . "$np $ns\n"\
 . "$app $k1p $k2p $k3p $xa $ya $za\n"\
 . "$aps $k1s $k2s $k3s $xa $ya $za\n"\
 . "specialInitialCondition"; 
$smCheckErrors=1; 
* 
include smDomain.h
* 
# Do not match interfaces geometrically since the fluid grid is initially deformed
  match interfaces geometrically 0 
continue
*
* -- set parameters for cgmp ---
* 
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  debug $debug
  dtMax $dtMax
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
      4
    exit
  continue
*
continue
* --
 plot domain: solid
 contour
   adjust grid for displacement 1
   exit
 plot all
$go

