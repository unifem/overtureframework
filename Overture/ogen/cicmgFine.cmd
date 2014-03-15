*
* circle in a channel with MG levels
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
  lines
    301 301     101 101 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  lines
    301 61   101 21 
  boundary conditions
    -1 -1 1 0
exit
*
exit
generate an overlapping grid
  specify number of multigrid levels
    3
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
exit
save an overlapping grid
cicmgFine.hdf
cicmgFine
exit
