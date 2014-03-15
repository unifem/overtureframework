*
* Ellipsoid in a box
*
create mappings
*
  Box
    specify corners
      -2. -2. -3. 2. 2. 3.
    lines
      21 21 31
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
      cell centering
      cell centered for all grids
    exit
  compute overlap
exit
*
save an overlapping grid
ellipsoidCC.hdf
ellipsoid
exit    