*
* Flow over a backward facing smoothed step.
*
* specify the overlapping grid to use:
backStepSmooth
* Specify the equations we solve:
  incompressible Navier Stokes
  exit
  turn off twilight zone 
  final time 40.
  times to plot .5
  plot and always wait
  * no plotting
  pde parameters
    nu
     .05
*      .0066667
*    turn on second order artificial diffusion
    done
  boundary conditions
    all=noSlipWall
    inlet(0,0)=inflowWithVelocityGiven, parabolic(d=.4,p=1,u=1.)
    corner(0,0)=inflowWithVelocityGiven, parabolic(d=.4,p=1,u=1.)
    mainChannel(1,0)=outflow
    inlet(1,1)=slipWall
    mainChannel(1,1)=slipWall
    done
*
  initial conditions
*      read from a show file
*      cylinder.show
*       9
  uniform flow
    p=1., u=1.
  exit
  project initial conditions
  continue
*




  pressure solver options
*    choose best iterative solver
     relative tolerance
       1.e-5
     absolute tolerance
       1.e-5
*   multigrid
*     multigrid parameters
*      alternating
*     debug
*       3
    exit

   exit
*

 
