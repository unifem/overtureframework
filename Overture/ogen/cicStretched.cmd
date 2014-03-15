*
* circle in a channel
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
  lines
    32 32 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  lines
    33 31
  boundary conditions
    -1 -1 1 0
exit
  stretch coordinates
    transform which mapping?
    Annulus
    stretch
      specify stretching along axis=1
        layers
        1
        1. 20. 0.
        exit
      exit
    mappingName
    annulus
    exit
*
exit
generate an overlapping grid
    square
    annulus
  done
  change parameters
    * choose implicit or explicit interpolation
    interpolation type
      implicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  * pause
  compute overlap
  exit
*
save an overlapping grid
cicStretched.hdf
cic
exit

