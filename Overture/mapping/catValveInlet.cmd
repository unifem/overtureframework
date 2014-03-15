*
* make a grid for the upper part of the port
*
open a data-base
  valveInlet.hdf
open an old file read-only
get from the data-base
  valveInlet
*
* make a surface grid around the rounded top of the valve stem exit
*
  hyperbolic
    start from which curve/surface?
      valveInlet
    choose the initial curve
    create a curve from the surface
    specify active sub-surfaces
      2
      done
   choose an edge
     specify edge curves
       2 3
       done
     done
   exit
   grow grid in opposite direction
   edit initial curve
     lines
       61 51  31
   exit
   distance to march
     10. 9. 6. 8. 7
   lines to march
     14 13  9   15 11 8
    * assign a BC at the start of the marching direction to avoid
    * projecting the ghost line.
    periodicity
      2 0
    boundary conditions
      -1 -1  1 0
   generate
    mappingName
      frontValveInletTop
    exit
*
* volume grid
*
  hyperbolic
    start from which curve/surface?
      frontValveInletTop
    grow grid in opposite direction
    lines to march
      7 5
    distance to march
      2.5 1.75
    uniform dissipation coefficient
      .3
    generate
    boundary conditions for marching
      bottom (side=0,axis=1)
      float y, fix x and z
      exit
    * pause
    boundary conditions
      -1 -1 1 0 1 0
    share 
       0  0 3 0 1 0
    * pause
    mappingName
      frontValveInletTopVolume
pause
    exit
*
* make a surface grid near the valve stem exit
*
  hyperbolic
    start from which curve/surface?
      valveInlet
    choose the initial curve
    create a curve from the surface
   x+r 90
   bigger
   bigger
   bigger
    specify active sub-surfaces
      6
      done
   choose an edge
     specify edge curves
       2 3
       done
     done
   exit
   grow grid in opposite direction
   distance to march
   * troubles with distance=7 : points stick out just a bit
     6. 7
   lines to march
     7 8
    * assign a BC at the start of the marching direction to avoid
    * projecting the ghost line.
    boundary conditions
      -1 -1  1 0
   generate
   * pause
    mappingName
      valveStemExitSurface
    exit
*
* volume grid
*
  hyperbolic
    start from which curve/surface?
      valveStemExitSurface
    grow grid in opposite direction
    lines to march
      5
    distance to march
      3.  2.5
    boundary conditions for marching
      bottom (side=0,axis=1)
      float y, fix x and z
      exit
    generate
    boundary conditions
      -1 -1 1 0 1 0
    share 
       0  0 6 0 3 0
    * pause
    mappingName
      valveStemExitVolume
    exit
*
  open a data-base
   frontValveExit.hdf
     open a new file
   put to the data-base
     frontValveInletTopVolume
  put to the data-base 
     valveStemExitVolume
  put to the data-base 
     valveExitCylinderVolume
   close the data-base
exit
