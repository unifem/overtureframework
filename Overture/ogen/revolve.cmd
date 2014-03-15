*
* Create a cylindrical body of revolution 
* from a Smoothed Polygon
*     cpu=48s (ov15 sun-ultra optimized)
create mappings
  SmoothedPolygon
    vertices
    7
    -1. 0.
    -1. .25
    -.8 .5
    0. .5
    .8 .5
    1. .25
    1. 0.
    n-dist
    fixed normal distance
    .1
    n-dist
    fixed normal distance
    .4
    corners
    specify positions of corners
    -1. 0.
    1. 0
    -1.4 0.
    1.4 0
    t-stretch
    0 5
    .15 10
    .15 10
    0 10
    .15 10
    .15 10
    0 10
  exit
* making a body of revolution
*  pause
  body of revolution
    tangent of line to revolve about
    1. 0 0
    mappingName
      cylinder
    lines
      55 25 7
    boundary conditions
      0 0 -1 -1 1 0 
    share
      0 0  0  0 1 0
  exit
* patch on the front singularity
  reparameterize
    mappingName
      front
    lines
      15 15 5
    orthographic
      specify sa,sb
        .5 .5
    exit
    boundary conditions
      0 0 0 0 1 0
    share
      0 0 0 0 1 0
  exit
*   patch on back singularity
  reparameterize
    mappingName
      back
    lines
      15 15 7
    orthographic
      choose north or south pole
        -1
      specify sa,sb
        .5 .5
    exit
    boundary conditions
      0 0 0 0 1 0
    share
      0 0 0 0 1 0
  exit
*
* Here is the box
*
  Box
    specify corners
      -2 -1 -1 2 1 1 
    lines
      61 31 31
    mappingName
      box
    exit
  * pause
exit
generate an overlapping grid
  box
  cylinder
  front
  back
  done
  compute overlap
exit
*
save an overlapping grid
revolve.hdf
revolve
exit
