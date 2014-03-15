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
  lines
    33 7
  boundary conditions
    -1 -1 1 0
exit
  Circle or ellipse
    specify radius of the circle
      .5
    specify centre
      1.25 0.
    exit
*
exit
generate an overlapping grid
    square
    Annulus
  done
  compute overlap
*
    build a cutout
    circle


