*
* Square Beside a Square
*
create mappings
  rectangle
    mappingName
      left-square
    lines
      7 5
    boundary conditions
      1 0 1 1
    share
      0 0 1 2
    exit
  rectangle
    mappingName
      right-square
    lines
      7 5 
    set corners
      .9 1.9 0. 1.   1. 2. 0. 1. 
    boundary conditions
      0 1 1 1
    share
      0 0 1 2
    exit
  exit this menu
  generate an overlapping grid
    left-square
    right-square
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
* pause
exit
save an overlapping grid
sbs.hdf
sbs
exit
