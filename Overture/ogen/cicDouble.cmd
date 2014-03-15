*
* Create a cylinder in a channel using two grids around the annulus
*
create mappings
  rectangle
    specify corners
      -2 -2 2 2
    lines
      31 31
    mappingName
      square
  exit
*
  Annulus
    inner and outer radii
      .5 1.0
    start and end angles
      -.3 .3
    mappingName
      rightAnnulus
    boundary conditions
     0 0 1 0
    share 
      0 0 1 0
  exit
*
  Annulus
    inner and outer radii
      .5 1.0
    start and end angles
      .2 .8
    boundary conditions
      0 0 1 0
    share 
      0 0 1 0
    mappingName
      leftAnnulus
  exit
exit
*
generate an overlapping grid
    square
    leftAnnulus
    rightAnnulus
  done
  change parameters
    * choose implicit or explicit interpolation
    * interpolation type
    *   implicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
  compute overlap
  pause
  exit
*
save an overlapping grid
cicDouble.hdf
cicDouble
exit
