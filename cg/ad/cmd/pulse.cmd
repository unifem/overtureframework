#
#   cgad example: advection-diffusion of a pulse (demonstrating user defined initial conditions)
# 
# Usage:
#   
#  cgad [-noplot] tz -g=<name> -tz=<poly/trig> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
#         -go=<go/halt/og> -kappa=<value> -solver=<yale/best> -order=<2/4> -ts=<adams2/euler/implicit> ...
#         -a=<val> -b=<val> -nc=<>
# 
# Examples:
# 
#      cgad pulse -g=square64.order2 -a=1. -b=0. -kappa=.01 -go=halt
# 
#  -- assign default values for parameters ---
$tFinal=5.; $tPlot=.1; $cfl=1.; $kappa=.1;  $kThermal=.1;  $a=1.; $b=1.; $c=1.; $nc=1; 
$x0=.25; $y0=.5; $z0=0.; $amp=1.; $alpha=40.;   # pulse parameters
$grid="cic"; $ts="adams PC"; $noplot=""; $go="halt"; $order = 2;  $bc="cic"; $show=""; 
$debug = 0;  $maxIterations=100; $tol=1.e-9; $atol=1.e-10; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.;
$solver="yale"; $ogesDebug=0; $ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
# $ksp="gmres"; 
#
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "kappa=f"=>\$kappa,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"nc=i"=>\$nc, \
 "ts=s"=>\$ts, "noplot=s"=>\$noplot, "go=s"=>\$go,"debug=i"=>\$debug,"a=f"=>\$a,"b=f"=>\$b,"bc=s"=>\$bc,\
 "x0=f"=>\$x0,"y0=f"=>\$y0,"z0=f"=>\$z0,"amp=f"=>\$amp,"alpha=f"=>\$alpha,"show=s"=>\$show );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="turn on polynomial"; }else{ $tz="turn on trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $ts eq "adams2" ){ $ts = "adams PC"; }
if( $ts eq "euler" ){ $ts = "forward Euler"; }
if( $go eq "go" ){ $go = "movie mode\n finish"; }
if( $go eq "halt" ){ $go = " "; }
if( $go eq "og" ){ $go = "open graphics"; }
# 
# 
$grid
# 
  convection diffusion
  number of components $nc 
  continue
# 
##  turn on memory checking
# 
  turn off twilight zone 
 # turn on trig
  final time $tFinal
  times to plot $tPlot
  plot and always wait
 # no plotting
# Next specify the file to save the results in. 
# This file can be viewed with Overture/bin/plotStuff.
  show file options
     compressed
      open
       $show
    frequency to flush
      100
    exit
#
  pde parameters
    kappa $kappa
    a $a
    b $b
    c $c
  done
# -- time-stepping method --
  $ts
  implicit factor .5 (1=BE,0=FE)
# 
  choose grids for implicit
    all=implicit
  done
# 
  dtMax $tPlot
  debug $debug
  cfl $cfl
# 
  boundary conditions
   # all=dirichletBoundaryCondition
    all=neumannBoundaryCondition
   done
#
   implicit time step solver options
    $solver
 # parallel bi-conjugate gradient stabilized
#*     lu preconditioner
#
     maximum number of iterations
      $maxIterations
     relative tolerance
       $tol
     absolute tolerance
       1.e-12
     maximum number of iterations
       $maxIterations
     debug 
       $ogesDebug
    exit
# 
  initial conditions
      OBIC:user defined...
        pulse
        $x0 $y0 $z0 $amp $alpha
        exit
  continue
continue
$go 
