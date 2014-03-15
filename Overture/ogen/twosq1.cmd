*
* two squares side by side
*
create mappings
  rectangle 
    specify corners
      0. 0. .65 1.
    lines
      11 11 
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
      11 11
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
    * do not interpolate ghost
  exit
  compute overlap
exit
save an overlapping grid
twosq1.hdf
twosq1
exit
