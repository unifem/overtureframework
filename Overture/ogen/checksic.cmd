* 
* Create a "square in a circle"
*
create mappings
* first make an annulus
  Annulus
    inner radius
      .4
    outer radius
      1.
    lines
      21 7
    mappingName
      annulus
    boundary conditions
      -1 -1 0 1
    exit
*  make a rectangle to fill in the inside of the annulus
  rectangle
    specify corners
      -1.4 -1.4 1.4 1.4
    lines
      21 21
    boundary conditions
      0 0 0 0
    mappingName
      inner-square
    exit
  exit
*
check overlap
  inner-square
  annulus
  done
