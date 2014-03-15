* Make a composite surface with a corner to test
* surface grid generator
  plane or rhombus
    mappingName
      plane1
    lines
      11 11
    exit
  plane or rhombus
    specify plane or rhombus by three points
     1 0. 0   1 1. 0   1.0  0. -.25    * 90 degree down turn
    lines
      11 16
    mappingName
      plane2
    exit
  plane or rhombus
    specify plane or rhombus by three points
     1 0. -.25   1 1. -.25   1.05  0. -.25    * 90 degree down turn
    lines
      11 16
    mappingName
      plane3
    exit
  plane or rhombus
    specify plane or rhombus by three points
     1.05 0. -.25   1.05 1. -.25   1.05  0. 0.
    lines
      11 16
    mappingName
      plane4
    exit
  plane or rhombus
    specify plane or rhombus by three points
     1.05 0. 0     1.05 1. 0.   2.05  0. 0.
    lines
      11 16
    mappingName
      plane5
    exit
  composite surface
    add a mapping
    plane1
    add a mapping
    plane2
    add a mapping
    plane3
    add a mapping
    plane4
    add a mapping
    plane5
    plot normals (toggle)
    flip normals (toggle)
    mappingName
      cornerPlane
    exit
  hyperbolic
    mappingName
      cornerPlane
    choose the initial curve
    create a curve from the surface
    coordinate line
      0
      axis1=.5
    distance to march
      1.5
    lines to march
      12
    reset
    x-r 90
    y-r 45
    x+r 45
    * plot reference surface (toggle)
    generate


