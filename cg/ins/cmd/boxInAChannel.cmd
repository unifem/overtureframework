#===============================================================
# cgins: moving 3D box
# Usage:
#     cgins boxInAChannel -g=<grid> -nu=<> -ts=<> -move=[0|1] -moveOnly=[0|1|2] -freq=<> -amp=<> -rotationFreq=<>
#                  -solver=[best|yale|mg] -psolver=[best|yale|mg] -ad2=[0|1] -freqFullUpdate=<>
#
# Command line args:
#  -freq=<> -amp=<> : frequency and amplitude of the up/down sinusoidal motion
#  -rotationFreq : frequency of rotation
#  -move : 0=no motion, 1=move
#  -moveOnly :  0=normal run, 1=move grids and call ogen, 2=move grids, no ogen.
#
# Examples: 
#
#  -- non-moving:
#  cgins boxInAChannel -g=loftedBoxe2.order2.ml1 -nu=1.e-3 -tp=.1 -tf=10. -move=0 -cfl=3. -psolver=mg -ts=afs -ad2=0 -ad4=1 -debug=3 -go=halt 
#  -- moving:
#  cgins boxInAChannel -g=loftedBoxe2.order2.ml1 -nu=1.e-3 -tp=.1 -tf=10. -move=1 -freq=.25 -amp=.25 -cfl=4. -psolver=mg -ts=afs -ad2=0 -ad4=1 -debug=3 -go=halt
# 
#===============================================================
#
$grid="joukowsky2de4.order2"; $show = " "; $tFinal=5.; $tPlot=.1; $nu=.1; $cfl=.9;
$pGrad=0.; $wing="wing"; $newts=0; 
$ad2=0; $ad21=1; $ad22=1;  $ad4=0; $ad41=1.; $ad42=1.; 
# 
$slowStartTime=-1.; $slowStartCFL=2.;
$implicitVariation="viscous"; $refactorFrequency=100; $impGrids="all=explicit"; $impFactor=.5; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.1; 
$order = 2; $fullSystem=0; $go="halt"; $move=1;  $moveOnly=0; 
$freqX=0.; $freqY=0.; $freqZ=0.; # rotation frequencies 
$show=" "; $restart="";  $outflowOption="neumann"; 
$psolver="best"; $solver="best"; 
$iluLevels=1; $ogesDebug=0; $project=1; 
$ts="im"; 
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$cdv=1.; 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
# -- for Kyle's AF scheme:
$afit = 10;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$cdv=1;  $cDt=.25;
$pi=4.*atan2(1.,1.);
$ogmgAutoChoose=1;  #1=ON, 2=robust options
$ogmgSsr=0;  # Show smoothing rates
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
  "freq=f"=>\$freq,"outflowOption=s"=>\$outflowOption,"newts=i"=>\$newts,"afit=i"=>\$afit,\
  "ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,"amp=f"=>\$amp,"freqX=f"=>\$freqX,"freqY=f"=>\$freqY,\
  "freqZ=f"=>\$freqZ,"ogmgAutoChoose=s"=>\$ogmgAutoChoose, "ogmgSsr=s"=>\$ogmgSsr,"slowStartCFL=f"=>\$slowStartCFL,\
   "slowStartTime=f"=>\$slowStartTime);
# -------------------------------------------------------------------------------------------------
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
  incompressible Navier Stokes
  $cmd="#";
  if( $moveOnly eq 1 ){ $cmd ="move and regenerate grids only"; }elsif( $moveOnly eq 2 ){ $cmd = "move grids only"; }
  $cmd
  exit
#
  show file options
   compressed
     OBPSF:maximum number of parallel sub-files 8
   open
     $show
    frequency to flush
      2
  exit  
#
  turn off twilight zone
# 
  final time $tFinal
  times to plot $tPlot
  plot and always wait
# 
#  plot residuals 1
#
  $project
$cmd="#";
if( $move eq 1 ){ $cmd = "turn on moving grids"; }
#  turn on moving grids
$cmd
#  detect collisions 1
#**********
  specify grids to move
    matrix motion
      # rotate the box about the x-axis
      rotate around a line
      point on line: 0 0 0
      tangent to line: 1 0 0
      edit time function
        $rfreq = $freqX*2.*$pi; 
        linear parameters: 0,$rfreq (a0,a1)
      exit
      # Add a rotation about the y-axis
      add composed motion
        rotate around a line
        point on line: 0 0 0
        tangent to line: 0 1. 0
        edit time function
          $rfreq = $freqY*2.*$pi; 
          linear parameters: 0,$rfreq (a0,a1)
        exit
        # Add a rotation about the z-axis
        add composed motion
          rotate around a line
          point on line: 0 0 0
          tangent to line: 0 0 1.
          edit time function
            $rfreq = $freqZ*2.*$pi; 
            linear parameters: 0,$rfreq (a0,a1)
          exit
        exit
      exit
    exit
    choose grids by share flag
      7
    done
  done
#**************
# 
  frequency for full grid gen update $freqFullUpdate
#
#*  useNewImplicitMethod
#  implicitFullLinearized
  implicit factor $impFactor
  dtMax $dtMax
  cfl $cfl
# 
  slow start $slowStartTime   # (seconds)
  slow start cfl $slowStartCFL
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
    #  turn on 2nd-order AD here:
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22 $ad22,$ad22
    OBPDE:fourth-order artificial diffusion $ad4
    OBPDE:ad41,ad42 $ad41, $ad42
#
    if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
    $cmd
   done
#*
# Here is were we specify a pressure gradient for flow in a periodic channel:
# This is done by adding a const forcing to the "u" equation 
if( $pGrad != 0 ){ $cmds ="user defined forcing\n constant forcing\n 1 $pGrad\n  done\n exit";}else{ $cmds="*"; }
$cmds
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels;
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
    bcNumber7=noSlipWall
    bcNumber1=inflowWithVelocityGiven, uniform(u=1.)
    $d=.5; 
#    box(0,0)=inflowWithVelocityGiven, parabolic(d=$d,p=1.,u=1.)
#    box(0,0)=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)
    bcNumber2=outflow
   done
#
  debug $debug
# initial conditions: uniform flow or restart from a solution in a show file 
if( $restart eq "" ){ $cmds = "uniform flow\n u=1., v=0., p=1."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
#
  continue
#
$go 

