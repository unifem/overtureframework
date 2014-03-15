*
* cgad: test axisymmetric advection-diffusion 
*
* Usage:
*   
*  cgad [-noplot] axisym -g=<name> -tz=<poly/trig/none> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
*        -bc=<a|d|r>  -solver=<yale/best> -order=<2/4> -ts=<implicit> -debug=<num> ..,
*        -ad2=<0|1> -bg=<backGround> -project=<0/1> -imp=<val> -go=[run/halt/og]
* 
*  -imp : .5=CN, 1.=BE, 0.=FE
*  -go : run, halt, og=open graphics
*  -bc : d=dirichlet, a=dirichlet+axisymmetric, r=real BC's  
* 
* Examples: (offsetSquare grids are made with rectangle.cmd)
* 
*  cgad axisym -g=offsetSquare5 -a=1. -b=1. -kappa=.1 -tf=.2 -tp=.05 -tz=poly -bc=d
*  cgad axisym -g=squareOnAxis5 -a=1. -b=1. -kappa=.1 -tf=.2 -tp=.05 -tz=poly 
*  cgad axisym -g=shortHalfCylinder2 -a=1. -b=1. -kappa=.1 -tf=2. -tp=.1 -tz=trig 
* 
* implicit:
*  cgad axisym -g=offsetSquare5 -a=1. -b=1. -kappa=.1 -tf=1. -tp=.1 -dtMax=.1 -tz=poly -degreet=1 -ts=implicit -bc=d
*
$grid="halfCylinder.hdf"; $backGround="square"; 
$tFinal=1.; $tPlot=.1; $cfl=.9; $kappa=.05; $a=1.; $b=1.; 
$ts="adams PC"; $noplot=""; $imp=.5; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; 
$solver="yale"; $ogesDebug=0;
$bc="a"; 
* 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, \
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"kappa=f"=>\$kappa,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,\
 "dtMax=f"=>\$dtMax,"imp=f"=>\$imp,"bc=s"=>\$bc,"a=f"=>\$a,"b=f"=>\$b );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
* 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
* specify the overlapping grid to use:
$grid
* Specify the equations we solve:
  advection diffusion
  done
* 
* -- twilightzone options:
  $tz
  degree in space $degreex
  degree in time $degreet
  frequencies (x,y,z,t)   $fx $fy $fz $ft
* 
  pde parameters
    kappa $kappa
    a $a
    b $b
    c  .0
  done
* choose the time stepping:
  $ts
  implicit factor $imp (1=BE,0=FE)
* 
  choose grids for implicit
    all=implicit
*     square=explicit
    done
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  dtMax $dtMax
* 
* 
  plot and always wait
  * no plotting
* 
 turn on axisymmetric flow
* 
  boundary conditions
    if( $bc eq "d" ){ $cmd = "all=dirichletBoundaryCondition\n done"; }\
    elsif( $bc eq "a" ){ $cmd = "all=dirichletBoundaryCondition\n bcNumber13=axisymmetric\n done"; }\
    else{ $cmd = "done"; }
    $cmd
  done
  initial conditions
   if( $tz eq "turn off twilight zone" ){ $ic = "OBIC:uniform state T=0.\n OBIC:assign uniform state\n done";}else{ $ic = "done"; }
   $ic 
  debug $debug
  exit
  $go
