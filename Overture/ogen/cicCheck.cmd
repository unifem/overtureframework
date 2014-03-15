*
* check an annulus on a fine grid -- for hole cutting
*
create mappings
*
rectangle
  set corners
    -2. 2. -2. 2.
  lines
    61 61 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  lines
    11 5
  boundary conditions
    -1 -1 1 0
exit
*
exit
generate an overlapping grid
    square
    Annulus
  done
  display intermediate results
  compute overlap
  continue

