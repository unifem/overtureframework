*
* make a box with MG levels
*
create mappings
  Box
    lines
      17 17 17
    mappingName
      box
  exit
exit
*
generate an overlapping grid
  specify number of multigrid levels
    3
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
box16mg.hdf
box16mg
exit
