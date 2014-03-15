*
* Grid for a tube
*
create mappings
  Box
    mappingName
      box
    specify corners
    -.75  -.75  0. .75 .75  1.
    lines
      31 31 21 
    boundary conditions
      0 0 0 0 1 1 
    share
      0 0 0 0 0 0
  exit
*
  cylinder
    lines
      101 21 13  101 21 7   41 11 5 
    bounds on the radial variable
      .5 1.   .75 1.   .6 1.
    bounds on the axial variable
      0. 1.
    boundary conditions
      -1 -1 1 1 0 1
    share
      0 0 0 0 0 0
    mappingName
     cylinder
    exit
exit
*
  generate an overlapping grid
    box
    cylinder
   done
   change parameters
    ghost points
      all
      2 2 2 2 2 2
    interpolation type
      explicit for all grids
    order of accuracy
     fourth order
   exit
   compute overlap
* pause
 exit
*
save an overlapping grid
tube2.order4.hdf
tube
exit
