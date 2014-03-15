#
# cgins: flow past a moving sphere in a box
#
# Usage:
#   
#  cgins [-noplot] sib -g=<name> -tf=<tFinal> -tp=<tPlot> -order=<2/4> -model=<ins/boussinesq> -ts=<pc|im> ...
#         -debug=<num> -ad2=<> -bg=<backGround> -project=<0/1> -iv=[viscous/adv/full] -imp=<val> -rf=<val> ...
#         -move=[0|shift|rotate|oscillate|matrix] -simulateMotion=[0|1|2] -freqFullUpdate=<>
#         -show=<name> -restart=<name> -go=[run/halt/og]
#
# -simulateMotion : 
#   0 = solve and move grids
#   1 = move and regenerate grids only
#   2 = move grids only
# 
# Examples
#    ogen noplot sibArg -factor=1 -interp=e -order=2
#    cgins sibMove -g=sibe1.order2 -nu=.1 -tf=5. -tp=.01 -ts=im -show="sibMove.show"
#  -- restart example: 
#    cgins sibMove -g=sibe1.order2 -nu=.1 -tf=5. -tp=.01 -ts=im -show="sibMoveRestart.show" -restart="sibMove.show"
# 
#    cgins sibMove -g=sibe2.order2 -nu=.1 -tf=5. -tp=.01 -ts=im -debug=1
#
# -- Fourth-order:
#  cgins sibMove -g=sibi1.order4 -nu=.05 -tf=5. -tp=.01 -ts=pc -move=shift [OK
# -- AFS
#  cgins sibMove -g=sibi1.order4 -nu=.05 -tf=5. -tp=.05 -ts=afs -move=shift -freqFullUpdate=1 [OK
#  cgins sibMove -g=sibe1.order2 -nu=.05 -tf=5. -tp=.05 -ts=afs -move=shift -freqFullUpdate=1 [OK
# 
# --- set default values for parameters ---
# 
$grid="sibe1.order2.hdf"; $backGround="box"; 
$tFinal=1.; $tPlot=.1; $cfl=.9; $nu=.05; $Prandtl=.72; $thermalExpansivity=.1; 
$gravity = "0. 0. 0."; 
$move="shift"; $rate=1.; $simulateMotion=0; $xshift=-1.; $yshift=0.; $zshift=0.; 
$model="ins"; $ts="pc"; $noplot=""; $implicitVariation="viscous"; $refactorFrequency=100; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; 
$show=" "; $restart=""; $newts=0;
$solver="best"; $ogesDebug=0; $project=1; $cdv=1.; $ad2=1; $ad22=2.; 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
# 
# -- for Kyle's AF scheme:
$afit = 20;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,"move=s"=>\$move,\
 "xshift=f"=>\$xshift,"yshift=f"=>\$yshift,"zshift=f"=>\$zshift,"simulateMotion=f"=>\$simulateMotion,"newts=i"=>\$newts,\
  "freqFullUpdate=i"=>\$freqFullUpdate );
# -------------------------------------------------------------------------------------------------
$kThermal=$nu/$Prandtl; 
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
# 
if( $ts eq "fe" ){ $ts="forward Euler";  }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       }
if( $ts eq "pc" ){ $ts="adams PC";       }
if( $ts eq "mid"){ $ts="midpoint";       }  
if( $ts eq "pc4" ){ $ts="adams PC order 4"; $useNewImp=0; } # NOTE: turn off new implicit for fourth order
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;  $useNewImp=0;}
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $project eq "1" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $simulateMotion eq 0 ){ $simulateMotion="solve and move grids"; }elsif( $simulateMotion eq 1 ){ $simulateMotion="move and regenerate grids only"; }else{ $simulateMotion="move grids only"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# specify the overlapping grid to use:
$grid
# Specify the equations we solve:
  $model
  $simulateMotion
  exit
  turn off twilight zone
# 
  $ts
  $implicitVariation
  $newts
  # -- for the AFS scheme:
  compact finite difference
  # -- convergence parameters for the af scheme
  max number of AF corrections $afit
  AF correction relative tol $aftol
  # optionally turn this on to improve stability of the high-order AF scheme by using 2nd-order dissipation at the boundary
  OBPDE:use boundary dissipation in AF scheme 1
#
  show file options
    compressed
    open
      $show
    frequency to flush
      10
    exit
# 
  frequency for full grid gen update $freqFullUpdate
#
# ***********
  turn on moving grids
#**********
  specify grids to move
   if( $move eq "shift" || $move eq "0" ){ $cmd="translate\n $xshift $yshift $zshift\n 1."; }
   if( $move eq "rotate" ){ $cmd="rotate\n 0. 0. 0 \n $rate 0. "; }
   if( $move eq "matrix" ){ $cmd="matrix motion\n point on line: .1 .1 0\n tangent to line: 0 0 1\n exit"; }
   $freq=.5; $amp=.25; $t0=0.; 
   if( $move eq "oscillate" ){ $cmd="oscillate\n 1. 0. 0.\n $freq \n $amp \n  $t0"; }
     $cmd
     # grids to move:
     north-pole
     south-pole
   done
  done
# 
  # outer box is done explicitly
  choose grids for implicit
    all=implicit
    $backGround=explicit
    done
  final time $tFinal
  times to plot $tPlot
   plot and always wait
  no plotting
  pde parameters
    nu
      $nu
    done
* 
  OBPDE:second-order artificial diffusion $ad2
  OBPDE:ad21,ad22  $ad22 $ad22
#
  pressure solver options
     choose best iterative solver
     # these tolerances are chosen for PETSc
     relative tolerance
       $rtolp
     absolute tolerance
       $atolp
    exit
# 
  implicit time step solver options
     choose best iterative solver
     # these tolerances are chosen for PETSc
     relative tolerance
       $rtol
     absolute tolerance
       $atol
    exit
#
# -- do not project initial conditions on a restart
if( $restart eq "" ){ $projectCmd = "project initial conditions"; }else{ $projectCmd="#"; }
$projectCmd
# 
# initial conditions: uniform flow or restart from a solution in a show file 
if( $restart eq "" ){ $cmds = "uniform flow\n u=1., p=1."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
#
  debug $debug
#
  boundary conditions
    all=noSlipWall
    $backGround=slipWall
    $backGround(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.)
    $backGround(1,0)=outflow , pressure(1.*p+1.*p.n=0.)
# 
# -- oscillating inflow: 
#-    box(0,0)=inflowWithVelocityGiven , parabolic(d=.2,p=1.,u=1.), oscillate(a0=1.5,t0=.0,omega=.5)
# 
    done
  exit
  y+r:0 25
  x+r:0 25
#
$go
