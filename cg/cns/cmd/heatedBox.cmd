*
* Heated box in a gravitational field
*
$tFinal=10.; $tPlot=.1; $show=" "; $backGround="square"; 
$mu=.01; $kThermal=.14; 
$gg=10.; $gravity = "0 $gg 0.";
$T0=300.; $Twall=$T0+10.; 
* 
* $grid="square5.hdf"; $tPlot=.01;
$grid="square20.hdf"; $tPlot=.5; $show="heatedBox.show"; $tFinal=5.;
* $grid="nonSquare20.hdf"; $tPlot=.1;
* $grid="rotatedSquare20.hdf"; $tPlot=.1; $gg=$gg/sqrt(2.); $gravity="-$gg $gg 0."; 
* $grid="square40.hdf"; $tPlot=.5;
* $grid="square40.hdf"; $mu=.01; $kThermal=.014; $tPlot=.5;
*
* $grid="box20.hdf"; $backGround="box"; $tPlot=.05; 
* 
$grid
*
*   compressible Navier Stokes (Godunov)  
  compressible Navier Stokes (Jameson)
  exit
  turn off twilight
*
*
  final time $tFinal 
  times to plot $tPlot
**
  plot and always wait
  * no plotting
***
  show file options
    compressed
      open
      $show
    frequency to flush
     4
    exit
***
  reduce interpolation width
    2
* **************
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
    $backGround(0,0)=noSlipWall, uniform(T=$Twall)
    $backGround(1,0)=noSlipWall, uniform(T=$T0)
    * adiabatic walls:
    $backGround(0,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    $backGround(1,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
* 
*   $backGround(0,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
*   $backGround(1,2)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
    $backGround(0,1)=slipWall 
    $backGround(1,1)=slipWall 
    done
*  debug
*    1
  initial conditions
    uniform flow
      r=1. u=0. T=$T0
  exit
  continue
* 
  contour
    ghost lines 1
    exit


