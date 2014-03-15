*
* cgins example: flow in a cylindrical pipe
*
* srun -N1 -n2 -ppdebug $cginsp noplot pipe.cmd
*
$tFinal=10.; $tPlot=.05; $nu=.025; $show=" "; $debug=3; $width=.15; 
$solver = "choose best iterative solver"; $rtolp=1.e-4; $atolp=1.e-4; $pdebug=0; $rtoli=1.e-4; $atoli=1.e-4; $idebug=0; 
**** choose small tol's for checking serial vs parallel
$rtolp=1.e-7; $atolp=1.e-7; $pdebug=0; $rtoli=1.e-7; $atoli=1.e-7
***
$order4 = "fourth order accurate";
$order = "second order accurate"; 
* 
$grid = "pipee1.order2.hdf";  $tFinal=.2; $nu=.1; $show="pipe.show"
* $grid = "pipee2.order2.hdf";  $tFinal=.2; $nu=.005; $show="pipe.show"
* $grid = "pipee4.order2.hdf";  $tFinal=.2; $nu=.001; $width=.25; $show="pipe.show"
* $grid = "pipee8.order2.hdf";  $tFinal=.2; $nu=.0005; $width=.25; $show="pipe.show"
* $grid = "pipe1eL4.hdf"; $nu=.01; 
* $grid = "pipe2.hdf";  $nu=.005; $solver="multigrid"; 
* $grid = "pipe4.hdf";  $nu=.001; $solver="multigrid"; $tPlot=1.; $tFinal=10.; $show="pipes4.show"; 
* 
* $grid="pipe2eL4.order4.hdf"; $nu=.005; $order=$order4;
*
* grid name:
$grid
* 
  incompressible Navier Stokes
  exit
* 
*
$order
*
  show file options
    * specify the max number of parallel hdf sub-files: 
      OBPSF:maximum number of parallel sub-files 8
    compressed
    open
      $show
    frequency to flush
      5
    exit
  turn off twilight zone
* 
  final time $tFinal
  times to plot $tPlot
*  plot and always wait
*   no plotting
*
**  implicit
  choose grids for implicit
    all=implicit
*    box=explicit
    done
* 
  pde parameters
    nu
     $nu
    done
*
  debug $debug
*
   pressure solver options
    * PETSc
    * choose best iterative solver
    * multigrid
    * yale
* 
     $solver
* 
     relative tolerance
       $rtolp
     absolute tolerance
       $atolp
*mg      multigrid parameters
*mg      *  alternating
*mg      * cycles: 1=V, 2=W
*mg      number of cycles
*mg        2
*mg      residual tolerance
*mg        1.e-8 1.e-10
*mg      error tolerance
*mg        1.e-2
*mg      debug
*mg       3
*mg      maximum number of levels
*mg        5 4 3 2
*mg      maximum number of extra levels
*mg        4 3 2 1
*mg      debug
*mg        3
*mg      exit
    exit
*
   implicit time step solver options
    * PETSc
    * choose best iterative solver
    * multigrid
    * yale
* 
     $solver
* 
     relative tolerance
       $rtoli
     absolute tolerance
       $atoli
*mg      multigrid parameters
*mg      *  alternating
*mg      * cycles: 1=V, 2=W
*mg      number of cycles
*mg        2
*mg      residual tolerance
*mg        1.e-8
*mg      error tolerance
*mg        1.e-2
*mg      debug
*mg       3
*mg      maximum number of levels
*mg        5 4 3 2
*mg      maximum number of extra levels
*mg        4 3 2 1
*mg      debug
*mg        1
*mg      exit
    exit
*
  boundary conditions
    all=noSlipWall
    bcNumber1=inflowWithVelocityGiven,  parabolic(d=$width,p=1,u=1.)
*    bcNumber1=inflowWithVelocityGiven , parabolic(d=$width,p=1.,u=1.), oscillate(a0=1.5,t0=.0,omega=.5)
    bcNumber2=outflow, pressure(1.*p+1.*p.n=0.)
    done
*
  initial conditions
  uniform flow
    p=1., u=1., v=0. w=0.
  done
*
***  project initial conditions
  continue
*

movie mode
finish
