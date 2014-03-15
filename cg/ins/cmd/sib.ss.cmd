*
* cgins example: flow past a sphere in a box using the "steady state" solver
* 
* Usage:
*   
*  cgins [-noplot] sib.ss.cmd -g=<name> -nu=<value> -its=<tFinal> -pits=<tPlot> -solver=<yale/best> -model=<ins/boussinesq> -debug=<num>
* where
*   -its    : max iterations
*   -pits   : plot iterations; number of iterations between plot output.
* 
* Examples:
* 
*      cgins noplot sib.ss.cmd -g=sibe1.order2.hdf -nu=.5 -its=10 -pits=10
*      cgins sib.ss.cmd -g=sibe2.order2.hdf -nu=.5 -pits=10
*      cgins sib.ss.cmd -g=sibe2.order2.hdf -nu=.25 -model=boussinesq -pits=10
*      cgins sib.ss.cmd -g=sibe4.order2.hdf -nu=.5 -pits=10
*      srun -N1 -n2 -ppdebug $cginsp sib.ss.cmd -g=sibe1.order2.hdf -nu=.5 -pits=10 
* 
*  cgins noplot sib.ss.cmd -g=sibe2.order2.hdf -nu=.5 -pits=100 -its=1000 -go=go > ! sib2.ss.out &
* 
* --- set default values for parameters ---
* 
$nu=1.; $model="ins"; $ts="steady state RK-line"; $noplot=""; $backGround="square"; $go="halt";
$debug = 0;  $tPlot=.1; $maxIterations=15; $ad2=5.; 
$show="sib.ss.show";  $solver="best"; $rtol=1.e-4; $atol=1.e-5; 
$ogesDebug=0; $its=1000; $pits=100; $cfl=1.; $nu=.1; $Prandtl=.72; $thermalExpansivity=.1; $T0=1.;
* 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
* $ksp="gmres"; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"its=i"=>\$its,"pits=i"=>\$pits,"mits=i"=>\$maxIterations,"model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "show=s"=>\$show,"debug=i"=>\$debug,"go=s"=>\$go, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "gravity=s"=>\$gravity, "noplot=s"=>\$noplot );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="turn on polynomial"; }else{ $tz="turn on trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
*
* 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "go" ){ $go = "movie mode\n finish"; }
*
$kThermal=$nu/$Prandtl;
* Here is the overlapping grid to use:
$grid
* old:
* cylBox1r.hdf
* cyl2.hdf
* orthoSphere1
* 
  $model
*  SpalartAllmaras
  exit
*
   show file options
    uncompressed
     open
       $show
     frequency to flush
       10
     exit
  debug $debug
*
  turn on polynomial
  degree in space 2
  degree in time 0
  assign TZ initial conditions 0
  turn off twilight zone
*
***
  max iterations $its
  plot iterations $pits
***
   plot and always wait
*  no plotting
*
    steady state RK-line
    dtMax 1.
*
  cfl $cfl
*
*
 * 
  pde parameters
    nu  $nu
    kThermal $kThermal
    gravity
      $gravity
   done
  pde options...
  OBPDE:second-order artificial diffusion 1
  OBPDE:ad21,ad22  $ad2 $ad2
********
*  OBPDE:fourth-order artificial diffusion
*  OBPDE:use implicit fourth-order artificial diffusion 1
*  OBPDE:ad41,ad42 1,1
********
  close pde options
  * use GMRES to solve the pressure equation
  pressure solver options
     $solver
     * these tolerances are chosen for PETSc
     maximum number of iterations
       $maxIterations
     relative tolerance
       $rtol
     absolute tolerance
       $atol
    exit
*
  project initial conditions
  initial conditions
    uniform flow
     u=1., p=1., T=$T0
  exit
  boundary conditions
   all=noSlipWall
*   all=slipWall
   box(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.,T=$T0)
   box(1,0)=outflow
   box(1,1)=slipWall
   box(0,1)=slipWall
   box(0,2)=slipWall
   box(1,2)=slipWall
*    box(1,0)=noSlipWall
   north-pole=noSlipWall
   south-pole=noSlipWall
  done
  exit
  y+r:0 25
  x+r:0 25
$go
