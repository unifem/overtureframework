*
* circle in a channel
*
create mappings
*
rectangle
  set corners
    -2. 2. -2. 2.
  lines
    2049 2049  1025 1025 513 513 257 257 129 129 65 65 33 33 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  lines
    2049 65   1025 65 257 513 129  257 65 129 33 65 17 33 9  33 17  33 9
  outer radius
*   1.1
*     .8
    .9
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
    interpolation type
      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
  compute overlap
  exit
*
save an overlapping grid
cic7e.hdf
cic
exit
