*
* cgins command file: flow past a cylinder with a passive scalar source on a boundary
*
# $grid="cic.hdf"; $nu=.1; $tFinal=2.; $tPlot=.25; $show="passive.show"; $psolver="yale"; 
$grid="cice2.order2.hdf"; $nu=.1; $tFinal=2.; $tPlot=.25; $show="passive.show"; $psolver="yale"; 
* $grid="cice2.order2.hdf"; $psolver="choose best iterative solver"; 
*
* specify the overlapping grid to use:
$grid 
* 
  incompressible Navier Stokes
  passive scalar advection
  exit
*
  show file options
    compressed
    open
      $show
    frequency to flush
      20
    exit
*
  turn off twilight zone 
  implicit
  choose grids for implicit
    all=implicit
    square=explicit
    done
  final time $tFinal 
  times to plot $tPlot 
*  plot and always wait
  no plotting
*
  OBPDE:nu $nu
  OBPDE:passive scalar diffusion coefficient .1
* 
   pressure solver options
    $psolver
    * yale
     relative tolerance
       1.e-8 1.e-4  1.e-6  1.e-4
     absolute tolerance
       1.e-6
    exit
  boundary conditions
   square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1., s=0.)
   square(1,0)=outflow
   square(0,1)=slipWall
   square(1,1)=slipWall
   Annulus(0,1)=noSlipWall, userDefinedBoundaryData
   * user defined boundary values are assigned here: (see common/src/userDefinedBoundaryValues.C)
     wall with scalar flux
     * amp, radius, x0, y0, z0
       1. .5 0. .5 0.
     done
   done
 initial conditions
 uniform flow
    p=1., u=1., s=0.
    exit
  project initial conditions
*
  plot and always wait
  continue
  plot:s
*

  movie mode
  finish







