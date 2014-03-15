*
* cgad -- test the twilightzone
*
* Usage:
*   
*  cgad [-noplot] tz -g=<name> -tz=[poly|trig|pulse] -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
*         -go=<go/halt/og> -kappa=<value> -solver=<yale/best> -order=<2/4> -ts=[adams2|euler|implicit|midPoint] ...
*         -a=<val> -b=<val> -bc=<d|m|n> -amr=[0|1] -useNewStep=[0|1] -varCoeff=[poly]
*
*  bc : d=dirichlet, m=mixed boundary condition
*  useNewStep : 1=use new advance steps time stepping routines
* 
* Examples:
* 
*  cgad tz -g=square10 -degreex=2 -degreet=2 
*  cgad -noplot tz -g=square10 -degreex=2 -degreet=0 -kappa=.5 -ts=implicit -go=og
*  cgad -noplot tz -g=square5 -degreex=2 -degreet=2 -kappa=1. -go=go -tp=.01 -tf=.05     [ exact 
*  cgad -noplot tz -g=square5 -degreex=2 -degreet=2 -kappa=1. -ts=implicit -go=go -tp=.01 -tf=.05
* 
*  cgad tz -g=square5 -degreex=2 -degreet=2 -kappa=1. -ts=implicit -go=go -tp=.01 -tf=.05
* 
*  cgad -noplot tz -g=square5 -degreex=2 -degreet=0 -kappa=1. -ts=implicit -go=go -tp=.1 -tf=.5
* 
*  cgad noplot tz -g=nonSquare8 -degreex=2 -degreet=2 
*  cgad tz -g=sise -degreex=2 -degreet=2 
*  cgad tz -g=sise -degreex=2 -degreet=2 -ts=implicit -bc=m
*  cgad noplot tz -g=cice -degreex=2 -degreet=2 
*  cgad tz -g=cic4 -tz=trig 
*  cgad tz -g=square20.order4 -order=4 -degreex=4 -degreet=2 
* 
*  cgad noplot tz -g=box10 -degreex=2 -degreet=2 -solver=best
*  cgad noplot tz -g=nonBox5 -degreex=2 -degreet=2 -solver=best
*  cgad noplot tz -g=bibe -degreex=2 -degreet=2 -solver=best
* -- pulse with amr:
*  cgad tz -g=square40 -tz=pulse -tp=.05 -amr=1 -ts=euler -amrTol=.05 -useNewStep=1
*  cgad tz -g=square40 -tz=pulse -tp=.05 -amr=1 -ts=euler -amrTol=.05 -useNewStep=1 -levels=3
*
* --- variable coefficients  ----
#  cgad -noplot tz -g=square20.order2 -degreex=2 -degreet=2 -kappa=1. -tp=.1 -tf=1. -varCoeff=poly -go=og
# 
* --- ADI ----
*  cgad tz -g=square10 -degreex=1 -degreet=0 -tp=.1 -ts=adi -go=halt -a=0. -b=0. -debug=15
*  cgad noplot tz -g=square5 -degreex=2 -degreet=0 -tp=.01 -tf=.05 -ts=adi -go=go -a=0. -b=0. -debug=15
* 
* --- set default values for parameters ---
* 
$tFinal=1.; $cfl=.9; $kappa=.1;  $kThermal=.1; 
$ts="adams PC"; $noplot=""; $go="halt"; $a=1.; $b=1.; $c=0.; 
$debug = 0;  $tPlot=.1; $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $bc="d"; 
$order = 2; $useNewStep=0; $varCoeff=""; 
$amr=0; $ratio=2; $levels=2; $bufferZones=2; $amrTol=1.e-2; 
$xPulse=.5; $yPulse=.5; $zPulse=0.;
* 
$solver="yale"; $ogesDebug=0; $ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
* $ksp="gmres"; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "kappa=f"=>\$kappa,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"amr=i"=>\$amr, \
 "ts=s"=>\$ts, "noplot=s"=>\$noplot, "go=s"=>\$go,"debug=i"=>\$debug,"a=f"=>\$a,"b=f"=>\$b,"c=f"=>\$c,\
 "bc=s"=>\$bc, "amrTol=f"=>\$amrTol, "useNewStep=i"=>\$useNewStep,"levels=i"=>\$levels,"varCoeff=s"=>\$varCoeff );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="turn on polynomial"; }elsif( $tz eq "trig" ){ $tz="turn on trigonometric"; }\
               else{ $tz="OBTZ:pulse"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $ts eq "adams2" ){ $ts = "adams PC"; }
if( $ts eq "euler" ){ $ts = "forward Euler"; }
if( $ts eq "midPoint" ){ $ts = "midpoint"; }
if( $amr eq "0" ){ $amr="turn off adaptive grids"; }else{ $amr="turn on adaptive grids"; }
if( $go eq "go" ){ $go = "movie mode\n finish"; }
if( $go eq "halt" ){ $go = "*"; }
if( $go eq "og" ){ $go = "open graphics"; }
*
* 
$grid
* 
* $grid = "square20";
* $grid="cice.hdf";
* $grid = "box10";
* $grid="innerOuter3d2.hdf"; $tPlot=.001;
* $grid="bib.hdf"; $tPlot=.01; 
* $grid="boxsbs1.hdf"; $tPlot=.01; $debug=31; $degreeSpace=0; $degreeTime=1;
* 
* test mixed boundaries: 
* $grid="matchingSquares"; $degreeSpace=1; $degreeTime=1; $tPlot=.01
*
$grid
* 
  advection diffusion
* 
  continue
*
  $tz
  OBTZ:degree in space $degreex
  OBTZ:degree in time $degreet
  frequencies (x,y,z,t)   $fx $fy $fz $ft
  OBTZ:pulse center $xPulse $yPulse $zPulse
  OBTZ:pulse velocity 1 0 0
  OBTZ:pulse amplitude, exponent, power 1 60 1
*
* -- time-stepping method --
  $ts
  implicit factor .5 (1=BE,0=FE)
*  implicit factor 0. (1=BE,0=FE)
  if( $useNewStep eq 1 ){ $cmd ="use new advanceSteps versions"; }else{ $cmd="#"; }
  $cmd
* 
  choose grids for implicit
    all=implicit
  done
* 
  dtMax $tPlot
* 
  pde parameters
    kappa $kappa
    a $a
    b $b
    c $c
#
    $cmd="#"; $dct=0; $dcx=2; 
    if( $varCoeff eq "poly" ){ $cmd="OBPDE:user defined coefficients\n polynomial coefficients\n $dct $dcx\n done"; }
    $cmd 
  done
* 
  boundary conditions
    if( $bc eq "d" ){ $bcCommand="all=dirichletBoundaryCondition"; }elsif( $bc eq "m"){ $bcCommand="all=mixedBoundaryCondition, mixedDerivative(1.*t+1.*t.n=0.)"; }else{ $bcCommand="all=neumannBoundaryCondition"; }
    * ****
    $bcCommand="all=dirichletBoundaryCondition\n square(1,0)=mixedBoundaryCondition";
    $bcCommand
  done
*
   implicit time step solver options
    $solver
     * parallel bi-conjugate gradient stabilized
**     lu preconditioner
*
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
* 
*****  Here we optionally turn on AMR *******
  $amr
  order of AMR interpolation
     3
  error threshold
     $amrTol
  regrid frequency
    $regrid=$ratio*$bufferZones;
    $regrid
  change error estimator parameters
    set scale factors
      1 1 1 1 1 1 
    done
    weight for first difference
      1.
    weight for second difference
      1.
    exit
    truncation error coefficient
      1.
    show amr error function
  change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $levels
    number of buffer zones
      $bufferZones
    grid efficiency
      .7
  exit
*****
 debug $debug
 final time $tFinal
 times to plot $tPlot
 done
 continue
 $go

