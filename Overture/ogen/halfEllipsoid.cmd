*
* command file to create a grid for the end of an ellipsoid
*
create mappings
* first make an ellipsoid
CrossSection
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
make an overlapping grid
  1
  north-pole
  GRPAR
    numberOfGhostPoints
      2 2 2 2 2 2
  EXIT
EXIT
save an overlapping grid
halfEllipsoid.hdf
halfEllipsoid
exit
