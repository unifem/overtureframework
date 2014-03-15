*
* Grid for a tube
*
create mappings
  Box
    mappingName
      box
    specify corners
    -.7  -.7  0. .7 .7  1.
    lines
      15 15 11 
    boundary conditions
      0 0 0 0 1 1 
    share
      0 0 0 0 0 0
  exit
*
  cylinder
    lines
      51 11 7 51 11 5 
    bounds on the radial variable
     .4 1.
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
tube1.order4.hdf
tube
exit
