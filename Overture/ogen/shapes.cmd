*
create mappings
  rectangle
    mappingName
      backGround
    set corners
      -1.25 1.25 -1. 1. 
    lines
      53 41    51 41 
    exit
*
  SmoothedPolygon
    vertices
      5
       .866025 0.
       .866025 .5
      0. 0 .
       .866025 -.5
       .866025 0.
    boundary conditions
      -1 -1 1 0
    n-dist
    fixed normal distance
      -.15
    n-stretch
      1. 2.5 0
    t-stretch
      0. 0.
      .15 30
      .15 30
      .15 30
      .15 30
    lines
      77 9  75 7
    mappingName
      triangle
    exit
*
*
* failed if square is moved right by .25
  SmoothedPolygon
    vertices
      5
       -1. -.25 
       -1. -.75
       -.25 -.75
       -.25 -.25
       -1.  -.25
    boundary conditions
      -1 -1 1 0
    n-dist
    fixed normal distance
      -.15
    n-stretch
      1. 2.5 0
    t-stretch
      .15 30.
      .15 30
      .15 30
      .15 30
      .15 30
    lines
      77 9  75 7
    mappingName
      square
    exit
*
  Annulus
    lines
      37 9 33 7
    inner and outer
      .25 .5
    centre
      -.5 .35
    boundary conditions
      -1 -1 1 0
    mappingName
      annulus
  exit
*
  exit this menu
*
 generate an overlapping grid
    backGround
    triangle
    annulus
    square
  change parameters
    * choose implicit or explicit interpolation
    * interpolation type
    *   implicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
  compute overlap
  exit
*
save an overlapping grid
shapes.hdf
shapes
exit

