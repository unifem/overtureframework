*
* two squares side by side
*
create mappings
  rectangle 
    specify corners
      0. 0. .65 1.
    lines
      33  25
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
      33 25
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
twosq3.hdf
twosq3
exit
