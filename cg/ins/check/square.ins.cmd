*
* OverBlown command file for a square
*
square20
  incompressible Navier Stokes
  exit
*  turn on polynomial
  turn on trigonometric
  final time 1.
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
continue
* 
movie mode
finish
