*
* create a grid for testing the mismatch at corners
*
create mappings
  rectangle
    mappingName
      baseGrid
    lines
      11 11
    share
      1 0 2 0
   exit
*
  rectangle
    mappingName
      refinement0
  specify corners
    0. 0. .5 .5 
    lines
      8 8   
    boundary conditions
      1 0 1 0
    share
      1 0 2 0
   exit
* Now shift the refinement a bit so it doesn't match
  rotate/scale/shift
    mappingName
      refinement
    shift
       -.02 -.02
    exit
  exit
*
  generate an overlapping grid
    baseGrid
    refinement
    done
    compute overlap
  exit
*
exit
