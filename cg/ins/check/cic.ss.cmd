*
* OverBlown command file for cic -- steady state computation
*
cic.hdf
* cic2.hdf
* cisc.hdf
* cisc2.hdf
  incompressible Navier Stokes
*  SpalartAllmaras
  exit
*
   show file options
     * uncompressed
*     open
*        cic.ss.show
     frequency to flush
       2
     exit
*   debug
*    31 
*
  turn on polynomial
  degree in space 2
  degree in time 0
*   turn on trig
  frequencies (x,y,z,t) .5 .5 .5 0.   1. 1. 1. 0.
*  assign TZ initial conditions 0
  turn off twilight zone 
*
  max iterations 100
* 
  final time 1.
  times to plot .2
  plot and always wait
*   no plotting
  pde options...
  OBPDE:second-order artificial diffusion 1
*   OBPDE:ad21,ad22 10. 10.
  OBPDE:nu .1   0.001   0.2
* use this for finer grids and larger nu
  OBPDE:divergence damping 0.2
  close pde options
*
    steady state RK-line
    dtMax .5
*
 cfl
   1.
*
   pressure solver options
    * PETSc
    * choose best iterative solver
    yale
     relative tolerance
       1.e-8 1.e-4  1.e-6  1.e-4
     absolute tolerance
       1.e-6
    exit
  boundary conditions
*     all=dirichletBoundaryCondition
    all=noSlipWall
     square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.,n=.1)
    square(1,0)=outflow
    square(0,1)=slipWall
    square(1,1)=slipWall
    done
  initial conditions
  uniform flow
    p=0., u=1., n=.1
    exit
  project initial conditions
*
  continue
*
movie mode
finish

