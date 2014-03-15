*
* cgcns command file for a shock hitting a sphere (axisymmetric)
* 
* Usage: 
*    cgcns [noplot] axisymmetricSphere -g=<name> -l=<levels> -r=[ratio] -tf=<tFinal> -tp=<tPlot> -x0=<num> -show=<name>
*
* Examples:
*
*   cgcns noplot axisymmetricSphere -g=oneBump2e.order2.hdf -tf=.2 -tp=.1 -l=1 -show="oneBump.show"
*  
* Parallel Examples
* 
*  mpirun -np 1 $cgcnsp axisymmetricSphere -g=oneBump2e.order2.hdf -tp=.1 -show="oneBump.show"
* 
*  srun -N1 -n4 -ppdebug $cgcnsp noplot axisymmetricSphere -g=oneBump2e.order2.hdf -l=2 -r=2 -tf=1.25 -tp=.25 -x0=-1. -show=oneBump2l2r2.show >! oneBump2l2r2.N1.n4.out &
*
*  srun -N1 -n8 -ppdebug $cgcnsp noplot axisymmetricSphere -g=oneBump2e.order2.hdf -l=2 -r=4 -tf=1.8 -tp=.1 -x0=-1.5 -show=oneBump2l2r4.show >! oneBump2l2r4.N1.n8.out &
*
* srun -N2 -n16 -ppdebug $cgcnsp noplot axisymmetricSphere -g=oneBump2e.order2.hdf -l=2 -r=4 -tf=1.8 -tp=.2 -x0=-1.5 -show=oneBump2l2r4.show > ! oneBump2l2r4.N2.n16.out & 
* srun -N4 -n16 -ppdebug $cgcnsp noplot axisymmetricSphere -g=oneBump2e.order2.hdf -l=2 -r=4 -tf=1.8 -tp=.2 -x0=-1.5 -show=oneBump2l2r4.show > ! oneBump2l2r4.N4.n16.out & 
* srun -N4 -n32 -ppdebug $cgcnsp noplot axisymmetricSphere -g=oneBump2e.order2.hdf -l=2 -r=4 -tf=1.8 -tp=.2 -x0=-1.5 -show=oneBump2l2r4.show > ! oneBump2l2r4.N4.n32.out & 
* srun -N4 -n32 -ppdebug $cgcnsp noplot axisymmetricSphere -g=oneBump2e.order2.hdf -l=3 -r=4 -tf=1.8 -tp=.1 -x0=-1.5 -show=oneBump2l3r4.show > ! oneBump2l3r4.N4.n32.out & 
*
* srun -N4 -n8 -ppdebug $cgcnsp noplot axisymmetricSphere -g=oneBump2e.order2.hdf -l=2 -r=4 -tf=1.8 -tp=.1 -x0=-1.5 -show=oneBump2l2r4.show > ! oneBump2l2r4.N4.n8.out &
*
* srun -N1 -n2 -ppdebug $cgcnsp noplot -writeCollective axisymmetricSphere -g=oneBump2e.order2.hdf -l=2 -r=4 -tf=1.8 -tp=.2 -x0=-1.5 -show=oneBump2l2r4.show > ! oneBump2l2r4.N1.n2.out & 
* 
* mpirun -np 2 -machinefile hostfile $cgcnsp noplot oneBumpRun
* 
* srun -N1 -n4 -ppdebug $cgcnsp noplot axisymmetricSphere >! oneBump4l2r2.N1n4.out 
* totalview srun -a -N1 -n4 -ppdebug $cgcnsp noplot axisymmetricSphere 
* srun -N1 -n8 -ppdebug memcheck_all $cgcnsp noplot axisymmetricSphere 
*
* --- set default values for parameters ---
$show = " ";
$nrl=2;  # number of refinement levels
$cfl=.9; $debug=1; $tol=.002; $ratio=4; $x0=-1.5; $dtMax=1.e10; $nbz=2;
$tFinal=1.8; $tPlot=.2;
*
$amrOn="turn on adaptive grids";
$amrOff="turn off adaptive grids";
$amr=$amrOn;
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"l=i"=> \$nrl,"r=i"=> \$ratio, "tf=f"=>\$tFinal, \
            "tp=f"=>\$tPlot, "x0=f"=>\$x0, "show=s"=>\$show );
* -------------------------------------------------------------------------------------------------
*
* $grid ="oneBump1e.order2.hdf"; $tFinal=1.4; $tPlot=.1; $x0=-1.; $amr=$amrOn; $ratio=4; $nrl=2;  $show = " ";
* 
* totalview srun -a -N2 -n16 -ppdebug $cgcnsp noplot axisymmetricSphere >! oneBump2l3r4.out &
* $grid ="oneBump2e.order2.hdf"; $tFinal=1.2; $tPlot=.1; $x0=-1.; $amr=$amrOn; $ratio=4; $nrl=3;  $show = " ";
* $grid ="oneBump2e.order2.hdf"; $tFinal=1.4; $tPlot=.1; $x0=-1.; $amr=$amrOn; $ratio=4; $nrl=2;  $show = " ";
*
* $grid ="oneBump3e.order2.hdf"; $tFinal=1.8; $tPlot=.1; $x0=-1.; $amr=$amrOn; $ratio=4; $nrl=2;  $show = " ";
*
* $grid ="oneBump4e.order2.hdf"; $tFinal=1.8; $tPlot=.1; $x0=-1.5; $amr=$amrOn; $ratio=4; $nrl=3; 
* $grid ="oneBump4e.order2.hdf"; $tFinal=1.8; $tPlot=.1; $x0=-1.5; $amr=$amrOn; $ratio=2; $nrl=2;  $show = "oneBump4l2r2.show";
* $grid ="oneBump4e.order2.hdf"; $tFinal=1.8; $tPlot=.1; $x0=-1.5; $amr=$amrOn; $ratio=4; $nrl=2;  $show = " ";
* $grid ="oneBump4e.order2.hdf"; $tFinal=1.8; $tPlot=.1; $x0=-1.; $amr=$amrOn; $ratio=4; $nrl=2;  $show = " ";
* totalview srun -a -N1 -n8 -ppdebug $cgcnsp noplot axisymmetricSphere 
*           srun -N2 -n16 -ppdebug $cgcnsp noplot axisymmetricSphere >! oneBump4l2r4.out &
* $grid ="oneBump4e.order2.hdf"; $tFinal=1.4; $tPlot=.1; $x0=-1.; $amr=$amrOn; $ratio=4; $nrl=2;  $show = " ";
* $grid ="oneBump4e.order2.hdf"; $tFinal=1.4; $tPlot=.1; $x0=-1.; $amr=$amrOn; $ratio=2; $nrl=2;  $show = " ";
*           srun -N2 -n16 -ppdebug $cgcnsp noplot axisymmetricSphere >! oneBump4l3r4.out &
* $grid ="oneBump4e.order2.hdf"; $tFinal=1.8; $tPlot=.1; $x0=-1.5; $amr=$amrOn; $ratio=4; $nrl=3;  $show = " ";
*    srun -N2 -n16 -ppdebug $cgcnsp noplot axisymmetricSphere >! oneBump4l2r4.out &
* $grid ="oneBump4e.order2.hdf"; $tFinal=1.5; $tPlot=.5; $x0=-1.5; $amr=$amrOn; $ratio=4; $nrl=2;  $show = "oneBump4l2r4.show";
*
$grid
***
 compressible Navier Stokes (Jameson) 
**   compressible Navier Stokes (Godunov)
axisymmetric flow with swirl 1
  exit
cylindrical axis is x axis
*
* turn off graphics
*
  turn off twilight
  final time $tFinal
  times to plot $tPlot
*  plot and always wait
  no plotting
***
  show file options
     compressed
    * specify the max number of parallel hdf sub-files: 
      OBPSF:maximum number of parallel sub-files 8
     open
       $show
    frequency to flush
      10
    exit
* -- specify which variables will appear in the show file:
*    showfile options...
*    OBPSF:show variable: rho 1
*    OBPSF:show variable: u 0
*    OBPSF:show variable: v 0
*    OBPSF:show variable: w 0
*    OBPSF:show variable: T 0
*    OBPSF:show variable: Mach Number 0
*    OBPSF:show variable: p 1
*    close show file options
*
***
* -----------------------------------------------------------------------
  turn on memory checking
* -----------------------------------------------------------------------
*
*
   pde options
   OBPDE:mu 0.
   OBPDE:kThermal 0.
   OBPDE:gamma 1.4
   OBPDE:artificial viscosity 1.
   close pde options
*
**  pde parameters
*     mu
*     0.
*     kThermal
*     0.
*  done
*
* OBPDE:exact Riemann solver
   OBPDE:Roe Riemann solver
* OBPDE:HLL Riemann solver
   OBPDE:Godunov order of accuracy 2
*
  reduce interpolation width
    2
 order of extrapolation for interpolation neighbours
     1
*
*  order of extrapolation for second ghost line
*     1
*  order of extrapolation for interpolation neighbours
*    2
*
  turn on axisymmetric flow
*
  boundary conditions
   all=slipWall
   channel(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
   channel(1,0)=superSonicOutflow
   channel(0,1)=axisymmetric
   bottomAnnulus(0,0)=axisymmetric
   bottomAnnulus(1,0)=axisymmetric
  done
  cfl
   $cfl
  dtMax $dtMax
*
**  check for floating point errors
*
  $amr
*
  order of AMR interpolation
      2
  error threshold
     $tol
  regrid frequency
     $regrid=$nbz*$ratio;
     $regrid
  change error estimator parameters
    set scale factors
      1. 1. 1. 1. 1. 1.
    done
    weight for first difference
    0.
    weight for second difference
    1.
    exit
    truncation error coefficient
    0.
**show amr error function
  change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $nrl
    number of buffer zones
       $nbz
    grid efficiency
      .7
    turn on load balancer
    change load balancer
      KernighanLin
      * sequential assignment
      * random assignment
      * all to all
    exit
  exit
  debug $debug
*
  initial conditions
   step function
      x=$x0
      r=2.6667 u=1.25 e=10.119
      r=1. u=0. e=1.786
    continue
   continue
*
*    initial conditions
*      read from a show file
*      oneBump4l2.show
*      -1
*      exit
*    continue
*

movie mode
finish

