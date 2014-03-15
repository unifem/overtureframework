*
* Make a cylinder in a box
*
create mappings
  Box
    mappingName
      box
    specify corners
    -.5 -.5 -.5 .5 .5 .5
    lines
      41 41 41  21 21 21  11 11 11  
    boundary conditions
      1 1 2 2 3 3 
    share
      0 0 0 0 1 2
  exit
*
  Cylinder
    mappingName
      cylinder
    bounds on the radial variable
      .2 .4
    bounds on the axial variable
      -.5 .5
    lines
      89 41 9  45 21 5  23 11 3 
    boundary conditions
     -1 -1 1 1 1 0   -1 -1 1 1 2 0
    share
      0 0 1 2 0 0
  exit
exit
*
*
generate an overlapping grid
    box
    cylinder
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
* pause
  compute overlap
  exit
*
save an overlapping grid
cylBox3.hdf
cylBox3
exit


