* ******************************************
*  axisymmetric flow past two half annuli
* ******************************************
*
* halfAnnulus2.hdf
twoBump.hdf
*
   compressible Navier Stokes (Godunov)  
* use old version for now
**  define integer parameter oldVersion 1
*
*   one step
*   add extra variables
*     1
  exit
  turn off twilight
*
*  do not use iterative implicit interpolation
*
  final time .4
  times to plot .2 
**  no plotting
*
  reduce interpolation width
    2
  boundary conditions
    * all=noSlipWall uniform(T=.3572)
    all=slipWall 
    channel(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119,s=0)
    channel(1,0)=superSonicOutflow
    channel(0,1)=axisymmetric
    bottomAnnulus(0,0)=axisymmetric
    bottomAnnulus(1,0)=axisymmetric
  done
*
  turn on axisymmetric flow 
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
*
  initial conditions
     step function
      x=-.5
      r=2.6667 u=1.25 e=10.119 
      r=1. e=1.786 s=0.
  continue
continue
* 
movie mode
finish
