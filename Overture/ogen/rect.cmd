* make a rectangle
create mappings
  rectangle
    mappingName
      rectangle
    set corners
      0. 2.0 0. 1.
    lines
      41 6 
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
  rect.hdf
  rect
exit

