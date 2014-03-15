*
* circle in a channel
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
  lines
    15 15 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  lines
    17 4
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
    * do not interpolate ghost
    * choose implicit or explicit interpolation
    interpolation type
      * implicit for all grids
      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit

  display intermediate
  set debug
    31

  pause
  compute overlap
  exit
*
save an overlapping grid
cicc.hdf
cicc
exit
