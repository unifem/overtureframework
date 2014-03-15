*
* circle in a channel with MG levels
*
create mappings
*
rectangle
  set corners
    -2. 2.  -2. 2.
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
    2
  square
  Annulus
  done
  change parameters
    interpolation type
      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  * pause
  compute overlap
exit
save an overlapping grid
cicmg.hdf
cic
exit
