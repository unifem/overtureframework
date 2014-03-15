# make a simple square
create mappings
  rectangle
    set corners
    0. 1. 0. .2
    mappingName
      square
    lines
      751 3
    boundary conditions
      1 1 1 1
  exit
exit
#
generate an overlapping grid
  square
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
#
save an overlapping grid
  multifluidChannelGrid.hdf
  channel
exit

