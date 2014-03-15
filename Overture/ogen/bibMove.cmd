*
* box in a box
*
create mappings
  Box
    specify corners
      -1. -1. -1. 1. 1. 1.
    lines
      31 31 31
    * periodicity
    *  0 0 1
    mappingName
      outer-box
  exit
  Box
    specify corners
      -.75 -.75 -.75 .25 .25 .25
    lines
      15 15 15 
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
    ghost points
      all
      2 2 2 2 2 2
  exit
*  pause
  compute overlap
exit
save an overlapping grid
bibMove.hdf
bib
exit

