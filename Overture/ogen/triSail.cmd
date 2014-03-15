*
create mappings
*
* first make a little "corner" grid that we will use to sweep around the sail boundary
  smoothedPolygon
    vertices
      4
      .0125   0.0
      .0125  -.1
      -.0125 -.1
      -.0125  0.
    lines
      15 7
    n-dist
    fixed normal distance
      .075  .1 
     sharpness
      10
      10
      10
      10
     t-stretch
      0 5
      .25 15
      .25 15
      0 5
    mappingName
      edge2d
    exit
*
* define a curve for the boundary of the sail sheet
  spline (3D)
    enter spline points
      15
      .5    0  0
      .75   0  0
      .975  .025  0
      .75   .5 0
      .5   1.  0
      .25  1.5 0
      .1   1.8   0
      .03  1.925  0
      .0   1.8   0
      0    1.5 0
      0    1. 0
      0     .5 0
      .025  .025 0
      .25   0 0
      .5    0 0
    periodicity
      2
    shape preserving (toggle)
    lines
      101
    curvature weight
      1
    * pause
    exit
*
* sweep a grid around the edge of the sail
  sweep
   use center of sweep curve
   boundary conditions
     0 0 1 0 -1 -1 
   share
     0 0 1 0 0 0
   mappingName
     edge
    * pause
   exit
*
* define the front face of the sail 
  dataPointMapping
    enter points
      3 3
      2 2 2
      -.0125 -.0125 .0125
      1.0125 -.0125 .0125
      -.0125 1.95   .0125
      .1     1.95   .0125
*
      -.0125 -.0125 .0895
      1.     -.0125 .0895
      -.0125 1.95   .0895
      .1     1.95   .0895
    boundary conditions
      0 0 0 0 1 0
    share
      0 0 0 0 1 0
    lines
      25 51 6   31 61 6   21 41 6
    mappingName
      front
    * pause
    exit
*
  dataPointMapping
    enter points
      3 3
      2 2 2
      -.0125 -.0125 -.0895
      1.0125 -.0125 -.0895
      -.0125 1.95   -.0895
      .1     1.95   -.0895
*
      -.0125 -.0125 -.0125
      1.     -.0125 -.0125
      -.0125 1.95   -.0125
      .1     1.95   -.0125
    boundary conditions
      0 0 0 0 0 1
    share
      0 0 0 0 0 1
    lines
      25 51 6  31 61 6  21 41 6
    mappingName
      back
    * pause
    exit
*
  box
    set corners
      -.25 1.25 -.25 2.25 -.25 .25 
    lines
      41 81 31  51 101 31 
    mappingName
      container
    exit
  exit this menu
*
  generate an overlapping grid
    container
    front
    back
    edge
    done choosing mappings
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
    change the plot
      toggle grids on and off
        0 : container is (on)
      exit this menu
    exit
  display intermediate results
  pause
  compute overlap
  exit
*
save an overlapping grid
squareSail.hdf
squareSail
exit


    change the plot
      toggle grids on and off
        0 : container is (on)
      exit this menu
    exit
    display intermediate