*
* make a box with MG levels
*
create mappings
  Box
    lines
      65 65 65 
    mappingName
      box
  exit
exit
*
generate an overlapping grid
  specify number of multigrid levels
    5
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
box64mg.hdf
box64mg
exit
