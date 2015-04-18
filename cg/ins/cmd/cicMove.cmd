#===============================================================
# cgins: moving cylinder-in-a-channel example
#
#   cgins cicMove -g=<> -move=[0|shift|rotate|oscillate|matrix] -simulateMotion=[0|1|2] -memoryCheck=[0|1]
#
# -simulateMotion : 
#   0 = solve and move grids
#   1 = move and regenerate grids only
#   2 = move grids only
#
# Examples: 
#    cgins cicMove -g=cice2.order2 
#    cgins cicMove -g=cice2.order2 -impGrids="all=implicit"
#
# - simulate motion only:
#    cgins cicMove -g=cice2.order2 -simulateMotion=1  [ regenerate grids (Ogen) but do not solve equations
#    cgins cicMove -g=cice2.order2 -simulateMotion=2  [ just move grids
#
# -- restart example: 
#   - first run:
#    ogen noplot cicArg -order=2 -interp=e -factor=2 
#    cgins -noplot cicMove -g=cice2.order2 -tp=.1 -tf=.5 -show="cicMove.show" -go=go
#   -- then run 
#    cgins cicMove -g=cice2.order2 -tp=.1 -tf=1. -restart="cicMove.show" -show="cicMove2.show" -go=halt
#     (or in parallel:)
#    mpirun -np 2 $cginsp -noplot cicMove -g=cice2.order2 -tp=.1 -tf=1. -restart="cicMove.show" -show="cicMove2.show" -freqFullUpdate=1 -go=og
# -- matrix motion
#    cgins cicMove -g=cice2.order2 -move=matrix
# -- fourth-order (*new* 111127)
#    cgins cicMove -g=cice2.order4 -ts=pc  -ad2=0 -ad4=1 [OK
#    cgins cicMove -g=cice2.order4 -ts=pc4 -ad2=1 -ad4=0 [OK
#    cgins cicMove -g=cice2.order4 -ts=pc4 -ad2=0 -ad4=1 [BAD !
#    cgins cicMove -g=cice2.order4 -ts=im                [BAD
# -- AFS: 
#    cgins cicMove -g=cice2.order2 -ts=afs  -ad2=1 -bcTop=noSlipWall -tp=.05 -debug=2 [OK
#    cgins cicMove -g=cice2.order4 -ts=afs -ad2=0 -ad4=1 -bcTop=noSlipWall -tp=.05 -debug=2 [OK
#  -- multigrid
#  cgins cicMove -g=cice2.order2.ml2 -ts=im -nu=.1 -psolver=mg  -solver=mg -ad2=0 [ok
#  cgins cicMove -g=cice2.order2.ml2 -ts=im -nu=.1 -psolver=mg  -solver=mg -ad2=1 [ok
#  cgins cicMove -g=cice4.order2.ml3 -ts=im -nu=.01 -psolver=mg  -solver=mg -ad2=1 [ok
#  cgins noplot cicMove -g=cice16.order2.ml4 -ts=im -nu=1.e-3 -psolver=mg  -solver=mg -ad2=1 -tp=.1 -tf=1. -show="cicMove.show" -go=go >! cicMove.out
#  cgins cicMove -g=sise1.order2.ml3 -move=rotate -bg="outer-square" -gridToMove="inner-square" -bcTop=noSlipWall -ts=im -nu=.1  -freqFullUpdate=1 -psolver=mg -solver=mg [OK]
# 
# Parallel:
#  mpirun -np 2 $cginsp cicMove -g=cice2.order2 -ts=pc -freqFullUpdate=1 -tp=.005 -move=0 [ok
#  mpirun -np 2 $cginsp -noplot cicMove -g=cice2.order2 -ts=im -freqFullUpdate=1 -tp=.1 -tf=.5 -go=go  [OK
#  srun -N1 -n2 -ppdebug $cginsp cicMove -g=cice2.order2 -ts=im -freqFullUpdate=1 -tp=.05 [ok:zeus
#
#  mpirun -np 1 $cginsp cicMove -g=cice2.order2 -ts=pc -freqFullUpdate=1 -tp=.005 -go=og [ 
#  srun -N1 -n2 -ppdebug $cginsp cicMove -g=cice2.order2 -ts=im -freqFullUpdate=1 -tp=.05 [ok:zeus
#  srun -N1 -n4 -ppdebug $cginsp cicMove -g=cice2.order2 -ts=im -freqFullUpdate=1 -tp=.1 -impGrids="all=implicit"
#  srun -N1 -n4 -ppdebug $cginsp cicMove -g=cice6.order2 -ts=im -freqFullUpdate=1 -tp=.1 -nu=.01 -impGrids="all=implicit"
#
# Parallel and multigrid: 
#   mpirun -np 2 $cginsp cicMove -g=cice2.order2.ml2 -ts=im -nu=.1  -freqFullUpdate=1 -psolver=mg -solver=mg  
#   srun -N1 -n2 -ppdebug $cginsp cicMove -g=cice2.order2.ml2 -ts=im -nu=.1  -freqFullUpdate=1 -psolver=mg -solver=mg  
#  
#
# Parallel and multigrid: *BUG FOUND* 100417 -- POGI not updated properly for implicit interp.
#  -- rotating square: 
#
# TROUBLE APPEARS HERE: Ogmg coarse grid matrix seems to get junk in it after some steps
# mpirun -np 1 $cginsp cicMove -g=sise1.order2.ml3 -move=rotate -bg="outer-square" -gridToMove="inner-square" -bcTop=noSlipWall -ts=pc -nu=.1 -freqFullUpdate=1 -psolver=mg -rtolp=1.e-6 -ogesDebug=7 -tp=.001 -rate=100. -project=0 > ! junk
# 
#  mpirun -np 2 $cginsp cicMove -g=sise1.order2.ml2 -move=rotate -bg="outer-square" -gridToMove="inner-square" -bcTop=noSlipWall -ts=im -nu=.1  -freqFullUpdate=1 -psolver=mg -solver=mg  [OK for many steps
#  mpirun -np 1 $cginsp cicMove -g=sise1.order2.ml3 -move=rotate -bg="outer-square" -gridToMove="inner-square" -bcTop=noSlipWall -ts=im -nu=.1  -freqFullUpdate=1 -psolver=mg -rtolp=1.e-6 -ogesDebug=7 -tp=.01  [Trouble after t=.4 
# *** BAD: 
# mpirun -np 1 $cginsp cicMove -g=sise2.order2.ml2 -move=rotate -bg="outer-square" -gridToMove="inner-square" -bcTop=noSlipWall -ts=im -nu=.1 -freqFullUpdate=1 -psolver=mg -rtolp=1.e-6 -tp=.01
#
#  -- translating circle: 
#  mpirun -np 2 $cginsp cicMove -g=cice2.order2.ml2 -ts=im -nu=.1  -freqFullUpdate=1 -psolver=mg -solver=mg  [OK for some steps
#  srun -N1 -n2 -ppdebug $cginsp cicMove -g=cice2.order2.ml2 -ts=im -nu=.1 -psolver=mg -solver=mg -freqFullUpdate=1 [OK
# 
#  srun -N1 -n1 -ppdebug $cginsp -noplot cicMove -g=cice2.order2 -ts=pc -freqFullUpdate=1 -tp=.005
#  totalview srun -a -N1 -n1 -ppdebug $cginsp -noplot cicMove -g=cice2.order2 -ts=pc -freqFullUpdate=1
#  srun -N1 -n1 -ppdebug memcheck_all $cginsp cicMove -g=cice2.order2 -ts=pc -freqFullUpdate=1 -tp=.005
#
# --- Check for bugs:
#  mpirun -np 1 $cginsp cicMove -g=square8 -ts=pc -freqFullUpdate=1 -tp=.01 -gridToMove=square [trouble
#  mpirun -np 2 $cginsp -noplot cicMove -g=nonSquare8 -ts=pc -freqFullUpdate=1 -gridToMove=square -tp=.1 -tf=.5 -go=go [ok
#  mpirun -np 4 $cginsp -noplot cicMove -g=nonSquare8 -ts=im -freqFullUpdate=1 -gridToMove=square -bcTop=noSlipWall -tp=.1 -tf=.5 -go=go [ok
#  mpirun -np 2 $cginsp -noplot cicMove -g=nonSquare8 -ts=im -freqFullUpdate=1 -gridToMove=square -bcTop=noSlipWall -tp=.1 -tf=.5 -psolver=mg -go=go [ok
#  mpirun -np 1 $cginsp -noplot cicMove -g=nonSquare8 -ts=im -freqFullUpdate=1 -gridToMove=square -bcTop=noSlipWall -tp=.05 -tf=.05 -psolver=mg -solver=mg -go=go -move=0 -debug=7 >! junk [ n=1: ok, n=2: bad
# mpirun -np 1 $cginsp -noplot cicMove -g=nonSquare8 -ts=im -freqFullUpdate=1 -gridToMove=square -bcTop=noSlipWall -tp=.05 -tf=.1 -psolver=mg -solver=mg -go=go -move=1 -debug=3 > ! junk [OK
# mpirun -np 2 $cginsp -noplot cicMove -g=nonSquare8 -ts=im -freqFullUpdate=1 -gridToMove=square -bcTop=noSlipWall -tp=.05 -tf=.05 -psolver=mg -go=go -move=1 -debug=3 > ! junk  [OK pressure=MG
# mpirun -np 2 $cginsp -noplot cicMove -g=nonSquare8 -ts=im -freqFullUpdate=1 -gridToMove=square -bcTop=noSlipWall -tp=.05 -tf=.1 -solver=mg -go=go -move=1 -debug=7 > ! junk  [OK
#
# NEXT BUG: -- bugs found ---
# mpirun -np 1 $cginsp -noplot cicMove -g=cice2.order2.ml2 -ts=im -nu=.1 -freqFullUpdate=1 -tp=.05 -tf=.05 -go=go [null row in matrix? 
# mpirun -np 1 $cginsp -noplot cicMove -g=sise1.order2.ml2 -gridToMove="inner-square" -ts=im -nu=.1 -freqFullUpdate=1 -tp=.05 -tf=.05 -go=go [null row in matrix? 
# mpirun -np 1 $cginsp -noplot cicMove -g=sise1.order2.ml2 -gridToMove="inner-square" -ts=im -nu=.1 -freqFullUpdate=1 -tp=.05 -tf=.1 -go=go [empty row
# mpirun -np 1 $cginsp -noplot cicMove -g=square8 -gridToMove=square -bcTop=noSlipWall -ts=im -nu=.1 -freqFullUpdate=1 -tp=.05 -tf=.1 -go=go [OK
# BAD: empty row: 
#  mpirun -np 1 $cginsp -noplot cicMove -g=sise1.order2.ml2 -gridToMove="inner-square" -ts=pc -nu=.1 -freqFullUpdate=1 -tp=.05 -tf=.1 -go=go 
#===============================================================
#
$grid="cic2.hdf"; $show = " "; $tFinal=5.; $tPlot=.1; $nu=.1; $cfl=.9; $memoryCheck=0; $flush=100; 
$pGrad=0.; $bg="square"; $gridToMove="Annulus"; $move="shift"; $rate=1.; $simulateMotion=0; 
# 
$implicitVariation="viscous"; $refactorFrequency=100; $impGrids="all=explicit"; $impFactor=.5; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.05; 
$order = 2; $fullSystem=0; $go="halt";  $outflowOption="neumann"; $bcTop="slipWall"; 
$show=" "; $restart="";
$psolver="best"; $solver="best"; 
$iluLevels=1; $ogesDebug=0; $project=1; $newts=0; $useNewImp=0
$ts="im"; 
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$cdv=1.; $ad2=1; $ad21=1.; $ad22=1.; $ad4=0; $ad41=1.; $ad42=1.; 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
# For oscillate motion:
$vx=0.; $vy=1.; $t0=.5;    $freq=.5; $amp=.25; 
# -- for Kyle's AF scheme:
$afit = 20;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
#
## $ogesCmd="do not scale rows";   # ****************** why was this done??
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "psolver=s"=>\$psolver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,\
 "imp=f"=>\$implicitFactor,"simulateMotion=f"=>\$simulateMotion,"memoryCheck=i"=>\$memoryCheck,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,"move=s"=>\$move,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"outflowOption=s"=>\$outflowOption,\
  "bg=s"=>\$bg,"gridToMove=s"=>\$gridToMove,"bcTop=s"=>\$bcTop,"ogesDebug=i"=>\$ogesDebug,"rate=f"=>\$rate,\
  "ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,"newts=i"=>\$newts,"flush=i"=>\$flush,\
  "vx=f"=>\$vx,"vy=f"=>\$vy,"t0=f"=>\$t0,"freq=f"=>\$freq,"amp=f"=>\$amp );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
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
if( $project eq "1" && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $simulateMotion eq 0 ){ $simulateMotion="solve and move grids"; }elsif( $simulateMotion eq 1 ){ $simulateMotion="move and regenerate grids only"; }else{ $simulateMotion="move grids only"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
#
$grid
# 
  incompressible Navier Stokes
  $simulateMotion
  exit
#
if( $memoryCheck ne 0 ){ $cmd="turn on memory checking"; }else{ $cmd="#"; }
$cmd
#
  show file options
   open
     $show
    frequency to flush
      $flush
  exit  
#
  turn off twilight zone
# 
  final time $tFinal
  times to plot $tPlot
  plot and always wait
# 
# -- trouble here with AFS: 
  plot residuals 0
#
  $project
if( $move ne "0" ){ $cmd="turn on moving grids"; }else{ $cmd="#"; }
  $cmd
#  detect collisions 1
#**********
  specify grids to move
   if( $move eq "shift" || $move eq "0" ){ $cmd="translate\n 1. 0. 0.\n -.5"; }
   if( $move eq "rotate" ){ $cmd="rotate\n 0. 0. 0 \n $rate 0. "; }
   if( $move eq "matrix" ){ $cmd="matrix motion\n translate along a line\n point on line: .0 .0 0\n tangent to line: -1 0 0\n edit time function\n linear parameters: 0,.5 (a0,a1)\n exit\n exit"; }
   # oscillate options:
   if( $move eq "oscillate" ){ $cmd="oscillate\n $vx $vy 0.\n $freq \n $amp \n  $t0"; }
       $cmd
       $gridToMove
      done
  done
#**************
# 
  frequency for full grid gen update $freqFullUpdate
#
#*  useNewImplicitMethod
#  implicitFullLinearized
  implicit factor $impFactor
  cfl $cfl
  dtMax $dtMax
# 
# use full implicit system 1
# use implicit time stepping
  $ts
  $newts
  # -- for the AFS scheme:
  compact finite difference
  # -- convergence parameters for the af scheme
  max number of AF corrections $afit
  AF correction relative tol $aftol
  # optionally turn this on to improve stability of the high-order AF scheme by using 2nd-order dissipation at the boundary
  OBPDE:use boundary dissipation in AF scheme 1
#
  choose grids for implicit
    all=implicit
#    all=explicit
#    $impGrids
#    $gridToMove=implicit
    done
#
  pde parameters
    nu $nu
    #  turn on 2nd-order AD here:
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22 $ad21 , $ad22
    OBPDE:fourth-order artificial diffusion $ad4
    OBPDE:ad41,ad42 $ad41, $ad42
   if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
   $cmd
 done
#
  debug $debug
#
#*
# Here is were we specify a pressure gradient for flow in a periodic channel:
# This is done by adding a const forcing to the "u" equation 
if( $pGrad != 0 ){ $cmds ="user defined forcing\n constant forcing\n 1 $pGrad\n  done\n exit";}else{ $cmds="*"; }
$cmds
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogmgDebug=$ogesDebug; $ogmgCoarseGridSolver="best"; $ogmgDebugcg=$ogesDebug; $ogmgRtolcg=$rtolp; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  boundary conditions
    all=noSlipWall
    $bg(0,0)=inflowWithVelocityGiven, uniform(u=1.)
    $d=.5; 
    $bg(0,0)=inflowWithVelocityGiven, parabolic(d=$d,p=1.,u=1.)
#    $bg(0,0)=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)
    $bg(1,0)=outflow
    $bg(0,1)=$bcTop
    $bg(1,1)=$bcTop
   done
#
# initial conditions: uniform flow or restart from a solution in a show file 
if( $restart eq "" ){ $cmds = "uniform flow\n u=1., v=0., p=1."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
#
  continue
#
$go 
