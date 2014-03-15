*
* cgins example: test the steady state solver with twilight-zone flow
*
* Usage: 
*    cgins [noplot] ss -g=<name> -degreex=<> -degreet=<> -nu=<val> -its=<tFinal> -pits=<tPlot> -ic=<uniform/tz>...
*                      -debug=<> -show=<name> -bg=<grid-name> -cfl=<num> -solver=<yale/best> -model=<ins/boussinesq> ...
*                      -ad2=[0/1] -go=[run/halt/og]
*
*  -ic = initial conditions 
*  -go : run, halt, og=open graphics
* 
* Examples:
*
*  cgins ss -g=square10.hdf -nu=.5 -degreex=2                      [exact]
*  cgins ss -g=nonSquare10.hdf -nu=.5 -degreex=2                      [exact]
*  cgins ss -g=sis.hdf -nu=.5 -degreex=2 -bg=outer-square          [exact]
*  cgins ss -g=square64.hdf -nu=.5 -degreex=2                      [exact, takes a while to converge]
*  cgins ss -g=box10.hdf -nu=.5 -degreex=2 -bg=box                 [exact]
*  cgins ss -g=nonBox8.hdf -nu=.5 -degreex=2 -bg=box                 [exact]
*  cgins ss -g=cic2.hdf -nu=.5 -degreex=2  -bg=none  
*
* Testing:
*      cgins noplot ss -g=square10.hdf -nu=.5 -degreex=2 -ic=tz -pits=1 -its=5 -debug=3 
* 
* Boussinesq and Line Solver:
*  cgins ss noplot -g=square5.hdf -nu=.5 -degreex=2 -model=boussinesq -pits=1 -ic=tz -debug=15
*  cgins ss -g=square10.hdf -nu=.5 -degreex=2 -model=boussinesq -pits=1 -gravity="0  -1. 0."
*  cgins ss -g=sise1.order2.hdf -nu=.5 -degreex=2 -model=boussinesq -pits=1 -gravity="0  -1. 0."   [exact]
*  cgins ss -g=rotatedSquare10.hdf -nu=.5 -degreex=2 -model=boussinesq -pits=10
*  cgins ss -g=box10.hdf -nu=.5 -degreex=2 -bg=box -model=boussinesq -pits=10          [exact]
* 
* parallel: ** NOTE by default start from uniform IC's and converge to TZ 
*  mpirun -np 2 $cginsp ss noplot -g=square8.hdf -nu=.5 -degreex=2 -pits=1 -solver=best -ic=tz -go=go -its=5
*  mpirun -np 2 $cginsp ss noplot -g=square8pn.hdf -nu=.5 -degreex=0 -pits=1 -solver=best -ic=tz -go=go -its=1 -debug=15
*  mpirun -np 2 $cginsp ss noplot -g=sise1.order2.hdf -nu=.5 -degreex=0 -pits=1 -solver=best -ic=tz -go=go -its=5
* 
*  mpirun -np 2 $cginsp ss noplot -g=cice1.order2.hdf -nu=.5 -degreex=0 -pits=1 -solver=best -ic=tz -go=go -its=5
* 
*  mpirun -np 1 $cginsp ss -g=square10.hdf -nu=.5 -degreex=2 -ic=tz -pits=10 -solver=best     [exact]
*  mpirun -np 2 $cginsp ss -g=square10.hdf -nu=.5 -degreex=2 -ic=tz -pits=10 -solver=best     [exact]
* 
*  mpirun -np 2 $cginsp ss -g=sise1.order2.hdf -nu=.5 -degreex=2 -ic=tz -its=10 -pits=1 -solver=best  [exact]
*  mpirun -np 2 $cginsp ss -g=cice2.order2.hdf -nu=.5 -degreex=2 -ic=tz -its=10 -pits=1 -solver=best  [runs!]
*  mpirun -np 2 $cginsp ss -g=box10.hdf -nu=.5 -degreex=2 -solver=best -ic=tz -pits=10     [exact]
*  mpirun -np 2 $cginsp ss -g=sibe2.order2.hdf -nu=.5 -degreex=2 -bg=box -ic=tz -its=10 -pits=1 -solver=best  []
*  srun -N1 -n2 -ppdebug $cginsp ss -g=sibe2.order2.hdf -nu=.5 -degreex=2 -pits=1 -solver=best 
* 
* testing: 
* 
*  mpirun -np 2 $cginsp ss noplot -g=square8.hdf -nu=.5 -degreex=0 -pits=1 -solver=best -debug=15 -go=og
* 
*  srun -N1 -n2 -ppdebug $cginsp ss -g=square5.hdf -nu=.5 -degreex=2 -solver=best -pits=1 -debug=15
*  totalview srun -a -N1 -n2 -ppdebug $cginsp -noplot ss -g=square5.hdf -nu=.5 -degreex=2 -pits=1 -solver=best -debug=15
* 
*  srun -N1 -n2 -ppdebug memcheck_all $cginsp -noplot ss -g=square10.hdf -nu=.5 -degreex=2 -pits=1 -solver=best -debug=15
*
*  srun -N1 -n2 -ppdebug $cginsp ss -g=cice.hdf -nu=.5 -degreex=0 -pits=1 -solver=best -debug=15
*  mpirun -np 2 $cginsp ss -g=cice.hdf -nu=.5 -degreex=0 -pits=1 -solver=best -debug=15
*  mpirun -np 2 $cginsp ss -g=cic1e.hdf -nu=.5 -degreex=0 -pits=1 -solver=best -debug=15
* 
*  mpirun -np 2 $cginsp ss -g=square5pn.hdf -nu=.5 -degreex=0 -solver=best -pits=1 -debug=15
*  mpirun -np 2 $cginsp ss -g=square5np.hdf -nu=.5 -degreex=0 -solver=best -pits=1 -debug=15
*  srun -N1 -n2 -ppdebug $cginsp ss -g=square5pn.hdf -nu=.5 -degreex=0 -solver=best -pits=1
*  srun -N1 -n2 -ppdebug $cginsp ss -g=square5np.hdf -nu=.5 -degreex=0 -pits=1 -solver=best -debug=15
* 
*  srun -N1 -n2 -ppdebug $cginsp ss -g=rsis2e.hdf -nu=.5 -degreex=2 -pits=1 -solver=best -debug=3
*
* 
* --- set default values for parameters ---
$cfl=.9;  $nu=.25; $Prandtl=.72; $debug=1; $its=10000; $pits=100; $degreex=2; $degreet=0; $rtol=1.e-8; $atol=1.e-6; 
$show = " "; $solver="yale"; $model="ins"; 
$bg=square; # back-ground grid
$gravity = "0. 0. 0."; $ad2=0; 
$ic ="uniform";  $go="halt"; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions("g=s"=>\$grid,"its=i"=> \$its,"pits=i"=> \$pits,"nu=f"=>\$nu,"cfl=f"=>\$cfl,"debug=i"=> \$debug, \
           "degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "show=s"=>\$show, "bg=s"=>\$bg, "noplot=s"=>\$noplot, \
           "solver=s"=>\$solver, "model=s"=>\$model, "gravity=s"=>\$gravity, "ic=s"=>\$ic, "go=s"=>\$go,\
           "ad2=i"=> \$ad2 );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver = "choose best iterative solver"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }\
 elsif( $model eq "sa" ){ $model="incompressible Navier Stokes\n SpalartAllmaras"; }\
 else                   { $model = "incompressible Navier Stokes\n Boussinesq model"; }
if( $ic eq "tz" ){ $ic="initial conditions\n exit";}else\
                 { $ic="initial conditions\n uniform flow\n  p=0., u=0., n=.1, T=0.\n exit";}
* 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
$kThermal=$nu/$Prandtl;
*
* flatPlate1
* 
* --- here is the grid ---
$grid
*
  $model
*  SpalartAllmaras
  * to save space we only solve this many tridiagonal systems at once: 
  define integer parameter maximumNumberOfLinesToSolveAtOneTime 100
  exit
*
   show file options
     * uncompressed
     open
       $show
     frequency to flush
       2
     exit
*
  debug $debug
*
*  turn off twilight zone 
  turn on polynomial
  degree in space $degreex
  degree in time 0
*  turn on trig
  frequencies (x,y,z,t) 2. 2. 2. 0.
*   assign TZ initial conditions 0
*
*
  max iterations $its
  plot iterations $pits
*
  plot and always wait
*
  OBPDE:second-order artificial diffusion $ad2
  OBPDE:ad21,ad22 1. 1.
 * 
  pde parameters
    nu  $nu
    kThermal $kThermal
    gravity
      $gravity
   done
*
    steady state RK-line
    dtMax .5
*
  cfl $cfl
*
   pressure solver options
    * PETSc
    * choose best iterative solver
    $solver
     relative tolerance
       $rtol
     absolute tolerance
       $atol
    exit
  boundary conditions
*   all=dirichletBoundaryCondition
   all=noSlipWall
*     all=slipWall
*    square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.,n=.1)
*     $bg(0,1)=slipWall
*    square(1,1)=noSlipWall
*     $bg(0,0)=slipWall
** 
 $bg(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.,n=.1)
 $bg(1,0)=outflow
* 
*  $bg(0,0)=noSlipWall , mixedDerivative(1.*t+1.*t.n=0.)
*  $bg(1,0)=noSlipWall , mixedDerivative(1.*t+1.*t.n=0.)
*   $bg(0,0)=noSlipWall , mixedDerivative(1.*t+1.*t.n=0.)
*   $bg(1,0)=noSlipWall , mixedDerivative(1.*t+1.*t.n=0.)
  $bg(0,1)=noSlipWall , mixedDerivative(1.*t+1.*t.n=0.)
  $bg(1,1)=noSlipWall , mixedDerivative(1.*t+1.*t.n=0.)
*  all=noSlipWall , mixedDerivative(1.*t+1.*t.n=0.)
*   $bg(0,0)=slipWall
*   $bg(1,1)=slipWall
*   $bg(0,1)=slipWall
* 
   done
*   continue
*  continue
****************************************
* set initial conditions:
$ic
****************************************
* 
*  project initial conditions
*
  continue
*  plot and always wait
* 
$go


continue
continue
finish


  pause

*
