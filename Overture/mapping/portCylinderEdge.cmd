*
*  make a hyperbolic surface grid for a valve
* 
open a data-base
  portCylinderEdge.hdf
open an old file read-only
get from the data-base
  portCylinderEdge
hyperbolic
    grow grid in opposite direction
    choose the initial curve
    create a curve from the surface
    specify active sub-surfaces
      21
      22
      18
      25
      -1
    choose an edge
    specify an edge curve
      5
    specify an edge curve
      17
    specify an edge curve
      7
    specify an edge curve
      2
    specify an edge curve
      1
    specify an edge curve
      13
    done
    exit
    edit initial curve
      lines
        61
      exit
    edit reference surface
      hide sub-surfaces
        19
        20
        -1
      exit
    distance to march
      7
    lines to march
      7
    grow grid in opposite direction
    generate
    exit
  change a mapping
  portCylinderEdge
    unhide all sub-surfaces
    hide sub-surfaces
      18
      21
      22
      25
      -1
    exit
  hyperbolic
    grow grid in opposite direction
    distance to march
      5
    lines to march
      5
    boundary conditions for marching
      bottom (side=0,axis=1)
      match to a mapping
      portCylinderEdge
      exit
    generate









    exit
  hyperbolic
    boundary conditions for marching
      bottom (side=0,axis=1)
      match to a mapping
      lowerPortCylinder
      exit
    uniform dissipation coefficient
      .05
    distance to march
      1
    lines to march
      2
    plot boundary condition mappings (toggle)
    debug 
     3
    generate





junk
* surfaces 15 16 1 2 
* curves 7, 5, 9, 1, 2, 15
    choose an edge
    set view 0.0514286 -0.128571 0 1 0.878943 0.230315 -0.417629 0.385512 0.172437 0.906447 0.280783 -0.957716 0.0627735
    bigger
    bigger
    bigger
    bigger
    bigger
    bigger
    bigger
    specify an edge curve
      26
    specify an edge curve
      40
    done
    exit
    reparameterize initial curve
      restrict parameter space
        exit
      set bounds
        .4 .6
      exit

    edit initial curve
      lines
        101
      exit
    generate
