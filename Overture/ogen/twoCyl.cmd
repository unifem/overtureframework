*
* Create a grid exterior two two intersecting pipes
*
create mappings
*
* Here is the box
*
  Box
    specify corners
      -1. -1. -1. 1. 1. 1.
    lines
      * 61 31 31  trouble here with holes being cut on the box below the cylinder
      26 26 26   32 32 32 62 32 32   
    mappingName
      box
    share
      1 1 0 2 0 0
    exit
*
  Cylinder
    mappingName
      main-cylinder
    orientation
      1 2 0
    bounds on the axial variable
      -1. 1.
    bounds on the radial variable
      .5 .75
    boundary conditions
      -1 -1 1 2 3 0
    lines
      31 21 6
    share
      0 0 1 1 3 0
    exit
*
  Cylinder
    mappingName
      top-cylinder
    orientation
      2 0 1
    bounds on the axial variable
      .25 1.
    bounds on the radial variable
      .3 .6
    boundary conditions
      -1 -1 0 2 3 0
    lines
      25 15 5
    share
      0 0 0 2 3 0
    exit
*
  exit
generate an overlapping grid
  box
  top-cylinder
  main-cylinder
  done
  change the plot
    toggle grids on and off
    0 : box is (on)
    exit this menu
    x+r
    y+r
    y+r
    x+r
  exit this menu
  display intermediate results
  pause
  compute overlap
