*
* Same resoluition as sib2.order4 but in a bigger box
*
create mappings
* first make a sphere
Sphere
outer radius
  1. 
exit
*
* now make a mapping for the north pole
*
reparameterize
  orthographic
    specify sa,sb
      2.25 2.25
  exit
  lines
    61 61 21    * 45 45 15   15 15 5
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
      2.25 2.25
  exit
  lines
    61 61 21     * 45 45 15  15 15 5
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
    -4 4 -4 4 -4 4   -2 2 -2 2 -2 2 
  lines
    161 161 161 81 81 81   *  61 61 61  21 21 21
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
    interpolation type
      explicit for all grids
    ghost points
      all
      3 2 2 2 2 2
    order of accuracy
      fourth order
  exit
  compute overlap
exit
save an overlapping grid
sib2a.order4.hdf
sib
exit
