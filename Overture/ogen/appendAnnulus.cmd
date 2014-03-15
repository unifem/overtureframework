*
* add mappings to an existing overlapping grid
*
create mappings
*
  annulus
  centre
    1. 1. 
  boundary conditions
    -1 -1 1 0
  mappingName
    annulus2
  exit
*
  rectangle
    boundary conditions
      0 0 0 0
    set corners
      -1.5 -.5 -1.5 -.5 
    mappingName
     refine
    exit
*
  exit this menu
  generate an overlapping grid
    read in an old grid
      cic
    annulus2
    refine

