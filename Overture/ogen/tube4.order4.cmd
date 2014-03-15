*
* Grid for a tube
*
create mappings
  Box
    mappingName
      box
    specify corners
    -.925 -.925 0. .925 .925  1.
    lines
      149 149 81   69 69 41  29 29 21 
    boundary conditions
      0 0 0 0 1 1 
    share
      0 0 0 0 0 0
  exit
*
  cylinder
    lines
      421 81 13  211 41 13 *  101 21 13  
    bounds on the radial variable
      .875 1.  .75 1.   .5 1.
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
    order of accuracy
     fourth order
    interpolation type
      explicit for all grids
   exit
   compute overlap
* pause
 exit
*
save an overlapping grid
tube4.order4.hdf
tube
exit
