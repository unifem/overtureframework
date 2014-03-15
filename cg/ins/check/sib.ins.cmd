*
* Sphere in a box
*
sib
  incompressible Navier Stokes
  exit
  turn on polynomial
  * turn on trigonometric
  final time .1
  times to plot .05
  * plot and always wait
  no plotting
  pde parameters
    nu
    .05
    done
  boundary conditions
    all=noSlipWall
   done
  pressure solver options
     * choose best iterative solver
     * use slap solver since it will always be there (makes tests consistent)
     slap
     * these tolerances are chosen for PETSc
     relative tolerance
       1.e-6  1.e-5
     absolute tolerance
       1.e-8  1.e-7
    exit
continue
*
movie mode
finish
