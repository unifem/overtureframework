******************************************************************
*  Steady ZND detonation down a channel: Ignition and Growth model
******************************************************************
*
*  channel : x=[-1.5,-.5], profile is at x=-1
* channel.hdf
*  This next grid is [0,1] so we need to shift the profile by 1.5 
channelShort.hdf
*
  compressible Navier Stokes (Godunov)
  ignition and growth
  JWL equation of state
  include LX17puck.data
  exit
  turn off twilight
*
  final time 1.e-3
*
  times to plot 1.e-3
* no plotting
  plot and always wait
*
  show file options
    compressed
    * open
    *   test.show
    frequency to flush
      2
    exit
*
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
      8
  change error estimator parameters
    set scale factors
      1 10000 10000 10000 10000 10000 10000
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
      4
    default number of refinement levels
      3
    number of buffer zones
      2
    grid efficiency
      .7
  exit
**************
  pde parameters
    mu
     0.0
    kThermal
     0.0
    heat release
      1.
    rate constant
      1.
   reciprocal activation energy
      0.065
  done
***************
  cfl
   .8
* OBPDE:exact Riemann solver
  OBPDE:Roe Riemann solver
* OBPDE:HLL Riemann solver
  OBPDE:Godunov order of accuracy 2
******************
  debug
    0
*
**************************
  initial conditions
    user defined
      1d profile from a data file
      profilePuck.data
    exit
  exit
*
  boundary conditions
    all=slipWall
    rectangle(0,0)=superSonicInflow uniform(r=1.31631,u=0.2403,T=0.182556,s=1.,a=0.72543,b=0.7597)
    done
*
continue
*
movie mode
finish

