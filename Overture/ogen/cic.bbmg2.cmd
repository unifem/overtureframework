*
* circle in a channel
*
create mappings
*
rectangle
  set corners
    -2. 2. -2. 2.
  lines
    65 65 33 33 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  lines
    65 17 33 9  33 17  33 9
  outer radius
   1.1
*  centre for annulus
*    .1 0
  boundary conditions
    -1 -1 1 0
exit
*
exit
generate an overlapping grid
    square
    Annulus
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
  exit
*
save an overlapping grid
cic.bbmg2.hdf
cic
exit
