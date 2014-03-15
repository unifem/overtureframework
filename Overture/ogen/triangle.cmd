*
create mappings
  rectangle
    mappingName
      backGround
    specify corners
      -1. -1.5 2. 1.5
    lines
      301 301  241 241  121 121  61 61
    exit
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
      301 25 241 21  121 11  61 6  75 7
    mappingName
      triangle
    exit
  exit this menu
  generate an overlapping grid
    backGround
    triangle
    change parameters
      interpolation type
        explicit for all grids
      ghost points
        all
        2 2 2 2 2 2
    exit
    * pause
    compute overlap
  exit
*
save an overlapping grid
triangle.hdf
triangle
exit
