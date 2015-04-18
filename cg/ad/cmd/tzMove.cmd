#
# cgad: moving grid examples
#
#
# Usage:
#   
#  cgad [-noplot] tzMove -g=<name> -tz=<poly/trig> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
#         -go=<go/halt/og> -kappa=<value> -solver=<yale/best> -order=<2/4> -ts=<adams2/euler/implicit> ...
#         -gridToMove=<name> -move=<shift/rotate> -ref=<fixed/rigid> -a=<val> -b=<val>
# 
#   -ts : time-stepping, euler, adams2, implicit. 
#   -ref : reference frame
# 
# Examples:
#    cgad tzMove -g=cice2.order2 -gridToMove="Annulus" -move=shift
#    cgad tzMove -g=stir -gridToMove="stir" -move=rotate
#    cgad tzMove -g=square20 -gridToMove="square" -move=shift -ref=rigid
#    cgad tzMove -g=innerOuter2d -gridToMove="all" -move=shift -ref=rigid
# 
# $grid="sis.hdf"; $gridToMove="inner-square"; 
#
# $grid="square5.hdf"; $gridToMove="square"; 
# $grid="square20.hdf"; $gridToMove="square"; $tPlot=.1; 
#
# --- set default values for parameters ---
# 
$tFinal=1.; $tPlot=.025; $cfl=.9; $kappa=.1;  $kThermal=.1; $dtMax=0.1; $show = " "; 
$ts="adams2"; $noplot=""; $go="halt"; $a=1.; $b=1.; $c=0.;  
$debug = 0;  $tPlot=.1; $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.;
$order = 2; $gridToMove="square"; $move="shift"; $refFrame="fixed"; 
$bdfOrder=2; 
# 
$solver="yale"; $ogesDebug=0; $ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
# $ksp="gmres"; 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "kappa=f"=>\$kappa,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"move=s"=>\$move,"ref=s"=>\$ref, \
 "ts=s"=>\$ts, "noplot=s"=>\$noplot, "go=s"=>\$go,"debug=i"=>\$debug,"gridToMove=s"=>\$gridToMove,"a=f"=>\$a,"b=f"=>\$b,\
 "bdfOrder=i"=>\$bdfOrder,"cfl=f"=>\$cfl,"dtMax=f"=>\$dtMax );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="turn on polynomial"; }else{ $tz="turn on trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $ts eq "adams2" ){ $ts = "adams PC"; }
if( $ts eq "pc" ||  $ts eq "pc2" ){ $ts = "adams PC"; }
if( $ts eq "euler" ){ $ts = "forward Euler"; }
if( $ts eq "bdf" ){ $ts = "BDF"; }
if( $ts eq "implicit" ){ $ts = "implicit"; }
if( $go eq "go" ){ $go = "movie mode\n finish"; }
if( $go eq "halt" ){ $go = " "; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $move eq "shift" ){ $move="translate\n  1. 0. 0.\n 1. 0. 0."; }
if( $move eq "rotate" ){ $move="rotate\n  0. 0. 0.\n .5 .0"; }
if( $ref eq "fixed" ){ $ref="fixed reference frame"; }
if( $ref eq "rigid" ){ $ref="rigid body reference frame"; }
#
$grid
# 
  convection diffusion
  $ref 
 # fixed reference frame
 # rigid body reference frame
#
  exit
  show file options
 # compressed
    open
     $show
    frequency to flush
      5
    exit
#*  turn off twilight zone
#*  project initial conditions
#
#   On a translating grid: x(r,t) = a*r + b*t   --> Poly(x)*Poly(t) --> Poly(a*r+b*t)*Poly(t)
#     so TZ function is a higher degree in time. degreeSpace=1, degreeTime=1 should be exact
  degree in space $degreex
  degree in time $degreet
#
  final time $tFinal
  times to plot $tPlot
  plot and always wait
 # no plotting
# 
  pde parameters
    kappa $kappa
    a $a
    b $b
    c $c
  done
# 
  turn on moving grids
# 
  specify grids to move
    $move
    $gridToMove
    done
#
    debug: $debug 
  done
 # use implicit time stepping
  $ts
  BDF order $bdfOrder
  cfl $cfl
  dtMax $dtMax
  #
  choose grids for implicit
    all=implicit
#      all=explicit
#      stir=implicit
   done
# 
  debug $debug 
  boundary conditions
    all=dirichletBoundaryCondition
    done
#   initial conditions
#     uniform flow
#      p=1.
#   exit
# 
  continue
 $go