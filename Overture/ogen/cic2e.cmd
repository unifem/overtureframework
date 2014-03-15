*
* circle in a channel
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
  lines
    25 25 13 13 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  inner radius
   1.
  outer
   .5  
  lines
    31 5  16 3
  boundary conditions
    -1 -1 0 1 1 0
exit
*
exit
generate an overlapping grid
    square
    Annulus
  done
  change parameters
    * choose implicit or explicit interpolation
    interpolation type
      * implicit for all grids
      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  * pause
  compute overlap
  exit
*
save an overlapping grid
cic2e.hdf
cic2
exit
