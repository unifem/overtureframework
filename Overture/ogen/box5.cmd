*
* make a box
*
create mappings
  Box
    lines
      6 6 6
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
box5.hdf
box
exit




