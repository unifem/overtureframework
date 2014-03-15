*
* cgins example: compute flow past an H-grid
*
* specify the overlapping grid to use:
$tFinal=40.; $tPlot=.1; $show = " "; $nu=.1; 
* specify the overlapping grid to use:
$grid="hgrid.hdf";
*
$grid
* 
* Specify the equations we solve:
  incompressible Navier Stokes
  exit
  turn off twilight zone 
  final time $tFinal
  times to plot $tPlot
  plot and always wait
  * no plotting
* choose implicit time stepping:
  implicit
* but integrate the square explicitly:
  choose grids for implicit
    all=implicit
    backGround=explicit
    done
  pde parameters
    nu .025
    turn on second order artificial diffusion
    done
  boundary conditions
    all=noSlipWall
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

 
