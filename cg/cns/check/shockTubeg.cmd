*
* OverBlown command file for a shock-tube
*
 channelShort
  compressible Navier Stokes (Godunov)  
  exit
  turn off twilight
  final time .1
  times to plot .05
  plot and always wait
  * no plotting
  show file options
    compressed
    * open
    *   shockTube.show
    frequency to flush
      2
    exit
*
 reduce interpolation width
   2
*
  pde parameters
    mu
     0.
    kThermal
     0.
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
  boundary conditions
    rectangle=slipWall
    rectangle(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
    rectangle(1,0)=superSonicOutflow
    done
  initial conditions
    step function
      x=.25
      r=2.6667 u=1.25 e=10.119
      r=1. e=1.786
    continue
***************
  debug
    0 3
***************
   continue
movie mode
finish

