*
* Create a intake port
*
create mappings
*
  Annulus
    mappingName
      annulus
    inner
      .5
    exit
*
  body of revolution
    revolve which mapping?
      annulus
    mappingName
      port
    choose a point on the line to revolve about
      2 0 0
    start/end angle
      0 90
    lines
       31 9 21  41 9 31  31 9 21 
    boundary conditions
      -1 -1 0 3 1 2
    share
       0  0 0 3 1 2
    exit
*
  rectangle
    specify corners
      -.6 -.6 .6 .6
    mappingName
      coreEnd
    exit
*
  body of revolution
    mappingName
      core
    choose a point on the line to revolve about
      2 0 0
    start/end angle
      0 90
    boundary conditions
      0 0 0 0 1 2
    share
       0  0 0 0 1 2
    lines
      11 11 15  15 15 21
    exit
exit
*
  generate an overlapping grid
      core
      port
    done choosing mappings
    change parameters
      ghost points
        all
       2 2 2 2 2 2
    exit
    * pause
    compute overlap
    * pause
  exit
*
save an overlapping grid
port.hdf
port
exit




*
* Here is a port on a box (for the two-stroke-engine)
*
create mappings
*
  Box
  specify
  -1.-1. -1.    1. 1. 1.
  lines
  11 11 11 
  boundary
  1 2 3 1 2 3 
  mappingName
  box
  exit
  *
  SmoothedPolygon
    vertices
    4
    -1. -2.
    -1.2 -2.
    -1.2 -1.
    -1. -1.
    n-dist
    variable normal distance
    .35 .3 5.
    .3 .25 5.
    .25 .25 5.
    sharpness
    20.
    20.
    20.
    20.
    n-stretch
    1. 1. 0.
    t-stretch
    1. 0
    1. 8.
    1. 4.
    1. 0.
    lines
    31 9
    mappingName
    2d-port
    pause
    exit
  *
  * make a 3d port
  *
  body of revolution
  revolve which mapping?
  2d-port
  start/end angle
  -10. 10.
  x+r
  y+r
  y+r
  boundary conditions
  1 1 2 2 3 3
  lines
  31 7 7
  plot
  colour boundaries by boundary condition number 
  exit
  tangent of line to revolve about
  0 1 0
  choose a point on the line to revolve about
  0 0 0
  choose a point on the line to revolve about
  0 0 0
  mappingName
  3d-port
  pause
  exit
  *
  * Stretch coordinates
  stretch coordinates
  transform which mapping?
  3d-port
  stretch
  specify stretching along axis=0
  layers
  1
  1. 2. 1.
  exit
  exit
  exit
  *
  * shift to the right spot
  *
  rotate/scale/shift
  transform which mapping?
  3d-port
  shift
  .025 -.05 0.
  mappingName
  port-1
  exit
