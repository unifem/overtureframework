***************************************************************
*  Example showing a moving cylinder in a compressible flow
*     You may have to make the cic grid used below
**************************************************************
*
cic.hdf
*
*  Either Jameson or Godunov should work
*   compressible Navier Stokes (Jameson)
  compressible Navier Stokes (Godunov)
*   one step
  exit
  turn off twilight
*
*  do not use iterative implicit interpolation
*
  final time .4
*
  times to plot .1 
 no plotting
*plot and always wait
*
  show file options
    compressed
*    open
*      cicShockMoveG5L3.show
    frequency to flush
      2
    exit
  * no plotting
*****************************
  * There can be trouble if the grid moves too fast
  turn on moving grids
  specify grids to move
      rigid body
        mass
          .05    50
        moments of inertia
          1.
        initial centre of mass
           0. 0.
        done
        Annulus
       done
  done
***************************
  reduce interpolation width
    2
*****
 turn on adaptive grids
  order of AMR interpolation
      2
  error threshold
      .0005
  regrid frequency
     4  8
  change error estimator parameters
    set scale factors
      1 10000 10000 10000 10000
    done
    weight for first difference
    0.
    weight for second difference
    .03
    exit
    truncation error coefficient
    1.
    show amr error function
  change adaptive grid parameters
    refinement ratio
      2 4 2 4
    default number of refinement levels
      2 3 2
    number of buffer zones
      2
    grid efficiency
      .7
  exit
*****
  boundary conditions
    all=slipWall
    square(0,1)=superSonicInflow uniform(r=2.6069,T=.943011,u=0.,v=0.694444,s=0.0)
    done
*
  pde parameters
    mu
     0.0
    kThermal
     0.0
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
***************
*  cfl
*   .5 
*    .95
*   OBPDE:exact Riemann solver
* OBPDE:Roe Riemann solver
* OBPDE:HLL Riemann solver
*   OBPDE:Godunov order of accuracy 2
******************
*  debug
*    1
*
  initial conditions
   step function
    y=-.7
    T=.943011, u=0., v=.694444, r=2.6069
    T=.714286, u=0., v=0., r=1.4
  continue
continue
*
movie mode
finish
