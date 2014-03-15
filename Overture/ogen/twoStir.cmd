*
* make a DOUBLE stirring stick
*
create mappings
  rectangle
    specify corners
     -.75 -.5 .75 .5
    lines
      51 35
*     31 31
*     41 41 
    boundary conditions
      1 1 1 1
    mappingName
      backGround
  exit
*
SmoothedPolygon
* start on a side so that the polygon is symmetric
  mappingName
    left-stir
  vertices 
    6
    -.35  .00
    -.35  .25
    -.25  .25
    -.25 -.25
    -.35 -.25
    -.35  .00
  n-stretch
    1. 2.0 0.
*   1. 5.0 0.
  n-dist
    fixed normal distance
    .125
  periodicity
    2
  lines
    61 7
*   61 9
  t-stretch
    0. 1.
    1. 7.
    1. 7.
    1. 7.
    1. 7.
    0. 1.
  boundary conditions
    -1 -1 1 0
  exit
SmoothedPolygon
* start on a side so that the polygon is symmetric
  mappingName
    right-stir
  vertices 
    6
     .30  .05 
     .55  .05 
     .55 -.05 
     .05 -.05
     .05  .05
     .30  .05
  n-stretch
    1. 2.0 0.
*   1. 5.0 0.
  n-dist
    fixed normal distance
    .125
  periodicity
    2
  lines
    61 7
*   61 9
  t-stretch
    0. 1.
    1. 7.
    1. 7.
    1. 7.
    1. 7.
    0. 1.
  boundary conditions
    -1 -1 1 0
  exit
exit
*
* now make an overlapping grid
*
generate an overlapping grid
  backGround
  left-stir
  right-stir
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
save an overlapping grid
twoStir.hdf
twoStir
exit
