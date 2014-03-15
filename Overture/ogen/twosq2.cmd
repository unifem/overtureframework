*
* two squares side by side
*
create mappings
  rectangle 
    specify corners
      0. 0. .65 1.
    lines
      17 13 
    boundary conditions
      1 0 -1 -1
    mappingName
      leftSquare
    exit
*
  rectangle
    specify corners
      .35 0. 1. 1.
    lines
      17 13 
    boundary conditions
      0 1 -1 -1
    mappingName
      rightSquare
    exit
  exit
*
generate an overlapping grid
  leftSquare
  rightSquare
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
    interpolate ghost
  exit
  compute overlap
exit
save an overlapping grid
twosq2.hdf
twosq2
exit
