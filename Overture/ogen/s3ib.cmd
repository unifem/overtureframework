*
* command file to create a sphere in a box
* the sphere is covered by 3 patches.
*
*
create mappings
* first make a sphere
Sphere
  boundary conditions
    0 0 -1 -1 1 0 
  share
    0 0 0 0 1 0
  lines 
   21 21 7
exit
*
* now make a mapping for the north pole
*
reparameterize
  orthographic
    specify sa,sb
      1. 1.
  exit
  lines
    15 15 5
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
      1. 1.
  exit
  lines
    15 15 5
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
  sphere
  north-pole
  south-pole
  done
  change parameters
    * use old
    ghost points
      all
      2 2 2 2 2 2
  exit
  pause
  compute overlap
exit
save an overlapping grid
s3ib.hdf
sib
exit
