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
      * -.4 .4 -.4 .4   * for matching sisa.hdf
      -.5 .5 -.5 .5  -.50001 .50001 -.50001 .50001 
    lines
      * 5 5     * for matching sisa.hdf
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
      implicit for all grids
      * explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
save an overlapping grid
sis.hdf
* sisa.hdf
sis
exit

