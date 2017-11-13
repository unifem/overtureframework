#
# Cgins: Oscillating bubble under surface tension -- compare to na exact solution
#
# Usage:
#   
#  cgins [-noplot] oscillatingBubble -g=<name> -pGrad=<f> -surfaceTension=<f> -tf=<tFinal> -tp=<tPlot> ...
#        -solver=<yale/best> -order=<2/4> -model=<ins/boussinesq> -ts=<implicit> -debug=<num> ..,
#        -ad2=<0|1> -project=<0/1> -iv=[viscous/adv/full] -imp=<val> -rf=<val> ...
#        -smoothSurface=[0|1] -numberOfSurfaceSmooths=<i> -freeSurfaceOption=[none|tractionForce]
#        -go=[run/halt/og]
# 
#  -surfaceTension : surface tension coefficient
#  -pAtmosphere : atmosphere pressure
#  -iv : implicit variation : viscous=viscous terms implicit, adv=viscous + advection, full=full linearized version
#  -imp : .5=CN, 1.=BE, 0.=FE
#  -rf : refactor frequency
#  -go : run, halt, og=open graphics
#  -ad2 : turn on or off the 2nd order artificial dissipation 
#  -dg : the name of thee grid to deform or -dg="share=<num>" to choose all grids with a given share value 
#  -df, -da : deformation frequency and amplitude. 
# 
# Examples: (Grid from freeSurfaceGrid2d.cmd)
# 
#  cgins oscillatingBubble -g=oscillatingBubbleGride2.order2 -dg="share=100" -nu=.05 -tf=2. -tp=.01 -model=ins -go=halt 
#
# --- set default values for parameters ---
# 
$grid="oscillatingBubbleGride2.order2.hdf"; $backGround="backGround"; $bcn="noSlipWall"; $pGrad=0.; 
$deformationType="free surface"; 
$deformingGrid="share=100"; # deforming grid is identified by a share value of 100 
# 
$tFinal=1.; $tPlot=.1; $cfl=.9; $nu=.05; $Prandtl=.72; $thermalExpansivity=.1; 
$gravity = "0. 0. 0."; 
$model="ins"; $ts="adams PC"; $noplot=""; $implicitVariation="viscous"; $refactorFrequency=100; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.05; 
$order = 2; $fullSystem=0; $go="halt"; 
$ogesDebug=0; $project=0; $cdv=1.; $ad2=0; $ad22=2.; 
$projectForMoving=0;
$psolver="yale"; $solver="yale"; 
$iluLevels=1; $ogesDebug=0; 
$rtolp=1.e-4; $atolp=1.e-5;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
$surfaceTension=.1; $pAtmosphere=0.;
$smoothSurface=0; $numberOfSurfaceSmooths=3;
$freeSurfaceOption="none"; 
# Decouple implicit BCs (e.g. free surface) so we can solve scalar velociity implicit equations
$decoupleImplicitBoundaryConditions=0;
# 
$amp=1; $casenumber=3;
$predictorOrder=0; # 0=use default 
$useNewTimeSteppingStartup=1;  # this will regenerate past time grids and solutions
$surfacePredictor="leap-frog";
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "psolver=s"=>\$psolver,"tz=s"=>\$tz, "show=s"=>\$show,\
  "order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"bcn=s"=>\$bcn,\
 "iv=s"=>\$implicitVariation,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
  "dg=s"=>\$deformingGrid,"dt=s"=>\$deformationType,\
  "surfaceTension=f"=>\$surfaceTension,"pAtmosphere=f"=>\$pAtmosphere,"pGrad=f"=>\$pGrad,\
  "smoothSurface=i"=>\$smoothSurface,"numberOfSurfaceSmooths=i"=>\$numberOfSurfaceSmooths,\
  "freeSurfaceOption=s"=>\$freeSurfaceOption,"rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,\
  "amp=f"=>\$amp,"casenumber=i"=>\$casenumber,"projectForMoving=i"=>\$projectForMoving,\
  "useNewTimeSteppingStartup=i"=>\$useNewTimeSteppingStartup,\
  "decoupleImplicitBoundaryConditions=i"=>\$decoupleImplicitBoundaryConditions,\
  "surfacePredictor=s"=>\$surfacePredictor );
# -------------------------------------------------------------------------------------------------
$kThermal=$nu/$Prandtl; 
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
#
if( $ts eq "fe" ){ $ts="forward Euler";}
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;}
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
# 
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $project eq "1" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# specify the overlapping grid to use:
$grid
# Specify the equations we solve:
  $model
  # define real parameter kappa $kThermal
  # define real parameter thermalExpansivity $thermalExpansivity
  # define real parameter adcBoussinesq 0. 
  # Here is the surface tension
  define real parameter surfaceTension $surfaceTension
  define real parameter pAtmosphere $pAtmosphere
  done
# 
  show file options
    compressed
    open
      $show
    frequency to flush
      5 
    exit
# -- twilightzone options:
  $tz
  degree in space $degreex
  degree in time $degreet
  frequencies (x,y,z,t)   $fx $fy $fz $ft
# 
# choose the time stepping:
  $ts
# -- testing: 
$useNewTimeSteppingStartup=1; 
use new time-stepping startup $useNewTimeSteppingStartup
# 
#**************************** START: DEFINE THE DEFORMING GRID *********************
$generatePastHistory=1; # fix this for "advect-body" motion
$numberOfPastTimeLevels=3; 
$gridEvolutionVelocityAccuracy=2; 
$gridEvolutionAccelerationAccuracy=2; 
if( $tz eq "turn off twilight zone" ){ $useKnown=1; }else{ $useKnown=0; }
# 
 turn on moving grids
  specify grids to move
    deforming body
      user defined deforming body
        $deformationType
         debug
            $debug
        velocity order of accuracy\n $gridEvolutionVelocityAccuracy
        acceleration order of accuracy\n $gridEvolutionAccelerationAccuracy
        generate past history $generatePastHistory
        #use known solution for initial conditions $useKnown
	#number of past time levels: $numberOfPastTimeLevels
        # optionally turn on surface smoothing:
        smooth surface $smoothSurface
	free surface predictor $surfacePredictor
        use known solution for initial conditions $useKnown
        number of surface smooths: $numberOfSurfaceSmooths
	past time dt: $tPlot
      done
      if( $deformingGrid =~ /^share=/ ){ $deformingGrid =~ s/^share=//; \
                 $deformingGrid="choose grids by share flag\n $deformingGrid"; };
      $deformingGrid
   done
 done
#************************* END: DEFINE THE DEFORMING GRID *********************
##  useNewImplicitMethod
  $implicitVariation
  refactor frequency $refactorFrequency
  choose grids for implicit
    all=implicit
#     square=explicit
    done
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  dtMax $dtMax
# 
# 
  plot and always wait
 # no plotting
#
# Here is where we turn on gravity as a constant pressure gradient in the  negative y direction : 
if( $pGrad != 0 ){ $cmds ="user defined forcing\n constant forcing\n 2 $pGrad\n  done\n exit";}else{ $cmds="*"; }
$cmds
#
  pde parameters
    nu $nu
    kThermal $kThermal
    gravity
      $gravity
# 
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22  $ad22, $ad22
    OBPDE:divergence damping  $cdv 
  done
# Decouple implicit BCs (e.g. free surface) so we can solve scalar velociity implicit equations
  OBPDE:decouple implicit boundary conditions $decoupleImplicitBoundaryConditions
#
  maximum number of iterations for implicit interpolation
     10 
#***************************************************
#
  # turn off echo of command file to the terminal:
  echo to terminal 0
  pressure solver options
   # $ogesDebug=$debug; 
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogesDtol=1e20; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; $ogesIluLevels=1; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
  echo to terminal 1
#
#***************************************************
# 
  boundary conditions
    $u=.0; $T=1.; 
    all=$bcn
    #
    $cmd
    bcNumber1=slipWall
    bcNumber2=slipWall
    bcNumber3=slipWall
    # bcNumber3=inflowWithPressureAndTangentialVelocityGiven, uniform(p=0.)
    bcNumber4=freeSurfaceBoundaryCondition
    #bcNumber4=dirichletBoundaryCondition
  done
# 
  initial conditions
   # ****** DEFINE THE KNOWN SOLUTION ******
   OBTZ:user defined known solution
     oscillating bubble
      $amp,$casenumber
    done
  done
  debug $debug
  $project
  project initial conditions for moving grids $projectForMoving 
#
#-    output options...
#-    frequency to save probes 2
#-    create a probe
#-      file name probeFile.dat
#-      nearest grid point to -.5 .1 0.
#-      exit
#-    close output options
#
  exit
  $go


# 
      erase
      grid
        bigger:0
        exit this menu
