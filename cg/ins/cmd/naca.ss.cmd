*
* cgins: steady state solver : command file for flow past a naca0012 airfoil
*  use either naca0012.hdf or naca.hype.hdf
*
* naca.hype
$tFinal=10.; $tPlot=1.; $debug=3; $cfl=.9; $its=10000; $pits=100; $show="naca.ss.show"; 
$solver="yale"; $rtol=1.e-8; $atol=1.e-6; $dtMax=1.e-4; 
$grid="naca0012.hdf"; $nu=1.e-8; 
*
$grid
*
  incompressible Navier Stokes
  exit
*
  show file options
    open
      $show
    frequency to flush
      5
  exit
*
  max iterations $its
  plot iterations $pits
  turn off twilight zone 
* 
  final time $tFinal.
  times to plot $tPlot
  debug $debug
*
  plot and always wait
  * no plotting
  pde parameters
    * the next value for nu is too small to have any effect.
    nu
      $nu
   turn on second order artificial diffusion
  OBPDE:ad21,ad22 5. 5.
  done
*
    steady state RK-line
    dtMax $dtMax
*
  cfl $cfl
   pressure solver options
    $solver
     relative tolerance
       $rtol
     absolute tolerance
       $atol
    exit
* 
  boundary conditions
    all=slipWall
    backGround(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.)
    backGround(1,0)=outflow
    backGround(0,1)=slipWall
    backGround(1,1)=slipWall
    done
  initial conditions
  uniform flow
    p=1., u=1.
  done
  project initial conditions
  continue
