*
* circle in a channel
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
  lines
    289 289   145 145 73 73  49 49 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  outer
   1.  
  lines
    361 25  181 25  91  13
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
      * explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  * pause
  compute overlap
  exit
*
save an overlapping grid
cic6.hdf
cic6
exit
