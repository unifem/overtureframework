*
* make a 1D overlapping grid
*
create mappings
  line
    specify end points
      0. .75
    lines
      11
  boundary conditions
    1 0
  exit
  line
    specify end points
    .25 1.
  boundary conditions
    0 1
  mappingName
    right-side
  exit
exit
make an overlapping grid
    line
    right-side
  Done
  Specify new MappedGrid Parameters
    numberOfGhostPoints
      2 2 2 2 2 2
      Repeat
  Done
Done
save an overlapping grid
line.hdf
line
exit
