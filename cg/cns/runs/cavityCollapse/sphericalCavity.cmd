# 
# 
# Test of the multifluid solver -- shock hitting a spherical cavity
#  
# 
$tFinal=.08; $tPlot=.008; $show=" "; $debug=0; 
$ratio=4; $buffer=2; $errTol=.005;  $efficiency=.5;
#
$levels=2; $show=" "; $debug=0; 
# 
#
# channel.hdf
sphericalCavityGrid.hdf
#
  compressible Navier Stokes (multi-fluid)
  stiffened gas equation of state
  exit
#
  turn off twilight
#
  final time $tFinal
  times to plot $tPlot
  cfl .8
#  dtMax 7.e-5
 debug
   $debug
  no plotting
#  plot and always wait
  show file options
    compressed
    open
      $show
    frequency to flush
      1
    exit
  reduce interpolation width
    2
#
  initial conditions 
    user defined
      circular (smooth) interface with shock M=1.2
      exit
    exit
#
  pde options
    OBPDE:Godunov order of accuracy 2
    OBPDE:artificial viscosity 1.
#    OBPDE:artificial diffusion 2. 2. 2. 2. 2. 2.
#    OBPDE:artificial diffusion .5 .5 .5 .5 0. 0.
    OBPDE:artificial diffusion 0. 0. 0. 0. 0. 0.
    OBPDE:mu 0.
    OBPDE:kThermal 0.
    OBPDE:Rg (gas constant) 1.
    OBPDE:heat release 0.
  close pde options
#
  turn on axisymmetric flow
  boundary conditions
    all=superSonicOutflow
# (side,axis)
#*    channel(0,1)=slipWall
    channel(0,1)=axisymmetric
  done
#
  adaptive grid options...
##    use user defined error estimator 1
   close adaptive grid options
#
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
      1 1 1 1 1 1
    done
    weight for first difference
      1. 0. 1. 
    weight for second difference
      1. 0. 1. 
    exit
#    truncation error coefficient
#      1.
   show amr error function 1
   change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $levels
    number of buffer zones
      $buffer
    grid efficiency
      $efficiency
  exit
continue
#

movie mode
finish

