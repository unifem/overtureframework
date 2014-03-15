*
* command file to create half a spherical shell
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
    1 2 3 4 5 6
  mappingName
    north-pole
exit
exit
*
*
generate an overlapping grid
  north-pole
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
*
save an overlapping grid
 halfSphere.hdf
 halfSphere
exit
