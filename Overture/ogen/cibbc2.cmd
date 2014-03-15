*
* circle in a bigger channel
*
create mappings
*
rectangle
  set corners
    -8. 8. -8. 8. -4. 4. -4. 4.
  lines
    961 961  481 481 241 241 
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
     321 17  161 9 641  33  161 9  81 5     
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
  exit
  compute overlap
  exit
*
save an overlapping grid
cibbc2.hdf
cibc
exit

