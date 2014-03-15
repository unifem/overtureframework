*
* square in a square
*
create mappings
  rectangle
    specify corners
      -1. -1. 1. 1.
    lines
      81 81 
      * 21 21
    mappingName
      outer-square
    exit
*
  rectangle
    specify corners
      -.50001 -.50001 .50001 .50001
    lines
      41 41 
      * 11 11
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
*    discretization width
*       all
*       5 5 
*      interpolation width
*       all
*       all
*       5 5 
  exit
  compute overlap
exit
save an overlapping grid
sis4.hdf
sis4
exit

