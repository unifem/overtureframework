*
* square in a square
*
create mappings
  rectangle
    specify corners
      -1. -1. 1. 1.
    lines
      * 11 11
      21 21
    mappingName
      outer-square
    exit
*
  rectangle
    specify corners
      -.5 -.5 .5 .5
    lines
      * 6 6
      11 11
    boundary conditions
      0 0 0 0
    mappingName
      inner-square
    exit
  exit
*
make an overlapping grid
  outer-square
  inner-square
  Done
  Specify new CompositeGrid parameters
    interpolationIsImplicit
    * Explicit
    Implicit
    Repeat
  Done
  Specify new MappedGrid Parameters
    isCellCentered
      Yes
      Repeat
    numberOfGhostPoints
      2 2 2 2 2 2
      Repeat
    discretizationWidth
      5 5
      Repeat
  Done
Done
save an overlapping grid
sisCC.hdf
sis
exit

