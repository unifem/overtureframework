# 
# 
# Test of the multifluid solver -- shock hitting a bubble
#  
#  cgcns multifluid
# 
$tFinal=.3; $tPlot=.05; $show=" "; $radius=.2;  $x0=.5;  $y0=.5; $xShock=.2; $debug=0; 
$ratio=4; $buffer=2; $errTol=.05;  $efficiency=.7;
#
# $grid="square5.hdf"; $levels=2; $show=" "; $debug=63; 
# $grid="square16.hdf"; $levels=3; $show=" "; $tPlot=.01; 
$grid="square32.order2.hdf"; $levels=2; $show="mf.show"; $tPlot=.02; 
# $grid="square32.hdf"; $levels=2; $show=" "; $tPlot=.0001; $ratio=2; $buffer=1; $errTol=.01;  
# $grid="square64.hdf"; $levels=2; $show=" "; $tFinal=.5; $tPlot=.01; $ratio=2; $errTol=.01; $show="multiBubble.show";
# $grid="square256.hdf"; $levels=2; $show=""; $tFinal=.5; $tPlot=.1; $ratio=2; $errTol=.01; 
# $grid="square1024.hdf"; $levels=2; $show="bubble.show"; $tFinal=.5; $tPlot=.1; $ratio=2; $errTol=.01; 
# 
#
$grid
#
  compressible Navier Stokes (multi-fluid)
  stiffened gas equation of state
  ignition-pressure reaction rate
# 
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
#
  turn off twilight
#
  final time $tFinal
  times to plot $tPlot
  cfl .8
 debug
   $debug
# no plotting
  plot and always wait
  show file options
    compressed
    open
      $show
    frequency to flush
      1
    exit
  reduce interpolation width
    2
  boundary conditions
    all=slipWall 
  done
#
  initial conditions 
     # -- shock hitting a bubble --
#1     user defined
#1       bubble with shock
#1       r=1.0, u=0.0, v=0.0, T=1.0, lambda=0.0
#1       * radius, x0, y0:
#1 *      .3 -1. 0.
#1 *      .1 0.35 0.5
#1        $radius $x0 $y0 
#1       r=0.137980769, u=0.0, v=0.0, T=7.24738676, lambda=1.0
#1         $xShock 
#1       r=1.376363972628, u=0.3947286019216,v=0.0 ,T=1.140541333, lambda=0.0
#1     exit
#   -- contact : ?
    OBIC:step function...
     $gammaLeft=1.67; $mu1Left=1./($gammaLeft-1.); $gammaRight=1.4; $mu1Right=1./($gammaRight-1.); 
     $rhoLeft=2.; $rhoRight=1.; $TLeft=1./$rhoLeft; $TRight=1./$rhoRight;
      OBIC:state behind r=$rhoLeft, u=0, v=0, T=$TLeft,  mu1=$mu1Left, mu2=0
      OBIC:state ahead r=$rhoRight, u=0, v=0, T=$TRight, mu1=$mu1Right, mu2=0
      OBIC:step: a*x+b*y+c*z=d 1, 0, 0, .5 (a,b,c,d)
      OBIC:assign step function
  exit
#
  pde options
    OBPDE:mu 0.
    OBPDE:kThermal 0.
    OBPDE:Rg (gas constant) 1.
    OBPDE:heat release 0.
    OBPDE:artificial viscosity 0.3
  close pde options
#
  boundary conditions
    all=superSonicOutflow
  done
#**
#
#  turn on adaptive grids
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
#    truncation error coefficient
#      1.
    show amr error function
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


    contour
      wire frame (toggle)
      plot:p
      exit
# 

movie mode
finish








#  cice2.hdf
$tFinal=1.; $tPlot=.1; 
#
# $grid="/home/banks20/OverBlown.v20d/Grids/square40.hdf"; $levels=2; $show="multi.show";
# $grid="square16.hdf"; $levels=2; $show=" "; $tPlot=.05;
$grid="square32.hdf"; $levels=2; $show=" "; $tPlot=.05;
# $grid="square64.hdf"; $levels=2; $show=" "; $tPlot=.05;
# $grid="square128.hdf"; $levels=2; $show=" "; $tPlot=.025;
# $grid="square256.hdf"; $levels=2; $show="multi2.show";
# 
#
$grid
#
  compressible Navier Stokes (multi-component)
#*  one step
#
    define real parameter gamma1   1.67
    define real parameter cv1      3.11
    define real parameter pi1      0.0
    define real parameter gamma2   1.4
    define real parameter cv2      0.72
    define real parameter pi2      0.0
    define integer parameter slope 1
    define integer parameter fix   1
  exit
  turn off twilight
#
  final time (tf=)
    $tFinal
  times to plot (tp=)
    $tPlot
  cfl
    .8
  debug 
    0 31
# no plotting
  plot and always wait
  show file options
    compressed
    open
      $show
    frequency to flush
      1
    exit
  reduce interpolation width
    2
  boundary conditions
    all=slipWall 
  done
#
  initial conditions 
    user defined
      bubble with shock
      r=1.0, u=0.0, v=0.0, T=1.0, lambda=0.0
#      .3 -1. 0.
#       .1 0.35 0.5
      .2 0.5 0.5
      r=0.137980769, u=0.0, v=0.0, T=7.24738676, lambda=1.0
      0.2
      r=1.376363972628, u=0.3947286019216,v=0.0 ,T=1.140541333, lambda=0.0
    exit
#    OBIC:step function
#      x= -1.2
#      T=1.2, u=0.5, v=0.1, r=1.1, s=0.0
#      T=.5, u=0.2, v=0.1, r=.2, s=0.0
  exit
#
  pde options
    OBPDE:mu 0.
    OBPDE:kThermal 0.
    OBPDE:Rg (gas constant) 1.
    OBPDE:heat release 0.
    OBPDE:artificial viscosity 0.3
  close pde options
#
  boundary conditions
    all=slipWall
    all=superSonicOutflow
  done
#**
#
#*  turn on adaptive grids
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
#

movie mode
finish
