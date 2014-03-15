*
* OverBlown command file for a square
*
box5
  incompressible Navier Stokes
  exit
  turn on polynomial
  * turn on trig
  final time .2
  times to plot .1
  * plot and always wait
  no plotting
  pde parameters
    nu
    .1
    done
  boundary conditions
    all=noSlipWall
   done
continue
movie mode
finish
