*
* cgins: flow through two pipes that join with a smooth fillet
*
* Usage:
*  cgins [-noplot] twoPipes -g=<name> -tf=<tFinal> -tp=<tPlot> ...
*                     -solver=<yale/best> -order=<2/4> -model=<ins/boussinesq> -ts=<implicit>
* 
* Examples:
* 
*  cgins twoPipes -g=twoPipese2.order2.hdf -show=twoPipes.show
*  cgins twoPipes -g=twoPipese2.order2.hdf -ts=implicit
* 
*  srun -N1 -n2 -ppdebug $cginsp noplot twoPipes -g=twoPipese2.order2.hdf -show=twoPipes.show >! twoPipes.out
*  srun -N2 -n16 -ppdebug $cginsp -noplot twoPipes -g=twoPipese4.order2.hdf -nu=.005 -show=twoPipes4.show >! twoPipes4.out &
*
* --- set default values for parameters ---
* 
$grid=twoPipese2.order2.hdf; $show=" "; 
$model="ins"; $ts="adams PC"; $noplot=""; 
$tFinal=1.; $tPlot=.1; $cfl=.9;  $maxIterations=100; $tol=1.e-3; $atol=1.e-4; 
$nu=.01; $prandtl=.72; $thermalExpansivity=.1;
$debug = 1; $ogesDebug=0; 
* 
$solver="best"; 
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
* $ksp="gmres"; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"model=s"=>\$model,"nu=f"=>\$nu,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order, \
 "ts=s"=>\$ts, "noplot=s"=>\$noplot );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
* 
$kThermal=$nu/$prandtl;
* 
* grid name:
$grid
* 
* equations to solve:
  $model
  define real parameter kappa $kThermal
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
  exit
*
turn off twilight zone
* 
* 
final time $tFinal
times to plot $tPlot
plot and always wait
* 
cfl $cfl
pde parameters
  nu $nu
done
* -- time stepping method: 
$ts
*
  choose grids for implicit
    all=implicit
  done
***
show file options
   compressed
   OBPSF:maximum number of parallel sub-files 4
   open
     $show
  frequency to flush
    2
exit
*
   pressure solver options
     $solver
     define petscOption -ksp_type $ksp
     define petscOption -pc_type $pc
     define petscOption -sub_ksp_type $subksp
     define petscOption -sub_pc_type $subpc
     define petscOption -pc_factor_levels $iluLevels
     define petscOption -sub_pc_factor_levels $iluLevels
     * define petscOption -pc_factor_nonzeros_along_diagonal 1
*
     maximum number of iterations
      $maxIterations
     relative tolerance
       $tol 
     absolute tolerance
       $atol
     maximum number of iterations
       $maxIterations
     debug 
      $ogesDebug
    exit
*
   implicit time step solver options
    $solver
     maximum number of iterations
      $maxIterations
     relative tolerance
       $tol
     absolute tolerance
       1.e-12
     maximum number of iterations
       $maxIterations
     debug 
       $ogesDebug
    exit
* 
initial conditions
  uniform flow
   u=0., p=1.
done
boundary conditions
  all=noSlipWall
  bcNumber4=inflowWithVelocityGiven, parabolic(d=.2,p=1.,v=-1.)
*  bcNumber4=inflowWithVelocityGiven, uniform(p=1.,v=-1.)
  bcNumber1=outflow
  bcNumber2=outflow
done  
* 
debug $debug
* 
project initial conditions
*
continue
movie mode
finish


* plot grids with wire frame
  grid
    plot shaded surfaces (3D) toggle 0
    exit this menu
  continue
