*
* cgins: Heated cylinder in a gravitational field (Boussinesq flow)
*       
*  cgins [-noplot] heatedCyl -g=<name> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -ts=<implicit> -debug=<num> ..,
*                     -nu=<num> -Prandtl=<num> -Twall=<> -bg=<backGround> -show=<name> -implicitFactor=<num> -bc=[wall|inflowOutflow]
* 
* Examples:
*    cgins heatedCyl -g=cice2.order2 -nu=.05 -tf=10. -tp=1. -ts=implicit -go=halt
*    cgins heatedCyl -g=cice4.order2 -nu=.01 -tf=10. -tp=1. -ts=implicit
*    srun -N1 -n1 -ppdebug $cginsp heatedCyl -g="cice2.order2.hdf" -nu=.05 -tf=10. -tp=1. -ts=implicit -solver=best
*   
#  -- inflow at bottom and outflow at the top:
#    cgins heatedCyl -g=cice2.order2 -nu=.05 -tf=10. -tp=1. -ts=implicit -bc=inflowOutflow
#
#  -- fourth-order
#    cgins heatedCyl -g=cice4.order4 -nu=.01 -tf=10. -tp=1. -ts=implicit -useNewImp=0 -go=halt
#    cgins heatedCyl -g=cice2.order4 -nu=.05 -tf=10. -tp=1. -ts=implicit -useNewImp=0 -go=halt
#    cgins heatedCyl -g=cice2.order4 -nu=.05 -tf=10. -tp=1. -ts=pc -useNewImp=0 -go=halt
*   --- flow inside a disk: 
*    cgins noplot heatedCyl -g="sice2.order2.hdf" -nu=.05 -tf=10. -tp=.1 -go=og -dtMax=.001
*    cgins noplot heatedCyl -g="sice2.order2.hdf" -nu=.05 -tf=10. -tp=.1 -ts=implicit -dtMax=.05 -go=og
*    cgins noplot heatedCyl -g="square5.hdf" -nu=.05 -tf=10. -tp=.1 -ts=implicit -dtMax=.01 -go=og
# -- MG:
#    cgins heatedCyl -g=cice2.order2.ml2 -nu=.05 -tf=10. -tp=1. -ts=implicit -bc=inflowOutflow -solver=mg -psolver=mg
#    cgins heatedCyl -g=cice2.order4.ml2 -nu=.05 -tf=10. -tp=1. -ts=implicit -bc=inflowOutflow -solver=mg -psolver=mg -useNewImp=0 -debug=3
* 
* --- set default values for parameters ---
* 
$grid="cic.hdf"; $ts="adams PC"; $noplot=""; $backGround="square"; $vIn=.1; $T0=0.; $cfl=.9; $go="halt"; 
$tFinal=20.; $tPlot=.1; $dtMax=.1; $degreeSpace=2; $degreeTime=2; $show=" "; 
$debug=1; $debugp=0; $debugi=0; $useNewImp=1;
$nu=.1; $Prandtl=.72; $thermalExpansivity=.1; $Twall=1.;  $implicitFactor=.5; $implicitVariation="viscous"; 
$tz=0; # turn on tz here
$gravity = "0 -10. 0."; $cdv=1.; $cDt=.25; 
$solver="yale";
$bc="wall";
$psolver="choose best iterative solver"; $solver="choose best iterative solver"; 
$iluLevels=1; $ogesDebug=0; 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
* 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"implicitFactor=f"=>\$implicitFactor, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "psolver=s"=>\$psolver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,\
  "debug=i"=>\$debug,"debugp=i"=>\$debugp,"debugi=i"=>\$debugi, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"Prandtl=f"=>\$Prandtl,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "noplot=s"=>\$noplot,\
 "go=s"=>\$go,"dtMax=f"=>\$dtMax,"cDt=f"=>\$cDt,"iv=s"=>\$implicitVariation,"useNewImp=i"=>\$useNewImp,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"Twall=f"=>\$Twall,"bc=s"=>\$bc );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
if( $ts eq "fe" ){ $ts="forward Euler";}
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "pc4" ){ $ts="adams PC order 4";  $useNewImp=0; } # NOTE: turn off new implicit for fourth order
if( $ts eq "mid"){ $ts="midpoint"; }  
# 
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;}
if( $useNewImp eq 1 ){ $useNewImp ="useNewImplicitMethod"; }else{ $useNewImp="#"; }
if( $implicitVariation eq "viscous" ){ $implicitVariation = "$useNewImp\n implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "$useNewImp\n implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "$useNewImp\n implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
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
* 
  choose grids for implicit
    all=implicit
   done
* 
*
  final time $tFinal
  times to plot $tPlot
  dtMax $dtMax
  cfl $cfl
#
  pde parameters
    nu  $nu
    kThermal $kThermal
    gravity
      $gravity
    OBPDE:divergence damping  $cdv 
    OBPDE:cDt div damping $cDt
   done
* 
  debug $debug
***
  show file options
    compressed
      open
      $show
    frequency to flush
      4
    exit
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogesDebug=$debugp; $ogmgDebug=$debugp; $ogmgCoarseGridSolver="best"; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; $ogesDebug=$debugi;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  boundary conditions
    all=noSlipWall
#     Annulus(0,1)=noSlipWall, uniform(T=$Twall)
#   -- heat flux BC:
#   Annulus(0,1)=noSlipWall, mixedDerivative(0.*t+1.*t.n=1.)
    # test user defined variable temperature BC:
##     Annulus(0,1)=noSlipWall, userDefinedBoundaryData
#    Annulus(0,1)=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.), userDefinedBoundaryData
#    variable temperature
#      -10. 10. 0.    
#    done
#
    Annulus(0,1)=noSlipWall, uniform(T=$Twall)
    * $backGround(1,1)=inflowWithVelocityGiven, uniform(p=1.,v=$vIn)
    * $backGround(0,1)=outflow
    if( $bc eq "inflowOutflow" ){ $cmds= "bcNumber3=inflowWithVelocityGiven, uniform(p=1.,v=$vIn)\n bcNumber4=outflow\n";}else{ $cmds="#"; }
    $cmds
  done
* 
  initial conditions
  if( $tz eq "0" ){ $commands="uniform flow\n" . "p=1., u=0., v=$vIn, T=$T0\n" . "continue"; }else{ $commands="continue";}
    $commands
* 
  continue
  plot:T
  $go
  
