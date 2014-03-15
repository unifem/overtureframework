$tFinal=1.; $tPlot=.1; $debug=0;
$grid="square20.hdf";
* 
$grid
*
*  -------------Start domain 1 --------------
  Cgins fluid
* 
  incompressible Navier Stokes
  continue
* 
  forward Euler
* 
  OBTZ:polynomial
  OBTZ:twilight zone flow 1
  OBTZ:degree in space 2
  OBTZ:degree in time 1
* 
    pressure solver options
     * PETScNew
     choose best iterative solver
     * multigrid
     * yale
* for superlu_dist
*     preonly
*     lu preconditioner
*     parallel preonly
*    parallel superlu_dist
*     superlu_dist
* 
*  bcgs with inner lu solve
     parallel bi-conjugate gradient stabilized
**     preonly
**     lu preconditioner
*
     define petscOption -pc_type hypre
     define petscOption -pc_hypre_type boomeramg
     define petscOption -pc_hypre_boomeramg_strong_threshold .5
     * -pc_hypre_boomeramg_coarsen_type <Falgout> (one of) CLJP Ruge-Stueben  modifiedRuge-Stueben   Falgout
     define petscOption -pc_hypre_boomeramg_coarsen_type Falgout
* 
     relative tolerance
       $tol
     absolute tolerance
       1.e-15
     debug 
       $debug
    exit
* 
  boundary conditions
   all=dirichletBoundaryCondition
   Annulus=noSlipWall
   square(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
   square(1,0)=outflow , pressure(1.*p+1.*p.n=0.)
  done
*
 continue
*  -------------End domain 1 ----------------
* -- set parameters for cgmp ---
  final time $tFinal
  times to plot $tPlot
*  turn off twilight
  debug flag $debug
  continue
*