* =============================================================================================
* cgins: Sliding plug in a channel for testing cgins and implicit time stepping
* 
* Usage:
*   
*  cgins [-noplot] slider -g=<name> -tf=<tFinal> -tp=<tPlot> -nu=<> -cfl=<> -rate=<> -ramp=<> -ts=<> ...
*                         -predictorOrder=[1|2] -vShift=<val> -go=[run/halt/og]
* 
*  -rate : rotate rate, rotations per second
*  -ramp : slow start ramp time 
*  -ts : "adams PC" or "implicit" 
*  -go : run, halt, og=open graphics
* 
* Examples: 
* 
*  cgins slider -g=slideri2.order2.hdf -nu=.05 -tf=1. -tp=.05 -impGrids="all=implicit" -go=halt 
* -- tz: 
*  cgins slider -g=slideri2.order2.hdf -nu=.05 -tf=1. -tp=.05 -impGrids="all=implicit" -tz=poly -degreex=1 -degreet=1  -predictorOrder=1 -go=halt [exact]
* explicit: 
*  cgins slider -g=slideri2.order2.hdf -nu=.05 -tf=1. -tp=.05 -impGrids="all=implicit" -tz=poly -degreex=1 -degreet=0 -go=halt -ts="adams PC"  -predictorOrder=1 [exact]
* =============================================================================================
* 
* --- set default values for parameters ---
$tFinal=.5; $tPlot=.025; $show = " "; $vShift=1.; 
$rate=.5; $ramp=0.; $nu=.05; $cfl=.9; $dtMax=.1; $ts="implicit"; $impGrids="all=explicit"; 
$psolver="yale"; $solver="yale"; $iluLevels=1; $rtol=1.e-5; $atol=1.e-6; 
$tz="none"; $degreex=2; $degreet=2; $project=1; $bcn="#"; $predictorOrder=2; $fullGridGenFreq=10;
* 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,\
 "iv=s"=>\$implicitVariation,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor, "bcn=s"=>\$bcn,\
  "bc=s"=>\$bc,"rate=f"=>\$rate,"ramp=f"=>\$ramp,"impGrids=s"=>\$impGrids,\
  "degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "predictorOrder=i"=>\$predictorOrder,"vShift=f"=>\$vShift,\
  "fullGridGenFreq=i"=>\$fullGridGenFreq );
* -------------------------------------------------------------------------------------------------
*
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; $project=0; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.;  $project=0; }
* 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
* 
$grid
* 
  incompressible Navier Stokes
  * simulate grid motion only 1
  exit
  show file options
    * compressed
    open
     $show
    frequency to flush
      5
    exit
* 
  $tz
  degree in space $degreex
  degree in time $degreet
* 
*  project initial conditions
if( $project eq "1" ){ $projectCmd = "project initial conditions"; }else{ $projectCmd="#"; }
$projectCmd
*
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  dtMax $dtMax
  plot and always wait
  * no plotting
* 
 turn on moving grids
* ---
 frequency for full grid gen update $fullGridGenFreq
* 
  specify grids to move
    translate
      1. 0. 0.
      $vShift
    slider
    done
  done
* -- specify the time stepping method
  $ts 
  * 
  if( $predictorOrder eq 1 ){ $predictorOrder = "first order predictor"; }else{ $predictorOrder="#"; }
  $predictorOrder
  * for implicit time stepping
  choose grids for implicit
    * all=implicit
    * all=explicit
    $impGrids
     * backGround=implicit
     slider=implicit
    done
  pressure solver options
     $psolver
     * yale
     * these tolerances are chosen for PETSc
     number of incomplete LU levels
       $iluLevels
     * do these next two work?
     define petscOption -pc_factor_levels $iluLevels
     define petscOption -sub_pc_factor_levels $iluLevels
     relative tolerance
       $rtol 
     absolute tolerance
       $atol
    exit
  implicit time step solver options
     * choose best iterative solver
     $solver
     * 
     number of incomplete LU levels
       $iluLevels
     define petscOption -pc_factor_levels $iluLevels
     define petscOption -sub_pc_factor_levels $iluLevels
     * these tolerances are chosen for PETSc
     relative tolerance
       $rtol
     absolute tolerance
       $atol
     * debug 
     *   3
    exit
* 
  pde parameters
    nu $nu
  done
  debug $debug
  boundary conditions
     all=noSlipWall
     * backGround(0,0)=inflowWithVelocityGiven, parabolic(d=$d,p=1.,u=0.)
     backGround(0,0)=inflowWithPressureAndTangentialVelocityGiven uniform(p=1.)
     * backGround(1,0)=outflow , pressure(1.*p+1.*p.n=1.)
*      backGround(0,1)=slipWall
     backGround(1,1)=noSlipWall, uniform(u=1,v=0)
     backGround(0,1)=noSlipWall, uniform(u=1,v=0)
*
*      all=dirichletBoundaryCondition
     $bcn
    done
$icCmds = "initial conditions\n uniform flow\n p=1.\n exit";
if( $tz ne "turn off twilight zone" ){ $icCmds="#"; }
$icCmds
* 
  continue
      plot:u
      contour
        ghost lines 1
        wire frame (toggle)
        exit
  $go
