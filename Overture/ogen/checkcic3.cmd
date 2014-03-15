*
* circle in a channel
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
  outer radius
    3.
  lines
    32 21
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
