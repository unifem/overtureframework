create mappings
  *
  SmoothedPolygon
    mappingName
    inlet-left
    vertices
    3
    -.5 .5
    -.5 0.
    -1. 0.
    n-dist
    fixed normal distance
    .6
    sharpness
    10
    10
    10
    t-stretch
    0 50
    .15 10
    0. 50
    lines
    31 9
    boundary conditions
    1 1 1 0
    share
    1 2 3 0
    exit
  *
  SmoothedPolygon
    mappingName
    inlet-right
    vertices
    3
    .5 .5
    .5 0.
    1. 0.
    n-dist
    fixed normal distance
    -.6
    sharpness
    10
    10
    10
    t-stretch
    0 50
    .15 10
    0. 50
    lines
    31 9
    boundary conditions
    1 1 1 0
    share
    1 2 3 0
    exit
  *
  rectangle
    mappingName
    square
    specify corners
    -1. -1. 1. 0.
    lines
    41 21
    boundary conditions
    1 1 1 1
    share
    2 2 0 3
    exit
*
  Annulus
    centre for annulus
      0. -.5
    inner radius
      .2
    outer radius
      .4
    mappingName
      annulus
    boundary conditions
      -1 -1 1 0
    exit
  exit this menu
  generate an overlapping grid
    square
    inlet-left
    inlet-right
    annulus
    change parameters
      prevent hole cutting
        square
        all
        done
      exit
    display intermediate results
