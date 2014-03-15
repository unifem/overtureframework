*
*  make a hyperbolic surface grid for a valve
* 
open a data-base
  lowerPortCylinder.hdf
open an old file read-only
get from the data-base
  lowerPortCylinder
hyperbolic
    grow grid in opposite direction
    choose the initial curve
    create a curve from the surface
    specify active sub-surfaces
      1  2 15 16
      done
    choose an edge
    specify edge curves
      7 5 9 1 2 15
      done
    done
    exit
    edit initial curve
      lines
        65
      exit
    distance to march
      7
    lines to march
      7
    grow grid in opposite direction
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
