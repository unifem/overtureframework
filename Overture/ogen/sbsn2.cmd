*
* Square Beside a Square
*
create mappings
  rectangle
    mappingName
      left-square1
    lines
      21 21 
    boundary conditions
      1 0 1 1
    share
      0 0 1 2
    exit
  rotate/scale/shift
    mappingName
    left-square
    exit
  rectangle
    mappingName
      right-square1
    lines
      21 21
    set corners
      .9 1.9 0. 1.   1. 2. 0. 1. 
    boundary conditions
      0 1 1 1
    share
      0 0 1 2
    exit
  rotate/scale/shift
    mappingName
    right-square
    exit
  exit this menu
  generate an overlapping grid
    left-square
    right-square
    done
  change parameters
    interpolation type
      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
save an overlapping grid
sbsn2.hdf
sbs
exit
