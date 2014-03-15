*
* cgins example: flow past a cylinder using the "steady state" solver
* 
* Usage:
*   
*  cgins [-noplot] cic.ss.cmd -g=<name> -nu=<value> -its=<tFinal> -pits=<tPlot> -solver=<yale/best> -model=<ins/boussinesq> ...
*                             -debug=<num> -go=[run/halt/og]
* where
*   -its    : max iterations
*   -pits   : plot iterations; number of iterations between plot output.
*  -go : run, halt, og=open graphics
* 
* Examples:
* 
*   cgins cic.ss.cmd -g=cice2.order2.hdf -nu=.5 -its=10000 -pits=10
*   cgins cic.ss.cmd -g=cice2.order2.hdf -nu=.5 -model=boussinesq -its=1000 -pits=50
*   mpirun -np 2 $cginsp cic.ss.cmd -g=cice2.order2.hdf -nu=.5 -solver=best -its=100 -pits=10 
*   srun -N1 -n2 -ppdebug $cginsp cic.ss.cmd -g=cice2.order2.hdf -nu=.5 -solver=best -its=100 -pits=10 
* 
*  -- set dtMax to be not too big: -- this eventually blows up due to inflow at outflow 
*   cgins cic.ss.cmd -g=cice6.order2.hdf -ad2=5. -nu=1.e-4 -its=10000 -pits=50 -dtMax=.05
* 
*  -- use full implicit solver (we must solve the equations to a high enough tol.)
*    cgins noplot cic.ss.cmd -g=cice2.order2.hdf -nu=.5 -tp=1. -tf=15. -dtMax=.05 -ts=implicit -solver=best -rf=10 -show="cic2.full.show" -go=go >! cic2.full.out
*    
* --- set default values for parameters ---
* 
$nu=1.; $model="ins"; $ts="steady state RK-line"; $noplot=""; $backGround="square"; 
$debug = 0;  $tFinal=1.; $tPlot=.1; $maxIterations=100; $rtol=1.e-4; $atol=1.e-5; $dtMax=.5; 
$show="cic.ss.show";  $solver="yale"; 
$ogesDebug=0; $its=10000; $pits=100; $cfl=1.; $nu=.1;  $Prandtl=.72; $thermalExpansivity=.1;
$gravity = "0. 0. 0."; $T0=1.; $go="halt"; 
$implicitVariation="full"; $refactorFrequency=100; $ad2=0; $implicitFactor=1.;
* 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
* $ksp="gmres"; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"its=i"=>\$its,"pits=i"=>\$pits,"model=s"=>\$model,"dtMax=f"=>\$dtMax,\
 "tp=f"=>\$tPlot,"tf=f"=>\$tFinal, "solver=s"=>\$solver, "show=s"=>\$show,"debug=i"=>\$debug,"go=s"=>\$go, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "gravity=s"=>\$gravity, "noplot=s"=>\$noplot,\
 "rf=i"=> \$refactorFrequency, "iv=s"=>\$implicitVariation,"imp=f"=>\$implicitFactor,"ad2=f"=>\$ad2 );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="turn on polynomial"; }else{ $tz="turn on trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
*
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
* 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
* 
$kThermal=$nu/$Prandtl;
*
* Here is the overlapping grid to use:
$grid
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
       100
     exit
  debug $debug
*
  turn on polynomial
  degree in space 2
  degree in time 0
*   turn on trig
  frequencies (x,y,z,t) .5 .5 .5 0.   1. 1. 1. 0.
*  assign TZ initial conditions 0
  turn off twilight zone 
*
  max iterations $its
  plot iterations $pits
  plot residuals 1
* 
  final time $tFinal
  times to plot $tPlot
*
  plot and always wait
  pde options...
  OBPDE:second-order artificial diffusion 1
  OBPDE:ad21,ad22 $ad2 $ad2
 * 
  pde parameters
    nu  $nu
    kThermal $kThermal
    gravity
      $gravity
   done
* use this for finer grids and larger nu
  OBPDE:divergence damping 0.2
  close pde options
*
*   
  useNewImplicitMethod
  refactor frequency $refactorFrequency
  $implicitVariation
* 
  implicit factor $implicitFactor 
  $ts 
  choose grids for implicit
    all=implicit
   done
* 
   dtMax $dtMax
*
 cfl $cfl
*
   pressure solver options
    $solver
     relative tolerance
       $rtol
     absolute tolerance
       $atol
    exit
  implicit time step solver options
     $solver
     * PETSc
     * these tolerances are chosen for PETSc
     relative tolerance
       $rtol
     absolute tolerance
       $atol
    exit
  boundary conditions
*     all=dirichletBoundaryCondition
    all=noSlipWall
     square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.,n=.1,T=$T0)
    square(1,0)=outflow
    square(0,1)=slipWall
    square(1,1)=slipWall
    done
  initial conditions
  uniform flow
    p=0., u=1., n=.1, T=$T0
    exit
  project initial conditions
*
  continue
$go


*
movie mode
finish

