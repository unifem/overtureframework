* 
* Create a "square in a circle"
*
create mappings
* first make an annulus
  Annulus
    inner radius
      .6
    outer radius
      1.
    lines
      21 3
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
      9 9 
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
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
save an overlapping grid
sic1.hdf
sic1
exit
