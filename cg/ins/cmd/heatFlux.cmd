#==============================================================================================================
# cgins: test the evaluation of the heat flux using probes
#
# Usage:
#     cgins heatFlux -g=<grid> -nu=<> -ts=[im|pc] -solver=[best|yale|mg] ...
#                  -case=[strip2d|strip3d|square|box]|boxPoly] ...
#                  -psolver=[best|yale|mg] -ad2=[0|1] -model=[ins|boussinesq] ...
#                  -boundaryProbeMeasure=[average|integral]
#
# - case : 
#     strip2d : 2d strip (periodic in y)
#     strip3d : 3d strip  (periodic in z)
#     square  : 
#     box     : trig solution in a box
#     boxPoly : polynomial solution in a box
#
# Examples: 
#
# -- periodic strip in 2D: 
#   -- grids: 
#    ogen -noplot squareArg -periodic=np -order=2 -nx=32
#   -- IM22: 
#    ogen -noplot squareArg -periodic=np -order=2 -nx=32
#     cgins heatFlux -g=square32np.order2 -case=strip2d -ts=im -nu=.72 -tp=.1 -adcBoussinesq=0. -thermalConductivity=.1 -ad2=0 -go=halt
#   -- AFS22:
#    cgins -noplot heatFlux -g=square32np.order2 -case=strip2d -ts=afs -cfl=2. -nu=.72 -tf=10 -tp=.5 -adcBoussinesq=0. -thermalConductivity=.1 -ad2=0 -go=og
#   -- PC22: 
#     cgins heatFlux -g=square32np.order2 -case=strip2d -ts=pc -nu=.72 -tp=.1 -gravity="0. 0. 0." -ad2=0 -go=halt
# 
# -- 2D square:
#    cgins heatFlux -g=square32.order2 -case=square -ts=afs -dtMax=.05 -nu=.72 -tf=10 -tp=.5 -adcBoussinesq=0. -ad2=0 -thermalConductivity=.1 -go=halt 
# 
# -- 3D strip:
#    cgins heatFlux -g=box16.order2 -case=strip3d -ts=afs -dtMax=.05 -nu=.72 -tf=10 -tp=.5 -adcBoussinesq=0. -ad2=0 -thermalConductivity=.1 -go=halt 
# 
# -- 3D box:
#    cgins heatFlux -g=box16.order2 -case=box -ts=afs -dtMax=.05 -nu=.72 -tf=10 -tp=.5 -adcBoussinesq=0. -ad2=0 -thermalConductivity=.1 -go=halt 
#   
#==============================================================================================================
#
$grid="joukowsky2de4.order2"; $show = " "; $tFinal=10.; $tPlot=.1; $nu=.1; $cfl=.9;
$kThermal=-1.; $thermalExpansivity=.1; $thermalConductivity=.05; $adcBoussinesq=0.; $heatSource=1.; 
$pGrad=0.; $wing="wing"; $ad2=1; $model="boussinesq"; $newts=0; $case="square"; 
$gravity=" 0. 0. 0.";
# 
$implicitVariation="viscous"; $refactorFrequency=100; $impGrids="all=explicit"; $impFactor=.5; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.05; 
$order = 2; $fullSystem=0; $go="halt"; $move=1;  $moveOnly=0; $freq=1.; 
$show=" "; $restart="";  
# $outflowOption="neumann"; 
$outflowOption="extrapolate"; 
$psolver="yale"; $solver="yale"; 
$iluLevels=1; $ogesDebug=0; $project=0; 
$ts="im"; 
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$cdv=1.; $ad2=1; $ad22=2.; 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
# probes:
$boundaryProbeMeasure="average"; 
#
  $mx=1.; $my=1.; $mz=1.; $tc=4; 
# 
# -- for Kyle's AF scheme:
$afit = 10;
$aftol=1.e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; $ad4=4;
$cdv=1;  $cDt=.25;
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex,"degreet=i"=>\$degreet,"model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "psolver=s"=>\$psolver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,"ad2=i"=>\$ad2,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"move=i"=>\$move,"moveOnly=i"=>\$moveOnly,\
  "freq=f"=>\$freq,"outflowOption=s"=>\$outflowOption,"kThermal=f"=>\$kThermal,"gravity=s"=>\$gravity,\
  "thermalConductivity=f"=>\$thermalConductivity,"adcBoussinesq=f"=>\$adcBoussinesq,"newts=i"=>\$newts,"case=s"=>\$case,\
  "mx=i"=>\$mx, "my=i"=>\$my,"mz=i"=>\$mz,"heatSource=f"=>\$heatSource,\
   "boundaryProbeMeasure=s"=>\$boundaryProbeMeasure );
# -------------------------------------------------------------------------------------------------
if( $kThermal < 0 ){ $kThermal=$nu/.72; }
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
#
if( $case eq "box" || $case eq "strip3d" || $case eq "boxPoly" ){ $nd=3; }else{ $nd=2; }
# 
if( $ts eq "fe" ){ $ts="forward Euler";  }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       }
if( $ts eq "pc" ){ $ts="adams PC";       }
if( $ts eq "mid"){ $ts="midpoint";       }  
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;}
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $project eq "1" && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
#
$grid
# 
  $model
  $cmd="#";
  if( $moveOnly eq 1 ){ $cmd ="move and regenerate grids only"; }elsif( $moveOnly eq 2 ){ $cmd = "move grids only"; }
  $cmd
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq $adcBoussinesq
  exit
#
  show file options
   compressed
     OBPSF:maximum number of parallel sub-files 8
   open
     $show
    frequency to flush
      1
  exit  
#
  turn off twilight zone
# 
  final time $tFinal
  times to plot $tPlot
  plot and always wait
  cfl $cfl
# 
##  plot residuals 1
#
  $project
# 
#*  useNewImplicitMethod
#  implicitFullLinearized
  implicit factor $impFactor
  $newts
  dtMax $dtMax
# 
# use full implicit system 1
# use implicit time stepping
  $ts
  compact finite difference
  # -- convergence parameters for the af scheme
  max number of AF corrections $afit
  AF correction relative tol $aftol
  # optionally turn this on to improve stability of the high-order AF scheme by using 2nd-order dissipation at the boundary
  #  OBPDE:use boundary dissipation in AF scheme 1
#
  choose grids for implicit
    all=implicit
#    all=explicit
#    $impGrids
#    $wing=implicit
    done
  pde parameters
    nu $nu
    kThermal $kThermal
    thermal conductivity $thermalConductivity
    gravity
     $gravity
    #  turn on 2nd-order AD here:
    OBPDE:second-order artificial diffusion $ad2
     OBPDE:ad21,ad22 $ad22,$ad22
    if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
    $cmd
   done
#*
  # -- add body forcing 
  forcing options... 
    # Solution is T = (amp/alpha) * ( 1 - exp(-alpha*t) ) * sin( mx*pi*(x-x0) ) * sin( my*pi*(y-y0))
    # alpha =  pi^2 *( mx^2 + my^2 )
    $pi = 4.*atan2(1.,1.);
    if( $nd eq 2 ){ $mz=0.; $tc=3; }
    if( $nd eq 2 ){ $za=-.01; $zb=.01; }else{ $za=-.01; $zb=1.01; }
    $amp= $pi*$pi*( $mx*$mx + $my*$my + $mz*$mz ); # choose amp so max(T)=1
    $cmds=" "; # If cmds is not set below we will halt with a blank line
    # define a trig heat-source forcing 
    if( $case eq "square" || $case eq "box" ){ $cmds="user defined forcing...\n trigonmetric forcing\n $tc $amp $mx $my $mz 0. 0. 0.\n done"; }  
    # -- define a square region with a heat source
    if( $case eq "strip2d" || $case eq "strip3d" ){ $cmds="body forcing...\n heat coefficient: $heatSource\n box: -.01 1.01 -.01 1.01 $za $zb (xa,xb, ya,yb, za,zb)\n body forcing name: boxWithHeatSource\n add heat source\n"; }
    $ampPoly=$kThermal*2.*(4.**$nd); # exact solution is T = x(1-x) y(1-y) z(1-z) 
    if( $case eq "boxPoly" ){ $cmds="user defined forcing...\n polynomial forcing\n $tc $ampPoly\n done"; }  
    $cmds
  exit
#+  body forcing... 
#+    # -- define a square region with a heat source
#+    # $dragLinear=5.; $dragQuadratic=10.; 
#+    # drag coefficients: $dragLinear $dragQuadratic
#+    heat coefficient: 1.
#+    box
#+    box: -.01 1.01 -.01 1.01 -.01 .01 (xa,xb, ya,yb, za,zb)
#+    body forcing name: boxWithHeatSource
#+    add heat source
#+  exit
# 
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogesDtol=1.e5; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  boundary conditions
    all=noSlipWall
    # set the temperature to 0 on the left wall
# 
    bcNumber1=noSlipWall, variableBoundaryData
      box: -.01 .01 -.01 1.01 $za $zb (xa,xb, ya,yb, za,zb)
      temperature forcing: 0.
      boundary forcing name: leftWallFixedT
      add temperature forcing
    exit
    bcNumber2=noSlipWall, variableBoundaryData
      box: .99 1.01 -.01 1.011 $za $zb (xa,xb, ya,yb, za,zb)
      temperature forcing: 0.
      boundary forcing name: rightWallFixedT
      add temperature forcing
    exit
#
#+    bcNumber3=noSlipWall, variableBoundaryData
#+      box: -.01 1.01 -.01 .01 1 $za $zb (xa,xb, ya,yb, za,zb)
#+      temperature forcing: 0.
#+      boundary forcing name: bottomWallFixedT
#+      add temperature forcing
#+    exit
#+    bcNumber4=noSlipWall, variableBoundaryData
#+      box: -.01 1.01 .99 1.01  $za $zb (xa,xb, ya,yb, za,zb)
#+      temperature forcing: 0.
#+      boundary forcing name: topWallFixedT
#+      add temperature forcing
#+    exit
   done
#
#  Choose probes:
#
    frequency to save probes 2
#
    check probes...

#
#   -- here is a probe BOUNDARY region (save results in the first file)
    create a probe 
      probe name leftWallHeatFlux
      file name heatFluxProbe.dat
      boundary forcing region: leftWallFixedT
      heat flux
      # average
      $boundaryProbeMeasure
    exit
#
    create a probe 
      probe name rightWallHeatFlux
      file name heatFluxProbe.dat
      boundary forcing region: rightWallFixedT
      heat flux
      # average
      $boundaryProbeMeasure
    exit
#   -- here is a probe defined as an integral over a boundary surface ---
    create a probe
      probe name bottomWallFlux
      file name bottomFluxProbe.dat
      heat flux
      boundary region
      total
      integral
      #
      define surface...
        print valid grid faces
        define surface by grid faces
          # side axis grid
          0 0 0
        done
        exit
      exit
#   -- here is a location probe:
    create a probe
      probe name centerTemperature
      file name centerTemperatureProbe.dat
      all components
      location 0.51 .51  0 0
      exit
#
#+    create a probe 
#+      probe name bottomWallHeatFlux
#+      file name heatFluxProbe.dat
#+      boundary forcing region: bottomWallFixedT
#+      heat flux
#+      average
#+    exit
#+    create a probe 
#+      probe name topWallHeatFlux
#+      file name heatFluxProbe.dat
#+      boundary forcing region: topWallFixedT
#+      heat flux
#+      average
#+    exit
#   -- here is a probe that measures the average temperature
    create a probe 
      probe name averageT
      file name heatFluxProbe.dat
      # compute integral over the full volume:
      full domain region
      temperature 
      integral
    exit
#
  debug $debug
# initial conditions: uniform flow or restart from a solution in a show file 
if( $restart eq "" ){ $cmds = "uniform flow\n u=0., v=0., p=1., T=0."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
#
  continue
#
  plot:T
  contour
#    vertical scale factor 0.
  exit
#
$go 

