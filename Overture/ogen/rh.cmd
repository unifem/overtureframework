*
* make a box for Jerry
*
create mappings
  Box
    specify corners
    0. -1.e6 0. 2.e6 1.e6 1.e3 
    lines
      41 41 41
    boundary conditions
     1 1 2 2 3 3
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
rh.hdf
rh
exit

