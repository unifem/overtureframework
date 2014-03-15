*
* This example shows how to build an overlapping grid and then
* incrementally add new component grids
*
create mappings
*
rectangle
  set corners
    -2. 2. -2. 2.
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
    33 7
*  centre
*    0. 1.
  boundary conditions
    -1 -1 1 0
exit
*
rectangle
  set corners
    -2. 0. -2. 0.
  lines
    21 21 
  boundary conditions
    0 0 0 0
  mappingName
  square2
exit
*
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
*   -- first build the grid with two components
  compute overlap
*  Now add an additional grid -- this uses an algorithm optimised for this purpose
  add grids
   square2
  done
  compute overlap
  exit
*
save an overlapping grid
cicAdd.hdf
cic
exit

