* make a rectangle
create mappings
  rectangle
    mappingName
      rectangle
    specify corners
      0. 0. 1. 5.
    lines
      11 51 
    boundary conditions
      1 1 1 1
  exit
exit
*
generate an overlapping grid
  rectangle
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
*
save an overlapping grid
  channel2.hdf
  channel2
exit
