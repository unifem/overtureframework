*
* circle in a channel with MG levels
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
  lines
    45 45 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  lines
    65 9 
  boundary conditions
    -1 -1 1 0
exit
*
exit
generate an overlapping grid
  specify number of multigrid levels
    4
  square
  Annulus
  done
  change parameters
    interpolation type
      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
     * do not interpolate ghost
  exit
  compute overlap
  pause
exit
save an overlapping grid
cicmg2.hdf
cic
exit
