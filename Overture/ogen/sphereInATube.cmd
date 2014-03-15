*
* command file to create a sphere in cylindrical tube
*
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
      2.5 2.5
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
* Here is the cylinder
*
  * main cylinder
  Cylinder
    mappingName
      cylinder
    * orient the cylinder so y-axis is axial direction
    orientation
      2 0 1
    bounds on the radial variable
      .3 .8 
    bounds on the axial variable
      -1. 1.
    lines
      55 21 9
    boundary conditions
      -1 -1 2 3 0 4
    share
      0 0 2 3 0 0 
  exit
* core of the main cylinder
  Box
    mappingName
      cylinderCore
    specify corners
    -.5 -1. -.5  .5 1. .5
    lines
      19 21 19
    boundary conditions
      0 0 2 3 0 0 
    share
      0 0 2 3 0 0
  exit
*  pause
*
exit
generate an overlapping grid
  cylinderCore 
  cylinder
  north-pole
  south-pole
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
   * display intermediate
  compute overlap
  * continue
  * pause
exit
save an overlapping grid
sphereInATube.hdf
sit
exit
