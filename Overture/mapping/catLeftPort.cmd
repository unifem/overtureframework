*
* make a grid for the left port
*
open a data-base
  leftPort.hdf
open an old file read-only
get from the data-base
  leftPort
*
* make the left port surface
*
  hyperbolic
    start from which curve/surface?
      leftPort
    choose the initial curve
    create a curve from the surface
    specify active sub-surfaces
      0 5 
      done
   choose an edge
     specify edge curves
       3 7 
     done
     done
   exit
    change reference surface plot parameters
      plot grid lines on boundaries (3D) toggle
      plot shaded surfaces (3D) toggle
      set surface colour
      brass
      exit
   x+r:0
   x+r:0
   bigger
   plot axes   
   pause
   distance to march
   * maybe 26
     21.  25.5 
   lines to march
     35 41
*   grow grid in opposite direction
   generate
   pause
    mappingName
      leftPortSurface
    exit
*
* volume grid
*
  hyperbolic
    start from which curve/surface?
      leftPortSurface
    lines to march
      15
    distance to march
      5.
    boundary conditions for marching
      bottom (side=0,axis=1)
      fix y, float x and z
      exit
* -- something funny about upper BC y=constant with implicit
    implicit coefficient
      .75
    generate
    lines
      31 21 5
    boundary conditions
      -1 -1 3 0 1 0
    share
       0  0 3 0 1 0
    * pause
    mappingName
      leftPortVolume
    pause
    exit
*
  open a data-base
   leftPortGrid.hdf
     open a new file
   put to the data-base
     leftPortVolume
   close the data-base
exit
 
