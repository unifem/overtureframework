*
* two squares side by side ** multigrid **
*
create mappings
  rectangle 
    specify corners
      0. 0. .65 1.
    lines
      13 9
    boundary conditions
      1 0 1 1
    mappingName
      leftSquare
    exit
*
  rectangle
    specify corners
      .35 0. 1. 1.
    lines
      13 9
    boundary conditions
      0 1 1 1
    mappingName
      rightSquare
    exit
  exit
*
make an overlapping grid
  Change the number of multigrid levels
    2
  leftSquare
  rightSquare
  Done
  Specify new MappedGrid Parameters
    numberOfGhostPoints
      2 2 2 2 2 2
    repeat
  Done
Done
save an overlapping grid
twosq.mg.hdf
twosq
exit
