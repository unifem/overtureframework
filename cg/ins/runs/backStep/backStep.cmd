#
# Flow over a backward facing smoothed step.
#  Usage:
#       cgins [-noplot] backStep -tf=<f> -tp=<f> -g=[gridName] -uDrag=<f> -vDrag=<f> -nu=<f> -show=<> ...
#               -append=[0|1] -go=[go|halt|og]
#
# Examples:
#   cgins backStep -g=backStepGride1.order2.hdf -nu=.05 -uDrag=50. -vDrag=0. -go=halt 
# Implicit:
#   cgins backStep -g=backStepGride1.order2.hdf -nu=.05 -uDrag=50. -vDrag=0. -ts=im -dtMax=.01 -tp=.1 -tf=10 -go=halt 
#
# --- 3D:
# cgins backStep -g=backStepInChannel3dGride2.order4.ml1 -nu=.005 -parabolicWidth=.05 -bcTop=noSlipWall -bcSide=outflow -ad4=1 -ad41=2. -ad42=1. -cfl=3. -ts=afs -psolver=mg -rtolp=1.e-4 -atolp=1.e-4  -dtMax=.01 -tf=200. -tp=.1 -debug=3 -go=halt
#
$tFinal=10.; $tPlot=.5; 
$grid="backStepGride1.order2.hdf"; $nu=.05; $uDrag=0.; $vDrag=0.;  $project=1; $outflowOption="neumann"; 
# NOTE take atol small enough  for Ogmg coarse grid solver or else trouble if no it's are taken
$solver="yale";  $rtoli=1.e-5; $atoli=1.e-9; $idebug=0; 
$psolver="yale"; $rtolp=1.e-5; $atolp=1.e-9; $pdebug=0; $dtolp=1.e20; 
$ts="pc"; $show=" "; $go="halt"; 
$ad2=0; $ad21=2.; $ad22=1.;
$ad4=0; $ad41=2.; $ad42=1.;
$cfl=.9; $dtMax=.01;  $newts=0; 
# -- for Kyle's AF scheme:
$afit = 20;  # max iterations for AFS
$aftol=1e-3;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$cdv=1;  $cDt=.25; $flushFrequency=5; 
$ogmgAutoChoose=1; $ogmgMaxIterations=30; $ogmgCoarseGridSolver="best"; 
$ogmgIlucgLevels=5; # for coarse grid solve, over-ride auto parameters
$bcTop="slipWall"; $bcSide=""; 
$parabolicWidth=.05; # width of parabolic inflow region
$outputYplus=0; # set to 1 to output info about yPlus
$append=0; # set to "1" to append to an existing show file 
# 
$slowStartSteps=-1; $slowStartCFL=.5; $slowStartRecomputeDt=100; $slowStartTime=-1.; $recomputeDt=10000;
# 
# -------------------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"implicitFactor=f"=>\$implicitFactor, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"refactorFrequency=i"=>\$refactorFrequency, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl,"cfls=f"=>\$cfls,"noplot=s"=>\$noplot,\
 "go=s"=>\$go,"dtMax=f"=>\$dtMax,"uDrag=f"=>\$uDrag,"vDrag=f"=>\$vDrag,"iv=s"=>\$implicitVariation,"Tin=f"=>\$Tin,\
 "solver=s"=>\$solver,"psolver=s"=>\$psolver,"pc=s"=>\$pc,"outflowOption=s"=>\$outflowOption,\
 "debug=i"=>\$debug,"pdebug=i"=>\$pdebug,"idebug=i"=>\$idebug,"project=i"=>\$project,"cfl=f"=>\$cfl,\
 "restart=s"=>\$restart,"useNewImp=i"=>\$useNewImp,"p0=f"=>\$p0,"addedMass=i"=>\$addedMass,"delta=f"=>\$delta,\
 "bdebug=i"=>\$bdebug,"E=f"=>\$E,"ampProjectVelocity=i"=>\$ampProjectVelocity,"tension=f"=>\$tension,\
  "tMax=f"=>\$tMax,"pMax=f"=>\$pMax,"append=i"=>\$append,\
  "beamPlotScaleFactor=f"=>\$beamPlotScaleFactor,"numElem=i"=>\$numElem,"fluidOnTwoSides=i"=>\$fluidOnTwoSides,\
  "ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22, "ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,\
  "newts=i"=>\$newts,"project=i"=>\$project,"bcTop=s"=>\$bcTop,"outputYplus=i"=>\$outputYplus,\
   "slowStartCFL=f"=>\$slowStartCFL, "slowStartTime=f"=>\$slowStartTime,"recomputeDt=i"=>\$recomputeDt,\
  "slowStartSteps=i"=>\$slowStartSteps,"slowStartRecomputeDt=i"=>\$slowStartRecomputeDt,\
  "ogmgAutoChoose=i"=>\$ogmgAutoChoose, "flushFrequency=i"=>\$flushFrequency, "parabolicWidth=f"=>\$parabolicWidth,\
  "ogmgCoarseGridSolver=s"=>\$ogmgCoarseGridSolver,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,\
  "cdv=f"=>\$cdv,"cDt=f"=>\$cDt,"bcSide=s"=>\$bcSide,"ogmgIlucgLevels=i"=>\$ogmgIlucgLevels );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $psolver eq "AMG" ){ $psolver="algebraic multigrid"; }
if( $pc eq "ilu" ){ $pc = "incomplete LU preconditioner"; }elsif( $pc eq "lu" ){ $pc = "lu preconditioner"; }else{ $pc="#"; }
if( $ogmgCoarseGridSolver eq "amg" ||  $ogmgCoarseGridSolver eq "AMG" ){ $ogmgCoarseGridSolver="algebraic multigrid"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
if( $ts eq "fe" ){ $ts="forward Euler";}
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;}
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
if( $restart eq $show ){ $append=1; }  # append results to $show
# 
# specify the overlapping grid to use:
$grid
# Specify the equations we solve:
  incompressible Navier Stokes
  exit
# choose time stepping method:
  $ts
#
  turn off twilight zone 
  final time $tFinal
  times to plot $tPlot
  plot and always wait
 # no plotting
  $ts
  if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "*"; }
  $newts
  # -- for the AFS scheme:
  compact finite difference
  # -- convergence parameters for the af scheme
  max number of AF corrections $afit
  AF correction relative tol $aftol
  # optionally turn this on to improve stability of the high-order AF scheme by using 2nd-order dissipation at the boundary
  OBPDE:use boundary dissipation in AF scheme 1
# 
  dtMax $dtMax
  pde parameters
    nu $nu
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22 $ad21, $ad22
    OBPDE:fourth-order artificial diffusion $ad4
    OBPDE:ad41,ad42 $ad41, $ad42
    if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
    $cmd
    OBPDE:expect inflow at outflow
    #
    OBPDE:cDt div damping $cDt
    OBPDE:divergence damping  $cdv
  done
#
 cfl $cfl
# 
  slow start cfl $slowStartCFL
  slow start steps $slowStartSteps
  slow start recompute dt $slowStartRecomputeDt
  slow start $slowStartTime   # (seconds)
#
  recompute dt every $recomputeDt
#
  choose grids for implicit
   all=implicit
  done
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogmgDebug=$ogesDebug;
   $ogmgRtolcg=$rtolp; $ogmgAtolcg=$atolp; $ogmgRtolcg=$rtolp; $ogmgAtolcg=$atolp;
   # TEST Hypre AMG: 
   # $ogmgCoarseGridSolver="AMG";
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   # $ogmgSsr=1;  
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; $ogmgOpav=0; $ogmgRtolcg=1.e-6; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
#
  boundary conditions
    all=noSlipWall
    bcNumber1=inflowWithVelocityGiven, parabolic(d=$parabolicWidth,p=1,u=1.)
    if( $bcTop ne "outflow" ){ $cmd="bcNumber4=$bcTop"; }else{ $cmd="#"; }
    $cmd
    # bcNumber4=slipWall
    # Right boundary: outflow
    #     mainChannel(1,0)=outflow
    # NOTE: $cpn=.01; is BAD for long channel N.B. MG fails with nan's
    $cpn=1.; 
    bcNumber2=outflow, pressure(1.*p+$cpn*p.n=0.)
    # bcNumber2=outflow, pressure(1.*p+0*p.n=0.)
    # Top boundary: outflow
    if( $bcTop eq "outflow" ){ $cmd="bcNumber4=outflow, pressure(1.*p+0*p.n=0.)"; }else{ $cmd="#"; }
    $cmd 
    if( $bcSide ne "outflow" ){ $cmd="bcNumber5=$bcSide\n bcNumber6=$bcSide"; }else{ $cmd="#"; }
    $cmd
    if( $bcSide eq "outflow" ){ $cmd="bcNumber5=outflow, pressure(1.*p+0*p.n=0.)\n bcNumber6=outflow, pressure(1.*p+0*p.n=0.)"; }else{ $cmd="#"; }
    $cmd
#
#    inlet(1,1)=slipWall
#    mainChannel(1,1)=slipWall
    done
#
  debug $debug
#
$userForcingCmds = "user defined forcing...\n drag forcing\n $uDrag $vDrag 0.\n  exit";
if( $uDrag ne 0 || $vDrag ne 0 ){ $cmd = $userForcingCmds; }else{ $cmd="#"; }
$cmd
#
  show file options
    if( $append eq 0 ){ $cmd="OBPSF:create new show file"; }else{ $cmd="OBPSF:append to old show file"; }
       $cmd
    compressed
     OBPSF:maximum number of parallel sub-files 8
      open
      $show
    frequency to flush
      $flushFrequency
    exit
#
# initial conditions: uniform flow or restart from a solution in a show file 
if( $restart eq "" ){ $cmds = "uniform flow\n u=1., v=0., p=1."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
#
if( $project eq "1" && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
  $project
#
  output yPlus $outputYplus
#
  continue
#
$go

 
