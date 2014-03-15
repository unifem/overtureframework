*
* circle in a channel
*
create mappings
*
rectangle
  set corners
    -2. 2. -2. 2.
  lines
    11 11  15 15  21 21  32 32 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  lines
    11 3  15 4 21 5  33 7
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
    interpolation type
      implicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
  compute overlap
  exit
*
save an overlapping grid
cic2d.hdf
cic2d
exit

