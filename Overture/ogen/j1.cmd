* make cylindrical (two to avoid periodicity) and rectangular meshes  
create mappings
  Cylinder
    centre for cylinder
      0. 0. 0.
    bounds on theta
      -0.3 .3
    bounds on the axial variable
*      0. 2.e3
      0. 2.       
    bounds on the radial variable
*      .75e6 1.e6
      .75 1. 
    lines
    61 13 11
    boundary conditions
      0 0 3 3 0 1
    share
      0 0 1 2 0 3
    mappingName
      cyl1
  exit
  stretch coordinates
    transform which mapping?
    cyl1
    stretch
      specify stretching along axis=2 (radius)
        stretching type
         exponential
        exponential parameters
          1. 0. -1. -2. 0.
        exit
      exit
    exit
  Cylinder
    centre for cylinder
      0. 0. 0.
    bounds on theta
      .2 .8
    bounds on the axial variable
*      0. 2.e3
      0. 2.       
    bounds on the radial variable
*      .75e6 1.e6
      .75 1. 
    lines
      61 13 11
    boundary conditions
      0 0 3 3 0 1
    share
      0 0 1 2 0 3
    mappingName
      cyl2
  exit
  stretch coordinates
    transform which mapping?
    cyl2
    stretch
      specify stretching along axis=2 (radius)
        stretching type
         exponential
        exponential parameters
          1. 0. -1. -2. 0.
        exit
      exit
    exit
  Box
    specify corners
*      -1.e6 -1.e6 0. 1.e6 1.e6 2.e3
      -1. -1. 0. 1. 1. 2.   
    lines
      41 41 13
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
  stretched-cyl1
  stretched-cyl2
  done
  change parameters
    ghost points
    all
    2 2 2 2 2 2
  exit
  pause
  compute overlap
exit
* save the overlapping grid
save an overlapping grid
  j2.hdf
  cyl
exit