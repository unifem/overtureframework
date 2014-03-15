*
* circle in a channel
*
create mappings
*
box
  set corners
    -2. 2. -2. 2. 0. 1.
  lines
    11 11 3   32 32 3
  boundary conditions
    1 1 1 1 1 1
*  periodicity
*    0 0 1
  share
    0 0 0 0 1 2
  mappingName
    square
exit
*
Annulus
  lines
    33 7
  boundary conditions
    -1 -1 1 0
  mappingName
   annulus2d
exit
*
sweep
  lines
    11 3 3   33 7 3
  periodicity
    2 0 0
*    2 0 1
  share
    0 0 0 0 1 2  
  mappingName
   Annulus
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
  pause
  exit
*
save an overlapping grid
cic3d.hdf
cic3d
exit

