*
* build a hyperbolic volume grid on surface
* defined by the intersection of a cylinder 
* and a plane.
*
  Cylinder
    orientation
      2 0 1
    surface or volume (toggle)
    mappingName
      cylinder
    bounds on the radial variable
      .5 1.
    bounds on the axial variable
      0 1.
    exit
  plane or rhombus
    specify plane or rhombus by three points
      -1. 0 -1. 1 0 -1. -1. 0 1.
    lines
      21 21
    exit
  composite surface
    add a mapping
    cylinder
    add a mapping
    plane
    determine topology
    flip normals (toggle)
    plot normals (toggle)
    exit
  hyperbolic
    choose the initial curve
    create a curve from the surface
    specify active sub-surfaces
      0
    done
    choose an edge
      specify edge curves
        3
      done
    done
    exit
    lines to march
      21
    distance to march
      1.5
    generate
     pause
    exit
  hyperbolic
    distance to march
      .1
    lines to march
      5
    generate
