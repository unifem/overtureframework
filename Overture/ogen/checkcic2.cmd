*
* circle in a channel
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
  lines
    11 11 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  lines
    15 3
  boundary conditions
    -1 -1 1 0
exit
*
exit
check overlap
  square
  Annulus
  done
junk
