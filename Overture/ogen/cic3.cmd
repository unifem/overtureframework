*
* circle in a channel
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
  lines
    49 49  25 25 13 13 
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
    61 9    31 5 
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
*  display intermediate
  compute overlap
  exit
*
save an overlapping grid
cic3.hdf
cic3
exit
