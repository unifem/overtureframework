  cic
*
   compressible Navier Stokes (Godunov)  
*   one step
  exit
  turn off twilight
*
  final time .1
  times to plot .1 
*  plot and always wait
  no plotting
  reduce interpolation width
    2
  boundary conditions
    * all=noSlipWall uniform(T=.3572)
    all=slipWall 
    square(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119,s=0)
    square(1,0)=superSonicOutflow
    square(0,1)=slipWall
    square(1,1)=slipWall
    done
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
*
  turn on adaptive grids
*   save error function to the show file
  show amr error function
  order of AMR interpolation
      2
  error threshold
     .2 .1
  regrid frequency
    4 8
  change error estimator parameters
    default number of smooths
      1
    set scale factors     
      2 1 1 1 1 
    done
    exit
  change adaptive grid parameters
    refinement ratio
      4 2 4 2 
    default number of refinement levels
      2 3 
    number of buffer zones
      2
    grid efficiency
      .7 .5 
  exit
*
  initial conditions
    step function
      x=-1.5  x=-.5 x=-1.75
      r=2.6667 u=1.25 e=10.119 s=0.
      r=1. e=1.786 s=0.
  continue
continue
movie mode
finish

