*
* cgins command file for flow past some buildings
* 
*   o demonstrates the use of the "steady-state" solver
*   NOTES:
*     o you will need to generate the multiBuildings grid in the Overture/sampleGrids directory
*     o it is often necessary to start the steady state solver with
*       a large second-order dissipation (10, 10) until things settle down.
*       The solver can be then restarted (from the show file) with lower values
*       of the second-order dissipation.
*     o you probably want to use the PETSc solvers for this example
* 
* 
$rtol=1.e-3; $atol=1.e-4; $maxIterations=200; $plotIterations=50; 
$maxPressureIterations=10; 
* 
$grid="multiBuildings.hdf";  $show="multiBuildings.show"; 
*
$grid 
*
  incompressible Navier Stokes
  exit
* 
  show file options
    compressed
   open
     $show
   * Name to use for a restart:
   *   multiBuildings2.show
    frequency to flush
      1
    exit
  turn off twilight zone
*
*    ==== choose the steady state solver here ====
    steady state RK-line
    dtMax 1.
*
***
  max iterations $maxIterations
  plot iterations $plotIterations
***
  plot and always wait
* 
 pde parameters
    nu
      .001
    done
* 
  OBPDE:second-order artificial diffusion 1
  OBPDE:ad21,ad22  10. 10. 
********
  OBPDE:fourth-order artificial diffusion
  OBPDE:use implicit fourth-order artificial diffusion 1
  OBPDE:ad41,ad42 1,1
********
*
  maximum number of iterations for implicit interpolation
    10
*
*
  * use an iterative solver for the pressure equation
   pressure solver options
     * PETSc
     * SLAP
     choose best iterative solver
     maximum number of iterations
      $maxPressureIterations
     relative tolerance
       $rtol
     absolute tolerance
       $atol
    exit
****
  initial conditions
    uniform flow
     u=1., p=1.
  exit
****
****** for a restart uncomment the next lines and comment out the previous lines
****** NB: Be sure to use a new name for the show file for the restarted solution
******     or else it will already have been over-written by now
*  initial conditions
*    read from a show file
*     multiBuildings.show
*      -1
*  exit
*****
  boundary conditions
*   The boundary conditions have been assigned numbers 1,2,3,4 when the grid was
*   generated. Here we convert these numbers into boundary conditions:
    bcNumber1=noSlipWall
    bcNumber2=slipWall
    bcNumber3=inflowWithVelocityGiven, uniform(p=1.,u=1.)
    bcNumber4=outflow
    done
  exit
* 

  movie mode
  finish

























