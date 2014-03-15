create mappings
  rectangle
    specify corners
      -1 -1 1 1
    lines
      51 51 40 40
    mappingName
      BigSquare
    share
      1 2 0 0
    exit
  rectangle
    specify corners
      -1.0 -.75 1.0 -0.25
    lines
      81 21
    mappingName
      SurfaceRect
    boundary conditions
      1 1 0 0
    share
      1 2 0 0
    exit
  Annulus
    centre for annulus
      -.25 .35
    inner radius
      .1
    outer radius
      .3
    lines
      31 10
    boundary conditions
      -1 -1 1 0
    mappingName
      leftObstacle
    exit
  Annulus
    centre for annulus
      .25 .1
    inner radius
      .15
    outer radius
      .3
    outer radius
      .4
    lines
      31 10
    boundary conditions
      -1 -1 1 0
    mappingName
      rightObstacle
    exit
  exit this menu
  generate a hybrid mesh
    BigSquare
    leftObstacle
    rightObstacle
    SurfaceRect
    compute overlap
  exit
  use triangle
  continue generation
  exit
  save grid in ingrid format
  obstacle.hyb.msh
exit
*
