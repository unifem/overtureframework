* make a simple square, fourth order
create mappings
  rectangle
    mappingName
      square
    lines
      11 11
    boundary conditions
      1 1 1 1
  exit
exit
*
make an overlapping grid
  square
  Done
  Specify new MappedGrid Parameters
    numberOfGhostPoints
      2 2 2 2 2 2
  discretizationWidth
   5 5
  Done
Done
*
save an overlapping grid
  square11f.hdf
  square11f
exit
