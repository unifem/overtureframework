# *******************************************************
# cgins command file: 
#     Flow through a periodic channel with an imposed pressure gradient
#
# Usage:
#    cgins [-noplot] channel -g=<gridName> -tf=<> -tp=<> -nu=<> -solver=[best|yale|mg] -psolver=[best|yale|mg] -u0=<> -u1=<> ...
#                           -ax=<> -ay=<> -pgf=<>
# 
# Parameters:  
#  nu : viscosity
#  u0 : magnitude of Poiseuille flow 
#  u1 : velocity of the upper wall
#  u2 : magnitude of velocity perturbation (see below)
#  ax,ay : define the perturbation (below)
#  pgf : pressure gradient is $nu*u0*$pgf 
# 
#  Couette-Poiseuille flow with a divergence free perturbation
#   u = u0*(y-ya)(yb-y)/[.5*(yb-ya)]^2 + u1*(y-ya)/(yb-ya) 
#     + u2*      sin(ax*pi*x/(yb-ya))*cos(ay*pi*(y-ya)/(yb-ya))
#   v =-u2*ax/ay*cos(ax*pi*x/(yb-ya))*sin(ay*pi*(y-ya)/(yb-ya))
#
# Examples:
#   cgins channel -g=channel5.order2.hdf -nu=.005 -tf=20. -tp=1. -u2=.1 -axp=2. -ay=2. -show="channel5.show" -go=halt
#   cgins -noplot channel -g=channel10.order2.hdf -nu=1.e-4 -tf=20. -tp=.5 -u2=.1 -axp=4. -ay=2. -show="channel10.show" -go=go >! channel10.out
#     channel20 = 1.2M pts 
#   cgins -noplot channel -g=channel20.order2.hdf -nu=2.e-5 -tf=40. -tp=.5 -u2=.1 -ax=4. -ay=2. -show="channel20.show" -go=go >! channel20.out
# 
# -- periodic channel with a pressure gradient:
#   cgins cylinder -g=cicpe2.order2.hdf -tf=2. -tp=.1 -nu=.1 -pressureGradient=.1 -go=halt
#
# -- implicit time stepping:
#   cgins channel -g=channel5.order2.hdf -nu=1.e-2 -tf=20. -tp=.1 -u2=.1 -ax=2. -ay=2. -ts=im -debug=3 -go=halt
#   cgins channel -g=channel10.order2.hdf -nu=1.e-4 -tf=20. -tp=.1 -u2=.1 -ax=4. -ay=2. -ts=im -debug=3 -go=halt
# 
# -- parallel (works, but petsc has some trouble at start with the singular problem-- need to look into this)
#   srun -N1 -n4 -ppdebug $cginsp channel -g=channel10.order2.hdf -nu=1.e-4 -tf=40. -tp=.5 -u2=.1 -ax=4. -ay=2. -debug=3
# 
# -- multigrid solver: *not quite working yet* (We need to compute null vectors the first time we solve on a given grid)
#   cgins channel -g=channel5.order2.ml3.hdf -nu=1.e-2 -tf=20. -tp=.01 -u2=.1 -ax=4. -ay=2. -psolver=mg -rtolp=1.e-5 -debug=3 -debugmg=3 -go=halt
# 
# -- turbulence model:
#   cgins channel -g=channel5.order2.hdf -nu=.005 -tf=20. -tp=1. -u2=.1 -axp=2. -ay=2. -show="channel5.show" -go=halt
#
# -- wall model
#   cgins cmd/channel.cmd -g=channel1.order2 -ts=afs -u2=0 -nu=1e-3 -ay=1 -ax=1 -ad2=1 -ad4=0 -extrabc='$ebc=qq(all=penaltyBoundaryCondition,penaltyWallFunctionBC\nincludeAD 1\nwernerWengle\nnoSlipWall 1\ndone);' -ad21=0 -ad22=0 -show="channel.f1.ww.show" noplot nopause -tp=1
#
# *******************************************************
# -- assign default values for all parameters: 
$tFinal=30.; $tPlot=.1; $nu=.1; $show=" "; $debug=0; $debugmg=0; $dtMax=.02; 
$rtol=1.e-4; $atol=1.e-4; $solver="best"; 
$rtolp=1.e-3; $atolp=1.e-4; $psolver="best";
$pgf=8.; $pressureGradient=$nu*$u0*$pgf; $nullVector="leftNullVector.hdf";
$ts="pc"; $go="halt";
$ad2=0; $ad21=1.; $ad22=1.;   # for 2nd-order artificial dissipation, ad2=0 means no dissipation
$ad4=0; $ad41=1.; $ad42=1.;   # for 4th-order artificial dissipation, ad4=0 means no dissipation
#
$pi=4.*atan2(1.,1.);
$length=2.*$pi; 
$u0=1.; $u1=0.; $u2=0.; $ax=2./$pi; $ay=1.; $ya=-1.; $yb=1.; $axp=-1.;  $yb=1.;
$pressureGradient=$nu*$u0*8.;
$extrabc="#";
# 
# $grid="channel1.order2.hdf"; $nu=0.1;  $pressureGradient=$nu*$u0*8.;
# $grid="channel2.order2.hdf"; $nu=.0002; $pressureGradient=$nu*$u0*8.; $u2=.1; $tPlot=.1; $show="channel.show";
# $grid="channel4.order2.hdf"; $nu=.0001; $pressureGradient=$nu*$u0*8.; $ax=4./$pi; $u2=.1; $tPlot=.5; $show="channel.show";
# $grid="channel5.order2.hdf"; $nu=.0002; $pressureGradient=$nu*$u0*8.; $u2=.1; $tPlot=.5; $show="channel.show";
# $grid="channel5.order2.hdf"; $nu=.0001; $pressureGradient=$nu*$u0*8.; $u2=.1; $tPlot=.5; $show="channel.show";
# $grid="channel10.order2.hdf"; $nu=.0001; $pressureGradient=$nu*$u0*8.; $u2=.2; $ax=10; $ay=10.; $tPlot=.5; $show="channel.show";  
# $grid="channel40.order2.hdf"; $tPlot=.5; $nu=.00001; $pressureGradient=$nu*$u0*8.; $u2=.1; $show="channel.show";
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"tp=f"=>\$tPlot,"u0=f"=>\$u0,"u1=f"=>\$u1,"u2=f"=>\$u2,"ax=f"=>\$ax, "ay=f"=>\$ay,\
 "axp=f"=>\$axp,"pgf=f"=>\$pgf, "model=s"=>\$model,\
 "solver=s"=>\$solver, "psolver=s"=>\$psolver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,\
  "ad4=i"=>\$ad4,"ad41=f"=>\$ad41,"ad42=f"=>\$ad42,\
 "imp=f"=>\$implicitFactor,"rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,\
  "restart=s"=>\$restart,"move=s"=>\$move,"debugmg=i"=>\$debugmg,"nullVector=s"=>\$nullVector,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"outflowOption=s"=>\$outflowOption,\
  "bg=s"=>\$bg,"gridToMove=s"=>\$gridToMove,"bcTop=s"=>\$bcTop,"ogesDebug=i"=>\$ogesDebug,"rate=f"=>\$rate,\
  "extrabc=s"=>\$extrabc);
# -------------------------------------------------------------------------------------------------
# $ax=$ax/$pi;  # scale ax by pi 
if( $axp > 0 ){ $ax = $axp/$pi; } # periodic version of ax 
$pressureGradient=$nu*$u0*$pgf; 
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
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
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "*"; }
if ( $extrabc ne "#" ) {eval($extrabc);} else { $ebc=$extrabc;};
#
# -- here is the grid we use: 
$grid
#
# $u0 = $pressureGradient/$nu; 
#
  incompressible Navier Stokes
  exit
  turn off twilight zone 
#
# ** Warning: with multigrid one must take all grids to be implicit
#* implicit
  choose grids for implicit
   all=implicit
  done
#
  final time $tFinal
  times to plot $tPlot 
#
  show file options
    compressed
    open
      $show
    frequency to flush
      5
    exit
#
  no plotting
  plot and always wait
#plot residuals 1
#  Here is the time stepping method
  $ts
  $newts
  choose grids for implicit
    all=implicit
    done
#
    maximum number of iterations for implicit interpolation
      10
#
  pde parameters
    nu
     $nu 
    #  turn on 2nd-order AD here:
    OBPDE:second-order artificial diffusion $ad2
     OBPDE:ad21,ad22 $ad21 , $ad22
    OBPDE:fourth-order artificial diffusion $ad4
     OBPDE:ad41,ad42 $ad41 , $ad42
  OBPDE:use boundary dissipation in AF scheme 0
    done
# 
  dtMax $dtMax
# 
  user defined forcing
    constant forcing
 # add a forcing to the u equation
      1 $pressureGradient
    done
    exit
#************************************
   pressure solver options
 # PETSc
     $psolver
# 
     relative tolerance
       $rtol
     absolute tolerance
       $atol 
    maximum allowable increase in the residual
    1e8
#     define petscOption -info 1
#      define petscOption -ksp_monitor stdout
#       define petscOption -ksp_monitor stdout
#       define petscOption -ksp_view
#*       define petscOption -on_error_attach_debugger gdb
$nvSolver="choose best iterative solver"; $iluLevels=2;
 $rtolnv=1.e-12; $atolnv=1.e-14;
      multigrid parameters
       # -- options for singular problems: we need to compute a null vector (or use an existing one)
       problem is singular 1
       null vector option:readOrComputeAndSave
       null vector file name:$nullVector
       null vector solve options...
         $nvSolver
          relative tolerance
            $rtolnv
          absolute tolerance
            $atolnv
 #    define petscOption -ksp_type gmres
 #    define petscOption -ksp_type richardson
 #     define petscOption -ksp_type preonly
 #     define petscOption -pc_type lu
          define petscOption -pc_factor_levels $iluLevels
          define petscOption -ksp_monitor stdout
          define petscOption -ksp_view
      exit
 #  alternating
 # cycles: 1=V, 2=W
      $ssr="#";
      if( $debugmg > 1 ){ $ssr="show smoothing rates"; }
      $ssr
      line zebra direction 2
      number of cycles
        2
      residual tolerance
        $rtolp
      error tolerance
        $atolp
      maximum number of interpolation iterations
        4
      debug
        $debugmg
      exit
    exit
#
   implicit time step solver options
     $solver
#-    multigrid
     relative tolerance
       $atol
     absolute tolerance
       $rtol 
#    define petscOption -ksp_monitor stdout
#    block size
#       2
      multigrid parameters
 #  alternating
 # cycles: 1=V, 2=W
      number of cycles
        2
      residual tolerance
        $rtol
      error tolerance
        $atol
      maximum number of interpolation iterations
        4
      debug
        $debugmg
      exit
    exit
#
  boundary conditions
#
    all=noSlipWall
    channel(1,1)=noSlipWall, uniform(u=$u1,v=0)
    $ebc
#
    done
# 
  initial conditions
  OBIC:user defined...
    # Couette-Poiseuille flow with a divergence free perturbation:
    #   u = u0*(y-ya)(yb-y)/[.5*(yb-ya)]^2 + u1*(y-ya)/(yb-ya) 
    #     + u2*      sin(ax*pi*x/(yb-ya))*cos(ay*pi*(y-ya)/(yb-ya))
    #   v =-u2*ax/ay*cos(ax*pi*x/(yb-ya))*sin(ay*pi*(y-ya)/(yb-ya))
    couette profile
 # Enter u0,u1,u2, ax,ay, ya,yb
    $u0 $u1 $u2 $ax $ay $ya $yb 
    exit
##  uniform flow
##    p=0., u=1, v=.0
  done
#  project initial conditions
  debug $debug
  continue
# 
# 
  $go
