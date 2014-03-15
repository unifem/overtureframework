*
* Make a 2d test valve, 2d version of valve3d
*  
create mappings
rectangle
  specify corners
    .4 -.5 1. .25
  lines
    27 32 
  boundary conditions
    1 1 1 1
  share
    1 2 3 0
  mappingName
    square
exit
*  Make a 2d cross-section of the valve
  SmoothedPolygon
    mappingName
      valve
    vertices
      4
      .4 0.
      .85 0.
      .65 -.2
      .4 -.2
    n-dist
    fixed normal distance
      .15
    sharpness
      30 
      30 
      30
      30
    lines
      41 11 35
    boundary conditions
      2 2 1 0
    share
      1 1 0 0
  exit
* 2D cross section for the stopper
  SmoothedPolygon
    mappingName
      stopper
    vertices
      4
      .65 -.5
      .65 -.3
      .85 -.1
      1. -.1
    n-dist
      fixed normal distance
      .15
    boundary conditions
      1 1 2 0 
    share
      3 2 0 0
    lines
      35 11 41
  exit
exit
* 
generate an overlapping grid
    square
    stopper
    valve
  done 
  change parameters
    * make the grid cell-centered
    cell centering
      cell centered for all grids
  exit
  compute overlap
  exit
exit

