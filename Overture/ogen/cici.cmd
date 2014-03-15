*
* circle in a channel with an intersecting grid
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
  lines
    32 32 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  lines
    32 7
  boundary conditions
    -1 -1 1 0
exit
*
rectangle
  specify corners
    -.5  1.75 .5 2.75
  lines
    11 11 
  boundary conditions
    1 1 0 1
  mappingName
  intersectingSquare
exit
exit
*
make an overlapping grid
    square
    Annulus
    intersectingSquare
  Done
  Specify new CompositeGrid parameters
    mayCutHoles
      Yes
      No
      Repeat
  Done specifying CompositeGrid parameters
Done specifying the CompositeGrid
save an overlapping grid
cici.hdf
cici
exit

