*
* command file to create 2 spheres in a box
*
*
create mappings
* first make a sphere
Sphere
  centre for sphere
    -.5 -.5 -.5
  mappingName
    sphere1
exit
*
* now make a mapping for the north pole
*
reparameterize
  transform which mapping?
  sphere1
  orthographic
    specify sa,sb
      2.5 2.5
  exit
  lines
    15 15 5
  boundary conditions
    0 0 0 0 1 0
  share
    0 0 0 0 2 0
  mappingName
    north-pole1
exit
*
* now make a mapping for the south pole
*
reparameterize
  transform which mapping?
  sphere1
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
  share
    0 0 0 0 2 0
  mappingName
    south-pole1
exit
*
* make a second sphere
*
Sphere
  centre for sphere
    +.5 +.5  .5
  mappingName
    sphere2
exit
*
* now make a mapping for the north pole
*
reparameterize
  transform which mapping?
  sphere2
  orthographic
    specify sa,sb
      2.5 2.5
  exit
  lines
    15 15 5
  boundary conditions
    0 0 0 0 1 0
  share
    0 0 0 0 2 0
  mappingName
    north-pole2
exit
*
* now make a mapping for the south pole
*
reparameterize
  transform which mapping?
  sphere2
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
  share
    0 0 0 0 2 0
  mappingName
    south-pole2
exit
*
* Here is the box
*
Box
  specify corners
    -2 -2 -2 2 2 2 
  lines
    21 21 21  31 31 31
  mappingName
    box
  exit
*
* pause
exit
*
generate an overlapping grid
  box
  north-pole2
  south-pole2
  north-pole1
  south-pole1
 done
  change parameters
    improve quality
    ghost points
      all
      2 2 2 2 2 2
  exit
  * pause
 compute overlap
exit
save an overlapping grid
twoSphere.hdf
twoSphere
exit
