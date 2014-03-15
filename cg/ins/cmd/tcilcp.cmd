*
* Cgins parallel example
* srun -N1 -n2 -ppdebug $cginsp tcilcp
*
*  mpirun -np 2 $cginsp tcilcp.cmd 
*
$tol=1.e-6; $cfl=.95;
$tPlot=.05; $tFinal=.1;  $debug=0; $nu=.1; 
$orderOfAccuracy = "second order accurate"; 
*
$gridName="cice2.order2.hdf"; $nu=.02; $tPlot=.1; $tFinal=1.;
* $gridName="cic3e.hdf"; $nu=.02; $tPlot=.1; $tFinal=1.; 
* $gridName="cic3e.hdf"; $nu=.02; $tPlot=.1; $tFinal=1.; 
* $gridName="tcilc1e.hdf"; $nu=.02; $tPlot=.1; $tFinal=1.; 
* $gridName="tcilc2e.hdf"; $nu=.02; $tPlot=.1; $tFinal=1.; 
* 
$gridName
*
  incompressible Navier Stokes
  exit
*
**  fourth order accurate
$orderOfAccuracy
  turn off twilight zone 
*
**  implicit
   choose grids for implicit
     all=implicit
*     square=explicit
    done
*
*
  final time $tFinal
  times to plot $tPlot
  plot and always wait
  * no plotting
  pde parameters
    nu $nu
  done
  * OBPDE:divergence damping 0.
*
* OBPDE:use new fourth order boundary conditions 1
*
*
  cfl $cfl
*
* -----------------------
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
*-     define petscOption -pc_type hypre
*-     define petscOption -pc_hypre_type boomeramg
*-     define petscOption -pc_hypre_boomeramg_strong_threshold .5
     * -pc_hypre_boomeramg_coarsen_type <Falgout> (one of) CLJP Ruge-Stueben  modifiedRuge-Stueben   Falgout
*-     define petscOption -pc_hypre_boomeramg_coarsen_type Falgout
* 
     relative tolerance
       $tol
     absolute tolerance
       1.e-15
     debug 
       $debug
    exit
* --------------------
   implicit time step solver options
     * choose best iterative solver
     * these tolerances are chosen for PETSc
     choose best iterative solver
     * PETScNew     
     relative tolerance
       $tol
     absolute tolerance
       1.e-15
     debug 
      $debug
    exit
*
*
  boundary conditions
   all=noSlipWall
   square=slipWall
   square(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
   square(1,0)=outflow , pressure(1.*p+1.*p.n=0.)
  done
  initial conditions
  uniform flow
    p=1., u=1.
  exit
* 
  debug $debug
* 
 project initial conditions
 continue
*


 continue
 finish

 continue

*  movie mode
  finish

 movie mode
 finish
