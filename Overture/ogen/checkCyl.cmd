create mappings
  Cylinder
    bounds on the radial variable
      .4  1.
    lines
      21 11 7
    mappingName
      cylinder
    boundary conditions
      -1 -1 2 2 0 1
    share
      0 0 1 2 0 0
  exit
  Box
    specify corners
      -.6 -.6 -1. .6 .6 1.
    mappingName
      core
    boundary conditions
      0 0 0 0 1 2
    share
      0 0 0 0 1 2
    lines
      13 13
    exit
  exit this menu
check overlap
  cylinder
  core
  done
