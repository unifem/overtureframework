*********************************************************************
*   Computation of a perturbed detonation in a tube
*
* To run without graphics use:
*      cg noplot detTube
*  
* Sample parallel commands:
*   mpirun -np 2 $cgcnsp -readCollective -writeCollective detTube
*   srun -N2 -n8 -ppdebug $cgcnsp -noplot detTube >! detTube.mf.N2n8.out  &
*   srun -N1 -n8 -ppdebug $cgcnsp -noplot detTube >! detTube.mf.N1n8.out  &
*   srun -N2 -n16 -ppdebug $cgcnsp -noplot detTube >! detTube.N2n16.out  &
* Notes:
*  (1) You will need to generate the grid using ogen 
*        and the command file Overture/sampleGrids/tubeArg.cmd
*********************************************************************
$tFinal=.5; $tPlot=.05; $debug=1; $cfl=.8; 
* If the $show variable is blank then no show file is saved:
$show=" ";
* $show="detTube.show";
$dataFile = "detTubeOneStepProfile.data";
*
* $grid="tubee2.order2.hdf"; $refinementLevels=2; $refinementRatio=4; $show="detTube2.show"; 
* $grid="tubee2.order2.hdf"; $refinementLevels=2; $refinementRatio=2; $tPlot=.01; $show="detTube2l2r4.show"; 
* $grid="tubee2.order2.hdf"; $refinementLevels=2; $refinementRatio=4; $show="detTube2l2r4.show"; $tPlot=.05; $tFinal=.2;
$grid="tubee2.order2.hdf"; $refinementLevels=2; $refinementRatio=4; $show="detTube2l2r4.show"; $tPlot=.002; $tFinal=.002;
* $grid="tubee4.order2.hdf"; $refinementLevels=2; $refinementRatio=4; $show="detTube4l2r4.show"; 
* $grid="tubee4.order2.hdf"; $refinementLevels=2; $refinementRatio=4; $show="detTube4.show"; 
*
$grid
*
  compressible Navier Stokes (Godunov)
  one step
  exit
  turn off twilight
  recompute dt interval
    2
  turn off twilight
  final time $tFinal
  times to plot $tPlot
*
*
  plot and always wait
*
  show file options
    compressed
     open
      $show
    frequency to flush
      1
    exit
*
  cfl
    $cfl
*
  pde options
  OBPDE:mu 0.
  OBPDE:kThermal 0.
  OBPDE:gamma 1.4
*  OBPDE:heat release 3.0
  OBPDE:heat release 4.0
*  OBPDE:1/(activation Energy) 0.06   * this was the hard case
  OBPDE:1/(activation Energy) 0.075
  OBPDE:rate constant 1
  OBPDE:artificial viscosity 1.5
  close pde options
*
*
  boundary conditions
   all=slipWall
   bcNumber2=outflow
   bcNumber3=superSonicInflow uniform(r=1.0,u=-3.2,T=9.325417e-01,s=2.033854e-02)
*   channel(0,0)=outflow
*   channel(1,0)=superSonicInflow uniform(r=1.0,u=-3.2,T=9.325417e-01,s=2.033854e-02)
*    channel(0,0)=outflow
  done
*
 debug 
   $debug
reduce interpolation width
  2
*
* turn on user defined output
*
  turn on adaptive grids
  order of AMR interpolation
     2
  error threshold
      .1
  regrid frequency
    $regridFrequency=$refinementRatio*2;
    $regridFrequency
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
   **   show amr error function
  change adaptive grid parameters
    refinement ratio
      $refinementRatio
    default number of refinement levels
      $refinementLevels
    number of buffer zones
       2
    grid efficiency
      .7
    turn on load balancer
  exit
***********************************
  initial conditions
    user defined
**      1d profile from a data file 
      1d profile from a data file perturbed
      $dataFile
      * 
      * a0*sin(2*Pi*f0*y)*exp(-beta*(x-x0)^2) 
      * a0, f0, x0, beta
       .3 4. .25 30.
    exit
  exit
********************************************
continue
*
* pause
* 
movie mode
finish
