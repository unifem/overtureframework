*
* cgcns command file for a shock hitting a 3d bubble
*
$show = "  ";
$cfl=.9; $radius=.2; $debug=1; $x0=.5; $y0=.5; $z0=.5; $xShock=.2; 
$ratio=2; $buffer=2; $errTol=.05;  $efficiency=.7; $levels=2;
*
* $gridName ="box20.hdf"; $tFinal=.2; $tPlot=.05;
* $gridName ="box40.hdf"; $tFinal=.2; $tPlot=.02;
$gridName ="box80.hdf"; $tFinal=.2; $tPlot=.05; $efficiency=.5; $show="bubble3d.show"; 
*
$gridName
***
** compressible Navier Stokes (Jameson)
   compressible Navier Stokes (Godunov)
  exit
  turn off twilight
  final time $tFinal
  times to plot $tPlot
*  plot and always wait
  no plotting
***
  show file options
    compressed
     open
       $show
    frequency to flush
     1
    exit
***
*  -----------------------------------------------------------------------
** OBPDE:Godunov order of accuracy 1
*  -----------------------------------------------------------------------
*
  pde parameters
      mu
      0.
      kThermal
      0.
  done
  boundary conditions
   all=slipWall
   box(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119)
   box(1,0)=superSonicOutflow
  done
  cfl
   $cfl
*
  debug
    $debug 
*
  reduce interpolation width
  2
*
* turn on user defined output
*
*
  turn on adaptive grids
  order of AMR interpolation
      2
  error threshold
     $errTol
  regrid frequency
     $regrid=2*$ratio; 
     $regrid
  change error estimator parameters
    set scale factors     
      1 1 1 1 1
    done
    weight for first difference
      1. 
    weight for second difference
      1. 
    exit
*    truncation error coefficient
*      1.
**    show amr error function
  change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $levels
    number of buffer zones
      $buffer
    grid efficiency
      $efficiency
    turn on load balancer
    change load balancer
      * KernighanLin
      * sequential assignment
      * random assignment
    exit
  exit
*
  initial conditions 
    user defined
      bubble with shock
      * back-ground values:
      * r=1.0, u=0.0, v=0.0, T=1.0
      r=1. e=1.786
      * radius, x0, y0:
       $radius $x0 $y0 $z0
      * values inside the bubble:
      r=0.137980769, u=0.0, v=0.0, w=0., T=7.24738676
        $xShock 
      * values to the left of the shock:
*      r=1.376363972628, u=0.3947286019216, v=0.0, w=0., T=1.140541333
      r=2.6667, u=1.25, v=0.0, w=0., e=10.119
    exit
  continue
 continue
* 
movie mode
finish



  initial conditions
      OBIC:user defined...
        bubbles
        1
        r=1.,u=0.,v=0.,T=1.
        $radb $x0 $y0 $z0
        r=.5 ,u=0.,v=0.,T=2.
        exit
    continue
   continue


  contour
    plot:T
    wire frame (toggle)
    exit



