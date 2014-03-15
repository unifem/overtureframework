*
*  make a grid for the left lower port of the 
* 
  Annulus
    inner radius
      60.
    outer radius
      80
    lines
      41 5
    make 3d (toggle)
      0.
    exit
  rotate/scale/shift
    rotate
      90 0
      0 0
    shift
      78 0 105
   mappingName
     rotatedAnnulus
  exit
*
open a data-base
  lowerPortCylinder.hdf
open an old file read-only
get from the data-base
  lowerPortCylinder
*
  change a mapping
  lowerPortCylinder
    add a mapping
    rotatedAnnulus
    delete sub-surfaces
      1 
      2  
      15
      16
      -1
    determine topology
    exit
* make the strip that joins the two ports
  hyperbolic
    start from which curve/surface?
      lowerPortCylinder
    choose the initial curve
    create a curve from the surface
    project a line
    choose end points
      39 0 129 72 0 108
    exit
    exit
    grow grid in both directions (toggle)
    distance to march
      6
      6
    lines to march
      6
      6
    generate
    mappingName
      joinStripSurface
    exit
* make the volume grid for the strip
  hyperbolic
    distance to march
      4
    lines to march
      7
    generate
    mappingName
      joinStrip
    boundary conditions
      0 0 0 0 1 0
    share  
      0 0 0 0 1 0
    exit
* 
* surface grid for left port
*
  hyperbolic
    start from which curve/surface?
      lowerPortCylinder
    choose the initial curve
    create a curve from the surface
    turn off all sub-surfaces
    pick active sub-surfaces
    specify active sub-surfaces
      2 3
      done
    choose an edge
    specify edge curves
      2 16
    done
    done
    exit
    grow grid in opposite direction
    edit initial curve
      lines
        61  71  81  101
      exit
    distance to march
      11.  10.
    lines to march
      8   9
    generate
    exit
*
* volume grid for left port
*
  hyperbolic
    mappingName
      leftPort
    grow grid in opposite direction
    distance to march
      9.
    lines to march
      9  11
    boundary conditions for marching
      bottom (side=0,axis=1)
      fix y, float x and z
      exit
    generate
    boundary conditions
      -1 -1 2 0 1 0 
    share  
       0  0 2 0 1 0
    exit
*
* Now create the right port
*
  hyperbolic
    start from which curve/surface?
    lowerPortCylinder
    grow grid in opposite direction
    choose the initial curve
    create a curve from the surface
    turn off all sub-surfaces
    specify active sub-surfaces
      0 1
      done
    choose an edge
    specify an edge curve
      2 8
      done
    done
    exit
    edit initial curve
      lines
        61  71  81  101
      exit
    distance to march
      11.  10.
    lines to march
      8   9
    generate
    mappingName
      rightPortSurface
    exit
*
* volume grid for right port
*
  hyperbolic
    mappingName
      rightPort
    grow grid in opposite direction
    distance to march
      9.
    lines to march
      9  11
    boundary conditions for marching
      bottom (side=0,axis=1)
      fix y, float x and z
      exit
    generate
    boundary conditions
      -1 -1 3 0 1 0 
    share  
       0  0 3 0 1 0
    exit
*
  open a data-base
   catGrids.hdf
     open a new file
   put to the data-base
     rightPort
   put to the data-base
     leftPort
   put to the data-base
     joinStrip
   close the data-base
 
