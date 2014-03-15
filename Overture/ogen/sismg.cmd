*
* square in a square with MG levels
*
create mappings
  rectangle
    specify corners
      -1. -1. 1. 1.
    lines
      17 17
    mappingName
      outer-square
    exit
*
  rectangle
    specify corners
      -.5 -.5 .5 .5
    lines
      9 9
    boundary conditions
      0 0 0 0
    mappingName
      inner-square
    exit
  exit
generate an overlapping grid
  outer-square
  inner-square
  done
  change parameters
    * interpolation type
    *  explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
     * do not interpolate ghost
  exit
  compute overlap
exit
save an overlapping grid
sismg.hdf
sis
exit

