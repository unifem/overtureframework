*
* dropping cylinders
*
  dropFlow.hdf
  incompressible Navier Stokes
  exit
*
  show file options
  *  open
  *   twoDrop.show
    frequency to flush
      4
  exit  
  turn off twilight zone
  degree in space
    1
  project initial conditions
  turn on moving grids
  detect collisions 1
  specify grids to move
      rigid body
        mass
          .5
        moments of inertia
          1.
        initial centre of mass
           .25 -2.75
        done
        drop
       done
      rigid body
        mass
          .5 
        moments of inertia
          1.
        initial centre of mass
          .75 -1.75
        done
        drop2
      done
      rigid body
        mass
          .5 
        moments of inertia
          1.
        initial centre of mass
          -.75 -1.75
        done
        drop3
      done
  done
  * use implicit time stepping
  *   implicit
  choose grids for implicit
     all=explicit
     drop=implicit
    done
  pde parameters
    nu
      .025
*  turn on gravity
  gravity
     0. -1. 0.
    done
  boundary conditions
    all=noSlipWall
    $velocity=.5;
    channel(0,1)=inflowWithVelocityGiven,  parabolic(d=.2,p=1,v=$velocity)
    channel(1,1)=outflow
    done
  initial conditions
    uniform flow
     p=1., v=$velocity
  exit
  final time 6.
  times to plot .1 
  plot and always wait
 * no plotting
*  debug
*     63
  continue
*


    grid
      raise the grid by this amount (2D) 1
      raise the grid by this amount (2D) 2
    exit this menu
    plot:v



*  output resolution
*   512
* movie and save
