*
* cgins: Flow past a 3D cylinder in a channel (the cylinder is possibly heated)
*       
*  cgins [-noplot] cylinderInAChannel -g=<name> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -ts=<implicit> -debug=<num> ..,
*         -nu=<num> -Prandtl=<num> -bg=<backGround> -show=<name> -implicitFactor=<num> ...,
*         -solver=[best|yale|mg] -psolver=[best|yale|mg] -pc=[ilu|lu]
# Options:
#  -project : 1=project initial conditions
#  -solver : implicit solver
#  -psolver : pressure solver
* 
* NOTES:
*     - Variables are scaled by U=1m/s, L=1m, T=1K
*     - Coefficient of thermal expansion is 1/T (1/K) = 3.4e-3 (1/K) at 70F, 21C 
*     - nu = 1.5e-5 m^2/s
*     - Pr =.713
*     
# Examples:
#    cgins cylinderInAChannel -g=cylinderInAShortChannele1.order2 -nu=.05 -tf=10. -tp=.1 -ts=pc [OK
#    cgins cylinderInAChannel -g=cylinderInAShortChannele1.order2 -nu=.05 -tf=10. -tp=.1 -ts=im [OK
#  -- MG: 
#    cgins cylinderInAChannel -g=cylinderInAShortChannele1.order2.ml2 -nu=.05 -tf=10. -tp=.1 -ts=im -psolver=mg [OK
#    cgins cylinderInAChannel -g=cylinderInAShortChannele2.order2.ml2 -nu=.02 -tf=10. -tp=.1 -ts=im -psolver=mg [OK
#    -- periodic 
#    cgins cylinderInAChannel -g=cylinderInAChannele1.order2p.ml2 -nu=.05 -ad2=1 -tf=10. -tp=.1 -ts=im -psolver=mg -solver=mg [OK - ad2 needed
# -- order=4
#    OK but div large at some edges!
#    cgins cylinderInAChannel -g=cylinderInAShortChannele1.order4.ml2 -nu=.05 -tf=10. -tp=.1 -ts=pc4 -psolver=best
#    cgins cylinderInAChannel -g=cylinderInAShortChannele2.order4 -nu=.05 -tf=10. -tp=.1 -ts=pc4 -psolver=best
# 
# -- order=4 + MG
#    cgins cylinderInAChannel -g=cylinderInAChannele1.order4p.ml2 -nu=.05 -ad4=1 -tf=10. -tp=.1 -ts=im -psolver=mg -solver=mg -useNewImp=0 [OK
#    cgins cylinderInAChannel -g=cylinderInAShortChannele1.order4.ml2 -nu=.05 -tf=10. -tp=.01 -ts=pc4 -psolver=mg
#
#    cgins cylinderInAChannel -g=cylinderInAChannele2.order4.ml2 -nu=.05 -tf=10. -tp=.01 -ts=pc4 -psolver=mg
#    cgins cylinderInAChannel -g=cylinderInAShortChannele2.order4.ml2 -nu=.05 -tf=10. -tp=.1 -ts=pc4 -psolver=mg
#
# -- Box:
#   cgins cylinderInAChannel -g=box16.order4 -nu=.05 -tf=10. -tp=.01 -ts=pc4 -psolver=mg -p0=0.  [OK
#   cgins cylinderInAChannel -g=box16.order4 -nu=.05 -tf=10. -tp=.01 -ts=pc4 -psolver=mg -p0=1.  [OK
#   cgins cylinderInAChannel -g=box16.order4 -nu=.05 -tf=10. -tp=.01 -ts=pc4 -psolver=best -debug=3 -p0=1.
#
#    -- trouble with implicit system: row iwth zero coeff (insImp)
#    cgins cylinderInAChannel -g=cylinderInAShortChannele2.order4 -nu=.05 -tf=10. -tp=.1 -ts=im -useNewImp=0 -solver=best -psolver=best
#
* --- set default values for parameters ---
* 
$grid="cic.hdf"; $ts="adams PC"; $noplot=""; $backGround="square"; $uIn=1.; $v0=0.; $T0=0.; $p0=0.; $cfl=.9; $useNewImp=1;
$tFinal=20.; $tPlot=.1; $dtMax=.1; $degreeSpace=2; $degreeTime=2; $show=" "; $debug=1; $go="halt";
$nu=.1; $Prandtl=.72; $thermalExpansivity=3.4e-3; $Tin=-10.;  $implicitFactor=.5; $implicitVariation="viscous"; 
$tz=0; # turn on tz here
$ad2=0; $ad21=2.; $ad22=2.; $ad4=0; $ad41=2.; $ad42=2.; $outflowOption="neumann";
$gravity = "0 -10. 0."; $cdv=1.; $cDt=.25; $project=1; $restart=""; 
* $solver="choose best iterative solver";
$solver="yale";  $rtoli=1.e-5; $atoli=1.e-6; $idebug=0; 
$psolver="yale"; $rtolp=1.e-5; $atolp=1.e-6; $pdebug=0; $dtolp=1.e20; 
$pc="ilu"; $refactorFrequency=500; 
* 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"implicitFactor=f"=>\$implicitFactor, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"refactorFrequency=i"=>\$refactorFrequency, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"Prandtl=f"=>\$Prandtl,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "noplot=s"=>\$noplot,\
 "go=s"=>\$go,"dtMax=f"=>\$dtMax,"cDt=f"=>\$cDt,"iv=s"=>\$implicitVariation,"Tin=f"=>\$Tin,"ad2=i"=>\$ad2,\
 "solver=s"=>\$solver,"psolver=s"=>\$psolver,"pc=s"=>\$pc,"outflowOption=s"=>\$outflowOption,"ad4=i"=>\$ad4,\
 "debug=i"=>\$debug,"pdebug=i"=>\$pdebug,"idebug=i"=>\$idebug,"project=i"=>\$project,"cfl=f"=>\$cfl,\
 "restart=s"=>\$restart,"useNewImp=i"=>\$useNewImp,"p0=f"=>\$p0 );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $pc eq "ilu" ){ $pc = "incomplete LU preconditioner"; }elsif( $pc eq "lu" ){ $pc = "lu preconditioner"; }else{ $pc="#"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
if( $ts eq "fe" ){ $ts="forward Euler";}
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "pc4" ){ $ts="adams PC order 4"; $useNewImp=0; }
if( $useNewImp eq 1 ){ $useNewImp ="useNewImplicitMethod"; }else{ $useNewImp="#"; }
if( $implicitVariation eq "viscous" ){ $implicitVariation = "$useNewImp\n implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "$useNewImp\n implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "$useNewImp\n implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
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
##  Boussinesq model
*   define real parameter kappa $kThermal
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
  continue
* 
  OBTZ:polynomial
  OBTZ:twilight zone flow $tz
  degree in space $degreeSpace
  degree in time $degreeTime
* 
* choose time stepping method:
  $ts
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
   OBPDE:second-order artificial diffusion $ad2
   OBPDE:ad21,ad22 $ad21, $ad22
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
   $ogesDtol=$dtolp; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtoli; $ogesAtol=$atoli; $ogesPC=$pc; $ogesDebug=$idebug; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
 cfl $cfl
* 
  boundary conditions
    # all=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    all=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    #- all=noSlipWall , mixedDerivative(1.*t+1.*t.n=0.)
    # bcNumber1=inflowWithVelocityGiven, parabolic(p=1.,u=$uIn,T=$Tin)
    bcNumber1=inflowWithVelocityGiven, uniform(p=1.,u=$uIn,T=$Tin)
    bcNumber2=outflow , pressure(1.*p+.1*p.n=0.)
    # *wdh* 2014/08/31     bcNumber2=outflow
    bcNumber3=slipWall
    bcNumber4=slipWall
    bcNumber5=slipWall
    bcNumber6=slipWall
    # computer:
    # $Tcomputer=5.; 
    # bcNumber8=noSlipWall , uniform(p=1.,v=0.,T=$Tcomputer)
  done
* 
  if( $restart eq "" ){ $cmds = "uniform flow\n" . "p=$p0, u=$uIn, v=0, T=$T0\n"; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
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
  
