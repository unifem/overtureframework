*
* circle in a channel
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
  lines
    21 21 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  lines
    21 5 
  boundary conditions
    -1 -1 1 0
exit
*
exit
make an overlapping grid
  square
  Annulus
  Done
  Specify new CompositeGrid parameters
    interpolationIsImplicit
    Explicit
*    Implicit
    Repeat
  Done
  Specify new MappedGrid Parameters
    numberOfGhostPoints
      2 2 2 2 2 2
    Repeat
  Done
  pause
Done
save an overlapping grid
fail.hdf
cic
exit
