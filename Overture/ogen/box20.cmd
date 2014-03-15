*
* make a box
*
create mappings
  Box
    lines
      21 21 21 
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
* save an overlapping grid
save a grid (compressed)
box20.hdf
box
exit




