*
* make a grid for the upper part of the port
*
open a data-base
  upperPort.hdf
open an old file read-only
get from the data-base
  upperPort
*
* make a surface grid starting from a line on the top
*
  hyperbolic
    start from which curve/surface?
      upperPort
    choose the initial curve
    create a curve from the surface
    project a line
    * ratio 5:3
    choose end points
       60.8 68 139.  53 72 135     61. 68 139.4  53 72 135      61.25 68 139.25 53 72 135 
    number of points
      15
    exit
   exit
   * grow grid in opposite direction
   distance to march
     20.
   lines to march
     11
   equidistribution weight
     .5
   uniform dissipation coefficient
     .5
   grow grid in both directions
    boundary conditions for marching
      left   (side=0,axis=0)
      outward splay
        .5
      exit
   generate
    mappingName
      topFrontPort
pause
    exit
*
* volume grid
*
  hyperbolic
    start from which curve/surface?
      topFrontPort
    lines to march
      6
    distance to march
      5. 4. 3.
    * grow grid in opposite direction
    generate
    * pause
    mappingName
      topFrontPortVolume
    boundary conditions
      0 0 0 0 1 0
    share
      0 0 0 0 1 0
pause
    exit
*
* make a surface grid starting from a line on the front
*  This is the BIG grid
*
  hyperbolic
    start from which curve/surface?
      upperPort
    choose the initial curve
    create a curve from the surface
    project a line
    choose end points
       82 30 150 82 64 150   82 30 150 82 65 150          82 30 150 82 60 150    82 25 150 82 55 150
    number of points
      39  31 41
    exit
   exit
   * grow grid in opposite direction
   distance to march
     46. 44  
   lines to march
     41  31 45  
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
   *   pause
    mappingName
      frontPort
   * lines
   *  21 45
    exit
*
* volume grid
*
  hyperbolic
    start from which curve/surface?
      frontPort
    lines to march
      7 6
    distance to march
      5. 3.
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
*
  hyperbolic
    start from which curve/surface?
      upperPort
    choose the initial curve
    create a curve from the surface
    specify active sub-surfaces
      3 2
      done
   choose an edge
     specify edge curves
       4 14
       done
     done
   exit
   grow grid in opposite direction
   edit initial curve
     lines
       61 41
   exit
   distance to march
     16. 15  20   34
   lines to march
     16  12   15   27
   generate
    boundary conditions
      -1 -1 4 0 1 0
    share
      0 0 4 0 1 0
    mappingName
      frontPortUpper
    exit
*
* volume grid
*
  hyperbolic
    start from which curve/surface?
      frontPortUpper
    lines to march
      15 11  4
    distance to march
      3.5 3.  1.
    grow grid in opposite direction
    boundary conditions for marching
      bottom (side=0,axis=1)
      fix y, float x and z
      exit
    generate
    boundary conditions
      -1 -1 4 0 1 0
    share
       0  0 4 0 1 0
    lines
      61 16 7
    mappingName
      frontPortUpperVolume
    exit
*
* make a surface starting from the bottom of the upper port
*
* make a surface grid starting from a spline in the mid port area -- this
* will be the end of the local grid around the port
*
  hyperbolic
    start from which curve/surface?
      upperPort
    choose the initial curve
    create a curve from the surface
    project a spline
      * dx=58-41=17  dz=120-150=-30  
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
      * There is a sharp corner on part of the surface and this causes problems.
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
      11
    distance to march
      5.5
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
    boundary conditions
      -1 -1 2 0 1 0
    share
      0 0 2 0 1 0
    * pause
    lines
      61 9 6 
    mappingName
      frontPortEndVolume
    exit
*
  open a data-base
   frontPortUpper.hdf
     open a new file
   put to the data-base
     frontPortVolume
   put to the data-base
     frontPortUpperVolume
   put to the data-base
     frontPortEndVolume
   put to the data-base
     topFrontPortVolume
   close the data-base
exit
 
