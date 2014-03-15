*
* **** this grid is made to fourth order ***
*
*
create mappings
* first make a sphere
Sphere
outer radius
  .75
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
    121 121 21  *  61 61 21     * 145 145 25   97 97 17  97 97 33  49 49 17  25 25 9  23 23 7   12 12 4
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
    121 121 21  * 145 145 25  97 97 17  97 97 33 49 49 17 25 25 9  23 23 7  12 12 4 
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
    161 161 161 *  81 81 81  * 193 193 193  129 129 129  65 65 65 33 33 33  17 17 17 
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
      2 2 2 2 2 2
    order of accuracy
      fourth order
  exit
  compute overlap
exit
* save an overlapping grid
save a grid (compressed)
sib4.order4.hdf
sib4
exit
