#
# Cgins: free surface in 3D
#
# Usage:
#   
#  cgins [-noplot] freeSurface3d -g=<name> -pGrad=<f> -surfaceTension=<f> -tf=<tFinal> -tp=<tPlot> ...
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
# Examples: 
#  Grid: ogen -noplot freeSurfaceGrid3d -interp=e -factor=1
#  cgins freeSurface3d -g=freeSurfaceGrid3de1.order2.hdf -nu=.05 -tf=2. -tp=.01 -model=ins -surfaceTension=.0 -ad2=1 -go=halt 
#
# -- turn on gravity: 
#  cgins freeSurface3d -g=freeSurfaceGrid3de1.order2.hdf -nu=.05 -tf=2. -tp=.01 -model=ins -surfaceTension=.0 -pGrad=-5. -ad2=1 -go=halt 
#
# --- set default values for parameters ---
# 
$grid="halfCylinder.hdf"; $backGround="backGround"; $bcn="noSlipWall"; $pGrad=0.; 
$deformingGrid="share=100"; $deformFrequency=2.; $deformAmplitude=1.; $deformationType="advect body"; 
$tFinal=1.; $tPlot=.1; $cfl=.9; $nu=.05; $Prandtl=.72; $thermalExpansivity=.1; 
$gravity = "0. 0. 0."; 
$model="ins"; $ts="adams PC"; $noplot=""; $implicitVariation="full"; $refactorFrequency=100; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; 
$solver="best"; $rtol=1.e-4; $atol=1.e-6; $ogesDebug=0; $project=0; $cdv=1.; $ad2=0; $ad22=1.; 
$bc="a"; 
$surfaceTension=.1; $pAtmosphere=0.;
$smoothSurface=1; $numberOfSurfaceSmooths=3;
$freeSurfaceOption="none"; 
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"bcn=s"=>\$bcn,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
  "bc=s"=>\$bc,"dg=s"=>\$deformingGrid,"dt=s"=>\$deformationType,"da=f"=>\$deformAmplitude,"df=f"=>\$deformFrequency,\
  "surfaceTension=f"=>\$surfaceTension,"pAtmosphere=f"=>\$pAtmosphere,"pGrad=f"=>\$pGrad,\
  "smoothSurface=i"=>\$smoothSurface,"numberOfSurfaceSmooths=i"=>\$numberOfSurfaceSmooths,\
  "freeSurfaceOption=s"=>\$freeSurfaceOption );
# -------------------------------------------------------------------------------------------------
$kThermal=$nu/$Prandtl; 
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
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
  define real parameter kappa $kThermal
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
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
# 
#****************************
 turn on moving grids
  specify grids to move
    deforming body
      user defined deforming body
        $deformationType
        # turn on surface smoothing:
        smooth surface $smoothSurface
        number of surface smooths: $numberOfSurfaceSmooths
      done
      if( $deformingGrid =~ /^share=/ ){ $deformingGrid =~ s/^share=//; \
                 $deformingGrid="choose grids by share flag\n $deformingGrid"; };
      $deformingGrid
   done
 done
#**************************
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
if( $pGrad != 0 ){ $cmds ="user defined forcing\n constant forcing\n 3 $pGrad\n  done\n exit";}else{ $cmds="*"; }
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
  pressure solver options
     $solver
     relative tolerance
       $rtol
     absolute tolerance
       $atol
    exit
  implicit time step solver options
     $solver
     relative tolerance
       $rtol
     absolute tolerance
       $atol 
    exit
# 
  boundary conditions
    $u=.0; $T=1.; 
    # all=slipWall
    # annulus(0,1)=noSlipWall, uniform(T=$T)
    all=$bcn, uniform(T=$T)
    #     all=slipWall, uniform(T=$T)
    # $backGround=slipWall
    bcNumber6=freeSurfaceBoundaryCondition
# 
     # pressure pulse: p = .5*pMax*[ 1 - cos(2*pi*t/tMax) ],  for 0 <=t<=tMax, p=0 other-wise
    if( $freeSurfaceOption eq "tractionForce" ){ $cmd="bcNumber6=freeSurfaceBoundaryCondition, userDefinedBoundaryData\n pressure pulse\n   .1 1 \n  done"; }else{ $cmd="#"; }
    $cmd
# 
    bcNumber1=slipWall
    bcNumber2=slipWall
    bcNumber3=slipWall
    bcNumber4=slipWall
  done
# 
  initial conditions
   if( $tz eq "turn off twilight zone" ){ $ic = "uniform flow\n p=1., u=$u, T=0. \n done";}else{ $ic = "done"; }
   $ic 
  done
  debug $debug
  $project
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
#
 x-r 90
 set home
  $go


# 
      erase
      grid
        bigger:0
        exit this menu
