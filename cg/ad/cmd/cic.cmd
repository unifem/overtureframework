*
*   cgad example: advection-diffusion around a body
* 
* Usage:
*   
*  cgad [-noplot] tz -g=<name> -tz=<poly/trig> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
*         -go=<go/halt/og> -kappa=<value> -solver=<yale/best> -order=<2/4> -ts=<adams2/euler/implicit> ...
*         -a=<val> -b=<val> -nc=<>
* 
* Examples:
* 
*      cgad cic -g=cic.hdf -bc=cic -go=halt
*      cgad cic -g=cice4.order2.hdf -bc=cic -ts=implicit               [ circle-in-a-channel, implicit time stepping ]
*      cgad cic -g=sibe2.order2.hdf -ts=implicit -bc=sib -solver=best  [ sphere-in-a-box, implicit time stepping ]
*
*  mpirun -np 2 $cgadp cic -g=cice2.hdf -bc=cic -go=halt
* 
*  -- assign default values for parameters ---
$tFinal=5.; $tPlot=.1; $cfl=1.; $kappa=.1;  $kThermal=.1;  $a=1.; $b=1.; $c=1.; $nc=1; 
$grid="cic"; $ts="adams PC"; $noplot=""; $go="halt"; $order = 2;  $bc="cic";
$debug = 0;  $maxIterations=100; $tol=1.e-9; $atol=1.e-10; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.;
$solver="yale"; $ogesDebug=0; $ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
* $ksp="gmres"; 
*
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "kappa=f"=>\$kappa,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"nc=i"=>\$nc, \
 "ts=s"=>\$ts, "noplot=s"=>\$noplot, "go=s"=>\$go,"debug=i"=>\$debug,"a=f"=>\$a,"b=f"=>\$b,"bc=s"=>\$bc );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="turn on polynomial"; }else{ $tz="turn on trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $ts eq "adams2" ){ $ts = "adams PC"; }
if( $ts eq "euler" ){ $ts = "forward Euler"; }
if( $go eq "go" ){ $go = "movie mode\n finish"; }
if( $go eq "halt" ){ $go = " "; }
if( $go eq "og" ){ $go = "open graphics"; }
* 
* 
$grid
* 
  convection diffusion
  number of components $nc 
  continue
* 
  turn on memory checking
* 
  turn off twilight zone 
  * turn on trig
  final time $tFinal
  times to plot $tPlot
  plot and always wait
  * no plotting
  pde parameters
    kappa $kappa
    a $a
    b $b
    c $c
  done
* -- time-stepping method --
  $ts
  implicit factor .5 (1=BE,0=FE)
* 
  choose grids for implicit
    all=implicit
  done
* 
  dtMax $tPlot
  debug $debug
  cfl $cfl
* 
  boundary conditions
    * all=dirichletBoundaryCondition
    all=neumannBoundaryCondition
    if( $bc eq "cic" ){ $bcCommand="Annulus(0,1)=dirichletBoundaryCondition, uniform(T=1.)"; }
    if( $bc eq "sib" ){ $bcCommand="north-pole(0,2)=dirichletBoundaryCondition, uniform(T=1.)\n" .\
                                   "south-pole(0,2)=dirichletBoundaryCondition, uniform(T=1.)"; }
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
  initial conditions
    OBIC:uniform state T=0. 
    OBIC:assign uniform state
  continue
continue
$go 
