*
* create a square grid with 2 multigrid levels, periodic in 1 direction
*
create mappings
  rectangle
    lines
      17 17
    periodicity
      1 0     
  exit
exit this menu
make an overlapping grid
  Change the number of multigrid levels
  2
  square
  Done choosing Mappings
  Specify new MappedGrid Parameters
    numberOfGhostPoints
      2 2 2 2 2 2
    Repeat
  Done
Done specifying the CompositeGrid
save an overlapping grid
  square16mgp.hdf
square
exit
