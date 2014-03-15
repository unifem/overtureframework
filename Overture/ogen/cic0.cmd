*
* circle in a channel
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
  lines
   12 12
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  outer
   1.475    ********** 
  lines
    15 5
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
      implicit for all grids
     * explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  * pause
  display intermediate results
  compute overlap
  continue



  exit
*
save an overlapping grid
cic0.hdf
cic0
exit

