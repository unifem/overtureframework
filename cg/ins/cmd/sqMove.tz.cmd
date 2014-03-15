************************************************************
*     Moving square with twilight-zone flow
*
*    degreex=1 degreet=1 translate : [exact]
************************************************************
*
*
$tFinal=.05; $tPlot=.01; $debug=3; 
$degreeSpace=1; $degreeTime=1; 
$translationVelocity=1.; 
$grid="square5.hdf";
* $grid="square20.hdf";
* 
$grid
*
  incompressible Navier Stokes
  exit
*
* forward Euler
*  adams order 2
*  adams PC order 4
adams PC
* midpoint
* 
  final time $tFinal
  times to plot $tPlot
  turn on polynomial
  plot and always wait
  * no plotting
* 
  * turn on trig
*   On a translating grid: x(r,t) = a*r + b*t   --> Poly(x)*Poly(t) --> Poly(a*r+b*t)*Poly(t)
*     so TZ function is a higher degree in time. degreeSpace=1, degreeTime=1 should be exact
  degree in space $degreeSpace
  degree in time $degreeTime
***********
  turn on moving grids
  specify grids to move
    translate
      1. 0. 0.
      $translationVelocity
#  specify grids to move 
#      oscillate
#      1. 0. 0.
#      1.
#      .25
#      0.
*
    square
    done
  done
***************
*
  pde parameters
    nu  .1
  done
  boundary conditions
    all=noSlipWall
   done
*
   debug $debug
*  
continue
*
continue
continue
continue
finish

