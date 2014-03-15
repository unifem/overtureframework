*
* make a box 
*
create mappings
  Box
    lines
      33 33 33
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
save an overlapping grid
box32.hdf
box32
exit
