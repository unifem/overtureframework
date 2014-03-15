#===============================================================
# cgins: two-dimensional centrifugal pump
# Usage:
#     cgins pump2d -g=<grid> -nu=<> -ts=<> -pIn=<> -uInflow=<> -move=[0|1] -moveOnly=[0|1|2] -freq=<> -solver=[best|yale|mg] ...
#                  -psolver=[best|yale|mg] -ad2=[0|1] -freqFullUpdate=<>
#
#  -uInflow : if >0 this is the normal (radial) component of the inflow on the inlet.
#
#  moveOnly :  0=normal run, 1=move grids and call ogen, 2=move grids, no ogen.
#
# Examples: 
#    cgins pump2d -g=pump2dGride4.order2.s8.hdf -nu=1.e-2 -tp=.001 -project=0 -ts=pc
#
# OLD: 
#  -- restart example:
#   - first run:
#      cgins -noplot pump2d -g=joukowsky2de2.order2 -nu=1.e-2 -tp=.02 -tf=.1  -show="j2.show" -go=og
#   - then run:
#      mpirun -np 2 $cginsp -noplot pump2d -g=joukowsky2de2.order2 -nu=1.e-2 -tp=.02 -tf=.1 -freqFullUpdate=1 -show="j2.show" -go=go
#      cgins -noplot pump2d -g=joukowsky2de2.order2 -nu=1.e-2 -tp=.01 -tf=.2 -restart="j2.show" -show="j2a.show" -go=og
#      mpirun -np 2 $cginsp -noplot pump2d -g=joukowsky2de2.order2 -nu=1.e-2 -tp=.01 -tf=.2 -freqFullUpdate=1 -restart="j2.show" -show="j2a.show" -go=og
#  -- multigrid: 
#    cgins pump2d -g=joukowsky2de2.order2.ml3 -nu=1.e-2 -tp=.01 -psolver=mg  
#    cgins pump2d -g=joukowsky2de2.order2.ml3 -nu=1.e-2 -tp=.01 -psolver=mg -solver=mg -ad2=0
#
#  -- move grids but do not solve: 
#    cgins pump2d -g=joukowsky2de2.order2 -nu=1.e-2 -tp=.025 -ts=pc -moveOnly=1
#    cgins -noplot pump2d -g=joukowsky2de4.order2 -nu=1.e-3 -tp=1. -tf=10. -show="joukowsky2d4.show" 
#    cgins pump2d -g=joukowsky2de4.order2 -impGrids="all=implicit"
#  mpirun -np 2 -all-local $cginsp pump2d -g=joukowsky2de2.order2 -nu=1.e-2 -tp=.01  -ts=pc -moveOnly=1
# srun -N1 -n4 -ppdebug $cginsp pump2d -g=joukowsky2de2.order2 -freqFullUpdate=1 -tp=.01
#  srun -N1 -n1 -ppdebug $cginsp pump2d -g=joukowsky2de4.order2 -ts=pc -freqFullUpdate=1 -tp=.01
#  srun -N1 -n1 -ppdebug $cginsp -noplot pump2d -g=joukowsky2de4.order2 -ts=pc -freqFullUpdate=1
#  totalview srun -a -N1 -n1 -ppdebug $cginsp -noplot pump2d -g=joukowsky2de4.order2 -ts=pc -freqFullUpdate=1
#
#  mpirun -np 1 $cginsp pump2d -g=joukowsky2de2.order2.ml3 -freqFullUpdate=1 -tp=.001 -nu=.01 [Ok
#  mpirun -np 1 $cginsp pump2d -g=joukowsky2de2.order2.ml3 -freqFullUpdate=1 -tp=.001 -nu=.01 -psolver=mg -solver=mg 
#===============================================================
#
$grid="pump2dGride4.order2.s8"; $show = " "; $tFinal=5.; $tPlot=.1; $nu=.1; $cfl=.9;
$uInflow=-1.;  # -1 means set pressure at inflow
$ad2=1; $ad21=1.; $ad22=1.;  $ad4=0; $ad41=1.; $ad42=1.; $newts=0; 
$rate=.25; $ramp=0.; $pIn=10.;
$slowStartSteps=-1; $slowStartCFL=4.; $slowStartRecomputeDt=100; $slowStartTime=-1.; $recomputeDt=10000;
# 
$implicitVariation="viscous"; $refactorFrequency=100; $impGrids="all=explicit"; $impFactor=.5; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.05; 
$order = 2; $fullSystem=0; $go="halt"; $move=1;  $moveOnly=0; $freq=1.; 
$show=" "; $restart="";  $outflowOption="neumann"; 
$psolver="best"; $solver="best"; 
$iluLevels=1; $ogesDebug=0; $project=1; 
$ts="im"; 
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$cdv=1.; $ad2=1; $ad22=2.; 
$rtolp=1.e-3; $atolp=1.e-12;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-12;    # tolerances for the implicit solver
# -- for Kyle's AF scheme:
$afit = 10;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$cdv=1;  $cDt=.25;
$ogmgAutoChoose=1;
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "psolver=s"=>\$psolver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"imp=f"=>\$implicitFactor,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,"ad2=i"=>\$ad2,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"move=i"=>\$move,"moveOnly=i"=>\$moveOnly,\
  "freq=f"=>\$freq,"outflowOption=s"=>\$outflowOption,"rate=f"=>\$rate,"ramp=f"=>\$ramp,"pIn=f"=>\$pIn,"newts=i"=>\$newts,\
  "slowStartCFL=f"=>\$slowStartCFL, "slowStartTime=f"=>\$slowStartTime,"aftol=f"=>\$aftol,"uInflow=f"=>\$uInflow,\
  "ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,\
  "ogmgAutoChoose=i"=>\$ogmgAutoChoose,"slowStartSteps=i"=>\$slowStartSteps,"slowStartRecomputeDt=i"=>\$slowStartRecomputeDt  );
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
if( $ts eq "afs"){ $ts="approximate factorization"; $newts = 1;}
# 
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
if( $project eq "1" && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
#
$grid
# 
  incompressible Navier Stokes
  $cmd="#";
  if( $moveOnly eq 1 ){ $cmd ="move and regenerate grids only"; }elsif( $moveOnly eq 2 ){ $cmd = "move grids only"; }
  $cmd
  exit
#
  show file options
   compressed
     OBPSF:maximum number of parallel sub-files 8
   open
     $show
    frequency to flush
      2
  exit  
#
  turn off twilight zone
# 
  final time $tFinal
  times to plot $tPlot
  plot and always wait
# 
  $project
$cmd="#";
if( $move eq 1 ){ $cmd = "turn on moving grids"; }
#  turn on moving grids
$cmd
#  detect collisions 1
#**********
  specify grids to move
    rotate
      0. 0. 0.
*    specify rate and rampInterval (rampInterval=0. => impulsive start, .5=slow start)
      $rate $ramp
    blade_1
    blade_2
    blade_3
    blade_4
    blade_5
    blade_6
    blade_7
    done
  done
#**************
# 
  frequency for full grid gen update $freqFullUpdate
#
#*  useNewImplicitMethod
#  implicitFullLinearized
  implicit factor $impFactor
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
  done
#
  pde parameters
    nu $nu
    #  turn on 2nd-order AD here:
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22 $ad21,$ad22
    #  turn on 4th-order AD here:
    OBPDE:fourth-order artificial diffusion $ad4
    OBPDE:ad41,ad42 $ad41, $ad42
    #
    if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
    $cmd
   done
  cfl $cfl
# 
  slow start cfl $slowStartCFL
  slow start steps $slowStartSteps
  slow start recompute dt $slowStartRecomputeDt
  slow start $slowStartTime   # (seconds)
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp;
   ##  $ogesIluLevels=$iluLevels;
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
    if( $uInflow ne -1 ){ $cmd="bcNumber3=inflowWithVelocityGiven, userDefinedBoundaryData\n" . \
              "normal component of velocity\n $uInflow 0.\n  done"; }\
    else{ $cmd="bcNumber3=inflowWithPressureAndTangentialVelocityGiven, uniform(p=$pIn)"; }
    $cmd
    bcNumber2=outflow, pressure(1.*p+0.*p.n=0.)
#
#    backGround(0,0)=inflowWithVelocityGiven, uniform(u=1.)
#    $d=.5; 
#    backGround(0,0)=inflowWithVelocityGiven, parabolic(d=$d,p=1.,u=1.)
   done
#
  debug $debug
# initial conditions: uniform flow or restart from a solution in a show file 
if( $restart eq "" ){ $cmds = "uniform flow\n u=0., v=0., p=1."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
#
  continue
#
$go 
