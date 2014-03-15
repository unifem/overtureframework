* 
* Create a "square in a circle"
*
create mappings
* first make an annulus
  Annulus
    inner radius
      .75
    outer radius
      1.
    lines
      161 13 81 7  41 5  21 3
    mappingName
      annulus
    boundary conditions
      -1 -1 0 1
    exit
*  make a rectangle to fill in the inside of the annulus
  rectangle
    specify corners
      -.75 -.75 .75 .75 -2. -2. 2. 2. 
    lines
      57 57 29 29 33 33  17 17 9 9 
    boundary conditions
      0 0 0 0
    mappingName
      inner-square
    exit
  exit
*
*
generate an overlapping grid
  inner-square
  annulus
  done
  change parameters
    interpolation type
      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
* pause
exit
save an overlapping grid
circle2.hdf
circle2
exit
