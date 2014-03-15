*
* all-speed flow past a body
*
$show =" "; $Mach=.1; $tFinal=.5; $tPlot=.1; $debug=0; $cfl=.9; $anu=5.; $nuRho=1.; 
$method ="linearized all speed implicit"; 
* $method="all speed implicit"; 
* $grid="ellipse.hdf"; $method="adams order 2"; $cfl=.25; $anu=10.; $backGround="backGround";
* $grid="ellipse.hdf"; $backGround="backGround"; $Mach=.1; $tPlot=.001;
$grid="sinfoil.hdf"; $backGround="airfoil"; $Mach=.1; $tPlot=.1;
* $grid="sinfoil2.hdf"; $backGround="airfoil"; $Mach=.1; $tPlot=.0003;
*
$grid
*
  all speed Navier Stokes
  exit
*
  turn off twilight zone 
* 
  $method
* 
  OBPDE:densityFromGasLawAlgorithm
  * OBPDE:defaultAlgorithm
*
* 
  final time $tFinal
  cfl $cfl 
  times to plot $tPlot
* 
* Next specify the file to save the results in. 
* This file can be viewed with Overture/bin/plotStuff.
  show file options
    * compressed
    open
     $show
    frequency to flush
      5
    exit
*
  plot and always wait
  * no plotting
  debug $debug 
*
  pde parameters
    Mach number
     $Mach
**   OBPDE:pressureLevel 10.
    Reynolds number
     10.
    OBPDE:nuRho $nuRho
    OBPDE:anu $anu
* 
*     OBPDE:linearize implicit method 0
* 
    done
* 
  boundary conditions
    all=slipWall  uniform(T=1.)
    $backGround(0,0)=subSonicInflow uniform(r=1.,u=.1,v=0.,T=1.)
*    $backGround(1,0)=subSonicOutflow uniform(T=1.)
*     $backGround(1,0)=subsonicOutflow  pressure(1.*p+1.*p.n=1.)
    $backGround(1,0)=convectiveOutflow  pressure(1.*p+1.*p.n=1.)
    $backGround(0,1)=slipWall
    $backGround(1,1)=slipWall
    done
  initial conditions
    uniform flow
      r=1., u=.1, T=1.
  exit
  project initial conditions
  continue
* 

 movie mode
 finish 
