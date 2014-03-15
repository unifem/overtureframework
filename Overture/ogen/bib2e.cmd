*
* box in a box
*
create mappings
  Box
    specify corners
      -1. -1. -1. 1. 1. 1.
    lines
      41 41 41 11 11 11
    * periodicity
    *  0 0 1
    mappingName
      outer-box
  exit
  Box
    specify corners
      -.5 -.5 -.5 .5 .5 .5
    lines
      25 25 25 7 7 7 
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
    order of accuracy
      second order    
  exit
*  pause
  compute overlap
exit
* save an overlapping grid
save a grid (compressed)
bib2e.hdf
bib
exit

