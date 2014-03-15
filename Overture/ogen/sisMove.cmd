*
* square in a square, fine grid
*
create mappings
  rectangle
    specify corners
      -1. -1. 1. 1.
    lines
      41 41 
    mappingName
      outer-square
    exit
*
  rectangle
    specify corners
      -.75 -.75 .25 .25 
    lines
      21 21 
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
      implicit for all grids
      * explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
save an overlapping grid
sisMove.hdf
sisMove
exit

