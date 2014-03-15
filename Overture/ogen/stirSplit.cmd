*
* make a stirring stick in two parts to test
* the case when a boundary cuts points that actually
* are inside the domain on another grid (i.e. a very thin
* object )
*
create mappings
  rectangle
    specify corners
      -.5 -.5 .5 .5
    lines
*     35 35
     21 21 16 16 15 15  35 35 21 21  25 25
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
     .0    .25
     .025  .25
     .025 -.25
    -.025 -.25
    -.025  .25
     .0    .25
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
   61 7
*  31 5
  t-stretch
    0. 1.
    1. 9.
    1. 9.
    1. 9.
    1. 9.
    0. 1.
  boundary conditions
    -1 -1 1 0
  share
    0 0 1 0
  mappingName
    stir
  exit
  * pause
*
  reparameterize
    transform which mapping?
      stir
    restrict parameter space
      set corners
        0. .5 0. 1.  -.25 .25 0. 1.
      exit
    mappingName
      rightStir
    exit
*
  reparameterize
    transform which mapping?
     stir
    restrict parameter space
      set corners
         .5 1. 0. 1.  .25 .75  0. 1.
      exit
    mappingName
      leftStir
* pause
    exit
exit
*
* now make an overlapping grid
*
generate an overlapping grid
  backGround
*  stir
  leftStir
  rightStir
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
  exit
*
save an overlapping grid
stirSplit.hdf
stirSplit
exit


