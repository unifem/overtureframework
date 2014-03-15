#===============================================================
# cgins: moving flat plate wing
# Usage:
#     cgins flatPlateWing -g=<grid> -nu=<> -ts=<> -move=[0|1] -moveOnly=[0|1|2] -amp=<> -freq=<> -project=[0|1]
#
#  moveOnly :  0=normal run, 1=move grids and call ogen, 2=move grids, no ogen.
#
# Examples: 
#  -- not moving: 
#    cgins flatPlateWing -g=flatPlateWingGride2.order2 -nu=5.e-2 -ad22=1. -tp=.05 -move=0 -debug=3 [OK
#  -- simulate motion: 
#    cgins flatPlateWing -g=flatPlateWingGride2.order2 -nu=1.e-2 -tp=.05 -moveOnly=2
#  -- simulate motion with ogen called:
#    cgins flatPlateWing -g=flatPlateWingGride2.order2 -nu=1.e-2 -tp=.05 -moveOnly=1  -freqFullUpdate=1 [OK
#  -- real moving grid run: 
#    cgins -noplot flatPlateWing -g=flatPlateWingGride2.order2 -nu=2.e-2 -tp=.05 -tf=1. -debug=3 -freqFullUpdate=1 -show="flatPlateWing2.show" -go=go >! flatPlateWing2.out &
#
# --- FROM sphereMove.cmd:
#  cgins -noplot sphereMove -g=sphereInABoxe2L6.order2.ml2 -nu=1.e-3 -freq=.25 -tf=8. -tp=.2 -freqFullUpdate=1 -psolver=mg -solver=mg -show=sphereMove2.show -go=go
#
#  -- move grids but do not solve: 
#    cgins sphereMove -g=sphereInABoxe1.order2 -nu=1.e-2 -tp=.01 -ts=pc -moveOnly=1 -freqFullUpdate=1
#
#    cgins -noplot sphereMove -g=sphereInABoxe1.order2 -nu=1.e-3 -tp=1. -tf=10. -show="joukowsky2d4.show" 
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
$grid="flatPlateWingGride2.order2"; $show = " "; $tFinal=5.; $tPlot=.1; $nu=.1; $cfl=.9;
$uInflow=1.;
# 
$implicitVariation="viscous"; $refactorFrequency=100; $impGrids="all=explicit"; $impFactor=.5; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; $newts=0; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.05; 
$order = 2; $fullSystem=0; $go="halt"; $move=1;  $moveOnly=0; $amp=.5; $freq=1.; 
$show=" "; $restart="";
$ts="im"; $outflowOption="neumann"; 
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$cdv=1.; $ad2=1; $ad22=1.; 
$psolver="choose best iterative solver"; $solver="choose best iterative solver"; 
$iluLevels=1; $ogesDebug=0; $project=1; 
$rtolp=1.e-4; $atolp=1.e-5;  # tolerances for the pressure solve
$rtol=1.e-5; $atol=1.e-6;    # tolerances for the implicit solver
# 
$pi = 4.*atan2(1.,1.);
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"move=i"=>\$move,"moveOnly=i"=>\$moveOnly,\
  "amp=f"=>\$amp,"freq=f"=>\$freq, "solver=s"=>\$solver, "psolver=s"=>\$psolver,"uInflow=f"=>\$uInflow,\
  "outflowOption=s"=>\$outflowOption,"newts=i"=>\$newts );
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
if( $ts eq "afs"){ $ts="approximate factorization"; $newts=1;}
# 
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
  plot residuals 1
#
  $project
$cmd="#";
if( $move eq 1 ){ $cmd = "turn on moving grids"; }
$cmd
#  detect collisions 1
#**********
  $freqy=.25; $freqz=.125;  # rotations per second
  specify grids to move 
    matrix motion 
      # rotate about the y-axis:
      rotate around a line
      point on line: 0 0 0
      tangent to line: 0 1. 0.
      edit time function
        linear function
        $freq=$freqy*2.*$pi; 
        linear parameters: 0,$freq (a0,a1)
        # sinusoidal function
        # sinusoid parameters: 1,1,0 (b0,f0,t0)
        exit
      add composed motion
        # rotate about the z-axis:
        rotate around a line
        point on line: 0 0 0
        tangent to line: 0 0. 1. 
        edit time function
          linear function
          $freq=$freqz*2.*$pi; 
          linear parameters: 0,$freq (a0,a1)
          exit
       exit
      exit
      choose grids by share flag
      10
      wingBox
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
  choose grids for implicit
   all=implicit
   backGround=explicit
  done
#
  if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "*"; }
  $newts
#
  pde parameters
    nu $nu
    #  turn on 2nd-order AD here:
    OBPDE:second-order artificial diffusion 1
    OBPDE:ad21,ad22 $ad22, $ad22
    if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
    $cmd
  done
#*
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
    all=slipWall
    bcNumber10=noSlipWall
    bcNumber1=inflowWithVelocityGiven, uniform(u=$uInflow)
    # The outflow pressure BC scales with the size of the domain:  p.n ~ p.x ->  p.r/L   x=r*L 
    $domainLength=4.; $apn=1./$domainLength; 
    bcNumber2=outflow , pressure(1.*p+$apn*p.n=0.)
#    $d=.5; 
#    backGround(0,0)=inflowWithVelocityGiven, parabolic(d=$d,p=1.,u=1.)
#    backGround(0,0)=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)
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
  grid
    toggle grid 0 0
  exit this menu
#
$go 




