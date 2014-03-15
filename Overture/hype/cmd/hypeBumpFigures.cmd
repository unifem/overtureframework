* Generate a hyperbolic grid starting from a spline
spline
enter spline points
7
0 0
1 0
2 1
3 2
4 1
5 0
6 0
mappingName
spline
exit
hyperbolic
  mappingName
    bump
  distance to march
   1.
  lines to march
    11
  grow grid in both directions
* set default parameters
  implicit coefficient
    1.
  uniform dissipation coefficient
   0.25
  volume smoothing iterations
    20  
  curvature speed coefficient
    0.
  upwind dissipation coefficient
    0.
  equidistribution weight
    0.  
  generate
  change plot parameters
    plot number labels
  exit
  save postscript
    hypeBump1.ps
  pause
*
  uniform dissipation coefficient
   0.05
  generate
  save postscript
    hypeBump2.ps
  pause
*
  uniform dissipation coefficient
   0.5
  generate
  save postscript
    hypeBump3.ps
  pause
*
  uniform dissipation coefficient
   0.25
  implicit coefficient
    .5
  generate
  save postscript
    hypeBump4.ps
  pause
*
  implicit coefficient
    .0
  generate
  save postscript
    hypeBump5.ps
  pause
*
  implicit coefficient
    1.
  volume smoothing iterations
    40  
  generate
  save postscript
    hypeBump6.ps
  pause
*
  volume smoothing iterations
    5
  generate
  save postscript
    hypeBump7.ps
  pause
