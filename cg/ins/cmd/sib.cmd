#
# cgins: flow past a sphere in a box
#
# Usage:
#   
#  cgins [-noplot] sib -g=<name> -tf=<tFinal> -tp=<tPlot> -order=<2/4> -model=<ins/boussinesq> -ts=<pc|im|pc4> ...
#         -debug=<num> -ad2=<> -bg=<backGround> -project=<0/1> -iv=[viscous/adv/full] -imp=<val> -rf=<val> ...
#         -inflow=[uniform|parabolic|pressure] -passive=[0|1] -nuPassive=<f> -go=[run/halt/og]
# 
# Examples
#   cgins sib -g=sibe1.order2 -bg=box -nu=.05 -tf=5. -tp=.1 -ts=im -debug=3 [OK
#   cgins sib -g=sibe2.order2 -bg=box -nu=.05 -tf=5. -tp=.1 -ts=im -debug=1
# 
# -- two spheres in a channel:
#   cgins sib -g=twoSpheresInAChannele2.order2 -nu=.02 -tf=5. -tp=.1 -ts=im -debug=3
#   cgins sib -g=twoSpheresInAChannele4.order2 -nu=.01 -tf=5. -tp=.1 -ts=im -debug=3
# 
# -- fourth-order
#   cgins sib -g=sibe2.order4.ml2 -bg=box -ts=im -nu=1.e-2 -ad4=1 -tf=20. -tp=.01 
# 
#   cgins sib -g=sibe2.order4 -bg=box -order=4 -nu=.02 -tf=5. -tp=.1 -ts=im -debug=3 -solver=mg -psolver=mg
#   cgins sib -g=sibe2.order4 -bg=box -order=4 -nu=.02 -tf=5. -tp=.02 -ts=pc4 -ad2=0 -debug=3 [OK? 
# 
#   -- pressure inflow and 4th order: (turn off projection for now)
#   cgins sib -g=sibe2.order4.ml2 -bg=box -inflow=pressure -ts=pc -nu=1.e-2 -ad4=1 -tf=20. -tp=.01 -psolver=mg -project=0
#   cgins sib -g=sibe2.order4.ml2 -bg=box -inflow=pressure -ts=im -nu=1.e-2 -ad4=1 -tf=20. -tp=.01 -solver=mg -psolver=mg -project=0
# -- MG 
#   cgins sib -g=sibe2.order2.ml2 -bg=box -ts=im -nu=1.e-2 -ad4=1 -tf=20. -tp=.1 -solver=mg -psolver=mg [OK
#   cgins sib -g=sibe2.order4.ml2 -bg=box -ts=im -nu=1.e-2 -ad4=1 -tf=20. -tp=.01 -solver=mg -psolver=mg -debug=3
#   -- trouble: 
#   cgins sib -g=sibe2.order4.ml3 -bg=box -ts=im -nu=1.e-2 -ad4=1 -tf=20. -tp=.01 -solver=mg -psolver=mg -debug=3
#
# -- Passive scalar: NOTE: 
#   cgins sib -g=sibe1.order2 -bg=box -nu=.05 -tf=5. -tp=.1 -ts=pc -passive=1 -nuPassive=.01 -debug=3 [Ok
#   cgins sib -g=sibe2.order2 -bg=box -nu=.02 -tf=5. -tp=.1 -ts=pc -passive=1 -nuPassive=.005 -debug=3 [Ok
# 
#   cgins sib -g=sibe1.order2 -bg=box -nu=.05 -tf=5. -tp=.1 -ts=im -passive=1 -nuPassive=.01 -debug=3 -newts=1  [TO-DO 
#   cgins sib -g=sibe2.order2 -bg=box -nu=.02 -tf=5. -tp=.1 -ts=im -passive=1 -nuPassive=.005 -debug=3 -newts=1 [TO-DO
#   
# 
# srun -N1 -n2 -ppdebug $cginsp sib -g=sibe2.order2 -bg=box -nu=.05 -tf=5. -tp=.1 -ts=pc -debug=1
# 
#  1.8 M pts: 
# $grid="sibe6.order2.hdf"; $show="sib.show"; $nu=.005; $tFinal=5.; $tPlot=.5; 
# $grid="sibe1.order2.hdf"; $show="sib.show"; 
#
# -- two spheres:
# srun -N4 -n64 -ppbatch $cginsp -noplot sib -g=twoSpheresInAChannele8.order4.ml4.hdf -nu=2.e-5 -tf=20. -tp=.05 -ts=afs -psolver=mg -ad2=0 -ad4=1 -cfl=3.5 -slowStartCFL=3.5 -slowStartSteps=100 -slowStartRecomputeDt=10 -recomputeDt=50 -numberOfParallelGhost=4 -debug=1 -project=1 -show=twoSpheresO4G8.show -go=go
# 
# --- set default values for parameters ---
# 
$grid="sibe1.order2.hdf"; $backGround="backGround"; $restart="";
$tFinal=1.; $tPlot=.1; $cfl=.9; $nu=.05; $Prandtl=.72; $thermalExpansivity=.1; 
$gravity = "0. 0. 0."; $inflow = "uniform"; 
$passive=0; $nuPassive=.05; 
$model="ins"; $ts="pc"; $noplot=""; $implicitVariation="viscous"; $refactorFrequency=100; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; 
$ogesDebug=0; $project=1; $cdv=1.;
$rtolp=1.e-3; $atolp=1.e-4; $psolver="best"; $iluLevels=3;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;  $solver="best";                  # tolerances for the implicit solver
$ad2=0; $ad21=1.; $ad22=1.;   # for 2nd-order artificial dissipation, ad2=0 means no dissipation
$ad4=0; $ad41=1.; $ad42=1.;   # for 4th-order artificial dissipation, ad4=0 means no dissipation
$newts = 0; $outflowOption="neumann";
$slowStartSteps=-1; $slowStartCFL=4.; $slowStartRecomputeDt=100; $slowStartTime=-1.; $recomputeDt=10000;
# -- for Kyle's AF scheme:
$afit = 10;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$cdv=1;  $cDt=.25;
$ogmgAutoChoose=1;  #1=ON, 2=robust options
$ogmgSsr=0;  # Show smoothing rates
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"imp=f"=>\$implicitFactor,"psolver=s"=>\$psolver,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"inflow=s"=>\$inflow,\
 "ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"ad4=i"=>\$ad4,"ad42=f"=>\$ad42,"outflowOption=s"=>\$outflowOption,"newts=i"=>\$newts,\
  "slowStartCFL=f"=>\$slowStartCFL, "slowStartTime=f"=>\$slowStartTime,"slowStartSteps=i"=>\$slowStartSteps,\
  "slowStartRecomputeDt=i"=>\$slowStartRecomputeDt,"restart=s"=>\$restart,"nuPassive=f"=>\$nuPassive,\
  "passive=i"=>\$passive );
# -------------------------------------------------------------------------------------------------
$kThermal=$nu/$Prandtl; 
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
if( $ts eq "pc4" ){ $ts="adams PC order 4"; }
if( $ts eq "mid"){ $ts="midpoint";       }  
if( $ts eq "afs"){ $ts="approximate factorization"; $newts = 1;}
if( $ts eq "afs"){ $ts="approximate factorization"; $newts = 1;}
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $project eq "1" && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "*"; }
#
# specify the overlapping grid to use:
$grid
# Specify the equations we solve:
  $model
  if( $passive ne 0 ){ $cmd="passive scalar advection"; }else{ $cmd="#"; }
  $cmd
  exit
# -- order of accuracy: 
# $order 
  turn off twilight zone
# 
  $ts
  $implicitVariation
  $newts
  # -- for the AFS scheme:
  compact finite difference
  # -- convergence parameters for the af scheme
  max number of AF corrections $afit
  AF correction relative tol $aftol
  # optionally turn this on to improve stability of the high-order AF scheme by using 2nd-order dissipation at the boundary
  OBPDE:use boundary dissipation in AF scheme 1
#
# the following line is for ts=afs only
compact finite difference
#
  show file options
    compressed
   compressed
     OBPSF:maximum number of parallel sub-files 16
    open
      $show
    frequency to flush
      2
    exit
  # outer box is done explicitly
  choose grids for implicit
    all=implicit
    $backGround=explicit
    done
  final time $tFinal
  times to plot $tPlot
   plot and always wait
  no plotting
  pde parameters
    nu
      $nu
   OBPDE:passive scalar diffusion coefficient $nuPassive
   OBPDE:expect inflow at outflow
   if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
   $cmd
  done
  cfl $cfl
# 
  slow start cfl $slowStartCFL
  slow start steps $slowStartSteps
  slow start recompute dt $slowStartRecomputeDt
  slow start $slowStartTime   # (seconds)
#
  maximum number of iterations for implicit interpolation
    10
# 
* 
  OBPDE:second-order artificial diffusion $ad2
  OBPDE:ad21,ad22  $ad22 $ad22
  OBPDE:fourth-order artificial diffusion $ad4
  OBPDE:ad41,ad42 $ad42 , $ad42
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogmgDebug=$ogesDebug; $ogmgCoarseGridSolver="best"; $ogmgRtolcg=$rtolp; $ogmgAtolcg=$atolp; $ogmgRtolcg=$rtolp; $ogmgAtolcg=$atolp;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   # $ogmgSsr=1;  $ogmgAutoChoose=1; 
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; $ogmgOpav=0; $ogmgRtolcg=1.e-6; $ogesDtol=1.e20; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  $project
# 
  initial conditions
  if( $restart eq "" ){ $cmds = "uniform flow\n u=1., v=0., p=1., s=0."; }\
    else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
  $cmds
#    uniform flow
#     u=1., p=1.
  exit
#
  debug $debug
#
  boundary conditions
   # all=noSlipWall
   #    $backGround=slipWall
   # new way: 110320
   all=slipWall
   bcNumber1=inflowWithVelocityGiven, uniform(p=1.,u=1.,s=1.)
   if( $inflow eq "uniform" ){ $cmd = "bcNumber1=inflowWithVelocityGiven, uniform(p=1.,u=1.)"; }\
   elsif( $inflow eq "parabolic" ){ $cmd = "bcNumber1=inflowWithVelocityGiven , parabolic(d=.5,p=1.,u=1.)"; }\
   elsif( $inflow eq "pressure" ){ $cmd = "bcNumber1=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)"; }\
   else{ $cmd = "bcNumber1=inflowWithVelocityGiven, uniform(p=1.,u=1.)"; } 
   $cmd
# 
   bcNumber2=outflow , pressure(1.*p+1.*p.n=0.)
   bcNumber7=noSlipWall
# 
# -- oscillating inflow: 
#-    box(0,0)=inflowWithVelocityGiven , parabolic(d=.2,p=1.,u=1.), oscillate(a0=1.5,t0=.0,omega=.5)
# 
    done
  exit
  y+r:0 25
  x+r:0 25
#
  contour
  exit
$go


  grid
    toggle grid 0 0
    plot block boundaries 0
    plot grid lines 0
    grid colour 2 BRASS
    grid colour 3 BRASS
    grid colour 4 BRASS
    grid colour 5 BRASS
    grid colour 6 BRASS
    grid colour 7 BRASS
  exit this menu
  contour
    delete contour plane 1
    delete contour plane 1
    delete contour plane 0
    add contour plane  0.00000e+00  0.00000e+00  1.00000e+00 -1.17643e-02  6.04567e-01 -4.51765e-01 
    add contour plane  0.00000e+00  1.00000e+00  0.00000e+00 -1.02583e+00 -4.94666e-01  5.62264e-01 
 exit
$go
