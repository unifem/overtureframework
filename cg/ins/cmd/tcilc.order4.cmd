*
* cgins - INS - fourth order
*
* tcilc4.order4
* tcilc3.order4.hdf
$tPlot=.01; $tFinal=.01; $nu=.1; $tol=1.e-5; 
$order = "fourth order accurate";
$order2 = "second order accurate"; 
$solver="choose best iterative solver";
* 
* $gridName ="tcilc3e.order4.hdf"; $tPlot=.05; $tFinal=.1; $show="tcilc3e.show"; $nu=.01; 
* $gridName ="tcilc4e.order4.hdf"; $tPlot=.025; $tFinal=.05; $show="tcilc4e.show"; $nu=.005; $tol=1.e-4; 
* $gridName ="tcilc5e.order4.hdf"; $tPlot=.0125; $tFinal=.025; $show="tcilc5e.show"; $nu=.0025; $tol=1.e-4; 
*
* $gridName ="tcilc5e.hdf"; $tPlot=.001; $tFinal=.001; $show="tcilc5e.show"; $nu=.001; $tol=1.e-4; $order=$order2; 
$gridName ="tcilc2e.order4.hdf"; $tPlot=.05; $tFinal=.1; $show="tcilc2e.show"; $nu=.02; 
* $gridName ="tcilc3e.order4.hdf"; $tPlot=.05; $tFinal=.1; $show="tcilc3e.show"; $nu=.01; 
*
$gridName
* 
  incompressible Navier Stokes
  exit
*
$order
*
   implicit
   choose grids for implicit
     all=implicit
     square=explicit
   done
*
**  turn on poly
*   degree in space 4
*   (space=4,time=1) works  (space=4,time=2) no
*   degree in time 1
*  turn on trig
**  frequencies (x,y,z,t) 2 2 2 2
* frequencies (x,y,z,t) 1 1 1 0
  turn off twilight zone 
*
  final time $tFinal
  times to plot $tPlot
*
  show file options
    compressed
    open
      $show
    frequency to flush
      1
    exit
*
  plot and always wait
  no plotting
*  OBPDE:divergence damping 0.
  pde parameters
    * nu=.01 for tcilc3 (.005 too small?)
    nu
      $nu
  done
   pressure solver options
     $solver
*    multigrid
* for superlu_dist
*     preonly
*     lu preconditioner
*     parallel preonly
*    parallel superlu_dist
*     superlu_dist
* 
*  bcgs with inner lu solve
*      parallel bi-conjugate gradient stabilized
*      preonly
*      lu preconditioner
*
*     yale
     relative tolerance
       $tol
     absolute tolerance
       1.e-10
      number of incomplete LU levels
         5 7 5
      *
    exit
*
   implicit time step solver options
     $solver
*    multigrid
* for superlu_dist
*     preonly
*     lu preconditioner
*     parallel preonly
*    parallel superlu_dist
*     superlu_dist
* 
*  bcgs with inner lu solve
*      parallel bi-conjugate gradient stabilized
*      preonly
*      lu preconditioner
*
*     yale
     relative tolerance
       $tol
     absolute tolerance
       1.e-10
      number of incomplete LU levels
         5 7 5
      *
    exit
*
*    pressure solver options
*     debug
*      63
*   exit
  boundary conditions
*    all=dirichletBoundaryCondition
     all=noSlipWall
*     square(0,0)=noSlipWall
*     all=slipWall
    square(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
    square(1,0)=outflow 
    square(0,1)=slipWall
    square(1,1)=slipWall
*    square(1,0)=outflow , pressure(.1*p+1.*p.n=0.)
*     square(0,0)=inflowWithVelocityGiven , parabolic(d=.2,p=1.,u=1.), oscillate(t0=.3,omega=2.5)
   done
  initial conditions
  uniform flow
    p=1., u=1.
    exit
**  project initial conditions
  debug
    0
*  check error on ghost 
*   2
 continue

 movie mode
 finish
