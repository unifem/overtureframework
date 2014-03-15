*
*  Make a channel with an sinusoidal airfoil along the bottom
*
create mappings
  change a mapping
  Airfoil
    airfoil type
      1
  exit
*
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
    101 29 
  boundary conditions
    2 3 1 1
  mappingName
    airfoil
  exit
exit
*
make an overlapping grid
  1
  airfoil
  Specify new MappedGrid Parameters
    numberOfGhostPoints
      2 2 2 2
    Repeat
  Done
Done
save an overlapping grid
sinfoil2.hdf
sinfoil
exit
