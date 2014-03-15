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
  box
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 3
    order of accuracy
     fourth order
  exit
  compute overlap
exit
save an overlapping grid
box16.order4.hdf
box4
exit
