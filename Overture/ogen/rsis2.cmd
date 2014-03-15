*
* rotated square in a square
*
create mappings
  rectangle
    specify corners
      -1. -1. 1. 1.
    lines
      49 49  51 51
    mappingName
      outer-square
    exit
*
  rectangle
    specify corners
      -.35 -.35 .35 .35 
    lines
      25 25  26 26 
    boundary conditions
      0 0 0 0
    mappingName
      inner-square
  exit
  rotate/scale/shift
    transform
      inner-square
    rotate
      45
      0. 0. 0.
    mappingName
    inner-rotated-square
  exit
exit
*
generate an overlapping grid
  outer-square
  inner-rotated-square
  done
  change parameters
    ghost points
      all
       2 2 2 2 
*       3 3 3 3 
*    discretization width
*     all
*     5 5 
  exit
  compute overlap
exit
save an overlapping grid
rsis2.hdf
rsis
exit

