  spline (3D)
    enter spline points
      5
      -.1 0. 0.
      -.1 1. 0.
       .0 1.1 0.
       .1 1. 0.
       .1 0. 0.
    shape preserving (toggle)
    exit
  reparameterize
    equidistribution
    curvature weight
      1.
    mappingName
      spline0
    exit
*
  spline (3D)
    enter spline points
      2
      -1. 0.  2.
      1.  0.  2.
    mappingName
      spline1
    exit
  CrossSection
    general
      2
    spline0
    spline1
    mappingName
     ridge
    exit
  hyperbolic
    choose the initial curve
    spline0
    distance to march
      1.
    grow grid in opposite direction
    plot reference surface (toggle)
    implicit coefficient
      0
    y-r
    y-r
    x+r
    pause
    equidistribution weight
      .25
    curvature weight
      .1
    generate


    pause
    curvature weight
      1.
    generate

