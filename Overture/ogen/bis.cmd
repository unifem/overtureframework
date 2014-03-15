create mappings
  Box
    specify corners
      -.6 -.6 -.6 .6 .6 .6   -1. -1. -1. 1. 1. 1.
    lines
      11 11 11 
    boundary conditions
      0 0 0 0 0 0
    exit
  Sphere
    boundary conditions
      0 0 -1 -1 0 1
    exit
  exit this menu
  generate an overlapping grid
    box
    sphere
    display intermediate results


    set debug
     7

    compute overlap
