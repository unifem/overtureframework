* Test of the multicomponent solver
*  cice2.hdf
$tFinal=.01; $tPlot=.01; 
*
$grid="square20.hdf"; $levels=2; $show="multi2.show";
* 
*
$grid
*
  compressible Navier Stokes (multi-component)
**  one step
*
    define real parameter gamma1   1.67
    define real parameter cv1      3.11
    define real parameter pi1      0.0
    define real parameter gamma2   1.4
    define real parameter cv2      0.72
    define real parameter pi2      0.0
    define integer parameter slope 1
    define integer parameter fix   1
    define integer parameter useDon 1
    define real parameter gammai   1.67
    define real parameter gammar   1.4
  exit
  turn off twilight
*
  final time $tFinal
  times to plot $tPlot
  cfl
    .8
  debug 
    0 31
* no plotting
  plot and always wait
  show file options
    compressed
*    open
*      $show
    frequency to flush
      1
    exit
  reduce interpolation width
    2
  boundary conditions
    all=slipWall 
  done
*
  initial conditions 
    user defined
      bubble with shock
      r=1.0, u=0.0, v=0.0, T=1.0, lambda=0.0
*      .3 -1. 0.
      .1 0.35 0.5
      r=0.137980769, u=0.0, v=0.0, T=7.24738676, lambda=1.0
      0.2
      r=1.376363972628, u=0.3947286019216,v=0.0 ,T=1.140541333, lambda=0.0
    exit
*    step function
*      x= -1.2
*      T=1.2, u=0.5, v=0.1, r=1.1, s=0.0
*      T=.5, u=0.2, v=0.1, r=.2, s=0.0
  exit
*
  pde options
    OBPDE:mu 0.
    OBPDE:kThermal 0.
    OBPDE:Rg (gas constant) 1.
    OBPDE:heat release 0.
    OBPDE:artificial viscosity 0.3
  close pde options
*
  boundary conditions
    all=slipWall
    all=superSonicOutflow
  done
***
*
  turn on adaptive grids
  order of AMR interpolation
      2
  error threshold
     .0001 .0005
  regrid frequency
      8
  change error estimator parameters
    set scale factors     
      1 10000 10000 10000 1
    done
    weight for first difference
    0.
    weight for second difference
    .03
    exit
    truncation error coefficient
    1.
    show amr error function
  change adaptive grid parameters
    refinement ratio
      4
    default number of refinement levels
      $levels
    number of buffer zones
      2
    grid efficiency
      .7
  exit
continue
*
movie mode
finish
