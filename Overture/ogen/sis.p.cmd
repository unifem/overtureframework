*
* square in a square
*
create mappings
  rectangle
    set corners
      -1. 1. -1. 1.
    lines
      11 11
    mappingName
      outer-square
    exit
*
  rectangle
    set corners
      -.5 .5 -.5 .5  -.50001 .50001 -.50001 .50001 
    lines
      6 6
    boundary conditions
      0 0 0 0
    mappingName
      inner-square
    exit
  exit
*
generate an overlapping grid
  outer-square
  inner-square
  done
  change parameters
    * choose implicit or explicit interpolation
    interpolation type
      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
save a grid with arrays
sis.p.hdf
sis
exit

