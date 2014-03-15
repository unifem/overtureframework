*********************************************************************
*   Computation of a perturbed detonation
*    which forms a cellular pattern
*
* To run without graphics use:
*      cg noplot detCell
* Sample parallel commands:
*   mpirun -np 2 $cgcnsp noplot -readCollective -writeCollective detCell
*   mpirun -np 2 $cgcnsp noplot -writeCollective detCell
*  
* Notes:
*  (1) You will need to generate the grid detChannel.hdf using ogen 
*        and the command file Overture/sampleGrids/detChannel.cmd
*  (2) The cells are well formed (but not regular) by t=.25
*
* 
*********************************************************************
$tFinal=.5; $tPlot=.05; $cfl=.8; $debug=1; 
* If the $show variable is blank then no show file is saved:
* $show=" ";
$dataFile = "detCellOneStepProfile.data";
*
* $grid="detChannel.hdf"; $refinementLevels=2; $refinementRatio=4; $tPlot=.01; $show="detCelll2r4.show";
$grid="detChannel.hdf"; $refinementLevels=2; $refinementRatio=4; $show="detCelll2r4.show";
* $grid="detChannel.hdf"; $refinementLevels=2; $refinementRatio=4; 
* $grid="detChannel.hdf"; $refinementLevels=2; $refinementRatio=4; 
* Here is a finer grid run:
* $grid="detChannel.hdf"; $refinementLevels=3; $refinementRatio=4; 
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
 cfl 
  $cfl
 debug $debug
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
   channel(0,0)=outflow
   channel(1,0)=superSonicInflow uniform(r=1.0,u=-3.2,T=9.325417e-01,s=2.033854e-02)
   channel(0,0)=outflow
  done
*
reduce interpolation width
  2
*
* turn on user defined output
*
  turn on adaptive grids
* 
  order of AMR interpolation
     2
  error threshold
      .0005
  regrid frequency
    $regridFrequency=$refinementRatio*2;
    $regridFrequency
  change error estimator parameters
    set scale factors
      1 10000 10000 10000 10000
    done
    weight for first difference
      0.
    weight for second difference
      .03
    exit
    truncation error coefficient
      1.
    *    show amr error function
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
