*
* make a box 
*
create mappings
  Box
    lines
      129 129 129  65 65 65 
    mappingName
      box
  exit
exit
*
generate an overlapping grid
  box
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
save a grid (compressed)
box128.hdf
box128
exit
