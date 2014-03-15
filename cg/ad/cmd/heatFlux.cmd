*
*   cgad example: apply a heat flux BC
*   
*  cgad heatFlux -g="square40.hdf" -a=0. -b=0. -c=0. -kappa=.1 -go=halt
*  cgad heatFlux -g="square40.hdf" -a=0. -b=0. -c=0. -kappa=.1 -ts=imp -go=halt  (t large soln is u = (x-.5)^2 + (y-.5)^2 + 4*kappa*t + const)
* 
$tFinal=5.; $tPlot=.1; $grid = "square40.hdf"; $kappa=.1; $a=0.; $b=0.; $c=0.; $dtmax=.1; 
* 
* ----------------------------- get command line arguments ---------------------------------------
*  -- first get any commonly used options: (requires the env variable CG to be set)
$getCommonOptions = "$ENV{'CG'}/mp/cmd/getCommonOptions.h";
include $getCommonOptions
*  -- now get additional options: 
GetOptions("a=f"=>\$a,"b=f"=>\$b,"c=f"=>\$c,"kappa=f"=>\$kappa );
* -------------------------------------------------------------------------------------------------
*
$grid
* 
  convection diffusion
* 
  continue
* 
  turn off twilight zone 
  * turn on trig
  final time $tFinal
  times to plot $tPlot
  plot and always wait
  * no plotting
* -- time-stepping method --
  $ts
  implicit factor $implicitFactor (1=BE,0=FE)
  dtMax $dtMax
* 
  choose grids for implicit
    all=implicit
  done
*
  pde parameters
    kappa $kappa
    a $a
    b $b
    c $c
  done
  boundary conditions
    * all=dirichletBoundaryCondition
    * square(0,0)=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=1.)
    all=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=1.)
    * use a smaller dtMax with the following BC: 
    * all=mixedBoundaryCondition, mixedDerivative(1.*t+.01*t.n=1.)
   done
*  debug
*    7
  initial conditions
    OBIC:uniform state T=0. 
    OBIC:assign uniform state
  continue
* 
 continue
 $go 
