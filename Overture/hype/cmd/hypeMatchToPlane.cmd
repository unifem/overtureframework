* test the 'match to plane' boundary condition
* for the hyperbolic grid generator.
  plane or rhombus
    exit
  hyperbolic
    boundary conditions for marching
      left   (side=0,axis=0)
      match to a plane
        0 0 0 0 1 0  -.3 0 1.
      exit
    lines to march
      7
    distance to march
      .6
    generate
