*
*  Make a channel with an airfoil along the bottom
*
create mappings
  stretch coordinates
    transform which mapping?
      Airfoil
    stretch
      specify stretching along axis=1
        layers
          1
        1. 5. 0.
    exit
  exit
  lines
    51 15
  boundary conditions
    2 3 1 1
  mappingName
  airfoil
  exit
exit
make an overlapping grid
  airfoil
  Done
  Specify new MappedGrid Parameters
    numberOfGhostPoints
      2 2 2 2
    Repeat
  Done
Done
save an overlapping grid
arcfoil.hdf
arcfoil
exit

