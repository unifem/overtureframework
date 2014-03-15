*
* cgins: flow through two pipes that join
*
* grid name:
* 
pipes.hdf
*
* equations to solve:
  incompressible Navier Stokes
  exit
*
turn off twilight zone
project initial conditions
final time  1.
times to plot .05
plot and always wait
* save the speed in the show file:
show file variables
  speed
done
pde parameters
  nu
  .05
  done
* use iterative solver for the pressure equation
  pressure solver options
     choose best iterative solver
     * slap
     relative tolerance
       1.e-6  
     absolute tolerance
       1.e-7 
  exit
initial conditions
  uniform flow
   u=0., p=1.
done
boundary conditions
  all=noSlipWall
  mainPipe(0,1)=inflowWithVelocityGiven, parabolic(d=.2,p=1.,u=1.)
  mainCore(0,0)=inflowWithVelocityGiven, parabolic(d=.2,p=1.,u=1.)
  mainPipe(1,1)=outflow
  mainCore(1,0)=outflow
  branchCore(1,1)=outflow
  branchPipe(1,1)=outflow
done  
continue
* plot grids with wire frame
  grid
    plot shaded surfaces (3D) toggle 0
    exit this menu
  continue
