#===============================================================
#
# cgins: flow past a cylinder using the multigrid solver Ogmg
# 
# Usage:
#     cgins cic.mg -g=<grid> -nu=<> -ts=<> -move=[0|1] -moveOnly=[0|1|2] -freq=<> ...
#                  -ts=<pc|im|fe|be|mid> -debug=<> -debugp=<> -debugi=<> -opave=[0|1] -recomputeDt=<> 
#                  -ssr=[0|1] -outflowOption=[extrap|neumann] -go=[go|halt]
#
#  debugp : debug for pressure solve
#  debugi : debug for implicit solve
#  moveOnly :  0=normal run, 1=move grids and call ogen, 2=move grids, no ogen.
#  opav : MG operator averaging
#  ssr  : =1 show MG smoothing rates
#
# Examples: 
#   cgins cic.mg -g=cice2.order2.ml2.hdf -ts=pc -nu=.01
#   cgins cic.mg -g="cice2.order2.ml2.hdf" -ts=im -nu=.01
#   cgins cic.mg -g=cic.bbmg -ts=im -nu=.01 -ad2=1 [trouble with ad2 on
#   cgins cic.mg -g="cic.bbmg3.hdf" -ts=im -nu=.005
#   cgins cic.mg -g="cic.bbmg4.hdf" -ts=im -nu=1.e-3 
#   cgins cic.mg -g="cic.bbmg5.hdf" -ts=im -nu=5.e-4 -debugi=3
#   cgins cic.mg -g="cic.bbmg6.hdf" -ts=im -nu=2.e-4             (1M pts)
#   cgins cic.mg -g="cic.bbmg7.hdf" -ts=im -nu=5.e-5             (4M pts)
#
#  -- Neumann BC at outflow:
#   cgins cic.mg -g="cice2.order2.ml2.hdf" -ts=im -nu=.01 -outflowOption=neumann
#  -- cylinder in a long channel:
#  cgins cic.mg -g="cicLongChannele2.hdf" -nu=.01 -tp=.01 -tf=.01 -show="cic5e.show"
#  -- two cylinders:
#   cgins cic.mg -g="tcilc4e.hdf" -nu=.001                        (.8M)
#   cgins cic.mg -g="tcilc6e.hdf" -nu=1.e-4 -show="tcilc6.show"   (13M)
#
# -- parallel
#  mpirun -np 2 $cginsp cic.mg -g=cice2.order2.ml2 -ts=pc -nu=.01 -opav=0  [OK]
#  mpirun -np 2 $cginsp cic.mg -g=cice2.order2.ml2 -ts=im -nu=.01 -opav=0  [OK]
#  mpirun -np 2 $cginsp cic.mg -g=cice2.order2.ml2 -ts=im -nu=.01 -opav=1  [OK]
#  mpirun -np 2 $cginsp cic.mg -g=cice2.order2.ml2 -ts=im -nu=.01 -opav=1 -recomputeDt=2 [Ok (Neumann outflow)
#   -- Ok until time step "not" adjusted --> interpolant deleted ??
#  mpirun -np 2 $cginsp cic.mg -g="cicLongChannele2.hdf" -ts=im -nu=.01   [Ok, n=2,4
#  mpirun -np 2 $cginsp cic.mg -g="cicLongChannele8.hdf" -ts=im -nu=.001 -tf=50. -tp=1. -show="cicLong8.show"
#
#  srun -N1 -n2 -ppdebug $cginsp noplot cic.mg -g=cice2.order2.ml2 -ts=pc -nu=.01 -tf=.5 -go=go  [ok, n=2,4
#  srun -N1 -n4 -ppdebug $cginsp noplot cic.mg -g=cice2.order2.ml2 -ts=pc -nu=.01 -tf=.1 -go=go -ad2=1 [ok n=2,4
#  srun -N1 -n2 -ppdebug $cginsp noplot cic.mg -g=cice2.order2.ml2 -ts=im -nu=.01 -tf=.1 -go=go  [ok, n=2,4
#  srun -N1 -n2 -ppdebug $cginsp noplot cic.mg -g=cice2.order2.ml2 -ts=im -nu=.01 -tf=.1 -go=go -ad2=1 [BAD
#  srun -N1 -n2 -ppdebug $cginsp noplot cic.mg -g=cice2.order2.ml2 -ts=im -nu=.01 -tf=.1 -go=go -ad2=1 -solver=best [ok
#  srun -N1 -n2 -ppdebug $cginsp noplot cic.mg -g=cice2.order2.ml2 -ts=im -nu=.01 -tf=.5 -recomputeDt=2 -go=go [ok, n=2,4
#  srun -N1 -n2 -ppdebug $cginsp cic.mg -g=cicLongChannele2 -ts=im -nu=.01 -go=halt         [ok, n=2,4
#  srun -N1 -n4 -ppdebug $cginsp cic.mg -g=cicLongChannele2 -ts=im -nu=1.e-3 -ad2=1 -go=halt [SEG fault
#  srun -N1 -n2 -ppdebug $cginsp cic.mg -g=cice2.order2.ml2 -ts=im -nu=1.e-3 -ad2=1 -go=halt [NAN
#
#  srun -N1 -n2 -ppdebug $cginsp cic.mg -g=square8 -ts=im -nu=.1 -ad2=1 -tp=.01 -go=halt [ok
#  srun -N1 -n4 -ppdebug $cginsp cic.mg -g=square16 -ts=im -nu=.1 -ad2=1 -tp=.01 -go=halt [ok
#  srun -N1 -n2 -ppdebug $cginsp cic.mg -g=nonSquare16.order2 -ts=im -nu=.1 -ad2=1 -tp=.01 -bcn=ns -go=halt [BAD
# srun -N1 -n2 -ppdebug $cginsp cic.mg -g=nonSquare8.order2 -ts=im -nu=.1 -ad2=1 -tp=.05 -tf=.1 -bcn=ns -psolver=best -go=halt
# srun -N1 -n2 -ppdebug $cginsp noplot cic.mg -g=nonSquare8.order2 -ts=im -nu=.1 -ad2=1 -tp=.05 -tf=.1 -bcn=ns -psolver=best -go=go > ! junk [BAD 
# srun -N1 -n2 -ppdebug $cginsp noplot cic.mg -g=square8 -ts=im -nu=.1 -ad2=0 -tp=.05 -tf=.1 -bcn=ns -psolver=best -go=go > ! junkp [BAD 
#===============================================================
#
$grid="cic.bbmg2.hdf"; $show = " "; $tFinal=5.; $tPlot=.1; $nu=.1; $cfl=.9; $bcn="#"; 
# 
$implicitVariation="viscous"; $refactorFrequency=100; $impGrids="all=explicit"; $impFactor=.5; $dtMax=.05; 
$debug = 3;  $debugp=1; $debugi=1; $opav=1; $ssr=0; 
$maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.05; 
$fullSystem=0; $go="halt"; $move=0;  $moveOnly=0; $freq=1.; 
$show=" "; $restart="";  $outflowOption="neumann"; 
$psolver="mg"; $solver="mg"; $ogmgDebug=0; $ogmgSsr=0;
$iluLevels=1; $ogesDebug=0; $project=1; 
$ts="im"; 
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$cdv=1.; 
$ad2=0; $ad21=1.; $ad22=1.;   # for 2nd-order artificial dissipation, ad2=0 means no dissipation
$ad4=0; $ad41=1.; $ad42=1.;   # for 4th-order artificial dissipation, ad4=0 means no dissipation
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
$refactorFrequency=10000; $recomputeDt=10000; 
$ogmgSaveGrid=""; $ogmgReadGrid=""; # save or read a MG grid with levels built
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "psolver=s"=>\$psolver, "tz=s"=>\$tz,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"imp=f"=>\$implicitFactor,"show=s"=>\$show,"bcn=s"=>\$bcn,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"move=i"=>\$move,"moveOnly=i"=>\$moveOnly,\
  "freq=f"=>\$freq,"debugp=i"=>\$debugp,"debugi=i"=>\$debugi,"opav=i"=>\$opav,"ssr=i"=>\$ssr,"recomputeDt=i"=>\$recomputeDt,\
  "refactorFrequency=i"=>\$refactorFrequency,"ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,\
  "ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,"ogmgSaveGrid=s"=>\$ogmgSaveGrid,"ogmgReadGrid=s"=>\$ogmgReadGrid,\
  "outflowOption=s"=>\$outflowOption,"ogmgDebug=i"=>\$ogmgDebug );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
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
if( $project eq "1" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
# if( $bcn "ns" ){ $bcn = "square=noSlipWall"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#if( $opav eq "0" ){ $opav = "do not average coarse grid equations"; }\
#  elsif( $opav eq "2" ){ $opav ="do not average coarse curvilinear grid equations"; }else{ $opav = "#"; }
if( $ssr eq 1 ){ $ssr="show smoothing rates"; }else{ $ssr="#"; }
#
#
# -- old --
# $tFinal=10; $tPlot=.1; $debug=0; $show=" ";
# 
# cic.bbmg.hdf
# cic.bbmg2.hdf
# $gridName="cic.bbmg2.hdf"; $show="cic.mg.show"; $nu=.01;
# $gridName="cic.bbmg4.hdf"; $show="cic.mg.show"; $nu=.01;
# cic.bbmg6.hdf
# cilc2.hdf
# cylinder in a long channel: (1M pts)
# cilc3.hdf
# $gridName="cic2e.hdf"; $show="cic2e.show"; $nu=.01; $tPlot=1.; $tFinal=.1; 
# $gridName="cic5e.hdf"; $show="cic5e.show"; $nu=.01; $tPlot=.01; $tFinal=.01; 
# $gridName="cic7e.hdf"; $show="cic7a.show"; $nu=.0005; $tPlot=1.; $tFinal=5.; 
# $gridName="cic7e.hdf"; $show="cic7a.show"; $nu=.0005; $tPlot=.5; $tFinal=3.; 
#
# --------------
# two cylinders in a long channel:
# tcilc2.hdf
# $gridName="tcilc4e.hdf"; $show="tcilc4.show"; $nu=.001;
#   3.2M pts:  (nu=.0005)
# $gridName="tcilc5e.hdf"; $show="tcilc5.show"; $nu=.0005; 
# 13M pts nu=.0001
# $gridName="tcilc6e.hdf"; $show="tcilc6.show"; $nu=.0001;
#
$grid
  incompressible Navier Stokes
  exit
  turn off twilight zone 
#
$ts
  choose grids for implicit
   all=implicit
#   square=explicit
  done
#
  final time $tFinal
  times to plot $tPlot
# 
  cfl $cfl
#
  recompute dt every $recomputeDt
  refactor frequency $refactorFrequency
  dtMax $dtMax
#
  show file options
    compressed
    open
     $show
    frequency to flush
      1
    exit
#
  no plotting
  plot and always wait
#
    maximum number of iterations for implicit interpolation
      10
#
  pde parameters
    nu $nu
#  turn on 2nd-order AD here:
   OBPDE:second-order artificial diffusion $ad2
   OBPDE:ad21,ad22 $ad21 , $ad22
   OBPDE:fourth-order artificial diffusion $ad4
   OBPDE:ad41,ad42 $ad41 , $ad42
   if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
   $cmd
  done
# 
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogesDebug=$debug; $ogmgDebug=$debug; $ogmgCoarseGridSolver="best";  $ogmgOpav=$opav;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; $ogesDebug=$debugi; $ogmgDebug=$debug; $ogmgOpav=$opav;
   # $ogmgDebug=3; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  boundary conditions
#
#    all=dirichletBoundaryCondition
    all=noSlipWall
    square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.)
#    -- can be trouble for 1.*p+0.01*p.n=0.
    square(1,0)=outflow , pressure(1.*p+1.*p.n=0.)
    square(0,1)=slipWall
    square(1,1)=slipWall
#
#    all=dirichletBoundaryCondition
    $bcn
    done
  initial conditions
  uniform flow
    p=1., u=1.
  done
$project
 # save a restart file
#   Oges::debug
#      63
  debug
    $debug 
  continue
 #   continue
# 
$go


  movie mode
  finish
