# Grid for the spherical cavity run
create mappings
  rectangle
    set corners
      0. 1. 0. .5
    mappingName
      channel
    lines
      201 101
    boundary conditions
      1 1 1 1
  exit
exit
#
generate an overlapping grid
  channel
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
  sphericalCavityGrid.hdf
  sphericalCavityGrid
exit
