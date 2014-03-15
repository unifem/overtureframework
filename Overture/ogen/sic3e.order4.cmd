* 
* Create a "square in a circle"
*
create mappings
* first make an annulus
  Annulus
    inner radius
      .8
    outer radius
      1.
    lines
      161 7  61 7 31 5
    mappingName
      annulus
    boundary conditions
      -1 -1 0 1
    exit
*  make a rectangle to fill in the inside of the annulus
  rectangle
    set corners
      -.85 .85  -.85 .85
    lines
      49 49  25 25 11 11
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
    order of accuracy
     fourth order
  exit
  compute overlap
exit
save an overlapping grid
sic3e.order4.hdf
sic
exit

