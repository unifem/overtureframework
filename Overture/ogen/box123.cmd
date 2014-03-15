*
* make a box with unequal side
*
create mappings
  Box
    specify corners
      0. 0. 0. 1. 1.5 2.0
    lines
      11 11 11
    mappingName
      box
  exit
exit
*
make an overlapping grid
  box
  Done
  Specify new MappedGrid Parameters
    numberOfGhostPoints
      2 2 2 2 2 2
  Done
Done
save an overlapping grid
box123.hdf
box
exit
