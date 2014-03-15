*
* make a box with MG levels
*
create mappings
  Box
    lines
      9 9 9
    mappingName
      box
  exit
exit
*
generate an overlapping grid
  specify number of multigrid levels
    2
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
box8mg.hdf
box8mg
exit
