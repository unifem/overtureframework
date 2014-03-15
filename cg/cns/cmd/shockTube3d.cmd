*
* cgcns command file for a 3d shock-tube
*
$tFinal=1.; $tPlot=.1; $show="";
$numberOfLevels=2;  $refinementRatio=4;  $regrid=2; 
* 
* box40
* box40-2-2
*
* $grid="/home/henshaw/Overture/ogen/box10-2-2"; 
$grid="box10"; $numberOfLevels=3;   $regrid=8; $tPlot=.01;  $show="shockTube3d.show";
*
$grid
*
*  compressible Navier Stokes (Jameson)  
  compressible Navier Stokes (Godunov)  
  exit
  turn off twilight
  final time $tFinal
  times to plot $tPlot
  plot and always wait
*  no plotting
  show file options
    uncompressed
      open
        $show
      frequency to flush
        2
      exit
*
*   save a restart file 1
*
*  variable time step PC
*
  pde parameters
      mu
      0.
      kThermal
      0.
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
  boundary conditions
    box=slipWall
    box(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
    box(1,0)=superSonicOutflow
    done
* 
  cfl
   1.
* 
 turn on adaptive grids
  order of AMR interpolation
      2
  error threshold
       .4
  regrid frequency
    $regrid
  change error estimator parameters
    default number of smooths
      1
    set scale factors     
      2 1 1 1 1 
    done
    exit
  change adaptive grid parameters
    refinement ratio
      $refinementRatio
    default number of refinement levels
      $numberOfLevels
  exit
*
  initial conditions
    * x=.5
    step function
      x=.25
      r=2.6667 u=1.25 e=10.119
      r=1. e=1.786
    continue
  debug 
   1 7 31
  continue




