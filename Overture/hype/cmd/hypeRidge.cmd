  spline (3D)
    enter spline points
      5
      -.1 0. 0.
      -.1 1. 0.
       .0 1.1 0.
       .1 1. 0.
       .1 0. 0.
    shape preserving (toggle)
    lines
      31
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
      5
      -.1 0.  2.
      -.1 1.  2.
       .0 1.1 2.
       .1 1.  2.
       .1 0.  2.
    shape preserving (toggle)
    exit
  reparameterize
    equidistribution
    curvature weight
      1.
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
    grow grid in opposite direction
    distance to march
      1.
    plot reference surface (toggle)
    implicit coefficient
      0
    y-r
    y-r
    x+r
    generate
    smooth
      normal curvature weight
        5.

      boundary conditions
       bottom
        slip orthogonal
       top
        slip orthogonal
      exit


      line attraction
       1
       0 7. 5. .5
      boundary conditions
       bottom
        slip orthogonal
       top
        slip orthogonal
      exit

    equidistribution weight
      .25

    generate


    pause
    curvature weight
      1.
    generate

