*
* cgcns command file for testing the compressible multiphase code
*
* some definitions
$show = " ";
$tFinal=.1; $tPlot=.05; $debug=1; $x0=.1; 
$cfl=.5;
* 
$gridName ="qcic.hdf";
*
* Start OB commands
*
$gridName
*
   compressible multiphase
*
* EOS parameters
    define real parameter gammaSolid   5.0
    define real parameter gammaGas     1.35
    define real parameter p0Solid      0.160206804822
    define real parameter bGas         0.377783425214
*
* source parameters
    define real parameter delta        15.
    define real parameter rmuc         40.
    define real parameter htrans       0.01
    define real parameter cratio       1.6
    define real parameter abmin        1.e-4
    define real parameter abmax        1.
*
* compaction potential reference parameters
    define real parameter asRef        0.73
    define real parameter rsRef        5.02933658067
    define real parameter psRef        3.56808028557e-4
    define real parameter pgRef        1.18428340552e-5
*
* heat release
    define real parameter heat         0.117946639276
*
* chemical reaction parameters
    define real parameter sigma        4.
    define real parameter pgi          1.4e-5
    define real parameter anu          1.
*
* source integrator parameters
    define real parameter tol          1.e-5
*
* solid contact jump conditions parameters (set lcont.ne.0 to accept linearized contact solution)
    define real parameter rtol         1.e-4
    define integer parameter lcont     1
*
* Numerical integration of expansion solution (set nrmax=1 for trapezoidal rule, nrmax>1 for Romberg)
    define real parameter atol         1.e-4
    define integer parameter nrmax     10
*
* middle state calculation parameters
*    define real parameter toli         1.e-3
*    define real parameter tolv         1.e-5
*    define integer parameter itmax     10
*
  exit
  turn off twilight
  final time $tFinal
  times to plot $tPlot
  cfl
   $cfl
*  dtMax
*   .02
  no plotting
*
  show file options
    compressed
      open
        $show
    frequency to flush
     1
    exit
*
* -----------------------------------------------------------------------
*
  OBPDE:exact Riemann solver
  OBPDE:Godunov order of accuracy 2
  OBPDE:artificial viscosity 1.
  OBPDE:artificial diffusion 0. 0. 0. 0. 0. 0. 0. 0. 0.
*
  pde parameters
      mu
      0.
      kThermal
      0.
  done
*
  boundary conditions
*    all=superSonicOutflow
    all=slipWall
* side, direction
*    channel(1,0)=superSonicOutflow
    done
*
  debug $debug
*
    initial conditions
      OBIC:step function...
      OBIC:state behind rs=5.02933658067, us=0., vs=0., ts=7.094534693270948e-04, rg=2.64701925298e-3, ug=0., vg=0., tg=4.474026413622568e-02, as=.93
      OBIC:state ahead rs=5.02933658067, us=0., vs=0., ts=7.094534693270948e-05, rg=2.64701925298e-3, ug=0., vg=0., tg=4.474026413622568e-03, as=.73
      OBIC:step: a*x+b*y+c*z=d 1, -.8, 0, $x0, (a,b,c,d)
      OBIC:step sharpness 10 (-1=step)
      OBIC:assign step function
      close step function
      exit
*     continue
*
  reduce interpolation width
  2
*
  maximum number of iterations for implicit interpolation
  10
*
*  turn on adaptive grids
  order of AMR interpolation
      2
  error threshold
      1.e-3
  regrid frequency
      8
  change error estimator parameters
    set scale factors
      1.e8 1.e8 1.e8 1.e8 1. 1.e8 1.e8 1.e8 1.
*      1. 1. 1. 1. 1. 1. 1. 1. 1.
*      1900. 1900. 1.e8 1.e8 1900. 1900. 1.e8 1.e8 1.
    done
    weight for first difference
    0.
    weight for second difference
    1.
    exit
    truncation error coefficient
    0.0
    show amr error function
  change adaptive grid parameters
    refinement ratio
      4
    default number of refinement levels
      2
    number of buffer zones
      2
    grid efficiency
      .7
  exit
  continue
* 
movie mode
finish
