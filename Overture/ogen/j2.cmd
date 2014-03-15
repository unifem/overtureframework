* make cylindrical (two to avoid periodicity) and rectangular meshes  
create mappings
  Cylinder
    centre for cylinder
      0. 0. 0.
    bounds on theta
      -0.30 .30
    bounds on the axial variable
*      0. 2.e3
      0. 2.       
    bounds on the radial variable
*      .75e6 1.e6
      .75 1. 
    lines
    61 19 11
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
      .20 .80
    bounds on the axial variable
*      0. 2.e3
      0. 2.       
    bounds on the radial variable
*      .75e6 1.e6
      .75 1. 
    lines
      61 19 11
    boundary conditions
      0 0 3 3 0 1
    share
      0 0 1 2 0 3
    mappingName
      cyl2
  exit
  Box
    specify corners
*      -1.e6 -1.e6 0. 1.e6 1.e6 2.e3
      -1. -1. 0. 1. 1. 2.   
    lines
      61 61 19
    boundary conditions
      0 0 0 0 3 3
    share
      0 0 0 0 1 2 
    mappingName
      cent
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
  compute overlap
exit
* save the overlapping grid
save an overlapping grid
  j2.hdf
  cyl
exit