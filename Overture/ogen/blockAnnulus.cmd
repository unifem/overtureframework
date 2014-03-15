*
* Create a "block-structured" grid for two annulii using the 
* "maximize overlap" option
*
create mappings
  annulus
    inner and outer radii
    .5 1.
    boundary conditions
     -1 -1 1 0
    mappingName
     innerAnnulus
    exit
  annulus
    inner and outer radii
      1. 1.5
    boundary conditions
    -1 -1 0 1
    mappingName
      outerAnnulus
    exit
  exit this menu
generate an overlapping grid
  innerAnnulus
  outerAnnulus
  change parameters
    maximize overlap
    exit
  compute overlap
  exit
*
* save an overlapping grid
save a grid (compressed)
blockAnnulus.hdf
blockAnnulus
exit
