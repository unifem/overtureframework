*
* Make a circle in a square
*
create mappings
*  make a rectangle as a background grid
  rectangle
    specify corners
      -1. -1. 1. 1.
    lines
      25 25
    mappingName
      outer-square
    exit
* make an annulus
  Annulus
    inner radius
      .4
    outer radius
      .8
    lines
      * 21 7
      41 8
    mappingName
      annulus
    boundary conditions
      -1 -1 1 0 
    exit
  stretch coordinates
    stretch
      specify stretching along axis=1
      layers
      1
      1. 2. 0.
      exit
    exit
    mappingName
    stretched-annulus
  exit
exit
*
make an overlapping grid
  outer-square
  stretched-annulus
  Done
  Specify new MappedGrid Parameters
    numberOfGhostPoints
      2 2 2 2 2 2
    Repeat
  Done
  * Plot
  * pause
  Done
Done
save an overlapping grid
  cis.hdf
  cis
exit

