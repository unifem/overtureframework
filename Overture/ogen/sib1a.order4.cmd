*
* command file to create a sphere in a box
*
create mappings
* first make a sphere
Sphere
exit
*
* now make a mapping for the north pole
*
reparameterize
  orthographic
    specify sa,sb
      2.5 2.5
  exit
  lines
    41 41 13  21 21 7  31 31 11  61 61 21 45 45 15   15 15 5
  boundary conditions
    0 0 0 0 1 0
  share
    0 0 0 0 1 0
  mappingName
    north-pole
exit
*
* now make a mapping for the south pole
*
reparameterize
  orthographic
    choose north or south pole
      -1
    specify sa,sb
      2.5 2.5
  exit
  lines
    41 41 13   21 21 7  31 31 11 61 61 21 45 45 15  15 15 5
  boundary conditions
    0 0 0 0 1 0
  share
    0 0 0 0 1 0
  mappingName
    south-pole
exit
*
* Here is the box
*
Box
  set corners
    -2 2 -2 2 -2 2 
  lines
    61 61 61    31 31 31   41 41 41   81 81 81 61 61 61  21 21 21
  mappingName
    box
  exit
exit
*
generate an overlapping grid
  box
  north-pole
  south-pole
  done
  change parameters
*    interpolation type
*      explicit for all grids
    ghost points
      all
      2 2 2 2 2 3
    order of accuracy
      fourth order
  exit
  compute overlap
exit
save an overlapping grid
sib1a.order4.hdf
sib
exit
