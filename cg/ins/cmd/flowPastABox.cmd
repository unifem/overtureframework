# *******************************************************
# cgins command file: 
#     Flow past a 3D box on a surface
#
# Usage:
#    cgins [-noplot] flowPastABox -g=<gridName> -ts=[pc|im] -tf=<> -tp=<> -nu=<> -solver=[best|yale|mg] ...
#                    -psolver=[best|yale|mg] -model=[b|vp] -tm=[les] -gravity=<> -surfaceTemp=<>
# 
# Parameters:  
#  nu : viscosity
#  tf : final time
#  tp : time to plot
#  ts : time-stepping method, pc=explicit predictor-corrector, im=implicit predictor-corrector
#  gravity : e.g. set to "0 -1. 0" to turn on 
#  surfaceTemp : surface temperature
#  tm : turbulence model (les)
#
# Examples: (see surfaceFlow.cmd for more examples)
#
#   cgins flowPastABox -g=loftedHalfBoxe2.order2 -ts=pc -nu=1.e-2 -ad2=1 -tf=20. -tp=.01 
#   cgins flowPastABox -g=loftedHalfBoxe2.order2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.1 
#   cgins flowPastABox -g=loftedHalfBoxe2.order2.ml2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.001  -solver=mg -psolver=mg
# - order=4 
#   TROUBLE: mg + order=4 
#   cgins flowPastABox -g=loftedHalfBoxe2.order4.ml2 -ts=im -nu=1.e-2 -ad4=1 -tf=20. -tp=.001  -solver=mg -psolver=mg
#
# Test:
#  OK: with noSlipWall -- *FIX ME for slip wall*
#  mpirun -np 1 $cginsp flowPastABox -g=nonBibe1.order2.ml2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.001  -solver=mg -psolver=mg -ogesDebug=3 -project=0
#  OK: 
#  mpirun -np 1 $cginsp flowPastABox -g=bibe4.order2.ml3 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.001  -solver=mg -psolver=mg -ogesDebug=3 -project=0
#  OK: 
#  mpirun -np 1 $cginsp flowPastABox -g=bibe1.order2.ml2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.001  -solver=mg -psolver=mg -ogesDebug=7 -project=0
#
#   -- the next needs noSlipWall's but OK
#   mpirun -np 2 $cginsp flowPastABox -g=cice2.order2.ml2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.001  -solver=mg -psolver=mg -ogesDebug=3
#   
# OK: 
#   mpirun -np 1 $cginsp flowPastABox -g=boxBesideBoxe1.order2.ml2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.001  -solver=mg -psolver=mg -ogesDebug=3 -project=0
#   mpirun -np 1 $cginsp flowPastABox -g=nonBibe1.order2.ml2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.001 -solver=mg -psolver=mg -ogesDebug=3 -project=0
# 
#   OK: TURN OFF line smooth *** -> OK 
#   mpirun -np 1 $cginsp flowPastABox -g=sibe1.order2.ml2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.001  -solver=mg -psolver=mg -project=0 -ogmgAutoChoose=0
#   BAD:    
#   mpirun -np 1 $cginsp flowPastABox -g=sibe1.order2.ml2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.001  -solver=mg -psolver=mg -ogesDebug=7 -project=0
#   BAD: 
#   mpirun -np 1 $cginsp flowPastABox -g=sibe2.order2.ml2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.001  -solver=mg -psolver=mg -ogesDebug=7 -project=0
#
#   Trouble:
#   mpirun -np 2 $cginsp flowPastABox -g=loftedHalfBoxe2.order2.ml2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.001  -solver=mg -psolver=mg -ogesDebug=3
#   BAD: 
#   mpirun -np 1 $cginsp flowPastABox -g=loftedHalfBoxe2.order2.ml2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.001  -solver=mg -psolver=mg
#   OK:
#   cgins flowPastABox -g=loftedHalfBoxe2.order2.ml2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.001  -solver=mg -psolver=mg -ogesDebug=3
#   cgins flowPastABox -g=box32.order2 -ts=im -nu=1.e-2 -ad2=1 -tf=20. -tp=.1 -solver=mg -psolver=mg
#
# -- assign default values for all parameters: 
$tFinal=30.; $tPlot=.1; $nu=.1; $show=" "; $debug=0; $ogesDebug=0; $debugmg=0; $dtMax=.02; $order=2; 
$restart=""; $restartSolution=-1; $outflowOption="neumann";
$rtol=1.e-4; $atol=1.e-4; $solver="best"; 
$rtolp=1.e-3; $atolp=1.e-4; $psolver="best"; $iluLevels=3; 
$project=1; $ts="pc"; $go="halt";
$ad2=0; $ad21=1.; $ad22=1.;   # for 2nd-order artificial dissipation, ad2=0 means no dissipation
$ad4=0; $ad41=1.; $ad42=1.;   # for 4th-order artificial dissipation, ad4=0 means no dissipation
$model="#";  $gravity = "0. 0. 0."; $thermalExpansivity=.1;
$tm = "#"; 
$surfaceTemp=0.; # surface temperature
$lesOption=0; $lesPar1=.01; 
$refactorFrequency=500; $ogmgAutoChoose=1; 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"tp=f"=>\$tPlot,"u0=f"=>\$u0,"u1=f"=>\$u1,"u2=f"=>\$u2,"ax=f"=>\$ax, "ay=f"=>\$ay,\
 "axp=f"=>\$axp,"pgf=f"=>\$pgf, "model=s"=>\$model,"tm=s"=>\$tm,\
 "solver=s"=>\$solver, "psolver=s"=>\$psolver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"gravity=s"=>\$gravity,\
  "ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,"restartSolution=i"=>\$restartSolution,\
 "imp=f"=>\$implicitFactor,"rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,\
  "restart=s"=>\$restart,"move=s"=>\$move,"debugmg=i"=>\$debugmg,"nullVector=s"=>\$nullVector,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"outflowOption=s"=>\$outflowOption,\
  "bg=s"=>\$bg,"gridToMove=s"=>\$gridToMove,"bcTop=s"=>\$bcTop,"ogesDebug=i"=>\$ogesDebug,"rate=f"=>\$rate,\
  "surfaceTemp=f"=>\$surfaceTemp,"lesOption=i"=>\$lesOption,"lesPar1=f"=>\$lesPar1,\
  "refactorFrequency=i"=>\$refactorFrequency, "ogmgAutoChoose=i"=>\$ogmgAutoChoose );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
# 
if( $model eq "vp" ){ $model ="visco-plastic model"; }
if( $model eq "b" ){ $model ="Boussinesq model"; }
if( $tm eq "les" ){ $tm ="LargeEddySimulation"; }
if( $ts eq "fe" ){ $ts="forward Euler";  }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       }
if( $ts eq "pc" ){ $ts="adams PC";       }
if( $ts eq "mid"){ $ts="midpoint";       }  
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $order eq "2" ){ $order = "second order accurate"; }\
elsif( $order eq "4" ){ $order = "fourth order accurate"; }\
elsif( $order eq "6" ){ $order = "sixth order accurate";}\
else { $order = "eighth order accurate";}
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# -- here is the grid we use: 
$grid
#
#
  incompressible Navier Stokes
  # Choose Boussineq: 
  $model
  # Choose turbulence model: 
  $tm 
# 
  $nuVP=.0; $etaVP=$nu; $yieldStressVP=0.; $exponentVP=1.; $kThermal=$nu; $epsVP=1.e-3; 
  define real parameter nuViscoPlastic $nuVP
  define real parameter etaViscoPlastic $etaVP
  define real parameter yieldStressViscoPlastic $yieldStressVP
  define real parameter exponentViscoPlastic $exponentVP 
  define real parameter epsViscoPlastic $epsVP
  define real parameter thermalExpansivity $thermalExpansivity
# 
  # Define LES parameters that are accessed by getLargeEddySimulationViscosity.bf 
  define integer parameter lesOption $lesOption
  define real parameter lesPar1 $lesPar1
  exit
  turn off twilight zone 
# -- order of accuracy: 
# $order 
#
# ** Warning: with multigrid one must take all grids to be implicit
#* implicit
  choose grids for implicit
   all=implicit
  done
#
  final time $tFinal
  times to plot $tPlot 
  refactor frequency $refactorFrequency
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
#  Here is the time stepping method
  $ts
# **********
#  useNewImplicitMethod
#
  choose grids for implicit
    all=implicit
    done
#
    maximum number of iterations for implicit interpolation
      10
#
  pde parameters
   nu $nu 
   kThermal $kThermal
   gravity
     $gravity
   #  turn on 2nd-order AD here:
   OBPDE:second-order artificial diffusion $ad2
   OBPDE:ad21,ad22 $ad21 , $ad22
   OBPDE:fourth-order artificial diffusion $ad4
   OBPDE:ad41,ad42 $ad41 , $ad42
   #  OBPDE:check for inflow at outflow
   # This next option will use Neumann BC's on the velocity at outflow
   OBPDE:expect inflow at outflow
   if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
   $cmd
  done
# 
  dtMax $dtMax
# 
#************************************
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
#
    all=slipWall
    # all=noSlipWall
    # bc=3 : base 
    bcNumber3=noSlipWall, uniform(T=$surfaceTemp)
    # bc=7: box: 
    bcNumber7=noSlipWall, uniform(T=$surfaceTemp)
    bcNumber1=inflowWithVelocityGiven, parabolic(d=.10,p=1,u=1.,T=1.)
##    bcNumber1=inflowWithVelocityGiven, uniform(d=.10,p=1,u=1.,T=1.)
    bcNumber2=outflow,  pressure(1.*p+1.*p.n=0.)
#    bcNumber2=outflow,  pressure(1.*p+5.*p.n=0.)
#
    done
# 
# initial conditions: uniform flow or restart from a solution in a show file 
if( $restart eq "" ){ $cmds = "uniform flow\n u=1., v=0., p=1."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number $restartSolution \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
  if( $project eq "1"  && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
  $project
  debug $debug
  continue
# 
# 
  $go
