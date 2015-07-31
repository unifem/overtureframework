#
#  TEST: set a user defined traction boundary values.
#
# cgins command file for flow past a cylinder
#
# Usage:
#   cgins [-noplot] specifiedTraction -g=<name> -tf=<> -tp=<> -nu=<> -ts=[pc|im|afs] -iv=[viscous|full] -tm=[les] ...
#          -solver=[best|yale|mg] -project=[0|1] -model=[ins|boussinesq] ...
#         -psolver=[best|yale|mg] -pc=[ilu|lu] -inflow=[uniform|parabolic|control|pressure|radial] -oscillate=[0|1] ...
#         -wall=[noSlip|slip] -cyl=[noSlip|slip] -restart=<showFile>...
#
# Options:
#  -project : 1=project initial conditions
#  -solver : implicit solver
#  -iv : implicit variation: viscous=viscous terms implicit. full=all terms implicit. 
#  -psolver : pressure solver
#  -inflow : inflow boundary condition
#       = radial : specify inflow on the annulus using the user defined normal component boundary values option
#  -oscillate : =1 to use an oscillating inflow 
#  -wall : boundary condition for the top and bottom walls 
#  -cyl : boundary condition for the cylinder
#
# Examples:
#   cgins specifiedTraction -g=square20.order2
# 
$tFinal=10.; $tPlot=.05; $nu=.1; $cfl=.9; $debug=1; $show=" ";  $go="halt"; $project=0; $newts = 0; $dtMax=.1; 
$solver="yale";  $rtoli=1.e-5; $atoli=1.e-6; $idebug=0; 
$psolver="yale"; $rtolp=1.e-5; $atolp=1.e-6; $pdebug=0;
$recomputeDt=200; $refactorFrequency=1000; $userDefinedOutput=0; 
$slowStartSteps=-1; $slowStartCFL=.5; $slowStartRecomputeDt=100; 
$model="ins";  $restart="";
$grid="cice2.order2.hdf"; $outflowOption="neumann";
$order = -1;  # use default order of accuracy based on the grid
$ad2=1; $ad21=.5; $ad22=.5;   $ad4=0; $ad41=1.;  $ad42=1.; 
$pc="ilu"; 
$ts="im";  # time stepping
$iv=""; # implicit variation 
$fullImplicitSystem=0; # 1=do not use multiple scalar systems for implicit solves, even if possible
$tm = "#"; # turbulence model
$lesOption=0; $lesPar1=.01; 
$inflow = "uniform"; $wall="noSlip"; $cyl="noSlip"; $oscillate=0; $uInflow=0.; 
$Prandtl=.72; $thermalExpansivity=.1; $Tin=1.;
# -- for Kyle's AF scheme:
$afit = 10;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$cdv=1;  $cDt=.25;
$Tset=1.; $Kp=0.; $Ki=1.; $Kd=0.; # for inflow control
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"tp=f"=>\$tPlot,"nu=f"=>\$nu,"recomputeDt=i"=>\$recomputeDt,\
            "debug=i"=>\$debug,"pdebug=i"=>\$pdebug,"idebug=i"=>\$idebug,"order=i"=>\$order,"show=s"=>\$show,\
            "solver=s"=>\$solver,"psolver=s"=>\$psolver,"pc=s"=>\$pc,"project=i"=>\$project,"dtMax=f"=>\$dtMax,\
            "go=s"=>\$go,"tm=s"=>\$tm,"ts=s"=>\$ts,"lesOption=i"=>\$lesOption,"lesPar1=f"=>\$lesPar1,\
            "ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,\
            "inflow=s"=>\$inflow,"wall=s"=>\$wall,"cyl=s"=>\$cyl,"outflowOption=s"=>\$outflowOption,\
            "iv=s"=>\$iv,"newts=i"=>\$newts,"cfl=f"=>\$cfl, "model=s"=>\$model,"oscillate=i"=>\$oscillate,\
            "restart=s"=>\$restart,"Kp=f"=>\$Kp,"Ki=f"=>\$Ki,"Kd=f"=>\$Kd,"uInflow=f"=>\$uInflow,\
            "fullImplicitSystem=i"=>\$fullImplicitSystem,\
            "slowStartCFL=f"=>\$slowStartCFL,"slowStartSteps=i"=>\$slowStartSteps,"slowStartRecomputeDt=i"=>\$slowStartRecomputeDt );
#
$kThermal=$nu/$Prandtl;
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }\
  else{ $model = "incompressible Navier Stokes\n Boussinesq model"; }
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $pc eq "ilu" ){ $pc = "incomplete LU preconditioner"; }elsif( $pc eq "lu" ){ $pc = "lu preconditioner"; }else{ $pc="#"; }
if( $tm eq "les" ){ $tm ="LargeEddySimulation"; }
if( $ts eq "fe" ){ $ts="forward Euler";}
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "pc4" ){ $ts="adams PC order 4"; }
if( $ts eq "mid"){ $ts="midpoint"; }  
if( $ts eq "afs"){ $ts="approximate factorization"; $newts = 1;}
if( $iv eq "full" ){ $iv = "useNewImplicitMethod\n use full implicit system 1\n implicitFullLinearized"; }else{ $iv="#"; }
if( $order eq "2" ){ $order = "second order accurate"; }elsif( $order eq "4" ){ $order = "fourth order accurate"; }else{ $order="#"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "*"; }
# 
# specify the overlapping grid to use:
$grid
# Specify the equations we solve:
  $model
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
#
  $tm
  # Define LES parameters that are accessed by getLargeEddySimulationViscosity.bf 
  define integer parameter lesOption $lesOption
  define real parameter lesPar1 $lesPar1
  exit
# Set the order of accuracy
$order
#
# Next specify the file to save the results in. 
# This file can be viewed with Overture/bin/plotStuff.
  show file options
     compressed
      open
       $show
    frequency to flush
      2
    exit
#   display parameters
  turn off twilight zone 
#
# choose the time stepping:
  $ts
  $newts
  # fullImplicitSystem=1: do not use multiple scalar systems for implicit solves, even if possible
  use full implicit system $fullImplicitSystem
  # -- for the AFS scheme:
  compact finite difference
  # -- convergence parameters for the af scheme
  max number of AF corrections $afit
  AF correction relative tol $aftol
  # optionally turn this on to improve stability of the high-order AF scheme by using 2nd-order dissipation at the boundary
  OBPDE:use boundary dissipation in AF scheme 1
#
# -- choose the implicit variation (implicitViscous is default)
$iv
# ---------
#
# but integrate the square explicitly:
  choose grids for implicit
    all=implicit
##    square=explicit
    done
  dtMax $dtMax
#
  final time $tFinal
  times to plot $tPlot
#  
  plot and always wait
#
  recompute dt every $recomputeDt
  refactor frequency $refactorFrequency
# 
  pde parameters
    nu $nu
    kThermal $kThermal
#  turn on 2nd-order AD here:
   OBPDE:second-order artificial diffusion $ad2
     OBPDE:ad21,ad22 $ad21 $ad22
#  turn on 4th-order AD here: 
   OBPDE:fourth-order artificial diffusion $ad4
     OBPDE:ad41,ad42 $ad41,$ad42
   OBPDE:expect inflow at outflow
   if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
   $cmd
#
   done
#
# 
  cfl $cfl
# 
# -- Here we assume that the grid was made with boundary conditions:
#   1=inflow, 2=outflow, 5=bodies
  boundary conditions
   all=noSlipWall
   bcNumber1=freeSurfaceBoundaryCondition, userDefinedBoundaryData
    pressure pulse
      1 1
   done
#
  done
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesPC=$pc; $ogesDebug=$pdebug;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtoli; $ogesAtol=$atoli; $ogesPC=$pc; $ogesDebug=$idebug;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
# initial conditions: uniform flow or restart from a solution in a show file 
if( $restart eq "" ){ $cmds = "uniform flow\n u=$uInflow, v=0., p=1., T=0."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
# 
#  plot residuals 1
  allow user defined output $userDefinedOutput
  if( $project eq "1"  && $restart eq "" ){ $cmd="project initial conditions"; }else{ $cmd="#"; }
  $cmd
  debug $debug 
  continue
#
$go


