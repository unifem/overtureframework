*
* square in a square
*
create mappings
  rectangle
    set corners
      -1. 1. -1. 1.
    lines
      13 13
    mappingName
      outer-square
    exit
*
  rectangle
    set corners
      -.5 .5 -.5 .5 
    lines
      9 9   
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
* save an overlapping grid
save a grid (compressed)
sise.hdf
sise
exit

