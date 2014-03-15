*
* Make a hyperbolic surface grid over
*  part of a sphere
*
Sphere
  surface or volume (toggle)
  mappingName
    sphere
exit
* Make a circle that we can use as a
* starting curve for the surface grid
Circle or ellipse (3D)
  specify radius of the circle
    .5
  lines
    11 
exit
* rotate the circle so the surface 
* grid will cross the polar 
* singularities on the sphere
rotate/scale/shift
  transform which mapping?
    circle
  rotate
    45. 1
    0. 0. 0.
  mappingName
    rotated-circle
exit
* now make the surface grid
hyperbolic surface
  choose the reference surface
    sphere
  choose the initial curve
    rotated-circle
  number of lines in marching direction
    6  
  far field distance (ETAMX)
  .5
  generate the hyperbolic surface grid
  smooth
    arclength weight
     0.
    area weight
     1.
    curvature weight
     0.
