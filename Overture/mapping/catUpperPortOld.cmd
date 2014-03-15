*
* make a grid for the upper part of the port
*
open a data-base
  valveInlet.hdf
open an old file read-only
get from the data-base
  valveInlet
*
*
* make a surface grid on the valve exit cylinder
*
  hyperbolic
    start from which curve/surface?
      valveInlet
    choose the initial curve
    create a curve from the surface
   x+r 45
   bigger
   bigger
   bigger
    specify active sub-surfaces
      1 4
      done
   choose an edge
     specify edge curves
       13 12 8 10 9
       done
     done
   exit
   edit initial curve
     lines
       51 
   exit
   grow grid in both directions
   distance to march
     13. 15
     1.
   lines to march
     14  16
     2
   generate
   pause
    mappingName
      valveExitCylinder
    exit
*
* volume grid
*
  hyperbolic
    start from which curve/surface?
      valveExitCylinder
    lines to march
      5
    distance to march
      2.5
    generate
    periodicity
     2 0
    boundary conditions
      -1 -1 0 0 1 0
    share
       0  0 0 0 1 0
    * pause
    mappingName
      valveExitCylinderVolume
    exit









*
* make a surface grid on the valve exit cylinder
*
  hyperbolic
    start from which curve/surface?
      valveInlet
    choose the initial curve
    create a curve from the surface
   x+r 45
   bigger
   bigger
   bigger
    specify active sub-surfaces
      1
      4
      -1
   choose an edge
     specify an edge curve
       13 12 8 10 9
     done
   exit
   * grow grid in opposite direction
   distance to march
     15
   lines to march
     16
   generate
   pause
    mappingName
      valveExitCylinder
    exit
*
* volume grid
*
  hyperbolic
    start from which curve/surface?
      valveExitCylinder
    lines to march
      5
    distance to march
      2.5
    generate
    boundary conditions
      -1 -1 0 0 1 0
    * pause
    mappingName
      valveExitCylinderVolume
    exit
*
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
      -1
   choose an edge
     specify an edge curve
       2 3
     done
   exit
   grow grid in opposite direction
   distance to march
     7
   lines to march
     8
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
      2.5
    boundary conditions for marching
      bottom (side=0,axis=1)
      float y, fix x and z
      exit
    generate
    boundary conditions
      -1 -1 1 0 1 0
    * pause
    mappingName
      valveStemExitVolume
    exit
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
      -1
   x+r 90
   bigger
   bigger
   bigger
   choose an edge
     specify an edge curve
       2 3
     done
   exit
   grow grid in opposite direction
   edit initial curve
     lines
       31
   exit
   distance to march
     7
   lines to march
     8
    * assign a BC at the start of the marching direction to avoid
    * projecting the ghost line.
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
      5
    distance to march
      1.75
    uniform dissipation coefficient
      .3
    boundary conditions for marching
      bottom (side=0,axis=1)
      float y, fix x and z
      exit
    generate
    * pause
    boundary conditions
      -1 -1 1 0 1 0
    mappingName
      frontValveInletTopVolume
    exit
*
  open a data-base
   frontValveExit.hdf
     open a new file
   put to the data-base
     frontValveInletTopVolume
  put to the data-base 
     valveStemExitVolume
*  put to the data-base 
*     valveExitCylinderVolume
   close the data-base
exit
