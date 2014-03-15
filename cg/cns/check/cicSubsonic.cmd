*
* Sub-sonic viscous flow past a cylinder
*
  cic.hdf
*
  compressible Navier Stokes (Jameson)
  exit
  turn off twilight
*
*
  final time .2 
  times to plot .1
**
  plot and always wait
  * no plotting
***
  show file options
    * compressed
    * open
    *   cicSubsonic.show
    frequency to flush
     4
    exit
***
  reduce interpolation width
    2
* **************
  pde parameters
    mu
     0.1
    kThermal
     0.14
  done
*
  boundary conditions
    Annulus(0,1)=noSlipWall uniform(u=.0,T=300.)
    square(0,0)=subSonicInflow uniform(r=1.,u=2.,T=300.)
    square(1,0)=subSonicOutflow mixedDerivative(1.*t+1.*t.n=300.)
    square(0,1)=slipWall
    square(1,1)=slipWall
    done
*  debug
*    1
  initial conditions
    uniform flow
      r=1. u=2. T=300.
  exit
  continue
*
  movie mode
  finish




