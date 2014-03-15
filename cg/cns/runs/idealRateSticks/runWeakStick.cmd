$N=320;
*
$grid="stickGrid" . $N . ".hdf";
*
$tFinal=20.0;
$tPlot=5.0;
$cfl=0.8;
*
$showFile="weakStick". $N . ".show";
*
$amr="turn on adaptive grids";
*$amr="turn off adaptive grids";
$amrRatio=4;
$amrLevels=3;
*
$artificialViscosity=0.2;
*
$grid
*
  compressible Navier Stokes (multi-component)
  one step pressure law
*
  define real parameter gammai   2.0
  define real parameter gammar   3.0
  define real parameter cvi      1.0
  define real parameter cvr      1.0
*
  define integer parameter slope 1
  define integer parameter fix   1
  define integer parameter useDon 1
  exit
*
  turn off twilight 
*
  final time (tf=)
    $tFinal
  times to plot (tp=)
    $tPlot
  cfl 
    $cfl
*
* dtMax
* .0025
*
  OBPDE:Roe Riemann solver
* OBPDE:HLL Riemann solver
  OBPDE:Godunov order of accuracy 2
  OBPDE:interpolate primitive and pressure
* plot and always wait
  no plotting
* debug
* 31
*
  show file options
    compressed
     open
     $showFile
    frequency to flush
    1
    exit
*
  pde options
  OBPDE:mu 0.
  OBPDE:kThermal 0.
*  OBPDE:Rg (gas constant) 1.
*  OBPDE:gamma 2.0
  OBPDE:heat release 0.04
  OBPDE:1/(activation Energy) 1.0
  OBPDE:rate constant 10.0
  OBPDE:artificial viscosity $artificialViscosity
  close pde options
*************************
  initial conditions
    user defined
      rate stick (reaction)
      2.0
    exit
  done
*************************
*
* turn on axisymmetric flow
*
  boundary conditions
    all=superSonicOutflow
*    all=slipWall
    square(0,0)=slipWall
    done
*
  reduce interpolation width
  2
*
* turn on user defined output
*
  $amr
  order of AMR interpolation
      2
  error threshold
      .0005
  regrid frequency
      8
  change error estimator parameters
    set scale factors
      1 1 1 1 1 1 
    done
    weight for first difference
    0.
    weight for second difference
    1.    .03
    exit
    truncation error coefficient
    1.
    show amr error function
  change adaptive grid parameters
    refinement ratio
      $amrRatio
    default number of refinement levels
      $amrLevels
    number of buffer zones
      2
    grid efficiency
      .7
  exit
continue
movie mode
finish

