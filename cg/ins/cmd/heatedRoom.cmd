*
* cgins: Heated room (Boussinesq flow)
*       
*  cgins [-noplot] heatedRoom -g=<name> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -ts=<implicit> -debug=<num> ..
*         -nu=<num> -Prandtl=<num> -bg=<backGround> -show=<name> -implicitFactor=<num> ...
*         -solver=[best|yale|mg] -psolver=[best|yale|mg] -pc=[ilu|lu] ...
#         -inflow=[uniform|parabolic|control] -initialTime=<>
# Options:
#  -project : 1=project initial conditions
#  -solver : implicit solver
#  -psolver : pressure solver
#  -initialTime : if set, use this as the initial time for a RESTART.
* 
* NOTES:
*     - Variables are scaled by U=1m/s, L=1m, T=1K
*     - Coefficient of thermal expansion is 1/T (1/K) = 3.4e-3 (1/K) at 70F, 21C 
*     - nu = 1.5e-5 m^2/s
*     - Pr =.713
*     
* Examples:
*    cgins heatedRoom -g=room2de2.order2 -nu=.02 -tf=10. -tp=.1 -ts=im
*    cgins heatedRoom -g=room2de4.order2 -nu=.005 -tf=10. -tp=1. -ts=im
*    cgins -noplot heatedRoom -g=room2de8.order2 -nu=.001 -tf=100. -tp=5. -ts=im -show=heatedRoom.show -go=go >! heatedRoom.out
#  - restart example:
#    cgins -noplot heatedRoom -g=room2de2.order2 -nu=.02 -tf=.5 -tp=.1 -ts=im -show="hr2.show" -go=go
#    cgins -noplot heatedRoom -g=room2de2.order2 -nu=.02 -tf=1. -tp=.1 -ts=im -restart="hr2.show" -show="hr2a.show" -go=go
* 
# -- order=4
#    cgins heatedRoom -g=room2de4.order4 -nu=.002 -tf=100. -tp=.1 -ts=im -useNewImp=0 -go=halt
# 
* -- MG:
*    cgins heatedRoom -g=room2de4.order2.ml2 -nu=.02 -tf=10. -tp=.1 -ts=im -solver=mg -psolver=mg
*  
*    srun -N1 -n1 -ppdebug $cginsp heatedRoom -g="cice2.order2.hdf" -nu=.05 -tf=10. -tp=1. -ts=implicit -solver=best
* 
*    mpirun -np 2 $cginsp heatedRoom -g=room2de4.order2 -nu=.02 -tf=10. -tp=.1 -ts=im -solver=best -psolver=best
* 
* --- set default values for parameters ---
* 
$grid="cic.hdf"; $ts="adams PC"; $noplot=""; $backGround="square"; $vIn=-.5; $v0=0.; $T0=0.; $cfl=.9; $useNewImp=1;
$tFinal=20.; $tPlot=.1; $dtMax=.1; $degreeSpace=2; $degreeTime=2; $show=" "; $debug=1; $go="halt";
$nu=.1; $Prandtl=.72; $thermalExpansivity=3.4e-3; $Tin=-10.;  $implicitFactor=.5; $implicitVariation="viscous"; 
$tz=0; # turn on tz here
$outflowOption="neumann";
$ad2=1; $ad21=1.; $ad22=1.;  $ad4=0; $ad41=1.; $ad42=1.; $newts=0; 
$gravity = "0 -10. 0."; $cdv=1.; $cDt=.25; $project=0; $restart=""; $initialTime=""; 
* $solver="choose best iterative solver";
$solver="yale";  $rtoli=1.e-5; $atoli=1.e-6; $idebug=0; 
$psolver="yale"; $rtolp=1.e-5; $atolp=1.e-6; $pdebug=0;
$pc="ilu"; $refactorFrequency=500; 
#
$inflow="parabolic";
$Tset=0.; $Kp=1.; $Ki=1.; $Kd=0.; # for inflow control
# -- for Kyle's AF scheme:
$afit = 10;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$cdv=1;  $cDt=.25;
$ogmgAutoChoose=1;
* 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"implicitFactor=f"=>\$implicitFactor, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"refactorFrequency=i"=>\$refactorFrequency, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"Prandtl=f"=>\$Prandtl,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "noplot=s"=>\$noplot,\
 "go=s"=>\$go,"dtMax=f"=>\$dtMax,"cDt=f"=>\$cDt,"iv=s"=>\$implicitVariation,"Tin=f"=>\$Tin,\
 "solver=s"=>\$solver,"psolver=s"=>\$psolver,"pc=s"=>\$pc,"outflowOption=s"=>\$outflowOption,\
 "debug=i"=>\$debug,"pdebug=i"=>\$pdebug,"idebug=i"=>\$idebug,"project=i"=>\$project,"cfl=f"=>\$cfl,\
 "restart=s"=>\$restart,"initialTime=f"=>\$initialTime,"useNewImp=i"=>\$useNewImp,"aftol=f"=>\$aftol,\
 "inflow=s"=>\$inflow,"Tset=f"=>\$Tset,"Kp=f"=>\$Kp,"Ki=f"=>\$Ki,"Kd=f"=>\$Kd,"vIn=f"=>\$vIn,"T0=f"=>\$T0,\
 "ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,\
  "ogmgAutoChoose=i"=>\$ogmgAutoChoose );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $pc eq "ilu" ){ $pc = "incomplete LU preconditioner"; }elsif( $pc eq "lu" ){ $pc = "lu preconditioner"; }else{ $pc="#"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
if( $useNewImp eq 1 ){ $useNewImp ="useNewImplicitMethod"; }else{ $useNewImp="#"; }
if( $implicitVariation eq "viscous" ){ $implicitVariation = "$useNewImp\n implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "$useNewImp\n implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "$useNewImp\n implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $ts eq "fe" ){ $ts="forward Euler";}
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "pc4" ){ $ts="adams PC order 4"; }
if( $ts eq "afs"){ $ts="approximate factorization"; $newts = 1;}
#
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
if( $project eq "1" && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
*
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
$kThermal=$nu/$Prandtl;
*
* specify the grid: 
$grid
* 
  incompressible Navier Stokes
  Boussinesq model
*   define real parameter kappa $kThermal
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
  continue
* 
  OBTZ:twilight zone flow $tz
  OBTZ:polynomial
  degree in space $degreeSpace
  degree in time $degreeTime
* 
* choose time stepping method:
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
*   
  * number of PC corrections 5
  $implicitVariation
  implicit factor $implicitFactor 
  refactor frequency $refactorFrequency
* 
  choose grids for implicit
    all=implicit
   done
* 
*
  final time $tFinal
  times to plot $tPlot
  dtMax $dtMax
  pde parameters
    nu  $nu
    kThermal $kThermal
    gravity
      $gravity
    OBPDE:divergence damping  $cdv 
    OBPDE:cDt div damping $cDt
    #  turn on 2nd-order AD here:
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22 $ad21,$ad22
    #  turn on 4th-order AD here:
    OBPDE:fourth-order artificial diffusion $ad4
    OBPDE:ad41,ad42 $ad41, $ad42
    # MG solver currently wants a Neumann BC at outflow
    if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
     $cmd
   done
* 
  debug $debug
***
  show file options
    compressed
     OBPSF:maximum number of parallel sub-files 8
      open
      $show
    frequency to flush
      4
    exit
***
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesPC=$pc; $ogesDebug=$pdebug; $ogmgDebug=$pdebug;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtoli; $ogesAtol=$atoli; $ogesPC=$pc; $ogesDebug=$idebug; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
 cfl $cfl
# -- define controls:
  if( $inflow eq "control" ){ $cmd = "controls...\nPID\n targetComponent: 3\n Kp, Ki, Kd: $Kp,$Ki,$Kd (PID gains)\n Set point: $Tset\n exit"; }else{ $cmd="#"; }
  $cmd
#
  boundary conditions
    all=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    #- all=noSlipWall , mixedDerivative(1.*t+1.*t.n=0.)
    ## bcNumber5=inflowWithVelocityGiven, uniform(p=1.,v=$vIn,T=$Tin)
    ## bcNumber5=inflowWithVelocityGiven, parabolic(p=1.,v=$vIn,T=$Tin)
    if( $inflow eq "uniform" ){ $cmd = "bcNumber5=inflowWithVelocityGiven, uniform(p=1.,v=$vIn,T=$Tin)"; }\
    elsif( $inflow eq "parabolic" ){ $cmd = "bcNumber5=inflowWithVelocityGiven, parabolic(p=1.,v=$vIn,T=$Tin)"; }\
    elsif( $inflow eq "control" ){ $cmd = "bcNumber5=inflowWithVelocityGiven, userDefinedBoundaryData\n" . \
               "inflow with control\n 0. $vIn 0.\n done\n" . \
               "inflow with control\n 0. $vIn 0.\n done\n" . \
               "inflow with control\n 0. $vIn 0.\n done"; }
    else{ $cmd = "bcNumber5=inflowWithVelocityGiven, uniform(p=1.,v=$vIn,T=$Tin)"; } 
    $cmd
    bcNumber6=outflow
    # computer:
    $Tcomputer=5.; 
    bcNumber8=noSlipWall , uniform(p=1.,v=0.,T=$Tcomputer)
  done
* 
  if( $initialTime ne "" ){ $initialTime = "initial time $initialTime"; }
  if( $restart eq "" ){ $cmds = "uniform flow\n" . "p=1., u=0., v=$v0, T=$T0\n"; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file\n $initialTime"; }
#
  initial conditions
    $cmds
  exit
* 
  $project
  continue
  plot:T
* 
  $go
  
