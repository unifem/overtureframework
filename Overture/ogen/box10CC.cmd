*
* make a box
*
create mappings
  Box
    lines
      11 11 11
    mappingName
      box
  exit
exit
*
generate an overlapping grid
  box
  done
  change parameters
    * make the grid cell-centered
    cell centering
      cell centered for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
  pause
exit
save an overlapping grid
box10CC.hdf
box
exit




