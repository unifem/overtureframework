*
* command file to create a box in a sphere
*
create mappings
* first make a sphere
Sphere
  inner and outer radii
   .70 1.  .8 1.   .6 1.
exit
*
* now make a mapping for the north pole
*
reparameterize
  orthographic
    specify sa,sb
      2.2 2.2   2.0 2.0  2. 2.   2.25 2.25  2.5 2.5
  exit
  lines
    61 61 11 * 41 41 7 *  21 21 7  
  boundary conditions
    0 0 0 0 0 1
  share
    0 0 0 0 0 1
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
      2.2 2.2   2.35 2.35   2.0 2.0 2. 2. 2.25 2.25 2.5 2.5
  exit
  lines
    61 61 11 * 41 41 7 *  21 21 7  
  boundary conditions
    0 0 0 0 0 1
  share
    0 0 0 0 0 1
  mappingName
    south-pole
exit
*
* Here is the box
*
Box
  set corners
    -.9 .9 -.9 .9 -.9 .9  -1. 1. -1. 1. -1. 1.-.8 .8 -.8 .8 -.8 .8 
  lines
    61 61 61 33 33 33 * 16 16 16 
  boundary conditions
    0 0 0 0 0 0
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
      2 2 2 2 2 3
    order of accuracy
      fourth order
  exit
  compute overlap
* pause
  print grid statistics
exit
save an overlapping grid
bis2e.order4.hdf
bis
exit
