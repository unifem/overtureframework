#===============================================================
# cgins: test the user defined forcing
# Usage:
#     cgins bodyForcing -g=<grid> -nu=<> -ts=[im|pc] -move=[0|1] -moveOnly=[0|1|2] -freq=<> -solver=[best|yale|mg] ...
#                  -psolver=[best|yale|mg] -ad2=[0|1] -model=[ins|boussinesq]
#
#  moveOnly :  0=normal run, 1=move grids and call ogen, 2=move grids, no ogen.
#
# Examples: 
#    cgins bodyForcing -g=square32.order2 -model=boussinesq -nu=1.e-2 -tp=.01 -go=halt
#    cgins bodyForcing -g=square64.order2 -model=boussinesq -nu=1.e-2 -tp=.1 -go=halt
#    cgins bodyForcing -g=square100.order2 -model=boussinesq -nu=1.e-3 -tp=.01 -go=halt -psolver=mg -solver=mg -outflowOption=neumann -debug=3  
#    cgins bodyForcing -g=square400.order2 -model=boussinesq -nu=1.e-3 -tp=.01 -go=halt -psolver=mg -solver=mg -outflowOption=neumann -debug=3  
#    cgins bodyForcing -g=square1000.order2 -model=boussinesq -nu=1.e-3 -tp=.01 -go=halt -psolver=mg -solver=mg -outflowOption=neumann -debug=3  
#    cgins bodyForcing -g=channel5.order2.hdf -model=boussinesq -nu=1.e-2 -tp=.01 -go=halt
#
# --3D:
#    cgins bodyForcing -g=box32.order2 -model=boussinesq -nu=1.e-2 -tp=.01  -psolver=mg -solver=mg -outflowOption=neumann -go=halt -debug=3
#    cgins bodyForcing -g=box64.order2 -model=boussinesq -nu=1.e-2 -tp=.01  -psolver=mg -solver=mg -outflowOption=neumann -go=halt -debug=3
#    - 1M pts, mem=.32Gb : 
#    cgins bodyForcing -g=box100.order2 -model=boussinesq -nu=5.e-3 -tp=.05 -ts=pc -psolver=mg -solver=mg -outflowOption=neumann -go=halt -debug=3
#
#  -- trouble here: MG coarse grid solver has trouble -- is the coarse grid too coarse?
#    cgins bodyForcing -g=square128.order2 -nu=1.e-3 -tp=.01 -go=halt -psolver=mg -solver=mg -outflowOption=neumann -debug=3  
#
# === Boussinesq 
#    cgins bodyForcing -g=square64.order2 -model=boussinesq -nu=1.e-2 -tp=.1 -go=halt
#===============================================================
#
$grid="joukowsky2de4.order2"; $show = " "; $tFinal=5.; $tPlot=.1; $nu=.1; $cfl=.9;
$kThermal=-1.; $thermalExpansivity=.1; $thermalConductivity=.05; 
$pGrad=0.; $wing="wing"; $ad2=1; $model="boussinesq";
$gravity=" 0. -1. 0.";
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
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "psolver=s"=>\$psolver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,"ad2=i"=>\$ad2,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"move=i"=>\$move,"moveOnly=i"=>\$moveOnly,\
  "freq=f"=>\$freq,"outflowOption=s"=>\$outflowOption,"kThermal=f"=>\$kThermal );
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
if( $ts eq "fe" ){ $ts="forward Euler";  }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       }
if( $ts eq "pc" ){ $ts="adams PC";       }
if( $ts eq "mid"){ $ts="midpoint";       }  
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
  define real parameter adcBoussinesq 0. 
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
# 
  plot residuals 1
#
  $project
# 
#*  useNewImplicitMethod
#  implicitFullLinearized
  implicit factor $impFactor
  dtMax $dtMax
# 
# use full implicit system 1
# use implicit time stepping
  $ts
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

  body forcing... 
    # -- define a square region where there is a drag and heat source:
    $dragLinear=5.; $dragQuadratic=10.; 
    drag coefficients: $dragLinear $dragQuadratic
    heat coefficient: 1
    box
    box: .2 .4 .2 .4 -.01 .01 (xa,xb, ya,yb, za,zb)
    body forcing name: boxWithDrag
    add drag force
    body forcing name: boxWithHeatSource
    add heat source
    # define a temperature variation from a set a data points:
    define temperature time variation...
       mapping function
         2
         0 0
         1 2
      exit
    # change the material properties: (finish me!)
#-    piecewise constant material
#-     rho 2.2
#-     Cp 3.3
#-     thermalConductivity 4.4
#-     set material properties
#
#   -- define an ellipse where there is an immersed boundary
    ellipse
    ellipse: .25 .125 1.  .5 .75 0. (a,b,c, x0,y0,z0)
    region lines: 40 10 5
    # Here is the velocity on the inside of the immersed body:
    body velocity: 0. -.1 0.
    body forcing name: ellipseImmersedBoundary
    add immersed boundary
    body forcing name: ellipseHeatSource
    add heat source
    # -- store material properties in less efficient way: 
#-     variable material
#-     rho .66
#-     Cp  1.25
#-     thermalConductivity 2.2
#-     set material properties
#
  exit
# 
# Here is were we specify the user defined forcing:
#- user defined forcing
#-   drag forcing
#- exit
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
#123    all=slipWall
    all=noSlipWall
    # This next works: 
    # bcNumber4=noSlipWall, uniform(u=0.,v=-1.)
# 
#    bcNumber1=inflowWithVelocityGiven, uniform(u=1.,T=0.)
     # Define two overlapping inflow regions -- the forcings add together     
     bcNumber1=inflowWithVelocityGiven, variableBoundaryData
       # set the region to be a box:
       box
       box: -.01 .01 .25 .75 -.01 .01 (xa,xb, ya,yb, za,zb)
       velocity forcing: 1. 0. 0.
       boundary forcing name: inflowVelocity
       add velocity forcing
       temperature forcing: .5
       boundary forcing name: inflowTemperature
       add temperature forcing
       #
       # define a temperature variation from a set a data points:
       # define temperature time variation...
       #   mapping function
       #     2
       #     0 1
       #     1 0
       #  exit
       #
       # -- add a second infow that ADDS to the first --
       #2 box: -.01 .01 .40 .60 -.01 .01 (xa,xb, ya,yb, za,zb)
       #2 velocity forcing: .5 0. 0.
       #2 add velocity forcing
       #2 temperature forcing: .25
       #2 add temperature forcing
     exit
    $d=.5; 
#    backGround(0,0)=inflowWithVelocityGiven, parabolic(d=$d,p=1.,u=1.)
#    backGround(0,0)=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)
     bcNumber2=outflow
#
#     -- local inflow on a wall:
    # OK using inflow: 
    ## bcNumber3=inflowWithVelocityGiven, variableBoundaryData
    # -- This next needs to be fixed:
     bcNumber3=noSlipWall, variableBoundaryData
      box: .2 .6 -.01 .01 -.01 .01 (xa,xb, ya,yb, za,zb)
      velocity forcing: 0. 1. 0.
      # define a parabolic inflow profile
      parabolic forcing profile
      parabolic depth: 0.1
      # 
      # Here is a tanh profile:
      tanh forcing profile
      tanh exponent: 40.
      # now create the velocity forcing
      add velocity forcing
      boundary forcing name: inflowTemperatureBottom
      temperature forcing: 1.
      add temperature forcing
     exit
     #
     #     -- top wall: local heat flux on an isothermal wall ---
     #
     bcNumber4=noSlipWall, variableBoundaryData
       # set the region to be a box:
       box
       box: .1 .3 .99 1.01 -.01 .01 (xa,xb, ya,yb, za,zb)
       temperature BC is constant coefficients
       # This looks like a dirichlet BC -- maybe Neumann not implemented yet??
       a0, an: 0, 1. (a0*T+an*T.n=)
       set temperature BC
       temperature forcing: 1.
       add temperature forcing
     exit
     #
     #     -- top wall: local specified T on an isothermal wall ---
     #
     bcNumber4=noSlipWall, variableBoundaryData
       # set the region to be a box:
       box
       box: .6 .8 .99 1.01 -.01 .01 (xa,xb, ya,yb, za,zb)
       temperature BC is constant coefficients
       # This looks like a dirichlet BC -- maybe Neumann not implemented yet??
       a0, an: 1, 0. (a0*T+an*T.n=)
       set temperature BC
       temperature forcing: -1.
       boundary forcing name: topWallFixedT
       add temperature forcing
     exit
#
#      
#     -- local heat flux on a wall:
#1    # bcNumber3=noSlipWall, variableBoundaryData
#1    bcNumber3=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.), variableBoundaryData
#1      box
#1      box: .25 .75 -.01 .01 -.01 .01 (xa,xb, ya,yb, za,zb)
#1      temperature forcing: 20.
#1      add temperature forcing
#1     #  velocity forcing: 0. 1. 0.
#1     #  add velocity forcing
#1    exit
#-
#
   done
#
#  Choose probes:
#
    frequency to save probes 2
#
#   -- create a probe that measures the average temperture in a region --
    create a probe 
      probe name boxTemp
      file name probeBoxWithDrag.dat
      body forcing region: boxWithDrag
      temperature
      average
    exit
#
#   -- here is another probe region (save results in the same file)
    create a probe 
      probe name boxTemp2
      file name probeBoxWithDrag.dat
      body forcing region: boxWithDrag
      temperature
      average
    exit
#
#   -- here is another probe region (save results in the a new file)
    create a probe 
      probe name boxTemp3
      file name probeBoxWithDrag2.dat
      body forcing region: boxWithDrag
      temperature
      average
    exit
#   -- here is a probe BOUNDARY region (save results in the first file)
    create a probe 
      probe name inflowTemp
      file name probeBoxWithDrag.dat
      boundary forcing region: inflowTemperature
      temperature
      average
    exit
#   -- here is a probe BOUNDARY region (save results in the first file)
    create a probe 
      probe name topWallRightHeatFlux
      file name probeBoxWithDrag.dat
      boundary forcing region: topWallFixedT
      heat flux
      average
    exit
#   -- here is a probe that measures the average temperature
    create a probe 
      probe name averageT
      file name probeBoxWithDrag.dat
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
    vertical scale factor 0.
    exit
#
$go 

