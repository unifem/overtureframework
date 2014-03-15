*
* two squares one on top of the other.
* test the ray tracing
*
create mappings
  rectangle 
    specify corners
      0. -.25 1. 1.
    lines
      9 7
    boundary conditions
      1 1 0 2
    share
      1 2 0 0
    mappingName
      topSquare
    exit
*
  rectangle
    specify corners
      .0 0. 1. .75
    lines
      9 7
    boundary conditions
      1 1 2 0
    share
      1 2 0 0
    mappingName
      bottomSquare
    exit
  exit
*
generate an overlapping grid
  bottomSquare
  topSquare
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
    * do not interpolate ghost
  exit
  pause
  compute overlap
exit
save an overlapping grid
tsq.hdf
tsq
exit
