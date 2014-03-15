# **********************************************************************************************************
# cgins command file: Flow past two cylinders in a channel
#
#    cgins [-noplot] tcilc -g=<grid-name> -nu=<> -tp=<> -tf=<> -show=<> -debug=<> -project=[0|1] ...
#          -ts=[pc|im|afs|ss] -solver=[best|mg|yale] -psolver=[best|mg|yale] -cpn=<> -inflow=[uniform|ramp] ...
#          -implicitVariation=[viscous/adv/full] -implicitFactor=<val> -refactorFrequency=<> ...
#          -plotResiduals=[0|1]
#
#  -implicitVariation : viscous=viscous terms implicit, full=full linearized version
#  -implicitFactor : .5=CN, 1.=BE, 0.=FE
#  -refactorFrequency : refactor frequency
#  -cpn : coefficient of p.n in the outflow BC. Normally increase for longer domains. 
#
# Examples:
#   cgins tcilc -g=tcilce2.order2 -nu=.01 -tp=.1 -tf=20                              
#   cgins tcilc -g=tcilce2.order2 -nu=.01 -tp=.1 -tf=20 -solver=yale -psolver=yale
# 
#   cgins tcilc -g=tcilce2.order2 -nu=1.e-8 -tp=.02 -tf=20 -ad2=1 -solver=yale -psolver=yale [OK (but nu is too small)
#   cgins tcilc -g=tcilce2.order2 -nu=1.e-8 -tp=.02 -tf=20 -ad4=1 -solver=yale -psolver=yale -ts=pc [OK, but ts=im *BAD*
#
# -- Steady state line solver
#   cgins tcilc -g=tcilce2.order2 -nu=.1 -ts=ss -plotIterations=100 -maxIterations=5000 -psolver=yale 
# -- Full implicit solver
#   cgins tcilc -g=tcilce2.order2 -nu=.1 -ts=im -implicitVariation=full -implicitFactor=1. -refactorFrequency=20 -tp=.5 -tf=100. -dtMax=.1 -solver=yale -psolver=yale -plotResiduals=1
# -- order 4:
#   cgins tcilc -g=tcilce2.order4 -nu=.01 -tp=.1 -tf=20 -solver=yale -psolver=yale -ad4=1    [OK
#   cgins tcilc -g=tcilce2.order4 -nu=.01 -tp=.1 -tf=20 -solver=yale -psolver=yale     [ok]
#   cgins tcilc -g=tcilce1.order4 -nu=.01 -tp=.01 -tf=20 -solver=yale -psolver=yale  [ok
#   cgins -noplot tcilc -g=tcilce1.order4 -nu=.01 -tp=.05 -tf=20 -solver=yale -psolver=yale -go=go -ad4=1 [OK
#   cgins -noplot tcilc -g=tcilce1.order4 -nu=.01 -tp=.01 -tf=20 -ts=pc -solver=yale -psolver=yale -go=go [Trouble
#   cgins -noplot tcilc -g=tcilce1.order4 -nu=.01 -tp=.01 -tf=20 -ts=pc4 -solver=yale -psolver=yale -go=go [Trouble
# 
# - test cic: 
#   cgins -noplot tcilc -g=cice2.order4 -nu=.01 -tp=.01 -tf=20 -solver=yale -psolver=yale -go=og -ad4=0 [ok
#   cgins -noplot tcilc -g=cice2.order4 -nu=.01 -tp=.01 -tf=20 -solver=yale -psolver=yale -go=og -ad4=1 -cfl=.25 [ad4 OK with cfl=.25
#   cgins -noplot tcilc -g=cice2.order4 -nu=.01 -tp=.01 -tf=20 -ts=pc4 -solver=yale -psolver=yale -go=og [trouble at inflow
#
# -- multigrid
#   cgins tcilc -g=tcilce2.order2.ml2 -nu=.01 -tp=.1 -tf=20 -solver=mg -psolver=mg    [ok]
#   cgins tcilc -g=tcilce4.order2.ml3 -nu=1.e-3 -tp=.1 -tf=20 -solver=mg -psolver=mg  
#
# -- multigrid + order 4:
#   cgins tcilc -g=tcilce2.order4.ml2 -nu=1.e-2 -tp=.1 -tf=20 -solver=mg -psolver=mg [ok
#   cgins tcilc -g=tcilce4.order4.ml3 -nu=1.e-3 -tp=.1 -tf=20 -solver=mg -psolver=mg [ok
#   cgins tcilc -g=tcilce16.order4.ml3 -nu=5.e-5 -tp=.001 -tf=20 -solver=mg -psolver=mg -debug=3 [1.2M , ok
#   cgins tcilc -g=tcilce32.order4.ml4 -nu=1.e-5 -tp=.001 -tf=20 -solver=mg -psolver=mg -debug=3 [5.M, trouble: rtol?
#
# -- parallel
#   mpirun -np 4 $cginsp tcilc -g=tcilce2.order2 -nu=.01 -tp=.2 -tf=20    [ok]
#   mpirun -np 2 $cginsp tcilc -g=tcilce2.order2.ml2 -nu=1.e-2 -tp=.1 -tf=20 -solver=mg -psolver=mg  [n=1,2,4 ok
#
# -- parallel + order 4:
#  mpirun -np 2 $cginsp -noplot tcilc -g=tcilce2.order4.ml2 -ts=pc -nu=1.e-2 -tp=.01 -tf=10 -rtolp=1.e-5 -rtol=1.e-5 -solver=best -psolver=mg -go=og [trouble Inflow?
#  mpirun -np 2 $cginsp -noplot tcilc -g=tcilce2.order4.ml2 -ts=pc4 -nu=1.e-2 -tp=.01 -tf=10 -rtolp=1.e-5 -rtol=1.e-5 -solver=best -psolver=mg -go=og [trouble Inflow?
#  -- implicit:
#  mpirun -np 1 $cginsp -noplot tcilc -g=tcilce2.order4.ml2 -nu=1.e-2 -tp=.01 -tf=10 -rtolp=1.e-4 -rtol=1.e-5 -solver=best -psolver=best -go=og [OK
#  mpirun -np 1 $cginsp -noplot tcilc -g=tcilce2.order4.ml2 -nu=1.e-2 -tp=.01 -tf=10 -rtolp=1.e-5 -rtol=1.e-5 -solver=best -psolver=mg -go=og [OK
#  mpirun -np 2 $cginsp -noplot tcilc -g=tcilce2.order4.ml2 -nu=1.e-2 -tp=.01 -tf=10 -rtolp=1.e-5 -rtol=1.e-5 -solver=best -psolver=mg -go=og [OK
# 
#  mpirun -np 1 $cginsp -noplot tcilc -g=tcilce2.order4.ml2 -nu=1.e-2 -tp=.01 -tf=20 -rtolp=1.e-5 -rtol=1.e-5 -solver=mg -psolver=mg -go=og [trouble: line solver BC for 4th order?  
#
# === bug:
# cgins -noplot tcilc -g=tcilce4.order4.ml3 -nu=1.e-3 -tp=.1 -tf=20 -solver=best -rtol=1.e-6 -rtolp=1.e-5 -psolver=mg -go=og [TROUBLE: nans by t=.3 --> nu TOO small, nu=1.e-2 ok
# cgins -noplot tcilc -g=tcilce2.order4.ml2 -nu=.05 -tp=.1 -tf=20 -solver=best -psolver=mg -go=og [OK
# ********************************************************************************************************
#
$grid="tcilce2.order2"; $show = " "; $tFinal=5.; $tPlot=.1; $nu=.1; $cfl=.9; $inflow="uniform"; 
# 
$implicitVariation="viscous"; $impGrids="all=explicit"; $newts=0;
$debug = 1;  $debugp=0; $debugi=0; $opav=1; $ssr=0; $plotResiduals=0; 
$maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.05; 
$fullSystem=0; $go="halt"; $move=0;  $moveOnly=0; $freq=1.; 
$show=" "; $restart="";  $outflowOption="neumann"; 
$psolver="choose best iterative solver"; $solver="choose best iterative solver"; 
$iluLevels=1; $ogesDebug=0; $project=1; 
$ts="im"; 
$implicitFactor=.5;
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$cdv=1.; $ad2=0; $ad21=1.; $ad22=1.;  $ad4=0; $ad41=1.; $ad42=1.; 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
$refactorFrequency=10000; $recomputeDt=10000; 
$cpn=1.; 
# -- for Kyle's AF scheme:
$afit = 10;  # max iterations for AFS
$aftol=1e-2;
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$cdv=1;  $cDt=.25;
# -- for steady state solver
$maxIterations=200; $plotIterations=50; 
$ogmgAutoChoose=1;
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "psolver=s"=>\$psolver, "tz=s"=>\$tz, "show=s"=>\$show,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "implicitVariation=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"implicitFactor=f"=>\$implicitFactor,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
 "freqFullUpdate=i"=>\$freqFullUpdate,"move=i"=>\$move,"moveOnly=i"=>\$moveOnly,\
  "freq=f"=>\$freq,"debugp=i"=>\$debugp,"debugi=i"=>\$debugi,"opav=i"=>\$opav,"ssr=i"=>\$ssr,"recomputeDt=i"=>\$recomputeDt,\
  "refactorFrequency=i"=>\$refactorFrequency,"ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,\
  "ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,"outflowOption=s"=>\$outflowOption,"cpn=f"=>\$cpn,\
  "ogmgAutoChoose=i"=>\$ogmgAutoChoose,"inflow=s"=>\$inflow,"iluLevels=i"=>\$iluLevels,\
  "maxIterations=i"=>\$maxIterations,"plotIterations=i"=>\$plotIterations,"plotResiduals=i"=>\$plotResiduals);
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
if( $ts eq "pc4" ){ $ts="adams PC order 4"; }
if( $ts eq "mid"){ $ts="midpoint";       }  
if( $ts eq "afs"){ $ts="approximate factorization"; $newts = 1;}
if( $ts eq "ss"){ $ts="steady state RK-line"; }
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous\n useNewImplicitMethod"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized\n useNewImplicitMethod"; }\
else{ $implicitVariation = "implicitFullLinearized\n useNewImplicitMethod"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
if( $project eq "1" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $opav eq "0" ){ $opav = "do not average coarse grid equations"; }\
  elsif( $opav eq "2" ){ $opav ="do not average coarse curvilinear grid equations"; }else{ $opav = "#"; }
if( $ssr eq 1 ){ $ssr="show smoothing rates"; }else{ $ssr="#"; }
# old:
#* $tFinal=50.; $tPrint=.1; $nu=.1;
#* *
#* * two cylinders in a long channel:
#* $gridName="tcilce2.order2.hdf"; $nu=.01; 
#* * $gridName="tcilc1e.hdf"; $nu=.02;
#* * $gridName="tcilc2e.hdf"; $nu=.01;
#* * $gridName="tcilc3.hdf"; $nu=.005;  
#* *
#* * $gridName="square20"; 
#* * $gridName="cic2"; 
#
$grid
#
  incompressible Navier Stokes
  exit
#
  turn off twilight zone 
#
  refactor frequency $refactorFrequency
  implicit factor $implicitFactor 
  $implicitVariation
  dtMax $dtMax
#  -- choose time-stepping method:
  $ts
  $newts
  # -- for the AFS scheme:
  compact finite difference
  # -- convergence parameters for the af scheme
  max number of AF corrections $afit
  AF correction relative tol $aftol
  # optionally turn this on to improve stability of the high-order AF scheme by using 2nd-order dissipation at the boundary
  OBPDE:use boundary dissipation in AF scheme 1
  # -- for steady state solver:
  max iterations $maxIterations
  plot iterations $plotIterations
  plot residuals $plotResiduals
#
##  OBPDE:use new fourth order boundary conditions 1
#
  choose grids for implicit
   all=implicit
   # square=explicit
  done
#
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
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
  no plotting
  plot and always wait
#
    maximum number of iterations for implicit interpolation
      10
#
  recompute dt every $recomputeDt
  refactor frequency $refactorFrequency
# 
  pde parameters
    nu
     $nu 
    #  turn on 2nd-order AD here:
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22 $ad21 , $ad22
    OBPDE:fourth-order artificial diffusion $ad4
    OBPDE:ad41,ad42 $ad41 , $ad42
    if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
    $cmd
  done
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogesDebug=$debugp; $ogmgDebug=$debugp; $ogmgCoarseGridSolver="best"; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; $ogesDebug=$debugi;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
#
  boundary conditions
#
    all=noSlipWall
#    all=slipWall
    $cmd ="square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.)"; 
    if( $inflow eq "ramp" ){ $cmd="square(0,0)=inflowWithVelocityGiven, ramp(ta=0.,tb=1.,ua=0.,ub=1.)"; }
    $cmd 
#    -- can be trouble for 1.*p+0.01*p.n=0.
    square(1,0)=outflow , pressure(1.*p+$cpn*p.n=0.)
    square(0,1)=slipWall
    square(1,1)=slipWall
#
    done
  initial conditions
  uniform flow
    p=1., u=1.
  done
  $project
  debug
   $debug
  continue
  $go



  movie mode
  finish
