*
* make a grid for the upper part of the port
*
open a data-base
  upperPort.hdf
open an old file read-only
get from the data-base
  upperPort
*
* make a surface grid starting from a spline in the mid port area
*
  hyperbolic
    start from which curve/surface?
      upperPort
    choose the initial curve
    create a curve from the surface
    project a spline
      * dx=58-41=17  dz=120-150=-30   dx:dz=3:-5
      enter spline points
        9
        41.  30  150
        41.  50  150
        41.  70  150
        49.5 70. 135.
        58.  70. 120.
        58.  50. 120.
        58.  30. 120.
        49.5 30. 135.
        41.  30  150
      periodicity
        2
      shape preserving
      lines
       61
      exit
      * we have to project onto a plane and then back onto the
      * composite surface a number of times to ensure that the
      * initial curve lies on the surface and nearly in the specified plane.
      change the projected spline
        project onto a plane
         41 70 150
         30 0 +17
      exit
      change the projected spline
        project onto a plane
         41 70 150
         30 0 +17
      exit
      change the projected spline
        project onto a plane
         41 70 150
         30 0 +17
      exit
      change the projected spline
        project onto a plane
         41 70 150
         30 0 +17
      exit
      change the projected spline
        project onto a plane
         41 70 150
         30 0 +17
      exit
    exit
   exit
   * grow grid in opposite direction
   distance to march
     8.
   lines to march
     9
   uniform dissipation coefficient
     .3
   generate
   * pause
    mappingName
      frontPortEnd
    exit
*
* volume grid
*
  hyperbolic
    start from which curve/surface?
      frontPortEnd
    lines to march
      5
    distance to march
      2.5
    * grow grid in opposite direction
    uniform dissipation coefficient
     .3
    boundary conditions for marching
      bottom (side=0,axis=1)
      match to a plane
    * plane : 41.  30  150    41 70 150        58 30 120 
    * normal 30 0 17
        24.  10  180    41 90 150   75 10 90 
      exit
    generate
    pause
    boundary conditions
      0 0 2 0 1 0
    share
      0 0 2 0 1 0
    mappingName
      frontPortEndVolume
    exit
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
     44  42 36 31 21 42
   lines to march
     45  43 37 32  22 43
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
   * pause
    mappingName
      frontPort
    lines
     21 45
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
    * pause
    boundary conditions
      0 0 0 0 1 0
    share
      0 0 0 0 1 0
    mappingName
      frontPortVolume
    exit
