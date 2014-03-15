*
* create a grid for testing the mismatch at corners
*
create mappings
  Box
    mappingName
      baseGrid
    lines
      11 11 11
    share
      1 0 2 0 3 0
   exit
*
  Box
    mappingName
      refinement0
  specify corners
    0. 0. 0. .5 .5 .5
    lines
      8 8 8
    boundary conditions
      1 0 1 0 1 0
    share
      1 0 2 0 3 0
   exit
* Now shift the refinement a bit so it doesn't match
  rotate/scale/shift
    mappingName
      refinement
    shift
       -.02 -.02 -.02
    exit
  exit
*
  generate an overlapping grid
    baseGrid
    refinement
    done
    * display intermediate
    compute overlap
  exit
*
save an overlapping grid
mismatchBox.hdf
mismatchBox
exit
