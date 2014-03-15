*
*  make two hyperbolic grids where the ports hit the cylinder
*  with the nasty corners
*
  open a data-base
  lowerPortCylinder.hdf
  open an old file read-only
  get from the data-base
  lowerPortCylinder
*
* surface grid for the left port
*
  hyperbolic
    start from which curve/surface?
      lowerPortCylinder
    grow grid in opposite direction
    choose the initial curve
    create a curve from the surface
    specify active sub-surfaces
      1 2 15 16
      done
    choose an edge
    specify edge curves
      7 5 9 1 2 15
      done
    done
    exit
    edit reference surface
      * hide 0-16 except 1 2 15 16
      hide sub-surfaces
        0 3 4 5 6 7 8 9 10 11 12 13 14 
        done
      exit
    *  pause
    edit initial curve
      lines
      21     
      * right=[.2,.35]  left=[.475,.625]
      restrict the domain
        .465 .615 
      exit
    distance to march
      11
    lines to march
       21
    * grow grid in opposite direction
    * assign a BC at the start of the marching direction to avoid
    * projecting the ghost line.
    boundary conditions
      0 0 1 0
    generate
    * plot reference surface (toggle)
    *  pause
    lines
      21 7 
    mappingName
     leftPortEdgeSurface
    exit
*
* surface grid for the right port
*
  hyperbolic
    start from which curve/surface?
      lowerPortCylinder
    grow grid in opposite direction
    choose the initial curve
    create a curve from the surface
    specify active sub-surfaces
      1 2 15 16  
      done
    choose an edge
    specify edge curves
      7 5 9 1 2 15
      done
    done
    exit
    edit initial curve
      lines
      35 31  21     
      * right=[.2,.35]  left=[.475,.625]
      restrict the domain
        .2 .37  .2 .35 
      exit
    distance to march
      11.
    lines to march
       21
    * grow grid in opposite direction
    boundary conditions
      0 0 1 0
    generate
    * plot reference surface (toggle)
    lines
      35  11  31 7 
    * pause
    mappingName
     rightPortEdgeSurface
    exit
  change a mapping
*
* Now hide surfaces on the CompositeSurface to make it easier
* to grow the volume grid
*
  lowerPortCylinder
    unhide all sub-surfaces
    hide sub-surfaces
      1 2 15 16 
      done
    exit
*
* volume grid for the left port
*
  hyperbolic
    start from which curve/surface?
      leftPortEdgeSurface
    boundary conditions for marching
      bottom (side=0,axis=1)
      match to a mapping
      lowerPortCylinder
      exit
    grow grid in opposite direction
*    we can't go too far or else we off the edge as
*    we march up the face of the port
    distance to march
      4.8 5. 5.35 5.25 5. 4.75   5.   5.5 5
    lines to march
      11 11 8 6
    uniform dissipation coefficient
     .1
    geometric stretching, specified ratio
       1.1
    generate
    * pause
    mappingName
      leftPortEdgeVolume
    boundary conditions
      0 0 1 0 5 0
    share  
      0 0 1 0 5 0
    * pause
    exit
*
* volume grid for the right port
*
  hyperbolic
    start from which curve/surface?
      rightPortEdgeSurface
    boundary conditions for marching
      bottom (side=0,axis=1)
      match to a mapping
      lowerPortCylinder
      exit
    grow grid in opposite direction
*    we can't go too far or else we go off the edge as
*    we march up the face of the port
    distance to march
      5.5   6.0  6.2 6.35 6.25 6. 5.75   5.45  5.5       6. 5
    lines to march
      17 15 13 11 8 7 6
    uniform dissipation coefficient
     .03
    geometric stretching, specified ratio
      1.2
    generate
    pause
    mappingName
      rightPortEdgeVolume
    boundary conditions
      0 0 1 0 5 0
    share  
      0 0 1 0 5 0
    * pause
    exit
*
* build DPM so we don't have to read in the CS as the BC mapping.
  DataPointMapping
    build from a mapping
    leftPortEdgeVolume
    mappingName
      leftPortEdge
    exit
*
  DataPointMapping
    build from a mapping
    rightPortEdgeVolume
    mappingName
      rightPortEdge
    exit
*
  open a data-base
    catPortCylinderEdge.hdf
  open a new file
  put to the data-base
  leftPortEdge
  put to the data-base
  rightPortEdge
  close the data-base
  exit this menu

