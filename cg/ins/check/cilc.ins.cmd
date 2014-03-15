*
* OverBlown command file for flow past a cylinder
*
* specify the overlapping grid to use:
cilc.hdf
* Specify the equations we solve:
  incompressible Navier Stokes
  exit
*
  turn off twilight zone 
* choose implicit time stepping:
  implicit
* but integrate the square explicitly:
  choose grids for implicit
    all=implicit
    square=explicit
    done
  final time .2
  times to plot .1
*  plot and always wait
  no plotting
  pde parameters
    nu
      .01
     turn off second order artificial diffusion
     turn off fourth order artificial diffusion
    done
*
  boundary conditions
    all=noSlipWall
    square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.)
    square(1,0)=outflow
*   square(1,0)=outflow, mixedDerivative(1.*p+1.*p.n=0.)
    square(0,1)=slipWall
    square(1,1)=slipWall
    done
  initial conditions
  uniform flow
    p=1., u=1.
  exit
  project initial conditions
  continue
  movie mode
  finish
