*
* Cgins parallel examples with Twilight-zone flow
*
* mpirun -np 2 $cginsp -readCollective -writeCollective p.cmd
* mpirun -np 2 $cginsp p.cmd
*
*  mpirun -np 2 $cginsp p.cmd 
*  srun -N1 -n2 -ppdebug $cginsp p.cmd 
*  srun -N1 -n2 -ppdebug memcheck_all $cginsp noplot p.cmd 
*
$tol=1.e-11; $cfl=.95; $ogesDebug=3; 
$tz=1; # twilight-zone flow is on
$tPlot=.05; $tFinal=.1;  $degreeX=2; $degreeT=2; $nu=.1; 
$orderOfAccuracy = "second order accurate"; 
*
$gridName="square5.hdf"; $tPlot=.1; $tFinal=1.; $degreeX=2; $degreeT=2; 
*  $gridName="square5.hdf"; $tPlot=.2; $tFinal=1.; $degreeX=0; $degreeT=1; 
*  $gridName="square8.order4.hdf"; $tPlot=.05; $tFinal=.05;
* $gridName="square10.hdf"; $tPlot=.1; $tFinal=.5;
* $gridName="square20.hdf"; $tPlot=.05; $tFinal=.2; $tz=1; 
* $gridName="sise2.order2.hdf"; $tPlot=.1; $tFinal=1.;
* $gridName="rsise.hdf"; $tPlot=.05; $tFinal=.1;
* $gridName="nonSquare5.hdf"; $tPlot=.05;  $tFinal=.4; $degreeX=2; $degreeT=2; $tol=1.e-9;
* $gridName="nonSquare8.hdf"; $tPlot=.05; $tFinal=.2;
* $gridName="nonSquare10.hdf"; $tPlot=.01; $tFinal=.1;
* $gridName="square20.hdf"; $tPlot=.1; $tFinal=.1;
* $gridName="nonSquare20.hdf"; $tPlot=.1; $tFinal=.1;
* $gridName="square128.hdf"; $tPlot=.002; $tFinal=.02; $nu=.01; 
* $gridName="square256.hdf"; $tPlot=.001; $tFinal=.01; $nu=.01; 
* $gridName="square512.hdf"; $tPlot=.001; $tFinal=.02; $nu=.001; 
* $gridName="rotatedSquare.hdf"; $tPlot=.05; $tFinal=.1;
* $gridName="cice.hdf"; $tPlot=.05; $tFinal=.1;
* $gridName="cice2.order2.hdf"; $tPlot=.05; $tFinal=.1;
* $gridName="cic3e.hdf"; $tPlot=.05; $tFinal=.1;
*
* $gridName="box5.hdf"; $tPlot=.025; $tFinal=.05;
* $gridName="box4.order4.hdf"; $tPlot=.025; $tFinal=.05; $orderOfAccuracy = "fourth order accurate"; 
* $gridName="nonBox5.hdf"; $tPlot=.025; $tFinal=.05;
* $gridName="bibe.hdf"; $tPlot=.025; $tFinal=.05;
* $gridName="sib2e.hdf"; $tPlot=.01; $tFinal=.05;  $tol=1.e-5;
* 
$gridName
*
  incompressible Navier Stokes
  exit
*
**  fourth order accurate
$orderOfAccuracy
*
  implicit
   choose grids for implicit
     all=implicit
*     square=explicit
    done
*
*
  OBTZ:polynomial
**  turn on trig
  OBTZ:twilight zone flow $tz
  degree in space $degreeX
  degree in time $degreeT
*
  final time $tFinal
  times to plot $tPlot
  * plot and always wait
  no plotting
  * OBPDE:divergence damping 0.
*
OBPDE:use new fourth order boundary conditions 1
*
*
  cfl $cfl
*
  pde parameters
    nu
     $nu
    done
   pressure solver options
*     PETScNew
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
*     define petscOption -pc_type hypre
*     define petscOption -pc_hypre_type boomeramg
*     define petscOption -pc_hypre_boomeramg_strong_threshold .5
     * -pc_hypre_boomeramg_coarsen_type <Falgout> (one of) CLJP Ruge-Stueben  modifiedRuge-Stueben   Falgout
*     define petscOption -pc_hypre_boomeramg_coarsen_type Falgout
* 
     relative tolerance
       $tol
     absolute tolerance
       1.e-15
     debug 
       $ogesDebug
    exit
*
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
      1 7 0 1 7 3
    exit
*
*
  boundary conditions
   all=dirichletBoundaryCondition
*   square(0,1)=slipWall
*    square(0,0)=slipWall
*    all=noSlipWall
*   all=slipWall
*   box(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
*   box(0,1)=noSlipWall
*   box(1,0)=outflow , pressure(1.*p+1.*p.n=0.)
*   outer-box(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
*   outer-box(1,0)=outflow , pressure(1.*p+1.*p.n=0.)
  Annulus=noSlipWall
*   all=noSlipWall
**  square(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
  square(0,0)=inflowWithVelocityGiven , parabolic(d=.2,p=1.,u=1.)
  square(1,0)=outflow , pressure(1.*p+1.*p.n=0.)
*   square(0,1)=slipWall
*   square(1,1)=slipWall
   square(0,1)=noSlipWall
   square(1,1)=noSlipWall
*
  outer-square(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
  outer-square(1,0)=outflow , pressure(.1*p+1.*p.n=0.)
  outer-square(1,1)=noSlipWall
*
*   square(0,0)=noSlipWall
*     square(0,0)=slipWall
*     all=slipWall
*   square(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
*    square(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
*    square(0,1)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
*    square(1,0)=outflow 
*    square(1,0)=outflow , pressure(.1*p+1.*p.n=0.)
*     square(0,0)=inflowWithVelocityGiven , parabolic(d=.2,p=1.,u=1.), oscillate(t0=.3,omega=2.5)
   done
  debug
   1  31 1 31 
*   check error on ghost
*     1
 continue
*
movie mode
finish

 continue
 finish

 continue

*  movie mode
  finish

 movie mode
 finish
