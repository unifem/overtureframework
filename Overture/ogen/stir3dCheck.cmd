*
* make a stirring stick
*
create mappings
  box
    set corners
      -.5 .5 -.5 .5 0. 1.
    lines
      51 51 3
    boundary conditions
      1 1 1 1 1 1
    share
      0 0 0 0 1 2
    mappingName
      backGround
  exit
*
SmoothedPolygon
* start on a side so that the polygon is symmetric
  vertices 
    6
    -.05  .00
    -.05  .25
     .05  .25
     .05 -.25
    -.05 -.25
    -.05  .00
  n-stretch
*    1. 2.0 0.
   1. 4.0 0.
  n-dist
    fixed normal distance
*   .125
    .05 .1
  periodicity
    2
  lines
*    61 7
    15 7  21 7 69 8
  t-stretch
    0. 1.
    1. 9.
    1. 9.
    1. 9.
    1. 9.
    0. 1.
  boundary conditions
    -1 -1 1 0
  mappingName
    stir
  exit
  * pause
  rotate/scale/shift
    transform which mapping
      stir
    rotate
      45
       0. 0. 0.
    mappingName
      stirRotated
    exit
*
  line (3D)
    set end points
      0 0 0 0 0 1
    exit
*
  sweep
    choose sweep surface/curve
    stirRotated
    line
    lines
      21 11 3
    y-r
    boundary conditions
      -1 -1 1 0 1 1
    share
      0 0 0 0 1 2
    mappingName
      stir3d
    exit
exit
*
* now make an overlapping grid
*
generate an overlapping grid
  backGround
  stir3d
  done
  display intermediate
  compute overlap
  continue


