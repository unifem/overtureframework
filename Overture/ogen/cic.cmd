#
# circle in a channel
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
#  centre
#    0. 1.
  boundary conditions
    -1 -1 1 0
exit
#
exit
generate an overlapping grid
    square
    Annulus
  done
# 
  change parameters
 # choose implicit or explicit interpolation
 # interpolation type
 #   implicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
#   display intermediate results
  compute overlap
#  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
cic.hdf
cic
exit

