  spline
    enter spline points
      7
      .5 0
      1. 0
      1. .01
      .5 .01
      0 .01
      0 0 
      .5 0
    shape preserving (toggle)
    periodicity
      2
    lines
      51
    exit
  reparameterize
    transform which mapping?
    splineMapping
    equidistribution
    curvature weight
      10.
    re-evaluate equidistribution
    re-evaluate equidistribution
    re-evaluate equidistribution
    re-evaluate equidistribution
    exit
  hyperbolic
    distance to march
      .125
*    curvature weight
*      10.
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
    hypeLine1.ps
  pause
*
  uniform dissipation coefficient
   0.05
  generate
  save postscript
    hypeLine2.ps
  pause
*
  uniform dissipation coefficient
   0.5
  generate
  save postscript
    hypeLine3.ps
  pause
*
  uniform dissipation coefficient
   0.25
  implicit coefficient
    .5
  generate
  save postscript
    hypeLine4.ps
  pause
*
  implicit coefficient
    .0
  generate
  save postscript
    hypeLine5.ps
  pause
*
  implicit coefficient
    1.
  volume smoothing iterations
    40  
  generate
  save postscript
    hypeLine6.ps
  pause
*
  volume smoothing iterations
    5
  generate
  save postscript
    hypeLine7.ps
  pause




