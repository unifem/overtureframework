* make cylindrical (two to avoid periodicity) and rectangular meshes  
create mappings
  Cylinder
    centre for cylinder
      0. 0. 0.
    bounds on theta
      -.05 .55
    bounds on the axial variable
      0. 2.e3
*     -1. 0.       
    bounds on the radial variable
      .75e6 1.e6
*      .75 1. 
    lines
    61 9 11
    boundary conditions
      0 0 3 3 0 1
    share
      0 0 1 2 0 3
    mappingName
      cyl1
  exit
  Cylinder
    centre for cylinder
      0. 0. 0.
    bounds on theta
      .45 1.05
    bounds on the axial variable
      0. 2.e3
*      -1. 0.       
    bounds on the radial variable
      .75e6 1.e6
*      .75 1. 
    lines
      61 9 11
    boundary conditions
      0 0 3 3 0 1
    share
      0 0 1 2 0 3
    mappingName
      cyl2
  exit
  Box
    specify corners
      -1.e6 -1.e6 0. 1.e6 1.e6 2.e3
*      -1. -1. -1. 1. 1. 0.   
    lines
      41 41 9
    boundary conditions
      0 0 0 0 3 3
    share
      0 0 0 0 1 2 
    mappingName
      cent
  exit
  DataPointMapping
    read file
    tail.plot3d
    boundary conditions
      0 0 0 0 3 3
    share
      0 0 0 0 1 2 
  exit
exit this menu
* generate an overlapping grid and add ghost points
generate an overlapping grid
  cent
  cyl1
  cyl2
  done
  change parameters
    ghost points
    all
    2 2 2 2 2 2
  exit
  pause
  compute overlap
