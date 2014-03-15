***************************************************************
*  sibMove.cmd -- cgcns example --
* 
*     A moving sphere
* 
* Examples:
*    cgcns sibMove
*
*  You may have to make the grid used below (sampleGrids/sibArg.cmd)
**************************************************************
*
$tFinal=1.3; $tPlot=.1; $show=" "; $debug=1; 
$ratio=2; $levels=2; $bufferZones=2; $tol=.05; 
$amrOff = "turn off adaptive grids";
$amrOn = "turn on adaptive grids";
$amr=$amrOff;
* 
$grid="sibi1.order2.hdf"; $amr=$amrOn; $tPlot=.05; $levels=3;
* $grid="sibi4.order2.hdf"; $amr=$amrOff; $tPlot=.05;
* $grid="sibi8.order2.hdf"; $amr=$amrOff; $tPlot=.1; $show="sibShockMove.show"; 
* 
*
$grid
*
*  Either Jameson or Godunov should work
*   compressible Navier Stokes (Jameson)
  compressible Navier Stokes (Godunov)
*   one step
  exit
  turn off twilight
*
*  do not use iterative implicit interpolation
*
  final time $tFinal
  times to plot $tPlot
*plot and always wait
*
  show file options
    compressed
    open
      $show
    frequency to flush
      1
    exit
  * no plotting
****
* turn on user defined output
****
*****************************
  turn on moving grids
  specify grids to move
 *       oscillate
 *        1. .5 0
 *       1.
 *      .25
 *       0.
     translate
       1. 0. 0.
        1. 
      south-pole
      north-pole
    done
  done
***************************
  reduce interpolation width
    2
*****
  $amr
  order of AMR interpolation
      2
  error threshold
     $tol
  regrid frequency
    $regrid=$ratio*$bufferZones;
    $regrid
  change error estimator parameters
    set scale factors
      1 1 1 1 1 1 
    done
    weight for first difference
      1.
    weight for second difference
      1.
    exit
    truncation error coefficient
      1.
    show amr error function
  change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $levels
    number of buffer zones
      $bufferZones
    grid efficiency
      .7
  exit
*****
  boundary conditions
    all=slipWall
  done
*
  pde parameters
    mu
     0.0
    kThermal
     0.0
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
***************
*   OBPDE:exact Riemann solver
* OBPDE:Roe Riemann solver
* OBPDE:HLL Riemann solver
*   OBPDE:Godunov order of accuracy 2
******************
 debug $debug
*
 initial conditions
   uniform flow
    r=1. e=1.786 s=0.
  continue
continue
*

movie mode
finish




