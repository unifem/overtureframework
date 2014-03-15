  spline
    periodicity
      2
    enter spline points
      9
      1 0
      .4 .4
      0 1
      -.4 .4
      -1. 0
      -.4 -.4
      0 -1
      .4 -.4
      1 0
    shape preserving (toggle)
    lines
      51
    mappingName
      star

    exit
    reparameterize
      equidistribution


  hyperbolic
    distance to march
      .5
    generate
    grow grid in opposite direction
    generate
    distance to march
      .1
    generate
    set view -0.68 0.08 0 3.24074 1 0 0 0 1 0 0 0 1
    uniform dissipation coefficient
      .1
    generate
    reset
    set view 0.00251886 -0.578947 0 5.51389 1 0 0 0 1 0 0 0 1
    reset
    set view -0.622166 0.0287081 0 5.67143 1 0 0 0 1 0 0 0 1
    reset
    exit
  exit this menu
