*
*   test some boundary conditions for the hyperbolic grid generator
*
  line (2D)
    specify end points
      0 0 .5 1.
    exit
  hyperbolic
    boundary conditions for marching
      right  (side=1,axis=0)
        fix y, float x and z
      left   (side=0,axis=0)
        fix y, float x and z
      exit
    distance to march
      .5
    lines to march
      11
    generate
    change plot parameters
      plot ghost lines
        1
    exit