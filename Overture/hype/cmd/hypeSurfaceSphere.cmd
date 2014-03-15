*
* Make a hyperbolic surface grid over
*  part of a sphere
*
Sphere
  surface or volume (toggle)
  lines 
    31 31
  mappingName
    sphere
exit
* Make a circle that we can use as a
* starting curve for the surface grid
Circle or ellipse (3D)
  specify radius of the circle
    .5
  specify start/end angles
    -.05 .55
  lines
    21 
  mappingName
   curve1
exit
Circle or ellipse (3D)
  specify radius of the circle
    .5
  specify start/end angles
    .45 1.05 
  lines
    21 
  mappingName
   curve2
exit
*
* now make the surface grid
hyperbolic 

  surface grid
    choose boundary curve 0
  done
  distance to march .25
  lines to march 5
  generate


  choose the initial curve
    rotated-circle


  lines to march
    6
  distance to march
    .5
  implicit
    0.
  grow grid in opposite direction
  boundary conditions
    0 0 1 0
  generate


*  here we do an elliptic smoothing step.
  smooth
    boundary condition
      top
      slip orthogonal
    exit
