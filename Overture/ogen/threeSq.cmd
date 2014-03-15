*
* three overlapping squares
*
create mappings
  rectangle 
    set corners
      0. .5 .5 1.
    lines
      11 11 9 9 
    boundary conditions
      1 0 0 1
    mappingName
      leftSquare
    exit
*
  rectangle
    set corners
      .5 1. .5 1.
    lines
      7 7 9 9 
    boundary conditions
      0 1 0 1
    mappingName
      rightSquare
    exit
*
  rectangle
    set corners
      0. 1. 0. .5
    lines
      17 9
    boundary conditions
      1 1 1 0
    mappingName
      bottom
    exit
  exit
*
generate an overlapping grid
  leftSquare
  rightSquare
  bottom
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
  exit
*
save an overlapping grid
threeSq.hdf
threeSq
exit
