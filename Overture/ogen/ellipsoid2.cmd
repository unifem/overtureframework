*
* Ellipsoid in a box
*
create mappings
*
  Box
    set corners
      -2.5 2.5 -2.5 2.5 -4. 5. 
    lines
      51 51 91 
    exit
*
  CrossSection
    mappingName
      ellipsoid
    ellipse
    a,b,c for ellipse
      1. 1. 2
    lines
      51 49 9   19 25 5
    share
      0 0 0 0 1 0
    exit
*
  reparameterize
    mappingName
      north-pole
    orthographic
      choose north or south pole
        +1
      specify sa,sb
        .5 .5
      exit
    lines
      15 15 9   11 11 5
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
      specify sa,sb
        .5 .5
      exit
    lines
      15 15 9  11 11 5
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
  compute overlap
exit
*
save an overlapping grid
ellipsoid2.hdf
ellipsoid
exit    