*
* circle in a channel, for fourth order accuracy
*
create mappings
*
rectangle
  set corners
    -2. 2. -2. 2.
  lines
      121 121 481 481 121 121 61 61  241 241 121 121  61 61  32 32 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  outer radius
    .75
  lines
     161 9 641  33  161 9  81 5     
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
      2 2 2 3
    order of accuracy
      fourth order
  exit
  compute overlap
  exit
*
save an overlapping grid
cice.order4.hdf
cic
exit

