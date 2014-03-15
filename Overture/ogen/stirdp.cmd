*
* make a stirring stick
*
create mappings
  rectangle
    specify corners
      -.5 -.5 .5 .5
    lines
      35 35
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
    .1
  periodicity
    2
  lines
*    61 7
    69 8
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
*
   DataPointMapping
     build from a mapping
       stir
     mappingName
       stir-dp
  exit
exit
*
* now make an overlapping grid
*
generate an overlapping grid
  backGround
  stir-dp
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
save an overlapping grid
stirdp.hdf
stir
exit
