*
* OverBlown command file for testing the compressible multiphase code
*
$show = " ";
$cfl=1.; 
*
$gridName ="square40.hdf"; $tFinal=.1; $tPlot=.05;
*
$gridName
***
   compressible multiphase
*
    define real parameter gammaSolid   1.4
    define real parameter gammaGas     1.4
    define real parameter p0Solid      0.
    define real parameter delta        0.
    define real parameter rmuc         0.
    define real parameter htrans       0.
    define real parameter cratio       1.
    define real parameter abmin        .0001
    define real parameter abmax        .9999
  exit
  turn off twilight
  final time $tFinal
  times to plot $tPlot 
  no plotting
***
  show file options
    compressed
*    open
*       $show
    frequency to flush
     1
    exit
***
* -----------------------------------------------------------------------
* 
  pde parameters
      mu
      0.
      kThermal
      0.
  done
  boundary conditions
    all=slipWall
    # square(0,0)=superSonicInflow uniform(rs=1.,ts=1.,rg=.2,tg=1.5,as=.8)
    # square(1,0)=superSonicOutflow
  done
  cfl
   $cfl
*
OBPDE:Godunov order of accuracy 1
*
  debug
    1 0 7 0 63 
    initial conditions
      OBIC:step function...
      OBIC:state behind rs=1. ts=1. rg=.2 tg=1.5 as=.8
      OBIC:state ahead  rs=1. ts=1. rg=1. tg=1.  as=.3
      OBIC:step: a*x+b*y+c*z=d 1, .8, 0, .75, (a,b,c,d)
      OBIC:step sharpness 10 (-1=step)
      OBIC:assign step function
      close step function
      exit
    continue
#   initial conditions
#     step function
#       x=.5
# *
#       rs=1. ts=1. rg=.2 tg=1.5 as=.8
#       rs=1. ts=1. rg=1. tg=1.  as=.3
# *
# *      rs=1. us=0. ts=1.  rg=0.2 ug=0.0 tg=1.5 as=0.5
# *      rs=.125 us=0. ts=.8  rg=1.0 ug=0.0 tg=1.0 as=0.5
#    continue
#   continue
*
movie mode
finish
