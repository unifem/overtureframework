  rectangle
    specify corners
      -.5 -.5 .5 .5
    lines
*     35 35
     15 15  35 35 21 21  25 25
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
     .0   .25
     .05  .25
     .05 -.25
    -.05 -.25
    -.05  .25
     .0   .25
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
*   61 7
   31 5
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
  reparameterize
    transform which mapping?
     stir
    restrict parameter space
      set corners
        0. .5 0. 1.  .5 1. 0. 1.  .25 .75  0. 1.
      exit
    mappingName
      rightStir
    check inverse
     