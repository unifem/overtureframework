*
* Heated box in a gravitational field
*
$debug = 0;
$oges_debug =0;
$tFinal=10.; $tPlot=1; $show=" "; $backGround="square"; 
$mu=.01; $kThermal=.14; 
$gg=-10.; $gravity = "0. $gg 0.";
$T0=300.; $Twall=$T0+10.; 
*$T0=310.; $Twall=$T0-10.; 
* 
 $grid="square10.hdf"; $tPlot=.01;
 $grid="square5.hdf"; $tPlot=.01;
$grid="square20.hdf"; $tPlot=50; $show="heatedBox_newton.show"; $tFinal=5.;
$grid="square40.hdf"; $tPlot=50; $show="heatedBox_newton.show"; $tFinal=5.;
*
* $grid="nonSquare20.hdf"; $tPlot=.1;
* $grid="rotatedSquare20.hdf"; $tPlot=.1; $gg=$gg/sqrt(2.); $gravity="-$gg $gg 0."; 
* $grid="square40.hdf"; $tPlot=.5;
* $grid="square40.hdf"; $mu=.01; $kThermal=.014; $tPlot=.5;
*
* $grid="box20.hdf"; $backGround="box"; $tPlot=.05; 
* 
$grid
*
*  compressible Navier Stokes (Jameson)
steady-state compressible Navier Stokes (newton)
  exit
  turn off twilight
*
*
  final time $tFinal 
  times to plot $tPlot
**
  plot and always wait
  plot residuals 1
  * no plotting
***
***
*  reduce interpolation width
*    2
* **************
 refactor frequency 1
  implicit time step solver options
block size
4
choose best iterative solver
absolute tolerance
1e-8
choose best direct solver
*harwell
exit
OBPDE:av2,av4 0.,1e-5
Oges::debug (od=)
$oges_debug
debug
$debug
implicit factor 1.
  pde parameters
    mu
     $mu
    kThermal
     $kThermal
    gravity
      $gravity
  done
*
  boundary conditions
*    $backGround(0,0)=noSlipWall, uniform(T=$Twall,u=0,v=0)
*    $backGround(1,0)=noSlipWall, uniform(T=$T0,u=0,v=0)
    * adiabatic walls:
    $backGround(0,0)=noSlipWall, userDefinedBoundaryData
      linear ramp in x
      1 0. 0. 0.
      2 0. 0. 0.
      3 $Twall 0. 0.
    done
    done
    $backGround(1,0)=noSlipWall, userDefinedBoundaryData
      linear ramp in x
      1 0. 0. 0.
      2 0. 0. 0.
      3 $T0 0. 0.
    done
    done
    $backGround(0,1)=noSlipWall, userDefinedBoundaryData
      linear ramp in x
      1 0. 0. 0.
      2 0. 0. 0.
      3 -$Twall 0. -1.
    done
    done
    $grid(1,1)=noSlipWall,uniform(T=$T0,u=0,v=0)
    $backGround(1,1)=noSlipWall, userDefinedBoundaryData
      linear ramp in x
      1 0. 0. 0.
      2 0. 0. 0.
      3 -$T0 0. -1.
    done
    done
*    $grid(0,1)=noSlipWall,uniform(T=$T0,u=0,v=0)
*    $backGround(0,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
*    $backGround(1,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
* 
*   $backGround(0,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
*   $backGround(1,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    done
*  debug
*    1
  initial conditions
    uniform flow
*      r=1. u=1e-8, v=1e-8. T=$T0
      r=1. u=0, v=0. T=$T0
  exit
  continue
  plot iterations 1
  plot residuals 1
* 
  contour
    ghost lines 1
    exit


