*
* test grid for remove unused edges
*
create mappings
  Annulus
    start and end angles
      .5 1.
    boundary conditions
      1 1 1 0
    share
      1 1 0 0
    lines
      25 11
    exit
  rectangle
    specify corners
      -1. -1 1. 0. 
    lines
      31 15
    share
      0 0 0 1
    exit
*
  rectangle
    mappingName
      refinement
    specify corners
      -.75 -.5 .75 .0 
    lines
      31 11
    boundary conditions
      0 0 0 1
    share
      0 0 0 1
    exit
  exit this menu
*
  generate an overlapping grid
    square
    Annulus
    refinement
  done
  change parameters
*   we need to manually cut holes where the square and refinement grid
*   have a shared boundary that is outside the domain
    manual hole cutting
      square
        8 22 10 14 0 0
      refinement
        7 23  8 10 0 0
      done
  done
* display intermediate
* pause
  compute overlap
  exit
*
save an overlapping grid
edgeRefinement.hdf
edgeRefinement
exit