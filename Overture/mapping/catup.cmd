*
* make a grid for the upper part of the port
*
open a data-base
  upperPort.hdf
open an old file read-only
get from the data-base
  upperPort
*
* make a surface grid starting from a line on the front
*
  hyperbolic
    start from which curve/surface?
      upperPort
    choose the initial curve
    create a curve from the surface
    project a line
    choose end points
       82 30 150 82 65 150  82 30 150 82 60 150    82 25 150 82 55 150
    number of points
      41
    exit
   exit
   * grow grid in opposite direction
   distance to march
     42 36 31 21 42
   lines to march
     43 37 32  22 43
   equidistribution weight
     .5
   uniform dissipation coefficient
     .5
    boundary conditions for marching
      right  (side=1,axis=0)
      fix y, float x and z
      exit
    boundary conditions for marching
      left   (side=0,axis=0)
      fix y, float x and z
      exit
   grow grid in both directions
   generate
   pause
    mappingName
      frontPort
    lines
     21 43
    exit
*
* volume grid
*
  hyperbolic
    start from which curve/surface?
      frontPort
    lines to march
      6
    distance to march
      3.
    * grow grid in opposite direction
    generate
    pause
    mappingName
      frontPortVolume
    exit
