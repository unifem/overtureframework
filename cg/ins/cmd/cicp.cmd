*
* cgins command file for cic
*
* examples:
*   mpirun -np 2 $cginsp cicp.cmd 
*   totalview srun -a -N2 -n2 -ppdebug $cginsp cicp.cmd 
*
$orderOfAccuracy = "second order accurate"; $backGround="square"; $debug=0; 
$order4 = "fourth order accurate";
$tPlot=.1; $tFinal=1.; $nu=.1; $tol=1.e-7; $maxIterations=200; 
$show=" ";
*
* -- set the solver:
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu";
* $ksp="bcgs"; $pc="hypre"; 
$iluLevels=1; 
*
* $gridName="cice.hdf"; $tPlot=.1;  $tFinal=.1; $tol=1.e-5; $nu=.2; 
* $gridName="cic2.stretched.hdf"; $tPlot=.01;  $tFinal=.1; $tol=1.e-5; $nu=.05; 
* $gridName="nonSquare5.hdf";  $tPlot=.01; $tFinal=.5; $tol=1.e-5; 
* $gridName="nonBox5.hdf";  $tPlot=.01; $tFinal=.5; $tol=1.e-5; 
$gridName="cice2.hdf";  $tPlot=.1; $tFinal=1.; $tol=1.e-5;
* $gridName="cice2.order4.hdf";  $tPlot=.1; $tFinal=.1; $tol=1.e-5; $orderOfAccuracy = $order4;
* $gridName="cice2.order4.hdf";  $tPlot=.25; $tFinal=.5; $tol=1.e-5; 
* $gridName="cice2.order4.hdf";  $orderOfAccuracy = "fourth order accurate"; $tPlot=.02; 
* 
* $gridName="cic3e.hdf"; $tPlot=.5; $tFinal=.5; $nu=.02;  $show="cic3p.show"; $tol=1.e-3; 
* ---- need to remake cic4e -- currently it is not 2x coarser than 5e
* $gridName="cic4e.hdf"; $tPlot=1.; $tFinal=1.; $nu=.01;  $show="cic4p.show"; $tol=1.e-4; 
*   250K pts:
*  $gridName="cic5e.hdf"; $tPlot=.025; $tFinal=.025; $nu=.01;  $show="cic5p.show"; $tol=1.e-3; 
*   1.1M pts:
* $gridName="cic6e.hdf"; $tPlot=.01; $tFinal=.01; $nu=.002;  $show="cic6p.show"; $tol=1.e-5; 
*   4.31M pts:
* $gridName="cic7e.hdf"; $tPlot=.05; $tFinal=.05; $nu=.001;  $show="cic7p.show"; $tol=1.e-4; $show=" ";
* $gridName="cic7e.hdf"; $tPlot=.1; $tFinal=.1; $nu=.001;  $show="cic7p.show"; $tol=1.e-4; $show=" ";
* $gridName="cic7e.hdf"; $tPlot=.2; $tFinal=.2; $nu=.001;  $show="cic7p.show"; $tol=1.e-4; $show=" ";
*   17.1M pts:
* $gridName="cic8e.hdf"; $tPlot=.001; $tFinal=.001; $nu=.0005;  $show="cic8p.show"; $tol=1.e-6; 
*
* $gridName="tcilc1e.hdf"; $tPlot=.1;  $tFinal=.1; $tol=1.e-5; $nu=.05; 
*
* --- two cylinders ---
*  $gridName="tcilc3e.hdf"; $tPlot=.01; $tFinal=.01; $nu=.005;  $show="tcilc4e.show"; $tol=1.e-3; 
*  $gridName="tcilc4e.hdf"; $tPlot=.01; $tFinal=.01; $nu=.001;  $show="tcilc4e.show"; $tol=1.e-4; 
*   tcilc5e: 3.3M pts, -np4, pressure ok, implicit=not-enough-memory
*                      -np8, pressure ok, implicit=ok
*  $gridName="tcilc5e.hdf"; $tPlot=.002; $tFinal=.01; $nu=.0005;  $show="tcilc6e.show"; $tol=1.e-4; 
*    -np=16 no,  -np=32 pressure=ok
*  $gridName="tcilc6e.hdf"; $tPlot=.002; $tFinal=.01; $nu=.0001;  $show="tcilc6e.show"; $tol=1.e-4; 
*
* $gridName="sib2e.hdf"; $tPlot=.05;  $tFinal=1.; $tol=1.e-5; $nu=.05; $backGround="box";
* $gridName="cylinderInAShortChannel1.hdf"; $tPlot=.01;  $tFinal=.1; $tol=1.e-3; $nu=.05; $backGround="box"; 
* $gridName="cylinderInAChannel1.hdf"; $tPlot=1.;  $tFinal=40.; $tol=1.e-3; $nu=.025; $backGround="box"; $show="cicp.show";
*
* $gridName="nonBox8.hdf"; $tPlot=.01;  $tFinal=.1; $tol=1.e-3; $nu=.1; $backGround="box";
* 
*
$gridName
*
  incompressible Navier Stokes
  exit
*
   show file options
     * uncompressed
    open
      $show
     frequency to flush
       1
     exit
*
$orderOfAccuracy
*
 turn off twilight zone 
** turn on polynomial
* 
  implicit
   choose grids for implicit
     all=implicit
     $backGround=explicit
   done
*
  final time $tFinal
  times to plot $tPlot
  * plot and always wait
*   disable plotting 1
  plot and always wait
*  no plotting
* 
  pde parameters
    nu $nu
*    divergence damping
*      2.
    done
*
*   slow start time interval
*     .25
*   slow start cfl
*     .25
*  frequency to flush the show file
*    1
   pressure solver options
    * PETScNew
    choose best iterative solver
    * yale
* for superlu_dist
*     preonly
*     lu preconditioner
*     parallel preonly
*    parallel superlu_dist
*     superlu_dist
* 
     parallel bi-conjugate gradient stabilized
*
     * define petscOption -ksp_monitor stdout
     * define petscOption -ksp_view
     define petscOption -ksp_type $ksp
     define petscOption -pc_type $pc
     define petscOption -sub_ksp_type $subksp
     define petscOption -sub_pc_type $subpc
     define petscOption -pc_factor_levels $iluLevels
     define petscOption -sub_pc_factor_levels $iluLevels
*    -- hypre options (if hypre is used)
     define petscOption -pc_hypre_boomeramg_max_levels 30 
     define petscOption -ksp_type bcgs
     define petscOption -ksp_monitor 1
     define petscOption -pc_hypre_type boomeramg
     define petscOption -pc_hypre_boomeramg_strong_threshold .5
     * -pc_hypre_boomeramg_coarsen_type <Falgout> (one of) CLJP Ruge-Stueben  modifiedRuge-Stueben   Falgout
     define petscOption -pc_hypre_boomeramg_coarsen_type Falgout
     define petscOption -pc_hypre_boomeramg_print_statistics 0 
**
     maximum number of iterations
      $maxIterations
     relative tolerance
       $tol 
     absolute tolerance
       1.e-12
     maximum number of iterations
       $maxIterations
     debug 
      1
    exit
*
   implicit time step solver options
    * PETScNew
    choose best iterative solver
    * yale
     parallel bi-conjugate gradient stabilized
*
     * define petscOption -ksp_monitor stdout
     * define petscOption -ksp_view
     define petscOption -ksp_type $ksp
     define petscOption -pc_type $pc
     define petscOption -sub_ksp_type $subksp
     define petscOption -sub_pc_type $subpc
     define petscOption -pc_factor_levels $iluLevels
     define petscOption -sub_pc_factor_levels $iluLevels
*    -- hypre options (if hypre is used)
     define petscOption -pc_hypre_boomeramg_max_levels 30 
     define petscOption -ksp_type bcgs
     define petscOption -ksp_monitor 1
     define petscOption -pc_hypre_type boomeramg
     define petscOption -pc_hypre_boomeramg_strong_threshold .5
     * -pc_hypre_boomeramg_coarsen_type <Falgout> (one of) CLJP Ruge-Stueben  modifiedRuge-Stueben   Falgout
     define petscOption -pc_hypre_boomeramg_coarsen_type Falgout
     define petscOption -pc_hypre_boomeramg_print_statistics 0 
*
     maximum number of iterations
      $maxIterations
     relative tolerance
       $tol
     absolute tolerance
       1.e-12
     maximum number of iterations
       $maxIterations
     debug 
       1
    exit
********
**    OBPDE:include artificial diffusion in pressure equation 1
**    OBPDE:fourth-order artificial diffusion
**    OBPDE:use implicit fourth-order artificial diffusion 1
**    OBPDE:ad41,ad42 1,1
********
  boundary conditions
   all=slipWall
   square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.)
*    square(0,0)=inflowWithVelocityGiven, ramp(ta=0.,tb=1.,ua=0.,ub=2.)
*    square(0,0)=inflowWithVelocityGiven, userDefinedBoundaryData
*      variable inflow
*     1. 0. 0. 
*      done
*    square(0,0)=inflowWithVelocityGiven, userDefinedBoundaryData
*      time dependent inflow
*      1. 0. 0. 
*      done
    square(1,0)=outflow
    square(0,1)=slipWall
    square(1,1)=slipWall
*
    outer-square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.)
    outer-square(1,0)=outflow
    outer-square(0,1)=slipWall
    outer-square(1,1)=slipWall
*
    box(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.)
    box(1,0)=outflow
*     cylinder=noSlipWall
    cylinder(0,2)=noSlipWall
*
    annulus1=noSlipWall
    annulus2=noSlipWall
**    all=dirichletBoundaryCondition
* 
** test parabolic inflow
   square(0,1)=noSlipWall
   square(1,1)=noSlipWall
   square(0,0)=inflowWithVelocityGiven , parabolic(d=1.,p=1.,u=1.)
* 
   done
  initial conditions
  uniform flow
    p=1., u=1.
    exit
  project initial conditions
*
  debug $debug
  continue
*

  movie mode
  finish







