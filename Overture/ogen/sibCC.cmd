*
* command file to create a sphere in a box
*
*  time to make: 594s new: 3.5
*
create mappings
* first make a sphere
Sphere
  share
    0 0 0 0 1 0
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
    15 15 5
  boundary conditions
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
    15 15 5
  boundary conditions
    0 0 0 0 1 0
  mappingName
    south-pole
exit
*
* Here is the box
*
Box
  specify corners
    -2 -2 -2 2 2 2 
  lines
    21 21 21
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
    * make the grid cell-centered
    cell centering
      cell centered for all grids
*    interpolation type
*      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  * pause
  compute overlap
exit
save an overlapping grid
sibCC.hdf
sib
exit
