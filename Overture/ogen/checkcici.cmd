*
* circle in a channel with an intersecting grid
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
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
    32 7
  boundary conditions
    -1 -1 1 0
exit
*
rectangle
  specify corners
    -.5  1.75 .5 2.75
  lines
    11 11 
  boundary conditions
    1 1 0 1
  mappingName
  intersectingSquare
exit
exit
*
check overlap
    square
    Annulus
    intersectingSquare
  done
  change parameters
    prevent hole cutting
      square
      intersectingSquare
      intersectingSquare
      square
   done
    non-conforming
      intersectingSquare
exit
cut holes and remove exterior points


