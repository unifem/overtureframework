*
* box in a box
*
create mappings
  Box
    specify corners
      -1. -1. -1. 1. 1. 1.
    lines
      11 11 11
    * periodicity
    *  0 0 1
    mappingName
      outer-box
  exit
  Box
    specify corners
      -.5 -.5 -.5 .5 .5 .5
    lines
      7 7 7 
    mappingName
      inner-box
    boundary conditions
      0 0 0 0 0 0
  exit
exit
*
generate an overlapping grid
  outer-box
  inner-box
  done
  change parameters
    interpolation type
      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
*  pause
  compute overlap
exit
save an overlapping grid
bibe.hdf
bib
exit

