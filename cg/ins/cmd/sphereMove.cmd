#===============================================================
# cgins: moving 3d sphere in a box 
# Usage:
#     cgins [-noplot] sphereMove -g=<grid> -nu=<> -ts=<> -move=[0|1] -moveOnly=[0|1|2] -amp=<> -freq=<> -xshift=<> -yshift=<> -zshift=<>  -project=[0|1] ...
#           -go=[go|halt|og]
#
#  -amp, -freq: amplitude and frequency of the sinusoidal motion
#  -xshift, -yshift, -zshift : translate along this vector
#  -moveOnly :  0=normal run, 1=move grids and call ogen, 2=move grids, no ogen.
#
# Examples: 
#    cgins sphereMove -g=sphereInABoxe1.order2 -nu=1.e-2 -tp=.01 -freqFullUpdate=1 -ts=im  [OK
#    cgins sphereMove -g=sphereInABoxe1.order2 -nu=1.e-2 -tp=.01 -freqFullUpdate=1 -ts=pc  [OK
#    cgins sphereMove -g=sphereInABoxe1.order2 -nu=1.e-2 -tp=.01 -freqFullUpdate=1 -ts=afs  [OK
#
#  -- longer box: 
#  cgins -noplot sphereMove -g=sphereInABoxe2L6.order2.ml2 -nu=1.e-3 -freq=.25 -tf=8. -tp=.2 -freqFullUpdate=1 -psolver=mg -solver=mg -show=sphereMove2.show -go=go
#
#  -- order=4:
#   cgins sphereMove -g=sphereInABoxe1.order4.ml1 -nu=1.e-2 -tp=.01 -freqFullUpdate=1 -ts=im -solver=mg -psolver=mg -ad2=0 -ad4=1 [im, OK
#   cgins sphereMove -g=sphereInABoxe1.order4.ml1 -nu=1.e-2 -tp=.01 -freqFullUpdate=1 -ts=pc -solver=mg -psolver=mg -ad2=0 -ad4=1 [pc, OK
#   cgins sphereMove -g=sphereInABoxe1.order4.ml1 -nu=1.e-2 -tp=.01 -freqFullUpdate=1 -ts=pc4 -solver=mg -psolver=mg -ad2=0 -ad4=1 [pc4, OK
#   cgins sphereMove -g=sphereInABoxe1.order4.ml1 -nu=1.e-2 -tp=.01 -freqFullUpdate=1 -ts=afs -solver=mg -psolver=mg -ad2=0 -ad4=1 [afs, OK
#
#  -- move grids but do not solve: 
#    cgins sphereMove -g=sphereInABoxe1.order2 -nu=1.e-2 -tp=.01 -ts=pc -moveOnly=1 -freqFullUpdate=1
#
#    cgins -noplot sphereMove -g=sphereInABoxe1.order2 -nu=1.e-3 -tp=1. -tf=10. -show="sphere1.show" 
#    cgins sphereMove -g=sphereInABoxe1.order2 -impGrids="all=implicit"
#
# -- parallel
#  OK: 
#  mpirun -np 2 $cginsp sphereMove -g=sphereInABoxe1.order2 -nu=1.e-2 -tp=.05 -freqFullUpdate=1 -psolver=mg -solver=mg
#  mpirun -np 2 -all-local $cginsp sphereMove -g=joukowsky2de2.order2 -nu=1.e-2 -tp=.01  -ts=pc -moveOnly=1
#  srun -N1 -n4 -ppdebug $cginsp sphereMove -g=sphereInABoxe1.order2 -nu=1.e-2 -tp=.01 -freqFullUpdate=1
# 
#  totalview srun -a -N1 -n1 -ppdebug $cginsp -noplot sphereMove -g=joukowsky2de4.order2 -ts=pc -freqFullUpdate=1
#===============================================================
#
$grid="sphereInABoxe1.order2"; $show = " "; $tFinal=5.; $tPlot=.1; $nu=.1; $cfl=.9;
$pGrad=0.; $xshift=-1.; $yshift=0.; $zshift=0.; 
# 
$implicitVariation="viscous"; $refactorFrequency=100; $impGrids="all=explicit"; $impFactor=.5; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.05; 
$order = 2; $fullSystem=0; $go="halt"; $move=1;  $moveOnly=0; $amp=.5; $freq=1.; 
$show=" "; $restart=""; $newts=0;
$ts="im"; 
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$cdv=1.; $ad2=1; $ad21=1.; $ad22=1.; $ad4=0; $ad41=1.; $ad42=1.; 
$psolver="choose best iterative solver"; $solver="choose best iterative solver"; 
$iluLevels=1; $ogesDebug=0; $project=1; 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
# 
# -- for Kyle's AF scheme:
$afit = 20;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
#
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"imp=f"=>\$implicitFactor,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"move=i"=>\$move,"moveOnly=i"=>\$moveOnly,\
  "amp=f"=>\$amp,"freq=f"=>\$freq, "solver=s"=>\$solver, "psolver=s"=>\$psolver,"xshift=f"=>\$xshift,"yshift=f"=>\$yshift,"zshift=f"=>\$zshift,\
  "ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42 );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
#
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
if( $ts eq "pc4" ){ $ts="adams PC order 4"; $useNewImp=0; } # NOTE: turn off new implicit for fourth order
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;  $useNewImp=0;}
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $project eq "1" && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
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
##  plot residuals 1
#
  $project
$cmd="#";
if( $move eq 1 ){ $cmd = "turn on moving grids"; }
$cmd
#  detect collisions 1
#**********
  specify grids to move
    matrix motion
      translate along a line
      point on line: 0 0 0
      tangent to line: $xshift $yshift $zshift
      edit time function
        sinusoidal function
        # specify amplitude of the plunge and frequency
        sinusoid parameters: $amp $freq 0. (b0,f0,t0)
      exit
    exit
#   -- choose which grids to move by the share value 
   choose grids by share flag
      1
   done
# 
  done
#**************
# 
  frequency for full grid gen update $freqFullUpdate
#
#*  useNewImplicitMethod
#  implicitFullLinearized
  implicit factor $impFactor
  dtMax $dtMax
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
  choose grids for implicit
   all=implicit
   backGround=explicit
  done
  pde parameters
    nu $nu
    #  turn on 2nd-order AD here:
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22 $ad21, $ad22
    OBPDE:fourth-order artificial diffusion $ad4
    OBPDE:ad41,ad42 $ad41, $ad42
  done
#*
# Here is were we specify a pressure gradient for flow in a periodic channel:
# This is done by adding a const forcing to the "u" equation 
if( $pGrad != 0 ){ $cmds ="user defined forcing\n constant forcing\n 1 $pGrad\n  done\n exit";}else{ $cmds="*"; }
$cmds
#
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogmgDebug=$ogesDebug; $ogmgCoarseGridSolver="best"; $ogmgRtolcg=$rtolp; $ogmgAtolcg=$atolp; $ogmgRtolcg=$rtolp; $ogmgAtolcg=$atolp;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   # $ogmgSsr=1;  $ogmgAutoChoose=1; 
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; $ogmgOpav=0; $ogmgRtolcg=1.e-6; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  boundary conditions
    all=noSlipWall
    backGround=slipWall
    backGround(0,0)=inflowWithVelocityGiven, uniform(u=1.)
    $d=.5; 
#    backGround(0,0)=inflowWithVelocityGiven, parabolic(d=$d,p=1.,u=1.)
#    backGround(0,0)=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)
    backGround(1,0)=outflow
   done
#
  debug $debug
#
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




