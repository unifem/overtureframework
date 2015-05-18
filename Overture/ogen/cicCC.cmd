#
# circle in a channel, cell centered grid
#
create mappings
#
rectangle
  set corners
    -2. 2. -2. 2.
  lines
    32 32 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
#
Annulus
  lines
    33 7
  boundary conditions
    -1 -1 1 0
exit
#
exit
generate an overlapping grid
    square
    Annulus
  done
  change parameters
 # make the grid cell-centered
    cell centering
      cell centered for all grids
    exit
  compute overlap
  exit
#
save an overlapping grid
cicCC.hdf
cicCC
exit

