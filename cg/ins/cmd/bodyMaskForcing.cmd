#===============================================================
# cgins: test the body forcing from a mask option
# Usage:
#     cgins bodyMaskForcing -g=<grid> -uInflow=<> -nu=<> -ts=[im|pc|afs] -move=[0|1] -moveOnly=[0|1|2] -freq=<>  ...
#                  -solver=[best|yale|mg] -psolver=[best|yale|mg] -ad2=[0|1] -model=[ins|boussinesq]
#
#  moveOnly :  0=normal run, 1=move grids and call ogen, 2=move grids, no ogen.
#
# Examples: 
#    cgins bodyMaskForcing -g=square40.order2 -nu=1.e-2 -tp=.01 -go=halt
# 
# NOTE: 1m/s = 2.24 mph
#===============================================================
#
$grid="joukowsky2de4.order2"; $show = " "; $tFinal=5.; $tPlot=.1; $nu=.1; $cfl=.9; $uInflow=1.; 
$kThermal=-1.; $thermalExpansivity=.1; $thermalConductivity=.05; $newts=0;
$pGrad=0.; $wing="wing"; $ad2=1; $model="ins";
$gravity=" 0. -1. 0.";
$bodyMaskShowFile="bodyMask.show";
# 
$implicitVariation="viscous"; $refactorFrequency=100; $impGrids="all=explicit"; $impFactor=.5; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=10.; 
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
# -- for Kyle's AF scheme:
$afit = 10;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$cdv=1;  $cDt=.25;
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "psolver=s"=>\$psolver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,"ad2=i"=>\$ad2,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"move=i"=>\$move,"moveOnly=i"=>\$moveOnly,\
  "freq=f"=>\$freq,"outflowOption=s"=>\$outflowOption,"kThermal=f"=>\$kThermal,\
  "bodyMaskShowFile=s"=>\$bodyMaskShowFile,"newts=i"=>\$newts,"uInflow=f"=>\$uInflow );
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
if( $ts eq "afs"){ $ts="approximate factorization"; $newts = 1;}
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
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
  $project
# 
#*  useNewImplicitMethod
#  implicitFullLinearized
  implicit factor $impFactor
  dtMax $dtMax
  cfl $cfl
# 
# use full implicit system 1
# use implicit time stepping
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
    choose region...
    mask from grid function
     $bodyMaskShowFile
      -1
    set immersed boundary
#
  exit
# 
# Here is were we specify the user defined forcing:
#- user defined forcing
#-   drag forcing
#- exit
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogesDtol=1.e10; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  boundary conditions
    all=slipWall
    # This next works: 
    # bcNumber4=noSlipWall, uniform(u=$uInflow,v=0.)
# 
   bcNumber1=inflowWithVelocityGiven, uniform(u=$uInflow,T=0.)
   bcNumber2=outflow
# 
   # box(0,0)=inflowWithVelocityGiven, uniform(u=$uInflow,T=0.)
   # box(1,0)=outflow
   done
#
  debug $debug
# initial conditions: uniform flow or restart from a solution in a show file 
if( $restart eq "" ){ $cmds = "uniform flow\n u=$uInflow, v=0., p=1., T=0."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
#
  continue
#
$cmd="plot options...\n" .\
  " plot body force mask surface 1\n" .\
  " plot body force mask\n" .\
  "   iso-surface values 1 0. (num, value1, value2, ...)\n" .\
  "   pick colour...\n" .\
  "   PIC:bronze\n" .\
  "   colour iso-surface 0 BRONZE\n" .\
  "   delete contour plane 0\n" .\
  "   delete contour plane 1\n" .\
  "   delete contour plane 0\n" .\
  "   close colour choices\n" .\
  "   exit\n" .\
  " close plot options";
## $cmd
#
$go 

  plot options...
  plot body force mask surface 1
  plot body force mask
    # iso-surface values 1 .5  (num, value1, value2, ...)
    iso-surface values 1 0  (num, value1, value2, ...)
    colour contour surface 0 BRASS
##    delete contour plane 0
##     delete contour plane 1
##     delete contour plane 0
pause
    exit
  close plot options
# 
$go 

