# cgcns command file: Example showing the use of ramped inflow values
# 
# Ramp the inflow values from (ra,ua,Ta) to (rb,ub,Tb) over the time interval (ta,tb)
#
# Usage:
#    cgcns rampedInflow.cmd 
#
$tFinal=1.; $tPlot=.05; $backGround="rectangle"; $show=" "; 
$show = " ";  $format="%18.12e";  $debug=0; 
$numberOfLevels=2; $regrid=4; $refinementRatio=4; $errTol=.01; $numberOfSmooths=1; 
$amrOn="turn on adaptive grids";
$amrOff="turn off adaptive grids";
$amr=$amrOff; 
#
$grid="square80.order2"; $debug=3; $backGround="square"; $show=" ";
# 
  $grid
  compressible Navier Stokes (Godunov)  
  exit
# 
  turn off twilight
  final time $tFinal
  times to plot $tPlot
  plot and always wait
# 
  show file options
    compressed
    open
      $show 
    frequency to flush
      2
    exit
#
 reduce interpolation width
   2
#
#
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
#
  output format $format
#
  boundary conditions
    $backGround=slipWall
    # Ramp the inflow values from (ra,ua,Ta) to (rb,ub,Tb) over the time interval (ta,tb)
    #  (NOTE: Use T instead of e for ramp values)
    $gamma=1.4; 
    $ra=1.; $ua=0; $ea=1.786; $Ta=($gamma-1.)*( $ea/$ra-.5*$ua*$ua );
    $rb=2.66667; $ub=1.25; $eb=10.119; $Tb=($gamma-1.)*( $eb/$rb-.5*$ub*$ub ); 
    $backGround(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119), ramp(ta=0.,tb=.5,ra=1.,rb=2.66667,ua=0.,ub=1.25,Ta=$Ta,Tb=$Tb)
    $backGround(1,0)=superSonicOutflow
    done
# 
 $amr
#
## turn on user defined error estimator
#
  order of AMR interpolation
      2
  error threshold
     $errTol
  regrid frequency
     $regrid
  change error estimator parameters
    default number of smooths
      $numberOfSmooths
    set scale factors     
      2 1 1 1 1 
    done
    exit
  change adaptive grid parameters
    refinement ratio
      $refinementRatio
    default number of refinement levels
      $numberOfLevels
    number of buffer zones
      2
  exit
#
#  allow user defined output 1
#
  initial conditions
    uniform flow
      r=$ra u=$ua v=0. T=$Ta
    continue
  debug
    $debug
   continue
# 





