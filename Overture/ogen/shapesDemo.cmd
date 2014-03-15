*
create mappings
  rectangle
    mappingName
      backGround
    set corners
      -1.25 1.25 -1. 1. 
    lines
      51 41 
pause
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
      75 7
    mappingName
      triangle
pause
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
      75 7
    mappingName
      square
pause
    exit
*
  Annulus
    lines
      33 7
    inner and outer
      .25 .5
    centre
      -.5 .35
    boundary conditions
      -1 -1 1 0
    mappingName
      annulus
pause
  exit
*
  exit this menu
*
  generate a hybrid mesh
    backGround
    triangle
    annulus
    square
plot
pause
    compute overlap
    exit
      erase and exit
    * bigger:0 1.1
  Advancing Front...
    plot all edges 1
  exit
  plot component grids (toggle) 1
pause
  continue generation


    set plotting frequency (<1 for never)
      -1
