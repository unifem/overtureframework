*
* command file to create a sphere in a long channel
*
*
create mappings
* first make a sphere
Sphere
  outer radius
   .75    .8
exit
*
* now make a mapping for the north pole
*
reparameterize
  orthographic
    specify sa,sb
      2. 2.
  exit
  lines
    * 31 31 9  
    21 21 7
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
      2. 2.
  exit
  lines
    * 31 31 9  
    21 21 7
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
  specify corners
    -2 -2 -2 2 2 10
  lines
    * 41 41 121  
    31 31 91
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
    ghost points
      all
      2 2 2 2 2 2
  exit
  * pause
  compute overlap
exit
save an overlapping grid
silc.hdf
silc
exit
