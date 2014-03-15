*
* Ellipsoid in a box
*
create mappings
*
  Box
    set corners
      -2. 2. -2. 2. -3.5 3.5
    lines
      21 21 35
    exit
*
  CrossSection
    mappingName
      ellipsoid
    ellipse
    a,b,c for ellipse
      1. 1. 2
    lines
      19 25 5
    share
      0 0 0 0 1 0
    exit
*
  reparameterize
    mappingName
      north-pole
    lines
      11 11 5
    share
      0 0 0 0 1 0
    exit
*
  reparameterize
    mappingName
      south-pole
    orthographic
      choose north or south pole
        -1
      exit
    lines
      11 11 5
    share
      0 0 0 0 1 0
    exit
exit
*
  generate an overlapping grid
    box
    ellipsoid
    north-pole
    south-pole
  done
  change parameters
   interpolation type
     explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
#   debug 
#      3
  compute overlap
# continue
# continue
# continue
# continue
# continue
# continue
exit
*
save an overlapping grid
ellipsoid.hdf
ellipsoid
exit    