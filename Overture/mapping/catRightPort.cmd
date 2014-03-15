*
* make a grid for the left port
*
open a data-base
  rightPort.hdf
open an old file read-only
get from the data-base
  rightPort
pause
*
* make the right port surface
*
  hyperbolic
    start from which curve/surface?
      rightPort
    choose the initial curve
    create a curve from the surface
    specify active sub-surfaces
      0 8
      done
   choose an edge
     specify edge curves
       4 14
       done
     done
   exit
   distance to march
   * maybe 26
     21. 25.
   lines to march
     35
   grow grid in opposite direction
   generate
   * pause
    mappingName
      rightPortSurface
    exit
*
* volume grid
*
  hyperbolic
    start from which curve/surface?
      rightPortSurface
    lines to march
      15
    distance to march
      5.
    boundary conditions for marching
      bottom (side=0,axis=1)
      fix y, float x and z
      exit
*   uniform dissipation coefficient
*     .1
*   volume smoothing iterations
*     30
* -- something funny about upper BC y=constant with implicit
    implicit coefficient
      .75
    generate
    * pause
    lines
      31 21 5
    boundary conditions
      -1 -1 3 0 1 0
    share
       0  0 3 0 1 0
    * pause
    mappingName
      rightPortVolume
    exit
*
  open a data-base
   rightPortGrid.hdf
     open a new file
   put to the data-base
     rightPortVolume
   close the data-base
*
exit