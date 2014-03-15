# ===========================================================================================================================
# cgins : Four dropping cylinders
#
# Usage:
#  cgins [-noplot] sib -g=<name> -tf=<tFinal> -tp=<tPlot> -inflowVelocity=<val> -mass=<> ...
#         -forceLimit=<val> -debug=<num> -ad2=<> -project=<0/1> -rf=<val> -movingWall=[0|1]...
#         -show=<name> -restart=<name> -solver=[best|mg] -flush=<ival> -go=[run/halt/og]
#
# NOTE: new ogen drops.cmd creates grids at twice the grid spacing for the a given $factor
#
# Examples:
#
#  ogen noplot drops -order=2 -factor=2 -ml=2 
# -- ts=pc:
#  cgins drops -g=dropsi2.order2 -nu=.01 -tp=.05 -tf=10. -inflowVelocity=.65 -forceLimit=40. -solver=mg -psolver=mg -go=halt
#  cgins noplot drops -g=dropsi2.order2 -nu=.01 -tp=.05 -tf=10. -inflowVelocity=.65 -forceLimit=40. -show=drops2.show -go=go >! drops2.out &
#  cgins drops -g=dropsi4.order2 -nu=.01 -tp=.05 -tf=10. -ts=pc -inflowVelocity=.65 -forceLimit=80. -solver=mg -psolver=mg -go=halt
#  cgins drops -g=dropsi4.order2 -nu=.01 -tp=.02 -tf=10. -ts=pc -inflowVelocity=.65 -forceLimit=200. -solver=mg -psolver=mg -go=halt 
# -- ts=im:
#  cgins drops -g=dropsi2.order2 -nu=.01 -tp=.05 -tf=10. -ts=im -inflowVelocity=.65 -forceLimit=200. -solver=mg -psolver=mg -go=halt [Ok to t=10
#  cgins drops -g=dropsi4.order2 -nu=.01 -tp=.02 -tf=10. -ts=im -inflowVelocity=.65 -forceLimit=200. -solver=mg -psolver=mg -go=halt 
# -- ts=afs
#  cgins drops -g=dropsi4.order2 -nu=.01 -tp=.02 -tf=10. -ts=afs -inflowVelocity=.65 -forceLimit=200. -solver=mg -psolver=mg -go=halt [TROUBLE
#
# -- restart: 
# cgins drops -g=dropsi2.order2 -nu=.01 -tp=.01 -tf=10. -inflowVelocity=.65 -forceLimit=40. -restart=drops2.show -show=drops2a.show 
#
# cgins noplot drops -g=dropsi4.order2 -nu=.005 -tp=.1 -tf=10. -inflowVelocity=.75 -forceLimit=40. -show=drops4.show -go=go >! drops4.out &
# 
#  -- this should be similar to original movie:
# cgins noplot drops -g=dropsi8.order2 -nu=.005 -tp=.1 -tf=10. -inflowVelocity=.75 -forceLimit=40. -show=drops8.show -go=go >! drops8.out &
#
# -- order=4
#  cgins drops -g=dropsi2.order4 -nu=.01 -tp=.05 -tf=10. -inflowVelocity=.65 -forceLimit=40. -ad2=0 -ad4=1 -go=halt [TROUBLE
#  cgins drops -g=dropsi2.order4 -nu=.01 -tp=.05 -tf=10. -inflowVelocity=.65 -forceLimit=40. -solver=mg -psolver=mg -go=halt [TROUBLE
# 
# -- One drop:
#   ogen -noplot drops -order=2 -numDrops=1 -xa=-1. -xb=1. -ya=-2 -yb=1.5 -x1=0 -y1=0 -factor=2 -ml=2
#   cgins drops -g=drops1i2.order2 -numDrops=1 -nu=.01 -tp=.1 -tf=10. -inflowVelocity=.01 -ts=im -forceLimit=40. -solver=mg -psolver=mg -go=halt
#   cgins drops -g=drops1i2.order2 -numDrops=1 -nu=.01 -tp=.1 -tf=10. -inflowVelocity=.01 -ts=afs -forceLimit=40. -solver=mg -psolver=mg -go=halt [TROUBLE
#
# ============================================================================================================================
$model="ins"; $solver = "best"; $show=" "; $ts="pc"; $noplot=""; $numDrops=5; $mass=.5; $gravity=-1.; 
$nu = .005; $dtMax=.05; $newts=0; $movingWall=0; 
# for nu=.005 the terminal velocity of one drop is about .9 -- for low Re the velocity is prop. to Re
$inflowVelocity=.9;
$tFinal=10.; $tPlot=.1; $cfl=.9; $debug=0; $go="halt"; $project=0; $refactorFrequency=100;
$sep=5.; $forceLimit=30.; $cdv=.1; $flush=5; $ad21=2.; $ad22=2.; 
$restart=""; 
# 
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$cdv=1.; $ad2=1; $ad21=1; $ad22=1;  $ad4=0; $ad41=2.; $ad42=2.; 
$psolver="best"; $solver="best"; 
$iluLevels=1; $ogesDebug=0; 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
# -- for Kyle's AF scheme:
$afit = 20;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
# 
# $gridName = "drops0.hdf"; $show="drops0.show"; $nu=.01; $inflowVelocity=.5;  $tolerance = "1.e-6";
# $gridName = "drops0.hdf"; $show="drops0A.show"; $nu=.01; $inflowVelocity=.5;  $tolerance = "1.e-6"; $forceLimit=25.;
# $gridName = "drops0.hdf"; $show="drops0.show"; $nu=.01; $inflowVelocity=.6; $tFinal=30.; $sep=4.; $tolerance = "1.e-6"; $forceLimit=30.;
# $gridName = "drops0.hdf"; $show="drops0Restart.show"; $nu=.01; $inflowVelocity=.9/2.; $cdv=.1; $tolerance = "1.e-6";
# $gridName = "drops.hdf"; $show="drops.show"; $nu=.005;  $inflowVelocity=.9; $tFinal=30.; $sep=4.; $tolerance = "1.e-7"; $forceLimit=40.;
# $gridName = "drops.hdf"; $show="drops.show"; $nu=.005;  $inflowVelocity=.8; $tFinal=30.; $sep=4.; $tolerance = "1.e-7"; $forceLimit=40.;
# $gridName = "drops.hdf"; $show="drops.show"; $nu=.005;  $inflowVelocity=.75; $tFinal=30.; $sep=5.; $tolerance = "1.e-7"; $forceLimit=40.;
# $gridName = "drops.hdf"; $show="dropsRestart.show"; $nu=.005;  $inflowVelocity=.75; $tFinal=30.; $sep=5.; $tolerance = "1.e-7"; $forceLimit=40.;
# $gridName = "drops.hdf"; $show="dropsA.show"; $nu=.005;  $inflowVelocity=.75; $tFinal=40.; $sep=5.; $tolerance = "1.e-7"; $forceLimit=40.;
# 
#* $gridName = "dropsi2.order4.hdf"; $show=" "; $nu=.005;  $inflowVelocity=.75; $tFinal=40.; $sep=5.; $tolerance = "1.e-7"; $forceLimit=40.; $order=$order4;
#
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"model=s"=>\$model,"inflowVelocity=f"=>\$inflowVelocity,\
 "tp=f"=>\$tPlot,"solver=s"=>\$solver,"psolver=s"=>\$psolver,"show=s"=>\$show,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl,"go=s"=>\$go,"numDrops=i"=> \$numDrops,"newts=i"=> \$newts,\
 "noplot=s"=>\$noplot,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,\
 "dtMax=f"=>\$dtMax,"freqFullUpdate=i"=>\$freqFullUpdate,"mass=f"=>\$mass,"movingWall=i"=>\$movingWall,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
 "forceLimit=f"=>\$forceLimit,"sep=f"=>\$sep,"flush=i"=>\$flush,"gravity=f"=>\$gravity,\
 "ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42 );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
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
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
  $grid
  incompressible Navier Stokes
  exit
#
#**************************************
  show file options
    open
     $show
    frequency to flush
     $flush
  exit  
#************************************
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
#
  recompute dt every 5
#
  plot and always wait
#
#************************************
  turn off twilight zone
# 
$project
#**************************
  turn on moving grids
#*************************
  detect collisions 1
  minimum separation for collisions $sep
#************************
  specify grids to move
    debug 
      $debug
#    ---------------------
      improve quality of interpolation
      limit forces
        $forceLimit $forceLimit
#    ---------------------
# -- defineBody(name,mass) : define a rigid body with a given name and mass
sub defineBody\
{ local($name,$mass)=@_; \
  $cmds ="rigid body\n mass\n $mass \n moments of inertia\n 1.\n done\n $name\n done"; \
}
    defineBody(drop1,$mass);
    $cmds
    if( $numDrops > 1 ){ defineBody(drop2,$mass); }else{ $cmds="#"; }
    $cmds
    if( $numDrops > 2 ){ defineBody(drop3,$mass); }else{ $cmds="#"; }
    $cmds
    if( $numDrops > 3 ){ defineBody(drop4,$mass); }else{ $cmds="#"; }
    $cmds
    if( $numDrops > 4 ){ defineBody(drop5,$mass); }else{ $cmds="#"; }
    $cmds
  done
# 
  frequency for full grid gen update $freqFullUpdate
#
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
     channel=explicit
    done
# 
  pde parameters
    nu  $nu
#  turn on gravity
  gravity
     0. $gravity 0.
#
   #  turn on 2nd-order AD here:
   OBPDE:second-order artificial diffusion $ad2
   OBPDE:ad21,ad22 $ad21, $ad22
   OBPDE:fourth-order artificial diffusion $ad4
   OBPDE:ad41,ad42 $ad41, $ad42
   OBPDE:divergence damping  $cdv
   # OBPDE:check for inflow at outflow
   OBPDE:expect inflow at outflow
  done
#*************************************
  boundary conditions
    if( $movingWall eq 0 ){ $cmd = "all=noSlipWall"; }else{ $cmd = "all=noSlipWall, uniform(v=$inflowVelocity)"; }
    $cmd
    channel(0,1)=inflowWithVelocityGiven,  parabolic(d=.2,p=1,v=$inflowVelocity)
    channel(1,1)=outflow , pressure(1.*p+0.0*p.n=0.)
    done
#**********************************************
$ic ="initial conditions\n uniform flow\n p=1., v=$inflowVelocity\n exit\n";
if( $restart ne "" ){ $ic = "initial conditions\n read from a show file\n $restart\n -1 \n exit\n"; }
$ic 
#*********************************************
#
  maximum number of iterations for implicit interpolation
     10 
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
 debug $debug
 continue
#
#    grid
#      raise the grid by this amount (2D) 1
#     raise the grid by this amount (2D) 2
#   exit this menu
    plot:v
#
  $go




# output resolution
#  512
#movie and save
#  twoDrop
