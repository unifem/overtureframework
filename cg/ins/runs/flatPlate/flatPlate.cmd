#
# cgins example: test the turbulence models for a flat plate boundary layer flow
#
# Usage: 
#    cgins [-noplot] flatPlate -g=<name> -tm=[bl|sa|ke|none] -nu=<val> -its=<tFinal> -pits=<tPlot> -ic=<uniform/tz>...
#               -debug=<> -show=<name> -bg=<grid-name> -cfl=<num> -solver=<yale/best> -model=<ins/boussinesq> ...
#               -ad2=[0/1] -ad21=<> -ad22=<> -iv=[viscous/adv/full] -imp=<val> -dtMax=<val> -rf=<val> ...
#               -project=[0|1] -useNewImp=[0|1] -inflow=[uniform|parabolic|blasius] -go=[run/halt/og]
#
#  -tm : bl=Bladwin-Lomax, sa=Spalart-Almaras, ke=K-Epsilon
#  -ic = initial conditions 
#  -go : run, halt, og=open graphics
#  -iv : implicit variation : viscous=viscous terms implicit, adv=viscous + advection, full=full linearized version
#  -imp : .5=CN, 1.=BE, 0.=FE
#  -rf : refactor frequency
# 
# Examples:
#
# --- No turbulence model:
# 
#  cgins flatPlate -g=flatPlate2.order2.dy.01.hdf -tm=none -nu=1.e-3 -debug=1 -tp=.1 -tf=10. -ts=imp -imp=1. -dtMax=.05 -rf=2 -go=halt
#  cgins flatPlate -g=flatPlate4.order2.dy.005.hdf -tm=none -nu=1.e-4 -debug=1 -tp=.1 -tf=10. -ts=imp -imp=1. -dtMax=.05 -rf=2 -go=halt
#  cgins flatPlate -g=flatPlate4.order2.dy.005.hdf -tm=none -nu=1.e-4 -debug=1 -tp=.1 -tf=10. -ts=imp -imp=1. -dtMax=.05 -rf=2 -go=halt
#  cgins flatPlate -g=flatPlate16.order2.dy.001.ml3.hdf -tm=none -nu=1.e-5 -psolver=mg -debug=1 -tp=.1 -tf=10. -ts=imp -imp=1. -dtMax=.05 -rf=2 -go=halt
#
# --- Baldwin Lomax:
# 
#  -ts=pc: 
#  cgins flatPlate -g=flatPlate2.order2.dy.01.hdf -tm=bl -nu=1.e-3 -debug=1 -tp=.1 -tf=10. -ts=pc -project=0 -dtMax=.05 -rf=2 -go=halt
#  -ts=im : OK: 
#  cgins flatPlate -g=flatPlate2.order2.dy.01.hdf -tm=bl -nu=1.e-3 -debug=1 -tp=.1 -tf=10. -ts=imp -imp=1. -dtMax=.05 -rf=2 -go=halt
# 
# Spalart-Alamaras: No ts=imp implemented ?
# OK: 
# cgins flatPlate -g=flatPlate2.order2.dy.01.hdf -tm=sa -nu=1.e-3 -debug=1 -tp=.1 -tf=10. -ts=pc -useNewImp=0 -imp=1. -dtMax=.05 -rf=2 -go=halt
# 
# --- K-epsilon: 
# 
#  cgins -noplot flatPlate -g=flatPlate1.order2.dy.01.hdf -tm=ke -nu=1.e-3 -debug=1 -tp=.01 -tf=10. -ts=imp -imp=1. -dtMax=1.e-4 -rf=10 -go=og
# -- slow start needed: (later can increase to dt=5e-3
#  cgins -noplot flatPlate -g=flatPlate2.order2.dy.01.hdf -tm=ke -nu=1.e-3 -debug=1 -tp=.01 -tf=10. -ts=imp -imp=1. -dtMax=1.e-4 -rf=10 -go=og
#  -- slow start needed: later increase to dt=5.e-4
#  cgins -noplot flatPlate -g=flatPlate2.order2.dy.001.hdf -tm=ke -nu=1.e-3 -debug=1 -tp=.01 -tf=10. -ts=imp -imp=1. -dtMax=1.e-5 -rf=10 -go=og
# OK: 
#  cgins -noplot flatPlate -g=flatPlate2.order2.dy.01.hdf -tm=ke -nu=1.e-3 -debug=1 -tp=.01 -tf=10. -useNewImp=1 -ts=pc -dtMax=.05 -rf=2 -go=og
#  cgins -noplot flatPlate -g=flatPlate2.order2.dy.001.hdf -tm=ke -nu=1.e-3 -debug=1 -tp=.01 -tf=10. -useNewImp=1 -ts=pc -dtMax=.05 -rf=2 -go=og
# 
# OLD:
#  cgins flatPlate -g=flatPlate.hdf -nu=1.e-3 -debug=1 -tp=.1 -tf=10. -ts=imp -imp=1. -dtMax=.05 -rf=2 -go=go 
#
# --- set default values for parameters ---
$tFinal=1.; $tPlot=.1; $cfl=.9;  $nu=.01; $Prandtl=.72; $debug=1; $its=10000; $pits=100; $project=0; 
$inflow="parabolic"; $tm="none"; 
$k0=1.e-4; $eps0=1.e-4; # inflow and initial conditions for k and eps
$tz="poly"; $degreex=2; $degreet=2;  $fx=.5; $ft=0.; $rtol=1.e-8; $atol=1.e-6; $dtMax=.5; $refactorFrequency=100; 
$show = " "; $solver="yale"; $model="ins"; $numberOfCorrections=1; $ts="line";
$bg=square; # back-ground grid
$gravity = "0. 0. 0."; $implicitFactor=.5; 
$ad2=1; $ad21=1.; $ad22=1.;  $ad4=0; $ad41=1.; $ad42=1.; 
$cDt=0.; # = .25; # cDt=0 -> turn this off
$ic ="tz";  $go="halt"; $implicitVariation="viscous"; $useFull=0; 
$ReBlasius=-1.; # by default the Re for Blasius is 1/nu 
$useKnown=1; 
# 
$psolver="choose best iterative solver"; $solver="choose best iterative solver"; 
$iluLevels=1; $ogesDebug=0; 
$rtolp=1.e-4; $atolp=1.e-5;  # tolerances for the pressure solve
$rtol=1.e-7; $atol=1.e-8;    # tolerances for the implicit solver
$useNewImp=1; # use the new implicit method 
$outflowOption="neumann"; $restart="";
$xOffset=1.; # offset from leading edge for Blasius
$newts=0; 
# -- for Kyle's AF scheme:
$afit = 10;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$cdv=1;  $cDt=.25;
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions("g=s"=>\$grid,"its=i"=> \$its,"pits=i"=> \$pits,"nu=f"=>\$nu,"cfl=f"=>\$cfl,"debug=i"=> \$debug, \
           "show=s"=>\$show, "bg=s"=>\$bg, "noplot=s"=>\$noplot,"ts=s"=>\$ts,"restart=s"=>\$restart, \
           "solver=s"=>\$solver,"psolver=s"=>\$psolver,  "model=s"=>\$model, "gravity=s"=>\$gravity, \
           "dtMax=f"=>\$dtMax,"tp=f"=>\$tPlot,"tf=f"=>\$tFinal,"imp=f"=>\$implicitFactor,"cDt=f"=>\$cDt,\
           "ad2=i"=> \$ad2,"ad21=f"=> \$ad21,"ad22=f"=> \$ad22,"k0=f"=>\$k0,"eps0=f"=>\$eps0,\
           "rf=i"=> \$refactorFrequency, "iv=s"=>\$implicitVariation,"tz=s"=>\$tz,"fx=f"=>\$fx,"ReBlasius=f"=>\$ReBlasius,\
           "ic=s"=>\$ic,"tm=s"=>\$tm,"useNewImp=i"=>\$useNewImp,"outflowOption=s"=>\$outflowOption,"inflow=s"=>\$inflow,\
           "useKnown=i"=>\$useKnown, "xOffset=f"=>\$xOffset,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42, "go=s"=>\$go );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }\
 elsif( $model eq "sa" ){ $model="incompressible Navier Stokes\n SpalartAllmaras"; }\
 else                   { $model = "incompressible Navier Stokes\n Boussinesq model"; }
if( $ic eq "tz" ){ $ic="initial conditions\n exit";}else\
                 { $ic="initial conditions\n uniform flow\n  p=0., u=0., n=.1, T=0.\n exit";}
# 
if( $useNewImp eq 1 ){ $useNewImp ="useNewImplicitMethod"; }else{ $useNewImp="#"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $ts eq "line" ){ $ts="steady state RK-line"; }
if( $ts eq "imp" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "afs"){ $ts="approximate factorization"; $newts = 1;}
#
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $tm eq "bl" ){ $tm = "Baldwin-Lomax"; }\
elsif( $tm eq "sa" ){ $tm = "SpalartAllmaras"; }\
elsif( $tm eq "ke" ){ $tm = "k-epsilon"; }\
else{ $tm = "#"; }
#
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
#
# if( $ogesDebug ne "-1" ){ $ogesDebug=$debug; }
#
$grid
# flatPlate121
#flatPlate.121x301
# flatPlateExponential
#
  incompressible Navier Stokes
  $tm
  # Baldwin-Lomax
  # SpalartAllmaras
  exit
# define the time-stepping method:
  $ts
  # -- for the AFS scheme:
  $newts
  compact finite difference
  # -- convergence parameters for the af scheme
  max number of AF corrections $afit
  AF correction relative tol $aftol
  # optionally turn this on to improve stability of the high-order AF scheme by using 2nd-order dissipation at the boundary
  OBPDE:use boundary dissipation in AF scheme 1
  #
  ## first order predictor
  number of PC corrections $numberOfCorrections
# 
  dtMax $dtMax
# 
#
  max iterations $its
  plot iterations $pits
  if( $useKnown eq 0 ){ $cmd="plot residuals 1"; }else{ $cmd="#"; }
  $cmd
# 
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
#
   show file options
    # uncompressed
    save augmented variables 1
    open
     $show 
     frequency to flush
       200
   exit
#
# 
 turn off twilight zone
  plot and always wait
#
  pde parameters
   if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
    $cmd
    nu $nu
    # set cDt=0. to turn this limit off (Then the pressure RHS does not depend on dt)
    ## cDt div damping $cDt
  done  
  pde options...
#  OBPDE:turbulence trip positions
#    0,-1,-1,-1
#    0,10,0,0
#*
  OBPDE:second-order artificial diffusion $ad2
  OBPDE:ad21,ad22  $ad21 $ad22
  #  turn on 4th-order AD here:
  OBPDE:fourth-order artificial diffusion $ad4
  OBPDE:ad41,ad42 $ad41, $ad42
#
##  OBPDE:check for inflow at outflow
#  OBPDE:expect inflow at outflow
# 
#  maybe the eddy viscosity needs a larger AD?
#   OBPDE:ad21,ad22 2 2 10 10 5. 5. 1. 1. .5 .5  2. 0. 10. 10. 2. 2.  15. 15.
#   OBPDE:ad21n,ad22n 2 2 10 10 5. 5. 1. 1. .5 .5  2. 0. 10. 10. 2. 2.  15. 15.
#   we need to decrease the divergence damping -- at least at the start
#*  OBPDE:divergence damping 1.
# increase source term to artificially create n
#   OBPDE:SA scale factor 2
#
# **************************************
#   OBPDE:use p.n=0 boundary condition
#
  close pde options
#
# 
  $useNewImp
  refactor frequency $refactorFrequency
# 
  implicit factor $implicitFactor 
  use full implicit system $useFull
  $implicitVariation
#
# --- choose the known solution: 
#
  OBTZ:user defined known solution
    flat plate boundary layer
      $U=1.; 
      $U $xOffset
    done
# 
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogmgDebug=$ogesDebug; $ogmgCoarseGridSolver="best"; 
   $ogmgRtolcg=$rtolp*1.e-3; $ogmgAtolcg=$atolp*1.e-3;  # *** solve coarse grid equations more accurately
   # $ogmgDebug=3; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   # $ogmgSsr=1;  $ogmgAutoChoose=1; 
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; $ogmgOpav=0; $ogmgRtolcg=1.e-6; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
  boundary conditions
   # all=noSlipWall, uniform(u=0.,v=0.,n=.0001,k=$k0,eps=$eps0)
   all=noSlipWall
 #    square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.,n=.0001)
   $d = sqrt( $nu ); # do this for now 
   # ReBlasius = U*x/nu -- determines the value of "x" for the inflow plane
   if( $ReBlasius<0 ){ $ReBlasius= 1./$nu; }
   # -- TROUBLE: k and eps do not go to zero at the wall:
   if( $inflow eq "parabolic" ){ $cmd="bcNumber1=inflowWithVelocityGiven, parabolic(d=$d,p=1.,u=1.,n=1.e-7,k=$k0,eps=$eps0)"; }
   if( $inflow eq "uniform" ){ $cmd="bcNumber1=inflowWithVelocityGiven, uniform(d=$d,p=1.,u=1.,n=1.e-7,k=$k0,eps=$eps0)"; }
   if( $inflow eq "blasius"){ $cmd="bcNumber1=inflowWithVelocityGiven, blasius(R=$ReBlasius,u=1.,n=1.e-7,k=$k0,eps=$eps0)"; }
   # if( $useKnown ne 0 ){ $cmd="bcNumber1=dirichletBoundaryCondition"; }
   if( $useKnown ne 0 ){ $cmd="bcNumber1=inflowWithVelocityGiven, userDefinedBoundaryData\n known solution\n  done"; }
    $cmd 
  #
  #    square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.,n=1.e-8)
   $ap=.1; $apn=1.; 
   $ap=1.; $apn=0.;   # dirichlet at outflow
   $ap=1.; $apn=1.;   # mixed 
   bcNumber2=outflow, pressure($ap*p+$apn*p.n=0.)
   # bcNumber4=slipWall
   bcNumber4=outflow, pressure($ap*p+$apn*p.n=0.)
   # all=dirichletBoundaryCondition
  done
  # initial conditions: uniform flow or restart from a solution in a show file 
  if( $restart eq "" ){ $cmds = "uniform flow\n p=1., u=1., n=1.e-7, k=$k0, eps=$eps0"; }\
    else{ $cmds = "OBIC:show file name $restart\n use grid from show file 0\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  if( $useKnown ne 0 ){ $cmds="#"; }
  initial conditions
    $cmds
  exit
# 
  debug $debug
  if( $project eq "1" ){ $cmd="project initial conditions"; }else{ $cmd="#"; }
  $cmd
#
  continue
#
  $go


pause
plot:u
 movie mode
pause
 contour
 line plots
  specify lines
    1,1000  
    2,0,2,1
    save results to a matlab file
      laminar121_ss_10000x2.m
    exit this menu
  specify lines
    1,1000  
    3,0,3,1
    save results to a matlab file
      laminar121_ss_10000x3.m
    exit this menu
  specify lines
    1,1000  
    4,0,4,1
    save results to a matlab file
      laminar121_ss_10000x4.m
    exit this menu
  specify lines
    1,1000  
    5,0,5,1
    save results to a matlab file
      laminar121_ss_10000x5.m
    exit this menu
  specify lines
    1,1000  
    6,0,6,1
    save results to a matlab file
      laminar121_ss_10000x6.m
    exit this menu

