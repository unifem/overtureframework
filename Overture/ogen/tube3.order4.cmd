*
* Grid for a tube
*
create mappings
  Box
    mappingName
      box
    specify corners
    -.85  -.85  0. .85 .85  1.
    lines
      69 69 41  29 29 21 
    boundary conditions
      0 0 0 0 1 1 
    share
      0 0 0 0 0 0
  exit
*
  cylinder
    lines
      211 41 13 *  101 21 13  
    bounds on the radial variable
      .75 1.  * .5 1. 
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
 exit
*
save an overlapping grid
tube3.order4.hdf
tube
exit
