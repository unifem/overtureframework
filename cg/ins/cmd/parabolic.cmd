*
* Cgins test parabolic inflow
*
* mpirun -np 2 $cginsp parabolic
* mpirun -np 2 $cginsp parabolic.cmd
*
*  mpirun -np 2 $cginsp parabolic.cmd 
*  srun -N1 -n2 -ppdebug $cginsp parabolic.cmd 
*
$rtol=1.e-5; $atol=1.e-6; $cfl=.95; $debug=1; $backGround="square"; 
$tz=0; # twilight-zone flow is off
$tPlot=.05; $tFinal=.1;  $degreeX=2; $degreeT=2; $nu=.1; $its=100; $pits=5; 
$orderOfAccuracy = "second order accurate"; 
*
* $gridName="square20.hdf"; $tPlot=.05; $tFinal=.2; $tz=0; 
$gridName="box20.hdf"; $tPlot=.02; $tFinal=.05; $backGround="box";
* 
$gridName
*
  incompressible Navier Stokes
  exit
*
**  fourth order accurate
$orderOfAccuracy
*
*
    steady state RK-line
    dtMax .5
* 
*
  max iterations $its
  plot iterations $pits
* 
**  implicit
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
  cfl $cfl
*
  pde parameters
    nu
     $nu
    done
   pressure solver options
     choose best iterative solver
     relative tolerance
       $tol
     absolute tolerance
       1.e-15
     debug 
       3
    exit
*
   implicit time step solver options
     * choose best iterative solver
     * these tolerances are chosen for PETSc
     choose best iterative solver
     * PETScNew     
     relative tolerance
       $rtol
     absolute tolerance
       $atol
     debug 
      1 7 0 1 7 3
    exit
*
*
  boundary conditions
   all=noSlipWall
*   $backGround(0,0)=inflowWithVelocityGiven , parabolic(d=.4,p=1.,u=1.)
*   $backGround(1,0)=outflow , pressure(1.*p+1.*p.n=0.)
  $backGround(0,1)=inflowWithVelocityGiven , parabolic(d=.4,p=1.,v=1.)
  $backGround(1,1)=outflow , pressure(1.*p+1.*p.n=0.)
   done
  debug $debug
* 
  initial conditions
  uniform flow
*   p=1., u=1.
    p=1., v=1. 
  exit
*   project initial conditions
 continue
*
plot:v 


 continue
 finish

 continue

*  movie mode
  finish

 movie mode
 finish
