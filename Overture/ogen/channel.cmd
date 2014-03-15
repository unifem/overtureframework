* make a rectangle
create mappings
  rectangle
    mappingName
      rectangle
    specify corners
      0. 0. 5. 1.
    lines
      51 11 
      * 51 5
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
  channel.hdf
  channel
exit
